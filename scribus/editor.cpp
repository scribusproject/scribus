#include "editor.h"
#include "editor.moc"
#include "selfield.h"
#include "prefsfile.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qfiledialog.h>

extern QPixmap loadIcon(QString nam);
extern PrefsFile* prefsFile;

Editor::Editor( QWidget* parent, QString daten, ScribusView* vie)
								: QDialog( parent, "editor", true, 0 )
{
    setCaption( tr( "Editor" ) );
  	setIcon(loadIcon("AppIcon.png"));
	view = vie;
	dirs = prefsFile->getContext("dirs");
	EditorLayout = new QVBoxLayout( this, 0, 0, "EditorLayout");
    EditTex = new QTextEdit( this, "EditTex" );
	
  	fmenu = new QPopupMenu();
  	fmenu->insertItem(loadIcon("DateiNeu16.png"), tr("&New"), EditTex, SLOT(clear()), CTRL+Key_N);
  	fmenu->insertItem(loadIcon("DateiOpen16.png"), tr("&Open..."), this, SLOT(OpenScript()));
  	fmenu->insertItem( tr("Save &As..."), this, SLOT(SaveAs()));
	fmenu->insertSeparator();
	fmenu->insertItem( tr("&Save and Exit"), this, SLOT(accept()));
	fmenu->insertItem( tr("&Exit without Saving"), this, SLOT(reject()));
  	emenu = new QPopupMenu();
  	emenu->insertItem( tr("&Undo"), EditTex, SLOT(undo()), CTRL+Key_Z);
  	emenu->insertItem( tr("&Redo"), EditTex, SLOT(redo()));
	emenu->insertSeparator();
	emenu->insertItem(loadIcon("editcut.png"), tr("Cu&t"), EditTex, SLOT(cut()), CTRL+Key_X);
	emenu->insertItem(loadIcon("editcopy.png"), tr("&Copy"), EditTex, SLOT(copy()), CTRL+Key_C);
	emenu->insertItem(loadIcon("editpaste.png"), tr("&Paste"), EditTex, SLOT(paste()), CTRL+Key_V);
	emenu->insertItem(loadIcon("editdelete.png"), tr("C&lear"), EditTex, SLOT(del()), CTRL+Key_V);
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
		SelectFields* dia = new SelectFields(this, "", "", &view->Doc->Pages, 0);
		if (dia->exec())
		EditTex->insert(dia->S_Fields);
		delete dia;
	}
}

void Editor::OpenScript()
{
	QString fileName = QFileDialog::getOpenFileName(dirs->get("editor_open", "."), 
	                                                tr("Javascripts (*.js);;All Files (*)"),this);
	if (!fileName.isEmpty())
	{
		dirs->set("editor_open", fileName.left(fileName.findRev("/")));
		QFile file( fileName );
		if ( file.open( IO_ReadOnly ) )
		{
			QTextStream ts( &file );
			EditTex->setText( ts.read() );
			file.close();
		}
	}
}

void Editor::SaveAs()
{
	QString fn = QFileDialog::getSaveFileName(dirs->get("editor_save", "."), tr("Javascripts (*.js);;All Files (*)"), this);
	if (!fn.isEmpty())
  	{
		dirs->set("editor_save", fn.left(fn.findRev("/")));
		QFile file( fn );
		if ( file.open( IO_WriteOnly ) )
		{
			QTextStream ts( &file );
			ts << EditTex->text();
			EditTex->setModified(false);
			file.close();
		}
  	}
}
