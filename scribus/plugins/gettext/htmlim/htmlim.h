#ifndef HTMLIM_H
#define HTMLIM_H

#include <scribus.h>
#include <gtwriter.h>

#include "config.h"

extern "C" void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" QString FileFormatName();

extern "C" QStringList FileExtensions();

#ifdef HAVE_XML

#include "htmlreader.h"

class HTMLIm 
{
public:
	HTMLIm(QString fname, gtWriter *w);
	~HTMLIm();
private:
	QString filename;
	gtWriter *writer;
	gtParagraphStyle *pstyle;
	int defaultFontSize;
	void importText();
};

#endif // HAVE_XML

#endif // HTMLIM_H
