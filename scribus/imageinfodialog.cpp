#include "imageinfodialog.h"
#include "imageinfodialog.moc"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

ImageInfoDialog::ImageInfoDialog( QWidget* parent, ScImage::ImageInfoRecord *info  )
		: QDialog( parent, "ImageInfo", true, 0 )

{
	setCaption( tr( "Image Info" ) );
	setIcon(loadIcon("AppIcon.png"));

	ImageInfoDialogLayout = new QVBoxLayout( this, 10, 5, "InsertTableLayout");
	GenGroup = new QGroupBox(this, "GenGroup");
	GenGroup->setTitle( tr("General Info"));
	GenGroup->setColumnLayout(0, Qt::Vertical);
	GenGroup->layout()->setSpacing(5);
	GenGroup->layout()->setMargin(5);
	layout3 = new QGridLayout(GenGroup->layout());
	Text1g = new QLabel( tr( "Has Embedded Profile:" ), GenGroup, "Text1g" );
	layout3->addWidget( Text1g, 0, 0 );
	emProfile = new QLabel( "", GenGroup, "emProfile" );
	layout3->addWidget( emProfile, 0, 1 );
	if (info->isEmbedded)
		emProfile->setText( tr("Yes"));
	else
		emProfile->setText( tr("No"));
	Text2g = new QLabel( tr( "Profile Name:" ), GenGroup, "Text2g" );
	layout3->addWidget( Text2g, 1, 0 );
	emProfileN = new QLabel( info->profileName, GenGroup, "emProfileN" );
	layout3->addWidget( emProfileN, 1, 1 );
	Text3g = new QLabel( tr( "Has Embedded Paths:" ), GenGroup, "Text3g" );
	layout3->addWidget( Text3g, 2, 0 );
	emPath = new QLabel( "", GenGroup, "emPath" );
	layout3->addWidget( emPath, 2, 1 );
	if (info->PDSpathData.count() != 0)
		emPath->setText( tr("Yes"));
	else
		emPath->setText( tr("No"));
	Text4g = new QLabel( tr( "Has Layers:" ), GenGroup, "Text3g" );
	layout3->addWidget( Text4g, 3, 0 );
	emLayer = new QLabel( "", GenGroup, "emLayer" );
	layout3->addWidget( emLayer, 3, 1 );
	if (info->layerInfo.count() != 0)
		emLayer->setText( tr("Yes"));
	else
		emLayer->setText( tr("No"));
	ImageInfoDialogLayout->addWidget(GenGroup);

	ExGroup = new QGroupBox(this, "ExGroup");
	ExGroup->setTitle( tr("EXIF Info"));
	ExGroup->setColumnLayout(0, Qt::Vertical);
	ExGroup->layout()->setSpacing(5);
	ExGroup->layout()->setMargin(5);
	layout2 = new QGridLayout(ExGroup->layout());
	Text1 = new QLabel( tr( "Comment:" ), ExGroup, "Text1" );
	layout2->addWidget( Text1, 0, 0 );
	Comment = new QLabel(info->exifInfo.comment, ExGroup, "Comment" );
	layout2->addWidget( Comment, 0, 1 );
	Text2 = new QLabel( tr( "Camera Model:" ), ExGroup, "Text2" );
	layout2->addWidget( Text2, 1, 0 );
	Model = new QLabel(info->exifInfo.cameraName, ExGroup, "Model" );
	layout2->addWidget( Model, 1, 1 );
	Text3 = new QLabel( tr( "Camera Manufacturer:" ), ExGroup, "Text3" );
	layout2->addWidget( Text3, 2, 0 );
	Manufact = new QLabel(info->exifInfo.cameraVendor, ExGroup, "Manufact" );
	layout2->addWidget( Manufact, 2, 1 );
	ImageInfoDialogLayout->addWidget(ExGroup);
	layout1 = new QHBoxLayout( 0, 0, 5, "layout1");
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton" );
	okButton->setDefault( true );
	layout1->addWidget( okButton );
	ImageInfoDialogLayout->addLayout( layout1 );
	resize(minimumSizeHint());

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
} 
