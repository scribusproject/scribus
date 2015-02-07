/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QDomDocument>
#include <QInputDialog>
#include <QListWidget>
#include <QTableWidgetItem>
#include <QTextCodec>
#include <QTextStream>

#include "downloadmanager/scdlmgr.h"
#include "prefs_hyphenator.h"
#include "langmgr.h"
#include "prefsstructs.h"
#include "scpaths.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "third_party/zip/scribus_zip.h"
#include "util.h"
#include "util_file.h"
#include "util_icon.h"

extern ScribusQApp* ScQApp;

Prefs_Hyphenator::Prefs_Hyphenator(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);

	QStringList languageList;
	LanguageManager::instance()->fillInstalledHyphStringList(&languageList);
	languageList.sort();
	hyphLanguageComboBox->addItems( languageList );

	//<<DL
	updateDictList();
	downloadLocation=ScPaths::downloadDir();
	setAvailDictsXMLFile(downloadLocation + "scribus_hyph_dicts.xml");
	downloadProgressBar->setVisible(false);
	dlLabel->setVisible(false);
	connect(hyphDownloadButton, SIGNAL(clicked()), this, SLOT(downloadHyphDicts()));
	connect(availHyphListDownloadButton, SIGNAL(clicked()), this, SLOT(updateAvailDictList()));
	//>DL

	exceptionAddButton->setIcon(QIcon(loadIcon("16/list-add.png")));
	exceptionEditButton->setEnabled(false);
	exceptionRemoveButton->setIcon(QIcon(loadIcon("16/list-remove.png")));
	exceptionRemoveButton->setEnabled(false);
	ignoreAddButton->setIcon(QIcon(loadIcon("16/list-add.png")));
	ignoreEditButton->setEnabled(false);
	ignoreRemoveButton->setIcon(QIcon(loadIcon("16/list-remove.png")));
	ignoreRemoveButton->setEnabled(false);
	connect(ignoreAddButton, SIGNAL(clicked()), this, SLOT(addToIgnoreList()));
	connect(ignoreEditButton, SIGNAL(clicked()), this, SLOT(editIgnoreListEntry()));
	connect(ignoreRemoveButton, SIGNAL(clicked()), this, SLOT(removeIgnoreListEntry()));
	connect(ignoreListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(enableIgnoreButtons()));
	connect(exceptionAddButton, SIGNAL(clicked()), this, SLOT(addToExceptList()));
	connect(exceptionEditButton, SIGNAL(clicked()), this, SLOT(editExceptListEntry()));
	connect(exceptionRemoveButton, SIGNAL(clicked()), this, SLOT(removeExceptListEntry()));
	connect(exceptionListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(enableExceptButtons()));
}

Prefs_Hyphenator::~Prefs_Hyphenator()
{
}

void Prefs_Hyphenator::languageChange()
{
}

void Prefs_Hyphenator::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	hyphSuggestionsCheckBox->setChecked(!prefsData->hyphPrefs.Automatic);
	hyphAutoCheckBox->setChecked(prefsData->hyphPrefs.AutoCheck);
	setCurrentComboItem(hyphLanguageComboBox, LanguageManager::instance()->getLangFromAbbrev(prefsData->hyphPrefs.Language, true));
	smallestWordSpinBox->setValue(prefsData->hyphPrefs.MinWordLen);
	maxConsecutiveCountSpinBox->setValue(prefsData->hyphPrefs.HyCount);
	ignoreListWidget->addItems(prefsData->hyphPrefs.ignoredWords.toList());
	ignoreListWidget->sortItems();
	exceptionListWidget->addItems(prefsData->hyphPrefs.specialWords.values());
	exceptionListWidget->sortItems();
}

void Prefs_Hyphenator::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->hyphPrefs.MinWordLen = smallestWordSpinBox->value();
	prefsData->hyphPrefs.Language = LanguageManager::instance()->getAbbrevFromLang(hyphLanguageComboBox->currentText(), true, false);
	prefsData->hyphPrefs.Automatic = !hyphSuggestionsCheckBox->isChecked();
	prefsData->hyphPrefs.AutoCheck = hyphAutoCheckBox->isChecked();
	prefsData->hyphPrefs.HyCount = maxConsecutiveCountSpinBox->value();
	QSet<QString> ignoreList;
	for (int row = 0; row < ignoreListWidget->count(); row++)
	{
		ignoreList.insert(ignoreListWidget->item(row)->text());
	}
	prefsData->hyphPrefs.ignoredWords = ignoreList;
	QHash<QString, QString> exceptionList;
	for (int row = 0; row < exceptionListWidget->count(); row++)
	{
		exceptionList.insert(exceptionListWidget->item(row)->text().remove("-"), exceptionListWidget->item(row)->text());
	}
	prefsData->hyphPrefs.specialWords = exceptionList;
}

void Prefs_Hyphenator::addToIgnoreList()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Ignore List"), tr("Add a new Entry"), QLineEdit::Normal, "", &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (ignoreListWidget->findItems(text, Qt::MatchExactly).count() == 0)
			ignoreListWidget->addItem(text);
		ignoreListWidget->sortItems();
	}
}

void Prefs_Hyphenator::editIgnoreListEntry()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Ignore List"), tr("Edit Entry"), QLineEdit::Normal, ignoreListWidget->currentItem()->text(), &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (ignoreListWidget->findItems(text, Qt::MatchExactly).count() == 0)
			ignoreListWidget->currentItem()->setText(text);
		ignoreListWidget->sortItems();
	}
}

void Prefs_Hyphenator::removeIgnoreListEntry()
{
	QListWidgetItem *item = ignoreListWidget->takeItem(ignoreListWidget->row(ignoreListWidget->currentItem()));
	delete item;
	if (ignoreListWidget->count() == 0)
	{
		ignoreEditButton->setEnabled(false);
		ignoreRemoveButton->setEnabled(false);
	}
}

void Prefs_Hyphenator::enableIgnoreButtons()
{
	ignoreEditButton->setEnabled(true);
	ignoreRemoveButton->setEnabled(true);
}

void Prefs_Hyphenator::addToExceptList()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Exception List"), tr("Add a new Entry"), QLineEdit::Normal, "", &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (exceptionListWidget->findItems(text, Qt::MatchExactly).count() == 0)
			exceptionListWidget->addItem(text);
		exceptionListWidget->sortItems();
	}
}

void Prefs_Hyphenator::editExceptListEntry()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Exception List"), tr("Edit Entry"), QLineEdit::Normal, exceptionListWidget->currentItem()->text(), &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (exceptionListWidget->findItems(text, Qt::MatchExactly).count() == 0)
			exceptionListWidget->currentItem()->setText(text);
		exceptionListWidget->sortItems();
	}
}

void Prefs_Hyphenator::removeExceptListEntry()
{
	QListWidgetItem *item = exceptionListWidget->takeItem(exceptionListWidget->row(exceptionListWidget->currentItem()));
	delete item;
	if (exceptionListWidget->count() == 0)
	{
		exceptionEditButton->setEnabled(false);
		exceptionRemoveButton->setEnabled(false);
	}
}

void Prefs_Hyphenator::enableExceptButtons()
{
	exceptionEditButton->setEnabled(true);
	exceptionRemoveButton->setEnabled(true);
}

void Prefs_Hyphenator::downloadHyphDicts()
{
	hyphDownloadButton->setEnabled(false);
	int rows=availHyphDictTableWidget->rowCount();
	QStringList dlLangs;
	for (int i=0; i<rows; ++i)
	{
		QTableWidgetItem *dlItem=availHyphDictTableWidget->item(i,3);
		if (dlItem->checkState()==Qt::Checked)
			dlLangs<<availHyphDictTableWidget->item(i,1)->text();
	}
	//qDebug()<<dlLangs;
	downloadList.clear();
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(true);
	dlLabel->setVisible(true);
	int i=0;
	QString userDictDir(ScPaths::getUserDictDir(true));
	foreach(DictData d, dictList)
	{
		if (dlLangs.contains(d.lang))
		{
			if (d.filetype=="zip")
			{
				ScQApp->dlManager()->addURL(d.url, true, downloadLocation, userDictDir);
				++i;
			}
			if (d.filetype=="plain")
			{
				//qDebug()<<d.url<<d.files;
				QStringList plainURLs(d.files.split(";", QString::SkipEmptyParts));
				foreach (QString s, plainURLs)
				{
					ScQApp->dlManager()->addURL(d.url+"/"+s, true, downloadLocation, userDictDir);
					++i;
				}
				downloadList.append(d);
			}
		}
	}
	if (i>0)
	{
		downloadProgressBar->setRange(0, i);
		connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadHyphDictsFinished()));
		connect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(updateProgressBar()));
		connect(ScQApp->dlManager(), SIGNAL(fileFailed(const QString&)), this, SLOT(updateProgressBar()));
		ScQApp->dlManager()->startDownloads();
	}
}

void Prefs_Hyphenator::updateDictList()
{
	bool dictsFound=LanguageManager::instance()->findHyphDictionaries(dictionaryPaths);
	if (!dictsFound)
		return;
	dictionaryMap.clear();
	LanguageManager::instance()->findHyphDictionarySets(dictionaryPaths, dictionaryMap);

	hyphDictTableWidget->clear();
	hyphDictTableWidget->setRowCount(dictionaryMap.count());
	hyphDictTableWidget->setColumnCount(3);
	QMapIterator<QString, QString> i(dictionaryMap);
	int row=0;
	while (i.hasNext())
	{
		 i.next();
		 int column=0;
		 //qDebug()<<i.key()<<i.value()<<LanguageManager::instance()->getLangFromAbbrev(i.key(), false);
		 QTableWidgetItem *newItem1 = new QTableWidgetItem(LanguageManager::instance()->getLangFromAbbrev(i.key()));
		 newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable);
		 hyphDictTableWidget->setItem(row, column++, newItem1);
		 QTableWidgetItem *newItem2 = new QTableWidgetItem(i.key());
		 newItem2->setFlags(newItem1->flags());
		 hyphDictTableWidget->setItem(row, column++, newItem2);
		 QTableWidgetItem *newItem3 = new QTableWidgetItem(i.value());
		 newItem3->setFlags(newItem1->flags());
		 newItem3->setToolTip(i.value());
		 hyphDictTableWidget->setItem(row, column++, newItem3);
		 ++row;
	}
	QStringList headers;
	headers << tr("Language") << tr("Code") << tr("Location");
	hyphDictTableWidget->setHorizontalHeaderLabels(headers);
	hyphDictTableWidget->resizeColumnsToContents();
}

void Prefs_Hyphenator::updateAvailDictList()
{
	availHyphListDownloadButton->setEnabled(false);
	ScQApp->dlManager()->addURL("http://services.scribus.net/scribus_hyph_dicts.xml", true, downloadLocation, downloadLocation);
	connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	ScQApp->dlManager()->startDownloads();
}

void Prefs_Hyphenator::downloadDictListFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	setAvailDictsXMLFile(downloadLocation + "scribus_hyph_dicts.xml");
	availHyphListDownloadButton->setEnabled(true);
}

void Prefs_Hyphenator::downloadHyphDictsFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	updateDictList();
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(false);
	dlLabel->setVisible(false);
	hyphDownloadButton->setEnabled(true);
}

void Prefs_Hyphenator::updateProgressBar()
{
	downloadProgressBar->setValue(downloadProgressBar->value()+1);
}

void Prefs_Hyphenator::setAvailDictsXMLFile(QString availDictsXMLDataFile)
{
	QFile dataFile(availDictsXMLDataFile);
	dataFile.open(QIODevice::ReadOnly);
	QTextStream ts(&dataFile);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( "scribus_hyph_dicts" );
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
					if (e.attribute("type")=="hyph")
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
						//else
						//	qDebug()<<"hysettings : availDicts : invalid URL"<<d.url;
					}
				}
			}
		}
		n = n.nextSibling();
	}
	availHyphDictTableWidget->clear();
	availHyphDictTableWidget->setRowCount(dictList.count());
	availHyphDictTableWidget->setColumnCount(4);
	int row=0;
	foreach(DictData d, dictList)
	{
		int column=0;
		//qDebug()<<d.version<<d.files<<d.url<<d.desc<<d.license;
		QTableWidgetItem *newItem1 = new QTableWidgetItem(d.desc);
		newItem1->setFlags(newItem1->flags() & ~Qt::ItemIsEditable);
		availHyphDictTableWidget->setItem(row, column++, newItem1);
		QTableWidgetItem *newItem2 = new QTableWidgetItem(d.lang);
		newItem2->setFlags(newItem1->flags());
		availHyphDictTableWidget->setItem(row, column++, newItem2);
		QTableWidgetItem *newItem3 = new QTableWidgetItem();
		newItem3->setCheckState(dictionaryMap.contains(d.lang) ? Qt::Checked : Qt::Unchecked);
		newItem3->setFlags(newItem1->flags() & ~Qt::ItemIsUserCheckable);
		availHyphDictTableWidget->setItem(row, column++, newItem3);
		QTableWidgetItem *newItem4 = new QTableWidgetItem();
		newItem4->setCheckState(d.download ? Qt::Checked : Qt::Unchecked);
		availHyphDictTableWidget->setItem(row, column++, newItem4);
		++row;
	}
	QStringList headers;
	headers << tr("Language") << tr("Code") << tr("Installed") << tr("Download");
	availHyphDictTableWidget->setHorizontalHeaderLabels(headers);
	availHyphDictTableWidget->resizeColumnsToContents();
}
