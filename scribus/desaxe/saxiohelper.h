/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SAXHELPER_H
#define SAXHELPER_H

#include "desaxe_conf.h"
#include <QList>
#include <QStack>
#include <QStringList>
#include <QVector>

class ScFace;
class FPointArray;

Xml_string toXMLString(unsigned int val);
Xml_string toXMLString(int val);
Xml_string toXMLString(unsigned long val);
Xml_string toXMLString(long val);
Xml_string toXMLString(double val);
Xml_string toXMLString(float val);
Xml_string toXMLString(bool val);
Xml_string toXMLString(const Xml_string& val);

Xml_string toXMLString(const ScFace& val);
Xml_string toXMLString(const FPointArray& path);

Xml_string toXMLString(const QList<double>& doublelist);
Xml_string toXMLString(const QList<float>& floatlist);
Xml_string toXMLString(const QList<int>& intlist);
Xml_string toXMLString(const QVector<double>& doublelist);
Xml_string toXMLString(const QVector<float>& floatlist);
Xml_string toXMLString(const QVector<int>& intlist);
Xml_string toXMLString(const QStringList& list);

unsigned int parseUInt(const Xml_string& str);
int parseInt(const Xml_string& str);
unsigned long parseULong(const Xml_string& str);
long parseLong(const Xml_string& str);
double parseDouble(const Xml_string& str);
float parseFloat(const Xml_string& str);
bool parseBool(const Xml_string& str);
QStringList parseStringList(const Xml_string& str);
QList<double> parseDoubleList(const Xml_string& str);
QList<int> parseIntList(const Xml_string& str);
QVector<double> parseDoubleVector(const Xml_string& str);
QVector<int> parseIntVector(const Xml_string& str);
QStack<int> parseIntStack(const Xml_string& str);


Xml_string mkXMLName(const QString& any);


template<class ENUM>
inline
ENUM parseEnum(const Xml_string& str)
{
	return static_cast<ENUM>(str.toUInt());
}


template<class DATA>
inline
DATA parse(const Xml_string& str)
{
	return static_cast<DATA>(str);
}

template<>
inline
const Xml_string& parse<const Xml_string&>(const Xml_string& str)
{
	return str;
}

template<>
inline
Xml_string parse<Xml_string>(const Xml_string& str)
{
	return str;
}

template<>
inline
unsigned int parse<unsigned int>(const Xml_string& str)
{
	return parseUInt(str);
}

template<>
inline
int parse<int>(const Xml_string& str)
{
	return parseInt(str);
}

template<>
inline
unsigned long parse<unsigned long>(const Xml_string& str)
{
	return parseULong(str);
}

template<>
inline
long parse<long>(const Xml_string& str)
{
	return parseLong(str);
}

template<>
inline
double parse<double>(const Xml_string& str)
{
	return parseDouble(str);
}

template<>
inline
float parse<float>(const Xml_string& str)
{
	return parseFloat(str);
}

template<>
inline
bool parse<bool>(const Xml_string& str)
{
	return parseBool(str);
}

template<>
inline
QStringList parse<QStringList>(const Xml_string& str)
{
	return parseStringList(str);
}

template<>
inline
QList<int> parse<QList<int> >(const Xml_string& str)
{
	return parseIntList(str);
}

template<>
inline
QList<double> parse<QList<double> >(const Xml_string& str)
{
	return parseDoubleList(str);
}


#endif
