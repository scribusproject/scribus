#include "mpalette.h"
#include "mpalette.moc"
#include <cmath>
#include <qpoint.h>
#include <qwmatrix.h>
extern QPixmap loadIcon(QString nam);
extern float UmReFaktor;

Mpalette::Mpalette( QWidget* parent, preV *Prefs)
    : QDialog( parent, "Mfloat", false, Qt::WStyle_Customize | Qt::WStyle_Title | Qt::WStyle_Tool)
{
    setCaption( tr( "Measurements" ) );
    HaveDoc = false;
    HaveItem = false;
		Umrech = UmReFaktor;
    setIcon( loadIcon("AppIcon.xpm") );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );

    MpalLayout = new QVBoxLayout( this, 5, 1, "MpalLayout");
    setOrientation(Qt::Vertical);
    QFont f(font());
    f.setPointSize(f.pointSize()-1);
    setFont(f);
    buttonGroup5 = new QButtonGroup( this, "buttonGroup5" );
    buttonGroup5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, buttonGroup5->sizePolicy().hasHeightForWidth() ) );
    buttonGroup5->setFrameShape( QButtonGroup::NoFrame );
    buttonGroup5->setExclusive( TRUE );
    buttonGroup5->setColumnLayout(0, Qt::Vertical );
    buttonGroup5->setTitle( QString::null );
    buttonGroup5->layout()->setSpacing( 0 );
    buttonGroup5->layout()->setMargin( 0 );
    buttonGroup5Layout = new QGridLayout( buttonGroup5->layout() );
    buttonGroup5Layout->setAlignment( Qt::AlignTop );
    SGeom = new QToolButton( buttonGroup5, "toolButton24" );
    SGeom->setToggleButton( true );
    SGeom->setOn( true );
    SGeom->setFont(f);
    SGeom->setText( tr( "X, Y, Z" ) );
    buttonGroup5Layout->addWidget( SGeom, 0, 0 );
    SShape = new QToolButton( buttonGroup5, "toolButton25" );
    SShape->setToggleButton( true );
    SShape->setFont(f);
    SShape->setText( tr( "Shape" ) );
    buttonGroup5Layout->addWidget( SShape, 0, 1 );
    SText = new QToolButton( buttonGroup5, "toolButton26" );
    SText->setToggleButton( true );
    SText->setFont(f);
    SText->setText( tr( "Text" ) );
    buttonGroup5Layout->addWidget( SText, 1, 0 );
    SImage = new QToolButton( buttonGroup5, "toolButton27" );
    SImage->setToggleButton( true );
    SImage->setFont(f);
    SImage->setText( tr( "Image" ) );
    buttonGroup5Layout->addWidget( SImage, 1, 1 );
    SLine = new QToolButton( buttonGroup5, "toolButton28" );
    SLine->setToggleButton( true );
    SLine->setFont(f);
    SLine->setText( tr( "Line" ) );
    buttonGroup5Layout->addWidget( SLine, 2, 0 );
    SColor = new QToolButton( buttonGroup5, "toolButton29" );
    SColor->setToggleButton( true );
    SColor->setFont(f);
    SColor->setText( tr( "Colors" ) );
    buttonGroup5Layout->addWidget( SColor, 2, 1 );
    MpalLayout->addWidget( buttonGroup5 );
    TabStack = new QWidgetStack( this, "TabStack" );
    TabStack->setFrameShape( QWidgetStack::NoFrame );

    page = new QWidget( TabStack, "page" );
    pageLayout = new QVBoxLayout( page, 0, 5, "pageLayout");

    GeoGroup = new QGroupBox( page, "GeoGroup" );
    GeoGroup->setTitle( tr( "Geometry" ) );
    GeoGroup->setColumnLayout(0, Qt::Vertical );
    GeoGroup->layout()->setSpacing( 4 );
    GeoGroup->layout()->setMargin( 5 );
    GeoGroupLayout = new QGridLayout( GeoGroup->layout() );
    GeoGroupLayout->setAlignment( Qt::AlignTop );
    Text1 = new QLabel( GeoGroup, "Text1" );
    Text1->setText( tr( "X-Pos:" ) );
    GeoGroupLayout->addWidget( Text1, 0, 0 );
    Text1a = new QLabel( GeoGroup, "Text1a" );
    Text1a->setText( tr( "Y-Pos:" ) );
    GeoGroupLayout->addWidget( Text1a, 1, 0 );
    Text2 = new QLabel( GeoGroup, "Text2" );
    Text2->setText( tr( "Width:" ) );
    GeoGroupLayout->addWidget( Text2, 2, 0 );
    Text2a = new QLabel( GeoGroup, "Text2a" );
    Text2a->setText( tr( "Height:" ) );
    GeoGroupLayout->addWidget( Text2a, 3, 0 );
    Xpos = new MSpinBox( GeoGroup, 2 );
    Xpos->setSuffix( tr( " pt" ) );
		Xpos->setMaxValue( 300000 );
		Xpos->setMinValue( -300000 );
    GeoGroupLayout->addWidget( Xpos, 0, 1 );
    Ypos = new MSpinBox( GeoGroup, 2 );
    Ypos->setSuffix( tr( " pt" ) );
		Ypos->setMaxValue( 300000 );
		Ypos->setMinValue( -300000 );
    GeoGroupLayout->addWidget( Ypos, 1, 1 );
    Width = new MSpinBox( GeoGroup, 2 );
    Width->setSuffix( tr( " pt" ) );
    GeoGroupLayout->addWidget( Width, 2, 1 );
    Height = new MSpinBox( GeoGroup, 2 );
    Height->setSuffix( tr( " pt" ) );
    GeoGroupLayout->addWidget( Height, 3, 1 );
    pageLayout->addWidget( GeoGroup );

    layout60 = new QHBoxLayout( 0, 0, 5, "layout60");
    QSpacerItem* spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60->addItem( spacer1 );

    LayerGroup = new QGroupBox( page, "LayerGroup" );
    LayerGroup->setTitle( tr( "Level" ) );
    LayerGroup->setColumnLayout(0, Qt::Vertical );
    LayerGroup->layout()->setSpacing( 4 );
    LayerGroup->layout()->setMargin( 5 );
    LayerGroupLayout = new QGridLayout( LayerGroup->layout() );
    LayerGroupLayout->setAlignment( Qt::AlignTop );
    Zup = new QToolButton( LayerGroup, "Zup" );
    Zup->setMaximumSize( QSize( 22, 22 ) );
    Zup->setPixmap(loadIcon("up.png"));
    LayerGroupLayout->addWidget( Zup, 0, 0 );
    ZDown = new QToolButton( LayerGroup, "ZDown" );
    ZDown->setMaximumSize( QSize( 22, 22 ) );
    ZDown->setPixmap(loadIcon("down.png"));
    LayerGroupLayout->addWidget( ZDown, 1, 0 );
    ZTop = new QToolButton( LayerGroup, "ZTop" );
    ZTop->setMaximumSize( QSize( 22, 22 ) );
    ZTop->setPixmap(loadIcon("top.png"));
    LayerGroupLayout->addWidget( ZTop, 0, 1 );
    ZBottom = new QToolButton( LayerGroup, "ZBottom" );
    ZBottom->setMaximumSize( QSize( 22, 22 ) );
    ZBottom->setPixmap(loadIcon("bottom.png"));
    LayerGroupLayout->addWidget( ZBottom, 1, 1 );

    layout60->addWidget( LayerGroup );
    QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60->addItem( spacer2 );

    Layout44 = new QVBoxLayout( 0, 0, 4, "Layout44");
    QSpacerItem* spacer3 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout44->addItem( spacer3 );
    FlipH = new QToolButton( page, "MirrorH" );
    FlipH->setPixmap(loadIcon("FlipH.xpm"));
    FlipH->setToggleButton( true );
    Layout44->addWidget( FlipH );
    FlipV = new QToolButton( page, "MirrorV" );
    FlipV->setPixmap(loadIcon("FlipV.xpm"));
    FlipV->setToggleButton( true );
    Layout44->addWidget( FlipV );
    QSpacerItem* spacer4 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout44->addItem( spacer4 );
    layout60->addLayout( Layout44 );
    QSpacerItem* spacer5 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60->addItem( spacer5 );
    pageLayout->addLayout( layout60 );

    layout60a = new QHBoxLayout( 0, 0, 5, "layout60a");
    RotationGroup = new QButtonGroup( page, "RotationGroup" );
    RotationGroup->setTitle( tr( "Rotation" ) );
    RotationGroup->setColumnLayout(0, Qt::Vertical );
    RotationGroup->layout()->setSpacing( 4 );
    RotationGroup->layout()->setMargin( 5 );
    RotationGroupLayout = new QHBoxLayout( RotationGroup->layout() );
    RotationGroupLayout->setAlignment( Qt::AlignTop );
    Layout12 = new QGridLayout( 0, 1, 1, 0, 0, "Layout12"); 
    TopLeft = new QRadioButton( RotationGroup, "TopLeft" );
    TopLeft->setText( tr( "" ) );
    TopLeft->setChecked( true );
    Layout12->addWidget( TopLeft, 0, 0 );
    Line1 = new QFrame( RotationGroup, "Line1" );
    Line1->setMinimumSize( QSize( 20, 4 ) );
    Line1->setMaximumSize( QSize( 20, 4 ) );
    Line1->setFrameShape( QFrame::HLine );
    Line1->setFrameShadow( QFrame::Plain );
    Line1->setLineWidth( 3 );
    Line1->setFrameShape( QFrame::HLine );
    Layout12->addWidget( Line1, 0, 1 );
    TopRight = new QRadioButton( RotationGroup, "TopRight" );
    TopRight->setText( tr( "" ) );
    Layout12->addWidget( TopRight, 0, 2 );
    Line2 = new QFrame( RotationGroup, "Line2" );
    Line2->setMinimumSize( QSize( 4, 20 ) );
    Line2->setMaximumSize( QSize( 4, 20 ) );
    Line2->setFrameShape( QFrame::VLine );
    Line2->setFrameShadow( QFrame::Plain );
    Line2->setLineWidth( 3 );
    Line2->setFrameShape( QFrame::VLine );
    Layout12->addWidget( Line2, 1, 0, Qt::AlignHCenter );
    Center = new QRadioButton( RotationGroup, "Center" );
    Center->setText( tr( "" ) );
    Layout12->addWidget( Center, 1, 1, Qt::AlignHCenter );
    Line4 = new QFrame( RotationGroup, "Line4" );
    Line4->setMinimumSize( QSize( 4, 20 ) );
    Line4->setMaximumSize( QSize( 4, 20 ) );
    Line4->setFrameShape( QFrame::VLine );
    Line4->setFrameShadow( QFrame::Plain );
    Line4->setLineWidth( 3 );
    Line4->setFrameShape( QFrame::VLine );
    Layout12->addWidget( Line4, 1, 2, Qt::AlignHCenter );
    BottomLeft = new QRadioButton( RotationGroup, "BottomLeft" );
    BottomLeft->setText( tr( "" ) );
    Layout12->addWidget( BottomLeft, 2, 0 );
    Line5 = new QFrame( RotationGroup, "Line5" );
    Line5->setMinimumSize( QSize( 20, 4 ) );
    Line5->setMaximumSize( QSize( 20, 4 ) );
    Line5->setFrameShape( QFrame::HLine );
    Line5->setFrameShadow( QFrame::Plain );
    Line5->setLineWidth( 3 );
    Line5->setFrameShape( QFrame::HLine );
    Layout12->addWidget( Line5, 2, 1 );
    BottomRight = new QRadioButton( RotationGroup, "BottomRight" );
    BottomRight->setText( tr( "" ) );
    Layout12->addWidget( BottomRight, 2, 2 );
    RotationGroupLayout->addLayout( Layout12 );
    Layout15_2 = new QVBoxLayout( 0, 3, 0, "Layout15_2"); 
    Text3 = new QLabel( RotationGroup, "Text3" );
    Text3->setText( tr( "by:" ) );
    Layout15_2->addWidget( Text3 );
    Rot = new MSpinBox( RotationGroup, 2);
    Rot->setSuffix(" °");
    Rot->setWrapping( true );
    Layout15_2->addWidget( Rot );
    RotationGroupLayout->addLayout( Layout15_2 );
    layout60a->addWidget(RotationGroup);
    QSpacerItem* spacer12 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60a->addItem( spacer12 );
    pageLayout->addLayout(layout60a);
    QSpacerItem* spacer13 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout->addItem( spacer13 );
    TabStack->addWidget( page, 0 );

    page_2 = new QWidget( TabStack, "page_2" );
    pageLayout_2 = new QVBoxLayout( page_2, 0, 5, "pageLayout_2");

    ShapeGroup = new QButtonGroup( page_2, "ShapeGroup" );
    ShapeGroup->setFrameShape( QButtonGroup::NoFrame );
    ShapeGroup->setTitle( tr( "" ) );
    ShapeGroup->setExclusive( true );
    ShapeGroup->setColumnLayout(0, Qt::Vertical );
    ShapeGroup->layout()->setSpacing( 2 );
    ShapeGroup->layout()->setMargin( 0 );
    ShapeGroupLayout = new QHBoxLayout( ShapeGroup->layout() );
    ShapeGroupLayout->setAlignment( Qt::AlignTop );
    SRect = new QToolButton( ShapeGroup, "SRect" );
    SRect->setMaximumSize( QSize( 22, 22 ) );
    SRect->setPixmap(loadIcon("rect.png"));
    ShapeGroupLayout->addWidget( SRect );
    SOval = new QToolButton( ShapeGroup, "SOval" );
    SOval->setMaximumSize( QSize( 22, 22 ) );
    SOval->setPixmap(loadIcon("oval.png"));
    ShapeGroupLayout->addWidget( SOval );
    SCustom = new QToolButton( ShapeGroup, "SCustom" );
    SCustom->setMaximumSize( QSize( 22, 22 ) );
    SCustom->setPixmap(loadIcon("irreg.png"));
    ShapeGroupLayout->addWidget( SCustom );
    pageLayout_2->addWidget( ShapeGroup );

    EditShape = new QToolButton( page_2, "EditShape" );
    EditShape->setText( tr( "Edit Shape..." ) );
    pageLayout_2->addWidget( EditShape );

    Layout13 = new QHBoxLayout( 0, 0, 2, "Layout13"); 

    Text4 = new QLabel( page_2, "Text4" );
    Text4->setText( tr( "Round\nCorners:" ) );
    Layout13->addWidget( Text4 );

    RoundRect = new QSpinBox( page_2, "Corner" );
    RoundRect->setSuffix( tr( " pt" ) );
    Layout13->addWidget( RoundRect );
    pageLayout_2->addLayout( Layout13 );

    Textflow = new QCheckBox( page_2, "Textflow" );
    Textflow->setText( tr( "Text flows around Box" ) );
    pageLayout_2->addWidget( Textflow );

    Distance = new QGroupBox( page_2, "Distance" );
    Distance->setTitle( tr( "Distance of Text" ) );
    Distance->setColumnLayout(0, Qt::Vertical );
    Distance->layout()->setSpacing( 2 );
    Distance->layout()->setMargin( 5 );
    DistanceLayout = new QGridLayout( Distance->layout() );
    DistanceLayout->setAlignment( Qt::AlignTop );
    Text14 = new QLabel( Distance, "Text14" );
    Text14->setText( tr( "Top:" ) );
    DistanceLayout->addWidget( Text14, 0, 0 );
    Text15 = new QLabel( Distance, "Text15" );
    Text15->setText( tr( "Bottom:" ) );
    DistanceLayout->addWidget( Text15, 1, 0 );
    Text16 = new QLabel( Distance, "Text16" );
    Text16->setText( tr( "Left:" ) );
    DistanceLayout->addWidget( Text16, 2, 0 );
    Text17 = new QLabel( Distance, "Text17" );
    Text17->setText( tr( "Right:" ) );
    DistanceLayout->addWidget( Text17, 3, 0 );
    DTop = new MSpinBox( Distance, 1 );
    DTop->setSuffix( tr( " pt" ) );
    DTop->setMaxValue( 3000 );
    DTop->setMinValue( 0 );
    DistanceLayout->addWidget( DTop, 0, 1 );
    DBottom = new MSpinBox( Distance, 1 );
    DBottom->setSuffix( tr( " pt" ) );
    DBottom->setMaxValue( 3000 );
    DBottom->setMinValue( 0 );
    DistanceLayout->addWidget( DBottom, 1, 1 );
    DLeft = new MSpinBox( Distance, 1 );
    DLeft->setSuffix( tr( " pt" ) );
    DLeft->setMaxValue( 3000 );
    DLeft->setMinValue( 0 );
    DistanceLayout->addWidget( DLeft, 2, 1 );
    DRight = new MSpinBox( Distance, 1 );
    DRight->setSuffix( tr( " pt" ) );
    DRight->setMaxValue( 3000 );
    DRight->setMinValue( 0 );
    DistanceLayout->addWidget( DRight, 3, 1 );
    pageLayout_2->addWidget( Distance );
    QSpacerItem* spacer6 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout_2->addItem( spacer6 );
    TabStack->addWidget( page_2, 1 );

    page_3 = new QWidget( TabStack, "page_3" );
    pageLayout_3 = new QVBoxLayout( page_3, 0, 5, "pageLayout_3");

    layout47 = new QHBoxLayout( 0, 0, 5, "layout47");

    layout46 = new QVBoxLayout( 0, 0, 5, "layout46");

    layout41 = new QGridLayout( 0, 1, 1, 0, 5, "layout41");

/*    Fonts = new QToolButton( page_3, "Fonts" );
    Fonts->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, 0, 0, Fonts->sizePolicy().hasHeightForWidth() ) );
    Fonts->setMinimumSize( QSize( 150, 0 ) );
    Fonts->setMaximumSize( QSize( 250, 32767 ) );
    Fonts->setPopup(FontMenu);
    Fonts->setPopupDelay(1);
    Fonts->setText( tr( "Font" ) );   */

    Fonts = new FontCombo(page_3, Prefs);
//    Fonts->setMinimumSize( QSize( 175, 24 ) );
/*		Fonts->setEditable(false);
		QStringList rlist;
		rlist.clear();
    SCFontsIterator it(Prefs->AvailFonts);
    for ( ; it.current(); ++it)
    	{
			if (it.current()->UseFont)
    		rlist.append(it.currentKey());
    	}
		rlist.sort();
    Fonts->clear();
    for (QStringList::ConstIterator it2 = rlist.begin(); it2 != rlist.end(); ++it2)
			{
      new FontListItem(Fonts, *it2, Prefs->AvailFonts[*it2]->Font);
			}   */
//		Fonts->insertStringList(rlist);


    layout41->addMultiCellWidget( Fonts, 0, 0, 0, 1 );

    Text20 = new QLabel( page_3, "Text20" );
    Text20->setText(tr("Size:"));
    layout41->addWidget( Text20, 1, 0 );
    Size = new QSpinBox( page_3, "Size" );
    Size->setPrefix( tr( "" ) );
    Size->setSuffix( tr( " pt" ) );
    Size->setMaxValue( 3000 );
    Size->setMinValue( -3000 );
    layout41->addWidget( Size, 1, 1 );
    layout46->addLayout( layout41 );

    Layout1 = new QHBoxLayout( 0, 0, 0, "Layout1");

    Underline = new QToolButton( page_3, "Underline" );
    Underline->setMaximumSize( QSize( 22, 22 ) );
    Underline->setText( tr( "" ) );
    Underline->setPixmap(loadIcon("Unter.xpm"));
    Underline->setToggleButton( true );
    Layout1->addWidget( Underline );

    ButtonGroup1 = new QButtonGroup( page_3, "ButtonGroup1" );
    ButtonGroup1->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup1->setTitle( tr( "" ) );
    ButtonGroup1->setExclusive( true );
    ButtonGroup1->setRadioButtonExclusive( false );
    ButtonGroup1->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1->layout()->setSpacing( 0 );
    ButtonGroup1->layout()->setMargin( 0 );
    ButtonGroup1Layout = new QHBoxLayout( ButtonGroup1->layout() );
    ButtonGroup1Layout->setAlignment( Qt::AlignTop );

    Subs = new QToolButton( ButtonGroup1, "Subs" );
    Subs->setMaximumSize( QSize( 22, 22 ) );
    Subs->setText( tr( "" ) );
    Subs->setPixmap(loadIcon("Tief.xpm"));
    Subs->setToggleButton( true );
    ButtonGroup1Layout->addWidget( Subs );

    Supers = new QToolButton( ButtonGroup1, "Supers" );
    Supers->setMaximumSize( QSize( 22, 22 ) );
    Supers->setText( tr( "" ) );
    Supers->setPixmap(loadIcon("Hoch.xpm"));
    Supers->setToggleButton( true );
    ButtonGroup1Layout->addWidget( Supers );
    Layout1->addWidget( ButtonGroup1 );

    Kapital = new QToolButton( page_3, "Kapital" );
    Kapital->setMaximumSize( QSize( 22, 22 ) );
    Kapital->setText( tr( "" ) );
    Kapital->setPixmap(loadIcon("Kapital.xpm"));
    Kapital->setToggleButton( true );
    Layout1->addWidget( Kapital );

    Strike = new QToolButton( page_3, "Strike" );
    Strike->setMaximumSize( QSize( 22, 22 ) );
    Strike->setText( tr( "" ) );
    Strike->setPixmap(loadIcon("Strike.xpm"));
    Strike->setToggleButton( true );
    Layout1->addWidget( Strike );
    Revert = new QToolButton( page_3, "Bold" );
    Revert->setMaximumSize( QSize( 22, 22 ) );
    Revert->setText( tr( "" ) );
    Revert->setPixmap(loadIcon("Revers.png"));
    Revert->setToggleButton( true );
    Layout1->addWidget( Revert );
    QSpacerItem* spacer7 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer7 );
    layout46->addLayout( Layout1 );
    layout47->addLayout( layout46 );
    pageLayout_3->addLayout( layout47 );

    GroupAlign = new QButtonGroup( page_3, "GroupAlign" );
    GroupAlign->setFrameShape( QButtonGroup::NoFrame );
    GroupAlign->setTitle( tr( "" ) );
    GroupAlign->setExclusive( true );
    GroupAlign->setColumnLayout(0, Qt::Vertical );
    GroupAlign->layout()->setSpacing( 0 );
    GroupAlign->layout()->setMargin( 0 );
    GroupAlignLayout = new QGridLayout( GroupAlign->layout() );
    GroupAlignLayout->setAlignment( Qt::AlignTop );

    TextL = new QToolButton( GroupAlign, "TextL" );
    TextL->setMaximumSize( QSize( 22, 22 ) );
    TextL->setPixmap(loadIcon("text_left.png"));
    TextL->setToggleButton( true );
    TextL->setOn( true );
    GroupAlignLayout->addWidget( TextL, 0, 0 );

    TextR = new QToolButton( GroupAlign, "TextR" );
    TextR->setMaximumSize( QSize( 22, 22 ) ); 
    TextR->setPixmap(loadIcon("text_right.png"));
    TextR->setToggleButton( true );
    GroupAlignLayout->addWidget( TextR, 0, 1 );

    TextC = new QToolButton( GroupAlign, "TextC" );
    TextC->setMaximumSize( QSize( 22, 22 ) ); 
    TextC->setPixmap(loadIcon("text_center.png"));
    TextC->setToggleButton( true );
    GroupAlignLayout->addWidget( TextC, 0, 2 );

    TextB = new QToolButton( GroupAlign, "TextB" );
    TextB->setMaximumSize( QSize( 22, 22 ) ); 
    TextB->setPixmap(loadIcon("text_block.png"));
    TextB->setToggleButton( true );
    GroupAlignLayout->addWidget( TextB, 0, 3 );

    TextF = new QToolButton( GroupAlign, "TextF" );
    TextF->setMaximumSize( QSize( 22, 22 ) );
    TextF->setPixmap(loadIcon("text_force.png"));
    TextF->setToggleButton( true );
    GroupAlignLayout->addWidget( TextF, 0, 4 );
    pageLayout_3->addWidget( GroupAlign );

    GroupBox3 = new QGroupBox( page_3, "GroupBox3" );
    GroupBox3->setTitle( tr( "Custom Spacing" ) );
    GroupBox3->setColumnLayout(0, Qt::Vertical );
    GroupBox3->layout()->setSpacing( 2 );
    GroupBox3->layout()->setMargin( 5 );
    GroupBox3Layout = new QGridLayout( GroupBox3->layout() );
    GroupBox3Layout->setAlignment( Qt::AlignTop );

    LineSp = new MSpinBox( GroupBox3, 1 );
    LineSp->setSuffix( tr( " pt" ) );
    GroupBox3Layout->addWidget( LineSp, 1, 1 );

    Extra = new MSpinBox( GroupBox3, 1 );
    Extra->setSuffix( tr( " pt" ) );
    GroupBox3Layout->addWidget( Extra, 0, 1 );

    Text5 = new QLabel( GroupBox3, "Text5" );
    Text5->setText( tr( "Kerning:" ) );
    GroupBox3Layout->addWidget( Text5, 0, 0 );

    Text6 = new QLabel( GroupBox3, "Text6" );
    Text6->setText( tr( "Linespacing:" ) );

    GroupBox3Layout->addWidget( Text6, 1, 0 );
    pageLayout_3->addWidget( GroupBox3 );

    GroupBox3a = new QGroupBox( page_3, "GroupBox3" );
    GroupBox3a->setTitle( tr( "Styles" ) );
    GroupBox3a->setColumnLayout(0, Qt::Vertical );
    GroupBox3a->layout()->setSpacing( 2 );
    GroupBox3a->layout()->setMargin( 5 );
    GroupBox3aLayout = new QVBoxLayout( GroupBox3a->layout() );
    GroupBox3aLayout->setAlignment( Qt::AlignTop );
		Spal = new Spalette(GroupBox3a);
    GroupBox3aLayout->addWidget( Spal );
    pageLayout_3->addWidget( GroupBox3a );

    QSpacerItem* spacer8 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout_3->addItem( spacer8 );
    TabStack->addWidget( page_3, 2 );

    page_4 = new QWidget( TabStack, "page_4" );
    pageLayout_4 = new QVBoxLayout( page_4, 0, 5, "pageLayout_4");

    FreeScale = new QRadioButton( page_4, "FreeScale" );
    FreeScale->setText( tr( "Free Scaling" ) );
    FreeScale->setChecked( true );
    pageLayout_4->addWidget( FreeScale );

    layout43 = new QGridLayout( 0, 1, 1, 0, 5, "layout43");
    Text11 = new QLabel( page_4, "Text11" );
    Text11->setText( tr( "Position:" ) );
    layout43->addWidget( Text11, 0, 0 );
    LXpos = new MSpinBox( page_4, 2 );
    LXpos->setSuffix( tr( " pt" ) );
    layout43->addWidget( LXpos, 0, 1 );
    LYpos = new MSpinBox( page_4, 2 );
    LYpos->setSuffix( tr( " pt" ) );
    layout43->addWidget( LYpos, 1, 1 );
    Text12 = new QLabel( page_4, "Text12" );
    Text12->setText( tr( "Horiz. Scale:" ) );
    layout43->addWidget( Text12, 2, 0 );
    ScaleX = new MSpinBox( page_4, 1 );
    ScaleX->setSuffix( tr( " %" ) );
    layout43->addWidget( ScaleX, 2, 1 ); 
    Text13 = new QLabel( page_4, "Text13" );
    Text13->setText( tr( "Vert. Scale:" ) );
    layout43->addWidget( Text13, 3, 0 );
    ScaleY = new MSpinBox( page_4, 1 );
    ScaleY->setSuffix( tr( " %" ) );
    layout43->addWidget( ScaleY, 3, 1 );
    Kette = new LinkButton( page_4 );
    Kette->setToggleButton( true );
    Kette->setAutoRaise( true );
    Kette->setMaximumSize( QSize( 15, 32767 ) );
    layout43->addMultiCellWidget( Kette, 2, 3, 2, 2 );
    pageLayout_4->addLayout( layout43 );

    Layout24 = new QVBoxLayout( 0, 0, 3, "Layout24"); 

    FrameScale = new QRadioButton( page_4, "FixedScale" );
    FrameScale->setText( tr( "Scale to Frame Size" ) );
    Layout24->addWidget( FrameScale );

    Layout18 = new QHBoxLayout( 0, 0, 6, "Layout18"); 

    Frame4 = new QFrame( page_4, "Frame4" );
    Frame4->setMinimumSize( QSize( 15, 2 ) );
    Frame4->setMaximumSize( QSize( 15, 10 ) );
    Frame4->setFrameShape( QFrame::NoFrame );
    Frame4->setFrameShadow( QFrame::Plain );
    Layout18->addWidget( Frame4 );

    Aspect = new QCheckBox( page_4, "Aspect" );
    /*<<<<<<< mpalette.cpp*/
    Aspect->setEnabled( false );
    Aspect->setText( tr( "Keep Aspect Ratio" ) );
    Aspect->setChecked( true );
    /*=======*/
    Aspect->setEnabled( FALSE );
    Aspect->setText( tr( "Proportional" ) );
    Aspect->setChecked( TRUE );
    /*>>>>>>> 1.8*/
    Layout18->addWidget( Aspect );
    Layout24->addLayout( Layout18 );
    pageLayout_4->addLayout( Layout24 );
    QSpacerItem* spacer9 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout_4->addItem( spacer9 );
    TabStack->addWidget( page_4, 3 );

    page_5 = new QWidget( TabStack, "page_5" );
    pageLayout_5 = new QVBoxLayout( page_5, 0, 5, "pageLayout_5");

    Layout13_2 = new QHBoxLayout( 0, 0, 4, "Layout13_2"); 

    LineMode = new QComboBox( true, page_5, "LMode" );
		LineMode->setEditable(false);
    LineMode->insertItem( tr( "Left Point" ) );
    LineMode->insertItem( tr( "Endpoints" ) );
    Layout13_2->addWidget( LineMode );
		LMode = false;
    QSpacerItem* spacer10 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout13_2->addItem( spacer10 );
    pageLayout_5->addLayout( Layout13_2 );

    Layout12_2 = new QGridLayout( 0, 1, 1, 0, 3, "Layout12_2"); 
    Text8 = new QLabel( page_5, "Text8" );
    Text8->setText( tr( "Linestyle:" ) );
    Layout12_2->addWidget( Text8, 0, 0 );
    LStyle = new LineCombo(page_5);
    Layout12_2->addWidget( LStyle, 1, 0 );
    TextLabel1 = new QLabel( page_5, "TextLabel1" );
    TextLabel1->setText( tr( "Linewidth:" ) );
    Layout12_2->addWidget( TextLabel1, 2, 0 );
    LSize = new MSpinBox( page_5, 1 );
    LSize->setSuffix( tr( " pt" ) );
		LSize->setDecimals(10);
    Layout12_2->addWidget( LSize, 3, 0 );
    Text9 = new QLabel( page_5, "Text9" );
    Text9->setText( tr( "Edges:" ) );
    Layout12_2->addWidget( Text9, 4, 0 );
    LJoinStyle = new QComboBox( true, page_5, "LJoin" );
    LJoinStyle->setEditable(false);
    LJoinStyle->insertItem(loadIcon("MiterJoin.png"), tr( "MiterJoin" ) );
    LJoinStyle->insertItem(loadIcon("BevelJoin.png"), tr( "BevelJoin" ) );
    LJoinStyle->insertItem(loadIcon("BevelJoin.png"), tr( "RoundJoin" ) );
    Layout12_2->addWidget( LJoinStyle, 5, 0 );
    Text10 = new QLabel( page_5, "Text10" );
    Text10->setText( tr( "Endings:" ) );
    Layout12_2->addWidget( Text10, 6, 0 );
    LEndStyle = new QComboBox( true, page_5, "LCap" );
    LEndStyle->setEditable(false);
    LEndStyle->insertItem(loadIcon("ButtCap.png"), tr( "FlatCap" ) );
    LEndStyle->insertItem(loadIcon("SquareCap.png"), tr( "SquareCap" ) );
    LEndStyle->insertItem(loadIcon("RoundCap.png"), tr( "RoundCap" ) );
    Layout12_2->addWidget( LEndStyle, 7, 0 );
    pageLayout_5->addLayout( Layout12_2 );

		StyledLine = new QListBox(page_5, "StyledL");
		StyledLine->insertItem(tr("No Style"));
		pageLayout_5->addWidget(StyledLine);

    QSpacerItem* spacer11 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pageLayout_5->addItem( spacer11 );
    TabStack->addWidget( page_5, 4 );
    Cpal = new Cpalette(this);
    TabStack->addWidget(Cpal, 5);
    MpalLayout->addWidget( TabStack );
    QToolTip::add( Size, tr( "Font Size" ) );
    QToolTip::add( LineSp, tr( "Linespacing" ) );
    QToolTip::add( Extra, tr( "Manual Kerning" ) );
    QToolTip::add( Revert, tr( "Reverse Writing" ) );
    QToolTip::add( Underline, tr( "Underline" ) );
    QToolTip::add( Kapital, tr( "Small Caps" ) );
    QToolTip::add( Subs, tr( "Subscript" ) );
    QToolTip::add( Supers, tr( "Superscript" ) );
    QToolTip::add( Strike, tr( "Strikeout" ) );
    QToolTip::add( TextL, tr( "Align Text Left" ) );
    QToolTip::add( TextR, tr( "Align Text Right" ) );
    QToolTip::add( TextB, tr( "Align Text Block" ) );
    QToolTip::add( TextC, tr( "Align Text Center" ) );
    QToolTip::add( FlipH, tr( "Flip horizontal" ) );
    QToolTip::add( FlipV, tr( "Flip vertical" ) );
    QToolTip::add( Zup, tr( "Move one Level up" ) );
    QToolTip::add( ZDown, tr( "Move one Level down" ) );
    QToolTip::add( ZTop, tr( "Move to Front" ) );
    QToolTip::add( ZBottom, tr( "Move to Back" ) );
    connect(Xpos, SIGNAL(valueChanged(int)), this, SLOT(NewX()));
    connect(Ypos, SIGNAL(valueChanged(int)), this, SLOT(NewY()));
    connect(Width, SIGNAL(valueChanged(int)), this, SLOT(NewW()));
    connect(Height, SIGNAL(valueChanged(int)), this, SLOT(NewH()));
    connect(Rot, SIGNAL(valueChanged(int)), this, SLOT(NewR()));
    connect(RoundRect, SIGNAL(valueChanged(int)), this, SLOT(NewRR()));
    connect(LineSp, SIGNAL(valueChanged(int)), this, SLOT(NewLsp()));
    connect(Size, SIGNAL(valueChanged(int)), this, SLOT(NewSize()));
    connect(Extra, SIGNAL(valueChanged(int)), this, SLOT(NewExtra()));
    connect(ScaleX, SIGNAL(valueChanged(int)), this, SLOT(HChange()));
    connect(ScaleY, SIGNAL(valueChanged(int)), this, SLOT(VChange()));
    connect(LXpos, SIGNAL(valueChanged(int)), this, SLOT(NewLocalXY()));
    connect(LYpos, SIGNAL(valueChanged(int)), this, SLOT(NewLocalXY()));
    connect(LSize, SIGNAL(valueChanged(int)), this, SLOT(NewLS()));
    connect(LStyle, SIGNAL(activated(int)), this, SLOT(NewLSty()));
    connect(LJoinStyle, SIGNAL(activated(int)), this, SLOT(NewLJoin()));
    connect(LEndStyle, SIGNAL(activated(int)), this, SLOT(NewLEnd()));
    connect(LineMode, SIGNAL(activated(int)), this, SLOT(NewLMode()));
    connect(Kette, SIGNAL(clicked()), this, SLOT(ToggleKette()));
    connect(FlipH, SIGNAL(clicked()), this, SLOT(DoFlipH()));
    connect(FlipV, SIGNAL(clicked()), this, SLOT(DoFlipV()));
    connect(GroupAlign, SIGNAL(clicked(int)), this, SLOT(NewAli()));
    connect(Kapital, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Revert, SIGNAL(clicked()), this, SLOT(DoRevert()));
    connect(Underline, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Strike, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Supers, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Subs, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(FreeScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
    connect(FrameScale, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
    connect(Aspect, SIGNAL(clicked()), this, SLOT(ChangeScaling()));
    connect(Zup, SIGNAL(clicked()), this, SLOT(DoRaise()));
    connect(ZDown, SIGNAL(clicked()), this, SLOT(DoLower()));
    connect(ZTop, SIGNAL(clicked()), this, SLOT(DoFront()));
    connect(ZBottom, SIGNAL(clicked()), this, SLOT(DoBack()));
    connect(RotationGroup, SIGNAL(clicked(int)), this, SLOT(NewRotMode(int)));
    connect(Textflow, SIGNAL(clicked()), this, SLOT(DoFlow()));
    connect(SRect, SIGNAL(clicked()), this, SLOT(MakeRect()));
    connect(SOval, SIGNAL(clicked()), this, SLOT(MakeOval()));
    connect(SCustom, SIGNAL(clicked()), this, SLOT(MakeIrre()));
    connect(EditShape, SIGNAL(clicked()), this, SLOT(EditSh()));
    connect(DTop, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
    connect(DLeft, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
    connect(DRight, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
    connect(DBottom, SIGNAL(valueChanged(int)), this, SLOT(NewTDist()));
    connect(buttonGroup5, SIGNAL(clicked(int)), this, SLOT(SelTab(int)));
  	connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
  	connect(StyledLine, SIGNAL(selected(int)), this, SIGNAL(EditLSt()));
    connect(Fonts, SIGNAL(activated(int)), this, SLOT(NewTFont(int)));
		HaveItem = false;
		Xpos->setValue(0);
		Ypos->setValue(0);
		Width->setValue(0);
		Height->setValue(0);
		Rot->setValue(0);
		RoundRect->setValue(0);
		TabStack->raiseWidget(0);
		TabStack->widget(0)->setEnabled(false);
		SGeom->setEnabled(false);
		SShape->setEnabled(false);
		SText->setEnabled(false);
		SImage->setEnabled(false);
		SLine->setEnabled(false);
		SColor->setEnabled(false);
}


void Mpalette::closeEvent(QCloseEvent *ce)
{
	emit Schliessen();
	ce->accept();
}

void Mpalette::SelTab(int t)
{
	TabStack->raiseWidget(t);
}

void Mpalette::SetDoc(ScribusDoc *d)
{
	doc = d;
	HaveDoc = true;
	Xpos->setMaxValue( 300000 );
	Xpos->setMinValue( -300000 );
	Ypos->setMaxValue( 300000 );
	Ypos->setMinValue( -300000 );
	Width->setMaxValue( 300000 );
	Width->setMinValue( 100 );
	Height->setMaxValue( 300000 );
	Height->setMinValue( 100 );
	Rot->setMaxValue( 36000 );
	Rot->setMinValue( 0 );
	RoundRect->setMaxValue( 3000 );
	RoundRect->setMinValue( -3000 );
	LYpos->setMaxValue( 300000 );
	LYpos->setMinValue( -300000 );
	ScaleY->setMaxValue( 3000 );
	ScaleY->setMinValue( 1 );
	ScaleX->setMaxValue( 3000 );
	ScaleX->setMinValue( 1 );
	LXpos->setMaxValue( 300000 );
	LXpos->setMinValue( -300000 );
	Size->setMaxValue( 512 );
	Size->setMinValue( 1 );
	LineSp->setMaxValue( 3000 );
	LineSp->setMinValue( 1 );
	Extra->setMaxValue( 3000 );
	Extra->setMinValue( -3000 );
	LSize->setMaxValue( 120 );
	LSize->setMinValue( 1 );
  Xpos->setDecimals(100);
  Ypos->setDecimals(100);
  Width->setDecimals(100);
  Height->setDecimals(100);
  Rot->setDecimals(100);
  Extra->setDecimals(10);
  LineSp->setDecimals(10);
  LXpos->setDecimals(100);
  LYpos->setDecimals(100);
  ScaleX->setDecimals(10);
  ScaleY->setDecimals(10);
  DTop->setDecimals(10);
  DLeft->setDecimals(10);
  DBottom->setDecimals(10);
  DRight->setDecimals(10);
}

void Mpalette::UnsetDoc()
{
	HaveDoc = false;
}

void Mpalette::SetCurItem(PageItem *i)
{
 	disconnect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	HaveItem = false;
	CurItem = i;
	if (i->IFont != "");
		Fonts->setCurrentText(i->IFont);
//		Fonts->setText(i->IFont);
	RoundRect->setValue(qRound(i->RadRect));
  DLeft->setValue(static_cast<int>(i->Extra*10));
  DTop->setValue(static_cast<int>(i->TExtra*10));
  DBottom->setValue(static_cast<int>(i->BExtra*10));
  DRight->setValue(static_cast<int>(i->RExtra*10));
  Revert->setOn(i->Reverse);
	if (i->NamedLStyle == "")
		{
		StyledLine->setCurrentItem(0);
		LStyle->setEnabled(true);
		LSize->setEnabled(true);
		LJoinStyle->setEnabled(true);
		LEndStyle->setEnabled(true);
		}
	else
		{
		StyledLine->setSelected(StyledLine->findItem(i->NamedLStyle), true);
		LStyle->setEnabled(false);
		LSize->setEnabled(false);
		LJoinStyle->setEnabled(false);
		LEndStyle->setEnabled(false);
		}
  connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	if (i->Locked)
		{
		Xpos->setEnabled(false);
		Ypos->setEnabled(false);
		Width->setEnabled(false);
		Height->setEnabled(false);
		Rot->setEnabled(false);
		RoundRect->setEnabled(false);
		return;
		}
	else
		{
		Xpos->setEnabled(true);
		Ypos->setEnabled(true);
		Width->setEnabled(true);
		Height->setEnabled(true);
		Rot->setEnabled(true);
		RoundRect->setEnabled(true);
		}
	if (((i->PType == 4) || (i->PType == 2) || (i->PType == 3)) &&  (!i->ClipEdited))
		RoundRect->setEnabled(true);
	else
		RoundRect->setEnabled(false);
	if (i->PType == 5)
		{
		if (LMode)
			{
    	Text1->setText( tr( "X1:" ) );
    	Text2->setText( tr( "X2:" ) );
    	Text1a->setText( tr( "Y1:" ) );
    	Text2a->setText( tr( "Y2:" ) );
			Rot->setEnabled(false);
			Height->setEnabled(true);
			}
		else
			{
    	Text1->setText( tr( "X-Pos:" ) );
    	Text2->setText( tr( "Width:" ) );
    	Text1a->setText( tr( "Y-Pos:" ) );
    	Text2a->setText( tr( "Height:" ) );
			Rot->setEnabled(true);
			Height->setEnabled(false);
			}
		HaveItem = true;
		return;
		}
	else
		{
		if (i->PType == 2)
			{
			if (i->ScaleType)
				{
				FreeScale->setChecked(true);
				FrameScale->setChecked(false);
				Aspect->setEnabled(false);
				Aspect->setChecked(i->AspectRatio);
				}
			else
				{
				FreeScale->setChecked(false);
				FrameScale->setChecked(true);
				Kette->setOn(false);
				Aspect->setEnabled(true);
				Aspect->setChecked(i->AspectRatio);
				}
			}
		}
	HaveItem = true;
}

void Mpalette::NewSel(int nr)
{
	if (doc->ActPage->GroupSel)
		{
    Text1->setText( tr( "X-Pos:" ) );
    Text2->setText( tr( "Width:" ) );
    Text1a->setText( tr( "Y-Pos:" ) );
    Text2a->setText( tr( "Height:" ) );
		HaveItem = false;
		Xpos->setValue(0);
		Ypos->setValue(0);
		Width->setValue(0);
		Height->setValue(0);
		Rot->setValue(0);
		RoundRect->setValue(0);
		HaveItem = true;
		Xpos->setEnabled(true);
		Ypos->setEnabled(true);
		Width->setEnabled(true);
		Height->setEnabled(true);
		Rot->setEnabled(true);
		SGeom->setEnabled(true);
		TabStack->widget(0)->setEnabled(true);
		}
	else
		{
		FlipH->setEnabled(false);
		FlipV->setEnabled(false);
		ShapeGroup->setEnabled(false);
		RoundRect->setEnabled(false);
		Distance->setEnabled(false);
		EditShape->setEnabled(false);
		LineMode->setEnabled(false);
		TopLeft->setEnabled(true);
		TopRight->setEnabled(true);
		BottomLeft->setEnabled(true);
		BottomRight->setEnabled(true);
		Center->setEnabled(true);
		SGeom->setOn(true);
		TabStack->widget(0)->setEnabled(true);
		TabStack->widget(1)->setEnabled(false);
		TabStack->widget(2)->setEnabled(false);
		TabStack->widget(3)->setEnabled(false);
		TabStack->widget(4)->setEnabled(false);
		TabStack->widget(5)->setEnabled(true);
		SColor->setEnabled(true);
		switch (nr)
			{
			case -1:
    		Text1->setText( tr( "X-Pos:" ) );
    		Text2->setText( tr( "Width:" ) );
    		Text1a->setText( tr( "Y-Pos:" ) );
    		Text2a->setText( tr( "Height:" ) );
				Rot->setEnabled(true);
				Height->setEnabled(true);
				HaveItem = false;
				Xpos->setValue(0);
				Ypos->setValue(0);
				Width->setValue(0);
				Height->setValue(0);
				Rot->setValue(0);
				RoundRect->setValue(0);
				for (int ws = 0; ws < 6; ++ws)
					{
					TabStack->widget(ws)->setEnabled(false);
					}
				SGeom->setEnabled(false);
				SShape->setEnabled(false);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(false);
				SColor->setEnabled(false);
				break;
			case 1:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				ShapeGroup->setEnabled(true);
				EditShape->setEnabled(true);
				HaveItem = true;
				break;
			case 2:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(true);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(3)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				FlipH->setEnabled(true);
				FlipV->setEnabled(true);
				ShapeGroup->setEnabled(true);
				RoundRect->setEnabled(true);
				EditShape->setEnabled(true);
				HaveItem = true;
				break;
			case 3:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				ShapeGroup->setEnabled(true);
				RoundRect->setEnabled(true);
				EditShape->setEnabled(true);
				HaveItem = true;
				break;
			case 4:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(true);
				SImage->setEnabled(false);
				SLine->setEnabled(false);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(2)->setEnabled(true);
				FlipH->setEnabled(true);
				FlipV->setEnabled(true);
				ShapeGroup->setEnabled(true);
				RoundRect->setEnabled(true);
				Distance->setEnabled(true);
				EditShape->setEnabled(true);
				HaveItem = true;
				break;
			case 5:
				SGeom->setEnabled(true);
				SShape->setEnabled(false);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				LineMode->setEnabled(true);
				TopLeft->setEnabled(false);
				TopRight->setEnabled(false);
				BottomLeft->setEnabled(false);
				BottomRight->setEnabled(false);
				Center->setEnabled(false);
				HaveItem = true;
				break;
			case 6:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				ShapeGroup->setEnabled(true);
				EditShape->setEnabled(true);
				HaveItem = true;
				break;
			case 7:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(false);
				SImage->setEnabled(false);
				SLine->setEnabled(true);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				EditShape->setEnabled(true);
				HaveItem = true;
				break;
			case 8:
				SGeom->setEnabled(true);
				SShape->setEnabled(true);
				SText->setEnabled(true);
				SImage->setEnabled(false);
				SLine->setEnabled(false);
				TabStack->widget(1)->setEnabled(true);
				TabStack->widget(2)->setEnabled(true);
				TabStack->widget(4)->setEnabled(true);
				EditShape->setEnabled(true);
				HaveItem = true;
				break;
			}
		}
	updateGeometry();
	setFocus();
	repaint();
}

void Mpalette::UnitChange()
{
	float old = Umrech;
	bool tmp = HaveItem;
	HaveItem = false;
	QString ein;
	Umrech = UmReFaktor;
	switch (doc->Einheit)
		{
		case 0:
			ein = " pt";
			break;
		case 1:
    	ein = " mm";
			break;
		case 2:
    	ein = " in";
			break;
		case 3:
    	ein = " p";
			break;
		}
	Xpos->setSuffix( ein );
	Ypos->setSuffix( ein );
	Width->setSuffix( ein );
	Height->setSuffix( ein );
	LXpos->setSuffix( ein );
	LYpos->setSuffix( ein );
	Xpos->setValue(qRound(static_cast<float>(Xpos->value()) / old * UmReFaktor));
	Ypos->setValue(qRound(static_cast<float>(Ypos->value()) / old * UmReFaktor));
	Width->setValue(qRound(static_cast<float>(Width->value()) / old * UmReFaktor));
	Height->setValue(qRound(static_cast<float>(Height->value()) / old * UmReFaktor));
	LXpos->setValue(qRound(static_cast<float>(LXpos->value()) / old * UmReFaktor));
	LYpos->setValue(qRound(static_cast<float>(LYpos->value()) / old * UmReFaktor));
	HaveItem = tmp;
}

void Mpalette::setXY(float x, float y)
{
	bool tmp = HaveItem;
	HaveItem = false;
	Xpos->setValue(qRound(x*UmReFaktor*100));
	Ypos->setValue(qRound(y*UmReFaktor*100));
	if (LMode)
		setBH(CurItem->Width, CurItem->Height);
	HaveItem = tmp;
}

void Mpalette::setBH(float x, float y)
{
	bool tmp = HaveItem;
	HaveItem = false;
	QWMatrix ma;
	QPoint dp;
	if ((LMode) && (CurItem->PType == 5))
		{
		ma.translate(static_cast<float>(Xpos->value()) / UmReFaktor / 100, static_cast<float>(Ypos->value()) / UmReFaktor / 100);
		ma.rotate(static_cast<float>(Rot->value())/100*(-1));
		dp = ma * QPoint(static_cast<int>(x), static_cast<int>(y));
		Width->setValue(qRound(dp.x()*UmReFaktor*100));
		Height->setValue(qRound(dp.y()*UmReFaktor*100));
		}
	else
		{
		RoundRect->setMaxValue(static_cast<int>(QMIN(x, y)/2));
		Width->setValue(qRound(x*UmReFaktor*100));
		Height->setValue(qRound(y*UmReFaktor*100));
		}
	HaveItem = tmp;
}

void Mpalette::setR(float r)
{
	bool tmp = HaveItem;
	float rr = r;
	if (r > 0)
		rr = 360 - rr;
	HaveItem = false;
	Rot->setValue(qRound(fabs(rr)*100));
	HaveItem = tmp;
}

void Mpalette::setRR(float r)
{
	bool tmp = HaveItem;
	HaveItem = false;
	RoundRect->setValue(qRound(r));
	HaveItem = tmp;
}

void Mpalette::setRM(int r)
{
	bool tmp = HaveItem;
	HaveItem = false;
	RotationGroup->setButton(r);
	HaveItem = tmp;
}

void Mpalette::setLsp(float r)
{
	bool tmp = HaveItem;
	HaveItem = false;
	LineSp->setValue(qRound(r*10));
	HaveItem = tmp;
}

void Mpalette::setSize(int s)
{
	bool tmp = HaveItem;
	HaveItem = false;
	Size->setValue(s);
	HaveItem = tmp;
}

void Mpalette::setExtra(float e)
{
	bool tmp = HaveItem;
	HaveItem = false;
	Extra->setValue(qRound(e*10));
	HaveItem = tmp;
}

void Mpalette::ChangeScaling()
{
	if (FreeScale == sender())
		{
		FrameScale->setChecked(false);
		FreeScale->setChecked(true);
		Aspect->setEnabled(false);
		}
	if (FrameScale == sender())
		{
		FrameScale->setChecked(true);
		FreeScale->setChecked(false);
		Aspect->setEnabled(true);
		}
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->ScaleType = FreeScale->isChecked();
		CurItem->AspectRatio = Aspect->isChecked();
		if (!CurItem->ScaleType)
			doc->ActPage->AdjustPictScale(CurItem);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		setFocus();
		}
}

void Mpalette::setLvalue(float scx, float scy, float x, float y)
{
	bool tmp = HaveItem;
	HaveItem = false;
	LXpos->setValue(qRound(x * UmReFaktor * 100));
	LYpos->setValue(qRound(y * UmReFaktor * 100));
	ScaleX->setValue(qRound(scx * 1000));
	ScaleY->setValue(qRound(scy * 1000));
	HaveItem = tmp;
}

void Mpalette::setSvalue(float s)
{
	bool tmp = HaveItem;
	HaveItem = false;
	LSize->setValue(qRound(s*10));
	HaveItem = tmp;
}

void Mpalette::setLIvalue(PenStyle p, PenCapStyle pc, PenJoinStyle pj)
{
	bool tmp = HaveItem;
	HaveItem = false;
	switch (p)
		{
		case SolidLine:
			LStyle->setCurrentItem(0);
			break;
		case DashLine:
			LStyle->setCurrentItem(1);
			break;
		case DotLine:
			LStyle->setCurrentItem(2);
			break;
		case DashDotLine:
			LStyle->setCurrentItem(3);
			break;
		case DashDotDotLine:
			LStyle->setCurrentItem(4);
			break;
		default:
			LStyle->setCurrentItem(0);
			break;
		}
	switch (pc)
		{
		case FlatCap:
			LEndStyle->setCurrentItem(0);
			break;
		case SquareCap:
			LEndStyle->setCurrentItem(1);
			break;
		case RoundCap:
			LEndStyle->setCurrentItem(2);
			break;
		default:
			LEndStyle->setCurrentItem(0);
			break;
		}
	switch (pj)
		{
		case MiterJoin:
			LJoinStyle->setCurrentItem(0);
			break;
		case BevelJoin:
			LJoinStyle->setCurrentItem(1);
			break;
		case RoundJoin:
			LJoinStyle->setCurrentItem(2);
			break;
		default:
			LJoinStyle->setCurrentItem(0);
			break;
		}
	HaveItem = tmp;
}

void Mpalette::setStil(int s)
{
	Supers->setOn(false);
	Subs->setOn(false);
	Strike->setOn(false);
	Underline->setOn(false);
	Kapital->setOn(false);
	if (s & 1)
		Supers->setOn(true);
	if (s & 2)
		Subs->setOn(true);
	if (s & 8)
		Underline->setOn(true);
	if (s & 16)
		Strike->setOn(true);
	if (s & 64)
		Kapital->setOn(true);
}

void Mpalette::setAli(int e)
{
	bool tmp = HaveItem;
	HaveItem = false;
	switch (e)
		{
		case 0:
			TextL->setOn(true);
			break;
		case 2:
			TextR->setOn(true);
			break;
		case 1:
			TextC->setOn(true);
			break;
		case 3:
			TextB->setOn(true);
			break;
		case 4:
			TextF->setOn(true);
			break;
		default:
			TextL->setOn(false);
			TextR->setOn(false);
			TextC->setOn(false);
			TextB->setOn(false);
			TextF->setOn(false);
			break;
		}
	HaveItem = tmp;
}

void Mpalette::NewX()
{
	float x,y,w,h, gx, gy, gh, gw;
	x = static_cast<float>(Xpos->value()) / UmReFaktor / 100;
	y = static_cast<float>(Ypos->value()) / UmReFaktor / 100;
	w = static_cast<float>(Width->value()) / UmReFaktor / 100;
	h = static_cast<float>(Height->value()) / UmReFaktor / 100;
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			doc->ActPage->moveGroup(x - gx, 0, true);
			}
		else
			{
			if ((CurItem->PType == 5) && (LMode))
				{
				float r = atan2(h-y,w-x)*(180.0/3.1415927);
				w = sqrt(pow(w-x,2)+pow(h-y,2));
				doc->ActPage->MoveItem(x - CurItem->Xpos, 0, CurItem, true);
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
				doc->ActPage->RotateItem(r, CurItem->ItemNr);
				}
			else
				doc->ActPage->MoveItem(x - CurItem->Xpos, 0, CurItem, true);
			}
		emit DocChanged();
		}
}

void Mpalette::NewY()
{
	float x,y,w,h, gx, gy, gh, gw;
	x = static_cast<float>(Xpos->value()) / UmReFaktor / 100;
	y = static_cast<float>(Ypos->value()) / UmReFaktor / 100;
	w = static_cast<float>(Width->value()) / UmReFaktor / 100;
	h = static_cast<float>(Height->value()) / UmReFaktor / 100;
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			doc->ActPage->moveGroup(0, y - gy, true);
			}
		else
			{
			if ((CurItem->PType == 5) && (LMode))
				{
				float r = atan2(h-y,w-x)*(180.0/3.1415927);
				w = sqrt(pow(w-x,2)+pow(h-y,2));
				doc->ActPage->MoveItem(0, y - CurItem->Ypos, CurItem, true);
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
				doc->ActPage->RotateItem(r, CurItem->ItemNr);
				}
			else
				doc->ActPage->MoveItem(0, y - CurItem->Ypos, CurItem);
			}
		emit DocChanged();
		}
}

void Mpalette::NewW()
{
	float x,y,w,h, gx, gy, gh, gw;
	x = static_cast<float>(Xpos->value()) / UmReFaktor / 100;
	y = static_cast<float>(Ypos->value()) / UmReFaktor / 100;
	w = static_cast<float>(Width->value()) / UmReFaktor / 100;
	h = static_cast<float>(Height->value()) / UmReFaktor / 100;
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			doc->ActPage->HowTo = 1;
			doc->ActPage->scaleGroup(w / gw, w / gw);
			setBH(w, (w / gw) * gh);
			}
		else
			{
			CurItem->OldB2 = CurItem->Width;
			CurItem->OldH2 = CurItem->Height;
			if (CurItem->PType == 5)
				{
				if (LMode)
					{
					float r = atan2(h-y,w-x)*(180.0/3.1415927);
					doc->ActPage->RotateItem(r, CurItem->ItemNr);
					w = sqrt(pow(w-x,2)+pow(h-y,2));
					}
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
				}
			else
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
			emit DocChanged();
			}
		}
}

void Mpalette::NewH()
{
	float x,y,w,h, gx, gy, gh, gw;
	x = static_cast<float>(Xpos->value()) / UmReFaktor / 100;
	y = static_cast<float>(Ypos->value()) / UmReFaktor / 100;
	w = static_cast<float>(Width->value()) / UmReFaktor / 100;
	h = static_cast<float>(Height->value()) / UmReFaktor / 100;
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			{
			doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
			doc->ActPage->HowTo = 1;
			doc->ActPage->scaleGroup(h / gh, h / gh);
			setBH((h / gh) * gw, h);
			}
		else
			{
			CurItem->OldB2 = CurItem->Width;
			CurItem->OldH2 = CurItem->Height;
			if (CurItem->PType == 5)
				{
				if (LMode)
					{
					float r = atan2(h-y,w-x)*(180.0/3.1415927);
					doc->ActPage->RotateItem(r, CurItem->ItemNr);
					w = sqrt(pow(w-x,2)+pow(h-y,2));
					}
				doc->ActPage->SizeItem(w, CurItem->Height, CurItem->ItemNr, true);
				}
			else
				doc->ActPage->SizeItem(CurItem->Width, h, CurItem->ItemNr, true);
			}
		emit DocChanged();
		}
}

void Mpalette::NewR()
{
	if ((HaveDoc) && (HaveItem))
		{
		if (doc->ActPage->GroupSel)
			return;
		doc->ActPage->RotateItem(static_cast<float>(Rot->value())/100*(-1), CurItem->ItemNr);
		emit DocChanged();
		}
}

void Mpalette::NewRR()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->RadRect = RoundRect->value();
		doc->ActPage->SetFrameRounded();
		emit DocChanged();
		}
}

void Mpalette::NewLsp()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineSpa(static_cast<float>(LineSp->value())/10);
		emit DocChanged();
		}
}

void Mpalette::NewSize()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->chFSize(Size->value());
		emit DocChanged();
		}
}

void Mpalette::NewExtra()
{
	if ((HaveDoc) && (HaveItem))
		{
		if ((CurItem->HasSel) || (doc->AppMode == 1))
			{
			doc->ActPage->chKerning(static_cast<float>(Extra->value())/10);
			emit DocChanged();
			}
		else
			{
			if (uint(CurItem->CPos) != CurItem->Ptext.count())
				{
				CurItem->Ptext.at(CurItem->CPos)->cextra = static_cast<float>(Extra->value())/10;
				CurItem->Dirty = true;
				CurItem->paintObj();
				emit DocChanged();
				}
			}
		}
}

void Mpalette::NewLocalXY()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLocalXY(static_cast<float>(LXpos->value()) / UmReFaktor / 100, static_cast<float>(LYpos->value()) / UmReFaktor / 100);
		emit DocChanged();
		}
}

void Mpalette::NewLocalSC()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLocalSc(static_cast<float>(ScaleX->value()) / 1000, static_cast<float>(ScaleY->value()) / 1000);
		emit DocChanged();
		}
}

void Mpalette::NewLS()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineWidth(static_cast<float>(LSize->value())/10);
		emit DocChanged();
		}
}

void Mpalette::NewLSty()
{
	PenStyle c = SolidLine;
	switch (LStyle->currentItem())
		{
		case 0:
			c = SolidLine;
			break;
		case 1:
			c = DashLine;
			break;
		case 2:
			c = DotLine;
			break;
		case 3:
			c = DashDotLine;
			break;
		case 4:
			c = DashDotDotLine;
			break;
		}
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineArt(c);
		emit DocChanged();
		}
}

void Mpalette::NewLMode()
{
	if (LineMode->currentItem() == 0)
		{
    Text1->setText( tr( "X-Pos:" ) );
    Text2->setText( tr( "Width:" ) );
    Text1a->setText( tr( "Y-Pos:" ) );
    Text2a->setText( tr( "Height:" ) );
		Rot->setEnabled(true);
		Height->setEnabled(false);
		LMode = false;
		}
	else
		{
    Text1->setText( tr( "X1:" ) );
    Text2->setText( tr( "X2:" ) );
    Text1a->setText( tr( "Y1:" ) );
    Text2a->setText( tr( "Y2:" ) );
		Rot->setEnabled(false);
		Height->setEnabled(true);
		LMode = true;
		}
	setBH(CurItem->Width, CurItem->Height);
	updateGeometry();
	setFocus();
	repaint();
}

void Mpalette::NewLJoin()
{
	PenJoinStyle c = MiterJoin;
	switch (LJoinStyle->currentItem())
		{
		case 0:
			c = MiterJoin;
			break;
		case 1:
			c = BevelJoin;
			break;
		case 2:
			c = RoundJoin;
			break;
		}
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineJoin(c);
		emit DocChanged();
		}
}

void Mpalette::NewLEnd()
{
	PenCapStyle c = FlatCap;
	switch (LEndStyle->currentItem())
		{
		case 0:
			c = FlatCap;
			break;
		case 1:
			c = SquareCap;
			break;
		case 2:
			c = RoundCap;
			break;
		}
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ChLineEnd(c);
		emit DocChanged();
		}
}

void Mpalette::ToggleKette()
{
	if (Kette->isOn())
		{
		ScaleY->setValue(ScaleX->value());
		NewLocalSC();
		}	
}

void Mpalette::HChange()
{
	if (Kette->isOn())
		{
		ScaleY->setValue(ScaleX->value());
		}
	NewLocalSC();
}

void Mpalette::VChange()
{
	if (Kette->isOn())
		{
		ScaleX->setValue(ScaleY->value());
		}
	NewLocalSC();
}

void Mpalette::DoFlipH()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->FlipImageH();
		emit DocChanged();
		}
}

void Mpalette::DoFlipV()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->FlipImageV();
		emit DocChanged();
		}
}

void Mpalette::NewAli()
{
	if ((HaveDoc) && (HaveItem))
		{
		if (TextL->isOn())
			emit NewAbStyle(0);
		if (TextR->isOn())
			emit NewAbStyle(2);
		if (TextC->isOn())
			emit NewAbStyle(1);
		if (TextB->isOn())
			emit NewAbStyle(3);
		if (TextF->isOn())
			emit NewAbStyle(4);
		}
}

void Mpalette::setTypeStyle()
{
	if (Supers == sender())
		{
		emit Stellung(1);
		Subs->setOn(false);
		emit BackHome();
		return;
		}
	if (Subs == sender())
		{
		emit Stellung(2);
		Supers->setOn(false);
		emit BackHome();
		return;
		}
	if (Underline == sender())
		{
		emit Stellung(8);
		emit BackHome();
		return;
		}
	if (Strike == sender())
		{
		emit Stellung(16);
		emit BackHome();
		return;
		}
	if (Kapital == sender())
		{
		emit Stellung(64);
		emit BackHome();
		return;
		}
	emit Stellung(0);
}

void Mpalette::DoLower()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->LowerItem();
		emit DocChanged();
		}
}

void Mpalette::DoRaise()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->RaiseItem();
		emit DocChanged();
		}
}

void Mpalette::DoFront()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ToFront();
		emit DocChanged();
		}
}

void Mpalette::DoBack()
{
	if ((HaveDoc) && (HaveItem))
		{
		doc->ActPage->ToBack();
		emit DocChanged();
		}
}

void Mpalette::NewRotMode(int m)
{
	if ((HaveDoc) && (HaveItem))
		doc->RotMode = m;
}

void Mpalette::DoFlow()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->Textflow = Textflow->isChecked();
		doc->ActPage->update();
		emit DocChanged();
		}
}

void Mpalette::MakeRect()
{
	if ((HaveDoc) && (HaveItem))
		{
		if ((CurItem->PType == 7) || (CurItem->PType == 8))
			return;
		doc->ActPage->SetRectFrame(CurItem);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		if ((CurItem->PType == 2) || (CurItem->PType == 4))
			return;
		CurItem->PType = 3;
		NewSel(3);
		TabStack->raiseWidget(1);
		}
}

void Mpalette::MakeOval()
{
	if ((HaveDoc) && (HaveItem))
		{
		if ((CurItem->PType == 7) || (CurItem->PType == 8))
			return;
		doc->ActPage->SetOvalFrame(CurItem);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		if ((CurItem->PType == 2) || (CurItem->PType == 4))
			return;
		CurItem->PType = 6;
		NewSel(6);
		TabStack->raiseWidget(1);
		}
}

void Mpalette::MakeIrre()
{
	if ((HaveDoc) && (HaveItem))
		{
		if ((CurItem->PType == 2) || (CurItem->PType == 4) || (CurItem->PType == 7) || (CurItem->PType == 8))
			return;
		doc->ActPage->ToPolyFrame();
		NewSel(6);
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		TabStack->raiseWidget(1);
		}
}

void Mpalette::EditSh()
{
	if ((HaveDoc) && (HaveItem))
		{
		if ((CurItem->PType == 1) || (CurItem->PType == 3))
			MakeIrre();
		emit EditCL();
		emit DocChanged();
		}
}

void Mpalette::NewTDist()
{
	if ((HaveDoc) && (HaveItem))
		{
		CurItem->Extra = static_cast<float>(DLeft->value()) / 10;
		CurItem->TExtra = static_cast<float>(DTop->value()) / 10;
		CurItem->BExtra = static_cast<float>(DBottom->value()) / 10;
		CurItem->RExtra = static_cast<float>(DRight->value()) / 10;
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		}
}

void Mpalette::NewTFont(int c)
{
	if ((HaveDoc) && (HaveItem))
		{
		emit NewTF(Fonts->text(c));
		}
}

void Mpalette::DoRevert()
{
	if ((HaveDoc) && (HaveItem))
		{
		if (Revert->isOn())
			{
			CurItem->flippedH = 1;
			CurItem->Reverse = true;
			}
		else
			{
			CurItem->flippedH = 0;
			CurItem->Reverse = false;
			}
		doc->ActPage->RefreshItem(CurItem);
		emit DocChanged();
		}
}

void Mpalette::SetLineFormats(ScribusDoc *dd)
{
  disconnect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
	StyledLine->clear();
	if (dd != 0)
		{
		StyledLine->insertItem(tr("No Style"));
		QMap<QString,multiLine>::Iterator it;
		for (it = doc->MLineStyles.begin(); it != doc->MLineStyles.end(); ++it)
			{
			StyledLine->insertItem(it.key());
			}
		StyledLine->setSelected(StyledLine->currentItem(), false);
		}
  connect(StyledLine, SIGNAL(clicked(QListBoxItem*)), this, SLOT(SetSTline(QListBoxItem*)));
}

void Mpalette::SetSTline(QListBoxItem *c)
{
	if (c == NULL)
  	return;
  if (c->listBox()->currentItem() == 0)
		{
		CurItem->NamedLStyle = "";
		LStyle->setEnabled(true);
		LSize->setEnabled(true);
		LJoinStyle->setEnabled(true);
		LEndStyle->setEnabled(true);
		}
	else
		{
		CurItem->NamedLStyle = c->text();
		LStyle->setEnabled(false);
		LSize->setEnabled(false);
		LJoinStyle->setEnabled(false);
		LEndStyle->setEnabled(false);
		}
	doc->ActPage->RefreshItem(CurItem);
	emit DocChanged();
}
