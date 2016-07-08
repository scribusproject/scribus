//
//  cff.cpp
//  Scribus
//
//  Created by Andreas Vox on 04.05.15.
//
//

#include "cff.h"

#include <cassert>
#include <cmath>
#include <QDebug>

namespace cff {
	
	static const char* cffDictKeys[] = {
		"version",
		"Notice",
		"FullName",
		"FamilyName",
		"Weight",
		"FontBBox",
		"BlueValues",
		"OtherBlues",
		"FamilyBlues",
		"FamilyOtherBlues",
		"StdHW",
		"StdVW",
		"",
		"UniqueID",
		"XUID",
		"charset",
		"Encoding",
		"CharStrings",
		"Private",
		"Subrs",
		"defaultWidthX",
		"nominalWidthX"
	};
	
	static const char* cffDictKeys0c[] = {
		"Copyright",
		"IsFixedPitch",
		"ItalicAngle",
		"UnderlinePosition",
		"UnderlineThickness",
		"PaintType",
		"CharstringType",
		"FontMatrix",
		"StrokeWidth",
		"BlueScale",
		"BlueShift",
		"BlueFuzz",
		"StemSnapH",
		"StemSnapV",
		"ForceBold",
		"-Reserved-",
		"-Reserved-",
		"LanguageGroup",
		"ExpansionFactor",
		"InitialRandomSeed",
		"SyntheticBase",
		"PostScript",
		"BaseFontName",
		"BaseFontBlend",
		"-Reserved-",
		"-Reserved-",
		"-Reserved-",
		"-Reserved-",
		"-Reserved-",
		"-Reserved-",
		"ROS",
		"CIDFontVersion",
		"CIDFontRevision",
		"CIDFontType",
		"CIDCount",
		"UIDBase",
		"FDArray",
		"FDSelect",
		"FontName"
	};
	
	static const char* cff_operator(cff::operator_type id)
	{
		if ((id & 0x0c00) == 0x0c00)
		{
			int idx = id & 0xff;
			if (idx >= 0 && idx <= 38)
				return cffDictKeys0c[idx];
			else
				return "";
		}
		else
		{
			if ( /* id >= 0 && */  id <= 21)
				return cffDictKeys[id];
			else
				return "";
		}
	}
	
	static const char* stdStrings[] = {
		/* 0 */
		".notdef",
		"space",
		"exclam",
		"quotedbl",
		"numbersign",
		"dollar",
		"percent",
		"ampersand",
		"quoteright",
		"parenleft",
		/* 10 */
		"parenright",
		"asterisk",
		"plus",
		"comma",
		"hyphen",
		"period",
		"slash",
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
		"colon",
		"semicolon",
		"less",
		/* 30 */
		"equal",
		"greater",
		"question",
		"at",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		/* 50 */
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"bracketleft",
		"backslash",
		"bracketrigh",
		"asciicircum",
		"underscore",
		"quoteleft",
		"a",
		"b",
		"c",
		"d",
		/* 70 */
		"e",
		"f",
		"g",
		"h",
		"i",
		"j",
		"k",
		"l",
		"m",
		"n",
		"o",
		"p",
		"q",
		"r",
		"s",
		"t",
		"u",
		"v",
		"w",
		"x",
		/* 90 */
		"y",
		"z",
		"braceleft",
		"bar",
		"braceright",
		"asciitilde",
		"exclamdown",
		"cent",
		"sterling",
		"fraction",
		/* 100  */
		"yen",
		"florin",
		"section",
		"currency",
		"quotesingle",
		"quotedblleft",
		"guillemotleft",
		"guilsinglleft",
		"guilsinglright",
		"fi",
		"fl",
		"endash",
		"dagger",
		"daggerdbl",
		"periodcentered",
		"paragraph",
		"bullet",
		"quotesinglbase",
		"quotedblbase",
		"quotedblright",
		/* 120 */
		"guillemotright",
		"ellipsis",
		"perthousand",
		"questiondown",
		"grave",
		"acute",
		"circumflex",
		"tilde",
		"macron",
		"breve",
		"dotaccent",
		"dieresis",
		"ring",
		"cedilla",
		"hungarumlaut",
		"ogonek",
		"caron",
		"emdash",
		"AE",
		"ordfeminine",
		/* 140 */
		"Lslash",
		"Oslash",
		"OE",
		"ordmasculine",
		"ae",
		"dotlessi",
		"lslash",
		"oslash",
		"oe",
		"germandbls",
		"onesuperior",
		"logicalnot",
		"mu",
		"trademark",
		"Eth",
		"onehalf",
		"plusminus",
		"Thorn",
		"onequarter",
		"divide",
		/* 160 */
		"brokenbar",
		"degree",
		"thorn",
		"threequarters",
		"twosuperior",
		"registered",
		"minus",
		"eth",
		"multiply",
		"threesuperior",
		"copyright",
		"Aacute",
		"Acircumflex",
		"Adieresis",
		"Agrave",
		"Aring",
		"Atilde",
		"Ccedilla",
		"Eacute",
		"Ecircumflex",
		/* 180 */
		"Edieresis",
		"Egrave",
		"Iacute",
		"Icircumflex",
		"Idieresis",
		"Igrave",
		"Ntilde",
		"Oacute",
		"Ocircumflex",
		"Odieresis",
		"Ograve",
		"Otilde",
		"Scaron",
		"Uacute",
		"Ucircumflex",
		"Udieresis",
		"Ugrave",
		"Yacute",
		"Ydieresis",
		"Zcaron",
		/* 200 */
		"aacute",
		"acircumflex",
		"adieresis",
		"agrave",
		"aring",
		"atilde",
		"ccedilla",
		"eacute",
		"ecircumflex",
		"edieresis",
		"egrave",
		"iacute",
		"icircumflex",
		"idieresis",
		"igrave",
		"ntilde",
		"oacute",
		"ocircumflex",
		"odieresis",
		"ograve",
		/* 220 */
		"otilde",
		"scaron",
		"uacute",
		"ucircumflex",
		"udieresis",
		"ugrave",
		"yacute",
		"ydieresis",
		"zcaron",
		"exclamsmall",
		"Hungarumlautsmall",
		"dollaroldstyle",
		"dollarsuperior",
		"ampersandsmall",
		"Acutesmall",
		"parenleftsuperior",
		"parenrightsuperior",
		"twodotenleader",
		"onedotenleader",
		"zerooldstyle",
		/* 240 */
		"oneoldstyle",
		"twooldstyle",
		"threeoldstyle",
		"fouroldstyle",
		"fiveoldstyle",
		"sixoldstyle",
		"sevenoldstyle",
		"2eightoldstyle",
		"nineoldstyle",
		"commasuperior",
		"threequartersemdash",
		"periodsuperior",
		"questionsmall",
		"asuperior",
		"bsuperior",
		"centsuperior",
		"dsuperior",
		"esuperior",
		"isuperior",
		"lsuperior",
		/* 260 */
		"msuperior",
		"nsuperior",
		"osuperior",
		"rsuperior",
		"ssuperior",
		"tsuperior",
		"ff",
		"ffi",
		"ffl",
		"parenleftinferior",
		"parenrightinferior",
		"Circumflexsmall",
		"hyphensuperior",
		"Gravesmall",
		"Asmall",
		"Bsmall",
		"Csmall",
		"Dsmall",
		"Esmall",
		"Fsmall",
		/* 280 */
		"Gsmall",
		"Hsmall",
		"Ismall",
		"Jsmall",
		"Ksmall",
		"Lsmall",
		"Msmall",
		"Nsmall",
		"Osmall",
		"Psmall",
		"Qsmall",
		"Rsmall",
		"Ssmall",
		"Tsmall",
		"Usmall",
		"Vsmall",
		"Wsmall",
		"Xsmall",
		"Ysmall",
		"Zsmall",
		/* 300 */
		"colonmonetary",
		"onefitted",
		"rupiah",
		"Tildesmall",
		"exclamdownsmall",
		"centoldstyle",
		"Lslashsmall",
		"Scaronsmall",
		"Zcaronsmall",
		"Dieresissmall",
		"Brevesmall",
		"Caronsmall",
		"Dotaccentsmall",
		"Macronsmall",
		"iguredash",
		"hypheninferior",
		"goneksmall",
		"Ringsmall",
		"Cedillasmall",
		"questiondownsmall",
		/* 320 */
		"oneeighth",
		"hreeeighths",
		"fiveeighths",
		"seveneighths",
		"onethird",
		"twothirds",
		"zerosuperior",
		"foursuperior",
		"fivesuperior",
		"sixsuperior",
		"sevensuperior",
		"eightsuperior",
		"ninesuperior",
		"zeroinferior",
		"oneinferior",
		"twoinferior",
		"threeinferior",
		"fourinferior",
		"fiveinferior",
		"sixinferior",
		/* 340 */
		"seveninferior",
		"eightinferior",
		"nineinferior",
		"centinferior",
		"dollarinferior",
		"periodinferior",
		"commainferior",
		"Agravesmall",
		"Aacutesmall",
		"Acircumflexsmall",
		"Atildesmall",
		"Adieresissmall",
		"Aringsmall",
		"AEsmall",
		"Ccedillasmall",
		"Egravesmall",
		"Eacutesmall",
		"Ecircumflexsmall",
		"Edieresissmall",
		"Igravesmall",
		/* 360 */
		"Iacutesmall",
		"Icircumflexsmall",
		"Idieresissmall",
		"Ethsmall",
		"Ntildesmall",
		"Ogravesmall",
		"Oacutesmall",
		"Ocircumflexsmall",
		"Otildesmall",
		"Odieresissmall",
		"OEsmall",
		"Oslashsmall",
		"Ugravesmall",
		"Uacutesmall",
		"Ucircumflexsmall",
		"Udieresissmall",
		"Yacutesmall",
		"Thornsmall",
		"Ydieresissmall",
		"001.000",
		/* 380 */
		"001.001",
		"001.002",
		"001.003",
		"Black",
		"Bold",
		"Book",
		"Light",
		"Medium",
		"Regular",
		"Roman",
		/* 390 */
		"Semibold"
	};
	
	bool CFF_Number::isCardinal() const
	{
		switch (type)
		{
			case cff_varnt_Error:
			case cff_varnt_Real:
				return false;
			default:
				return true;
		}
	}
	
	double CFF_Number::toDouble() const
	{
		switch (type)
		{
			case cff_varnt_Real:
				return card * std::pow(10.0, exponent);
			default:
				return toCardinal();
		}

	}
	
	int CFF_Number::toCardinal() const
	{
		return card;
	}

	
	CFF::CFF() : m_bytes(), m_offsetSize(4)
	{
		for (int i = 0; i <= sid_last_std; ++i)
		{
			m_strings.append(stdStrings[i]);
			m_sids[stdStrings[i]] = i;
		}
	}
	
	
	CFF::CFF(const QByteArray& cff) : m_bytes(cff)
	{
		// read header
		m_offsetSize = cff[cff_offSize];
		uint pos = cff[cff_hdrSize];
		qDebug() << "cff header" << m_offsetSize << "starts" << pos;
		// read names
		m_names = readIndex(pos);
		// read top dicts
		QList<QByteArray> topDicts = readIndex(pos);
		for (int i = 0; i < m_names.length(); ++i)
		{
			QByteArray fontName = m_names[i];
			qDebug() << i << fontName;
			if (fontName.length() > 0 && fontName[0] != char(0))
			{
				m_fontTopDicts[fontName] = getDict(topDicts[i]);
				uint privLength = m_fontTopDicts[fontName][cff_dict_Private].array[0].toCardinal();
				uint privOffset = m_fontTopDicts[fontName][cff_dict_Private].array[1].toCardinal();
				getDict(readSegment(privOffset, privLength));
			}
		}
		// read strings
		for (int i = 0; i <= sid_last_std; ++i)
		{
			m_strings.append(stdStrings[i]);
		}
		m_strings.append(readIndex(pos));
		for (int i = 0; i < m_strings.length(); ++i)
		{
//			if ( i > sid_last_std)
//				qDebug() << i << strings[i];
			m_sids[m_strings[i]] = i;
		}
		// read global subroutines
		m_globalSubr = readIndex(pos);
	}
	
	
	QByteArray CFF::readSegment(uint pos, uint size) const
	{
		return QByteArray::fromRawData(m_bytes.data() + pos, size);
	}
	
	
	uint CFF::readCard(uint pos) const
	{
		return static_cast<uchar>(m_bytes[pos]) << 8 | static_cast<uchar>(m_bytes[pos+1]);
	}
	
	
	
	QMap<operator_type,CFF_Variant> CFF::getDict(const QByteArray& dict) const
	{
		QMap<uint,CFF_Variant> result;
		QList<CFF_Number> stack;
		
		uint pos = 0;
		while (pos < (uint) dict.length())
		{
			CFF_Number num = parseDictElement(dict, pos);
			if (num.type == cff_varnt_Operator)
			{
				if (stack.length() == 1)
				{
					result[num.card] = CFF_Variant(stack[0]);
				}
				else if (stack.length() > 0)
				{
					result[num.card] = CFF_Variant(stack);
				}
				else
				{
					/* error */
				}
				stack = QList<CFF_Number>();
			}
			else
			{
				stack.append(num);
			}
		}
		if (stack.length() > 0)
		{
			/* error */
		}
		// TODO: adapt variant type according to operator
		return result;
	}
	
	
	CFF_Number CFF::parseDictElement(const QByteArray& dict, uint& pos) const
	{
		uint code = dict[pos];
		switch (code)
		{
			case cff_dict_TwoBytes:
				code = (code << 8) | dict[++pos];
				break;
			case cff_dict_Card16:
			case cff_dict_Card32:
				return parseCard(dict, pos);
			case cff_dict_Real:
				return parseReal(dict, pos);

			default:
				if (code >= cff_dict_minOperand)
				{
					return parseCard(dict, pos);
				}
				break;
		}
		++pos;
		CFF_Number num;
		num.type = cff_varnt_Operator;
		num.card = code;
		qDebug() << "parsed operator" << cff_operator(code) << "(" << code << ")";
		return num;
	}
	
	
	CFF_Number CFF::parseCard(const QByteArray& dict, uint& pos) const
	{
		CFF_Number result;
		result.type = cff_varnt_Card;
		result.exponent = 0;
		uint start=pos;
		uchar b0 = dict[pos++];
		uchar b1,b2,b3,b4;
		if (b0 == cff_dict_Card16)
		{
			b1 = dict[pos++];
			b2 = dict[pos++];
			result.card = static_cast<qint16>((b1 << 8) | b2);
		}
		else if (b0 == cff_dict_Card32)
		{
			b1 = dict[pos++];
			b2 = dict[pos++];
			b3 = dict[pos++];
			b4 = dict[pos++];
			result.card = static_cast<qint32>((b1 << 24) | (b2 << 16) | (b3 << 8) | b4);
		}
		else if (b0 < cff_dict_minOperand)
		{
			/* error */
		}
		else if (b0 <= cff_dict_maxSmallCard)
		{
			result.card = b0 + cff_dict_biasSmallCard;
		}
		else if (b0 <= cff_dict_maxPosCard)
		{
			b1 = dict[pos++];
			result.card = (b0 - cff_dict_minPosCard) * 256 + b1 + cff_dict_biasPosCard;
		}
		else if (b0 <= cff_dict_maxNegCard)
		{
			b1 = dict[pos++];
			result.card = (cff_dict_minNegCard - b0) * 256 - b1 + cff_dict_biasNegCard;
		}
		else
		{
			/* error */
		}
		qDebug() << "parsed" << QByteArray::fromRawData(dict.data()+start, pos-start).toHex() << "to card " << result.card;
		return result;
	}
	
	
	CFF_Number CFF::parseReal(const QByteArray& dict, uint& pos) const
	{
		assert( dict[pos] == cff_dict_Real );
		uint start = pos++;

		CFF_Number result;
		result.type = cff_varnt_Real;
		result.card = 0;
		result.exponent = 0;
		bool upperNibble = false;
		bool haveSeenMinus = false;
		bool haveSeenE = false;;
		bool haveSeenEminus = false;
		int decimalPointAt = -1;
		uchar nibble, twoNibbles;
		do
		{
			// get nibble
			if (upperNibble)
			{
				nibble = twoNibbles & 0x0f;
				upperNibble = false;
			}
			else
			{
				twoNibbles = dict[pos++];
				nibble = twoNibbles >> 4;
				upperNibble = true;
			}
			
			// decode
			switch (nibble)
			{
				case cff_nibble_Minus:
					haveSeenMinus = !haveSeenMinus;
					break;
				case cff_nibble_Point:
					decimalPointAt = 0;
					break;
				case cff_nibble_PosExp:
					haveSeenE = true;
					break;
				case cff_nibble_NegExp:
					haveSeenEminus = true;
					break;
				case cff_nibble_End:
					break;
				default:
					if (nibble > cff_nibble_maxDigit)
					{
						/* error */
					}
					else if (haveSeenE || haveSeenEminus)
					{
						result.exponent *= 10;
						result.exponent += nibble;
					}
					else
					{
						result.card *= 10;
						result.card += nibble;
						if (decimalPointAt >= 0)
						{
							++decimalPointAt;
						}
					}
					break;
			}
		} while (nibble != cff_nibble_End);
			
		// finish
		if (haveSeenMinus)
			result.card = -result.card;
		if (haveSeenEminus)
			result.exponent = -result.exponent;
		if (decimalPointAt > 0)
			result.exponent -= decimalPointAt;
		qDebug() << "parsed" << QByteArray::fromRawData(dict.data()+start, pos-start).toHex() << "to real " << result.card << "E" << result.exponent;

		return result;
	}
	
	
	QList<QByteArray> CFF::readIndex(uint& pos) const
	{
		qDebug() << "read INDEX @" << pos;
		QList<QByteArray> result;
		uint N = readCard(pos);
		pos += 2;
		if (N == 0)
			return result;

		uint offSize = m_bytes[pos++];
		uint dataStart = pos + offSize * (N+1) - 1;
		qDebug() << "size" << N << "offsetsize" << offSize << "dataStart" << dataStart;
		uint start = 0;
		uint end = 0;
		for (uint c = 0; c < offSize; ++c)
		{
			start = start << 8 | (uchar) m_bytes[pos++];
		}
		start += dataStart;
		for (uint i = 0; i < N; ++i)
		{
			end = 0;
			for (uint c = 0; c < offSize; ++c)
			{
				end = end << 8 | (uchar) m_bytes[pos++];
			}
			end += dataStart;
			result.append(readSegment(start, end-start));
			start = end;
		}
		pos = end;
		qDebug() << "INDEX ends @" << end;
		return result;
	}
	
	
	QList<uint> CFF::readEncoding(uint& pos) const
	{
		QList<uint> result;
		for(int i = 0; i < 256; ++i)
			result.append(0);
		
		uchar format = m_bytes[pos++];
		uchar N = m_bytes[pos++];
		int gid;
		uchar code;
		switch (format)
		{
			case 0x0:
			case 0x80:
				for (gid = 1; gid <= N; ++gid)
				{
					code = m_bytes[pos++];
					if (result[code] == 0)
					{
						result[code] = gid;
					}
				}
				break;
			case 0x1:
			case 0x81:
				gid = 1;
				for (int i = 0; i < N; ++i)
				{
					uchar first = m_bytes[pos++];
					uchar nLeft = m_bytes[pos++];
					int lastCode = (int) first + (int) nLeft;
					for (int j = first; j <= lastCode; ++j)
					{
						if (result[j] == 0)
						{
							result[j] = gid;
						}
						++gid;
					}
				}
				break;
		}
		if (format >= 0x80)
		{
			uchar nSupplements = m_bytes[pos++];
			for (int i = 0; i < nSupplements; ++i)
			{
				code = m_bytes[pos++];
				gid = readCard(pos);
				pos += 2;
				result[code] = gid;
			}
		}
		return result;
	}
	
	
	QList<sid_type> CFF::readCharset(uint nGlyphs, uint& pos) const
	{
		QList<sid_type> result;
		result.append(0); // sid for .notdef
		
		uchar format = m_bytes[pos++];
		
		sid_type first;
		uchar nLeft1;
		uint nLeft2;
		
		switch (format)
		{
			case 0:
				for (uint gid = 1; gid < nGlyphs; ++gid)
				{
					result.append(readCard(pos));
					pos += 2;
				}
				break;
			case 1:
				while ((uint) result.length() < nGlyphs)
				{
					first = readCard(pos);
					pos += 2;
					nLeft1 = m_bytes[pos++];
					for (sid_type sid = first; sid <= first + nLeft1; ++sid)
					{
						result.append(sid);
					}
				}
				break;
			case 2:
				while ((uint) result.length() < nGlyphs)
				{
					first = readCard(pos);
					pos += 2;
					nLeft2 = readCard(pos);
					pos += 2;
					for (sid_type sid = first; sid <= first + nLeft2; ++sid)
					{
						result.append(sid);
					}
				}
				break;
		}
		return result;
	}
	
	
	QByteArray CFF::dump(const CFF_Variant& var) const
	{
		QByteArray result;
		switch(var.type)
		{
			case cff_varnt_Error:
				result += "ERROR";
				break;
			case cff_varnt_Card:
			case cff_varnt_Bool:
			case cff_varnt_SID:
				result += QByteArray::number(var.array[0].toCardinal());
				break;
			case cff_varnt_Real:
				result += QByteArray::number(var.array[0].toDouble());
				break;
			case cff_varnt_Operator:
				result += string(var.array[0].toCardinal());
				result += " (";
				result += QByteArray::number(var.array[0].toCardinal());
				result +=")";
				break;
			case cff_varnt_Delta:
				result += "Delta";
				/* fall through */
			case cff_varnt_Array:
				result += "[";
				for (int i = 0; i < var.array.length(); ++i)
				{
					result += QByteArray::number(var.array[i].toDouble());
					result += ", ";
				}
				result.chop(2);
				result += "]";
				break;
		}
		return result;
	}
	
	
	void CFF::dump()
	{
		qDebug() << "CFF" << m_fontTopDicts.count() << "fonts, size =" << m_bytes.size() << "offset size=" << m_offsetSize;
		QMap<QByteArray,QMap<operator_type,CFF_Variant> >::Iterator it;
		for (it = m_fontTopDicts.begin(); it != m_fontTopDicts.end(); ++it)
		{
			qDebug() << "Font" << it.key() << ":";
			QMap<operator_type, CFF_Variant>::Iterator it2;
			for (it2= it.value().begin(); it2 != it.value().end(); ++it2)
			{
				qDebug() << "\t" << cff_operator(it2.key()) << "=" << dump(it2.value());
			}
		}
	}
	
	
	static QByteArray num(uint n)
	{
		return QByteArray::number(n);
	}
	
	
	static void write(QDataStream& out, const QByteArray& data)
	{
		out.writeRawData(data.data(), data.length());
	}
	
	static void dumpData(const QList<QByteArray>& index, QDataStream& out, const QByteArray& indent)
	{
		for (int i = 0; i < index.length(); ++i)
		{
			write(out, indent);
			write(out, "<data idx='");
			write(out, num(i));
			write(out, "' length='");
			write(out, num(index[i].length()));
			write(out, "' >");
			write(out, index[i].toHex());
			write(out, "</data>\n");
		}
	}

	static void dumpStrings(const QList<QByteArray>& index, QDataStream& out, const QByteArray& indent)
	{
		for (int i = 0; i < index.length(); ++i)
		{
			write(out, indent);
			write(out, "<string idx='");
			write(out, num(i));
			write(out, "' length='");
			write(out, num(index[i].length()));
			write(out, "' >");
			write(out, index[i]);
			write(out, "</string>\n");
		}
	}

	static void dumpDict(const CFF& cff, const QMap<operator_type,CFF_Variant>& dict, QDataStream& out, const QByteArray& indent)
	{
		QMap<operator_type,CFF_Variant>::ConstIterator it;
		for (it = dict.cbegin(); it != dict.cend(); ++it)
		{
			write(out, indent);
			write(out, "<keyvalue key='");
			write(out, cff_operator(it.key()));
			write(out, "' value='");
			write(out, cff.dump(it.value()));
			write(out, "' />\n");
		}
	}
	
	void CFF::dump(QDataStream& out) const
	{
		write(out, "<CFF version='1.0' offsetSize='" + num(m_offsetSize) + "' >\n");
		for (int f = 0; f < m_names.length(); ++f)
		{
			QByteArray font = m_names[f];
			QMap<operator_type, CFF_Variant> topDict = m_fontTopDicts[font];
			write(out, "  <Font name='" + font + "' >\n");
			write(out, "    <TopDict>");
			dumpDict(*this, topDict, out, "      ");
			write(out, "    </TopDict>\n");
			uint pos;
			if (topDict.contains(cff_dict_Encoding))
			{
				uint encOffset = topDict[cff_dict_Encoding].array[0].toCardinal();
				if (encOffset > 4)
				{
					write(out, "    <Encoding>\n    ");
					pos = encOffset;
					QList<uint> enc = readEncoding(pos);
					write(out, readSegment(encOffset, pos-encOffset).toHex());
					write(out, "\n    </Encoding>\n");
				}
				else
					write(out, "    <Encoding predefined='" + num(encOffset) + "' />\n");
			}
			pos = topDict[cff_dict_CharStrings].array[0].toCardinal();
			QList<QByteArray> charStrings = readIndex(pos);
			uint nglyphs = charStrings.length();
			if (topDict.contains(cff_dict_charset))
			{
				uint charsetOffset = topDict[cff_dict_charset].array[0].toCardinal();
				if (charsetOffset > 4)
				{
					pos = charsetOffset;
					QList<sid_type> charset = readCharset(nglyphs, pos);
					write(out, "    <charset>\n");
					for (int i=0; i < charset.length(); ++i)
					{
						write(out, "      <char gid='" + num(i) + "' sid='" + num(charset[i]) + "' >");
						write(out, string(charset[i]) + "</char>\n");
					}
					write(out, "    </charset>\n");
				}
			}
			write(out, "    <CharStrings>\n");
			dumpData(charStrings, out, "      ");
			write(out, "    </CharStrings>\n");
			uint privateLength = topDict[cff_dict_Private].array[0].toCardinal();
			uint privateOffset = topDict[cff_dict_Private].array[1].toCardinal();
			write(out, "    <PrivateDict offset='" + num(privateOffset) + "' length='" + num(privateLength) + ">\n");
			QMap<operator_type,CFF_Variant> privateDict = getDict(readSegment(privateOffset, privateLength));
			dumpDict(*this, privateDict, out, "      ");
			write(out, "    </PrivateDict>\n");
			if (privateDict.contains(cff_dict_Subrs))
			{
				uint subrsOffset = privateDict[cff_dict_Subrs].array[0].toCardinal();
				pos = privateOffset + subrsOffset;
				QList<QByteArray> localSubrs = readIndex(pos);
				write(out, "    <LocalSubrs offset='" + num(subrsOffset) + "' length='" + num(pos-subrsOffset) + "' >\n");
				dumpData(localSubrs, out, "      ");
				write(out, "    </LocalSubrs>\n");
			}
			write(out, "  </Font>\n");
		}
		write(out, "  <Strings>\n");
		dumpStrings(m_strings, out, "    ");
		write(out, "  </Strings>\n");
		write(out, "  <GlobalSubrs>\n");
		dumpData(m_globalSubr, out, "    ");
		write(out, "  </GlobalSubrs>\n");
		write(out, "</CFF>\n");
	}
    
	/// encodes 'value' as exactly 'nBytes' bytes in Big Endian
	static QByteArray encodeBE(int nbytes, uint value)
	{
		QByteArray result;
		while (nbytes-- > 0)
		{
			result.prepend(static_cast<uchar>(value & 0xFF));
			value >>= 8;
		}
		return result;
	}
	
	
	/// calculates the required offset size to represent 'dataLength'
	static int requiredOffsetSize(int dataLength)
	{
		if (dataLength < 255)
			return 1;
		else if (dataLength < 65535)
			return 2;
		else if (dataLength < 0xFFFFFF)
			return 3;
		else
			return 4;
	}
	
	
	static void writeCard(QByteArray& bytes, int val)
	{
		int biasedVal = val - cff_dict_biasSmallCard;
		if (biasedVal >= cff_dict_minOperand && biasedVal <= cff_dict_maxSmallCard)
		{
			bytes.append((char) biasedVal);
			return;
		}
		if (val > 0)
		{
			biasedVal = val - cff_dict_biasPosCard;
			if (biasedVal >= 0 &&  biasedVal <= 1023)
			{
				biasedVal += (cff_dict_minPosCard << 8);
				bytes.append(encodeBE(2, biasedVal));
				qDebug() << "writeCard" << val << "as" << encodeBE(2, biasedVal).toHex();
				return;
			};
		}
		else
		{
			biasedVal = -val + cff_dict_biasNegCard;
			if (biasedVal >= 0 &&  biasedVal <= 1023)
			{
				biasedVal += (cff_dict_minNegCard << 8);
				bytes.append(encodeBE(2, biasedVal));
				qDebug() << "writeCard" << val << "as" << encodeBE(2, biasedVal).toHex();
				return;
			};
		}
		if (val >= -32768 && val <= 32767)
		{
			bytes.append((char) cff_dict_Card16);
			bytes.append(encodeBE(2, val));
		}
		else
		{
			bytes.append((char) cff_dict_Card32);
			bytes.append(encodeBE(4, val));
		}
	}
	
	
	/// creates one byte per digit (not ASCII!)
	static QByteArray bsdNibbles(long long val)
	{
		QByteArray result;
		if (val < 0)
			val = -val;
		
		do {
			result.prepend((char) (val % 10));
			val /= 10;
		}
		while (val != 0);
		
		return result;
	}
	
	
	static void writeReal(QByteArray& bytes, CFF_Number num)
	{
		QByteArray bsd;
		if (num.card < 0)
		{
			bsd.append((char) cff_nibble_Minus);
			bsd.append(bsdNibbles(-num.card));
		}
		else
		{
			bsd.append(bsdNibbles(num.card));
		}
		if (num.exponent < 0)
		{
			bsd.append((char) cff_nibble_NegExp);
			bsd.append(bsdNibbles(-num.exponent));
		}
		else if (num.exponent > 0)
		{
			bsd.append((char) cff_nibble_PosExp);
			bsd.append(bsdNibbles(num.exponent));
		}
		bsd.append((char) cff_nibble_End);
		if (bsd.length() % 2 == 1)
			bsd.append((char) cff_nibble_End);
		
		uint start = bytes.length();
		bytes.append((char) cff_dict_Real);
		for (int i= 0; i < bsd.length(); i += 2)
		{
			bytes.append((char) ((bsd[i] << 4) | bsd[i+1]));
		}
		qDebug() << "writeReal" << num.card << "E" << num.exponent << "as" << bytes.mid(start).toHex();
	}
	
	
	uint CFF::writeSegment(const QByteArray& data)
	{
		uint result = m_bytes.length();
		m_bytes.append(data);
		return result;
	}
	
	
	sid_type CFF::createSid(const QByteArray& str)
	{
		sid_type result;
		if (!m_sids.contains(str))
		{
			result = m_strings.length();
			m_strings.append(str);
			m_sids[str] = result;
			qDebug() << "new SID" << result << "for" << str;
		}
		else
		{
			result = m_sids[str];
		}
		return result;
	}
	
	
	uint CFF::writeTopDict(QByteArray name,
	                       QMap<operator_type, CFF_Variant> dict,
	                       QList<QByteArray> oldStrings,
	                       QHash<operator_type, uint>& patchAddresses)
	{
		m_offsetSize = 4;
		m_names.append(name);
		m_fontTopDicts[name] = dict;
		m_bytes.append((char) 1);
		m_bytes.append((char) 0);                     // format 1.0
		m_bytes.append((char) 4);                     // header length 4
		m_bytes.append((char) 4);                     // offsetSize 4
		
		// write Name index
		m_bytes.append(encodeBE(2,1));                  // count
		assert (name.length() < 255);
		m_bytes.append(encodeBE(1, 1));                 // offSize
		m_bytes.append(encodeBE(1, 1));                 // offset 1
		m_bytes.append(encodeBE(1, 1 + name.length())); // offset 2
		m_bytes.append(name);
		
		
		// write TopDict index
		QByteArray topDict = makeDict(dict, oldStrings, patchAddresses);
		int offSize = requiredOffsetSize(topDict.length());
		m_bytes.append(encodeBE(2, 1));                          // count
		m_bytes.append(encodeBE(1, offSize));                    // offSize
		m_bytes.append(encodeBE(offSize, 1));                    // offset 1
		m_bytes.append(encodeBE(offSize, 1 + topDict.length())); // offset 2
		uint start = m_bytes.size();
		m_bytes.append(topDict);
		return start;
	}
	
	
	QByteArray CFF::makeDict(QMap<operator_type, CFF_Variant> dict,
	                         QList<QByteArray> oldStrings,
	                         QHash<operator_type, uint>& patchAddresses)
	{
		QByteArray result;
		if (dict.contains(cff_dict_ROS))
		{
			// pull to front
			QList<CFF_Number> ros = dict[cff_dict_ROS].array;
			sid_type sid = ros[0].toCardinal();
			sid = createSid(oldStrings[sid]);
			writeCard(result, sid);
			sid = ros[1].toCardinal();
			sid = createSid(oldStrings[sid]);
			writeCard(result, sid);
			writeCard(result, ros[2].toCardinal());
			result.append(encodeBE(2, cff_dict_ROS));

		}
		if (dict.contains(cff_dict_SyntheticBase))
		{
			// pull to front
			writeCard(result, dict[cff_dict_ROS].array[0].toCardinal());
			result.append(encodeBE(2, cff_dict_SyntheticBase));
		}
		QMap<operator_type, CFF_Variant>::Iterator it;
		for (it = dict.begin(); it != dict.end(); ++it)
		{
			QList<CFF_Number> arr = it.value().array;
			switch (it.key())
			{
				case cff_dict_ROS:
				case cff_dict_SyntheticBase:
					/* already done */
					break;
				case cff_dict_charset:
				case cff_dict_Encoding:
				case cff_dict_CharStrings:
				case cff_dict_FDArray:
				case cff_dict_FDSelect:
				case cff_dict_Subrs:
					/* remember offset */
					patchAddresses[it.key()] = result.length();
					/* write 32 bit offset */
					result.append((char) cff_dict_Card32);
					result.append(encodeBE(4, arr[0].toCardinal()));
					break;
				case cff_dict_Private:
					/* remember offset */
					patchAddresses[it.key()] = result.length();
					/* write 32 bit length */
					result.append((char) cff_dict_Card32);
					result.append(encodeBE(4, arr[0].toCardinal()));
					/* write 32 bit offset */
					result.append((char) cff_dict_Card32);
					result.append(encodeBE(4, arr[1].toCardinal()));
					break;
				case cff_dict_version:
				case cff_dict_Notice:
				case cff_dict_Copyright:
				case cff_dict_FullName:
				case cff_dict_FamilyName:
				case cff_dict_Weight:
				case cff_dict_PostScript:
				case cff_dict_BaseFontName:
				case cff_dict_FontName:
				{
					/* write SID */
					sid_type sid = arr[0].toCardinal();
					sid = createSid(oldStrings[sid]);
					qDebug() << "writeDict SID" << arr[0].toCardinal() << "-->" << sid;
					writeCard(result, sid);
				}
					break;
				default:
					/* write numbers */
					for (int i = 0; i < arr.length(); ++i)
					{
						if (arr[i].type == cff_varnt_Real)
							writeReal(result, arr[i]);
						else
							writeCard(result, arr[i].toCardinal());
					}
					break;
			}
			// write operator
			if (it.key() >= 0x0c00)
			{
				result.append(encodeBE(2, it.key()));
			}
			else
			{
				result.append((char) it.key());
			}
		}
		return result;
	}
	
	
	void CFF::patch(QHash<operator_type, uint> patchPositions,
	                uint patchOffset,
	                operator_type op,
	                uint offset,
	                uint length)
	{
		if (patchPositions.contains(op))
		{
			uint pos = patchOffset + patchPositions[op];
			uchar c;
			switch (op)
			{
				case cff_dict_charset:
				case cff_dict_Encoding:
				case cff_dict_CharStrings:
				case cff_dict_Subrs:
				case cff_dict_FDArray:
				case cff_dict_FDSelect:
					assert (m_bytes[pos] == (char) cff_dict_Card32);
					++pos;
					m_bytes.replace(pos, 4, encodeBE(4, offset));
					qDebug() << "patch" << cff_operator(op) << "offset @" << pos << offset;
					break;
				case cff_dict_Private:
					c = m_bytes[pos];
					if (c == cff_dict_Card16)
					{
						if (length > 0)
						{
							m_bytes.replace(pos+1, 2, encodeBE(2, length));
							qDebug() << "patch priv short length @" << (pos+1) << length;
						}
						pos += 3;
					}
					else if (c == cff_dict_Card32)
					{
						if (length > 0)
						{
							m_bytes.replace(pos+1, 4, encodeBE(4, length));
							qDebug() << "patch priv length @" << (pos+1) << length;
						}
						pos += 5;
					}
					else if (c >= cff_dict_minOperand && c <= cff_dict_maxSmallCard)
					{
						pos += 1;
					}
					else if (c >= cff_dict_minPosCard && c <= cff_dict_maxNegCard)
					{
						pos += 2;
					}
					else
					{
						/* error */
					}
					assert (m_bytes[pos] == (char) cff_dict_Card32);
					++pos;
					m_bytes.replace(pos, 4, encodeBE(4, offset));
					qDebug() << "patch priv offset @" << pos << offset;
					break;
				default:
					/* error */
					break;
			}
		}
	}
	
	
	QByteArray CFF::makeIndex(QList<QByteArray> data) const
	{
		QByteArray result;
		uint size = 0;
		for (int i = 0; i < data.length(); ++i)
		{
			size += data[i].size();
		}
		int offSize = requiredOffsetSize(size);
		result.append(encodeBE(2, data.length())); // count
		if (data.length() == 0)
			return result;
		
		result.append(encodeBE(1, offSize));	   // offSize
		uint offset = 1;
		result.append(encodeBE(offSize, offset));  // offset 1
		for (int i=0; i < data.length(); ++i)
		{
			offset += data[i].length();
			result.append(encodeBE(offSize, offset));
		}
		for (int i=0; i < data.length(); ++i)
		{
			result.append(data[i]);
		}
		return result;
	}

	
	QByteArray CFF::makeCharset(QList<sid_type> sids) const
	{
		QByteArray result;
		// we won't bother with ranges for now
		result.append('\0'); // format 0
		for (int i = 1; i < sids.length(); ++i)
		{
			result.append(encodeBE(2, sids[i]));
		}
		return result;
	}
	
	
	QByteArray CFF::makeEncoding(QList<uint> encoding) const
	{
		QByteArray result;
#ifdef UNTESTET_CFF_MAKEENCODING
		QMap<uint, uchar> codes;
		QMap<uchar, uint> supplements;
		uint maxGid = 0;
		for (uchar c = 0; c < encoding.length(); ++c)
		{
			uint gid = encoding[c];
			if (gid > maxGid && gid < 256)
				maxGid = gid;
			
			if (gid != 0)
			{
				if (gid >= 256 || codes.contains(gid))
					supplements[c] = gid;
				else
					codes[gid] = c;
			}
		}
		result.append('\0');
		result.append((uchar) (maxGid+1));
		for (uint gid = 0; gid <= maxGid; ++gid);
		{
			if (codes.contains(gid))
				result.append(codes[gid]);
			else
				result.append('\0'); // this is probably not correct
		}
		QMap<uchar, uint>::Iterator it;
		result.append(supplements.count());
		for (it = supplements.begin(); it != supplements.end(); ++it)
		{
			result.append(it.key());
			result.append(encodeBE(2, it.value()));
		}
#else
		result.append((char) 0x80);			   // format 0 with supplements
		result.append((char) 0);				  // no encoded glyphs except supplements
		result.append((char) encoding.length());  // all supplements
		for (int c = 0; c < encoding.length(); ++c)
		{
			result.append((uchar)c);
			result.append(encodeBE(2, encoding[c]));
		}
#endif
		return result;
	}
	
	
	CFF CFF::extractSubset(uint faceIndex,
	                       QList<uint> cids) const
	{
		uint pos;
		
		// get top dict
		QByteArray fontName = m_names[faceIndex];
		QMap<operator_type, CFF_Variant> topDict = m_fontTopDicts[fontName];
		
		// get charstrings
		QList<QByteArray> charStrings;
		pos = topDict[cff_dict_CharStrings].array[0].toCardinal();
		charStrings = readIndex(pos);
		
		// get encoding (optional, maybe predefined 0-1)
		QList<uint> encoding;
		if (topDict.contains(cff_dict_Encoding))
		{
			uint offset = topDict[cff_dict_Encoding].array[0].toCardinal();
			if (offset > 1)
			{
				pos = offset;
				encoding = readEncoding(pos);
//				encodingBytes = readSegment(offset, pos-offset);
			}
			else
				encoding.append(offset);
		}
		
		// get charmap (optional, maybe predefined 0-2)
		QList<sid_type> charset;
		if (topDict.contains(cff_dict_charset))
		{
			uint offset = topDict[cff_dict_charset].array[0].toCardinal();
			if (offset > 2)
			{
				pos = offset;
				charset = readCharset(charStrings.count(), pos);
//				charsetBytes = readSegment(offset, pos-offset);
			}
			else
				charset.append(offset);
		}
		
		// get private dict
		QList<CFF_Number> lengthOffset = topDict[cff_dict_Private].array;
		QMap<operator_type, CFF_Variant> privateDict = getDict(readSegment(lengthOffset[1].toCardinal(), lengthOffset[0].toCardinal()));
		
		// get local subr (optional)
		QList<QByteArray> localSubrs;
		if (privateDict.contains(cff_dict_Subrs))
		{
			pos = lengthOffset[1].toCardinal() + privateDict[cff_dict_Subrs].array[0].toCardinal();
			localSubrs = readIndex(pos);
		}
		
		// now create new font
		CFF result;
		result.m_globalSubr = m_globalSubr;  // no changes
		
		// subset
		if (cids.length() > 0)
		{
			// normalize .notdef
			cids.removeAll(0);
			cids.prepend(0);
			
			// forget encoding
			topDict.remove(cff_dict_Encoding);
			encoding = QList<uint>();
		
			// new charset
			QList<sid_type> newCharset;
			// new charStrings
			QList<QByteArray> newCharStrings;

			for (int i = 0; i < cids.length(); ++i)
			{
				sid_type gid = cids[i];
				sid_type sid = charset[gid];
				if (sid < m_strings.length())
				{
					sid = result.createSid(m_strings[sid]);
				}
				newCharset.append(sid);
				newCharStrings.append(charStrings[gid]);
			}
			
			charset = newCharset;
			charStrings = newCharStrings;
		}
		else if (charset.length() > 1)
		{
			// copy over needed strings
			for (int i = 0; i < charset.length(); ++i)
			{
				sid_type cid = charset[i];
				if (cid < m_strings.length())
				{
					cid = result.createSid(m_strings[cid]);
				}
				charset[i] = cid;
			}
		}
		
		// create new private dict
		QHash<operator_type, uint> privatePatches;
		QByteArray privateBytes = result.makeDict(privateDict, m_strings, privatePatches);

		
		// write new header, name and topdict, remember offset positions for patching
		QHash<operator_type, uint> patchPositions;
		uint topDictOffset = result.writeTopDict(fontName, topDict, m_strings, patchPositions);
		
		// write strings
		// makeDict() needs to be called before this in order to create SIDs for used strings
		result.writeSegment(makeIndex(result.m_strings.mid(sid_last_std + 1)));
		
		// write global subr (required but maybe empty)
		result.writeSegment(makeIndex(m_globalSubr));

		// write encoding
		uint encodingOffset = encoding.size() > 1? result.writeSegment(makeEncoding(encoding)) : encoding.size() == 1? encoding[0] : 0;
		
		// write charset
		uint charsetOffset = charset.size() > 1? result.writeSegment(makeCharset(charset)) : charset.size() == 1? charset[0] : 0;
		
		// write charstrings
		uint charStringsOffset = result.writeSegment(makeIndex(charStrings));
		
		// write private dict
		uint privateOffset = result.writeSegment(privateBytes);
		
		// write local subr
		if (localSubrs.size() > 0)
		{
			uint localSubrOffset = result.writeSegment(makeIndex(localSubrs));

			result.patch(privatePatches, privateOffset, cff_dict_Subrs, localSubrOffset - privateOffset);
		}
		
		// patch topdict offset positions for charset, encoding, charstrings, private
		result.patch(patchPositions, topDictOffset, cff_dict_charset, charsetOffset);
		result.patch(patchPositions, topDictOffset, cff_dict_Encoding, encodingOffset);
		result.patch(patchPositions, topDictOffset, cff_dict_CharStrings, charStringsOffset);
		result.patch(patchPositions, topDictOffset, cff_dict_Private, privateOffset, privateBytes.length());
		
		// no FDArray and FDSelect yet
		
		return result;
	}
	
	
	
	QByteArray extractFace(const QByteArray& cff, int faceIndex)
	{
		return CFF(cff).extractSubset(faceIndex, QList<uint>()).data();
	}
	
	QByteArray subsetFace(const QByteArray& cff, QList<uint> cids)
	{
		return CFF(cff).extractSubset(0, cids).data();
	}
	
} // namespace
