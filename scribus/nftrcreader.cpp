/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/

#include "nftrcreader.h"

using namespace std;

nftrcreader::nftrcreader(std::vector<nfttemplate*> *tmplts,QString sourceDir)
{
	templates = tmplts;
	currentDir = sourceDir;
	setupCategories();
}

bool nftrcreader::startDocument()
{
	inSettings       = false;
	inTemplate       = false;
	inName           = false;
	inFile           = false;
	inTNail          = false;
	inImg            = false;
	inPSize          = false;
	inColor          = false;
	inDescr          = false;
	inUsage          = false;
	inScribusVersion = false;
	inDate           = false;
	inAuthor         = false;
	inEmail          = false;
	return true;
}

bool nftrcreader::startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs) 
{
	if (name == "name")
		inName = true;
	else if (name == "file")
		inFile = true;
	else if (name == "tnail")
		inTNail = true;
	else if (name == "img")
		inImg = true;
	else if (name == "psize")
		inPSize = true;
	else if (name == "color") 
		inColor = true;
	else if (name == "descr")
		inDescr = true;
	else if (name == "usage")
		inUsage = true;
	else if (name == "scribus_version")
		inScribusVersion = true;
	else if (name == "date")
		inDate = true;
	else if (name == "author")
		inAuthor = true;
	else if (name == "email")
		inEmail = true;


	if (name == "template") // new template starts here
	{
		inTemplate = true;
		QString category;
		QString enCat;
		for (int i = 0; i < attrs.count(); i++)
		{
			if (attrs.localName(i) == "category")
			{
				category = getCategory(attrs.value(i));
				enCat = attrs.value(i);
			}
		}
		tmpTemplate = new nfttemplate(currentFile, category); // create a new template
		tmpTemplate->enCategory = enCat;
	}

	if (name == "settings") 
		inSettings = true;

	return true;
}

bool nftrcreader::characters(const QString &ch) 
{
	QString tmp(ch);
	if (inName)
		tmpTemplate->name = tmp;
	else if (inFile)
	{
		if (tmp.left(1) == "/")
			tmpTemplate->file = tmp;
		else 
			tmpTemplate->file = currentDir + "/" + tmp;
	} else if (inTNail)
	{
		if (tmp.left(1) == "/")
			tmpTemplate->tnail = tmp;
		else
			tmpTemplate->tnail = currentDir + "/" + tmp;
	} else if (inImg)
	{
		if (tmp.left(1) == "/") 
			tmpTemplate->img = tmp;
		else
			tmpTemplate->img = currentDir + "/" + tmp;
	} else if (inPSize)
		tmpTemplate->psize = tmp; 
	else if (inColor)
		tmpTemplate->color = tmp; 
	else if (inDescr)
		tmpTemplate->descr = tmp; 
	else if (inUsage)
		tmpTemplate->usage = tmp;
	else if (inScribusVersion)
		tmpTemplate->scribusVersion = tmp;
	else if (inDate)
		tmpTemplate->date = tmp;
	else if (inAuthor)
		tmpTemplate->author = tmp;
	else if (inEmail)
		tmpTemplate->email = tmp; 

	return true;
}

bool nftrcreader::endElement(const QString&, const QString&, const QString &name)
{
	if (name == "template")
	{ // new template starts here
		inTemplate = false;
		if (tmpTemplate != NULL) // If we have a template already created
		{
			if (tmpTemplate->isValid()) // and the template really exists push
				templates->push_back(tmpTemplate); // it to the templates vector and start a new one
			else
			{
				delete tmpTemplate;
				tmpTemplate = 0;
			}
		}
	}
	if (inTemplate) {
		if (inName && name == "name")
			inName = false;
		else if (inFile && name == "file")
			inFile = false;
		else if (inTNail && name == "tnail")
			inTNail = false;
		else if (inImg && name == "img")
			inImg = false;
		else if (inPSize && name == "psize")
			inPSize = false;
		else if (inColor && name == "color") 
			inColor = false;
		else if (inDescr && name == "descr")
			inDescr = false;
		else if (inUsage && name == "usage")
			inUsage = false;
		else if (inScribusVersion && name == "scribus_version")
			inScribusVersion = false;
		else if (inDate && name == "date")
			inDate = false;
		else if (inAuthor && name == "author")
			inAuthor = false;
		else if (inEmail && name == "email")
			inEmail = false;
		else if (inTemplate && name == "template")
			inTemplate = false;
	}
	
	if (inSettings && name == "settings")
		inSettings = false;

	return true;
}

void nftrcreader::setSourceDir(QString source) 
{
	currentDir = source;
}

void nftrcreader::setSourceFile(QString sourceFile)
{
	currentFile = sourceFile;
}

QString nftrcreader::getCategory(QString cat) 
{
	QMap<QString, QString>::iterator it = cats.find(cat);
	if (it==cats.end())
		cats.insert(cat, cat);
	if (it != cats.end())
		return it.value();
	return cat;
}

void nftrcreader::setupCategories()
{
	// en will be used in template.xml and it will be then replaced with the lang when used for users
	// to get the categories in their language.
	cats.insert(QString("Newsletters"), QObject::tr("Newsletters"));
	cats.insert(QString("Brochures")  , QObject::tr("Brochures"));
	cats.insert(QString("Catalogs")   , QObject::tr("Catalogs"));
	cats.insert(QString("Flyers")     , QObject::tr("Flyers"));
	cats.insert(QString("Signs")      , QObject::tr("Signs"));
	cats.insert(QString("Cards")      , QObject::tr("Cards"));
	cats.insert(QString("Letterheads"), QObject::tr("Letterheads"));
	cats.insert(QString("Envelopes")  , QObject::tr("Envelopes"));
	cats.insert(QString("Business Cards"), QObject::tr("Business Cards"));
	cats.insert(QString("Calendars")  , QObject::tr("Calendars"));
	cats.insert(QString("Advertisements"), QObject::tr("Advertisements"));
	cats.insert(QString("Labels")     , QObject::tr("Labels"));
	cats.insert(QString("Menus")      , QObject::tr("Menus"));
	cats.insert(QString("Programs")   , QObject::tr("Programs"));
	cats.insert(QString("PDF Forms")  , QObject::tr("PDF Forms"));
	cats.insert(QString("PDF Presentations") , QObject::tr("PDF Presentations"));
	cats.insert(QString("Magazines")  , QObject::tr("Magazines"));
	cats.insert(QString("Posters")    , QObject::tr("Posters"));
	cats.insert(QString("Announcements") , QObject::tr("Announcements"));
	cats.insert(QString("Text Documents"), QObject::tr("Text Documents"));
	cats.insert(QString("Folds")        , QObject::tr("Folds"));
	cats.insert(QString("Media Cases")  , QObject::tr("Media Cases"));
	cats.insert(QString("Own Templates"), QObject::tr("Own Templates"));
}
