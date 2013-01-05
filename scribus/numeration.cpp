#include "numeration.h"
#include "util.h"

const QString getStringFromNum(NumFormat format, int num, const QChar leadingChar, const int charsLen)
{
	QString str = getStringFromSequence(format, num);
	if (charsLen > str.length())
		str = str.rightJustified(charsLen, leadingChar);
	return str;
}

const QString getAsterixStringFromNum(int num, QString asterix, const QChar leadingChar, const int charsLen)
{
	QString str = getStringFromSequence(Type_asterix, num, asterix);
	if (charsLen > str.length())
		str = str.rightJustified(charsLen, leadingChar);
	return str;
}

const QString getFormatName(int format)
{
	QString name = QString();
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
	Q_ASSERT(!name.isEmpty());
	return name;
}

const QStringList getFormatList()
{
	QStringList list;
	list << "1_2_3" << "i_ii_iii" << "I_II_III" << "a_b_c" << "A_B_C" << "*";
	return list;
}

const QString Numeration::numString(const int num)
{
	if (numFormat == Type_asterix)
		return getAsterixStringFromNum(num, asterix, lead, len);

	return getStringFromNum(numFormat, num, lead, len);
	
}
