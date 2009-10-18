/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCDOCOUTPUT_PS2_H
#define SCDOCOUTPUT_PS2_H

#include <vector>

#include <QFile>
#include <QTextStream>

#include "scribusapi.h"
#include "scpainterexbase.h"
#include "scpageoutput_ps2.h"
#include "scdocoutput.h"

class ScribusDoc;
class Page;

class SCRIBUS_API ScDocOutput_Ps2 : public ScDocOutput
{
protected:

	QFile m_file;
	QIODevice* m_device;
	QTextStream m_stream;
	ScribusDoc* m_doc;
	std::vector<int> m_pageNumbers;
	ScPs2OutputParams m_options;

	int m_status;
	QString m_lastError;

	QString m_author;
	QString m_creator;
	QString m_title;

	QRect   m_clip;

	virtual void begin(void);
	virtual void end(void);

	virtual ScPageOutput* createPageOutputComponent(int pageIndex);

	bool initializeCmsTransforms(void);

public:
	ScDocOutput_Ps2(ScribusDoc* doc, QString fileName, std::vector<int>& pageNumbers, QRect& clip, ScPs2OutputParams& options);
	~ScDocOutput_Ps2();

	void setAuthor(const QString& author) { m_author = author; }
	void setCreateor(const QString& creator) { m_creator = creator; }
	void setTitle(const QString& title) { m_title = title; }

	int status() { return m_status; }
	QString lastError() { return m_lastError; }
};

#endif
