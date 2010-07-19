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

#include <QString>
//#include <QVector>
#include <QMap>
//#include <QArray>
#include <utility>

#include "fpointarray.h"



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

	static const uint CONTROL_GLYPHS = 2000000000; // 2 billion
	
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

		QString forDocument;

		mutable ScFace::Status status;
		ScFace::FontType typeCode;
		ScFace::FontFormat formatCode;
		
		bool usable;
		bool embedPs;
		bool subset;
		
		bool isStroked;
		bool isFixedPitch;
		bool hasNames;
		uint maxGlyph;

		ScFaceData();
		virtual ~ScFaceData() { };
	protected:
			
		friend class ScFace;
		Status cachedStatus;
		
		// caches
		mutable QMap<uint,qreal>    m_glyphWidth;
		mutable QMap<uint,GlyphData> m_glyphOutline;
		mutable QMap<uint, uint>     m_cMap;
		
		// fill caches & members
		
		virtual void load()             const 
		{ 
			m_glyphWidth.clear();
			m_glyphOutline.clear();
			m_cMap.clear();

			status = qMax(cachedStatus, ScFace::LOADED);
		}
		
		virtual void unload()           const 
		{
			m_glyphWidth.clear();
			m_glyphOutline.clear();
			m_cMap.clear();

			status = ScFace::UNKNOWN;
		}
		
		virtual void loadGlyph(uint /*gl*/) const {}

		// dummy implementations
		virtual qreal ascent(qreal sz)           const { return sz; }
		virtual QString ascentAsString()    const { return "0" ; }
		virtual QString descentAsString()    const { return "0"; }
		virtual QString capHeightAsString()    const { return "0"; }
		virtual QString FontBBoxAsString()    const { return "0 0 0 0"; }
		virtual QString ItalicAngleAsString()    const { return "0"; }
		virtual qreal descent(qreal /*sz*/)          const { return 0.0; }
		virtual qreal xHeight(qreal sz)          const { return sz; }
		virtual qreal capHeight(qreal sz)        const { return sz; }
		virtual qreal height(qreal sz)           const { return sz; }
		virtual qreal strikeoutPos(qreal sz)     const { return sz / 2; }
		virtual qreal underlinePos(qreal /*sz*/)     const { return -1.0; }
		virtual qreal strokeWidth(qreal /*sz*/)      const { return 0.1; }
		virtual qreal maxAdvanceWidth(qreal sz)  const { return sz; }
		virtual uint   char2CMap(QChar /*ch*/)         const { return 0; }
		virtual qreal glyphKerning(uint gl1, uint gl2, qreal sz) const;
		virtual QMap<QString,QString> fontDictionary(qreal sz=1.0) const;
		virtual GlyphMetrics glyphBBox(uint gl, qreal sz) const;
		virtual bool EmbedFont(QString &/*str*/)       const { return false; }
		virtual void RawData(QByteArray & /*bb*/)      const {}
		virtual bool glyphNames(QMap<uint, std::pair<QChar, QString> >& gList) const;

		// these use the cache:
		virtual qreal      glyphWidth(uint gl, qreal sz)   const;
		virtual FPointArray glyphOutline(uint gl, qreal sz) const; 
		virtual FPoint      glyphOrigin (uint gl, qreal sz) const;
		
	};
		
	
	
	ScFace();
	ScFace(const ScFace& other);
	~ScFace();

	/// used as a null object
	static const ScFace& none();

	/// test for null object
	bool isNone() const   { return m->status == NULLFACE; }
	
	ScFace& operator=(const ScFace& other);
	/** two ScFaces are equal if they either are both NULLFACEs or they
		agree on family, style, variant and fontpath
		*/
	bool operator==(const ScFace& other) const ;
	bool operator!=(const ScFace& other) const { return ! (*this == other); }
	
	
	bool EmbedFont(QString &str);
	void RawData(QByteArray & bb);
	bool glyphNames(QMap<uint, std::pair<QChar, QString> >& gList);
	
	/// prevent unloading of face data
	void increaseUsage() const;
	
	/// unload face data if not used any more
	void decreaseUsage() const;
	
	/// unload face data. It will be reloaded on need
	void unload()      const;
	
	/// the name Scribus uses for this font
	QString scName()   const { return replacedName.isEmpty() ? m->scName : replacedName; }
	
	/// the name of the font which was used for replacement
	QString replacementName()   const { return m->scName; }
	
	/// the name of the font which was used for replacement
	QString replacementForDoc()   const { return replacedInDoc; }
	
	/// check if this is a replacement font
	bool isReplacement()   const { return !replacedName.isEmpty(); }
	
	/// makes a repalcement font for font "name" using this fonts data
	ScFace mkReplacementFor(QString name, QString doc) { 
		ScFace result(m); 
		result.replacedName = name; 
		result.replacedInDoc = doc; 
		return result; 
	}

	void chReplacementTo(ScFace& other, QString doc) { 
		QString oldName = replacedName;
		(*this) = other;
		replacedName = oldName;
		replacedInDoc = doc; 
	}
	
	/// the name PostScript uses for this font
	QString psName()   const { return m->psName; }
	
	/// the physical location of the fontfile
	QString fontPath() const { return m->faceIndex >= 0 ? QString("%1(%2)").arg(m->fontFile).arg(m->faceIndex+1) : m->fontFile; }
	
	/// the file path of the fontfile
	QString fontFilePath()      const { return m->fontFile; }
	
	/// if the fontfile contains more than one face, the index, else -1
	int faceIndex()    const { return m->faceIndex; }
	
	/// path name of the document this face is local to
	QString localForDocument()  const { return m->forDocument; }
	
	/// font type, eg. Type1 or TTF
	FontType type()    const { return m->typeCode; }
	
	/// font format, which might be a little more complicated
	FontFormat format()const { return m->formatCode; }
	
	/// test if this face can be used in documents
	bool usable()      const { return m->usable && !isNone(); }
	
	/// test if this face can be embedded in PS/PDF
	bool embedPs()     const { return m->embedPs && m->status < BROKENGLYPHS; }
	
	/// test if this face can be embedded as outlines in PS/PDF
	bool subset()      const { return m->subset && m->status < BROKEN; }
	
	void usable(bool flag)   { m->usable = flag; }
	void embedPs(bool flag)  { m->embedPs = flag; }
	void subset(bool flag)   { m->subset = flag; }
	
	/// deprecated? tells if the face has PS names
	bool hasNames()    const { return m->hasNames; }

	/// tells if this font is an outline font
	bool isStroked()   const { return m->isStroked; }

	/// tells if this font is a fixed pitch font
	bool isFixedPitch()const { return m->isFixedPitch; }
	
	/// tells if this is an OTF/CFF font
	bool isOTF()       const { return m->typeCode == OTF; }
		
	/// returns the highest glyph index in this face
	uint maxGlyph()    const { return m->maxGlyph; }
	
	/// returns the font family as seen by Scribus
	QString family()   const { return m->family; }
	
	/// returns the font style as seen by Scribus (eg. bold, Italic)
	QString style()    const { return m->style; }
	
	/// returns an additional discriminating String for this face
	QString variant()  const { return m->variant; }
	
	// font metrics
	QString ascentAsString()      const;
	QString descentAsString()     const;
	QString capHeightAsString()   const;
	QString fontBBoxAsString()    const;
	QString italicAngleAsString() const;
	qreal ascent(qreal sz=1.0)          const;
	qreal descent(qreal sz=1.0)         const;
	qreal xHeight(qreal sz=1.0)         const;
	qreal capHeight(qreal sz=1.0)       const;
	qreal height(qreal sz=1.0)          const;
	qreal strikeoutPos(qreal sz=1.0)    const;
	qreal underlinePos(qreal sz=1.0)    const;
	qreal strokeWidth(qreal sz=1.0)     const;
	qreal maxAdvanceWidth(qreal sz=1.0) const;
	
	/// deprecated
	QString stemV(qreal sz=1.0)    const { return fontDictionary(sz)["/StemV"]; }
	
	/// deprecated
	QString italicAngle(qreal sz=1.0)      const { return fontDictionary(sz)["/ItalicAngle"]; }
	
	/// deprecated
	QString fontBBox(qreal sz=1.0)         const { return fontDictionary(sz)["/FontBBox"]; }

	/// returns a map of values used for font dictionaries in PS/PDF
	QMap<QString,QString> fontDictionary(qreal sz=1.0) const { return m->fontDictionary(sz); }	
	// glyph interface
	
	/// returns the glyphs normal advance width at size 'sz'
	qreal glyphWidth(uint gl, qreal sz=1.0) const { return m->glyphWidth(gl, sz); }

	/// returns the glyph kerning between 'gl1' and 'gl2' at size 'sz'
	qreal glyphKerning(uint gl1, uint gl2, qreal sz=1.0) const { return qMax(gl1,gl2) < CONTROL_GLYPHS ? m->glyphKerning(gl1, gl2, sz) : 0; } 

	/// returns the glyphs bounding box at size 'sz', ie. the area where this glyph will produce marks
	GlyphMetrics glyphBBox(uint gl, qreal sz=1.0) const { return m->glyphBBox(gl, sz); }

	/// returns the glyph's outline as a cubic Bezier path
	FPointArray glyphOutline(uint gl, qreal sz=1.0) const { return m->glyphOutline(gl, sz); }

	/// returns the glyph's origin FIXME: what's that exactly?
	FPoint glyphOrigin(uint gl, qreal sz=1.0)    const { return m->glyphOrigin(gl, sz); }
	
	// char interface

	/// test if the face can render this char
	bool canRender(QChar ch)   const;

	/// translate unicode to glyph index
	uint char2CMap(QChar ch)   const;

	/// returns the combined glyph width and kerning for 'ch' if followed by 'ch2'
	qreal charWidth(QChar ch, qreal sz=1.0, QChar ch2 = QChar(0)) const;
	
	/// deprecated, see glyphBBox()
	qreal realCharWidth(QChar ch, qreal sz=1.0) const { return glyphBBox(char2CMap(ch),sz).width; }
	
	/// deprecated, see glyphBBox()
	qreal realCharHeight(QChar ch, qreal sz=1.0) const { GlyphMetrics gm=glyphBBox(char2CMap(ch),sz); return gm.ascent + gm.descent; }
	
	/// deprecated, see glyphBBox()
	qreal realCharAscent(QChar ch, qreal sz=1.0) const { return glyphBBox(char2CMap(ch),sz).ascent; }
	
	/// deprecated, see glyphBBox()
	qreal realCharDescent(QChar ch, qreal sz=1.0) const { return glyphBBox(char2CMap(ch),sz).descent; }
	
private:
		
	friend class SCFonts;
	
	ScFace(ScFaceData* md);
	ScFaceData* m;
	QString replacedName;
	QString replacedInDoc;
	
	void initFaceData();
	void checkAllGlyphs();
	uint emulateGlyph(QChar c) const;
};

#endif
