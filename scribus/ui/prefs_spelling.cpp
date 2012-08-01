/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QDebug>
#include <QDomDocument>
#include <QHeaderView>
#include <QInputDialog>
#include <QInputDialog>
#include <QListWidget>
#include <QTableWidgetItem>
#include <QTextCodec>
#include <QTextStream>


#include "prefs_spelling.h"
#include "fileunzip.h"
#include "langmgr.h"
#include "prefsstructs.h"
#include "scribuscore.h" //FIXME: for the ScCore call (remove this call)
#include "scribusdoc.h"
#include "scribus.h" //FIXME: for the ScCore call (remove this call)
#include "util_icon.h"
#include "util.h"
#include "util_file.h"

#include "scribusapp.h"
#include "scpaths.h"


extern ScribusQApp* ScQApp;

Prefs_Spelling::Prefs_Spelling(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);

	updateDictList();
	downloadLocation=ScPaths::downloadDir();
	setAvailDictsXMLFile(downloadLocation + "scribus_spell_dicts.xml");

	connect(spellDownloadButton, SIGNAL(clicked()), this, SLOT(downloadSpellDicts()));
	connect(availListDownloadButton, SIGNAL(clicked()), this, SLOT(updateAvailDictList()));
}

Prefs_Spelling::~Prefs_Spelling()
{
}

void Prefs_Spelling::languageChange()
{
}

void Prefs_Spelling::restoreDefaults(struct ApplicationPrefs *prefsData)
{
}

void Prefs_Spelling::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
}

void Prefs_Spelling::downloadSpellDicts()
{
	qDebug()<<"Now attempting downloads";
	int rows=availDictTableWidget->rowCount();
	QStringList dlLangs;
	for (int i=0; i<rows; ++i)
	{
		QTableWidgetItem *dlItem=availDictTableWidget->item(i,3);
		if (dlItem->checkState()==Qt::Checked)
			dlLangs<<availDictTableWidget->item(i,1)->text();
	}
	qDebug()<<dlLangs;
	int i=0;
	downloadList.clear();
	foreach(DictData d, dictList)
	{
		if (dlLangs.contains(d.lang))
		{
			if (d.filetype=="zip")
			{
				ScQApp->dlManager()->addURL(d.url, true, downloadLocation);
				downloadList.append(d);
				++i;
			}
			if (d.filetype=="plain")
			{
				//qDebug()<<d.url<<d.files;
				QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
				foreach (QString s, plainURLs)
					ScQApp->dlManager()->addURL(d.url+"/"+s, true, downloadLocation);
				downloadList.append(d);
				++i;
			}
		}
	}
	if (i>0)
	{
		connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadSpellDictsFinished()));
		ScQApp->dlManager()->startDownloads();
	}
}

void Prefs_Spelling::updateDictList()
{
	bool dictsFound=LanguageManager::instance()->findDictionaries(dictionaryPaths);
	if (!dictsFound)
		return;
	dictionaryMap.clear();
	LanguageManager::instance()->findDictionarySets(dictionaryPaths, dictionaryMap);

	dictTableWidget->clear();
	dictTableWidget->setRowCount(dictionaryMap.count());
	dictTableWidget->setColumnCount(3);
	QMapIterator<QString, QString> i(dictionaryMap);
	int row=0;
	while (i.hasNext())
	{
		 i.next();
		 int column=0;
		 qDebug()<<i.key()<<i.value();
		 QTableWidgetItem *newItem1 = new QTableWidgetItem(LanguageManager::instance()->getLangFromAbbrev(i.key()));
		 newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable);
		 dictTableWidget->setItem(row, column++, newItem1);
		 QTableWidgetItem *newItem2 = new QTableWidgetItem(i.key());
		 newItem2->setFlags(newItem1->flags());
		 dictTableWidget->setItem(row, column++, newItem2);
		 QTableWidgetItem *newItem3 = new QTableWidgetItem(i.value());
		 newItem3->setFlags(newItem1->flags());
		 newItem3->setToolTip(i.value());
		 dictTableWidget->setItem(row, column++, newItem3);
		 ++row;
	}
	QStringList headers;
	headers << tr("Language") << tr("Code") << tr("Location");
	dictTableWidget->setHorizontalHeaderLabels(headers);
	dictTableWidget->resizeColumnsToContents();
}

void Prefs_Spelling::updateAvailDictList()
{
	ScQApp->dlManager()->addURL("http://services.scribus.net/scribus_spell_dicts.xml", true, downloadLocation);
	connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	ScQApp->dlManager()->startDownloads();
}

void Prefs_Spelling::downloadDictListFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	setAvailDictsXMLFile(downloadLocation + "scribus_spell_dicts.xml");
}

void Prefs_Spelling::downloadSpellDictsFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	//qDebug()<<"Downloads All Finished";
	QString userDictDir(ScPaths::getUserDictDir(true));
	foreach(DictData d, downloadList)
	{
		QString basename = QFileInfo(d.url).fileName();
		QString filename=downloadLocation+basename;
		//qDebug()<<filename;
		if (d.filetype=="zip")
		{
			//qDebug()<<"zip data found"<<filename;
			FileUnzip* fun = new FileUnzip(filename);
			QStringList files=d.files.split(";", QString::SkipEmptyParts);
			foreach (QString s, files)
			{
				//qDebug()<<"Unzipping"<<userDictDir+s;
				QString data=fun->getFileToPath(s, userDictDir);
			}
			delete fun;
		}
		if (d.filetype=="plain")
		{
			QStringList files=d.files.split(";", QString::SkipEmptyParts);
			foreach (QString s, files)
			{
				//qDebug()<<"plain data found"<<downloadLocation<<userDictDir<<s;
				moveFile(downloadLocation+s, userDictDir+s);
			}
		}
	}

	updateDictList();
}

void Prefs_Spelling::setAvailDictsXMLFile(QString availDictsXMLDataFile)
{
	QFile dataFile(availDictsXMLDataFile);
	dataFile.open(QIODevice::ReadOnly);
	QTextStream ts(&dataFile);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( "scribus_spell_dicts" );
	QString data(ts.readAll());
	dataFile.close();
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol ))
	{
		if (data.toLower().contains("404 not found"))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<availDictsXMLDataFile;
		return;
	}
	dictList.clear();
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while( !n.isNull() ) {
		QDomElement e = n.toElement();
		if( !e.isNull() ) {
			if (e.tagName()=="dictionary")
			{
				if (e.hasAttribute("type") && e.hasAttribute("filetype"))
				{
					if (e.attribute("type")=="spell")
					{
						struct DictData d;
						d.desc=e.attribute("description");
						d.download=false;
						d.files=e.attribute("files");
						d.url=e.attribute("URL");
						d.version=e.attribute("version");
						d.lang=e.attribute("language");
						d.license=e.attribute("license");
						d.filetype=e.attribute("filetype");
						QUrl url(d.url);
						if (url.isValid() && !url.isEmpty() && !url.host().isEmpty())
							dictList.append(d);
						else
							qDebug()<<"hysettings : availDicts : invalid URL"<<d.url;
					}
				}
			}
		}
		n = n.nextSibling();
	}
	availDictTableWidget->clear();
	availDictTableWidget->setRowCount(dictList.count());
	availDictTableWidget->setColumnCount(4);
	int row=0;
	foreach(DictData d, dictList)
	{
		int column=0;
		//qDebug()<<d.version<<d.files<<d.url<<d.desc<<d.license;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(d.desc);
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable);
		availDictTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(d.lang);
		newItem2->setFlags(newItem1->flags());
		availDictTableWidget->setItem(row, column++, newItem2);
		QTableWidgetItem *newItem3 = new QTableWidgetItem();
		newItem3->setCheckState(dictionaryMap.contains(d.lang) ? Qt::Checked : Qt::Unchecked);
		newItem3->setFlags(newItem1->flags() & ~Qt::ItemIsUserCheckable);
		availDictTableWidget->setItem(row, column++, newItem3);
		QTableWidgetItem *newItem4 = new QTableWidgetItem();
		newItem4->setCheckState(d.download ? Qt::Checked : Qt::Unchecked);
		availDictTableWidget->setItem(row, column++, newItem4);
		++row;
	}
	QStringList headers;
	headers << tr("Language") << tr("Code") << tr("Installed") << tr("Download");
	availDictTableWidget->setHorizontalHeaderLabels(headers);
	availDictTableWidget->resizeColumnsToContents();
}
