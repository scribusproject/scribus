#ifndef TXTIM_H
#define TXTIM_H

#include <scribus.h>
#include "gtwriter.h"

extern "C" void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" QString FileFormatName();

extern "C" QStringList FileExtensions();

class TxtIm 
{
public:
	TxtIm(const QString& fname, const QString& enc, bool textOnly, gtWriter *w);
	~TxtIm();
	void write();
private:
	QString filename;
	QString encoding;
	QString text;
	gtWriter *writer;
	bool textOnly;
	void loadText();
	void toUnicode();
};

#endif // TXTIM_H
