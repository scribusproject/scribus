#ifndef HUNSPELLDIALOG_H
#define HUNSPELLDIALOG_H

#include <QDialog>
#include <QList>
#include <QStringList>
#include <QWidget>

#include "pluginapi.h"
#include "hunspelldict.h"
#include "hunspellpluginstructs.h"
#include "scribusdoc.h"
#include "text/storytext.h"

#include "ui_hunspelldialogbase.h"

class PLUGIN_API HunspellDialog : public QDialog, private Ui::HunspellDialogBase
{
	Q_OBJECT

	public:
		HunspellDialog(QWidget* parent, ScribusDoc *doc, PageItem* item);
		HunspellDialog(QWidget* parent, ScribusDoc *doc, StoryText* itemText);
		~HunspellDialog() {};

		void set(QMap<QString, QString>* dictionaryMap, QMap<QString, HunspellDict*> *hspellerMap, QList<WordsFound>* wfList);
		bool docChanged() {return m_docChanged;}

		void updateSuggestions(const WordsFound& wordFound);
		void updateSuggestions(const QStringList& newSuggestions);

	public slots:
		void goToNextWord(int i = -1);
		void ignoreAllWords();
		void changeWord();
		void changeAllWords();
		void replaceWord(int i);
		void languageComboChanged(const QString &);
		void setLanguageCombo(const QString &newLangAbbrev);

	private:
		ScribusDoc* m_doc { nullptr };
		PageItem*  m_item { nullptr };
		StoryText* m_iText { nullptr };
		QMap<QString, QString>* m_dictionaryMap { nullptr };
		QMap<QString, HunspellDict*> *m_hspellerMap { nullptr };
		QList<WordsFound>* m_wfList { nullptr };
		WordsFound currWF;
		int  m_wfListIndex { 0 };
		bool m_docChanged { false };
		bool m_returnToDefaultLang { false };
		int m_primaryLangIndex { 0 };
};

#endif // HUNSPELLDIALOG_H
