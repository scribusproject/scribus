/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "embeddedimageextractor.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSaveFile>

namespace
{
QString pathKey(const QString& path)
{
	return QDir::cleanPath(QFileInfo(path).absoluteFilePath()).toCaseFolded();
}

bool pathIsTaken(const QString& path, const QSet<QString>* reservedPaths)
{
	return QFileInfo::exists(path) || (reservedPaths && reservedPaths->contains(pathKey(path)));
}

QString windowsSafeBaseName(QString name)
{
	name = name.trimmed();
	name.replace(QRegularExpression(QStringLiteral("[\\x00-\\x1f<>:\"/\\\\|?*]")), QStringLiteral("_"));
	name.replace(QRegularExpression(QStringLiteral("\\s+")), QStringLiteral(" "));
	name.replace(QRegularExpression(QStringLiteral("[ .]+$")), QString());
	if (name.size() > 120)
		name.truncate(120);

	static const QRegularExpression reservedName(
		QStringLiteral("^(CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(?:\\..*)?$"),
		QRegularExpression::CaseInsensitiveOption);
	if (reservedName.match(name).hasMatch())
		name.prepend(QLatin1Char('_'));
	return name;
}
}

QString sanitizedImageBaseName(const QString& name)
{
	return windowsSafeBaseName(name);
}

QString embeddedImageExtension(const QString& sourcePath)
{
	QString extension = QFileInfo(sourcePath).suffix();
	extension.remove(QRegularExpression(QStringLiteral("[^A-Za-z0-9]")));
	if (extension.isEmpty() || extension.size() > 12)
		return QStringLiteral("img");
	return extension;
}

QString suggestedEmbeddedImageFileName(const QString& frameName, const QString& sourcePath, int fallbackNumber)
{
	QString baseName = sanitizedImageBaseName(frameName);
	if (baseName.isEmpty())
		baseName = QStringLiteral("Embedded Image %1").arg(qMax(1, fallbackNumber));
	return QStringLiteral("%1.%2").arg(baseName, embeddedImageExtension(sourcePath));
}

ImageExtractionPath resolveImageExtractionPath(const QString& directory, const QString& fileName,
	ImageExtractionConflict conflict, QSet<QString>* reservedPaths)
{
	ImageExtractionPath result;
	const QFileInfo requestedInfo(fileName);
	QString baseName = sanitizedImageBaseName(requestedInfo.completeBaseName());
	if (baseName.isEmpty())
		baseName = QStringLiteral("Embedded Image");
	QString extension = requestedInfo.suffix();
	extension.remove(QRegularExpression(QStringLiteral("[^A-Za-z0-9]")));
	if (extension.isEmpty())
		extension = QStringLiteral("img");

	const QDir outputDirectory(directory);
	result.path = outputDirectory.filePath(QStringLiteral("%1.%2").arg(baseName, extension));
	const bool reservedConflict = reservedPaths && reservedPaths->contains(pathKey(result.path));
	if (reservedConflict)
	{
		int copyNumber = 2;
		do
		{
			result.path = outputDirectory.filePath(
				QStringLiteral("%1-%2.%3").arg(baseName).arg(copyNumber).arg(extension));
			++copyNumber;
		}
		while (pathIsTaken(result.path, reservedPaths));
		result.renamed = true;
	}
	else if (QFileInfo::exists(result.path))
	{
		if (conflict == ImageExtractionConflict::Skip)
		{
			result.path.clear();
			result.skipped = true;
			return result;
		}
		if (conflict == ImageExtractionConflict::KeepBoth)
		{
			int copyNumber = 2;
			do
			{
				result.path = outputDirectory.filePath(
					QStringLiteral("%1-%2.%3").arg(baseName).arg(copyNumber).arg(extension));
				++copyNumber;
			}
			while (pathIsTaken(result.path, reservedPaths));
			result.renamed = true;
		}
	}

	result.path = QDir::cleanPath(QFileInfo(result.path).absoluteFilePath());
	if (reservedPaths)
		reservedPaths->insert(pathKey(result.path));
	return result;
}

bool copyEmbeddedImageBytes(const QString& sourcePath, const QString& destinationPath,
	bool replaceExisting, QString* errorMessage)
{
	auto fail = [errorMessage](const QString& message)
	{
		if (errorMessage)
			*errorMessage = message;
		return false;
	};

	if (sourcePath.isEmpty() || destinationPath.isEmpty())
		return fail(QObject::tr("The source or destination path is empty."));
	if (pathKey(sourcePath) == pathKey(destinationPath))
		return fail(QObject::tr("The source and destination refer to the same file."));
	if (!replaceExisting && QFileInfo::exists(destinationPath))
		return fail(QObject::tr("The destination file already exists."));

	QFile source(sourcePath);
	if (!source.open(QIODevice::ReadOnly))
		return fail(QObject::tr("Could not read the embedded image: %1").arg(source.errorString()));

	QSaveFile destination(destinationPath);
	if (!destination.open(QIODevice::WriteOnly))
		return fail(QObject::tr("Could not create the extracted image: %1").arg(destination.errorString()));

	QByteArray buffer(1024 * 1024, Qt::Uninitialized);
	while (!source.atEnd())
	{
		const qint64 bytesRead = source.read(buffer.data(), buffer.size());
		if (bytesRead < 0)
		{
			destination.cancelWriting();
			return fail(QObject::tr("Could not read the complete embedded image: %1").arg(source.errorString()));
		}
		if (bytesRead > 0 && destination.write(buffer.constData(), bytesRead) != bytesRead)
		{
			destination.cancelWriting();
			return fail(QObject::tr("Could not write the complete extracted image: %1").arg(destination.errorString()));
		}
	}
	if (!destination.commit())
		return fail(QObject::tr("Could not finish the extracted image: %1").arg(destination.errorString()));
	return true;
}
