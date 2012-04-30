#ifndef HUNSPELLDIALOG_H
#define HUNSPELLDIALOG_H

#include <QDialog>
#include <QList>
#include <QStringList>
#include <QWidget>

#include <hunspell/hunspell.hxx>

#include "pluginapi.h"
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
		void set(QStringList* dictEntries, Hunspell **hspellers, QList<WordsFound>* wfList);
		bool docChanged() {return m_docChanged;}

	public slots:
		void goToNextWord(int i=-1);
		void ignoreAllWords();
		void changeWord();
		void changeAllWords();
		void replaceWord(int i);

	private:
		ScribusDoc* m_doc;
		StoryText* m_Itext;
		QStringList* m_dictEntries;
		Hunspell **m_hspellers;
		QList<WordsFound>* m_wfList;
		WordsFound currWF;
		int wfListIndex;
		bool m_docChanged;
};

#endif // HUNSPELLDIALOG_H
