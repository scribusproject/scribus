#include "javadocs.h"
#include "javadocs.moc"
#include "query.h"
#include "editor.h"
#include <qmessagebox.h>

extern QPixmap loadIcon(QString nam);

JavaDocs::JavaDocs(QWidget* parent, ScribusDoc *doc, ScribusView* vie)
    : QDialog( parent, "Javadocs", true, 0 )
{
    setCaption( tr( "Edit JavaScripts" ) );
	setIcon(loadIcon("AppIcon.png"));
    Doc = doc;
	View = vie;
    JavaDocsLayout = new QHBoxLayout( this, 11, 6, "JavaDocsLayout"); 

    Scripts = new QListBox( this, "Scripts" );
    Scripts->setMinimumSize( QSize( 150, 200 ) );
	QMap<QString,QString>::Iterator it;
	for (it = Doc->JavaScripts.begin(); it != Doc->JavaScripts.end(); ++it)
		Scripts->insertItem(it.key());
    JavaDocsLayout->addWidget( Scripts );

    Layout1 = new QVBoxLayout( 0, 0, 6, "Layout1"); 

    EditScript = new QPushButton( tr( "&Edit..." ), this, "EditScript" );
    Layout1->addWidget( EditScript );

    AddScript = new QPushButton( tr( "&Add..." ), this, "AddScript" );
    Layout1->addWidget( AddScript );

    DeleteScript = new QPushButton( tr( "&Delete" ), this, "DeleteScript" );
    Layout1->addWidget( DeleteScript );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout1->addItem( spacer );

    ExitDia = new QPushButton( tr( "&Close" ), this, "ExitDia" );
    ExitDia->setDefault( true );
    Layout1->addWidget( ExitDia );
	if (Doc->JavaScripts.count() == 0)
	{
		EditScript->setEnabled(false);
		DeleteScript->setEnabled(false);
	}
	else
		Scripts->setCurrentItem(0);
  	JavaDocsLayout->addLayout( Layout1 );
	connect(AddScript, SIGNAL(clicked()), this, SLOT(slotAdd()));
	connect(EditScript, SIGNAL(clicked()), this, SLOT(slotEdit()));
	connect(DeleteScript, SIGNAL(clicked()), this, SLOT(slotDelete()));
	connect(ExitDia, SIGNAL(clicked()), this, SLOT(accept()));
	connect( Scripts, SIGNAL( selected(QListBoxItem*) ), this, SLOT( slotEdit() ) );
}

void JavaDocs::slotAdd()
{
	QString nam;
	Query *dia = new Query(this, "tt", 1, 0, tr("New Script:"), tr("New Script"));
	dia->Answer->setText( tr("New Script"));
	if (dia->exec())
	{
		nam = dia->Answer->text();
		while (Doc->JavaScripts.contains(nam) || (nam == ""))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			nam = dia->Answer->text();
		}
		Editor* dia2 = new Editor(this, "", View);
		if (dia2->exec())
		{
			EditScript->setEnabled(true);
			DeleteScript->setEnabled(true);
			Doc->JavaScripts[nam] = dia2->EditTex->text();
			Scripts->insertItem(nam);
		}
		delete dia2;
		delete dia;
	}
}

void JavaDocs::slotEdit()
{
	QString nam = Scripts->currentText();
	Editor* dia2 = new Editor(this, Doc->JavaScripts[nam], View);
	if (dia2->exec())
		Doc->JavaScripts[nam] = dia2->EditTex->text();
	delete dia2;
}

void JavaDocs::slotDelete()
{
	int exit=QMessageBox::warning(this,
	                              tr("Warning"),
	                              tr("Do you really want do delete this Script?"),
	                              tr("No"),
	                              tr("Yes"),
	                              0, 0, 0);
	if (exit == 1)
	{
		QString nam = Scripts->currentText();
		Doc->JavaScripts.remove(nam);
		Scripts->clear();
		QMap<QString,QString>::Iterator it;
		for (it = Doc->JavaScripts.begin(); it != Doc->JavaScripts.end(); ++it)
			Scripts->insertItem(it.key());
		if (Doc->JavaScripts.count() == 0)
		{
			EditScript->setEnabled(false);
			DeleteScript->setEnabled(false);
		}
		else
			Scripts->setCurrentItem(0);
	}
}
