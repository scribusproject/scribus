/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@welho.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <qfile.h>
#include <qfileinfo.h>
#include <qstring.h>
#include <qxml.h>
#include "prefsfile.h"
#include "prefsreader.h"
#include "prefstable.h"

PrefsFile::PrefsFile()
{
	prefsFilePath = "";
	ioEnabled = false;
}

PrefsFile::PrefsFile(const QString& pFilePath, bool write)
{
	prefsFilePath = pFilePath;
	ioEnabled = write;
	if (ioEnabled)
		canWrite();
	load();
}

PrefsContext* PrefsFile::getContext(const QString& contextName, bool persistent)
{
	if (!contexts.contains(contextName))
		contexts[contextName] = new PrefsContext(contextName, persistent);
	return contexts[contextName];
}

PrefsContext* PrefsFile::getPluginContext(const QString& contextName, bool persistent)
{
	if (!pluginContexts.contains(contextName))
		pluginContexts[contextName] = new PrefsContext(contextName, persistent);
	return pluginContexts[contextName];
}

void PrefsFile::load()
{
	PrefsReader* handler = new PrefsReader(&contexts, &pluginContexts);
	QFile* rc = new QFile(prefsFilePath);
	QXmlInputSource* source = new QXmlInputSource(rc);
	QXmlSimpleReader* reader = new QXmlSimpleReader();
	reader->setContentHandler(handler);
	reader->parse(source);
	delete reader;
	delete source;
	delete rc;
	delete handler;
}

void PrefsFile::write()
{
	if ((!ioEnabled) || ((contexts.size() == 0) && (pluginContexts.size() == 0)))
		return; // No prefs file path set -> can't write or no prefs to write
	QFile* prefsXML = new QFile(prefsFilePath);
	if (prefsXML->open(IO_WriteOnly))
	{
		QTextStream stream(prefsXML);
		stream.setEncoding(QTextStream::UnicodeUTF8);
		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		stream << "<preferences>\n";
		if (contexts.size() > 0)
		{
			stream << "\t<level name=\"application\">\n";
			writeContexts(&contexts, stream);
			stream << "\t</level>\n";
		}
		if (pluginContexts.size() > 0)
		{
			stream << "\t<level name=\"plugin\">\n";
			writeContexts(&pluginContexts, stream);
			stream << "\t</level>\n";
		}
		stream << "</preferences>\n";

		prefsXML->close();
	}
	delete prefsXML;
}

void PrefsFile::writeContexts(ContextMap* contextMap, QTextStream& stream)
{
	ContextMap::Iterator it;
	for (it = contextMap->begin(); it != contextMap->end(); ++it)
	{
		if ((it.data()->isPersistent()) && (!it.data()->isEmpty()))
		{
			stream << "\t\t<context name=\"" + replaceIllegalChars(it.key()) + "\">\n";
			AttributeMap::Iterator it2;
			PrefsContext* tmpCon = it.data();
			for (it2 = tmpCon->values.begin(); it2 != tmpCon->values.end(); ++it2)
			{
				stream << "\t\t\t<attribute key=\"" + replaceIllegalChars(it2.key()) + "\" ";
				stream << "value=\""  + replaceIllegalChars(it2.data()) + "\"/>\n";
			}
			TableMap::Iterator it3;
			for (it3 = tmpCon->tables.begin(); it3 != tmpCon->tables.end(); ++it3)
			{
				stream << "\t\t\t<table name=\"" + replaceIllegalChars(it3.key()) + "\">\n";
				PrefsTable* t = it3.data();
				for (int i = 0; i < t->height(); ++i)
				{
					stream << QString("\t\t\t\t<row index=\"%1\">\n").arg(i);
					for (int j = 0; j < t->width(); ++j)
					{
						stream << QString("\t\t\t\t\t<col index=\"%1\">").arg(j);
						stream << replaceIllegalChars(t->get(i, j, "__NOT__SET__")) << "</col>\n";
					}
					stream << "\t\t\t\t</row>\n";
				}
				stream << "\t\t\t</table>\n";
			}
			stream << "\t\t</context>\n";
		}
	}
}

QString PrefsFile::replaceIllegalChars(const QString& text)
{
	QString s = text;
	s.replace("&", "&amp;");
	s.replace("<", "&lt;");
	s.replace(">", "&gt;");
	s.replace("\"", "&quot;");
	s.replace("\'", "&apos;");
	return s;
}

void PrefsFile::canWrite()
{
	if (ioEnabled)
	{
		QFile f(prefsFilePath);
		QFileInfo fi(f);
		if (fi.exists())
			ioEnabled = fi.isWritable();
		else
		{
			QFile f2(prefsFilePath.left(prefsFilePath.findRev("/")));
			QFileInfo fi2(f2);
			ioEnabled = fi2.isWritable();
		}
	}
}

PrefsFile::~PrefsFile()
{
	ContextMap::Iterator it;
	for (it = contexts.begin(); it != contexts.end(); ++it)
		delete it.data();
	for (it = pluginContexts.begin(); it != pluginContexts.end(); ++it)
		delete it.data();
}
