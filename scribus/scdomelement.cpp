/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scclocale.h"
#include "scdomelement.h"

ScDomElement::ScDomElement() : QDomElement()
{

}

ScDomElement::ScDomElement(const QDomElement &elem) : QDomElement(elem)
{

}

int ScDomElement::valueAsInt (const QString& attrName, int def) const
{
	int retValue = def;
	QString attr = attribute(attrName);
	if (!attr.isEmpty())
	{
		bool success = false;
		int intVal = attr.toInt(&success);
		if (success)
			retValue = intVal;
	}
	return retValue;
}

int ScDomElement::valueAsInt (const QString& attrName, int min, int max, int def) const
{
	int value = valueAsInt(attrName, def);
	return qMin(max, qMax(value, min));
}

uint ScDomElement::valueAsUInt (const QString& attrName, uint def) const
{
	uint retValue = def;
	QString attr = attribute(attrName);
	if (!attr.isEmpty())
	{
		bool success = false;
		uint intVal = attr.toUInt(&success);
		if (success)
			retValue = intVal;
	}
	return retValue;
}

uint ScDomElement::valueAsUInt (const QString& attrName, uint min, uint max, uint def) const
{
	uint value = valueAsUInt(attrName, def);
	return qMin(max, qMax(value, min));
}

double ScDomElement::valueAsDouble (const QString& attrName, double def) const
{
	double retValue = def;
	QString attr = attribute(attrName);
	if (!attr.isEmpty())
		retValue = ScCLocale::toDoubleC(attr, def);
	return retValue;
}

double ScDomElement::valueAsDouble (const QString& attrName, double min, double max, double def) const
{
	double value = valueAsDouble(attrName, def);
	return qMin(max, qMax(value, min));
}
