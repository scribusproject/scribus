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

#include "gtgettext.h"

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
	QString gtdir = PREL;
	gtdir += "/lib/scribus/plugins/gettext";
#if defined(__hpux)
	QDir d(gtdir, "*.sl*", QDir::Name, QDir::Files | QDir::Executable | QDir::NoSymLinks);
#else
	QDir d(gtdir, "*.so*", QDir::Name, QDir::Files | QDir::Executable | QDir::NoSymLinks);
#endif

	struct ImporterData ida;
	ida.fileFormatName = "";

	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			if (DLLName(d[dc], &ida.fileFormatName, &ida.fileEndings, &ida.pointer))
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
	void *mo;
	const char *error;
	typedef void (*sdem)(QString filename, QString encoding, bool textOnly, gtWriter *writer);
	sdem demo;
	QString pfad = PREL;
	pfad += "/lib/scribus/plugins/gettext" + idata.soFilePath;
	mo = dlopen(pfad, RTLD_LAZY | RTLD_GLOBAL);
	if (!mo)
		return;

	dlerror();
	demo = (sdem)dlsym(mo, "GetText");
	if ((error = dlerror()) != NULL)
	{
		std::cout << "Can't find Symbol" << "\n";
		dlclose(mo);
		return;
	}
	gtWriter *w = new gtWriter(append);
	(*demo)(filePath, encoding, textOnly, w);
	delete w;
	dlclose(mo);
}

bool gtGetText::DLLName(QString name, QString *ffName, QStringList *fEndings, void **Zeig)
{
	void *mo;
	const char *error;
	typedef QString (*sdem0)();
	typedef QStringList (*sdem1)();
	sdem0 demo;
	sdem1 demo1;
	QString pfad = PREL;
	pfad += "/lib/scribus/plugins/gettext";
	if (name.left(1) != "/")
		pfad += "/";
	pfad += name;
	mo = dlopen(pfad, RTLD_LAZY | RTLD_GLOBAL);
	if (!mo)
	{
		std::cout << dlerror() << "\n";
		return false;
	}
	dlerror();
	demo = (sdem0)dlsym(mo, "FileFormatName");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*ffName = (*demo)();
	dlerror();
	demo1 = (sdem1)dlsym(mo, "FileExtensions");
	if ((error = dlerror()) != NULL)
	{
		dlclose(mo);
		return false;
	}
	*fEndings = (*demo1)();
	*Zeig = mo;
	dlclose(mo);

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
