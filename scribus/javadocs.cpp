#include "javadocs.h"
#include "javadocs.moc"
#include "query.h"
#include "editor.h"
extern QPixmap loadIcon(QString nam);

JavaDocs::JavaDocs(QWidget* parent, ScribusDoc *doc, ScribusView* vie)
    : QDialog( parent, "Javadocs", true, 0 )
{
    setCaption( tr( "Global JavaScripts" ) );
 		setIcon(loadIcon("AppIcon.png"));
    Doc = doc;
		View = vie;
    JavaDocsLayout = new QHBoxLayout( this, 11, 6, "JavaDocsLayout"); 

    Scripts = new QListBox( this, "Scripts" );
    Scripts->setMinimumSize( QSize( 150, 200 ) );
		QMap<QString,QString>::Iterator it;
		for (it = Doc->JavaScripts.begin(); it != Doc->JavaScripts.end(); ++it)
			{
			Scripts->insertItem(it.key());
			}
    JavaDocsLayout->addWidget( Scripts );

    Layout1 = new QVBoxLayout( 0, 0, 6, "Layout1"); 

    EditScript = new QPushButton( this, "EditScript" );
    EditScript->setText( tr( "Edit..." ) );
    Layout1->addWidget( EditScript );

    AddScript = new QPushButton( this, "AddScript" );
    AddScript->setText( tr( "Add..." ) );
    Layout1->addWidget( AddScript );

    DeleteScript = new QPushButton( this, "DeleteScript" );
    DeleteScript->setText( tr( "Delete" ) );
    Layout1->addWidget( DeleteScript );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout1->addItem( spacer );

    ExitDia = new QPushButton( this, "ExitDia" );
    ExitDia->setText( tr( "Close" ) );
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
}

void JavaDocs::slotAdd()
{
	QString nam;
	Query *dia = new Query(this, "tt", 1, 0, "New Script:", "New Script");
	dia->Answer->setText(tr("New Script"));
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
	QString nam = Scripts->currentText();
	Doc->JavaScripts.remove(nam);
	Scripts->clear();
	QMap<QString,QString>::Iterator it;
	for (it = Doc->JavaScripts.begin(); it != Doc->JavaScripts.end(); ++it)
		{
		Scripts->insertItem(it.key());
		}
	if (Doc->JavaScripts.count() == 0)
		{
		EditScript->setEnabled(false);
		DeleteScript->setEnabled(false);
		}
	else
		Scripts->setCurrentItem(0);
}
