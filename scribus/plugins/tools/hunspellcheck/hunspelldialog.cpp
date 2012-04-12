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
	changeOffset=0;
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
		++wfListIndex;
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
	sentence.insert(currWF.end-sentencePos+changeOffset,"</b></font>");
	sentence.insert(currWF.start-sentencePos+changeOffset,"<font color=red><b>");
	sentenceTextEdit->setText(sentence);

}

void HunspellDialog::ignoreAllWords()
{
	QString wordToIgnore=m_wfList->at(wfListIndex).w;
	//Do we start from 0 or from the instance of the word where we are... 0 for now
	for(int i=0;i<m_wfList->count();++i)
		if(m_wfList->at(i).w==wordToIgnore)
			m_wfList->value(i).ignore=true;
	goToNextWord();
}

void HunspellDialog::changeWord()
{
	//If we have ignored a word, skip to the next.
	if(m_wfList->at(wfListIndex).ignore && !m_wfList->at(wfListIndex).changed)
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
		{
			m_wfList->value(i).changed=true;
		}
	m_docChanged=true;
	goToNextWord();
}

void HunspellDialog::replaceWord(int i)
{
	StoryText *iText=&fTC->itemText;
	currWF=m_wfList->at(i);
	m_wfList->value(i).changed=true;
	QString newText(suggestionsListWidget->currentItem()->text());
	if (newText.length()==currWF.w.length())
	{
		for (int i = 0; i < currWF.w.length(); ++i)
			iText->replaceChar(currWF.start+i+changeOffset, newText[i]);
	}
	else
	{
		if (newText.length()>currWF.w.length())
		{
			for (int i = 0; i < currWF.w.length(); ++i)
				iText->replaceChar(currWF.start+i+changeOffset, newText[i]);
			for (int i = currWF.w.length(); i < newText.length(); ++i)
				iText->insertChars(currWF.start+i+changeOffset, newText.mid(i,1), true);
			int lengthDiff=newText.length()-currWF.w.length();
			changeOffset+=lengthDiff;
			qDebug()<<"Change Offset is:"<<changeOffset;
		}
		else
		{
			for (int i = 0; i < newText.length(); ++i)
				iText->replaceChar(currWF.start+i+changeOffset, newText[i]);
			int lengthDiff=currWF.w.length() - newText.length();
			iText->removeChars(currWF.start+changeOffset+newText.length(), lengthDiff);
			changeOffset-=lengthDiff;
			qDebug()<<"Change Offset is:"<<changeOffset;
		}
	}
	m_docChanged=true;
}
