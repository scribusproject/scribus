/****************************************************************************
** Form implementation generated from reading ui file 'mergedoc.ui'
**
** Created: Sam Apr 5 09:30:28 2003
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "mergedoc.h"
#include "mergedoc.moc"
#include "customfdialog.h"
#include "scribusXml.h"
#include "config.h"
#include <qcursor.h>

extern QPixmap loadIcon(QString nam);

MergeDoc::MergeDoc( QWidget* parent ) : QDialog( parent, "merge", true, 0 )
{
	setCaption( tr( "Import a Page" ) );
	setIcon(loadIcon("AppIcon.png"));
	Count = 0;

	MergeDocLayout = new QVBoxLayout( this, 11, 6, "MergeDocLayout"); 
	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setText( tr( "Document to load:" ) );
	MergeDocLayout->addWidget( textLabel1 );
	layout1 = new QHBoxLayout( 0, 0, 5, "layout1"); 
	Filename = new QLineEdit( this, "Filename" );
	Filename->setMinimumSize( QSize( 120, 0 ) );
	layout1->addWidget( Filename );
	Change = new QPushButton( this, "Change" );
	Change->setAutoDefault( FALSE );
	Change->setText( tr( "Change..." ) );
	layout1->addWidget( Change );
	MergeDocLayout->addLayout( layout1 );

	Inform = new QLabel( this, "Inform" );
	Inform->setText( tr( "Document contains: 0 Page(s)" ) );
	MergeDocLayout->addWidget( Inform );

	layout2 = new QHBoxLayout( 0, 0, 2, "layout2"); 
	textLabel3 = new QLabel( this, "textLabel3" );
	textLabel3->setText( tr( "Import Page Nr:" ) );
	layout2->addWidget( textLabel3 );
	PageNr = new QSpinBox( this, "PageNr" );
	PageNr->setMaxValue( 2000 );
	PageNr->setValue( 1 );
	PageNr->setEnabled(false);
	layout2->addWidget( PageNr );
	MergeDocLayout->addLayout( layout2 );

	layout3 = new QHBoxLayout( 0, 0, 2, "layout3"); 
	QSpacerItem* spacer = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout3->addItem( spacer );
	Import = new QPushButton( this, "Import" );
	Import->setText( tr( "Import" ) );
	Import->setEnabled(false);
	layout3->addWidget( Import );
	QSpacerItem* spacer_2 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout3->addItem( spacer_2 );
	Cancel = new QPushButton( this, "Cancel" );
	Cancel->setDefault( TRUE );
	Cancel->setText( tr( "Cancel" ) );
	layout3->addWidget( Cancel );
	QSpacerItem* spacer_3 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout3->addItem( spacer_3 );
	MergeDocLayout->addLayout( layout3 );
	resize( QSize(350, 134).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	connect( Import, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( Change, SIGNAL( clicked() ), this, SLOT( ChangeFile() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
MergeDoc::~MergeDoc()
{
    // no need to delete child widgets, Qt does it all for us
}

void MergeDoc::ChangeFile()
{
	QString fn;
#ifdef HAVE_LIBZ
	CustomFDialog dia(this, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
#else
	CustomFDialog dia(this, tr("Open"), tr("Documents (*.sla *.scd);;All Files (*)"));
#endif
	if (Filename->text() != "")
		dia.setSelection(Filename->text());
	if (dia.exec() == QDialog::Accepted)
		{
		fn = dia.selectedFile();
		if (!fn.isEmpty())
			{
			qApp->setOverrideCursor(QCursor(waitCursor), true);
			ScriXmlDoc *ss = new ScriXmlDoc();
			Count = ss->ReadPageCount(fn);
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			if (Count != 0)
				{
				Filename->setText(fn);
				Import->setEnabled(true);
				PageNr->setEnabled(true);
				PageNr->setValue(1);
				PageNr->setMaxValue(Count);
				Inform->setText(tr("Document contains: %1 Page(s)").arg(Count));
				}
			delete ss;
			}
		}
	else
		{
		Filename->setText("");
		Count = 0;
		Import->setEnabled(false);
		PageNr->setEnabled(false);
		PageNr->setValue(0);
		PageNr->setMaxValue(Count);
		Inform->setText(tr("Document contains: %1 Page(s)").arg(Count));
		}
}

