#include "dialog.h"
#include "dialog.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qdir.h>
#include <qfiledialog.h>
#include "prefsmanager.h"
#include <prefsfile.h>
#include <prefscontext.h>

#include "commonstrings.h"

/*
 *  Constructs a ExportForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ExportForm::ExportForm(QWidget* parent, int size, int quality, QString type)
	: QDialog(parent, "ExportForm", true, 0)
{
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("pixmapexport");
	ExportFormLayout = new QVBoxLayout( this, 10, 5, "ExportFormLayout");
	layout1 = new QHBoxLayout( 0, 0, 5, "layout1");
	TextLabel1 = new QLabel( this, "TextLabel1" );
	layout1->addWidget( TextLabel1 );
	OutputDirectory = new QLineEdit( this, "OutputDirectory" );
	OutputDirectory->setText(prefs->get("wdir", QDir::currentDirPath()));
	layout1->addWidget( OutputDirectory );
	OutputDirectoryButton = new QPushButton( this, "OutputDirectoryButton" );
	OutputDirectoryButton->setDefault( true );
	layout1->addWidget( OutputDirectoryButton );
	ExportFormLayout->addLayout( layout1 );

	layout3 = new QHBoxLayout( 0, 0, 5, "layout3");
	groupBox1 = new QGroupBox( this, "groupBox1" );
	groupBox1->setColumnLayout(0, Qt::Vertical );
	groupBox1->layout()->setSpacing( 5 );
	groupBox1->layout()->setMargin( 10 );
	groupBox1Layout = new QGridLayout( groupBox1->layout() );
	groupBox1Layout->setAlignment( Qt::AlignTop );
	TextLabel2 = new QLabel( groupBox1, "TextLabel2" );
	groupBox1Layout->addWidget( TextLabel2, 0, 0 );
	textLabel1 = new QLabel( groupBox1, "textLabel1" );
	groupBox1Layout->addWidget( textLabel1, 1, 0 );
	textLabel3 = new QLabel( groupBox1, "textLabel3" );
	groupBox1Layout->addWidget( textLabel3, 2, 0 );
	textLabel4 = new QLabel( groupBox1, "textLabel4" );
	groupBox1Layout->addWidget( textLabel4, 3, 0 );
	BitmapType = new QComboBox( false, groupBox1, "BitmapType" );
    BitmapType->clear();
	BitmapType->insertStrList(QImageIO::outputFormats());
	BitmapType->setCurrentText(type);
	BitmapType->setEditable(false);
	groupBox1Layout->addMultiCellWidget( BitmapType, 0, 0, 1, 2 );
	QualityBox = new QSpinBox( groupBox1, "QualityBox" );
	QualityBox->setMaxValue( 100 );
	QualityBox->setMinValue( 1 );
	QualityBox->setValue(quality);
	groupBox1Layout->addWidget( QualityBox, 1, 1 );
	DPIBox = new QSpinBox( groupBox1, "DPIBox" );
	DPIBox->setMaxValue( 2400 );
	DPIBox->setMinValue( 72 );
	DPIBox->setValue(size);
	groupBox1Layout->addWidget( DPIBox, 2, 1 );
	EnlargementBox = new QSpinBox( groupBox1, "EnlargementBox" );
	EnlargementBox->setMaxValue( 2400 );
	EnlargementBox->setMinValue( 1 );
	EnlargementBox->setValue(size);
	groupBox1Layout->addWidget( EnlargementBox, 3, 1 );
	layout3->addWidget( groupBox1 );

	ButtonGroup1 = new QButtonGroup( this, "ButtonGroup1" );
	ButtonGroup1->setColumnLayout(0, Qt::Vertical );
	ButtonGroup1->layout()->setSpacing( 5 );
	ButtonGroup1->layout()->setMargin( 10 );
	ButtonGroup1Layout = new QVBoxLayout( ButtonGroup1->layout() );
	ButtonGroup1Layout->setAlignment( Qt::AlignTop );
	OnePageRadio = new QRadioButton( ButtonGroup1, "OnePageRadio" );
	OnePageRadio->setChecked( true );
	ButtonGroup1Layout->addWidget( OnePageRadio );
	AllPagesRadio = new QRadioButton( ButtonGroup1, "AllPagesRadio" );
	ButtonGroup1Layout->addWidget( AllPagesRadio );
	layout2 = new QHBoxLayout( 0, 0, 5, "layout2");
	IntervalPagesRadio = new QRadioButton( ButtonGroup1, "IntervalPagesRadio" );
	layout2->addWidget( IntervalPagesRadio );
	RangeVal = new QLineEdit( ButtonGroup1, "RangeVal" );
	layout2->addWidget( RangeVal );
	RangeVal->setEnabled(false);
	ButtonGroup1Layout->addLayout( layout2 );
	layout3->addWidget( ButtonGroup1 );
	ExportFormLayout->addLayout( layout3 );

	layout4 = new QHBoxLayout( 0, 0, 5, "layout4");
	QSpacerItem* spacer = new QSpacerItem( 111, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout4->addItem( spacer );
	OkButton = new QPushButton( this, "OkButton" );
	layout4->addWidget( OkButton );
	CancelButton = new QPushButton( this, "CancelButton" );
	layout4->addWidget( CancelButton );
	ExportFormLayout->addLayout( layout4 );
	languageChange();
	readConfig();
	resize( QSize(447, 206).expandedTo(minimumSizeHint()) );

    // buddies
	TextLabel1->setBuddy( OutputDirectory );
	TextLabel2->setBuddy( BitmapType );
	textLabel1->setBuddy( QualityBox );
	textLabel3->setBuddy( DPIBox );
	textLabel4->setBuddy( EnlargementBox );
	connect(OutputDirectoryButton, SIGNAL(clicked()), this, SLOT(OutputDirectoryButton_pressed()));
	connect(OkButton, SIGNAL(clicked()), this, SLOT(OkButton_pressed()));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(IntervalPagesRadio, SIGNAL(stateChanged(int)), this, SLOT(IntervalPagesRadio_stateChanged(int)));
	connect(AllPagesRadio, SIGNAL(stateChanged(int)), this, SLOT(AllPagesRadio_stateChanged(int)));
	connect(OnePageRadio, SIGNAL(stateChanged(int)), this, SLOT(OnePageRadio_stateChanged(int)));
}

void ExportForm::OutputDirectoryButton_pressed()
{
	QString lastDir = prefs->get("wdir", ".");
	QString d = QFileDialog::getExistingDirectory(lastDir, this, "d", tr("Choose a Export Directory"), true);
	if (d.length()>0)
	{
		OutputDirectory->setText(d);
		prefs->set("wdir", d);
	}
}

void ExportForm::OkButton_pressed()
{
	bitmapType = BitmapType->currentText();
	writeConfig();
	accept();
}

void ExportForm::IntervalPagesRadio_stateChanged(int)
{
	RangeVal->setEnabled(true);
}

void ExportForm::AllPagesRadio_stateChanged(int)
{
	RangeVal->setEnabled(false);
}

void ExportForm::OnePageRadio_stateChanged(int)
{
	RangeVal->setEnabled(false);
}

/*
 *  Destroys the object and frees any allocated resources
 */
ExportForm::~ExportForm()
{
	// no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ExportForm::languageChange()
{
	setCaption( tr( "Export as Image(s)" ) );
	TextLabel1->setText( tr( "&Export to Directory:" ) );
	OutputDirectoryButton->setText( tr( "C&hange..." ) );
	groupBox1->setTitle( tr( "Options" ) );
	TextLabel2->setText( tr( "Image &Type:" ) );
	textLabel1->setText( tr( "&Quality:" ) );
	textLabel3->setText( tr( "&Resolution:" ) );
	textLabel4->setText( tr( "&Size:" ) );
	QualityBox->setSuffix( tr( " %" ) );
	DPIBox->setSuffix( tr( " dpi" ) );
	EnlargementBox->setSuffix( tr( " %" ) );
	ButtonGroup1->setTitle( tr( "Range" ) );
	OnePageRadio->setText( tr( "&Current page" ) );
	AllPagesRadio->setText( tr( "&All pages" ) );
	IntervalPagesRadio->setText( tr( "&Range" ) );
	OkButton->setText( CommonStrings::tr_OK );
	CancelButton->setText( CommonStrings::tr_Cancel );
	CancelButton->setAccel( QKeySequence( tr( "C" ) ) );
	QToolTip::add( IntervalPagesRadio, tr( "Export a range of pages" ) );
	QToolTip::add( RangeVal, tr( "Insert a comma separated list of tokens where\na token can be * for all the pages, 1-5 for\na range of pages or a single page number." ) );
	QToolTip::add( AllPagesRadio, tr( "Export all pages" ) );
	QToolTip::add( OnePageRadio, tr( "Export only the current page" ) );
	QToolTip::add( DPIBox, tr( "Resolution of the Images\nUse 72 dpi for Images intended for the Screen" ) );
	QToolTip::add( EnlargementBox, tr( "Size of the images. 100% for no changes, 200% for two times larger etc." ));
	QToolTip::add( QualityBox, tr( "The quality of your images - 100% is the best, 1% the lowest quality" ) );
	QToolTip::add( BitmapType, tr( "Available export formats" ) );
	QToolTip::add( OutputDirectory, tr( "The output directory - the place to store your images.\nName of the export file will be 'documentname-pagenumber.filetype'" ) );
	QToolTip::add( OutputDirectoryButton, tr( "Change the output directory" ) );
}


void ExportForm::readConfig()
{
	DPIBox->setValue(prefs->getUInt("DPIBox", 72));
	EnlargementBox->setValue(prefs->getUInt("EnlargementBox", 100));
	QualityBox->setValue(prefs->getUInt("QualityBox", 100));
	ButtonGroup1->setButton(prefs->getUInt("ButtonGroup1", 0));
	if (prefs->getInt("ButtonGroup1")==2)
		RangeVal->setEnabled(true);
	else
		RangeVal->setEnabled(false);
	BitmapType->setCurrentItem(prefs->getInt("BitmapType", 4));
	RangeVal->setText(prefs->get("RangeVal", ""));
}

void ExportForm::writeConfig()
{
	prefs->set("DPIBox", DPIBox->value());
	prefs->set("EnlargementBox", EnlargementBox->value());
	prefs->set("QualityBox", QualityBox->value());
	prefs->set("ButtonGroup1", ButtonGroup1->id(ButtonGroup1->selected()));
	prefs->set("BitmapType",BitmapType->currentItem());
	prefs->set("RangeVal", RangeVal->text());

}
