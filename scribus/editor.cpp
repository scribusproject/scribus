/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "editor.h"
//#include "editor.moc"
#include "selfield.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scribusview.h"

#include <qfile.h>
#include <q3textstream.h>
#include <q3filedialog.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3VBoxLayout>
#include <Q3PopupMenu>

extern QPixmap loadIcon(QString nam);

Editor::Editor( QWidget* parent, QString daten, ScribusView* vie) : QDialog( parent, "editor", true, 0 )
{
	setCaption( tr( "Editor" ) );
	setIcon(loadIcon("AppIcon.png"));
	view = vie;
	dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	EditorLayout = new Q3VBoxLayout( this, 0, 0, "EditorLayout");
	EditTex = new Q3TextEdit( this, "EditTex" );

	fmenu = new Q3PopupMenu();
	fmenu->insertItem(loadIcon("DateiNeu16.png"), tr("&New"), EditTex, SLOT(clear()), Qt::CTRL+Qt::Key_N);
	fmenu->insertItem(loadIcon("DateiOpen16.png"), tr("&Open..."), this, SLOT(OpenScript()));
	fmenu->insertItem( tr("Save &As..."), this, SLOT(SaveAs()));
	fmenu->insertSeparator();
	fmenu->insertItem( tr("&Save and Exit"), this, SLOT(accept()));
	fmenu->insertItem( tr("&Exit without Saving"), this, SLOT(reject()));
	emenu = new Q3PopupMenu();
	emenu->insertItem( tr("&Undo"), EditTex, SLOT(undo()), Qt::CTRL+Qt::Key_Z);
	emenu->insertItem( tr("&Redo"), EditTex, SLOT(redo()));
	emenu->insertSeparator();
	emenu->insertItem(loadIcon("editcut.png"), tr("Cu&t"), EditTex, SLOT(cut()), Qt::CTRL+Qt::Key_X);
	emenu->insertItem(loadIcon("editcopy.png"), tr("&Copy"), EditTex, SLOT(copy()), Qt::CTRL+Qt::Key_C);
	emenu->insertItem(loadIcon("editpaste.png"), tr("&Paste"), EditTex, SLOT(paste()), Qt::CTRL+Qt::Key_V);
	emenu->insertItem(loadIcon("editdelete.png"), tr("C&lear"), EditTex, SLOT(del()), Qt::CTRL+Qt::Key_V);
	emenu->insertSeparator();
	emenu->insertItem( tr("&Get Field Names"), this, SLOT(GetFieldN()));
	menuBar = new QMenuBar(this);
	menuBar->insertItem( tr("&File"), fmenu);
	menuBar->insertItem( tr("&Edit"), emenu);
	EditorLayout->setMenuBar( menuBar );

	EditTex->setMinimumSize( QSize( 400, 400 ) );
	EditTex->setText(daten);
	EditorLayout->addWidget( EditTex );
}

void Editor::GetFieldN()
{
	if (view != 0)
	{
		SelectFields* dia = new SelectFields(this, "", "", view->Doc, 0);
		if (dia->exec())
			EditTex->insert(dia->S_Fields);
		delete dia;
	}
}

void Editor::OpenScript()
{
	QString fileName = Q3FileDialog::getOpenFileName(dirs->get("editor_open", "."), tr("JavaScripts (*.js);;All Files (*)"),this);
	if (!fileName.isEmpty())
	{
		dirs->set("editor_open", fileName.left(fileName.findRev("/")));
		QFile file( fileName );
		if ( file.open( QIODevice::ReadOnly ) )
		{
			Q3TextStream ts( &file );
			EditTex->setText( ts.read() );
			file.close();
		}
	}
}

void Editor::SaveAs()
{
	QString fn = Q3FileDialog::getSaveFileName(dirs->get("editor_save", "."), tr("JavaScripts (*.js);;All Files (*)"), this);
	if (!fn.isEmpty())
	{
		dirs->set("editor_save", fn.left(fn.findRev("/")));
		QFile file( fn );
		if ( file.open( QIODevice::WriteOnly ) )
		{
			Q3TextStream ts( &file );
			ts << EditTex->text();
			EditTex->setModified(false);
			file.close();
		}
	}
}
