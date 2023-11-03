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
#include <QHash>
#include <QMap>
#include <QRegExp>
#include <QRawFont>
#ifdef Q_OS_WIN32
#include <QSet>
#include <QSettings>
#include <QStandardPaths>
#endif
#include <QString>
#include <QTextCodec>

#include <cstdlib>
#include <vector>

#include "scfonts.h"
#include "fonts/ftface.h"
#include "fonts/scface_ps.h"
#include "fonts/scface_ttf.h"
#include "fonts/scfontmetrics.h"

#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"

#include "scribuscore.h"
#include "scribusdoc.h"
#ifdef Q_OS_LINUX
#include <X11/X.h>
#include <X11/Xlib.h>
#endif

#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_TABLES_H

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ot.h>
#include <harfbuzz/hb-ft.h>

#include "scpaths.h"
#include "util_debug.h"



/***************************************************************************/

SCFonts::SCFonts()
{
//	insert("", ScFace::none()); // Wtf why inserting an empty entry here ????
	m_checkedFonts.clear();
}

void SCFonts::updateFontMap()
{
	fontMap.clear();

	SCFontsIterator it( *this );
	for ( ; it.hasNext(); it.next())
	{
		ScFace& currentFace = it.current();
		if (!currentFace.usable())
			continue;

		if (fontMap.contains(currentFace.family()))
		{
			if (!fontMap[currentFace.family()].contains(currentFace.style()))
			{
				fontMap[currentFace.family()].append(currentFace.style());
			}
		}
		else
		{
			QStringList styles;
			styles.append(currentFace.style());
			fontMap.insert(currentFace.family(), styles);
		}
	}
}

/* Add a path to the list of fontpaths, ensuring that
   a trailing slash is present.
   Checks to make sure this path is not already present
   before adding.
*/
void SCFonts::addPath(QString p)
{
	if (p.right(1) != "/")
		p += "/";
	if (!m_fontPaths.contains(p))
		m_fontPaths.insert(m_fontPaths.count(), p);
}

void SCFonts::addScalableFonts(const QString &path, const QString& DocName)
{
	//Make sure this is not empty or we will scan the whole drive on *nix
	//QString()+/ is / of course.
	if (path.isEmpty())
		return;
	FT_Library library = nullptr;
	QString pathfile, fullpath;
//	bool error;
//	error =
	FT_Init_FreeType( &library );
	QString pathname(path);
	if ( !pathname.endsWith("/") )
		pathname += "/";
	pathname = QDir::toNativeSeparators(pathname);
	QDir d(pathname, "*", QDir::Name, QDir::Dirs | QDir::Files | QDir::Readable);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint i = 0; i < d.count(); ++i)
		{
			// readdir may return . or .., which we don't want to recurse
			// over. Skip 'em.
			if (d[i] == "." || d[i] == "..")
				continue;
			fullpath = pathname + d[i];
			QFileInfo fi(fullpath);
			if (!fi.exists())      // Sanity check for broken Symlinks
				continue;
			
			QCoreApplication::processEvents();
			
			bool symlink = fi.isSymLink();
			if (symlink)
			{
				QFileInfo fi3(fi.symLinkTarget());
				if (fi3.isRelative())
					pathfile = pathname + fi.symLinkTarget();
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
					addScalableFonts(pathfile);
				continue;
			}
			QString ext = fi.suffix().toLower();
			QString ext2 = fi2.suffix().toLower();
			if ((ext != ext2) && (ext.isEmpty())) 
				ext = ext2;
			if ((ext == "ttc") || (ext == "dfont") || (ext == "pfa") || (ext == "pfb") || (ext == "ttf") || (ext == "otf"))
			{
				addScalableFont(pathfile, library, DocName);
			}
#ifdef Q_OS_MACOS
			else if (ext.isEmpty() && DocName.isEmpty())
			{
				bool error = addScalableFont(pathfile, library, DocName);
				if (error)
					error = addScalableFont(pathfile + "/..namedfork/rsrc",library, DocName);
			}
#endif				
		}
	}
	FT_Done_FreeType(library);
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
		if (strncmp(buf, T42_HEAD, strlen(T42_HEAD)) == 0) 
		{
			fmt = ScFace::TYPE42;
			type = ScFace::TTF;
		}
		else if (strncmp(buf, T1_HEAD, strlen(T1_HEAD)) == 0 ||
			     strncmp(buf, T1_ADOBE_HEAD, strlen(T1_ADOBE_HEAD)) == 0) 
		{
			fmt = ScFace::PFA;
			type = ScFace::TYPE1;
		}
		else if (strncmp(buf, PSFONT_ADOBE2_HEAD, strlen(PSFONT_ADOBE2_HEAD)) == 0 ||
			     strncmp(buf, PSFONT_ADOBE21_HEAD, strlen(PSFONT_ADOBE21_HEAD)) == 0 ||
			     strncmp(buf, PSFONT_ADOBE3_HEAD, strlen(PSFONT_ADOBE3_HEAD)) == 0) 
		{
			// Type2(CFF), Type0(Composite/CID), Type 3, Type 14 etc would end here
			fmt = ScFace::PFA;
			type = ScFace::UNKNOWN_TYPE;
		}
		else if (buf[0] == '\200' && buf[1] == '\1')
		{
			fmt = ScFace::PFB;
			type = ScFace::TYPE1;
		}
		else if (buf[0] == '\0' && buf[1] == '\1' 
				&& buf[2] == '\0' && buf[3] == '\0')
		{
			fmt = ScFace::SFNT;
			type = ScFace::TTF;
		}
		else if (strncmp(buf, "true", 4) == 0)
		{
			fmt = ScFace::SFNT;
			type = ScFace::TTF;
		}
		else if (strncmp(buf, "ttcf", 4) == 0)
		{
			fmt = ScFace::TTCF;
			type = ScFace::OTF;
		}
		else if (strncmp(buf, "OTTO", 4) == 0)
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
void getSubFontType(FT_Face face, ScFace::FontType & type) 
{
	if (FT_IS_SFNT(face)) 
	{
		FT_ULong ret = 0;
		bool hasGlyph = ! FT_Load_Sfnt_Table(face, TTAG_glyf, 0, nullptr,  &ret);
		hasGlyph &= ret > 0;
		bool hasCFF = ! FT_Load_Sfnt_Table(face, TTAG_CFF, 0, nullptr,  &ret);
		hasCFF &= ret > 0;
		if (hasGlyph)
			type = ScFace::TTF;
		else if (hasCFF)
			type = ScFace::OTF;
		// TODO: CID or no
	}
} 

// sort name records so the preferred ones come first
static bool nameComp(const FT_SfntName a, const FT_SfntName b)
{
	// sort preferred family first
	if (a.name_id != b.name_id)
	{
		if (a.name_id == TT_NAME_ID_PREFERRED_FAMILY)
			return true;
		if (b.name_id == TT_NAME_ID_PREFERRED_FAMILY)
			return false;
	}

	// sort Unicode platforms first
	// preferring MS platform as it is more likely to
	// not have bogus entries, being the more tested one.
	if (a.platform_id != b.platform_id)
	{
		if (a.platform_id == TT_PLATFORM_MICROSOFT)
			return true;
		if (b.platform_id == TT_PLATFORM_MICROSOFT)
			return false;
		if (a.platform_id == TT_PLATFORM_APPLE_UNICODE)
			return true;
		if (b.platform_id == TT_PLATFORM_APPLE_UNICODE)
			return false;
	}

	// sort Unicode encodings first
	if (a.encoding_id != b.encoding_id)
	{
		if (a.platform_id == TT_PLATFORM_MICROSOFT)
		{
			if (a.encoding_id == TT_MS_ID_UCS_4)
				return true;
			if (b.encoding_id == TT_MS_ID_UCS_4)
				return false;
			if (a.encoding_id == TT_MS_ID_UNICODE_CS)
				return true;
			if (b.encoding_id == TT_MS_ID_UNICODE_CS)
				return false;
		}
	}

	// sort English names first
	if (a.language_id != b.language_id)
	{
		if (a.platform_id == TT_PLATFORM_MICROSOFT)
		{
			if (a.language_id == TT_MS_LANGID_ENGLISH_UNITED_STATES)
				return true;
			if (b.language_id == TT_MS_LANGID_ENGLISH_UNITED_STATES)
				return false;
		}
	}

	// the rest is all the same for us
	return false;
}

static QString decodeNameRecord(FT_SfntName name)
{
	if (!name.string || name.string_len == 0)
		return QString();

	QByteArray encoding;
	if (name.platform_id == TT_PLATFORM_APPLE_UNICODE)
	{
		encoding = "UTF-16BE";
	}
	else if (name.platform_id == TT_PLATFORM_MICROSOFT)
	{
		switch (name.encoding_id) {
		case TT_MS_ID_SYMBOL_CS:
		case TT_MS_ID_UNICODE_CS:
		case TT_MS_ID_UCS_4:
			encoding = "UTF-16BE";
			break;
		case TT_MS_ID_SJIS:
			encoding = "Shift-JIS";
			break;
		case TT_MS_ID_GB2312:
			encoding = "GB18030";
			break;
		case TT_MS_ID_BIG_5:
			encoding = "Big5";
			break;
		default:
			break;
		}
	}

	if (encoding.isEmpty())
		return QString();

	QTextCodec* codec = QTextCodec::codecForName(encoding);
	if (!codec)
		return QString();

	QByteArray bytes((const char*) name.string, name.string_len);
	if (name.encoding_id == TT_MS_ID_GB2312 && bytes.startsWith('\0'))
	{
		// 16 bytes chars appears to be used as transport type for 8 bytes values.
		// We have to fix this so that Qt GB18030 codec process strings correctly
		for (int i = 0; i < (int) name.string_len / 2; ++i)
			bytes[i] = bytes[2 * i + 1];
		bytes.resize(name.string_len / 2);
	}

	QString string = codec->toUnicode(bytes);
	return string;
}

static QString getFamilyName(const FT_Face face)
{
	QString familyName(face->family_name);

	QVector<FT_SfntName> names;

	for (FT_UInt i = 0; i < FT_Get_Sfnt_Name_Count(face); i++)
	{
		FT_SfntName name;
		if (!FT_Get_Sfnt_Name(face, i, &name))
		{
			switch (name.name_id) {
			case TT_NAME_ID_FONT_FAMILY:
			case TT_NAME_ID_PREFERRED_FAMILY:
				if (name.platform_id != TT_PLATFORM_MACINTOSH)
					names.append(name);
				break;
			default:
				break;
			}
		}
	}

	if (!names.isEmpty())
	{
		std::sort(names.begin(), names.end(), nameComp);
		for (const FT_SfntName& name : qAsConst(names))
		{
			QString string(decodeNameRecord(name));
			if (!string.isEmpty())
			{
				familyName = string;
				break;
			}
		}
	}

	return familyName;
}

static QStringList getFontFeaturesFromTable(hb_tag_t table, hb_face_t *hb_face)
{
	QStringList fontFeaturesList;
	//get all supported Opentype Features
	unsigned count = hb_ot_layout_table_get_feature_tags(hb_face, table, 0, nullptr, nullptr);
	std::vector<hb_tag_t> features(count);
	hb_ot_layout_table_get_feature_tags(hb_face, table, 0,  &count, features.data());
	for (unsigned i = 0; i < count; ++i)
	{
		char feature[4] = {0};
		hb_tag_to_string(features[i], feature);
		std::string strFeature(feature, 4);
		fontFeaturesList.append(QString::fromStdString(strFeature));
	}
	fontFeaturesList.removeDuplicates();
	fontFeaturesList.sort();
	return fontFeaturesList;
}

static QStringList getFontFeatures(const FT_Face face)
{
	// Create hb-ft font and get hb_face from it
	hb_font_t *hb_font;
	hb_font = hb_ft_font_create(face, nullptr);
	hb_face_t *hb_face = hb_font_get_face(hb_font);
	//find Opentype Font Features in GSUB table
	QStringList featuresGSUB = getFontFeaturesFromTable(HB_OT_TAG_GSUB, hb_face);
	// find Opentype Font Features in GPOS table
	QStringList featuresGPOS = getFontFeaturesFromTable(HB_OT_TAG_GPOS, hb_face);

	hb_font_destroy(hb_font);

	QStringList fontFeatures = featuresGSUB + featuresGPOS;
	fontFeatures.removeDuplicates();
	fontFeatures.sort();
	return fontFeatures;
}

ScFace SCFonts::loadScalableFont(const QString &filename)
{
	ScFace t;
	if (filename.isEmpty())
		return t;
	FT_Library library = nullptr;
	bool error;
	error = FT_Init_FreeType( &library );
	QFileInfo fi(filename);
	if (!fi.exists())
		return t;
	bool Subset = false;
	char buf[128];
	QString glyName = "";
	ScFace::FontFormat format;
	ScFace::FontType   type;
	FT_Face         face = nullptr;
	error = FT_New_Face(library, QFile::encodeName(filename), 0, &face);
	if (error || (face == nullptr))
	{
		if (face != nullptr)
			FT_Done_Face(face);
		FT_Done_FreeType(library);
		return t;
	}
	getFontFormat(face, format, type);
	if (format == ScFace::UNKNOWN_FORMAT)
	{
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		return t;
	}
	bool HasNames = FT_HAS_GLYPH_NAMES(face);

	FT_UInt gindex = 0;
	FT_ULong charcode = FT_Get_First_Char(face, &gindex);
	while (gindex != 0)
	{
		error = FT_Load_Glyph(face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);
		if (error)
		{
			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return t;
		}
		FT_Get_Glyph_Name(face, gindex, buf, 128);
		QString newName(buf);
		if (newName == glyName)
		{
			HasNames = false;
			Subset = true;
		}
		glyName = newName;
		charcode = FT_Get_Next_Char(face, charcode, &gindex);
	}

	// Warning: code below is also present in addScalableFont, so if you do
	// any modification here, think also about modifying code in addScalableFont
	int faceIndex = 0;
	QString fam(getFamilyName(face));
	QStringList features(getFontFeatures(face));
	QString sty(face->style_name);
	if ((sty == "Regular" && face->style_flags != 0) || sty.isEmpty())
	{
		switch (face->style_flags)
		{
			case 0:
				sty = "Regular";
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
	QString fullName(fam);
	if (!sty.isEmpty())
		fullName += " " + sty;
	const char* psName = FT_Get_Postscript_Name(face);
	QString qpsName;
	if (psName)
		qpsName = QString(psName);
	else
		qpsName = fullName;
	if (t.isNone())
	{
		switch (format)
		{
			case ScFace::PFA:
				t = ScFace(new ScFace_PFA(fam, sty, "", fullName, qpsName, filename, faceIndex, features));
				t.subset(Subset);
				break;
			case ScFace::PFB:
				t = ScFace(new ScFace_PFB(fam, sty, "", fullName, qpsName, filename, faceIndex, features));
				t.subset(Subset);
				break;
			case ScFace::SFNT:
			case ScFace::TTCF:
			case ScFace::TYPE42:
				t = ScFace(new ScFace_ttf(fam, sty, "", fullName, qpsName, filename, faceIndex, features));
				getSubFontType(face, t.m_m->typeCode);
				t.subset(Subset);
				break;
			default:
				/* catching any types not handled above to silence compiler */
				break;
		}
		t.m_m->hasGlyphNames = HasNames;
		t.embedPs(true);
		t.usable(true);
		t.m_m->status = ScFace::UNKNOWN;
		if (face->num_glyphs > 2048)
			t.subset(true);
	}
	setBestEncoding(face);
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	return t;
}

static QString getFtError(int code)
{
#undef FTERRORS_H_
#define FT_ERRORDEF(e, v, s) ftErrors[e] = s;
	static QHash<int, QString> ftErrors;
#include FT_ERRORS_H
#undef FT_ERRORDEF

	if (ftErrors.contains(code))
		return ftErrors.value(code);
	return QString();
}

// Load a single font into the library from the passed filename. Returns true on error.
bool SCFonts::addScalableFont(const QString& filename, FT_Library &library, const QString& DocName)
{
	static bool firstRun;
	bool Subset = false;
	char buf[128];
	QString glyName = "";
	ScFace::FontFormat format;
	ScFace::FontType   type;
	FT_Face         face = nullptr;
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
	if (m_checkedFonts.count() == 0)
	{
		firstRun = true;
		ScCore->setSplashStatus( QObject::tr("Creating Font Cache") );
	}
	FT_Error error = FT_New_Face( library, QFile::encodeName(filename), 0, &face );
	if (error || (face == nullptr))
	{
		if (face != nullptr)
			FT_Done_Face(face);
		m_checkedFonts.insert(filename, foCache);
		addRejectedFont(filename, QObject::tr("Font is broken: \"%1\"").arg(getFtError(error)));
		if (m_showFontInfo)
			sDebug(QObject::tr("Font %1 is broken, discarding it. Error message: \"%2\"").arg(filename, getFtError(error)));
		return true;
	}
	if (face->family_name == nullptr)
	{
		addRejectedFont(filename, QObject::tr("Failed to load font: font family unspecified"));
		if (m_showFontInfo)
			sDebug(QObject::tr("Failed to load font %1 - font family unspecified").arg(filename));
		FT_Done_Face(face);
		m_checkedFonts.insert(filename, foCache);
		return true;
	}
	getFontFormat(face, format, type);
	if (format == ScFace::UNKNOWN_FORMAT) 
	{
		addRejectedFont(filename, QObject::tr("Failed to load font: font type unknown"));
		if (m_showFontInfo)
			sDebug(QObject::tr("Failed to load font %1 - font type unknown").arg(filename));
		FT_Done_Face(face);
		m_checkedFonts.insert(filename, foCache);
		return true;
	}
	// Some fonts such as Noto ColorEmoji are in fact bitmap fonts
	// and do not provide a valid value for units_per_EM
	if (face->units_per_EM == 0)
	{
		addRejectedFont(filename, QObject::tr("Failed to load font: font is not scalable"));
		if (m_showFontInfo)
			sDebug(QObject::tr("Failed to load font %1 - font is not scalable").arg(filename));
		FT_Done_Face(face);
		m_checkedFonts.insert(filename, foCache);
		return true;
	}
	bool HasNames = FT_HAS_GLYPH_NAMES(face);

	if (!m_checkedFonts.contains(filename))
	{
		if (!firstRun)
			ScCore->setSplashStatus( QObject::tr("New Font found, checking...") );
		FT_UInt gindex = 0;
		FT_ULong charcode = FT_Get_First_Char( face, &gindex );
		while ( gindex != 0 )
		{
			error = FT_Load_Glyph(face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);
			if (error)
			{
				auto errorMessage = QObject::tr("Font %1 has broken glyph %2 (charcode U+%3). Error message: \"%4\"")
							   .arg(filename)
							   .arg(gindex)
							   .arg(charcode, 4, 16, QChar('0'))
							   .arg(getFtError(error));
				addRejectedFont(filename, errorMessage);
				if (m_showFontInfo)
					sDebug(errorMessage);
				FT_Done_Face(face);
				m_checkedFonts.insert(filename, foCache);
				return true;
			}
			FT_Get_Glyph_Name(face, gindex, buf, 128);
			QString newName(buf);
			if (newName == glyName)
			{
				HasNames = false;
				Subset = true;
			}
			glyName = newName;
			charcode = FT_Get_Next_Char( face, charcode, &gindex );
		}
		foCache.isOK = true;
		m_checkedFonts.insert(filename, foCache);
	}
	else
	{
		auto& checkedFont = m_checkedFonts[filename];
		if (!checkedFont.isOK)
		{
			checkedFont.isChecked = true;
			FT_Done_Face(face);
			return true;
		}
		if (checkedFont.lastMod != foCache.lastMod)
		{
			ScCore->setSplashStatus( QObject::tr("Modified Font found, checking...") );
			FT_UInt gindex = 0;
			FT_ULong charcode = FT_Get_First_Char( face, &gindex );
			while ( gindex != 0 )
			{
				error = FT_Load_Glyph(face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);
				if (error)
				{
					auto errorMessage = QObject::tr("Font %1 has broken glyph %2 (charcode U+%3). Error message: \"%4\"")
								   .arg(filename)
								   .arg(gindex)
								   .arg(charcode, 4, 16, QChar('0'))
								   .arg(getFtError(error));
					addRejectedFont(filename, errorMessage);
					if (m_showFontInfo)
						sDebug(errorMessage);
					FT_Done_Face(face);
					m_checkedFonts.insert(filename, foCache);
					return true;
				}
				FT_Get_Glyph_Name(face, gindex, buf, 128);
				QString newName(buf);
				if (newName == glyName)
				{
					HasNames = false;
					Subset = true;
				}
				glyName = newName;
				charcode = FT_Get_Next_Char( face, charcode, &gindex );
			}
			foCache.isOK = true;
			checkedFont = foCache;
		}
		else
		{
			checkedFont.isOK = true;
			checkedFont.isChecked = true;
		}
	}

	// Warning: code below is also present in loadScalableFont, so if you do
	// any modification here, think also about modifying code in loadScalableFont
	int faceIndex = 0;
	while (!error)
	{
		QString fam(getFamilyName(face));
		QStringList features(getFontFeatures(face));
		QString sty(face->style_name);
		if ((sty == "Regular" && face->style_flags != 0) || sty.isEmpty())
		{
			switch (face->style_flags)
			{
				case 0:
					sty = "Regular";
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
		QString fullName(fam);
		if (!sty.isEmpty())
			fullName += " " + sty;
		const char* psName = FT_Get_Postscript_Name(face);
		QString qpsName;
		if (psName)
			qpsName = QString(psName);
		else
			qpsName = fullName;
		ScFace t;
		if (contains(fullName))
		{
			t = (*this)[fullName];
			if (t.psName() != qpsName)
			{
				QString alt = " (" + qpsName + ")";
				fullName += alt;
				sty += alt;
			}
		}
		t = (*this)[fullName];
		if (t.isNone())
		{
			switch (format) 
			{
				case ScFace::PFA:
					t = ScFace(new ScFace_PFA(fam, sty, "", fullName, qpsName, filename, faceIndex, features));
					t.subset(Subset);
					break;
				case ScFace::PFB:
					t = ScFace(new ScFace_PFB(fam, sty, "", fullName, qpsName, filename, faceIndex, features));
					t.subset(Subset);
					break;
				case ScFace::SFNT:
					t = ScFace(new ScFace_ttf(fam, sty, "", fullName, qpsName, filename, faceIndex, features));
					getSubFontType(face, t.m_m->typeCode);
					t.subset(Subset);
					break;
				case ScFace::TTCF:
					t = ScFace(new ScFace_ttf(fam, sty, "", fullName, qpsName, filename, faceIndex, features));
					t.m_m->formatCode = ScFace::TTCF;
					t.m_m->typeCode = ScFace::TTF;
					getSubFontType(face, t.m_m->typeCode);
					t.subset(Subset);
					break;
				case ScFace::TYPE42:
					t = ScFace(new ScFace_ttf(fam, sty, "", fullName, qpsName, filename, faceIndex, features));
					getSubFontType(face, t.m_m->typeCode);
					t.subset(Subset);
					break;
				default:
				/* catching any types not handled above to silence compiler */
					break;
			}
			insert(fullName, t);
			t.m_m->hasGlyphNames = HasNames;
			t.embedPs(true);
			t.usable(true);
			t.m_m->status = ScFace::UNKNOWN;
			if (face->num_glyphs > 2048)
				t.subset(true);
			t.m_m->forDocument = DocName;
			//setBestEncoding(face); //AV
			if (m_showFontInfo)
				sDebug(QObject::tr("Font %1 loaded from %2(%3)").arg(t.psName(), filename).arg(faceIndex + 1));

/*
//debug
			QByteArray bb;
			t->rawData(bb);
			QFile dump(QString("/tmp/fonts/%1-%2").arg(fullName, psName));
			dump.open(IO_WriteOnly);
			QDataStream os(&dump);
			os.writeRawBytes(bb.data(), bb.size());
			dump.close();
*/
		}
		else 
		{
			if (m_showFontInfo)
				sDebug(QObject::tr("Font %1(%2) is duplicate of %3").arg(filename).arg(faceIndex + 1).arg(t.fontPath()));
			// this is needed since eg. AppleSymbols will happily return a face for *any* face_index
			if (faceIndex > 0) {
				break;
			}
		}
		if ((++faceIndex) >= face->num_faces)
			break;
		FT_Done_Face(face);
		face = nullptr;
		error = FT_New_Face(library, QFile::encodeName(filename), faceIndex, &face);
	} //while
	
	if (face != nullptr)
		FT_Done_Face(face);
	return error && faceIndex == 0;
}

void SCFonts::removeFont(const QString& name)
{
	remove(name);
	updateFontMap();
}

const ScFace& SCFonts::findFont(const QString& fontname, ScribusDoc *doc)
{
	if (fontname.isEmpty())
		return ScFace::none();
	
	PrefsManager& prefsManager = PrefsManager::instance();
	
	if (!contains(fontname) || !(*this)[fontname].usable())
	{
		QString replFont;
		if ((!prefsManager.appPrefs.fontPrefs.GFontSub.contains(fontname)) || (!(*this)[prefsManager.appPrefs.fontPrefs.GFontSub[fontname]].usable()))
		{
			replFont = doc ? doc->itemToolPrefs().textFont : prefsManager.appPrefs.itemToolPrefs.textFont;
		}
		else
			replFont = prefsManager.appPrefs.fontPrefs.GFontSub[fontname];
		ScFace repl = (*this)[replFont].mkReplacementFor(fontname, doc ? doc->documentFileName() : QString());
		insert(fontname, repl);
	}
	else if ( doc && !doc->UsedFonts.contains(fontname) )
	{
		doc->AddFont(fontname, qRound(doc->itemToolPrefs().textSize / 10.0));
	}
	return (*this)[fontname];	
}


const ScFace& SCFonts::findFont(const QString& fontFamily, const QString& fontStyle, ScribusDoc* doc)
{
	if (fontStyle.isEmpty())
		return findFont(fontFamily + " Regular", doc);
	return findFont(fontFamily + " " + fontStyle, doc);
}

QMap<QString,QString> SCFonts::getSubstitutions(const QList<QString>& skip) const
{
	QMap<QString,QString> result;
	for (auto it = begin(); it != end(); ++it)
	{
		if (it.value().isReplacement() && !skip.contains(it.key()))
			result[it.key()] = it.value().replacementName();
	}
	return result;
}


void SCFonts::setSubstitutions(const QMap<QString,QString>& substitutes, ScribusDoc* doc)
{
	for (auto it = substitutes.begin(); it != substitutes.end(); ++it)
	{
		if (it.key() == it.value())
			continue;
		ScFace& font(const_cast<ScFace&>(findFont(it.key(), doc)));
		if (font.isReplacement())
		{
			font.chReplacementTo(const_cast<ScFace&>(findFont(it.value(), doc)), doc->documentFileName());
		}
	}
}


#ifdef HAVE_FONTCONFIG
// Use Fontconfig to locate and load fonts.
void SCFonts::addFontconfigFonts()
{
	// All-in-one library setup. Perhaps this should be in
	// the SCFonts constructor.
	FcConfig* config = FcInitLoadConfigAndFonts();
	// The pattern controls what fonts to match. In this case we want to
	// match all scalable fonts, but ignore bitmap fonts.
	FcPattern* pat = FcPatternBuild(nullptr,
									FC_SCALABLE, FcTypeBool, true,
									nullptr);
	// The ObjectSet tells FontConfig what information about each match to return.
	// We currently just need FC_FILE, but other info like font family and style
	// is available - see "man fontconfig".
	FcObjectSet* os = FcObjectSetBuild (FC_FILE, (char *) nullptr);
	if (!os)
	{
		qFatal("SCFonts::addFontconfigFonts() FcObjectSet* os failed to build object set");
		return;
	}
	// Now ask fontconfig to retrieve info as specified in 'os' about fonts
	// matching pattern 'pat'.
	FcFontSet* fs = FcFontList(config, pat, os);
	if (!fs)
	{
		qFatal("SCFonts::addFontconfigFonts() FcFontSet* fs failed to create font list");
		return;
	}
	FcConfigDestroy(config);
	FcObjectSetDestroy(os);
	FcPatternDestroy(pat);
	// Create the Freetype library
//	bool error;
	FT_Library library = nullptr;
	FT_Init_FreeType( &library );
	// Now iterate over the font files and load them
	for (int i = 0; i < fs->nfont; i++)
	{
		FcChar8 *file = nullptr;
		if (FcPatternGetString (fs->fonts[i], FC_FILE, 0, &file) == FcResultMatch)
		{
			if (m_showFontInfo)
				sDebug(QObject::tr("Loading font %1 (found using fontconfig)").arg(QString((char*)file)));
			addScalableFont(QString((char*)file), library, "");
		}
		else
			if (m_showFontInfo)
			{
				auto errorMessage = QObject::tr("Failed to load a font - freetype2 couldn't find the font file");
				addRejectedFont(QString((char*)file), errorMessage);
				sDebug(errorMessage);
			}
	}
	FT_Done_FreeType(library);
	FcFontSetDestroy(fs);
}

#elif defined(Q_OS_WIN32)

void SCFonts::addRegistryFonts()
{
	const QStringList keys = { QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"),
		                       QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"),
#if defined(Q_OS_WIN64)
	                           QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"),
		                       QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"),
#endif
	                         };
	QSet<QString> foundFonts;

	FT_Library library = nullptr;
	FT_Init_FreeType( &library );

	for (const auto key : keys)
	{
		const QSettings fontRegistry(key, QSettings::NativeFormat);
		const QStringList allKeys = fontRegistry.allKeys();

		int size = allKeys.size();
		for (int i = 0; i < size; ++i)
		{
			const QString &fontKey = allKeys.at(i);
			const QString fileName = fontRegistry.value(fontKey).toString();

			QFileInfo fontInfo(fileName);
			if (fontInfo.isRelative())
				continue; // Font located in system font directory, nothing to do
			if (!fontInfo.exists())
				continue; // Sanity check for broken Symlinks
			QString fontPath = fontInfo.absoluteFilePath();
			QString pathName = fontInfo.absolutePath();

			qApp->processEvents();
			
			bool isSymlink = fontInfo.isSymLink();
			if (isSymlink)
			{
				QFileInfo symlinkInfo(fontInfo.symLinkTarget());
				if (symlinkInfo.isRelative())
					fontPath = pathName + fontInfo.symLinkTarget();
				else
					fontPath = symlinkInfo.absoluteFilePath();
			}

			fontPath = QDir::toNativeSeparators(fontPath);
			if (foundFonts.contains(fontPath))
				continue;

			QFileInfo fontInfo2(fontPath);
			QString ext = fontInfo.suffix().toLower();
			QString ext2 = fontInfo2.suffix().toLower();
			if ((ext != ext2) && (ext.isEmpty())) 
				ext = ext2;
			if ((ext == "ttc") || (ext == "dfont") || (ext == "pfa") || (ext == "pfb") || (ext == "ttf") || (ext == "otf"))
			{
				foundFonts.insert(fontPath);
				addScalableFont(fontPath, library, QString());
			}
		}
	}

	FT_Done_FreeType(library);
}

void SCFonts::addType1RegistryFonts()
{
	const QStringList keys = { QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Type 1 Installer\\Type 1 Fonts"),
		                       QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Type 1 Installer\\Type 1 Fonts"),
#if defined(Q_OS_WIN64)
	                           QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Type 1 Installer\\Type 1 Fonts"),
		                       QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Type 1 Installer\\Type 1 Fonts"),
#endif
	                         };
	QSet<QString> foundFonts;

	FT_Library library = nullptr;
	FT_Init_FreeType( &library );

	for (const auto key : keys)
	{
		const QSettings fontRegistry(key, QSettings::NativeFormat);
		const QStringList allKeys = fontRegistry.allKeys();

		int size = allKeys.size();
		for (int i = 0; i < size; ++i)
		{
			const QString &fontKey = allKeys.at(i);
			const QStringList fileNames = fontRegistry.value(fontKey).toStringList();
			if (fileNames.size() <= 0)
				continue;
			if (fileNames.at(0) != "T")
				continue;

			for (int j = 1; j < fileNames.count(); ++j)
			{
				QString fileName = fileNames.at(j);

				QFileInfo fontInfo(fileName);
				if (fontInfo.isRelative())
					continue; // Font located in system font directory, nothing to do
				if (!fontInfo.exists())
					continue; // Sanity check for broken Symlinks

				QString ext = fontInfo.suffix().toLower();
				if ((ext != "pfa") && (ext != "pfb"))
					continue;

				QString fontPath = fontInfo.absoluteFilePath();
				QString pathName = fontInfo.absolutePath();

				qApp->processEvents();
			
				bool isSymlink = fontInfo.isSymLink();
				if (isSymlink)
				{
					QFileInfo symlinkInfo(fontInfo.symLinkTarget());
					if (symlinkInfo.isRelative())
						fontPath = pathName + fontInfo.symLinkTarget();
					else
						fontPath = symlinkInfo.absoluteFilePath();
				}

				fontPath = QDir::toNativeSeparators(fontPath);
				if (foundFonts.contains(fontPath))
					continue;

				QFileInfo fontInfo2(fontPath);
				QString ext2 = fontInfo2.suffix().toLower();
				if ((ext != ext2) && (ext.isEmpty())) 
					ext = ext2;
				if ((ext == "pfa") || (ext == "pfb"))
				{
					foundFonts.insert(fontPath);
					addScalableFont(fontPath, library, QString());
					break;
				}
			}
		}
	}

	FT_Done_FreeType(library);
}

#endif

/* Add an extra path to our font search path,
 * allowing a user to have extra fonts installed
 * only for this user. Can also be used also as an emergency
 * fallback if no suitable fonts are found elsewhere */
void SCFonts::addUserPath(const QString& pf)
{
	PrefsContext *pc = PrefsManager::instance().prefsFile->getContext("Fonts");
	PrefsTable *extraDirs = pc->getTable("ExtraFontDirs");
	for (int i = 0; i < extraDirs->getRowCount(); ++i)
		addPath(extraDirs->get(i, 0));
}

void SCFonts::readFontCache(const QString& pf)
{
	QFile fr(pf + "/cfonts.xml");
	QFileInfo fir(fr);
	if (fir.exists())
		fr.remove();
	m_checkedFonts.clear();
	struct testCache foCache;
	QDomDocument docu("fontcacherc");
	QFile f(pf + "/checkfonts150.xml");
	if (!f.open(QIODevice::ReadOnly))
		return;
	ScCore->setSplashStatus( QObject::tr("Reading Font Cache") );
	QTextStream ts(&f);
	ts.setCodec("UTF-8");
	QString errorMsg;
	int errorLine = 0, errorColumn = 0;
	if ( !docu.setContent(ts.readAll(), &errorMsg, &errorLine, &errorColumn) )
	{
		f.close();
		return;
	}
	f.close();
	QDomElement elem = docu.documentElement();
	if (elem.tagName() != "CachedFonts")
		return;
	QDomNode DOC = elem.firstChild();
	while (!DOC.isNull())
	{
		QDomElement dc = DOC.toElement();
		if (dc.tagName()=="Font")
		{
			foCache.isChecked = false;
			foCache.isOK = static_cast<bool>(dc.attribute("Status", "1").toInt());
			foCache.lastMod = QDateTime::fromString(dc.attribute("Modified"), Qt::ISODate);
			m_checkedFonts.insert(dc.attribute("File"), foCache);
		}
		DOC = DOC.nextSibling();
	}
}

void SCFonts::writeFontCache()
{
	QString prefsLocation = PrefsManager::instance().preferencesLocation();
	writeFontCache(prefsLocation);
}

void SCFonts::writeFontCache(const QString& pf)
{
	QDomDocument docu("fontcacherc");
	QString st="<CachedFonts></CachedFonts>";

	docu.setContent(st);
	QDomElement elem = docu.documentElement();
	for (auto it = m_checkedFonts.cbegin(); it != m_checkedFonts.cend(); ++it)
	{
		const auto& checkedFont = it.value();

		bool saveItem = checkedFont.isChecked;
		if (!checkedFont.isChecked) // Font might be located in another local Scribus font folder
			saveItem = QFile::exists(it.key());
		if (!saveItem)
			continue;

		QDomElement fosu = docu.createElement("Font");
		fosu.setAttribute("File", it.key());
		fosu.setAttribute("Status", static_cast<int>(checkedFont.isOK));
		fosu.setAttribute("Modified", checkedFont.lastMod.toString(Qt::ISODate));
		elem.appendChild(fosu);
	}

	ScCore->setSplashStatus( QObject::tr("Writing updated Font Cache") );

	QFile file(pf + "/checkfonts150.xml");
	if (!file.open(QIODevice::WriteOnly))
		return;
	QTextStream s(&file);
	s.setCodec("UTF-8");
	s << docu.toString();
	file.close();
}

void SCFonts::getFonts(const QString& pf, bool showFontInfo)
{
	m_showFontInfo = showFontInfo;
	m_fontPaths.clear();
	readFontCache(pf);
	ScCore->setSplashStatus( QObject::tr("Searching for Fonts") );
	addUserPath(pf);

	// Search the system paths
	QStringList ftDirs = ScPaths::systemFontDirs();
	for (int i = 0; i < ftDirs.count(); i++)
		addScalableFonts( ftDirs[i] );

#ifdef Q_OS_WIN32
	// Search fonts outside system paths using Windows Registry
	addRegistryFonts();
	addType1RegistryFonts();
#endif

	// Search Scribus font path
	if (!ScPaths::instance().fontDir().isEmpty() && QDir(ScPaths::instance().fontDir()).exists())
		addScalableFonts( ScPaths::instance().fontDir() );

	//Add downloaded user fonts
	QString userFontDir(ScPaths::userFontDir(false));
	if (QDir(userFontDir).exists())
		addScalableFonts( userFontDir );

// if fontconfig is there, it does all the work
#if HAVE_FONTCONFIG
	// Search fontconfig paths
	QStringList::iterator fpi, fpend = m_fontPaths.end();
	for (fpi = m_fontPaths.begin() ; fpi != fpend; ++fpi) 
		addScalableFonts(*fpi);
	addFontconfigFonts();
#else
	// add user and X11 fonts:
	QStringList::iterator fpi, fpend = m_fontPaths.end();
	for (fpi = m_fontPaths.begin() ; fpi != fpend; ++fpi) 
		addScalableFonts(*fpi);
#endif
	updateFontMap();
	writeFontCache(pf);
}

void SCFonts::addRejectedFont(const QString& fontPath, const QString& message)
{
	rejectedFonts.insert(fontPath, message);
}
