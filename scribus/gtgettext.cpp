/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
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

#include "gtgettext.h"
#include "pluginmanager.h"
#include "scpaths.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "selection.h"
//Added by qt3to4:
#include <QPixmap>
#include "util_icon.h"


gtGetText::gtGetText(ScribusDoc* doc)
{
	m_Doc=doc;
	loadImporterPlugins();
}

void gtGetText::launchImporter(int importer, const QString& filename, bool textOnly, 
								const QString& encoding, bool append, PageItem* target)
{
	struct ImporterData ida;
	bool callImporter = true;
	if (importer == -1)
	{
		QString fend = filename.right(filename.length() - filename.lastIndexOf(".") - 1);
		if (importerMap.find(fend) != importerMap.end())
			ida = *importerMap[fend];
		else
		{
			dias = new gtDialogs();
			callImporter = dias->runImporterDialog(ilist);
			if (callImporter)
				ida = importers[dias->getImporter()];
			delete dias;
		}
	}
	else
	{
		ida = importers[importer];
	}
	PageItem* targetFrame=target;
	if (targetFrame==0)
		targetFrame=m_Doc->m_Selection->itemAt(0);

	if (targetFrame!=0 && callImporter)
		CallDLL(ida, filename, encoding, textOnly, append, targetFrame);
}

void gtGetText::loadImporterPlugins()
{
	QString gtdir = ScPaths::instance().pluginDir();
	gtdir += "gettext";
	QString libPattern = QString("*.%1*").arg(PluginManager::platformDllExtension());
	QDir d(gtdir, libPattern, QDir::Name, (QDir::Filter) PluginManager::platformDllSearchFlags());

	struct ImporterData ida;
	ida.fileFormatName = "";

	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			if (DLLName(d[dc], &ida.fileFormatName, &ida.fileEndings))
			{
				// no plugin's "format name" marks "don't load plug"
				if (ida.fileFormatName.isNull())
					continue;
				ida.soFilePath = d[dc];
				if (ida.soFilePath.left(1) != "/")
					ida.soFilePath = "/" + ida.soFilePath;
				importers.push_back(ida);
			}
		}
	}
	createMap();
}

ImportSetup gtGetText::run()
{
	QString filters = "";
	QString allSupported = QObject::tr("All Supported Formats") + " (";
	for (uint i = 0; i < importers.size(); ++i)
	{
		if (importers[i].fileEndings.count() != 0)
		{
			filters += importers[i].fileFormatName + " (";
			for (int j = 0; j < importers[i].fileEndings.count(); ++j)
			{
				filters += "*." + importers[i].fileEndings[j] + " ";
				allSupported += "*." + importers[i].fileEndings[j] + " ";
			}
			filters = filters.trimmed();
			filters += ");;";
		}
	}
	allSupported = allSupported.trimmed();
	allSupported += ");;";
	filters = allSupported + filters;
	filters += QObject::tr("All Files (*)");
	for (uint i = 0;  i < importers.size(); ++i)
		ilist.append(importers[i].fileFormatName);
	dias = new gtDialogs();
	ImportSetup impsetup;
	impsetup.runDialog=false;
	if (dias->runFileDialog(filters, ilist))
	{
		impsetup.runDialog=true;
		impsetup.encoding=dias->getEncoding();
		impsetup.filename=dias->getFileName();
		impsetup.importer=dias->getImporter();
		impsetup.textOnly=dias->importTextOnly();
// 		launchImporter(dias->getImporter(), dias->getFileName(),
// 		               dias->importTextOnly(), dias->getEncoding(), append);
	}
	delete dias;
	return impsetup;
}

void gtGetText::CallDLL(const ImporterData& idata, const QString& filePath,
                        const QString& encoding, bool textOnly, bool append, PageItem* importItem)
{
	void* gtplugin;
	typedef void (*sdem)(QString filename, QString encoding, bool textOnly, gtWriter *writer);
	sdem fp_GetText;
	QString pluginFilePath = QString("%1/gettext/%2").arg(ScPaths::instance().pluginDir()).arg(idata.soFilePath);
	gtplugin = PluginManager::loadDLL(pluginFilePath);
	if (!gtplugin)
	{
		qWarning("Failed to load plugin %s", pluginFilePath.ascii());
		return;
	}
	fp_GetText = (sdem) PluginManager::resolveSym(gtplugin, "GetText");
	if (!fp_GetText)
	{
		qWarning("Failed to get GetText() from %s", pluginFilePath.ascii());
		PluginManager::unloadDLL(gtplugin);
		return;
	}
	gtWriter *w = new gtWriter(append, importItem);
	(*fp_GetText)(filePath, encoding, textOnly, w);
	delete w;
	PluginManager::unloadDLL(gtplugin);
}

bool gtGetText::DLLName(QString name, QString *ffName, QStringList *fEndings)
{
	void* gtplugin;
	typedef QString (*sdem0)();
	typedef QStringList (*sdem1)();
	sdem0 fp_FileFormatName;
	sdem1 fp_FileExtensions;
	QString pluginFilePath = QString("%1/gettext/%2").arg(ScPaths::instance().pluginDir()).arg(name);
	gtplugin = PluginManager::loadDLL(pluginFilePath);
	if (!gtplugin)
	{
		qWarning("Failed to load plugin %s", pluginFilePath.ascii());
		return false;
	}
	fp_FileFormatName = (sdem0) PluginManager::resolveSym( gtplugin, "FileFormatName");
	if (!fp_FileFormatName)
	{
		qWarning("Failed to get FileFormatName() from %s", pluginFilePath.ascii());
		PluginManager::unloadDLL(gtplugin);
		return false;
	}
	fp_FileExtensions = (sdem1) PluginManager::resolveSym( gtplugin, "FileExtensions");
	if (!fp_FileExtensions)
	{
		qWarning("Failed to get FileExtensions() from %s", pluginFilePath.ascii());
		PluginManager::unloadDLL(gtplugin);
		return false;
	}
	*ffName = (*fp_FileFormatName)();
	*fEndings = (*fp_FileExtensions)();
	PluginManager::unloadDLL(gtplugin);
	return true;
}

void gtGetText::createMap()
{
	for (uint i = 0; i < importers.size(); ++i)
	{
		for (int j = 0; j < importers[i].fileEndings.count(); ++j)
				importerMap.insert(importers[i].fileEndings[j], &importers[i]);
	}
}

gtGetText::~gtGetText()
{

}
