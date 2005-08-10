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

#ifndef GTGETTEXT_H
#define GTGETTEXT_H

#include <iostream>
#include <vector>

#include <qdir.h>
#include <qmap.h>
#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>

#include "scconfig.h"
#include "scribusapi.h"
#include "gtdialogs.h"
#include "gtwriter.h"
#include "scfonts.h"
#include "pageitem.h"

struct ImporterData {
	QString     soFilePath;
	QString     fileFormatName;
	QStringList fileEndings;
};

/*
	GetText handles the open file dialog and importer plugins loading and launching.
*/
class SCRIBUS_API gtGetText
{
private:
	std::vector<ImporterData> importers;
	QMap<QString, ImporterData*> importerMap; // map ImporterDatas to file endings for easy launc for desired plugin
	void loadImporterPlugins();
	void launchImporter(int importer, const QString& filename, bool textOnly, const QString& encoding, bool append);
	void CallDLL(const ImporterData& idata, const QString& filePath,
	             const QString& encoding, bool textOnly, bool append);
	bool DLLName(QString name, QString *ffName, QStringList *fileEndings);
	void createMap();
	gtDialogs* dias;
	QStringList ilist;
public:
	gtGetText();
	~gtGetText();
	void run(bool append);
};

#endif
