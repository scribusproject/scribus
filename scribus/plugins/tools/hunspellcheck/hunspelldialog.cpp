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
#include "undomanager.h"
#include "undotransaction.h"

HunspellDialog::HunspellDialog(QWidget *parent, ScribusDoc *doc, PageItem *item)
              : QDialog(parent),
                m_doc(doc),
                m_item(item),
                m_iText(&item->itemText)
{
	setupUi( this );
	setModal( true );

	connect (ignoreOncePushButton, SIGNAL(clicked()), this, SLOT(goToNextWord()));
	connect (ignoreAllPushButton, SIGNAL(clicked()), this, SLOT(ignoreAllWords()));
	connect (changePushButton, SIGNAL(clicked()), this, SLOT(changeWord()));
	connect (changeAllPushButton, SIGNAL(clicked()), this, SLOT(changeAllWords()));
	connect (languagesComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(languageComboChanged(const QString &)));
}

HunspellDialog::HunspellDialog(QWidget *parent, ScribusDoc *doc, StoryText * itemText)
              : QDialog(parent),
                m_doc(doc),
                m_iText(itemText)
{
	setupUi( this );
	setModal( true );

	connect (ignoreOncePushButton, SIGNAL(clicked()), this, SLOT(goToNextWord()));
	connect (ignoreAllPushButton, SIGNAL(clicked()), this, SLOT(ignoreAllWords()));
	connect (changePushButton, SIGNAL(clicked()), this, SLOT(changeWord()));
	connect (changeAllPushButton, SIGNAL(clicked()), this, SLOT(changeAllWords()));
	connect (languagesComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(languageComboChanged(const QString &)));
}

void HunspellDialog::set(QMap<QString, QString>* dictionaryMap, QMap<QString, HunspellDict*> *hspellerMap, QList<WordsFound> *wfList)
{
	m_dictionaryMap = dictionaryMap;
	m_hspellerMap = hspellerMap;
	m_wfList = wfList;
	bool b = languagesComboBox->blockSignals(true);
	languagesComboBox->clear();

	QStringList langsToAdd;
	auto it = m_dictionaryMap->cbegin();
	while (it != m_dictionaryMap->cend())
	{
		QString lang = LanguageManager::instance()->getLangFromAbbrev(it.key(), true);
		if (!langsToAdd.contains(lang))
			langsToAdd << (!lang.isEmpty() ? lang : it.key());
		++it;
	}
	languagesComboBox->addItems(langsToAdd);
	languagesComboBox->setCurrentIndex(0);
	m_primaryLangIndex = 0;
	languagesComboBox->blockSignals(b);
	m_wfListIndex = 0;
	goToNextWord(0);
}

void HunspellDialog::updateSuggestions(const WordsFound& wordFound)
{
	QStringList suggestions;
	if (m_hspellerMap->contains(wordFound.lang))
		suggestions = (*m_hspellerMap)[wordFound.lang]->suggest(wordFound.w);
	updateSuggestions(suggestions);
}

void HunspellDialog::updateSuggestions(const QStringList &newSuggestions)
{
	suggestionsListWidget->clear();
	suggestionsListWidget->addItems(newSuggestions);
	suggestionsListWidget->setCurrentRow(0);
}

void HunspellDialog::goToNextWord(int i)
{
	if (m_returnToDefaultLang)
	{
		bool b = languagesComboBox->blockSignals(true);
		languagesComboBox->setCurrentIndex(m_primaryLangIndex);
		languagesComboBox->blockSignals(b);
	}
	if (i >= 0)
		m_wfListIndex = i;
	else
	{
		do {
			++m_wfListIndex;
		} while (m_wfListIndex < m_wfList->count() && (m_wfList->at(m_wfListIndex).changed || m_wfList->at(m_wfListIndex).ignore));
		//qDebug()<<"selected word index"<<m_wfListIndex;
	}
	if (m_wfListIndex >= m_wfList->count())
	{
		statusLabel->setText(tr("Spelling check complete"));
		suggestionsListWidget->clear();
		sentenceTextEdit->clear();
		changePushButton->setEnabled(false);
		changeAllPushButton->setEnabled(false);
		ignoreOncePushButton->setEnabled(false);
		ignoreAllPushButton->setEnabled(false);
		return;
	}
	statusLabel->setText("");
	currWF = m_wfList->at(m_wfListIndex);
	setLanguageCombo(currWF.lang);
	updateSuggestions(currWF);

	int sentencePos = 0;
	QString sentence(m_iText->sentence(currWF.start + currWF.changeOffset, sentencePos));
	sentence.insert(currWF.end - sentencePos + currWF.changeOffset, "</b></font>");
	sentence.insert(currWF.start - sentencePos + currWF.changeOffset, "<font color=red><b>");
	sentenceTextEdit->setText(sentence);
}

void HunspellDialog::ignoreAllWords()
{
	if (m_wfListIndex < 0 || m_wfListIndex >= m_wfList->count())
		return;
	QString wordToIgnore = m_wfList->at(m_wfListIndex).w;
	//Do we start from 0 or from the instance of the word where we are... 0 for now
	for (int i = 0; i < m_wfList->count(); ++i)
	{
		if (m_wfList->at(i).w == wordToIgnore)
			(*m_wfList)[i].ignore = true;
	}
	goToNextWord();
}

void HunspellDialog::changeWord()
{
	//If we have ignored a word or its already changed, skip to the next.
	if (m_wfList->at(m_wfListIndex).ignore || m_wfList->at(m_wfListIndex).changed)
		goToNextWord();
	replaceWord(m_wfListIndex);
	goToNextWord();
}

void HunspellDialog::changeAllWords()
{
	if (m_wfList->at(m_wfListIndex).ignore && !m_wfList->at(m_wfListIndex).changed)
		return;

	UndoTransaction transaction;
	if ((m_item != nullptr) && UndoManager::undoEnabled())
		transaction = UndoManager::instance()->beginTransaction(m_item->getUName(), m_item->getUPixmap());

	QString wordToChange = m_wfList->at(m_wfListIndex).w;
	//Do we start from 0 or from the instance of the word where we are... 0 for now
	for (int i = 0; i < m_wfList->count(); ++i)
		if (m_wfList->at(i).w == wordToChange)
			replaceWord(i);

	if (transaction)
		transaction.commit();

	goToNextWord();
}

void HunspellDialog::replaceWord(int i)
{
	//TODO: rehyphenate after the replacement
	QListWidgetItem* suggestionItem = suggestionsListWidget->currentItem();
	if (!suggestionItem)
		return;
	int replaceStart = m_wfList->at(i).start + m_wfList->at(i).changeOffset;
	QString oldText(m_iText->word(replaceStart));
	QString newText(suggestionItem->text());

	UndoTransaction transaction;
	if ((m_item != nullptr) && UndoManager::undoEnabled())
	{
		UndoObject* undoTarget = m_item->isNoteFrame() ? dynamic_cast<UndoObject*>(m_item->doc()) : dynamic_cast<UndoObject*>(m_item);
		transaction = UndoManager::instance()->beginTransaction(m_item->getUName(), m_item->getUPixmap());
		if (oldText.length() > 0)
		{
			auto is = new ScItemState<CharStyle>(Um::DeleteText, "", Um::IDelete);
			is->set("DELETE_FRAMETEXT");
			is->set("ETEA",  QString("delete_frametext"));
			is->set("TEXT_STR", oldText);
			is->set("START", replaceStart);
			is->setItem(m_item->itemText.charStyle(replaceStart));
			if (m_item->isNoteFrame())
				is->set("noteframeName", m_item->getUName());
			UndoManager::instance()->action(undoTarget, is);
		}
		if (newText.length() > 0)
		{
			auto ss = new SimpleState(Um::InsertText, "", Um::ICreate);
			ss->set("INSERT_FRAMETEXT");
			ss->set("ETEA", QString("insert_frametext"));
			ss->set("TEXT_STR", newText);
			ss->set("START", replaceStart);
			UndoObject * undoTarget = m_item;
			if (m_item->isNoteFrame())
				ss->set("noteframeName", m_item->getUName());
			UndoManager::instance()->action(undoTarget, ss);
		}
	}

	int lengthDiff = m_iText->replaceWord(m_wfList->at(i).start + m_wfList->at(i).changeOffset, newText);
	if (lengthDiff != 0)
	{
		for (int k = i; k < m_wfList->count(); ++k)
			(*m_wfList)[k].changeOffset += lengthDiff;
	}

	if (transaction)
		transaction.commit();

	(*m_wfList)[i].changed = true;
	m_docChanged = true;
}

void HunspellDialog::languageComboChanged(const QString &newLanguage)
{
	m_returnToDefaultLang = true;
	QString wordLang = LanguageManager::instance()->getAbbrevFromLang(newLanguage, false);
	if (!m_hspellerMap->contains(wordLang))
	{
		//qDebug() << "hspeller"<<wordLang<<"does not exist";
		return;
	}
	//qDebug() << wordLang << newLanguage;
	if (m_wfList->count() == 0)
		return;
	if (m_wfListIndex >= m_wfList->count())
		m_wfListIndex = 0;
	QString word = m_wfList->at(m_wfListIndex).w;
	if ((*m_hspellerMap)[wordLang]->spell(word) == 0)
	{
		QStringList replacements = (*m_hspellerMap)[wordLang]->suggest(word);
		updateSuggestions(replacements);
	}
	else
	{
		(*m_wfList)[m_wfListIndex].changed = true;
		m_docChanged = true;
		goToNextWord();
	}
}

void HunspellDialog::setLanguageCombo(const QString &newLangAbbrev)
{
	int i = 0;
	if (m_dictionaryMap->contains(newLangAbbrev))
		i = languagesComboBox->findText(LanguageManager::instance()->getLangFromAbbrev(newLangAbbrev, true));
	bool b = languagesComboBox->blockSignals(true);
	languagesComboBox->setCurrentIndex(i);
	languagesComboBox->blockSignals(b);
}
