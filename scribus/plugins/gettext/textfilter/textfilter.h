/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TEXTFILTER_H
#define TEXTFILTER_H

#include "pluginapi.h"

#include <prefscontext.h>
#include "gtwriter.h"
#include "tffilter.h"

extern "C" PLUGIN_API void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" PLUGIN_API QString FileFormatName();

extern "C" PLUGIN_API QStringList FileExtensions();

class TextFilter
{
private:
	PrefsContext* m_prefs;
	QString m_text;
	QString m_encoding;
	QString m_filename;
	gtWriter* m_writer;
	std::vector<tfFilter*> *m_filters;
	void loadText();
	void write();
	void replace(QString* m_text);
	void replaceHex(QString* m_text);
public:
	TextFilter(const QString& fname, const QString& enc, gtWriter* w);
	~TextFilter();
};

#endif // TEXTFILTER_H
