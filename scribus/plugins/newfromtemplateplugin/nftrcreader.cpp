/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
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

	
	if (name == "template") { // new template starts here
		inTemplate = true;
		QString category;
		for (int i = 0; i < attrs.count(); i++) {
			if (attrs.localName(i) == "category") {
				category = getCategory(attrs.value(i));
			}
		}
		tmpTemplate = new nfttemplate(category); // create a new template
	}
	
	if (name == "settings") {
		inSettings = true;
	}

	return true;
}

bool nftrcreader::characters(const QString &ch) 
{
	QString tmp(ch);
	if (inName) {
		tmpTemplate->name = tmp;
	} else if (inFile) {
		if (tmp.left(1) == "/")
			tmpTemplate->file = tmp;
		else 
			tmpTemplate->file = currentDir + "/" + tmp;
	} else if (inTNail) {
		if (tmp.left(1) == "/")
			tmpTemplate->tnail = tmp;
		else
			tmpTemplate->tnail = currentDir + "/" + tmp; 
	} else if (inImg) {
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
	if (name == "template") { // <template id="1">, new template starts here
		inTemplate = false;
		if (tmpTemplate != NULL) { // If we have a template already created push
			templates->push_back(tmpTemplate); // it to the templates vector and start a new one
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

QString nftrcreader::getCategory(QString cat) 
{
	for (uint i = 0; i < cats.size(); i++) {
		if (cat == *cats[i]->first)
			return *cats[i]->second;
	}
	
	return cat;
}

void nftrcreader::setupCategories()
{
	// en will be used in template.xml and it will be then replaced with the lang when used for users
	// to get the categories in their language.
	QString* en = new QString("Newsletters"); QString* lang = new QString(QObject::tr("Newsletters"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Brochures"); lang = new QString(QObject::tr("Brochures"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Catalogs"); lang = new QString(QObject::tr("Catalogs"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Flyers"); lang = new QString(QObject::tr("Flyers"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Signs"); lang = new QString(QObject::tr("Signs"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Cards"); lang = new QString(QObject::tr("Cards"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Letterheads"); lang = new QString(QObject::tr("Letterheads"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Envelopes"); lang = new QString(QObject::tr("Envelopes"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Business Cards"); lang = new QString(QObject::tr("Business Cards"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Calendars"); lang = new QString(QObject::tr("Calendars"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Advertisements"); lang = new QString(QObject::tr("Advertisements"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Labels"); lang = new QString(QObject::tr("Labels"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Menus"); lang = new QString(QObject::tr("Menus"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Programs"); lang = new QString(QObject::tr("Programs"));
	cats.push_back(new Pair(en,lang));
	en = new QString("PDF Forms"); lang = new QString(QObject::tr("PDF Forms"));
	cats.push_back(new Pair(en,lang));
	en = new QString("PDF Presentations"); lang = new QString(QObject::tr("PDF Presentations"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Magazines"); lang = new QString(QObject::tr("Magazines"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Posters"); lang = new QString(QObject::tr("Posters"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Announcements"); lang = new QString(QObject::tr("Announcements"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Text Documents"); lang = new QString(QObject::tr("Text Documents"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Folds"); lang = new QString(QObject::tr("Folds"));
	cats.push_back(new Pair(en,lang));
	en = new QString("Own Templates"); lang = new QString(QObject::tr("Own Templates"));
	cats.push_back(new Pair(en,lang));
}

nftrcreader::~nftrcreader()
{
	for (uint i = 0; i < cats.size(); i++) {
		delete cats[i];
	}
}
