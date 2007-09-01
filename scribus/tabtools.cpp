/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tabtools.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QStackedWidget>
#include <QLabel>
#include <QSpinBox>
#include <QToolButton>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QToolTip>

#include "sccombobox.h"
#include "colorcombo.h"
#include "units.h"
#include "scrspinbox.h"
#include "linecombo.h"
#include "fontcombo.h"
#include "polygonwidget.h"
#include "arrowchooser.h"
#include "linkbutton.h"
#include "scribusstructs.h"
#include "scribus.h"
#include "util_icon.h"
#include "prefsmanager.h"
#include "scfonts.h"
#include "sampleitem.h"
#include "commonstrings.h"
#include "colorlistbox.h"


TabTools::TabTools( QWidget* parent, struct toolPrefs *prefsData, int unitIndex, ScribusDoc* doc) : QWidget( parent, "tabtools", 0 )
{
	docu = doc;
	fontPreview = false;

	tabToolsLayout = new QHBoxLayout(this);
	tabToolsLayout->setMargin(0);
	tabToolsLayout->setSpacing(5);
	buttonGroupTools = new QGroupBox( this );
	buttonGroupToolsLayout = new QVBoxLayout(buttonGroupTools);
	buttonGroupToolsLayout->setMargin(5);
	buttonGroupToolsLayout->setSpacing(5);
	toolText = new QToolButton( buttonGroupTools );
	toolText->setCheckable( true );
	toolText->setAutoExclusive(true);
	toolText->setIconSize(QSize(16, 16));
	toolText->setIcon( QIcon( loadIcon("16/insert-text-frame.png") ) );
	buttonGroupToolsLayout->addWidget( toolText );
	toolImage = new QToolButton( buttonGroupTools );
	toolImage->setCheckable( true );
	toolImage->setAutoExclusive(true);
	toolImage->setIconSize(QSize(16, 16));
	toolImage->setIcon( QIcon( loadIcon("16/insert-image.png") ) );
	buttonGroupToolsLayout->addWidget( toolImage );
	toolShape = new QToolButton( buttonGroupTools );
	toolShape->setCheckable( true );
	toolShape->setAutoExclusive(true);
	toolShape->setIconSize(QSize(16, 16));
	toolShape->setIcon( QIcon( loadIcon("16/draw-rectangle.png") ) );
	buttonGroupToolsLayout->addWidget( toolShape);
	toolPoly = new QToolButton( buttonGroupTools );
	toolPoly->setCheckable( true );
	toolPoly->setAutoExclusive(true);
	toolPoly->setIconSize(QSize(16, 16));
	toolPoly->setIcon( QIcon( loadIcon("16/draw-polygon.png") ) );
	buttonGroupToolsLayout->addWidget( toolPoly );
	toolLine = new QToolButton( buttonGroupTools );
	toolLine->setCheckable( true );
	toolLine->setAutoExclusive(true);
	toolLine->setIconSize(QSize(16, 16));
	toolLine->setIcon( QIcon( loadIcon("Stift.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolLine );
	toolZoom = new QToolButton( buttonGroupTools );
	toolZoom->setCheckable( true );
	toolZoom->setAutoExclusive(true);
	toolZoom->setIconSize(QSize(16, 16));
	toolZoom->setIcon( QIcon( loadIcon("16/zoom.png") ) );
	buttonGroupToolsLayout->addWidget( toolZoom );
	toolMisc = new QToolButton( buttonGroupTools );
	toolMisc->setCheckable( true );
	toolMisc->setAutoExclusive(true);
	toolMisc->setIconSize(QSize(16, 16));
	toolMisc->setIcon( QIcon( loadIcon("configure.png") ) );
	buttonGroupToolsLayout->addWidget( toolMisc );
	QSpacerItem *spacer = new QSpacerItem( 1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	buttonGroupToolsLayout->addItem( spacer );
	tabToolsLayout->addWidget( buttonGroupTools );

	subStackTools = new QStackedWidget( this );
	subStackTools->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	subStackTools->setFrameShape( QFrame::StyledPanel );
	subStackTools->setFrameShadow( QFrame::Sunken );
	QFont f(this->font());
	f.setPointSize(f.pointSize()+3);
	f.setBold(true);

	subTabText = new QWidget( subStackTools, "subTabText" );
	subTabTextLayout = new QGridLayout( subTabText );
	subTabTextLayout->setMargin(10);
	subTabTextLayout->setSpacing(5);
	subTabTextLayout->setAlignment( Qt::AlignTop );
	textHeadLine = new QLabel( tr( "Text" ), subTabText, "textHeadLine" );
	textHeadLine->setFont(f);
	subTabTextLayout->addWidget( textHeadLine, 0, 0, 1, 4, Qt::AlignHCenter | Qt::AlignTop );
	fontComboText = new FontCombo(subTabText);
	subTabTextLayout->addWidget( fontComboText, 1, 1, 1, 2, Qt::AlignLeft );
	textLabel1b = new QLabel( fontComboText, tr( "Font:" ), subTabText, "textLabel1b" );
	subTabTextLayout->addWidget( textLabel1b, 1, 0 );
	sizeComboText = new ScComboBox( subTabText );

	subTabTextLayout->addWidget( sizeComboText, 2, 1, 1, 2, Qt::AlignLeft );
	textLabel2b = new QLabel(sizeComboText, tr( "Size:" ), subTabText, "textLabel2b" );
	subTabTextLayout->addWidget( textLabel2b, 2, 0 );

	colorComboText = new ColorCombo( false, subTabText );

	subTabTextLayout->addWidget( colorComboText, 3, 1 );
	textLabel3b = new QLabel(colorComboText, tr( "Text Color:" ), subTabText, "textLabel3b" );
	subTabTextLayout->addWidget( textLabel3b, 3, 0 );
	shadingText = new QSpinBox( subTabText, "shadingText" );
	shadingText->setMaximum( 100 );
	shadingText->setSuffix( tr( " %" ) );
	shadingText->setMinimum( 0 );
	subTabTextLayout->addWidget( shadingText, 3, 3, Qt::AlignLeft );
	textLabelTS = new QLabel( shadingText, tr( "Shading:" ), subTabText, "textLabelTS" );
	subTabTextLayout->addWidget( textLabelTS, 3, 2 );

	colorComboStrokeText = new ColorCombo( false, subTabText );

	subTabTextLayout->addWidget( colorComboStrokeText, 4, 1 );
	textLabel3b2 = new QLabel(colorComboStrokeText, tr( "Text Stroke:" ), subTabText, "textLabel3b2" );
	subTabTextLayout->addWidget( textLabel3b2, 4, 0 );
	shadingTextStroke = new QSpinBox( subTabText, "shadingTextStroke" );
	shadingTextStroke->setMaximum( 100 );
	shadingTextStroke->setSuffix( tr( " %" ) );
	shadingTextStroke->setMinimum( 0 );

	subTabTextLayout->addWidget( shadingTextStroke, 4, 3, Qt::AlignLeft );
	textLabelTSS = new QLabel( shadingTextStroke, tr( "Shading:" ), subTabText, "textLabelTS" );
	subTabTextLayout->addWidget( textLabelTSS, 4, 2 );

	colorComboTextBackground = new ColorCombo( false, subTabText );

	subTabTextLayout->addWidget( colorComboTextBackground, 5, 1 );
	textLabel3bT = new QLabel(colorComboTextBackground, tr( "Fill Color:" ), subTabText, "textLabel3b" );
	subTabTextLayout->addWidget( textLabel3bT, 5, 0 );
	shadingTextBack = new QSpinBox( subTabText, "shadingTextBack" );
	shadingTextBack->setMaximum( 100 );
	shadingTextBack->setSuffix( tr( " %" ) );
	shadingTextBack->setMinimum( 0 );

	subTabTextLayout->addWidget( shadingTextBack, 5, 3, Qt::AlignLeft );
	textLabelTSB = new QLabel( shadingTextBack, tr( "Shading:" ), subTabText, "textLabelTSS" );
	subTabTextLayout->addWidget( textLabelTSB, 5, 2 );

	colorComboTextLine = new ColorCombo( false, subTabText );

	subTabTextLayout->addWidget( colorComboTextLine, 6, 1 );
	textLabel3bTL = new QLabel(colorComboTextLine, tr( "Stroke Color:" ), subTabText, "textLabel3b2" );
	subTabTextLayout->addWidget( textLabel3bTL, 6, 0 );
	shadingTextLine = new QSpinBox( subTabText, "shadingTextStroke" );
	shadingTextLine->setMaximum( 100 );
	shadingTextLine->setSuffix( tr( " %" ) );
	shadingTextLine->setMinimum( 0 );

	subTabTextLayout->addWidget( shadingTextLine, 6, 3, Qt::AlignLeft );
	textLabelTSL = new QLabel( shadingTextLine, tr( "Shading:" ), subTabText, "textLabelTSL" );
	subTabTextLayout->addWidget( textLabelTSL, 6, 2 );

	tabFillCombo = new ScComboBox( subTabText );

	subTabTextLayout->addWidget( tabFillCombo, 7, 1, Qt::AlignLeft );
	textLabel3b2t = new QLabel(tabFillCombo, tr( "Tab Fill Character:" ), subTabText, "textLabel3b2t" );
	subTabTextLayout->addWidget( textLabel3b2t, 7, 0 );
	gapTab = new ScrSpinBox( 1, 200, subTabText, unitIndex );

	subTabTextLayout->addWidget( gapTab, 7, 3, Qt::AlignLeft );
	textLabel3b2t2 = new QLabel(gapTab, tr( "Tab Width:" ), subTabText, "textLabel3b2t2" );
	subTabTextLayout->addWidget( textLabel3b2t2, 7, 2 );
	columnsText = new QSpinBox( subTabText, "columnsText" );
	columnsText->setMinimum( 1 );
	columnsText->setMaximum(100);

	subTabTextLayout->addWidget( columnsText, 8, 1, Qt::AlignLeft );
	textLabel4b = new QLabel(columnsText, tr("Colu&mns:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel4b, 8, 0 );
	gapText = new ScrSpinBox( 0, 200, subTabText, unitIndex );

	subTabTextLayout->addWidget( gapText, 8, 3, Qt::AlignLeft );
	textLabel5b = new QLabel(gapText, tr("&Gap:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel5b, 8, 2 );
	previewText = new QLabel( tr( "Woven silk pyjamas exchanged for blue quartz" ), subTabText, "previewText" );
	previewText->setMinimumSize(QSize(400, 170));
	previewText->setMaximumSize(QSize(400, 170));
	previewText->setFrameShape(QFrame::Box);
	previewText->setAlignment( Qt::AlignTop | Qt::AlignLeft );
	subTabTextLayout->addWidget( previewText, 9, 0, 1, 4 );
	subStackTools->addWidget( subTabText );

	subTabShape = new QWidget( subStackTools, "subTabShape" );
	subTabShapeLayout = new QGridLayout( subTabShape );
	subTabShapeLayout->setMargin(10);
	subTabShapeLayout->setSpacing(5);
	subTabShapeLayout->setAlignment( Qt::AlignTop );
	shapeHeadLine = new QLabel( tr( "Shapes" ), subTabShape, "shapeHeadLine" );
	shapeHeadLine->setFont(f);
	subTabShapeLayout->addWidget( shapeHeadLine, 0, 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop );
	colorComboLineShape = new ColorCombo( false, subTabShape );

	subTabShapeLayout->addWidget( colorComboLineShape, 1, 1, Qt::AlignLeft );
	textLabel7b = new QLabel( colorComboLineShape, tr( "&Line Color:" ), subTabShape, "textLabel7b" );
	subTabShapeLayout->addWidget( textLabel7b, 1, 0 );
	shadingLineShape = new QSpinBox( subTabShape, "shadingLineShape" );
	shadingLineShape->setMaximum( 100 );
	shadingLineShape->setSuffix( tr( " %" ) );
	shadingLineShape->setMinimum( 0 );

	subTabShapeLayout->addWidget( shadingLineShape, 2, 1, Qt::AlignLeft );
	textLabel8b = new QLabel( shadingLineShape, tr( "&Shading:" ), subTabShape, "textLabel8b" );
	subTabShapeLayout->addWidget( textLabel8b, 2, 0 );
	comboFillShape = new ColorCombo( false, subTabShape );

	subTabShapeLayout->addWidget( comboFillShape, 3, 1, Qt::AlignLeft );
	textLabel9b = new QLabel( comboFillShape, tr( "&Fill Color:" ), subTabShape, "textLabel9b" );
	subTabShapeLayout->addWidget( textLabel9b, 3, 0 );
	shadingFillShape = new QSpinBox( subTabShape, "shadingFillShape" );
	shadingFillShape->setMaximum( 100 );
	shadingFillShape->setSuffix( tr( " %" ) );
	shadingFillShape->setMinimum( 0 );

	subTabShapeLayout->addWidget( shadingFillShape, 4, 1, Qt::AlignLeft );
	textLabel10b = new QLabel( shadingFillShape, tr( "S&hading:" ), subTabShape, "textLabel10b" );
	subTabShapeLayout->addWidget( textLabel10b, 4, 0 );
	comboStyleShape = new LineCombo(subTabShape);
	comboStyleShape->setEditable(false);

	subTabShapeLayout->addWidget( comboStyleShape, 5, 1, Qt::AlignLeft );
	textLabel11b = new QLabel( comboStyleShape, tr( "Line Style:" ), subTabShape, "textLabel11b" );
	subTabShapeLayout->addWidget( textLabel11b, 5, 0 );
	lineWidthShape = new ScrSpinBox( 0, 36, subTabShape, 1 );
	lineWidthShape->setDecimals(1);
	lineWidthShape->setSuffix( tr( " pt" ) );

	subTabShapeLayout->addWidget( lineWidthShape, 6, 1, Qt::AlignLeft );
	textLabel12b = new QLabel( lineWidthShape, tr( "Line &Width:" ), subTabShape, "TextLabel2_3_4" );
	subTabShapeLayout->addWidget( textLabel12b, 6, 0 );
	subStackTools->addWidget( subTabShape );

	subTabLine = new QWidget( subStackTools, "subTabLine" );
	subTabLineLayout = new QGridLayout( subTabLine );
	subTabLineLayout->setMargin(10);
	subTabLineLayout->setSpacing(5);
	subTabLineLayout->setAlignment( Qt::AlignTop );
	lineHeadLine = new QLabel( tr( "Lines" ), subTabLine, "lineHeadLine" );
	lineHeadLine->setFont(f);
	subTabLineLayout->addWidget( lineHeadLine, 0, 0, 1, 3, Qt::AlignHCenter | Qt::AlignTop );
	colorComboLine = new ColorCombo( false, subTabLine );

	subTabLineLayout->addWidget( colorComboLine, 1, 1, Qt::AlignLeft );
	textLabel13b = new QLabel(colorComboLine, tr("&Line Color:"), subTabLine, "textLabel13b" );
	subTabLineLayout->addWidget( textLabel13b, 1, 0 );
	shadingLine = new QSpinBox( subTabLine, "shadingLine" );
	shadingLine->setMaximum( 100 );

	shadingLine->setSuffix( tr( " %" ) );
	subTabLineLayout->addWidget( shadingLine, 2, 1, Qt::AlignLeft );
	textLabel14b = new QLabel(shadingLine, tr("&Shading:"), subTabLine, "textLabel14b" );
	subTabLineLayout->addWidget( textLabel14b, 2, 0 );
	comboStyleLine = new LineCombo(subTabLine);
	comboStyleLine->setEditable(false);

	subTabLineLayout->addWidget( comboStyleLine, 3, 1, Qt::AlignLeft );
	textLabel15b = new QLabel(subTabLine, tr("Line S&tyle:"), subTabLine, "textLabel15b" );
	subTabLineLayout->addWidget( textLabel15b, 3, 0 );
	startArrow = new ArrowChooser(subTabLine, true);
	subTabLineLayout->addWidget( startArrow, 5, 1 );
	endArrow = new ArrowChooser(subTabLine, false);
	subTabLineLayout->addWidget( endArrow, 5, 2 );

	arrowText = new QLabel( tr( "Arrows:" ), subTabLine, "arrowText" );
	subTabLineLayout->addWidget( arrowText, 4, 0, 2, 1 );
	startArrowText = new QLabel( startArrow, tr( "Start:" ), subTabLine, "startArrowText" );
	subTabLineLayout->addWidget( startArrowText, 4, 1 );
	endArrowText = new QLabel( endArrow, tr( "End:" ), subTabLine, "endArrowText" );
	subTabLineLayout->addWidget( endArrowText, 4, 2 );
	lineWidthLine = new ScrSpinBox( 1, 36, subTabLine, 1 );
	lineWidthLine->setDecimals(1);
	lineWidthLine->setSuffix( tr( " pt" ) );

	subTabLineLayout->addWidget( lineWidthLine, 6, 1, Qt::AlignLeft );
	textLabel16b = new QLabel(lineWidthLine, tr("Line &Width:"), subTabLine, "textLabel16b" );
	subTabLineLayout->addWidget( textLabel16b, 6, 0 );
	subStackTools->addWidget( subTabLine );

	subTabImage = new QWidget( subStackTools, "subTabImage" );
	subTabImageLayout = new QGridLayout( subTabImage );
	subTabImageLayout->setMargin(10);
	subTabImageLayout->setSpacing(5);
	subTabImageLayout->setAlignment( Qt::AlignTop );
	imageHeadLine = new QLabel( tr( "Images" ), subTabImage, "imageHeadLine" );
	imageHeadLine->setFont(f);
	subTabImageLayout->addWidget( imageHeadLine, 0, 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop );

	buttonGroup3 = new QGroupBox( subTabImage );
	buttonGroup3->setCheckable( true );
	buttonGroup3->setTitle( tr( "&Free Scaling" ) );
	buttonGroup3Layout = new QGridLayout( buttonGroup3 );
	buttonGroup3Layout->setMargin(10);
	buttonGroup3Layout->setSpacing(5);
	buttonGroup3Layout->setAlignment( Qt::AlignTop );
	scalingHorizontal = new QSpinBox( buttonGroup3, "scalingHorizontal" );
	scalingHorizontal->setMaximum( 1000 );
	scalingHorizontal->setMinimum( 1 );

	scalingHorizontal->setSuffix( tr( " %" ) );
	buttonGroup3Layout->addWidget( scalingHorizontal, 0, 1, Qt::AlignRight );
	textLabel17b = new QLabel(scalingHorizontal, tr("&Horizontal Scaling:"), buttonGroup3, "textLabel17b" );
	buttonGroup3Layout->addWidget( textLabel17b, 0, 0 );
	scalingVertical = new QSpinBox( buttonGroup3, "scalingVertical" );
	scalingVertical->setMaximum( 1000 );
	scalingVertical->setMinimum( 1 );
	scalingVertical->setSuffix( tr( " %" ) );

	buttonGroup3Layout->addWidget( scalingVertical, 1, 1, Qt::AlignRight );
	textLabel18b = new QLabel(scalingVertical, tr( "&Vertical Scaling:" ), buttonGroup3, "textLabel18b" );
	buttonGroup3Layout->addWidget( textLabel18b, 1, 0 );
	chainButton = new LinkButton( buttonGroup3 );
	chainButton->setToggleButton( true );
	chainButton->setAutoRaise(true);
	buttonGroup3Layout->addWidget( chainButton, 0, 2, 2, 1, Qt::AlignLeft );
	subTabImageLayout->addWidget( buttonGroup3, 1, 0, 1, 2 );

	buttonGroup5 = new QGroupBox( subTabImage, "buttonGroup5" );
	buttonGroup5->setCheckable( true );
	buttonGroup5->setTitle( tr( "&Scale Picture to Frame Size" ) );
	buttonGroup5Layout = new QHBoxLayout( buttonGroup5 );
	buttonGroup5Layout->setMargin(10);
	buttonGroup5Layout->setSpacing(5);
	buttonGroup5Layout->setAlignment( Qt::AlignTop );
	checkRatioImage = new QCheckBox( buttonGroup5, "checkRatioImage" );
	checkRatioImage->setText( tr( "Keep Aspect &Ratio" ) );
	buttonGroup5Layout->addWidget( checkRatioImage );
	subTabImageLayout->addWidget( buttonGroup5, 2, 0, 1, 2 );
	
	comboFillImage = new ColorCombo( false, subTabImage );
	subTabImageLayout->addWidget( comboFillImage, 3, 1, Qt::AlignLeft );
	textLabel19b = new QLabel(comboFillImage, tr( "F&ill Color:" ), subTabImage, "textLabel19b" );
	subTabImageLayout->addWidget( textLabel19b, 3, 0 );
	shadingFillImage = new QSpinBox( subTabImage, "shadingFillImage" );
	shadingFillImage->setMaximum( 100 );

	shadingFillImage->setSuffix( tr( " %" ) );
	subTabImageLayout->addWidget( shadingFillImage, 4, 1, Qt::AlignLeft );
	textLabel20b = new QLabel(shadingFillImage, tr( "S&hading:" ), subTabImage, "textLabel20b" );
	subTabImageLayout->addWidget( textLabel20b, 4, 0 );
	embeddedPath = new QCheckBox( subTabImage, "embeddedPath" );
	embeddedPath->setText( tr( "Use embedded Clipping Path" ) );

	subTabImageLayout->addWidget( embeddedPath, 5, 0, 1, 2 );
	buttonGroupRes = new QGroupBox( subTabImage, "buttonGroup3" );
	buttonGroupRes->setTitle( tr( "On Screen Preview" ) );
	buttonGroupResLayout = new QVBoxLayout( buttonGroupRes );
	buttonGroupResLayout->setSpacing( 5 );
	buttonGroupResLayout->setMargin( 10 );
	buttonGroupResLayout->setAlignment( Qt::AlignTop );
	checkFullRes = new QRadioButton( buttonGroupRes, "checkFullRes" );
	checkFullRes->setText( tr( "Full Resolution Preview" ) );
	buttonGroupResLayout->addWidget( checkFullRes );
	checkNormalRes = new QRadioButton( buttonGroupRes, "checkNormalRes" );
	checkNormalRes->setText( tr( "Normal Resolution Preview" ) );
	buttonGroupResLayout->addWidget( checkNormalRes );
	checkHalfRes = new QRadioButton( buttonGroupRes, "checkHalfRes" );
	checkHalfRes->setText( tr( "Low Resolution Preview" ) );
	buttonGroupResLayout->addWidget( checkHalfRes );

	subTabImageLayout->addWidget( buttonGroupRes, 6, 0, 1, 2 );
	subStackTools->addWidget( subTabImage );

	subTabPolygon = new QWidget( subStackTools, "subTabPolygon" );
	subTabPolygonLayout = new QGridLayout( subTabPolygon );
	subTabPolygonLayout->setSpacing( 5 );
	subTabPolygonLayout->setMargin( 10 );
	subTabPolygonLayout->setAlignment( Qt::AlignTop );
	polygonHeadLine = new QLabel( tr( "Regular Polygons" ), subTabPolygon, "imageHeadLine" );
	polygonHeadLine->setFont(f);
	subTabPolygonLayout->addWidget( polygonHeadLine, 0, 0, Qt::AlignHCenter | Qt::AlignTop );
	polyWidget = new PolygonWidget(subTabPolygon, prefsData->polyC, prefsData->polyFd, prefsData->polyF, prefsData->polyS, prefsData->polyR);
	subTabPolygonLayout->addWidget( polyWidget, 1, 0 );
	subStackTools->addWidget( subTabPolygon );

	subTabZoom = new QWidget( subStackTools, "subTabZoom" );
	subTabZoomLayout = new QGridLayout( subTabZoom );
	subTabZoomLayout->setSpacing( 5 );
	subTabZoomLayout->setMargin( 10 );
	subTabZoomLayout->setAlignment( Qt::AlignTop );
	zoomHeadLine = new QLabel( tr( "Zoom" ), subTabZoom, "zoomHeadLine" );
	zoomHeadLine->setFont(f);
	subTabZoomLayout->addWidget( zoomHeadLine, 0, 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop );
	minimumZoom = new QSpinBox( subTabZoom, "minimumZoom" );
	minimumZoom->setMaximum( 3200 );
	minimumZoom->setMinimum( 10 );
	minimumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( minimumZoom, 1, 1, Qt::AlignLeft );
	textLabel21b = new QLabel( minimumZoom, tr( "Mi&nimum:" ), subTabZoom, "textLabel21b" );
	subTabZoomLayout->addWidget( textLabel21b, 1, 0);

	maximumZoom = new QSpinBox( subTabZoom, "maximumZoom" );
	maximumZoom->setMaximum( 3200 );
	maximumZoom->setMinimum( 10 );
	maximumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( maximumZoom, 2, 1, Qt::AlignLeft );
	textLabel22b = new QLabel( maximumZoom, tr( "Ma&ximum:" ), subTabZoom, "textLabel22b" );
	subTabZoomLayout->addWidget( textLabel22b, 2, 0 );
	
	zoomStep = new QSpinBox( 101, 500, 1, subTabZoom, "zoomStep" );
	zoomStep->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( zoomStep, 3, 1, Qt::AlignLeft );
	textLabel23b = new QLabel( zoomStep, tr( "&Stepping:" ), subTabZoom, "textLabel23b" );
	subTabZoomLayout->addWidget( textLabel23b, 3, 0 );
	subStackTools->addWidget( subTabZoom );

	subTabGeneral = new QWidget( subStackTools, "subTabGeneral" );
	subTabGeneralLayout = new QGridLayout( subTabGeneral, 1, 1, 11, 6, "subTabGeneralLayout");
	subTabGeneralLayout->setSpacing( 5 );
	subTabGeneralLayout->setMargin( 10 );
	subTabGeneralLayout->setAlignment( Qt::AlignTop );
	generalHeadLine = new QLabel( tr( "Miscellaneous Settings" ), subTabGeneral, "generalHeadLine" );
	generalHeadLine->setFont(f);
	subTabGeneralLayout->addWidget( generalHeadLine, 0, 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop );

	genDispBox = new QGroupBox( subTabGeneral, "genDispBox" );
	genDispBox->setTitle( tr( "Item Duplicate" ) );
	subTabGeneralLayout2 = new QGridLayout( genDispBox );
	subTabGeneralLayout2->setSpacing( 5 );
	subTabGeneralLayout2->setMargin( 10 );

	genDispX = new ScrSpinBox( -1000, 1000, genDispBox, unitIndex );
//	genDispX->setSuffix( tr( " pt" ) );
	subTabGeneralLayout2->addWidget( genDispX, 0, 1, Qt::AlignLeft );
	genText1 = new QLabel( genDispX, tr( "X Displacement" ), genDispBox, "genText1" );
	subTabGeneralLayout2->addWidget( genText1, 0, 0);

	genDispY = new ScrSpinBox( -1000, 1000, genDispBox, unitIndex );
//	genDispY->setSuffix( tr( " pt" ) );
	subTabGeneralLayout2->addWidget( genDispY, 1, 1, Qt::AlignLeft );
	genText2 = new QLabel( genDispY, tr( "Y Displacement" ), genDispBox, "genText2" );
	subTabGeneralLayout2->addWidget( genText2, 1, 0);
	subTabGeneralLayout->addWidget( genDispBox, 1, 0);

	genRotBox = new QGroupBox( subTabGeneral, "genRotBox" );
	genRotBox->setTitle( tr( "Rotation Tool" ) );
	subTabGeneralLayout3 = new QGridLayout( genRotBox );
	subTabGeneralLayout3->setSpacing( 5 );
	subTabGeneralLayout3->setMargin( 10 );

	genRot = new ScrSpinBox( 1, 90, genRotBox, 1 );
	genRot->setDecimals(1);
	genRot->setSuffix("");
	subTabGeneralLayout3->addWidget( genRot, 0, 1, Qt::AlignLeft );
	genText3 = new QLabel( genRot, tr( "Constrain to:" ), genRotBox, "genText3" );
	subTabGeneralLayout3->addWidget( genText3, 0, 0);
	genText4 = new QLabel( tr( "Degrees" ), genRotBox, "genText4" );
	subTabGeneralLayout3->addWidget( genText4, 0, 2);
	subTabGeneralLayout->addWidget( genRotBox, 1, 1);

	subStackTools->addWidget( subTabGeneral );

	tabToolsLayout->addWidget( subStackTools );
	toolText->setChecked(true);

	// Neccessary for document prefs
	//restoreDefaults(prefsData, unitIndex);

	toolText->setToolTip( tr( "Text Frame Properties" ) );
	toolImage->setToolTip( tr( "Picture Frame Properties" ) );
	toolShape->setToolTip( tr( "Shape Drawing Properties" ) );
	toolZoom->setToolTip( tr( "Magnification Level Defaults" ) );
	toolLine->setToolTip( tr( "Line Drawing Properties" ) );
	toolPoly->setToolTip( tr( "Polygon Drawing Properties" ) );
	toolMisc->setToolTip( tr( "Other Properties" ) );
	fontComboText->setToolTip( tr( "Font for new text frames" ) );
	sizeComboText->setToolTip( tr( "Size of font for new text frames" ) );
	colorComboText->setToolTip( tr( "Color of font" ) );
	columnsText->setToolTip( tr( "Number of columns in a text frame" ) );
	gapText->setToolTip( tr( "Gap between text frame columns" ) );
	previewText->setToolTip( tr( "Sample of your font" ) );
	buttonGroup3->setToolTip( tr( "Picture frames allow pictures to scale to any size" ) );
	scalingHorizontal->setToolTip( tr( "Horizontal scaling of images" ) );
	scalingVertical->setToolTip( tr( "Vertical scaling of images" ) );
	chainButton->setToolTip( tr( "Keep horizontal and vertical scaling the same" ) );
	buttonGroup5->setToolTip( tr( "Pictures in picture frames are scaled to the size of the frame" ) );
	checkRatioImage->setToolTip( tr( "Automatically scaled pictures keep their original proportions" ) );
	comboFillImage->setToolTip( tr( "Fill color of picture frames" ) );
	shadingFillImage->setToolTip( tr( "Saturation of color of fill" ) );
	colorComboLineShape->setToolTip( tr( "Line color of shapes" ) );
	shadingLineShape->setToolTip( tr( "Saturation of color of lines" ) );
	comboFillShape->setToolTip( tr( "Fill color of shapes" ) );
	shadingFillShape->setToolTip( tr( "Saturation of color of fill" ) );
	comboStyleShape->setToolTip( tr( "Line style of shapes" ) );
	lineWidthShape->setToolTip( tr( "Line width of shapes" ) );
	minimumZoom->setToolTip( tr( "Minimum magnification allowed" ) );
	maximumZoom->setToolTip( tr( "Maximum magnification allowed" ) );
	zoomStep->setToolTip( tr( "Change in magnification for each zoom operation" ) );
	colorComboLine->setToolTip( tr( "Color of lines" ) );
	shadingLine->setToolTip( tr( "Saturation of color" ) );
	comboStyleLine->setToolTip( tr( "Style of lines" ) );
	lineWidthLine->setToolTip( tr( "Width of lines" ) );
	genDispX->setToolTip( tr( "Horizontal displacement of page items") );
	genDispY->setToolTip( tr( "Vertical displacement of page items" ) );
	genRot->setToolTip( tr( "Constrain value for the rotation tool when the Control key is pressed" ) );
	embeddedPath->setToolTip( "<qt>" + tr( "Use the embedded clipping paths in images when importing them. JPEG, PSD and TIFF are the image formats which can embedded clipping paths." ) + "</qt>"  );
	//enableSignals(true);
}

void TabTools::enableSignals(bool on)
{
	if (on)
	{
		connect(toolShape, SIGNAL(clicked()), this, SLOT(setTool()));
		connect(toolPoly, SIGNAL(clicked()), this, SLOT(setTool()));
		connect(toolImage, SIGNAL(clicked()), this, SLOT(setTool()));
		connect(toolText, SIGNAL(clicked()), this, SLOT(setTool()));
		connect(toolLine, SIGNAL(clicked()), this, SLOT(setTool()));
		connect(toolZoom, SIGNAL(clicked()), this, SLOT(setTool()));
		connect(toolMisc, SIGNAL(clicked()), this, SLOT(setTool()));
		connect(fontComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
		connect(sizeComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
		connect(colorComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
		connect(colorComboTextBackground, SIGNAL(activated(int)), this, SLOT(setSample()));
		connect(shadingTextBack, SIGNAL(valueChanged(int)), this, SLOT(setSample()));
		connect(shadingText, SIGNAL(valueChanged(int)), this, SLOT(setSample()));
		connect(buttonGroup3, SIGNAL(toggled(bool)), this, SLOT(changeImageScalingFree(bool)));
		connect(buttonGroup5, SIGNAL(toggled(bool)), this, SLOT(changeImageScalingRatio(bool)));
		connect(chainButton, SIGNAL(clicked()), this, SLOT(toggleChain()));
		connect(scalingHorizontal, SIGNAL(valueChanged(int)), this, SLOT(hChange()));
		connect(scalingVertical, SIGNAL(valueChanged(int)), this, SLOT(vChange()));
		connect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar(int)));
	}
	else
	{
		disconnect(toolShape, SIGNAL(clicked()), this, SLOT(setTool()));
		disconnect(toolPoly, SIGNAL(clicked()), this, SLOT(setTool()));
		disconnect(toolImage, SIGNAL(clicked()), this, SLOT(setTool()));
		disconnect(toolText, SIGNAL(clicked()), this, SLOT(setTool()));
		disconnect(toolLine, SIGNAL(clicked()), this, SLOT(setTool()));
		disconnect(toolZoom, SIGNAL(clicked()), this, SLOT(setTool()));
		disconnect(toolMisc, SIGNAL(clicked()), this, SLOT(setTool()));
		disconnect(fontComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
		disconnect(sizeComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
		disconnect(colorComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
		disconnect(colorComboTextBackground, SIGNAL(activated(int)), this, SLOT(setSample()));
		disconnect(shadingTextBack, SIGNAL(valueChanged(int)), this, SLOT(setSample()));
		disconnect(shadingText, SIGNAL(valueChanged(int)), this, SLOT(setSample()));
		disconnect(buttonGroup3, SIGNAL(toggled(bool)), this, SLOT(changeImageScalingFree(bool)));
		disconnect(buttonGroup5, SIGNAL(toggled(bool)), this, SLOT(changeImageScalingRatio(bool)));
		disconnect(chainButton, SIGNAL(clicked()), this, SLOT(toggleChain()));
		disconnect(scalingHorizontal, SIGNAL(valueChanged(int)), this, SLOT(hChange()));
		disconnect(scalingVertical, SIGNAL(valueChanged(int)), this, SLOT(vChange()));
		disconnect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar(int)));
	}
}

void TabTools::restoreDefaults(struct toolPrefs *prefsData, int unitIndex)
{
	enableSignals(false);

	PrefsManager* prefsManager=PrefsManager::instance();
	unit = unitGetSuffixFromIndex(unitIndex);
	precision = unitGetPrecisionFromIndex(unitIndex);
	unitRatio = unitGetRatioFromIndex(unitIndex);

	for (int fc=0; fc<fontComboText->count(); ++fc)
	{
		if (fontComboText->text(fc) == prefsData->defFont)
		{
			fontComboText->setCurrentItem(fc);
			break;
		}
	}
	QString ar_sizes[] = {" 7", " 9", "10", "11", "12", "14", "18", "24", "36", "48", "60", "72"};
	size_t f_size = sizeof(ar_sizes) / sizeof(*ar_sizes);
	sizeComboText->clear();
	for (uint s = 0; s < f_size; ++s)
		sizeComboText->insertItem(ar_sizes[s] + tr(" pt"));
	for (int a = 0; a < sizeComboText->count(); ++a)
	{
		if (sizeComboText->text(a).left(2).toInt() == prefsData->defSize / 10)
		{
			sizeComboText->setCurrentItem(a);
			break;
		}
	}

	
	ColorList::Iterator itc, endOfColorList;
	ColorList* colorList = (docu != 0) ? (&docu->PageColors) : prefsManager->colorSetPtr();
	colorComboText->clear();
	colorComboText->insertItem(CommonStrings::tr_NoneColor);
	if (prefsData->dPenText == CommonStrings::None)
		colorComboText->setCurrentItem(colorComboText->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		colorComboText->insertSmallItem( itc.value(), docu, itc.key() );
		if (itc.key() == prefsData->dPenText)
			colorComboText->setCurrentItem(colorComboText->count()-1);
	}

	shadingText->setValue(prefsData->dTextPenShade);

	colorComboStrokeText->clear();
	colorComboStrokeText->insertItem(CommonStrings::tr_NoneColor);
	if (prefsData->dStrokeText == CommonStrings::None)
		colorComboStrokeText->setCurrentItem(colorComboStrokeText->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		colorComboStrokeText->insertSmallItem( itc.value(), docu, itc.key() );
		if (itc.key() == prefsData->dStrokeText)
			colorComboStrokeText->setCurrentItem(colorComboStrokeText->count()-1);
	}
	shadingTextStroke->setValue(prefsData->dTextStrokeShade);

	colorComboTextBackground->clear();
	colorComboTextBackground->insertItem(CommonStrings::tr_NoneColor);
	if (prefsData->dTextBackGround == CommonStrings::None)
		colorComboTextBackground->setCurrentItem(colorComboTextBackground->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		colorComboTextBackground->insertSmallItem( itc.value(), docu, itc.key() );
		if (itc.key() == prefsData->dTextBackGround)
			colorComboTextBackground->setCurrentItem(colorComboTextBackground->count()-1);
	}
	shadingTextBack->setValue(prefsData->dTextBackGroundShade);

	colorComboTextLine->clear();
	colorComboTextLine->insertItem(CommonStrings::tr_NoneColor);
	if (prefsData->dTextLineColor == CommonStrings::None)
		colorComboTextLine->setCurrentItem(colorComboTextLine->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		colorComboTextLine->insertSmallItem( itc.value(), docu, itc.key() );
		if (itc.key() == prefsData->dTextLineColor)
			colorComboTextLine->setCurrentItem(colorComboTextLine->count()-1);
	}
	shadingTextLine->setValue(prefsData->dTextLineShade);

	tabFillCombo->clear();
	tabFillCombo->insertItem( tr("None", "tab fill" ));
	tabFillCombo->insertItem( tr("Dot"));
	tabFillCombo->insertItem( tr("Hyphen"));
	tabFillCombo->insertItem( tr("Underscore"));
	tabFillCombo->insertItem( tr("Custom"));
	if (prefsData->tabFillChar.isEmpty())
	{
		tabFillCombo->setCurrentItem(0);
	}
	else if (prefsData->tabFillChar == ".")
	{
		tabFillCombo->setCurrentItem(1);
	}
	else if (prefsData->tabFillChar == "-")
	{
		tabFillCombo->setCurrentItem(2);
	}
	else if (prefsData->tabFillChar == "_")
	{
		tabFillCombo->setCurrentItem(3);
	}
	else
	{
		tabFillCombo->setCurrentItem(4);
		tabFillCombo->setEditable(true);
		tabFillCombo->setEditText( CommonStrings::trCustomTabFill + prefsData->tabFillChar);
	}
	gapText->setNewUnit(unitIndex);
	gapTab->setNewUnit(unitIndex);
	gapTab->setValue(prefsData->dTabWidth * unitRatio);
	columnsText->setValue(prefsData->dCols);
	gapText->setValue(prefsData->dGap * unitRatio);

	colorComboLineShape->clear();
	colorComboLineShape->insertItem(CommonStrings::tr_NoneColor);
	if (prefsData->dPen == CommonStrings::None)
		colorComboLineShape->setCurrentItem(colorComboLineShape->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		colorComboLineShape->insertSmallItem( itc.value(), docu, itc.key() );
		if (itc.key() == prefsData->dPen)
			colorComboLineShape->setCurrentItem(colorComboLineShape->count()-1);
	}
	shadingLineShape->setValue(prefsData->dShade2);

	comboFillShape->clear();
	comboFillShape->insertItem( tr("None"));
	if (prefsData->dBrush == CommonStrings::None)
		comboFillShape->setCurrentItem(comboFillShape->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		comboFillShape->insertSmallItem( itc.value(), docu, itc.key() );
		if (itc.key() == prefsData->dBrush)
			comboFillShape->setCurrentItem(comboFillShape->count()-1);
	}

	shadingFillShape->setValue(prefsData->dShade);
	switch (prefsData->dLineArt)
	{
		case Qt::SolidLine:
			comboStyleShape->setCurrentItem(0);
			break;
		case Qt::DashLine:
			comboStyleShape->setCurrentItem(1);
			break;
		case Qt::DotLine:
			comboStyleShape->setCurrentItem(2);
			break;
		case Qt::DashDotLine:
			comboStyleShape->setCurrentItem(3);
			break;
		case Qt::DashDotDotLine:
			comboStyleShape->setCurrentItem(4);
			break;
		default:
			comboStyleShape->setCurrentItem(0);
			break;
	}

	lineWidthShape->setValue(prefsData->dWidth);

	colorComboLine->clear();
	colorComboLine->insertItem(CommonStrings::tr_NoneColor);
	if (prefsData->dPenLine == CommonStrings::None)
		colorComboLine->setCurrentItem(colorComboLine->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		colorComboLine->insertSmallItem( itc.value(), docu, itc.key() );
		if (itc.key() == prefsData->dPenLine)
			colorComboLine->setCurrentItem(colorComboLine->count()-1);
	}

	shadingLine->setValue(prefsData->dShadeLine);

	switch (prefsData->dLstyleLine)
	{
		case Qt::SolidLine:
			comboStyleLine->setCurrentItem(0);
			break;
		case Qt::DashLine:
			comboStyleLine->setCurrentItem(1);
			break;
		case Qt::DotLine:
			comboStyleLine->setCurrentItem(2);
			break;
		case Qt::DashDotLine:
			comboStyleLine->setCurrentItem(3);
			break;
		case Qt::DashDotDotLine:
			comboStyleLine->setCurrentItem(4);
			break;
		default:
			comboStyleLine->setCurrentItem(0);
			break;
	}

	if (docu != 0)
	{
		startArrow->rebuildList(&docu->arrowStyles);
		endArrow->rebuildList(&docu->arrowStyles);
	}
	else
	{
		startArrow->rebuildList(&prefsManager->appPrefs.arrowStyles);
		endArrow->rebuildList(&prefsManager->appPrefs.arrowStyles);
	}
	startArrow->setCurrentItem(prefsData->dStartArrow);
	endArrow->setCurrentItem(prefsData->dEndArrow);
	lineWidthLine->setValue(prefsData->dWidthLine);

	buttonGroup3->setChecked( prefsData->scaleType );
	scalingHorizontal->setValue(qRound(prefsData->scaleX * 100));
	scalingVertical->setValue(qRound(prefsData->scaleY * 100));
	buttonGroup5->setChecked( !prefsData->scaleType );
	checkRatioImage->setChecked(prefsData->aspectRatio);

	comboFillImage->clear();
	comboFillImage->insertItem(CommonStrings::tr_NoneColor);
	if (prefsData->dBrushPict == CommonStrings::None)
		comboFillImage->setCurrentItem(comboFillImage->count()-1);
	endOfColorList=colorList->end();
	for (itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		comboFillImage->insertSmallItem( itc.value(), docu, itc.key() );
		if (itc.key() == prefsData->dBrushPict)
			comboFillImage->setCurrentItem(comboFillImage->count()-1);
	}

	shadingFillImage->setValue( prefsData->shadePict );
	embeddedPath->setChecked(prefsData->useEmbeddedPath);
	switch (prefsData->lowResType)
	{
		case 0:
			checkFullRes->setChecked(true);
			break;
		case 1:
			checkNormalRes->setChecked(true);
			break;
		case 2:
			checkHalfRes->setChecked(true);
			break;
	}

	minimumZoom->setValue(prefsData->magMin);
	maximumZoom->setValue(prefsData->magMax);
	zoomStep->setValue( prefsData->magStep );

	genDispX->setValue(prefsData->dispX);
	genDispY->setValue(prefsData->dispY);
	genDispX->setNewUnit(unitIndex);
	genDispY->setNewUnit(unitIndex);
	genRot->setValue(prefsData->constrain);

	setSample();

	enableSignals(true);
}

void TabTools::setCustomFillChar(const QString &txt)
{
	if (txt == CommonStrings::trCustomTabFill)
		return;
	disconnect(tabFillCombo, SIGNAL(textChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
	disconnect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar(int)));
	QString ret = txt.right(1);
	if (tabFillCombo->editable())
	{
		tabFillCombo->setCurrentItem(4);
		tabFillCombo->setEditText( CommonStrings::trCustomTabFill+ret);
		tabFillCombo->changeItem( CommonStrings::trCustomTabFill+ret, 4);
	}
	connect(tabFillCombo, SIGNAL(textChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
	connect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar(int)));
}

void TabTools::setFillChar(int act)
{
	disconnect(tabFillCombo, SIGNAL(textChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
	disconnect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar(int)));
	switch (act)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			tabFillCombo->setCurrentItem(act);
			tabFillCombo->setEditable(false);
			break;
		case 4:
			tabFillCombo->setEditable(true);
			tabFillCombo->setEditText(CommonStrings::trCustomTabFill);
			tabFillCombo->changeItem(CommonStrings::trCustomTabFill, 4);
			tabFillCombo->lineEdit()->setFocus();
			tabFillCombo->lineEdit()->setCursorPosition(CommonStrings::trCustomTabFill.length());
			connect(tabFillCombo, SIGNAL(textChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
			break;
	}
	connect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar(int)));
}

void TabTools::toggleChain()
{
	if (chainButton->isOn())
		scalingVertical->setValue(scalingHorizontal->value());
}

void TabTools::hChange()
{
	if (chainButton->isOn())
		scalingVertical->setValue(scalingHorizontal->value());
}

void TabTools::vChange()
{
	if (chainButton->isOn())
		scalingHorizontal->setValue(scalingVertical->value());
}

void TabTools::changeImageScalingFree(bool)
{
	if (buttonGroup3->isChecked())
		buttonGroup5->setChecked(false);
	else
		buttonGroup5->setChecked(true);
}

void TabTools::changeImageScalingRatio(bool)
{
	if (buttonGroup5->isChecked())
		buttonGroup3->setChecked(false);
	else
		buttonGroup3->setChecked(true);
}

void TabTools::setSample()
{
	if (!fontPreview)
		return;

	SampleItem *si = new SampleItem(docu);
	si->setText( tr("Woven silk pyjamas exchanged for blue quartz"));
	if (colorComboTextBackground->currentText() != CommonStrings::tr_NoneColor)
	{
		if (docu != 0)
		{
			si->setBgColor(docu->PageColors[colorComboTextBackground->currentText()].getRawRGBColor());
			si->setBgShade(shadingTextBack->value());
		}
		else
		{
			PrefsManager* prefsManager=PrefsManager::instance();
			ColorList* colorList=prefsManager->colorSetPtr();
			si->setBgColor((*colorList)[colorComboTextBackground->currentText()].getRawRGBColor());
			si->setBgShade(shadingTextBack->value());
		}
	}
	else
		si->setBgColor(paletteBackgroundColor());

	if (colorComboText->currentText() != CommonStrings::tr_NoneColor)
	{
		if (docu != 0)
		{
			si->setTxColor(docu->PageColors[colorComboText->currentText()].getRawRGBColor());
			si->setTxShade(shadingText->value());
		}
		else
		{
			PrefsManager* prefsManager=PrefsManager::instance();
			ColorList* colorList=prefsManager->colorSetPtr();
			si->setTxColor((*colorList)[colorComboText->currentText()].getRawRGBColor());
			si->setTxShade(shadingText->value());
		}
	}
	else
		si->setTxColor(paletteBackgroundColor());
	si->setFont(fontComboText->currentText());
	si->setFontSize(sizeComboText->currentText().left(2).toInt() * 10, true);
	previewText->setPixmap(si->getSample(previewText->width(), previewText->height()));
	delete si;
}

void TabTools::setTool()
{
	if (toolText == sender())
		subStackTools->setCurrentIndex(0);
	if (toolShape == sender())
		subStackTools->setCurrentIndex(1);
	if (toolLine == sender())
		subStackTools->setCurrentIndex(2);
	if (toolImage == sender())
		subStackTools->setCurrentIndex(3);
	if (toolPoly == sender())
		subStackTools->setCurrentIndex(4);
	if (toolZoom == sender())
		subStackTools->setCurrentIndex(5);
	if (toolMisc == sender())
		subStackTools->setCurrentIndex(6);
}

void TabTools::enableFontPreview(bool state)
{
	fontPreview = state;
	setSample();
}

void TabTools::unitChange(int docUnitIndex)
{
	gapText->setNewUnit(docUnitIndex);
	gapTab->setNewUnit(docUnitIndex);
	genDispX->setNewUnit(docUnitIndex);
	genDispY->setNewUnit(docUnitIndex);
}
