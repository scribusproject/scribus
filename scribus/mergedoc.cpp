#include "mergedoc.h"
#include "mergedoc.moc"
#include "customfdialog.h"
#include "scribusXml.h"
#include "prefsfile.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#include <qcursor.h>

extern QPixmap loadIcon(QString nam);
extern PrefsFile* prefsFile;

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
	layout1 = new QGridLayout(1, 1, 5, "layout");
	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setText( tr( "From Document:" ) );
	layout1->addWidget( textLabel1, 0, 0 );
	Filename = new QLineEdit( this, "Filename" );
	layout1->addWidget( Filename, 0, 1 );
	Change = new QPushButton( this, "Change" );
	Change->setAutoDefault( FALSE );
	Change->setText( tr( "Change..." ) );
	layout1->addWidget( Change, 0, 2 );
	textLabel3 = new QLabel( this, "textLabel3" );
	textLabel3->setText( tr( "Import Page(s):" ) );
	layout1->addWidget( textLabel3, 1, 0 );
	if (Mpa)
	{
		textLabel3->setText( tr("Import Template") );
		PageNa = new QComboBox( true, this, "Templ" );
		PageNa->setEditable(false);
		PageNa->setEnabled(false);
		layout1->addWidget( PageNa, 1, 1 );
	}
	else
	{
		PageNr = new QLineEdit( this, "PageNr" );
		PageNr->setEnabled(false);
		QToolTip::add( PageNr, tr( "Insert a comma separated list of tokens where\n"
		                           "a token can be * for all the pages, 1-5 for\n"
		                           "a range of pages or a single page number.") );
		layout1->addWidget( PageNr, 1, 1 );
		fromLabel = new QLabel(this, "fromLabel");
		fromLabel->setText(tr(" from 0"));
		layout1->addWidget( fromLabel, 1, 2 );
		Create = new QCheckBox( this, "Create" );
		Create->setText(tr("Create Page(s)"));
		layout1->addWidget( Create, 2, 0 );
		Where = new QComboBox( FALSE, this, "positionCombo" );
		Where->insertItem(tr("before Page"));
		Where->insertItem(tr("after Page"));
		Where->insertItem(tr("at End"));
		Where->setCurrentItem( 2 );
		Where->setEnabled( false );
		layout1->addWidget( Where, 2, 1 );
		ActualPage = new QSpinBox( this, "pageSpin" );
		ActualPage->setMinValue( 1 );
		ActualPage->setMaxValue( targetCount );
		ActualPage->setValue( activePage );
		ActualPage->setEnabled(false);
		layout1->addWidget( ActualPage, 2, 2 );
	}
	MergeDocLayout->addLayout( layout1 );
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
	if (!Mpa)
	{
		connect( Where, SIGNAL( activated(int) ), this, SLOT( CheckDestPageStatus(int) ) );
		connect( Create, SIGNAL( clicked() ), this, SLOT( enableCreateWidgets() ) );
	}
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
	PrefsContext* dirs = prefsFile->getContext("dirs");
	QString wdir = dirs->get("merge", ".");
#ifdef HAVE_LIBZ
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
#else
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.scd);;All Files (*)"));
#endif
	if (Filename->text() != "")
		dia.setSelection(Filename->text());
	if (dia.exec() == QDialog::Accepted)
	{
		fn = dia.selectedFile();
		if (!fn.isEmpty())
		{
			dirs->set("merge", fn.left(fn.findRev("/")));
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
