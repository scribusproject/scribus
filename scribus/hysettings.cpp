/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "hysettings.h"
#include "hyphenator.h"
#include "scribusdoc.h"
#include "util.h"
#include "QHeaderView"
#include <QInputDialog>

HySettings::HySettings( QWidget* parent, QMap<QString,QString>* langs ) : QWidget( parent, "Settings" )
{
	langsMap = *langs;
	setupUi(this);
	QMap<QString,QString>::Iterator it;
	QStringList lada;
	for (it = langs->begin(); it != langs->end(); ++it)
		lada.append(it.data());
	lada.sort();
	language->addItems(lada);
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
/*
	layout3 = new QGridLayout( this );
	layout3->setMargin(0);
	layout3->setSpacing(5);
	layout3->setAlignment( Qt::AlignTop );
	verbose = new QCheckBox( tr("&Hyphenation Suggestions"), this, "Verbose");
	layout3->addWidget(verbose, 0, 0, 1, 2);
	
	input = new QCheckBox( tr("Hyphenate Text Automatically &During Typing"), this, "inp");
	layout3->addWidget(input, 1, 0, 1, 2);
	
		// languages
// 	langMgr.init();
// 	QStringList languageList;
// 	langMgr.fillInstalledStringList(&languageList, true);
// 	languageList.sort();
// 	guiLangCombo->insertStringList( languageList );
	
	language = new ScComboBox( false, this, "Language" );
	QMap<QString,QString>::Iterator it;
	QStringList lada;
	for (it = langs->begin(); it != langs->end(); ++it)
		lada.append(it.data());
	lada.sort();
	language->insertStringList(lada);
	text1 = new QLabel( language, tr( "&Language:" ), this, "Text1" );
	layout3->addWidget( text1, 2, 0 );
	layout3->addWidget( language, 2, 1, Qt::AlignLeft );
	
	wordLen = new QSpinBox( this, "WordLen" );
	wordLen->setMinValue( 3 );
	text2 = new QLabel( wordLen, tr( "&Smallest Word:" ), this, "Text2" );
	layout3->addWidget( text2, 3, 0 );
	layout3->addWidget( wordLen, 3, 1, Qt::AlignLeft );
	
	maxCount = new QSpinBox( this, "MaxCount" );
	maxCount->setMinValue( 0 );
	text3 = new QLabel(maxCount, tr("Consecutive Hyphenations &Allowed:"), this, "Text3");
	layout3->addWidget( text3, 4, 0 );
	layout3->addWidget( maxCount, 4, 1, Qt::AlignLeft);
	
	QToolTip::add( verbose, tr( "A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option." ) );
	QToolTip::add( input, tr("Enables automatic hyphenation of your text while typing."));
	QToolTip::add( wordLen, tr( "Length of the smallest word to be hyphenated." ) );
	QToolTip::add( maxCount, tr( "Maximum number of Hyphenations following each other.\nA value of 0 means unlimited hyphenations." ) );
*/
}

void HySettings::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	verbose->setChecked(!prefsData->Automatic);
	input->setChecked(prefsData->AutoCheck);
	language->setCurrentText(langsMap[prefsData->Language]);
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
	language->setCurrentText(langsMap[prefsData->Language]);
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
	return language->currentText();
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
