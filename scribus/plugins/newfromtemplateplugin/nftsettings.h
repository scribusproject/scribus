/***************************************************************************
 *   Riku Leino, riku.leino@gmail.com                                          *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef NFTSETTINGS_H
#define NFTSETTINGS_H

#include "config.h"
#include <vector>
#include <qstring.h>
#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>
#include "nfttemplate.h"
#include "nftrcreader.h"
 
class nftsettings {
private:
	QString scribusShare;
	QString scribusUserHome;
	QString userTemplateDir;
	QString nftHomeDir;
	QString lang;
	nftrcreader* handler;
	QXmlSimpleReader* reader;
	void read();
	void addTemplates(QString dir);
	QString findTemplateXml(QString dir);
public:
	std::vector<nfttemplate*> templates;
	nftsettings(QString guilang, QString templateDir);
	~nftsettings();
};

#endif // NFTSETTINGS_H
