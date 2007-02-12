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

template <class ENUM>
ENUM parseEnum(Xml_string str)
{
	return static_cast<ENUM>(str.toUInt());
}


#endif
