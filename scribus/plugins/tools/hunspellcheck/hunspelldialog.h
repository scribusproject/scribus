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
		HunspellDialog(QWidget* parent, ScribusDoc *doc, StoryText* iText);
		~HunspellDialog() {};
		void set(QMap<QString, QString>* dictionaryMap, QMap<QString, HunspellDict*> *hspellerMap, QList<WordsFound>* wfList);
		bool docChanged() {return m_docChanged;}
		void updateSuggestions(QStringList& newSuggestions);

	public slots:
		void goToNextWord(int i=-1);
		void ignoreAllWords();
		void changeWord();
		void changeAllWords();
		void replaceWord(int i);
		void languageComboChanged(const QString &);
		void setLanguageCombo(const QString &newLangAbbrev);

	private:
		ScribusDoc* m_doc;
		StoryText* m_iText;
		QMap<QString, QString>* m_dictionaryMap;
		QMap<QString, HunspellDict*> *m_hspellerMap;
		QList<WordsFound>* m_wfList;
		WordsFound currWF;
		int wfListIndex;
		bool m_docChanged;
		bool m_returnToDefaultLang;
		int m_primaryLangIndex;
};

#endif // HUNSPELLDIALOG_H
