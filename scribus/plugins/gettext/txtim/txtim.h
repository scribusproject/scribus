#ifndef TXTIM_H
#define TXTIM_H

#include "pluginapi.h"
#include "gtwriter.h"

extern "C" PLUGIN_API void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" PLUGIN_API QString FileFormatName();

extern "C" PLUGIN_API QStringList FileExtensions();

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
