#include "imageinfodialog.h"
#include "imageinfodialog.moc"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

ImageInfoDialog::ImageInfoDialog( QWidget* parent, ScImage::ExifValues *info  )
		: QDialog( parent, "ImageInfo", true, 0 )

{
	setCaption( tr( "Image Info" ) );
	setIcon(loadIcon("AppIcon.png"));

	ImageInfoDialogLayout = new QVBoxLayout( this, 10, 5, "InsertTableLayout");
	layout2 = new QGridLayout( 0, 1, 1, 0, 5, "layout2");
	Text1 = new QLabel( tr( "Comment:" ), this, "Text1" );
	layout2->addWidget( Text1, 0, 0 );
	Comment = new QLabel(info->comment, this, "Comment" );
	layout2->addWidget( Comment, 0, 1 );
	Text2 = new QLabel( tr( "Camera Model:" ), this, "Text2" );
	layout2->addWidget( Text2, 1, 0 );
	Model = new QLabel(info->cameraName, this, "Model" );
	layout2->addWidget( Model, 1, 1 );
	Text3 = new QLabel( tr( "Camera Manufacturer:" ), this, "Text3" );
	layout2->addWidget( Text3, 2, 0 );
	Manufact = new QLabel(info->cameraVendor, this, "Manufact" );
	layout2->addWidget( Manufact, 2, 1 );
	ImageInfoDialogLayout->addLayout( layout2 );
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
