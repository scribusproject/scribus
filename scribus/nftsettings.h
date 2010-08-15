/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#ifndef NFTSETTINGS_H
#define NFTSETTINGS_H

#include "scconfig.h"
#include <vector>
#include <QString>
#include <QDir>
#include <QFile>
#include "nfttemplate.h"
#include "nftrcreader.h"
 
class nftsettings {
private:
	QString scribusShare;
	QString scribusUserHome;
	QString nftHomeDir;
	QString lang;
	nftrcreader* handler;
	QXmlSimpleReader* reader;
	void read();
	void addTemplates(QString dir);
	QString findTemplateXml(QString dir);
public:
	std::vector<nfttemplate*> templates;
	nftsettings(QString guilang);
	~nftsettings();
};

#endif // NFTSETTINGS_H
