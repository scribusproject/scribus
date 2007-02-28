
#include "saxiohelper.h"
#include "saxio.h"
#include "scfonts.h"
#include "fpointarray.h"




const Xml_string SaxIO::saxxDefaultElem("");



Xml_string mkXMLName(QString any)
{
	QString result("");
	for (unsigned int i=0; i < any.length(); ++i)
	{
		unsigned int ch = any[i].unicode();
		if (ch == ':' || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z')
			|| (0xC0 <= ch && ch <= 0xD6) || (0xD8 <= ch && ch <= 0xF6)
			|| (0xF8 <= ch && ch <= 0x2FF) || (0x370 <= ch && ch <= 0x37D)
			|| (0x37F <= ch && ch <= 0x1FFF) || (0x200C <= ch && ch <= 0x200D)
			|| (0x2070 <= ch && ch <= 0x218F) || (0x2C00 <= ch && ch <= 0x2FEF)
			|| (0x3001 <= ch && ch <= 0xD7FF) || (0xF900 <= ch && ch <= 0xFDCF)
			|| (0xFDF0 <= ch && ch <= 0xFFFD) || ch > 0xFFFF
			|| (i > 0 && (ch == '-' || ch == '.' || ('0' <= ch && ch <= '9') || ch == 0xB7
						  || (0x300 <= ch && ch <= 0x36F) || (0x203F <= ch && ch <= 0x2040) )))
		{
			result += ch;
		}
		else
		{
			result += '_';
			result += ('0' + ch >> 12 & 15);
			result += ('0' + ch >>  8 & 15);
			result += ('0' + ch >>  4 & 15);
			result += ('0' + ch       & 15);
		}
	}
	return result;
}

/*
 NameStartChar ::= ":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D] 
 | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]
 NameChar	   ::=   	NameStartChar | "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
*/


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
