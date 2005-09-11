
#include <qfile.h>
#include <qstring.h>
#include <qobject.h>

#include <sys/types.h>

#include "scfonts_ttf.h"
#include "scfontmetrics.h"
#include "util.h"
#include "scconfig.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
#include FT_INTERNAL_STREAM_H

QString Foi_ttf::RealName()
{
	return(cached_RealName);
}

bool Foi_ttf::ReadMetrics()
{
	Ascent = "0";
	CapHeight = "0";
	Descender = "0";
	ItalicAngle = "0";
	StdVW = "1";
	FontBBox = "0 0 0 0";
	IsFixedPitch = false;
	if(metricsread)
		return(true);
	CharWidth.clear();
	GlyphArray.clear();
	isStroked = false;
	QString tmp, tmp2, tmp3, tmp4;
	bool			error;
	FT_Library library;
	FT_ULong  charcode;
	FT_UInt   gindex;
	FT_Face   face;
	FPointArray outlines;
	double x, y;
	struct GlyphR GRec;
	char *buf[50];
	QString glyName = "";
	error = FT_Init_FreeType( &library );
	if (error)
	{
		UseFont = false;
		sDebug(QObject::tr("Font %1 is broken (FreeType), discarding it").arg(Datei));
		return false;
	}
	error = FT_New_Face( library, Datei, faceIndex, &face );
	if (error)
	{
		UseFont = false;
		sDebug(QObject::tr("Font %1 is broken (no Face), discarding it").arg(Datei));
		return false;
	}
	uniEM = static_cast<double>(face->units_per_EM);
	HasKern = FT_HAS_KERNING(face);
	HasNames = FT_HAS_GLYPH_NAMES(face);
	Ascent = tmp.setNum(face->ascender * 1000 / uniEM);
	Descender = tmp.setNum(face->descender * 1000 / uniEM);
	CapHeight = Ascent;
	numDescender = face->descender / uniEM;
	numAscent = face->ascender / uniEM;
	underline_pos = face->underline_position / uniEM;
	strikeout_pos = numAscent / 3;
	strokeWidth = face->underline_thickness / uniEM;
	ItalicAngle = "0";
	StdVW = "1";
	FontBBox = tmp.setNum(face->bbox.xMin * 1000 / uniEM)+" "+tmp2.setNum(face->bbox.yMin * 1000 / uniEM)+" "+tmp3.setNum(face->bbox.xMax * 1000 / uniEM)+" "+tmp4.setNum(face->bbox.yMax * 1000 / uniEM);
	IsFixedPitch = face->face_flags & 4;
	setBestEncoding(face);
	gindex = 0;
	charcode = FT_Get_First_Char( face, &gindex );
	int invalidGlyphs = 0;
	int goodGlyphs = 0;
	while ( gindex != 0 )
	{
		error = FT_Load_Glyph( face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
		if (error)
		{
			++invalidGlyphs;
			sDebug(QObject::tr("Font %1  has invalid glyph %2 (charcode %3), discarding it").arg(Datei).arg(gindex).arg(charcode));
			charcode = FT_Get_Next_Char( face, charcode, &gindex );
			continue;
		}
		++goodGlyphs;
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
/* The following lines are a check for some weired fonts which have invalid "post" tables */
		FT_Get_Glyph_Name(face, gindex, buf, 50);
		QString newName = QString(reinterpret_cast<char*>(buf));
		if (newName == glyName)
			HasNames = false;
		glyName = newName;
		charcode = FT_Get_Next_Char( face, charcode, &gindex );
	}
	UseFont = (invalidGlyphs == 0);
	FT_Done_FreeType( library );
	HasMetrics=UseFont;
	metricsread=UseFont;
	return(HasMetrics);
}


namespace {
uint word(QByteArray const & bb, uint pos) 
{
	unsigned char * pp = reinterpret_cast<unsigned char*>(bb.data()) + pos;
	return pp[0] << 24 | pp[1] << 16 | pp[2] << 8 | pp[3];
}
uint word16(QByteArray const & bb, uint pos) 
{
	unsigned char * pp = reinterpret_cast<unsigned char*>(bb.data()) + pos;
	return pp[0] << 8 | pp[1];
}
QString tag(QByteArray const & bb, uint pos)
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
	if (to + len > dst.size())
		return false;
	if (from + len > src.size())
		return false;
	
	memcpy(dst.data() + to, src.data() + from, len);
	return true;
}
} //namespace


void Foi_ttf::RawData(QByteArray & bb) {
	if (formatCode == TTCF) {
		QByteArray coll;
		Foi::RawData(coll);
		// access table for faceIndex
		if (faceIndex >= static_cast<int>(word(coll, 8)))
		{
			bb.resize(0);
			return;
		}
		static const uint OFFSET_TABLE_LEN = 12;
		static const uint   TDIR_ENTRY_LEN = 16;
		uint faceOffset = word(coll, 12 + 4 * faceIndex);
		uint nTables    = word16(coll, faceOffset + 4);
		sDebug(QObject::tr("extracting face %1 from font %2 (offset=%3, nTables=%4)").arg(faceIndex).arg(Datei).arg(faceOffset).arg(nTables));
		uint headerLength = OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * nTables;   
		uint tableLengths = 0;
		// sum table lengths incl padding
		for (uint i=0; i < nTables; ++i)
		{
			tableLengths += word(coll, faceOffset + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 12);
			tableLengths = (tableLengths+3) & ~3;
		}
		bb.resize(headerLength + tableLengths);
		if (! bb.data())
			return;
		// write header
		sDebug(QObject::tr("memcpy header: %1 %2 %3").arg(0).arg(faceOffset).arg(headerLength));
		if (!copy(bb, 0, coll, faceOffset, headerLength))
			return;

		uint pos = headerLength;
		for (uint i=0; i < nTables; ++i)
		{
			uint tableSize  = word(coll, faceOffset + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 12);
			uint tableStart = word(coll, faceOffset + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 8);
			sDebug(QObject::tr("table '%1'").arg(tag(coll, tableStart)));
			sDebug(QObject::tr("memcpy table: %1 %2 %3").arg(pos).arg(tableStart).arg(tableSize));
			if (!copy(bb, pos, coll, tableStart, tableSize)) break;
			// write new offset to table entry
			sDebug(QObject::tr("memcpy offset: %1 %2 %3").arg(OFFSET_TABLE_LEN + TDIR_ENTRY_LEN*i + 8).arg(pos).arg(4));
			memcpy(bb.data() + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 8, &pos, 4);
			pos += tableSize;
			// pad
			while (pos & 3 != 0)
				bb.data()[pos++] = '\0';
		}
	}	
	else if (formatCode == TYPE42) {
		Foi::RawData(bb);
	}
	else {
		Foi::RawData(bb);
	}
}

bool Foi_ttf::EmbedFont(QString &str)
{
	if (formatCode == Foi::TYPE42) {
		//easy:
		QByteArray bb;
		Foi::RawData(bb);
		str += bb;
		return true;
	}
	QString tmp4;
	QString tmp2 = "";
	QString tmp3 = "";
	int counter = 0;
	char *buf[50];
	FT_Library library;
	FT_Face face;
	FT_ULong  charcode;
	FT_UInt   gindex;
	FT_Init_FreeType(&library);
	FT_New_Face(library, Datei, faceIndex, &face);
	const FT_Stream fts = face->stream;
	if (FT_Stream_Seek(fts, 0L)) {
		FT_Done_FreeType( library );
		return(false);
	}
	str+="%!PS-TrueTypeFont\n";
	str+="11 dict begin\n";
	str+="/FontName /" + RealName() + " def\n";
	str+="/Encoding /ISOLatin1Encoding where {pop ISOLatin1Encoding} {StandardEncoding} ifelse def\n";
	str+="/PaintType 0 def\n/FontMatrix [1 0 0 1 0 0] def\n";
	str+="/FontBBox ["+FontBBox+"] def\n";
	str+="/FontType 42 def\n";
	str+="/FontInfo 8 dict dup begin\n";
	str+="/FamilyName (" + RealName() + ") def\n";
	str+="end readonly def\n";
	unsigned char *tmp = new unsigned char[65535];
	int length;
	char linebuf[80];
	str += "/sfnts [";
	int poso=0;
	do {
		int posi=0;
		length= fts->size - fts->pos;
		if (length > 65534) {
			length = 65534;
		}
		if (!FT_Stream_Read(fts,tmp,length))
		{
			str+="\n<\n";
			for (int j = 0; j < length; j++)
			{
				unsigned char u=tmp[posi];
				linebuf[poso]=((u >> 4) & 15) + '0';
				if(u>0x9f) linebuf[poso]+='a'-':';
				++poso;
				u&=15; linebuf[poso]=u + '0';
				if(u>0x9) linebuf[poso]+='a'-':';
				++posi;
				++poso;
				if (poso > 70)
				{
					linebuf[poso++]='\n';
					linebuf[poso++]=0;
					str += linebuf;
					poso = 0;
				}
			}
			linebuf[poso++]=0;
			str += linebuf;
			poso = 0;
			str += "00\n>";
		}
		else {
			sDebug(QObject::tr("Font %1 is broken (read stream), no embedding").arg(Datei).arg(gindex));
			str += "\n] def\n";
			return false;
		}
	}
	while (length==65534);
	str += "\n] def\n";
	delete tmp;
	setBestEncoding(face);
	gindex = 0;
	charcode = FT_Get_First_Char(face, &gindex );
	while (gindex != 0)
	{
		FT_Get_Glyph_Name(face, gindex, buf, 50);
		tmp2 += "/"+QString(reinterpret_cast<char*>(buf))+" "+tmp3.setNum(gindex)+" def\n";
		 charcode = FT_Get_Next_Char(face, charcode, &gindex );
		counter++;
	}
	FT_Done_FreeType( library );
	tmp4.setNum(counter);
	str += "/CharStrings " + tmp4 + " dict dup begin\n"+tmp2;
	str += "end readonly def\n";
	str += "FontName currentdict end definefont pop\n";
	return(true);
}

