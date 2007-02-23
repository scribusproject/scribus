
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


Xml_string toXMLString(const QValueList<double>& doublelist)
{
	QString result = "";
	QValueList<double>::const_iterator dax;
	for (dax = doublelist.begin(); dax != doublelist.end(); ++dax)
		result += toXMLString(*dax) + " ";
	return result;
}


QValueList<double> parseDoubleList(Xml_string str)
{
	QStringList strlist = QStringList::split(' ', str);
	QValueList<double> result;
	for (QStringList::iterator it=strlist.begin(); it != strlist.end(); ++it)
		result.append(parseDouble(*it));
	return result;
}


Xml_string toXMLString(const QValueList<int>& intlist)
{
	QString result = "";
	QValueList<int>::const_iterator dax;
	for (dax = intlist.begin(); dax != intlist.end(); ++dax)
		result += toXMLString(*dax) + " ";
	return result;
}


QValueList<int> parseIntList(Xml_string str)
{
	QStringList strlist = QStringList::split(' ', str);
	QValueList<int> result;
	for (QStringList::iterator it=strlist.begin(); it != strlist.end(); ++it)
		result.append(parseInt(*it));
	return result;
}


QValueStack<int> parseIntStack(Xml_string str)
{
	QStringList strlist = QStringList::split(' ', str);
	QValueStack<int> result;
	for (QStringList::iterator it=strlist.begin(); it != strlist.end(); ++it)
		result.append(parseInt(*it));
	return result;
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
