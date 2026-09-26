/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "datamergesource.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QObject>
#include <QSet>

#include "dynamicvariable.h"
#include "documentchecker.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "undomanager.h"
#include "util.h"

namespace
{
constexpr qint64 MaximumSourceBytes = 32 * 1024 * 1024;

bool parseCsvRows(const QString& text, QVector<QStringList>& rows, QString& error)
{
	QStringList row;
	QString field;
	bool inQuotes = false;
	bool quoteClosed = false;
	bool rowStarted = false;
	for (qsizetype index = 0; index < text.size(); ++index)
	{
		const QChar character = text.at(index);
		if (quoteClosed && character != QLatin1Char(',') && character != QLatin1Char('\n') && character != QLatin1Char('\r'))
		{
			error = QObject::tr("A CSV quoted field must be followed by a comma or line ending.");
			return false;
		}
		if (character == QLatin1Char('"'))
		{
			if (inQuotes)
			{
				if (index + 1 < text.size() && text.at(index + 1) == QLatin1Char('"'))
				{
					field.append(QLatin1Char('"'));
					++index;
				}
				else
				{
					inQuotes = false;
					quoteClosed = true;
				}
			}
			else if (field.isEmpty())
			{
				inQuotes = true;
				rowStarted = true;
			}
			else
			{
				error = QObject::tr("A CSV quote must start a field or escape another quote.");
				return false;
			}
			continue;
		}
		if (character == QLatin1Char(',') && !inQuotes)
		{
			row.append(field);
			field.clear();
			quoteClosed = false;
			rowStarted = true;
			continue;
		}
		if ((character == QLatin1Char('\n') || character == QLatin1Char('\r')) && !inQuotes)
		{
			row.append(field);
			field.clear();
			quoteClosed = false;
			if (rowStarted || row.size() > 1)
				rows.append(row);
			row.clear();
			rowStarted = false;
			if (character == QLatin1Char('\r') && index + 1 < text.size() && text.at(index + 1) == QLatin1Char('\n'))
				++index;
			continue;
		}
		field.append(character);
		rowStarted = true;
	}
	if (inQuotes)
	{
		error = QObject::tr("The CSV data ends inside a quoted field.");
		return false;
	}
	if (rowStarted || !field.isEmpty() || !row.isEmpty())
	{
		row.append(field);
		rows.append(row);
	}
	return true;
}

bool jsonScalarToString(const QJsonValue& value, QString& result)
{
	if (value.isString())
		result = value.toString();
	else if (value.isDouble())
		result = QString::number(value.toDouble(), 'g', 17);
	else if (value.isBool())
		result = value.toBool() ? QStringLiteral("true") : QStringLiteral("false");
	else if (value.isNull())
		result.clear();
	else
		return false;
	return true;
}

QString safeFileComponent(const QString& value)
{
	QString component;
	for (const QChar character : value.simplified())
	{
		if (character.unicode() < 32 || QStringLiteral("<>:\"/\\|?*").contains(character))
			continue;
		component.append(character);
		if (component.size() >= 80)
			break;
	}
	while (!component.isEmpty() && (component.endsWith(QLatin1Char(' ')) || component.endsWith(QLatin1Char('.'))))
		component.chop(1);
	return component;
}

bool hasCriticalPreflightErrors(const ScribusDoc* doc)
{
	auto critical = [](PreflightError error) {
		return error == PreflightError::MissingImage || error == PreflightError::MissingGlyph
			|| error == PreflightError::TextOverflow || error == PreflightError::BrokenCrossReference;
	};
	for (const auto& errors : doc->docItemErrors)
	{
		for (auto it = errors.constBegin(); it != errors.constEnd(); ++it)
		{
			if (critical(it.key()))
				return true;
		}
	}
	for (const auto& errors : doc->masterItemErrors)
	{
		for (auto it = errors.constBegin(); it != errors.constEnd(); ++it)
		{
			if (critical(it.key()))
				return true;
		}
	}
	for (const auto& errors : doc->pageErrors)
	{
		for (auto it = errors.constBegin(); it != errors.constEnd(); ++it)
		{
			if (critical(it.key()))
				return true;
		}
	}
	return false;
}
}

bool DataMergeSource::load(const QString& path, const QString& requestedFormat, QString* error)
{
	m_fields.clear();
	m_rows.clear();
	auto fail = [this, error](const QString& message) {
		m_fields.clear();
		m_rows.clear();
		if (error)
			*error = message;
		return false;
	};
	if (error)
		error->clear();

	QString format = requestedFormat.trimmed().toLower();
	if (format.startsWith(QLatin1Char('.')))
		format.remove(0, 1);
	if (format.isEmpty())
		format = QFileInfo(path).suffix().toLower();
	if (format != QStringLiteral("csv") && format != QStringLiteral("json"))
		return fail(QObject::tr("Unsupported data source format '%1'; use CSV or JSON.").arg(format));

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return fail(QObject::tr("Could not read data source '%1'.").arg(path));
	if (file.size() > MaximumSourceBytes)
		return fail(QObject::tr("The data source exceeds the 32 MB safety limit."));
	QByteArray data = file.readAll();
	if (data.size() > MaximumSourceBytes)
		return fail(QObject::tr("The data source exceeds the 32 MB safety limit."));
	if (data.startsWith("\xEF\xBB\xBF"))
		data.remove(0, 3);

	if (format == QStringLiteral("csv"))
	{
		const QString text = QString::fromUtf8(data);
		if (text.toUtf8() != data)
			return fail(QObject::tr("The CSV data source is not valid UTF-8."));
		QVector<QStringList> rows;
		QString csvError;
		if (!parseCsvRows(text, rows, csvError))
			return fail(csvError);
		if (rows.isEmpty())
			return true;
		m_fields = rows.takeFirst();
		QSet<QString> headers;
		for (QString& field : m_fields)
		{
			field = field.trimmed();
			if (field.isEmpty())
				return fail(QObject::tr("CSV headers must not be empty."));
			if (headers.contains(field))
				return fail(QObject::tr("CSV header '%1' occurs more than once.").arg(field));
			headers.insert(field);
		}
		for (const QStringList& row : rows)
		{
			if (row.size() != m_fields.size())
				return fail(QObject::tr("A CSV record has %1 fields but the header has %2.").arg(row.size()).arg(m_fields.size()));
			m_rows.append(row);
		}
		return true;
	}

	QJsonParseError parseError;
	const QJsonDocument document = QJsonDocument::fromJson(data, &parseError);
	if (parseError.error != QJsonParseError::NoError || !document.isArray())
		return fail(QObject::tr("JSON data source must contain an array of objects: %1.").arg(parseError.errorString()));
	const QJsonArray array = document.array();
	QVector<QJsonObject> objects;
	QSet<QString> headers;
	for (const QJsonValue& value : array)
	{
		if (!value.isObject())
			return fail(QObject::tr("Each JSON data record must be an object."));
		const QJsonObject object = value.toObject();
		for (auto it = object.constBegin(); it != object.constEnd(); ++it)
		{
			if (it.key().isEmpty())
				return fail(QObject::tr("JSON field names must not be empty."));
			QString scalar;
			if (!jsonScalarToString(it.value(), scalar))
				return fail(QObject::tr("JSON field '%1' must be a scalar value.").arg(it.key()));
			if (!headers.contains(it.key()))
			{
				headers.insert(it.key());
				m_fields.append(it.key());
			}
		}
		objects.append(object);
	}
	for (const QJsonObject& object : objects)
	{
		QStringList row;
		for (const QString& field : m_fields)
		{
			QString value;
			if (object.contains(field) && !jsonScalarToString(object.value(field), value))
				return fail(QObject::tr("JSON field '%1' must be a scalar value.").arg(field));
			row.append(value);
		}
		m_rows.append(row);
	}
	return true;
}

QMap<QString, QString> DataMergeSource::record(int index) const
{
	QMap<QString, QString> result;
	const QStringList& values = m_rows.at(index);
	for (int column = 0; column < m_fields.size(); ++column)
		result.insert(m_fields.at(column), values.value(column));
	return result;
}

bool DataMergeBatchExporter::exportPdfs(ScribusMainWindow* mainWindow, const DataMergeSource& source,
	const QMap<QString, QString>& mapping, const QString& directory, const QString& requestedPrefix,
	DataMergeBatchResult& result, const std::function<bool(int, int)>& progress,
	const DataMergeBatchOptions& options)
{
	result = DataMergeBatchResult();
	auto fail = [&result](const QString& message) {
		result.error = message;
		return false;
	};
	if (!mainWindow || !mainWindow->doc || !mainWindow->view)
		return fail(QObject::tr("No document is available for data-merge export."));
	if (source.recordCount() == 0)
		return fail(QObject::tr("The data source has no records to export."));
	const int lastRecord = options.lastRecord < 0 ? source.recordCount() : options.lastRecord;
	if (options.firstRecord < 1 || lastRecord > source.recordCount() || lastRecord < options.firstRecord)
		return fail(QObject::tr("The requested record range is outside the data source."));
	const int fileNameColumn = options.fileNameField.isEmpty() ? -1 : source.fields().indexOf(options.fileNameField);
	if (!options.fileNameField.isEmpty() && fileNameColumn < 0)
		return fail(QObject::tr("The filename field '%1' does not exist in the data source.").arg(options.fileNameField));
	if (mainWindow->doc->DocPages.isEmpty())
		return fail(QObject::tr("The document has no pages to export."));
	ScribusDoc* doc = mainWindow->doc;
	const QDir outputDir(directory);
	if (!outputDir.exists())
		return fail(QObject::tr("The output folder does not exist: %1").arg(directory));

	QString prefix = requestedPrefix;
	if (prefix.isEmpty())
		prefix = QFileInfo(doc->documentFileName()).completeBaseName();
	if (prefix.isEmpty())
		prefix = QStringLiteral("merged");
	if (prefix == QStringLiteral(".") || prefix == QStringLiteral("..")
		|| prefix.endsWith(QLatin1Char(' ')) || prefix.endsWith(QLatin1Char('.')))
		return fail(QObject::tr("The PDF filename prefix is not valid."));
	for (const QChar character : prefix)
	{
		if (character.unicode() < 32 || QStringLiteral("<>:\"/\\|?*").contains(character))
			return fail(QObject::tr("The PDF filename prefix contains a character that is unsafe on Windows."));
	}

	struct Binding { QString id; QString name; int column; };
	QVector<Binding> bindings;
	QSet<QString> targets;
	for (auto it = mapping.constBegin(); it != mapping.constEnd(); ++it)
	{
		const int column = source.fields().indexOf(it.key());
		if (column < 0)
			return fail(QObject::tr("The mapped source field '%1' does not exist.").arg(it.key()));
		const DynamicVariable* variable = doc->dynamicVariable(it.value());
		if (!variable || variable->type != DynamicVariableResolver::UserDefined)
			return fail(QObject::tr("Field '%1' is not mapped to a writable user-defined variable.").arg(it.key()));
		if (targets.contains(variable->id))
			return fail(QObject::tr("More than one field is mapped to variable '%1'.").arg(variable->name));
		targets.insert(variable->id);
		bindings.append({variable->id, variable->name, column});
	}
	if (bindings.isEmpty())
		return fail(QObject::tr("Map at least one source field to a user-defined variable."));

	const int digitCount = qMax(4, QString::number(source.recordCount()).size());
	QStringList outputPaths;
	for (int index = options.firstRecord - 1; index < lastRecord; ++index)
	{
		QString name = QStringLiteral("%1-record-%2")
			.arg(prefix, QString::number(index + 1).rightJustified(digitCount, QLatin1Char('0')));
		if (fileNameColumn >= 0)
		{
			const QString component = safeFileComponent(source.row(index).value(fileNameColumn));
			if (!component.isEmpty())
				name += QLatin1Char('-') + component;
		}
		const QString fileName = name + QStringLiteral(".pdf");
		const QString path = outputDir.absoluteFilePath(fileName);
		const QFileInfo file(path);
		if (file.exists() || file.isSymLink())
			return fail(QObject::tr("The output file already exists; no PDFs were exported: %1").arg(path));
		outputPaths.append(path);
	}

	QMap<QString, QString> originalValues;
	for (const Binding& binding : bindings)
		originalValues.insert(binding.id, doc->dynamicVariable(binding.id)->value);
	const bool wasModified = doc->isModified();
	const PDFOptions originalPdfOptions = doc->pdfOptions();
	{
		UndoBlocker blockUndo;
		PDFOptions& pdfOptions = doc->pdfOptions();
		pdfOptions.doMultiFile = false;
		pdfOptions.openAfterExport = false;
		pdfOptions.firstUse = false;
		if (pdfOptions.useDocBleeds)
			pdfOptions.bleeds = doc->bleedsVal();
		doc->reorganiseFonts();
		if (pdfOptions.FontEmbedding == PDFOptions::EmbedFonts)
		{
			for (const QString& fontName : doc->UsedFonts.keys())
			{
				if (!pdfOptions.EmbedList.contains(fontName) && !pdfOptions.SubsetList.contains(fontName))
					pdfOptions.SubsetList.append(fontName);
			}
		}
		else if (pdfOptions.FontEmbedding == PDFOptions::OutlineFonts)
			pdfOptions.OutlineList = doc->UsedFonts.keys();

		std::vector<int> pageNumbers;
		for (int page = 1; page <= doc->DocPages.count(); ++page)
			pageNumbers.push_back(page);
		for (int index = 0; index < outputPaths.size(); ++index)
		{
			if (progress && !progress(index, outputPaths.size()))
			{
				result.cancelled = true;
				break;
			}
			const int sourceIndex = options.firstRecord - 1 + index;
			const QStringList& values = source.row(sourceIndex);
			for (const Binding& binding : bindings)
			{
				if (!doc->updateDynamicVariable(binding.id, binding.name, values.value(binding.column)))
				{
					result.error = QObject::tr("Could not apply data record %1 to the document.").arg(sourceIndex + 1);
					break;
				}
			}
			if (!result.error.isEmpty())
				break;
			doc->regionsChanged()->update(QRectF());
			ReOrderText(doc, mainWindow->view);
			if (options.failOnPreflight)
			{
				DocumentChecker::checkDocument(doc);
				if (hasCriticalPreflightErrors(doc))
				{
					result.error = QObject::tr("Critical preflight errors in data record %1; export stopped.").arg(sourceIndex + 1);
					break;
				}
			}
			QMap<int, QImage> thumbnails;
			for (int page : pageNumbers)
			{
				QImage thumbnail(10, 10, QImage::Format_ARGB32_Premultiplied);
				if (pdfOptions.Thumbnails)
					thumbnail = mainWindow->view->PageToPixmap(page - 1, 100, Pixmap_DontReloadImages | Pixmap_DrawWhiteBackground);
				thumbnails.insert(page, thumbnail);
			}
			const QString& path = outputPaths.at(index);
			pdfOptions.fileName = path;
			bool exportCancelled = false;
			if (!mainWindow->getPDFDriver(path, pageNumbers, thumbnails, result.error, &exportCancelled))
			{
				result.cancelled = exportCancelled;
				if (result.error.isEmpty() && !exportCancelled)
					result.error = QObject::tr("PDF export failed for record %1.").arg(sourceIndex + 1);
				break;
			}
			if (!QFileInfo(path).isFile() || QFileInfo(path).size() == 0)
			{
				result.error = QObject::tr("PDF export did not create a usable file for record %1.").arg(sourceIndex + 1);
				break;
			}
			result.files.append(path);
		}
		for (const Binding& binding : bindings)
			doc->updateDynamicVariable(binding.id, binding.name, originalValues.value(binding.id));
		doc->pdfOptions() = originalPdfOptions;
		ReOrderText(doc, mainWindow->view);
		doc->regionsChanged()->update(QRectF());
		doc->setModified(wasModified);
	}
	return !result.cancelled && result.error.isEmpty();
}
