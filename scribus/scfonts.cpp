/*
	Scribus font handling code - copyright (C) 2001 by
	Alastair M. Robinson

	Based on code by Christian Töpp and Franz Schmid.

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
#include <X11/X.h>
#include <X11/Xlib.h>
#include <qtextcodec.h>
#include <qregexp.h>

#include <cstdlib>
#include "scfonts.h"
#include "scfonts_ttf.h"
#ifdef HAVE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
#endif

extern bool loadText(QString nam, QString *Buffer);
extern bool GlyNames(QMap<uint, QString> *GList, QString Dat);

SCFonts_Encoding Foi::StdEncoding("Stdenc.txt");
SCFonts_Encoding Foi::TTFEncoding("TTFenc.txt");

Foi::Foi(QString scname, QString path, bool embedps, SCFonts_Encoding &encoding) :
	SCName(scname), Datei(path), EmbedPS(embedps), Encoding(encoding)
{
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
	Class Foi_postscript
	Subclass of Foi, for Postscript fonts that could possibly have a .afm file
	associated with them for metrics information.
*/

class Foi_postscript : public Foi
{
	public:
		Foi_postscript(QString scname, QString path, bool embedps, SCFonts_Encoding &encoding) :
		Foi(scname,path,embedps,encoding), metricsread(false)
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
#ifdef HAVE_FREETYPE
			HasMetrics=true;
#else	
			QString afm=Datei.left(Datei.length()-3);
			QFile f(afm+"afm");
			if(f.exists())
				HasMetrics=true;
			else
			{
				f.setName(afm+"AFM");
				if(f.exists())
					HasMetrics=true;
				else
				{
					f.setName(afm+"Afm");
					if(f.exists())
						HasMetrics=true;
				}
			}
#endif
		}

		virtual QString RealName()
		{
			if(cached_RealName)
				return(cached_RealName);
			QString tmp = "a /NoName";
			QString tmp2 = "";
			loadText(Datei, &tmp2);
			int bd = tmp2.find("/FontName");
			tmp = tmp2.mid(bd, 80);
			QString rn,dummy;
			QTextIStream (&tmp) >> dummy >> rn;
			cached_RealName=rn.remove(0,1);
			return(cached_RealName);
		}

		virtual bool ReadMetrics()  // routine by Franz Schmid - modified by Alastair M. Robinson
		{
			if(metricsread)
				return(HasMetrics);
#ifdef HAVE_FREETYPE
			CharWidth.clear();
			QString tmp;
			bool			error;
			FT_Library library;
			FT_ULong  charcode;
			FT_UInt   gindex;
			FT_Face   face;
			error = FT_Init_FreeType( &library );
			if (error)
				{
				UseFont = false;
				return false;
				}
			error = FT_New_Face( library, Datei, 0, &face );
			if (error)
				{
				UseFont = false;
				return false;
				}
			uniEM = static_cast<float>(face->units_per_EM);
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
			CapHeight = Ascent;
			ItalicAngle = "0";
			StdVW = "1";
			FontBBox = tmp.setNum(face->bbox.xMin)+" "+tmp.setNum(face->bbox.yMin)+" "+tmp.setNum(face->bbox.xMax)+" "+tmp.setNum(face->bbox.yMax);
			IsFixedPitch = face->face_flags & 4;
			gindex = 0;
			charcode = FT_Get_First_Char( face, &gindex );
			while ( gindex != 0 )
				{
				FT_Load_Glyph( face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
				CharWidth.insert(charcode, face->glyph->metrics.horiAdvance / uniEM);
				charcode = FT_Get_Next_Char( face, charcode, &gindex );
				}
			FT_Done_FreeType( library );
			HasMetrics=true;
			metricsread=true;
#else
			QString afnm,tmpaf, afmz, AFMName, Enco;
			QStringList wt3, wt4, wtiso;
			int AFMIndex, AFMWidth;
			float afmwid;
			bool stdEnc = true;
			AFMIndex = 0;
			AFMWidth = 0;

			for (int am = 0; am < 256; ++am)
				CharWidth[am] = 0;

			afnm=Datei.left(Datei.length()-3);
			QFile afm(afnm+"afm");
			if(!(afm.exists()))
			{
				afm.setName(afnm+"Afm");
				if(!(afm.exists()))
				{
					afm.setName(afnm+"AFM");
				}
			}
			if (afm.exists())
			{
				if (afm.open(IO_ReadOnly))
				{
					QTextStream ts7(&afm);
					tmpaf = ts7.read();
					afm.close();
					QTextStream ts8(&tmpaf, IO_ReadOnly);
					while (!ts8.atEnd())
					{
						afmz = ts8.readLine();
						afmz = afmz.simplifyWhiteSpace();
						if (afmz.startsWith("Ascender"))
							Ascent = afmz.remove(0, 9);
						if (afmz.startsWith("CapHeight"))
							CapHeight = afmz.remove(0, 10);
						if (afmz.startsWith("Descender"))
							Descender = afmz.remove(0, 10);
						if (afmz.startsWith("ItalicAngle"))
							ItalicAngle = afmz.remove(0, 12);
						if (afmz.startsWith("StdVW"))
							StdVW = afmz.remove(0, 6);
						if (afmz.startsWith("IsFixedPitch"))
							{
							Enco = afmz.remove(0, 13);
							if (Enco == "false")
								IsFixedPitch = false;
							else
								IsFixedPitch = true;
							}
						if (afmz.startsWith("FontBBox"))
							FontBBox = afmz.remove(0, 9);
						if (afmz.startsWith("StartCharMetrics"))
							break;
						if (afmz.startsWith("EncodingScheme"))
						{
							Enco = afmz.stripWhiteSpace();
							Enco = Enco.remove(0, 15);
							if (Enco == "FontSpecific")
								stdEnc = false;
							else
								stdEnc = true;
						}
					}
					while (!afmz.startsWith("EndCharMetrics"))
					{
						afmz = ts8.readLine();
						wt3 = QStringList::split(";", afmz);
						AFMIndex=0;
						AFMName="";
						for (uint wtc = 0; wtc < wt3.count(); ++wtc)
						{
							wt4 = QStringList::split(" ", wt3[wtc]);
							if (wt4[0] == "C")
								AFMIndex = wt4[1].toInt();
							if (wt4[0] == "WX")
								AFMWidth = wt4[1].toInt();
							if (wt4[0] == "N")
								AFMName = wt4[1];
						}
						afmwid = float(AFMWidth) / 1000.0;
						if (AFMIndex > 0)		// Whatever the encoding of the font, we need
							{									// to extract metrics for ISO-latin1.
																// Either we can map from the font's encoding
																// to latin 1 by name (if the font has standard
																// glyph names), or we can map by code, assuming
																// the codes correspond to standard encoding.
							if(Encoding.present(AFMName))
								{
								CharWidth[Encoding[AFMName]] = afmwid;
								}
							else
								{
/*								if(StdEncoding.present(AFMIndex))
									{
									QString &glyph=StdEncoding[AFMIndex];
									if(Encoding.present(glyph))
										CharWidth[Encoding[glyph]] = afmwid;
									else
										CharWidth[AFMIndex] = afmwid;
									}
								else    */
									CharWidth[AFMIndex] = afmwid;
								}
							}
						else
							{
							if(Encoding.present(AFMName))
								CharWidth[Encoding[AFMName]] = afmwid;
							}
						}
					HasMetrics=true;
					metricsread=true;
				}
				else
				{
					HasMetrics=false;
				}
			}
#endif
			return(HasMetrics);
		}
	private:
		QString cached_RealName;
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
		Foi_pfb(QString scname, QString path, bool embedps, SCFonts_Encoding &encoding) :
		Foi_postscript(scname,path,embedps,encoding)
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
						if ((bb[j] == char(0x80)) && (static_cast<int>(bb[j+1]) == 3))
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
		Foi_pfa(QString scname, QString path, bool embedps, SCFonts_Encoding &encoding) :
		Foi_postscript(scname,path,embedps,encoding)
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
	if(ExtraPath.length())
	{
		system("xset fp- "+ExtraPath);
		system("xset fp rehash");
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

void SCFonts::AddScalableFonts(const QString &path)
{
  QFile fp(QString(path)+"fonts.scale");
	if(!(fp.exists()))
		fp.setName(QString(path)+"fonts.dir");
	if(fp.open(IO_ReadOnly))
	{
		int entries;
		QString tmp;
		QTextStream ts(&fp);
		entries = ts.readLine().toInt();
		while(tmp = ts.readLine())
		{
			int pos=tmp.find(" ");
			QString filename=tmp.left(pos);
			QFileInfo fo(QString(path)+filename);
			if (!fo.exists())
				continue;
			tmp=tmp.right(tmp.length()-(pos+1)).stripWhiteSpace();

			QStringList wt = QStringList::split("-",tmp);
			if(wt.count()>4)
			{
				QString FoEnc = wt[wt.count()-2].lower()+"-"+wt[wt.count()-1].lower();
				QString LoEnc = QString(QTextCodec::codecForLocale()->name()).lower().replace(QRegExp(" "), "");
				QString FName=wt[1];
				QString Style=wt[2];
				Style[0] = Style[0].upper();
				FName[0] = FName[0].upper();
				QString SCName = FName + " " + Style;
//				QString SCName = FName[0].upper() + FName.remove(0,1) + " ";
//				SCName += Style[0].upper() + Style.remove(0,1);
				if (wt[3] == "i") SCName += " Italic";
				if (wt[3] == "o") SCName += " Oblique";
#if 0
				if (wt[4] != "normal")  // sadly, QT seems to ignore this field
				{
					QString scn_wid=wt[4][0].upper() + wt[4].remove(0,1);
					SCName+=" " + scn_wid;
				}
#endif
				if ((!(find(SCName)))
						&& ((FoEnc == LoEnc)
						|| ((FoEnc == "iso8859-1") && (LoEnc == "iso8859-15"))
						|| ((FoEnc == "iso8859-1") && (LoEnc == "utf-8"))
						|| (FoEnc == "adobe-fontspecific")))
				{
					Foi *t=0;
					if(filename.contains(".pfa",false))
						t=new Foi_pfa(SCName,path+filename,true,Encoding);
					else if(filename.contains(".pfb",false))
						t=new Foi_pfb(SCName,path+filename,true,Encoding);
					else if(filename.contains(".ttf",false))
						t=new Foi_ttf(SCName,path+filename,true,Encoding);
					if(t)
					{
						t->Font.setPointSize(12);
						t->Font.setRawName(tmp);
						t->Font.setPointSize(12);
						t->FontEnc = FoEnc;
						insert(SCName,t);
						t->EmbedPS = true;
						t->UseFont = true;
//						if(t->HasMetrics)
//							t->ReadMetrics();
						t->CharWidth[13] = 0;
					}
				}
			}
		}
		fp.close();
	}
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
	if ( fx.exists() )
	{
		if (fx.open(IO_ReadOnly))
		{
			QTextStream tsx(&fx);
			while (!tsx.atEnd())
				{
				ExtraPath += tsx.readLine();
				ExtraPath += ",";
				}
			fx.close();
		}
		ExtraPath = ExtraPath.left(ExtraPath.length()-1);
		system("xset fp+ "+ExtraPath);
		system("xset fp rehash");
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
}

void SCFonts::FreeExtraFonts()
{
	if(ExtraPath.length())
		{
		system("xset fp- "+ExtraPath);
		system("xset fp rehash");
		ExtraPath = "";
		}
}
