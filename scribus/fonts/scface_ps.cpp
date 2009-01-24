/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include "ftface.h"
#include "scface_ps.h"



QStringList ScFace_postscript::findFontMetrics(const QString& fontPath)  const
{
	QStringList metricsFiles;
	QFileInfo fi(fontPath);

	QString fontDir  = fi.absolutePath();
	QString fontFile = fi.fileName();

	metricsFiles += findFontMetrics(fontDir, fontFile);

	//if no metrics found look in afm and pfm subdirs
	if ( metricsFiles.size() <= 0 )
	{
		QDir dir;
		if (dir.exists(fontDir + "/AFMs"))
			metricsFiles += findFontMetrics(fontDir + "/AFMs", fontFile);
		if (dir.exists(fontDir + "/afm") && metricsFiles.size() <= 0)
			metricsFiles += findFontMetrics(fontDir + "/afm", fontFile);
		if (dir.exists(fontDir + "/Pfm") && metricsFiles.size() <= 0)
			metricsFiles += findFontMetrics(fontDir + "/Pfm", fontFile);
		if (dir.exists(fontDir + "/pfm") && metricsFiles.size() <= 0)
			metricsFiles += findFontMetrics(fontDir + "/pfm", fontFile);
	}

	return metricsFiles;
}

QStringList ScFace_postscript::findFontMetrics(const QString& baseDir, const QString& baseName) const
{
	QStringList metricsFiles;
	QString     basePath = baseDir + "/" + baseName;
	QString     afnm = basePath.left(basePath.length()-3);

	// Look for afm files
	QString afmName(afnm+"afm");
	if(QFile::exists(afmName))
		metricsFiles.append(afmName);
	else
	{
		afmName = afnm+"Afm";
		if(QFile::exists(afmName))
			metricsFiles.append(afmName);
		else
		{
			afmName = afnm+"AFM";
			if(QFile::exists(afmName))
				metricsFiles.append(afmName);
		}
	}

	// Look for pfm files
	QString pfmName(afnm+"pfm");
	if(QFile::exists(pfmName))
		metricsFiles.append(pfmName);
	else
	{
		pfmName = afnm+"Pfm";
		if(QFile::exists(pfmName))
			metricsFiles.append(pfmName);
		else
		{
			afmName = afnm+"PFM";
			if(QFile::exists(pfmName))
				metricsFiles.append(pfmName);
		}
	}

	return metricsFiles;
}

bool ScFace_postscript::loadFontMetrics(FT_Face face, const QString& fontPath) const
{
	bool metricsFound = false;
	QStringList fontMetrics = findFontMetrics(fontPath);
	if (fontMetrics.size() > 0)
	{
		bool brokenMetric = false;
		QString metricsFile;
		for (int i = 0; i < fontMetrics.size(); ++i)
		{
			metricsFile = fontMetrics.at(i);
			if (FT_Attach_File(face, metricsFile.toLocal8Bit().constData()))
			{
				qDebug() << QObject::tr("Font %1 has broken metrics in file %2, ignoring metrics").arg(fontPath).arg(metricsFile);
				brokenMetric = true;
			}
			else
			{
				if (brokenMetric)
					qDebug() << QObject::tr("Valid metrics were found for font %1, using metrics in file %2").arg(fontFile).arg(metricsFile);
				metricsFound = true;
				break;
			}
		}
	}
	else
		qDebug() << QObject::tr("No metrics found for font %1, ignoring font").arg(fontPath);
	return metricsFound;
}
