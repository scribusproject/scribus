#include "modify.h"
#include "modify.moc"
#include "config.h"
#include <qmessagebox.h>
#ifdef HAVE_CMS
	#include CMS_INC
extern bool SoftProofing;
extern bool Gamut;
extern bool CMSuse;
#endif
extern bool CMSavail;
extern float UmReFaktor;
extern QPixmap loadIcon(QString nam);

NameWidget::NameWidget(QWidget* parent) : QLineEdit(parent)
{
}

void NameWidget::focusOutEvent(QFocusEvent *)
{
	emit Leaved();
}

ModObj::ModObj( QWidget* parent, PageItem *b, CListe Farben, ScribusDoc *docu, ScribusView *vie, ProfilesL *InputProfiles)
				: QDialog( parent, "Mod", true, 0 )
{
  	setIcon(loadIcon("AppIcon.xpm"));
    CurMod = b;
    doc = docu;
		view = vie;
    bool embed = false;
    ModObjLayout = new QVBoxLayout( this );
    ModObjLayout->setSpacing( 6 );
    ModObjLayout->setMargin( 0 );
    Layout5 = new QGridLayout;
    Layout5->setSpacing( 6 );
    Layout5->setMargin( 10 );
    GroupSpecial = new QGroupBox( this, "GroupSpecial" );
    GroupSpecial->setTitle( tr( "Other" ) );
    GroupSpecial->setColumnLayout(0, Qt::Vertical );
    GroupSpecial->layout()->setSpacing( 0 );
    GroupSpecial->layout()->setMargin( 0 );
    GroupSpecialLayout = new QHBoxLayout( GroupSpecial->layout() );
    GroupSpecialLayout->setAlignment( Qt::AlignTop );
    GroupSpecialLayout->setSpacing( 6 );
    GroupSpecialLayout->setMargin( 10 );
    Layout2 = new QVBoxLayout;
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );
    FlippedH = new QCheckBox( GroupSpecial, "FlippedH" );
    FlippedH->setMinimumSize( QSize( 150, 22 ) );
    FlippedH->setText( tr( "Horizontal flipped" ) );
		FlippedH->setChecked(b->flippedH % 2 != 0);
    Layout2->addWidget( FlippedH );
    FlippedV = new QCheckBox( GroupSpecial, "FlippedV" );
    FlippedV->setMinimumSize( QSize( 150, 22 ) );
    FlippedV->setText( tr( "Vertical flipped" ) );
		FlippedV->setChecked(b->flippedV % 2 != 0);
    Layout2->addWidget( FlippedV );
    Textflow = new QCheckBox( GroupSpecial, "Textflow" );
    Textflow->setMinimumSize( QSize( 150, 22 ) );
    Textflow->setText( tr( "Text flows around Box" ) );
    Textflow->setChecked(b->Textflow);
    Layout2->addWidget( Textflow );
    Textflow2 = new QCheckBox( GroupSpecial, "Textflow2" );
    Textflow2->setMinimumSize( QSize( 150, 22 ) );
    Textflow2->setText( tr( "Use Bounding Box" ) );
    Textflow2->setChecked(b->Textflow2);
    Layout2->addWidget( Textflow2 );
		ToggleFlow();
    Printable = new QCheckBox( GroupSpecial, "Printable" );
    Printable->setMinimumSize( QSize( 150, 22 ) );
    Printable->setText( tr( "Don't print" ) );
    Printable->setChecked(!b->isPrintable);
    Layout2->addWidget( Printable );
    GroupSpecialLayout->addLayout( Layout2 );
    Layout5->addWidget( GroupSpecial, 1, 0 );

    GroupPos = new QGroupBox( this, "GroupPos" );
    GroupPos->setTitle( tr( "Position" ) );
    GroupPos->setColumnLayout(0, Qt::Vertical );
    GroupPos->layout()->setSpacing( 0 );
    GroupPos->layout()->setMargin( 0 );
    GroupPosLayout = new QHBoxLayout( GroupPos->layout() );
    GroupPosLayout->setAlignment( Qt::AlignTop );
    GroupPosLayout->setSpacing( 6 );
    GroupPosLayout->setMargin( 11 );
    Layout1 = new QGridLayout;
    Layout1->setSpacing( 3 );
    Layout1->setMargin( 0 );
    TextLabel1 = new QLabel( GroupPos, "TextLabel1" );
    TextLabel1->setText( tr( "X-Pos:" ) );
    Layout1->addWidget( TextLabel1, 0, 0 );
    TextLabel1_2 = new QLabel( GroupPos, "TextLabel1_2" );
    TextLabel1_2->setText( tr( "Y-Pos:" ) );
    Layout1->addWidget( TextLabel1_2, 1, 0 );
    TextLabel1_2_2 = new QLabel( GroupPos, "TextLabel1_2_2" );
    TextLabel1_2_2->setText( tr( "Width:" ) );
    Layout1->addWidget( TextLabel1_2_2, 2, 0 );
    TextLabel1_2_2_2 = new QLabel( GroupPos, "TextLabel1_2_2_2" );
    TextLabel1_2_2_2->setText( tr( "Height:" ) );
    Layout1->addWidget( TextLabel1_2_2_2, 3, 0 );
    TextLabel1_2_2_2_2 = new QLabel( GroupPos, "TextLabel1_2_2_2_2" );
    TextLabel1_2_2_2_2->setText( tr( "Angle:" ) );
    Layout1->addWidget( TextLabel1_2_2_2_2, 4, 0 );
    Xpos = new MSpinBox(GroupPos, 2);
    Xpos->setMinimumSize( QSize( 85, 22 ) );
    Xpos->setSuffix( tr( " pt" ) );
    Xpos->setMaxValue( 300000 );
    Xpos->setMinValue( -300000 );
    Xpos->setValue(qRound(b->Xpos*UmReFaktor*100));
    Xpos->setLineStep(100);
    Layout1->addWidget( Xpos, 0, 1 );
		Ypos = new MSpinBox( GroupPos, 2);
		Ypos->setMinimumSize( QSize( 85, 22 ) );
    Ypos->setSuffix( tr( " pt" ) );
    Ypos->setMaxValue( 300000 );
    Ypos->setMinValue( -300000 );
    Ypos->setValue(qRound(b->Ypos*UmReFaktor*100));
    Ypos->setLineStep(100);
    Layout1->addWidget( Ypos, 1, 1 );
    Width = new MSpinBox( GroupPos, 2 );
    Width->setMinimumSize( QSize( 85, 22 ) );
    Width->setSuffix( tr( " pt" ) );
    Width->setMaxValue( 300000 );
    Width->setMinValue( 0 );
    origWidth = qRound(b->Width*UmReFaktor*100);
    Width->setValue(origWidth);
    Width->setLineStep(100);
    Layout1->addWidget( Width, 2, 1 );
    Height = new MSpinBox( GroupPos, 2 );
    Height->setMinimumSize( QSize( 85, 22 ) );
    Height->setSuffix( tr( " pt" ) );
    Height->setMaxValue( 300000 );
    Height->setMinValue( 0 );
    origHeight = qRound(b->Height*UmReFaktor*100);
    Height->setValue(origHeight);
    Height->setLineStep(100);
    Layout1->addWidget( Height, 3, 1 );
    Angle = new MSpinBox( GroupPos, 2 );
    Angle->setMinimumSize( QSize( 85, 22 ) );
    Angle->setSuffix(" °");
    Angle->setMaxValue( 300000 );
    Angle->setMinValue( -300000 );
    Angle->setValue(qRound(b->Rot*-100));
    Angle->setLineStep(100);
    Layout1->addWidget( Angle, 4, 1 );
    GroupPosLayout->addLayout( Layout1 );
    Layout5->addWidget( GroupPos, 0, 0 );
    Props = new QButtonGroup( this, "Props" );
    Props->setTitle( tr( "Properties" ) );
    Props->setColumnLayout(0, Qt::Vertical );
    Props->layout()->setSpacing( 0 );
    Props->layout()->setMargin( 0 );
    PropsLayout = new QHBoxLayout( Props->layout() );
    PropsLayout->setAlignment( Qt::AlignTop );
    PropsLayout->setSpacing( 6 );
    PropsLayout->setMargin( 11 );
    Layout4 = new QGridLayout;
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );
		CListe::Iterator it;
		ProfilesL::Iterator itP;
		QPixmap pm;
		Height->setEnabled(true);
    QSpacerItem* spacerSC1 = new QSpacerItem( 30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    QSpacerItem* spacerSC2 = new QSpacerItem( 30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    switch (b->PType)
    	{
    	case 8:
    		setCaption(tr("Modify Pathtext"));
    		NormText2 = new QCheckBox( Props, "NormText" );
    		NormText2->setMinimumSize(QSize(140, 22));
    		NormText2->setText( tr( "Show Curve" ) );
    		Layout4->addMultiCellWidget( NormText2, 0, 0, 0, 1 );
    		NormText2->setChecked(b->PoShow);
    		TextLabel3 = new QLabel( Props, "TextLabel3" );
    		TextLabel3->setMinimumSize(QSize(110, 22));
    		TextLabel3->setText( tr( "Startoffset:" ) );
    		Layout4->addWidget( TextLabel3, 1, 0);
    		Distance = new MSpinBox( Props, 1 );
    		Distance->setMinimumSize(QSize(65, 22));
    		Distance->setSuffix( tr( " pts" ) );
    		Distance->setMaxValue( 3000 );
    		Distance->setMinValue( 0 );
    		Distance->setValue(static_cast<int>(b->Extra*10));
    		Distance->setLineStep(10);
    		Layout4->addWidget( Distance, 1, 1);
    		TextLabel2 = new QLabel( Props, "TextLabel2" );
    		TextLabel2->setMinimumSize(QSize(110, 22));
    		TextLabel2->setText( tr( "Distance from Curve:" ) );
    		Layout4->addWidget( TextLabel2, 2, 0);
    		LineW = new MSpinBox( Props, 1 );
    		LineW->setMinimumSize( QSize( 65, 22 ) );
    		LineW->setSuffix( tr( " pts" ) );
    		LineW->setMaxValue( 3000 );
    		LineW->setMinValue( -3000 );
				LineW->setLineStep(10);
    		LineW->setValue( qRound(b->BaseOffs * -10) );
    		Layout4->addWidget( LineW, 2, 1);
    		break;
    	case 4:
    		setCaption(tr("Modify Textframe"));
    		NormText = new QRadioButton( Props, "NormText" );
    		NormText->setMinimumSize(QSize(140, 22));
    		NormText->setText( tr( "Ordinary Text" ) );
    		Layout4->addMultiCellWidget( NormText, 0, 0, 0, 1 );
    		Bookmark = new QRadioButton( Props, "Bookmark" );
    		Bookmark->setMinimumSize(QSize(140, 22));
    		Bookmark->setText( tr( "Is PDF-Bookmark" ) );
    		Layout4->addMultiCellWidget( Bookmark, 1, 1, 0, 1 );
    		Annotation = new QRadioButton( Props, "Annotation" );
    		Annotation->setMinimumSize(QSize(140, 22));
    		Annotation->setText( tr( "Is PDF-Annotation" ) );
    		Layout4->addMultiCellWidget( Annotation, 2, 2, 0, 1 );
    		NormText->setChecked(true);
				if (b->isBookmark)
					{
    			Bookmark->setChecked(true);
					}
				if (b->isAnnotation)
					{
    			Annotation->setChecked(true);
					}
    		TextLabel3 = new QLabel( Props, "TextLabel3" );
    		TextLabel3->setMinimumSize(QSize(110, 22));
    		TextLabel3->setText( tr( "Textdistance:" ) );
    		Layout4->addWidget( TextLabel3, 3, 0);
    		TextLabel1_3 = new QLabel( Props, "TextLabel1_3" );
    		TextLabel1_3->setMinimumSize(QSize(110, 22));
    		TextLabel1_3->setText( tr( "Background Color:" ) );
    		Layout4->addWidget( TextLabel1_3, 4, 0);
    		TextLabel2 = new QLabel( Props, "TextLabel2" );
    		TextLabel2->setMinimumSize(QSize(110, 22));
    		TextLabel2->setText( tr( "Shading:" ) );
    		Layout4->addWidget( TextLabel2, 5, 0);
    		Distance = new MSpinBox( Props, 1 );
    		Distance->setMinimumSize(QSize(65, 22));
    		Distance->setSuffix( tr( " pt" ) );
    		Distance->setMaxValue( 3000 );
    		Distance->setMinValue( 0 );
    		Distance->setValue(static_cast<int>(b->Extra*10));
    		Distance->setLineStep(10);
    		Layout4->addWidget( Distance, 3, 1, Qt::AlignLeft);
    		Background = new QComboBox( true, Props, "Background" );
    		Background->setMinimumSize(QSize(95, 21));
    		Background->setEditable(false);
    		Background->insertItem(tr("None"));
    		if (b->Pcolor == "None")
    			Background->setCurrentItem(Background->count()-1);
    		pm = QPixmap(15, 15);
				for (it = Farben.begin(); it != Farben.end(); ++it)
					{
					pm.fill(Farben[it.key()].getRGBColor());
					Background->insertItem(pm, it.key());
    			if (it.key() == b->Pcolor)
    				{
    				Background->setCurrentItem(Background->count()-1);
    				}
					}
    		Layout4->addWidget( Background, 4, 1);
    		Shade = new QSpinBox( Props, "Distance_2" );
    		Shade->setMinimumSize(QSize(65, 22));
    		Shade->setSuffix( tr( " %" ) );
    		Shade->setMaxValue( 100 );
    		Shade->setMinValue( 0 );
    		Shade->setValue( b->Shade );
    		Layout4->addWidget( Shade, 5, 1, Qt::AlignLeft);
		    TextLabel2_3b = new QLabel( Props, "TextLabel2_3b" );
    		TextLabel2_3b->setText( tr( "Corner Radius:" ) );
    		Layout4->addWidget( TextLabel2_3b, 6, 0 );
    		Radius = new QSpinBox(Props, "RR");
    		Radius->setMinimumSize( QSize( 65, 22 ) );
    		Radius->setSuffix( tr( " pt" ) );
   			Radius->setMaxValue(QMIN(static_cast<int>(b->Width),static_cast<int>(b->Height))/2 );
    		Radius->setMinValue(QMIN(static_cast<int>(b->Width),static_cast<int>(b->Height))/2 * -1);
    		Radius->setValue(qRound(b->RadRect));
    		if (b->ClipEdited)
    			Radius->setEnabled(false);
    		Layout4->addWidget( Radius, 6, 1, Qt::AlignLeft );
    		break;
    	case 2:
    		setCaption(tr("Modify Pictureframe"));

    		ScaleGroup = new QButtonGroup( Props, "ScaleGroup" );
    		ScaleGroup->setTitle( tr( "Scaling" ) );
    		ScaleGroup->setExclusive( false );
    		ScaleGroup->setRadioButtonExclusive(true);
    		ScaleGroup->setColumnLayout(0, Qt::Vertical );
    		ScaleGroup->layout()->setSpacing( 5 );
    		ScaleGroup->layout()->setMargin( 10 );
    		ScaleGroupLayout = new QVBoxLayout( ScaleGroup->layout() );
    		ScaleGroupLayout->setAlignment( Qt::AlignTop );
    		FreeScale = new QRadioButton( ScaleGroup, "FreeScale" );
    		FreeScale->setText( tr( "Free Scaling" ) );
    		ScaleGroupLayout->addWidget( FreeScale );
    		ScaleLayout2 = new QHBoxLayout( 0, 0, 5, "ScaleLayout2");
   		 	ScaleLayout2->addItem( spacerSC1 );
    		ScaleLayout1 = new QGridLayout( 0, 1, 1, 0, 5, "ScaleLayout1");
		    TextLabel3_2 = new QLabel( ScaleGroup, "TextLabel3_2" );
		    TextLabel3_2->setMinimumSize(QSize(110, 22));
    		TextLabel3_2->setText( tr( "Local X-Pos:" ) );
    		ScaleLayout1->addWidget( TextLabel3_2, 0, 0);
		    TextLabel3_2_2 = new QLabel( ScaleGroup, "TextLabel3_2_2" );
		    TextLabel3_2_2->setMinimumSize(QSize(110, 22));
    		TextLabel3_2_2->setText( tr( "Local Y-Pos:" ) );
    		ScaleLayout1->addWidget( TextLabel3_2_2, 1, 0);
				TextLabel3_2_2_2 = new QLabel( ScaleGroup, "TextLabel3_2_2_2" );
				TextLabel3_2_2_2->setMinimumSize(QSize(110, 22));
    		TextLabel3_2_2_2->setText( tr( "Horizontal Scaling" ) );
    		ScaleLayout1->addWidget( TextLabel3_2_2_2, 2, 0);
    		TextLabel3_2_2_2_2 = new QLabel( ScaleGroup, "TextLabel3_2_2_2_2" );
    		TextLabel3_2_2_2_2->setMinimumSize(QSize(110, 22));
    		TextLabel3_2_2_2_2->setText( tr( "Vertical Scaling" ) );
    		ScaleLayout1->addWidget( TextLabel3_2_2_2_2, 3, 0);
    		LXpos = new MSpinBox( ScaleGroup, 2 );
    		LXpos->setMinimumSize(QSize(90, 22));
    		LXpos->setSuffix( tr( " pt" ) );
    		LXpos->setMaxValue( 300000 );
    		LXpos->setMinValue( -300000 );
    		LXpos->setLineStep(100);
    		LXpos->setValue(qRound(b->LocalX*UmReFaktor*100));
    		ScaleLayout1->addWidget(LXpos, 0, 1);
    		LYpos = new MSpinBox( ScaleGroup, 2 );
    		LYpos->setMinimumSize(QSize(90, 22));
    		LYpos->setSuffix( tr( " pt" ) );
    		LYpos->setMaxValue( 300000 );
    		LYpos->setMinValue( -300000 );
    		LYpos->setLineStep(100);
    		LYpos->setValue(qRound(b->LocalY*UmReFaktor*100));
    		ScaleLayout1->addWidget(LYpos, 1, 1);
    		HScale = new MSpinBox( ScaleGroup, 1 );
    		HScale->setMinimumSize(QSize(70, 22));
    		HScale->setSuffix( tr( " %" ) );
    		HScale->setMaxValue( 10000 );
    		HScale->setMinValue( 1 );
    		HScale->setLineStep(10);
    		HScale->setValue( static_cast<int>(b->LocalScX*1000) );
    		ScaleLayout1->addWidget(HScale, 2, 1);
    		VScale = new MSpinBox( ScaleGroup, 1 );
    		VScale->setMinimumSize(QSize(70, 22));
    		VScale->setSuffix( tr( " %" ) );
    		VScale->setMaxValue( 10000 );
    		VScale->setMinValue( 1 );
    		VScale->setLineStep(10);
   			VScale->setValue( static_cast<int>(b->LocalScY*1000) );
    		ScaleLayout1->addWidget(VScale, 3, 1);
    		Kette = new LinkButton( ScaleGroup );
    		Kette->setMinimumSize(QSize(16, 50));
    		Kette->setToggleButton( true );
    		Kette->setAutoRaise(true);
   			ScaleLayout1->addMultiCellWidget(Kette, 2, 3, 2, 2, Qt::AlignLeft);
    		ScaleLayout2->addLayout( ScaleLayout1 );
    		ScaleGroupLayout->addLayout( ScaleLayout2 );
    		FrameScale = new QRadioButton( ScaleGroup, "FrameScale" );
    		FrameScale->setText( tr( "Scale Picture to Frame Size" ) );
    		ScaleGroupLayout->addWidget( FrameScale );
    		ScaleLayout3 = new QHBoxLayout( 0, 0, 6, "ScaleLayout3");
    		ScaleLayout3->addItem( spacerSC2 );
    		Aspect = new QCheckBox( ScaleGroup, "Aspect" );
    		Aspect->setText( tr( "Keep Aspect Ratio" ) );
				Aspect->setChecked(b->AspectRatio);
    		ScaleLayout3->addWidget( Aspect );
    		ScaleGroupLayout->addLayout( ScaleLayout3 );
   			Layout4->addMultiCellWidget(ScaleGroup, 0, 0, 0, 2);
				if (b->ScaleType)
					{
					FreeScale->setChecked(true);
					FrameScale->setChecked(false);
					}
				else
					{
					FreeScale->setChecked(false);
					FrameScale->setChecked(true);
					}
    		Visible = new QCheckBox( Props, "Visible" );
    		Visible->setText( tr( "Image visible" ) );
    		Visible->setChecked(b->PicArt);
   			Layout4->addMultiCellWidget(Visible, 4, 4, 0, 2);
    		TextLabel1_3 = new QLabel( Props, "TextLabel1_3" );
    		TextLabel1_3->setMinimumSize(QSize(110, 22));
    		TextLabel1_3->setText( tr( "Background Color:" ) );
    		Layout4->addWidget( TextLabel1_3, 5, 0);
    		TextLabel2 = new QLabel( Props, "TextLabel2" );
    		TextLabel2->setMinimumSize(QSize(110, 22));
    		TextLabel2->setText( tr( "Shading:" ) );
    		Layout4->addWidget( TextLabel2, 6, 0);
    		Background = new QComboBox( true, Props, "Background" );
    		Background->setMinimumSize(QSize(95,21));
    		Background->setEditable(false);
    		pm = QPixmap(15, 15);
    		Background->insertItem(tr("None"));
    		if (b->Pcolor == "None")
    			Background->setCurrentItem(Background->count()-1);
				for (it = Farben.begin(); it != Farben.end(); ++it)
					{
					pm.fill(Farben[it.key()].getRGBColor());
					Background->insertItem(pm, it.key());
    			if (it.key() == b->Pcolor)
    				{
    				Background->setCurrentItem(Background->count()-1);
    				}
					}
   			Layout4->addMultiCellWidget(Background, 5, 5, 1, 2);
    		Shade = new QSpinBox( Props, "Shade" );
    		Shade->setMinimumSize(QSize(65, 22));
    		Shade->setSuffix( tr( " %" ) );
    		Shade->setMaxValue( 100 );
    		Shade->setMinValue( 0 );
    		Shade->setValue( b->Shade );
   			Layout4->addMultiCellWidget(Shade, 6, 6, 1, 2, Qt::AlignLeft);
		    TextLabel2_3b = new QLabel( Props, "TextLabel2_3b" );
    		TextLabel2_3b->setText( tr( "Corner Radius:" ) );
    		Layout4->addWidget( TextLabel2_3b, 7, 0 );
    		Radius = new QSpinBox(Props, "RR");
    		Radius->setMinimumSize( QSize( 65, 22 ) );
    		Radius->setSuffix( tr( " pt" ) );
   			Radius->setMaxValue(QMIN(static_cast<int>(b->Width),static_cast<int>(b->Height))/2 );
    		Radius->setMinValue(QMIN(static_cast<int>(b->Width),static_cast<int>(b->Height))/2 * -1);
    		Radius->setValue(qRound(b->RadRect));
    		if (b->ClipEdited)
    			Radius->setEnabled(false);
    		Layout4->addWidget( Radius, 7, 1, Qt::AlignLeft );
#ifdef HAVE_CMS
				if ((CMSuse) && (CMSavail))
					{
    			Text1 = new QLabel( Props, "Text1" );
    			Text1->setText( tr( "Input Profile:" ) );
					Layout4->addWidget( Text1, 8, 0 );
    			InputP = new QComboBox( true, Props, "InputP" );
    			InputP->setMinimumSize( QSize( 190, 22 ) );
    			InputP->setEditable(false);
					for (itP = InputProfiles->begin(); itP != InputProfiles->end(); ++itP)
						{
						InputP->insertItem(itP.key());
    				if (itP.key() == b->IProfile)
    					{
    					InputP->setCurrentItem(InputP->count()-1);
    					}
						}
					if (!InputProfiles->contains(b->IProfile))
						{
						InputP->insertItem(b->IProfile);
    				InputP->setCurrentItem(InputP->count()-1);
    				embed = true;
						}
					else
						{
						if (b->EmProfile != "")
							InputP->insertItem(b->EmProfile);
						embed = false;
						}
					EmbedP = embed;
   				Layout4->addMultiCellWidget(InputP, 8, 8, 1, 3, Qt::AlignLeft);
    			Text2 = new QLabel( Props, "Text2" );
    			Text2->setText( tr( "Rendering Intent:" ) );
					Layout4->addWidget( Text2, 9, 0 );
    			MonitorI = new QComboBox( true, Props, "MonitorI" );
    			MonitorI->insertItem( tr( "Perceptual" ) );
    			MonitorI->insertItem( tr( "Relative Colorimetric" ) );
    			MonitorI->insertItem( tr( "Saturation" ) );
    			MonitorI->insertItem( tr( "Absolute Colorimetric" ) );
    			MonitorI->setMinimumSize( QSize( 190, 22 ) );
    			MonitorI->setEditable(false);
    			MonitorI->setCurrentItem(b->IRender);
   				Layout4->addMultiCellWidget(MonitorI, 9, 9, 1, 3, Qt::AlignLeft);
    			connect(InputP, SIGNAL(activated(const QString&)), this, SLOT(EmProf(const QString&)));
   				}
#endif				
    		connect(Kette, SIGNAL(clicked()), this, SLOT(ToggleKette()));
    		connect(HScale, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
    		connect(VScale, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
				ChangeScaling();
    		break;
    	default:
    		setCaption( tr( "Modify Frame" ) );
    		TextLabel1_3_2 = new QLabel( Props, "TextLabel1_3_2" );
    		TextLabel1_3_2->setText( tr( "Line Color:" ) );
    		Layout4->addWidget( TextLabel1_3_2, 2, 0 );
    		TextLabel2_2 = new QLabel( Props, "TextLabel2_2" );
    		TextLabel2_2->setText( tr( "Shading:" ) );
    		Layout4->addWidget( TextLabel2_2, 3, 0 );
		    TextLabel1_3 = new QLabel( Props, "TextLabel1_3" );
    		TextLabel1_3->setText( tr( "Fill Color:" ) );
    		Layout4->addWidget( TextLabel1_3, 0, 0 );
		    TextLabel1_3_3 = new QLabel( Props, "TextLabel1_3_3" );
    		TextLabel1_3_3->setText( tr( "Linestyle:" ) );
    		Layout4->addWidget( TextLabel1_3_3, 4, 0 );
		    TextLabel2 = new QLabel( Props, "TextLabel2" );
    		TextLabel2->setText( tr( "Shading:" ) );
    		Layout4->addWidget( TextLabel2, 1, 0 );
		    TextLabel2_3 = new QLabel( Props, "TextLabel2_3" );
    		TextLabel2_3->setText( tr( "Linewidth:" ) );
    		Layout4->addWidget( TextLabel2_3, 5, 0 );
    		Foreground = new QComboBox( true, Props, "ComboBox1" );
    		Foreground->setMinimumSize( QSize( 95, 21 ) );
    		Foreground->setEditable(false);
    		pm = QPixmap(15, 15);
		    if ((b->PType != 5) || (b->PType != 7))
		    	{
    			Foreground->insertItem(tr("None"));
    			if (b->Pcolor2 == "None")
    				Foreground->setCurrentItem(Foreground->count()-1);
    			}
				for (it = Farben.begin(); it != Farben.end(); ++it)
					{
					pm.fill(Farben[it.key()].getRGBColor());
					Foreground->insertItem(pm, it.key());
    			if (it.key() == b->Pcolor2)
    				Foreground->setCurrentItem(Foreground->count()-1);
					}
    		Layout4->addWidget( Foreground, 2, 1 );
    		Shade2 = new QSpinBox( Props, "Shade2" );
    		Shade2->setMinimumSize( QSize( 65, 22 ) );
    		Shade2->setSuffix( tr( " %" ) );
    		Shade2->setMaxValue( 100 );
    		Shade2->setMinValue( 0 );
    		Shade2->setValue( b->Shade2 );
    		Layout4->addWidget( Shade2, 3, 1, Qt::AlignLeft );
    		Background = new QComboBox( true, Props, "Background" );
    		Background->setMinimumSize( QSize( 95, 21 ) );
    		Background->setEditable(false);
    		pm = QPixmap(15, 15);
    		Background->insertItem(tr("None"));
    		if (b->Pcolor == "None")
    			Background->setCurrentItem(Background->count()-1);
				for (it = Farben.begin(); it != Farben.end(); ++it)
					{
					pm.fill(Farben[it.key()].getRGBColor());
					Background->insertItem(pm, it.key());
    			if (it.key() == b->Pcolor)
    				{
    				Background->setCurrentItem(Background->count()-1);
    				}
					}
    		Layout4->addWidget( Background, 0, 1 );
    		Shade = new QSpinBox( Props, "Shade" );
    		Shade->setMinimumSize( QSize( 65, 22 ) );
    		Shade->setSuffix( tr( " %" ) );
    		Shade->setMaxValue( 100 );
    		Shade->setMinValue( 0 );
    		Shade->setValue( b->Shade );
    		Layout4->addWidget( Shade, 1, 1, Qt::AlignLeft );
    		Linestyle = new LineCombo(Props);
    		Linestyle->setMinimumSize( QSize( 95, 21 ) );
				switch (b->PLineArt)
					{
					case SolidLine:
						Linestyle->setCurrentItem(0);
						break;
					case DashLine:
						Linestyle->setCurrentItem(1);
						break;
					case DotLine:
						Linestyle->setCurrentItem(2);
						break;
					case DashDotLine:
						Linestyle->setCurrentItem(3);
						break;
					case DashDotDotLine:
						Linestyle->setCurrentItem(4);
						break;
					default:
						Linestyle->setCurrentItem(0);
						break;
					}
    		Layout4->addWidget( Linestyle, 4, 1 );
    		LineW = new MSpinBox( Props, 1 );
    		LineW->setMinimumSize( QSize( 65, 22 ) );
    		LineW->setSuffix( tr( " pts" ) );
    		LineW->setMaxValue( 120 );
    		LineW->setMinValue( 1 ); // hopefull - PFJ
																 // No, linewidth 0 is handled by setting the Linecolor to None
																 // due to some obscure Bugs in Qt. F.S.
				LineW->setLineStep(10);
    		LineW->setValue( qRound(b->Pwidth * 10) );
    		Layout4->addWidget( LineW, 5, 1, Qt::AlignLeft );
		    if ((b->PType == 5) || (b->PType == 7))
		    	{
    			LineW->setMinValue( 1 );
    			Background->setEnabled(false);
    			Shade->setEnabled(false);
    			TextLabel1_3->setEnabled(false);
    			TextLabel2->setEnabled(false);
					if (b->PType == 5)
						Height->setEnabled(false);
		    	}
		    FlippedH->setEnabled(false);
		    FlippedV->setEnabled(false);
		    if (b->PType == 3)
		    	{
		    	TextLabel2_3b = new QLabel( Props, "TextLabel2_3b" );
    			TextLabel2_3b->setText( tr( "Corner Radius:" ) );
    			Layout4->addWidget( TextLabel2_3b, 6, 0 );
    			Radius = new QSpinBox(Props, "RR");
    			Radius->setMinimumSize( QSize( 65, 22 ) );
    			Radius->setSuffix( tr( " pt" ) );
   				Radius->setMaxValue(QMIN(static_cast<int>(b->Width),static_cast<int>(b->Height))/2 );
    			Radius->setMinValue(0);
    			Radius->setValue(qRound(b->RadRect));
    			Layout4->addWidget( Radius, 6, 1, Qt::AlignLeft );
    			}
    		break;
    	}
		TextName = new QLabel( Props, "TextName" );
    TextName->setText( tr( "Name:" ) );
    Layout4->addWidget( TextName, 10, 0 );
		NameEdit = new NameWidget(Props);
    NameEdit->setText(b->AnName);
    Layout4->addWidget( NameEdit, 10, 1 );
    PropsLayout->addLayout( Layout4 );
    Layout5->addMultiCellWidget( Props, 0, 1, 1, 1 );
		Layout5->addColSpacing(0, GroupSpecial->layout()->minimumSize().width()+20);
    ModObjLayout->addLayout( Layout5 );

    Layout3 = new QHBoxLayout;
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 10 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer );
    OK = new QPushButton( this, "OK" );
    OK->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, OK->sizePolicy().hasHeightForWidth() ) );
    OK->setText( tr( "OK" ) );
    OK->setDefault( true );
    Layout3->addWidget( OK );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer_2 );
    Apply = new QPushButton( this, "Apply" );
    Apply->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, Apply->sizePolicy().hasHeightForWidth() ) );
    Apply->setText( tr( "Apply" ) );
    Layout3->addWidget( Apply );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer_3 );
    Cancel = new QPushButton( this, "Cancel" );
    Cancel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, Cancel->sizePolicy().hasHeightForWidth() ) );
    Cancel->setText( tr( "Cancel" ) );
    Layout3->addWidget( Cancel );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( spacer_4 );
    ModObjLayout->addLayout( Layout3 );
		if (b->Locked)
			{
			Xpos->setEnabled(false);
			Ypos->setEnabled(false);
			Width->setEnabled(false);
			Height->setEnabled(false);
			Angle->setEnabled(false);
			}
    UnitChange();
    setMinimumSize(ModObjLayout->minimumSize());
    setFocus();
    // signals and slots connections
    connect(Cancel, SIGNAL(clicked()), this, SLOT(NoModifyExit()));
    connect(OK, SIGNAL(clicked()), this, SLOT(DoModifyExit()));
    connect(Apply, SIGNAL(clicked()), this, SLOT(DoModify()));
		connect(Textflow, SIGNAL(clicked()), this, SLOT(ToggleFlow()));
		connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
		if (CurMod->PType == 2)
			{
   		connect(FreeScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
   		connect(FrameScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
    	}
}


void ModObj::DoModifyExit()
{
	if (!DoModify())
		return;
	accept();
}

void ModObj::NoModifyExit()
{
	disconnect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	reject();
}

bool ModObj::DoModify()
{
	float oldx, oldy;
	disconnect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	if (NewName())
		{
		connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
		return false;
		}
	connect(NameEdit, SIGNAL(Leaved()), this, SLOT(NewName()));
	CurMod->AnName = NameEdit->text();
	int itNr = CurMod->ItemNr;
	doc->ActPage->MoveItem(static_cast<float>(Xpos->value())/UmReFaktor/100-CurMod->Xpos, 0, CurMod);
	doc->ActPage->MoveItem(0, static_cast<float>(Ypos->value())/UmReFaktor/100-CurMod->Ypos, CurMod);
	if (origWidth != Width->value())
		doc->ActPage->SizeItem(static_cast<float>(Width->value())/UmReFaktor/100, CurMod->Height, itNr);
	origWidth = Width->value();
	if (origHeight != Height->value())
		doc->ActPage->SizeItem(CurMod->Width, static_cast<float>(Height->value())/UmReFaktor/100, itNr);
	origHeight = Height->value();
	doc->ActPage->RotateItem(static_cast<float>(Angle->value())/-100, itNr);
	CurMod->isPrintable = !Printable->isChecked();
	CurMod->Textflow = Textflow->isChecked();
	CurMod->Textflow2 = Textflow2->isChecked();
	CurMod->flippedH = static_cast<int>(FlippedH->isChecked());
	CurMod->flippedV = static_cast<int>(FlippedV->isChecked());
	switch (CurMod->PType)
		{
		case 8:
			CurMod->Extra = static_cast<float>(Distance->value()) / 10;
			CurMod->BaseOffs = static_cast<float>(-LineW->value()) / 10;
			CurMod->PoShow = NormText2->isChecked();
			doc->ActPage->UpdatePolyClip(CurMod);
			doc->ActPage->AdjustItemSize(CurMod);
			break;
		case 4:
			if (Bookmark->isChecked() != CurMod->isBookmark)
				doc->ActPage->ToggleBookmark();
			if (Annotation->isChecked() != CurMod->isAnnotation)
				doc->ActPage->ToggleAnnotation();
			CurMod->Extra = static_cast<float>(Distance->value()) / 10;
			CurMod->Pcolor = Background->currentText();
			if (CurMod->Pcolor == tr("None"))
				CurMod->Pcolor = "None";
			CurMod->Shade = Shade->value();
			CurMod->Dirty = true;
			if (!CurMod->ClipEdited)
				{
				CurMod->RadRect = static_cast<float>(Radius->value());
				if (Radius->value() != 0)
					{
					CurMod->FrameType = 2;
					CurMod->PLineJoin = RoundJoin;
					}
				else
					{
					CurMod->FrameType = 0;
					CurMod->PLineJoin = MiterJoin;
					}
				doc->ActPage->UpdateClip(CurMod);
				}
			break;
		case 2:
			CurMod->Pcolor = Background->currentText();
			if (CurMod->Pcolor == tr("None"))
				CurMod->Pcolor = "None";
			CurMod->Shade = Shade->value();
			CurMod->PicArt = Visible->isChecked();
			CurMod->LocalX = static_cast<float>(LXpos->value())/ UmReFaktor / 100;
			CurMod->LocalY = static_cast<float>(LYpos->value())/ UmReFaktor / 100;
			oldx = CurMod->LocalViewX / CurMod->LocalScX;
			oldy = CurMod->LocalViewY / CurMod->LocalScY;
			CurMod->LocalScX = static_cast<float>(HScale->value()) / 1000;
			CurMod->LocalScY = static_cast<float>(VScale->value()) / 1000;
			CurMod->LocalViewX = oldy * CurMod->LocalScX;
			CurMod->LocalViewY = oldy * CurMod->LocalScY;
			CurMod->ScaleType = FreeScale->isChecked();
			CurMod->AspectRatio = Aspect->isChecked();
			if (!CurMod->ScaleType)
				{
				doc->ActPage->AdjustPictScale(CurMod);
    		doc->ActPage->AdjustPreview(CurMod);
				}
			if (!CurMod->ClipEdited)
				{
				CurMod->RadRect = static_cast<float>(Radius->value());
				if (Radius->value() != 0)
					{
					CurMod->FrameType = 2;
					CurMod->PLineJoin = RoundJoin;
					}
				else
					{
					CurMod->FrameType = 0;
					CurMod->PLineJoin = MiterJoin;
					}
				doc->ActPage->UpdateClip(CurMod);
				}
#ifdef HAVE_CMS
			if ((CMSuse) && (CMSavail))
				{
				bool CMchange = false;
				if (CurMod->IProfile != InputP->currentText())
					{
					CurMod->IProfile = InputP->currentText();
					CMchange = true;
					}
				if (CurMod->IRender != MonitorI->currentItem())
					{
					CurMod->IRender = MonitorI->currentItem();
					CMchange = true;
					}
				if (CurMod->UseEmbedded != EmbedP)
					{
					CurMod->UseEmbedded = EmbedP;
					CMchange = true;
					}
				if (CMchange)
					doc->ActPage->LoadPict(CurMod->Pfile, CurMod->ItemNr);
				}
#endif
			break;
		default:
			if ((CurMod->PType != 5) || (CurMod->PType != 7))
				{
				CurMod->Pcolor = Background->currentText();
				if (CurMod->Pcolor == tr("None"))
					CurMod->Pcolor = "None";
				CurMod->Shade = Shade->value();
				}
			if (CurMod->PType == 3)
				{
				if (!CurMod->ClipEdited)
					{
					CurMod->RadRect = static_cast<float>(Radius->value());
					if (Radius->value() != 0)
						CurMod->PLineJoin = RoundJoin;
					else
						CurMod->PLineJoin = MiterJoin;
					}
				}
			CurMod->Pcolor2 = Foreground->currentText();
			if (CurMod->Pcolor2 == tr("None"))
				CurMod->Pcolor2 = "None";
			CurMod->Shade2 = Shade2->value();
			doc->ActPage->ChLineWidth(static_cast<float>(LineW->value()) / 10.0);
			switch (Linestyle->currentItem())
				{
				case 0:
					CurMod->PLineArt = SolidLine;
					break;
				case 1:
					CurMod->PLineArt = DashLine;
					break;
				case 2:
					CurMod->PLineArt = DotLine;
					break;
				case 3:
					CurMod->PLineArt = DashDotLine;
					break;
				case 4:
					CurMod->PLineArt = DashDotDotLine;
					break;
				}
			break;
		}
	doc->ActPage->repaint();
	emit DocChanged();
	return true;
}

void ModObj::ChangeScaling()
{
	if (FreeScale->isChecked())
		{
		TextLabel3_2->setEnabled(true);
		TextLabel3_2_2->setEnabled(true);
		TextLabel3_2_2_2->setEnabled(true);
		TextLabel3_2_2_2_2->setEnabled(true);
		LXpos->setEnabled(true);
		LYpos->setEnabled(true);
		HScale->setEnabled(true);
		VScale->setEnabled(true);
		Kette->setEnabled(true);
		Aspect->setEnabled(false);
		}
	else
		{
		TextLabel3_2->setEnabled(false);
		TextLabel3_2_2->setEnabled(false);
		TextLabel3_2_2_2->setEnabled(false);
		TextLabel3_2_2_2_2->setEnabled(false);
		LXpos->setEnabled(false);
		LYpos->setEnabled(false);
		HScale->setEnabled(false);
		VScale->setEnabled(false);
		Kette->setEnabled(false);
		Aspect->setEnabled(true);
		}	
}

void ModObj::ToggleFlow()
{
	if (Textflow->isChecked())
		Textflow2->setEnabled(true);
	else
		Textflow2->setEnabled(false);
}

void ModObj::ToggleKette()
{
	if (Kette->isOn())
		VScale->setValue(HScale->value());
}

void ModObj::HChange()
{
	if (Kette->isOn())
		VScale->setValue(HScale->value());
}

void ModObj::VChange()
{
	if (Kette->isOn())
		HScale->setValue(VScale->value());
}

void ModObj::UnitChange()
{
	switch (doc->Einheit)
		{
		case 0:
    	Xpos->setSuffix( tr( " pt" ) );
    	Ypos->setSuffix( tr( " pt" ) );
    	Width->setSuffix( tr( " pt" ) );
    	Height->setSuffix( tr( " pt" ) );
    	if (CurMod->PType == 2)
    		{
    		LXpos->setSuffix( tr( " pt" ) );
    		LYpos->setSuffix( tr( " pt" ) );
    		}
    	if (CurMod->PType == 3)
    		Radius->setSuffix( tr( " pt" ) );
			break;
		case 1:
    	Xpos->setSuffix( tr( " mm" ) );
    	Ypos->setSuffix( tr( " mm" ) );
    	Width->setSuffix( tr( " mm" ) );
    	Height->setSuffix( tr( " mm" ) );
    	if (CurMod->PType == 2)
    		{
    		LXpos->setSuffix( tr( " mm" ) );
    		LYpos->setSuffix( tr( " mm" ) );
    		}
    	if (CurMod->PType == 3)
    		Radius->setSuffix( tr( " mm" ) );
			break;
		case 2:
    	Xpos->setSuffix( tr( " in" ) );
    	Ypos->setSuffix( tr( " in" ) );
    	Width->setSuffix( tr( " in" ) );
    	Height->setSuffix( tr( " in" ) );
    	if (CurMod->PType == 2)
    		{
    		LXpos->setSuffix( tr( " in" ) );
    		LYpos->setSuffix( tr( " in" ) );
    		}
    	if (CurMod->PType == 3)
    		Radius->setSuffix( tr( " in" ) );
			break;
		case 3:
    	Xpos->setSuffix( tr( " p" ) );
    	Ypos->setSuffix( tr( " p" ) );
    	Width->setSuffix( tr( " p" ) );
    	Height->setSuffix( tr( " p" ) );
    	if (CurMod->PType == 2)
    		{
    		LXpos->setSuffix( tr( " p" ) );
    		LYpos->setSuffix( tr( " p" ) );
    		}
    	if (CurMod->PType == 3)
    		Radius->setSuffix( tr( " p" ) );
			break;
		}
}

bool ModObj::NewName()
{
	QString NameOld = CurMod->AnName;
	QString NameNew = NameEdit->text();
	bool found = false;
	for (uint a = 0; a < view->Pages.count(); ++a)
		{
		for (uint b = 0; b < view->Pages.at(a)->Items.count(); ++b)
			{
			if ((NameNew == view->Pages.at(a)->Items.at(b)->AnName) && (view->Pages.at(a)->Items.at(b) != CurMod))
				{
				found = true;
				break;
				}
			}
		if (found)
			break;
		}
	if (found)
		{
		QMessageBox::warning(this, tr("Warning"), tr("Name \"%1\" isn't unique.\nPlease choose another.").arg(NameNew), tr("OK"));
		NameEdit->setText(NameOld);
		NameEdit->setFocus();
		}
	return found;	
}

void ModObj::EmProf(const QString& prn)
{
	if (prn.startsWith("Embedded"))
    EmbedP = true;
	else
    EmbedP = false;
}
