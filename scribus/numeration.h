#ifndef NUMERATION_H
#define NUMERATION_H

#include <QString>
#include <QStringList>
#include <QMap>

enum NumFormat
{
	Type_1_2_3,
	Type_1_2_3_ar,
	Type_i_ii_iii,
	Type_I_II_III,
	Type_a_b_c,
	Type_A_B_C,
	Type_Alphabet_ar,
	Type_Abjad_ar,
	Type_asterix,
	Type_CJK,
	Type_Hebrew,
	Type_None=99
};

enum NumerationRange
{
	NSRdocument,
	NSRsection, // deprecated, kept only to allow loading old documents
	NSRstory,
	NSRpage, // deprecated, kept only to allow loading old documents
	NSRframe // deprecated, kept only to allow loading old documents
};

class Numeration
{
public:
	Numeration() : numFormat(Type_1_2_3), asterix(QString()), lead('0') {}
	Numeration(NumFormat f) : numFormat(f), asterix("*") {}
	
	QString numString(int num) const;

	NumFormat numFormat;
	QString asterix;
	QChar lead;
	int len { 0 };
	NumerationRange range { NSRdocument };
	QString prefix;
	QString suffix;
	int start { 1 };
};

struct NumFormatPair
{
	NumFormat format;
	QString   str;
};

//struct used by ScribusDoc for storing numerations used in document
struct NumStruct
{
	QString m_name;
	QList<Numeration> m_nums;
	QList<int> m_counters;
	int m_lastlevel { -1 };
};

//util functions for use without Numeration class

//convert passed num to a string usable when saving file
QString fromNumToString(NumFormat format);
//convert passed num to a string usable in a combobox
QString fromNumToUserString(NumFormat format);
//convert string to its NumFormat representation
NumFormat fromStringToNum(const QString& str);
//convert user string to its NumFormat representation
NumFormat fromUserStringToNum(const QString& str);
//convert passed num to string using numeration style
QString getStringFromNum(NumFormat format, int num, QChar leadingChar='0', int charsLen=0);
//convert passed num to string with custom chars
QString getAsterixStringFromNum(int num, const QString& asterix, QChar leadingChar='_', int charsLen=0);
//return numeration name from type
QStringList getFormatList();

#endif // NUMERATION_H
