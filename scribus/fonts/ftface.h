#ifndef FT_FACE_H
#define FT_FACE_H


#include <QString>
//#include <QVector>
#include <QMap>
//#include <QArray>

#include "scribusapi.h"

#include "fonts/scface.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "fpointarray.h"

FT_Error ftIOFunc( FT_Stream stream, unsigned long pos, unsigned char* buffer, unsigned long count);


/*! \brief Base Class FtFace provides an ScFace private implementation
     for Freetype based fonts. Subclasses are ScFace_ps and ScFace_ttf.

Below is the old docs for class Foi:

This is subclassed by a class to handle Type1 fonts, a class
to handle TrueType fonts, and potentially any other type that becomes appropriate in
the future.
Note the virtual destructor, needed to ensure that the correct destructor is called
for subclasses

The RealName field has been changed from a data member to a member function.
This is because the only place the PostScript real name of a font is required is
the printing code, so it's cheaper to extract this information only when it is
required, for just the used fonts, than for every one of potentially hundreds at
application startup!  This also allows for the fact that truetype fonts will require
a different method of extracting their names.

One implication of using a base class/subclass model for fonts:  It is no longer
possible to store the ScFace structures in a QMap.  This is because QMap allocates
its own structures, and copies the supplied data to them.  A QMap<QString,ScFace>
would demote all subclasses to ScFace classes, and hence break the polymorphism.
QDict can be used instead, with very little change to the rest of the code, since
it stores references to the data instead of copying the data.  With AutoDelete set
to true, it will automatically dispose of all data when its destructor is called,
so there are no extra cleaning-up chores to take care of.
*/
struct SCRIBUS_API FtFace : public ScFace::ScFaceData
{

	FtFace(QString fam, QString sty, QString variant, QString scname, 
		   QString psname, QString path, int face);
	
	FT_Face ftFace() const;

	virtual ~FtFace();
	
	// font metrics
	qreal ascent(qreal sz=1.0)    const { return m_ascent * sz; }
	qreal descent(qreal sz=1.0)   const { return m_descent * sz; }
	qreal xHeight(qreal sz=1.0)   const { return m_xHeight * sz; }
	qreal capHeight(qreal sz=1.0) const { return m_capHeight * sz; }
	qreal height(qreal sz=1.0)    const { return m_height * sz; }
	qreal strikeoutPos(qreal sz=1.0)    const { return m_strikeoutPos * sz; }
	qreal underlinePos(qreal sz=1.0)    const { return m_underlinePos * sz; }
	qreal strokeWidth(qreal /*sz*/)     const { return m_strokeWidth; }
	qreal maxAdvanceWidth(qreal sz=1.0) const { return m_maxAdvanceWidth * sz; }
	QString ascentAsString()    const { return Ascent; }
	QString descentAsString()    const { return Descender; }
	QString capHeightAsString()    const { return CapHeight; }
	QString FontBBoxAsString()    const { return FontBBox; }
	QString ItalicAngleAsString()    const { return ItalicAngle; }
	
	
//FIXME	QMap<QString,QString> fontDictionary(qreal sz=1.0)      const;
	
	uint         char2CMap(QChar ch)                         const;

	qreal       glyphKerning (uint gl1, uint gl2, qreal sz) const;
//	GlyphMetrics glyphBBox (uint gl,               qreal sz) const;

	void RawData   (QByteArray & bb)            const;
	bool glyphNames(QMap<uint, std::pair<QChar, QString> >& GList) const;
	void load      ()                           const;
	void unload    ()                           const;
	void loadGlyph (uint ch)                    const;

protected:
	mutable FT_Face m_face;
	
	static FT_Library library;

	mutable QString Ascent;
	mutable QString CapHeight;
	mutable QString Descender;
	mutable QString ItalicAngle;
	mutable QString StdVW;
	QString FontEnc;
	mutable QString FontBBox;

	mutable int m_encoding;
	
	mutable qreal m_uniEM;
	mutable qreal m_ascent;
	mutable qreal m_descent;
	mutable qreal m_height;
	mutable qreal m_xHeight;
	mutable qreal m_capHeight;
	mutable qreal m_maxAdvanceWidth;
	mutable qreal m_underlinePos;
	mutable qreal m_strikeoutPos;
	mutable qreal m_strokeWidth;
	
};

#endif
