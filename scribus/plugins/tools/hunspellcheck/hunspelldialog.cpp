/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QComboBox>
#include <QListWidget>
#include <QTextEdit>
#include "hunspelldialog.h"
#include "langmgr.h"


HunspellDialog::HunspellDialog(QWidget *parent, ScribusDoc *doc, StoryText *iText)
{
	setupUi( this );
	setModal( true );

	connect (ignoreOncePushButton, SIGNAL(clicked()), this, SLOT(goToNextWord()));
	connect (ignoreAllPushButton, SIGNAL(clicked()), this, SLOT(ignoreAllWords()));
	connect (changePushButton, SIGNAL(clicked()), this, SLOT(changeWord()));
	connect (changeAllPushButton, SIGNAL(clicked()), this, SLOT(changeAllWords()));
	connect (languagesComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(languageComboChanged(const QString &)));

	m_doc=doc;
	m_docChanged=false;
	m_iText=iText;
	m_returnToDefaultLang=false;
	m_primaryLangIndex=0;
}

void HunspellDialog::set(QMap<QString, QString>* dictionaryMap, QMap<QString, HunspellDict*> *hspellerMap, QList<WordsFound> *wfList)
{
	m_dictionaryMap=dictionaryMap;
	m_hspellerMap=hspellerMap;
	m_wfList=wfList;
	bool b=languagesComboBox->blockSignals(true);
	languagesComboBox->clear();
	QMap<QString, QString>::iterator it = m_dictionaryMap->begin();
	while (it != dictionaryMap->end())
	{
		QString lang=LanguageManager::instance()->getLangFromAbbrev(it.key(), true);
		languagesComboBox->addItem(!lang.isEmpty() ? lang : it.key());
		++it;
	}
	languagesComboBox->setCurrentIndex(0);
	m_primaryLangIndex=0;
	languagesComboBox->blockSignals(b);
	wfListIndex=0;
	goToNextWord(0);
}

void HunspellDialog::updateSuggestions(QStringList &newSuggestions)
{
	suggestionsListWidget->clear();
	suggestionsListWidget->addItems(newSuggestions);
	suggestionsListWidget->setCurrentRow(0);
}

void HunspellDialog::goToNextWord(int i)
{
	if (m_returnToDefaultLang)
	{
		bool b=languagesComboBox->blockSignals(true);
		languagesComboBox->setCurrentIndex(m_primaryLangIndex);
		languagesComboBox->blockSignals(b);
	}
	if (i>=0)
		wfListIndex=i;
	else
	{
		do {
			++wfListIndex;
		} while (wfListIndex<m_wfList->count() && (m_wfList->at(wfListIndex).changed || m_wfList->at(wfListIndex).ignore));
		//qDebug()<<"selected word index"<<wfListIndex;
	}
	if (wfListIndex>=m_wfList->count())
	{
		statusLabel->setText(tr("Spelling check complete"));
		suggestionsListWidget->clear();
		sentenceTextEdit->clear();
		changePushButton->setEnabled(false);
		changeAllPushButton->setEnabled(false);
		return;
	}
	else
		statusLabel->setText("");
	currWF=m_wfList->at(wfListIndex);
	setLanguageCombo(currWF.lang);
	updateSuggestions(currWF.replacements);

	int sentencePos=0;
	QString sentence(m_iText->sentence(currWF.start, sentencePos));
	sentence.insert(currWF.end-sentencePos+currWF.changeOffset,"</b></font>");
	sentence.insert(currWF.start-sentencePos+currWF.changeOffset,"<font color=red><b>");
	sentenceTextEdit->setText(sentence);

}

void HunspellDialog::ignoreAllWords()
{
	QString wordToIgnore=m_wfList->at(wfListIndex).w;
	//Do we start from 0 or from the instance of the word where we are... 0 for now
	for(int i=0;i<m_wfList->count();++i)
		if(m_wfList->at(i).w==wordToIgnore)
			(*m_wfList)[i].ignore=true;
	goToNextWord();
}

void HunspellDialog::changeWord()
{
	//If we have ignored a word or its already changed, skip to the next.
	if(m_wfList->at(wfListIndex).ignore || m_wfList->at(wfListIndex).changed)
		goToNextWord();
	replaceWord(wfListIndex);
	goToNextWord();
}

void HunspellDialog::changeAllWords()
{
	if(m_wfList->at(wfListIndex).ignore && !m_wfList->at(wfListIndex).changed)
		return;
	QString wordToChange=m_wfList->at(wfListIndex).w;
	//Do we start from 0 or from the instance of the word where we are... 0 for now
	for(int i=0;i<m_wfList->count();++i)
		if(m_wfList->at(i).w==wordToChange)
			replaceWord(i);
	goToNextWord();
}

void HunspellDialog::replaceWord(int i)
{
	//TODO: rehypenate after the replacement
	QString newText(suggestionsListWidget->currentItem()->text());
	int lengthDiff=m_iText->replaceWord(m_wfList->at(i).start+m_wfList->at(i).changeOffset, newText);
	if (lengthDiff!=0)
	{
		for (int k=i; k<m_wfList->count();++k)
			(*m_wfList)[k].changeOffset+=lengthDiff;
	}
	(*m_wfList)[i].changed=true;
	m_docChanged=true;
}

void HunspellDialog::languageComboChanged(const QString &newLanguage)
{
	m_returnToDefaultLang=true;
	QString wordLang = LanguageManager::instance()->getAbbrevFromLang(newLanguage, true, false, 1);
	if (!m_hspellerMap->contains(wordLang) )
	{
		//qDebug()<<"hspeller"<<wordLang<<"does not exist";
		wordLang=LanguageManager::instance()->getAbbrevFromLang(newLanguage, true, false, 2);
		if (!m_hspellerMap->contains(wordLang) )
		//{
			//qDebug()<<"hspeller"<<wordLang<<"ALTERNATIVE does not exist";
			return;
		//}
		//else
			//qDebug()<<"hspeller"<<wordLang<<"ALTERNATIVE exists";
	}

	if (m_wfList->count() == 0)
		return;
	if (wfListIndex >= m_wfList->count())
		wfListIndex = 0;

	QString word = m_wfList->at(wfListIndex).w;
	if ((*m_hspellerMap)[wordLang]->spell(word) == 0)
	{
		QStringList replacements = (*m_hspellerMap)[wordLang]->suggest(word);
		updateSuggestions(replacements);
	}
	else
	{
		(*m_wfList)[wfListIndex].changed = true;
		m_docChanged = true;
		goToNextWord();
	}
}

void HunspellDialog::setLanguageCombo(const QString &newLangAbbrev)
{
	QMap<QString, QString>::iterator it = m_dictionaryMap->begin();
	int i = 0;
	while (it != m_dictionaryMap->end())
	{
		if (it.key() == newLangAbbrev)
			break;
		++i;
		++it;
	}
	bool b = languagesComboBox->blockSignals(true);
	languagesComboBox->setCurrentIndex(i);
	languagesComboBox->blockSignals(b);
}
