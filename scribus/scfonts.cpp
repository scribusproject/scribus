/*
	Scribus font handling code - copyright (C) 2001 by
	Alastair M. Robinson

	Based on code by Christian Tpp and Franz Schmid.

	Contributed to the Scribus project under the terms of
	the GNU General Public License version 2, or any later
	version, at your discretion.
*/

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
#include <X11/X.h>
#include <X11/Xlib.h>
#include <qregexp.h>

#include <cstdlib>
#include "scfonts.h"
#include "scfonts_ttf.h"

#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

extern FPointArray traceChar(FT_Face face, uint chr, int chs, double *x, double *y, bool *err);

extern bool loadText(QString nam, QString *Buffer);
extern bool GlyNames(QMap<uint, QString> *GList, QString Dat);

Foi::Foi(QString scname, QString path, bool embedps) :
	SCName(scname), Datei(path), EmbedPS(embedps)
{
	isOTF = false;
	Subset = false;
}

QString Foi::RealName()
{
	QString result="Base";
	return(result);
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
	return GlyNames(GList, Datei);
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
	error = FT_New_Face( library, Datei, 0, &face );
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
	Subclass of Foi, for Postscript fonts that could possibly have a .afm file
	associated with them for metrics information.
*/

class Foi_postscript : public Foi
{
	public:
		Foi_postscript(QString scname, QString path, bool embedps) :
		Foi(scname,path,embedps), metricsread(false)
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
			HasMetrics=true;
		}

		virtual QString RealName()
		{
			return(cached_RealName);
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
				qDebug(QObject::tr("Font %1 is broken, discarding it").arg(Datei));
				return false;
			}
			error = FT_New_Face( library, Datei, 0, &face );
			if (error)
			{
				UseFont = false;
				qDebug(QObject::tr("Font %1 is broken, discarding it").arg(Datei));
				return false;
			}
			uniEM = static_cast<double>(face->units_per_EM);
			QString afnm = Datei.left(Datei.length()-3);
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
			if (face->num_glyphs < 257)
			{
				for(int u = 0; u < face->num_charmaps; u++)
				{
					if (face->charmaps[u]->encoding == FT_ENCODING_ADOBE_CUSTOM)
					{
						FT_Set_Charmap(face,face->charmaps[u]);
						break;
					}
				}
			}
			gindex = 0;
			charcode = FT_Get_First_Char( face, &gindex );
			while ( gindex != 0 )
			{
				error = FT_Load_Glyph( face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
				if (error)
				{
					UseFont = false;
					qDebug(QObject::tr("Font %1 is broken, discarding it").arg(Datei));
					break;
				}
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
			FT_Done_FreeType( library );
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
		Foi_pfb(QString scname, QString path, bool embedps) :
		Foi_postscript(scname,path,embedps)
		{
		}

		virtual bool EmbedFont(QString &str)
		{
			QFile f(Datei);
			QString tmp2 = "";
			if (f.open(IO_ReadOnly))
			{
				QByteArray bb(f.size());
				f.readBlock(bb.data(), f.size());
				if ((bb[0] == char(0x80)) && (static_cast<int>(bb[1]) == 1))
				{
					QString tmp3="";
					QString tmp4 = "";
					uint posi,cxxc=0;
					for (posi = 6; posi < bb.size(); ++posi)
					{
						if ((bb[posi] == char(0x80)) && (static_cast<int>(bb[posi+1]) == 2))
							break;
						str += bb[posi];
					}
					uint ulen;
					ulen = bb[posi+2] & 0xff;
					ulen |= (bb[posi+3] << 8) & 0xff00;
					ulen |= (bb[posi+4] << 16) & 0xff0000;
					ulen |= (bb[posi+5] << 24) & 0xff000000;
          if (ulen > bb.size())
						ulen = bb.size()-7;
					posi += 6;
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
					posi += 6;
					for (uint j = posi; j < bb.size(); ++j)
					{
						if ((bb[j] == static_cast<char>(0x80)) && (static_cast<int>(bb[j+1]) == 3))
							break;
						if(bb[j]=='\r')
							str+="\n";
						else
							str += bb[j];
					}
					str += "\n";
					cxxc = 0;
				}
				f.close();
				return(true);
			}
			else
				return(false);
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
		Foi_pfa(QString scname, QString path, bool embedps) :
		Foi_postscript(scname,path,embedps)
		{
		}
		virtual bool EmbedFont(QString &str)
		{
			return loadText(Datei, &str);
		}
};


/***************************************************************************/

SCFonts::~SCFonts()
{
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
	FT_Library library = NULL;
	QString pathfile;
	bool error;
	error = FT_Init_FreeType( &library );        
	QDir d(path, "*", QDir::Name, QDir::Dirs | QDir::Files | QDir::Readable);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			QFileInfo fi(path+d[dc]);
			if (!fi.exists())      // Sanity check for broken Symlinks
				continue;
			if (fi.isSymLink())
			{
				QFileInfo fi3(fi.readLink());
				if (fi3.isRelative())
					pathfile = path+fi.readLink();
				else
					pathfile = fi3.absFilePath();
			}
			else
				pathfile = path+d[dc];
			QFileInfo fi2(pathfile);
			QString ext = fi.extension(false).lower();
			QString ext2 = fi2.extension(false).lower();
			if ((ext != ext2) && (ext == "")) ext = ext2;
			if ((ext == "pfa") || (ext == "pfb") || (ext == "ttf") || (ext == "otf"))
			{
				error = AddScalableFont(pathfile,library, DocName);
				if (error)
					qDebug(QObject::tr("Font %1 is broken, discarding it").arg(pathfile));
			}
		}
	}
	FT_Done_FreeType( library );
}

// Load a single font into the library from the passed filename. Returns true on error.
bool SCFonts::AddScalableFont(QString filename, FT_Library &library, QString DocName)
{
	FT_Face face = NULL;
	bool error = FT_New_Face( library, filename, 0, &face );
	if (error)
		return true;
	QString ts = QString(face->family_name) + " " + QString(face->style_name);
	if (!find(ts))
	{
		Foi *t=0;
		// Yes, I realise we're checking the extension twice, but it's not
		// a big cost, and nicer than passing it into this function.
		//QFileInfo fi2(filename);
		QString ext = QFileInfo(filename).extension(false).lower();
		if(ext == "pfa")
			t = new Foi_pfa(ts, filename, true);
		else if(ext == "pfb")
			t = new Foi_pfb(ts, filename, true);
		else if(ext == "ttf")
			t = new Foi_ttf(ts, filename, true);
		else if(ext == "otf")
			t = new Foi_ttf(ts, filename, true);
		if(t)
		{
			t->cached_RealName = QString(FT_Get_Postscript_Name(face));
			t->Font = qApp->font();
			t->Font.setPointSize(qApp->font().pointSize());
			if (ext == "otf")
			{
				t->isOTF = true;
				t->Subset = true;
			}
			insert(ts,t);
			t->EmbedPS = true;
			t->UseFont = true;
			t->CharWidth[13] = 0;
			t->CharWidth[28] = 0;
			t->CharWidth[9] = 1;
			t->Family = QString(face->family_name);
			t->PrivateFont = DocName;
		}
	}
	if (face != 0)
		FT_Done_Face( face );
	return false;
}

void SCFonts::removeFont(QString name)
{
	remove(name);
}

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

/* Add an extra path to the X-Server's Fontpath
 * allowing a user to have extra Fonts installed
 * only for this user. Was used also as an emergency
 * fallback if no suitable Font was found elsewere */
void SCFonts::AddUserPath(QString pf)
{
	QFile fx(pf+"/scribusfont.rc");
	ExtraPath="";
	if ((fx.exists()) && (fx.size() > 0))
	{
		if (fx.open(IO_ReadOnly))
		{
			QTextStream tsx(&fx);
			while (!tsx.atEnd())
				{
				ExtraPath = tsx.readLine();
				AddPath(ExtraPath);
				}
			fx.close();
		}
	}
}

void SCFonts::GetFonts(QString pf)
{
	FontPath.clear();
	AddUserPath(pf);
	AddXFontPath();
	AddXFontServerPath();
	for(QStrListIterator fpi(FontPath) ; fpi.current() ; ++fpi)
		AddScalableFonts(fpi.current());
	AddFontconfigFonts();
}

// Use Fontconfig to locate and load fonts.
void SCFonts::AddFontconfigFonts()
{
#ifdef HAVE_FONTCONFIG
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
    for (i = 0; i < fs->nfont; i++) {
        FcChar8 *file = NULL;
        if (FcPatternGetString (fs->fonts[i], FC_FILE, 0, &file) == FcResultMatch)
        {
            error = AddScalableFont(QString((char*)file), library, "");
            if (error)
                qDebug(QObject::tr("Font %1 (found using fontconfig) is broken, discarding it").arg(QString((char*)file)));
        }
        else
            qDebug(QObject::tr("Failed to load a font - freetype couldn't find the font file"));
    }
    FT_Done_FreeType( library );
#endif
}
