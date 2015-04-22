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
    
    uchar   byte(QByteArray const & bb, uint pos);
    uint    word(QByteArray const & bb, uint pos);
    void    putWord(QByteArray & bb, uint pos, uint val);
    uint    word16(QByteArray const & bb, uint pos);
    bool    copy(QByteArray & dst, uint to, QByteArray & src, uint from, uint len);
    QByteArray  tag(QByteArray const & bb, uint pos);
    


/**
 This class checks the post table of a ttf font.
 */
class SCRIBUS_API PostTable {
public:
    bool usable;
    QString errorMsg;
    int numberOfGlyphs() const;
    QString nameFor(uint glyphId) const;
    void readFrom(FT_Face face);
private:
    QList<QString> names;
};

} //namespace


#endif
