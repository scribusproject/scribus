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
	m_templates = tmplts;
	m_currentDir = sourceDir;
	setupCategories();
}

bool nftrcreader::parse(QString filePath)
{
	bool firstElement = true;

	QFile file(filePath);
	if (!file.open(QFile::ReadOnly))
		return false;

	QXmlStreamReader reader(&file);
	while (!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (tType != QXmlStreamReader::StartElement)
			continue;

		QStringRef tagName = reader.name();
		if (firstElement)
		{
			if (tagName != "templates")
				return false;
			firstElement = false;
			continue;
		}

		if (tagName == "template")
		{
			QXmlStreamAttributes attrs = reader.attributes();
			QString attrCat  = attrs.value("category").toString();
			QString category = getCategory(attrCat);

			nfttemplate* tmpTemplate = new nfttemplate(m_currentFile, category);
			tmpTemplate->enCategory = attrCat;
			if (parseTemplate(reader, tmpTemplate))
				m_templates->push_back(tmpTemplate);
			else
				delete tmpTemplate;
		}
		else
		{
			reader.skipCurrentElement();
		}
	}

	return (!reader.hasError());
}

bool nftrcreader::parseTemplate(QXmlStreamReader& reader, nfttemplate* nftTemplate)
{
	if (!reader.isStartElement())
		return false;

	QStringRef elemName = reader.name();
	if (elemName != "template")
		return false;

	while (!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType tType = reader.readNext();
		if (reader.hasError())
			return false;

		if (tType == QXmlStreamReader::EndElement)
			break;
		if (tType != QXmlStreamReader::StartElement)
			continue;

		QStringRef tagName = reader.name();
		if (tagName == "name")
			nftTemplate->name = reader.readElementText();
		else if (tagName == "file")
		{
			QString tmp = reader.readElementText();
			if (tmp.left(1) == "/")
				nftTemplate->file = tmp;
			else 
				nftTemplate->file = m_currentDir + "/" + tmp;
		}
		else if (tagName == "tnail")
		{
			QString tmp = reader.readElementText();
			if (tmp.left(1) == "/")
				nftTemplate->tnail = tmp;
			else 
				nftTemplate->tnail = m_currentDir + "/" + tmp;
		}
		else if (tagName == "img")
		{
			QString tmp = reader.readElementText();
			if (tmp.left(1) == "/")
				nftTemplate->img = tmp;
			else 
				nftTemplate->img = m_currentDir + "/" + tmp;
		}
		else if (tagName == "psize")
			nftTemplate->psize = reader.readElementText();
		else if (tagName == "color")
			nftTemplate->color = reader.readElementText();
		else if (tagName == "descr")
			nftTemplate->descr = reader.readElementText();
		else if (tagName == "usage")
			nftTemplate->usage = reader.readElementText();
		else if (tagName == "scribus_version")
			nftTemplate->scribusVersion = reader.readElementText();
		else if (tagName == "date")
			nftTemplate->date = reader.readElementText();
		else if (tagName == "author")
			nftTemplate->author = reader.readElementText();
		else if (tagName == "email")
			nftTemplate->email = reader.readElementText();
		else
		{
			reader.skipCurrentElement();
		}
	}

	return (nftTemplate->isValid());
}

void nftrcreader::setSourceDir(QString source) 
{
	m_currentDir = source;
}

void nftrcreader::setSourceFile(QString sourceFile)
{
	m_currentFile = sourceFile;
}

QString nftrcreader::getCategory(QString cat) 
{
	QMap<QString, QString>::iterator it = m_cats.find(cat);
	if (it==m_cats.end())
		m_cats.insert(cat, cat);
	if (it != m_cats.end())
		return it.value();
	return cat;
}

void nftrcreader::setupCategories()
{
	// en will be used in template.xml and it will be then replaced with the lang when used for users
	// to get the categories in their language.
	m_cats.insert(QString("Advertisements"), QObject::tr("Advertisements"));
	m_cats.insert(QString("Announcements") , QObject::tr("Announcements"));
	m_cats.insert(QString("Brochures")  , QObject::tr("Brochures"));
	m_cats.insert(QString("Business Cards"), QObject::tr("Business Cards"));
	m_cats.insert(QString("Calendars")  , QObject::tr("Calendars"));
	m_cats.insert(QString("Cards")      , QObject::tr("Cards"));
	m_cats.insert(QString("Catalogs")   , QObject::tr("Catalogs"));
	m_cats.insert(QString("Envelopes")  , QObject::tr("Envelopes"));
	m_cats.insert(QString("Flyers")     , QObject::tr("Flyers"));
	m_cats.insert(QString("Folds")      , QObject::tr("Folds"));
	m_cats.insert(QString("Grids")      , QObject::tr("Grids"));
	m_cats.insert(QString("Labels")     , QObject::tr("Labels"));
	m_cats.insert(QString("Letterheads"), QObject::tr("Letterheads"));
	m_cats.insert(QString("Magazines")  , QObject::tr("Magazines"));
	m_cats.insert(QString("Media Cases")  , QObject::tr("Media Cases"));
	m_cats.insert(QString("Menus")      , QObject::tr("Menus"));
	m_cats.insert(QString("Newsletters"), QObject::tr("Newsletters"));
	m_cats.insert(QString("Own Templates"), QObject::tr("Own Templates"));
	m_cats.insert(QString("PDF Forms")  , QObject::tr("PDF Forms"));
	m_cats.insert(QString("PDF Presentations") , QObject::tr("PDF Presentations"));
	m_cats.insert(QString("Posters")    , QObject::tr("Posters"));
	m_cats.insert(QString("Programs")   , QObject::tr("Programs"));
	m_cats.insert(QString("Signs")      , QObject::tr("Signs"));
	m_cats.insert(QString("Text Documents"), QObject::tr("Text Documents"));
}
