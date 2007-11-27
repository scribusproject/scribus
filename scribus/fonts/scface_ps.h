/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
#ifndef SCFACE_PS_H
#define SCFACE_PS_H

#include <QString>
#include <QStringList>
#include <QFont>
#include <QMap>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

#include "scribusapi.h"
#include "fpointarray.h"
#include "scconfig.h"


/*
	Class ScFace_postscript
	Subclass of ScFace, for PostScript fonts that could possibly have a .afm file
	associated with them for metrics information.
*/

class ScFace_postscript : public FtFace
{
	public:
		ScFace_postscript(QString fam, QString sty, QString alt, QString scname, QString psname, QString path, int face) :
		FtFace(fam,sty,alt,scname,psname,path,face)
		{
			isFixedPitch = false;
			typeCode = ScFace::TYPE1;
		}

		virtual void load()  const // routine by Franz Schmid - modified by Alastair M. Robinson
		{
			FtFace::load();
//			bool error;
			FT_Face face = ftFace();
			if (!face)
			{
				const_cast<ScFace_postscript*>(this)->usable = false;
				qDebug(QObject::tr("Font %1 is broken (no Face), discarding it").arg(fontFile).toLatin1().constData());
				return;
			}
			QString afnm = fontFile.left(fontFile.length()-3);
			QFile afm(afnm+"afm");
			if(!(afm.exists()))
			{
				afm.setFileName(afnm+"Afm");
			}
			if(!(afm.exists()))
			{
				afm.setFileName(afnm+"AFM");
			}
			if(!(afm.exists()))
			{
				afm.setFileName(afnm+"pfm");
			}
			if(!(afm.exists()))
			{
				afm.setFileName(afnm+"Pfm");
			}
			if(!(afm.exists())) {
				afm.setFileName(afnm+"PFM");
			}
			if (afm.exists())
			{
				if (FT_Attach_File(face, afm.fileName().toLocal8Bit().constData()))
					qDebug(QObject::tr("Font %1 has broken metrics in file %2, ignoring metrics").arg(fontFile).arg(afm.fileName()).toLatin1().constData());
				else
					// re-initialize: ScFaceData::load() just clears caches,
					// FtFace::load() skips FT_New_Face if m_face is already defined.
					// dont mind checking glyphs again for now (PS files have only 255 glyphs max, anyway)
					FtFace::load();
			}
//			Ascent = tmp.setNum(face->ascender);
//			Descender = tmp.setNum(face->descender);
//			CapHeight = Ascent;
//			ItalicAngle = "0";
//			StdVW = "1";
//			FontBBox = tmp.setNum(face->bbox.xMin)+" "+tmp2.setNum(face->bbox.yMin)+" "+tmp3.setNum(face->bbox.xMax)+" "+tmp4.setNum(face->bbox.yMax);
/*
 setBestEncoding(face);
			gindex = 0;
			charcode = FT_Get_First_Char( face, &gindex );
			int goodGlyph = 0;
			int invalidGlyph = 0;
			while ( gindex != 0 )
			{
				error = FT_Load_Glyph( face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
				if (error)
				{
					++invalidGlyph;
					sDebug(QObject::tr("Font %1 has broken glyph %2 (charcode %3)").arg(fontPath()).arg(gindex).arg(charcode));
					charcode = FT_Get_Next_Char( face, charcode, &gindex );
					continue;
				}
				++goodGlyph;
				double ww = face->glyph->metrics.horiAdvance / uniEM;
				if (face->glyph->format == FT_GLYPH_FORMAT_PLOTTER)
					isStroked = true;
				error = false;
				outlines = traceChar(face, charcode, 10, &x, &y, &error);
				if (!error)
				{
					CharWidth.insert(charcode, ww);
					GRec.Outlines = outlines.copy();
					GRec.x = x;
					GRec.y = y;
					GlyphArray.insert(charcode, GRec);
				}
				charcode = FT_Get_Next_Char( face, charcode, &gindex );
			}
 */
		}
};

/*
	Class ScFace_pfb
	Subclass of ScFace, specifically for Adobe type 1 .pfb fonts.
	Implements: RealName() and EmbedFont().
*/

class ScFace_pfb : public ScFace_postscript
{
	public:
		ScFace_pfb(QString fam, QString sty, QString alt, QString scname, QString psname, QString path, int face) :
		ScFace_postscript(fam,sty,alt,scname,psname,path,face)
		{
			formatCode = ScFace::PFB;
		}

		virtual bool EmbedFont(QString &str) const
		{
			QByteArray bb;
			RawData(bb);
			QString tmp2 = "";
			if ((bb.size() > 2) &&  (bb[0] == char(0x80)) && (static_cast<int>(bb[1]) == 1))
			{
				QString tmp3="";
				QString tmp4 = "";
				int posi,cxxc=0;
				for (posi = 6; posi < bb.size(); ++posi)
				{
					if ((bb[posi] == char(0x80)) && (posi+1 < bb.size()) && (static_cast<int>(bb[posi+1]) == 2))
						break;
					str += bb[posi];
				}
				int ulen;
				if (posi+6 < bb.size()) 
				{
					ulen = bb[posi+2] & 0xff;
					ulen |= (bb[posi+3] << 8) & 0xff00;
					ulen |= (bb[posi+4] << 16) & 0xff0000;
					ulen |= (bb[posi+5] << 24) & 0xff000000;
					posi += 6;
					if (posi + ulen > bb.size())
						ulen = bb.size() - posi - 1;
					char linebuf[80];
					cxxc=0;
					for (int j = 0; j < ulen; ++j)
					{
						unsigned char u=bb[posi];
						linebuf[cxxc]=((u >> 4) & 15) + '0';
						if(u>0x9f) linebuf[cxxc]+='a'-':';
						++cxxc;
						u&=15; linebuf[cxxc]=u + '0';
						if(u>0x9) linebuf[cxxc]+='a'-':';
						++posi;
						++cxxc;
						if (cxxc > 72)
						{
							linebuf[cxxc++]='\n';
							linebuf[cxxc++]=0;
							str += linebuf;
							cxxc = 0;
						}
					}
					linebuf[cxxc]=0;
					str += linebuf;
					str += "\n";
				}
				posi += 6;
				for (int j = posi; j < bb.size(); ++j)
				{
					if ((bb[j] == static_cast<char>(0x80)) && (j+1 < bb.size()) && (static_cast<int>(bb[j+1]) == 3))
						break;
					if(bb[j]=='\r')
						str+="\n";
					else
						str += bb[j];
				}
				str += "\n";
				cxxc = 0;
				return true;
			}
			else 
			{
				qDebug(QObject::tr("Font %1 cannot be read, no embedding").arg(fontFile).toLatin1().constData());
				return false;
			}
		}
};

/*
	Class ScFace_pfa
	Subclass of ScFace, specifically for Adobe type 1 and type 3 .pfa fonts.
	Implements: RealName() and EmbedFont().
*/

class ScFace_pfa : public ScFace_postscript
{
	public:
		ScFace_pfa(QString fam, QString sty, QString alt, QString scname, QString psname, QString path, int face) :
		ScFace_postscript(fam,sty,alt,scname,psname,path,face)
		{
			formatCode = ScFace::PFA;
		}
		virtual bool EmbedFont(QString &str) const
		{
			QByteArray bb;
			RawData(bb);
			if (bb.size() > 2 && bb[0] == '%' && bb[1] == '!') 
			{
				// this is ok since bb will not contain '\0'
				str.append(bb);
				return true; 
			}
			qDebug(QObject::tr("Font %1 cannot be read, no embedding").arg(fontFile).toLatin1().constData());
			return false;
		}
};


#endif
