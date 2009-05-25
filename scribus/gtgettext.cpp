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
#include "ui/gtdialogs.h"
#include "gtwriter.h"
//Added by qt3to4:
#include <QPixmap>
#include "util_icon.h"

// Constructor
gtGetText::gtGetText(ScribusDoc* doc)
{
	// Attach to the active document
	m_Doc=doc;
	// Load the plugins array.
	loadImporterPlugins();
} // gtGetText::gtGetText(ScribusDoc* doc)

// Look at the results of the file selection dialog and figure out if you need to use an importer.
// Prompt the user if the importer to use isn't obvious.
void gtGetText::launchImporter(int importer, const QString& filename, bool textOnly, 
								const QString& encoding, bool append, PageItem* target)
{
	// Struct for the plugin info, we'll load this up from the array.
	struct ImporterData ida;
	// Do we need to call an importer? Unsure what happens if this ever becomes false.
	bool callImporter = true;
	// Check and see if the file selection dialog returned the index position of the 
	// importer to be used. If it isn't, try to figure out what it is. If we can't,
	// prompt the user.
	if (importer == -1)
	{
		// Attempt to determine the importer based on the file's extension. 
		// Create a Qstring with what could be an extension.
		QString fend = filename.right(filename.length() - filename.lastIndexOf(".") - 1);
		// Look for that extension in the importer Qmap. 
		if (importerMap.find(fend) != importerMap.end())
			// If the map is found, assign ida to the corresponding struct in the map.
			ida = *importerMap[fend];
		// Otherwise, try and ask the user.
		else
		{
			// Create a new dialog
			dias = new gtDialogs();
			// Pop up the dialog asking the user to select the type from our list (ilist) of 
			// importable file types. If one is not selected, set callImporter to false.
			callImporter = dias->runImporterDialog(ilist);
			// If we're gonna call an importer, we need to copy it's struct to ida.
			if (callImporter)
				ida = importers[dias->getImporter()];
			// Destroy the diag
			delete dias;
		} // else - if (importerMap.find(fend) != importerMap.end())
	}
	else // If we know which importer to use
	{
		// Copy the importer's struct to ida.
		ida = importers[importer];
	}	// else - if (importer == -1)
	
	// Create a target text frame for the imported text and assign it to the parameter "target"
	PageItem* targetFrame=target;
	
	// If the targetframe is 0 ( no frame selected/created? ) then reassign it to the 
	// (questionable interpretation here) first frame in the documentation.
	if (targetFrame==0)
		targetFrame=m_Doc->m_Selection->itemAt(0);

	// If the targetframe is not zero, and we do need to call the importer, 
	// Run the importer via "CallDLL" and pass it what it needs to know.
	if (targetFrame!=0 && callImporter)
		CallDLL(ida, filename, encoding, textOnly, append, targetFrame);
}  //void gtGetText::launchImporter(int importer, const QString& filename, bool textOnly, 
   //						const QString& encoding, bool append, PageItem* target)

// Find the available plugins based on the environment, validate they load, and 
// create quick lookup mappings.
void gtGetText::loadImporterPlugins()
{
	// Get the path to the plugins
	QString gtdir = ScPaths::instance().pluginDir();
	// Append the gettext path to the plugin
	gtdir += "gettext";
	// Set the search parameteer for the platform specific extension for the plugins ( DLL, so, etc. )
	QString libPattern = QString("*.%1*").arg(PluginManager::platformDllExtension());
	// Search for matches.
	QDir d(gtdir, libPattern, QDir::Name, (QDir::Filter) PluginManager::platformDllSearchFlags());

	// Initialize a structure for the importers found
	struct ImporterData ida;
	ida.fileFormatName = "";

	// Check and see if the directory existed and if the count of files matching is greater than 0.
	if ((d.exists()) && (d.count() != 0))
	{
		// loop through the entries.
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			// Verify the Plugin will load. If it does, collect the info on the plugin ( Name, extension, format, etc )
			if (DLLName(d[dc], &ida.fileFormatName, &ida.fileEndings))
			{
				// no plugin's "format name" marks "don't load plug"
				if (ida.fileFormatName.isNull())
					continue;
				// Store the path to the plugin.
				ida.soFilePath = d[dc];
				// If the plugin path does not begin with /, prepend a /.
				if (ida.soFilePath.left(1) != "/")
					ida.soFilePath = "/" + ida.soFilePath;
				// Add the plugin data to the end of the importer's vector.
				importers.push_back(ida);
			}	// if (DLLName(d[dc], &ida.fileFormatName, &ida.fileEndings))
		}  // for (uint dc = 0; dc < d.count(); ++dc)
	}  // if ((d.exists()) && (d.count() != 0))
	// Create the Importer Extension to Plugin data qmap.
	createMap();
}  // void gtGetText::loadImporterPlugins()

// Creates the dialog for the user to import a file based on the supported file formats.
ImportSetup gtGetText::run()
{
	// Initialize a filters list.
	QString filters = "";
	// Create a string for the "All supported files filter". Start with the label then loop through
	// the importers vector and add all of the file extensions supported.
	QString allSupported = QObject::tr("All Supported Formats") + " (";
	// Loop through the importers vector.
	for (uint i = 0; i < importers.size(); ++i)
	{
		// If there are any file extnsions declared by the importer
		if (importers[i].fileEndings.count() != 0)
		{
			// Add the importer name to the filters list
			filters += importers[i].fileFormatName + " (";
			// Loop though the extensions supported by the importer
			for (int j = 0; j < importers[i].fileEndings.count(); ++j)
			{
				// Add the extension to both the filter and allSupported strings
				filters += "*." + importers[i].fileEndings[j] + " ";
				allSupported += "*." + importers[i].fileEndings[j] + " ";
			}  // for (int j = 0; j < importers[i].fileEndings.count(); ++j)
			// Trim the Qstring
			filters = filters.trimmed();
			// Append "entry of entry" information to the end of the filter.
			filters += ");;";
		}  // if (importers[i].fileEndings.count() != 0)
	}  // for (uint i = 0; i < importers.size(); ++i)
	// Trim the allSupported QString and append "end of entry" data to the end of it.
	allSupported = allSupported.trimmed();
	allSupported += ");;";
	// Prepend allSupported to the filters Qstring.
	filters = allSupported + filters;
	// Add an "all files" entry to the end of the filters QString
	filters += QObject::tr("All Files (*)");
	// Populate ilist with the file importer names.
	for (uint i = 0;  i < importers.size(); ++i)
		ilist.append(importers[i].fileFormatName);
	// Create a new dialog.
	dias = new gtDialogs();
	// Create a new ImportSetup struct
	ImportSetup impsetup;
	// INitialize runDialog to false
	impsetup.runDialog=false;
	// If we get a true back from the File selection Dialog ( which we send our filters and extensions lists )
	if (dias->runFileDialog(filters, ilist))
	{
		// Set the runDialog to true
		impsetup.runDialog=true;
		// Copy the other values for the struct from the dialog results
		impsetup.encoding=dias->getEncoding();
		impsetup.filename=dias->getFileName();
		impsetup.importer=dias->getImporter();
		impsetup.textOnly=dias->importTextOnly();
// 		launchImporter(dias->getImporter(), dias->getFileName(),
// 		               dias->importTextOnly(), dias->getEncoding(), append);
	}  // if (dias->runFileDialog(filters, ilist))
	// Destroy the dialog.
	delete dias;
	// Return the ImportSetup struct.
	return impsetup;
}  // ImportSetup gtGetText::run()

// Loads, validates, and executes the Importer code.
void gtGetText::CallDLL(const ImporterData& idata, const QString& filePath,
                        const QString& encoding, bool textOnly, bool append, PageItem* importItem)
{
	// Pointer for the loaded plugin.
	void* gtplugin;
	// Type definition for GetText pointer in the function in question.
	typedef void (*sdem)(QString filename, QString encoding, bool textOnly, gtWriter *writer);
	// The point to the above.
	sdem fp_GetText;
	// Initialize Path to the "DLL"
	QString pluginFilePath = QString("%1/gettext/%2").arg(ScPaths::instance().pluginDir()).arg(idata.soFilePath);
	// Attempt to load the plugin, store the pointer in gtplugin
	gtplugin = PluginManager::loadDLL(pluginFilePath);
	// If gtplugin is NULL we failed to load the plugin. Report an error to the user and exit the method.
	if (!gtplugin)
	{
		qWarning("Failed to load plugin %s", pluginFilePath.toAscii().constData());
		return;
	} // if (!gtplugin)
	// Attempt to map the GetText method to to the pointer via the PluginManager. Store the result in fp_GetText.
	fp_GetText = (sdem) PluginManager::resolveSym(gtplugin, "GetText");
	// If fp_GetText is NULL, we could not find the symbol, report the error, unload the "DLL" and exit the method.
	if (!fp_GetText)
	{
		qWarning("Failed to get GetText() from %s", pluginFilePath.toAscii().constData());
		PluginManager::unloadDLL(gtplugin);
		return;
	}  // if (!fp_GetText)
	// Create a new writer object in "append"'s mode ( true or false ) attached to the importItem
	gtWriter *w = new gtWriter(append, importItem);
	// Execute the importer's "GetText" method.
	(*fp_GetText)(filePath, encoding, textOnly, w);
	// Destroy the writer
	delete w;
	// Unload the plugin.
	PluginManager::unloadDLL(gtplugin);
}  // void gtGetText::CallDLL(const ImporterData& idata, const QString& filePath,
   //                     const QString& encoding, bool textOnly, bool append, PageItem* importItem)

// Loads the "DLL", validates the importer is good, populates the passed parameters with 
// the plugin information.
bool gtGetText::DLLName(QString name, QString *ffName, QStringList *fEndings)
{
	// Pointer to the plugin, once loaded
	void* gtplugin;
	// typedef of Qstring to map the importer name (FileFormatName) method results to.
	typedef QString (*sdem0)();
	// typedef of QStringList to map the file extensions supported method results to.
	typedef QStringList (*sdem1)();
	// The actual importer name object
	sdem0 fp_FileFormatName;
	// The actual extensions supported object
	sdem1 fp_FileExtensions;
	// Initialise the plugin file path ( with filename )
	QString pluginFilePath = QString("%1/gettext/%2").arg(ScPaths::instance().pluginDir()).arg(name);
	// Attempt to load the plugin.
	gtplugin = PluginManager::loadDLL(pluginFilePath);
	// if gtplugin is NULL we were unable to load the plugin. Return an error and exit the method.
	if (!gtplugin)
	{
		qWarning("Failed to load plugin %s", pluginFilePath.toAscii().constData());
		return false;
	}
	// Attempt to resolve the plugin symbol to the importer name (FileFormatName)
	fp_FileFormatName = (sdem0) PluginManager::resolveSym( gtplugin, "FileFormatName");
	// if fp_FileFormatName is NULL, we could not find the FileFormatName symbol. The plugin is incomplete.
	// Report an error, unload the plugin, and exit the method.
	if (!fp_FileFormatName)
	{
		qWarning("Failed to get FileFormatName() from %s", pluginFilePath.toAscii().constData());
		PluginManager::unloadDLL(gtplugin);
		return false;
	}
	// Attempt to resolve the plugin symbol to the list of supported file extensions.
	fp_FileExtensions = (sdem1) PluginManager::resolveSym( gtplugin, "FileExtensions");
	// if fp_FileExtensions is NULL, we could not find the FileExtensions symbol. The plugin is incomplete.
	// Report an error, unload the plugin, and exit the method.
	if (!fp_FileExtensions)
	{
		qWarning("Failed to get FileExtensions() from %s", pluginFilePath.toAscii().constData());
		PluginManager::unloadDLL(gtplugin);
		return false;
	}
	// Set the format name based on the resolved method.
	*ffName = (*fp_FileFormatName)();
	// Set the extensions list based on the resolved method.
	*fEndings = (*fp_FileExtensions)();
	// Unload the plugin
	PluginManager::unloadDLL(gtplugin);
	// Successfully return!
	return true;
}  // bool gtGetText::DLLName(QString name, QString *ffName, QStringList *fEndings)

// Create the importer Qmap.
void gtGetText::createMap()
{
	// Loop through the importers Vector
	for (uint i = 0; i < importers.size(); ++i)
	{
		// Loop through each file extension the importer uses/importers and create an individual 
		// Qmap entry for it.
		for (int j = 0; j < importers[i].fileEndings.count(); ++j)
				importerMap.insert(importers[i].fileEndings[j], &importers[i]);
	}  // for (uint i = 0; i < importers.size(); ++i)
}  // void gtGetText::createMap()

// Destructor
gtGetText::~gtGetText()
{
	// Nothing needs to happen here.
}
