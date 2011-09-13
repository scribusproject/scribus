/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef DOCIM_H
#define DOCIM_H

#include "scconfig.h"

#include "pluginapi.h"
#include <QBuffer>
#include <QObject>
#include <QStringList>

class gtWriter;
class QProcess;

extern "C" PLUGIN_API void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

/*! \brief Returns the string with importer description.
\retval QString a string or QString::null when the plugin should not be loaded.
*/
extern "C" PLUGIN_API QString FileFormatName();

extern "C" PLUGIN_API QStringList FileExtensions();

class DocIm : public QObject
{
	Q_OBJECT
public:
	DocIm(const QString& fname, const QString& enc, bool textOnly, gtWriter *w);
	~DocIm();
	void write();
	bool isRunning();
private:
	QString filename;
	QString encoding;
	QBuffer textBuffer;
	QBuffer errorBuffer;
	gtWriter *writer;
	QProcess *proc;
	bool failed;
	bool textOnly;
};

#endif // DOCIM_H
