/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scslainforeader.h"

#include <qfile.h>
#include <qstring.h>
#include <qxml.h>

class ScSlaInfoHandler : public QXmlDefaultHandler
{
protected:
	QString m_title;
	QString m_author;
	QString m_format;

	bool m_firstStartElement;
	bool m_isScribusDocument;
	bool m_readInfoSuccess;

	void reset(void);

public:
	ScSlaInfoHandler(void) { reset(); }

	bool isScribusDoc(void) const { return m_isScribusDocument; }
	bool readSuccess(void)  const { return m_readInfoSuccess; } 

	const QString& title(void)  const { return m_title; }
	const QString& author(void) const { return m_author; }
	const QString& format(void) const { return m_format; }

	bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs);
};

void ScSlaInfoHandler::reset(void)
{
	m_title  = "";
	m_author = "";
	m_format = "";
	m_firstStartElement = true;
	m_isScribusDocument = false;
	m_readInfoSuccess   = false;
}

bool ScSlaInfoHandler::startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs)
{
	bool stopParsing = false;
	if (m_firstStartElement)
	{
		if (name == "SCRIBUS" || name == "SCRIBUSUTF8" || name == "SCRIBUSUTF8NEW")
		{
			m_format = attrs.value("Version");
			m_isScribusDocument = true;
		}
		else
		{
			m_isScribusDocument = false;
			stopParsing = true;
		}
	}
	else if (name == "DOCUMENT")
	{
		m_title  = attrs.value("TITLE");
		m_author = attrs.value("AUTHOR");
		m_readInfoSuccess = true;
		stopParsing = true;
	}
	m_firstStartElement = false;
	return !stopParsing;
}

void ScSlaInfoReader::resetFileInfos(void)
{
	m_title  = "";
	m_author = "";
	m_format = "";
}

bool ScSlaInfoReader::readInfos(const QString& fileName)
{
	bool isScribusDocument = false;
	bool readInfoSuccess   = false;
	bool firstStartElement = true;

	resetFileInfos();

	QFile file(fileName);
	if (!file.open(IO_ReadOnly))
		return false;

	char fileData[64] = { 0 };
	int  bytesRead = file.readBlock(fileData, 63);
	fileData[bytesRead] = 0;

	if (!QString(fileData).startsWith("<SCRIBUS"))
	{
		file.close();
		return false;
	}
	file.reset();

	ScSlaInfoHandler infoHandler;
	QXmlInputSource  source(&file);
	QXmlSimpleReader reader;
	reader.setContentHandler(&infoHandler);
	reader.parse(source);

	isScribusDocument = infoHandler.isScribusDoc();
	readInfoSuccess   = infoHandler.readSuccess();

	m_format = infoHandler.format();
	m_title  = infoHandler.title();
	m_author = infoHandler.author();

	file.close();

	return (isScribusDocument && readInfoSuccess);
}
