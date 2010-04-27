/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_KEYBOARDSHORTCUTS_H
#define PREFS_KEYBOARDSHORTCUTS_H

#include <QMap>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QKeyEvent>
#include <QEvent>

#include "ui_prefs_keyboardshortcutsbase.h"
#include "prefs_pane.h"
#include "scribusapi.h"
#include "scribusstructs.h"


class SCRIBUS_API Prefs_KeyboardShortcuts : public Prefs_Pane, Ui::Prefs_KeyboardShortcuts
{
	Q_OBJECT

	public:
		Prefs_KeyboardShortcuts(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_KeyboardShortcuts();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

		bool event( QEvent* ev );
		void keyPressEvent(QKeyEvent *k);
		void keyReleaseEvent(QKeyEvent *k);
		static QString getKeyText(int KeyC);

	public slots:
		void languageChange();

protected:
	QMap<QString,Keys> keyMap;
	QMap<QString,Keys>::Iterator currentKeyMapRow;
	QMap<QString, QString> keySetList;
	QMap<QTreeWidgetItem*, QString> lviToActionMap;
	QList<QTreeWidgetItem*> lviToMenuMap;
	QVector< QPair<QString, QStringList> >* defMenus;
	QVector< QPair<QString, QStringList> >* defNonMenuActions;
	QTreeWidgetItem * selectedLVI;
	int keyCode;
	QString Part0;
	QString Part1;
	QString Part2;
	QString Part3;
	QString Part4;

	void insertActions();
	void importKeySet(QString);
	bool exportKeySet(QString);
	QStringList scanForSets();
	bool checkKey(int Code);

protected slots:
	void setKeyText();
	void dispKey(QTreeWidgetItem* current, QTreeWidgetItem* previous=0);
	void setNoKey();
	void loadKeySetFile();
	void importKeySetFile();
	void exportKeySetFile();
	void resetKeySet();
	void clearSearchString();
	void applySearch( const QString & newss );
};

#endif // PREFS_KEYBOARDSHORTCUTS_H
