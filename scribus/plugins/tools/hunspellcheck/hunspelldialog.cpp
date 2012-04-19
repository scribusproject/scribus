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


HunspellDialog::HunspellDialog(QWidget *parent, ScribusDoc *doc, PageItem *frameToCheck)
{
	setupUi( this );
	setModal( true );

	connect (ignoreOncePushButton, SIGNAL(clicked()), this, SLOT(goToNextWord()));
	connect (ignoreAllPushButton, SIGNAL(clicked()), this, SLOT(ignoreAllWords()));
	connect (changePushButton, SIGNAL(clicked()), this, SLOT(changeWord()));
	connect (changeAllPushButton, SIGNAL(clicked()), this, SLOT(changeAllWords()));

	m_doc=doc;
	m_docChanged=false;
	fTC=frameToCheck;
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
	StoryText *iText=&fTC->itemText;
	int sentencePos=qMax(0,iText->prevSentence(currWF.start));
	sentencePos=qMax(sentencePos, iText->nextWord(sentencePos));
	int nextSentencePos=qMin(iText->length(), iText->nextSentence(currWF.end));
	QString sentence=iText->text(sentencePos, nextSentencePos-sentencePos);
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
	m_docChanged=true;
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
	m_docChanged=true;
	goToNextWord();
}

void HunspellDialog::replaceWord(int i)
{
	StoryText *iText=&fTC->itemText;
	currWF=m_wfList->at(i);
	(*m_wfList)[i].changed=true;
	//qDebug()<<"Replacing word"<<i<<m_wfList->value(i).w<<m_wfList->value(i).changed;
	QString newText(suggestionsListWidget->currentItem()->text());
	if (newText.length()==currWF.w.length())
	{
		for (int j = 0; j < currWF.w.length(); ++j)
			iText->replaceChar(currWF.start+j+currWF.changeOffset, newText[j]);
	}
	else
	{
		int lengthDiff=newText.length()-currWF.w.length();
		if (newText.length()>currWF.w.length())
		{
			for (int j = 0; j < currWF.w.length(); ++j)
				iText->replaceChar(currWF.start+j+currWF.changeOffset, newText[j]);
			for (int j = currWF.w.length(); j < newText.length(); ++j)
				iText->insertChars(currWF.start+j+currWF.changeOffset, newText.mid(j,1), true);
		}
		else
		{
			for (int j = 0; j < newText.length(); ++j)
				iText->replaceChar(currWF.start+j+currWF.changeOffset, newText[j]);
			iText->removeChars(currWF.start+currWF.changeOffset+newText.length(), -lengthDiff);
		}
		for (int k=i; k<m_wfList->count();++k)
			(*m_wfList)[k].changeOffset+=lengthDiff;
	}
	m_docChanged=true;
}
