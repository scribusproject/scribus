/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCXMLSTREAMREADER_H
#define SCXMLSTREAMREADER_H

#include <QXmlStreamAttributes>
#include <QXmlStreamReader>

#include "scribusapi.h"

class SCRIBUS_API ScXmlStreamAttributes : public QXmlStreamAttributes
{
public:
	ScXmlStreamAttributes(void);
	ScXmlStreamAttributes(const QXmlStreamAttributes& attrs);

	bool    valueAsBool   (const char*    attrName, bool def = false) const;
	bool    valueAsBool   (const QString& attrName, bool def = false) const;
	int     valueAsInt    (const char*    attrName, int def = 0) const;
	int     valueAsInt    (const QString& attrName, int def = 0) const;
	int     valueAsInt    (const char*    attrName, int min, int max, int def = 0) const;
	int     valueAsInt    (const QString& attrName, int min, int max, int def = 0) const;
	uint    valueAsUInt   (const char*    attrName, uint def = 0) const;
	uint    valueAsUInt   (const QString& attrName, uint def = 0) const;
	double  valueAsDouble (const char*    attrName, double def = 0.0) const;
	double  valueAsDouble (const QString& attrName, double def = 0.0) const;
	QString valueAsString (const char*    attrName, const QString& def = QString()) const;
	QString valueAsString (const QString& attrName, const QString& def = QString()) const;
};

class SCRIBUS_API ScXmlStreamReader : public QXmlStreamReader
{
public:
	explicit ScXmlStreamReader(const QString& string) : QXmlStreamReader(string) {};
	explicit ScXmlStreamReader(QIODevice* device) : QXmlStreamReader(device) {};

	ScXmlStreamAttributes scAttributes(void) const;

	void readToElementEnd(void);
};

#endif
