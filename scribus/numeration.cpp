#include <array>

#include "numeration.h"
#include "util.h"

static const std::array<NumFormatPair, 12> numFormatStringPairs = { {
	{ Type_1_2_3, QString("Type_1_2_3") },
	{ Type_1_2_3_ar, QString("Type_1_2_3_ar") },
	{ Type_i_ii_iii, QString("Type_i_ii_iii") },
	{ Type_I_II_III, QString("Type_I_II_III") },
	{ Type_a_b_c, QString("Type_a_b_c") },
	{ Type_A_B_C, QString("Type_A_B_C") },
	{ Type_Alphabet_ar, QString("Type_Alphabet_ar") },
	{ Type_Abjad_ar, QString("Type_Abjad_ar") },
	{ Type_Hebrew, QString("Type_Hebrew") },
	{ Type_asterix, QString("Type_asterix") },
	{ Type_CJK, QString("Type_CJK") },
	{ Type_None, QString("Type_None") },
}};

static const std::array<NumFormatPair, 12> numFormatUserStringPairs = { {
	{ Type_1_2_3, QString::fromLatin1("1, 2, 3, ...") },
	{ Type_1_2_3_ar, QString::fromUtf8("\xE2\x80\xAD١, ٢, ٣, ...\xE2\x80\xAC") },
	{ Type_i_ii_iii, QString::fromLatin1("i, ii, iii, ...") },
	{ Type_I_II_III, QString::fromLatin1("I, II, III, ...") },
	{ Type_a_b_c, QString::fromLatin1("a, b, c, ...") },
	{ Type_A_B_C, QString::fromLatin1("A, B, C, ...") },
	{ Type_Alphabet_ar, QString::fromUtf8("\xE2\x80\xADأ, ب, ت, ...\xE2\x80\xAC") },
	{ Type_Abjad_ar, QString::fromUtf8("\xE2\x80\xADأ, ب, ج, ...\xE2\x80\xAC") },
	{ Type_Hebrew, QString::fromUtf8("\xE2\x80\xADא, ב, ג, ...\xE2\x80\xAC") },
	{ Type_asterix, QString::fromLatin1("*") },
	{ Type_CJK, QString::fromLatin1("CJK") },
	{ Type_None, QString::fromLatin1("None") }
}};

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
	     << fromNumToUserString(Type_Alphabet_ar)
	     << fromNumToUserString(Type_Abjad_ar)
	     << fromNumToUserString(Type_Hebrew)
	     << fromNumToUserString(Type_asterix)
	     << fromNumToUserString(Type_CJK);
	return list;
}

QString Numeration::numString(int num) const
{
	if (numFormat == Type_asterix)
		return getAsterixStringFromNum(num, asterix, lead, len);

	return getStringFromNum(numFormat, num, lead, len);
}
