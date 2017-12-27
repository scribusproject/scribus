//
//  sfnt.cpp
//  Scribus
//
//  Created by Andreas Vox on 18.04.15.
//
//

#include "fonts/sfnt.h"
#include "fonts/sfnt_format.h"

#include FT_TRUETYPE_TABLES_H
#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_IDS_H

#include <QDebug>

namespace sfnt {
	
	uchar byte(const QByteArray & bb, uint pos)
	{
		const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
		return pp[0];
	}
	quint32 word(const QByteArray & bb, uint pos)
	{
		const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
		return pp[0] << 24 | pp[1] << 16 | pp[2] << 8 | pp[3];
	}
	
	void putWord(QByteArray & bb, uint pos, quint32 val)
	{
		unsigned char * pp = reinterpret_cast<unsigned char*>(bb.data()) + pos;
		*pp++ = (val >> 24) & 0xFF;
		*pp++ = (val >> 16) & 0xFF;
		*pp++ = (val >> 8) & 0xFF;
		*pp++ = (val) & 0xFF;
	}
	
	void appendWord(QByteArray & bb, quint32 val)
	{
		uint pos = bb.size();
		bb.resize(pos + 4);
		putWord(bb, pos, val);
	}
	
	quint16 word16(const QByteArray & bb, uint pos)
	{
		const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
		return pp[0] << 8 | pp[1];
	}
	
	void putWord16(QByteArray & bb, uint pos, quint16 val)
	{
		unsigned char * pp = reinterpret_cast<unsigned char*>(bb.data()) + pos;
		*pp++ = (val >> 8) & 0xFF;
		*pp++ = (val) & 0xFF;
	}
	
	void appendWord16(QByteArray & bb, quint16 val)
	{
		uint pos = bb.size();
		bb.resize(pos + 2);
		putWord16(bb, pos, val);
	}
	
	const QByteArray tag(const QByteArray& bb, uint pos)
	{
		return QByteArray::fromRawData(bb.constData() + pos, 4);
	}
	
	const QByteArray tag(uint word)
	{
		QByteArray result;
		result.resize(4);
		result[0] = (word >> 24) & 0xFF;
		result[1] = (word >> 16) & 0xFF;
		result[2] = (word >> 8) & 0xFF;
		result[3] = (word) & 0xFF;
		return result;
	}
	
	bool copy(QByteArray & dst, uint to, const QByteArray & src, uint from, uint len)
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



uint PostTable::numberOfGlyphs() const
{
	if (m_names.length() > 0)
		return m_names.length();
	else
		return post_format10_names_count;
}

QString PostTable::nameFor(uint glyph) const
{
	if (glyph < (uint) m_names.length())
	{
		return m_names[glyph];
	}
	else if (glyph < sfnt::post_format10_names_count)
	{
		return post_format10_names[glyph];
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
	//qDebug() << "load post" << error << size;
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
	
	switch (sfnt::word(postData, ttf_post_format))
	{
		case sfnt::post_format10:
			usable = true;
			m_names.clear();
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
			errorMsg = QString("unsupported post format %1").arg(sfnt::word(postData,0));
			usable = false;
			return;
			
	}
	QMap<QString,uint> usedNames;
	QList<QByteArray> pascalStrings;
	
	uint nrOfGlyphs = sfnt::word16(postData, ttf_post_header_length);
	uint stringPos = ttf_post_header_length + 2 + 2 * nrOfGlyphs;
	while (stringPos < (uint) postData.length())
	{
		int strLen = byte(postData, stringPos);
		++stringPos;
		pascalStrings.append(postData.mid(stringPos, strLen));
		stringPos += strLen;
	}
	uint pos = ttf_post_header_length + 2;
	for (uint gid = 0; gid < nrOfGlyphs; ++gid)
	{
		uint nameIndex = sfnt::word16(postData, pos);
		pos += 2;
		QString name;
		if (nameIndex < sfnt::post_format10_names_count)
			name = sfnt::post_format10_names[nameIndex];
		else if (nameIndex < pascalStrings.length() + sfnt::post_format10_names_count)
			name = pascalStrings[nameIndex - sfnt::post_format10_names_count];
		else {
			usable = false;
			errorMsg = QString("missing name %1 for glyph %2").arg(nameIndex).arg(gid);
			return;
		}
		if (name != ".notdef" && name[0] != QChar(0) && usedNames.contains(name))
		{
			usable = false;
			errorMsg = QString("duplicate name %1 used for glyphs %2 and %3").arg(name).arg(gid).arg(usedNames[name]);
			return;
		}
		usedNames[name] = gid;
		m_names.append(name);
	}
	errorMsg = "";
	usable = true;
}



	int copyTable(QByteArray& ttf, uint destDirEntry, uint pos, const QByteArray& source, uint dirEntry)
	{
		FT_ULong tag = word(source, dirEntry + ttf_TableRecord_tag);
		uint checksum = word(source, dirEntry + ttf_TableRecord_checkSum);
		uint tableStart = word(source, dirEntry + ttf_TableRecord_offset);
		uint tableSize  = word(source, dirEntry + ttf_TableRecord_length);
		
		if (!copy(ttf, pos, source, tableStart, tableSize))
			return -1;
		
		putWord(ttf, destDirEntry + ttf_TableRecord_tag, tag);
		putWord(ttf, destDirEntry + ttf_TableRecord_checkSum, checksum);
		putWord(ttf, destDirEntry + ttf_TableRecord_offset, pos);
		putWord(ttf, destDirEntry + ttf_TableRecord_length, tableSize);
		
		return tableSize;
	}


QByteArray extractFace(const QByteArray& coll, int faceIndex)
{
	QByteArray result;
	
	const int numFonts = word(coll, ttc_numFonts);
	if (faceIndex >= static_cast<int>(numFonts))
	{
		return result;
	}

	uint faceOffset = sfnt::word(coll, ttc_OffsetTables + 4 * faceIndex);
	uint nTables	= sfnt::word16(coll, faceOffset + ttf_numtables);
	
	//qDebug() << QObject::tr("extracting face %1 from font %2 (offset=%3, nTables=%4)").arg(faceIndex).arg("collection").arg(faceOffset).arg(nTables);
	
	uint headerLength = ttf_TableRecords + ttf_TableRecord_Size * nTables;
	
	uint tableLengths = 0;
	// sum table lengths incl padding
	for (uint i=0; i < nTables; ++i)
	{
		tableLengths += sfnt::word(coll, faceOffset + ttf_TableRecords + ttf_TableRecord_Size * i + ttf_TableRecord_length);
		tableLengths = (tableLengths+3) & ~3;
	}
	result.resize(headerLength + tableLengths);
	if (!result.data())
	{
		result.resize(0);
		return result;
	}
	
	// write header
	//		sDebug(QObject::tr("memcpy header: %1 %2 %3").arg(0).arg(faceOffset).arg(headerLength));
	if (!copy(result, 0, coll, faceOffset, headerLength))
	{
		result.resize(0);
		return result;
	}
	uint pos = headerLength;
	for (uint i=0; i < nTables; ++i)
	{
		uint sourceDirEntry = faceOffset + ttf_TableRecords + ttf_TableRecord_Size * i;
		uint destDirEntry = ttf_TableRecords + ttf_TableRecord_Size * i;
		
		int tableSize = copyTable(result, destDirEntry, pos, coll, sourceDirEntry);
		if (tableSize < 0)
		{
			result.resize(0);
			return result;
		}
		pos += tableSize;

		// pad
		while ((pos & 3) != 0)
			result.data()[pos++] = '\0';
	}
	return result;
}


	uint getTableDirEntry(const QByteArray& ttf, const QByteArray& ttfTag)
	{
		uint nTables = word16(ttf, ttf_numtables);
		uint pos = ttf_TableRecords;
		for (uint i=0; i < nTables; ++i)
		{
			if (ttfTag == tag(word(ttf, pos + ttf_TableRecord_tag)))
			{
				return pos;
			}
			pos += ttf_TableRecord_Size;
		}
		return 0;
	}


	const QByteArray getTable(const QByteArray& ttf, const QByteArray& ttfTag)
	{
		uint pos = getTableDirEntry(ttf, ttfTag);
		if (pos <= 0)
			return QByteArray();

		uint offset = word(ttf, pos + ttf_TableRecord_offset);
		uint length = word(ttf, pos + ttf_TableRecord_length);
		return QByteArray::fromRawData(ttf.constData() + offset, length);
	}

	QByteArray createTableDir(const QList<QByteArray>& tags)
	{
		QByteArray result;
		uint numTables = tags.length();
		uint tableRecordsSize = numTables * ttf_TableRecord_Size;
		result.resize(ttf_TableRecords +  tableRecordsSize);
		uint entrySelector = 0;
		uint searchRange = ttf_TableRecord_Size;
		while (2*searchRange < tableRecordsSize)
		{
			searchRange *= 2;
			++entrySelector;
		}
		uint rangeShift = tableRecordsSize - searchRange;
		
		putWord(result, ttf_sfnt_version, 0x00010000);
		putWord16(result, ttf_numtables, numTables);
		putWord16(result, ttf_searchRange, searchRange);
		putWord16(result, ttf_entrySelector, entrySelector);
		putWord16(result, ttf_rangeShift, rangeShift);
		for (uint i = 0; i < numTables; ++i)
		{
			copy(result, ttf_TableRecords + i*ttf_TableRecord_Size + ttf_TableRecord_tag, tags[i], 0, 4);
			putWord(result, ttf_TableRecords + i*ttf_TableRecord_Size + ttf_TableRecord_checkSum, 0);
			putWord(result, ttf_TableRecords + i*ttf_TableRecord_Size + ttf_TableRecord_offset, 0);
			putWord(result, ttf_TableRecords + i*ttf_TableRecord_Size +ttf_TableRecord_length, 0);
		}
		return result;
	}


	quint32 calcTableChecksum(QByteArray& table)
	{
		quint32 Sum = 0L;
		for (int pos = 0; pos < table.length(); pos += 4)
			Sum += word(table, pos);
		return Sum;
	}


	void writeTable(QByteArray& ttf, const QByteArray& tag, QByteArray& table)
	{
		//qDebug() << "writing table" << tag << table.size() << "@" << ttf.size();
		uint length = table.size();
		while (table.size() & 0x3)
			table.append('\0');
		uint offset = ttf.size();
		uint checksum = calcTableChecksum(table);
		uint pos = getTableDirEntry(ttf, tag);
		putWord(ttf, pos + ttf_TableRecord_checkSum, checksum);
		putWord(ttf, pos + ttf_TableRecord_offset, offset);
		putWord(ttf, pos + ttf_TableRecord_length, length);
		ttf.append(table);
	}


	bool hasLongLocaFormat(const QByteArray& ttf)
	{
		const QByteArray head = getTable(ttf, "head");
		uint idxToLocFormat = word16(head, ttf_head_indexToLocFormat);
//		qDebug() << "loca format:" << (void*)idxToLocFormat;
		return idxToLocFormat == 1;
	}


	QList<quint32> readLoca(const QByteArray& ttf)
	{
		QList<quint32> result;
		const QByteArray loca = getTable(ttf, "loca");
		if (hasLongLocaFormat(ttf))
		{
			for (int i = 0; i < loca.length(); i+=4)
			{
				result.append(word(loca, i));
			}
		}
		else
		{
			for (int i = 0; i < loca.length(); i+=2)
			{
				result.append(word16(loca, i) * 2);
			}
		}
		return result;
	}

	const QByteArray writeLoca(const QList<uint>& loca, bool longFormat)
	{
		QByteArray result;
		if (longFormat)
		{
			for(int i=0; i < loca.length(); ++i)
				appendWord(result, loca[i]);
		}
		else
		{
			for(int i=0; i < loca.length(); ++i)
				appendWord16(result, loca[i] / 2);
		}
		return result;
	}


	QList<std::pair<qint16,quint16> > readHmtx(const QByteArray& ttf)
	{
		QList<std::pair<qint16,quint16> > result;
		const QByteArray hhea = getTable(ttf, "hhea");
		const QByteArray hmtx = getTable(ttf, "hmtx");
		uint endOfLongHorMetrics = 4 * word16(hhea, ttf_hhea_numOfLongHorMetrics);
		qint16 advance;
		quint16 leftSideBearing;
		uint pos = 0;
		while (pos < endOfLongHorMetrics)
		{
			advance = word16(hmtx, pos);
			leftSideBearing = word16(hmtx, pos+2);
			//qDebug() << pos << "hmtx" << advance << leftSideBearing;
			result.append(std::pair<qint16,quint16>(advance, leftSideBearing));
			pos += 4;
		}
		while (pos < (uint) hmtx.length())
		{
			leftSideBearing = word16(hmtx, pos);
			//qDebug() << pos << "hmtx =" << advance << leftSideBearing;
			result.append(std::pair<qint16,quint16>(advance, leftSideBearing));
			pos += 2;
		}
		return result;
	}


	QByteArray writeHmtx(const QList<std::pair<qint16,quint16> >& longHorMetrics)
	{
		QByteArray result;
		QList<std::pair<qint16,quint16> >::const_iterator it;
		//int i = 0;
		for (it = longHorMetrics.cbegin(); it < longHorMetrics.cend(); ++it)
		{
			//qDebug() << "hmtx" << i++ << it->first << it->second;
			appendWord16(result, it->first);
			appendWord16(result, it->second);
		}
		return result;
	}


	QMap<uint, uint> readCMap(const QByteArray& ttf)
	{
		QMap<uint,uint> result;
		const QByteArray cmaps = getTable(ttf, "cmap");
		
		uint numSubtables = word16(cmaps, ttf_cmap_numberSubtables);
		uint startOfUnicodeTable = 0;
		uint startOfSymbolTable = 0;
		uint unicodeFormat = 0, symbolFormat = 0;
		uint pos = ttf_cmap_encodings;
		for (uint i = 0; i < numSubtables; ++i)
		{
			uint platform = word16(cmaps, pos + ttf_cmap_encoding_platformID);
			uint encoding = word16(cmaps, pos + ttf_cmap_encoding_platformSpecificID);
			uint offset = word(cmaps, pos + ttf_cmap_encoding_offset);
			uint format = word16(cmaps, offset + ttf_cmapx_format);
			pos += ttf_cmap_encoding_Size;
			
			if (format < 4 || format > 12)
				continue;
			if (platform == 0 || (platform == 3 && encoding == 1))
			{
				startOfUnicodeTable = offset;
				unicodeFormat = format;
				break;
			}
			if (platform == 3 && encoding == 0) // MS Symbol cmap
			{
				startOfSymbolTable = offset;
				symbolFormat = format;
				continue;
			}
			format = 1; // no such format
		}
		// If no unicode cmap was found, fallback on ms symbol cmap
		uint format = unicodeFormat;
		if ((unicodeFormat <= 0) && (symbolFormat > 0))
		{
			startOfUnicodeTable = startOfSymbolTable;
			format = symbolFormat;
		}
		//qDebug() << "reading cmap format" << format;
		switch(format)
		{
			case 4:
			{
				uint segCount2 = word16(cmaps, startOfUnicodeTable + ttf_cmap4_segCountX2);
				uint endCodes = startOfUnicodeTable + ttf_cmap4_EndCodes;
				uint startCodes = endCodes + segCount2 + ttf_cmap4_StartCodes_EndCodes;
				uint idDeltas = startCodes + segCount2 + ttf_cmap4_IdDeltas_StartCodes;
				uint idRangeOffsets = idDeltas + segCount2 + ttf_cmap4_IdRangeOffsets_IdDeltas;
				//uint glyphIndexArray = idRangeOffsets + segCount2 + ttf_cmap4_GlyphIds_IdRangeOffsets;
				
				for (uint seg = 0; seg < segCount2; seg+=2)
				{
					uint start = word16(cmaps, startCodes + seg);
					uint end = word16(cmaps, endCodes + seg);
					uint idDelta = word16(cmaps, idDeltas + seg);
					uint idRangeOffset = word16(cmaps, idRangeOffsets + seg);
					for (uint c = start; c <= end; ++c)
					{
						quint16 glyph;
						if (idRangeOffset > 0)
						{
							uint glyphIndexAdress = idRangeOffset + 2*(c-start) + (idRangeOffsets + seg);
							glyph = word16(cmaps, glyphIndexAdress);
							if (glyph != 0)
								glyph += idDelta;
						}
						else
						{
							glyph = c + idDelta;
						}
						if (!result.contains(c))
						{
							// search would always find the one in the segment with the lower endcode, i.e. earlier segment
							if (c < 256 || glyph == 0)
								//qDebug() << "(" << QChar(c) << "," << glyph << ")";
							result[c] = glyph;
						}
						else
						{
							// nothing to do. No idea if fonts with overlapping cmap4 segments exist, though.
						}
					}
				}
				break;
			}
			case 6:
			{
				uint firstCode = word16(cmaps, startOfUnicodeTable + ttf_cmap6_firstCode);
				uint count = word16(cmaps, startOfUnicodeTable + ttf_cmap6_entryCount);
				pos = word16(cmaps, startOfUnicodeTable + ttf_cmap6_glyphIndexArray);
				for (uint i = 0; i < count; ++i)
				{
					result[firstCode + i] = word16(cmaps, pos);
					pos += 2;
				}
				break;
			}
			case 12:
			{
				uint nGroups = word(cmaps, startOfUnicodeTable + ttf_cmap12_nGroups);
				pos = startOfUnicodeTable + ttf_cmap12_Groups;
				for (uint grp = 0; grp < nGroups; ++grp)
				{
					uint start = word(cmaps, pos + ttf_cmap12_Group_startCharCode);
					uint end = word(cmaps, pos + ttf_cmap12_Group_endCharCode);
					uint gid = word(cmaps, pos + ttf_cmap12_Group_startGlyphCode);
					for (uint c = start; c <= end; ++c)
					{
						result[c] = gid;
						++gid;
					}
					pos += ttf_cmap12_Group_Size;
				}
				break;
			}
			default:
			{
				qDebug() << "unsupported cmap format" << format;
				break;
			}
		}
		return result;
	}



	const QByteArray writeCMap(const QMap<uint, uint>& cmap)
	{
		// we always write only one table: platform=3(MS), encoding=1(Unicode 16bit)
		QByteArray result;
		appendWord16(result, 0); // version
		appendWord16(result, 1); // number of subtables
		appendWord16(result, 3); // platformID Microsoft
		appendWord16(result, 1); // encodingID UnicodeBMP
		appendWord(result, result.size() + 4); // offset
		
		// find the segments
		
		QList<uint> chars;
		QMap<uint, uint>::ConstIterator cit;
		//qDebug() << "writing cmap";
		bool cmapHasData = false;
		for(cit = cmap.cbegin(); cit != cmap.cend(); ++cit)
		{
			uint ch = cit.key();
			if (!QChar::requiresSurrogates(ch) && cit.value() != 0)
			{
				//qDebug() << "(" << QChar(cit.key()) << "," << cit.value() << ")";
				chars.append(ch);
				cmapHasData = true;
			}
//			qDebug() << QChar(ch) << QChar::requiresSurrogates(ch) << cit.value();
		}
		std::sort(chars.begin(), chars.end());
		
		QList<quint16> startCodes;
		QList<quint16> endCodes;
		QList<quint16> idDeltas;
		QList<quint16> rangeOffsets;
		if (cmapHasData)
		{
			uint pos = 0;
			do {
				quint16 start = chars[pos];
				quint16 delta = cmap[start] - start;
				quint16 rangeOffset = 0;
				quint16 end = start;
				quint16 next;
				++pos;
				while (pos < (uint) chars.length() && (next = chars[pos]) == end+1)
				{
					end = next;
					if (delta != (quint16)(cmap[chars[pos]] - next))
					{
						rangeOffset = 1; // will be changed later
					}
					++pos;
				}
				startCodes.append(start);
				endCodes.append(end);
				idDeltas.append(delta);
				rangeOffsets.append(rangeOffset);
			}
			while (pos < (uint) chars.length());
		}
		
		startCodes.append(0xFFFF);
		endCodes.append(0xFFFF);
		idDeltas.append(1); // makes gid 0
		rangeOffsets.append(0);
		
		// write the tables
		
		uint startOfTable = result.size();
		result.resize(startOfTable + ttf_cmap4_EndCodes);

		uint segCount = endCodes.length();
		uint segCountX2 = 2 * segCount;
		uint entrySelector = 0;
		uint searchRange = 2;
		while (searchRange <= segCount)
		{
			++entrySelector;
			searchRange *= 2;
		}
		putWord16(result, startOfTable + ttf_cmapx_format, 4);
		/* ttf_cmap4_length is set later */
		putWord16(result, startOfTable + ttf_cmap4_language, 0);
		putWord16(result, startOfTable + ttf_cmap4_segCountX2, segCountX2);
		putWord16(result, startOfTable + ttf_cmap4_searchRange, searchRange);
		putWord16(result, startOfTable + ttf_cmap4_entrySelector, entrySelector);
		putWord16(result, startOfTable + ttf_cmap4_rangeShift, segCountX2 - searchRange);
		
		for (uint i = 0; i < segCount; ++i)
		{
			appendWord16(result, endCodes[i]);
		};
		appendWord16(result, 0); // reservedPad
		
		for (uint i = 0; i < segCount; ++i)
		{
			appendWord16(result, startCodes[i]);
		};
		for (uint i = 0; i < segCount; ++i)
		{
			appendWord16(result, idDeltas[i]);
		};
		
		
		uint startOfIdRangeOffsetTable = result.size();
		uint startOfGlyphIndexArray = startOfIdRangeOffsetTable + segCountX2;
		result.resize(startOfGlyphIndexArray);
		
		for (uint i = 0; i < segCount; ++i)
		{
			uint idRangeOffsetAddress = startOfIdRangeOffsetTable + 2*i;
			if (rangeOffsets[i] == 0)
			{
				//quint16 dbg = startCodes[i] + idDeltas[i];
				//qDebug() << QChar(startCodes[i]) << "-" << QChar(endCodes[i]) << "/" << (endCodes[i]-startCodes[i]+1) << "+" << idDeltas[i] << "-->" << dbg;
				putWord16(result, idRangeOffsetAddress, 0);
			}
			else
			{
				quint16 idRangeOffset = result.size() - idRangeOffsetAddress;
				putWord16(result, idRangeOffsetAddress, idRangeOffset);

				//qDebug() << QChar(startCodes[i]) << "-" << QChar(endCodes[i]) << "/" << (endCodes[i]-startCodes[i]+1) << "@" << idRangeOffset << "+" << idDeltas[i];

				uint startCode = startCodes[i];
				uint segLength = (endCodes[i]-startCode+1);
				for (uint offset = 0; offset < segLength; ++offset)
				{
					quint16 glyph = cmap[startCode + offset];
					if (glyph != 0)
					{
						glyph -= idDeltas[i];
					}
					appendWord16(result, glyph);
				}
			}
		};
		
		putWord16(result, startOfTable + ttf_cmap4_length, result.size() - startOfTable);
		return result;
	 }
	
	
	QList<uint> copyGlyphComponents(QByteArray& destGlyf, const QByteArray& srcGlyf, uint srcOffset,
	                                QMap<uint,uint>& newForOldGid, uint& nextFreeGid)
	{
		QList<uint> result;
		
		uint destStart = destGlyf.size();
		destGlyf.resize(destStart + ttf_glyf_headerSize);
		copy(destGlyf, destStart, srcGlyf, srcOffset, ttf_glyf_headerSize);
		
		uint pos = srcOffset + ttf_glyf_headerSize;
		bool haveInstructions = false;
		uint flags = 0;
		do {
			/* flags */
			flags = word16(srcGlyf, pos);
			pos += 2;
			haveInstructions |= (flags & ttf_glyf_ComponentFlag_WE_HAVE_INSTRUCTIONS);
			appendWord16(destGlyf, flags);
			
			/* glyphindex */
			uint glyphIndex = word16(srcGlyf, pos);
			pos += 2;
			if (!newForOldGid.contains(glyphIndex))
			{
				result.append(glyphIndex);
				newForOldGid[glyphIndex] = nextFreeGid++;
			}
			glyphIndex = newForOldGid[glyphIndex];
			appendWord16(destGlyf, glyphIndex);
			
			/* args */
			if ( flags & ttf_glyf_ComponentFlag_ARG_1_AND_2_ARE_WORDS) {
				appendWord16(destGlyf, word16(srcGlyf, pos)); // arg1
				pos += 2;
				appendWord16(destGlyf, word16(srcGlyf, pos)); // arg2
				pos += 2;
			}
			else {
				appendWord16(destGlyf, word16(srcGlyf, pos)); // arg1and2
				pos += 2;
			}
			if ( flags & ttf_glyf_ComponentFlag_WE_HAVE_A_SCALE ) {
				appendWord16(destGlyf, word16(srcGlyf, pos)); // scale
				pos += 2;
			} else if ( flags & ttf_glyf_ComponentFlag_WE_HAVE_AN_X_AND_Y_SCALE ) {
				appendWord16(destGlyf, word16(srcGlyf, pos)); // xscale
				pos += 2;
				appendWord16(destGlyf, word16(srcGlyf, pos)); // yscale
				pos += 2;
			} else if ( flags & ttf_glyf_ComponentFlag_WE_HAVE_A_TWO_BY_TWO ) {
				appendWord16(destGlyf, word16(srcGlyf, pos)); // xscale
				pos += 2;
				appendWord16(destGlyf, word16(srcGlyf, pos)); // scale01
				pos += 2;
				appendWord16(destGlyf, word16(srcGlyf, pos)); // scale10
				pos += 2;
				appendWord16(destGlyf, word16(srcGlyf, pos)); // yscale
				pos += 2;
			}
		} while ( flags & ttf_glyf_ComponentFlag_MORE_COMPONENTS );
			
		if (haveInstructions)
		{
			uint numInstr = word16(srcGlyf, pos);
			appendWord16(destGlyf, numInstr);
			pos += 2;
			uint destPos = destGlyf.size();
			destGlyf.resize(destPos + numInstr);
			copy(destGlyf, destPos, srcGlyf, pos, numInstr);
		}
		
		return result;
	}
	

	QList<uint> copyGlyph(QList<uint>& destLoca, QByteArray& destGlyf, uint destGid,
	                      const QList<uint>& srcLoca, const QByteArray& srcGlyf, uint srcGid,
	                      QMap<uint,uint>& newForOldGid, uint& nextFreeGid)
	{
		QList<uint> compositeElements;
		uint glyphStart = srcLoca[srcGid];
		uint glyphLength = srcLoca[srcGid+1] - glyphStart;
		destLoca.append(destGlyf.size());
		//int i = 0;
		if (glyphLength > 0)
		{
			quint16 nrOfContours = word16(srcGlyf, glyphStart);
			if (nrOfContours <= ttf_glyf_Max_numberOfContours)
			{
				// simple glyph
				uint destStart = destGlyf.size();
				//qDebug() << i++ << ":" << nrOfContours << "contours" << glyphStart << "-->" << destStart << "/" << glyphLength;
				destGlyf.resize(destStart + glyphLength);
				copy(destGlyf, destStart, srcGlyf, glyphStart, glyphLength);
			}
			else
			{
				compositeElements.append(copyGlyphComponents(destGlyf, srcGlyf, glyphStart, newForOldGid, nextFreeGid));
				//qDebug() << i++ << ":" << srcGid << "composite glyph brought" << compositeElements.size() << "more glyphs";
			}
			// Data must be aligned at least on 2 byte boundary, however
			// a 4-byte alignment is recommended by TTF specs for reasons
			// related to CPU and efficiency
			int targetSize = (destGlyf.size() + 3) & ~3;
			if (destGlyf.size() < targetSize)
			{
				destGlyf.reserve(targetSize);
				while (destGlyf.size() < targetSize)
					destGlyf.append('\0');
			}
		}

		return compositeElements;
	}
	
	
	QByteArray subsetFace(const QByteArray& ttf, QList<uint>& glyphs)
	{
		QMap<QByteArray,QByteArray> tables;
		
		
//		qDebug() << "loca table:" << (void*) oldLoca[0] << (void*) oldLoca[1] << (void*) oldLoca[2] << (void*) oldLoca[3] << (void*) oldLoca[4] << (void*) oldLoca[5] << (void*) oldLoca[6] << (void*) oldLoca[7];
		

		QMap<uint,uint> newForOldGid;
		if (glyphs.length() == 0)
		{
			tables["loca"] = getTable(ttf, "loca");
			tables["glyf"] = getTable(ttf, "glyf");
		}
		else
		{
			QList<uint> oldLoca = readLoca(ttf);
			const QByteArray oldGlyf = getTable(ttf, "glyf");

			QList<quint32> newLoca;
			QByteArray newGlyf;
			glyphs.removeAll(0);
			glyphs.prepend(0);

			for (int i = 0; i < glyphs.length(); ++i)
			{
				uint oldGid = glyphs[i];
				newForOldGid[oldGid] = i;
			}
			
			uint nextFreeGid = glyphs.length();
			for (int i = 0; i < glyphs.length(); ++i)
			{
				uint oldGid = glyphs[i];
				glyphs.append(copyGlyph(newLoca, newGlyf, i,
				                        oldLoca, oldGlyf, oldGid,
				                        newForOldGid, nextFreeGid));
			}
			newLoca.append(newGlyf.length());
			
			tables["loca"] = writeLoca(newLoca, hasLongLocaFormat(ttf));
			tables["glyf"] = newGlyf;
		}
		
		
		QMap<uint,uint> cmap = readCMap(ttf);
		QMap<uint,uint>::iterator it;
		uint firstChar = 0xFFFFFFFF;
		uint lastChar = 0;
		for (it = cmap.begin(); it != cmap.end(); ++it)
		{
			if (glyphs.length() > 0 && !glyphs.contains(it.value()))
			{
				it.value() = 0;
			}
			else if (it.value() != 0)
			{
				if (glyphs.length() > 0)
				{
					//qDebug() << "MAP" << QChar(it.key()) << it.value() << "-->" << newForOldGid[it.value()];
					it.value() = newForOldGid[it.value()];
				}
				if (it.key() < firstChar)
					firstChar = it.key();
				else if (it.key() > lastChar)
					lastChar = it.key();
			}
		}
		tables["cmap"] = writeCMap(cmap);

		
		QByteArray os2 = getTable(ttf, "OS/2");
		if (os2.length() > ttf_os2_usLastCharIndex)
		{
			// TODO: adapt unicode ranges
			putWord16(os2, ttf_os2_usFirstCharIndex, firstChar < 0xFFFF ? firstChar : 0xFFFF);
			putWord16(os2, ttf_os2_usLastCharIndex, lastChar < 0xFFFF ? lastChar : 0xFFFF);
			tables["OS/2"] = os2;
		}
		
		if (glyphs.length() > 0)
		{
			QList<std::pair<qint16, quint16> > oldHmtx = readHmtx(ttf);
			QList<std::pair<qint16, quint16> > newHmtx;
			newHmtx.append(std::pair<qint16, quint16>(1234, 123));
			for (int i = 1; i < glyphs.length(); ++i)
				newHmtx.append(newHmtx[0]);
			QMap<uint,uint>::const_iterator iter;
			for (iter = newForOldGid.cbegin(); iter != newForOldGid.cend(); ++iter)
			{
				//qDebug() << "hmtx" << iter.key() << " -> " << iter.value() << "=" << oldHmtx[iter.key()].first;
				newHmtx[iter.value()] = oldHmtx[iter.key()];
			}
			tables["hmtx"] = writeHmtx(newHmtx);
		}
		else
		{
			tables["hmtx"] = getTable(ttf, "hmtx");
		}
		
		QByteArray maxp = getTable(ttf, "maxp");
		if (glyphs.length() > 0)
		{
			putWord16(maxp, ttf_maxp_numGlyphs, glyphs.length());
		}
		tables["maxp"] = maxp;
		
		QByteArray hhea = getTable(ttf, "hhea");
		if (glyphs.length() > 0)
		{
			putWord16(hhea, ttf_hhea_numOfLongHorMetrics, glyphs.length());
		}
		tables["hhea"] = hhea;
		
		QByteArray post = getTable(ttf, "post");
		if (word(post, ttf_post_format) != post_format30)
		{
			putWord(post, ttf_post_format, post_format30);
			post.truncate(ttf_post_header_length);
		}
		tables["post"] = post;
		
		// TODO: kern table
		
		QByteArray name = getTable(ttf, "name");
		if (name.length() > 0)
			tables["name"] = name;
		
		QByteArray prep = getTable(ttf, "prep");
		if (prep.length() > 0)
			tables["prep"] = prep;
		
		QByteArray cvt = getTable(ttf, "cvt ");
		if (cvt.length() > 0)
			tables["cvt "] = cvt;

		QByteArray fpgm = getTable(ttf, "fpgm");
		if (fpgm.length() > 0)
			tables["fpgm"] = fpgm;
		

		QByteArray head = getTable(ttf, "head");
		putWord(head, ttf_head_checkSumAdjustment, 0);
		tables["head"] = head;
		

		QByteArray font = createTableDir(tables.keys());
		QMap<QByteArray,QByteArray>::iterator tableP;
		for (tableP = tables.begin(); tableP != tables.end(); ++tableP)
		{
			writeTable(font, tableP.key(), tableP.value());
		}
		
		uint checkSumAdjustment = 0xB1B0AFBA - calcTableChecksum(font);
		uint headTable = getTableDirEntry(font, "head");
		headTable = word(font, headTable + ttf_TableRecord_offset);
		putWord(font, headTable + ttf_head_checkSumAdjustment, checkSumAdjustment);
		
		// done!
		
		return font;
	}
	
} // namespace sfnt
