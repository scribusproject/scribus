/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scslainforeader.h"

#include <QFile>
#include <QXmlStreamReader>

void ScSlaInfoReader::resetFileInfos(void)
{
	m_title.resize(0);
	m_author.resize(0);
	m_format.resize(0);
}

bool ScSlaInfoReader::readInfos(const QString& fileName)
{
	bool isScribusDocument = false;
	bool readInfoSuccess   = false;
	bool firstStartElement = true;

	resetFileInfos();

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QByteArray bytes = file.read(512);
	if (!bytes.contains("SCRIBUS") && !bytes.contains("SCRIBUSUTF8") && !bytes.contains("SCRIBUSUTF8NEW"))
	{
		file.close();
		return false;
	}
	file.reset();

	QXmlStreamReader reader(&file);
	while (!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType ttype = reader.readNext();
		if (reader.hasError()) break;

		if (ttype == QXmlStreamReader::StartElement)
		{
			QStringRef nodeName = reader.name();
			if (firstStartElement)
			{
				if (nodeName == "SCRIBUS" || nodeName == "SCRIBUSUTF8" || nodeName == "SCRIBUSUTF8NEW")
				{
					QXmlStreamAttributes attrs = reader.attributes();
					m_format = attrs.value(QLatin1String("Version")).toString();
					isScribusDocument = true;
				}
				else
				{
					isScribusDocument = false;
					break;
				}
			}
			else if (nodeName == "DOCUMENT")
			{
				QXmlStreamAttributes attrs = reader.attributes();
				m_title  = attrs.value(QLatin1String("TITLE")).toString();
				m_author = attrs.value(QLatin1String("AUTHOR")).toString();
				readInfoSuccess = true;
				break;
			}
			firstStartElement = false;
		}
	}
	isScribusDocument &= !reader.hasError();
	file.close();

	return (isScribusDocument && readInfoSuccess);
}
