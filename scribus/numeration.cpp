#include "numeration.h"
#include "util.h"

QString getStringFromNum(NumFormat format, int num, QChar leadingChar, int charsLen)
{
	QString str = getStringFromSequence(format, num);
	if (charsLen > str.length())
		str = str.rightJustified(charsLen, leadingChar);
	return str;
}

QString getAsterixStringFromNum(int num, QString asterix, QChar leadingChar, int charsLen)
{
	QString str = getStringFromSequence(Type_asterix, num, asterix);
	if (charsLen > str.length())
		str = str.rightJustified(charsLen, leadingChar);
	return str;
}

QStringList getFormatList()
{
	QStringList list;
	list << QString::fromLatin1("1, 2, 3, ...")
	     // U+202D...U+202C to force the string to be displayed LTR
	     << QString::fromUtf8("\xE2\x80\xAD١, ٢, ٣, ...\xE2\x80\xAC")
	     << QString::fromLatin1("i, ii, iii, ...")
	     << QString::fromLatin1("I, II, III, ...")
	     << QString::fromLatin1("a, b, c, ...")
	     << QString::fromLatin1("A, B, C, ...")
	     << QString::fromUtf8("\xE2\x80\xADأ, ب, ت, ...\xE2\x80\xAC")
	     << QString::fromUtf8("\xE2\x80\xADأ, ب, ج, ...\xE2\x80\xAC")
	     << QString::fromLatin1("*")
	     << QString::fromLatin1("CJK");
	return list;
}

QString Numeration::numString(int num) const
{
	if (numFormat == Type_asterix)
		return getAsterixStringFromNum(num, asterix, lead, len);

	return getStringFromNum(numFormat, num, lead, len);
}
