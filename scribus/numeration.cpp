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

QString getFormatName(int format)
{
	QString name;
	if (format == Type_1_2_3)
		name = "1_2_3";
	else if (format == Type_i_ii_iii)
		name = "i_ii_iii";
	else if (format == Type_I_II_III)
		name = "I_II_III";
	else if (format == Type_a_b_c)
		name = "a_b_c";
	else if (format == Type_A_B_C)
		name = "A_B_C";
	else if (format == Type_asterix)
		name = "*";
	else if (format == Type_CJK)
		name = "CJK";
	Q_ASSERT(!name.isEmpty());
	return name;
}

QStringList getFormatList()
{
	QStringList list;
	list << "1_2_3" << "i_ii_iii" << "I_II_III" << "a_b_c" << "A_B_C" << "*" << "CJK";
	return list;
}

QStringList getFormatListTr()
{
	QStringList list;
	list << QObject::tr("1, 2, 3, ...") << QObject::tr("i, ii, iii, ...") << QObject::tr("I, II, III, ...") << QObject::tr("a, b, c, ...") << QObject::tr("A, B, C, ...") << QObject::tr("*") << QObject::tr("CJK");
	return list;
}

QString Numeration::numString(int num) const
{
	if (numFormat == Type_asterix)
		return getAsterixStringFromNum(num, asterix, lead, len);

	return getStringFromNum(numFormat, num, lead, len);
}
