#include "imageinfodialog.h"
#include "imageinfodialog.moc"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

ImageInfoDialog::ImageInfoDialog( QWidget* parent, ScImage::ImageInfoRecord *info  ) : QDialog( parent, "ImageInfo", true, 0 )
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
	Text0g = new QLabel( tr( "Date / Time:" ), GenGroup, "Text0g" );
	layout3->addWidget( Text0g, 0, 0 );
	timeInfo = new QLabel( info->exifInfo.dateTime, GenGroup, "timeInfo" );
	layout3->addWidget( timeInfo, 0, 1 );
	Text1g = new QLabel( tr( "Has Embedded Profile:" ), GenGroup, "Text1g" );
	layout3->addWidget( Text1g, 1, 0 );
	emProfile = new QLabel( "", GenGroup, "emProfile" );
	layout3->addWidget( emProfile, 1, 1 );
	if (info->isEmbedded)
		emProfile->setText( tr("Yes"));
	else
		emProfile->setText( tr("No"));
	Text2g = new QLabel( tr( "Profile Name:" ), GenGroup, "Text2g" );
	layout3->addWidget( Text2g, 2, 0 );
	emProfileN = new QLabel( info->profileName, GenGroup, "emProfileN" );
	layout3->addWidget( emProfileN, 2, 1 );
	Text3g = new QLabel( tr( "Has Embedded Paths:" ), GenGroup, "Text3g" );
	layout3->addWidget( Text3g, 3, 0 );
	emPath = new QLabel( "", GenGroup, "emPath" );
	layout3->addWidget( emPath, 3, 1 );
	if (info->PDSpathData.count() != 0)
		emPath->setText( tr("Yes"));
	else
		emPath->setText( tr("No"));
	Text4g = new QLabel( tr( "Has Layers:" ), GenGroup, "Text3g" );
	layout3->addWidget( Text4g, 4, 0 );
	emLayer = new QLabel( "", GenGroup, "emLayer" );
	layout3->addWidget( emLayer, 4, 1 );
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
	Text1 = new QLabel( "", ExGroup, "Text1" );
	layout2->addWidget( Text1, 0, 0 );
	Comment = new QLabel(info->exifInfo.comment, ExGroup, "Comment" );
	layout2->addWidget( Comment, 0, 1 );
	Text4 = new QLabel( "", ExGroup, "Text4" );
	layout2->addWidget( Text4, 1, 0 );
	UserComment = new QLabel(info->exifInfo.userComment, ExGroup, "UserComment" );
	layout2->addWidget( UserComment, 1, 1 );
	int a = 2;
	if (info->typ == 1)
	{
		Text5 = new QLabel( tr("Artist:"), ExGroup, "Text5" );
		layout2->addWidget( Text5, 2, 0 );
		Artist = new QLabel(info->exifInfo.artist, ExGroup, "Artist" );
		layout2->addWidget( Artist, 2, 1 );
		a = 3;
	}
	Text2 = new QLabel( "", ExGroup, "Text2" );
	layout2->addWidget( Text2, a, 0 );
	Model = new QLabel(info->exifInfo.cameraName, ExGroup, "Model" );
	layout2->addWidget( Model, a, 1 );
	Text3 = new QLabel( "", ExGroup, "Text3" );
	layout2->addWidget( Text3, a+1, 0 );
	Manufact = new QLabel(info->exifInfo.cameraVendor, ExGroup, "Manufact" );
	layout2->addWidget( Manufact, a+1, 1 );
	ImageInfoDialogLayout->addWidget(ExGroup);
	switch (info->typ)
	{
		case 0:
			Text1->setText( tr( "Comment:" ));
			Text4->setText( tr( "User Comment:" ));
			Text2->setText( tr( "Camera Model:" ));
			Text3->setText( tr( "Camera Manufacturer:" ));
			break;
		case 1:
			Text1->setText( tr( "Description:" ));
			Text4->setText( tr( "Copyright:" ));
			Text2->setText( tr( "Scanner Model:" ));
			Text3->setText( tr( "Scanner Manufacturer:" ));
			break;
		default:
			ExGroup->hide();
			break;
	}
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
