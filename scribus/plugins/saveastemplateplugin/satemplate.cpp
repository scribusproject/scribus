/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#include "satemplate.h"
#include "satdialog.h"

#include "scpaths.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "prefsfile.h"
#include "prefsmanager.h"

int saveastemplateplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* saveastemplateplugin_getPlugin()
{
	SaveAsTemplatePlugin* plug = new SaveAsTemplatePlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void saveastemplateplugin_freePlugin(ScPlugin* plugin)
{
	SaveAsTemplatePlugin* plug = dynamic_cast<SaveAsTemplatePlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

SaveAsTemplatePlugin::SaveAsTemplatePlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

SaveAsTemplatePlugin::~SaveAsTemplatePlugin() {};

void SaveAsTemplatePlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "SaveAsDocumentTemplate";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Save as &Template...");
	// Shortcut
	m_actionInfo.keySequence = "Ctrl+Alt+S";
	// Menu
	m_actionInfo.menu = "File";
	m_actionInfo.menuAfterName = "SaveAs";
	m_actionInfo.enabledOnStartup = true;
	m_actionInfo.needsNumObjects = -1;
}

const QString SaveAsTemplatePlugin::fullTrName() const
{
	return QObject::tr("Save As Template");
}

const ScActionPlugin::AboutData* SaveAsTemplatePlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Riku Leino <riku@scribus.info>");
	about->shortDescription = tr("Save a document as a template");
	about->description = tr("Save a document as a template. Good way to ease the "
	                        "initial work for documents with a constant look");
    // about->version
    // about->releaseDate
    // about->copyright
	about->license = "GPL";
	return about;
}

void SaveAsTemplatePlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool SaveAsTemplatePlugin::run(ScribusDoc* doc, QString target)
/*{
	Q_ASSERT(target.isEmpty());
	Sat = new MenuSAT();
	Sat->RunSATPlug();
	delete Sat;
	Sat = 0;
	return true;
}
*/
/* jghali's fix when the new file dialog is cancelled SaT is still active in the menu - PL */
{
	m_Doc=doc;
 	if ( m_Doc )
	{
		Q_ASSERT(target.isEmpty());
		Sat = new MenuSAT();
		Sat->RunSATPlug(m_Doc);
		delete Sat;
		Sat = 0;
	}
	return true;
}
void MenuSAT::RunSATPlug(ScribusDoc* doc)
{
	QDir templates(ScPaths::getApplicationDataDir());
	if (!templates.exists("templates"))
	{
		templates.mkdir("templates");
	}
	QString currentPath(QDir::currentPath());
	QString currentFile(doc->DocName);
	bool hasName = doc->hasName;
	bool isModified = doc->isModified();
	QString userTemplatesDir = PrefsManager::instance()->appPrefs.documentTemplatesDir;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString oldCollect = dirs->get("collect", ".");
	QString templatesDir = ".";
	if (userTemplatesDir.isEmpty())
		templatesDir = ScPaths::getApplicationDataDir() + "templates";
	else
	{
		if (userTemplatesDir.right(1) == "/")
			userTemplatesDir = userTemplatesDir.left(userTemplatesDir.length() - 1);
		templatesDir = userTemplatesDir;
	}
	dirs->set("collect", templatesDir);
	if (doc->scMW()->Collect().isEmpty())
		return;
	if (oldCollect != ".")
		dirs->set("collect", oldCollect);
	QString docPath = doc->DocName;
	QString docDir = docPath.left(docPath.lastIndexOf('/'));
	QString docName = docPath.right(docPath.length() - docPath.lastIndexOf('/') - 1);
	docName = docName.left(docName.lastIndexOf(".s"));

	if (currentFile !=  doc->DocName)
	{
		satdialog* satdia = new satdialog(doc->scMW(),docName,
                                          static_cast<int>(doc->pageWidth + 0.5),
                                          static_cast<int>(doc->pageHeight + 0.5));
		if (satdia->exec())
		{
			sat* s = new sat(doc, satdia, docPath.right(docPath.length() - docPath.lastIndexOf('/') - 1),docDir);
			s->createImages();
			s->createTmplXml();
			delete s;
		}
		// Restore the state that was before ScMW->Collect()
		doc->DocName = currentFile;
		doc->hasName = hasName;
		doc->setModified(isModified);
		QString newCaption=currentFile;
		if (isModified)
			newCaption.append('*');
		doc->scMW()->updateActiveWindowCaption(newCaption);
		doc->scMW()->removeRecent(docPath);
		QDir::setCurrent(currentPath);
		delete satdia;
	}
}

// --------------------- CLASS sat ------------------------------------------------//

sat::sat(ScribusDoc* doc, satdialog* satdia, QString fileName, QString tmplDir)
{
	lang = ScCore->getGuiLanguage();
	m_Doc = doc;
	dia = satdia;
	dir = tmplDir;
	if (dir.right(1) == "/")
		dir = tmplDir.left(tmplDir.length() - 1);
	file = fileName;
	tmplXmlFile = findTemplateXml(dir);
}

void sat::createTmplXml()
{
	QFile tmplXml(tmplXmlFile);
	if (tmplXml.exists())
	{
		appendTmplXml();
		return; // We don't want to overwrite the template.xml file
	}
	QString xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	xml += "<templates>\n";
	xml += getTemplateTag();
	xml += "</templates>\n";
	if ( tmplXml.open( QIODevice::WriteOnly ) )
	{
		QTextStream stream(&tmplXml);
		stream.setCodec("UTF-8");
		stream << xml;
		tmplXml.close();
	}
}

void sat::createImages()
{
	QString tnsmallName = dia->nameEdit->text() + "tn.png";
	QString tnlargeName = dia->nameEdit->text() + ".png";
	double pageh = m_Doc->pageHeight;
	double pagew = m_Doc->pageWidth;
	int pageSizeSmall = 0;
	int pageSizeLarge = 0;
	if (pageh > pagew)
	{
		pageSizeSmall = static_cast<int>(pageh / 10);
		pageSizeLarge = static_cast<int>(pageh / 3);
	}
	else
	{
		pageSizeSmall = static_cast<int>(pagew / 10);
		pageSizeLarge = static_cast<int>(pagew / 3);
	}
	QImage tnsmall = m_Doc->view()->PageToPixmap(0,pageSizeSmall);
	QImage tnlarge = m_Doc->view()->PageToPixmap(0,pageSizeLarge);
	tnsmall.save(dir+"/"+tnsmallName,"PNG",70);
	tnlarge.save(dir+"/"+tnlargeName, "PNG", 70);
}

void sat::appendTmplXml()
{
	QFile tmplXml(tmplXmlFile);
	if (tmplXml.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&tmplXml);
		QString tmp = stream.readLine();
		QString file = "";
		while (!tmp.isNull())
		{
			file += tmp + "\n";
			tmp = stream.readLine();
			if (tmp.indexOf("</templates>") != -1)
				file += getTemplateTag();
		}
		tmplXml.close();
		if ( tmplXml.open( QIODevice::WriteOnly ) )
		{
			QTextStream stream2(&tmplXml);
			stream2.setCodec("UTF-8");
			stream2 << file;
			tmplXml.close();
		}
	}
}

QString sat::getTemplateTag()
{
	QString category = dia->catsCombo->currentText();
	if (category.isEmpty())
		category = QObject::tr("Own Templates");
	else
		category = dia->cats.key(category, category);
	QDate now = QDate::currentDate();
	QString cat = QString(category);
	replaceIllegalChars(cat);
	QString tag = "\t<template category=\""+cat+"\">\n";
	QString name = QString(dia->nameEdit->text());
	replaceIllegalChars(name);
	tag += "\t\t<name>"+name+"</name>\n";
	tag += "\t\t<file>"+file+"</file>\n";
	tag += "\t\t<tnail>"+name+"tn.png"+"</tnail>\n";
	tag += "\t\t<img>"+name+".png"+"</img>\n";
	QString psize = QString(dia->psizeEdit->text());
	replaceIllegalChars(psize);
	tag += "\t\t<psize>"+psize+"</psize>\n";
	QString colors = QString(dia->colorsEdit->text());
	replaceIllegalChars(colors);
	tag += "\t\t<color>"+colors+"</color>\n";
	QString descr = QString(dia->descrEdit->toPlainText());
	replaceIllegalChars(descr);
	tag += "\t\t<descr>"+descr+"</descr>\n";
	QString usage = QString(dia->usageEdit->toPlainText());
	replaceIllegalChars(usage);
	tag += "\t\t<usage>"+usage+"</usage>\n";
	QString scribus_version = QString(VERSION);
	replaceIllegalChars(scribus_version);
	tag += "\t\t<scribus_version>" + scribus_version + "</scribus_version>\n";
	QString date = QString(now.toString(Qt::ISODate));
	replaceIllegalChars(date);
	tag += "\t\t<date>" + date + "</date>\n";
	QString author = QString(dia->authorEdit->text());
	replaceIllegalChars(author);
	tag += "\t\t<author>"+author+"</author>\n";
	QString email = QString(dia->emailEdit->text());
	replaceIllegalChars(email);
	tag += "\t\t<email>"+email+"</email>\n";
	tag += "\t</template>\n";

	return tag;
}

void sat::replaceIllegalChars(QString& s)
{
	s.replace("&amp;", "&");
	s.replace("&lt;", "<");
	s.replace("&gt;", ">");
	s.replace("&quot;", "\"");
	s.replace("&apos;", "\'");
	s.replace("&", "&amp;");
	s.replace("<", "&lt;");
	s.replace(">", "&gt;");
	s.replace("\"", "&quot;");
	s.replace("\'", "&apos;");
}

QString sat::findTemplateXml(QString dir)
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

sat::~sat()
{
}
