/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scxmlstreamreader.h"

#include "scclocale.h"

ScXmlStreamAttributes::ScXmlStreamAttributes(void) : QXmlStreamAttributes()
{

}

ScXmlStreamAttributes::ScXmlStreamAttributes(const QXmlStreamAttributes& attrs)
					 : QXmlStreamAttributes(attrs)
{

}

bool ScXmlStreamAttributes::valueAsBool (const char* attrName, bool def) const
{
	bool retValue = def;
	QStringRef att = value(QLatin1String(attrName));
	if (!att.isEmpty())
	{
		bool success = false;
		QString strVal = QString::fromRawData(att.constData(), att.length());
		int intVal = strVal.toInt(&success);
		if (success)
			retValue = static_cast<bool>(intVal);
	}
	return retValue;
}

bool ScXmlStreamAttributes::valueAsBool (const QString& attrName, bool def) const
{
	bool retValue = def;
	QStringRef att = value(attrName);
	if (!att.isEmpty())
	{
		bool success = false;
		QString strVal = QString::fromRawData(att.constData(), att.length());
		int intVal = strVal.toInt(&success);
		if (success)
			retValue = static_cast<bool>(intVal);
	}
	return retValue;
}

int ScXmlStreamAttributes::valueAsInt (const char* attrName, int def) const
{
	int retValue = def;
	QStringRef att = value(QLatin1String(attrName));
	if (!att.isEmpty())
	{
		bool success = false;
		QString strVal = QString::fromRawData(att.constData(), att.length());
		int intVal = strVal.toInt(&success);
		if (success)
			retValue = intVal;
	}
	return retValue;
}

int ScXmlStreamAttributes::valueAsInt (const QString& attrName, int def) const
{
	int retValue = def;
	QStringRef att = value(attrName);
	if (!att.isEmpty())
	{
		bool success = false;
		QString strVal = QString::fromRawData(att.constData(), att.length());
		int intVal = strVal.toInt(&success);
		if (success)
			retValue = intVal;
	}
	return retValue;
}

double ScXmlStreamAttributes::valueAsDouble (const char* attrName, double def) const
{
	double retValue = def;
	QStringRef att = value(QLatin1String(attrName));
	if (!att.isEmpty())
	{
		bool success = false;
		QString strVal = QString::fromRawData(att.constData(), att.length());
		retValue = ScCLocale::toDoubleC(strVal, def);
	}
	return retValue;
}

double ScXmlStreamAttributes::valueAsDouble (const QString& attrName, double def) const
{
	double retValue = def;
	QStringRef att = value(attrName);
	if (!att.isEmpty())
	{
		bool success = false;
		QString strVal = QString::fromRawData(att.constData(), att.length());
		retValue = ScCLocale::toDoubleC(strVal, def);
	}
	return retValue;
}

ScXmlStreamAttributes ScXmlStreamReader::scAttributes(void) const
{
	ScXmlStreamAttributes attrs(attributes());
	return attrs;
}