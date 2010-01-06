/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/*
	Scribus font handling code - copyright (C) 2001 by
	Alastair M. Robinson

	Based on code by Christian Tpp and Franz Schmid.

	Contributed to the Scribus project under the terms of
	the GNU General Public License version 2, or any later
	version, at your discretion.
*/

#include <QApplication>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QGlobalStatic>
#include <QMap>
#include <QRegExp>
#include <QString>


#include <cstdlib>
#include "scfonts.h"
#include "fonts/ftface.h"
#include "fonts/scface_ps.h"
#include "fonts/scface_ttf.h"
#include "fonts/scfontmetrics.h"

#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "scribus.h"
#include "scribuscore.h"
#ifdef Q_WS_X11
#include <X11/X.h>
#include <X11/Xlib.h>
#endif

#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_TABLES_H


#include "scpaths.h"
#include "util.h"



/***************************************************************************/

SCFonts::SCFonts() : QMap<QString,ScFace>(), FontPath()
{
//	insert("", ScFace::none()); // Wtf why inserting an empty entry here ????
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
	for ( ; it.hasNext(); it.next())
	{
		if (it.current().usable())
		{
			if (fontMap.contains(it.current().family()))
			{
				if (!fontMap[it.current().family()].contains(it.current().style()))
				{
					fontMap[it.current().family()].append(it.current().style());
				}
			}
			else
			{
				QStringList styles;
				styles.append(it.current().style());
				fontMap.insert(it.current().family(), styles);
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
	if(!FontPath.contains(p))
		FontPath.insert(FontPath.count(),p);
}

void SCFonts::AddScalableFonts(const QString &path, QString DocName)
{
	//Make sure this is not empty or we will scan the whole drive on *nix
	//QString::null+/ is / of course.
	if (path.isEmpty())
		return;
	FT_Library library = NULL;
	QString pathfile, fullpath;
	bool error;
	error = FT_Init_FreeType( &library );
	QString pathname(path);
	if ( !pathname.endsWith("/") )
		pathname += "/";
	pathname=QDir::toNativeSeparators(pathname);
	QDir d(pathname, "*", QDir::Name, QDir::Dirs | QDir::Files | QDir::Readable);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			// readdir may return . or .., which we don't want to recurse
			// over. Skip 'em.
			if (d[dc] == "." || d[dc] == "..")
				continue;
			fullpath = pathname+d[dc];
			QFileInfo fi(fullpath);
			if (!fi.exists())      // Sanity check for broken Symlinks
				continue;
			
			qApp->processEvents();
			
			bool symlink = fi.isSymLink();
			if (symlink)
			{
				QFileInfo fi3(fi.readLink());
				if (fi3.isRelative())
					pathfile = pathname+fi.readLink();
				else
					pathfile = fi3.absoluteFilePath();
			}
			else
				pathfile = fullpath;
			QFileInfo fi2(pathfile);
			if (fi2.isDir()) 
			{
				if (symlink)
				{
					// Check if symlink points to a parent directory
					// in order to avoid infinite recursion
					QString fullpath2 = fullpath, pathfile2 = pathfile;
					if (ScCore->isWinGUI())
					{
						// Ensure both path use same separators on Windows
						fullpath2 = QDir::toNativeSeparators(fullpath2.toLower());
						pathfile2 = QDir::toNativeSeparators(pathfile2.toLower());
					}
					if (fullpath2.startsWith(pathfile2))
						continue;
				}
				if (DocName.isEmpty())
					AddScalableFonts(pathfile);
				continue;
			}
			QString ext = fi.suffix().toLower();
			QString ext2 = fi2.suffix().toLower();
			if ((ext != ext2) && (ext.isEmpty())) 
				ext = ext2;
			if ((ext == "ttc") || (ext == "dfont") || (ext == "pfa") || (ext == "pfb") || (ext == "ttf") || (ext == "otf"))
			{
				error = AddScalableFont(pathfile, library, DocName);
			}
#ifdef Q_OS_MAC
			else if (ext.isEmpty() && DocName.isEmpty())
			{
				error = AddScalableFont(pathfile, library, DocName);
				if (error)
					error = AddScalableFont(pathfile + "/..namedfork/rsrc",library, DocName);
			}
#endif				
		}
	}
	FT_Done_FreeType( library );
}


/*****
   What to do with font files:
   - note mod. date
   - in FontCache?  => load faces from cache
   - load via FT    => or broken
   - for all faces:
       load face
       get fontinfo (type, names, styles, global metrics)
       check encoding(s)
       (calc. hash sum)
       create cache entry
 *****/


/**
 * tests magic words to determine the fontformat and preliminary fonttype
 */
void getFontFormat(FT_Face face, ScFace::FontFormat & fmt, ScFace::FontType & type)
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
	
	fmt = ScFace::UNKNOWN_FORMAT;
	type = ScFace::UNKNOWN_TYPE;
	if (ftIOFunc(fts, 0L, reinterpret_cast<FT_Byte *>(buf), 128) == FONT_NO_ERROR) 
	{
		if(strncmp(buf,T42_HEAD,strlen(T42_HEAD)) == 0) 
		{
			fmt = ScFace::TYPE42;
			type = ScFace::TTF;
		}
		else if(strncmp(buf,T1_HEAD,strlen(T1_HEAD)) == 0 ||
			    strncmp(buf,T1_ADOBE_HEAD,strlen(T1_ADOBE_HEAD)) == 0) 
		{
			fmt = ScFace::PFA;
			type = ScFace::TYPE1;
		}
		else if(strncmp(buf,PSFONT_ADOBE2_HEAD,strlen(PSFONT_ADOBE2_HEAD)) == 0 ||
			    strncmp(buf,PSFONT_ADOBE21_HEAD,strlen(PSFONT_ADOBE21_HEAD)) == 0 ||
			    strncmp(buf,PSFONT_ADOBE3_HEAD,strlen(PSFONT_ADOBE3_HEAD)) ==0) 
		{
			// Type2(CFF), Type0(Composite/CID), Type 3, Type 14 etc would end here
			fmt = ScFace::PFA;
			type = ScFace::UNKNOWN_TYPE;
		}
		else if(buf[0] == '\200' && buf[1] == '\1')
		{
			fmt = ScFace::PFB;
			type = ScFace::TYPE1;
		}
		else if(buf[0] == '\0' && buf[1] == '\1' 
				&& buf[2] == '\0' && buf[3] == '\0')
		{
			fmt = ScFace::SFNT;
			type = ScFace::TTF;
		}
		else if(strncmp(buf,"true",4) == 0)
		{
			fmt = ScFace::SFNT;
			type = ScFace::TTF;
		}
		else if(strncmp(buf,"ttcf",4) == 0)
		{
			fmt = ScFace::TTCF;
			type = ScFace::OTF;
		}
		else if(strncmp(buf,"OTTO",4) == 0)
		{
			fmt = ScFace::SFNT;
			type = ScFace::OTF;
		}
		// missing: raw CFF
	}
}

/**
 * Checks face, which must be sfnt based, for the subtype.
 * Possible values: TTF, CFF, OTF
 */
void getSFontType(FT_Face face, ScFace::FontType & type) 
{
	if (FT_IS_SFNT(face)) 
	{
		FT_ULong ret = 0;
		bool hasGlyph = ! FT_Load_Sfnt_Table(face, TTAG_glyf, 0, NULL,  &ret);
		hasGlyph &= ret > 0;
		bool hasCFF = ! FT_Load_Sfnt_Table(face, TTAG_CFF, 0, NULL,  &ret);
		hasCFF &= ret > 0;
		if (hasGlyph)
			type = ScFace::TTF;
		else if (hasCFF)
			type = ScFace::OTF;
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
	ScFace::FontFormat format;
	ScFace::FontType   type;
	FT_Face         face = NULL;
	struct testCache foCache;
	QFileInfo fic(filename);
	QDateTime lastMod = fic.lastModified();
	QTime lastModTime = lastMod.time();
	if (lastModTime.msec() != 0)  //Sometime file time is stored with precision up to msecs
	{
		lastModTime.setHMS(lastModTime.hour(), lastModTime.minute(), lastModTime.second());
		lastMod.setTime(lastModTime);
	}
	foCache.isOK = false;
	foCache.isChecked = true;
	foCache.lastMod = lastMod;
	if (checkedFonts.count() == 0)
	{
		firstRun = true;
		ScCore->setSplashStatus( QObject::tr("Creating Font Cache") );
	}
	bool error = FT_New_Face( library, QFile::encodeName(filename), 0, &face );
	if (error) 
	{
		if (face != NULL)
			FT_Done_Face( face );
		checkedFonts.insert(filename, foCache);
		if (showFontInformation)
			sDebug(QObject::tr("Font %1 is broken, discarding it").arg(filename));
		return true;
	}
	getFontFormat(face, format, type);
	if (format == ScFace::UNKNOWN_FORMAT) 
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
				ScCore->setSplashStatus( QObject::tr("New Font found, checking...") );
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
			if (checkedFonts[filename].lastMod != foCache.lastMod)
			{
				ScCore->setSplashStatus( QObject::tr("Modified Font found, checking...") );
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
		ScFace t;
		if (contains(ts))
		{
			t = (*this)[ts];
			if (t.psName() != qpsName)
			{
				alt = " ("+qpsName+")";
				ts += alt;
				sty += alt;
			}
		}
		t = (*this)[ts];
		if (t.isNone())
		{
			switch (format) 
			{
				case ScFace::PFA:
					t = ScFace(new ScFace_pfa(fam, sty, "", ts, qpsName, filename, faceindex));
					t.subset(Subset);
					break;
				case ScFace::PFB:
					t = ScFace(new ScFace_pfb(fam, sty, "", ts, qpsName, filename, faceindex));
					t.subset(Subset);
					break;
				case ScFace::SFNT:
					t = ScFace(new ScFace_ttf(fam, sty, "", ts, qpsName, filename, faceindex));
					getSFontType(face, t.m->typeCode);
					if (t.type() == ScFace::OTF) 
					{
						t.subset(true);
					}
					else
						t.subset(Subset);
					break;
				case ScFace::TTCF:
					t = ScFace(new ScFace_ttf(fam, sty, "", ts, qpsName, filename, faceindex));
					t.m->formatCode = ScFace::TTCF;
					t.m->typeCode = ScFace::TTF;
					//getSFontType(face, t.m->typeCode);
					if (t.type() == ScFace::OTF) 
					{
						t.subset(true);
					}
					else
						t.subset(Subset);
					break;
				case ScFace::TYPE42:
					t = ScFace(new ScFace_ttf(fam, sty, "", ts, qpsName, filename, faceindex));
					getSFontType(face, t.m->typeCode);
					if (t.type() == ScFace::OTF) 
					{
						t.subset(true);
					}
					else
						t.subset(Subset);
					break;
				default:
				/* catching any types not handled above to silence compiler */
					break;
			}
			insert(ts,t);
			t.m->hasNames = HasNames;
			t.embedPs(true);
			t.usable(true);
			t.m->status = ScFace::UNKNOWN;
			if (face->num_glyphs > 2048)
				t.subset(true);
			t.m->forDocument = DocName;
			//setBestEncoding(face); //AV
/*			switch (face->charmap? face->charmap->encoding : -1)
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
*/			if (showFontInformation)
				sDebug(QObject::tr("Font %1 loaded from %2(%3)").arg(t.psName()).arg(filename).arg(faceindex+1));

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
		}
		else 
		{
			if (showFontInformation)
				sDebug(QObject::tr("Font %1(%2) is duplicate of %3").arg(filename).arg(faceindex+1).arg(t.fontPath()));
			// this is needed since eg. AppleSymbols will happily return a face for *any* face_index
			if (faceindex > 0) {
				break;
			}
		}
		FT_Done_Face(face);
		face=NULL;
		++faceindex;
		error = FT_New_Face( library, QFile::encodeName(filename), faceindex, &face );
	} //while
	
	if (face != 0)
		FT_Done_Face( face );
	return error && faceindex == 0;
}

void SCFonts::removeFont(QString name)
{
	remove(name);
	updateFontMap();
}

const ScFace& SCFonts::findFont(const QString& fontname, ScribusDoc *doc)
{
	if (fontname.isEmpty())
		return ScFace::none();
	
	PrefsManager* prefsManager = PrefsManager::instance();
	
	if (!contains(fontname) || !(*this)[fontname].usable())
	{
		QString replFont;
		if ((!prefsManager->appPrefs.GFontSub.contains(fontname)) || (!(*this)[prefsManager->appPrefs.GFontSub[fontname]].usable()))
		{
			replFont = doc ? doc->toolSettings.defFont : prefsManager->appPrefs.toolSettings.defFont;
		}
		else
			replFont = prefsManager->appPrefs.GFontSub[fontname];
		ScFace repl = (*this)[replFont].mkReplacementFor(fontname, doc ? doc->DocName : QString(""));
		insert(fontname, repl);
	}
	else if ( doc && !doc->UsedFonts.contains(fontname) )
	{
		doc->AddFont(fontname, qRound(doc->toolSettings.defSize / 10.0));
	}
	return (*this)[fontname];	
}


QMap<QString,QString> SCFonts::getSubstitutions(const QList<QString> skip) const
{
	QMap<QString,QString> result;
	QMap<QString,ScFace>::ConstIterator it;
	for (it = begin(); it != end(); ++it)
	{
		if (it.value().isReplacement() && !skip.contains(it.key()))
			result[it.key()] = it.value().replacementName();
	}
	return result;
}


void SCFonts::setSubstitutions(const QMap<QString,QString>& substitutes, ScribusDoc* doc)
{
	QMap<QString,QString>::ConstIterator it;
	for (it = substitutes.begin(); it != substitutes.end(); ++it)
	{
		if (it.key() == it.value())
			continue;
		ScFace& font(const_cast<ScFace&>(findFont(it.key(), doc)));
		if (font.isReplacement())
		{
			font.chReplacementTo(const_cast<ScFace&>(findFont(it.value(), doc)), doc->DocName);
		}
	}
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
	// is available - see "man fontconfig".
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
		}
		else
			if (showFontInformation)
				sDebug(QObject::tr("Failed to load a font - freetype2 couldn't find the font file"));
	}
	FT_Done_FreeType( library );
}

#elif defined(Q_WS_X11)

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

	if (fs.open(QIODevice::ReadOnly))
	{
		QString fsconfig,paths,tmp;
		QTextStream tsx(&fs);
		fsconfig = tsx.readAll();
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
		paths = paths.simplified();
		paths.replace(QRegExp(" "), "");
		paths.replace(QRegExp(","), "\n");
		paths += "\n";
		paths = paths.trimmed();

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
	if(!f.open(QIODevice::ReadOnly))
		return;
	ScCore->setSplashStatus( QObject::tr("Reading Font Cache") );
	QTextStream ts(&f);
	ts.setCodec("UTF-8");
	QString errorMsg;
	int errorLine = 0, errorColumn = 0;
	if( !docu.setContent(ts.readAll(), &errorMsg, &errorLine, &errorColumn) )
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
		if (it.value().isChecked)
		{
			QDomElement fosu = docu.createElement("Font");
			fosu.setAttribute("File",it.key());
			fosu.setAttribute("Status",static_cast<int>(it.value().isOK));
			fosu.setAttribute("Modified",it.value().lastMod.toString(Qt::ISODate));
			elem.appendChild(fosu);
		}
	}
	ScCore->setSplashStatus( QObject::tr("Writing updated Font Cache") );
	QFile f(pf + "/checkfonts.xml");
	if(f.open(QIODevice::WriteOnly))
	{
		QTextStream s(&f);
		s.setCodec("UTF-8");
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
	ScCore->setSplashStatus( QObject::tr("Searching for Fonts") );
	AddUserPath(pf);
	// Search the system paths
	QStringList ftDirs = ScPaths::getSystemFontDirs();
	for (int i = 0; i < ftDirs.count(); i++)
		AddScalableFonts( ftDirs[i] );
	// Search Scribus font path
	if (!ScPaths::instance().fontDir().isEmpty() && QDir(ScPaths::instance().fontDir()).exists())
		AddScalableFonts( ScPaths::instance().fontDir() );
// if fontconfig is there, it does all the work
#if HAVE_FONTCONFIG
	// Search fontconfig paths
	QStringList::iterator fpi, fpend = FontPath.end();
	for(fpi = FontPath.begin() ; fpi != fpend; ++fpi) 
		AddScalableFonts(*fpi);
	AddFontconfigFonts();
#else
// on X11 look there:
#ifdef Q_WS_X11
	AddXFontPath();
	AddXFontServerPath();
#endif
// add user and X11 fonts:
	QStringList::iterator fpi, fpend = FontPath.end();
	for(fpi = FontPath.begin() ; fpi != fpend; ++fpi) 
		AddScalableFonts(*fpi);
#endif
	updateFontMap();
	WriteCacheList(pf);
}
