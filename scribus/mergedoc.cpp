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

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#include <qcursor.h>

extern QPixmap loadIcon(QString nam);

MergeDoc::MergeDoc( QWidget* parent, bool Mpages, int targetDocPageCount, int activePage ) : 
    QDialog( parent, "merge", true, 0 )
{
	if (Mpages)
		setCaption( tr("Import Template") );
	else
		setCaption( tr( "Import Page(s)" ) );
	setIcon(loadIcon("AppIcon.png"));
	Count = 0;
	Mpa = Mpages;
	targetCount = targetDocPageCount;
	MergeDocLayout = new QVBoxLayout( this, 10, 10, "MergeDocLayout"); 
	layout1 = new QHBoxLayout( 0, 0, 5, "layout1"); 
	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setText( tr( "From Document:" ) );
	textLabel1->setMinimumWidth( 125 ); // 1. Column
	layout1->addWidget( textLabel1 );
	Filename = new QLineEdit( this, "Filename" );
	Filename->setMinimumWidth( 150 ); // 2. Column
	layout1->addWidget( Filename );
	Change = new QPushButton( this, "Change" );
	Change->setAutoDefault( FALSE );
	Change->setText( tr( "Change..." ) );
	Change->setMinimumWidth( 80 ); // 3. Column
	layout1->addWidget( Change );
	MergeDocLayout->addLayout( layout1 );

// 	Inform = new QLabel( this, "Inform" );
// 	Inform->setText( tr( "Document contains: 0 Page(s)" ) );
// 	MergeDocLayout->addWidget( Inform );

	layout2 = new QHBoxLayout( 0, 0, 5, "layout2"); 
	textLabel3 = new QLabel( this, "textLabel3" );
	textLabel3->setText( tr( "Import Page(s):" ) );
	textLabel3->setMinimumWidth( 125 ); // 1. Column
	layout2->addWidget( textLabel3 );
	if (Mpa)
	{
		textLabel3->setText( tr("Import Template") );
		PageNa = new QComboBox( true, this, "Templ" );
		PageNa->setMinimumWidth( 150 ); // 2. Column
		PageNa->setEditable(false);
		PageNa->setEnabled(false);
		layout2->addWidget( PageNa );
		layout2->addStretch(10);
	}
	else
	{
		PageNr = new QLineEdit( this, "PageNr" );
		PageNr->setEnabled(false);
		PageNr->setMinimumWidth( 150 ); // 2. Column
		QToolTip::add( PageNr, tr( "Insert a comma separated list of tokens where\n"
		                           "a token can be * for all the pages, 1-5 for\n"
		                           "a range of pages or a single page number.") );
		layout2->addWidget( PageNr );
		fromLabel = new QLabel(this, "fromLabel");
		fromLabel->setText(tr(" from 0"));
		fromLabel->setMinimumWidth( 80 ); // 3. Column
		layout2->addWidget( fromLabel );
	}

	MergeDocLayout->addLayout( layout2 );
	if (!Mpa)
	{
		layout2_6 = new QHBoxLayout( 0, 0, 2, "layout2_6" );
		Create = new QCheckBox( this, "Create" );
		Create->setText(tr("Create Page(s)"));
		Create->setMinimumWidth( 125 ); // 1. Column
		layout2_6->addWidget( Create );
		Where = new QComboBox( FALSE, this, "positionCombo" );
		Where->insertItem(tr("before Page"));
		Where->insertItem(tr("after Page"));
		Where->insertItem(tr("at End"));
		Where->setCurrentItem( 2 );
		Where->setEnabled( false );
		Where->setMinimumWidth( 150 ); // 2. Column
		layout2_6->addWidget( Where );
		ActualPage = new QSpinBox( this, "pageSpin" );
		ActualPage->setMinValue( 1 );
		ActualPage->setMaxValue( targetCount );
		ActualPage->setValue( activePage );
		ActualPage->setEnabled(false);
		ActualPage->setMinimumWidth( 80 ); // 3. Column
		layout2_6->addWidget( ActualPage );
		MergeDocLayout->addLayout( layout2_6 );
	}
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
	if (Where)
		connect( Where, SIGNAL( activated(int) ), this, SLOT( CheckDestPageStatus(int) ) );
	if (Create)
		connect( Create, SIGNAL( clicked() ), this, SLOT( enableCreateWidgets() ) );
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
	int dummy;
	bool ret = false;
	Count = 0;
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
			if (Mpa)
				ret = ss->ReadPageCount(fn, &dummy, &Count);
			else
				ret = ss->ReadPageCount(fn, &Count, &dummy);
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			if ((ret) && (Count != 0))
			{
				Filename->setText(fn);
				Import->setEnabled(true);
				if (Mpa)
				{
					PageNa->clear();
					PageNa->setEnabled(true);
					PageNa->insertStringList(ss->MNames);
				}
				else
				{
					PageNr->setEnabled(true);
				}
				if (!Mpa)
					fromLabel->setText( tr(" from %1").arg(Count));
			}
			delete ss;
		}
	}
	else
	{
		Filename->setText("");
		Count = 0;
		Import->setEnabled(false);
		if (Mpa)
		{
			PageNa->clear();
			PageNa->setEnabled(false);
		}
		else
		{
			PageNr->setEnabled(false);
		}
		if (!Mpa)
			fromLabel->setText( tr(" from %1").arg(Count));
	}
}

void MergeDoc::CheckDestPageStatus(int positionComboSelection)
{
	if (positionComboSelection == 2)
		ActualPage->setEnabled( false );
	else
		ActualPage->setEnabled( true );
}

void MergeDoc::enableCreateWidgets()
{
	if (Create->isChecked())
	{
		Where->setEnabled(true);
		if (Where->currentItem() != 2)
			ActualPage->setEnabled(true);
	}
	else
	{
		Where->setEnabled(false);
		ActualPage->setEnabled(false);
	}
}
