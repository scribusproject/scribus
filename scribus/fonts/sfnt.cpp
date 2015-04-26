//
//  sfnt.cpp
//  Scribus
//
//  Created by Andreas Vox on 18.04.15.
//
//

#include "fonts/sfnt.h"

#include FT_TRUETYPE_TABLES_H
#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_IDS_H

#include <QDebug>

namespace sfnt {
    
    uchar byte(QByteArray const & bb, uint pos)
    {
        const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
        return pp[0];
    }
    
    uint word(QByteArray const & bb, uint pos)
    {
        const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
        return pp[0] << 24 | pp[1] << 16 | pp[2] << 8 | pp[3];
    }
    
    void putWord(QByteArray & bb, uint pos, uint val)
    {
        unsigned char * pp = reinterpret_cast<unsigned char*>(bb.data()) + pos;
        *pp++ = (val >> 24) & 0xFF;
        *pp++ = (val >> 16) & 0xFF;
        *pp++ = (val >> 8) & 0xFF;
        *pp++ = (val) & 0xFF;
    }
    
    uint word16(QByteArray const & bb, uint pos)
    {
        const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
        return pp[0] << 8 | pp[1];
    }
    
    QByteArray tag(QByteArray const & bb, uint pos)
    {
        char buf[5] = "1234";
        buf[0] = bb.data()[pos];
        buf[1] = bb.data()[pos+1];
        buf[2] = bb.data()[pos+2];
        buf[3] = bb.data()[pos+3];
        return buf;
    }
    
    bool copy(QByteArray & dst, uint to, QByteArray & src, uint from, uint len)
    {
        if (!dst.data())
            return false;
        if (!src.data())
            return false;
        if (to + len > static_cast<uint>(dst.size()))
            return false;
        if (from + len > static_cast<uint>(src.size()))
            return false;
        
        memcpy(dst.data() + to, src.data() + from, len);
        return true;
    }
    


enum post_format {
    post_format10 = 0x00010000,
    post_format20 = 0x00020000,
    post_format25 = 0x00025000,
    post_format30 = 0x00030000,
    post_format40 = 0x00040000,
};

    
const uint post_format10_names_count = 258;

    
static const char* post_format10_names[] = {
    ".notdef",
    ".null",
    "nonmarkingreturn",
    "space",
    "exclam",
    "quotedbl",
    "numbersign",
    "dollar",
    "percent",
    "ampersand",
    "quotesingle",
    "parenleft",
    "parenright",
    "asterisk",
    "plus",
    "comma",
    "hyphen",
    "period",
    "slash",
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
    "colon",
    "semicolon",
    "less",
    "equal",
    "greater",
    "question",
    "at",
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    "bracketleft",
    "backslash",
    "bracketright",
    "asciicircum",
    "underscore",
    "grave",
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "braceleft",
    "bar",
    "braceright",
    "asciitilde",
    "Adieresis",
    "Aring",
    "Ccedilla",
    "Eacute",
    "Ntilde",
    "Odieresis",
    "Udieresis",
    "aacute",
    "agrave",
    "acircumflex",
    "adieresis",
    "atilde",
    "aring",
    "ccedilla",
    "eacute",
    "egrave",
    "ecircumflex",
    "edieresis",
    "iacute",
    "igrave",
    "icircumflex",
    "idieresis",
    "ntilde",
    "oacute",
    "ograve",
    "ocircumflex",
    "odieresis",
    "otilde",
    "uacute",
    "ugrave",
    "ucircumflex",
    "udieresis",
    "dagger",
    "degree",
    "cent",
    "sterling",
    "section",
    "bullet",
    "paragraph",
    "germandbls",
    "registered",
    "copyright",
    "trademark",
    "acute",
    "dieresis",
    "notequal",
    "AE",
    "Oslash",
    "infinity",
    "plusminus",
    "lessequal",
    "greaterequal",
    "yen",
    "mu",
    "partialdiff",
    "summation",
    "product",
    "pi",
    "integral",
    "ordfeminine",
    "ordmasculine",
    "Omega",
    "ae",
    "oslash",
    "questiondown",
    "exclamdown",
    "logicalnot",
    "radical",
    "florin",
    "approxequal",
    "Delta",
    "guillemotleft",
    "guillemotright",
    "ellipsis",
    "nonbreakingspace",
    "Agrave",
    "Atilde",
    "Otilde",
    "OE",
    "oe",
    "endash",
    "emdash",
    "quotedblleft",
    "quotedblright",
    "quoteleft",
    "quoteright",
    "divide",
    "lozenge",
    "ydieresis",
    "Ydieresis",
    "fraction",
    "currency",
    "guilsinglleft",
    "guilsinglright",
    "fi",
    "fl",
    "daggerdbl",
    "periodcentered",
    "quotesinglbase",
    "quotedblbase",
    "perthousand",
    "Acircumflex",
    "Ecircumflex",
    "Aacute",
    "Edieresis",
    "Egrave",
    "Iacute",
    "Icircumflex",
    "Idieresis",
    "Igrave",
    "Oacute",
    "Ocircumflex",
    "apple",
    "Ograve",
    "Uacute",
    "Ucircumflex",
    "Ugrave",
    "dotlessi",
    "circumflex",
    "tilde",
    "macron",
    "breve",
    "dotaccent",
    "ring",
    "cedilla",
    "hungarumlaut",
    "ogonek",
    "caron",
    "Lslash",
    "lslash",
    "Scaron",
    "scaron",
    "Zcaron",
    "zcaron",
    "brokenbar",
    "Eth",
    "eth",
    "Yacute",
    "yacute",
    "Thorn",
    "thorn",
    "minus",
    "multiply",
    "onesuperior",
    "twosuperior",
    "threesuperior",
    "onehalf",
    "onequarter",
    "threequarters",
    "franc",
    "Gbreve",
    "gbreve",
    "Idotaccent",
    "Scedilla",
    "scedilla",
    "Cacute",
    "cacute",
    "Ccaron",
    "ccaron",
    "dcroat"
};



int PostTable::numberOfGlyphs() const
{
    if (names.length() > 0)
        return names.length();
    else
        return sfnt::post_format10_names_count;
}

QString PostTable::nameFor(uint glyph) const
{
    if (glyph < names.length())
    {
        return names[glyph];
    }
    else if (glyph < sfnt::post_format10_names_count)
    {
        return sfnt::post_format10_names[glyph];
    }
    else
    {
        return ".notdef";
    }
}


void PostTable::readFrom(FT_Face face)
{
    QByteArray postData;
    FT_ULong size = 0;
    int error = FT_Load_Sfnt_Table ( face, TTAG_post , 0, NULL, &size );
//    qDebug() << TTAG_post << error << size;
    if (error || size == 0)
    {
        errorMsg = "no post table";
        usable = false;
        return;
    }
    postData.resize(size);
    error = FT_Load_Sfnt_Table ( face, TTAG_post , 0, reinterpret_cast<FT_Byte*>(postData.data()), &size );
    if (error)
    {
        errorMsg = "can't load post table";
        usable = false;
        return;
    }
    
    switch (sfnt::word(postData, 0))
    {
        case sfnt::post_format10:
            usable = true;
            names.clear();
            return;
        case sfnt::post_format20:
            break;
        case sfnt::post_format30:
            errorMsg = QString("post table has no glyph names");
            usable = false;
            return;
            
        case sfnt::post_format25:
        case sfnt::post_format40:
        default:
            errorMsg = QString("unsupported post format %1").arg(QString::number(sfnt::word(postData,0),16));
            usable = false;
            return;
            
    }
    QMap<QString,uint> usedNames;
    QList<QByteArray> pascalStrings;
    
    const uint post_header_length = 32;
    uint nrOfGlyphs = sfnt::word16(postData, post_header_length);
    uint stringPos = post_header_length + 2 + 2 * nrOfGlyphs;
//   qDebug() << "#glyphs" << nrOfGlyphs << "start strings" << stringPos;
    while (stringPos < postData.size())
    {
        int strLen = sfnt::byte(postData,stringPos);
        ++stringPos;
        pascalStrings.append(postData.mid(stringPos, strLen));
        stringPos += strLen;
//        qDebug() << pascalStrings.length() << pascalStrings[pascalStrings.length()-1];

    }
    uint pos = post_header_length + 2;
    for (int gid = 0; gid < nrOfGlyphs; ++gid)
    {
        uint nameIndex = sfnt::word16(postData, pos);
        pos += 2;
        QString name;
//	qDebug() << "looking up name index " << nameIndex;
        if (nameIndex < sfnt::post_format10_names_count) {
            name = sfnt::post_format10_names[nameIndex];
	}
        else if (nameIndex < pascalStrings.length() + sfnt::post_format10_names_count) {
            name = pascalStrings[nameIndex - sfnt::post_format10_names_count];
        }
	else {
            usable = false;
	    errorMsg = QString("missing name %1 for glyph %2").arg(nameIndex).arg(gid);
            return;
        }
        if (name != ".notdef" && usedNames.contains(name))
        {
            usable = false;
            errorMsg = QString("duplicate name %1 used for glyphs %2 and %3").arg(name).arg(gid).arg(usedNames[name]);
            return;
        }
//	qDebug() << name << "=" << gid;
        usedNames[name] = gid;
        names.append(name);
    }
    errorMsg = "";
    usable = true;
}

} // namespace sfnt


