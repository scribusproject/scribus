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
#include "scribus.h"

extern QPixmap loadIcon(QString nam);
extern ScribusApp* ScApp;

gtGetText::gtGetText()
{
	loadImporterPlugins();
}

void gtGetText::launchImporter(int importer, const QString& filename,
                               bool textOnly, const QString& encoding, bool append)
{
	struct ImporterData ida;
	bool callImporter = true;
	if (importer == -1)
	{
		QString fend = filename.right(filename.length() - filename.findRev(".") - 1);
		if (importerMap.find(fend) != importerMap.end())
			ida = *importerMap[fend];
		else
		{
			callImporter = dias->runImporterDialog(ilist);
			if (callImporter)
				ida = importers[dias->getImporter()];
		}
	}
	else
	{
		ida = importers[importer];
	}

	if (callImporter)
		CallDLL(ida, filename, encoding, textOnly, append);
}

void gtGetText::loadImporterPlugins()
{
	QString gtdir = ScPaths::instance().pluginDir();
	gtdir += "gettext";
	QString libPattern = QString("*.%1*").arg(PluginManager::platformDllExtension());
	QDir d(gtdir, libPattern, QDir::Name, QDir::Files | QDir::Executable | QDir::NoSymLinks);

	struct ImporterData ida;
	ida.fileFormatName = "";

	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			if (DLLName(d[dc], &ida.fileFormatName, &ida.fileEndings))
			{
				ida.soFilePath = d[dc];
				if (ida.soFilePath.left(1) != "/")
					ida.soFilePath = "/" + ida.soFilePath;
				importers.push_back(ida);
			}
		}
	}
	createMap();
}

void gtGetText::run(bool append)
{
	QString filters = "";
	QString allSupported = QObject::tr("All Supported Formats") + " (";
	for (uint i = 0; i < importers.size(); ++i)
	{
		if (importers[i].fileEndings.count() != 0)
		{
			filters += importers[i].fileFormatName + " (";
			for (uint j = 0; j < importers[i].fileEndings.count(); ++j)
			{
				filters += "*." + importers[i].fileEndings[j] + " ";
				allSupported += "*." + importers[i].fileEndings[j] + " ";
			}
			filters = filters.stripWhiteSpace();
			filters += ");;";
		}
	}
	allSupported = allSupported.stripWhiteSpace();
	allSupported += ");;";
	filters = allSupported + filters;
	filters += QObject::tr("All Files (*)");
	for (uint i = 0;  i < importers.size(); ++i)
		ilist.append(importers[i].fileFormatName);
	dias = new gtDialogs();
	if (dias->runFileDialog(filters, ilist))
	{
		launchImporter(dias->getImporter(), dias->getFileName(),
		               dias->importTextOnly(), dias->getEncoding(), append);
	}
	delete dias;
}

void gtGetText::CallDLL(const ImporterData& idata, const QString& filePath,
                        const QString& encoding, bool textOnly, bool append)
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
	gtWriter *w = new gtWriter(append);
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
		for (uint j = 0; j < importers[i].fileEndings.count(); ++j)
				importerMap.insert(importers[i].fileEndings[j], &importers[i]);
	}
}

gtGetText::~gtGetText()
{

}
