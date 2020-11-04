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



ScFace_PostScript::ScFace_PostScript(const QString& fam, const QString& sty, const QString& alt, const QString& scname, const QString& psname, const QString& path, int face, const QStringList& features) :
	FtFace(fam,sty,alt,scname,psname,path,face,features)
{
	isFixedPitch = false;
	typeCode = ScFace::TYPE1;
}

QStringList ScFace_PostScript::findFontMetrics(const QString& fontPath)  const
{
	QStringList metricsFiles;
	QFileInfo fi(fontPath);

	QString fontDir  = fi.absolutePath();
	QString fontFile = fi.completeBaseName();

	metricsFiles += findFontMetrics(fontDir, fontFile);

	//if no metrics found look in afm and pfm subdirs
	if ( metricsFiles.empty() )
	{
		QDir dir;
		if (dir.exists(fontDir + "/AFMs"))
			metricsFiles += findFontMetrics(fontDir + "/AFMs", fontFile);
		if (dir.exists(fontDir + "/afm") && metricsFiles.empty())
			metricsFiles += findFontMetrics(fontDir + "/afm", fontFile);
		if (dir.exists(fontDir + "/Pfm") && metricsFiles.empty())
			metricsFiles += findFontMetrics(fontDir + "/Pfm", fontFile);
		if (dir.exists(fontDir + "/pfm") && metricsFiles.empty())
			metricsFiles += findFontMetrics(fontDir + "/pfm", fontFile);
	}
	return metricsFiles;
}

QStringList ScFace_PostScript::findFontMetrics(const QString& baseDir, const QString& baseName) const
{
	QStringList metricsFiles;
	QString     afnm = baseDir + "/" + baseName + ".";
	// Look for afm files
	QString afmName(afnm + "afm");
	if (QFile::exists(afmName))
		metricsFiles.append(afmName);
	else
	{
		afmName = afnm + "Afm";
		if (QFile::exists(afmName))
			metricsFiles.append(afmName);
		else
		{
			afmName = afnm + "AFM";
			if (QFile::exists(afmName))
				metricsFiles.append(afmName);
		}
	}
	// Look for pfm files
	QString pfmName(afnm + "pfm");
	if (QFile::exists(pfmName))
		metricsFiles.append(pfmName);
	else
	{
		pfmName = afnm + "Pfm";
		if (QFile::exists(pfmName))
			metricsFiles.append(pfmName);
		else
		{
			pfmName = afnm + "PFM";
			if (QFile::exists(pfmName))
				metricsFiles.append(pfmName);
		}
	}
	return metricsFiles;
}

bool ScFace_PostScript::loadFontMetrics(FT_Face face, const QString& fontPath) const
{
	QStringList fontMetrics = findFontMetrics(fontPath);
	if (fontMetrics.empty())
	{
		qDebug() << QObject::tr("No metrics found for font %1, ignoring font").arg(fontPath);
		return false;
	}

	bool brokenMetric = false;
	bool metricsFound = false;
	QString metricsFile;
	for (int i = 0; i < fontMetrics.size(); ++i)
	{
		metricsFile = fontMetrics.at(i);
		if (FT_Attach_File(face, metricsFile.toLocal8Bit().constData()) == 0)
		{
			if (brokenMetric)
				qDebug() << QObject::tr("Valid metrics were found for font %1, using metrics in file %2").arg(fontFile, metricsFile);
			metricsFound = true;
			break;
		}
		
		qDebug() << QObject::tr("Font %1 has broken metrics in file %2, ignoring metrics").arg(fontPath, metricsFile);
		brokenMetric = true;
	}
		
	return metricsFound;
}

void ScFace_PostScript::load() const // routine by Franz Schmid - modified by Alastair M. Robinson
{
	FtFace::load();
	//			bool error;
	FT_Face face = ftFace();
	if (!face)
	{
		const_cast<ScFace_PostScript*>(this)->usable = false;
		qDebug("%s", QObject::tr("Font %1 is broken (no Face), discarding it").arg(fontFile).toLocal8Bit().constData());
		return;
	}
	if (loadFontMetrics(face, fontFile))
	{
		// re-initialize: ScFaceData::load() just clears caches,
		// FtFace::load() skips FT_New_Face if m_face is already defined.
		// don't mind checking glyphs again for now (PS files have only 255 glyphs max, anyway)
		FtFace::load();
	}
}

ScFace_PFB::ScFace_PFB(const QString& fam, const QString& sty, const QString& alt, const QString& scname, const QString& psname, const QString& path, int face, const QStringList& features) :
	ScFace_PostScript(fam, sty, alt, scname, psname, path, face, features)
{
	formatCode = ScFace::PFB;
}

bool ScFace_PFB::embedFont(QByteArray& str) const
{
	QByteArray bb;
	rawData(bb);
	if ((bb.size() > 2) &&  (bb[0] == '\x80') && (static_cast<int>(bb[1]) == 1))
	{
		int posi,cxxc=0;
		for (posi = 6; posi < bb.size(); ++posi)
		{
			if ((bb[posi] == '\x80') && (posi + 1 < bb.size()) && (static_cast<int>(bb[posi + 1]) == 2))
				break;
			str += bb[posi];
		}
		int ulen;
		if (posi + 6 < bb.size())
		{
			ulen = bb[posi + 2] & 0xff;
			ulen |= (bb[posi + 3] << 8) & 0xff00;
			ulen |= (bb[posi + 4] << 16) & 0xff0000;
			ulen |= (bb[posi + 5] << 24) & 0xff000000;
			posi += 6;
			if (posi + ulen > bb.size())
				ulen = bb.size() - posi - 1;
			char linebuf[80];
			cxxc = 0;
			for (int j = 0; j < ulen; ++j)
			{
				unsigned char u = bb[posi];
				linebuf[cxxc] = ((u >> 4) & 15) + '0';
				if (u>0x9f)
					linebuf[cxxc] += 'a'-':';
				++cxxc;
				u &= 15;
				linebuf[cxxc] = u + '0';
				if (u>0x9)
					linebuf[cxxc] += 'a'-':';
				++posi;
				++cxxc;
				if (cxxc > 72)
				{
					linebuf[cxxc++] = '\n';
					linebuf[cxxc++] = 0;
					str += linebuf;
					cxxc = 0;
				}
			}
			linebuf[cxxc] = 0;
			str += linebuf;
			str += "\n";
		}
		posi += 6;
		for (int j = posi; j < bb.size(); ++j)
		{
			if ((bb[j] == '\x80') && (j + 1 < bb.size()) && (static_cast<int>(bb[j + 1]) == 3))
				break;
			if (bb[j] == '\r')
				str += "\n";
			else
				str += bb[j];
		}
		str += "\n";
		return true;
	}
	qDebug("%s", QObject::tr("Font %1 cannot be read, no embedding").arg(fontFile).toLatin1().constData());
	return false;
}

ScFace_PFA::ScFace_PFA(const QString& fam, const QString& sty, const QString& alt, const QString& scname, const QString& psname, const QString& path, int face, const QStringList& features) :
	ScFace_PostScript(fam, sty, alt, scname, psname, path, face, features)
{
	formatCode = ScFace::PFA;
}

bool ScFace_PFA::embedFont(QByteArray& str) const
{
	QByteArray bb;
	rawData(bb);
	if (bb.size() > 2 && bb[0] == '%' && bb[1] == '!')
	{
		// this is ok since bb will not contain '\0'
		str.append(bb);
		return true;
	}
	qDebug("%s", QObject::tr("Font %1 cannot be read, no embedding").arg(fontFile).toLatin1().constData());
	return false;
}
