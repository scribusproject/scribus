/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCDOMELEMENT_H
#define SCDOMELEMENT_H

#include <QDomElement>

class ScDomElement : public QDomElement
{
public:
	ScDomElement();
	ScDomElement(const QDomElement &elem); 

	int     valueAsInt    (const QString& attrName, int def = 0) const;
	int     valueAsInt    (const QString& attrName, int min, int max, int def = 0) const;
	uint    valueAsUInt   (const QString& attrName, uint def = 0) const;
	uint    valueAsUInt   (const QString& attrName, uint min, uint max, uint def = 0) const;
	double  valueAsDouble (const QString& attrName, double def = 0.0) const;
	double  valueAsDouble (const QString& attrName, double min, double max, double def = 0.0) const;
};

#endif
