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

QStringList getFormatListTr()
{
	QStringList list;
	list << QObject::tr("1, 2, 3, ...")
	     // U+202D...U+202C to force the string to be displayed LTR
	     << QObject::trUtf8("\u202d١, ٢, ٣, ...\u202c")
	     << QObject::tr("i, ii, iii, ...")
	     << QObject::tr("I, II, III, ...")
	     << QObject::tr("a, b, c, ...")
	     << QObject::tr("A, B, C, ...")
	     << QObject::trUtf8("\u202dأ, ب, ت, ...\u202c")
	     << QObject::trUtf8("\u202dأ, ب, ج, ...\u202c")
	     << QObject::tr("*")
	     << QObject::tr("CJK");
	return list;
}

QString Numeration::numString(int num) const
{
	if (numFormat == Type_asterix)
		return getAsterixStringFromNum(num, asterix, lead, len);

	return getStringFromNum(numFormat, num, lead, len);
}
