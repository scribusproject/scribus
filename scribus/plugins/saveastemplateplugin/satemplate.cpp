/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#include "satemplate.h"
#include "satemplate.moc"
#include <prefsfile.h>
#include <pluginmanager.h>

extern PrefsFile* prefsFile;
ScribusApp* Carrier;
QWidget* par;

QString name()
{
    return QObject::tr("Save as &Template...");
}

PluginManager::PluginType type()
{
	return PluginManager::Standard;
}

int ID()
{
	return 7;
}

QString actionName()
{
	return "SaveAsTemplate";
}

QString actionKeySequence()
{
	return "Ctrl+Alt+S";
}

QString actionMenu()
{
	return "File";
}

QString actionMenuAfterName()
{
	return "SaveAs";
}

bool actionEnabledOnStartup()
{
	return false;
}
/*
void InitPlug(QWidget *d, ScribusApp *plug)
{
	Carrier = plug;
	par = d;
	satm = new MenuSAT(d);
	int id = plug->fileMenu->insertItem(QObject::tr("Save as &Template..."), -1, plug->fileMenu->indexOf(plug->M_FileSaveAs)+1);
	plug->fileMenu->connectItem(id, satm, SLOT(RunSATPlug()));
	plug->fileMenu->setItemEnabled(id, 0);
	plug->MenuItemsFile.append(id);
}
*/
void cleanUpPlug()
{}

void run(QWidget *d, ScribusApp *plug)
{
	Carrier = plug;
	par = d;
	Sat = new MenuSAT(d);
	Sat->RunSATPlug();
}

void MenuSAT::RunSATPlug()
{
	QDir templates(QDir::homeDirPath() + "/.scribus");
	if (!templates.exists("templates"))
	{
		templates.mkdir("templates");
	}
	QString currentDirPath = QDir::currentDirPath();
	QString currentFile = Carrier->doc->DocName;
	bool hasName = Carrier->doc->hasName;
	bool isModified = Carrier->doc->isModified();
	QString userTemplatesDir = Carrier->Prefs.TemplateDir;
	PrefsContext* dirs = prefsFile->getContext("dirs");
	QString oldCollect = dirs->get("collect", ".");
	QString templatesDir = ".";
	if (userTemplatesDir == "")
		templatesDir = QDir::homeDirPath() + "/.scribus/templates";
	else
	{
		if (userTemplatesDir.right(1) == "/")
			userTemplatesDir = userTemplatesDir.left(userTemplatesDir.length() - 1);
		templatesDir = userTemplatesDir;
	}
	dirs->set("collect", templatesDir);
	if (Carrier->Collect() == "")
		return;
	if (oldCollect != ".")
		dirs->set("collect", oldCollect);
	QString docPath = Carrier->doc->DocName;
	QString docDir = docPath.left(docPath.findRev('/'));
	QString docName = docPath.right(docPath.length() - docPath.findRev('/') - 1);
	docName = docName.left(docName.findRev(".s"));

	if (currentFile !=  Carrier->doc->DocName)
	{
		satdialog* satdia = new satdialog(par,docName,
                                          static_cast<int>(Carrier->doc->PageB + 0.5),
                                          static_cast<int>(Carrier->doc->PageH + 0.5));
		if (satdia->exec())
		{
			sat* s = new sat(Carrier, satdia, docPath.right(docPath.length() - docPath.findRev('/') - 1),docDir);
			s->createImages();
			s->createTmplXml();
			delete s;
		}
		// Restore the state that was before Carrier->Collect()
		Carrier->doc->DocName = currentFile;
		Carrier->doc->hasName = hasName;
		if (isModified)
		{
			Carrier->doc->setModified();
			Carrier->ActWin->setCaption(currentFile+"*");
		}
		else
		{
			Carrier->doc->setUnModified();
			Carrier->ActWin->setCaption(currentFile);
		}
		Carrier->removeRecent(docPath);
		QDir::setCurrent(currentDirPath);
		delete satdia;
	}
}

// --------------------- CLASS sat ------------------------------------------------//

sat::sat(ScribusApp* scribusApp, satdialog* satdia, QString fileName, QString tmplDir)
{
	lang = scribusApp->getGuiLanguage();
	sapp = scribusApp;
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
	if ( tmplXml.open( IO_WriteOnly ) )
	{
		QTextStream stream(&tmplXml);
		stream.setEncoding(QTextStream::UnicodeUTF8);
		stream << xml;
		tmplXml.close();
	}
}

void sat::createImages()
{
	QString tnsmallName = dia->nameEdit->text() + "tn.png";
	QString tnlargeName = dia->nameEdit->text() + ".png";
	double pageh = sapp->doc->PageH;
	double pagew = sapp->doc->PageB;
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
	QImage tnsmall = sapp->view->PageToPixmap(0,pageSizeSmall);
	QImage tnlarge = sapp->view->PageToPixmap(0,pageSizeLarge);
	tnsmall.save(dir+"/"+tnsmallName,"PNG",70);
	tnlarge.save(dir+"/"+tnlargeName, "PNG", 70);
}

void sat::appendTmplXml()
{
	QFile tmplXml(tmplXmlFile);
	if (tmplXml.open(IO_ReadOnly))
	{
		QTextStream stream(&tmplXml);
		QString tmp = stream.readLine();
		QString file = "";
		while (tmp != NULL)
		{
			file += tmp + "\n";
			tmp = stream.readLine();
			if (tmp.find("</templates>") != -1)
				file += getTemplateTag();
		}
		tmplXml.close();
		if ( tmplXml.open( IO_WriteOnly ) )
		{
			QTextStream stream2(&tmplXml);
			stream2.setEncoding(QTextStream::UnicodeUTF8);
			stream2 << file;
			tmplXml.close();
		}
	}
}

QString sat::getTemplateTag()
{
	QString category = dia->catsCombo->currentText();
	if (category == "")
		category = QObject::tr("Own Templates");
	else
	{
		for (uint i = 0; i < dia->cats.size(); i++)
		{
			QString tmp = *dia->cats[i]->second;
			if (category == tmp)
			{
				category = *dia->cats[i]->first;
				break;
			}
		}
	}
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
	QString descr = QString(dia->descrEdit->text());
	replaceIllegalChars(descr);
	tag += "\t\t<descr>"+descr+"</descr>\n";
	QString usage = QString(dia->usageEdit->text());
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
