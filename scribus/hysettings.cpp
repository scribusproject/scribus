/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fileunzip.h"
#include "hysettings.h"
#include "hyphenator.h"
#include "langmgr.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "util_icon.h"
#include "util.h"
#include "util_file.h"
#include "scpaths.h"
#include <QDomDocument>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QProgressBar>
#include <QTableWidgetItem>

extern ScribusQApp* ScQApp;

HySettings::HySettings( QWidget* parent) : QWidget( parent )
{

	setupUi(this);

	downloadLocation=ScPaths::downloadDir();
	LanguageManager *lmg(LanguageManager::instance());
	language->setInsertPolicy(QComboBox::InsertAlphabetically);
	foreach(QString hlang, lmg->hyphLangs())
	{
		language->addItem( lmg->getLangFromAbbrev(hlang), lmg->getLangFromAbbrev(hlang,false) );
	}
	updateDictList();
	setAvailDictsXMLFile(downloadLocation + "scribus_spell_dicts.xml");
	downloadProgressBar->setVisible(false);
	dlLabel->setVisible(false);
	buttonExceptAdd->setIcon(QIcon(loadIcon("16/list-add.png")));
	buttonExceptEdit->setEnabled(false);
	buttonExceptRemove->setIcon(QIcon(loadIcon("16/list-remove.png")));
	buttonExceptRemove->setEnabled(false);
	buttonIgnoreAdd->setIcon(QIcon(loadIcon("16/list-add.png")));
	buttonIgnoreEdit->setEnabled(false);
	buttonIgnoreRemove->setIcon(QIcon(loadIcon("16/list-remove.png")));
	buttonIgnoreRemove->setEnabled(false);
	connect(buttonIgnoreAdd, SIGNAL(clicked()), this, SLOT(addToIgnoreList()));
	connect(buttonIgnoreEdit, SIGNAL(clicked()), this, SLOT(editIgnoreListEntry()));
	connect(buttonIgnoreRemove, SIGNAL(clicked()), this, SLOT(removeIgnoreListEntry()));
	connect(ignoreList, SIGNAL(itemSelectionChanged()), this, SLOT(enableIgnoreButtons()));
	connect(buttonExceptAdd, SIGNAL(clicked()), this, SLOT(addToExceptList()));
	connect(buttonExceptEdit, SIGNAL(clicked()), this, SLOT(editExceptListEntry()));
	connect(buttonExceptRemove, SIGNAL(clicked()), this, SLOT(removeExceptListEntry()));
	connect(exceptList, SIGNAL(itemSelectionChanged()), this, SLOT(enableExceptButtons()));
	connect(spellDownloadButton, SIGNAL(clicked()), this, SLOT(downloadSpellDicts()));
	connect(availListDownloadButton, SIGNAL(clicked()), this, SLOT(updateAvailDictList()));
}

void HySettings::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	verbose->setChecked(!prefsData->Automatic);
	input->setChecked(prefsData->AutoCheck);
	setCurrentComboItem(language, LanguageManager::instance()->getTransLangFromLang(prefsData->Language));
	wordLen->setValue(prefsData->MinWordLen);
	maxCount->setValue(prefsData->HyCount);
	ignoreList->addItems(prefsData->ignoredWords.toList());
	ignoreList->sortItems();
	exceptList->addItems(prefsData->specialWords.values());
	exceptList->sortItems();
}

void HySettings::restoreDefaults(ScribusDoc *prefsData)
{
	verbose->setChecked(!prefsData->Automatic);
	input->setChecked(prefsData->AutoCheck);
	setCurrentComboItem(language, LanguageManager::instance()->getTransLangFromLang(prefsData->Language));
	wordLen->setValue(prefsData->MinWordLen);
	maxCount->setValue(prefsData->HyCount);
	ignoreList->addItems(prefsData->docHyphenator->ignoredWords.toList());
	ignoreList->sortItems();
	exceptList->addItems(prefsData->docHyphenator->specialWords.values());
	exceptList->sortItems();
}

void HySettings::addToIgnoreList()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Ignore List"), tr("Add a new Entry"), QLineEdit::Normal, "", &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (ignoreList->findItems(text, Qt::MatchExactly).count() == 0)
			ignoreList->addItem(text);
		ignoreList->sortItems();
	}
}

void HySettings::editIgnoreListEntry()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Ignore List"), tr("Edit Entry"), QLineEdit::Normal, ignoreList->currentItem()->text(), &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (ignoreList->findItems(text, Qt::MatchExactly).count() == 0)
			ignoreList->currentItem()->setText(text);
		ignoreList->sortItems();
	}
}

void HySettings::removeIgnoreListEntry()
{
	QListWidgetItem *item = ignoreList->takeItem(ignoreList->row(ignoreList->currentItem()));
	delete item;
	if (ignoreList->count() == 0)
	{
		buttonIgnoreEdit->setEnabled(false);
		buttonIgnoreRemove->setEnabled(false);
	}
}

void HySettings::enableIgnoreButtons()
{
	buttonIgnoreEdit->setEnabled(true);
	buttonIgnoreRemove->setEnabled(true);
}

void HySettings::addToExceptList()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Exception List"), tr("Add a new Entry"), QLineEdit::Normal, "", &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (exceptList->findItems(text, Qt::MatchExactly).count() == 0)
			exceptList->addItem(text);
		exceptList->sortItems();
	}
}

void HySettings::editExceptListEntry()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Exception List"), tr("Edit Entry"), QLineEdit::Normal, exceptList->currentItem()->text(), &ok);
	if ((ok) && (!text.isEmpty()))
	{
		if (exceptList->findItems(text, Qt::MatchExactly).count() == 0)
			exceptList->currentItem()->setText(text);
		exceptList->sortItems();
	}
}

void HySettings::removeExceptListEntry()
{
	QListWidgetItem *item = exceptList->takeItem(exceptList->row(exceptList->currentItem()));
	delete item;
	if (exceptList->count() == 0)
	{
		buttonExceptEdit->setEnabled(false);
		buttonExceptRemove->setEnabled(false);
	}
}

void HySettings::enableExceptButtons()
{
	buttonExceptEdit->setEnabled(true);
	buttonExceptRemove->setEnabled(true);
}

void HySettings::downloadSpellDicts()
{
	spellDownloadButton->setEnabled(false);
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

	downloadList.clear();
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(true);
	dlLabel->setVisible(true);
	int i=0;
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
				{
					ScQApp->dlManager()->addURL(d.url+"/"+s, true, downloadLocation);
					++i;
				}
				downloadList.append(d);
			}
		}
	}
	if (i>0)
	{
		qDebug()<<"Files to download:"<<i;
		downloadProgressBar->setRange(0, i);
		connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadSpellDictsFinished()));
		connect(ScQApp->dlManager(), SIGNAL(fileReceived(const QString&)), this, SLOT(updateProgressBar()));
		ScQApp->dlManager()->startDownloads();
	}
}

void HySettings::updateDictList()
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
		 //qDebug()<<i.key()<<i.value();
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

void HySettings::updateAvailDictList()
{
	availListDownloadButton->setEnabled(false);
	ScQApp->dlManager()->addURL("http://services.scribus.net/scribus_spell_dicts.xml", true, downloadLocation);
	connect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	ScQApp->dlManager()->startDownloads();
}

void HySettings::downloadDictListFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	setAvailDictsXMLFile(downloadLocation + "scribus_spell_dicts.xml");
	availListDownloadButton->setEnabled(true);
}

void HySettings::downloadSpellDictsFinished()
{
	disconnect(ScQApp->dlManager(), SIGNAL(finished()), this, SLOT(downloadDictListFinished()));
	//qDebug()<<"Downloads All Finished";
	QString userDictDir(ScPaths::getUserDictDir(true));
	// List all downloaded files in order to handle identical
	// affix files while reducing potential errors related to
	// disk space
	QStringList allFileList;
	foreach(DictData d, downloadList)
	{
		allFileList += d.files.split(";", QString::SkipEmptyParts);
	}
	// Move downloaded files to destination
	foreach(DictData d, downloadList)
	{
		QString basename = QFileInfo(d.url).fileName();
		QString filename = downloadLocation+basename;
		QStringList files = d.files.split(";", QString::SkipEmptyParts);
		QString affixFile = affixFileName(files);
		QString dictFile  = dictFileName(files);
		//qDebug()<<filename;
		if (d.filetype=="zip")
		{
			//qDebug()<<"zip data found"<<filename;
			FileUnzip fun(filename);
			foreach (QString s, files)
			{
				//qDebug()<<"Unzipping"<<userDictDir+s;
				QString data = fun.getFile(s, userDictDir);
				allFileList.removeOne(s);
			}
		}
		if (d.filetype=="plain")
		{
			foreach (QString s, files)
			{
				//qDebug()<<"plain data found"<<downloadLocation<<userDictDir<<s;
				QString dstName = s;
				if (dstName == affixFile)
					dstName = QFileInfo(downloadLocation+dictFile).baseName() + ".aff";
				allFileList.removeOne(s);
				if (allFileList.contains(s))
				{
					copyFile(downloadLocation+s, userDictDir+dstName);
					continue;
				}
				moveFile(downloadLocation+s, userDictDir+dstName);
			}
		}
	}

	updateDictList();
	downloadProgressBar->setValue(0);
	downloadProgressBar->setVisible(false);
	dlLabel->setVisible(false);
	spellDownloadButton->setEnabled(true);
}

void HySettings::updateProgressBar()
{
	downloadProgressBar->setValue(downloadProgressBar->value()+1);
}

void HySettings::setAvailDictsXMLFile(QString availDictsXMLDataFile)
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

QString HySettings::affixFileName(QStringList files)
{
	for (int i = 0; i < files.count(); ++i)
	{
		const QString& file = files.at(i);
		if (file.endsWith(".aff", Qt::CaseInsensitive))
			return file;
	}
	return QString();
}

QString HySettings::dictFileName(QStringList files)
{
	for (int i = 0; i < files.count(); ++i)
	{
		const QString& file = files.at(i);
		if (file.endsWith(".dic", Qt::CaseInsensitive))
			return file;
	}
	return QString();
}

int HySettings::getWordLen()
{
	return wordLen->value();
}

int HySettings::getMaxCount()
{
	return maxCount->value();
}

bool HySettings::getVerbose()
{
	return verbose->isChecked();
}

bool HySettings::getInput()
{
	return input->isChecked();
}

QString HySettings::getLanguage()
{
	return language->itemData(language->currentIndex()).toString();
}

QSet<QString> HySettings::getIgnoreList()
{
	QSet<QString> ret;
	for (int row = 0; row < ignoreList->count(); row++)
	{
		ret.insert(ignoreList->item(row)->text());
	}
	return ret;
}

QHash<QString, QString> HySettings::getExceptionList()
{
	QHash<QString, QString> ret;
	for (int row = 0; row < exceptList->count(); row++)
	{
		ret.insert(exceptList->item(row)->text().remove("-"), exceptList->item(row)->text());
	}
	return ret;
}

void HySettings::setSpellingInvisible()
{
	tabWidget->removeTab(1);
}
