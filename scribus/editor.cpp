/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "editor.h"
#include "selfield.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scribusview.h"

#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QTextCursor>
#include <QFileDialog>
#include <QPixmap>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

#include "util_icon.h"

Editor::Editor( QWidget* parent, QString daten, ScribusView* vie) : QDialog( parent )
{
	setModal(true);
	setWindowTitle( tr( "Editor" ) );
	setWindowIcon(loadIcon("AppIcon.png"));
	view = vie;
	dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	EditorLayout = new QVBoxLayout(this);
	EditTex = new QTextEdit(this);
	newAct = new QAction(QIcon(loadIcon("16/document-new.png")), tr("&New"), this);
	newAct->setShortcut(tr("Ctrl+N"));
	connect(newAct, SIGNAL(triggered()), EditTex, SLOT(clear()));
	openAct = new QAction(QIcon(loadIcon("16/document-open.png")), tr("&Open..."), this);
	connect(openAct, SIGNAL(triggered()), this, SLOT(OpenScript()));
	saveAsAct = new QAction( tr("Save &As..."), this);
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(SaveAs()));
	saveExitAct = new QAction( tr("&Save and Exit"), this);
	connect(saveExitAct, SIGNAL(triggered()), this, SLOT(accept()));
	exitAct = new QAction( tr("&Exit without Saving"), this);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(reject()));
	undoAct = new QAction(QIcon(loadIcon("16/edit-undo.png")), tr("&Undo"), this);
	undoAct->setShortcut(tr("Ctrl+Z"));
	connect(undoAct, SIGNAL(triggered()), EditTex, SLOT(undo()));
	redoAct = new QAction(QIcon(loadIcon("16/edit-redo.png")),  tr("&Redo"), this);
	connect(redoAct, SIGNAL(triggered()), EditTex, SLOT(redo()));
	cutAct = new QAction(QIcon(loadIcon("16/edit-cut.png")), tr("Cu&t"), this);
	cutAct->setShortcut(tr("Ctrl+X"));
	connect(cutAct, SIGNAL(triggered()), EditTex, SLOT(cut()));
	copyAct = new QAction(QIcon(loadIcon("16/edit-copy.png")), tr("&Copy"), this);
	copyAct->setShortcut(tr("Ctrl+C"));
	connect(copyAct, SIGNAL(triggered()), EditTex, SLOT(copy()));
	pasteAct = new QAction(QIcon(loadIcon("16/edit-paste.png")), tr("&Paste"), this);
	pasteAct->setShortcut(tr("Ctrl-V"));
	connect(pasteAct, SIGNAL(triggered()), EditTex, SLOT(paste()));
	clearAct = new QAction(QIcon(loadIcon("16/edit-delete.png")), tr("C&lear"), this);
	connect(clearAct, SIGNAL(triggered()), this, SLOT(del()));
	getFieldAct = new QAction( tr("&Get Field Names"), this);
	connect(getFieldAct, SIGNAL(triggered()), this, SLOT(GetFieldN()));
	fmenu = new QMenu( tr("&File"));
	fmenu->addAction(newAct);
	fmenu->addAction(openAct);
	fmenu->addAction(saveAsAct);
	fmenu->addSeparator();
	fmenu->addAction(saveExitAct);
	fmenu->addAction(exitAct);
	emenu = new QMenu( tr("&Edit"));
	emenu->addAction(undoAct);
	emenu->addAction(redoAct);
	emenu->addSeparator();
	emenu->addAction(cutAct);
	emenu->addAction(copyAct);
	emenu->addAction(pasteAct);
	emenu->addAction(clearAct);
	emenu->addSeparator();
	emenu->addAction(getFieldAct);
	menuBar = new QMenuBar(this);
	menuBar->addMenu(fmenu);
	menuBar->addMenu(emenu);
	EditorLayout->setMenuBar( menuBar );
	EditTex->setMinimumSize( QSize( 400, 400 ) );
	EditTex->setPlainText(daten);
	EditorLayout->addWidget( EditTex );
}

void Editor::del()
{
	QTextCursor curs = EditTex->textCursor();
	curs.deleteChar();
	EditTex->setTextCursor(curs);
}

void Editor::GetFieldN()
{
	if (view != 0)
	{
		SelectFields* dia = new SelectFields(this, "", "", view->Doc, 0);
		if (dia->exec())
			EditTex->insertPlainText(dia->S_Fields);
		delete dia;
	}
}

void Editor::OpenScript()
{
	QString fileName = QFileDialog::getOpenFileName(this, dirs->get("editor_open", "."), tr("JavaScripts (*.js);;All Files (*)"));
	if (!fileName.isEmpty())
	{
		dirs->set("editor_open", fileName.left(fileName.lastIndexOf("/")));
		QFile file( fileName );
		if ( file.open( QIODevice::ReadOnly ) )
		{
			QTextStream ts( &file );
			EditTex->setPlainText( ts.readAll() );
			file.close();
		}
	}
}

void Editor::SaveAs()
{
	QString fn = QFileDialog::getSaveFileName(this, dirs->get("editor_save", "."), tr("JavaScripts (*.js);;All Files (*)"));
	if (!fn.isEmpty())
	{
		dirs->set("editor_save", fn.left(fn.lastIndexOf("/")));
		QFile file( fn );
		if ( file.open( QIODevice::WriteOnly ) )
		{
			QTextStream ts( &file );
			ts << EditTex->toPlainText();
			EditTex->document()->setModified(false);
			file.close();
		}
	}
}
