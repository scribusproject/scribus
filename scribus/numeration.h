#ifndef NUMERATION_H
#define NUMERATION_H

#include <QString>
#include <QStringList>
#include <QMap>

#include "scribusapi.h"

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
	Type_Bengali,
	Type_Burmese,
	Type_CJK,
	Type_Devanagari,
	Type_Greek,
	Type_Gujarati,
	Type_Gurmukhi,
	Type_Hebrew,
	Type_Kannada,
	Type_Khmer,
	Type_Lao,
	Type_Malayalam,
	Type_Odia,
	Type_Tamil,
	Type_Telugu,
	Type_Thai,
	Type_Tibetan,
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
	Numeration() = default;
	Numeration(NumFormat f) : numFormat(f), lead() {}
	
	QString numString(int num) const;

	NumFormat numFormat { Type_1_2_3 };
	QString asterix { "*" };
	QChar lead { '0' };
	int len { 0 };
	NumerationRange range { NSRdocument };
	QString prefix;
	QString suffix;
	int start { 1 };
};

struct NumFormatPair
{
	NumFormat format;
	QString str;
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
QString SCRIBUS_API fromNumToString(NumFormat format);
//convert passed num to a string usable in a combobox
QString SCRIBUS_API fromNumToUserString(NumFormat format);
//convert string to its NumFormat representation
NumFormat SCRIBUS_API fromStringToNum(const QString& str);
//convert user string to its NumFormat representation
NumFormat SCRIBUS_API fromUserStringToNum(const QString& str);
//convert passed num to string using numeration style
QString SCRIBUS_API getStringFromNum(NumFormat format, int num, QChar leadingChar='0', int charsLen=0);
//convert passed num to string with custom chars
QString SCRIBUS_API getAsterixStringFromNum(int num, const QString& asterix, QChar leadingChar='_', int charsLen=0);
//return numeration name from type
QStringList SCRIBUS_API getFormatList();

#endif // NUMERATION_H
