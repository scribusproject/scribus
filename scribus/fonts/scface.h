/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SC_FACE_H
#define SC_FACE_H

/* ScFace responsibilities:

font storage: type, format, filepath, index, document-local, substitute, etc.
usage:        use, embed, subset, ...
face info:    family, effect, alternative, flags, charset
encoding:     CMap2String, glyphnames, 
metrics:      cwidth, bearing, bbox, "real" widths, paths
opentype:     apply features, script support (-)
embedding:    fontdictionary, rawdata, embedPS, embedPDF, subsetPS, subsetPDF
virtual:      dispatch to constituents, handle embedding (-)
*/

#include <QHash>
#include <QMap>
#include <QString>
#include <QStringList>
#include <utility>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "fpointarray.h"

class CharStyle;

struct GlyphMetrics {
	qreal width;
	qreal ascent;
	qreal descent;
};



/*! \brief Base Class ScFace : This is a total rewrite of the old Foi class.

It uses a shared private implementation which must be a subclass of ScFontData.
ScFace objects are quite small and can be handled like value objects. Reference
counting ensures that the shared data is freed when the last ScFace object is
destructed.

ScFaceData has caches for face and glyph data. load() fills those caches for
face data, loadGlyph() fills the cache for glyphs. caches are always filled by
need, so you can call unload() any time (well, better not multithreaded...)
without producing errors. the increaseUsage() and decreaseUsage() keep track
of at how many places a face is used and automatically unload when the count 
reaches zero.
Other data is recalculated on demand. The implementation can choose to do its
own caching for this data.

ScFace uses a sophisticated lifecycle which controls the state of initialisation
and the performed checks. the normal lilecycle goes:

UNKNOWN  --load()-->  LOADED  --checkAllGlyphs()--> CHECKED

If the face can not be loaded, the status changes to BROKEN.
If only some glyphs can not be loaded, the status changes to 
BROKENGLYPHS. These fonts can still be used  and subsetted(outlined),
but not embedded.

ScFace constructors are only accessible to the ScFonts class which does
some checking before creating a new ScFace. These checks are recorded
in a cache and only reexecuted if the font file changes.
The field 'cachedStatus' allows load() to switch directly to one of the
states BROKEN, CHECKED or BROKENGLYPHS if this state was reached during a
previous run of Scribus.
*/
class SCRIBUS_API ScFace
{
public:
	enum Status  { UNKNOWN, LOADED, CHECKED, BROKENGLYPHS, BROKEN, NULLFACE };
	enum FontType { TYPE0, TYPE1, TYPE3, TTF, CFF, OTF, UNKNOWN_TYPE };
	enum FontFormat { PFA, PFB, TYPE2, TYPE42,
		// handled by freetype:	PFB_MAC, DFONT, HQX, MACBIN,
		SFNT, TTCF, UNKNOWN_FORMAT };

	typedef uint gid_type;
	typedef uint ucs4_type;
	struct GlyphEncoding
	{
		GlyphEncoding() { charcode = 0; toUnicode = "0000"; }
		ucs4_type charcode;
		QString   glyphName;
		QString   toUnicode;
	};
	typedef QMap<gid_type, GlyphEncoding> FaceEncoding;

	static const gid_type CONTROL_GLYPHS = 2000000000; // 2 billion

	struct GlyphData { 
		FPointArray Outlines;
		qreal x;
		qreal y;
		qreal bbox_width;
		qreal bbox_ascent;
		qreal bbox_descent;
		bool broken;
		GlyphData() : Outlines(), x(0), y(0), bbox_width(1), bbox_ascent(1), bbox_descent(0), broken(true) {}
	};

    
	/// see accessors for ScFace for docs
	class ScFaceData {
	public:
		/// controls destruction
		mutable int refs;
		/// controls load()
		mutable int usage;

		QString scName;
		QString fontFile;
		int     faceIndex;
		QString psName;
		QString family;
		QString style;
		QString variant;
		QStringList fontFeatures;

		QString forDocument;

		mutable ScFace::Status status;
		ScFace::FontType typeCode;
		ScFace::FontFormat formatCode;

		bool usable;
		bool embedPs;
		bool subset;
		bool outline;

		bool isCIDFont;
		bool isStroked;
		bool isFixedPitch;
		bool hasGlyphNames;
		gid_type maxGlyph;

		ScFaceData();
		virtual ~ScFaceData();
	protected:

		friend class ScFace;
		Status m_cachedStatus;

		// caches
		mutable QHash<gid_type, qreal>     m_glyphWidth;
		mutable QHash<gid_type, GlyphData> m_glyphOutline;
		//mutable QHash<gid_type, uint>      m_cMap;
		void* m_hbFont;

		// fill caches & members

		virtual void load()             const 
		{ 
			m_glyphWidth.clear();
			m_glyphOutline.clear();
			//m_cMap.clear();

			status = qMax(m_cachedStatus, ScFace::LOADED);
		}

		virtual void unload()           const 
		{
			m_glyphWidth.clear();
			m_glyphOutline.clear();
			//m_cMap.clear();

			status = ScFace::UNKNOWN;
		}

		virtual void loadGlyph(gid_type /*gl*/) const {}

		// dummy implementations
		virtual qreal ascent(qreal sz)           const { return sz; }
		virtual QString pdfAscentAsString()      const { return "0" ; }
		virtual QString pdfDescentAsString()     const { return "0"; }
		virtual QString pdfCapHeightAsString()   const { return "0"; }
		virtual QString pdfFontBBoxAsString()    const { return "0 0 0 0"; }
		virtual QString italicAngleAsString()    const { return "0"; }
		virtual qreal descent(qreal /*sz*/)      const { return 0.0; }
		virtual qreal xHeight(qreal sz)          const { return sz; }
		virtual qreal capHeight(qreal sz)        const { return sz; }
		virtual qreal height(qreal sz)           const { return sz; }
		virtual qreal strikeoutPos(qreal sz)     const { return sz / 2; }
		virtual qreal underlinePos(qreal /*sz*/)     const { return -1.0; }
		virtual qreal strokeWidth(qreal /*sz*/)      const { return 0.1; }
		virtual qreal maxAdvanceWidth(qreal sz)  const { return sz; }
		virtual gid_type char2CMap(uint /*ch*/)         const { return 0; }
		virtual QMap<QString,QString> fontDictionary(qreal sz=1.0) const;
		virtual GlyphMetrics glyphBBox(gid_type gl, qreal sz) const;
		virtual bool embedFont(QByteArray &/*str*/)       const { return false; }
		virtual void rawData(QByteArray & /*bb*/)      const {}
		virtual FT_Face ftFace() const { return 0; }
		virtual void* hbFont();

		virtual bool isItalic() const { return false; }
		virtual bool isBold()   const { return false; }

		virtual bool isCIDKeyed() const { return isCIDFont; }
		virtual bool hasNames() const { return hasGlyphNames; }
		virtual bool glyphNames(ScFace::FaceEncoding& gList) const;

		// these use the cache:
		virtual qreal       glyphWidth(gid_type gl, qreal sz)   const;
		virtual FPointArray glyphOutline(gid_type gl, qreal sz) const;
		virtual FPoint      glyphOrigin (gid_type gl, qreal sz) const;

		virtual bool isSymbolic() const { return false; }
	};



	ScFace();
	ScFace(const ScFace& other);
	~ScFace();

	/// used as a null object
	static const ScFace& none();

	/// test for null object
	bool isNone() const   { return m_m->status == NULLFACE; }

	/// test if font is a symbolic font
	bool isSymbolic() const;

	ScFace& operator=(const ScFace& other);
	/** two ScFaces are equal if they either are both NULLFACEs or they
		agree on family, style, variant and fontpath
		*/
	bool operator==(const ScFace& other) const ;
	bool operator!=(const ScFace& other) const { return ! (*this == other); }


	bool embedFont(QByteArray &str);
	void rawData(QByteArray & bb);
	bool glyphNames(FaceEncoding& gList);

	/// prevent unloading of face data
	void increaseUsage() const;

	/// unload face data if not used any more
	void decreaseUsage() const;

	/// unload face data. It will be reloaded on need
	void unload()      const;

	/// the name Scribus uses for this font
	QString scName()   const { return m_replacedName.isEmpty() ? m_m->scName : m_replacedName; }

	/// the name of the font which was used for replacement
	QString replacementName()   const { return m_m->scName; }

	/// the name of the font which was used for replacement
	QString replacementForDoc()   const { return m_replacedInDoc; }

	/// check if this is a replacement font
	bool isReplacement()   const { return !m_replacedName.isEmpty(); }

	/// makes a repalcement font for font "name" using this fonts data
	ScFace mkReplacementFor(QString name, QString doc) { 
		ScFace result(m_m);
		result.m_replacedName = name;
		result.m_replacedInDoc = doc;
		return result; 
	}

	void chReplacementTo(ScFace& other, QString doc) { 
		QString oldName = m_replacedName;
		(*this) = other;
		m_replacedName = oldName;
		m_replacedInDoc = doc;
	}

	/// the name PostScript uses for this font
	QString psName()   const { return m_m->psName; }

	/// the physical location of the fontfile
	QString fontPath() const { return m_m->faceIndex >= 0 ? QString("%1(%2)").arg(m_m->fontFile).arg(m_m->faceIndex+1) : m_m->fontFile; }

	/// the file path of the fontfile
	QString fontFilePath()      const { return m_m->fontFile; }

	/// if the fontfile contains more than one face, the index, else -1
	int faceIndex()    const { return m_m->faceIndex; }

	/// a HarfBuzz font for this font
	void* hbFont() const { return m_m->hbFont(); }

	/// path name of the document this face is local to
	QString localForDocument()  const { return m_m->forDocument; }

	/// font type, eg. Type1 or TTF
	FontType type()    const { return m_m->typeCode; }

	/// font format, which might be a little more complicated
	FontFormat format()const { return m_m->formatCode; }

	/// test if this face can be used in documents
	bool usable()      const { return m_m->usable && !isNone(); }

    /// test if this face should be outlined in documents
	bool outline()      const { return usable() && m_m->outline; }
    
	/// test if this face can be embedded in PS/PDF
	bool embedPs()     const { return m_m->embedPs && m_m->status < BROKENGLYPHS; }

	/// test if this face can be embedded as outlines in PS/PDF
	bool subset()      const { return m_m->subset && m_m->status < BROKEN; }

	void usable(bool flag)   { m_m->usable = flag; }
	void embedPs(bool flag)  { m_m->embedPs = flag; }
	void subset(bool flag)   { m_m->subset = flag; }
	void outline(bool flag)   { m_m->outline = flag; }

	/// deprecated? tells if the face has PS names
	bool hasNames()    const { return m_m->hasNames(); }

	/// tells if the font is a CID font
	bool isCIDKeyed() const { return m_m->isCIDFont; }

	/// tells if this font is an outline font
	bool isStroked()   const { return m_m->isStroked; }

	/// tells if this font is a fixed pitch font
	bool isFixedPitch()const { return m_m->isFixedPitch; }

	/// tells if this is an OTF/CFF font
	bool isOTF()       const { return m_m->typeCode == OTF; }

	/// returns the highest glyph index in this face
	gid_type maxGlyph() const { return m_m->maxGlyph; }

	/// returns the font family as seen by Scribus
	QString family()   const { return m_m->family; }

	/// returns the font style as seen by Scribus (eg. bold, Italic)
	QString style()    const { return m_m->style; }

	/// returns an additional discriminating String for this face
	QString variant()  const { return m_m->variant; }

	// font metrics
	QString pdfAscentAsString()      const;
	QString pdfDescentAsString()     const;
	QString pdfCapHeightAsString()   const;
	QString pdfFontBBoxAsString()    const;
	QString italicAngleAsString()    const;
	qreal ascent(qreal sz=1.0)          const;
	qreal descent(qreal sz=1.0)         const;
	qreal xHeight(qreal sz=1.0)         const;
	qreal capHeight(qreal sz=1.0)       const;
	qreal height(qreal sz=1.0)          const;
	qreal strikeoutPos(qreal sz=1.0)    const;
	qreal underlinePos(qreal sz=1.0)    const;
	qreal strokeWidth(qreal sz=1.0)     const;
	qreal maxAdvanceWidth(qreal sz=1.0) const;

	bool isItalic() const;
	bool isBold()   const;

//	/// deprecated
//	QString stemV(qreal sz=1.0)    const { return fontDictionary(sz)["/StemV"]; }

//	/// deprecated
//	QString italicAngle(qreal sz=1.0)      const { return fontDictionary(sz)["/ItalicAngle"]; }

//	/// deprecated
//	QString fontBBox(qreal sz=1.0)         const { return fontDictionary(sz)["/FontBBox"]; }

	/// returns a map of values used for font dictionaries in PS/PDF
	QMap<QString,QString> fontDictionary(qreal sz=1.0) const { return m_m->fontDictionary(sz); }
	// glyph interface

	/// returns the glyphs normal advance width at size 'sz'
	qreal glyphWidth(gid_type gl, qreal sz=1.0) const { return m_m->glyphWidth(gl, sz); }

	/// returns the glyphs bounding box at size 'sz', ie. the area where this glyph will produce marks
	GlyphMetrics glyphBBox(gid_type gl, qreal sz=1.0) const { return m_m->glyphBBox(gl, sz); }

	/// returns the glyph's outline as a cubic Bezier path
	FPointArray glyphOutline(gid_type gl, qreal sz=1.0) const { return m_m->glyphOutline(gl, sz); }

	/// returns the glyph's origin FIXME: what's that exactly?
	FPoint glyphOrigin(gid_type gl, qreal sz=1.0)    const { return m_m->glyphOrigin(gl, sz); }

	// char interface

	/// test if the face can render this char
	bool canRender(QChar ch)   const;

	/// translate unicode to glyph index
	gid_type char2CMap(uint ch)   const;

	gid_type emulateGlyph(uint u) const;

	gid_type hyphenGlyph() const;
	gid_type hyphenGlyph(const CharStyle& style) const;
	double hyphenWidth(const CharStyle& style, qreal size=1.0) const;
	QStringList fontFeatures() const;

private:

	friend class SCFonts;

	ScFace(ScFaceData* md);
	ScFaceData* m_m;
	QString m_replacedName;
	QString m_replacedInDoc;

	void initFaceData();
	void checkAllGlyphs();
};

#endif
