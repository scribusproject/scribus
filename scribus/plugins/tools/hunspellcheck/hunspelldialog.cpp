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


HunspellDialog::HunspellDialog(QWidget *parent, ScribusDoc *doc, StoryText *iText)
{
	setupUi( this );
	setModal( true );

	connect (ignoreOncePushButton, SIGNAL(clicked()), this, SLOT(goToNextWord()));
	connect (ignoreAllPushButton, SIGNAL(clicked()), this, SLOT(ignoreAllWords()));
	connect (changePushButton, SIGNAL(clicked()), this, SLOT(changeWord()));
	connect (changeAllPushButton, SIGNAL(clicked()), this, SLOT(changeAllWords()));

	m_doc=doc;
	m_docChanged=false;
	m_Itext=iText;
}

void HunspellDialog::set(QStringList *dictEntries, Hunspell **hspellers, QList<WordsFound> *wfList)
{
	m_dictEntries=dictEntries;
	m_hspellers=hspellers;
	m_wfList=wfList;

	languagesComboBox->addItems(*dictEntries);

	wfListIndex=0;
	goToNextWord(0);
}

void HunspellDialog::goToNextWord(int i)
{
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
	suggestionsListWidget->clear();
	suggestionsListWidget->addItems(currWF.replacements);
	suggestionsListWidget->setCurrentRow(0);

	int sentencePos=0;
	QString sentence(m_Itext->sentence(currWF.start, sentencePos));
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
	//qDebug()<<"Replacing word"<<i<m_wfList->at(i).w<<m_wfList->at(i).start;
	QString newText(suggestionsListWidget->currentItem()->text());
	int lengthDiff=m_Itext->replaceWord(m_wfList->at(i).start+m_wfList->at(i).changeOffset, newText);
	if (lengthDiff!=0)
	{
		for (int k=i; k<m_wfList->count();++k)
			(*m_wfList)[k].changeOffset+=lengthDiff;
	}
	(*m_wfList)[i].changed=true;
	m_docChanged=true;
}
