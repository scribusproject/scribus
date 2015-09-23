/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : Feb 2015
	copyright            : (C) 2015 by Craig Bradney
	email                : cbradney@scribus.info
***************************************************************************/

/***************************************************************************
*                                                                         *
*   mainWindow program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QComboBox>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QTextCodec>
#include <QTextStream>

#include "downloadmanager/scdlmgr.h"
#include "langmgr.h"
#include "resourcemanager.h"
#include "resourcemanagerlicense.h"
#include "scpaths.h"
#include "scribusapp.h"
#include "third_party/zip/scribus_zip.h"
#include "util.h"
#include "util_file.h"

extern ScribusQApp* ScQApp;

ResourceManager::ResourceManager(QWidget* parent)
{
	setupUi(this);
	downloadButton->setEnabled(false);
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(false);
	languageChange();

	connect(categoryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(categoryChanged()));
	connect(updateAvailableButton, SIGNAL(clicked()), this, SLOT(updateDownloadLists()));
	connect(showLicenseButton, SIGNAL(clicked()), this, SLOT(showLicense()));
	connect(downloadButton, SIGNAL(clicked()), this, SLOT(startDownload()));
}

void ResourceManager::languageChange()
{
#if defined(Q_OS_MAC)
	showInFSButton->setText( tr( "Show In Finder" ) );
#endif
#if defined(Q_OS_WIN32)
	showInFSButton->setText( tr( "Show In Explorer" ) );
#endif

	resourceCategories.clear();
	resourceCategories.insert(RM_FONTS, tr("Fonts"));
	resourceCategories.insert(RM_HYPH, tr("Hyphenation Dictionaries"));
	resourceCategories.insert(RM_SPELL, tr("Spelling Dictionaries"));
	//resourceCategories.insert(RM_TEMPLATES, tr("Templates"));
	//resourceCategories.insert(RM_PROFILES, tr("Color Profiles"));

	dataFiles.clear();
	dataFiles.insert(RM_FONTS, "scribus_fonts.xml");
	dataFiles.insert(RM_HYPH, "scribus_hyph_dicts.xml");
	dataFiles.insert(RM_SPELL, "scribus_spell_dicts.xml");
	//dataFiles.insert(RM_TEMPLATES, "scribus_templates.xml");
	//dataFiles.insert(RM_PROFILES, "scribus_profiles.xml");

	installedTableWidget->clear();
	availableTableWidget->clear();
	categoryComboBox->clear();
	categoryComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);

	QMapIterator<int, QString> i(resourceCategories);
	while (i.hasNext())
	{
		i.next();
		categoryComboBox->addItem(i.value(), i.key());
	}
	categoryChanged();
}

void ResourceManager::setFontCategory()
{
	updateInstalledFonts();
	updateAvailableFonts();
}

void ResourceManager::setHyphCategory()
{
	updateInstalledHyph();
	updateAvailableHyph();
}

void ResourceManager::setSpellCategory()
{
	updateInstalledSpell();
	updateAvailableSpell();
}

void ResourceManager::setTemplatesCategory()
{
	updateInstalledTemplates();
	updateAvailableTemplates();
}

void ResourceManager::updateInstalledFonts()
{
	installedTableWidget->clear();
	installedTableWidget->setSortingEnabled(false);
	QStringList headers;
	headers << tr("Description") << tr("Type") << tr("Installed") << tr("Download");
	installedTableWidget->setHorizontalHeaderLabels(headers);
	installedTableWidget->resizeColumnsToContents();
	installedTableWidget->setSortingEnabled(true);
	installedTableWidget->sortByColumn(0, Qt::AscendingOrder);
}

void ResourceManager::updateInstalledHyph()
{
	bool dictsFound=LanguageManager::instance()->findHyphDictionaries(dictionaryPaths);
	if (!dictsFound)
		return;
	dictionaryMap.clear();
	LanguageManager::instance()->findHyphDictionarySets(dictionaryPaths, dictionaryMap);

	installedTableWidget->clear();
	installedTableWidget->setRowCount(dictionaryMap.count());
	installedTableWidget->setColumnCount(4);
	installedTableWidget->setSortingEnabled(false);
	QMapIterator<QString, QString> i(dictionaryMap);
	int row=0;
	while (i.hasNext())
	{
		 i.next();
		 int column=0;
//		 qDebug()<<i.key()<<i.value()<<LanguageManager::instance()->getLangFromAbbrev(i.key(), false);
		 QTableWidgetItem *newItem1 = new QTableWidgetItem(LanguageManager::instance()->getLangFromAbbrev(i.key()));
		 newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		 installedTableWidget->setItem(row, column++, newItem1);
		 QTableWidgetItem *newItem2 = new QTableWidgetItem(i.key());
		 newItem2->setFlags(newItem1->flags());
		 installedTableWidget->setItem(row, column++, newItem2);
		 QTableWidgetItem *newItem3 = new QTableWidgetItem(i.value());
		 newItem3->setFlags(newItem1->flags());
		 newItem3->setToolTip(i.value());
		 installedTableWidget->setItem(row, column++, newItem3);
		 QTableWidgetItem *newItem4 = new QTableWidgetItem(i.value());
		 newItem4->setFlags(newItem1->flags());
		 newItem4->setToolTip(i.value());
		 installedTableWidget->setItem(row, column++, newItem4);
		 ++row;
	}
	QStringList headers;
	headers << tr("Language") << tr("Code") << tr("Location") << tr("Licence");
	installedTableWidget->setHorizontalHeaderLabels(headers);
	installedTableWidget->resizeColumnsToContents();
	installedTableWidget->setSortingEnabled(true);
	installedTableWidget->sortByColumn(0, Qt::AscendingOrder);
}

void ResourceManager::updateInstalledSpell()
{
	bool dictsFound=LanguageManager::instance()->findSpellingDictionaries(dictionaryPaths);
	if (!dictsFound)
		return;
	dictionaryMap.clear();
	LanguageManager::instance()->findSpellingDictionarySets(dictionaryPaths, dictionaryMap);

	installedTableWidget->clear();
	installedTableWidget->setRowCount(dictionaryMap.count());
	installedTableWidget->setColumnCount(3);
	installedTableWidget->setSortingEnabled(false);
	QMapIterator<QString, QString> i(dictionaryMap);
	int row=0;
	while (i.hasNext())
	{
		 i.next();
		 int column=0;
//		 qDebug()<<i.key()<<i.value()<<LanguageManager::instance()->getLangFromAbbrev(i.key(), false);
		 QTableWidgetItem *newItem1 = new QTableWidgetItem(LanguageManager::instance()->getLangFromAbbrev(i.key()));
		 newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		 installedTableWidget->setItem(row, column++, newItem1);
		 QTableWidgetItem *newItem2 = new QTableWidgetItem(i.key());
		 newItem2->setFlags(newItem1->flags());
		 installedTableWidget->setItem(row, column++, newItem2);
		 QTableWidgetItem *newItem3 = new QTableWidgetItem(i.value());
		 newItem3->setFlags(newItem1->flags());
		 newItem3->setToolTip(i.value());
		 installedTableWidget->setItem(row, column++, newItem3);
		 ++row;
	}
	QStringList headers;
	headers << tr("Language") << tr("Code") << tr("Location");
	installedTableWidget->setHorizontalHeaderLabels(headers);
	installedTableWidget->resizeColumnsToContents();
	installedTableWidget->setSortingEnabled(true);
	installedTableWidget->sortByColumn(0, Qt::AscendingOrder);
}

void ResourceManager::updateInstalledTemplates()
{
	installedTableWidget->clear();
}

void ResourceManager::updateAvailableFonts()
{
	QFile dataFile(ScPaths::downloadDir() + dataFiles[RM_FONTS]);
	if (!dataFile.exists())
		return;
	dataFile.open(QIODevice::ReadOnly);
	QTextStream ts(&dataFile);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( dataFiles[RM_FONTS] );
	QString data(ts.readAll());
	dataFile.close();
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol ))
	{
//		qDebug()<<errorMsg<<eline<<ecol;
		if (data.toLower().contains("404 not found"))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<dataFile.fileName();
		return;
	}
	fontList.clear();
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			if (e.tagName()=="font")
			{
				if (e.hasAttribute("type") && e.hasAttribute("filetype"))
				{
					struct DownloadItem d;
					d.desc=e.attribute("description");
					d.download=false;
					d.files=e.attribute("files");
					d.extractfiles=e.attribute("extractfiles");
					d.url=e.attribute("URL");
					d.version=e.attribute("version");
					d.lang=e.attribute("language");
					d.license=e.attribute("license");
					d.filetype=e.attribute("filetype");
					d.movetofile=e.attribute("movetofilename");
					d.type=e.attribute("type").toUpper();
					QUrl url(d.url);
					if (url.isValid() && !url.isEmpty() && !url.host().isEmpty())
						fontList.append(d);
//					else
//						qDebug()<<"rm : availFonts : invalid URL"<<d.url;
				}
			}
		}
		n = n.nextSibling();
	}
	availableTableWidget->clear();
	if(fontList.isEmpty())
	{
		downloadButton->setEnabled(false);
		return;
	}
	availableTableWidget->setRowCount(fontList.count());
	availableTableWidget->setColumnCount(4);
	availableTableWidget->setSortingEnabled(false);
	int row=0;
	foreach(DownloadItem d, fontList)
	{
		int column=0;
//		qDebug()<<d.version<<d.files<<d.url<<d.desc<<d.license;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(d.desc);
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		availableTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(d.type);
		newItem2->setFlags(newItem1->flags());
		availableTableWidget->setItem(row, column++, newItem2);
		QTableWidgetItem *newItem3 = new QTableWidgetItem();
		newItem3->setCheckState(dictionaryMap.contains(d.lang) ? Qt::Checked : Qt::Unchecked);
		newItem3->setFlags(newItem1->flags() & ~Qt::ItemIsUserCheckable);
		availableTableWidget->setItem(row, column++, newItem3);
		QTableWidgetItem *newItem4 = new QTableWidgetItem();
		newItem4->setFlags(newItem1->flags());
		newItem4->setCheckState(d.download ? Qt::Checked : Qt::Unchecked);
		availableTableWidget->setItem(row, column++, newItem4);
		++row;
	}
	QStringList headers;
	headers << tr("Description") << tr("Type") << tr("Installed") << tr("Download");
	availableTableWidget->setHorizontalHeaderLabels(headers);
	availableTableWidget->resizeColumnsToContents();
	availableTableWidget->setSortingEnabled(true);
	availableTableWidget->sortByColumn(0, Qt::AscendingOrder);
	downloadButton->setEnabled(true);
}

void ResourceManager::updateAvailableHyph()
{
	QFile dataFile(ScPaths::downloadDir() + dataFiles[RM_HYPH]);
	if (!dataFile.exists())
		return;
	dataFile.open(QIODevice::ReadOnly);
	QTextStream ts(&dataFile);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( QString(dataFiles[RM_HYPH]).remove(".xml") );
	QString data(ts.readAll());
	dataFile.close();
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol ))
	{
		if (data.toLower().contains("404 not found"))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<dataFile.fileName();
		return;
	}
	dictList.clear();
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			if (e.tagName()=="dictionary")
			{
				if (e.hasAttribute("type") && e.hasAttribute("filetype"))
				{
					if (e.attribute("type")=="hyph")
					{
						struct DownloadItem d;
						d.desc=e.attribute("description");
						d.download=false;
						d.files=e.attribute("files");
						d.extractfiles=e.attribute("extractfiles");
						d.url=e.attribute("URL");
						d.version=e.attribute("version");
						d.lang=e.attribute("language");
						d.license=e.attribute("license");
						d.filetype=e.attribute("filetype");
						d.type=e.attribute("type");
						QUrl url(d.url);
						if (url.isValid() && !url.isEmpty() && !url.host().isEmpty())
							dictList.append(d);
						//else
						//	qDebug()<<"hysettings : availDicts : invalid URL"<<d.url;
					}
				}
			}
		}
		n = n.nextSibling();
	}
	availableTableWidget->clear();
	if(dictList.isEmpty())
	{
		downloadButton->setEnabled(false);
		return;
	}
	availableTableWidget->setRowCount(dictList.count());
	availableTableWidget->setColumnCount(5);
	availableTableWidget->setSortingEnabled(false);
	int row=0;
	foreach(DownloadItem d, dictList)
	{
		int column=0;
		//qDebug()<<d.version<<d.files<<d.url<<d.desc<<d.license;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(d.desc);
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		availableTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(d.lang);
		newItem2->setFlags(newItem1->flags());
		availableTableWidget->setItem(row, column++, newItem2);
		QTableWidgetItem *newItem3 = new QTableWidgetItem();
		newItem3->setCheckState(dictionaryMap.contains(d.lang) ? Qt::Checked : Qt::Unchecked);
		newItem3->setFlags(newItem1->flags() & ~Qt::ItemIsUserCheckable);
		availableTableWidget->setItem(row, column++, newItem3);
		QTableWidgetItem *newItem4 = new QTableWidgetItem(d.license);
		availableTableWidget->setItem(row, column++, newItem4);
		newItem4->setFlags(newItem4->flags() & ~Qt::ItemIsEditable);
		QTableWidgetItem *newItem5 = new QTableWidgetItem();
		newItem5->setFlags(newItem1->flags());
		newItem5->setCheckState(d.download ? Qt::Checked : Qt::Unchecked);
		availableTableWidget->setItem(row, column++, newItem5);
		++row;
	}
	QStringList headers;
	headers << tr("Language") << tr("Code") << tr("Installed") << tr("License") << tr("Download");
	availableTableWidget->setHorizontalHeaderLabels(headers);
	availableTableWidget->resizeColumnsToContents();
	availableTableWidget->setSortingEnabled(true);
	availableTableWidget->sortByColumn(0, Qt::AscendingOrder);
	downloadButton->setEnabled(true);
}

void ResourceManager::updateAvailableSpell()
{
	QFile dataFile(ScPaths::downloadDir() + dataFiles[RM_SPELL]);
	if (!dataFile.exists())
		return;
	dataFile.open(QIODevice::ReadOnly);
	QTextStream ts(&dataFile);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( QString(dataFiles[RM_SPELL]).remove(".xml") );
	QString data(ts.readAll());
	dataFile.close();
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol ))
	{
		if (data.toLower().contains("404 not found"))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<dataFile.fileName();
		return;
	}
	dictList.clear();
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			if (e.tagName()=="dictionary")
			{
				if (e.hasAttribute("type") && e.hasAttribute("filetype"))
				{
					if (e.attribute("type")=="spell")
					{
						struct DownloadItem d;
						d.desc=e.attribute("description");
						d.download=false;
						d.files=e.attribute("files");
						d.extractfiles=e.attribute("extractfiles");
						d.url=e.attribute("URL");
						d.version=e.attribute("version");
						d.lang=e.attribute("language");
						d.license=e.attribute("license");
						d.filetype=e.attribute("filetype");
						d.type=e.attribute("type");
						QUrl url(d.url);
						if (url.isValid() && !url.isEmpty() && !url.host().isEmpty())
							dictList.append(d);
						//else
						//	qDebug()<<"hysettings : availDicts : invalid URL"<<d.url;
					}
				}
			}
		}
		n = n.nextSibling();
	}
	availableTableWidget->clear();
	if(dictList.isEmpty())
	{
		downloadButton->setEnabled(false);
		return;
	}
	availableTableWidget->setRowCount(dictList.count());
	availableTableWidget->setColumnCount(5);
	availableTableWidget->setSortingEnabled(false);
	int row=0;
	foreach(DownloadItem d, dictList)
	{
		int column=0;
		//qDebug()<<d.version<<d.files<<d.url<<d.desc<<d.license;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(d.desc);
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		availableTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(d.lang);
		newItem2->setFlags(newItem1->flags());
		availableTableWidget->setItem(row, column++, newItem2);
		QTableWidgetItem *newItem3 = new QTableWidgetItem();
		newItem3->setCheckState(dictionaryMap.contains(d.lang) ? Qt::Checked : Qt::Unchecked);
		newItem3->setFlags(newItem1->flags() & ~Qt::ItemIsUserCheckable);
		availableTableWidget->setItem(row, column++, newItem3);
		QTableWidgetItem *newItem4 = new QTableWidgetItem(d.license);
		availableTableWidget->setItem(row, column++, newItem4);
		newItem4->setFlags(newItem4->flags() & ~Qt::ItemIsEditable);
		QTableWidgetItem *newItem5 = new QTableWidgetItem();
		newItem5->setFlags(newItem1->flags());
		newItem5->setCheckState(d.download ? Qt::Checked : Qt::Unchecked);
		availableTableWidget->setItem(row, column++, newItem5);
		++row;
	}
	QStringList headers;
	headers << tr("Language") << tr("Code") << tr("Installed") << tr("License") << tr("Download");
	availableTableWidget->setHorizontalHeaderLabels(headers);
	availableTableWidget->resizeColumnsToContents();
	availableTableWidget->setSortingEnabled(true);
	availableTableWidget->sortByColumn(0, Qt::AscendingOrder);
	downloadButton->setEnabled(true);
}

void ResourceManager::updateAvailableTemplates()
{
	availableTableWidget->clear();
}

QString ResourceManager::findDestinationFolder()
{
	QString destinationFolder;
	int category = categoryComboBox->currentData().toInt();
	switch (category)
	{
		case RM_FONTS:
			destinationFolder=ScPaths::getUserFontDir(true);
			break;
		case RM_HYPH:
			destinationFolder=ScPaths::getUserDictDir(ScPaths::Hyph, true);
			break;
		case RM_SPELL:
			destinationFolder=ScPaths::getUserDictDir(ScPaths::Spell, true);
			break;
		case RM_TEMPLATES:
			//temporary
			destinationFolder=ScPaths::getTempFileDir();
			//TODO!!!! destinationFolder=ScPaths::getUserDictDir(true);
			break;
	}
	return destinationFolder;
}

void ResourceManager::categoryChanged()
{
	int category = categoryComboBox->currentData().toInt();
	switch (category)
	{
		case RM_FONTS:
			setFontCategory();
			break;
		case RM_HYPH:
			setHyphCategory();
			break;
		case RM_SPELL:
			setSpellCategory();
			break;
		case RM_TEMPLATES:
			setTemplatesCategory();
			break;
	}
}

void ResourceManager::updateDownloadLists()
{
	updateAvailableButton->setEnabled(false);
	downloadButton->setEnabled(false);
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(true);
	downloadProgressBar->setRange(0, dataFiles.count());
	foreach(QString f, dataFiles)
		ScQApp->dlManager()->addURL("http://services.scribus.net/"+f, true, ScPaths::downloadDir(), ScPaths::downloadDir());
	connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadListFinished()));
	connect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(updateProgressBar()));
	connect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(updateProgressBar()));
	ScQApp->dlManager()->startDownloads();
}

void ResourceManager::downloadListFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadListFinished()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(updateProgressBar()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(updateProgressBar()));

	int category = categoryComboBox->currentData().toInt();
	switch (category)
	{
		case RM_FONTS:
			updateAvailableFonts();
			break;
		case RM_HYPH:
			updateAvailableHyph();
			break;
		case RM_SPELL:
			updateAvailableSpell();
			break;
		case RM_TEMPLATES:
			updateAvailableTemplates();
			break;
	}
	updateAvailableButton->setEnabled(true);
	downloadButton->setEnabled(true);
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(false);
}

void ResourceManager::downloadFilesFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadFilesFinished()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(updateProgressBar()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(updateProgressBar()));
	categoryChanged();
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(false);
//	dlLabel->setVisible(false);
	downloadButton->setEnabled(true);

	int category = categoryComboBox->currentData().toInt();
	switch (category)
	{
		case RM_FONTS:
			foreach(DownloadItem d, downloadList)
			{
				//qDebug()<<d.desc<<d.download<<d.files<<d.type;
				if (d.filetype=="zip")
				{
					QString fn(ScPaths::getUserFontDir(true)+d.files);
					//qDebug()<<fn;
					QFile dledFile(fn);
					QFileInfo fi(dledFile);
					if (!dledFile.exists())
						qDebug()<<"File doesnt exist"<<fn;
					else
					{
						ScZipHandler* fun = new ScZipHandler();
						if (!fun->open(fn))
							qDebug()<<"Zip file doesnt open"<<fn;
						else
						{
							QStringList zipFileContents(fun->files());
							QStringList extractFiles(d.extractfiles.split(";", QString::SkipEmptyParts));
							QString toDir(ScPaths::getUserFontDir(false)+fi.baseName()+"/");
							QDir d(ScPaths::getUserFontDir(false));
							if (!d.exists(fi.baseName()))
								d.mkdir(fi.baseName());
							foreach (QString f2e, zipFileContents)
							{
								//qDebug()<<"Unzipping"<<f2e<<"to"<<toDir;
								fun->extract(f2e, toDir);
							}
						}
						delete fun;
					}
				}
				if (d.filetype=="plain")
				{
					//do nothing as the file is alread in place from dl mgr
				}
			}
			break;
		case RM_HYPH:
		case RM_SPELL:
			int fileType = category == RM_HYPH ? ScPaths::Hyph : ScPaths::Spell;
			foreach(DownloadItem d, downloadList)
			{
//				qDebug()<<d.desc<<d.download<<d.files<<d.type;
				if (d.filetype=="zip")
				{
					QString fn(ScPaths::getUserDictDir(static_cast<ScPaths::DictType>(fileType), true)+d.files);
//					qDebug()<<fn;
					QFile dledFile(fn);
					QFileInfo fi(dledFile);
					if (!dledFile.exists())
						qDebug()<<"File doesnt exist"<<fn;
					else
					{
						ScZipHandler* fun = new ScZipHandler();
						if (!fun->open(fn))
							qDebug()<<"Zip file doesnt open"<<fn;
						else
						{
							QStringList zipContents(fun->files());
							QStringList extractFiles(d.extractfiles.split(";", QString::SkipEmptyParts));
//							QString toDir(ScPaths::getUserDictDir(false)+fi.baseName()+"/");
							QString toDir(ScPaths::getUserDictDir(static_cast<ScPaths::DictType>(fileType), false));
							QDir d(toDir);
							if (d.exists())
							{
								foreach (QString f2e, zipContents)
								{
									if (extractFiles.contains(f2e))
									{
//										qDebug()<<"Unzipping"<<f2e<<"to"<<toDir;
										fun->extract(f2e, toDir);
									}
								}
							}
						}
						delete fun;
					}
				}
				if (d.filetype=="plain")
				{
					//do nothing as the file is alread in place from dl mgr
				}
			}
	}

}

void ResourceManager::downloadLicenseFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadLicenseFinished()));
}

void ResourceManager::downloadLicenseFileFinished(const QString &licenceFileName)
{
	disconnect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(downloadLicenseFileFinished(const QString&)));
	QFile dataFile(licenceFileName);
	QTextStream ts(&dataFile);
	QString data;
	if (dataFile.exists())
	{
		dataFile.open(QIODevice::ReadOnly);
		data = ts.readAll();
		dataFile.close();
	}
	ResourceManagerLicense* rml = new ResourceManagerLicense(parentWidget());
	rml->setText(data);
	if (rml->exec())
		rml->deleteLater();
	if (dataFile.exists())
		dataFile.remove();
}

void ResourceManager::downloadLicenseFileFailed(const QString&)
{
	disconnect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(downloadLicenseFileFailed(const QString&)));
	QMessageBox msgBox;
	msgBox.setText("The license file failed to download");
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}

void ResourceManager::updateProgressBar()
{
	downloadProgressBar->setValue(downloadProgressBar->value() + 1);
}

void ResourceManager::startDownload()
{
	int rows=availableTableWidget->rowCount();
	QStringList filesToDownload;
	for (int i=0; i<rows; ++i)
	{
		QTableWidgetItem *dlItem=availableTableWidget->item(i, 4);
		if (dlItem->checkState()==Qt::Checked)
			filesToDownload<<availableTableWidget->item(i, 0)->text();
	}
	if (filesToDownload.isEmpty())
		return;
//	qDebug()<<filesToDownload;
	downloadButton->setEnabled(false);
	downloadList.clear();
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(true);
//	dlLabel->setVisible(true);
	int dlCount=0;

	//Set up destination
	QString destinationFolder=findDestinationFolder();
	int category = categoryComboBox->currentData().toInt();
	switch (category)
	{
		case RM_FONTS:
			foreach(DownloadItem d, fontList)
			{
				if (filesToDownload.contains(d.desc))
				{
					if (d.filetype=="zip")
					{
						QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
						foreach (QString s, plainURLs)
						{
//							qDebug()<<"Requesting:"<<d.url+"/"+s;
							ScQApp->dlManager()->addURL(d.url+"/"+s, true, ScPaths::downloadDir(), destinationFolder, d.files);
							++dlCount;
						}
						downloadList.append(d);
						d.download=true;
					}
					if (d.filetype=="plain")
					{
						QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
						foreach (QString s, plainURLs)
						{
							ScQApp->dlManager()->addURL(d.url+"/"+s, true, ScPaths::downloadDir(), destinationFolder);
							++dlCount;
						}
						downloadList.append(d);
						d.download=true;
					}
				}
			}
			break;
		case RM_HYPH:
		case RM_SPELL:
			foreach(DownloadItem d, dictList)
			{
//				qDebug()<<d.desc;
				if (filesToDownload.contains(d.desc))
				{
					if (d.filetype=="zip")
					{
//						qDebug()<<"zip type:"<<d.url<<d.files;
						QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
						foreach (QString s, plainURLs)
						{
							ScQApp->dlManager()->addURL(d.url+"/"+s, true, ScPaths::downloadDir(), destinationFolder);
							++dlCount;
						}
						downloadList.append(d);
						d.download=true;
					}
					if (d.filetype=="plain")
					{
//						qDebug()<<"plain type:"<<d.url<<d.files;
						QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
						foreach (QString s, plainURLs)
						{
							ScQApp->dlManager()->addURL(d.url+"/"+s, true, ScPaths::downloadDir(), destinationFolder);
							++dlCount;
						}
						downloadList.append(d);
						d.download=true;
					}
				}
			}
			break;
		default:
			break;
	}
	if (dlCount>0)
	{
		downloadProgressBar->setRange(0, dlCount);
		connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadFilesFinished()));
		connect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(updateProgressBar()));
		connect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(updateProgressBar()));
		ScQApp->dlManager()->startDownloads();
	}
}

void ResourceManager::showLicense()
{
	bool localFile=false;
	int rows=availableTableWidget->rowCount();
	QString licenceFileName;
	QString lang;
	//Find our item we want to show the license for. If downloaded, set localFile true
	for (int i=0; i<rows; ++i)
	{
		QTableWidgetItem *dlItemI=availableTableWidget->item(i, 2);
		QTableWidgetItem *dlItemL=availableTableWidget->item(i, 3);
		localFile=dlItemI->checkState();
		if (dlItemL->isSelected())
		{
			licenceFileName = dlItemL->text();
			lang = availableTableWidget->item(i, 0)->text();
			break;
		}
	}
	if (!licenceFileName.isEmpty())
	{
		bool doDownload=true;
		QString data;
		//Set up destination and get from local file if it exists
		if (localFile)
		{
			QString destinationFolder=findDestinationFolder();
			QFile dataFile(destinationFolder + licenceFileName);
			QTextStream ts(&dataFile);
			if (dataFile.exists())
			{
				dataFile.open(QIODevice::ReadOnly);
				data = ts.readAll();
				dataFile.close();
				doDownload=false;
			}
		}
		//If we need to download the file
		if (doDownload)
		{
			QStringList filesToDownload;
			filesToDownload<<lang;
			//There's only one here... foreach?
			foreach(DownloadItem d, dictList)
			{
				if (filesToDownload.contains(d.desc))
				{
					if (d.filetype=="plain")
					{
						ScQApp->dlManager()->addURL(d.url+"/"+licenceFileName, true, ScPaths::downloadDir(), ScPaths::getTempFileDir());
						connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadLicenseFinished()));
						connect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(downloadLicenseFileFinished(const QString&)));
						connect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(downloadLicenseFileFailed(const QString&)));
						ScQApp->dlManager()->startDownloads();
					}
				}
			}
		}
		//temp fake return as data ihas not been downloaded
		if (doDownload)
			return;
	}
}

