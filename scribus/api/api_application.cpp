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
		bu += "C";
		bu += "-";
		bu += "-";
		bu += "T";
		bu += "-";
	#ifdef HAVE_FONTCONFIG
		bu += "F";
	#else
		bu += "*";
	#endif
		bu += "-";
		bu += "C";
		bu += cairo_version_string();

	// Some more information if we are not on a 32bit little endian Unix machine
	#if defined(Q_OS_WIN)
		bu += "-Windows";
	#elif defined(Q_OS_MACOS)
		bu += "-Mac";
	#elif defined(Q_OS_DARWIN)
		// dunno if anyone uses this...
		bu += "-Darwin";
	#endif
		if (QSysInfo::WordSize != 32)
			bu += QString("-%1bit").arg(QSysInfo::WordSize);
	#if Q_BYTE_ORDER == Q_BIG_ENDIAN
		if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
			bu += "-Big";
	#endif
		return bu;
	}

	QString getVersion()
	{
		return QString(VERSION);
	}

	int getVersionMajor()
	{
		QRegularExpression version_regex("(\\d+)\\.(\\d+)\\.(\\d+)(.*)");
		QRegularExpressionMatch match = version_regex.match(getVersion());
		if (!match.hasMatch())
			return -1;
		return match.captured(1).toInt();
	}

	int getVersionMinor()
	{
		QRegularExpression version_regex("(\\d+)\\.(\\d+)\\.(\\d+)(.*)");
		QRegularExpressionMatch match = version_regex.match(getVersion());
		if (!match.hasMatch())
			return -1;
		return match.captured(2).toInt();
	}

	int getVersionPatch()
	{
		QRegularExpression version_regex("(\\d+)\\.(\\d+)\\.(\\d+)(.*)");
		QRegularExpressionMatch match = version_regex.match(getVersion());
		if (!match.hasMatch())
			return -1;
		return match.captured(3).toInt();
	}

	QString getVersionSuffix()
	{
		QRegularExpression version_regex("(\\d+)\\.(\\d+)\\.(\\d+)(.*)");
		QRegularExpressionMatch match = version_regex.match(getVersion());
		if (!match.hasMatch())
			return QString();
		return match.captured(4);
	}

	QString getVersionScribus()
	{
		return QString("Scribus") + " " + getVersion();
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
		return getVersion().contains("svn", Qt::CaseInsensitive);
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
