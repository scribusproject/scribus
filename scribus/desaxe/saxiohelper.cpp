
#include "saxiohelper.h"
#include "scfonts.h"
#include "fpointarray.h"


QString toXMLString(uint val)
{
	return QString::number(val);
}


QString toXMLString(int val)
{
	return QString::number(val);
}


QString toXMLString(double val)
{
	return QString::number(val);
}


QString toXMLString(const QString& val)
{
	return val;
}


QString toXMLString(const ScFace& val)
{
	return val.scName();
}


QString toXMLString(bool what)
{
	return what? "yes" : "no";
}

QString toXMLString(const FPointArray& path)
{
	QString result;
	bool hasPoint = false;
	double x, y, x1, y1, x2, y2, xe=0, ye=0; 
	uint i=0;
	while (i < path.size())
	{
		path.point(i++, &x, &y);
		if (x > 900000 && y > 900000)  // marker for closepath
		{
			result += "Z";
			hasPoint = false;
			continue;
		}
		if (!hasPoint || x != xe || y != ye) // start a subpath
		{
			result += "M";
			result += QString::number(x);
			result += " ";
			result += QString::number(y);
			result += "C";
			hasPoint = true;
		}
		else 
			result += " ";
		
		path.point(i++, &x1, &y1);
		path.point(i++, &x2, &y2);
		path.point(i++, &xe, &ye);
		result += QString::number(x1);
		result += " ";
		result += QString::number(y1);
		result += " ";
		result += QString::number(x2);
		result += " ";
		result += QString::number(y2);
		result += " ";
		result += QString::number(xe);
		result += " ";
		result += QString::number(ye);
	}
	return result;
}
