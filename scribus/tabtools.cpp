#include "tabtools.h"
#include "tabtools.moc"
#include <qtooltip.h>
#include <qspinbox.h>
#include "units.h"
#include "mspinbox.h"
#include "linecombo.h"
#include "fontcombo.h"
#include "polygonwidget.h"
#include "arrowchooser.h"
#include "linkbutton.h"
#include "scribusstructs.h"
#include "scribus.h"
extern QPixmap loadIcon(QString nam);
extern QPixmap fontSamples(QString da, int s, QString ts, QColor back);
extern double UmReFaktor;
extern ScribusApp* ScApp;

TabTools::TabTools( QWidget* parent, struct toolPrefs *prefsData, int unitIndex, ScribusDoc* doc) : QWidget( parent, "tabtools", 0 )
{
	unit = unitGetSuffixFromIndex(unitIndex);
	precision = unitGetPrecisionFromIndex(unitIndex);
	unitRatio = unitGetRatioFromIndex(unitIndex);
	
	fon = &ScApp->Prefs.AvailFonts;
	tabToolsLayout = new QHBoxLayout( this, 11, 6, "tabToolsLayout");
	buttonGroupTools = new QButtonGroup( this, "buttonGroupTools" );
	buttonGroupTools->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, buttonGroupTools->sizePolicy().hasHeightForWidth() ) );
	buttonGroupTools->setExclusive( true );
	buttonGroupTools->setRadioButtonExclusive( true );
	buttonGroupTools->setColumnLayout(0, Qt::Vertical );
	buttonGroupTools->layout()->setSpacing( 5 );
	buttonGroupTools->layout()->setMargin( 5 );
	buttonGroupTools->setTitle( QString::null );
	buttonGroupToolsLayout = new QVBoxLayout( buttonGroupTools->layout() );
	buttonGroupToolsLayout->setAlignment( Qt::AlignTop );
	toolText = new QToolButton( buttonGroupTools, "toolText" );
	toolText->setToggleButton( true );
	toolText->setText( QString::null );
	toolText->setIconSet( QIconSet( loadIcon("Text.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolText );
	toolImage = new QToolButton( buttonGroupTools, "toolImage" );
	toolImage->setToggleButton( true );
	toolImage->setText( QString::null );
	toolImage->setIconSet( QIconSet( loadIcon("Bild.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolImage );
	toolShape = new QToolButton( buttonGroupTools, "toolShape" );
	toolShape->setToggleButton( true );
	toolShape->setText( QString::null );
	toolShape->setIconSet( QIconSet( loadIcon("Rechtecke.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolShape);
	toolPoly = new QToolButton( buttonGroupTools, "toolPoly" );
	toolPoly->setToggleButton( true );
	toolPoly->setText( QString::null );
	toolPoly->setIconSet( QIconSet( loadIcon("spline.png") ) );
	buttonGroupToolsLayout->addWidget( toolPoly );
	toolLine = new QToolButton( buttonGroupTools, "toolLine" );
	toolLine->setToggleButton( true );
	toolLine->setText( QString::null );
	toolLine->setIconSet( QIconSet( loadIcon("Stift.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolLine );
	toolZoom = new QToolButton( buttonGroupTools, "toolZoom" );
	toolZoom->setToggleButton( true );
	toolZoom->setText( QString::null );
	toolZoom->setIconSet( QIconSet( loadIcon("Lupe.xpm") ) );
	buttonGroupToolsLayout->addWidget( toolZoom );
	tabToolsLayout->addWidget( buttonGroupTools );
	subStackTools = new QWidgetStack( this, "subStackTools" );
	subStackTools->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, subStackTools->sizePolicy().hasHeightForWidth() ) );
	subStackTools->setFrameShape( QWidgetStack::GroupBoxPanel );
	subStackTools->setFrameShadow( QWidgetStack::Sunken );

	subTabText = new QWidget( subStackTools, "subTabText" );
	subTabTextLayout = new QGridLayout( subTabText, 1, 1, 11, 6, "subTabTextLayout");
	fontComboText = new FontCombo(subTabText, &ScApp->Prefs);
	for (int fc=0; fc<fontComboText->count(); ++fc)
	{
		if (fontComboText->text(fc) == prefsData->defFont)
		{
			fontComboText->setCurrentItem(fc);
			break;
		}
	}
	subTabTextLayout->addMultiCellWidget( fontComboText, 0, 0, 1, 3, Qt::AlignLeft );
	textLabel1b = new QLabel( fontComboText, tr( "Font:" ), subTabText, "textLabel1b" );
	subTabTextLayout->addWidget( textLabel1b, 0, 0 );
	sizeComboText = new QComboBox( true, subTabText, "SizeCombo" );
	sizeComboText->setEditable(false);
	QString ar_sizes[] = {" 7", " 9", "10", "11", "12", "14", "18", "24", "36", "48", "60", "72"};
	size_t f_size = sizeof(ar_sizes) / sizeof(*ar_sizes);
	for (uint s = 0; s < f_size; ++s)
		sizeComboText->insertItem(ar_sizes[s] + tr(" pt"));
	for (int a = 0; a < sizeComboText->count(); ++a)
	{
		if (sizeComboText->text(a).left(2).toInt() == prefsData->defSize / 10)
			sizeComboText->setCurrentItem(a);
	}
	subTabTextLayout->addMultiCellWidget( sizeComboText, 1, 1, 1, 3, Qt::AlignLeft );
	textLabel2b = new QLabel(sizeComboText, tr( "Size:" ), subTabText, "textLabel2b" );
	subTabTextLayout->addWidget( textLabel2b, 1, 0 );
	colorComboText = new QComboBox( true, subTabText, "colorComboText" );
	colorComboText->setEditable(false);
	QPixmap pmT2;
	pmT2 = QPixmap(15, 15);
	ColorList::Iterator itc;
	colorComboText->insertItem( tr("None"));
	if (prefsData->dPenText == "None")
		colorComboText->setCurrentItem(colorComboText->count()-1);
	if (doc != 0)
	{
		for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
		{
			pmT2.fill(doc->PageColors[itc.key()].getRGBColor());
			colorComboText->insertItem(pmT2, itc.key());
			if (itc.key() == prefsData->dPenText)
				colorComboText->setCurrentItem(colorComboText->count()-1);
		}
	}
	else
	{
		for (itc = ScApp->Prefs.DColors.begin(); itc != ScApp->Prefs.DColors.end(); ++itc)
		{
			pmT2.fill(ScApp->Prefs.DColors[itc.key()].getRGBColor());
			colorComboText->insertItem(pmT2, itc.key());
			if (itc.key() == prefsData->dPenText)
				colorComboText->setCurrentItem(colorComboText->count()-1);
		}
	}
	subTabTextLayout->addMultiCellWidget( colorComboText, 2, 2, 1, 3, Qt::AlignLeft );
	textLabel3b = new QLabel(colorComboText, tr( "Fill Color:" ), subTabText, "textLabel3b" );
	subTabTextLayout->addWidget( textLabel3b, 2, 0 );
	colorComboStrokeText = new QComboBox( true, subTabText, "colorComboStrokeText" );
	colorComboStrokeText->setEditable(false);
	colorComboStrokeText->insertItem( tr("None"));
	if (prefsData->dStrokeText == "None")
		colorComboStrokeText->setCurrentItem(colorComboStrokeText->count()-1);
	if (doc != 0)
	{
		for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
		{
			pmT2.fill(doc->PageColors[itc.key()].getRGBColor());
			colorComboStrokeText->insertItem(pmT2, itc.key());
			if (itc.key() == prefsData->dStrokeText)
				colorComboStrokeText->setCurrentItem(colorComboStrokeText->count()-1);
		}
	}
	else
	{
		for (itc = ScApp->Prefs.DColors.begin(); itc != ScApp->Prefs.DColors.end(); ++itc)
		{
			pmT2.fill(ScApp->Prefs.DColors[itc.key()].getRGBColor());
			colorComboStrokeText->insertItem(pmT2, itc.key());
			if (itc.key() == prefsData->dStrokeText)
				colorComboStrokeText->setCurrentItem(colorComboStrokeText->count()-1);
		}
	}
	subTabTextLayout->addMultiCellWidget( colorComboStrokeText, 3, 3, 1, 3, Qt::AlignLeft );
	textLabel3b2 = new QLabel(colorComboStrokeText, tr( "Stroke Color:" ), subTabText, "textLabel3b2" );
	subTabTextLayout->addWidget( textLabel3b2, 3, 0 );
	columnsText = new QSpinBox( subTabText, "columnsText" );
	columnsText->setMinValue( 1 );
	columnsText->setMaxValue(100);
	columnsText->setValue(prefsData->dCols);
	subTabTextLayout->addWidget( columnsText, 4, 1, Qt::AlignLeft );
	textLabel4b = new QLabel(columnsText, tr("Colu&mns:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel4b, 4, 0 );
	gapText = new MSpinBox( 0, 200, subTabText, precision );
	gapText->setSuffix( unit );
	gapText->setValue(prefsData->dGap * unitRatio);
	subTabTextLayout->addWidget( gapText, 4, 3, Qt::AlignLeft );
	textLabel5b = new QLabel(gapText, tr("&Gap:"), subTabText, "TextCol");
	subTabTextLayout->addWidget( textLabel5b, 4, 2 );
	previewText = new QLabel( tr( "Woven silk pyjamas exchanged for blue quartz" ), subTabText, "previewText" );
	previewText->setMinimumSize( QSize( 280, 70 ) );
	previewText->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	subTabTextLayout->addMultiCellWidget( previewText, 5, 5, 0, 3 );
	subStackTools->addWidget( subTabText, 0 );

	subTabShape = new QWidget( subStackTools, "subTabShape" );
	subTabShapeLayout = new QGridLayout( subTabShape, 1, 1, 11, 6, "subTabShapeLayout");
	subTabShapeLayout->setAlignment( Qt::AlignTop );
	colorComboLineShape = new QComboBox( true, subTabShape, "colorComboLineShape" );
	colorComboLineShape->setEditable(false);
	colorComboLineShape->insertItem( tr("None"));
	if (prefsData->dPen == "None")
		colorComboLineShape->setCurrentItem(colorComboLineShape->count()-1);
	QPixmap pm2S = QPixmap(15, 15);
	if (doc != 0)
	{
		for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
		{
			pm2S.fill(doc->PageColors[itc.key()].getRGBColor());
			colorComboLineShape->insertItem(pm2S, itc.key());
			if (itc.key() == prefsData->dPen)
				colorComboLineShape->setCurrentItem(colorComboLineShape->count()-1);
		}
	}
	else
	{
		for (itc = ScApp->Prefs.DColors.begin(); itc != ScApp->Prefs.DColors.end(); ++itc)
		{
			pm2S.fill(ScApp->Prefs.DColors[itc.key()].getRGBColor());
			colorComboLineShape->insertItem(pm2S, itc.key());
			if (itc.key() == prefsData->dPen)
				colorComboLineShape->setCurrentItem(colorComboLineShape->count()-1);
		}
	}
	subTabShapeLayout->addWidget( colorComboLineShape, 0, 1, Qt::AlignLeft );
	textLabel7b = new QLabel( colorComboLineShape, tr( "&Line Color:" ), subTabShape, "textLabel7b" );
	subTabShapeLayout->addWidget( textLabel7b, 0, 0 );
	shadingLineShape = new QSpinBox( subTabShape, "shadingLineShape" );
	shadingLineShape->setMaxValue( 100 );
	shadingLineShape->setSuffix( tr( " %" ) );
	shadingLineShape->setMinValue( 0 );
	shadingLineShape->setValue(prefsData->dShade2);
	subTabShapeLayout->addWidget( shadingLineShape, 1, 1, Qt::AlignLeft );
	textLabel8b = new QLabel( shadingLineShape, tr( "&Shading:" ), subTabShape, "textLabel8b" );
	subTabShapeLayout->addWidget( textLabel8b, 1, 0 );
	comboFillShape = new QComboBox( true, subTabShape, "comboFillShape" );
	comboFillShape->setEditable(false);
	comboFillShape->insertItem( tr("None"));
	if (prefsData->dBrush == "None")
		comboFillShape->setCurrentItem(comboFillShape->count()-1);
	if (doc != 0)
	{
		for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
		{
			pm2S.fill(doc->PageColors[itc.key()].getRGBColor());
			comboFillShape->insertItem(pm2S, itc.key());
			if (itc.key() == prefsData->dBrush)
				comboFillShape->setCurrentItem(comboFillShape->count()-1);
		}
	}
	else
	{
		for (itc = ScApp->Prefs.DColors.begin(); itc != ScApp->Prefs.DColors.end(); ++itc)
		{
			pm2S.fill(ScApp->Prefs.DColors[itc.key()].getRGBColor());
			comboFillShape->insertItem(pm2S, itc.key());
			if (itc.key() == prefsData->dBrush)
				comboFillShape->setCurrentItem(comboFillShape->count()-1);
		}
	}
	subTabShapeLayout->addWidget( comboFillShape, 2, 1, Qt::AlignLeft );
	textLabel9b = new QLabel( comboFillShape, tr( "&Fill Color:" ), subTabShape, "textLabel9b" );
	subTabShapeLayout->addWidget( textLabel9b, 2, 0 );
	shadingFillShape = new QSpinBox( subTabShape, "shadingFillShape" );
	shadingFillShape->setMaxValue( 100 );
	shadingFillShape->setSuffix( tr( " %" ) );
	shadingFillShape->setMinValue( 0 );
	shadingFillShape->setValue(prefsData->dShade);
	subTabShapeLayout->addWidget( shadingFillShape, 3, 1, Qt::AlignLeft );
	textLabel10b = new QLabel( shadingFillShape, tr( "S&hading:" ), subTabShape, "textLabel10b" );
	subTabShapeLayout->addWidget( textLabel10b, 3, 0 );
	comboStyleShape = new LineCombo(subTabShape);
	comboStyleShape->setEditable(false);
	switch (prefsData->dLineArt)
	{
	case SolidLine:
		comboStyleShape->setCurrentItem(0);
		break;
	case DashLine:
		comboStyleShape->setCurrentItem(1);
		break;
	case DotLine:
		comboStyleShape->setCurrentItem(2);
		break;
	case DashDotLine:
		comboStyleShape->setCurrentItem(3);
		break;
	case DashDotDotLine:
		comboStyleShape->setCurrentItem(4);
		break;
	default:
		comboStyleShape->setCurrentItem(0);
		break;
	}
	subTabShapeLayout->addWidget( comboStyleShape, 4, 1, Qt::AlignLeft );
	textLabel11b = new QLabel( comboStyleShape, tr( "Line Style:" ), subTabShape, "textLabel11b" );
	subTabShapeLayout->addWidget( textLabel11b, 4, 0 );
	lineWidthShape = new MSpinBox( 0, 36, subTabShape, 1 );
	lineWidthShape->setSuffix( tr( " pt" ) );
	lineWidthShape->setValue(prefsData->dWidth);
	subTabShapeLayout->addWidget( lineWidthShape, 5, 1, Qt::AlignLeft );
	textLabel12b = new QLabel( lineWidthShape, tr( "Line &Width:" ), subTabShape, "TextLabel2_3_4" );
	subTabShapeLayout->addWidget( textLabel12b, 5, 0 );
	subStackTools->addWidget( subTabShape, 1 );

	subTabLine = new QWidget( subStackTools, "subTabLine" );
	subTabLineLayout = new QGridLayout( subTabLine, 1, 1, 11, 6, "subTabLineLayout");
	subTabLineLayout->setAlignment( Qt::AlignTop );
	colorComboLine = new QComboBox( true, subTabLine, "colorComboLine" );
	colorComboLine->setEditable(false);
	QPixmap pm2L = QPixmap(15, 15);
	colorComboLine->insertItem( tr("None"));
	if (prefsData->dPenLine == "None")
		colorComboLine->setCurrentItem(colorComboLine->count()-1);
	if (doc != 0)
	{
		for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
		{
			pm2L.fill(doc->PageColors[itc.key()].getRGBColor());
			colorComboLine->insertItem(pm2L, itc.key());
			if (itc.key() == prefsData->dPenLine)
				colorComboLine->setCurrentItem(colorComboLine->count()-1);
		}
	}
	else
	{
		for (itc = ScApp->Prefs.DColors.begin(); itc != ScApp->Prefs.DColors.end(); ++itc)
		{
			pm2L.fill(ScApp->Prefs.DColors[itc.key()].getRGBColor());
			colorComboLine->insertItem(pm2L, itc.key());
			if (itc.key() == prefsData->dPenLine)
				colorComboLine->setCurrentItem(colorComboLine->count()-1);
		}
	}
	subTabLineLayout->addMultiCellWidget( colorComboLine, 0, 0, 1, 2, Qt::AlignLeft );
	textLabel13b = new QLabel(colorComboLine, tr("&Line Color:"), subTabLine, "textLabel13b" );
	subTabLineLayout->addWidget( textLabel13b, 0, 0 );
	shadingLine = new QSpinBox( subTabLine, "shadingLine" );
	shadingLine->setMaxValue( 100 );
	shadingLine->setValue(prefsData->dShadeLine);
	shadingLine->setSuffix( tr( " %" ) );
	subTabLineLayout->addMultiCellWidget( shadingLine, 1, 1, 1, 2, Qt::AlignLeft );
	textLabel14b = new QLabel(shadingLine, tr("&Shading:"), subTabLine, "textLabel14b" );
	subTabLineLayout->addWidget( textLabel14b, 1, 0 );
	comboStyleLine = new LineCombo(subTabLine);
	comboStyleLine->setEditable(false);
	switch (prefsData->dLstyleLine)
	{
	case SolidLine:
		comboStyleLine->setCurrentItem(0);
		break;
	case DashLine:
		comboStyleLine->setCurrentItem(1);
		break;
	case DotLine:
		comboStyleLine->setCurrentItem(2);
		break;
	case DashDotLine:
		comboStyleLine->setCurrentItem(3);
		break;
	case DashDotDotLine:
		comboStyleLine->setCurrentItem(4);
		break;
	default:
		comboStyleLine->setCurrentItem(0);
		break;
	}
	subTabLineLayout->addMultiCellWidget( comboStyleLine, 2, 2, 1, 2, Qt::AlignLeft );
	textLabel15b = new QLabel(subTabLine, tr("Line S&tyle:"), subTabLine, "textLabel15b" );
	subTabLineLayout->addWidget( textLabel15b, 2, 0 );
	startArrow = new ArrowChooser(subTabLine, true);
	subTabLineLayout->addWidget( startArrow, 4, 1 );
	endArrow = new ArrowChooser(subTabLine, false);
	subTabLineLayout->addWidget( endArrow, 4, 2 );
	if (doc != 0)
	{
		startArrow->rebuildList(&doc->arrowStyles);
		endArrow->rebuildList(&doc->arrowStyles);
	}
	else
	{
		startArrow->rebuildList(&ScApp->Prefs.arrowStyles);
		endArrow->rebuildList(&ScApp->Prefs.arrowStyles);
	}
	startArrow->setCurrentItem(prefsData->dStartArrow);
	endArrow->setCurrentItem(prefsData->dEndArrow);
	arrowText = new QLabel( tr( "Arrows:" ), subTabLine, "arrowText" );
	subTabLineLayout->addMultiCellWidget( arrowText, 3, 4, 0, 0 );
	startArrowText = new QLabel( startArrow, tr( "Start:" ), subTabLine, "startArrowText" );
	subTabLineLayout->addWidget( startArrowText, 3, 1 );
	endArrowText = new QLabel( endArrow, tr( "End:" ), subTabLine, "endArrowText" );
	subTabLineLayout->addWidget( endArrowText, 3, 2 );
	lineWidthLine = new MSpinBox( 1, 36, subTabLine, 1 );
	lineWidthLine->setSuffix( tr( " pt" ) );
	lineWidthLine->setValue(prefsData->dWidthLine);
	subTabLineLayout->addMultiCellWidget( lineWidthLine, 5, 5, 1, 2, Qt::AlignLeft );
	textLabel16b = new QLabel(lineWidthLine, tr("Line &Width:"), subTabLine, "textLabel16b" );
	subTabLineLayout->addWidget( textLabel16b, 5, 0 );
	subStackTools->addWidget( subTabLine, 2 );

	subTabImage = new QWidget( subStackTools, "subTabImage" );
	subTabImageLayout = new QGridLayout( subTabImage, 1, 1, 11, 6, "subTabImageLayout");
	subTabImageLayout->setAlignment( Qt::AlignTop );
	buttonGroup3 = new QButtonGroup( subTabImage, "buttonGroup3" );
	buttonGroup3->setCheckable( true );
	buttonGroup3->setChecked( prefsData->scaleType );
	buttonGroup3->setColumnLayout(0, Qt::Vertical );
	buttonGroup3->layout()->setSpacing( 6 );
	buttonGroup3->layout()->setMargin( 11 );
	buttonGroup3->setTitle( tr( "&Free Scaling" ) );
	buttonGroup3Layout = new QGridLayout( buttonGroup3->layout() );
	buttonGroup3Layout->setAlignment( Qt::AlignTop );
	scalingHorizontal = new QSpinBox( buttonGroup3, "scalingHorizontal" );
	scalingHorizontal->setMaxValue( 1000 );
	scalingHorizontal->setMinValue( 1 );
	scalingHorizontal->setValue(qRound(prefsData->scaleX * 100));
	scalingHorizontal->setSuffix( tr( " %" ) );
	buttonGroup3Layout->addWidget( scalingHorizontal, 0, 1, Qt::AlignRight );
	textLabel17b = new QLabel(scalingHorizontal, tr("&Horizontal Scaling:"), buttonGroup3, "textLabel17b" );
	buttonGroup3Layout->addWidget( textLabel17b, 0, 0 );
	scalingVertical = new QSpinBox( buttonGroup3, "scalingVertical" );
	scalingVertical->setMaxValue( 1000 );
	scalingVertical->setMinValue( 1 );
	scalingVertical->setSuffix( tr( " %" ) );
	scalingVertical->setValue(qRound(prefsData->scaleY * 100));
	buttonGroup3Layout->addWidget( scalingVertical, 1, 1, Qt::AlignRight );
	textLabel18b = new QLabel(scalingVertical, tr( "&Vertical Scaling:" ), buttonGroup3, "textLabel18b" );
	buttonGroup3Layout->addWidget( textLabel18b, 1, 0 );
	chainButton = new LinkButton( buttonGroup3 );
	chainButton->setToggleButton( true );
	chainButton->setAutoRaise(true);
	buttonGroup3Layout->addMultiCellWidget( chainButton, 0, 1, 2, 2, Qt::AlignLeft );
	subTabImageLayout->addMultiCellWidget( buttonGroup3, 0, 0, 0, 1 );
	buttonGroup5 = new QButtonGroup( subTabImage, "buttonGroup5" );
	buttonGroup5->setCheckable( true );
	buttonGroup5->setChecked( !prefsData->scaleType );
	buttonGroup5->setColumnLayout(0, Qt::Vertical );
	buttonGroup5->layout()->setSpacing( 6 );
	buttonGroup5->layout()->setMargin( 11 );
	buttonGroup5->setTitle( tr( "&Scale Picture to Frame Size" ) );
	buttonGroup5Layout = new QHBoxLayout( buttonGroup5->layout() );
	buttonGroup5Layout->setAlignment( Qt::AlignTop );
	checkRatioImage = new QCheckBox( buttonGroup5, "checkRatioImage" );
	checkRatioImage->setText( tr( "Keep Aspect &Ratio" ) );
	checkRatioImage->setChecked(prefsData->aspectRatio);
	buttonGroup5Layout->addWidget( checkRatioImage );
	subTabImageLayout->addMultiCellWidget( buttonGroup5, 1, 1, 0, 1 );
	comboFillImage = new QComboBox( true, subTabImage, "comboFillImage" );
	comboFillImage->setEditable(false);
	QPixmap pm2I = QPixmap(15, 15);
	comboFillImage->insertItem( tr("None"));
	if (prefsData->dBrushPict == "None")
		comboFillImage->setCurrentItem(comboFillImage->count()-1);
	if (doc != 0)
	{
		for (itc = doc->PageColors.begin(); itc != doc->PageColors.end(); ++itc)
		{
			pm2I.fill(doc->PageColors[itc.key()].getRGBColor());
			comboFillImage->insertItem(pm2I, itc.key());
			if (itc.key() == prefsData->dBrushPict)
				comboFillImage->setCurrentItem(comboFillImage->count()-1);
		}
	}
	else
	{
		for (itc = ScApp->Prefs.DColors.begin(); itc != ScApp->Prefs.DColors.end(); ++itc)
		{
			pm2I.fill(ScApp->Prefs.DColors[itc.key()].getRGBColor());
			comboFillImage->insertItem(pm2I, itc.key());
			if (itc.key() == prefsData->dBrushPict)
				comboFillImage->setCurrentItem(comboFillImage->count()-1);
		}
	}
	subTabImageLayout->addWidget( comboFillImage, 2, 1, Qt::AlignLeft );
	textLabel19b = new QLabel(comboFillImage, tr( "F&ill Color:" ), subTabImage, "textLabel19b" );
	subTabImageLayout->addWidget( textLabel19b, 2, 0 );
	shadingFillImage = new QSpinBox( subTabImage, "shadingFillImage" );
	shadingFillImage->setMaxValue( 100 );
	shadingFillImage->setValue( prefsData->shadePict );
	shadingFillImage->setSuffix( tr( " %" ) );
	subTabImageLayout->addWidget( shadingFillImage, 3, 1, Qt::AlignLeft );
	textLabel20b = new QLabel(shadingFillImage, tr( "S&hading:" ), subTabImage, "textLabel20b" );
	subTabImageLayout->addWidget( textLabel20b, 3, 0 );
	subStackTools->addWidget( subTabImage, 3 );

	subTabPolygon = new QWidget( subStackTools, "subTabPolygon" );
	subTabPolygonLayout = new QHBoxLayout( subTabPolygon, 11, 6, "subTabPolygonLayout");
	subTabPolygonLayout->setAlignment( Qt::AlignTop );
	polyWidget = new PolygonWidget(subTabPolygon, prefsData->polyC, prefsData->polyFd, prefsData->polyF, prefsData->polyS, prefsData->polyR);
	subTabPolygonLayout->addWidget( polyWidget );
	subStackTools->addWidget( subTabPolygon, 4 );

	subTabZoom = new QWidget( subStackTools, "subTabZoom" );
	subTabZoomLayout = new QGridLayout( subTabZoom, 1, 1, 11, 6, "subTabZoomLayout");
	subTabZoomLayout->setAlignment( Qt::AlignTop );
	minimumZoom = new QSpinBox( subTabZoom, "minimumZoom" );
	minimumZoom->setMaxValue( 3200 );
	minimumZoom->setMinValue( 10 );
	minimumZoom->setValue(prefsData->magMin);
	minimumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( minimumZoom, 0, 1, Qt::AlignLeft );
	textLabel21b = new QLabel( minimumZoom, tr( "Mi&nimum:" ), subTabZoom, "textLabel21b" );
	subTabZoomLayout->addWidget( textLabel21b, 0, 0);
	maximumZoom = new QSpinBox( subTabZoom, "maximumZoom" );
	maximumZoom->setMaxValue( 3200 );
	maximumZoom->setMinValue( 10 );
	maximumZoom->setValue(prefsData->magMax);
	maximumZoom->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( maximumZoom, 1, 1, Qt::AlignLeft );
	textLabel22b = new QLabel( maximumZoom, tr( "Ma&ximum:" ), subTabZoom, "textLabel22b" );
	subTabZoomLayout->addWidget( textLabel22b, 1, 0 );
	zoomStep = new QSpinBox( subTabZoom, "zoomStep" );
	zoomStep->setMaxValue( 200 );
	zoomStep->setMinValue( 1 );
	zoomStep->setLineStep( 25 );
	zoomStep->setValue( prefsData->magStep );
	zoomStep->setSuffix( tr( " %" ) );
	subTabZoomLayout->addWidget( zoomStep, 2, 1, Qt::AlignLeft );
	textLabel23b = new QLabel( zoomStep, tr( "&Stepping:" ), subTabZoom, "textLabel23b" );
	subTabZoomLayout->addWidget( textLabel23b, 2, 0 );
	subStackTools->addWidget( subTabZoom, 5 );
	tabToolsLayout->addWidget( subStackTools );
	toolText->setOn(true);
	setSample();
	QToolTip::add( toolText, tr( "Text Frame Properties" ) );
	QToolTip::add( toolImage, tr( "Picture Frame Properties" ) );
	QToolTip::add( toolShape, tr( "Shape Drawing Properties" ) );
	QToolTip::add( toolZoom, tr( "Magnification Level Defaults" ) );
	QToolTip::add( toolLine, tr( "Line Drawing Properties" ) );
	QToolTip::add( toolPoly, tr( "Polygon Drawing Properties" ) );
	QToolTip::add( fontComboText, tr( "Font for new text frames" ) );
	QToolTip::add( sizeComboText, tr( "Size of font for new text frames" ) );
	QToolTip::add( colorComboText, tr( "Color of font" ) );
	QToolTip::add( columnsText, tr( "Number of columns in a text frame" ) );
	QToolTip::add( gapText, tr( "Gap between text frame columns" ) );
	QToolTip::add( previewText, tr( "Sample of your font" ) );
	QToolTip::add( buttonGroup3, tr( "Picture frames allow pictures to scale to any size" ) );
	QToolTip::add( scalingHorizontal, tr( "Horizontal scaling of images" ) );
	QToolTip::add( scalingVertical, tr( "Vertical scaling of images" ) );
	QToolTip::add( chainButton, tr( "Keep horizontal and vertical scaling the same" ) );
	QToolTip::add( buttonGroup5, tr( "Pictures in picture frames are scaled to the size of the frame" ) );
	QToolTip::add( checkRatioImage, tr( "Automatically scaled pictures keep their original proportions" ) );
	QToolTip::add( comboFillImage, tr( "Fill color of picture frames" ) );
	QToolTip::add( shadingFillImage, tr( "Saturation of color of fill" ) );
	QToolTip::add( colorComboLineShape, tr( "Line color of shapes" ) );
	QToolTip::add( shadingLineShape, tr( "Saturation of color of lines" ) );
	QToolTip::add( comboFillShape, tr( "Fill color of shapes" ) );
	QToolTip::add( shadingFillShape, tr( "Saturation of color of fill" ) );
	QToolTip::add( comboStyleShape, tr( "Line style of shapes" ) );
	QToolTip::add( lineWidthShape, tr( "Line width of shapes" ) );
	QToolTip::add( minimumZoom, tr( "Minimum magnification allowed" ) );
	QToolTip::add( maximumZoom, tr( "Maximum magnification allowed" ) );
	QToolTip::add( zoomStep, tr( "Change in magnification for each zoom operation" ) );
	QToolTip::add( colorComboLine, tr( "Color of lines" ) );
	QToolTip::add( shadingLine, tr( "Saturation of color" ) );
	QToolTip::add( comboStyleLine, tr( "Style of lines" ) );
	QToolTip::add( lineWidthLine, tr( "Width of lines" ) );
	connect(toolShape, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolPoly, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolImage, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolText, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolLine, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(toolZoom, SIGNAL(clicked()), this, SLOT(setTool()));
	connect(fontComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
	connect(sizeComboText, SIGNAL(activated(int)), this, SLOT(setSample()));
	connect(buttonGroup3, SIGNAL(clicked(int)), this, SLOT(changeImageScalingFree(int)));
	connect(buttonGroup5, SIGNAL(clicked(int)), this, SLOT(changeImageScalingRatio(int)));
	connect(chainButton, SIGNAL(clicked()), this, SLOT(toggleChain()));
	connect(scalingHorizontal, SIGNAL(valueChanged(int)), this, SLOT(hChange()));
	connect(scalingVertical, SIGNAL(valueChanged(int)), this, SLOT(vChange()));
}

void TabTools::restoreDefaults()
{
}

/*!
 \fn void TabTools::ToggleKette()
 \author Franz Schmid 
 \date  
 \brief TabTools (Tools, Image Frame), Sets Y Scale value from X Scale value when chain is toggled
 \param None
 \retval None 
 */
void TabTools::toggleChain()
{
	if (chainButton->isOn())
		scalingVertical->setValue(scalingHorizontal->value());
}

/*!
 \fn void TabTools::HChange()
 \author Franz Schmid 
 \date  
 \brief TabTools (Tools, Image Frame), Sets Y Scale value from X Scale value when X Scale value is changed
 \param None
 \retval None 
 */
void TabTools::hChange()
{
	if (chainButton->isOn())
		scalingVertical->setValue(scalingHorizontal->value());
}

/*!
 \fn void TabTools::vChange()
 \author Franz Schmid 
 \date  
 \brief TabTools (Tools, Image Frame). Sets X Scale value from Y Scale value when Y Scale value is changed
 \param None
 \retval None 
 */
void TabTools::vChange()
{
	if (chainButton->isOn())
		scalingHorizontal->setValue(scalingVertical->value());
}

void TabTools::changeImageScalingFree(int)
{
	if (buttonGroup3->isChecked())
		buttonGroup5->setChecked(false);
	else
		buttonGroup5->setChecked(true);
}

void TabTools::changeImageScalingRatio(int)
{
	if (buttonGroup5->isChecked())
		buttonGroup3->setChecked(false);
	else
		buttonGroup3->setChecked(true);
}

/*!
 \fn void TabTools::SetSample()
 \author Franz Schmid 
 \date  
 \brief TabTools (Tools, Text frame), Sets the sample text in selected font in text frame preferences
 \param None
 \retval None 
 */
void TabTools::setSample()
{
	QString ts = tr( "Woven silk pyjamas exchanged for blue quartz" );
	QString da = (*fon)[fontComboText->currentText()]->Datei;
	int s = sizeComboText->currentText().left(2).toInt();
	QPixmap pm = fontSamples(da, s, ts, paletteBackgroundColor());
	previewText->setPixmap(pm);
}

/*!
 \fn void TabTools::SetTool()
 \author Franz Schmid 
 \date  
 \brief TabTools (Tools), Raises widget for selected Tool properties
 \param None
 \retval None 
 */
void TabTools::setTool()
{
	if (toolText == sender())
		subStackTools->raiseWidget(0);
	if (toolShape == sender())
		subStackTools->raiseWidget(1);
	if (toolLine == sender())
		subStackTools->raiseWidget(2);
	if (toolImage == sender())
		subStackTools->raiseWidget(3);
	if (toolPoly == sender())
		subStackTools->raiseWidget(4);
	if (toolZoom == sender())
		subStackTools->raiseWidget(5);
}
