/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#include "satemplate.h"
#include "satemplate.moc"
#include "satdialog.h"
#include <qdir.h>
#include <iostream>
ScribusApp* Carrier;
QWidget* par;

QString Name()
{
	return QObject::tr("Save as Template...");
}

int Type()
{
	return 5;
}

void InitPlug(QWidget *d, ScribusApp *plug)
{
	Carrier = plug;
	par = d;
	satm = new MenuSAT(d);
	int ind = 0;
	for (uint a = 0; a < plug->fileMenu->count(); ++a)
	{
		if ((plug->fileMenu->text(plug->fileMenu->idAt(a)) == QObject::tr("Save as...")) ||
			(plug->fileMenu->text(plug->fileMenu->idAt(a)) == "Save as..."))
			break;
		ind++;
	}
	int id = plug->fileMenu->insertItem(QObject::tr("Save as Template..."), -1, ind+1);
	plug->fileMenu->connectItem(id, satm, SLOT(RunSATPlug()));
	plug->fileMenu->setItemEnabled(id, 0);
	plug->MenuItemsFile.append(id);
}

void CleanUpPlug()
{}

void Run(QWidget *d, ScribusApp *plug)
{}

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
	QDir::setCurrent(QDir::homeDirPath() + "/.scribus/templates");
	Carrier->Collect();
	QString docPath = Carrier->doc->DocName;
	QString docDir = docPath.left(docPath.findRev('/'));
	QString docName = docPath.right(docPath.length() - docPath.findRev('/') - 1);
	docName = docName.left(docName.findRev(".s"));
	if (currentFile !=  Carrier->doc->DocName)
	{ // TODO Check if the collect was canceled.
		satdialog* satdia = new satdialog(par,docName);
		if (satdia->exec())
		{
			sat* s = new sat(Carrier, satdia, docPath.right(docPath.length() - docPath.findRev('/') - 1),docDir);
			s->createImages();
			s->createTmplXml();
			delete s;
			// Restore the state that was before plug->Collect()
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
			Carrier->RemoveRecent(docPath);
		}
		delete satdia;
	}
}

// --------------------- CLASS sat ------------------------------------------------//

sat::sat(ScribusApp* scribusApp, satdialog* satdia, QString fileName, QString tmplDir)
{
	sapp = scribusApp;
	dia = satdia;
	dir = tmplDir;
	if (dir.right(1) == "/")
		dir = tmplDir.left(tmplDir.length() - 1);
	file = fileName;
	tmplXmlFile = QString(dir);
	if (tmplXmlFile.right(1) != "/")
		tmplXmlFile += "/";
	tmplXmlFile += "template.xml";
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
	QPixmap tnsmall = sapp->view->PageToPixmap(0,pageSizeSmall);
	QPixmap tnlarge = sapp->view->PageToPixmap(0,pageSizeLarge);
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
			{
				file += getTemplateTag();
			}
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
	std::cout << "Category: |" << category << "|" << "\n";
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
	QString tag = "\t<template category=\""+category+"\">\n";
	tag += "\t\t<name>"+dia->nameEdit->text()+"</name>\n";
	tag += "\t\t<file>"+file+"</file>\n";
	tag += "\t\t<tnail>"+dia->nameEdit->text()+"tn.png"+"</tnail>\n";
	tag += "\t\t<img>"+dia->nameEdit->text()+".png"+"</img>\n";
	tag += "\t\t<psize>"+dia->psizeEdit->text()+"</psize>\n";
	tag += "\t\t<color>"+dia->colorsEdit->text()+"</color>\n";
	tag += "\t\t<descr>"+dia->descrEdit->text()+"</descr>\n";
	tag += "\t\t<usage>"+dia->usageEdit->text()+"</usage>\n";
	tag += "\t\t<author>"+dia->authorEdit->text()+"</author>\n";
	tag += "\t\t<email>"+dia->emailEdit->text()+"</email>\n";
	tag += "\t</template>\n";

	return tag;
}

sat::~sat()
{
}
