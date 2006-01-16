/*
	Scribus font handling code - copyright (C) 2001 by
	Alastair M. Robinson

	Based on code by Christian Tpp and Franz Schmid.

	Contributed to the Scribus project under the terms of
	the GNU General Public License version 2, or any later
	version, at your discretion.
*/

#include <qglobal.h>
#include <qapplication.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qstrlist.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qfont.h>
#include <qdict.h>
#include <qmap.h>
#include <qdir.h>
#include <qregexp.h>

#include <cstdlib>
#include "scfonts.h"
#include "scfonts_ttf.h"
#include "scfontmetrics.h"

#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "scribus.h"
#ifdef Q_WS_X11
#include <X11/X.h>
#include <X11/Xlib.h>
#endif

#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

#include "scpaths.h"
#include "util.h"

#include FT_INTERNAL_STREAM_H
#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_TABLES_H

Foi::Foi(QString fam, QString sty, QString alt, QString psname, QString path, int face, bool embedps)
{
	isOTF = false;
	Subset = false;
	typeCode = Foi::UNKNOWN_TYPE;
	formatCode = Foi::UNKNOWN_FORMAT;
	SCName = alt;
	Family = fam;
	Effect = sty;
	cached_RealName = psname;
	fontFile = path;
	faceIndex_ = face;
	EmbedPS = embedps;
	Alternative = "";
}


FT_Face * ftFace() {
	// dummy for now
	return NULL;
}

QString Foi::RealName()
{
	return cached_RealName;
}

bool Foi::EmbedFont(QString &str)
{
	str+="Base";
	return(false);
}

bool Foi::ReadMetrics()
{
	HasMetrics=false;
	for(int i=0;i<256;++i)
		CharWidth[i]=0;
	return(false);
}

bool Foi::GlNames(QMap<uint, QString> *GList)
{
	return GlyNames(this, GList);
}

void Foi::RawData(QByteArray & bb)
{
	FT_Library library;
	FT_Face    face;
	bool       error;
	
	error = FT_Init_FreeType( &library );
	if (error)
		sDebug(QObject::tr("Freetype2 library not available"));
	else
		error = FT_New_Face( library, fontFile, faceIndex_, &face );
	if (error)
		sDebug(QObject::tr("Font %1 is broken, no embedding").arg(fontFile));
	else 
	{
		FT_Stream fts = face->stream;
		bb.resize(fts->size);
		error = FT_Stream_Seek(fts, 0L);
		if (!error)
			error = FT_Stream_Read(fts, reinterpret_cast<FT_Byte *>(bb.data()), fts->size);
		if (error) 
		{
			sDebug(QObject::tr("Font %1 is broken (read stream), no embedding").arg(fontFile));
			bb.resize(0);
		}
		/*
		if (showFontInformation)
		{
			QFile f(fontFile);
			qDebug(QObject::tr("RawData for Font %1(%2): size=%3 filesize=%4").arg(fontFile).arg(faceIndex_).arg(bb.size()).arg(f.size()));
		}
		*/
	}
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}


/*
void Foi::FontBez()
{
	FT_Face face;
	FT_Library library;
	QString ts;
	QPixmap pm(200, 100);
	bool error;
	int  pen_x;
	int YPos = qApp->font().pointSize();
	uint n;
	error = FT_Init_FreeType( &library );
	error = FT_New_Face( library, fontFile, 0, &face );
	ts = QString(face->family_name) + " " + QString(face->style_name);
	pm.fill();
	pen_x = 0;
	for (n = 0; n < ts.length(); n++)
		{
		uint dv = ts[n].unicode();
		error = FT_Set_Char_Size(	face, 0, qApp->font().pointSize()*64, 72, 72 );
		FT_Load_Char(face, dv, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_MONOCHROME);
		QByteArray bd(face->glyph->bitmap.rows * face->glyph->bitmap.pitch);
		uint yy = 0;
		uint adv;
		if ((face->glyph->bitmap.width % 8) == 0)
			adv = face->glyph->bitmap.width / 8;
		else
			adv = face->glyph->bitmap.width / 8 + 1;
		for (int y = 0; y < face->glyph->bitmap.rows; ++y)
			{
			memcpy(bd.data()+yy, face->glyph->bitmap.buffer+(y * face->glyph->bitmap.pitch), adv);
			yy += adv;
			}
		QBitmap bb(face->glyph->bitmap.width, face->glyph->bitmap.rows, (uchar*)bd.data(), false);
		QPixmap pixm(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		if (!pixm.isNull())
			{
			pixm.fill(Qt::black);
			pixm.setMask(bb);
			QPainter p;
			p.begin(&pm);
			p.drawPixmap(pen_x+face->glyph->bitmap_left, YPos-face->glyph->bitmap_top, pixm);
			p.end();
			}
		pen_x += face->glyph->advance.x >> 6;
		}
	int high = qApp->fontMetrics().height();
	QPixmap pm2(pen_x, high);
	bitBlt(&pm2, 0, 0, &pm, 0, 0, pen_x, high);
	Appearance = pm2;
	FT_Done_FreeType( library );
	return;
}    */

/*
	Class Foi_postscript
	Subclass of Foi, for PostScript fonts that could possibly have a .afm file
	associated with them for metrics information.
*/

class Foi_postscript : public Foi
{
	public:
		Foi_postscript(QString fam, QString sty, QString alt, QString psname, QString path, int face, bool embedps) :
		Foi(fam,sty,alt,psname,path,face,embedps), metricsread(false)
		{
			HasMetrics=false;
			HasKern = false;
			Ascent = "0";
			CapHeight = "0";
			Descender = "0";
			ItalicAngle = "0";
			StdVW = "1";
			FontBBox = "0 0 0 0";
			IsFixedPitch = false;
			typeCode = TYPE1;
			HasMetrics=true;
		}

		virtual QString RealName()
		{
			return(Foi::RealName());
		}

		virtual bool ReadMetrics()  // routine by Franz Schmid - modified by Alastair M. Robinson
		{
			if(metricsread)
				return(HasMetrics);
			CharWidth.clear();
			GlyphArray.clear();
			QString tmp, tmp2, tmp3, tmp4;
			double x, y;
			FPointArray outlines;
			struct GlyphR GRec;
			bool			error;
			FT_Library library;
			FT_ULong  charcode;
			FT_UInt   gindex;
			FT_Face   face;
			isStroked = false;
			error = FT_Init_FreeType( &library );
			if (error)
			{
				UseFont = false;
				sDebug(QObject::tr("Font %1 is broken (FreeType2), discarding it").arg(fontPath()));
				return false;
			}
			error = FT_New_Face( library, fontFilePath(), faceIndex(), &face );
			if (error)
			{
				UseFont = false;
				sDebug(QObject::tr("Font %1 is broken (no Face), discarding it").arg(fontPath()));
				FT_Done_FreeType( library );
				return false;
			}
			uniEM = static_cast<double>(face->units_per_EM);
			QString afnm = fontFilePath().left(fontFilePath().length()-3);
			QFile afm(afnm+"afm");
			if(!(afm.exists()))
			{
				afm.setName(afnm+"Afm");
				if(!(afm.exists()))
					afm.setName(afnm+"AFM");
			}
			if (afm.exists())
				error = FT_Attach_File(face, afm.name());
			HasKern = FT_HAS_KERNING(face);
			Ascent = tmp.setNum(face->ascender);
			Descender = tmp.setNum(face->descender);
			numDescender = face->descender / uniEM;
			numAscent = face->ascender / uniEM;
			underline_pos = face->underline_position / uniEM;
			strikeout_pos = numAscent / 3;
			strokeWidth = face->underline_thickness / uniEM;
			CapHeight = Ascent;
			ItalicAngle = "0";
			StdVW = "1";
			FontBBox = tmp.setNum(face->bbox.xMin)+" "+tmp2.setNum(face->bbox.yMin)+" "+tmp3.setNum(face->bbox.xMax)+" "+tmp4.setNum(face->bbox.yMax);
			IsFixedPitch = face->face_flags & 4;
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
			FT_Done_Face( face );
			FT_Done_FreeType( library );
			if (invalidGlyph > 0) {
				UseFont = false;
				sDebug(QObject::tr("Font %1 is broken and will be discarded").arg(fontPath()));
			}
			HasMetrics=UseFont;
			metricsread=UseFont;
			return(HasMetrics);
		}
	private:
		bool metricsread;
};

/*
	Class Foi_pfb
	Subclass of Foi, specifically for Adobe type 1 .pfb fonts.
	Implements: RealName() and EmbedFont().
*/

class Foi_pfb : public Foi_postscript
{
	public:
		Foi_pfb(QString fam, QString sty, QString alt, QString psname, QString path, int face, bool embedps) :
		Foi_postscript(fam,sty,alt,psname,path,face,embedps)
		{
			formatCode = PFB;
		}

		virtual bool EmbedFont(QString &str)
		{
			QByteArray bb;
			RawData(bb);
			QString tmp2 = "";
			if ((bb.size() > 2) &&  (bb[0] == char(0x80)) && (static_cast<int>(bb[1]) == 1))
			{
				QString tmp3="";
				QString tmp4 = "";
				uint posi,cxxc=0;
				for (posi = 6; posi < bb.size(); ++posi)
				{
					if ((bb[posi] == char(0x80)) && (posi+1 < bb.size()) && (static_cast<int>(bb[posi+1]) == 2))
						break;
					str += bb[posi];
				}
				uint ulen;
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
					for (uint j = 0; j < ulen; ++j)
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
				for (uint j = posi; j < bb.size(); ++j)
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
				sDebug(QObject::tr("Font %1 cannot be read, no embedding").arg(fontPath()));
				return false;
			}
		}
};

/*
	Class Foi_pfa
	Subclass of Foi, specifically for Adobe type 1 and type 3 .pfa fonts.
	Implements: RealName() and EmbedFont().
*/

class Foi_pfa : public Foi_postscript
{
	public:
		Foi_pfa(QString fam, QString sty, QString alt, QString psname, QString path, int face, bool embedps) :
		Foi_postscript(fam,sty,alt,psname,path,face,embedps)
		{
			formatCode = PFA;
		}
		virtual bool EmbedFont(QString &str)
		{
			QByteArray bb;
			RawData(bb);
			if (bb.size() > 2 && bb[0] == '%' && bb[1] == '!') 
			{
				// this is ok since bb will not contain '\0'
				str.append(bb);
				return true; 
			}
			return false;
		}
};



/***************************************************************************/

SCFonts::SCFonts() : QDict<Foi>(), FontPath(true)
{
	setAutoDelete(true);
	showFontInformation=false;
	checkedFonts.clear();
}

SCFonts::~SCFonts()
{
}

void SCFonts::updateFontMap()
{
	fontMap.clear();
	SCFontsIterator it( *this );
	for ( ; it.current(); ++it)
	{
		if (it.current()->UseFont)
		{
			if (fontMap.contains(it.current()->family()))
			{
				QStringList effect;
				if (!fontMap[it.current()->family()].contains(it.current()->style()))
				{
					fontMap[it.current()->family()].append(it.current()->style());
				}
			}
			else
			{
				QStringList effect;
				effect.append(it.current()->style());
				fontMap.insert(it.current()->family(), effect);
			}
		}
	}
}

/* Add a path to the list of fontpaths, ensuring that
   a trailing slash is present.
   Checks to make sure this path is not already present
   before adding.
*/
void SCFonts::AddPath(QString p)
{
	if(p.right(1) != "/")
	  p += "/";
	if(FontPath.find(p)==-1)
		FontPath.insert(FontPath.count(),p);
}

void SCFonts::AddScalableFonts(const QString &path, QString DocName)
{
	//Make sure this is not empty or we will scan the whole drive on *nix
	//QString::null+/ is / of course.
	if (path.isEmpty())
		return;
	FT_Library library = NULL;
	QString pathfile;
	bool error;
	error = FT_Init_FreeType( &library );
	QString pathname(path);
	if ( !pathname.endsWith("/") )
		pathname += "/";
	pathname=QDir::convertSeparators(pathname);
	QDir d(pathname, "*", QDir::Name, QDir::Dirs | QDir::Files | QDir::Readable);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			// readdir may return . or .., which we don't want to recurse
			// over. Skip 'em.
			if (d[dc] == "." || d[dc] == "..")
				continue;
			QFileInfo fi(pathname+d[dc]);
			if (!fi.exists())      // Sanity check for broken Symlinks
				continue;
			
			qApp->processEvents();
			
			if (fi.isSymLink())
			{
				QFileInfo fi3(fi.readLink());
				if (fi3.isRelative())
					pathfile = pathname+fi.readLink();
				else
					pathfile = fi3.absFilePath();
			}
			else
				pathfile = pathname+d[dc];
			QFileInfo fi2(pathfile);
			if (fi2.isDir()) 
			{
				if (DocName.isEmpty())
					AddScalableFonts(pathfile);
				continue;
			}
			QString ext = fi.extension(false).lower();
			QString ext2 = fi2.extension(false).lower();
			if ((ext != ext2) && (ext.isEmpty())) 
				ext = ext2;
			if ((ext == "ttc") || (ext == "dfont") || (ext == "pfa") || (ext == "pfb") || (ext == "ttf") || (ext == "otf"))
			{
				error = AddScalableFont(pathfile, library, DocName);
				if (error)
					sDebug(QObject::tr("Font %1 is broken, discarding it").arg(pathfile));
			}
#ifdef FT_MACINTOSH
			else if (ext.isEmpty() && DocName.isEmpty())
			{
				error = AddScalableFont(pathfile, library, DocName);
				if (error)
					error = AddScalableFont(pathfile + "/rsrc",library, DocName);
				if (error)
					sDebug(QObject::tr("Font %1 is broken, discarding it").arg(pathfile));
			}
#endif				
		}
	}
	FT_Done_FreeType( library );
}

/**
 * tests magic words to determine the fontformat and preliminary fonttype
 */
void getFontFormat(FT_Face face, Foi::FontFormat & fmt, Foi::FontType & type)
{
	static const char* T42_HEAD      = "%!PS-TrueTypeFont";
	static const char* T1_HEAD      = "%!FontType1";
	static const char* T1_ADOBE_HEAD = "%!PS-AdobeFont-1";
	static const char* PSFONT_ADOBE2_HEAD  = "%!PS-Adobe-2.0 Font";
	static const char* PSFONT_ADOBE21_HEAD  = "%!PS-Adobe-2.1 Font";
	static const char* PSFONT_ADOBE3_HEAD  = "%!PS-Adobe-3.0 Resource-Font";
	static const int   FONT_NO_ERROR = 0;
	
	const FT_Stream fts = face->stream;
	char buf[128];
	
	fmt = Foi::UNKNOWN_FORMAT;
	type = Foi::UNKNOWN_TYPE;
	if (FT_Stream_Seek(fts, 0L) == FONT_NO_ERROR && FT_Stream_Read(fts, reinterpret_cast<FT_Byte *>(buf), 128) == FONT_NO_ERROR) 
	{
		if(strncmp(buf,T42_HEAD,strlen(T42_HEAD)) == 0) 
		{
			fmt = Foi::TYPE42;
			type = Foi::TTF;
		}
		else if(strncmp(buf,T1_HEAD,strlen(T1_HEAD)) == 0 ||
			    strncmp(buf,T1_ADOBE_HEAD,strlen(T1_ADOBE_HEAD)) == 0) 
		{
			fmt = Foi::PFA;
			type = Foi::TYPE1;
		}
		else if(strncmp(buf,PSFONT_ADOBE2_HEAD,strlen(PSFONT_ADOBE2_HEAD)) == 0 ||
			    strncmp(buf,PSFONT_ADOBE21_HEAD,strlen(PSFONT_ADOBE21_HEAD)) == 0 ||
			    strncmp(buf,PSFONT_ADOBE3_HEAD,strlen(PSFONT_ADOBE3_HEAD)) ==0) 
		{
			// Type2(CFF), Type0(Composite/CID), Type 3, Type 14 etc would end here
			fmt = Foi::PFA;
			type = Foi::UNKNOWN_TYPE;
		}
		else if(buf[0] == '\200' && buf[1] == '\1')
		{
			fmt = Foi::PFB;
			type = Foi::TYPE1;
		}
		else if(buf[0] == '\0' && buf[1] == '\1' 
				&& buf[2] == '\0' && buf[3] == '\0')
		{
			fmt = Foi::SFNT;
			type = Foi::TTF;
		}
		else if(strncmp(buf,"true",4) == 0)
		{
			fmt = Foi::SFNT;
			type = Foi::TTF;
		}
		else if(strncmp(buf,"ttcf",4) == 0)
		{
			fmt = Foi::TTCF;
			type = Foi::OTF;
		}
		else if(strncmp(buf,"OTTO",4) == 0)
		{
			fmt = Foi::SFNT;
			type = Foi::OTF;
		}
		// missing: raw CFF
	}
}

/**
 * Checks face, which must be sfnt based, for the subtype.
 * Possible values: TTF, CFF, OTF
 */
void getSFontType(FT_Face face, Foi::FontType & type) 
{
	if (FT_IS_SFNT(face)) 
	{
		FT_ULong ret = 0;
		bool hasGlyph = ! FT_Load_Sfnt_Table(face, TTAG_glyf, 0, NULL,  &ret);
		hasGlyph &= ret > 0;
		bool hasCFF = ! FT_Load_Sfnt_Table(face, TTAG_CFF, 0, NULL,  &ret);
		hasCFF &= ret > 0;
		if (hasGlyph)
			type = Foi::TTF;
		else if (hasCFF)
			type = Foi::OTF;
		// TODO: CID or no
	}
} 

// Load a single font into the library from the passed filename. Returns true on error.
bool SCFonts::AddScalableFont(QString filename, FT_Library &library, QString DocName)
{
	static bool firstRun;
	bool Subset = false;
	char *buf[50];
	QString glyName = "";
	Foi::FontFormat format;
	Foi::FontType   type;
	FT_Face         face = NULL;
	struct testCache foCache;
	QFileInfo fic(filename);
	foCache.isOK = false;
	foCache.isChecked = true;
	foCache.lastMod = fic.lastModified();
	if (checkedFonts.count() == 0)
	{
		firstRun = true;
		ScMW->setSplashStatus( QObject::tr("Creating Font Cache") );
	}
	bool error = FT_New_Face( library, filename, 0, &face );
	if (error) 
	{
		if (face != NULL)
			FT_Done_Face( face );
		checkedFonts.insert(filename, foCache);
		return true;
	}
	getFontFormat(face, format, type);
	if (format == Foi::UNKNOWN_FORMAT) 
	{
		if (showFontInformation)
			sDebug(QObject::tr("Failed to load font %1 - font type unknown").arg(filename));
		checkedFonts.insert(filename, foCache);
		error = true;
	}
	bool HasNames = FT_HAS_GLYPH_NAMES(face);
	if (!error)
	{
		if (!checkedFonts.contains(filename))
		{
			if (!firstRun)
				ScMW->setSplashStatus( QObject::tr("New Font found, checking...") );
			FT_UInt gindex = 0;
			FT_ULong charcode = FT_Get_First_Char( face, &gindex );
			while ( gindex != 0 )
			{
				error = FT_Load_Glyph( face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
				if (error)
				{
					if (showFontInformation)
						sDebug(QObject::tr("Font %1 has broken glyph %2 (charcode %3)").arg(filename).arg(gindex).arg(charcode));
					if (face != NULL)
						FT_Done_Face( face );
					checkedFonts.insert(filename, foCache);
					return true;
				}
				FT_Get_Glyph_Name(face, gindex, buf, 50);
				QString newName = QString(reinterpret_cast<char*>(buf));
				if (newName == glyName)
				{
					HasNames = false;
					Subset = true;
				}
				glyName = newName;
				charcode = FT_Get_Next_Char( face, charcode, &gindex );
			}
			foCache.isOK = true;
			checkedFonts.insert(filename, foCache);
		}
		else
		{
			if (!checkedFonts[filename].isOK)
			{
				checkedFonts[filename].isChecked = true;
				return true;
			}
			if (checkedFonts[filename].lastMod != fic.lastModified())
			{
				ScMW->setSplashStatus( QObject::tr("Modified Font found, checking...") );
				FT_UInt gindex = 0;
				FT_ULong charcode = FT_Get_First_Char( face, &gindex );
				while ( gindex != 0 )
				{
					error = FT_Load_Glyph( face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
					if (error)
					{
						if (showFontInformation)
							sDebug(QObject::tr("Font %1 has broken glyph %2 (charcode %3)").arg(filename).arg(gindex).arg(charcode));
						if (face != NULL)
							FT_Done_Face( face );
						checkedFonts.insert(filename, foCache);
						return true;
					}
					FT_Get_Glyph_Name(face, gindex, buf, 50);
					QString newName = QString(reinterpret_cast<char*>(buf));
					if (newName == glyName)
					{
						HasNames = false;
						Subset = true;
					}
					glyName = newName;
					charcode = FT_Get_Next_Char( face, charcode, &gindex );
				}
				foCache.isOK = true;
				checkedFonts[filename] = foCache;
			}
			else
			{
				checkedFonts[filename].isOK = true;
				checkedFonts[filename].isChecked = true;
			}
		}
	}
	int faceindex=0;
	while (!error)
	{
		QString fam(face->family_name);
		QString sty(face->style_name);
		if (sty == "Regular")
		{
			switch (face->style_flags)
			{
				case 0:
					break;
				case 1:
					sty = "Italic";
					break;
				case 2:
					sty = "Bold";
					break;
				case 3:
					sty = "Bold Italic";
					break;
				default:
					break;
			}
		}
		QString ts(fam + " " + sty);
		QString alt("");
		const char* psName = FT_Get_Postscript_Name(face);
		QString qpsName;
		if (psName)
			qpsName = QString(psName);
		else
			qpsName = ts;
		Foi *t = find(ts);
		if (t)
		{
			if (t->RealName() != qpsName)
			{
				alt = " ("+qpsName+")";
				ts += alt;
				sty += alt;
			}
		}
		t = find(ts);
		if (!t)
		{
			switch (format) 
			{
				case Foi::PFA:
					t = new Foi_pfa(fam, sty, ts, qpsName, filename, faceindex, true);
					t->Subset = Subset;
					break;
				case Foi::PFB:
					t = new Foi_pfb(fam, sty, ts, qpsName, filename, faceindex, true);
					t->Subset = Subset;
					break;
				case Foi::SFNT:
					t = new Foi_ttf(fam, sty, ts, qpsName, filename, faceindex, true);
					getSFontType(face, t->typeCode);
					if (t->typeCode == Foi::OTF) 
					{
						t->isOTF = true;
						t->Subset = true;
					}
					else
						t->Subset = Subset;
					break;
				case Foi::TTCF:
					t = new Foi_ttf(fam, sty, ts, qpsName, filename, faceindex, true);
					t->formatCode = Foi::TTCF;
					t->typeCode = Foi::TTF;
					//getSFontType(face, t->typeCode);
					if (t->typeCode == Foi::OTF) 
					{
						t->isOTF = true;
						t->Subset = true;
					}
					else
						t->Subset = Subset;
					break;
				case Foi::TYPE42:
					t = new Foi_ttf(fam, sty, ts, qpsName, filename, faceindex, true);
					getSFontType(face, t->typeCode);
					if (t->typeCode == Foi::OTF) 
					{
						t->isOTF = true;
						t->Subset = true;
					}
					else
						t->Subset = Subset;
					break;
				default:
				/* catching any types not handled above to silence compiler */
					break;
			}
			insert(ts,t);
			t->HasNames = HasNames;
			t->EmbedPS = true;
			t->UseFont = true;
			t->CharWidth[13] = 0;
			t->CharWidth[28] = 0;
			t->CharWidth[9] = 1;
			t->PrivateFont = DocName;
			//setBestEncoding(face); //AV
			switch (face->charmap? face->charmap->encoding : -1)
			{
				case	 FT_ENCODING_ADOBE_STANDARD: 
					t->FontEnc = QString("StandardEncoding");
					break;
				case	 FT_ENCODING_APPLE_ROMAN: 
					t->FontEnc = QString("MacRomanEncoding");
					break;
				case	 FT_ENCODING_ADOBE_EXPERT: 
					t->FontEnc = QString("MacExpertEncoding");
					break;
				case	 FT_ENCODING_ADOBE_LATIN_1: 
					t->FontEnc = QString("WinAnsiEncoding");
					break;
				case	 FT_ENCODING_UNICODE: 
					t->FontEnc = QString("Unicode");
					break;
				default:
					t->FontEnc = QString();
			}
			if (showFontInformation)
				sDebug(QObject::tr("Font %1 loaded from %2(%3)").arg(t->RealName()).arg(filename).arg(faceindex+1));

/*
//debug
			QByteArray bb;
			t->RawData(bb);
			QFile dump(QString("/tmp/fonts/%1-%2").arg(ts).arg(psName));
			dump.open(IO_WriteOnly);
			QDataStream os(&dump);
			os.writeRawBytes(bb.data(), bb.size());
			dump.close();
*/
			FT_Done_Face(face);
			face=NULL;
			++faceindex;
			error = FT_New_Face( library, filename, faceindex, &face );
		}
		else 
		{
			if (showFontInformation)
				sDebug(QObject::tr("Font %1(%2) is duplicate of %3").arg(filename).arg(faceindex+1).arg(t->fontPath()));
			// this is needed since eg. AppleSymbols will happily return a face for *any* face_index
			error = true;
		}
	} //while
	
	if (face != 0)
		FT_Done_Face( face );
	return false;
}

void SCFonts::removeFont(QString name)
{
	remove(name);
	updateFontMap();
}

#ifdef HAVE_FONTCONFIG
// Use Fontconfig to locate and load fonts.
void SCFonts::AddFontconfigFonts()
{
	// All-in-one library setup. Perhaps this should be in
	// the SCFonts constructor.
	FcConfig* FcInitLoadConfigAndFonts();
	// The pattern controls what fonts to match. In this case we want to
	// match all scalable fonts, but ignore bitmap fonts.
	FcPattern* pat = FcPatternBuild(NULL,
									FC_SCALABLE, FcTypeBool, true,
									NULL);
	// The ObjectSet tells FontConfig what information about each match to return.
	// We currently just need FC_FILE, but other info like font family and style
	// is availible - see "man fontconfig".
	FcObjectSet* os = FcObjectSetBuild (FC_FILE, (char *) 0);
	// Now ask fontconfig to retrieve info as specified in 'os' about fonts
	// matching pattern 'pat'.
	FcFontSet* fs = FcFontList(0, pat, os);
	FcObjectSetDestroy(os);
	FcPatternDestroy(pat);
	// Create the Freetype library
	bool error;
	FT_Library library = NULL;
	error = FT_Init_FreeType( &library );
	// Now iterate over the font files and load them
	int i;
	for (i = 0; i < fs->nfont; i++) 
	{
		FcChar8 *file = NULL;
		if (FcPatternGetString (fs->fonts[i], FC_FILE, 0, &file) == FcResultMatch)
		{
			if (showFontInformation)
				sDebug(QObject::tr("Loading font %1 (found using fontconfig)").arg(QString((char*)file)));
			error = AddScalableFont(QString((char*)file), library, "");
			if (showFontInformation && error)
				sDebug(QObject::tr("Font %1 (found using fontconfig) is broken, discarding it").arg(QString((char*)file)));
		}
		else
			if (showFontInformation)
				sDebug(QObject::tr("Failed to load a font - freetype2 couldn't find the font file"));
	}
	FT_Done_FreeType( library );
}

#else
#if !defined(QT_MAC) && !defined(_WIN32)

void SCFonts::AddXFontPath()
{
	int pathcount,i;
	Display *display=XOpenDisplay(NULL);
	char **fontpath=XGetFontPath(display,&pathcount);
	for(i=0; i<pathcount; ++i)
		AddPath(fontpath[i]);
	XFreeFontPath(fontpath);
}

/* replacement for AddXFontServerPath() for correctly parsing
 * RedHad-Style /etc/X11/fs/config files */

void SCFonts::AddXFontServerPath()
{
	QFile fs("/etc/X11/fs/config");
	if(!(fs.exists()))
	{
		fs.setName("/usr/X11R6/lib/X11/fs/config");
		if(!(fs.exists()))
		{
			fs.setName("/usr/X11/lib/X11/fs/config");
			if(!(fs.exists()))
				return;
		}
	}

	if (fs.open(IO_ReadOnly))
	{
		QString fsconfig,paths,tmp;
		QTextStream tsx(&fs);
		fsconfig = tsx.read();
		fs.close();

		int pos = fsconfig.find("catalogue");
		paths = fsconfig.mid(pos);
		paths = paths.mid(paths.find("=")+1);

		pos=0;
		do {
			pos = paths.find("\n",pos+1);
		} while (pos > -1 && paths.mid(pos-1, 1) == ",");

		if(pos<0) pos=paths.length();
		paths = paths.left(pos);
		paths = paths.simplifyWhiteSpace();
		paths.replace(QRegExp(" "), "");
		paths.replace(QRegExp(","), "\n");
		paths += "\n";
		paths = paths.stripWhiteSpace();

		pos=-1;
		do {
			int pos2;
			pos2 = paths.find("\n",pos+1);
			tmp = paths.mid(pos+1,(pos2-pos)-1);
			pos=pos2;

			AddPath(tmp);

		} while (pos > -1);
	}
}
#endif
#endif

/* Add an extra path to our font search path,
 * allowing a user to have extra fonts installed
 * only for this user. Can also be used also as an emergency
 * fallback if no suitable fonts are found elsewere */
void SCFonts::AddUserPath(QString )
{
	PrefsContext *pc = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *extraDirs = pc->getTable("ExtraFontDirs");
	for (int i = 0; i < extraDirs->getRowCount(); ++i)
		AddPath(extraDirs->get(i, 0));
}

void SCFonts::ReadCacheList(QString pf)
{
	QFile fr(pf + "/cfonts.xml");
	QFileInfo fir(fr);
	if (fir.exists())
		fr.remove();
	checkedFonts.clear();
	struct testCache foCache;
	QDomDocument docu("fontcacherc");
	QFile f(pf + "/checkfonts.xml");
	if(!f.open(IO_ReadOnly))
		return;
	ScMW->setSplashStatus( QObject::tr("Reading Font Cache") );
	QTextStream ts(&f);
	ts.setEncoding(QTextStream::UnicodeUTF8);
	QString errorMsg;
	int errorLine = 0, errorColumn = 0;
	if( !docu.setContent(ts.read(), &errorMsg, &errorLine, &errorColumn) )
	{
		f.close();
		return;
	}
	f.close();
	QDomElement elem = docu.documentElement();
	if (elem.tagName() != "CachedFonts")
		return;
	QDomNode DOC = elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc = DOC.toElement();
		if (dc.tagName()=="Font")
		{
			foCache.isChecked = false;
			foCache.isOK = static_cast<bool>(dc.attribute("Status", "1").toInt());
			foCache.lastMod = QDateTime::fromString(dc.attribute("Modified"), Qt::ISODate);
			checkedFonts.insert(dc.attribute("File"), foCache);
		}
		DOC = DOC.nextSibling();
	}
}

void SCFonts::WriteCacheList(QString pf)
{
	QDomDocument docu("fontcacherc");
	QString st="<CachedFonts></CachedFonts>";
	docu.setContent(st);
	QDomElement elem = docu.documentElement();
	QMap<QString, testCache>::Iterator it;
	for (it = checkedFonts.begin(); it != checkedFonts.end(); ++it)
	{
		if (it.data().isChecked)
		{
			QDomElement fosu = docu.createElement("Font");
			fosu.setAttribute("File",it.key());
			fosu.setAttribute("Status",static_cast<int>(it.data().isOK));
			fosu.setAttribute("Modified",it.data().lastMod.toString(Qt::ISODate));
			elem.appendChild(fosu);
		}
	}
	ScMW->setSplashStatus( QObject::tr("Writing updated Font Cache") );
	QFile f(pf + "/checkfonts.xml");
	if(f.open(IO_WriteOnly))
	{
		QTextStream s(&f);
		s.setEncoding(QTextStream::UnicodeUTF8);
		s<<docu.toString();
		f.close();
	}
	checkedFonts.clear();
}

void SCFonts::GetFonts(QString pf, bool showFontInfo)
{
	showFontInformation=showFontInfo;
	FontPath.clear();
	ReadCacheList(pf);
	ScMW->setSplashStatus( QObject::tr("Searching for Fonts") );
	AddUserPath(pf);
	// Search the system paths
	QStringList ftDirs = ScPaths::getSystemFontDirs();
	for (unsigned int i = 0; i < ftDirs.count(); i++)
		AddScalableFonts( ftDirs[i] );
	// Search Scribus font path
	if (!ScPaths::instance().fontDir().isEmpty() && QDir(ScPaths::instance().fontDir()).exists())
		AddScalableFonts( ScPaths::instance().fontDir() );
// if fontconfig is there, it does all the work
#if HAVE_FONTCONFIG
	// Search fontconfig paths
	for(QStrListIterator fpi(FontPath) ; fpi.current() ; ++fpi)
		AddScalableFonts(fpi.current());
	AddFontconfigFonts();
#else
// on X11 look there:
#ifdef Q_WS_X11
	AddXFontPath();
	AddXFontServerPath();
#endif
// add user and X11 fonts:
	for(QStrListIterator fpi(FontPath) ; fpi.current() ; ++fpi) 
		AddScalableFonts(fpi.current());
#endif
	updateFontMap();
	WriteCacheList(pf);
}
