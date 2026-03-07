#include <array>

#include "numeration.h"
#include "util.h"

static const std::array<NumFormatPair, 27> numFormatStringPairs =
{
	{
		{ Type_1_2_3, QString("Type_1_2_3") },
		{ Type_1_2_3_ar, QString("Type_1_2_3_ar") },
		{ Type_i_ii_iii, QString("Type_i_ii_iii") },
		{ Type_I_II_III, QString("Type_I_II_III") },
		{ Type_a_b_c, QString("Type_a_b_c") },
		{ Type_A_B_C, QString("Type_A_B_C") },
		{ Type_Abjad_ar, QString("Type_Abjad_ar") },
		{ Type_Alphabet_ar, QString("Type_Alphabet_ar") },
		{ Type_asterix, QString("Type_asterix") },
		{ Type_Bengali, QString("Type_Bengali") },
		{ Type_Burmese, QString("Type_Burmese") },
		{ Type_CJK, QString("Type_CJK") },
		{ Type_Devanagari, QString("Type_Devanagari") },
		{ Type_Greek, QString("Type_Greek") },
		{ Type_Gujarati, QString("Type_Gujarati") },
		{ Type_Gurmukhi, QString("Type_Gurmukhi") },
		{ Type_Hebrew, QString("Type_Hebrew") },
		{ Type_Kannada, QString("Type_Kannada") },
		{ Type_Khmer, QString("Type_Khmer") },
		{ Type_Lao, QString("Type_Lao") },
		{ Type_Malayalam, QString("Type_Malayalam") },
		{ Type_Odia, QString("Type_Odia") },
		{ Type_Tamil, QString("Type_Tamil") },
		{ Type_Telugu, QString("Type_Telugu") },
		{ Type_Thai, QString("Type_Thai") },
		{ Type_Tibetan, QString("Type_Tibetan") },
		{ Type_None, QString("Type_None") }
	}
};

static const std::array<NumFormatPair, 27> numFormatUserStringPairs =
{
	{
		{ Type_1_2_3, QString::fromLatin1("1, 2, 3, ...") },
		{ Type_1_2_3_ar, QString::fromUtf8("\xE2\x80\xAD١, ٢, ٣, ...\xE2\x80\xAC") },
		{ Type_i_ii_iii, QString::fromLatin1("i, ii, iii, ...") },
		{ Type_I_II_III, QString::fromLatin1("I, II, III, ...") },
		{ Type_a_b_c, QString::fromLatin1("a, b, c, ...") },
		{ Type_A_B_C, QString::fromLatin1("A, B, C, ...") },
		{ Type_Abjad_ar, QObject::tr("Arabic: ") + QString::fromUtf8("\xE2\x80\xADأ, ب, ج, ...\xE2\x80\xAC") },
		{ Type_Alphabet_ar, QObject::tr("Arabic: ") + QString::fromUtf8("\xE2\x80\xADأ, ب, ت, ...\xE2\x80\xAC") },
		{ Type_asterix, QString::fromLatin1("*") },
		{ Type_Bengali, QObject::tr("Bengali: ") + QString::fromUtf8("১, ২, ৩, ...")  },
		{ Type_Burmese, QObject::tr("Burmese: ") + QString::fromUtf8("၁, ၂, ၃, ...")  },
		{ Type_CJK, QString::fromLatin1("CJK") },
		{ Type_Devanagari, QObject::tr("Devanagari: ") + QString::fromUtf8("१, २, ३, ...") },
		{ Type_Greek, QObject::tr("Greek: ") + QString::fromUtf8("α, β, γ, ...")},
		{ Type_Gujarati, QObject::tr("Gujarati: ") + QString::fromUtf8("૧, ૨, ૩, ...")  },
		{ Type_Gurmukhi, QObject::tr("Gurmukhi: ") + QString::fromUtf8("੧, ੨, ੩, ...")  },
		{ Type_Hebrew, QObject::tr("Hebrew: ") + QString::fromUtf8("\xE2\x80\xADא, ב, ג, ...\xE2\x80\xAC") },
		{ Type_Kannada, QObject::tr("Kannada: ") + QString::fromUtf8("೧, ೨, ೩, ...") },
		{ Type_Khmer, QObject::tr("Khmer: ") + QString::fromUtf8("១, ២, ៣, ...") },
		{ Type_Lao, QObject::tr("Lao: ") + QString::fromUtf8("໑, ໒, ໓, ...") },
		{ Type_Malayalam, QObject::tr("Malayalam: ") + QString::fromUtf8("൧, ൨, ൩, ...") },
		{ Type_Odia, QObject::tr("Odia: ") + QString::fromUtf8("୧, ୨, ୩, ...") },
		{ Type_Tamil, QObject::tr("Tamil: ") + QString::fromUtf8("௧, ௨, ௩, ...") },
		{ Type_Telugu, QObject::tr("Telugu: ") + QString::fromUtf8("౧, ౨, ౩, ...") },
		{ Type_Thai, QObject::tr("Thai: ") + QString::fromUtf8("๑, ๒, ๓, ...") },
		{ Type_Tibetan, QObject::tr("Tibetan: ") + QString::fromUtf8("༡, ༢, ༣, ...") },
		{ Type_None, QString::fromLatin1("None") }
	}
};

QString fromNumToString(NumFormat format)
{
	size_t size = numFormatStringPairs.size();
	for (size_t i = 0; i < size; ++i)
	{
		const NumFormatPair& fmtPair = numFormatStringPairs[i];
		if (fmtPair.format == format)
			return fmtPair.str;
	}

	return numFormatStringPairs[0].str; // Type_1_2_3
}

QString fromNumToUserString(NumFormat format)
{
	size_t size = numFormatUserStringPairs.size();
	for (size_t i = 0; i < size; ++i)
	{
		const NumFormatPair& fmtPair = numFormatUserStringPairs[i];
		if (fmtPair.format == format)
			return fmtPair.str;
	}

	return numFormatUserStringPairs[0].str; // Type_1_2_3
}

NumFormat fromStringToNum(const QString& str)
{
	size_t size = numFormatStringPairs.size();
	for (size_t i = 0; i < size; ++i)
	{
		const NumFormatPair& fmtPair = numFormatStringPairs[i];
		if (fmtPair.str == str)
			return fmtPair.format;
	}

	return numFormatStringPairs[0].format; // Type_1_2_3
}

NumFormat fromUserStringToNum(const QString& str)
{
	size_t size = numFormatUserStringPairs.size();
	for (size_t i = 0; i < size; ++i)
	{
		const NumFormatPair& fmtPair = numFormatUserStringPairs[i];
		if (fmtPair.str == str)
			return fmtPair.format;
	}

	return numFormatUserStringPairs[0].format; // Type_1_2_3
}

QString getStringFromNum(NumFormat format, int num, QChar leadingChar, int charsLen)
{
	QString str = getStringFromSequence(format, num);
	if (charsLen > str.length())
		str = str.rightJustified(charsLen, leadingChar);
	return str;
}

QString getAsterixStringFromNum(int num, const QString& asterix, QChar leadingChar, int charsLen)
{
	QString str = getStringFromSequence(Type_asterix, num, asterix);
	if (charsLen > str.length())
		str = str.rightJustified(charsLen, leadingChar);
	return str;
}

QStringList getFormatList()
{
	QStringList list;
	list << fromNumToUserString(Type_1_2_3)
		 << fromNumToUserString(Type_1_2_3_ar)
		 << fromNumToUserString(Type_i_ii_iii)
		 << fromNumToUserString(Type_I_II_III)
		 << fromNumToUserString(Type_a_b_c)
		 << fromNumToUserString(Type_A_B_C)
		 << fromNumToUserString(Type_Abjad_ar)
		 << fromNumToUserString(Type_Alphabet_ar)
		 << fromNumToUserString(Type_asterix)
		 << fromNumToUserString(Type_Bengali)
		 << fromNumToUserString(Type_Burmese)
		 << fromNumToUserString(Type_CJK)
		 << fromNumToUserString(Type_Devanagari)
		 << fromNumToUserString(Type_Greek)
		 << fromNumToUserString(Type_Gujarati)
		 << fromNumToUserString(Type_Gurmukhi)
		 << fromNumToUserString(Type_Hebrew)
		 << fromNumToUserString(Type_Kannada)
		 << fromNumToUserString(Type_Khmer)
		 << fromNumToUserString(Type_Lao)
		 << fromNumToUserString(Type_Malayalam)
		 << fromNumToUserString(Type_Odia)
		 << fromNumToUserString(Type_Tamil)
		 << fromNumToUserString(Type_Telugu)
		 << fromNumToUserString(Type_Thai)
		 << fromNumToUserString(Type_Tibetan)
			;
	return list;
}

QString Numeration::numString(int num) const
{
	if (numFormat == Type_asterix)
		return getAsterixStringFromNum(num, asterix, lead, len);

	return getStringFromNum(numFormat, num, lead, len);
}
