/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#include "nftsettings.h"

nftsettings::nftsettings(QString guilang)
{
	lang = guilang;
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
	// Add templates from the dir itself
	QString tmplFile = findTemplateXml(dir + "/templates");
	QFile* tmplxml = new QFile(QDir::convertSeparators(tmplFile));
	handler->setSourceDir(dir + "/templates");
	handler->setSourceFile(tmplFile);
	if (tmplxml->exists())
	{
		QXmlInputSource* source = new QXmlInputSource(tmplxml);
		reader->parse(source);
		delete source;
	}
	delete tmplxml;
	
	
	// And from all the subdirectories. template.xml file is only searched one dir level deeper than the dir
	QDir tmpldir(dir + "/templates");
	if (tmpldir.exists())
	{
		tmpldir.setFilter(QDir::Dirs);
		QStringList dirs = tmpldir.entryList();
		for (uint i = 0; i < dirs.size(); ++i)
		{
			if ((dirs[i] != ".") && (dirs[i] != "..")) {
				tmplFile = findTemplateXml(dir + "/templates/" + dirs[i]);
				QFile* tmplxml = new QFile(QDir::convertSeparators(tmplFile));
				handler->setSourceDir(dir+"/templates/"+dirs[i]);
				handler->setSourceFile(tmplFile);
				if (tmplxml->exists())
				{
					QXmlInputSource* source = new QXmlInputSource(tmplxml);
					reader->parse(source);
					delete source;
				}
				delete tmplxml;
			}
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
	if ( rc.open( IO_WriteOnly ) )
	{
		QTextStream stream(&rc);
		stream.setEncoding(QTextStream::UnicodeUTF8);
		stream << text;
		rc.close();
	}
}

QString nftsettings::findTemplateXml(QString dir)
{
	QString tmp = dir + "/template." + lang + ".xml";
	if (QFile(tmp).exists())
		return tmp;

	if (lang.length() > 2)
	{
		tmp = dir + "/template." + lang.left(2) + ".xml";
		if (QFile(tmp).exists())
			return tmp;
	}
	return dir + "/template.xml";	
}

nftsettings::~ nftsettings()
{
	delete reader;
	delete handler;
	for (uint i = 0; i < templates.size(); ++i)
	{
		if (templates[i] != NULL)
			delete templates[i];
	}
}
