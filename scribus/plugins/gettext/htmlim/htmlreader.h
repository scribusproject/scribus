/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#ifndef HTMLREADER_H
#define HTMLREADER_H

#include "config.h"

#ifdef HAVE_XML

#include <libxml/xmlmemory.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/debugXML.h>
#include <libxml/xmlerror.h>
#include <libxml/globals.h>

#include <qobject.h>
#include <qdir.h>
#include <qxml.h>
#include <qstring.h>

#include <gtparagraphstyle.h>
#include <gtwriter.h>

class HTMLReader : public QXmlDefaultHandler
{
private:
	QString currentDir;
	QString currentFile;
	QString defaultColor;
	QString defaultWeight;
	QString defaultSlant;
	QString templateCategory;
	QString href;
	gtWriter *writer;
	gtParagraphStyle *pstyle;
	gtParagraphStyle *pstylec;
	gtParagraphStyle *pstyleli;
	gtParagraphStyle *pstyleh1;
	gtParagraphStyle *pstyleh2;
	gtParagraphStyle *pstyleh3;
	gtParagraphStyle *pstyleh4;
	gtParagraphStyle *pstylecode;
	gtParagraphStyle *pstylep;
	gtParagraphStyle *pstylepre;
	bool inOL;
	int  nextItemNumber;
	bool inUL;
	bool inLI;
	bool addedLI;
	bool inH1;
	bool inH2;
	bool inH3;
	bool inH4;
	bool inA;
	bool inCenter;
	bool inCode;
	bool inBody;
	bool inPre;
	bool inP;
	bool lastCharWasSpace;
	void initPStyles();
	void toggleEffect(FontEffect e);
	void setItalicFont();
	void unsetItalicFont();
	void setBlueFont();
	void setDefaultColor();
	void setBoldFont();
	void unSetBoldFont();
	static HTMLReader* hreader;
public:
	HTMLReader(gtParagraphStyle *ps, gtWriter *w);
	~HTMLReader();
	void parse(QString filename);
	static void startElement(void *user_data, const xmlChar * fullname, const xmlChar ** atts);
	static void endElement(void *user_data, const xmlChar * name);
	static void characters(void *user_data, const xmlChar * ch, int len);
	bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs);
	bool endElement(const QString&, const QString&, const QString &name);
	bool characters(const QString &ch);
};

#endif // HAVE_XML

#endif
