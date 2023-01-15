/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EDITOR_H
#define EDITOR_H

#include <QDialog>

#include "scribusapi.h"

class QTextEdit;
class QMenu;
class QMenuBar;
class QVBoxLayout;
class QAction;
class ScribusView;
class PrefsContext;
#ifdef Q_OS_MACOS
	class QHBoxLayout;
	class QPushButton;
#endif

class SCRIBUS_API Editor : public QDialog
{
		Q_OBJECT

public:
	Editor (QWidget* parent, const QString& daten, ScribusView* vie = 0 );
	~Editor() {};

	QTextEdit* EditTex;
	QMenu* fmenu;
	QMenu* emenu;
	QMenuBar* menuBar;
	QAction* newAct;
	QAction* openAct;
	QAction* saveAsAct;
	QAction* saveExitAct;
	QAction* exitAct;
	QAction* undoAct;
	QAction* redoAct;
	QAction* cutAct;
	QAction* copyAct;
	QAction* pasteAct;
	QAction* clearAct;
	QAction* getFieldAct;
	ScribusView* view;
#ifdef Q_OS_MACOS
	QPushButton* PushButton1;
	QPushButton* PushButton2;
#endif

private slots:
	void del();
	void iconSetChange();
	void OpenScript();
	void SaveAs();
	void GetFieldNames();

protected:
	QVBoxLayout* EditorLayout;
#ifdef Q_OS_MACOS
	QHBoxLayout* Layout1_2;
#endif
	PrefsContext* dirs;
};

#endif // EDITOR_H
