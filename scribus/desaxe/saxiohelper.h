#ifndef SAXHELPER_H
#define SAXHELPER_H

#include "desaxe_conf.h"
#include <qvaluelist.h>
#include <qvaluestack.h>


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

Xml_string toXMLString(const QValueList<double>& doublelist);
Xml_string toXMLString(const QValueList<int>& intlist);

unsigned int parseUInt(Xml_string str);
int parseInt(Xml_string str);
unsigned long parseULong(Xml_string str);
long parseLong(Xml_string str);
double parseDouble(Xml_string str);
float parseFloat(Xml_string str);
bool parseBool(Xml_string str);
QValueList<double> parseDoubleList(Xml_string str);
QValueList<int> parseIntList(Xml_string str);
QValueStack<int> parseIntStack(Xml_string str);


Xml_string mkXMLName(QString any);


template<class ENUM>
inline
ENUM parseEnum(Xml_string str)
{
	return static_cast<ENUM>(str.toUInt());
}


template<class DATA>
inline
DATA parse(Xml_string str)
{
	return static_cast<DATA>(str);
}

template<>
inline
unsigned int parse<unsigned int>(Xml_string str)
{
	return parseUInt(str);
}

template<>
inline
int parse<int>(Xml_string str)
{
	return parseInt(str);
}

template<>
inline
unsigned long parse<unsigned long>(Xml_string str)
{
	return parseULong(str);
}

template<>
inline
long parse<long>(Xml_string str)
{
	return parseLong(str);
}

template<>
inline
double parse<double>(Xml_string str)
{
	return parseDouble(str);
}

template<>
inline
float parse<float>(Xml_string str)
{
	return parseFloat(str);
}

template<>
inline
bool parse<bool>(Xml_string str)
{
	return parseBool(str);
}

template<>
inline
QValueList<int> parse<QValueList<int> >(Xml_string str)
{
	return parseIntList(str);
}

template<>
inline
QValueList<double> parse<QValueList<double> >(Xml_string str)
{
	return parseDoubleList(str);
}


#endif
