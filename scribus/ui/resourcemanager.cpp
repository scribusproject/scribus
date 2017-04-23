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

#include <QByteArray>
#include <QComboBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QList>
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
	dataReceivedLabel->setVisible(false);
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
	resourceCategories.insert(RM_HELP, tr("Help Manuals"));
	resourceCategories.insert(RM_PALETTES, tr("Palettes"));
	//resourceCategories.insert(RM_TEMPLATES, tr("Templates"));
	//resourceCategories.insert(RM_PROFILES, tr("Color Profiles"));
//	resourceCategories.insert(RM_TEST, tr("Test"));

	dataFiles.clear();
	dataFiles.insert(RM_FONTS, "scribus_fonts.xml");
	dataFiles.insert(RM_HYPH, "scribus_hyph_dicts.xml");
	dataFiles.insert(RM_SPELL, "scribus_spell_dicts.xml");
	dataFiles.insert(RM_HELP, "scribus_help.xml");
	dataFiles.insert(RM_PALETTES, "scribus_palettes.xml");
	//dataFiles.insert(RM_TEMPLATES, "scribus_templates.xml");
	//dataFiles.insert(RM_PROFILES, "scribus_profiles.xml");
//	dataFiles.insert(RM_TEST, "test.txt");


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

void ResourceManager::readAvailableFonts()
{
	QFile dataFile(ScPaths::downloadDir() + dataFiles[RM_FONTS]);
	if (!dataFile.exists())
		return;
	if (!dataFile.open(QIODevice::ReadOnly))
		return;
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
		if (data.contains("404 not found", Qt::CaseInsensitive))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<dataFile.fileName();
		return;
	}
	availableList.clear();
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
						availableList.append(d);
//					else
//						qDebug()<<"rm : availFonts : invalid URL"<<d.url;
				}
			}
		}
		n = n.nextSibling();
	}
}

void ResourceManager::readAvailableHelp()
{
	QFile dataFile(ScPaths::downloadDir() + dataFiles[RM_HELP]);
	if (!dataFile.exists())
		return;
	if (!dataFile.open(QIODevice::ReadOnly))
		return;
	QTextStream ts(&dataFile);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( QString(dataFiles[RM_HELP]).remove(".xml") );
	QString data(ts.readAll());
	dataFile.close();
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol ))
	{
		if (data.contains("404 not found", Qt::CaseInsensitive))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<dataFile.fileName();
		return;
	}
	availableList.clear();
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			if (e.tagName()=="help")
			{
				if (e.hasAttribute("type") && e.hasAttribute("filetype"))
				{
					if (e.attribute("type")=="scribusofficial")
					{
						struct DownloadItem d;
						d.desc=e.attribute("description");
						d.download=false;
						d.files=e.attribute("files");
						d.extractfiles="";
						d.url=e.attribute("URL");
						d.version=e.attribute("version");
						d.lang=e.attribute("language");
						d.license=e.attribute("license");
						d.filetype=e.attribute("filetype");
						d.type=e.attribute("type");
						QUrl url(d.url);
						if (url.isValid() && !url.isEmpty() && !url.host().isEmpty())
							availableList.append(d);
					}
				}
			}
		}
		n = n.nextSibling();
	}
}

void ResourceManager::readAvailablePalettes()
{
	QFile dataFile(ScPaths::downloadDir() + dataFiles[RM_PALETTES]);
	if (!dataFile.exists())
		return;
	if (!dataFile.open(QIODevice::ReadOnly))
		return;
	QTextStream ts(&dataFile);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( QString(dataFiles[RM_PALETTES]).remove(".xml") );
	QString data(ts.readAll());
	dataFile.close();
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol ))
	{
		if (data.contains("404 not found", Qt::CaseInsensitive))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<dataFile.fileName();
		return;
	}
	availableList.clear();
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			if (e.tagName()=="palette")
			{
				if (e.hasAttribute("type") && e.hasAttribute("filetype"))
				{
					//if (e.attribute("type")=="scribusofficial")
					{
						struct DownloadItem d;
						d.desc=e.attribute("description");
						d.download=false;
						d.files=e.attribute("files");
						d.extractfiles="";
						d.url=e.attribute("URL");
						d.version=e.attribute("version");
						d.lang=e.attribute("language");
						d.license=e.attribute("license");
						d.filetype=e.attribute("filetype");
						d.type=e.attribute("type");
						d.source=e.attribute("source");
						QUrl url(d.url);
						if (url.isValid() && !url.isEmpty() && !url.host().isEmpty())
							availableList.append(d);
					}
				}
			}
		}
		n = n.nextSibling();
	}
}

void ResourceManager::updateInstalledFonts()
{
	dictionaryMap.clear();
	QString fontDir(findDestinationFolder());
	foreach(DownloadItem d, availableList)
	{
		if (d.filetype!="zip")
		{
			if (QFileInfo::exists(fontDir+d.files))
				dictionaryMap.insert(d.desc, fontDir+d.files);
		}
		else
		{
			QFileInfo zfi(d.files);
			if (QFileInfo::exists(fontDir+zfi.baseName()))
				dictionaryMap.insert(d.desc, fontDir+zfi.baseName());
		}
	}

	installedTableWidget->clear();
	installedTableWidget->setRowCount(dictionaryMap.count());
	installedTableWidget->setColumnCount(2);
	installedTableWidget->setSortingEnabled(false);

	QMapIterator<QString, QString> i(dictionaryMap);
	int row=0;
	while (i.hasNext())
	{
		i.next();
		int column=0;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(i.key());
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		installedTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(i.value());
		newItem2->setFlags(newItem1->flags());
		installedTableWidget->setItem(row, column++, newItem2);
		++row;
	}
	QStringList headers;
	headers << tr("Description") << tr("Location") ;
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
	dictionaryMap.clear();
	installedTableWidget->clear();
}

void ResourceManager::updateInstalledHelp()
{
	dictionaryMap.clear();
	QString helpDir(findDestinationFolder());
	foreach(DownloadItem d, availableList)
	{
		if (QFileInfo::exists(helpDir+d.lang))
			dictionaryMap.insert(d.desc, helpDir+d.lang);
	}
	installedTableWidget->clear();
	installedTableWidget->setRowCount(dictionaryMap.count());
	installedTableWidget->setColumnCount(2);
	installedTableWidget->setSortingEnabled(false);

	QMapIterator<QString, QString> i(dictionaryMap);
	int row=0;
	while (i.hasNext())
	{
		i.next();
		int column=0;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(i.key());
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		installedTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(i.value());
		newItem2->setFlags(newItem1->flags());
		installedTableWidget->setItem(row, column++, newItem2);
		++row;
	}

	QStringList headers;
	headers << tr("Description") << tr("Location") ;
	installedTableWidget->setHorizontalHeaderLabels(headers);

	installedTableWidget->resizeColumnsToContents();
	installedTableWidget->setSortingEnabled(true);
	installedTableWidget->sortByColumn(0, Qt::AscendingOrder);
}

void ResourceManager::updateInstalledPalettes()
{
	dictionaryMap.clear();
	QString palDir(findDestinationFolder());
	foreach(DownloadItem d, availableList)
	{
		if (QFileInfo::exists(palDir+d.files))
			dictionaryMap.insert(d.desc, palDir+d.files);
	}

	installedTableWidget->clear();
	installedTableWidget->setRowCount(dictionaryMap.count());
	installedTableWidget->setColumnCount(2);
	installedTableWidget->setSortingEnabled(false);

	QMapIterator<QString, QString> i(dictionaryMap);
	int row=0;
	while (i.hasNext())
	{
		i.next();
		int column=0;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(i.key());
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		installedTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(i.value());
		newItem2->setFlags(newItem1->flags());
		installedTableWidget->setItem(row, column++, newItem2);
		++row;
	}
	QStringList headers;
	headers << tr("Description") << tr("Location") ;
	installedTableWidget->setHorizontalHeaderLabels(headers);
	installedTableWidget->resizeColumnsToContents();
	installedTableWidget->setSortingEnabled(true);
	installedTableWidget->sortByColumn(0, Qt::AscendingOrder);
}

void ResourceManager::updateInstalledTest()
{
	dictionaryMap.clear();
	installedTableWidget->clear();
}

void ResourceManager::updateAvailableFonts()
{

	availableTableWidget->clear();
	if(availableList.isEmpty())
	{
		downloadButton->setEnabled(false);
		return;
	}
	availableTableWidget->setRowCount(availableList.count());
	availableTableWidget->setColumnCount(4);
	availableTableWidget->setSortingEnabled(false);
	int row=0;
	foreach(DownloadItem d, availableList)
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
		newItem3->setCheckState(dictionaryMap.contains(d.desc) ? Qt::Checked : Qt::Unchecked);
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
	if (!dataFile.open(QIODevice::ReadOnly))
		return;
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
		if (data.contains("404 not found", Qt::CaseInsensitive))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<dataFile.fileName();
		return;
	}
	availableList.clear();
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
							availableList.append(d);
						//else
						//	qDebug()<<"hysettings : availDicts : invalid URL"<<d.url;
					}
				}
			}
		}
		n = n.nextSibling();
	}
	availableTableWidget->clear();
	if(availableList.isEmpty())
	{
		downloadButton->setEnabled(false);
		return;
	}
	availableTableWidget->setRowCount(availableList.count());
	availableTableWidget->setColumnCount(5);
	availableTableWidget->setSortingEnabled(false);
	int row=0;
	foreach(DownloadItem d, availableList)
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
	if (!dataFile.open(QIODevice::ReadOnly))
		return;
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
		if (data.contains("404 not found", Qt::CaseInsensitive))
			qDebug()<<"File not found on server";
		else
			qDebug()<<"Could not open file"<<dataFile.fileName();
		return;
	}
	availableList.clear();
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
							availableList.append(d);
						//else
						//	qDebug()<<"hysettings : availDicts : invalid URL"<<d.url;
					}
				}
			}
		}
		n = n.nextSibling();
	}
	availableTableWidget->clear();
	if(availableList.isEmpty())
	{
		downloadButton->setEnabled(false);
		return;
	}
	availableTableWidget->setRowCount(availableList.count());
	availableTableWidget->setColumnCount(5);
	availableTableWidget->setSortingEnabled(false);
	int row=0;
	foreach(DownloadItem d, availableList)
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

void ResourceManager::updateAvailableHelp()
{
	availableTableWidget->clear();
	if(availableList.isEmpty())
	{
		downloadButton->setEnabled(false);
		return;
	}
	availableTableWidget->setRowCount(availableList.count());
	availableTableWidget->setColumnCount(5);
	availableTableWidget->setSortingEnabled(false);
	int row=0;
	foreach(DownloadItem d, availableList)
	{
		int column=0;
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
	headers << tr("Description") << tr("Language") << tr("Installed") << tr("License") << tr("Download");
	availableTableWidget->setHorizontalHeaderLabels(headers);
	availableTableWidget->resizeColumnsToContents();
	availableTableWidget->setSortingEnabled(true);
	availableTableWidget->sortByColumn(0, Qt::AscendingOrder);
	downloadButton->setEnabled(true);
}

void ResourceManager::updateAvailablePalettes()
{
	availableTableWidget->clear();
	if(availableList.isEmpty())
	{
		downloadButton->setEnabled(false);
		return;
	}
	availableTableWidget->setRowCount(availableList.count());
	availableTableWidget->setColumnCount(5);
	availableTableWidget->setSortingEnabled(false);
	int row=0;
	foreach(DownloadItem d, availableList)
	{
		int column=0;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(d.desc);
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
		availableTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(d.source);
		newItem2->setFlags(newItem1->flags());
		availableTableWidget->setItem(row, column++, newItem2);
		QTableWidgetItem *newItem3 = new QTableWidgetItem();
		newItem3->setCheckState(dictionaryMap.contains(d.desc) ? Qt::Checked : Qt::Unchecked);
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
	headers << tr("Description") << tr("Source") << tr("Installed") << tr("License") << tr("Download");
	availableTableWidget->setHorizontalHeaderLabels(headers);
	availableTableWidget->resizeColumnsToContents();
	availableTableWidget->setSortingEnabled(true);
	availableTableWidget->sortByColumn(0, Qt::AscendingOrder);
	downloadButton->setEnabled(true);
}

void ResourceManager::updateAvailableTest()
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
			destinationFolder=ScPaths::userFontDir(true);
			break;
		case RM_HYPH:
			destinationFolder=ScPaths::userDictDir(ScPaths::Hyph, true);
			break;
		case RM_SPELL:
			destinationFolder=ScPaths::userDictDir(ScPaths::Spell, true);
			break;
		case RM_TEMPLATES:
			//temporary
			destinationFolder=ScPaths::tempFileDir();
			//TODO!!!! destinationFolder=ScPaths::getUserDictDir(true);
			break;
		case RM_HELP:
			destinationFolder=ScPaths::userHelpFilesDir(true);
			break;
		case RM_PALETTES:
			destinationFolder=ScPaths::userPaletteFilesDir(true);
			break;
		case RM_TEST:
			destinationFolder=ScPaths::downloadDir();
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
			readAvailableFonts();
			updateInstalledFonts();
			updateAvailableFonts();
			break;
		case RM_HYPH:
			updateInstalledHyph();
			updateAvailableHyph();
			break;
		case RM_SPELL:
			updateInstalledSpell();
			updateAvailableSpell();
			break;
		case RM_TEMPLATES:
			updateInstalledTemplates();
			updateAvailableTemplates();
			break;
		case RM_HELP:
			readAvailableHelp();
			updateInstalledHelp();
			updateAvailableHelp();
			break;
		case RM_PALETTES:
			readAvailablePalettes();
			updateInstalledPalettes();
			updateAvailablePalettes();
			break;
		case RM_TEST:
			updateInstalledTest();
			updateAvailableTest();
			break;
	}
}

void ResourceManager::updateDownloadLists()
{
	updateAvailableButton->setEnabled(false);
	downloadButton->setEnabled(false);
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(true);
	dataReceivedLabel->setVisible(true);
	downloadProgressBar->setRange(0, dataFiles.count());
	foreach(QString f, dataFiles)
		ScQApp->dlManager()->addURL("http://services.scribus.net/"+f, true, ScPaths::downloadDir(), ScPaths::downloadDir());
	foreach(QString f, dataFiles)
		ScQApp->dlManager()->addURL("http://services.scribus.net/"+f+".sha256", true, ScPaths::downloadDir(), ScPaths::downloadDir());
	connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadListFinished()));
	connect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(updateProgressBar()));
	connect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(updateProgressBar()));
	connect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(updateProgressBar()));
	connect(ScQApp->dlManager(), SIGNAL(fileDownloadProgress(qint64, qint64)), this, SLOT(updateProgressData(qint64, qint64)));
	ScQApp->dlManager()->startDownloads();
}

void ResourceManager::downloadListFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadListFinished()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(updateProgressBar()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(updateProgressBar()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileDownloadProgress(qint64, qint64)), this, SLOT(updateProgressData(qint64, qint64)));

	int category = categoryComboBox->currentData().toInt();
	bool fileOk=checkFileHash(ScPaths::downloadDir(), dataFiles[category], dataFiles[category] + ".sha256", QCryptographicHash::Sha256);
	switch (category)
	{
		case RM_FONTS:
			if (fileOk)
				updateAvailableFonts();
			break;
		case RM_HYPH:
			if (fileOk)
				updateAvailableHyph();
			break;
		case RM_SPELL:
			if (fileOk)
				updateAvailableSpell();
			break;
		case RM_TEMPLATES:
			if (fileOk)
				updateAvailableTemplates();
			break;
		case RM_HELP:
			if (fileOk)
				updateAvailableHelp();
			else
				qDebug()<<"Help Failure :(";
			break;
		case RM_PALETTES:
			if (fileOk)
				updateAvailablePalettes();
			else
				qDebug()<<"Palette Failure :(";
			break;
		case RM_TEST:
			if (fileOk)
				qDebug()<<"Success!!!";
			else
				qDebug()<<"Failure :(";
			break;
	}
	updateAvailableButton->setEnabled(true);
	downloadButton->setEnabled(true);
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(false);
	dataReceivedLabel->setVisible(false);
}

void ResourceManager::downloadFilesFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadFilesFinished()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(updateProgressBar()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(updateProgressBar()));
	disconnect(ScQApp->dlManager(), SIGNAL(fileDownloadProgress(qint64, qint64)), this, SLOT(updateProgressData(qint64, qint64)));

	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(false);
	dataReceivedLabel->setVisible(false);
	downloadButton->setEnabled(true);

	int category = categoryComboBox->currentData().toInt();
	switch (category)
	{
		case RM_FONTS:
			foreach(DownloadItem d, downloadList)
			{
				if (d.filetype=="zip")
				{
					QString fn(ScPaths::userFontDir(true)+d.files);
					QFile dledFile(fn);
					QFileInfo fi(dledFile);
					if (!dledFile.exists())
						qDebug()<<"File doesn\'t exist"<<fn;
					else
					{
						ScZipHandler* fun = new ScZipHandler();
						if (!fun->open(fn))
							qDebug()<<"Zip file doesn\'t open"<<fn;
						else
						{
							QStringList zipFileContents(fun->files());
							QStringList extractFiles(d.extractfiles.split(";", QString::SkipEmptyParts));
							QString toDir(ScPaths::userFontDir(false)+fi.baseName()+"/");
							QDir dir(ScPaths::userFontDir(false));
							if (!dir.exists(fi.baseName()))
								dir.mkdir(fi.baseName());
							foreach (const QString& f2e, zipFileContents)
							{
								fun->extract(f2e, toDir, ScZipHandler::SkipPaths);
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
			{
				int fileType = category == RM_HYPH ? ScPaths::Hyph : ScPaths::Spell;
				foreach(DownloadItem d, downloadList)
				{
					if (d.filetype=="zip")
					{
						QString fn(ScPaths::userDictDir(static_cast<ScPaths::DictType>(fileType), true)+d.files);
						if (!QFileInfo::exists(fn))
							qDebug()<<"File doesn\'t exist"<<fn;
						else
						{
							ScZipHandler* fun = new ScZipHandler();
							if (!fun->open(fn))
								qDebug()<<"Zip file doesn\'t open"<<fn;
							else
							{
								QStringList zipContents(fun->files());
								QStringList extractFiles(d.extractfiles.split(";", QString::SkipEmptyParts));
								QString toDir(ScPaths::userDictDir(static_cast<ScPaths::DictType>(fileType), false));
								QDir dir(toDir);
								if (dir.exists())
								{
									foreach (const QString& f2e, zipContents)
									{
										if (extractFiles.contains(f2e))
											fun->extract(f2e, toDir, ScZipHandler::SkipPaths);
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
			break;
		case RM_HELP:
			foreach(DownloadItem d, downloadList)
			{
				//qDebug()<<d.desc<<d.download<<d.files<<d.type;
				if (d.filetype=="zip")
				{
					QString fn(ScPaths::userHelpFilesDir(true)+d.files);
					//qDebug()<<fn;
					QFile dledFile(fn);
					QFileInfo fi(dledFile);
					QFile dledFileSHA256(fn+".sha256");
					QFileInfo fiSHA256(dledFileSHA256);
					if (!dledFile.exists() || !dledFileSHA256.exists())
						qDebug()<<"File doesn\'t exist"<<fn<<fn+".sha256";
					else
					{
						if (checkFileHash(ScPaths::userHelpFilesDir(false), fi.fileName(), fiSHA256.fileName(), QCryptographicHash::Sha256))
						{
							ScZipHandler* fun = new ScZipHandler();
							if (!fun->open(fn))
								qDebug()<<"Zip file doesn\'t open"<<fn;
							else
							{
								QStringList zipFileContents(fun->files());
								QStringList extractFiles(d.extractfiles.split(";", QString::SkipEmptyParts));
								QString toDir(ScPaths::userHelpFilesDir(false)+d.lang+"/");
								QDir dir(ScPaths::userHelpFilesDir(false));
								if (!dir.exists(d.lang))
									dir.mkdir(d.lang);
								foreach (const QString& f2e, zipFileContents)
								{
									//qDebug()<<"Unzipping"<<f2e<<"to"<<toDir;
									fun->extract(f2e, toDir, ScZipHandler::ExtractPaths);
								}
							}
							delete fun;
						}
						else
							qDebug()<<"checksum failed for"<<fn + ".sha256";
						dledFile.remove();
						dledFileSHA256.remove();
					}
				}
				if (d.filetype=="plain")
				{
					//do nothing as the file is alread in place from dl mgr
				}
			}
			break;
	}
	categoryChanged();
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
		if (!dataFile.open(QIODevice::ReadOnly))
			return;
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

void ResourceManager::updateProgressData(qint64 bytesReceived, qint64 bytesTotal)
{
	QString totalText;
	if (bytesTotal == -1)
		totalText = QString("%1 kB").arg(bytesReceived/1000);
	else
//	{
		totalText = QString("%1 / %2 kB").arg(bytesReceived/1000).arg(bytesTotal/1000);
//		QString totalTextWidth = QString("%1 / %2 kB").arg(bytesTotal).arg(bytesTotal);
//		dataReceivedLabel->setMinimumWidth(QWidget::fontMetrics().width(totalTextWidth));
//	}
	dataReceivedLabel->setText(totalText);
}

void ResourceManager::startDownload()
{
	int rows = availableTableWidget->rowCount();
	int columns = availableTableWidget->columnCount();
	QStringList filesToDownload;
	for (int i=0; i < rows; ++i)
	{
		QTableWidgetItem *dlItem = availableTableWidget->item(i, columns - 1);
		if (dlItem->checkState() == Qt::Checked)
			filesToDownload<<availableTableWidget->item(i, 0)->text();
	}
	if (filesToDownload.isEmpty())
		return;
//	qDebug()<<filesToDownload;
	downloadButton->setEnabled(false);
	downloadList.clear();
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(true);
	dataReceivedLabel->setVisible(true);
//	dlLabel->setVisible(true);
	int dlCount=0;

	//Set up destination
	QString destinationFolder(findDestinationFolder());
	int category = categoryComboBox->currentData().toInt();
	switch (category)
	{
		case RM_FONTS:
			foreach(DownloadItem d, availableList)
			{
				if (filesToDownload.contains(d.desc))
				{
					if (d.filetype=="zip")
					{
						QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
						foreach (const QString& s, plainURLs)
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
						foreach (const QString& s, plainURLs)
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
			foreach(DownloadItem d, availableList)
			{
//				qDebug()<<d.desc;
				if (filesToDownload.contains(d.desc))
				{
					if (d.filetype=="zip")
					{
//						qDebug()<<"zip type:"<<d.url<<d.files;
						QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
						foreach (const QString& s, plainURLs)
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
						foreach (const QString& s, plainURLs)
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
		case RM_HELP:
			foreach(DownloadItem d, availableList)
			{
//				qDebug()<<d.desc;
				if (filesToDownload.contains(d.desc))
				{
					if (d.filetype=="zip")
					{
//						qDebug()<<"zip type:"<<d.url<<d.files;
						QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
						foreach (const QString& s, plainURLs)
						{
							ScQApp->dlManager()->addURL(d.url+"/"+s, true, ScPaths::downloadDir(), destinationFolder);
							ScQApp->dlManager()->addURL(d.url+"/"+s+".sha256", true, ScPaths::downloadDir(), destinationFolder);
							dlCount+=2;
						}
						downloadList.append(d);
						d.download=true;
					}
				}
			}
			break;
		case RM_PALETTES:
			foreach(DownloadItem d, availableList)
			{
//				qDebug()<<d.desc;
				if (filesToDownload.contains(d.desc))
				{
					if (d.filetype=="zip")
					{
//						qDebug()<<"zip type:"<<d.url<<d.files;
						QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
						foreach (const QString& s, plainURLs)
						{
							ScQApp->dlManager()->addURL(d.url+"/"+s, true, ScPaths::downloadDir(), destinationFolder);
							ScQApp->dlManager()->addURL(d.url+"/"+s+".sha256", true, ScPaths::downloadDir(), destinationFolder);
							dlCount+=2;
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
		connect(ScQApp->dlManager(), SIGNAL(fileDownloadProgress(qint64, qint64)), this, SLOT(updateProgressData(qint64, qint64)));
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
			if (dataFile.exists() && dataFile.open(QIODevice::ReadOnly))
			{
				QTextStream ts(&dataFile);
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
/*			int category = categoryComboBox->currentData().toInt();
			switch (category)
			{
				case RM_HYPH:
				case RM_SPELL:
				case RM_PALETTES:
*/
			foreach(const DownloadItem& d, availableList)
			{
				if (filesToDownload.contains(d.desc))
				{
					ScQApp->dlManager()->addURL(d.url+"/"+licenceFileName, true, ScPaths::downloadDir(), ScPaths::tempFileDir());
					connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadLicenseFinished()));
					connect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(downloadLicenseFileFinished(const QString&)));
					connect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(downloadLicenseFileFailed(const QString&)));
					ScQApp->dlManager()->startDownloads();
				}
/*
 * 					}
					break;
*/
			}
		}
	}
}

