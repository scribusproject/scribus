/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFile>
#include <QString>
#include <QObject>

#include <sys/types.h>

#include "fonts/scface_ttf.h"
#include "fonts/scfontmetrics.h"
#include "util.h"
#include "scconfig.h"



namespace {
uint word(QByteArray const & bb, uint pos) 
{
	const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
	return pp[0] << 24 | pp[1] << 16 | pp[2] << 8 | pp[3];
}
uint word16(QByteArray const & bb, uint pos) 
{
	const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
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
	if (to + len > static_cast<uint>(dst.size()))
		return false;
	if (from + len > static_cast<uint>(src.size()))
		return false;
	
	memcpy(dst.data() + to, src.data() + from, len);
	return true;
}
} //namespace


void ScFace_ttf::RawData(QByteArray & bb) const {
	if (formatCode == ScFace::TTCF) {
		QByteArray coll;
		FtFace::RawData(coll);
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
		sDebug(QObject::tr("extracting face %1 from font %2 (offset=%3, nTables=%4)").arg(faceIndex).arg(fontFile).arg(faceOffset).arg(nTables));
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
			while ((pos & 3) != 0)
				bb.data()[pos++] = '\0';
		}
	}	
	else if (formatCode == ScFace::TYPE42) {
		FtFace::RawData(bb);
	}
	else {
		FtFace::RawData(bb);
	}
}

bool ScFace_ttf::EmbedFont(QString &str) const
{
	if (formatCode == ScFace::TYPE42) {
		//easy:
		QByteArray bb;
		FtFace::RawData(bb);
		str += bb;
		return true;
	}
	QString tmp4;
	QString tmp2 = "";
	QString tmp3 = "";
	int counter = 0;
	char *buf[50];
	FT_ULong  charcode;
	FT_UInt   gindex;
	FT_Face face = ftFace();
	if (!face) {
		return false;
	}
	const FT_Stream fts = face->stream;
	if (ftIOFunc(fts, 0L, NULL, 0)) {
		return(false);
	}
	str+="%!PS-TrueTypeFont\n";
	str+="11 dict begin\n";
	str+="/FontName /" + psName + " def\n";
	str+="/Encoding /ISOLatin1Encoding where {pop ISOLatin1Encoding} {StandardEncoding} ifelse def\n";
	str+="/PaintType 0 def\n/FontMatrix [1 0 0 1 0 0] def\n";
	str+="/FontBBox ["+FontBBox+"] def\n";
	str+="/FontType 42 def\n";
	str+="/FontInfo 8 dict dup begin\n";
	str+="/FamilyName (" + psName + ") def\n";
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
		if (!ftIOFunc(fts, 0L, tmp, length))
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
			sDebug(QObject::tr("Font %1 is broken (read stream), no embedding").arg(fontFile));
			str += "\n] def\n";
			status = qMax(status,ScFace::BROKENGLYPHS);
			return false;
		}
	} while (length==65534);
	
	str += "\n] def\n";
	delete[] tmp;
	gindex = 0;
	charcode = FT_Get_First_Char(face, &gindex );
	while (gindex != 0)
	{
		FT_Get_Glyph_Name(face, gindex, buf, 50);
		tmp2 += "/"+QString(reinterpret_cast<char*>(buf))+" "+tmp3.setNum(gindex)+" def\n";
		 charcode = FT_Get_Next_Char(face, charcode, &gindex );
		counter++;
	}
	tmp4.setNum(counter);
	str += "/CharStrings " + tmp4 + " dict dup begin\n"+tmp2;
	str += "end readonly def\n";
	str += "FontName currentdict end definefont pop\n";
	return(true);
}

