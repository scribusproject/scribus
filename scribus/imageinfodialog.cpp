/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "imageinfodialog.h"
#include "imageinfodialog.moc"
#include "commonstrings.h"
//Added by qt3to4:
#include <QPixmap>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

extern QPixmap loadIcon(QString nam);

ImageInfoDialog::ImageInfoDialog( QWidget* parent, ImageInfoRecord *info  ) : QDialog( parent, "ImageInfo", true, 0 )
{
	setCaption( tr( "Image Info" ) );
	setIcon(loadIcon("AppIcon.png"));

	ImageInfoDialogLayout = new Q3VBoxLayout( this, 10, 5, "InsertTableLayout");
	GenGroup = new Q3GroupBox(this, "GenGroup");
	GenGroup->setTitle( tr("General Info"));
	GenGroup->setColumnLayout(0, Qt::Vertical);
	GenGroup->layout()->setSpacing(5);
	GenGroup->layout()->setMargin(5);
	layout3 = new Q3GridLayout(GenGroup->layout());
	Text0g = new QLabel( tr( "Date / Time:" ), GenGroup, "Text0g" );
	layout3->addWidget( Text0g, 0, 0 );
	timeInfo = new QLabel( info->exifInfo.dateTime, GenGroup, "timeInfo" );
	layout3->addWidget( timeInfo, 0, 1 );
	Text1g = new QLabel( tr( "Has Embedded Profile:" ), GenGroup, "Text1g" );
	layout3->addWidget( Text1g, 1, 0 );
	emProfile = new QLabel( "", GenGroup, "emProfile" );
	layout3->addWidget( emProfile, 1, 1 );
	if (info->isEmbedded)
		emProfile->setText(CommonStrings::trYes);
	else
		emProfile->setText(CommonStrings::trNo);
	Text2g = new QLabel( tr( "Profile Name:" ), GenGroup, "Text2g" );
	layout3->addWidget( Text2g, 2, 0 );
	emProfileN = new QLabel( info->profileName, GenGroup, "emProfileN" );
	layout3->addWidget( emProfileN, 2, 1 );
	Text3g = new QLabel( tr( "Has Embedded Paths:" ), GenGroup, "Text3g" );
	layout3->addWidget( Text3g, 3, 0 );
	emPath = new QLabel( "", GenGroup, "emPath" );
	layout3->addWidget( emPath, 3, 1 );
	if (info->PDSpathData.count() != 0)
		emPath->setText(CommonStrings::trYes);
	else
		emPath->setText(CommonStrings::trNo);
	Text4g = new QLabel( tr( "Has Layers:" ), GenGroup, "Text3g" );
	layout3->addWidget( Text4g, 4, 0 );
	emLayer = new QLabel( "", GenGroup, "emLayer" );
	layout3->addWidget( emLayer, 4, 1 );
	if (info->layerInfo.count() != 0)
		emLayer->setText(CommonStrings::trYes);
	else
		emLayer->setText(CommonStrings::trNo);
	ImageInfoDialogLayout->addWidget(GenGroup);

	ExGroup = new Q3GroupBox(this, "ExGroup");
	ExGroup->setTitle( tr("EXIF Info"));
	ExGroup->setColumnLayout(0, Qt::Vertical);
	ExGroup->layout()->setSpacing(5);
	ExGroup->layout()->setMargin(5);
	layout2 = new Q3GridLayout(ExGroup->layout());
	Text1 = new QLabel( "", ExGroup, "Text1" );
	layout2->addWidget( Text1, 0, 0 );
	Comment = new QLabel(info->exifInfo.comment, ExGroup, "Comment" );
	layout2->addWidget( Comment, 0, 1 );
	Text4 = new QLabel( "", ExGroup, "Text4" );
	layout2->addWidget( Text4, 1, 0 );
	UserComment = new QLabel(info->exifInfo.userComment, ExGroup, "UserComment" );
	layout2->addWidget( UserComment, 1, 1 );
	int a = 2;
	if (info->type == 1)
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

	Text6 = new QLabel( "", ExGroup, "Text6" );
	layout2->addWidget( Text6, a+2, 0 );
	QString tag = "";
	if (info->exifInfo.ExposureTime != 0)
	{
		float exposureTime = info->exifInfo.ExposureTime;
		tag = QString().sprintf("%6.3f", exposureTime);
		if ((exposureTime > 0.0) && (exposureTime <= 0.5))
			tag += QString().sprintf(" (1/%d)", (int)(0.5 + 1/exposureTime) );
	}
	if (info->type == 0)
	{
		ExposureTime = new QLabel(tag, ExGroup, "ExposureTime" );
		layout2->addWidget( ExposureTime, a+2, 1 );
		Text7 = new QLabel( "", ExGroup, "Text7" );
		layout2->addWidget( Text7, a+3, 0 );
		ApertureFNumber = new QLabel(QString().sprintf("f/%3.1f", info->exifInfo.ApertureFNumber), ExGroup, "ApertureFNumber" );
		layout2->addWidget( ApertureFNumber, a+3, 1 );
		Text8 = new QLabel( "", ExGroup, "Text8" );
		layout2->addWidget( Text8, a+4, 0 );
		ISOequivalent = new QLabel(QString().sprintf("%2d", info->exifInfo.ISOequivalent), ExGroup, "ISOequivalent" );
		layout2->addWidget( ISOequivalent, a+4, 1 );
	}
	ImageInfoDialogLayout->addWidget(ExGroup);
	switch (info->type)
	{
		case 0:
			Text1->setText( tr( "Comment:" ));
			Text4->setText( tr( "User Comment:" ));
			Text2->setText( tr( "Camera Model:" ));
			Text3->setText( tr( "Camera Manufacturer:" ));
			Text6->setText( tr( "Exposure time" ));
			Text7->setText( tr( "Aperture:" ));
			Text8->setText( tr( "ISO equiv.:" ));
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
	layout1 = new Q3HBoxLayout( 0, 0, 5, "layout1");
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
