#ifndef SAXHELPER_H
#define SAXHELPER_H

#include "desaxe_conf.h"
#include <q3valuelist.h>
#include <q3valuestack.h>


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

Xml_string toXMLString(const Q3ValueList<double>& doublelist);
Xml_string toXMLString(const Q3ValueList<int>& intlist);

unsigned int parseUInt(const Xml_string& str);
int parseInt(const Xml_string& str);
unsigned long parseULong(const Xml_string& str);
long parseLong(const Xml_string& str);
double parseDouble(const Xml_string& str);
float parseFloat(const Xml_string& str);
bool parseBool(const Xml_string& str);
Q3ValueList<double> parseDoubleList(const Xml_string& str);
Q3ValueList<int> parseIntList(const Xml_string& str);
Q3ValueStack<int> parseIntStack(const Xml_string& str);


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
Q3ValueList<int> parse<Q3ValueList<int> >(const Xml_string& str)
{
	return parseIntList(str);
}

template<>
inline
Q3ValueList<double> parse<Q3ValueList<double> >(const Xml_string& str)
{
	return parseDoubleList(str);
}


#endif
