/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_INDEXES_H
#define PREFS_INDEXES_H

#include <QStringList>
#include <QListWidget>
#include <QListWidgetItem>

#include "ui_prefs_indexesbase.h"
#include "pagestructs.h"
#include "prefs_pane.h"
#include "prefsstructs.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Indexes : public Prefs_Pane, Ui::Prefs_Indexes
{
	Q_OBJECT

	public:
		Prefs_Indexes(QWidget* parent, ScribusDoc* doc = nullptr);
		~Prefs_Indexes();

		void restoreDefaults(struct ApplicationPrefs *prefsData) override;
		void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const override;

		void changeEvent(QEvent *e) override;
		virtual void enableGUIWidgets();

		void updateStyleComboBoxes();
		void updateIndexListBox();
		void generatePageItemList();
		void setIndexName(const QString& newName);

	public slots:
		void languageChange();
		void addIndex();
		void deleteIndex();
		void indexListWidgetItemEdited(QListWidgetItem *qlwi);
		void itemFrameSelected(const QString &frameName);
		void listNonPrintingChecked(bool b);
		void combineIdenticalEntriesChecked(bool b);
		void caseSenstiveChecked(bool b);
		void autoCapitalizeEntriesChecked(bool b);
		void addAlphaSeparatorsChecked(bool b);
		void headingStyleSelected(const QString& style);
		void separatorStyleSelected(const QString& style);
		void level1StyleSelected(const QString& style);
		void level2StyleSelected(const QString& style);
		void level3StyleSelected(const QString& style);
		void selectIndex(int numberSelected);

		protected:
		int numSelected { -1 };
		ScribusDoc *m_Doc {nullptr};
		IndexSetupVector localIndexSetupVector;
};

#endif // PREFS_INDEXES_H
