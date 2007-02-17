
#include "saxiohelper.h"
#include "saxio.h"
#include "scfonts.h"
#include "fpointarray.h"




const Xml_string SaxIO::saxxDefaultElem("");


// FIXME: this is too Qt dependent
Xml_string toXMLString(unsigned int val)
{
	return QString::number(val);
}

unsigned int parseUInt(Xml_string str)
{
	return str.toUInt();
}

Xml_string toXMLString(int val)
{
	return QString::number(val);
}


int parseInt(Xml_string str)
{
	return str.toInt();
}


Xml_string toXMLString(unsigned long val)
{
	return QString::number(val);
}

unsigned long parseULong(Xml_string str)
{
	return str.toULong();
}

Xml_string toXMLString(long val)
{
	return QString::number(val);
}


long parseLong(Xml_string str)
{
	return str.toLong();
}


Xml_string toXMLString(double val)
{
	return QString::number(val);
}


double parseDouble(Xml_string str)
{
	return str.toDouble();
}


Xml_string toXMLString(float val)
{
	return QString::number(val);
}


float parseFloat(Xml_string str)
{
	return str.toFloat();
}


Xml_string toXMLString(bool what)
{
	return what? "yes" : "no";
}


bool parseBool(Xml_string str)
{
	return str == "yes";
}


Xml_string toXMLString(const Xml_string& val)
{
	return val;
}



// FIXME: the following should not be in desaxe:
Xml_string toXMLString(const ScFace& val)
{
	return val.scName();
}


Xml_string toXMLString(const FPointArray& path)
{
	return path.svgPath();
}
