/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCXMLSTREAMWRITER_H
#define SCXMLSTREAMWRITER_H

#include <type_traits>

#include <QByteArray>
#include <QString>
#include <QXmlStreamWriter>

#include "pdfversion.h"
#include "scribusapi.h"

class SCRIBUS_API ScXmlStreamWriter : public QXmlStreamWriter
{
public:
	ScXmlStreamWriter(void) : QXmlStreamWriter() {}
	explicit ScXmlStreamWriter(QByteArray* array) : QXmlStreamWriter(array) {}
	explicit ScXmlStreamWriter(QIODevice* device) : QXmlStreamWriter(device) {}
	explicit ScXmlStreamWriter(QString*   string) : QXmlStreamWriter(string) {}

	void writeAttribute(const QString& name, const QString& value) { QXmlStreamWriter::writeAttribute(name, value); }
	void writeAttribute(const QString& name, const PDFVersion& value) { QXmlStreamWriter::writeAttribute(name, QString::number(value)); }

	template<typename T, std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>, bool> = true>
	void writeAttribute(const QString & name, T value)    { QXmlStreamWriter::writeAttribute(name, QString::number(value)); }

	template<typename T, std::enable_if_t< std::is_floating_point_v<T>, bool> = true>
	void writeAttribute(const QString & name, T value) { QXmlStreamWriter::writeAttribute(name, QString::number(value, 'g', 15)); }
};

#endif
