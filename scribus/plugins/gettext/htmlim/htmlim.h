#ifndef HTMLIM_H
#define HTMLIM_H

#include <scribus.h>
#include <gtwriter.h>
#include "htmlreader.h"

extern "C" void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" QString FileFormatName();

extern "C" QStringList FileExtensions();

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

#endif // HTMLIM_H
