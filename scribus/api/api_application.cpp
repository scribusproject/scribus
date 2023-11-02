/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.

Scribus is copyright The Scribus Team per the COPYING file.
*/

#include <QRegularExpression>
#include <QSysInfo>

#include "api_application.h"

#include "scconfig.h"
#ifdef HAVE_SVNVERSION
	#include "svnversion.h"
#endif

#include <cairo.h>

namespace ScribusAPI {

	QString getBuildInformation()
	{
		QString bu;
		// Some more information if we are not on a 32bit little endian Unix machine
#if defined(Q_OS_WIN)
		bu += QLatin1String("Windows");
#elif defined(Q_OS_MACOS)
		bu += QLatin1String("Mac");
#elif defined(Q_OS_DARWIN)
		// dunno if anyone uses this...
		bu += "Darwin";
#endif
		if (QSysInfo::WordSize != 32)
			bu += QString("-%1bit").arg(QSysInfo::WordSize);
		bu += QLatin1String("-");
		bu += QSysInfo::buildCpuArchitecture();
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
		if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
			bu += "-Big";
#endif
		return bu;
	}

	QString getVersion()
	{
		return QStringLiteral(VERSION);
	}

	int getVersionMajor()
	{
		QRegularExpression version_regex(QStringLiteral("(\\d+)\\.(\\d+)\\.(\\d+)(.*)"));
		QRegularExpressionMatch match = version_regex.match(getVersion());
		if (!match.hasMatch())
			return -1;
		return match.captured(1).toInt();
	}

	int getVersionMinor()
	{
		QRegularExpression version_regex(QStringLiteral("(\\d+)\\.(\\d+)\\.(\\d+)(.*)"));
		QRegularExpressionMatch match = version_regex.match(getVersion());
		if (!match.hasMatch())
			return -1;
		return match.captured(2).toInt();
	}

	int getVersionPatch()
	{
		QRegularExpression version_regex(QStringLiteral("(\\d+)\\.(\\d+)\\.(\\d+)(.*)"));
		QRegularExpressionMatch match = version_regex.match(getVersion());
		if (!match.hasMatch())
			return -1;
		return match.captured(3).toInt();
	}

	QString getVersionSuffix()
	{
		QRegularExpression version_regex(QStringLiteral("(\\d+)\\.(\\d+)\\.(\\d+)(.*)"));
		QRegularExpressionMatch match = version_regex.match(getVersion());
		if (!match.hasMatch())
			return QString();
		return match.captured(4);
	}

	QString getVersionScribus()
	{
		return QStringLiteral("Scribus") + " " + getVersion();
	}

	QByteArray getVersionScribusAsByteArray()
	{
		return QByteArray("Scribus") + " " + QByteArray(VERSION);
	}

	QString getVersionScribusTranslated()
	{
		return QObject::tr("Scribus") + " " + getVersion();
	}

	QByteArray getVersionScribusTranslatedAsByteArray()
	{
		return QObject::tr("Scribus").toUtf8() + " " + QByteArray(VERSION);
	}

	bool isSVN()
	{
		return getVersion().contains(QLatin1String("svn"), Qt::CaseInsensitive);
	}

	bool haveSVNRevision()
	{
#if defined(HAVE_SVNVERSION) && defined(SVNVERSION)
		return true;
#endif
		return false;
	}

	QString getSVNRevision()
	{
#if defined(HAVE_SVNVERSION) && defined(SVNVERSION)
		if (isSVN())
			return QString("%1").arg(SVNVERSION);
#endif
		return QString();
	}

}
