/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#include "nftsettings.h"
#include <iostream>

nftsettings::nftsettings()
{
	scribusShare = PREL;
	scribusShare += "/share/scribus";
	scribusUserHome = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus");
	nftRCFile = QDir::convertSeparators(scribusUserHome+"/nftrc.xml");
	read();
}

void nftsettings::read()
{
	QDir scribus(scribusUserHome);
	if (!scribus.exists("nftrc.xml")) {
		getDefaults();
	}
	
	handler = new nftrcreader(&templates,scribusUserHome);
	
	QFile* rc = new QFile(nftRCFile);
	QXmlInputSource* source = new QXmlInputSource(rc);
	
	reader = new QXmlSimpleReader();
	reader->setContentHandler(handler);
	reader->parse(source);
	delete source;
	delete rc;
	
	addTemplates(scribusShare);
	addTemplates(scribusUserHome);
}

void nftsettings::addTemplates(QString dir) // dir will be searched for a sub folder called templates
{
	QDir tmpldir(dir + "/templates");
	if (tmpldir.exists()) {
		tmpldir.setFilter(QDir::Dirs);
		QStringList dirs = tmpldir.entryList();
		for (uint i = 0; i < dirs.size(); i++) {
			QFile* tmplxml = new QFile(QDir::convertSeparators(dir + "/templates/" + dirs[i] + "/template.xml"));
			handler->setSourceDir(dir+"/templates/"+dirs[i]);
			if (tmplxml->exists()) {
				QXmlInputSource* source = new QXmlInputSource(tmplxml);
				reader->parse(source);
				delete source;
			}
			delete tmplxml;
		}
	}
}

void nftsettings::write() 
{
// 	QFile rc(nftRCFile);
}

void nftsettings::getDefaults()
{
	QString tPrel = PREL;
	QString tmplDir = tPrel + "/share/scribus/templates";
	// Create the defaults ~/.scribus/nft/nftrc.xml file
	QString text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	text += "<nft>\n";
	text += "\t<settings>\n";
	text += "\t</settings>\n";
	text += "</nft>\n";

	QFile rc(nftRCFile);
	if ( rc.open( IO_WriteOnly ) ) {
		QTextStream stream(&rc);
		stream.setEncoding(QTextStream::UnicodeUTF8);
		stream << text;
		rc.close();
	}
}

nftsettings::~ nftsettings()
{
	delete reader;
	delete handler;
	for (uint i = 0; i < templates.size(); i++) {
		delete templates[i];
	}
}
