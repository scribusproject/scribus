#ifndef NUMERATION_H
#define NUMERATION_H

#include <QString>
#include <QStringList>
#include <QMap>

typedef enum 
{
	Type_1_2_3,
	Type_i_ii_iii,
	Type_I_II_III,
	Type_a_b_c,
	Type_A_B_C,
	Type_asterix,
	Type_None=99
} NumFormat;

typedef enum {
	NSRdocument,
	NSRsection,
	NSRstory,
	NSRpage,
	NSRframe
//	NSRblock //used for contignous numeration eg. paragraphs - paragraph without numbering reset counter
} NumerationRange;

class Numeration
{
public:
	Numeration() : numFormat(Type_1_2_3), asterix(QString()), lead('0'), len(0), range(NSRdocument), prefix(QString()), suffix(QString()), start(1) {}
	Numeration(NumFormat f) : numFormat(f), asterix("*") {}
	const QString numString(const int num);

	NumFormat numFormat;
	QString asterix;
	QChar lead;
	int len;
	NumerationRange range;
	QString prefix;
	QString suffix;
	int start;
};

//struct used by ScribusDoc for storing numerations used in document
typedef struct {
	QString m_name;
	QList<Numeration> m_nums;
	QList<int> m_counters;
	int m_lastlevel;
} NumStruct;

//util functions for use without Numeration class
//convert passed num to string using numeration style
const QString getStringFromNum(NumFormat format, int num, const QChar leadingChar='0', const int charsLen=0);
//convert passed num to string with custom chars
const QString getAsterixStringFromNum(int num, QString asterix, const QChar leadingChar='_', const int charsLen=0);
//return numeration name from type
const QString getFormatName(int format);
const QStringList getFormatList();

#endif // NUMERATION_H
