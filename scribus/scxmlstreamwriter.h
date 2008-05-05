/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCXMLSTREAMWRITER_H
#define SCXMLSTREAMWRITER_H

#include "scribusapi.h"

#include <QString>
#include <QXmlStreamWriter>

class SCRIBUS_API ScXmlStreamWriter : public QXmlStreamWriter
{
public:
	ScXmlStreamWriter(void) : QXmlStreamWriter() {}
	ScXmlStreamWriter(QIODevice* device) : QXmlStreamWriter(device) {}
	ScXmlStreamWriter(QString*   string) : QXmlStreamWriter(string) {}
	void writeAttribute(const QString & name, const QString & value) { QXmlStreamWriter::writeAttribute(name, value); }
	void writeAttribute(const QString & name, int value)    { QXmlStreamWriter::writeAttribute(name, QString::number(value)); }
	void writeAttribute(const QString & name, uint value)   { QXmlStreamWriter::writeAttribute(name, QString::number(value)); }
	void writeAttribute(const QString & name, double value) { QXmlStreamWriter::writeAttribute(name, QString::number(value)); }
};

#endif
