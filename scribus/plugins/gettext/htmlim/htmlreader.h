/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#ifndef HTMLREADER_H
#define HTMLREADER_H

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
	QString templateCategory;
	QString href;
	gtWriter *writer;
	gtParagraphStyle *pstyle;
	gtParagraphStyle *pstylec;
	gtParagraphStyle *pstyleli;
	gtParagraphStyle *pstyleh1;
	gtParagraphStyle *pstyleh2;
	gtParagraphStyle *pstyleh3;
	bool inOL;
	int  nextItemNumber;
	bool inUL;
	bool inLI;
	bool addedLI;
	bool inH1;
	bool inH2;
	bool inH3;
	bool inA;
	bool inCenter;
	bool inCode;
	void initPStyles();
	void toggleUnderlining();
	void setBlueFont();
	void setDefaultFont();
	void setBoldFont();
	void unSetBoldFont();
public:
	HTMLReader(gtParagraphStyle *ps, gtWriter *w);
	~HTMLReader();
	bool startDocument();
	bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs);
	bool endElement(const QString&, const QString&, const QString &name);
	bool characters(const QString &ch);
	bool endDocument();
};

#endif
