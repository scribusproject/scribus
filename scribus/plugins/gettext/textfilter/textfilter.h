#ifndef TEXTFILTER_H
#define TEXTFILTER_H

#include <scribus.h>
#include <prefscontext.h>
#include <gtwriter.h>
#include "tffilter.h"

extern "C" void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" QString FileFormatName();

extern "C" QStringList FileExtensions();

class TextFilter
{
private:
	PrefsContext* prefs;
	QString text;
	QString encoding;
	QString filename;
	gtWriter* writer;
	std::vector<tfFilter*> *filters;
	void loadText();
	void toUnicode();
	void write();
	void replace(QString* text);
	void replaceHex(QString* text);
public:
	TextFilter(const QString& fname, const QString& enc, gtWriter* w);
	~TextFilter();
};

#endif // TEXTFILTER_H
