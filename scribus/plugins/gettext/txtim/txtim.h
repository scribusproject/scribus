/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TXTIM_H
#define TXTIM_H

#include "pluginapi.h"
#include "gtwriter.h"

extern "C" PLUGIN_API void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" PLUGIN_API QString FileFormatName();

extern "C" PLUGIN_API QStringList FileExtensions();

class QByteArray;

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
	QString toUnicode(const QByteArray& rawText);
};

#endif // TXTIM_H
