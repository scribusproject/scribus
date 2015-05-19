//
//  sfnt.h
//  Scribus
//
//  Created by Andreas Vox on 18.04.15.
//
//

#ifndef Scribus_sfnt_h
#define Scribus_sfnt_h

#include "scribusapi.h"

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QString>

#include <ft2build.h>
#include FT_FREETYPE_H



namespace sfnt {
    
    uchar byte(QByteArray const & bb, uint pos);
    quint32  word(QByteArray const & bb, uint pos);
    void  putWord(QByteArray & bb, uint pos, quint32 val);
    void  appendWord(QByteArray& bb, quint32 val);
    quint16  word16(QByteArray const & bb, uint pos);
    void  putWord16(QByteArray & bb, uint pos, quint16 val);
    void  appendWord16(QByteArray& bb, quint16 val);
    bool  copy(QByteArray & dst, uint to, const QByteArray & src, uint from, uint len);
    const QByteArray tag(QByteArray const & bb, uint pos);
    const QByteArray getTable(const QByteArray& ttf, const QByteArray& ttfTag);
    

    QByteArray subsetFace(const QByteArray& ttf, QList<uint>& glyphs);
    QByteArray extractFace(const QByteArray& ttfColl, int faceIndex);

/**
 This class checks the post table of a ttf font.
 */
class SCRIBUS_API PostTable {
public:
    bool usable;
    QString errorMsg;
    uint numberOfGlyphs() const;
    QString nameFor(uint glyphId) const;
    void readFrom(FT_Face face);
private:
    QList<QString> names;
};

} //namespace


/**
	An object holding a table of kerning pairs extracted from
	a kern feature such as found in a GPOS table
 */
class SCRIBUS_API KernFeature
{
    typedef QMap<quint16, QList<quint16> > ClassDefTable; // <Class index (0 to N) , list of glyphs >
    
public:
    /**
     * Build a ready-to-use kerning pairs table
     * @param face a valid FT_Face, It won’t be store by KernFeature
     */
    KernFeature ( FT_Face face );
    KernFeature ( const KernFeature& kf );
    ~KernFeature();
    
    /**
     * Get the kerning value for a pair of glyph indexes.
     * @param glyph1 Index of the left glyph in logical order
     * @param glyph2 Index of the right glyph in logical order
     * @return the unscaled delta to apply to xadvance of the first glyph
     */
    double getPairValue ( unsigned int glyph1, unsigned int glyph2 ) const;
    
    /**
     * The table can have been invalidated if something went wrong at any moment.
     * @return True if valid, False otherwise.
     */
    bool isValid() const {return m_valid;}
    
private:
    bool m_valid;
    QByteArray GPOSTableRaw;
    QMap<quint16,QList<quint16> > coverages;
    mutable QMap<quint16, QMap<quint16, double> > pairs;
    QMap< quint16, QMap<quint16, ClassDefTable> > classGlyphFirst; // < subtable offset, map<offset, class definition table> > for first glyph
    QMap< quint16, QMap<quint16, ClassDefTable> > classGlyphSecond; // < subtable offset, map<offset, class definition table> > for second glyph
    QMap< quint16, QMap<int, QMap<int, double> > > classValue; // < subtable offset, map<class1, map<class2, value> > >
    
    void makeCoverage();
    void makePairs ( quint16 subtableOffset );
    
    ClassDefTable getClass (bool leftGlyph, quint16 classDefOffset, quint16 coverageId );
    inline quint16 toUint16 ( quint16 index );
    inline qint16 toInt16 ( quint16 index );
    
    QString FontName;// for debugging purpose
};


#endif

