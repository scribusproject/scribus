/***************************************************************************
 *   Riku Leino, riku.leino@gmail.com                                          *
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
