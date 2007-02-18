#ifndef SAXHELPER_H
#define SAXHELPER_H

#include "desaxe_conf.h"

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

unsigned int parseUInt(Xml_string str);
int parseInt(Xml_string str);
unsigned long parseULong(Xml_string str);
long parseLong(Xml_string str);
double parseDouble(Xml_string str);
float parseFloat(Xml_string str);
bool parseBool(Xml_string str);

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


#endif
