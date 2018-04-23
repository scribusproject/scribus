/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scribusdoc.cpp  -  description
                             -------------------
    begin                : Fre Apr  6 21:47:55 CEST 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <memory>
#include <utility>
#include <sstream>

#include <QByteArray>
#include <QDebug>
#include <QDialog>
#include <QEventLoop>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QPointer>
#include <QProgressBar>
#include <QtAlgorithms>
#include <QTime>
//#include <qtconcurrentmap.h>

#include "actionmanager.h"
#include "appmodes.h"
#include "text/boxes.h"
#include "canvas.h"
#include "colorblind.h"
#include "commonstrings.h"
#include "desaxe/digester.h"
#include "fileloader.h"
#include "filewatcher.h"
#include "fpoint.h"
#include "hyphenator.h"
#include "langmgr.h"
#include "notesstyles.h"
#include "numeration.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"
#include "pageitem_latexframe.h"
#include "pageitem_line.h"
#include "pageitem_noteframe.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#ifdef HAVE_OSG
	#include "pageitem_osgframe.h"
#endif
#include "colormgmt/sccolormgmtenginefactory.h"
#include "pageitem_arc.h"
#include "pageitem_group.h"
#include "pageitem_regularpolygon.h"
#include "pageitem_spiral.h"
#include "pageitem_symbol.h"
#include "pagesize.h"
#include "pagestructs.h"
#include "pdfwriter.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "resourcecollection.h"
#include "scclocale.h"
#include "sccolorengine.h"
#include "sclimits.h"
#include "scpage.h"
#include "scpainter.h"
#include "scraction.h"
#include "scribusXml.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribuswin.h"
#include "selection.h"
#include "serializer.h"
#include "tableborder.h"
#include "text/textlayoutpainter.h"
#include "text/textshaper.h"
#include "ui/guidemanager.h"
#include "ui/hruler.h"
#include "ui/inserttablecolumnsdialog.h"
#include "ui/inserttablerowsdialog.h"
#include "ui/layers.h"
#include "ui/mark2item.h"
#include "ui/mark2mark.h"
#include "ui/markanchor.h"
#include "ui/markinsert.h"
#include "ui/marksmanager.h"
#include "ui/markvariabletext.h"
#include "ui/notesstyleseditor.h"
#include "ui/outlinepalette.h"
#include "ui/pagepalette.h"
#include "ui/storyeditor.h"
#include "ui/tablecolumnwidthsdialog.h"
#include "ui/tablerowheightsdialog.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"


// static const bool FRAMESELECTION_EDITS_DEFAULTSTYLE = false;



/**
 This class forwards change events for pages and pageitems to 
 the region occupied by this page or pageitem.
 */
class DocUpdater : public Observer<ScPage*>, public Observer<PageItem*>
{
	ScribusDoc* doc;
	int  m_updateEnabled;
	bool m_docChangeNeeded;
public:
	DocUpdater(ScribusDoc* d) : doc(d), m_updateEnabled(0), m_docChangeNeeded(false) {}

	bool inUpdateSession() const
	{ 
		return m_updateEnabled > 0;
	}

	void beginUpdate(void)
	{ 
		if (m_updateEnabled == 0)
			m_docChangeNeeded = false;
		++m_updateEnabled;
	}

	void endUpdate(void)
	{
		--m_updateEnabled;
		if (m_updateEnabled <= 0)
		{
			if (m_docChangeNeeded)
			{
				doc->changed();
				m_docChangeNeeded = false;
			}
		}
	}
	
	void changed(ScPage* pg, bool /*doLayout*/)
	{
		QRectF pagebox(pg->xOffset(), pg->yOffset(), pg->width(), pg->height());
		doc->invalidateRegion(pagebox);
		doc->regionsChanged()->update(pagebox);
		if (m_updateEnabled <= 0)
		{
			doc->changed();
			return;
		}
		m_docChangeNeeded = true;
	}
	
	void changed(PageItem* it, bool doLayout)
	{
		it->invalidateLayout();
		if (doLayout)
			it->layout();
		double x, y, w, h;
		QTransform t = it->getTransform();
		w = it->visualWidth();
		h = it->visualHeight();
		x = -it->visualLineWidth() / 2.0;
		y = -it->visualLineWidth() / 2.0;
		QRectF upRect = t.mapRect(QRectF(x, y, w, h));
		doc->regionsChanged()->update(upRect);
		if (m_updateEnabled <= 0)
		{
			doc->changed();
			return;
		}
		m_docChangeNeeded = true;
	}

	void setDocChangeNeeded(bool changeNeeded = true)
	{
		m_docChangeNeeded = changeNeeded;
	}
};




ScribusDoc::ScribusDoc() : UndoObject( tr("Document")), Observable<ScribusDoc>(NULL),
	m_hasGUI(false),
	m_docFilePermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner),
	m_appPrefsData(PrefsManager::instance()->appPrefs),
	m_docPrefsData(PrefsManager::instance()->appPrefs),
	m_undoManager(UndoManager::instance()),
	m_loading(false),
	m_modified(false),
	m_ActiveLayer(0),
	m_rotMode(0),
	m_automaticTextFrames(0),
	m_masterPageMode(false),
	m_symbolEditMode(false),
	m_inlineEditMode(false),
	m_ScMW(0),
	m_View(0),
	m_guardedObject(this),
	m_serializer(NULL),
	m_tserializer(NULL),
	is12doc(false),
	NrItems(0),
	First(1), Last(0),
	viewCount(0), viewID(0),
	SnapGrid(false),
	SnapGuides(true),
	SnapElement(false), GuideLock(false),
	minCanvasCoordinate(FPoint(0, 0)),
	rulerXoffset(0.0), rulerYoffset(0.0),
	Pages(0), MasterPages(), DocPages(),
	MasterNames(),
	Items(0), MasterItems(), DocItems(), FrameItems(),
	m_Selection(new Selection(this, true)),
	PageSp(1), PageSpa(0),
	FirstPnum(1),
	PageColors(this, true),
	appMode(modeNormal),
	SubMode(-1),
	ShapeValues(0),
	ValCount(0),
	DocName( tr("Document")+"-"),
	UsedFonts(),
	AllFonts(&m_appPrefsData.fontPrefs.AvailFonts),
	AObjects(),
	CurrentSel(-1),
	nodeEdit(),
	LastAuto(0), FirstAuto(0),
	DraggedElem(0),
	ElemToLink(0),
	DragElements(),
	m_docParagraphStyles(),
	m_docCharStyles(),
	m_docTableStyles(),
	m_docCellStyles(),
	Layers(),
	GroupCounter(1),
	colorEngine(ScCore->defaultEngine),
	JavaScripts(),
	TotalItems(0),
	RePos(false),
	BookMarks(),
	OldBM(false),
	hasName(false),
	isConverted(false),
	autoSaveTimer(new QTimer(this)),
	MLineStyles(),
	WinHan(0),
	DoDrawing(true),
	OpenNodes(),
	CurTimer(0),
	pageErrors(),
	docLayerErrors(),
	docItemErrors(),
	masterItemErrors(),
	symReturn(), symNewLine(), symTab(), symNonBreak(), symNewCol(), symNewFrame(),
	docHyphenator(0),
	m_itemCreationTransaction(NULL),
	m_alignTransaction(NULL),
	m_currentPage(NULL),
	m_updateManager(),
	m_docUpdater(NULL),
	m_flag_notesChanged(false),
	flag_restartMarksRenumbering(false),
	flag_updateMarksLabels(false),
	flag_updateEndNotes(false),
	flag_layoutNotesFrames(true),
	flag_Renumber(false),
	flag_NumUpdateRequest(false)
{
	m_docUnitRatio=unitGetRatioFromIndex(m_docPrefsData.docSetupPrefs.docUnitIndex);
	m_docPrefsData.docSetupPrefs.pageHeight=0;
	m_docPrefsData.docSetupPrefs.pageWidth=0;
	m_docPrefsData.docSetupPrefs.pagePositioning=0;
	maxCanvasCoordinate=(FPoint(m_docPrefsData.displayPrefs.scratch.left() + m_docPrefsData.displayPrefs.scratch.right(), m_docPrefsData.displayPrefs.scratch.top() + m_docPrefsData.displayPrefs.scratch.bottom())),
	init();
	m_docPrefsData.pdfPrefs.bleeds = m_docPrefsData.docSetupPrefs.bleeds;
	m_docPrefsData.pdfPrefs.useDocBleeds = true;
	Print_Options.firstUse = true;
	drawAsPreview = false;
	viewAsPreview = false;
	editOnPreview = false;
	previewVisual = 0;
	dontResize = false;
	//create default numeration
	NumStruct * numS = new NumStruct;
	numS->m_name = "default";
	Numeration newNum;
	numS->m_nums.insert(0, newNum);
	numS->m_counters.insert(0, 0);
	numS->m_lastlevel = -1;
	numerations.insert("default", numS);

	currentEditedTextframe = NULL;
}


ScribusDoc::ScribusDoc(const QString& docName, int unitindex, const PageSize& pagesize, const MarginStruct& margins, const DocPagesSetup& pagesSetup) : UndoObject( tr("Document")),
	m_hasGUI(false),
	m_docFilePermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner),
	m_appPrefsData(PrefsManager::instance()->appPrefs),
	m_docPrefsData(PrefsManager::instance()->appPrefs),
	m_undoManager(UndoManager::instance()),
	m_loading(false),
	m_modified(false),
	m_ActiveLayer(0),
	m_docUnitRatio(unitGetRatioFromIndex(m_appPrefsData.docSetupPrefs.docUnitIndex)),
	m_rotMode(0),
	m_automaticTextFrames(pagesSetup.autoTextFrames),
	m_masterPageMode(false),
	m_symbolEditMode(false),
	m_inlineEditMode(false),
	m_ScMW(0),
	m_View(0),
	m_guardedObject(this),
	m_serializer(NULL),
	m_tserializer(NULL),
	is12doc(false),
	NrItems(0),
	First(1), Last(0),
	viewCount(0), viewID(0),
	SnapGrid(false),
	SnapGuides(true),
	SnapElement(false),
	GuideLock(false),
	minCanvasCoordinate(FPoint(0, 0)),
	rulerXoffset(0.0), rulerYoffset(0.0),
	Pages(0), MasterPages(), DocPages(),
	MasterNames(),
	Items(0), MasterItems(), DocItems(), FrameItems(),
	m_Selection(new Selection(this, true)),
	PageSp(pagesSetup.columnCount), PageSpa(pagesSetup.columnDistance),
	FirstPnum(pagesSetup.firstPageNumber),
	PageColors(this, true),
	appMode(modeNormal),
	SubMode(-1),
	ShapeValues(0),
	ValCount(0),
	DocName(docName),
	UsedFonts(),
	AllFonts(&m_appPrefsData.fontPrefs.AvailFonts),
	AObjects(),
	CurrentSel(-1),
	nodeEdit(),
	LastAuto(0), FirstAuto(0),
	DraggedElem(0),
	ElemToLink(0),
	DragElements(),
	m_docParagraphStyles(),
	m_docCharStyles(),
	m_docTableStyles(),
	m_docCellStyles(),
	Layers(),
	GroupCounter(1),
	colorEngine(ScCore->defaultEngine),
	JavaScripts(),
	TotalItems(0),
	RePos(false),
	BookMarks(),
	OldBM(false),
	hasName(false),
	isConverted(false),
	autoSaveTimer(new QTimer(this)),
	MLineStyles(),
	WinHan(0),
	DoDrawing(true),
	OpenNodes(),
	CurTimer(0),
	pageErrors(),
	docLayerErrors(),
	docItemErrors(),
	masterItemErrors(),
	symReturn(), symNewLine(), symTab(), symNonBreak(), symNewCol(), symNewFrame(),
	docHyphenator(0),
	m_itemCreationTransaction(NULL),
	m_alignTransaction(NULL),
	m_currentPage(NULL),
	m_updateManager(),
	m_docUpdater(NULL),
	m_flag_notesChanged(false),
	flag_restartMarksRenumbering(false),
	flag_updateMarksLabels(false),
	flag_updateEndNotes(false),
	flag_layoutNotesFrames(true),
	flag_Renumber(false),
	flag_NumUpdateRequest(false)
{
	m_docPrefsData.docSetupPrefs.docUnitIndex=unitindex;
	m_docPrefsData.docSetupPrefs.pageHeight=pagesize.height();
	m_docPrefsData.docSetupPrefs.pageWidth=pagesize.width();
	m_docPrefsData.docSetupPrefs.pageSize=pagesize.name();
	m_docPrefsData.docSetupPrefs.margins=margins;
	maxCanvasCoordinate=(FPoint(m_docPrefsData.displayPrefs.scratch.left() + m_docPrefsData.displayPrefs.scratch.right(), m_docPrefsData.displayPrefs.scratch.top() + m_docPrefsData.displayPrefs.scratch.bottom())),
	setPageSetFirstPage(pagesSetup.pageArrangement, pagesSetup.firstPageLocation);
	init();
	m_docPrefsData.pdfPrefs.bleeds = m_docPrefsData.docSetupPrefs.bleeds;
	m_docPrefsData.pdfPrefs.useDocBleeds = true;
	m_docPrefsData.docSetupPrefs.pageOrientation=pagesSetup.orientation;
	m_docPrefsData.docSetupPrefs.pagePositioning=pagesSetup.pageArrangement;
	Print_Options.firstUse = true;
	drawAsPreview = false;
	viewAsPreview = false;
	editOnPreview = false;
	previewVisual = 0;
	dontResize = false;
	currentEditedTextframe = NULL;
}


void ScribusDoc::init()
{
	Q_CHECK_PTR(m_Selection);
	Q_CHECK_PTR(autoSaveTimer);

	HasCMS = false;
	m_docPrefsData.colorPrefs.DCMSset.CMSinUse = false;

	colorEngine = ScCore->defaultEngine;
	SetDefaultCMSParams();

	// init update change management
	setUpdateManager(&m_updateManager);
	m_itemsChanged.setUpdateManager(&m_updateManager);
	m_pagesChanged.setUpdateManager(&m_updateManager);
	m_regionsChanged.setUpdateManager(&m_updateManager);
	nodeEdit.setUpdateManager(&m_updateManager);
	
	// setup update() event chains
	m_docUpdater = new DocUpdater(this);
	m_itemsChanged.connectObserver(m_docUpdater);
	m_pagesChanged.connectObserver(m_docUpdater);

	PrefsManager *prefsManager = PrefsManager::instance();
	m_docPrefsData.colorPrefs.DCMSset = prefsManager->appPrefs.colorPrefs.DCMSset;
	m_docPrefsData.pdfPrefs.SolidProf = m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
	m_docPrefsData.pdfPrefs.ImageProf = m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
	m_docPrefsData.pdfPrefs.PrintProf = m_docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile;
	m_docPrefsData.pdfPrefs.Intent  = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
	m_docPrefsData.pdfPrefs.Intent2 = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;

	AddFont(m_appPrefsData.itemToolPrefs.textFont);//, prefsData.AvailFonts[prefsData.itemToolPrefs.textFont]->Font);
	//FIXME: aren't we doing this now anyway with prefs struct copy?
	m_docPrefsData.itemToolPrefs.textFont = m_appPrefsData.itemToolPrefs.textFont;
	m_docPrefsData.itemToolPrefs.textSize = m_appPrefsData.itemToolPrefs.textSize;
	m_docPrefsData.itemToolPrefs.textTabFillChar = m_appPrefsData.itemToolPrefs.textTabFillChar;
	m_docPrefsData.opToolPrefs.dispX = m_appPrefsData.opToolPrefs.dispX;
	m_docPrefsData.opToolPrefs.dispY = m_appPrefsData.opToolPrefs.dispY;
	m_docPrefsData.opToolPrefs.constrain = m_appPrefsData.opToolPrefs.constrain;

	PageColors.ensureDefaultColors();
	if (m_appPrefsData.itemToolPrefs.shapeLineColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.shapeLineColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.shapeLineColor]);
	m_docPrefsData.itemToolPrefs.shapeLineColor = m_appPrefsData.itemToolPrefs.shapeLineColor;
	if (m_appPrefsData.itemToolPrefs.lineColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.lineColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.lineColor]);
	m_docPrefsData.itemToolPrefs.lineColor = m_appPrefsData.itemToolPrefs.lineColor;
	if (m_appPrefsData.itemToolPrefs.textColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.textColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.textColor]);
	m_docPrefsData.itemToolPrefs.textColor = m_appPrefsData.itemToolPrefs.textColor;
	if (m_appPrefsData.itemToolPrefs.textStrokeColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.textStrokeColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.textStrokeColor]);
	m_docPrefsData.itemToolPrefs.textStrokeColor = m_appPrefsData.itemToolPrefs.textStrokeColor;
	if (m_appPrefsData.itemToolPrefs.shapeFillColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.shapeFillColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.shapeFillColor]);
	m_docPrefsData.itemToolPrefs.shapeFillColor = m_appPrefsData.itemToolPrefs.shapeFillColor;
	if (m_appPrefsData.itemToolPrefs.imageFillColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.imageFillColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.imageFillColor]);
	m_docPrefsData.itemToolPrefs.imageFillColor = m_appPrefsData.itemToolPrefs.imageFillColor;
	if (m_appPrefsData.itemToolPrefs.imageStrokeColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.imageStrokeColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.imageStrokeColor]);
	m_docPrefsData.itemToolPrefs.imageStrokeColor = m_appPrefsData.itemToolPrefs.imageStrokeColor;
	if (m_appPrefsData.itemToolPrefs.textFillColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.textFillColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.textFillColor]);
	m_docPrefsData.itemToolPrefs.textFillColor = m_appPrefsData.itemToolPrefs.textFillColor;
	if (m_appPrefsData.itemToolPrefs.textLineColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.textLineColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.textLineColor]);
	if (m_appPrefsData.itemToolPrefs.calligraphicPenFillColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.calligraphicPenFillColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.calligraphicPenFillColor]);
	if (m_appPrefsData.itemToolPrefs.calligraphicPenLineColor != CommonStrings::None)
		PageColors.insert(m_appPrefsData.itemToolPrefs.calligraphicPenLineColor, m_appPrefsData.colorPrefs.DColors[m_appPrefsData.itemToolPrefs.calligraphicPenLineColor]);

	
	ParagraphStyle pstyle;
	pstyle.setDefaultStyle(true);
	pstyle.setName(CommonStrings::DefaultParagraphStyle);
	pstyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
	pstyle.setLineSpacing(15);
	pstyle.setAlignment(ParagraphStyle::Leftaligned);
	pstyle.setDirection(ParagraphStyle::LTR);
	pstyle.setLeftMargin(0);
	pstyle.setFirstIndent(0);
	pstyle.setRightMargin(0);
	pstyle.setGapBefore(0);
	pstyle.setGapAfter(0);
	pstyle.setHasDropCap(false);
	pstyle.setHasBullet(false);
	pstyle.setHasNum(false);
	pstyle.setHyphenConsecutiveLines(2);
	pstyle.setDropCapLines(2);
	pstyle.setParEffectOffset(0);
	pstyle.setBackgroundColor(CommonStrings::None);
	pstyle.setBackgroundShade(100);
	pstyle.charStyle().setParent("");
	
	CharStyle cstyle;
	cstyle.setDefaultStyle(true);
	cstyle.setName(CommonStrings::DefaultCharacterStyle);
	cstyle.setFont(m_appPrefsData.fontPrefs.AvailFonts[m_docPrefsData.itemToolPrefs.textFont]);
	cstyle.setFontSize(m_docPrefsData.itemToolPrefs.textSize);
	cstyle.setFontFeatures("");
	cstyle.setFeatures(QStringList(CharStyle::INHERIT));
	cstyle.setHyphenWordMin(3);
	cstyle.setFillColor(m_docPrefsData.itemToolPrefs.textColor);
	cstyle.setFillShade(m_docPrefsData.itemToolPrefs.textShade);
	cstyle.setStrokeColor(m_docPrefsData.itemToolPrefs.textStrokeColor);
	cstyle.setStrokeShade(m_docPrefsData.itemToolPrefs.textStrokeShade);
	cstyle.setBackColor(CommonStrings::None);
	cstyle.setBackShade(100);
	cstyle.setBaselineOffset(0);
	cstyle.setShadowXOffset(50);
	cstyle.setShadowYOffset(-50);
	cstyle.setOutlineWidth(10);
	cstyle.setUnderlineOffset(m_docPrefsData.typoPrefs.valueUnderlinePos);
	cstyle.setUnderlineWidth(m_docPrefsData.typoPrefs.valueUnderlineWidth);
	cstyle.setStrikethruOffset(m_docPrefsData.typoPrefs.valueStrikeThruPos);
	cstyle.setStrikethruWidth(m_docPrefsData.typoPrefs.valueStrikeThruPos);
	cstyle.setScaleH(1000);
	cstyle.setScaleV(1000);
	cstyle.setTracking(0);
	cstyle.setLanguage(PrefsManager::instance()->appPrefs.docSetupPrefs.language);
	
	m_docParagraphStyles.create(pstyle);
	m_docParagraphStyles.makeDefault( &(m_docParagraphStyles[0]) );
	
	m_docCharStyles.create(cstyle);
	m_docCharStyles.makeDefault( &(m_docCharStyles[0]) );
	
	m_docParagraphStyles[0].breakImplicitCharStyleInheritance();
	m_docParagraphStyles[0].charStyle().setContext( & m_docCharStyles );
//	docParagraphStyles[0].charStyle().setName( "cdocdefault" ); // DON'T TRANSLATE

	currentStyle = pstyle;

	// Create default table style.
	// TODO: We should have preferences for the default values.
	TableStyle defaultTableStyle;
	defaultTableStyle.setDefaultStyle(true);
	defaultTableStyle.setName(CommonStrings::DefaultTableStyle);
	defaultTableStyle.setFillColor(CommonStrings::None);
	defaultTableStyle.setFillShade(100);
	defaultTableStyle.setLeftBorder(TableBorder(1.0, Qt::SolidLine, "Black", 100));
	defaultTableStyle.setRightBorder(TableBorder(1.0, Qt::SolidLine, "Black", 100));
	defaultTableStyle.setTopBorder(TableBorder(1.0, Qt::SolidLine, "Black", 100));
	defaultTableStyle.setBottomBorder(TableBorder(1.0, Qt::SolidLine, "Black", 100));
	m_docTableStyles.create(defaultTableStyle);
	m_docTableStyles.makeDefault(&(m_docTableStyles[0]));

	// Create default table cell style.
	// TODO: We should have preferences for the default values.
	CellStyle defaultCellStyle;
	defaultCellStyle.setDefaultStyle(true);
	defaultCellStyle.setName(CommonStrings::DefaultCellStyle);
	defaultCellStyle.setFillColor(CommonStrings::None);
	defaultCellStyle.setFillShade(100);
	defaultCellStyle.setLeftBorder(TableBorder(1.0, Qt::SolidLine, "Black", 100));
	defaultCellStyle.setRightBorder(TableBorder(1.0, Qt::SolidLine, "Black", 100));
	defaultCellStyle.setTopBorder(TableBorder(1.0, Qt::SolidLine, "Black", 100));
	defaultCellStyle.setBottomBorder(TableBorder(1.0, Qt::SolidLine, "Black", 100));
	defaultCellStyle.setLeftPadding(1.0);
	defaultCellStyle.setRightPadding(1.0);
	defaultCellStyle.setTopPadding(1.0);
	defaultCellStyle.setBottomPadding(1.0);
	m_docCellStyles.create(defaultCellStyle);
	m_docCellStyles.makeDefault(&(m_docCellStyles[0]));
	
	Layers.addLayer( tr("Background") );
	// FIXME: Check PDF version input
	//TODO: Check if this is needed now we ue appPrefsData --> docPrefsData
	pdfOptions().Version = (PDFOptions::PDFVersion)m_appPrefsData.pdfPrefs.Version;

	pdfOptions().firstUse = true;
	docPatterns.clear();
	docGradients.clear();

	if (autoSave() && ScCore->usingGUI())
		autoSaveTimer->start(autoSaveTime());
	//Do this after all the collections have been created and cleared!
	m_masterPageMode=true; // quick hack to force the change of pointers in setMasterPageMode();
	setMasterPageMode(false);
	addSymbols();
	//for loading old documents where default notes style is not saved
	if (m_docNotesStylesList.isEmpty())
		m_docNotesStylesList.append(new NotesStyle());
}


ScribusDoc::~ScribusDoc()
{
	m_guardedObject.nullify();
	CloseCMSProfiles();
	ScCore->fileWatcher->stop();
	ScCore->fileWatcher->removeFile(DocName);
	QList<PageItem*> allItems;
	for (int i = 0; i < DocItems.count(); ++i)
	{
		PageItem *currItem = DocItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int j = 0; j < allItems.count(); j++)
		{
			currItem = allItems.at(j);
			if (currItem->imageIsAvailable)
				ScCore->fileWatcher->removeFile(currItem->Pfile);
			if ((currItem->asImageFrame()) && (!currItem->Pfile.isEmpty()))
			{
				QFileInfo fi(currItem->Pfile);
				ScCore->fileWatcher->removeDir(fi.absolutePath());
			}
		}
		allItems.clear();
	}
	for (int i = 0; i < MasterItems.count(); ++i)
	{
		PageItem *currItem = MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int j = 0; j < allItems.count(); j++)
		{
			currItem = allItems.at(j);
			if (currItem->imageIsAvailable)
				ScCore->fileWatcher->removeFile(currItem->Pfile);
			if ((currItem->asImageFrame()) && (!currItem->Pfile.isEmpty()))
			{
				QFileInfo fi(currItem->Pfile);
				ScCore->fileWatcher->removeDir(fi.absolutePath());
			}
		}
		allItems.clear();
	}
	for (QHash<int, PageItem*>::iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		PageItem *currItem = itf.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->imageIsAvailable)
				ScCore->fileWatcher->removeFile(currItem->Pfile);
			if ((currItem->asImageFrame()) && (!currItem->Pfile.isEmpty()))
			{
				QFileInfo fi(currItem->Pfile);
				ScCore->fileWatcher->removeDir(fi.absolutePath());
			}
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int i = 0; i < patterns.count(); ++i)
	{
		ScPattern pa = docPatterns[patterns[i]];
		for (int j = 0; j < pa.items.count(); j++)
		{
			PageItem *currItem = pa.items.at(j);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int k = 0; k < allItems.count(); k++)
			{
				currItem = allItems.at(k);
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					if ((currItem->imageIsAvailable) && (!currItem->Pfile.isEmpty()))
					{
						ScCore->fileWatcher->removeFile(currItem->Pfile);
						QFileInfo fi(currItem->Pfile);
						ScCore->fileWatcher->removeDir(fi.absolutePath());
					}
				}
			}
			allItems.clear();
		}
		while (!pa.items.isEmpty())
		{
			delete pa.items.takeFirst();
		}
	}
	//deleting resources alocated for marks and notes
	while (! m_docNotesList.isEmpty())
		delete m_docNotesList.takeFirst();
	while (!m_docMarksList.isEmpty())
		delete m_docMarksList.takeFirst();
	while (! m_docNotesStylesList.isEmpty())
		delete m_docNotesStylesList.takeFirst();
	docPatterns.clear();
	docGradients.clear();
	foreach (NumStruct* ns, numerations.values())
		delete ns;
	numerations.clear();
	while (!DocItems.isEmpty())
	{
		delete DocItems.takeFirst();
	}
	QList<PageItem*> tmList = FrameItems.values();
	while (!tmList.isEmpty())
	{
		delete tmList.takeFirst();
	}
	FrameItems.clear();

	while (!MasterPages.isEmpty())
	{
		delete MasterPages.takeFirst();
	}
	while (!DocPages.isEmpty())
	{
		delete DocPages.takeFirst();
	}
	while (!MasterItems.isEmpty())
	{
		delete MasterItems.takeFirst();
	}
	QMap<QString,int>::Iterator it3;
	for (it3 = UsedFonts.begin(); it3 != UsedFonts.end(); ++it3)
	{
		if (!(*AllFonts)[it3.key()].localForDocument().isEmpty())
			(*AllFonts).removeFont(it3.key());
		else
			(*AllFonts)[it3.key()].decreaseUsage();
	}
	if (docHyphenator)
		delete docHyphenator;
	if (m_serializer)
		delete m_serializer;
	if (m_tserializer)
		delete m_tserializer;
	if (m_docUpdater)
		delete m_docUpdater;
	if (!m_docPrefsData.docSetupPrefs.AutoSaveKeep)
	{
		if (autoSaveFiles.count() != 0)
		{
			for (int i = 0; i < autoSaveFiles.count(); i++)
			{
				QFile f(autoSaveFiles[i]);
				f.remove();
			}
			autoSaveFiles.clear();
		}
	}
	ScCore->fileWatcher->start();
}

bool ScribusDoc::inAnEditMode() const
{
	bool inEditMode = (appMode == modeDrawBezierLine ||
					   appMode == modeEdit ||
					   appMode == modeEditArc ||
					   appMode == modeEditClip ||
					   appMode == modeEditGradientVectors ||
					   appMode == modeEditMeshGradient ||
					   appMode == modeEditMeshPatch ||
					   appMode == modeEditPolygon ||
					   appMode == modeEditSpiral ||
					   appMode == modeEditTable ||
					   appMode == modeEditWeldPoint
					   ) ? true : false;

	return inEditMode;
}

bool ScribusDoc::inASpecialEditMode() const
{ // #12897, modes that use setSpecialEditMode() function in appmodehelper.cpp
	bool inSpecialEditMode = (appMode == modeDrawBezierLine ||
							  appMode == modeEditArc ||
							  appMode == modeEditGradientVectors ||
							  appMode == modeEditMeshGradient ||
							  appMode == modeEditMeshPatch ||
							  appMode == modeEditPolygon ||
							  appMode == modeEditSpiral ||
							  appMode == modeEditWeldPoint
							  ) ? true : false;

	return inSpecialEditMode;
}

QList<PageItem*> ScribusDoc::getAllItems(QList<PageItem*> &items)
{
	QList<PageItem*> ret;
	for (int em = 0; em < items.count(); ++em)
	{
		PageItem* embedded = items.at(em);
		ret.append(embedded);
		if (embedded->isGroup())
			ret += embedded->getAllChildren();
	}
	return ret;
}

QList<PageItem*> *ScribusDoc::parentGroup(PageItem* item, QList<PageItem*> *list)
{
	QList<PageItem*> *retList = NULL;
	if (list->contains(item))
		retList = list;
	else
	{
		for (int i = 0; i < list->count(); ++i)
		{
			PageItem* embedded = list->at(i);
			if (embedded->isGroup())
			{
				retList = parentGroup(item, &embedded->asGroupFrame()->groupItemList);
				if (retList != NULL)
					break;
			}
		}
	}
	return retList;
}

void ScribusDoc::setup(const int unitIndex, const int fp, const int firstLeft, const int orientation, const int firstPageNumber, const QString& defaultPageSize, const QString& documentName)
{
	m_docPrefsData.docSetupPrefs.docUnitIndex=unitIndex;
	setPageSetFirstPage(fp, firstLeft);
	m_docPrefsData.docSetupPrefs.pageOrientation = orientation;
	m_docPrefsData.docSetupPrefs.pageSize = defaultPageSize;
	FirstPnum = firstPageNumber;
	m_docPrefsData.docSetupPrefs.pagePositioning = fp;
	setName(documentName);
	HasCMS = false;
	if (!pdfOptions().UseLPI)
	{
		pdfOptions().LPISettings.clear();
		struct LPIData lpo;
		lpo.Frequency = 133;
		lpo.SpotFunc = 3;
		lpo.Angle = 105;
		pdfOptions().LPISettings.insert("Cyan", lpo);
		lpo.Angle = 75;
		pdfOptions().LPISettings.insert("Magenta", lpo);
		lpo.Angle = 90;
		pdfOptions().LPISettings.insert("Yellow", lpo);
		lpo.Angle = 45;
		pdfOptions().LPISettings.insert("Black", lpo);
		m_ActiveLayer = 0;
	}

	appMode = modeNormal;
	PrefsManager *prefsManager=PrefsManager::instance();

	m_docPrefsData.colorPrefs.DCMSset = prefsManager->appPrefs.colorPrefs.DCMSset;
	m_docPrefsData.pdfPrefs.SolidProf = m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
	m_docPrefsData.pdfPrefs.ImageProf = m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
	m_docPrefsData.pdfPrefs.PrintProf = m_docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile;
	m_docPrefsData.pdfPrefs.Intent  = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
	m_docPrefsData.pdfPrefs.Intent2 = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
	BlackPoint   = m_docPrefsData.colorPrefs.DCMSset.BlackPoint;
	SoftProofing = m_docPrefsData.colorPrefs.DCMSset.SoftProofOn;
	Gamut        = m_docPrefsData.colorPrefs.DCMSset.GamutCheck;
	IntentColors = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
	IntentImages = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
	if (ScCore->haveCMS() && m_docPrefsData.colorPrefs.DCMSset.CMSinUse)
	{
		if (OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles))
		{
			HasCMS = true;
			pdfOptions().SComp = m_docPrefsData.colorPrefs.DCMSset.ComponentsInput2;
		}
		else
		{
			SetDefaultCMSParams();
			HasCMS = false;
		}
	}
}


void ScribusDoc::setGUI(bool hasgui, ScribusMainWindow* mw, ScribusView* view)
{
	m_hasGUI = hasgui;
	m_ScMW = mw;
	//FIXME: stop using m_View
	m_View = view;
}

void ScribusDoc::createHyphenator()
{
	if (m_hasGUI)
	{
		if (docHyphenator)
			return;
		docHyphenator = new Hyphenator(m_ScMW, this);
		Q_CHECK_PTR(docHyphenator);
	}
	else
		docHyphenator = NULL;
}


void ScribusDoc::setLoading(const bool docLoading)
{
	m_loading = docLoading;
}


bool ScribusDoc::isLoading() const
{
	return m_loading;
}


//AV: eeks. That should not be used
ScribusView* ScribusDoc::view() const
{
	return m_View;
}


const ScGuardedPtr<ScribusDoc>& ScribusDoc::guardedPtr() const
{
	return m_guardedObject;
}


void ScribusDoc::CloseCMSProfiles()
{
	HasCMS = false;
	SetDefaultCMSParams();
}

void ScribusDoc::SetDefaultCMSParams()
{
	BlackPoint     = true;
	SoftProofing   = false;
	Gamut          = false;
	IntentColors   = Intent_Relative_Colorimetric;
	IntentImages   = Intent_Relative_Colorimetric;
	DocInputRGBProf       = ScCore->defaultRGBProfile;
	DocInputCMYKProf      = ScCore->defaultCMYKProfile;
	DocInputImageRGBProf  = ScCore->defaultRGBProfile;
	DocInputImageCMYKProf = ScCore->defaultCMYKProfile;
	DocDisplayProf        = ScCore->defaultRGBProfile;
	DocPrinterProf        = ScCore->defaultCMYKProfile;
	stdTransRGBMon        = ScCore->defaultRGBToScreenSolidTrans;
	stdTransCMYKMon       = ScCore->defaultCMYKToRGBTrans;
	stdTransRGB           = ScCore->defaultCMYKToRGBTrans;
	stdTransCMYK          = ScCore->defaultRGBToCMYKTrans;
	stdProof              = ScCore->defaultRGBToScreenSolidTrans;
	stdProofGC            = ScCore->defaultRGBToScreenSolidTrans;
	stdProofCMYK          = ScCore->defaultCMYKToRGBTrans;
	stdProofCMYKGC        = ScCore->defaultCMYKToRGBTrans;
	stdTransImg           = ScCore->defaultRGBToScreenImageTrans;
	stdProofImg           = ScCore->defaultRGBToScreenImageTrans;
	stdProofImgCMYK       = ScCore->defaultCMYKToScreenImageTrans;
	stdLabToRGBTrans      = ScCore->defaultLabToRGBTrans;
	stdLabToCMYKTrans     = ScCore->defaultLabToCMYKTrans;
	stdLabToScreenTrans   = ScCore->defaultLabToScreenTrans;
	stdProofLab           = ScCore->defaultLabToRGBTrans;
	stdProofLabGC         = ScCore->defaultLabToRGBTrans;
}

bool ScribusDoc::OpenCMSProfiles(ProfilesL InPo, ProfilesL InPoCMYK, ProfilesL MoPo, ProfilesL PrPo)
{
	HasCMS = false;
	ScColorProfile inputProf;

	colorEngine = colorMgmtEngineFactory.createDefaultEngine();
	ScColorMgmtStrategy colorStrategy;
	colorStrategy.useBlackPointCompensation = m_docPrefsData.colorPrefs.DCMSset.BlackPoint;
	colorStrategy.useBlackPreservation      = false;
	colorEngine.setStrategy(colorStrategy);

	DocDisplayProf   = ScCore->monitorProfile;
	DocInputRGBProf  = colorEngine.openProfileFromFile( InPo[m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile] );
	DocInputCMYKProf = colorEngine.openProfileFromFile( InPoCMYK[m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile] );
	DocPrinterProf   = colorEngine.openProfileFromFile( PrPo[m_docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile] );
	DocInputImageRGBProf  = colorEngine.openProfileFromFile( InPo[m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile] );
	DocInputImageCMYKProf = colorEngine.openProfileFromFile( InPoCMYK[m_docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile] );
	if ((!DocDisplayProf) || (!DocInputRGBProf) || (!DocInputCMYKProf) || (!DocPrinterProf) || (!DocInputImageCMYKProf) || (!DocInputImageRGBProf))
	{
		m_docPrefsData.colorPrefs.DCMSset.CMSinUse = false;
		return false;
	}

	int dcmsFlags   = 0;
	int dcmsFlagsGC = 0;
	if (m_docPrefsData.colorPrefs.DCMSset.GamutCheck)
		dcmsFlagsGC |= Ctf_GamutCheck;
	if (m_docPrefsData.colorPrefs.DCMSset.BlackPoint)
	{
		dcmsFlags   |= Ctf_BlackPointCompensation;
		dcmsFlagsGC |= Ctf_BlackPointCompensation;
	}
	stdTransRGBMon  = colorEngine.createTransform(DocInputRGBProf, Format_RGB_16,
										DocDisplayProf, Format_RGB_16,
										IntentColors, dcmsFlags);
	stdTransCMYKMon = colorEngine.createTransform(DocInputCMYKProf, Format_CMYK_16,
										DocDisplayProf, Format_RGB_16,
										IntentColors, dcmsFlags);
	// TODO : check input profiles used for images
	stdProofImg = colorEngine.createProofingTransform(DocInputImageRGBProf, Format_BGRA_8,
	                  DocDisplayProf, Format_BGRA_8, DocPrinterProf,
	                  IntentImages, Intent_Relative_Colorimetric, dcmsFlagsGC);
	stdProofImgCMYK = colorEngine.createProofingTransform(DocInputImageCMYKProf, Format_CMYK_8,
	                  DocDisplayProf, Format_BGRA_8, DocPrinterProf,
	                  IntentImages, Intent_Relative_Colorimetric, dcmsFlagsGC);
	stdTransImg = colorEngine.createTransform(DocInputRGBProf, Format_BGRA_8,
	                  DocDisplayProf, Format_BGRA_8, 
					  IntentImages, dcmsFlags);
	stdTransRGB = colorEngine.createTransform(DocInputCMYKProf, Format_CMYK_16,
					  DocInputRGBProf, Format_RGB_16,
					  IntentColors, dcmsFlags);
	stdTransCMYK = colorEngine.createTransform(DocInputRGBProf, Format_RGB_16,
					  DocInputCMYKProf, Format_CMYK_16,
					  IntentColors, dcmsFlags);

	ScColorProfile inputProfRGB;
	ScColorProfile inputProfCMYK;
	if (DocPrinterProf.colorSpace() == ColorSpace_Cmyk)
	{
		inputProf = (m_docPrefsData.colorPrefs.DCMSset.SoftProofOn && m_docPrefsData.colorPrefs.DCMSset.SoftProofFullOn) ? DocInputCMYKProf : DocPrinterProf;
		inputProfRGB  = DocInputRGBProf;
		inputProfCMYK = inputProf;
	}
	else
	{
		inputProf = (m_docPrefsData.colorPrefs.DCMSset.SoftProofOn && m_docPrefsData.colorPrefs.DCMSset.SoftProofFullOn) ? DocInputRGBProf : DocPrinterProf;
		inputProfRGB  = inputProf;
		inputProfCMYK = DocInputCMYKProf;
	}
	stdProof = colorEngine.createProofingTransform(inputProfRGB, Format_RGB_16,
						DocDisplayProf, Format_RGB_16,
						DocPrinterProf,
						IntentColors,
						Intent_Relative_Colorimetric, dcmsFlags);
	stdProofGC = colorEngine.createProofingTransform(inputProfRGB, Format_RGB_16,
						DocDisplayProf, Format_RGB_16,
						DocPrinterProf, IntentColors,
						Intent_Relative_Colorimetric, dcmsFlags| Ctf_GamutCheck);
	stdProofCMYK = colorEngine.createProofingTransform(inputProfCMYK, Format_CMYK_16,
						DocDisplayProf, Format_RGB_16,
						DocPrinterProf, IntentColors,
						Intent_Relative_Colorimetric, dcmsFlags);
	stdProofCMYKGC = colorEngine.createProofingTransform(inputProfCMYK, Format_CMYK_16,
						DocDisplayProf, Format_RGB_16,
						DocPrinterProf,
						IntentColors,
						Intent_Relative_Colorimetric, dcmsFlags | Ctf_GamutCheck);
	stdProofLab = colorEngine.createProofingTransform(ScCore->defaultLabProfile, Format_Lab_Dbl,
						DocDisplayProf, Format_RGB_16,
						DocPrinterProf,
						IntentColors,
						Intent_Relative_Colorimetric, dcmsFlags);
	stdProofLabGC = colorEngine.createProofingTransform(ScCore->defaultLabProfile, Format_Lab_Dbl,
						DocDisplayProf, Format_RGB_16,
						DocPrinterProf, IntentColors,
						Intent_Relative_Colorimetric, dcmsFlags| Ctf_GamutCheck);

	if (DocInputRGBProf.colorSpace() == ColorSpace_Rgb)
			m_docPrefsData.colorPrefs.DCMSset.ComponentsInput2 = 3;
	if (DocInputRGBProf.colorSpace() == ColorSpace_Cmyk)
			m_docPrefsData.colorPrefs.DCMSset.ComponentsInput2 = 4;
	if (DocInputRGBProf.colorSpace() == ColorSpace_Cmy)
			m_docPrefsData.colorPrefs.DCMSset.ComponentsInput2 = 3;

	stdLabToRGBTrans  = colorEngine.createTransform(ScCore->defaultLabProfile, Format_Lab_Dbl, DocInputRGBProf, Format_RGB_16, Intent_Absolute_Colorimetric, dcmsFlags);
	stdLabToCMYKTrans = colorEngine.createTransform(ScCore->defaultLabProfile, Format_Lab_Dbl, DocInputCMYKProf, Format_CMYK_16, Intent_Absolute_Colorimetric, dcmsFlags);
	stdLabToScreenTrans = colorEngine.createTransform(ScCore->defaultLabProfile, Format_Lab_Dbl, DocDisplayProf, Format_RGB_16, Intent_Absolute_Colorimetric, dcmsFlags);

	bool success = (stdTransRGBMon   && stdTransCMYKMon   && stdProofImg    && stdProofImgCMYK &&
					stdTransImg      && stdTransRGB       && stdTransCMYK   && stdProof        &&
					stdProofGC       && stdProofCMYK      && stdProofCMYKGC &&
					stdLabToRGBTrans && stdLabToCMYKTrans && stdLabToScreenTrans && stdProofLab && stdProofLabGC);
	if (!success)
	{
		CloseCMSProfiles();
		m_docPrefsData.colorPrefs.DCMSset.CMSinUse = false;
		QString message = tr("An error occurred while opening ICC profiles, color management is not enabled." );
		ScMessageBox::warning(m_ScMW, CommonStrings::trWarning, message);
	}
	return true;
}


void ScribusDoc::enableCMS(bool enable)
{
	m_ScMW->setStatusBarInfoText( tr("Adjusting Colors"));
	m_ScMW->mainWindowProgressBar->reset();
	int progressBarCount = PageColors.count() + MasterItems.count() + DocItems.count() + FrameItems.count();
	m_ScMW->mainWindowProgressBar->setMaximum(progressBarCount);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	bool oldCM = m_docPrefsData.colorPrefs.DCMSset.CMSinUse;
	bool newCM = enable;
	CloseCMSProfiles();
	m_docPrefsData.colorPrefs.DCMSset.CMSinUse = newCM;
	HasCMS       = m_docPrefsData.colorPrefs.DCMSset.CMSinUse;
	BlackPoint   = m_docPrefsData.colorPrefs.DCMSset.BlackPoint;
	SoftProofing = m_docPrefsData.colorPrefs.DCMSset.SoftProofOn;
	Gamut        = m_docPrefsData.colorPrefs.DCMSset.GamutCheck;
	IntentColors = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
	IntentImages = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
	if (!m_docPrefsData.colorPrefs.DCMSset.CMSinUse)
	{
		HasCMS = false;
		if (oldCM)
		{
			m_ScMW->recalcColors(m_ScMW->mainWindowProgressBar);
			RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK, m_ScMW->mainWindowProgressBar);
		}
	}
	else if (OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles) )
	{
		HasCMS = true;
		m_docPrefsData.pdfPrefs.SComp = m_docPrefsData.colorPrefs.DCMSset.ComponentsInput2;
		m_docPrefsData.pdfPrefs.SolidProf = m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
		m_docPrefsData.pdfPrefs.ImageProf = m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
		m_docPrefsData.pdfPrefs.PrintProf = m_docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile;
		m_docPrefsData.pdfPrefs.Intent  = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
		m_docPrefsData.pdfPrefs.Intent2 = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
		m_ScMW->recalcColors(m_ScMW->mainWindowProgressBar);
		RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK, m_ScMW->mainWindowProgressBar);
	}
	else
	{
		SetDefaultCMSParams();
		HasCMS = false;
	}
	m_ScMW->mainWindowProgressBar->setValue(progressBarCount);
	qApp->restoreOverrideCursor();
	m_ScMW->setStatusBarInfoText("");
	m_ScMW->mainWindowProgressBar->reset();
}


void ScribusDoc::getNamedResources(ResourceCollection& lists) const
{
	lists.availableFonts = AllFonts;
	lists.availableColors = const_cast<ColorList*>(& PageColors);

	const QList<PageItem*> * itemlist = & MasterItems;
	while (itemlist != NULL)
	{
		for (int i=0; i < itemlist->count(); ++i)
		{
			const PageItem * currItem = const_cast<QList<PageItem*>*>(itemlist)->at(i);
			if (currItem)
				currItem->getNamedResources(lists);
		}
		if (itemlist == &MasterItems)
			itemlist = &DocItems;
		else
			itemlist = NULL;
	}
	for (QHash<int, PageItem*>::const_iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		PageItem *currItem = itf.value();
		if (currItem)
			currItem->getNamedResources(lists);
	}
	for (int i = 0; i < m_docParagraphStyles.count(); ++i)
		m_docParagraphStyles[i].getNamedResources(lists);
	for (int i = 0; i < m_docCharStyles.count(); ++i)
		m_docCharStyles[i].getNamedResources(lists);
	for (int i = 0; i < m_docTableStyles.count(); ++i)
		m_docTableStyles[i].getNamedResources(lists);
	for (int i = 0; i < m_docCellStyles.count(); ++i)
		m_docCellStyles[i].getNamedResources(lists);
	
	QHash<QString,ScPattern>::ConstIterator it;
	for (it = docPatterns.begin(); it != docPatterns.end(); ++it)
	{
		ScPattern pa = *it;
		for (int o = 0; o < pa.items.count(); o++)
		{
			pa.items.at(o)->getNamedResources(lists);
		}
	}
	QHash<QString,VGradient>::ConstIterator itg;
	for (itg = docGradients.begin(); itg != docGradients.end(); ++itg)
	{
		QList<VColorStop*> cstops = itg.value().colorStops();
		for (uint cst = 0; cst < itg.value().Stops(); ++cst)
		{
			lists.collectColor(cstops.at(cst)->name);
		}
	}
}

bool ScribusDoc::styleExists(QString styleName)
{
	for (int i = 0; i < paragraphStyles().count(); ++i)
	{
		if (paragraphStyles()[i].name() == styleName)
			return true;
	}
	return false;
}

QList<int> ScribusDoc::getSortedStyleList()
{
	QList<int> retList;
	for (int i = 0; i < m_docParagraphStyles.count(); ++i)
	{
		if (m_docParagraphStyles[i].parent().isEmpty())
		{
			if (!retList.contains(i))
				retList.append(i);
			continue;
		}

		QList<int> retList2;
		QString name = m_docParagraphStyles[i].name();
		QString par  = m_docParagraphStyles[i].parent();
		retList2.prepend(i);
		while ((!par.isEmpty()) && (par != name))
		{
			int pp = m_docParagraphStyles.find(par);
			if ((pp >= 0) && (!retList2.contains(pp)))
				retList2.prepend(pp);
			par = (pp >= 0) ? m_docParagraphStyles[pp].parent() : QString();
		}
		for (int r = 0; r < retList2.count(); ++r)
		{
			if (!retList.contains(retList2[r]))
				retList.append(retList2[r]);
		}
	}
	return retList;
}

QList<int> ScribusDoc::getSortedCharStyleList()
{
	QList<int> retList;
	for (int i = 0; i < m_docCharStyles.count(); ++i)
	{
		if (m_docCharStyles[i].parent().isEmpty())
		{
			if (!retList.contains(i))
				retList.append(i);
			continue;
		}

		QList<int> retList2;
		QString name = m_docCharStyles[i].name();
		QString par  = m_docCharStyles[i].parent();
		retList2.prepend(i);
		while ((!par.isEmpty()) && (par != name))
		{
			int pp = m_docCharStyles.find(par);
			if ((pp >= 0) && (!retList2.contains(pp)))
				retList2.prepend(pp);
			par = (pp >= 0) ? m_docCharStyles[pp].parent() : QString();
		}
		for (int r = 0; r < retList2.count(); ++r)
		{
			if (!retList.contains(retList2[r]))
				retList.append(retList2[r]);
		}
	}
	return retList;
}

QList<int> ScribusDoc::getSortedTableStyleList()
{
	QList<int> retList;
	for (int i = 0; i < m_docTableStyles.count(); ++i)
	{
		if (m_docTableStyles[i].parent().isEmpty())
		{
			if (!retList.contains(i))
				retList.append(i);
			continue;
		}

		QList<int> retList2;
		QString name = m_docTableStyles[i].name();
		QString par  = m_docTableStyles[i].parent();
		retList2.prepend(i);
		while ((!par.isEmpty()) && (par != name))
		{
			int pp = m_docTableStyles.find(par);
			if ((pp >= 0) && (!retList2.contains(pp)))
				retList2.prepend(pp);
			par = (pp >= 0) ? m_docTableStyles[pp].parent() : QString();
		}
		for (int r = 0; r < retList2.count(); ++r)
		{
			if (!retList.contains(retList2[r]))
				retList.append(retList2[r]);
		}
	}
	return retList;
}

QList<int> ScribusDoc::getSortedCellStyleList()
{
	QList<int> retList;
	for (int i = 0; i < m_docCellStyles.count(); ++i)
	{
		if (m_docCellStyles[i].parent().isEmpty())
		{
			if (!retList.contains(i))
				retList.append(i);
			continue;
		}

		QList<int> retList2;
		QString name = m_docCellStyles[i].name();
		QString par  = m_docCellStyles[i].parent();
		retList2.prepend(i);
		while ((!par.isEmpty()) && (par != name))
		{
			int pp = m_docCellStyles.find(par);
			if ((pp >= 0) && (!retList2.contains(pp)))
				retList2.prepend(pp);
			par = (pp >= 0) ? m_docCellStyles[pp].parent() : QString();
		}
		for (int r = 0; r < retList2.count(); ++r)
		{
			if (!retList.contains(retList2[r]))
				retList.append(retList2[r]);
		}
	}
	return retList;
}

void ScribusDoc::replaceStyles(const QMap<QString,QString>& newNameForOld)
{
	ResourceCollection newNames;
	newNames.mapStyles(newNameForOld);
	replaceNamedResources(newNames);
}

void ScribusDoc::replaceNamedResources(ResourceCollection& newNames)
{
	// replace names in items
	QList<PageItem*> * itemlist = & MasterItems;
	while (itemlist != NULL)
	{
		for (int i=0; i < itemlist->count(); ++i)
		{
			PageItem * currItem = itemlist->at(i);
			if (currItem)
				currItem->replaceNamedResources(newNames);
		}
		if (itemlist == &MasterItems)
			itemlist = &DocItems;
		else
			itemlist = NULL;
	}
	foreach (NotesStyle* nStyle, m_docNotesStylesList)
	{ //update styles names in notes styles
		if (nStyle == NULL)
			continue;
		if (newNames.styles().contains(nStyle->notesParStyle()))
			nStyle->setNotesParStyle(newNames.styles().value(nStyle->notesParStyle()));
		if (newNames.charStyles().contains(nStyle->marksChStyle()))
			nStyle->setMarksCharStyle(newNames.charStyles().value(nStyle->marksChStyle()));
	}
	for (QHash<int, PageItem*>::iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		PageItem *currItem = itf.value();
		if (currItem)
			currItem->replaceNamedResources(newNames);
	}
	// replace names in styles...
	for (int i=m_docParagraphStyles.count()-1; i >= 0; --i)
	{
		if (newNames.styles().contains(m_docParagraphStyles[i].name()))
			m_docParagraphStyles.remove(i);
		else
			m_docParagraphStyles[i].replaceNamedResources(newNames);
	}
	for (int i=m_docCharStyles.count()-1; i >= 0; --i)
	{
		if (newNames.charStyles().contains(m_docCharStyles[i].name()))
			m_docCharStyles.remove(i);
		else
			m_docCharStyles[i].replaceNamedResources(newNames);
	}
	for (int i = m_docTableStyles.count() - 1; i >= 0; --i)
	{
		if (newNames.tableStyles().contains(m_docTableStyles[i].name()))
			m_docTableStyles.remove(i);
		else
			m_docTableStyles[i].replaceNamedResources(newNames);
	}
	for (int i = m_docCellStyles.count() - 1; i >= 0; --i)
	{
		if (newNames.cellStyles().contains(m_docCellStyles[i].name()))
			m_docCellStyles.remove(i);
		else
			m_docCellStyles[i].replaceNamedResources(newNames);
	}

	QHash<QString,ScPattern>::Iterator it = docPatterns.begin();
	while (it != docPatterns.end())
	{
		if (newNames.patterns().contains(it.key()))
		{
			it = docPatterns.erase(it);
			continue;
		}

		ScPattern pa = *it;
		for (int o = 0; o < pa.items.count(); o++)
		{
			pa.items.at(o)->replaceNamedResources(newNames);
		}
		++it;
	}

	QHash<QString,VGradient>::Iterator itg = docGradients.begin();
	while (itg != docGradients.end())
	{
		if (newNames.gradients().contains(itg.key()))
		{
			itg = docGradients.erase(itg);
			continue;
		}
			
		QMap<QString,QString>::ConstIterator itc;
		QList<VColorStop*> cstops = itg.value().colorStops();
		for (uint cst = 0; cst < itg.value().Stops(); ++cst)
		{
			itc = newNames.colors().find(cstops.at(cst)->name);
			if (itc != newNames.colors().end())
			{
				if (*itc != CommonStrings::None)
					cstops.at(cst)->name = *itc;
			}
		}
		++itg;
	}
	
	if (newNames.colors().count() > 0 || newNames.fonts().count() > 0)
	{
		m_docCharStyles.invalidate();
		m_docParagraphStyles.invalidate();
		m_docTableStyles.invalidate();
		m_docCellStyles.invalidate();
	}
	else
	{
		if (newNames.charStyles().count() > 0)
			m_docCharStyles.invalidate();
		if (newNames.styles().count() > 0)
			m_docParagraphStyles.invalidate();
		if (newNames.tableStyles().count() > 0)
			m_docTableStyles.invalidate();
		if (newNames.cellStyles().count() > 0)
			m_docCellStyles.invalidate();
	}
	if (!isLoading() && !(newNames.colors().isEmpty() && newNames.fonts().isEmpty() && newNames.patterns().isEmpty() 
			&& newNames.styles().isEmpty() && newNames.charStyles().isEmpty() && newNames.lineStyles().isEmpty()
			&& newNames.tableStyles().isEmpty() && newNames.cellStyles().isEmpty()))
		changed();
}


void ScribusDoc::replaceCharStyles(const QMap<QString,QString>& newNameForOld)
{
	ResourceCollection newNames;
	newNames.mapCharStyles(newNameForOld);
	replaceNamedResources(newNames);
}

void ScribusDoc::replaceTableStyles(const QMap<QString, QString>& newNameForOld)
{
	ResourceCollection newNames;
	newNames.mapTableStyles(newNameForOld);
	replaceNamedResources(newNames);
}

void ScribusDoc::replaceCellStyles(const QMap<QString, QString>& newNameForOld)
{
	ResourceCollection newNames;
	newNames.mapCellStyles(newNameForOld);
	replaceNamedResources(newNames);
}

void ScribusDoc::redefineStyles(const StyleSet<ParagraphStyle>& newStyles, bool removeUnused)
{
	m_docParagraphStyles.redefine(newStyles, false);
	if (removeUnused)
	{
		QMap<QString, QString> deletion;
		QString deflt("");
		for (int i=0; i < m_docParagraphStyles.count(); ++i)
		{
			const QString& nam(m_docParagraphStyles[i].name());
			if (newStyles.find(nam) < 0)
				deletion[nam] = deflt;
		}
		if (deletion.count() > 0)
			replaceStyles(deletion);
	}
	// repair charstyle context:
	for (int i=0; i < m_docParagraphStyles.count(); ++i)
	{
		ParagraphStyle& sty(m_docParagraphStyles[i]);
		if (m_docParagraphStyles.isDefault(sty))
		{
			sty.breakImplicitCharStyleInheritance(true);
			sty.charStyle().setContext( & m_docCharStyles );
		}
		else {
			sty.breakImplicitCharStyleInheritance(false);
		}
	}
	m_docParagraphStyles.invalidate();
	if (!isLoading())
	{
		flag_Renumber = true;
		flag_NumUpdateRequest = true;
	}
}

void ScribusDoc::redefineCharStyles(const StyleSet<CharStyle>& newStyles, bool removeUnused)
{
	m_docCharStyles.redefine(newStyles, false);
	if (removeUnused)
	{
		QMap<QString, QString> deletion;
		QString deflt("");
		for (int i=0; i < m_docCharStyles.count(); ++i)
		{
			const QString& nam(m_docCharStyles[i].name());
			if (newStyles.find(nam) < 0)
			{
				deletion[nam] = deflt;
			}
		}
		if (deletion.count() > 0)
			replaceCharStyles(deletion);
	}
	m_docCharStyles.invalidate();
}

void ScribusDoc::redefineTableStyles(const StyleSet<TableStyle>& newStyles, bool removeUnused)
{
	m_docTableStyles.redefine(newStyles, false);
	if (removeUnused)
	{
		QMap<QString, QString> deletion;
		QString deflt("");
		for (int i = 0; i < m_docTableStyles.count(); ++i)
		{
			const QString& nam(m_docTableStyles[i].name());
			if (newStyles.find(nam) < 0)
			{
				deletion[nam] = deflt;
			}
		}
		if (deletion.count() > 0)
			replaceTableStyles(deletion);
	}
	m_docTableStyles.invalidate();
}

void ScribusDoc::redefineCellStyles(const StyleSet<CellStyle>& newStyles, bool removeUnused)
{
	m_docCellStyles.redefine(newStyles, false);
	if (removeUnused)
	{
		QMap<QString, QString> deletion;
		QString deflt("");
		for (int i = 0; i < m_docCellStyles.count(); ++i)
		{
			const QString& nam(m_docCellStyles[i].name());
			if (newStyles.find(nam) < 0)
			{
				deletion[nam] = deflt;
			}
		}
		if (deletion.count() > 0)
			replaceCellStyles(deletion);
	}
	m_docCellStyles.invalidate();
}

/*
 * Split out from loadStyles in editFormats.cpp so it's callable from anywhere,
 * including plugins.
 * - 2004-09-14 Craig Ringer
 */
// don't like this here. could as well be a static method for reading this stuff into temp., then always use redefineXY() - av
void ScribusDoc::loadStylesFromFile(QString fileName)
{
	StyleSet<ParagraphStyle> *wrkStyles     = &m_docParagraphStyles;
	StyleSet<CharStyle> *wrkCharStyles      = &m_docCharStyles;
	QHash<QString, multiLine> *wrkLineStyles = &MLineStyles;
	
	int oldStyles = wrkStyles->count();
	int oldCharStyles = wrkCharStyles->count();
	int oldLineStyles = wrkLineStyles->count();
	
	if (!fileName.isEmpty())
	{
		FileLoader fl(fileName);
		if (fl.testFile() == -1)
		//TODO put in nice user warning
			return;

		if (!fl.readStyles(this, *wrkStyles))
		{
			//TODO put in nice user warning
		}

		if (!fl.readCharStyles(this, *wrkCharStyles))
		{
			//TODO put in nice user warning
		}

		if (!fl.readLineStyles(wrkLineStyles))
		{
			//TODO put in nice user warning
		}
		
		if ( !isLoading() && ((wrkStyles->count() > oldStyles)
				    || (wrkCharStyles->count() > oldCharStyles)
				    || (wrkLineStyles->count() > oldLineStyles) ) )
			changed();
	}
}

void ScribusDoc::loadStylesFromFile(QString fileName, StyleSet<ParagraphStyle> *tempStyles,
                                                      StyleSet<CharStyle> *tempCharStyles,
													  QHash<QString, multiLine> *tempLineStyles)
{
	StyleSet<ParagraphStyle> *wrkStyles     = tempStyles;
	StyleSet<CharStyle> *wrkCharStyles      = tempCharStyles;
	QHash<QString, multiLine> *wrkLineStyles = tempLineStyles;
	
	if (!fileName.isEmpty())
	{
		FileLoader fl(fileName);
		if (fl.testFile() == -1)
		//TODO put in nice user warning
			return;

		if (!fl.readStyles(this, *wrkStyles))
		{
			//TODO put in nice user warning
		}

		if (!fl.readCharStyles(this, *wrkCharStyles))
		{
			//TODO put in nice user warning
		}

		if (!fl.readLineStyles(wrkLineStyles))
		{
			//TODO put in nice user warning
		}
		
		// Which are the default styles 
		// Sadly StyleSet is not updated at import
		// and it might break something to do so. 
		// We need to loop then - pm
				
		QFileInfo fi(fileName);
		QString importPrefix(tr("Imported ","Prefix of imported default style"));
		QString importSuffix(" (" +  fi.baseName() + ")");
		
		for (int j(0) ; j < wrkStyles->count() ; ++j)
		{
			if ((*wrkStyles)[j].isDefaultStyle())
			{
				ParagraphStyle& parDefault((*wrkStyles)[j]);
				parDefault.setDefaultStyle(false);
				QMap<QString, QString> namesMap;
				namesMap[parDefault.name()] = importPrefix + parDefault.name() + importSuffix;
				for (int i(0) ; i < wrkStyles->count() ; ++i)
				{
					if ((*wrkStyles)[i] != parDefault)
						namesMap[(*wrkStyles)[i].name()] = (*wrkStyles)[i].name();
				}
				wrkStyles->rename(namesMap);
			}
		}
		for (int j(0) ; j < wrkCharStyles->count() ; ++j)
		{
			if ((*wrkCharStyles)[j].isDefaultStyle())
			{
				CharStyle& charDefault((*wrkCharStyles)[j]);
				charDefault.setDefaultStyle(false);
				QMap<QString, QString> namesMap;
				namesMap[charDefault.name()] = importPrefix + charDefault.name() + importSuffix;
				for (int i(0) ; i < wrkCharStyles->count() ; ++i)
				{
					if ((*wrkCharStyles)[i] != charDefault)
						namesMap[(*wrkCharStyles)[i].name()] = (*wrkCharStyles)[i].name();
				}
				wrkCharStyles->rename(namesMap);
			}
		}
	}
}


void ScribusDoc::lockGuides(bool isLocked)
{
	if (GuideLock == isLocked)
		return;
	GuideLock = isLocked;
	if (UndoManager::undoEnabled())
	{
		QString name;
		if (isLocked)
			name = Um::LockGuides;
		else
			name = Um::UnlockGuides;
		SimpleState *ss = new SimpleState(name, "", Um::ILockGuides);
		ss->set("GUIDE_LOCK", isLocked);
		m_undoManager->action(this, ss);
	}
}

void ScribusDoc::undoRedoBegin()
{
	m_docUpdater->beginUpdate();
	m_Selection->delaySignalsOn();
}

void ScribusDoc::undoRedoDone()
{
	m_Selection->delaySignalsOff();
	m_docUpdater->endUpdate();
}

void ScribusDoc::restore(UndoState* state, bool isUndo)
{
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	if (!ss)
		return;

	bool layersUndo=false;
	if (ss->contains("GROUP"))
		restoreGrouping(ss, isUndo);
	else if (ss->contains("UNGROUP"))
		restoreGrouping(ss, !isUndo);
	else if (ss->contains("GUIDE_LOCK"))
		restoreGuideLock(ss, isUndo);
	else if (ss->contains("UP_LAYER"))
	{
		if (isUndo)
			lowerLayer(ss->getInt("ACTIVE"));
		else
			raiseLayer(ss->getInt("ACTIVE"));
		layersUndo=true;
	}
	else if (ss->contains("DOWN_LAYER"))
	{
		if (isUndo)
			raiseLayer(ss->getInt("ACTIVE"));
		else
			lowerLayer(ss->getInt("ACTIVE"));
		layersUndo=true;
	}
	else if (ss->contains("PRINT_LAYER"))
	{
		bool print = ss->getBool("PRINT");
		setLayerPrintable(ss->getInt("ACTIVE"), isUndo ? !print : print);
		layersUndo=true;
	}
	else if (ss->contains("LAYER_FLOW"))
	{
		bool flow = ss->getBool("FLOW");
		setLayerFlow(ss->getInt("ACTIVE"), isUndo ? !flow : flow);
		layersUndo=true;
	}
	else if (ss->contains("LAYER_LOCK"))
	{
		bool lock = ss->getBool("LOCK");
		setLayerLocked(ss->getInt("ACTIVE"), isUndo ? !lock : lock);
		layersUndo=true;
	}
	else if (ss->contains("LAYER_TRANSPARENCY"))
	{
		double old_trans = ss->getDouble("OLD_TRANS");
		double new_trans = ss->getDouble("NEW_TRANS");
		setLayerTransparency(ss->getInt("ACTIVE"), isUndo ? old_trans : new_trans);
		layersUndo=true;
	}
	else if (ss->contains("LAYER_BLENDMODE"))
	{
		int old_blend = ss->getInt("OLD_BLENDMODE");
		int new_blend = ss->getInt("NEW_BLENDMODE");
		setLayerBlendMode(ss->getInt("ACTIVE"), isUndo ? old_blend : new_blend);
		layersUndo=true;
	}
	else if (ss->contains("ADD_LAYER"))
	{
		if (isUndo)
			deleteLayer(ss->getInt("LAYER_NR"), false);
		else
		{
			int layerID=addLayer( ss->get("NAME"), false );
			int newLayerID=ss->getInt("LAYER_NR");
			bool renumberedOk=renumberLayer(layerID, newLayerID);
			Q_ASSERT(renumberedOk);
		}
		layersUndo=true;
	}
	else if (ss->contains("REMOVE_LAYER"))
	{
		if (isUndo)
		{
			int layerID=addLayer( ss->get("NAME"), false );
			int newLayerID=ss->getInt("LAYER_NR");
			bool renumberedOk=renumberLayer(layerID, newLayerID);
			Q_ASSERT(renumberedOk);
			layerID=newLayerID;
			//Layer is at the top now, lower it until it reaches the old level
			int level = ss->getInt("LEVEL");
			while (layerLevelFromID(layerID)!=level)
				lowerLayer(layerID);
		}
		else
			deleteLayer(ss->getInt("LAYER_NR"), ss->getBool("DELETE"));
		layersUndo=true;
	}
	else if (ss->contains("CHANGE_NAME"))
	{
		QString name = ss->get("OLD_NAME");
		if (!isUndo)
			name = ss->get("NEW_NAME");
		changeLayerName(ss->getInt("ACTIVE"), name);
		layersUndo=true;
	}
	else if (ss->contains("MASTERPAGE_ADD"))
		restoreAddMasterPage(ss, isUndo);
	else if (ss->contains("MASTERPAGE_RENAME"))
		restoreMasterPageRenaming(ss, isUndo);
	else if (ss->contains("OLD_MASTERPAGE"))
		restoreMasterPageApplying(ss, isUndo);
	else if (ss->contains("PAGE_COPY"))
		restoreCopyPage(ss, isUndo);
	else if (ss->contains("PAGE_MOVE"))
		restoreMovePage(ss, isUndo);
	else if (ss->contains("PAGE_SWAP"))
		restoreSwapPage(ss, isUndo);
	else if (ss->contains("LEVEL_DOWN"))
		restoreLevelDown(ss,isUndo);
	else if (ss->contains("LEVEL_UP"))
		restoreLevelDown(ss,!isUndo);
	else if (ss->contains("LEVEL_BOTTOM"))
		restoreLevelBottom(ss,isUndo);
	else if (ss->contains("LEVEL_TOP"))
		restoreLevelBottom(ss,!isUndo);
	else if (ss->contains("PAGE_CHANGEPROPS"))
		restoreChangePageProperties(ss,isUndo);
	else if (ss->contains("DELETE_FRAMETEXT"))
	{
		PageItem * nF = getItemFromName(ss->get("noteframeName"));
		Q_ASSERT(nF != NULL);
		nF->asNoteFrame()->restoreDeleteNoteText(ss, isUndo);
	}
	else if (ss->contains("DELETE_FRAMEPARA"))
	{
		PageItem * nF = getItemFromName(ss->get("noteframeName"));
		Q_ASSERT(nF != NULL);
		nF->asNoteFrame()->restoreDeleteNoteParagraph(ss, isUndo);
	}
	else if (ss->contains("INSERT_FRAMETEXT"))
	{
		PageItem * nF = getItemFromName(ss->get("noteframeName"));
		Q_ASSERT(nF != NULL);
		nF->asNoteFrame()->restoreInsertNoteText(ss,isUndo);
	}
	else if (ss->contains("INSERT_FRAMEPARA"))
	{
		PageItem * nF = getItemFromName(ss->get("noteframeName"));
		Q_ASSERT(nF != NULL);
		nF->asNoteFrame()->restoreInsertNoteParagraph(ss,isUndo);
	}
	else if (ss->contains("NSTYLE"))
	{
		NotesStyle* NS = NULL;
		if ((ss->get("NSTYLE") == "new" && isUndo) || (ss->get("NSTYLE") == "delete" && !isUndo))
			deleteNotesStyle(ss->get("name"));
		else if ((ss->get("NSTYLE") == "new" && !isUndo) || (ss->get("NSTYLE") == "delete" && isUndo))
		{
			NS = new NotesStyle();
			NS->setName(ss->get("name"));
			NS->setStart(ss->getInt("start"));
			NS->setEndNotes(ss->getBool("endNotes"));
			NS->setType((NumFormat) ss->getInt("numFormat"));
			NS->setRange((NumerationRange) ss->getInt("range"));
			NS->setPrefix(ss->get("prefix"));
			NS->setSuffix(ss->get("suffix"));
			NS->setAutoNotesHeight(ss->getBool("autoH"));
			NS->setAutoNotesWidth(ss->getBool("autoW"));
			NS->setAutoWeldNotesFrames(ss->getBool("autoWeld"));
			NS->setAutoRemoveEmptyNotesFrames(ss->getBool("autoRemove"));
			NS->setSuperscriptInMaster(ss->getBool("superMaster"));
			NS->setSuperscriptInNote(ss->getBool("superNote"));
			NS->setMarksCharStyle(ss->get("marksChStyle"));
			NS->setNotesParStyle(ss->get("notesParStyle"));
			m_docNotesStylesList.append(NS);
			scMW()->emitUpdateRequest(reqMarksUpdate);
		}
		else if (ss->get("NSTYLE") == "edit")
		{
			if (isUndo)
				NS = getNotesStyle(ss->get("NEWname"));
			else
				NS = getNotesStyle(ss->get("name"));
			Q_ASSERT(NS != NULL);
			//check if Notes Style change form footnotes to endnotes or range of numeration was changed
			//if use delete all notes frames with current style
			bool delNF = false;
			if (ss->getBool("NEWendNotes") != ss->getBool("endNotes")
				|| ((isUndo && (ss->getBool("endNotes") && (NumerationRange) ss->getInt("range") != (NumerationRange) ss->getInt("NEWrange")))
				|| (!isUndo && (ss->getBool("NEWendNotes") && ((NumerationRange) ss->getInt("NEWrange") != (NumerationRange) ss->getInt("range"))))))
					delNF = true;
			if (delNF)
			{
				foreach (PageItem_NoteFrame* nF, listNotesFrames(NS))
					delNoteFrame(nF, false);
				flag_updateEndNotes = NS->isEndNotes();
			}
			if (isUndo)
			{
				NS->setName(ss->get("name"));
				NS->setStart(ss->getInt("start"));
				NS->setRange((NumerationRange) ss->getInt("range"));
				NS->setEndNotes(ss->getBool("endNotes"));
				NS->setType((NumFormat) ss->getInt("numFormat"));
				NS->setPrefix(ss->get("prefix"));
				NS->setSuffix(ss->get("suffix"));
				NS->setAutoNotesHeight(ss->getBool("autoH"));
				NS->setAutoNotesWidth(ss->getBool("autoW"));
				NS->setAutoWeldNotesFrames(ss->getBool("autoWeld"));
				NS->setAutoRemoveEmptyNotesFrames(ss->getBool("autoRemove"));
				NS->setSuperscriptInMaster(ss->getBool("superMaster"));
				NS->setSuperscriptInNote(ss->getBool("superNote"));
				NS->setMarksCharStyle(ss->get("marksChStyle"));
				NS->setNotesParStyle(ss->get("notesParStyle"));
			}
			else
			{
				NS->setName(ss->get("NEWname"));
				NS->setStart(ss->getInt("NEWstart"));
				NS->setRange((NumerationRange) ss->getInt("NEWrange"));
				NS->setEndNotes(ss->getBool("NEWendNotes"));
				NS->setType((NumFormat) ss->getInt("NEWnumFormat"));
				NS->setPrefix(ss->get("NEWprefix"));
				NS->setSuffix(ss->get("NEWsuffix"));
				NS->setAutoNotesHeight(ss->getBool("NEWautoH"));
				NS->setAutoNotesWidth(ss->getBool("NEWautoW"));
				NS->setAutoWeldNotesFrames(ss->getBool("NEWautoWeld"));
				NS->setAutoRemoveEmptyNotesFrames(ss->getBool("NEWautoRemove"));
				NS->setSuperscriptInMaster(ss->getBool("NEWsuperMaster"));
				NS->setSuperscriptInNote(ss->getBool("NEWsuperNote"));
				NS->setMarksCharStyle(ss->get("NEWmarksChStyle"));
				NS->setNotesParStyle(ss->get("NEWnotesParStyle"));
			}
			setNotesChanged(true);
			if ((ss->get("marksChStyle") != ss->get("NEWmarksChStyle"))
				|| (ss->getBool("superMaster") != ss->getBool("NEWsuperMaster")))
				invalidateMasterFrames(NS);
			updateNotesNums(NS);
			updateNotesFramesSettings(NS);
			if (flag_updateEndNotes)
				updateEndnotesFrames(NS, true);
			updateNotesFramesStyles(NS);
			if (notesChanged())
			{
				flag_updateMarksLabels = true;
				changed();
				regionsChanged()->update(QRectF());
			}
		}
		if (NS != NULL)
			scMW()->nsEditor->setNotesStyle(NS);
		else
			scMW()->nsEditor->updateNSList();
	}
	else if (ss->contains("DELETE_NOTE"))
	{
		ScItemsState *is = dynamic_cast<ScItemsState*>(state);
		if (is)
		{
			NotesStyle* nStyle = getNotesStyle(is->get("nStyle"));
			PageItem* master = NULL;
			if (is->contains("noteframeName"))
				master = getItemFromName(is->get("noteframeName"));
			else
				master = (PageItem*) is->getItem("inItem");
			if (isUndo)
			{
				TextNote* note = newNote(nStyle);
				Mark* mrk = newMark();
				mrk->setType(MARKNoteMasterType);
				mrk->setNotePtr(note);
				note->setMasterMark(mrk);
				note->setSaxedText(is->get("noteTXT"));
				master->itemText.insertMark(mrk, is->getInt("at"));
				master->invalid = true;
				if (!nStyle->isAutoRemoveEmptyNotesFrames())
				{
					PageItem_NoteFrame* nF = (PageItem_NoteFrame*) is->getItem("noteframe");
					Q_ASSERT(nF != NULL);
					master->asTextFrame()->setNoteFrame(nF);
				}
				setNotesChanged(true);
				if (note->isEndNote())
					flag_updateEndNotes = true;
			}
			else
			{
				TextNote* note = master->itemText.mark(is->getInt("at"))->getNotePtr();
				if (note->isEndNote())
					flag_updateEndNotes = true;
				deleteNote(note);
			}
			master->invalidateLayout();
			master->updateLayout();
		}
	}
	else if (ss->contains("MARK"))
	{
		ScItemsState *is = dynamic_cast<ScItemsState*>(state);
		if (is)
		{
			Mark* mrk = getMark(is->get("label"), (MarkType) is->getInt("type"));
			if (mrk == NULL && is->contains("labelOLD"))
				mrk = getMark(is->get("labelOLD"), (MarkType) is->getInt("type"));
			int pos = is->getInt("at");
			bool isAutoNoteFrame = false;
			PageItem* currItem = NULL;
			if (is->contains("noteframeName"))
			{
				currItem = getItemFromName(is->get("noteframeName"));
				if (currItem != NULL)
					isAutoNoteFrame = currItem->asNoteFrame()->isAutoFrame();
			}
			else
				currItem = (PageItem*) is->getItem("inItem");
			if (isUndo)
			{
				if (is->get("MARK") == "new")
				{
					Q_ASSERT(mrk != NULL);
					if (mrk->isNoteType())
					{
						TextNote* note = mrk->getNotePtr();
						NotesStyle* nStyle = note->notesStyle();
						if (note->isEndNote())
							flag_updateEndNotes = true;
						deleteNote(note);
						updateNotesNums(nStyle);
					}
					else
						eraseMark(mrk, true, currItem, true);
				}
				else if (is->get("MARK") == "replace")
				{
					Q_ASSERT(pos >= 0);
					Q_ASSERT(currItem != NULL);
					Q_ASSERT(mrk != NULL);
					Mark* mrk = getMark(is->get("label"), (MarkType) is->getInt("type"));
					currItem->itemText.replaceMark(pos, mrk);
					if (is->contains("strtxtOLD"))
					{
						mrk->setString(is->get("strtxtOLD"));
						invalidateVariableTextFrames(mrk, false);
					}
					if (is->contains("labelOLD"))
						mrk->label = is->get("labelOLD");
				}
				else if (is->get("MARK") == "edit")
				{
					Q_ASSERT(mrk != NULL);
					if (is->contains("labelOLD"))
						mrk->label = is->get("labelOLD");
					if (is->contains("strtxtOLD"))
					{
						mrk->setString(is->get("strtxtOLD"));
						invalidateVariableTextFrames(mrk, false);
					}
					if (is->contains("dNameOLD"))
						mrk->setMark(is->get("dNameOLD"), (MarkType) is->getInt("dTypeOLD"));
					if (is->getItem("itemPtrOLD") != NULL)
						mrk->setItemPtr((PageItem*) is->getItem("itemPtrOLD"));
				}
				else if (is->get("MARK") == "insert_existing")
				{
					Q_ASSERT(pos >= 0);
					Q_ASSERT(currItem != NULL);
					Q_ASSERT(mrk != NULL);
					currItem->itemText.removeChars(pos,1);
					if (is->contains("strOLD"))
					{
						mrk->setString(is->get("strOLD"));
						invalidateVariableTextFrames(mrk, false);
					}
					if (is->contains("labelOLD"))
						mrk->label = is->get("labelOLD");
				}
				else if (is->get("MARK") == "delete")
				{
					mrk = newMark();
					mrk->label = is->get("label");
					mrk->setType((MarkType) is->getInt("type"));
					Q_ASSERT(pos >= 0);
					Q_ASSERT(currItem != NULL);
					currItem->itemText.insertMark(mrk, pos);
					if (is->contains("strtxt"))
					{
						mrk->setString(is->get("strtxt"));
						invalidateVariableTextFrames(mrk, false);
					}
					if (is->contains("dName"))
						mrk->setMark(is->get("dName"), (MarkType) is->getInt("dType"));
					if (is->getItem("itemPtr") != NULL)
						mrk->setItemPtr((PageItem*) is->getItem("itemPtrOLD"));
				}
				else if (is->get("MARK") == "eraseFromText") ////for non-unique marks
				{
					Q_ASSERT(pos >= 0);
					Q_ASSERT(mrk != NULL);
					Q_ASSERT(currItem != NULL);
					currItem->itemText.insertMark(mrk, pos);
					if (is->contains("strNew"))
					{
						mrk->setString(is->get("strNEW"));
						invalidateVariableTextFrames(mrk, false);
					}
				}
				else if (is->get("MARK") == "delNonUnique")
				{
					//used if deleting non-unique marks by MarksManager
					mrk = newMark();
					mrk->label = is->get("label");
					mrk->setType((MarkType) is->getInt("type"));
					mrk->setString(is->get("strtxt"));
					for (int i=0; i < is->insertItemPos.count(); ++i)
					{
						PageItem* item = (PageItem*) is->insertItemPos[i].first;
						item->itemText.insertMark(mrk, is->insertItemPos[i].second);
						item->invalid = true;
					}
				}
				else
				{
					Q_ASSERT(false);
					qDebug() << "MARK undo - unhandled " << is->get("MARK");
				}
			}
			else  //REDO
			{
				if (is->get("MARK") == "new")
				{
					if (currItem == NULL)
					{
						qDebug() << "Wrong inItem in undo step for mark";
						return;
					}
					mrk = newMark();
					mrk->label = is->get("label");
					mrk->setType((MarkType) is->getInt("type"));
					Q_ASSERT(currItem != NULL);
					Q_ASSERT(pos >= 0);
					currItem->itemText.insertMark(mrk, pos);
					if (is->contains("strtxt"))
						mrk->setString(is->get("strtxt"));
					if (is->contains("dName"))
						mrk->setMark(is->get("dName"), (MarkType) is->getInt("dType"));
					if (is->getItem("itemPtr") != NULL)
						mrk->setItemPtr((PageItem*) is->getItem("itemPtrOLD"));
					if (mrk->isType(MARKNoteMasterType))
					{
						NotesStyle* nStyle = getNotesStyle(is->get("nStyle"));;
						TextNote* note = newNote(nStyle);
						mrk->setNotePtr(note);
						note->setMasterMark(mrk);
						if (nStyle->isEndNotes())
							flag_updateEndNotes = true;
						updateNotesNums(nStyle);
					}
				}
				else if (is->get("MARK") == "replace")
				{
					Q_ASSERT(currItem != NULL);
					Q_ASSERT(pos >= 0);
					currItem->itemText.replaceMark(pos, getMark(is->get("label"), (MarkType) is->getInt("type")));
					if (is->contains("strtxtNEW"))
					{
						mrk->setString(is->get("strtxtNEW"));
						invalidateVariableTextFrames(mrk, false);
					}
					if (is->contains("labelNEW"))
						mrk->label = is->get("labelNEW");
				}
				else if (is->get("MARK") == "edit")
				{
					if (is->contains("labelNEW"))
						mrk->label = is->get("labelNEW");
					if (is->contains("strtxtNEW"))
					{
						mrk->setString(is->get("strtxtNEW"));
						invalidateVariableTextFrames(mrk, false);
					}
					if (is->contains("dNameNEW"))
						mrk->setMark(is->get("dNameNEW"), (MarkType) is->getInt("dTypeNEW"));
					if (is->getItem("itemPtrNEW") != NULL)
						mrk->setItemPtr((PageItem*) is->getItem("itemPtrNEW"));
				}
				else if (is->get("MARK") == "insert_existing")
				{
					Q_ASSERT(currItem != NULL);
					Q_ASSERT(pos >= 0);
					currItem->itemText.insertMark(mrk, pos);
					if (is->contains("strNew"))
					{
						mrk->setString(is->get("strNEW"));
						invalidateVariableTextFrames(mrk, false);
					}
					if (is->contains("labelNEW"))
						mrk->label = is->get("labelNEW");
				}
				else if (is->get("MARK") == "eraseFromText") //for non-unique marks
				{
					Q_ASSERT(currItem != NULL);
					Q_ASSERT(pos >= 0);
					currItem->itemText.removeChars(pos,1);
				}
				else if (is->get("MARK") == "delete")
				{
					if (!mrk->isUnique())
					{
						Q_ASSERT(currItem != NULL);
						Q_ASSERT(pos >= 0);
						currItem->itemText.removeChars(pos,1);
					}
					else
					{
						if (mrk->isType(MARKNoteMasterType))
						{
							TextNote* note = mrk->getNotePtr();
							NotesStyle* nStyle = note->notesStyle();
							if (note->isEndNote())
								flag_updateEndNotes = true;
							deleteNote(note);
							updateNotesNums(nStyle);
						}
						else
							eraseMark(mrk, true);
					}
				}
				else if (is->get("MARK") == "delNonUnique")
				{
					//used if deleting non-unique marks by MarksManager
					eraseMark(mrk, true, NULL, true);
				}
				else
				{
					qDebug() << "MARK redo - unhandled " << is->get("MARK");
				}
			}
			scMW()->emitUpdateRequest(reqMarksUpdate);
			if (currItem != NULL && !isAutoNoteFrame)
			{
				currItem->invalidateLayout();
				currItem->updateLayout();
			}
		}
	}
	if (layersUndo)
	{
		if (ScCore->usingGUI())
		{
			if (ss->contains("ACTIVE"))
				scMW()->layerPalette->setActiveLayer(layerLevelFromID(ss->getInt("ACTIVE")),0);
			m_ScMW->changeLayer(ss->getInt("ACTIVE"));
			m_ScMW->layerPalette->rebuildList();
			if (m_ScMW->outlinePalette->isVisible())
				m_ScMW->outlinePalette->BuildTree();
		}
	}
}

void ScribusDoc::restoreLevelDown(SimpleState* ss, bool isUndo)
{
	ScItemState<QList<QPointer<PageItem> > > *is = dynamic_cast<ScItemState<QList<QPointer<PageItem> > > *>(ss);
	if (!is)
		return;
	QList<QPointer<PageItem> > listItem = is->getItem();
	m_Selection->clear();
	for (int i = 0; i<listItem.size();i++)
		m_Selection->addItem(listItem.at(i));
	if (isUndo)
		itemSelection_RaiseItem();
	else
		itemSelection_LowerItem();
}

void ScribusDoc::restoreLevelBottom(SimpleState* ss, bool isUndo)
{
	ScItemState<QList<QPointer<PageItem> > > *is = dynamic_cast<ScItemState<QList<QPointer<PageItem> > > *>(ss);
	if (!is)
		return;
	QList<QPointer<PageItem> > listItem = is->getItem();
	m_Selection->clear();
	for (int i = 0; i<listItem.size();i++)
		m_Selection->addItem(listItem.at(i));
	if (isUndo)
		bringItemSelectionToFront();
	else
		sendItemSelectionToBack();
}

void ScribusDoc::restoreGuideLock(SimpleState* ss, bool isUndo)
{
	if (isUndo)
		GuideLock = !ss->getBool("GUIDE_LOCK");
	else
		GuideLock = ss->getBool("GUIDE_LOCK");
}

void ScribusDoc::restoreAddMasterPage(SimpleState* ss, bool isUndo)
{
	QString pageName = ss->get("MASTERPAGE_NAME");
	int pageNr = ss->getInt("MASTERPAGE_NBR");

	bool oldMPMode  = masterPageMode();
	ScPage* oldPage = currentPage();

	setMasterPageMode(true);
	if (isUndo)
	{
		DummyUndoObject *duo = new DummyUndoObject();
		uint did = static_cast<uint>(duo->getUId());
		m_undoManager->replaceObject(Pages->at(MasterNames[pageName])->getUId(), duo);
		ss->set("DUMMY_ID", did);

		scMW()->deletePage2(MasterNames[pageName]);
		rebuildMasterNames();
	} 
	else 
	{
		ScPage* Mpage = addMasterPage(pageNr, pageName);
		setCurrentPage(Mpage);
		UndoObject *tmp = m_undoManager->replaceObject(
					ss->getUInt("DUMMY_ID"), Pages->at(MasterNames[pageName]));
		delete tmp;
	}
	setMasterPageMode(oldMPMode);
	if (!oldMPMode)
		setCurrentPage(oldPage);
	scMW()->pagePalette->updateMasterPageList();
	m_View->reformPages();
}

void ScribusDoc::restoreChangePageProperties(SimpleState* state, bool isUndo)
{
	bool oldMPMode  = masterPageMode();
	ScPage* oldPage = currentPage();

	setMasterPageMode(state->getBool("MASTER_PAGE_MODE"));
	setCurrentPage(Pages->at(state->getInt("PAGE_NUM")));
	if (isUndo)
	{
//		qDebug()<<"undo setting orientation to"<<state->getInt("OLD_PAGE_ORIENTATION");
		changePageProperties(state->getDouble("OLD_PAGE_INITIALTOP"), state->getDouble("OLD_PAGE_INITIALBOTTOM"),
				state->getDouble("OLD_PAGE_INITIALLEFT"), state->getDouble("OLD_PAGE_INITIALRIGHT"),
				state->getDouble("OLD_PAGE_INITIALHEIGHT"), state->getDouble("OLD_PAGE_INITIALWIDTH"),
				state->getDouble("OLD_PAGE_HEIGHT"), state->getDouble("OLD_PAGE_WIDTH"), state->getInt("OLD_PAGE_ORIENTATION"),
				state->get("OLD_PAGE_SIZE"), state->getInt("OLD_PAGE_MARGINPRESET"), state->getBool("OLD_PAGE_MOVEOBJECTS"), state->getInt("PAGE_NUM"), state->getInt("OLD_PAGE_TYPE"));
	}
	else
	{
//		qDebug()<<"redo setting orientation to"<<state->getInt("NEW_PAGE_ORIENTATION");
		changePageProperties(state->getDouble("NEW_PAGE_INITIALTOP"), state->getDouble("NEW_PAGE_INITIALBOTTOM"),
				state->getDouble("NEW_PAGE_INITIALLEFT"), state->getDouble("NEW_PAGE_INITIALRIGHT"),
				state->getDouble("NEW_PAGE_INITIALHEIGHT"), state->getDouble("NEW_PAGE_INITIALWIDTH"),
				state->getDouble("NEW_PAGE_HEIGHT"), state->getDouble("NEW_PAGE_WIDTH"), state->getInt("NEW_PAGE_ORIENTATION"),
				state->get("NEW_PAGE_SIZE"), state->getInt("NEW_PAGE_MARGINPRESET"), state->getBool("OLD_PAGE_MOVEOBJECTS"), state->getInt("PAGE_NUM"), state->getInt("NEW_PAGE_TYPE"));
	}
	setMasterPageMode(oldMPMode);
	setCurrentPage(oldPage);
}

void ScribusDoc::restoreGrouping(SimpleState* ss, bool isUndo)
{
	ScItemState<QList<QPointer<PageItem> > >* is = dynamic_cast<ScItemState<QList<QPointer<PageItem> > >*>(ss);
	if (!is)
		qFatal("ScribusDoc::restoreGrouping: dynamic cast failed");

	QList<QPointer<PageItem> > select = is->getItem();
	m_Selection->delaySignalsOn();
	for (int i = 0; i < select.count(); ++i)
		m_Selection->removeItem(select.at(i));
	Selection tempSelect(this, false);
	double x, y, w, h;
	if (isUndo)
	{
		tempSelect.addItem(select.last());
		tempSelect.setGroupRect();
		tempSelect.getGroupRect(&x, &y, &w, &h);
		itemSelection_UnGroupObjects(&tempSelect);
	}
	else
	{
		for (int i = 0; i < select.size()-1; ++i)
			tempSelect.addItem(select.at(i));
		tempSelect.setGroupRect();
		tempSelect.getGroupRect(&x, &y, &w, &h);
		select.removeLast();
		select.append(itemSelection_GroupObjects(false, false,&tempSelect));
		is->setItem(select);
	}
	QRectF rect(x, y , w, h);
	regionsChanged()->update(rect.adjusted(-10, -10, 20, 20));
	m_Selection->delaySignalsOff();
}

void ScribusDoc::setName(const QString& name)
{
	DocName = name;
}


void ScribusDoc::setModified(const bool isModified)
{
	if (m_modified != isModified)
	{
		m_modified = isModified;
		update();
	}
}


bool ScribusDoc::isModified() const
{
	return m_modified;
}


/** sets page properties */
void ScribusDoc::setPage(double w, double h, double t, double l, double r, double b, double sp, double ab, bool atf, int fp)
{
	m_docPrefsData.docSetupPrefs.pageWidth = w;
	m_docPrefsData.docSetupPrefs.pageHeight = h;
	m_docPrefsData.docSetupPrefs.margins.set(t, l, b ,r);
	PageSp = sp;
	PageSpa = ab;
	m_docPrefsData.docSetupPrefs.pagePositioning = fp;
	m_automaticTextFrames = atf;
}

void ScribusDoc::resetPage(int fp, MarginStruct* newMargins)
{
	if (newMargins!=0)
		m_docPrefsData.docSetupPrefs.margins = *newMargins;
	m_docPrefsData.docSetupPrefs.pagePositioning = fp;
}


bool ScribusDoc::AddFont(QString name, int fsize)
{
	bool ret = false;
	if (UsedFonts.contains(name))
		return true;

	if (! AllFonts->contains(name) || name == "" )
		return false;

	UsedFonts[name] = fsize;
	(*AllFonts)[name].increaseUsage();
	ret = true;
	return ret;
}


QStringList ScribusDoc::getItemAttributeNames()
{
	QStringList nameList;

	for (ObjAttrVector::Iterator it = itemAttributes().begin(); it!= itemAttributes().end(); ++it)
		nameList.append((*it).name);
	return nameList;
}


void ScribusDoc::addSymbols()
{
	symReturn.resize(0);
	symReturn.addQuadPoint(1.98438, 9.14062, 1.98438, 9.14062, 1.98438, 4.03125, 1.98438, 4.03125);
	symReturn.addQuadPoint(1.98438, 4.03125, 1.98438, 4.03125, 0.546875, 3.45312, 1.09375, 4);
	symReturn.addQuadPoint(0.546875, 3.45312, 0.546875, 3.45312, 0, 2.0625, 0, 2.92188);
	symReturn.addQuadPoint(0, 2.0625, 0, 2.0625, 0.65625, 0.5, 0, 1.04688);
	symReturn.addQuadPoint(0.65625, 0.5, 0.65625, 0.5, 2.3125, 0, 1.28125, 0);
	symReturn.addQuadPoint(2.3125, 0, 2.3125, 0, 5.40625, 0, 5.40625, 0);
	symReturn.addQuadPoint(5.40625, 0, 5.40625, 0, 5.40625, 0.84375, 5.40625, 0.84375);
	symReturn.addQuadPoint(5.40625, 0.84375, 5.40625, 0.84375, 4.70312, 0.84375, 4.70312, 0.84375);
	symReturn.addQuadPoint(4.70312, 0.84375, 4.70312, 0.84375, 4.70312, 9.14062, 4.70312, 9.14062);
	symReturn.addQuadPoint(4.70312, 9.14062, 4.70312, 9.14062, 3.875, 9.14062, 3.875, 9.14062);
	symReturn.addQuadPoint(3.875, 9.14062, 3.875, 9.14062, 3.875, 0.84375, 3.875, 0.84375);
	symReturn.addQuadPoint(3.875, 0.84375, 3.875, 0.84375, 2.78125, 0.84375, 2.78125, 0.84375);
	symReturn.addQuadPoint(2.78125, 0.84375, 2.78125, 0.84375, 2.78125, 9.14062, 2.78125, 9.14062);
	symReturn.addQuadPoint(2.78125, 9.14062, 2.78125, 9.14062, 1.98438, 9.14062, 1.98438, 9.14062);
	symNewLine.resize(0);
	symNewLine.addQuadPoint(6.51562, 2.625, 6.51562, 2.625, 0.90625, 2.64062, 0.90625, 2.64062);
	symNewLine.addQuadPoint(0.90625, 2.64062, 0.90625, 2.64062, 1.4375, 1.92188, 1.26562, 2.1875);
	symNewLine.addQuadPoint(1.4375, 1.92188, 1.4375, 1.92188, 1.76562, 1.14062, 1.75, 1.42188);
	symNewLine.addQuadPoint(1.76562, 1.14062, 1.76562, 1.14062, 1.60938, 1.03125, 1.60938, 1.03125);
	symNewLine.addQuadPoint(1.60938, 1.03125, 1.60938, 1.03125, 0.90625, 1.92188, 0.90625, 1.92188);
	symNewLine.addQuadPoint(0.90625, 1.92188, 0.90625, 1.92188, 0, 2.90625, 0.578125, 2.23438);
	symNewLine.addQuadPoint(0, 2.90625, 0, 2.90625, 0.75, 3.875, 0.75, 3.875);
	symNewLine.addQuadPoint(0.75, 3.875, 0.75, 3.875, 1.57812, 4.78125, 1.1875, 4.40625);
	symNewLine.addQuadPoint(1.57812, 4.78125, 1.57812, 4.78125, 1.65625, 4.79688, 1.65625, 4.79688);
	symNewLine.addQuadPoint(1.65625, 4.79688, 1.65625, 4.79688, 1.76562, 4.65625, 1.76562, 4.65625);
	symNewLine.addQuadPoint(1.76562, 4.65625, 1.76562, 4.65625, 0.90625, 3.17188, 1.73438, 4.34375);
	symNewLine.addQuadPoint(0.90625, 3.17188, 0.90625, 3.17188, 0.96875, 3.125, 0.96875, 3.125);
	symNewLine.addQuadPoint(0.96875, 3.125, 0.96875, 3.125, 6.75, 3.125, 6.75, 3.125);
	symNewLine.addQuadPoint(6.75, 3.125, 6.75, 3.125, 6.51562, 2.625, 6.51562, 2.625);
	symNewLine.addQuadPoint(6.51562, 2.625, 6.51562, 2.625, 6.51562, 2.625, 6.51562, 2.625);
	symNewLine.setMarker();
	symNewLine.addQuadPoint(6.875, 0, 6.875, 0, 6.51562, 0, 6.51562, 0);
	symNewLine.addQuadPoint(6.51562, 0, 6.51562, 0, 6.51562, 2.84375, 6.51562, 2.84375);
	symNewLine.addQuadPoint(6.51562, 2.84375, 6.51562, 2.84375, 6.75, 3.125, 6.51562, 3.125);
	symNewLine.addQuadPoint(6.75, 3.125, 6.75, 3.125, 6.85938, 3.0625, 6.85938, 3.0625);
	symNewLine.addQuadPoint(6.85938, 3.0625, 6.85938, 3.0625, 6.875, 0, 6.875, 0);
	symTab.resize(0);
	symTab.addQuadPoint(4.82812, 3.96875, 4.82812, 3.96875, 4.5625, 3.73438, 4.5625, 3.96875);
	symTab.addQuadPoint(4.5625, 3.73438, 4.5625, 3.73438, 5.07812, 3.10938, 4.5625, 3.57812);
	symTab.addQuadPoint(5.07812, 3.10938, 5.07812, 3.10938, 0, 3.10938, 0, 3.10938);
	symTab.addQuadPoint(0, 3.10938, 0, 3.10938, 0, 2.625, 0, 2.625);
	symTab.addQuadPoint(0, 2.625, 0, 2.625, 5.53125, 2.625, 5.53125, 2.625);
	symTab.addQuadPoint(5.53125, 2.625, 5.53125, 2.625, 6.3125, 1.8125, 6.3125, 1.8125);
	symTab.addQuadPoint(6.3125, 1.8125, 6.3125, 1.8125, 5.64062, 1.29688, 5.64062, 1.29688);
	symTab.addQuadPoint(5.64062, 1.29688, 5.64062, 1.29688, 0, 1.29688, 0, 1.29688);
	symTab.addQuadPoint(0, 1.29688, 0, 1.29688, 0, 0.8125, 0, 0.8125);
	symTab.addQuadPoint(0, 0.8125, 0, 0.8125, 5.01562, 0.8125, 5.01562, 0.8125);
	symTab.addQuadPoint(5.01562, 0.8125, 5.01562, 0.8125, 4.45312, 0.265625, 4.45312, 0.453125);
	symTab.addQuadPoint(4.45312, 0.265625, 4.45312, 0.265625, 4.6875, 0, 4.45312, 0);
	symTab.addQuadPoint(4.6875, 0, 4.6875, 0, 5.90625, 0.828125, 4.875, 0);
	symTab.addQuadPoint(5.90625, 0.828125, 5.90625, 0.828125, 6.9375, 1.79688, 6.9375, 1.64062);
	symTab.addQuadPoint(6.9375, 1.79688, 6.9375, 1.79688, 5.95312, 2.96875, 6.9375, 1.95312);
	symTab.addQuadPoint(5.95312, 2.96875, 5.95312, 2.96875, 4.82812, 3.96875, 4.98438, 3.96875);
	symNonBreak.resize(0);
	symNonBreak.addQuadPoint(1.32812, 2.59375, 1.32812, 2.59375, 0.390625, 2.21875, 0.796875, 2.59375);
	symNonBreak.addQuadPoint(0.390625, 2.21875, 0.390625, 2.21875, 0, 1.3125, 0, 1.84375);
	symNonBreak.addQuadPoint(0, 1.3125, 0, 1.3125, 0.390625, 0.390625, 0, 0.765625);
	symNonBreak.addQuadPoint(0.390625, 0.390625, 0.390625, 0.390625, 1.32812, 0, 0.796875, 0);
	symNonBreak.addQuadPoint(1.32812, 0, 1.32812, 0, 2.23438, 0.390625, 1.85938, 0);
	symNonBreak.addQuadPoint(2.23438, 0.390625, 2.23438, 0.390625, 2.60938, 1.29688, 2.60938, 0.765625);
	symNonBreak.addQuadPoint(2.60938, 1.29688, 2.60938, 1.29688, 2.23438, 2.21875, 2.60938, 1.84375);
	symNonBreak.addQuadPoint(2.23438, 2.21875, 2.23438, 2.21875, 1.32812, 2.59375, 1.875, 2.59375);
	symNewCol.resize(0);
	symNewCol.addQuadPoint(1.73438, 0, 1.73438, 0, 2.67188, 0.109375, 2.03125, 0);
	symNewCol.addQuadPoint(2.67188, 0.109375, 2.67188, 0.109375, 3.59375, 0.203125, 3.26562, 0.21875);
	symNewCol.addQuadPoint(3.59375, 0.203125, 3.59375, 0.203125, 3.79688, 0.1875, 3.64062, 0.203125);
	symNewCol.addQuadPoint(3.79688, 0.1875, 3.79688, 0.1875, 4, 0.171875, 3.92188, 0.171875);
	symNewCol.addQuadPoint(4, 0.171875, 4, 0.171875, 4.20312, 0.1875, 4.20312, 0.1875);
	symNewCol.addQuadPoint(4.20312, 0.1875, 4.20312, 0.1875, 4.3125, 1.39062, 4.20312, 0.5625);
	symNewCol.addQuadPoint(4.3125, 1.39062, 4.3125, 1.39062, 4.42188, 2.64062, 4.42188, 2.21875);
	symNewCol.addQuadPoint(4.42188, 2.64062, 4.42188, 2.64062, 4.28125, 2.73438, 4.28125, 2.73438);
	symNewCol.addQuadPoint(4.28125, 2.73438, 4.28125, 2.73438, 3.75, 1.03125, 4.01562, 2.64062);
	symNewCol.addQuadPoint(3.75, 1.03125, 3.75, 1.03125, 3.67188, 1.03125, 3.67188, 1.03125);
	symNewCol.addQuadPoint(3.67188, 1.03125, 3.67188, 1.03125, 0.28125, 6.20312, 0.28125, 6.20312);
	symNewCol.addQuadPoint(0.28125, 6.20312, 0.28125, 6.20312, 0, 5.95312, 0.03125, 6.17188);
	symNewCol.addQuadPoint(0, 5.95312, 0, 5.95312, 3.35938, 0.71875, 3.35938, 0.71875);
	symNewCol.addQuadPoint(3.35938, 0.71875, 3.35938, 0.71875, 3.375, 0.640625, 3.375, 0.640625);
	symNewCol.addQuadPoint(3.375, 0.640625, 3.375, 0.640625, 2.4375, 0.484375, 2.79688, 0.5625);
	symNewCol.addQuadPoint(2.4375, 0.484375, 2.4375, 0.484375, 1.67188, 0.140625, 1.71875, 0.328125);
	symNewCol.addQuadPoint(1.67188, 0.140625, 1.67188, 0.140625, 1.73438, 0, 1.73438, 0);
	symNewFrame.resize(0);
	symNewFrame.addQuadPoint(1.75, 6.20312, 1.75, 6.20312, 2.67188, 6.09375, 2.0625, 6.20312);
	symNewFrame.addQuadPoint(2.67188, 6.09375, 2.67188, 6.09375, 3.60938, 5.98438, 3.28125, 5.98438);
	symNewFrame.addQuadPoint(3.60938, 5.98438, 3.60938, 5.98438, 3.84375, 6.01562, 3.6875, 5.98438);
	symNewFrame.addQuadPoint(3.84375, 6.01562, 3.84375, 6.01562, 4.07812, 6.03125, 4, 6.03125);
	symNewFrame.addQuadPoint(4.07812, 6.03125, 4.07812, 6.03125, 4.20312, 6.01562, 4.20312, 6.01562);
	symNewFrame.addQuadPoint(4.20312, 6.01562, 4.20312, 6.01562, 4.32812, 4.79688, 4.21875, 5.625);
	symNewFrame.addQuadPoint( 4.32812, 4.79688, 4.32812, 4.79688, 4.42188, 3.5625, 4.42188, 3.98438);
	symNewFrame.addQuadPoint(4.42188, 3.5625, 4.42188, 3.5625, 4.29688, 3.45312, 4.29688, 3.45312);
	symNewFrame.addQuadPoint(4.29688, 3.45312, 4.29688, 3.45312, 3.75, 5.17188, 4.03125, 3.54688);
	symNewFrame.addQuadPoint(3.75, 5.17188, 3.75, 5.17188, 3.67188, 5.17188, 3.67188, 5.17188);
	symNewFrame.addQuadPoint(3.67188, 5.17188, 3.67188, 5.17188, 0.28125, 0, 0.28125, 0);
	symNewFrame.addQuadPoint(0.28125, 0, 0.28125, 0, 0, 0.25, 0.03125, 0.015625);
	symNewFrame.addQuadPoint(0, 0.25, 0, 0.25, 3.375, 5.46875, 3.375, 5.46875);
	symNewFrame.addQuadPoint(3.375, 5.46875, 3.375, 5.46875, 3.39062, 5.54688, 3.39062, 5.54688);
	symNewFrame.addQuadPoint(3.39062, 5.54688, 3.39062, 5.54688, 2.4375, 5.70312, 2.8125, 5.625);
	symNewFrame.addQuadPoint(2.4375, 5.70312, 2.4375, 5.70312, 1.67188, 6.0625, 1.71875, 5.875);
	symNewFrame.addQuadPoint(1.67188, 6.0625, 1.67188, 6.0625, 1.75, 6.20312, 1.75, 6.20312);
}


ScPage* ScribusDoc::addPage(const int pageIndex, const QString& masterPageName, const bool addAutoFrame)
{
	assert(masterPageMode()==false);
	ScPage* addedPage = new ScPage(m_docPrefsData.displayPrefs.scratch.left(), DocPages.count()*(m_docPrefsData.docSetupPrefs.pageHeight+m_docPrefsData.displayPrefs.scratch.bottom()+m_docPrefsData.displayPrefs.scratch.top())+m_docPrefsData.displayPrefs.scratch.top(), m_docPrefsData.docSetupPrefs.pageWidth, m_docPrefsData.docSetupPrefs.pageHeight);
	assert(addedPage!=NULL);
	addedPage->setDocument(this);
	addedPage->Margins.setTop(m_docPrefsData.docSetupPrefs.margins.top());
	addedPage->Margins.setBottom(m_docPrefsData.docSetupPrefs.margins.bottom());
	addedPage->initialMargins = m_docPrefsData.docSetupPrefs.margins;
	addedPage->setPageNr(pageIndex);
	addedPage->m_pageSize = m_docPrefsData.docSetupPrefs.pageSize;
	addedPage->setOrientation(m_docPrefsData.docSetupPrefs.pageOrientation);
	addedPage->marginPreset = m_docPrefsData.docSetupPrefs.marginPreset;
	DocPages.insert(pageIndex, addedPage);
	assert(DocPages.at(pageIndex)!=NULL);
	setCurrentPage(addedPage);
	if (!masterPageName.isEmpty())
		applyMasterPage(masterPageName, pageIndex);
	setLocationBasedPageLRMargins(pageIndex);
	if (addAutoFrame && m_automaticTextFrames)
		addAutomaticTextFrame(pageIndex);
	return addedPage;
}


ScPage* ScribusDoc::addMasterPage(const int pageNumber, const QString& pageName)
{
	ScPage* addedPage = new ScPage(m_docPrefsData.displayPrefs.scratch.left(), m_docPrefsData.displayPrefs.scratch.top(), m_docPrefsData.docSetupPrefs.pageWidth, m_docPrefsData.docSetupPrefs.pageHeight);
	assert(addedPage!=NULL);
	addedPage->setDocument(this);
	addedPage->Margins = m_docPrefsData.docSetupPrefs.margins;
	addedPage->initialMargins = m_docPrefsData.docSetupPrefs.margins;
	addedPage->m_pageSize = m_docPrefsData.docSetupPrefs.pageSize;
	addedPage->setOrientation(m_docPrefsData.docSetupPrefs.pageOrientation);
	addedPage->marginPreset = m_docPrefsData.docSetupPrefs.marginPreset;
	addedPage->MPageNam = "";
	int pgN = pageNumber;
	if (pageNumber > MasterPages.count())
		pgN = MasterPages.count();
	addedPage->setPageName(pageName);
	addedPage->setPageNr(pgN);
	MasterNames.insert(pageName, pgN);
	MasterPages.insert(pgN, addedPage);
	assert(MasterPages.at(pgN)!=NULL);
	if (!isLoading())
		changed();
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::NewMasterPage, "", Um::IDocument);
		ss->set("MASTERPAGE_ADD");
		ss->set("MASTERPAGE_NAME", pageName);
		ss->set("MASTERPAGE_NBR", pgN);
		m_undoManager->action(this, ss);
	}
	return addedPage;
}


bool ScribusDoc::renameMasterPage(const QString& oldPageName, const QString& newPageName)
{
	Q_ASSERT(oldPageName!=CommonStrings::masterPageNormal && oldPageName!=CommonStrings::trMasterPageNormal);
	if (!MasterNames.contains(oldPageName) || MasterNames.contains(newPageName))
		return false;

	//Rename our master page lists
	int number = MasterNames[oldPageName];
	MasterNames.insert(newPageName, number);
	MasterNames.remove(oldPageName);
	Q_ASSERT(MasterPages.at(number)->pageName()==oldPageName);
	MasterPages.at(number)->setPageName(newPageName);
	//Update any pages that were linking to our old name
	ScPage* docPage=NULL;
	for (int i=0; i < DocPages.count(); ++i )
	{
		docPage=DocPages[i];
		if (docPage->MPageNam == oldPageName)
			docPage->MPageNam = newPageName;
	}
	//Update any items that were linking to our old name
	int masterItemsCount=MasterItems.count();
	for (int i = 0; i < masterItemsCount; ++i)
	{
		if (MasterItems.at(i)->OnMasterPage == oldPageName)
			MasterItems.at(i)->setMasterPageName(newPageName);
	}
	changed();
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::RenameMasterPage, "", Um::IDocument);
		ss->set("MASTERPAGE_RENAME");
		ss->set("OLD_MASTERPAGE", oldPageName);
		ss->set("NEW_MASTERPAGE", newPageName);
		m_undoManager->action(this, ss);
	}
	return true;
}


void ScribusDoc::deleteMasterPage(const int pageNumber)
{
	assert(masterPageMode());
	assert( Pages->count() > 1 && Pages->count() > pageNumber );
	setCurrentPage(Pages->at(0));
	ScPage* page = Pages->takeAt(pageNumber);
	delete page;
	// #10658 : renumber masterpages and masterpage objects
	// in order to avoid crash after masterpage deletion
	for (int i = 0; i < MasterPages.count(); ++i)
		MasterPages.at(i)->setPageNr(i);
	for (int i = 0; i < MasterItems.count(); ++i)
	{
		if (MasterItems.at(i)->OwnPage > pageNumber)
			MasterItems.at(i)->OwnPage--;
	}
	changed();
}


void ScribusDoc::rebuildMasterNames(void)
{
	MasterNames.clear();
	for (int i = 0; i < MasterPages.count(); ++i)
		MasterNames[MasterPages.at(i)->pageName()] = i;
}


void ScribusDoc::replaceMasterPage(const QString& oldMasterPage)
{
	uint pageIndex = 0;
	QMap<QString,int>::Iterator it = MasterNames.begin();
	QListIterator<ScPage *> dpIt(DocPages);
	ScPage* docPage=NULL;
	while(dpIt.hasNext())
	{
		docPage=dpIt.next();
		if (docPage->MPageNam == oldMasterPage)
		{
			PageLocation pageLoc = locationOfPage(pageIndex);
			if (pageLoc == LeftPage)
			{
				if (MasterNames.contains( CommonStrings::trMasterPageNormalLeft))
					docPage->MPageNam = CommonStrings::trMasterPageNormalLeft;
				else if (MasterNames.contains( CommonStrings::trMasterPageNormal))
					docPage->MPageNam = CommonStrings::trMasterPageNormal;
				else
					docPage->MPageNam = it.key();
			}
			else if (pageLoc == RightPage)
			{
				if (MasterNames.contains( CommonStrings::trMasterPageNormalRight))
					docPage->MPageNam = CommonStrings::trMasterPageNormalRight;
				else if (MasterNames.contains( CommonStrings::trMasterPageNormal))
					docPage->MPageNam = CommonStrings::trMasterPageNormal;
				else
					docPage->MPageNam = it.key();
			}
			else
			{
				if (MasterNames.contains( CommonStrings::trMasterPageNormalMiddle))
					docPage->MPageNam = CommonStrings::trMasterPageNormalMiddle;
				else if (MasterNames.contains( CommonStrings::trMasterPageNormal))
					docPage->MPageNam = CommonStrings::trMasterPageNormal;
				else
					docPage->MPageNam = it.key();
			}
		}
		pageIndex++;
	}
}


void ScribusDoc::deletePage(const int pageNumber)
{
	assert( Pages->count() > 1 && Pages->count() > pageNumber );
	//#5561: If we are going to delete the first page, do not set the current page to it
	setCurrentPage(Pages->at(pageNumber!=0?0:1));
	ScPage* page = Pages->takeAt(pageNumber);
	delete page;
	reformPages();
	changed();
}

void ScribusDoc::swapPage(const int a, const int b)
{
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::SwapPage, "", Um::IDocument);
		ss->set("PAGE_SWAP");
		ss->set("PAGE_SWAP_FROM", a);
		ss->set("PAGE_SWAP_TO", b);
		m_undoManager->action(this, ss);
	}
	Pages->swap(a,b);
	reformPages();
	changed();
}

void ScribusDoc::restoreSwapPage(SimpleState* ss, bool isUndo)
{
	int a = ss->getInt("PAGE_SWAP_FROM");
	int b = ss->getInt("PAGE_SWAP_TO");

	if (isUndo)
		swapPage(b,a);
	else
		swapPage(a,b);
}

void ScribusDoc::movePage(const int fromPage, const int toPage, const int dest, const int position)
{
	QList<ScPage*> pageList;
	int numPages = dest;
	pageList.clear();
	for (int i = fromPage; i < toPage; ++i)
	{
		pageList.append(Pages->takeAt(fromPage));
		if (i <= numPages)
			--numPages;
	}
	int pageListCount=pageList.count();
	switch (position)
	{
		case 0: //Before Page
			for (int j = 0; j < pageListCount; ++j)
				Pages->insert(numPages++, pageList.at(j));
			break;
		case 1: //After Page
			for (int j = 0; j < pageListCount; ++j)
				Pages->insert(++numPages, pageList.at(j));
			break;
		case 2: //To End
			for (int j = 0; j < pageListCount; ++j)
				Pages->append(pageList.at(j));
			break;
	}

	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::MovePage, "", Um::IDocument);
		ss->set("PAGE_MOVE");
		ss->set("PAGE_MOVE_FROM", fromPage);
		ss->set("PAGE_MOVE_TO", toPage);
		ss->set("PAGE_MOVE_DEST", dest);
		ss->set("PAGE_MOVE_NEWPOS", position);
		m_undoManager->action(this, ss);
	}
	reformPages();
	if (m_View && m_ScMW)
	{
		m_View->reformPagesView();
		m_ScMW->updateGUIAfterPagesChanged();
	}
	updateEndnotesFrames();
	changed();
}

void ScribusDoc::restoreMovePage(SimpleState* ss, bool isUndo)
{
	int fromPage = ss->getInt("PAGE_MOVE_FROM");
	int toPage = ss->getInt("PAGE_MOVE_TO");
	int position = ss->getInt("PAGE_MOVE_NEWPOS");
	int dest = ss->getInt("PAGE_MOVE_DEST");

	if (isUndo)
	{
		int newPageDest = fromPage;
		int newPageFrom = 0;
		int newPageTo   = 0;
		int newPosition = 0;
		int pagesMoved  = toPage - fromPage;
		switch (position)
		{
			case 0: //Before Page
				newPageDest = (toPage <= dest) ? fromPage : toPage;
				newPageFrom = (toPage <= dest) ? (dest - pagesMoved) : dest;
				break;
			case 1: //After Page
				newPageDest = (toPage <= dest) ? fromPage : toPage;
				newPageFrom = (toPage <= dest) ? (dest-pagesMoved + 1) : (dest + 1);
				break;
			case 2: //To End
				newPageFrom = Pages->count() - pagesMoved;
				break;
		}
		newPageTo = newPageFrom + pagesMoved;
		movePage(newPageFrom, newPageTo, newPageDest, newPosition);
	}
	else
		movePage(fromPage, toPage, dest, position);
}


int ScribusDoc::addAutomaticTextFrame(const int pageNumber)
{
	if (!m_automaticTextFrames)
		return -1;
	ScPage *addToPage=DocPages.at(pageNumber);
	if ((!masterPageMode()) && (usesAutomaticTextFrames()))// && (!isLoading()))
	{
		int z = itemAdd(PageItem::TextFrame, PageItem::Unspecified,
						addToPage->Margins.left()+addToPage->xOffset(),
						addToPage->Margins.top()+addToPage->yOffset(), m_docPrefsData.docSetupPrefs.pageWidth-addToPage->Margins.right()-addToPage->Margins.left(),
						m_docPrefsData.docSetupPrefs.pageHeight-addToPage->Margins.bottom()-addToPage->Margins.top(),
						1, CommonStrings::None, m_docPrefsData.itemToolPrefs.shapeLineColor);
		Items->at(z)->isAutoText = true;
		Items->at(z)->Cols = qRound(PageSp);
		Items->at(z)->ColGap = PageSpa;
		if (LastAuto != 0)
			LastAuto->link(Items->at(z));
		else
			FirstAuto = Items->at(z);
		LastAuto = Items->at(z);
		Items->at(z)->setRedrawBounding();
		updateEndnotesFrames();
		return z;
	}
	return -1;
}


int ScribusDoc::addLayer(const QString& layerName, const bool activate)
{
	int lId = Layers.addLayer(layerName);
	if (lId==-1)
		return -1;
	if (activate)
		setActiveLayer(lId);
	const ScLayer* ll = Layers.layerByID(lId);
	if (!ll)
		qWarning() << "Layer added without undo, could not get layer back for undo action creation";
	else
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::AddLayer, "", Um::ICreate);
		ss->set("ADD_LAYER");
		ss->set("ACTIVE", m_ActiveLayer);
		ss->set("NAME", ll->Name);
		ss->set("LAYER_NR", ll->ID);
		m_undoManager->action(this, ss, DocName, Um::ILayer);
	}

	changed();
	return lId;
}


void ScribusDoc::copyLayer(int layerIDToCopy, int whereToInsert)
{
	if (!setActiveLayer(whereToInsert))
		return;
	Selection sourceSelection(this);
	for (int ite = 0; ite < Items->count(); ++ite)
	{
		PageItem *itemToCopy = Items->at(ite);
		if (itemToCopy->LayerID == layerIDToCopy)
		{
			sourceSelection.addItem(itemToCopy);
		}
	}
	if (sourceSelection.count() != 0)
	{
		ScriXmlDoc ss;
		QString dataS = ss.WriteElem(this, &sourceSelection);
		ss.ReadElemToLayer(dataS, m_appPrefsData.fontPrefs.AvailFonts, this, Pages->at(0)->xOffset(), Pages->at(0)->yOffset(), false, true, m_appPrefsData.fontPrefs.GFontSub, whereToInsert);
	}
	sourceSelection.clear();
	changed();
}


bool ScribusDoc::deleteLayer(const int layerID, const bool deleteItems)
{
	if (Layers.count() < 2)
		return false;
	const ScLayer* lToRemove = Layers.layerByID(layerID);
	if (!lToRemove)
		return false;
	int layerLevel = lToRemove->Level;
	QString name   = lToRemove->Name;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::Layer, Um::IDocument, Um::DeleteLayer, "", Um::IDelete);

	rebuildItemLists();
	if (ScCore->usingGUI())
		removeLayer(layerID, deleteItems);

	//Now delete the layer
	Layers.removeLayerByID(layerID);

	if (activeTransaction)
	{
		SimpleState *ss = new SimpleState(Um::DeleteLayer, "", Um::IDelete);
		ss->set("REMOVE_LAYER");
		ss->set("ACTIVE", layerID);
		ss->set("LEVEL", layerLevel);
		ss->set("NAME", name);
		ss->set("LAYER_NR", layerID);
		ss->set("DELETE", deleteItems);
		m_undoManager->action(this, ss, DocName, Um::ILayer);
		activeTransaction.commit();
	}

	changed();
	return true;
}


int ScribusDoc::activeLayer()
{
	return m_ActiveLayer;
}


const QString& ScribusDoc::activeLayerName()
{
	const ScLayer* ll = Layers.layerByID(m_ActiveLayer);
	Q_ASSERT(ll);
	return ll->Name;
}


bool ScribusDoc::setActiveLayer(const int layerToActivate)
{
	const ScLayer* ll = Layers.layerByID(layerToActivate);
	Q_ASSERT(ll);
	if (ll)
		m_ActiveLayer=layerToActivate;
	return (ll != NULL);
}


bool ScribusDoc::setActiveLayer(const QString& layerNameToActivate)
{
	const ScLayer* ll = Layers.layerByName(layerNameToActivate);
	Q_ASSERT(ll);
	if (ll)
		m_ActiveLayer=ll->ID;
	return (ll != NULL);
}


bool ScribusDoc::setLayerPrintable(const int layerID, const bool isPrintable)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			if (it->isPrintable!=isPrintable && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(isPrintable ? Um::PrintLayer : Um::DoNotPrintLayer, "", Um::IPrint);
				ss->set("PRINT_LAYER");
				ss->set("ACTIVE", it->ID);
				ss->set("PRINT", isPrintable);
				m_undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->isPrintable = isPrintable;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerPrintable(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->isPrintable;
	}
	return false;
}


bool ScribusDoc::setLayerVisible(const int layerID, const bool isViewable)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			it->isViewable = isViewable;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerVisible(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->isViewable;
	}
	return false;
}

bool ScribusDoc::setLayerSelectable(const int layerID, const bool isSelectable)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			it->isSelectable = isSelectable;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerSelectable(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->isSelectable;
	}
	return false;
}


bool ScribusDoc::setLayerLocked(const int layerID, const bool isLocked)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			// == because isEditable vs isLocked...
			if (it->isEditable==isLocked && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(isLocked ? Um::SetLayerLocked : Um::SetLayerUnlocked, "", Um::ILayer);
				ss->set("LAYER_LOCK");
				ss->set("ACTIVE", it->ID);
				ss->set("LOCK", isLocked);
				m_undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->isEditable = !isLocked;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerLocked(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return !it->isEditable;
	}
	return false;
}


bool ScribusDoc::setLayerFlow(const int layerID, const bool flow)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			if (it->flowControl!=flow && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(flow ? Um::FlowLayer : Um::DisableFlowLayer, "", Um::ITextFrame);
				ss->set("LAYER_FLOW");
				ss->set("ACTIVE", it->ID);
				ss->set("FLOW", flow);
				m_undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->flowControl = flow;
			found=true;
			break;
		}
	}
	if (found)
	{
		// #9188 : invalidate layout of items below layer
		for (it = Layers.begin(); it != itend; ++it)
		{
			if (it->ID == layerID)
				break;
			invalidateLayer(it->ID);
		}
		changed();
	}
	return found;
}


bool ScribusDoc::layerFlow(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->flowControl;
	}
	return false;
}


bool ScribusDoc::setLayerTransparency(const int layerID, double trans)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			if (it->transparency!=trans && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(Um::SetLayerTransparency, "", Um::ILayer);
				ss->set("LAYER_TRANSPARENCY");
				ss->set("ACTIVE", it->ID);
				ss->set("OLD_TRANS", it->transparency);
				ss->set("NEW_TRANS", trans);
				m_undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->transparency = trans;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


double ScribusDoc::layerTransparency(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->transparency;
	}
	return 1.0;
}


bool ScribusDoc::setLayerBlendMode(const int layerID, int blend)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			if (it->blendMode!=blend && UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(Um::SetLayerBlendMode, "", Um::ILayer);
				ss->set("LAYER_BLENDMODE");
				ss->set("ACTIVE", it->ID);
				ss->set("OLD_BLENDMODE", it->blendMode);
				ss->set("NEW_BLENDMODE", blend);
				m_undoManager->action(this, ss, it->Name, Um::ILayer);
			}
			it->blendMode = blend;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


int ScribusDoc::layerBlendMode(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->blendMode;
	}
	return 0;
}


bool ScribusDoc::setLayerOutline(const int layerID, const bool outline)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			it->outlineMode = outline;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerOutline(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->outlineMode;
	}
	return false;
}


bool ScribusDoc::setLayerMarker(const int layerID, QColor color)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	bool found=false;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
		{
			it->markerColor = color;
			found=true;
			break;
		}
	}
	if (found)
		changed();
	return found;
}


QColor ScribusDoc::layerMarker(const int layerID)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->ID == layerID)
			return it->markerColor;
	}
	return QColor(0, 0, 0);
}


int ScribusDoc::layerLevelFromID(const int layerID)
{
	int layerCount=Layers.count();
	for (int i=0; i < layerCount; ++i)
	{
		if (Layers[i].ID == layerID)
			return Layers[i].Level;
	}
	return -1;
}


int ScribusDoc::layerCount() const
{
	return Layers.count();
}


int ScribusDoc::layerIDFromLevel(const int layerLevel)
{
	int layerCount=Layers.count();
	for (int i=0; i < layerCount; ++i)
	{
		if (Layers[i].Level == layerLevel)
			return Layers[i].ID;
	}
	return -1;
}

int ScribusDoc::layerIDFromName(QString name)
{
	ScLayers::iterator itend=Layers.end();
	ScLayers::iterator it;
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->Name == name)
			return it->ID;
	}
	return -1;
}

bool ScribusDoc::lowerLayer(const int layerID)
{
	return lowerLayerByLevel(layerLevelFromID(layerID));
}


bool ScribusDoc::lowerLayerByLevel(const int layerLevel)
{
	if (Layers.count() < 2)
		return false;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::LowerLayer, "", Um::IDown);
		ss->set("DOWN_LAYER");
		ss->set("ACTIVE", layerIDFromLevel(layerLevel));
		m_undoManager->action(this, ss, DocName, Um::ILayer);
	}

	ScLayers::iterator it;
	ScLayers::iterator itend=Layers.end();
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->Level == layerLevel-1)
			break;
	}
	ScLayers::iterator it2;
	ScLayers::iterator it2end=Layers.end();
	for (it2 = Layers.begin(); it2 != it2end; ++it2)
	{
		if (it2->Level == layerLevel)
			break;
	}
	it2->Level -= 1;
	it->Level  += 1;
	// #9188 : invalidate layout of items in below layers
	int maxLevel = qMax(it->Level, it2->Level);
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->flowControl)
			invalidateLayer(it->ID);
		if (it->Level == maxLevel)
			break;
	}
	return true;
}


bool ScribusDoc::raiseLayer(const int layerID)
{
	return raiseLayerByLevel(layerLevelFromID(layerID));
}


bool ScribusDoc::raiseLayerByLevel(const int layerLevel)
{
	if (Layers.count() < 2)
		return false;
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::RaiseLayer, "", Um::IUp);
		ss->set("UP_LAYER");
		ss->set("ACTIVE", layerIDFromLevel(layerLevel));
		m_undoManager->action(this, ss, DocName, Um::ILayer);
	}

	ScLayers::iterator it;
	ScLayers::iterator itend=Layers.end();
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->Level == layerLevel+1)
			break;
	}
	ScLayers::iterator it2;
	ScLayers::iterator it2end=Layers.end();
	for (it2 = Layers.begin(); it2 != it2end; ++it2)
	{
		if (it2->Level == layerLevel)
			break;
	}
	it2->Level += 1;
	it->Level  -= 1;
	// #9188 : invalidate layout of items in below layers
	int maxLevel = qMax(it->Level, it2->Level);
	for (it = Layers.begin(); it != itend; ++it)
	{
		if (it->flowControl)
			invalidateLayer(it->ID);
		if (it->Level == maxLevel)
			break;
	}
	return true;
}


QString ScribusDoc::layerName(const int layerID) const
{
	int layerCount=Layers.count();
	for (int i=0; i < layerCount; ++i)
	{
		if (Layers[i].ID == layerID)
			return Layers[i].Name;
	}
	return QString::null;
}


bool ScribusDoc::changeLayerName(const int layerID, const QString& newName)
{
	int layerCount=Layers.count();
	bool found=false;
	for (int i=0; i < layerCount; ++i)
	{
		if (Layers[i].ID == layerID)
		{
			if (Layers[i].Name != newName)
			{
				if (UndoManager::undoEnabled())
				{
					SimpleState *ss = new SimpleState(Um::SetLayerName,
													QString(Um::FromTo).arg(Layers[i].Name).arg(newName),
													Um::IDown);
					ss->set("CHANGE_NAME");
					ss->set("ACTIVE", m_ActiveLayer);
					ss->set("NEW_NAME", newName);
					ss->set("OLD_NAME", Layers[i].Name);
					m_undoManager->action(this, ss, DocName, Um::ILayer);
				}
				Layers[i].Name = newName;
				found=true;
			}
			break;
		}
	}
	if (found)
		changed();
	return found;
}


bool ScribusDoc::layerContainsItems(const int layerID)
{
	QList<PageItem*> allItems;
	int masterItemsCount=MasterItems.count();
	for (int i = 0; i < masterItemsCount; ++i)
	{
		PageItem* currItem = MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->LayerID == layerID)
				return true;
		}
		allItems.clear();
	}
	int docItemsCount=DocItems.count();
	for (int i = 0; i < docItemsCount; ++i)
	{
		PageItem* currItem = DocItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->LayerID == layerID)
				return true;
		}
		allItems.clear();
	}
	return false;
}


void ScribusDoc::orderedLayerList(QStringList* list)
{
	Q_ASSERT(list!=NULL);
	int layerCount=Layers.count();
	if (layerCount != 0)
	{
		for (int i=0; i < layerCount; ++i)
		{
			ScLayers::iterator itend=Layers.end();
			for (ScLayers::iterator it = Layers.begin(); it != itend; ++it)
			{
				if (layerCount-(*it).Level-1 == i)
					list->append((*it).Name);
			}
 		}
 	}
}

int ScribusDoc::firstLayerID()
{
	QStringList newNames;
	orderedLayerList(&newNames);
	return layerIDFromName(newNames.first());
}

bool ScribusDoc::renumberLayer(const int layerID, const int newLayerID)
{
	int layerCount=Layers.count();
	int foundIndex = 0;
	bool found=false;
	//Find layer to renumber, if found the new number, return as it exists already.
	for (int i=0; i < layerCount; ++i)
	{
		if (Layers[i].ID == layerID)
		{
			foundIndex=i;
			found=true;
		}
		else
		if (Layers[i].ID == newLayerID)
			return false;
	}
	if (!found)
		return false;
	Layers[foundIndex].ID=newLayerID;
	return true;
}


//Make the doc delete the items, not the view. TODO: Currently does nada, zilch, zero
bool ScribusDoc::deleteTaggedItems()
{
//	QString tooltip = Um::ItemsInvolved + "\n";
	//Master Items
	for (int i = 0; i < MasterItems.count(); ++i)
	{
		if (MasterItems.at(i)->isTagged())
		{
		}
	}
	//Doc Items
	for (int i = 0; i < DocItems.count(); ++i)
	{
		if (DocItems.at(i)->isTagged())
		{
		}
	}
	return true;
}


void ScribusDoc::replaceLineStyleColors(const QMap<QString, QString>& colorMap)
{
	multiLine::iterator its;
	QMap<QString, QString>::const_iterator it;
	QHash<QString,multiLine>::iterator  itl;
	for (itl = MLineStyles.begin(); itl != MLineStyles.end(); ++itl)
	{
		multiLine& mline = itl.value();
		for (its = mline.begin(); its != mline.end(); ++its)
		{
			struct SingleLine& sline = *its;
			it = colorMap.find(sline.Color);
			if (it != colorMap.end())
				sline.Color = it.value();
		}
	}
}

void ScribusDoc::getUsedColors(ColorList &colorsToUse, bool spot)
{
	bool found;
	colorsToUse.clear();
	colorsToUse.setDocument(this);
	ColorList::Iterator it;

	ResourceCollection resources;
	this->getNamedResources(resources);
	const QMap<QString, QString>& resColors = resources.colors();

	for (it = PageColors.begin(); it != PageColors.end(); ++it)
	{
		found = false;
		// Tool preferences colors
		if ((it.key() == m_docPrefsData.itemToolPrefs.shapeFillColor) || (it.key() == m_docPrefsData.itemToolPrefs.shapeLineColor) || (it.key() == m_docPrefsData.itemToolPrefs.imageFillColor)
				 || (it.key() == m_docPrefsData.itemToolPrefs.imageStrokeColor) || (it.key() == m_docPrefsData.itemToolPrefs.lineColor) || (it.key() == m_docPrefsData.itemToolPrefs.textColor))
		{
			if (spot)
			{
				if (it.value().isSpotColor())
					colorsToUse.insert(it.key(), it.value());
			}
			else
				colorsToUse.insert(it.key(), it.value());
			continue;
		}
		// Current paragraph style colors
		if ((it.key() == currentStyle.charStyle().fillColor()) || (it.key() == currentStyle.charStyle().strokeColor()) || (it.key() == currentStyle.charStyle().backColor()))
			found = true;
		// Resources colors
		if (!found)
			found = resColors.contains(it.key());
		// Line styles colors
		if (!found)
			found = lineStylesUseColor(it.key());
		if (found)
		{
			if (spot)
			{
				if (it.value().isSpotColor())
					colorsToUse.insert(it.key(), it.value());
			}
			else
				colorsToUse.insert(it.key(), it.value());
			continue;
		}
	}
}

bool ScribusDoc::lineStylesUseColor(const QString& colorName)
{
	bool found = false;
	QHash<QString,multiLine>::const_iterator itm, itmend;
	multiLine::const_iterator its, itsend;
	itmend = MLineStyles.constEnd();
	for (itm = MLineStyles.constBegin(); itm != itmend && !found; ++itm)
	{
		const multiLine& ml = itm.value();
		itsend = ml.constEnd();
		for (its = ml.constBegin(); its != itsend; ++its)
		{
			if ( its->Color == colorName )
			{
				found = true;
				break;
			}
		}
	}
	return found;
}

void ScribusDoc::getUsedGradients(QHash<QString, VGradient> &gradients)
{
	ResourceCollection resources;
	this->getNamedResources(resources);
	const QMap<QString, QString>& resGradients = resources.gradients();
	QHash<QString, VGradient>::iterator it;
	for (it = docGradients.begin(); it != docGradients.end(); ++it)
	{
		if (resGradients.contains(it.key()))
			gradients.insert(it.key(), docGradients[it.key()]);
	}
}

bool ScribusDoc::addGradient(QString &name, VGradient &gradient)
{
	for (QHash<QString, VGradient>::Iterator it = docGradients.begin(); it != docGradients.end(); ++it)
 	{
		if (it.value() == gradient)
		{
			name = it.key();
			return false;
		}
 	}
	QString tmp;
	if (docGradients.contains(name))
		name += "("+tmp.setNum(docGradients.count())+")";
	docGradients.insert(name, gradient);
	return true;
}

void ScribusDoc::setGradients(QHash<QString, VGradient> &gradients)
{
	docGradients.clear();
	docGradients = gradients;
}

bool ScribusDoc::addPattern(QString &name, ScPattern& pattern)
{
	QString tmp;
	if (docPatterns.contains(name))
		name += "("+tmp.setNum(docPatterns.count())+")";
//		name = tr("Copy_of_")+name;
	docPatterns.insert(name, pattern);
	QList<PageItem*> allItems = getAllItems(pattern.items);
	for (int i = 0; i < allItems.count(); i++)
	{
		PageItem* currItem = allItems.at(i);
		currItem->setLayer(0);
	}
	return true;
}

void ScribusDoc::removePattern(QString name)
{
	docPatterns.remove(name);
	QList<PageItem*> allItems;
	for (int i = 0; i < DocItems.count(); ++i)
	{
		PageItem *currItem = DocItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->pattern() == name)
				currItem->setPattern("");
		}
		allItems.clear();
	}
	for (int i = 0; i < MasterItems.count(); ++i)
	{
		PageItem *currItem = MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int j = 0; j < allItems.count(); j++)
		{
			currItem = allItems.at(j);
			if (currItem->pattern() == name)
				currItem->setPattern("");
		}
		allItems.clear();
	}
	for (QHash<int, PageItem*>::iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		PageItem *currItem = itf.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->pattern() == name)
				currItem->setPattern("");
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if (currItem->pattern() == name)
					currItem->setPattern("");
			}
			allItems.clear();
		}
	}
}

ScPattern* ScribusDoc::checkedPattern(const QString &name)
{
	if (name.isEmpty() || !docPatterns.contains(name))
		return 0;
	ScPattern* pattern = &docPatterns[name];
	if (pattern->width <= 0 || pattern->height <= 0)
		return 0;
	if (pattern->getPattern()->isNull())
		return 0;
	return pattern;
}

void ScribusDoc::setPatterns(QHash<QString, ScPattern> &patterns)
{
	docPatterns.clear();
	docPatterns = patterns;
}

QStringList ScribusDoc::getUsedPatterns() const
{
	QList<PageItem*> allItems;
	QStringList results;
	for (int i = 0; i < MasterItems.count(); ++i)
	{
		PageItem* currItem = MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int j = 0; j < allItems.count(); j++)
		{
			currItem = allItems.at(j);
			if ((!results.contains(currItem->pattern())) && ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol)))
				results.append(currItem->pattern());
			if (!currItem->strokePattern().isEmpty())
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
			if (!currItem->patternMask().isEmpty())
			{
				if (!results.contains(currItem->patternMask()))
					results.append(currItem->patternMask());
			}
		}
		allItems.clear();
	}
	for (int i = 0; i < DocItems.count(); ++i)
	{
		PageItem* currItem = DocItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int j = 0; j < allItems.count(); j++)
		{
			currItem = allItems.at(j);
			if ((!results.contains(currItem->pattern())) && ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol)))
				results.append(currItem->pattern());
			if (!currItem->strokePattern().isEmpty())
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
			if (!currItem->patternMask().isEmpty())
			{
				if (!results.contains(currItem->patternMask()))
					results.append(currItem->patternMask());
			}
		}
		allItems.clear();
	}
	for (QHash<int, PageItem*>::const_iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		PageItem *currItem = itf.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int i = 0; i < allItems.count(); i++)
		{
			currItem = allItems.at(i);
			if ((!results.contains(currItem->pattern())) && ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol)))
				results.append(currItem->pattern());
			if (!currItem->strokePattern().isEmpty())
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
			if (!currItem->patternMask().isEmpty())
			{
				if (!results.contains(currItem->patternMask()))
					results.append(currItem->patternMask());
			}
		}
		allItems.clear();
	}
	for (QHash<QString, ScPattern>::const_iterator it = docPatterns.begin(); it != docPatterns.end(); ++it)
	{
		for (int i = 0; i < it.value().items.count(); ++i)
		{
			PageItem* currItem = it.value().items.at(i);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int j = 0; j < allItems.count(); j++)
			{
				currItem = allItems.at(j);
				if ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol))
				{
					const QString& patName = currItem->pattern();
					if (!patName.isEmpty() && !results.contains(patName))
						results.append(patName);
				}
				const QString& pat2 = currItem->strokePattern();
				if (!pat2.isEmpty() && !results.contains(pat2))
					results.append(pat2);
				const QString& pat3 = currItem->patternMask();
				if (!pat3.isEmpty() && !results.contains(pat3))
					results.append(pat3);
			}
			allItems.clear();
		}
	}
	return results;
}


QStringList ScribusDoc::getUsedPatternsSelection(Selection* customSelection)
{
	QStringList results;
	int selectedItemCount = customSelection->count();
	if (selectedItemCount != 0)
	{
		for (int i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = customSelection->itemAt(i);
			QList<PageItem*> allItems;
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int j = 0; j < allItems.count(); j++)
			{
				currItem = allItems.at(j);
				if ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol))
				{
					const QString& pat = currItem->pattern();
					if (!pat.isEmpty() && !results.contains(pat))
						results.append(currItem->pattern());
				}
				const QString& pat2 = currItem->strokePattern();
				if (!pat2.isEmpty() && !results.contains(pat2))
					results.append(currItem->strokePattern());
				const QString& pat3 = currItem->patternMask();
				if (!pat3.isEmpty() && !results.contains(pat3))
					results.append(currItem->patternMask());
			}
			allItems.clear();
		}
		QStringList results2 = results;
		for (int i = 0; i < results.count(); ++i)
		{
			QStringList pats = getUsedPatternsHelper(results[i], results2);
			if (pats.isEmpty())
				continue;
			for (int j = 0; j < pats.count(); j++)
			{
				if (!results2.contains(pats[j]))
					results2.append(pats[j]);
			}
		}
		return results2;
	}
	else
		return QStringList();
}

QStringList ScribusDoc::getUsedPatternsHelper(QString pattern, QStringList &results)
{
	if (!docPatterns.contains(pattern))
		return QStringList();
	ScPattern *pat = &docPatterns[pattern];
	QStringList pats;
	for (int i = 0; i < pat->items.count(); ++i)
	{
		QList<PageItem*> allItems;
		if (pat->items.at(i)->isGroup())
			allItems = pat->items.at(i)->getAllChildren();
		else
			allItems.append(pat->items.at(i));
		for (int j = 0; j < allItems.count(); j++)
		{
			PageItem *currItem = allItems.at(j);
			if ((currItem->GrType == 8) || (currItem->itemType() == PageItem::Symbol))
			{
				const QString& patName = currItem->pattern();
				if (!patName.isEmpty() && !results.contains(patName))
					pats.append(patName);
			}
			const QString& pat2 = currItem->strokePattern();
			if (!pat2.isEmpty() && !results.contains(pat2))
				pats.append(currItem->strokePattern());
			const QString& pat3 = currItem->patternMask();
			if (!pat3.isEmpty() && !results.contains(pat3))
				pats.append(currItem->patternMask());
		}
		allItems.clear();
	}
	if (!pats.isEmpty())
	{
		results += pats;
		for (int i = 0; i < pats.count(); ++i)
		{
			QStringList pp = getUsedPatternsHelper(pats[i], results);
			if (pp.isEmpty())
				continue;
			for (int cc = 0; cc < pp.count(); cc++)
			{
				if (!results.contains(pp[cc]))
					results.append(pp[cc]);
			}
		}
	}
	return results;
}

QStringList ScribusDoc::getPatternDependencyList(QStringList used)
{
	QStringList results;
	QStringList pp;
	QStringList pats = used;
	QStack<QStringList> patternStack;
	patternStack.push(pats);
	while (!pats.isEmpty())
	{
		for (int i = 0; i < pats.count(); ++i)
		{
			pp = getUsedPatternsHelper(pats[i], results);
		}
		pats = results;
		results.clear();
		if (!pats.isEmpty())
			patternStack.push(pats);
	}
	results.clear();
	while (patternStack.count() != 0)
	{
		pp = patternStack.pop();
		for (int i = 0; i < pp.count(); i++)
		{
			if (!results.contains(pp[i]))
				results.append(pp[i]);
		}
		if (patternStack.count() == 0)
			break;
	}
	return results;
}

QStringList ScribusDoc::getUsedSymbols()
{
	QList<PageItem*> allItems;
	QStringList results;
	for (int i = 0; i < MasterItems.count(); ++i)
	{
		PageItem* currItem = MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int j = 0; j < allItems.count(); j++)
		{
			currItem = allItems.at(j);
			if ((!results.contains(currItem->pattern())) && (currItem->itemType() == PageItem::Symbol) && (!currItem->pattern().isEmpty()))
				results.append(currItem->pattern());
			if ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath))
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
			if ((!results.contains(currItem->strokePattern())) && ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath)))
				results.append(currItem->pattern());
		}
		allItems.clear();
	}
	for (int i = 0; i < DocItems.count(); ++i)
	{
		PageItem* currItem = DocItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int j = 0; j < allItems.count(); j++)
		{
			currItem = allItems.at(j);
			if ((!results.contains(currItem->pattern())) && (currItem->itemType() == PageItem::Symbol) && (!currItem->pattern().isEmpty()))
				results.append(currItem->pattern());
			if ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath))
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
		}
		allItems.clear();
	}
	for (QHash<int, PageItem*>::iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		PageItem *currItem = itf.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int i = 0; i < allItems.count(); i++)
		{
			currItem = allItems.at(i);
			if ((!results.contains(currItem->pattern())) && (currItem->itemType() == PageItem::Symbol) && (!currItem->pattern().isEmpty()))
				results.append(currItem->pattern());
			if ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath))
			{
				if (!results.contains(currItem->strokePattern()))
					results.append(currItem->strokePattern());
			}
		}
		allItems.clear();
	}
	for (QHash<QString, ScPattern>::Iterator it = docPatterns.begin(); it != docPatterns.end(); ++it)
	{
		for (int i = 0; i < it.value().items.count(); ++i)
		{
			PageItem* currItem = it.value().items.at(i);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int j = 0; j < allItems.count(); j++)
			{
				currItem = allItems.at(j);
				if ((!results.contains(currItem->pattern())) && (currItem->itemType() == PageItem::Symbol) && (!currItem->pattern().isEmpty()))
					results.append(currItem->pattern());
				if ((!currItem->strokePattern().isEmpty()) && (currItem->patternStrokePath))
				{
					if (!results.contains(currItem->strokePattern()))
						results.append(currItem->strokePattern());
				}
			}
			allItems.clear();
		}
	}
	return results;
}

QStringList ScribusDoc::getUsedSymbolsHelper(QString pattern, QStringList &results)
{
	ScPattern *pat = &docPatterns[pattern];
	QStringList pats;
	for (int i = 0; i < pat->items.count(); ++i)
	{
		if (pat->items.at(i)->itemType() == PageItem::Symbol)
		{
			const QString& patName = pat->items.at(i)->pattern();
			if (!patName.isEmpty() && !results.contains(patName))
				pats.append(patName);
		}
	}
	if (!pats.isEmpty())
	{
		for (int i = 0; i < pats.count(); ++i)
			getUsedSymbolsHelper(pats[i], results);
	}
	return pats;
}

QMap<QString,int> ScribusDoc::reorganiseFonts()
{
	QMap<QString,int> Really;
	QList<PageItem*> allItems;

	bool wasMasterPageMode = m_masterPageMode;

	for (int i = 0; i < 2; ++i)
	{
		switch (i)
		{
			case 0:
				setMasterPageMode(true); // Necessary to avoid crash if some relayouting is necessary
				allItems = MasterItems;
				break;
			case 1:
				setMasterPageMode(false);
				allItems = DocItems;
				break;
		}
		PageItem* it = NULL;
		while (allItems.count() > 0)
		{
			it = allItems.takeFirst();
			if (it->isGroup() || it->isTable())
			{
				allItems = it->getChildren() + allItems;
				continue;
			}
			if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
			{
				if (it->invalid)
					it->layout();
				QString fontName(it->itemText.defaultStyle().charStyle().font().replacementName());
				Really.insert(fontName, UsedFonts[fontName]);
				int start = it->firstInFrame();
				int stop = it->lastInFrame();
				for (int e = start; e <= stop; ++e)
				{
					QString rep = it->itemText.charStyle(e).font().replacementName();
					if (Really.contains(rep))
						continue;
					Really.insert(rep, UsedFonts[rep]);
				}
			}
		}
	}

	setMasterPageMode(wasMasterPageMode);

	allItems = FrameItems.values();
	while (allItems.count() > 0)
	{
		PageItem *it = allItems.takeFirst();
		if (it->isGroup() || it->isTable())
		{
			allItems = it->getChildren() + allItems;
			continue;
		}
		if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
		{
			if (it->invalid)
				it->layout();
			QString fontName(it->itemText.defaultStyle().charStyle().font().replacementName());
			Really.insert(fontName, UsedFonts[fontName]);
			int start = it->firstInFrame();
			int stop = it->lastInFrame();
			for (int e = start; e <= stop; ++e)
			{
				QString rep = it->itemText.charStyle(e).font().replacementName();
				if (Really.contains(rep))
					continue;
				Really.insert(rep, UsedFonts[rep]);
			}
		}
	}

	QHash<QString, ScPattern>::const_iterator patIter;
	for (patIter = docPatterns.begin(); patIter != docPatterns.end(); ++patIter)
	{
		const ScPattern& pat = patIter.value();
		allItems = pat.items;
		while (allItems.count() > 0)
		{
			PageItem *it = allItems.takeFirst();
			if (it->isGroup() || it->isTable())
			{
				allItems = it->getChildren() + allItems;
				continue;
			}
			if ((it->itemType() == PageItem::TextFrame) || (it->itemType() == PageItem::PathText))
			{
				if (it->invalid)
					it->layout();
				QString fontName(it->itemText.defaultStyle().charStyle().font().replacementName());
				Really.insert(fontName, UsedFonts[fontName]);
				int start = it->firstInFrame();
				int stop = it->lastInFrame();
				for (int e = start; e <= stop; ++e)
				{
					QString rep = it->itemText.charStyle(e).font().replacementName();
					if (Really.contains(rep))
						continue;
					Really.insert(rep, UsedFonts[rep]);
				}
			}
		}
	}

	QMap<QString,int>::Iterator itfo, itnext;
	for (itfo = UsedFonts.begin(); itfo != UsedFonts.end(); itfo = itnext)
	{
		itnext = itfo;
		++itnext;
		if (!Really.contains(itfo.key()))
		{
			(*AllFonts)[itfo.key()].decreaseUsage();
			UsedFonts.erase(itfo);
		}
	}
	PrefsManager* prefsManager=PrefsManager::instance();
	AddFont(prefsManager->appPrefs.itemToolPrefs.textFont);//, prefsManager->appPrefs.AvailFonts[prefsManager->appPrefs.itemToolPrefs.textFont]->Font);
	AddFont(m_docPrefsData.itemToolPrefs.textFont);//, prefsManager->appPrefs.AvailFonts[itemToolPrefs.textFont]->Font);
	return Really;
}

void ScribusDoc::getUsedFonts(QMap<QString, QMap<uint, FPointArray> > & Really)
{
	QList<PageItem*>  allItems;
	QList<PageItem*>* itemLists[] = { &MasterItems, &DocItems };
	PageItem* it = NULL;

	for (int i = 0; i < 2; ++i)
	{
		allItems = *(itemLists[i]);
		while (allItems.count() > 0)
		{
			it = allItems.takeFirst();
			if (it->isGroup() || it->isTable())
			{
				allItems = it->getChildren() + allItems;
				continue;
			}
			checkItemForFonts(it, Really, i);
		}
	}

	allItems = FrameItems.values();
	while (allItems.count() > 0)
	{
		PageItem *ite = allItems.takeFirst();
		if (it->isGroup() || it->isTable())
		{
			allItems = it->getChildren() + allItems;
			continue;
		}
		checkItemForFonts(ite, Really, 3);
	}

	QStringList patterns = getUsedPatterns();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		allItems = pa.items;
		while (allItems.count() > 0)
		{
			it = allItems.takeFirst();
			if (it->isGroup() || it->isTable())
			{
				allItems = it->getChildren() + allItems;
				continue;
			}
			checkItemForFonts(it, Really, 3);
		}
	}
}

// a special painter that does not actually paint anything,
// but collects the glyphs used in the frame.
class UsedGlyphsPainter: public TextLayoutPainter
{
public:
	UsedGlyphsPainter(QMap<QString, QMap<uint, FPointArray> > & Really)
		: m_really(Really)
	{}

	void drawGlyph(const GlyphCluster& gc)
	{
		if (gc.isControlGlyphs())
			return;
		foreach (const GlyphLayout& gl, gc.glyphs()) {
			QString replacementName = font().replacementName();
			if (!replacementName.isEmpty())
			{
				FPointArray outline = font().glyphOutline(gl.glyph);
				m_really[replacementName].insert(gl.glyph, outline);
			}
		}
	}

	void drawGlyphOutline(const GlyphCluster& gc, bool)
	{
		drawGlyph(gc);
	}

	// we don't need this one
	void drawLine(QPointF, QPointF) {}
	void drawRect(QRectF) {}
	void drawObject(PageItem*) {}

private:
	QMap<QString, QMap<uint, FPointArray> > & m_really;
};

void ScribusDoc::checkItemForFonts(PageItem *it, QMap<QString, QMap<uint, FPointArray> > & Really, uint lc)
{
	if (!it->isTextFrame() && !it->isPathText())
		return;

	if (it->invalid)
	{
		bool wasMasterPageMode = m_masterPageMode;
		setMasterPageMode(it->OnMasterPage.length() > 0);
		it->layout();
		setMasterPageMode(wasMasterPageMode);
	}

	// This works pretty well except for the case of page numbers and al. placed on masterpages
	// where layout may depend on the page where the masterpage item is placed
	UsedGlyphsPainter p(Really);
	it->textLayout.render(&p);

	// Process page numbers and page count special characters
	int start = it->isTextFrame() ? it->firstInFrame() : 0;
	int stop  = it->isTextFrame() ? it->lastInFrame() + 1 : it->itemText.length();
	for (int e = start; e < stop; ++e)
	{
		const ScFace& font = it->itemText.charStyle(e).font();
		double fontSize = it->itemText.charStyle(e).fontSize();
		QString fontName = font.replacementName();
		if (!Really.contains(fontName) )
		{
			if (!fontName.isEmpty())
				Really.insert(fontName, QMap<uint, FPointArray>());
		}
		uint chr = it->itemText.text(e).unicode();
		if ((chr != SpecialChars::PAGENUMBER ) && (chr != SpecialChars::PAGECOUNT))
			continue;

		QStringList txtList;
		QString pageNumberText; //Our page number collection string
		if (chr == SpecialChars::PAGENUMBER)
		{
			if (e > 0 && it->itemText.text(e-1) == SpecialChars::PAGENUMBER)
				pageNumberText = SpecialChars::ZWNBSPACE;
			else if (lc!=0) //If not on a master page just get the page number for the page and the text
			{
				pageNumberText = QString("%1").arg(getSectionPageNumberForPageIndex(it->OwnPage),
								getSectionPageNumberWidthForPageIndex(it->OwnPage),
								getSectionPageNumberFillCharForPageIndex(it->OwnPage));
				txtList.append(pageNumberText);
			}
			else
			{
				//Else, for a page number in a text frame on a master page we must scan
				//all pages to see which ones use this page and get their page numbers.
				//We only add each character of the pages' page number text if its nothing
				//already in the pageNumberText variable. No need to add glyphs twice.
				QString newText;
				uint docPageCount = DocPages.count();
				for (uint a = 0; a < docPageCount; ++a)
				{
					if (DocPages.at(a)->MPageNam == it->OnMasterPage)
					{
						newText = QString("%1").arg(getSectionPageNumberForPageIndex(a),
										getSectionPageNumberWidthForPageIndex(a),
										getSectionPageNumberFillCharForPageIndex(a));
						txtList.append(newText);
					}
				}
			}
		}
		else //ch == SpecialChars::PAGECOUNT
		{
			if (lc != 0)
			{
				int key = getSectionKeyForPageIndex(it->OwnPage);
				if (key == -1)
					pageNumberText = "";
				else
					pageNumberText = QString("%1").arg(getStringFromSequence(m_docPrefsData.docSectionMap[key].type, m_docPrefsData.docSectionMap[key].toindex - m_docPrefsData.docSectionMap[key].fromindex + 1));
				txtList.append(pageNumberText);
			}
			else
			{
				QString newText;
				int docPageCount = DocPages.count();
				for (int a = 0; a < docPageCount; ++a)
				{
					if (DocPages.at(a)->MPageNam == it->OnMasterPage)
					{
						int key = getSectionKeyForPageIndex(a);
						if (key == -1)
							newText = "";
						else
							newText = QString("%1").arg(getStringFromSequence(m_docPrefsData.docSectionMap[key].type, m_docPrefsData.docSectionMap[key].toindex - m_docPrefsData.docSectionMap[key].fromindex + 1));
						txtList.append(newText);
					}
				}
			}
		}

		//Now scan and add any glyphs used in page numbers
		for (int a = 0; a < txtList.count(); ++a)
		{
			CharStyle style(font, fontSize);
			StoryText story;
			story.insertChars(txtList[a]);
			story.setCharStyle(0, txtList[a].count(), style);

			TextShaper textShaper(story, 0);
			QList<GlyphCluster> glyphRuns = textShaper.shape(0, story.length()).glyphs();

			foreach (const GlyphCluster &run, glyphRuns)
			{
				foreach (const GlyphLayout &gl, run.glyphs())
				{
					FPointArray outline(font.glyphOutline(gl.glyph));
					if (!fontName.isEmpty())
						Really[fontName].insert(gl.glyph, outline);
				}
			}
		}
	}

	// Process annotation fonts
	if (it->isAnnotation() && (it->isTextFrame() || it->isPathText()))
	{
		int annotType  = it->annotation().Type();
		bool requiredFont = ((annotType >= Annotation::Button) && (annotType <= Annotation::Listbox) && (annotType != Annotation::Checkbox));
		if (it->itemText.length() > 0 || requiredFont)
		{
			const ScFace& font = it->itemText.defaultStyle().charStyle().font();
			QString fontName = font.replacementName();

			if (!Really.contains(fontName) && !fontName.isEmpty())
				Really.insert(fontName, QMap<uint, FPointArray>());

			for (uint ww = 32; ww < 256; ++ww)
			{
				int unicode = Pdf::fromPDFDocEncoding(ww);
				uint glyph  = font.char2CMap(unicode);
				if (glyph > 0)
				{
					FPointArray outline(font.glyphOutline(glyph));
					if (!fontName.isEmpty())
						Really[fontName].insert(glyph, outline);
				}
			}
		}
	}
}


void ScribusDoc::getUsedProfiles(ProfilesL& usedProfiles)
{
	PageItem* it = NULL;
	QStringList profileNames;
	int counter = 0;
	usedProfiles.clear();

	QList<PageItem*> allItems;
	profileNames.append(m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile);
	profileNames.append(m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile);
	if (profileNames.indexOf(m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile) < 0 )
		profileNames.append(m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile);
	if (profileNames.indexOf(m_docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile) < 0 )
		profileNames.append(m_docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile);
	if (profileNames.indexOf(m_docPrefsData.colorPrefs.DCMSset.DefaultMonitorProfile) < 0 )
		profileNames.append(m_docPrefsData.colorPrefs.DCMSset.DefaultMonitorProfile);
	if (profileNames.indexOf(m_docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile) < 0 )
		profileNames.append(m_docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile);

	if (profileNames.indexOf(m_docPrefsData.pdfPrefs.SolidProf) < 0)
		profileNames.append(m_docPrefsData.pdfPrefs.SolidProf);
	if (profileNames.indexOf(m_docPrefsData.pdfPrefs.ImageProf) < 0)
		profileNames.append(m_docPrefsData.pdfPrefs.ImageProf);
	if (profileNames.indexOf(m_docPrefsData.pdfPrefs.PrintProf) < 0)
		profileNames.append(m_docPrefsData.pdfPrefs.PrintProf);
	
	for (int lc = 0; lc < 2; ++lc)
	{
		if (lc == 0)
			counter = MasterItems.count();
		else if (lc == 1)
			counter = DocItems.count();
		for (int d = 0; d < counter; ++d)
		{
			if (lc == 0)
				it = MasterItems.at(d);
			else if (lc == 1)
				it = DocItems.at(d);
			if (it->isGroup())
				allItems = it->getAllChildren();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				if (it->IProfile.isEmpty() || profileNames.contains(it->IProfile))
					continue;
				profileNames.append(it->IProfile);
			}
			allItems.clear();
		}
	}
	for (QHash<int, PageItem*>::iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		PageItem *it = itf.value();
		if (it->isGroup())
			allItems = it->getAllChildren();
		else
			allItems.append(it);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			it = allItems.at(ii);
			if (it->IProfile.isEmpty() || profileNames.contains(it->IProfile))
				continue;
			profileNames.append(it->IProfile);
		}
		allItems.clear();
	}

	for (QStringList::Iterator pIter = profileNames.begin(); pIter != profileNames.end(); pIter++)
	{
		if (ScCore->InputProfiles.contains(*pIter))
			usedProfiles[*pIter] = ScCore->InputProfiles[*pIter];
		else if (ScCore->InputProfilesCMYK.contains(*pIter))
			usedProfiles[*pIter] = ScCore->InputProfilesCMYK[*pIter];
		else if (ScCore->PrinterProfiles.contains(*pIter))
			usedProfiles[*pIter] = ScCore->PrinterProfiles[*pIter];
	}
}

bool ScribusDoc::useAcroFormFields() const
{
	QList<PageItem*>  allItems;
	const QList<PageItem*>* itemLists[] = { &MasterItems, &DocItems };
	const PageItem* it = NULL;

	for (int i = 0; i < 2; ++i)
	{
		allItems = *(itemLists[i]);
		while (allItems.count() > 0)
		{
			it = allItems.takeFirst();
			if (it->isGroup() || it->isTable())
			{
				allItems += it->getChildren();
				continue;
			}
			if ((it->itemType() != PageItem::TextFrame) || !it->isAnnotation())
				continue;
			if (it->annotation().isAcroFormField())
				return true;
		}
	}

	allItems = FrameItems.values();
	while (allItems.count() > 0)
	{
		it = allItems.takeFirst();
		if (it->isGroup() || it->isTable())
		{
			allItems += it->getChildren();
			continue;
		}
		if ((it->itemType() != PageItem::TextFrame) || !it->isAnnotation())
			continue;
		if (it->annotation().isAcroFormField())
			return true;
	}

	QStringList patterns = getUsedPatterns();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		allItems = pa.items;
		while (allItems.count() > 0)
		{
			it = allItems.takeFirst();
			if (it->isGroup() || it->isTable())
			{
				allItems += it->getChildren();
				continue;
			}
			if ((it->itemType() != PageItem::TextFrame) || !it->isAnnotation())
				continue;
			if (it->annotation().isAcroFormField())
				return true;
		}
	}

	return false;
}

bool ScribusDoc::useAnnotations() const
{
	QList<PageItem*>  allItems;
	const QList<PageItem*>* itemLists[] = { &MasterItems, &DocItems };
	const PageItem* it = NULL;

	for (int i = 0; i < 2; ++i)
	{
		allItems = *(itemLists[i]);
		while (allItems.count() > 0)
		{
			it = allItems.takeFirst();
			if (it->isGroup() || it->isTable())
			{
				allItems += it->getChildren();
				continue;
			}
			if (it->itemType() != PageItem::TextFrame)
				continue;
			if (it->isAnnotation())
				return true;
		}
	}

	allItems = FrameItems.values();
	while (allItems.count() > 0)
	{
		it = allItems.takeFirst();
		if (it->isGroup() || it->isTable())
		{
			allItems += it->getChildren();
			continue;
		}
		if (it->itemType() != PageItem::TextFrame)
			continue;
		if (it->isAnnotation())
			return true;
	}

	QStringList patterns = getUsedPatterns();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		allItems = pa.items;
		while (allItems.count() > 0)
		{
			it = allItems.takeFirst();
			if (it->isGroup() || it->isTable())
			{
				allItems += it->getChildren();
				continue;
			}
			if (it->itemType() != PageItem::TextFrame)
				continue;
			if (it->isAnnotation())
				return true;
		}
	}

	return false;
}

void ScribusDoc::setUnitIndex(const int newIndex)
{
	m_docPrefsData.docSetupPrefs.docUnitIndex=newIndex;
	m_docUnitRatio = unitGetRatioFromIndex( newIndex );
}


int ScribusDoc::unitIndex() const
{
	return m_docPrefsData.docSetupPrefs.docUnitIndex;
}


double ScribusDoc::unitRatio() const
{
	return m_docUnitRatio;
}


bool ScribusDoc::applyMasterPage(const QString& pageName, const int pageNumber)
{
	if (!MasterNames.contains(pageName))
		return false;

	if (UndoManager::undoEnabled())
	{
		if (DocPages.at(pageNumber)->MPageNam != pageName)
		{
			SimpleState *ss = new SimpleState(Um::ApplyMasterPage, QString(Um::FromTo).arg(DocPages.at(pageNumber)->MPageNam).arg(pageName));
			ss->set("PAGE_NUMBER", pageNumber);
			ss->set("OLD_MASTERPAGE", DocPages.at(pageNumber)->MPageNam);
			ss->set("NEW_MASTERPAGE", pageName);
			m_undoManager->action(this, ss);
		}
	}
	ScPage* Ap = DocPages.at(pageNumber);
	Ap->MPageNam = pageName;
	const int MpNr = MasterNames[pageName];
	ScPage* Mp = MasterPages.at(MpNr);
	PageItem *currItem;
	Ap->FromMaster.clear();
	for (int itn = 0; itn < MasterItems.count(); ++itn)
	{
		currItem = MasterItems.at(itn);
		if (currItem->OwnPage == MpNr)
			Ap->FromMaster.append(currItem);
	}
	if (!isLoading())
	{
		// PV - apply auto guides from MP only if there are no auto guides
		// on original page
		if (Ap->guides.horizontalAutoCount() != 0 || Ap->guides.verticalAutoCount() != 0)
			Mp->guides.copy(&Ap->guides, GuideManagerCore::Standard);
		else
			Mp->guides.copy(&Ap->guides);

		Ap->initialMargins.setTop(Mp->Margins.top());
		Ap->initialMargins.setBottom(Mp->Margins.bottom());
		if (pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns == 1)
		{
			Ap->initialMargins.setLeft(Mp->Margins.left());
			Ap->initialMargins.setRight(Mp->Margins.right());
		}
		else
		{
			PageLocation pageLoc=locationOfPage(pageNumber);
			if (pageLoc==LeftPage) //Left hand page
			{
				if (Mp->LeftPg != 0)
				{
					Ap->initialMargins.setRight(Mp->initialMargins.right());
					Ap->initialMargins.setLeft(Mp->initialMargins.left());
				}
				else
				{
					Ap->initialMargins.setLeft(Mp->initialMargins.right());
					Ap->initialMargins.setRight(Mp->initialMargins.left());
				}
			}
			else if (pageLoc==RightPage) // Right hand page
			{
				if (Mp->LeftPg != 0)
				{
					Ap->initialMargins.setLeft(Mp->initialMargins.right());
					Ap->initialMargins.setRight(Mp->initialMargins.left());
				}
				else
				{
					Ap->initialMargins.setRight(Mp->initialMargins.right());
					Ap->initialMargins.setLeft(Mp->initialMargins.left());
				}
			}
			else //Middle pages
			{
				Ap->initialMargins.setLeft(Mp->initialMargins.left());
				Ap->initialMargins.setRight(Mp->initialMargins.right());
			}
		}

		//#8212: Apply page settings
		Ap->setInitialHeight(Mp->initialHeight());
		Ap->setInitialWidth(Mp->initialWidth());
		Ap->setHeight(Mp->height());
		Ap->setWidth(Mp->width());
		Ap->setOrientation(Mp->orientation());
		Ap->m_pageSize = Mp->m_pageSize;
	}
	//TODO make a return false if not possible to apply the master page
	if (!isLoading())
		changed();
	return true;
}


void ScribusDoc::restoreMasterPageApplying(SimpleState* ss, bool isUndo)
{
	int pageNumber = ss->getInt("PAGE_NUMBER");
	QString oldName = ss->get("OLD_MASTERPAGE");
	QString newName = ss->get("NEW_MASTERPAGE");
	if (isUndo)
		applyMasterPage(oldName, pageNumber);
	else
		applyMasterPage(newName, pageNumber);
	scMW()->pagePalette->rebuildPages();
}

void ScribusDoc::restoreMasterPageRenaming(SimpleState* ss, bool isUndo)
{
	QString oldName = ss->get("OLD_MASTERPAGE");
	QString newName = ss->get("NEW_MASTERPAGE");
	if (isUndo)
		renameMasterPage(newName, oldName);
	else
		renameMasterPage(oldName, newName);
	scMW()->pagePalette->updateMasterPageList();
}

void ScribusDoc::restoreCopyPage(SimpleState* ss, bool isUndo)
{
	int pnum = ss->getInt("PAGE_NUM");
	int extPage = ss->getInt("EXISTING_PAGE");
	int whereTo = ss->getInt("WHERE_TO");
	int copyCount = ss->getInt("COPY_COUNT");

	if (isUndo)
	{
		int destLocation=extPage + 1;
		if (whereTo==0)
			--destLocation;
		else if (whereTo==2)
			destLocation=DocPages.count();
		for (int i = 0; i < copyCount; ++i)
		{
			m_ScMW->deletePage(destLocation, destLocation);
			if (whereTo == 2)
				--destLocation;
		}
	}
	else
	{
		copyPage(pnum, extPage, whereTo, copyCount);
		if (m_ScMW->outlinePalette->isVisible())
			m_ScMW->outlinePalette->BuildTree();
	}

}


//TODO: Handle saving to versions of SLA, and other formats
bool ScribusDoc::save(const QString& fileName, QString* savedFile)
{
	QProgressBar* mainWindowProgressBar=NULL;
	if (ScCore->usingGUI())
	{
		mainWindowProgressBar=m_ScMW->mainWindowProgressBar;
		mainWindowProgressBar->reset();
	}
	FileLoader fl(fileName);
	bool ret = fl.saveFile(fileName, this, savedFile);
	if (ret)
	{
		setName(fileName);
		setModified(false);
		hasName = true;
		isConverted = false;
	}
	return ret;
}


bool ScribusDoc::changePageProperties(const double initialTop, const double initialBottom, const double initialLeft, const double initialRight, const double initialHeight, const double initialWidth, const double height, const double width, const int orientation, const QString& pageSize, const int marginPreset, const bool moveObjects, const int pageNumber, const int pageType)
{
	bool retVal=true;
	if (pageNumber==-1)
	{
		//find page and set values
		return false;
	}
	else
	{
		if (currentPage()==NULL)
			retVal=false;
		else
		{
			QRectF pagebox(currentPage()->xOffset(), currentPage()->yOffset(), qMax( currentPage()->width(), width), qMax(currentPage()->height(), height));
			if (UndoManager::undoEnabled())
			{
				SimpleState *ss = new SimpleState(Um::ChangePageProps);//, QString("%1").arg(pageNumber), Um::IPage);
				ss->set("PAGE_CHANGEPROPS");
				ss->set("PAGE_NUM", pageNumber);
				ss->set("OLD_PAGE_INITIALTOP", currentPage()->initialMargins.top());
				ss->set("OLD_PAGE_INITIALBOTTOM", currentPage()->initialMargins.bottom());
				ss->set("OLD_PAGE_INITIALLEFT", currentPage()->initialMargins.left());
				ss->set("OLD_PAGE_INITIALRIGHT", currentPage()->initialMargins.right());
				ss->set("OLD_PAGE_INITIALHEIGHT", currentPage()->initialHeight());
				ss->set("OLD_PAGE_INITIALWIDTH", currentPage()->initialWidth());
				ss->set("OLD_PAGE_HEIGHT", currentPage()->height());
				ss->set("OLD_PAGE_WIDTH", currentPage()->width());
				ss->set("OLD_PAGE_ORIENTATION", currentPage()->orientation());
				ss->set("OLD_PAGE_SIZE", currentPage()->m_pageSize);
				ss->set("OLD_PAGE_TYPE", currentPage()->LeftPg);
				ss->set("OLD_PAGE_MARGINPRESET", currentPage()->marginPreset);
				ss->set("OLD_PAGE_MOVEOBJECTS", moveObjects);
				ss->set("NEW_PAGE_INITIALTOP", initialTop);
				ss->set("NEW_PAGE_INITIALBOTTOM", initialBottom);
				ss->set("NEW_PAGE_INITIALLEFT", initialLeft);
				ss->set("NEW_PAGE_INITIALRIGHT", initialRight);
				ss->set("NEW_PAGE_INITIALHEIGHT", initialHeight);
				ss->set("NEW_PAGE_INITIALWIDTH", initialWidth);
				ss->set("NEW_PAGE_HEIGHT", height);
				ss->set("NEW_PAGE_WIDTH", width);
				ss->set("NEW_PAGE_ORIENTATION", orientation);
				ss->set("NEW_PAGE_SIZE", pageSize);
				ss->set("NEW_PAGE_TYPE", pageType);
				ss->set("NEW_PAGE_MARGINPRESET", marginPreset);
				ss->set("MASTER_PAGE_MODE", masterPageMode());
				m_undoManager->action(this, ss);
			}
			//set the current page's values
			currentPage()->initialMargins.setTop(initialTop);
			currentPage()->initialMargins.setBottom(initialBottom);
			currentPage()->initialMargins.setLeft(initialLeft);
			currentPage()->initialMargins.setRight(initialRight);
			currentPage()->setInitialHeight(initialHeight);
			currentPage()->setInitialWidth(initialWidth);
			currentPage()->setHeight(height);
			currentPage()->setWidth(width);
			currentPage()->setOrientation(orientation);
			currentPage()->m_pageSize = pageSize;
			currentPage()->LeftPg = pageType;
			currentPage()->marginPreset = marginPreset;
			reformPages(moveObjects);
			invalidateRegion(pagebox);
			regionsChanged()->update(pagebox);
			changed();
		}
	}
	return retVal;
}

void ScribusDoc::recalculateColorsList(QList<PageItem*> *itemList)
{
	QList<PageItem*> allItems;
	for (int c = 0; c < itemList->count(); ++c)
	{
		PageItem *ite = itemList->at(c);
		if (ite->isGroup())
			allItems = ite->getAllChildren();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			ite->setLineQColor();
			ite->setFillQColor();
			ite->set4ColorColors(ite->GrColorP1, ite->GrColorP2, ite->GrColorP3, ite->GrColorP4);
			for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
				{
					MeshPoint mp = ite->meshGradientArray[grow][gcol];
					ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
				}
			}
			for (int grow = 0; grow < ite->meshGradientPatches.count(); grow++)
			{
				meshGradientPatch patch = ite->meshGradientPatches[grow];
				ite->setMeshPointColor(grow, 1, patch.TL.colorName, patch.TL.shade, patch.TL.transparency, true);
				ite->setMeshPointColor(grow, 2, patch.TR.colorName, patch.TR.shade, patch.TR.transparency, true);
				ite->setMeshPointColor(grow, 3, patch.BR.colorName, patch.BR.shade, patch.BR.transparency, true);
				ite->setMeshPointColor(grow, 4, patch.BL.colorName, patch.BL.shade, patch.BL.transparency, true);
			}
			QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
				ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops = ite->stroke_gradient.colorStops();
			for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
				ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops = ite->mask_gradient.colorStops();
			for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
				ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
			if (ite->GrType == 13)
				ite->createConicalMesh();
		}
		allItems.clear();
	}
}

void ScribusDoc::recalculateColorItem(PageItem *item)
{
	QList<PageItem*> allItems;
	if (item->isGroup())
		allItems = item->getAllChildren();
	else
		allItems.append(item);
	for (int ii = 0; ii < allItems.count(); ii++)
	{
		PageItem *ite = allItems.at(ii);
		ite->setLineQColor();
		ite->setFillQColor();
		ite->set4ColorColors(ite->GrColorP1, ite->GrColorP2, ite->GrColorP3, ite->GrColorP4);
		for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
		{
			for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
			{
				MeshPoint mp = ite->meshGradientArray[grow][gcol];
				ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
			}
		}
		for (int grow = 0; grow < ite->meshGradientPatches.count(); grow++)
		{
			meshGradientPatch patch = ite->meshGradientPatches[grow];
			ite->setMeshPointColor(grow, 1, patch.TL.colorName, patch.TL.shade, patch.TL.transparency, true);
			ite->setMeshPointColor(grow, 2, patch.TR.colorName, patch.TR.shade, patch.TR.transparency, true);
			ite->setMeshPointColor(grow, 3, patch.BR.colorName, patch.BR.shade, patch.BR.transparency, true);
			ite->setMeshPointColor(grow, 4, patch.BL.colorName, patch.BL.shade, patch.BL.transparency, true);
		}
		QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
		cstops = ite->stroke_gradient.colorStops();
		for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
		cstops = ite->mask_gradient.colorStops();
		for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
			ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
		if (ite->GrType == 13)
			ite->createConicalMesh();
	}
	allItems.clear();
}

void ScribusDoc::recalculateColors()
{
	// #12658, #13889 : disable undo temporarily, there is nothing to cancel here
	m_undoManager->setUndoEnabled(false);

	//Adjust Items of the 3 types to the colors
	QHash<QString, VGradient>::Iterator itGrad;
	for (itGrad = docGradients.begin(); itGrad != docGradients.end(); ++itGrad)
	{
		QList<VColorStop*> cstops = itGrad.value().colorStops();
		for (uint cst = 0; cst < itGrad.value().Stops(); ++cst)
		{
			if (cstops.at(cst)->name != CommonStrings::None)
			{
				const ScColor& col = PageColors[cstops.at(cst)->name];
				QColor tmp = ScColorEngine::getShadeColorProof(col, this, cstops.at(cst)->shade);
				if (viewAsPreview)
				{
					VisionDefectColor defect;
					tmp = defect.convertDefect(tmp, previewVisual);
				}
				cstops.at(cst)->color = tmp;
			}
		}
	}

	recalculateColorsList(&DocItems);
	recalculateColorsList(&MasterItems);
	QList<PageItem*> itemList = FrameItems.values();
	recalculateColorsList(&itemList);
	QList<PageItem*> allItems;
	for (QHash<int, PageItem*>::iterator it = FrameItems.begin(); it != FrameItems.end(); ++it)
	{
		PageItem *ite = it.value();
		if (ite->isGroup())
			allItems = ite->getAllChildren();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			ite->setLineQColor();
			ite->setFillQColor();
			ite->set4ColorColors(ite->GrColorP1, ite->GrColorP2, ite->GrColorP3, ite->GrColorP4);
			for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
				{
					MeshPoint mp = ite->meshGradientArray[grow][gcol];
					ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
				}
			}
			for (int grow = 0; grow < ite->meshGradientPatches.count(); grow++)
			{
				meshGradientPatch patch = ite->meshGradientPatches[grow];
				ite->setMeshPointColor(grow, 1, patch.TL.colorName, patch.TL.shade, patch.TL.transparency, true);
				ite->setMeshPointColor(grow, 2, patch.TR.colorName, patch.TR.shade, patch.TR.transparency, true);
				ite->setMeshPointColor(grow, 3, patch.BR.colorName, patch.BR.shade, patch.BR.transparency, true);
				ite->setMeshPointColor(grow, 4, patch.BL.colorName, patch.BL.shade, patch.BL.transparency, true);
			}
			QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
				ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops = ite->stroke_gradient.colorStops();
			for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
				ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
			cstops = ite->mask_gradient.colorStops();
			for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
				ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
			if (ite->GrType == 13)
				ite->createConicalMesh();
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		if (pa.items.count() <= 0)
			continue;
		for (int o = 0; o < pa.items.count(); o++)
		{
			QList<PageItem*> allItems;
			PageItem *ite = pa.items.at(o);
			if (ite->isGroup())
				allItems = ite->getAllChildren();
			else
				allItems.append(ite);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				ite = allItems.at(ii);
				ite->setLineQColor();
				ite->setFillQColor();
				ite->set4ColorColors(ite->GrColorP1, ite->GrColorP2, ite->GrColorP3, ite->GrColorP4);
				for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
				{
					for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
					{
						MeshPoint mp = ite->meshGradientArray[grow][gcol];
						ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
					}
				}
				for (int grow = 0; grow < ite->meshGradientPatches.count(); grow++)
				{
					meshGradientPatch patch = ite->meshGradientPatches[grow];
					ite->setMeshPointColor(grow, 1, patch.TL.colorName, patch.TL.shade, patch.TL.transparency, true);
					ite->setMeshPointColor(grow, 2, patch.TR.colorName, patch.TR.shade, patch.TR.transparency, true);
					ite->setMeshPointColor(grow, 3, patch.BR.colorName, patch.BR.shade, patch.BR.transparency, true);
					ite->setMeshPointColor(grow, 4, patch.BL.colorName, patch.BL.shade, patch.BL.transparency, true);
				}
				QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
				for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
					ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
				cstops = ite->stroke_gradient.colorStops();
				for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
					ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
				cstops = ite->mask_gradient.colorStops();
				for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
					ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
				if (ite->asImageFrame())
					loadPict(ite->Pfile, ite, true, false);
				if (ite->GrType == 13)
					ite->createConicalMesh();
			}
			allItems.clear();
		}
		PageItem *ite = pa.items.at(0);
		double minx =  std::numeric_limits<double>::max();
		double miny =  std::numeric_limits<double>::max();
		double maxx = -std::numeric_limits<double>::max();
		double maxy = -std::numeric_limits<double>::max();
		double x1, x2, y1, y2;
		ite->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
	}

	m_undoManager->setUndoEnabled(true);
}

bool ScribusDoc::copyPageToMasterPage(const int pageNumber, const int leftPage, const int maxLeftPage,  const QString& masterPageName, bool copyFromAppliedMaster)
{
	assert(!masterPageMode());
	if (masterPageMode())
		return false;
	int GrMax = GroupCounter;
	ScPage* sourcePage = Pages->at(pageNumber);
	int nr = MasterPages.count();
	ScPage* targetPage=addMasterPage(nr, masterPageName);
	assert(targetPage!=NULL);
	//Backup currentpage, and don't use sourcepage here as we might convert a non current page
	ScPage* oldCurrentPage = currentPage();
	//Must set current page for pasteitem to work properly
	setLoading(true);
	targetPage->copySizingProperties(sourcePage, sourcePage->Margins);
	//Grab the left page setting for the current document layout from the dialog, and increment, singlePage==1 remember.
	if (m_docPrefsData.docSetupPrefs.pagePositioning != singlePage)
	{
		int lp = leftPage;
		if (lp == 0)
			lp = 1;
		else if (lp == maxLeftPage-1)
			lp = 0;
		else
			++lp;
		targetPage->LeftPg = lp;
	}
	sourcePage->guides.copy(&targetPage->guides);
	int end = DocItems.count();
	int end2 = MasterItems.count();
	Selection tempSelection(this, false);
	m_Selection->clear();
	//Copy the items from our current document page's applied *master* page
	if (copyFromAppliedMaster)
	{
		if (!sourcePage->MPageNam.isEmpty() && MasterNames.contains(sourcePage->MPageNam))
		{
			ScPage* pageMaster=NULL;
			for (int i=0; i < MasterPages.count(); ++i )
			{
				pageMaster=MasterPages[i];
				if (pageMaster->pageName() == sourcePage->MPageNam)
					break;
			}
			if (Layers.count()!= 0 && pageMaster!=NULL)
			{
				int currActiveLayer = activeLayer();
				for (ScLayers::iterator it = Layers.begin(); it != Layers.end(); ++it)
				{
					setActiveLayer(it->ID);
					for (int ite = 0; ite < end2; ++ite)
					{
						PageItem *itemToCopy = MasterItems.at(ite);
						if ((itemToCopy->OnMasterPage == pageMaster->pageName()) && (it->ID == itemToCopy->LayerID))
							tempSelection.addItem(itemToCopy, true);
					}
					if (tempSelection.count() != 0)
					{
						ScriXmlDoc ss;
						setMasterPageMode(true);
						QString dataS = ss.WriteElem(this, &tempSelection);
						setCurrentPage(targetPage);
						ss.ReadElemToLayer(dataS, m_appPrefsData.fontPrefs.AvailFonts, this, targetPage->xOffset(), targetPage->yOffset(), false, true, m_appPrefsData.fontPrefs.GFontSub, it->ID);
						setMasterPageMode(false);
					}
					tempSelection.clear();
				}
				setActiveLayer(currActiveLayer);
			}
		}
	}
	//Copy the items from our current *document* page
	if (Layers.count()!= 0)
	{
		int currActiveLayer = activeLayer();
		for (ScLayers::iterator it = Layers.begin(); it != Layers.end(); ++it)
		{
			setActiveLayer(it->ID);
			for (int ite = 0; ite < end; ++ite)
			{
				PageItem *itemToCopy = DocItems.at(ite);
				if ((itemToCopy->OwnPage == static_cast<int>(sourcePage->pageNr())) && (it->ID == itemToCopy->LayerID))
					tempSelection.addItem(itemToCopy, true);
			}
			if (tempSelection.count() != 0)
			{
				ScriXmlDoc ss;
				QString dataS = ss.WriteElem(this, &tempSelection);
				setMasterPageMode(true);
				setCurrentPage(targetPage);
				ss.ReadElemToLayer(dataS, m_appPrefsData.fontPrefs.AvailFonts, this, targetPage->xOffset(), targetPage->yOffset(), false, true, m_appPrefsData.fontPrefs.GFontSub, it->ID);
				setMasterPageMode(false);
			}
			tempSelection.clear();
		}
		setActiveLayer(currActiveLayer);
	}
	//Make sure our copied items have the master page name and own page set.
	int end3 = MasterItems.count();
	for (int a = end2; a < end3; ++a)
	{
		PageItem *newItem = MasterItems.at(a);
		newItem->setMasterPage(MasterNames[masterPageName], masterPageName);
	}
	targetPage->MPageNam = "";
	setLoading(false);
	GroupCounter = GrMax + 1;
	//Reset the current page..
	setMasterPageMode(false);
	setCurrentPage(oldCurrentPage);
	changed();
	return true;
}

PageItem* ScribusDoc::createPageItem(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, double x, double y, double b, double h, double w, const QString& fill, const QString& outline)
{
	PageItem* newItem=NULL;
	switch( itemType )
	{
		//Q_ASSERTs here will warn on creation issues when a coder specifies the frameType incorrectly
		//for items that do not have/need a frameType for creation.
		case PageItem::ImageFrame:
			newItem = new PageItem_ImageFrame(this, x, y, b, h, w, m_docPrefsData.itemToolPrefs.imageFillColor, m_docPrefsData.itemToolPrefs.imageStrokeColor);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::TextFrame:
			newItem = new PageItem_TextFrame(this, x, y, b, h, w, CommonStrings::None, outline);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::Line:
			{
				//CB 5521 Remove false minimum line width
				double lineWidth = w; // == 0.0 ? 1.0 : w;
				newItem = new PageItem_Line(this, x, y, b, h, lineWidth, CommonStrings::None, outline);
//				Q_ASSERT(frameType==PageItem::Unspecified);
			}
			break;
		case PageItem::Table:
			newItem = new PageItem_Table(this, x, y, b, h, w, fill, outline);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::Polygon:
			newItem = new PageItem_Polygon(this, x, y, b, h, w, fill, outline);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Ellipse || frameType==PageItem::Unspecified);
			break;
		case PageItem::PolyLine:
			newItem = new PageItem_PolyLine(this, x, y, b, h, w, fill, outline);
//			Q_ASSERT(frameType==PageItem::Unspecified);
			break;
		case PageItem::PathText:
			//Currently used only in fileloader
			newItem = new PageItem_PathText(this, x, y, b, h, w, fill, outline);
//			Q_ASSERT(frameType==PageItem::Unspecified);
			break;
		case PageItem::LatexFrame:
			newItem = new PageItem_LatexFrame(this, x, y, b, h, w, m_docPrefsData.itemToolPrefs.imageFillColor, m_docPrefsData.itemToolPrefs.imageStrokeColor);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
#ifdef HAVE_OSG
		case PageItem::OSGFrame:
			newItem = new PageItem_OSGFrame(this, x, y, b, h, w, m_docPrefsData.itemToolPrefs.imageFillColor, m_docPrefsData.itemToolPrefs.imageStrokeColor);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
#endif
		case PageItem::Symbol:
			newItem = new PageItem_Symbol(this, x, y, b, h, w, CommonStrings::None, CommonStrings::None);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::Group:
			newItem = new PageItem_Group(this, x, y, b, h, w, CommonStrings::None, CommonStrings::None);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Unspecified);
			break;
		case PageItem::RegularPolygon:
			newItem = new PageItem_RegularPolygon(this, x, y, b, h, w, fill, outline);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Ellipse || frameType==PageItem::Unspecified);
			break;
		case PageItem::Arc:
			newItem = new PageItem_Arc(this, x, y, b, h, w, fill, outline);
//			Q_ASSERT(frameType==PageItem::Rectangle || frameType==PageItem::Ellipse || frameType==PageItem::Unspecified);
			break;
		case PageItem::Spiral:
			newItem = new PageItem_Spiral(this, x, y, b, h, w, fill, outline);
//			Q_ASSERT(frameType==PageItem::Unspecified);
			break;
		default:
//			qDebug() << "unknown item type";
			assert (false);
	}
	if (newItem != NULL)
	{
		//Add in item default values based on itemType and frameType
		itemAddDetails(itemType, frameType, newItem);
	}
	return newItem;
}

int ScribusDoc::itemAdd(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const double x, const double y, const double b, const double h, const double w, const QString& fill, const QString& outline, PageItem::ItemKind itemKind)
{
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled()) // && !m_itemCreationTransaction)
	{
		activeTransaction = m_undoManager->beginTransaction();
	}
	
	PageItem* newItem;
	if (itemType == PageItem::NoteFrame)
	{
		newItem = new PageItem_NoteFrame(this, x, y, b, h, w, CommonStrings::None, outline);
		itemAddDetails(PageItem::TextFrame, frameType, newItem);
	}
	else
		newItem = createPageItem(itemType, frameType, x, y, b, h, w, fill, outline); 
	
	Q_CHECK_PTR(newItem);
	if (newItem==NULL)
		return -1;

	if (itemKind == PageItem::InlineItem || itemKind == PageItem::PatternItem)
	{
		newItem->OnMasterPage.clear();
		newItem->OwnPage = -1;
	}
	
	Items->append(newItem);
	if (UndoManager::undoEnabled())
	{
		ScItemState<PageItem*> *is = new ScItemState<PageItem*>("Create PageItem");
		is->set("CREATE_ITEM");
		is->setItem(newItem);
		//Undo target rests with the Page for object specific undo
		UndoObject *target = Pages->at(0);
		if (newItem->OwnPage > -1)
			target = Pages->at(newItem->OwnPage);
		m_undoManager->action(target, is);
		//If the item is created "complete" (ie, not being created by drag/resize, commit to undomanager)
		if (activeTransaction)
		{
			//don't think we need this now ... newItem->checkChanges(true);
			activeTransaction.commit(target->getUName(), newItem->getUPixmap(),
											  Um::Create + " " + newItem->getUName(),  "", Um::ICreate);
		}
	}
	return Items->count()-1;
}


int ScribusDoc::itemAddArea(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, const double x, const double y, const double w, const QString& fill, const QString& outline, PageItem::ItemKind itemKind)
{
	double xo = m_currentPage->xOffset();
	double yo = m_currentPage->yOffset();
	QPair<double, double> tl = m_currentPage->guides.topLeft(x - xo, y - yo);
	QPair<double, double> tr = m_currentPage->guides.topRight(x - xo, y - yo);
	QPair<double, double> bl = m_currentPage->guides.bottomLeft(x - xo, y - yo);
	return itemAdd(itemType, frameType, tl.first + xo, tl.second + yo, tr.first - tl.first, bl.second - tl.second, w, fill, outline, itemKind);
}


int ScribusDoc::itemAddUserFrame(InsertAFrameData &iafData)
{
	double x1=0.0,y1=0.0,w1=iafData.width,h1=iafData.height;
	std::vector<int> pageNs;
	if (iafData.locationType == 0) // On the current page or on a range of pages
		pageNs.push_back(currentPage()->pageNr()+1);
	else if (iafData.locationType==1) // On all pages
	{
		parsePagesString(QString("1-%1").arg(Pages->count()), &pageNs, Pages->count());
	}
	else
		parsePagesString(iafData.pageList, &pageNs, Pages->count());
	ScPage* oldCurrentPage = currentPage();
	int z=-2;
	PageItem *prevItem=0; //Previous item for text frame linking
	if (iafData.linkToExistingFrame && iafData.linkToExistingFramePtr!=NULL && 
		   iafData.linkToExistingFramePtr->itemType()==PageItem::TextFrame &&
		   DocItems.contains(iafData.linkToExistingFramePtr))
		prevItem=iafData.linkToExistingFramePtr;
	UndoTransaction transaction;
	if (UndoManager::undoEnabled())
		transaction = m_undoManager->beginTransaction(iafData.frameType==PageItem::TextFrame ? Um::TextFrame : Um::ImageFrame,
													iafData.frameType==PageItem::TextFrame ? Um::ITextFrame : Um::IImageFrame,
													Um::InsertFrame, "", Um::ICreate);
	for (uint i=0;i<pageNs.size();++i)
	{
		ScPage* targetPage = Pages->at(pageNs[i]-1);
		//We need this for the itemAdd, FIXME later
		setCurrentPage(targetPage);
		
		if (iafData.positionType==0) // Frame starts at top left of page margins
		{
			x1 = targetPage->xOffset() + targetPage->Margins.left();
			y1 = targetPage->yOffset() + targetPage->Margins.top();
		}
		else if (iafData.positionType==1) // Frame starts at top left of page
		{
			x1 = targetPage->xOffset();
			y1 = targetPage->yOffset();
		}
		else if (iafData.positionType==2) // Frame starts at top left of page - bleeds
		{
			MarginStruct values;
			getBleeds(targetPage, m_docPrefsData.docSetupPrefs.bleeds, values);
			x1 = targetPage->xOffset() - values.left();
			y1 = targetPage->yOffset() - values.top();
		}
		else if (iafData.positionType==3) // Frame starts at custom position
		{
			x1 = targetPage->xOffset() + iafData.x;
			y1 = targetPage->yOffset() + iafData.y;
		}
		
		if (iafData.sizeType==0) // Frame is size of page margins
		{
			w1 = targetPage->width() - targetPage->Margins.right()- targetPage->Margins.left();
			h1 = targetPage->height() - targetPage->Margins.bottom()- targetPage->Margins.top();
		}
		else if (iafData.sizeType==1) // Frame is size of page
		{
			w1 = targetPage->width();
			h1 = targetPage->height();
		}
		else if (iafData.sizeType==2) // Frame is size of page + bleed 
		{
			w1 = targetPage->width() + m_docPrefsData.docSetupPrefs.bleeds.right() + m_docPrefsData.docSetupPrefs.bleeds.left();
			h1 = targetPage->height() + m_docPrefsData.docSetupPrefs.bleeds.bottom() + m_docPrefsData.docSetupPrefs.bleeds.top();
		}
		else if (iafData.sizeType==3) //Frame is size of imported image, we resize below when we load it
		{
			w1 = h1 =1;
		}
		else if (iafData.sizeType==4) // Frame is custom size
		{
			w1 = iafData.width;
			h1 = iafData.height;
		}
		z=itemAdd(iafData.frameType, PageItem::Unspecified, x1, y1, w1, h1, m_docPrefsData.itemToolPrefs.shapeLineWidth, CommonStrings::None, m_docPrefsData.itemToolPrefs.textColor);
		if (z!=-1)
		{
			PageItem* currItem=Items->at(z);
			setRedrawBounding(currItem);
			if (iafData.frameType==PageItem::ImageFrame && !iafData.source.isEmpty())
			{
				if (QFile::exists(iafData.source))
				{
					PrefsManager::instance()->prefsFile->getContext("dirs")->set("images", iafData.source.left(iafData.source.lastIndexOf("/")));
					currItem->EmProfile = "";
					currItem->pixm.imgInfo.isRequest = false;
					currItem->UseEmbedded = true;
					currItem->IProfile = m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
					currItem->IRender = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
					qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
					qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
					loadPict(iafData.source, currItem, false, true);
					if (iafData.sizeType==3) //Frame is size of imported image
					{
						currItem->setWidth(static_cast<double>(currItem->OrigW * 72.0 / currItem->pixm.imgInfo.xres));
						currItem->setHeight(static_cast<double>(currItem->OrigH * 72.0 / currItem->pixm.imgInfo.yres));
						currItem->OldB2 = currItem->width();
						currItem->OldH2 = currItem->height();
						currItem->updateClip();
					}
					qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
					qApp->restoreOverrideCursor();
				}
			}
			if (iafData.frameType==PageItem::TextFrame)
			{
				currItem->setColumns(iafData.columnCount);
				currItem->setColumnGap(iafData.columnGap/m_docUnitRatio);
				if (i==0 && iafData.linkToExistingFrame && prevItem != NULL)
				{
					prevItem->link(currItem);
				}
				if (i!=0 && iafData.linkTextFrames && prevItem != NULL)
				{
					prevItem->link(currItem);
				}
				if (!iafData.source.isEmpty() && prevItem == NULL && QFile::exists(iafData.source))
				{
					gtGetText* gt = new gtGetText(this);
					if (iafData.impsetup.runDialog)
						gt->launchImporter(iafData.impsetup.importer, iafData.impsetup.filename, iafData.impsetup.textOnly, iafData.impsetup.encoding, true, iafData.impsetup.prefixNames, currItem);
					delete gt;
				}
				prevItem=currItem;
			}
		}
	}

	if (transaction)
		transaction.commit();
	setCurrentPage(oldCurrentPage);
	changed();
	regionsChanged()->update(QRectF());
	return z;
}


void ScribusDoc::itemAddDetails(const PageItem::ItemType itemType, const PageItem::ItemFrameType frameType, PageItem* newItem)
{
	ParagraphStyle defaultParagraphStyle;
	Q_ASSERT(newItem->realItemType()==itemType);
	switch( itemType )
	{
		case PageItem::ImageFrame:
			newItem->setImageXYScale(m_docPrefsData.itemToolPrefs.imageScaleX, m_docPrefsData.itemToolPrefs.imageScaleY);
			newItem->ScaleType = m_docPrefsData.itemToolPrefs.imageScaleType;
			newItem->AspectRatio = m_docPrefsData.itemToolPrefs.imageAspectRatio;
			newItem->IProfile = m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
			newItem->IRender = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
			newItem->setFillShade(m_docPrefsData.itemToolPrefs.imageFillColorShade);
			newItem->setLineShade(m_docPrefsData.itemToolPrefs.imageStrokeColorShade);
			break;
		case PageItem::LatexFrame:
#ifdef HAVE_OSG
		case PageItem::OSGFrame:
#endif
			newItem->setFillShade(m_docPrefsData.itemToolPrefs.imageFillColorShade);
			newItem->setLineShade(m_docPrefsData.itemToolPrefs.imageStrokeColorShade);
			break;
		case PageItem::NoteFrame:
		case PageItem::TextFrame:
			newItem->setFillColor(m_docPrefsData.itemToolPrefs.textFillColor);
			newItem->setFillShade(m_docPrefsData.itemToolPrefs.textFillColorShade);
			newItem->setLineColor(m_docPrefsData.itemToolPrefs.textLineColor);
			newItem->setLineShade(m_docPrefsData.itemToolPrefs.textLineColorShade);
			// 13792 : Commented out because this trigger the first line of a text frame having
			// default character style explictly assigned, which we don't want
			/*defaultParagraphStyle.setParent(CommonStrings::DefaultParagraphStyle);
			defaultParagraphStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
			newItem->itemText.setDefaultStyle(defaultParagraphStyle);*/
			break;
		case PageItem::Line:
			newItem->PLineArt = Qt::PenStyle(m_docPrefsData.itemToolPrefs.lineStyle);
			newItem->setLineShade(m_docPrefsData.itemToolPrefs.lineColorShade);
			break;
		case PageItem::Polygon:
			if (frameType!=PageItem::Rectangle && frameType!=PageItem::Ellipse)
			{
				newItem->ClipEdited = true;
				newItem->FrameType = 3;
			}
			break;
		case PageItem::PolyLine:
		case PageItem::Spiral:
			newItem->ClipEdited = true;
			break;
		case PageItem::PathText:
			break;
		case PageItem::Symbol:
		case PageItem::Group:
		case PageItem::RegularPolygon:
		case PageItem::Arc:
			newItem->ClipEdited = true;
			newItem->FrameType = 3;
		//At this point, we cannot create a PathText item like this, only by conversion, do nothing
			break;
		default:
			break;
	}

	if (frameType==PageItem::Rectangle || itemType==PageItem::TextFrame || itemType==PageItem::ImageFrame || itemType==PageItem::LatexFrame || itemType==PageItem::OSGFrame || itemType==PageItem::Symbol || itemType==PageItem::Group || itemType==PageItem::Table || itemType==PageItem::NoteFrame)
	{
		newItem->SetRectFrame();
		//TODO one day hopefully, if (ScCore->usingGUI())
		newItem->setRedrawBounding();
		newItem->ContourLine = newItem->PoLine.copy();
	}

	if (frameType==PageItem::Ellipse)
	{
		newItem->SetOvalFrame();
		//TODO one day hopefully, if (ScCore->usingGUI())
		newItem->setRedrawBounding();
		newItem->ContourLine = newItem->PoLine.copy();
	}

	//ItemType Polygon
	if (itemType==PageItem::Polygon || itemType==PageItem::PolyLine || itemType==PageItem::Spiral || itemType == PageItem::RegularPolygon || itemType == PageItem::Arc)
	{
		newItem->PLineArt = Qt::PenStyle(m_docPrefsData.itemToolPrefs.shapeLineStyle);
		newItem->setFillShade(m_docPrefsData.itemToolPrefs.shapeFillColorShade);
		newItem->setLineShade(m_docPrefsData.itemToolPrefs.shapeLineColorShade);
		if ((itemType == PageItem::Polygon) || (itemType == PageItem::RegularPolygon) || (itemType == PageItem::Arc))
			newItem->ContourLine = newItem->PoLine.copy();
	}
}


bool ScribusDoc::itemAddCommit(PageItem* item)
{
	//TODO use the parameter
	if (m_itemCreationTransaction.isStarted() && appMode !=  modeDrawBezierLine)
	{
		if (item != NULL)
		{
			item->checkChanges(true);
			QString targetName = Um::ScratchSpace;
			if (item->OwnPage > -1)
				targetName = Pages->at(item->OwnPage)->getUName();
			m_itemCreationTransaction.commit(targetName, item->getUPixmap(), Um::Create + " " + item->getUName(),  "", Um::ICreate);
			m_itemCreationTransaction.reset();
			if (!isLoading())
				return true;
		}
	}
	return false;
}


int ScribusDoc::getItemNrfromUniqueID(uint unique)
{
	int ret = 0;
	for (int i = 0; i < Items->count(); ++i)
	{
		if (Items->at(i)->uniqueNr == unique)
		{
			ret = Items->indexOf(Items->at(i));
			break;
		}
	}
	return ret;
}

PageItem* ScribusDoc::getItemFromName(QString name)
{
	PageItem* ret = NULL;
	for (int i = 0; i < Items->count(); ++i)
	{
		if (Items->at(i)->itemName() == name)
			return Items->at(i);
	}
	return ret;
}

void ScribusDoc::rebuildItemLists()
{
	// #5826 Rebuild items list in case layer order as been changed
	QList<PageItem*> newDocItems, newMasterItems;
	Layers.sort();
	int layerCount = this->layerCount();
	for (int la = 0; la < layerCount; ++la)
	{
		PageItem* currItem;
		int layerID = Layers.at(la).ID;
		for (int it = 0; it < DocItems.count(); ++it)
		{
			currItem = DocItems.at(it);
			if (currItem->LayerID != layerID)
				continue;
			newDocItems.append(currItem);
		}
		for (int it = 0; it < MasterItems.count(); ++it)
		{
			currItem = MasterItems.at(it);
			if (currItem->LayerID != layerID)
				continue;
			newMasterItems.append(currItem);
		}
	}
	DocItems    = newDocItems;
	MasterItems = newMasterItems;
}


bool ScribusDoc::usesAutomaticTextFrames() const
{
	return m_automaticTextFrames;
}

void ScribusDoc::setUsesAutomaticTextFrames(const bool atf)
{
	m_automaticTextFrames=atf;
}

bool ScribusDoc::loadPict(QString fn, PageItem *pageItem, bool reload, bool showMsg)
{
	if (!reload)
	{
		if ((ScCore->fileWatcher->files().contains(pageItem->Pfile) != 0) && (pageItem->imageIsAvailable))
		{
			ScCore->fileWatcher->removeFile(pageItem->Pfile);
			if (pageItem->isTempFile)
			{
				QFile::remove(pageItem->Pfile);
				pageItem->Pfile.clear();
			}
			pageItem->isInlineImage = false;
			pageItem->isTempFile = false;
		}
	}
	if (!pageItem->loadImage(fn, reload, -1, showMsg))
	{
		if (!reload)
		{
			if (m_hasGUI)
			{
				QFileInfo fi(pageItem->Pfile);
				ScCore->fileWatcher->addDir(fi.absolutePath());
			}
		}
		return false;
	}
	if (!reload)
	{
		if (m_hasGUI)
		{
			ScCore->fileWatcher->addFile(pageItem->Pfile);
		}
	}
	if (!isLoading())
	{
		pageItem->update();
		changed();
	}
	return true;
}


void ScribusDoc::canvasMinMax(FPoint& minPoint, FPoint& maxPoint)
{
	PageItem *currItem;
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	int docItemsCount=Items->count();
	if (docItemsCount != 0)
	{
		for (int ic = 0; ic < docItemsCount; ++ic)
		{
			currItem = Items->at(ic);
			if (currItem->rotation() != 0.0)
			{
				FPointArray pb;
				pb.resize(0);
				pb.addPoint(FPoint(currItem->xPos(), currItem->yPos()));
				FPoint p1(currItem->width(), 0.0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0);
				pb.addPoint(p1);
				FPoint p2(currItem->width(), currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0);
				pb.addPoint(p2);
				FPoint p3(0.0, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0);
				pb.addPoint(p3);
				for (uint pc = 0; pc < 4; ++pc)
				{
					minx = qMin(minx, pb.point(pc).x());
					miny = qMin(miny, pb.point(pc).y());
					maxx = qMax(maxx, pb.point(pc).x());
					maxy = qMax(maxy, pb.point(pc).y());
				}
			}
			else
			{
				minx = qMin(minx, currItem->xPos());
				miny = qMin(miny, currItem->yPos());
				maxx = qMax(maxx, currItem->xPos() + currItem->width());
				maxy = qMax(maxy, currItem->yPos() + currItem->height());
			}
		}
	}
	else
	{
		ScPage* Seite;
		int docPageCount=Pages->count();
		MarginStruct pageBleeds;
		for (int a = 0; a < docPageCount; ++a)
		{
			Seite = Pages->at(a);
			getBleeds(Seite, pageBleeds);
			minx = qMin(minx, Seite->xOffset() - pageBleeds.left());
			miny = qMin(miny, Seite->yOffset() - pageBleeds.top());
			maxx = qMax(maxx, Seite->xOffset() + Seite->width() + pageBleeds.left() + pageBleeds.right());
			maxy = qMax(maxy, Seite->yOffset() + Seite->height() + pageBleeds.top() + pageBleeds.bottom());
		}
	}
	minPoint.setX(minx);
	minPoint.setY(miny);
	maxPoint.setX(maxx);
	maxPoint.setY(maxy);
}


int ScribusDoc::OnPage(double x2, double  y2)
{
	int retw = -1;
	if (masterPageMode())
	{
		int x = static_cast<int>(currentPage()->xOffset() - m_docPrefsData.docSetupPrefs.bleeds.left());
		int y = static_cast<int>(currentPage()->yOffset() - m_docPrefsData.docSetupPrefs.bleeds.top());
		int w = static_cast<int>(currentPage()->width() + m_docPrefsData.docSetupPrefs.bleeds.left() + m_docPrefsData.docSetupPrefs.bleeds.right());
		int h = static_cast<int>(currentPage()->height() + m_docPrefsData.docSetupPrefs.bleeds.bottom() + m_docPrefsData.docSetupPrefs.bleeds.top());
		if (QRect(x, y, w, h).contains(qRound(x2), qRound(y2)))
			retw = currentPage()->pageNr();
	}
	else
	{
		int docPageCount = Pages->count();
		MarginStruct pageBleeds;
		for (int a = 0; a < docPageCount; ++a)
		{
			getBleeds(a, pageBleeds);
			int x = static_cast<int>(Pages->at(a)->xOffset() - pageBleeds.left());
			int y = static_cast<int>(Pages->at(a)->yOffset() - pageBleeds.top());
			int w = static_cast<int>(Pages->at(a)->width() + pageBleeds.left() + pageBleeds.right());
			int h = static_cast<int>(Pages->at(a)->height() + pageBleeds.bottom() + pageBleeds.top());
			if (QRect(x, y, w, h).contains(qRound(x2), qRound(y2)))
			{
				retw = static_cast<int>(a);
				break;
			}
		}
	}
	return retw;
}


int ScribusDoc::OnPage(PageItem *currItem)
{
	int retw = -1;

	double w2 = currItem->visualWidth();
	double h2 = currItem->visualHeight();
	double x2 = -currItem->visualLineWidth() / 2.0;
	double y2 = -currItem->visualLineWidth() / 2.0;
	QTransform t = currItem->getTransform();
	QRectF itemRect = t.mapRect(QRectF(x2, y2, w2, h2));

	if (masterPageMode())
	{
		ScPage* currPage = currentPage();
		double x1 = currPage->xOffset() - m_docPrefsData.docSetupPrefs.bleeds.left();
		double y1 = currPage->yOffset() - m_docPrefsData.docSetupPrefs.bleeds.top();
		double w1 = currPage->width() + m_docPrefsData.docSetupPrefs.bleeds.left() + m_docPrefsData.docSetupPrefs.bleeds.right();
		double h1 = currPage->height() + m_docPrefsData.docSetupPrefs.bleeds.bottom() + m_docPrefsData.docSetupPrefs.bleeds.top();
		QRectF pageRect(x1, y1, w1, h1);
		if (itemRect.intersects(pageRect))
			retw = currPage->pageNr();
	}
	else
	{
		MarginStruct pageBleeds;
		int docPageCount = Pages->count();
		for (int a = 0; a < docPageCount; ++a)
		{
			getBleeds(a, pageBleeds);
			double x1 = Pages->at(a)->xOffset() - pageBleeds.left();
			double y1 = Pages->at(a)->yOffset() - pageBleeds.top();
			double w1 = Pages->at(a)->width() + pageBleeds.left() + pageBleeds.right();
			double h1 = Pages->at(a)->height() + pageBleeds.bottom() + pageBleeds.top();
			QRectF pageRect(x1, y1, w1, h1);
			if (itemRect.intersects(pageRect))
			{
				retw = static_cast<int>(a);
				break;
			}
		}
	}
	if ((retw == -1) && (currItem->isBookmark))
	{
		//FIXME emit DelBM(currItem);
		currItem->isBookmark = false;
	}
	return retw;
}


void ScribusDoc::GroupOnPage(PageItem* currItem)
{
	if (!currItem->isGroup())
		return;
	QList<PageItem*> Objects = currItem->getAllChildren();
	int Off_Page = -1;
	int On_Page = 999999;
	uint objectCount = Objects.count();
	for (uint a = 0; a < objectCount; ++a)
	{
		int res = OnPage(Objects.at(a));
		Off_Page = qMax(Off_Page, res);
		if (res != -1)
			On_Page = qMin(On_Page, res);
	}
	int final = -1;
	if (Off_Page != -1)
		final = On_Page;
	for (uint a = 0; a < objectCount; ++a)
	{
		Objects.at(a)->OwnPage = final;
	}
	currItem->OwnPage = OnPage(currItem);
}

void  ScribusDoc::fixItemPageOwner()
{
	int pageNr;
	ScPage* page;
	PageItem* currItem;
	MarginStruct pageBleeds;

	for (int i = 0; i < Items->count(); ++i)
	{
		currItem = Items->at(i);
		pageNr = currItem->OwnPage;

		// TODO check group owner
		if (currItem->getAllChildren().count() > 0)
			continue;

		// If item has a valid page, check that
		// specified page effectively contain the item
		if (pageNr >= 0 && pageNr < Pages->count())
		{
			page = Pages->at(pageNr);
			getBleeds(page, pageBleeds);
			double x1 = page->xOffset() - pageBleeds.left();
			double y1 = page->yOffset() - pageBleeds.top();
			double w1 = page->width()   + pageBleeds.left() + pageBleeds.right();
			double h1 = page->height()  + pageBleeds.bottom() + pageBleeds.top();
			QTransform t = currItem->getTransform();
			double w2 = currItem->visualWidth();
			double h2 = currItem->visualHeight();
			double x2 = -currItem->visualLineWidth() / 2.0;
			double y2 = -currItem->visualLineWidth() / 2.0;
			
			QRectF pageRect(x1, y1, w1, h1);
			QRectF itemRect = t.mapRect(QRectF(x2, y2, w2, h2));
			if (itemRect.intersects(pageRect))
				continue;
		}

		// If no or page owner is incorrect, recompute page owner
		currItem->setOwnerPage(OnPage(currItem));
 	}

	// #10379: Scribus crash when opening .sla document
	// OwnPage is not meaningful for inline frame
	for (QHash<int, PageItem*>::iterator it = FrameItems.begin(); it != FrameItems.end(); ++it)
	{
		currItem = it.value();
		currItem->OwnPage = -1;
 	}

	// #11274: Scribus crash when opening .sla document
	// OwnPage is not meaningful for pattern items
	QHash<QString, ScPattern>::iterator patternIt;
	for (patternIt = docPatterns.begin(); patternIt != docPatterns.end(); ++patternIt)
	{
		QList<PageItem*> patternItems = patternIt->items;
		while (patternItems.count() > 0)
		{
			PageItem* patItem = patternItems.takeAt(0);
			if (patItem->isGroup())
				patternItems += patItem->groupItemList;
			patItem->OwnPage = -1;
		}
	}
}

void ScribusDoc::fixCharacterStyles()
{
	for (int i = 0; i < m_docCharStyles.count(); ++i)
	{
		CharStyle& charStyle = m_docCharStyles[i];
		QString parentName = charStyle.parent();
		if (parentName.isEmpty())
			continue;
		if (!m_docCharStyles.contains(parentName))
			charStyle.setParent(QString());
	}
}

void ScribusDoc::fixParagraphStyles()
{
	for (int i = 0; i < m_docParagraphStyles.count(); ++i)
	{
		ParagraphStyle& parStyle = m_docParagraphStyles[i];
		QString parentName = parStyle.parent();
		if (!parentName.isEmpty())
		{
			if (!m_docParagraphStyles.contains(parentName))
				parStyle.setParent(QString());
		}
		QString charStyleName = parStyle.charStyle().parent();
		if (!charStyleName.isEmpty())
		{
			if (!m_docCharStyles.contains(charStyleName))
				parStyle.charStyle().setParent(QString());
		}
		QString peStyleName = parStyle.peCharStyleName();
		if (peStyleName.isEmpty())
			continue;
		if (!m_docCharStyles.contains(peStyleName))
			parStyle.resetPeCharStyleName();
	}
}

void ScribusDoc::fixNotesStyles()
{
	for (int i = 0; i < m_docNotesStylesList.count(); ++i)
	{
		NotesStyle* noteStyle = m_docNotesStylesList[i];
		QString markChStyle = noteStyle->marksChStyle();
		if (!markChStyle.isEmpty() && !m_docCharStyles.contains(markChStyle))
			noteStyle->setMarksCharStyle(QString());
		QString noteParStyle = noteStyle->notesParStyle();
		if (!noteParStyle.isEmpty() && !m_docParagraphStyles.contains(noteParStyle))
			noteStyle->setNotesParStyle(QString());
	}
}

struct oldPageVar
{
	uint newPg;
	double oldXO;
	double oldYO;
};


//CB TODO make a function to determine the place of the page.. ie, so we know the left and right margins
// without running this monster
void ScribusDoc::reformPages(bool moveObjects)
{
	QMap<uint, oldPageVar> pageTable;
	struct oldPageVar oldPg;
	int counter = pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].FirstPage;
	int rowcounter = 0;
	double maxYPos=0.0, maxXPos=0.0;
	double currentXPos=m_docPrefsData.displayPrefs.scratch.left(), currentYPos=m_docPrefsData.displayPrefs.scratch.top(), lastYPos=Pages->at(0)->initialHeight();
//	currentXPos += (pageWidth+pageSets[currentPageLayout].GapHorizontal) * counter;
	currentXPos += (m_docPrefsData.docSetupPrefs.pageWidth+m_docPrefsData.displayPrefs.pageGapHorizontal) * counter;

	lastYPos = Pages->at(0)->initialHeight();
	ScPage* page;
	int docPageCount=Pages->count();
	for (int i = 0; i < docPageCount; ++i)
	{
		page = Pages->at(i);
		oldPg.oldXO = page->xOffset();
		oldPg.oldYO = page->yOffset();
		oldPg.newPg = i;
		pageTable.insert(page->pageNr(), oldPg);
		page->setPageNr(i);
		if (masterPageMode())
		{
			page->setXOffset(m_docPrefsData.displayPrefs.scratch.left());
			page->setYOffset(m_docPrefsData.displayPrefs.scratch.top());
			if (page->LeftPg == 0)
			{
				page->Margins.setRight(page->initialMargins.right());
				page->Margins.setLeft(page->initialMargins.left());
			}
			else if ((page->LeftPg > 1) && (page->LeftPg < pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns))
			{
				page->Margins.setLeft(page->initialMargins.left());
				page->Margins.setRight(page->initialMargins.left());
			}
			else
			{
				page->Margins.setLeft(page->initialMargins.right());
				page->Margins.setRight(page->initialMargins.left());
			}
		}
		else
		{
			page->setWidth(page->initialWidth());
			page->setHeight(page->initialHeight());
			page->setXOffset(currentXPos);
			page->setYOffset(currentYPos);
			if (counter < pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns-1)
			{
				currentXPos += page->width() + m_docPrefsData.displayPrefs.pageGapHorizontal;
				lastYPos = qMax(lastYPos, page->height());
				if (counter == 0)
				{
					page->Margins.setLeft(page->initialMargins.right());
					page->Margins.setRight(page->initialMargins.left());
				}
				else
				{
					page->Margins.setLeft(page->initialMargins.left());
					page->Margins.setRight(page->initialMargins.left());
				}
			}
			else
			{
				currentXPos = m_docPrefsData.displayPrefs.scratch.left();
				if (pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns > 1)
					currentYPos += qMax(lastYPos, page->height())+m_docPrefsData.displayPrefs.pageGapVertical;
				else
					currentYPos += page->height()+m_docPrefsData.displayPrefs.pageGapVertical;
				lastYPos = 0;
				page->Margins.setRight(page->initialMargins.right());
				page->Margins.setLeft(page->initialMargins.left());
			}
			counter++;
			if (counter > pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns-1)
			{
				counter = 0;
				rowcounter++;
				if (rowcounter > pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Rows-1)
					rowcounter = 0;
			}
		}
		page->Margins.setTop(page->initialMargins.top());
		page->Margins.setBottom(page->initialMargins.bottom());
		maxXPos = qMax(maxXPos, page->xOffset()+page->width()+m_docPrefsData.displayPrefs.scratch.right());
		maxYPos = qMax(maxYPos, page->yOffset()+page->height()+m_docPrefsData.displayPrefs.scratch.bottom());
	}
	if (!isLoading())
	{
		m_undoManager->setUndoEnabled(false);
		this->beginUpdate();
		int docItemsCount = Items->count();
		for (int ite = 0; ite < docItemsCount; ++ite)
		{
			PageItem *item = Items->at(ite);
			if (item->OwnPage < 0)
			{
				if (item->isGroup())
					GroupOnPage(item);
				else
					item->OwnPage = OnPage(item);
			}
			else if (moveObjects)
			{
				oldPg = pageTable[item->OwnPage];
				item->moveBy(-oldPg.oldXO + Pages->at(oldPg.newPg)->xOffset(), -oldPg.oldYO + Pages->at(oldPg.newPg)->yOffset());
				item->OwnPage = static_cast<int>(oldPg.newPg);
				if (item->isGroup())
				{
					QList<PageItem*> groupItems = item->groupItemList;
					while (groupItems.count() > 0)
					{
						PageItem* groupItem = groupItems.takeAt(0);
						if (groupItem->isGroup())
							groupItems += groupItem->groupItemList;
						if (groupItem->OwnPage < 0)
							continue;
						oldPg = pageTable[groupItem->OwnPage];
						groupItem->OwnPage = static_cast<int>(oldPg.newPg);
					}
				}
			}
			else
			{
				if (item->isGroup())
					GroupOnPage(item);
				else
					item->OwnPage = OnPage(item);
			}
			item->setRedrawBounding();
		}
		this->endUpdate();
		m_undoManager->setUndoEnabled(true);
	}

	if (isLoading() && is12doc)
		return;
	if (!isLoading())
	{
		updateMarks(true);
		FPoint minPoint, maxPoint;
		canvasMinMax(minPoint, maxPoint);
		FPoint maxSize(qMax(maxXPos, maxPoint.x()+m_docPrefsData.displayPrefs.scratch.right()), qMax(maxYPos, maxPoint.y()+m_docPrefsData.displayPrefs.scratch.bottom()));
		adjustCanvas(FPoint(qMin(0.0, minPoint.x()-m_docPrefsData.displayPrefs.scratch.left()),qMin(0.0, minPoint.y()-m_docPrefsData.displayPrefs.scratch.top())), maxSize, true);
		changed();
	}
	else
	{
		FPoint maxSize(maxXPos, maxYPos);
		adjustCanvas(FPoint(0, 0), maxSize);
	}
}

void ScribusDoc::refreshGuides()
{
	for (int i = 0; i < Pages->count(); ++i)
	{
		ScPage* page = Pages->at(i);
		page->guides.addHorizontals(page->guides.getAutoHorizontals(page), GuideManagerCore::Auto);
		page->guides.addVerticals(page->guides.getAutoVerticals(page), GuideManagerCore::Auto);
	}
}

double ScribusDoc::getXOffsetForPage(const int pageNumber)
{
	if (Pages->at(pageNumber)!=NULL)
		return Pages->at(pageNumber)->xOffset();
	return -1.0;
}

double ScribusDoc::getYOffsetForPage(const int pageNumber)
{
	if (Pages->at(pageNumber)!=NULL)
		return Pages->at(pageNumber)->yOffset();
	return -1.0;
}

void ScribusDoc::getBleeds(int pageNumber, MarginStruct &bleedData)
{
	if (pageNumber >= 0 && pageNumber < Pages->size())
		getBleeds(Pages->at(pageNumber), bleedData);
	else
		qCritical() << "Attempting to get bleeds for non-existant page";
}

void ScribusDoc::getBleeds(const ScPage* page, MarginStruct& bleedData)
{
	getBleeds(page, m_docPrefsData.docSetupPrefs.bleeds, bleedData);
}

void ScribusDoc::getBleeds(const ScPage* page, const MarginStruct& baseValues, MarginStruct& bleedData)
{
	bleedData.setBottom(baseValues.bottom());
	bleedData.setTop(baseValues.top());
	if (pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns == 1)
	{
		bleedData.setRight(baseValues.right());
		bleedData.setLeft(baseValues.left());
	}
	else
	{
		PageLocation pageLocation = MiddlePage;
		if (page->pageName().isEmpty()) // Standard page
			pageLocation = locationOfPage(page->pageNr());
		else if (page->LeftPg == 1) // Left Master page
			pageLocation = LeftPage;
		else if (page->LeftPg == 0) // Right Master page
			pageLocation = RightPage;
		else // Middle Master page
			pageLocation = MiddlePage;

		if (pageLocation == LeftPage)
		{
			bleedData.setRight(baseValues.left());
			bleedData.setLeft(baseValues.right());
		}
		else if (pageLocation == RightPage)
		{
			bleedData.setRight(baseValues.right());
			bleedData.setLeft(baseValues.left());
		}
		else
		{
			bleedData.setRight(baseValues.left());
			bleedData.setLeft(baseValues.left());
		}
	}
}


PageItem* ScribusDoc::convertItemTo(PageItem *currItem, PageItem::ItemType newType, PageItem* secondaryItem)
{
	//Item to convert is null, return
	Q_ASSERT(currItem!=NULL);
	if (currItem == NULL)
		return NULL;
	//Don't attempt a Line conversion
	if (newType==PageItem::Line)
		return NULL;
	PageItem *oldItem = currItem;
	uint oldItemNr;
	if (currItem->isGroupChild())
		oldItemNr = currItem->Parent->asGroupFrame()->groupItemList.indexOf(currItem);
	else
		oldItemNr = Items->indexOf(currItem);
	//Remove old item from the doc's selection if it was in it
	bool removedFromSelection=m_Selection->removeItem(oldItem);
	//Create a new item from the old one
	UndoTransaction transactionConversion;
	PageItem *newItem;
	switch (newType)
	{
		case PageItem::ImageFrame:
			newItem = new PageItem_ImageFrame(*oldItem);
			break;
		case PageItem::TextFrame:
			newItem = new PageItem_TextFrame(*oldItem);
			if (UndoManager::undoEnabled() && oldItem->itemType()==PageItem::PathText)
				transactionConversion = m_undoManager->beginTransaction(currentPage()->getUName(), 0, Um::TextFrame, "", Um::ITextFrame);
			break;
		//We don't allow this
/*		case PageItem::Line:
			newItem = new PageItem_PolyLine(*oldItem);
			break; */
		case PageItem::Polygon:
			newItem = new PageItem_Polygon(*oldItem);
			break;
		case PageItem::PolyLine:
			newItem = new PageItem_PolyLine(*oldItem);
			break;
		case PageItem::PathText:
			if (secondaryItem==NULL)
				return NULL;
			if (UndoManager::undoEnabled())
				transactionConversion = m_undoManager->beginTransaction(currentPage()->getUName(), 0, Um::PathText, "", Um::ITextFrame);
			newItem = new PageItem_PathText(*oldItem);
			break;
		default:
			newItem=NULL;
			break;
	}
	Q_ASSERT(newItem!=NULL);
	//If the new item is null, return. If converting Text to Path, start a transaction
	//as the old bezier will be deleted
	if (newItem==NULL)
	{
		if (transactionConversion)
		{
			transactionConversion.cancel();
			transactionConversion.reset();
		}
		return NULL;
	}
	//Do new item type specific adjustments to the new item. Some of this may move when new
	//constructors are built into the item classes
	switch (newType)
	{
		case PageItem::ImageFrame:
			newItem->convertTo(PageItem::ImageFrame);
			break;
		case PageItem::TextFrame:
			newItem->convertTo(PageItem::TextFrame);
			if (oldItem->itemType()==PageItem::PathText)
			{
				uint newPolyItemNo = itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), CommonStrings::None, currItem->lineColor());
				PageItem *polyLineItem = Items->at(newPolyItemNo);
				polyLineItem->PoLine = currItem->PoLine.copy();
				polyLineItem->ClipEdited = true;
				polyLineItem->FrameType = 3;
				polyLineItem->setRotation(currItem->rotation());
				adjustItemSize(polyLineItem);

				newItem->setLineColor(CommonStrings::None);
				newItem->SetRectFrame();
				newItem->setRedrawBounding();
				newItem->setTextToFrameDistLeft(0);
			}
			break;
		//We don't allow this right now
	/*	case PageItem::Line:
			break; */
		case PageItem::Polygon:
			newItem->convertTo(PageItem::Polygon);
			newItem->ClipEdited = true;
			newItem->FrameType = 3;
			if (oldItem->itemType()==PageItem::PolyLine)
			{
				newItem->PoLine.addPoint(newItem->PoLine.point(newItem->PoLine.size()-2));
				newItem->PoLine.addPoint(newItem->PoLine.point(newItem->PoLine.size()-3));
				newItem->PoLine.addPoint(newItem->PoLine.point(0));
				newItem->PoLine.addPoint(newItem->PoLine.point(0));
			}
			newItem->Clip = FlattenPath(newItem->PoLine, newItem->Segments);
			newItem->ContourLine = newItem->PoLine.copy();
			break;
		case PageItem::PolyLine:
			newItem->convertTo(PageItem::PolyLine);
			newItem->ClipEdited = true;
			newItem->FrameType = 3;
			if (oldItem->itemType()==PageItem::Line)
			{
				QTransform ma;
				newItem->FrameType = 3;
				ma.rotate(newItem->rotation());
				newItem->PoLine.resize(0);
				newItem->PoLine.addPoint(0.0, 0.0);
				newItem->PoLine.addPoint(0.0, 0.0);
				newItem->PoLine.addPoint(newItem->width(), 0.0);
				newItem->PoLine.addPoint(newItem->width(), 0.0);
				newItem->PoLine.map(ma);
				newItem->setRotation(0.0);
			}
			adjustItemSize(newItem);
			break;
		case PageItem::PathText:
			{
				newItem->convertTo(PageItem::PathText);
				newItem->ClipEdited = true;
				newItem->PoLine = secondaryItem->PoLine.copy();
				newItem->setLineWidth(secondaryItem->lineWidth());
				newItem->setLineColor(secondaryItem->lineColor());
				newItem->PLineArt = secondaryItem->PLineArt;
				newItem->PLineEnd = secondaryItem->PLineEnd;
				newItem->PLineJoin = secondaryItem->PLineJoin;
				//FIXME: Stop using the view here
				adjustItemSize(newItem);
				double dx = secondaryItem->xPos() - newItem->xPos();
				double dy = secondaryItem->yPos() - newItem->yPos();
				moveItem(dx, dy, newItem);
				newItem->setRotation(secondaryItem->rotation());
				newItem->FrameType = 3;
			}
			break;
		default:
			newItem=NULL;
			break;
	}
	newItem->uniqueNr = oldItem->uniqueNr;
	if (oldItem->isGroupChild())
	{
		oldItem->Parent->asGroupFrame()->groupItemList.replace(oldItemNr, newItem);
		newItem->Parent = oldItem->Parent;
	}
	else
		Items->replace(oldItemNr, newItem);
	//FIXME: shouldn't we delete the oldItem ???
	//Add new item back to selection if old item was in selection
	if (removedFromSelection)
		m_Selection->addItem(newItem);
	// If converting a text frame to another object, drop links
	if (oldItem->asTextFrame() && (newType != PageItem::TextFrame))
		oldItem->dropLinks();
	// If converting text to path, delete the bezier
	if (newType == PageItem::PathText)
	{
		//FIXME: Stop using the view here
		m_View->SelectItem(secondaryItem);
		itemSelection_DeleteItem();
		regionsChanged()->update(QRectF());
		m_View->Deselect(true);
	}
	//Create the undo action for the new item
	if (UndoManager::undoEnabled())
	{
		ScItemState<QPair<PageItem*, PageItem*> > *is = new ScItemState<QPair<PageItem*, PageItem*> >("Convert Item");
		is->set("CONVERT_ITEM");
		is->setItem(qMakePair(oldItem, newItem));
		//Undo target rests with the Page for object specific undo
		UndoObject *target = Pages->at(0);
		if (newItem->OwnPage > -1)
			target = Pages->at(newItem->OwnPage);
		m_undoManager->action(target, is);
	}
	//Close any undo transaction
	if (transactionConversion)
		transactionConversion.commit();
	return newItem;
}

int ScribusDoc::currentPageNumber()
{
	return currentPage()->pageNr();
}


bool ScribusDoc::itemNameExists(const QString checkItemName)
{
	bool found = false;
	QList<PageItem*> allItems;
	int docItemCount = Items->count();
	for (int i = 0; i < docItemCount; ++i)
	{
		PageItem *currItem = Items->at(i);
		if (checkItemName == currItem->itemName())
			return true;
		if (currItem->isGroup())
		{
			allItems = currItem->getAllChildren();
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				if (checkItemName == allItems.at(ii)->itemName())
					return true;
			}
		}
		allItems.clear();
	}
	return found;
}


void ScribusDoc::setMasterPageMode(bool changeToMasterPageMode)
{
	if (changeToMasterPageMode == m_masterPageMode)
		return;
	if (changeToMasterPageMode)
	{
		Pages = &MasterPages;
		Items = &MasterItems;
	}
	else
	{
		Pages = &DocPages;
		Items = &DocItems;
	}
	m_masterPageMode = changeToMasterPageMode;
}

void ScribusDoc::setSymbolEditMode(bool mode, QString symbolName)
{
	if (mode == m_symbolEditMode)
		return;
	m_symbolEditMode = mode;
	if (mode)
	{
		m_storedLayerID = activeLayer();
		int layerID = firstLayerID();
		m_storedLayerLock = layerLocked(layerID);
		m_storedLayerVis = layerVisible(layerID);
		setActiveLayer(layerID);
		setLayerVisible(layerID, true);
		setLayerLocked(layerID, false);
		ScPattern pa = docPatterns[symbolName];
		m_currentEditedSymbol = symbolName;
		ScPage* addedPage = new ScPage(m_docPrefsData.displayPrefs.scratch.left(), m_docPrefsData.displayPrefs.scratch.top(), pa.width, pa.height);
		addedPage->setDocument(this);
		addedPage->Margins.set(0, 0, 0, 0);
		addedPage->initialMargins.set(0, 0, 0, 0);
		addedPage->setPageNr(0);
		addedPage->MPageNam = "";
		addedPage->setPageName("");
		TempPages.clear();
		TempPages.append(addedPage);
		Pages = &TempPages;
		Items = &docPatterns[symbolName].items;
		m_Selection->delaySignalsOn();
		for (int as = 0; as < Items->count(); ++as)
		{
			m_Selection->addItem(Items->at(as));
			Items->at(as)->setLayer(layerID);
		}
		QRectF selRect = m_Selection->getVisualGroupRect();
		moveGroup(addedPage->xOffset() - selRect.x(), addedPage->yOffset() - selRect.y());
		if (Items->at(0)->isGroup())
			Items->at(0)->asGroupFrame()->adjustXYPosition();
		m_Selection->clear();
		m_Selection->delaySignalsOff();
		m_ScMW->changeLayer(layerID);
		changed();
	}
	else
	{
		ScPage* addedPage = TempPages.at(0);
		if (Items->count() == 0)
		{
			removePattern(m_currentEditedSymbol);
		}
		else
		{
			PageItem* currItem = Items->at(0);
			if (Items->count() > 1)
			{
				if ((!currItem->isGroup()) && (Items->count() > 1))
				{
					itemAdd(PageItem::Group, PageItem::Rectangle, addedPage->xOffset(), addedPage->yOffset(), 10, 10, 0, CommonStrings::None, CommonStrings::None);
					PageItem *groupItem = Items->takeLast();
					groupItem->setLayer(firstLayerID());
					Items->insert(0, groupItem);
					double minx =  std::numeric_limits<double>::max();
					double miny =  std::numeric_limits<double>::max();
					double maxx = -std::numeric_limits<double>::max();
					double maxy = -std::numeric_limits<double>::max();
					for (int as = 1; as < Items->count(); ++as)
					{
						PageItem* currItem = Items->at(as);
						groupItem->groupItemList.append(currItem);
						double x1, x2, y1, y2;
						currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
						minx = qMin(minx, x1);
						miny = qMin(miny, y1);
						maxx = qMax(maxx, x2);
						maxy = qMax(maxy, y2);
					}
					Items->clear();
					Items->append(groupItem);
					for (int em = 0; em < groupItem->groupItemList.count(); ++em)
					{
						PageItem* currItem = groupItem->groupItemList.at(em);
						currItem->gXpos = currItem->xPos() - minx;
						currItem->gYpos = currItem->yPos() - miny;
						currItem->gWidth = maxx - minx;
						currItem->gHeight = maxy - miny;
					}
					groupItem->setXYPos(minx, miny, true);
					groupItem->setWidthHeight(maxx - minx, maxy - miny, true);
					groupItem->groupWidth = maxx - minx;
					groupItem->groupHeight = maxy - miny;
					groupItem->gWidth = maxx - minx;
					groupItem->gHeight = maxy - miny;
					groupItem->SetRectFrame();
					groupItem->ClipEdited = true;
					groupItem->FrameType = 3;
					groupItem->setTextFlowMode(PageItem::TextFlowDisabled);
					groupItem->AutoName = false;
					groupItem->setFillTransparency(0);
					groupItem->setLineTransparency(0);
					groupItem->asGroupFrame()->adjustXYPosition();
					GroupCounter++;
				}
			}
			currItem = Items->at(0);
			double minx =  std::numeric_limits<double>::max();
			double miny =  std::numeric_limits<double>::max();
			double maxx = -std::numeric_limits<double>::max();
			double maxy = -std::numeric_limits<double>::max();
			double x1, x2, y1, y2;
			currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
			minx = qMin(minx, x1);
			miny = qMin(miny, y1);
			maxx = qMax(maxx, x2);
			maxy = qMax(maxy, y2);
			currItem->gXpos = currItem->xPos() - minx;
			currItem->gYpos = currItem->yPos() - miny;
			currItem->setXYPos(currItem->gXpos, currItem->gYpos, true);
			ScPattern& currentEditedSymbol = docPatterns[m_currentEditedSymbol];
			currentEditedSymbol.pattern = currItem->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
			currentEditedSymbol.width  = maxx - minx;
			currentEditedSymbol.height = maxy - miny;
		}
		if (m_ScMW->patternsDependingOnThis.count() > 1)
		{
			for (int a = 1; a < m_ScMW->patternsDependingOnThis.count(); a++)
			{
				Items = &docPatterns[m_ScMW->patternsDependingOnThis[a]].items;
				PageItem *currItem = Items->at(0);
				double x1, x2, y1, y2;
				currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
				docPatterns[m_ScMW->patternsDependingOnThis[a]].pattern = currItem->DrawObj_toImage(qMin(qMax(x2 - x1, y2 - y1), 500.0));
			}
		}
		if (masterPageMode())
		{
			Pages = &MasterPages;
			Items = &MasterItems;
		}
		else
		{
			Pages = &DocPages;
			Items = &DocItems;
		}
		delete addedPage;
		setActiveLayer(m_storedLayerID);
		setLayerVisible(m_storedLayerID, m_storedLayerVis);
		setLayerLocked(m_storedLayerID, m_storedLayerLock);
		m_ScMW->changeLayer(m_storedLayerID);
	}
}

void ScribusDoc::setInlineEditMode(bool mode, int id)
{
	if (mode == m_inlineEditMode)
		return;
	m_inlineEditMode = mode;
	if (mode)
	{
		m_storedLayerID = activeLayer();
		int layerID = firstLayerID();
		m_storedLayerLock = layerLocked(layerID);
		m_storedLayerVis = layerVisible(layerID);
		setActiveLayer(layerID);
		setLayerVisible(layerID, true);
		setLayerLocked(layerID, false);
		PageItem *pa = FrameItems[id];
		pa->isEmbedded = false;
		m_currentEditedIFrame = id;
		QRectF bBox = pa->getVisualBoundingRect();
		ScPage* addedPage = new ScPage(m_docPrefsData.displayPrefs.scratch.left(), m_docPrefsData.displayPrefs.scratch.top(), bBox.width(), bBox.height());
		addedPage->setDocument(this);
		addedPage->Margins.set(0, 0, 0, 0);
		addedPage->initialMargins.set(0, 0, 0, 0);
		addedPage->setPageNr(0);
		addedPage->MPageNam = "";
		addedPage->setPageName("");
		TempPages.clear();
		TempPages.append(addedPage);
		Pages = &TempPages;
		EditFrameItems.clear();
		EditFrameItems.append(pa);
		Items = &EditFrameItems;
		m_Selection->delaySignalsOn();
		for (int as = 0; as < Items->count(); ++as)
		{
			m_Selection->addItem(Items->at(as));
			Items->at(as)->setLayer(layerID);
		}
		moveGroup(addedPage->xOffset(), addedPage->yOffset());
		if (Items->at(0)->isGroup())
			Items->at(0)->asGroupFrame()->adjustXYPosition();
		m_Selection->clear();
		m_Selection->delaySignalsOff();
		m_ScMW->changeLayer(layerID);
		changed();
	}
	else
	{
		ScPage* addedPage = TempPages.at(0);
		if (Items->count() == 0)
		{
			removeInlineFrame(m_currentEditedIFrame);
		}
		else
		{
			PageItem* currItem = Items->at(0);
			if (Items->count() > 1)
			{
				if ((!currItem->isGroup()) && (Items->count() > 1))
				{
					itemAdd(PageItem::Group, PageItem::Rectangle, addedPage->xOffset(), addedPage->yOffset(), 10, 10, 0, CommonStrings::None, CommonStrings::None);
					PageItem *groupItem = Items->takeLast();
					groupItem->setLayer(firstLayerID());
					Items->insert(0, groupItem);
					double minx =  std::numeric_limits<double>::max();
					double miny =  std::numeric_limits<double>::max();
					double maxx = -std::numeric_limits<double>::max();
					double maxy = -std::numeric_limits<double>::max();
					for (int as = 1; as < Items->count(); ++as)
					{
						PageItem* currItem = Items->at(as);
						groupItem->groupItemList.append(currItem);
						double x1, x2, y1, y2;
						currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
						minx = qMin(minx, x1);
						miny = qMin(miny, y1);
						maxx = qMax(maxx, x2);
						maxy = qMax(maxy, y2);
					}
					Items->clear();
					Items->append(groupItem);
					for (int em = 0; em < groupItem->groupItemList.count(); ++em)
					{
						PageItem* currItem = groupItem->groupItemList.at(em);
						currItem->gXpos = currItem->xPos() - minx;
						currItem->gYpos = currItem->yPos() - miny;
						currItem->gWidth = maxx - minx;
						currItem->gHeight = maxy - miny;
					}
					groupItem->setXYPos(minx, miny, true);
					groupItem->setWidthHeight(maxx - minx, maxy - miny, true);
					groupItem->groupWidth = maxx - minx;
					groupItem->groupHeight = maxy - miny;
					groupItem->gWidth = maxx - minx;
					groupItem->gHeight = maxy - miny;
					groupItem->SetRectFrame();
					groupItem->ClipEdited = true;
					groupItem->FrameType = 3;
					groupItem->setTextFlowMode(PageItem::TextFlowDisabled);
					groupItem->AutoName = false;
					groupItem->setFillTransparency(0);
					groupItem->setLineTransparency(0);
					groupItem->asGroupFrame()->adjustXYPosition();
					GroupCounter++;
				}
			}
			currItem = Items->at(0);
			double minx =  std::numeric_limits<double>::max();
			double miny =  std::numeric_limits<double>::max();
			double maxx = -std::numeric_limits<double>::max();
			double maxy = -std::numeric_limits<double>::max();
			double x1, x2, y1, y2;
			currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
			minx = qMin(minx, x1);
			miny = qMin(miny, y1);
			maxx = qMax(maxx, x2);
			maxy = qMax(maxy, y2);
			currItem->gXpos = currItem->xPos() - minx;
			currItem->gYpos = currItem->yPos() - miny;
			currItem->gWidth = maxx - minx;
			currItem->gHeight = maxy - miny;
			currItem->setXYPos(currItem->gXpos, currItem->gYpos, true);
			currItem->isEmbedded = true;
			currItem->inlineCharID = m_currentEditedIFrame;
			FrameItems[m_currentEditedIFrame] = currItem;
		}
		if (masterPageMode())
		{
			Pages = &MasterPages;
			Items = &MasterItems;
		}
		else
		{
			Pages = &DocPages;
			Items = &DocItems;
		}
		delete addedPage;
		EditFrameItems.clear();
		setActiveLayer(m_storedLayerID);
		setLayerVisible(m_storedLayerID, m_storedLayerVis);
		setLayerLocked(m_storedLayerID, m_storedLayerLock);
		m_ScMW->changeLayer(m_storedLayerID);
	}
}

void ScribusDoc::addSection(const int number, const QString& name, const uint fromindex, const uint toindex, const NumFormat type, const uint sectionstartindex, const bool reversed, const bool active, const QChar fillChar, int fieldWidth)
{
	struct DocumentSection newSection;
	int docPageCount=DocPages.count();
	bool empty=m_docPrefsData.docSectionMap.isEmpty();
	if (empty)
	{
		newSection.number=0;
		newSection.name="0";
		newSection.fromindex=0;
		newSection.toindex=docPageCount-1;
		newSection.type=Type_1_2_3;
		newSection.sectionstartindex=1;
		newSection.reversed=false;
		newSection.active=true;
		newSection.pageNumberFillChar=QChar();
		newSection.pageNumberWidth=0;
		m_docPrefsData.docSectionMap.insert(newSection.number, newSection);
	}
	else if (number!=-1)
	{
		newSection.number=number;
		newSection.name=name;
		newSection.fromindex=fromindex;
		newSection.toindex=toindex;
		if (newSection.toindex > (uint) docPageCount - 1)
			newSection.toindex = docPageCount - 1;
		newSection.type=type;
		newSection.sectionstartindex=sectionstartindex;
		newSection.reversed=reversed;
		newSection.active=active;
		newSection.pageNumberFillChar=fillChar;
		newSection.pageNumberWidth=fieldWidth;
		m_docPrefsData.docSectionMap.insert(newSection.number, newSection);
	}
}


bool ScribusDoc::deleteSection(const uint number)
{
	if (!m_docPrefsData.docSectionMap.contains(number))
		return false;
	if (m_docPrefsData.docSectionMap.count()<=1)
		return false;
	QMap<uint, DocumentSection>::Iterator itprev=m_docPrefsData.docSectionMap.begin();
	QMap<uint, DocumentSection>::Iterator it=itprev;
	uint currMaxIndex = itprev.value().toindex;
	for ( ; it != m_docPrefsData.docSectionMap.end(); ++it)
	{
		currMaxIndex=it.value().toindex;

		if (it.key()!=number)
			itprev=it;
		else
			break;
	}
	if (it != itprev)
		itprev.value().toindex=currMaxIndex;
	else {
		// special case: delete first section
		QMap<uint, DocumentSection>::Iterator itnext = it;
		++itnext;
		itnext.value().fromindex=it.value().fromindex;
	}
	m_docPrefsData.docSectionMap.erase(it);
	return true;
}


int ScribusDoc::getSectionKeyForPageIndex(const uint pageIndex) const
{
	int retVal = -1;
	DocumentSectionMap::ConstIterator it = m_docPrefsData.docSectionMap.begin();
	for (; it != m_docPrefsData.docSectionMap.end(); ++it)
	{
		if (pageIndex >= it.value().fromindex && pageIndex <= it.value().toindex)
		{
			retVal = it.key();
			break;
		}
	}
	return retVal;
}

QString ScribusDoc::getSectionNameForPageIndex(const uint pageIndex) const
{
	DocumentSectionMap::ConstIterator it = m_docPrefsData.docSectionMap.begin();
	for (; it != m_docPrefsData.docSectionMap.end(); ++it)
	{
		if (pageIndex >= it.value().fromindex && pageIndex <= it.value().toindex)
			return it.value().name;
	}
	return QString();
}


const QString ScribusDoc::getSectionPageNumberForPageIndex(const uint pageIndex) const
{
	QString retVal;
	int key=getSectionKeyForPageIndex(pageIndex);
	if (key==-1)
		return retVal;
	//If a section is inactive, theres no page numbers printed
	if (m_docPrefsData.docSectionMap[key].active==false)
		return "";
	uint sectionIndexOffset;
	if (!m_docPrefsData.docSectionMap[key].reversed)
		sectionIndexOffset = pageIndex - m_docPrefsData.docSectionMap[key].fromindex + m_docPrefsData.docSectionMap[key].sectionstartindex;
	else
		sectionIndexOffset = -pageIndex + m_docPrefsData.docSectionMap[key].toindex  + m_docPrefsData.docSectionMap[key].sectionstartindex;
	retVal=getStringFromSequence(m_docPrefsData.docSectionMap[key].type, sectionIndexOffset);
	return retVal;
}

const QChar ScribusDoc::getSectionPageNumberFillCharForPageIndex(const uint pageIndex) const
{
	QChar retVal;
	int key=getSectionKeyForPageIndex(pageIndex);
	if (key==-1)
		return retVal;

	//If a section is inactive, theres no page numbers printed
	if (m_docPrefsData.docSectionMap[key].active==false)
		return retVal;
	retVal = m_docPrefsData.docSectionMap[key].pageNumberFillChar;
	if (retVal == QChar(0))
		retVal = QChar(32);
	return retVal;
}

int ScribusDoc::getSectionPageNumberWidthForPageIndex(const uint pageIndex) const
{
	int retVal=0;
	int key=getSectionKeyForPageIndex(pageIndex);
	if (key==-1)
		return retVal;

	//If a section is inactive, theres no page numbers printed
	if (m_docPrefsData.docSectionMap[key].active==false)
		return retVal;
	retVal = qMin(m_docPrefsData.docSectionMap[key].pageNumberWidth, 20);			// added the qmin as a sanity check -> fixes bug #9721
	return retVal;
}

void ScribusDoc::updateSectionPageNumbersToPages()
{
	int docPageCount=DocPages.count();
	for (int i=0; i < docPageCount; ++i)
		DocPages.at(i)->setPageSectionNumber(getSectionPageNumberForPageIndex(i));
}


void ScribusDoc::addPageToSection(const uint otherPageIndex, const uint location, const uint count)
{
	uint fromIndex, toIndex;
	uint searchedIndex = (otherPageIndex > 0) ? (otherPageIndex - 1) : 0;
	if ((location == 0) && (searchedIndex > 0))
		--searchedIndex;
	DocumentSectionMap::Iterator it = m_docPrefsData.docSectionMap.begin();
	for (; it!= m_docPrefsData.docSectionMap.end(); ++it)
	{
		fromIndex = it.value().fromindex;
		toIndex   = it.value().toindex;
		if  (fromIndex > searchedIndex)
			it.value().fromindex += count;
		if  (toIndex >= searchedIndex)
			it.value().toindex += count;
	}
	//Now update the Pages' internal storage of their page number
	updateSectionPageNumbersToPages();
}


void ScribusDoc::removePageFromSection(const uint pageIndex)
{
	//Get the section of the new page index.
	uint fromIndex, toIndex;
	DocumentSectionMap::Iterator it = m_docPrefsData.docSectionMap.begin();
	for (; it!= m_docPrefsData.docSectionMap.end(); ++it)
	{
		if (pageIndex>=it.value().fromindex && pageIndex<=it.value().toindex)
		{
			fromIndex = it.value().fromindex;
			toIndex   = it.value().toindex - 1;
			if (fromIndex > toIndex) // Remove section in that case
				m_docPrefsData.docSectionMap.remove(it.key());
			break;
		}
	}
	for (it = m_docPrefsData.docSectionMap.begin(); it != m_docPrefsData.docSectionMap.end(); ++it)
	{
		fromIndex = it.value().fromindex;
		toIndex   = it.value().toindex;
		if  (fromIndex > pageIndex)
			--it.value().fromindex;
		if  (toIndex >= pageIndex)
			--it.value().toindex;
	}
	//Now update the Pages' internal storage of their page number
	updateSectionPageNumbersToPages();
}


void ScribusDoc::setFirstSectionFromFirstPageNumber()
{
	DocumentSectionMap::Iterator it = m_docPrefsData.docSectionMap.begin();
	it.value().sectionstartindex=FirstPnum;
	updateSectionPageNumbersToPages();
}


void ScribusDoc::copyPage(int pageNumberToCopy, int existingPage, int whereToInsert, int copyCount)
{
	UndoTransaction copyTransaction;
	if (UndoManager::undoEnabled())
	{
		copyTransaction = m_undoManager->beginTransaction(getUName(), Um::IDocument, Um::CopyPage, "", Um::ICreate);
		SimpleState *ss = new SimpleState(Um::Copy, "", Um::ICreate);
		ss->set("PAGE_COPY");
		ss->set("PAGE_NUM", pageNumberToCopy);
		ss->set("EXISTING_PAGE", existingPage);
		ss->set("WHERE_TO", whereToInsert);
		ss->set("COPY_COUNT", copyCount);
		m_undoManager->action(this, ss);
	}

	m_undoManager->setUndoEnabled(false);
	//CB Should we really be disabling auto text frames here?
	bool autoText = usesAutomaticTextFrames();
	setUsesAutomaticTextFrames(false);
	ScPage* from = DocPages.at(pageNumberToCopy);
	ScPage* lastDest = NULL;
	setCurrentPage(from);

	uint oldItems = Items->count();
	QList<QString> itemBuffer;
	Selection tempSelection(this, false);
	m_Selection->clear();
	tempSelection.delaySignalsOn();
	if (oldItems>0)
	{
		if (Layers.count()!= 0)
		{
			int currActiveLayer = activeLayer();
			for (ScLayers::iterator it = Layers.begin(); it != Layers.end(); ++it)
			{
				setActiveLayer(it->ID);
				for (uint ite = 0; ite < oldItems; ++ite)
				{
					PageItem *itemToCopy = Items->at(ite);
					if ((itemToCopy->OwnPage == static_cast<int>(from->pageNr())) && (it->ID == itemToCopy->LayerID))
						tempSelection.addItem(itemToCopy, true);
				}
				if (tempSelection.count() != 0)
				{
					ScriXmlDoc ss;
					QString dataS = ss.WriteElem(this, &tempSelection);
					itemBuffer.append(dataS);
				}
				else
					itemBuffer.append(QString());
				tempSelection.clear();
			}
			setActiveLayer(currActiveLayer);
		}
	}
	tempSelection.delaySignalsOff();


	for (int copyNumber=1; copyNumber<=copyCount; ++copyNumber)
	{
		//For multiple insertions we can insert in the same place
		int destLocation=existingPage;
		if (whereToInsert==0)
			--destLocation;
		else if (whereToInsert==2)
			destLocation=DocPages.count();
		ScPage* destination = new ScPage(m_docPrefsData.displayPrefs.scratch.left(), DocPages.count()*(m_docPrefsData.docSetupPrefs.pageHeight+m_docPrefsData.displayPrefs.scratch.bottom()+m_docPrefsData.displayPrefs.scratch.top())+m_docPrefsData.displayPrefs.scratch.top(), m_docPrefsData.docSetupPrefs.pageWidth, m_docPrefsData.docSetupPrefs.pageHeight);
		destination->setDocument(this);
		destination->setPageNr(destLocation);
		lastDest = destination;
		DocPages.insert(destLocation, destination);
		setLocationBasedPageLRMargins(destLocation);
		applyMasterPage(from->MPageNam, destLocation);
		destination->setInitialHeight(from->height());
		destination->setInitialWidth(from->width());
		destination->setOrientation(from->orientation());
		destination->m_pageSize = from->m_pageSize;
		//CB: Can possibly partially use the code from applyMasterPage here instead of runnin all of this again..
		//TODO make a function to do this margin stuff and use elsewhere too
		destination->initialMargins.setTop(from->initialMargins.top());
		destination->initialMargins.setBottom(from->initialMargins.bottom());
		if (pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns == 1)
		{
			destination->initialMargins.setLeft(from->initialMargins.left());
			destination->initialMargins.setRight(from->initialMargins.right());
		}
		else
		{
			if (locationOfPage(destination->pageNr()) != locationOfPage(from->pageNr()))
			{
				if (locationOfPage(destination->pageNr()) == MiddlePage)
				{
					destination->initialMargins.setRight(from->initialMargins.left());
					destination->initialMargins.setLeft(from->initialMargins.left());
				}
				else
				{
					destination->initialMargins.setRight(from->initialMargins.left());
					destination->initialMargins.setLeft(from->initialMargins.right());
				}
			}
			else
			{
				destination->initialMargins.setLeft(from->initialMargins.left());
				destination->initialMargins.setRight(from->initialMargins.right());
			}
		}
		reformPages();
		// FIXME: stop using m_View
		if (m_View)
			m_View->reformPagesView();
		if (itemBuffer.count() > 0)
		{
			int lcount = 0;
			ScLayers::iterator it;
			if (Layers.count()!= 0)
			{
				int currActiveLayer = activeLayer();
				bool savedAlignGrid   = this->SnapGrid;
				bool savedAlignGuides = this->SnapGuides;
				bool savedAlignElement = this->SnapElement;
				this->SnapGrid   = false;
				this->SnapGuides = false;
				this->SnapElement = false;
				for (it = Layers.begin(); it != Layers.end(); ++it)
				{
					if ((lcount < itemBuffer.count()) && !itemBuffer[lcount].isEmpty())
					{
						ScriXmlDoc ss;
						QString fragment = itemBuffer[lcount];
						ss.ReadElemToLayer(fragment, m_appPrefsData.fontPrefs.AvailFonts, this, destination->xOffset(), destination->yOffset(), false, true, m_appPrefsData.fontPrefs.GFontSub, it->ID);
					}
					lcount++;
				}
				this->SnapGrid   = savedAlignGrid;
				this->SnapGuides = savedAlignGuides;
				this->SnapElement = savedAlignElement;
				setActiveLayer(currActiveLayer);
			}
		}
		from->guides.copy(&destination->guides);
	}
	setUsesAutomaticTextFrames(autoText);
	addPageToSection(existingPage, whereToInsert, copyCount);
	if (lastDest != NULL)
		setCurrentPage(lastDest);
	else
		setCurrentPage(from);
	changed();
	m_undoManager->setUndoEnabled(true);
	if (copyTransaction)
		copyTransaction.commit();
}


void ScribusDoc::setLocationBasedPageLRMargins(const uint pageIndex)
{
	int setcol=pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns;
	if (setcol==1)
	{
		ScPage* pageToAdjust=DocPages.at(pageIndex);
		pageToAdjust->Margins.setLeft(pageToAdjust->initialMargins.left());
		pageToAdjust->Margins.setRight(pageToAdjust->initialMargins.right());
		return;
	}

	ScPage* pageToAdjust=DocPages.at(pageIndex);
	PageLocation pageLoc=locationOfPage(pageIndex);
	if (pageLoc==LeftPage) //Left hand page
	{
		pageToAdjust->Margins.setLeft(pageToAdjust->initialMargins.right());
		pageToAdjust->Margins.setRight(pageToAdjust->initialMargins.left());
	}
	else if (pageLoc==RightPage) // Right hand page
	{
		pageToAdjust->Margins.setRight(pageToAdjust->initialMargins.right());
		pageToAdjust->Margins.setLeft(pageToAdjust->initialMargins.left());
	}
	else //Middle pages
	{
		pageToAdjust->Margins.setLeft(pageToAdjust->initialMargins.left());
		pageToAdjust->Margins.setRight(pageToAdjust->initialMargins.left());
	}
}


PageLocation ScribusDoc::locationOfPage(int pageIndex) const
{
	int myCol=columnOfPage(pageIndex);
	if (myCol==0) //Left hand page
		return LeftPage;
	else if (myCol>= pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns-1) // Right hand page
		return RightPage;
	else //Middle pages
		return MiddlePage;
}

int ScribusDoc::columnOfPage(int pageIndex) const
{
	int setcol=pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns;
	return ((pageIndex % setcol) + pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].FirstPage) % setcol;
}

void ScribusDoc::RecalcPictures(ProfilesL *Pr, ProfilesL *PrCMYK, QProgressBar *dia)
{
	RecalcPictures(&MasterItems, Pr, PrCMYK, dia);
	RecalcPictures(&DocItems, Pr, PrCMYK, dia);
	QList<PageItem*> itemList = FrameItems.values();
	RecalcPictures(&itemList, Pr, PrCMYK, dia);
	QList<PageItem*> allItems;
	if (FrameItems.isEmpty())
		return;
	bool usingGUI=ScCore->usingGUI();
	int counter = 0;
	if (usingGUI && dia != NULL)
		counter = dia->value();
	for (QHash<int, PageItem*>::iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		PageItem *it = itf.value();
		if (it->isGroup())
			allItems = it->getAllChildren();
		else
			allItems.append(it);
		for (int i = 0; i < allItems.count(); i++)
		{
			it = allItems.at(i);
			if ((it->itemType() == PageItem::ImageFrame) && (it->imageIsAvailable))
			{
				if (it->pixm.imgInfo.colorspace == ColorSpaceCMYK)
				{
					if (!PrCMYK->contains(it->IProfile))
						it->IProfile = m_docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile;
				}
				else
				{
					if (!Pr->contains(it->IProfile))
						it->IProfile = m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
				}
				loadPict(it->Pfile, it, true);
			}
		}
		allItems.clear();
		if (usingGUI)
		{
			++counter;
			if (dia != NULL)
				dia->setValue(counter);
		}
	}
}

void ScribusDoc::RecalcPictures(QList<PageItem*>* items, ProfilesL *Pr, ProfilesL *PrCMYK, QProgressBar *dia)
{
	if (items->isEmpty())
		return;
	QList<PageItem*> allItems;
	bool usingGUI=ScCore->usingGUI();
	int counter = 0;
	if (usingGUI && dia != NULL)
		counter = dia->value();
	PageItem* it;
	int docItemCount = items->count();
	for (int i=0; i < docItemCount; ++i)
	{
		it = items->at(i);
		if (it->isGroup())
			allItems = it->getAllChildren();
		else
			allItems.append(it);
		for (int j = 0; j < allItems.count(); j++)
		{
			it = allItems.at(j);
			if ((it->itemType() == PageItem::ImageFrame) && (it->imageIsAvailable))
			{
				if (it->pixm.imgInfo.colorspace == ColorSpaceCMYK)
				{
					if (!PrCMYK->contains(it->IProfile))
						it->IProfile = m_docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile;
				}
				else
				{
					if (!Pr->contains(it->IProfile))
						it->IProfile = m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
				}
				loadPict(it->Pfile, it, true);
			}
		}
		allItems.clear();
		if (usingGUI)
		{
			++counter;
			if (dia != NULL)
				dia->setValue(counter);
		}
	}
}



void ScribusDoc::insertColor(QString nam, double c, double m, double y, double k)
{
	if (PageColors.contains(nam))
		return;
	ScColor tmp = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
	PageColors.insert(nam, tmp);
}

struct objOrdHelper
{
	int objNrSel;
	PageItem* parent;
};

void ScribusDoc::sendItemSelectionToBack()
{
	int docSelectionCount = m_Selection->count();
	if (docSelectionCount == 0)
		return;
	QRectF selRect = m_Selection->getGroupRect();
	if (UndoManager::undoEnabled())
	{
		ScItemState<QList<QPointer<PageItem> > > *is = new ScItemState<QList<QPointer<PageItem> > >(Um::LevelBottom);
		is->set("LEVEL_BOTTOM");
		is->setItem(m_Selection->selectionList());
		m_undoManager->action(this, is);
	}
	if (docSelectionCount > 1)
	{
		PageItem *firstItem = m_Selection->itemAt(0);
		for (int a = 1; a < docSelectionCount; ++a)
		{
			if (m_Selection->itemAt(a)->Parent != firstItem->Parent)
				return;
		}
	}
	PageItem *currItem;
	QMap<int, objOrdHelper> objOrder;
	int d;
	for (int c = 0; c < docSelectionCount; ++c)
	{
		objOrdHelper oHlp;
		currItem = m_Selection->itemAt(c);
		oHlp.objNrSel = c;
		if (currItem->isGroupChild())
		{
			if (currItem->Parent->asGroupFrame()->groupItemList.count() > 1)
			{
				d = currItem->Parent->asGroupFrame()->groupItemList.indexOf(currItem);
				oHlp.parent = currItem->Parent;
				objOrder.insert(d, oHlp);
			}
		}
		else if (Items->count() > 1)
		{
			d = Items->indexOf(currItem);
			oHlp.parent = NULL;
			objOrder.insert(d, oHlp);
		}
	}
	QList<objOrdHelper> objIndex = objOrder.values();
	for (int c = objIndex.count() - 1; c > -1; c--)
	{
		objOrdHelper oHlp = objIndex[c];
		PageItem* objItem = m_Selection->itemAt(oHlp.objNrSel);
		if (oHlp.parent == NULL)
		{
			Items->removeOne(objItem);
			Items->prepend(objItem);
		}
		else
		{
			oHlp.parent->asGroupFrame()->groupItemList.removeOne(objItem);
			oHlp.parent->asGroupFrame()->groupItemList.prepend(objItem);
		}
	}
	changed();
	invalidateRegion(selRect);
	regionsChanged()->update(QRectF());
}

void ScribusDoc::bringItemSelectionToFront()
{
	int docSelectionCount = m_Selection->count();
	if (docSelectionCount == 0)
		return;
	QRectF selRect = m_Selection->getGroupRect();
	if (UndoManager::undoEnabled())
	{
		ScItemState<QList<QPointer<PageItem> > > *is = new ScItemState<QList<QPointer<PageItem> > >(Um::LevelTop);
		is->set("LEVEL_TOP");
		is->setItem(m_Selection->selectionList());
		m_undoManager->action(this, is);
	}
	if (docSelectionCount > 1)
	{
		PageItem *firstItem = m_Selection->itemAt(0);
		for (int a = 1; a < docSelectionCount; ++a)
		{
			if (m_Selection->itemAt(a)->Parent != firstItem->Parent)
				return;
		}
	}
	PageItem *currItem;
	QMap<int, objOrdHelper> objOrder;
	int d;
	for (int c = 0; c < docSelectionCount; ++c)
	{
		objOrdHelper oHlp;
		currItem = m_Selection->itemAt(c);
		oHlp.objNrSel = c;
		if (currItem->isGroupChild())
		{
			if (currItem->Parent->asGroupFrame()->groupItemList.count() > 1)
			{
				d = currItem->Parent->asGroupFrame()->groupItemList.indexOf(currItem);
				oHlp.parent = currItem->Parent;
				objOrder.insert(d, oHlp);
			}
		}
		else if (Items->count() > 1)
		{
			d = Items->indexOf(currItem);
			oHlp.parent = NULL;
			objOrder.insert(d, oHlp);
		}
	}
	QList<objOrdHelper> objIndex = objOrder.values();
	for (int c = 0; c < objIndex.count(); ++c)
	{
		objOrdHelper oHlp = objIndex[c];
		PageItem* objItem = m_Selection->itemAt(oHlp.objNrSel);
		if (oHlp.parent == NULL)
		{
			Items->removeOne(objItem);
			Items->append(objItem);
		}
		else
		{
			oHlp.parent->asGroupFrame()->groupItemList.removeOne(objItem);
			oHlp.parent->asGroupFrame()->groupItemList.append(m_Selection->itemAt(oHlp.objNrSel));
		}
	}
	changed();
	invalidateRegion(selRect);
	regionsChanged()->update(QRectF());
	return;
}

void ScribusDoc::itemSelection_LowerItem()
{
	int docSelectionCount = m_Selection->count();
	if (docSelectionCount == 0)
		return;
	QRectF selRect = m_Selection->getGroupRect();
	if (UndoManager::undoEnabled())
	{
		ScItemState<QList<QPointer<PageItem> > > *is = new ScItemState<QList<QPointer<PageItem> > >(Um::LevelDown);
		is->set("LEVEL_DOWN");
		is->setItem(m_Selection->selectionList());
		m_undoManager->action(this, is);
	}
	if (docSelectionCount > 1)
	{
		PageItem *firstItem = m_Selection->itemAt(0);
		for (int a = 1; a < docSelectionCount; ++a)
		{
			if (m_Selection->itemAt(a)->Parent != firstItem->Parent)
				return;
		}
	}
	QList<PageItem*> *itemList;
	if (m_Selection->itemAt(0)->isGroupChild())
		itemList = &(m_Selection->itemAt(0)->parentGroup()->groupItemList);
	else
		itemList = Items;
	if (itemList->count() <= 1)
		return;
	int low = itemList->count();
	int high = 0;
	int d;
	QMap<int, int> ObjOrder;
	PageItem *currItem=NULL;
	for (int c = 0; c < docSelectionCount; ++c)
	{
		currItem = m_Selection->itemAt(c);
		int id = itemList->indexOf(currItem);
		low = qMin(id, low);
		high = qMax(id, high);
	}
	if (low == 0)
		return;
	PageItem *b2 = itemList->at(high);
	bool wasSignalDelayed = !m_Selection->signalsDelayed();
	m_Selection->delaySignalsOn();
	if (!wasSignalDelayed)
		m_Selection->disconnectAllItemsFromGUI();
	Selection tempSelection(*m_Selection);
	m_Selection->clear();
	m_Selection->addItem(itemList->at(low - 1));
	for (int c = 0; c < m_Selection->count(); ++c)
	{
		currItem = m_Selection->itemAt(c);
		d = itemList->indexOf(currItem);
		ObjOrder.insert(d, c);
		itemList->takeAt(d);
	}
	d = itemList->indexOf(b2);
	QList<int> Oindex = ObjOrder.values();
	for (int c = static_cast<int>(Oindex.count()-1); c > -1; c--)
	{
		itemList->insert(d+1, m_Selection->itemAt(Oindex[c]));
	}
	m_Selection->clear();
	*m_Selection = tempSelection;
	m_Selection->delaySignalsOff();
	changed();
	invalidateRegion(selRect);
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_RaiseItem()
{
	int docSelectionCount = m_Selection->count();
	if (docSelectionCount == 0)
		return;
	QRectF selRect = m_Selection->getGroupRect();
	if (UndoManager::undoEnabled())
	{
		ScItemState<QList<QPointer<PageItem> > > *is = new ScItemState<QList<QPointer<PageItem> > >(Um::LevelUp);
		is->set("LEVEL_UP");
		is->setItem(m_Selection->selectionList());
		m_undoManager->action(this, is);
	}
	if (docSelectionCount > 1)
	{
		PageItem *firstItem = m_Selection->itemAt(0);
		for (int a = 1; a < docSelectionCount; ++a)
		{
			if (m_Selection->itemAt(a)->Parent != firstItem->Parent)
				return;
		}
	}
	QList<PageItem*> *itemList;
	if (m_Selection->itemAt(0)->isGroupChild())
		itemList = &(m_Selection->itemAt(0)->parentGroup()->groupItemList);
	else
		itemList = Items;

	if (itemList->count() <= 1)
		return;
	int low = itemList->count();
	int high = 0;
	QMap<int, int> ObjOrder;
	PageItem *currItem=NULL;
	for (int i = 0; i < docSelectionCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		int id = itemList->indexOf(currItem);
		low = qMin(id, low);
		high = qMax(id, high);
	}
	if (high == itemList->count()-1)
		return;
	PageItem *b2 = itemList->at(low);
	bool wasSignalDelayed = !m_Selection->signalsDelayed();
	m_Selection->delaySignalsOn();
	if (!wasSignalDelayed)
		m_Selection->disconnectAllItemsFromGUI();
	Selection tempSelection(*m_Selection);
	m_Selection->clear();
	m_Selection->addItem(itemList->at(high + 1));
	for (int i = 0; i < m_Selection->count(); ++i)
	{
		currItem = m_Selection->itemAt(i);
		int d = itemList->indexOf(currItem);
		ObjOrder.insert(d, i);
		itemList->takeAt(d);
	}
	QList<int> Oindex = ObjOrder.values();
	for (int i = 0; i <static_cast<int>(Oindex.count()); ++i)
	{
		int d = itemList->indexOf(b2);
		if (d == -1)
			d = 0;
		itemList->insert(d, m_Selection->itemAt(Oindex[i]));
	}
	m_Selection->clear();
	*m_Selection = tempSelection;
	m_Selection->delaySignalsOff();
	changed();
	invalidateRegion(selRect);
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_SetSoftShadow(bool has, QString color, double dx, double dy, double radius, int shade, double opac, int blend, bool erase, bool objopa)
{
	if (color == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
														  Um::IGroup, Um::LineWidth, "", Um::ILineStyle);

	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = m_Selection->itemAt(i);
		currItem->setHasSoftShadow(has);
		currItem->setSoftShadowColor(color);
		currItem->setSoftShadowXOffset(dx);
		currItem->setSoftShadowYOffset(dy);
		currItem->setSoftShadowBlurRadius(radius);
		currItem->setSoftShadowShade(shade);
		currItem->setSoftShadowOpacity(opac);
		currItem->setSoftShadowBlendMode(blend);
		currItem->setSoftShadowErasedByObject(erase);
		currItem->setSoftShadowHasObjectTransparency(objopa);
		QRectF newRect = currItem->getVisualBoundingRect().adjusted(-dx, -dy, dx, dy);
		currItem->invalidateLayout();
		regionsChanged()->update(newRect);
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();

}

void ScribusDoc::itemSelection_SetLineWidth(double w)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
														  Um::IGroup, Um::LineWidth, "", Um::ILineStyle);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = m_Selection->itemAt(i);
		QRectF oldRect = currItem->getVisualBoundingRect();
		//cb moved to setlinewidth
		//currItem->Oldm_lineWidth = currItem->lineWidth();
		currItem->setLineWidth(w);
		if (currItem->asPolyLine() || currItem->asSpiral())
			currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2, 1.0)));
		if (currItem->asLine())
		{
			int ph = static_cast<int>(qMax(1.0, w / 2.0));
			currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
									 static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
									 -ph,static_cast<int>(currItem->height()+ph));
		}
		QRectF newRect = currItem->getVisualBoundingRect();
		//currItem->update();
		currItem->invalidateLayout();
		regionsChanged()->update(newRect.united(oldRect));
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
}



void ScribusDoc::itemSelection_SetLineArt(Qt::PenStyle w)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
														  Um::IGroup, Um::LineStyle, "", Um::ILineStyle);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		m_Selection->itemAt(i)->setLineStyle(w);
		m_Selection->itemAt(i)->update();
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
}



void ScribusDoc::itemSelection_SetLineJoin(Qt::PenJoinStyle w)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
														  Um::IGroup, Um::LineJoin, "", Um::ILineStyle);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		m_Selection->itemAt(i)->setLineJoin(w);
		m_Selection->itemAt(i)->update();
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
}



void ScribusDoc::itemSelection_SetLineEnd(Qt::PenCapStyle w)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
														  Um::IGroup, Um::LineEnd, "", Um::ILineStyle);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		m_Selection->itemAt(i)->setLineEnd(w);
		m_Selection->itemAt(i)->update();
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
}



void ScribusDoc::itemSelection_SetLineSpacing(double w, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setLineSpacing(w);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetFont(QString fon, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setFont((*AllFonts)[fon]);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "FONT");
}

void ScribusDoc::itemSelection_SetFontFeatures(QString fontfeature, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setFontFeatures(fontfeature);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "FONTFEATURES");
}

void ScribusDoc::itemSelection_SetHyphenWordMin(int wordMin, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setHyphenWordMin(wordMin);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "HYPHEN_WORDMIN");
}

void ScribusDoc::itemSelection_SetHyphenConsecutiveLines(int consecutiveLines, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setHyphenConsecutiveLines(consecutiveLines);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetHyphenChar(uint hyphenChar, Selection *customSelection)
{
	CharStyle newStyle;
	newStyle.setHyphenChar(hyphenChar);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "HYPHEN_CHAR");
}

void ScribusDoc::itemSelection_SetNamedCharStyle(const QString& name, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setParent(name.isEmpty()? BaseStyle::INHERIT_PARENT : name);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "NAMED_STYLE");
}



void ScribusDoc::itemSelection_SetNamedParagraphStyle(const QString& name, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setParent(name.isEmpty()? BaseStyle::INHERIT_PARENT : name);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection, true);
}


void ScribusDoc::itemSelection_SetNamedLineStyle(const QString &name, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	uint docSelectionCount   = itemSelection->count();
	if (docSelectionCount != 0)
	{
		UndoTransaction activeTransaction;
		m_updateManager.setUpdatesDisabled();
		if (UndoManager::undoEnabled() && docSelectionCount > 1)
			activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::LineStyle, name, Um::ILineStyle);
		for (uint aa = 0; aa < docSelectionCount; ++aa)
		{
			PageItem *currItem = itemSelection->itemAt(aa);
			currItem->setCustomLineStyle(name);
			currItem->update();
		}
		if (activeTransaction)
			activeTransaction.commit();
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}


void ScribusDoc::itemSelection_SetItemPen(QString farbe)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction activeTransaction;
		m_updateManager.setUpdatesDisabled();
		if (farbe == CommonStrings::tr_NoneColor)
			farbe = CommonStrings::None;
		if (selectedItemCount > 1 && UndoManager::undoEnabled())
			activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
															  Um::IGroup, Um::SetLineColor, farbe, Um::IFill);
		PageItem *currItem;
		for (int i = 0; i < selectedItemCount; ++i)
		{
			currItem = m_Selection->itemAt(i);
			if ((currItem->asLine()) && (farbe == CommonStrings::None))
				continue;

			currItem->setLineColor(farbe);
			currItem->update();
		}
		if (activeTransaction)
			activeTransaction.commit();
		m_updateManager.setUpdatesEnabled();
	}
	changed();
}

void ScribusDoc::itemSelection_SetParBackgroundColor(QString farbe, Selection* customSelection)
{
	if (farbe == CommonStrings::tr_NoneColor)
		farbe = CommonStrings::None;
	ParagraphStyle newStyle;
	newStyle.setBackgroundColor(farbe);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetParBackgroundShade(int sha, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setBackgroundShade(sha);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetBackgroundColor(QString farbe, Selection* customSelection)
{
	if (farbe == CommonStrings::tr_NoneColor)
		farbe = CommonStrings::None;
	CharStyle newStyle;
	newStyle.setBackColor(farbe);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "BACK_COLOR");
}

void ScribusDoc::itemSelection_SetBackgroundShade(int sha, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setBackShade(sha);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "BACK_SHADE");
}

void ScribusDoc::itemSelection_SetFillColor(QString farbe, Selection* customSelection)
{
	if (farbe == CommonStrings::tr_NoneColor)
		farbe = CommonStrings::None;
	CharStyle newStyle;
	newStyle.setFillColor(farbe);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "FILL_COLOR");
}

void ScribusDoc::itemSelection_SetFillShade(int sha, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setFillShade(sha);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "FILL_SHADE");
}

void ScribusDoc::itemSelection_SetStrokeColor(QString farbe, Selection* customSelection)
{
	if (farbe == CommonStrings::tr_NoneColor)
		farbe = CommonStrings::None;
	CharStyle newStyle;
	newStyle.setStrokeColor(farbe);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "STROKE_COLOR");
}

void ScribusDoc::itemSelection_SetStrokeShade(int sha, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setStrokeShade(sha);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "STROKE_SHADE");
}

void ScribusDoc::itemSelection_SetScaleV(int scale, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setScaleV(scale);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "SCALE_V");
}

void ScribusDoc::itemSelection_SetScaleH(int scale, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setScaleH(scale);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "SCALE_H");
}

void ScribusDoc::itemSelection_SetShadowOffsets(int shx, int shy, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setShadowXOffset(shx);
	newStyle.setShadowYOffset(shy);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "SHADOW_OFFSET");
}

void ScribusDoc::itemSelection_SetUnderline(int pos, int wid, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setUnderlineOffset(pos);
	newStyle.setUnderlineWidth(wid);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "UNDERLINE");
}

void ScribusDoc::itemSelection_SetStrikethru(int pos, int wid, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setStrikethruOffset(pos);
	newStyle.setStrikethruWidth(wid);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "STRIKE_THRU");
}

void ScribusDoc::itemSelection_SetBaselineOffset(int sha, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setBaselineOffset(sha);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "BASELINE_OFFSET");
}

void ScribusDoc::itemSelection_SetOutlineWidth(int wid, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setOutlineWidth(wid);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "OUTLINE_WIDTH");
}

void ScribusDoc::itemSelection_SetItemBrush(QString farbe)
{
	if (farbe == CommonStrings::tr_NoneColor)
		farbe = CommonStrings::None;
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction activeTransaction;
		m_updateManager.setUpdatesDisabled();
		if (selectedItemCount > 1 && UndoManager::undoEnabled())
			activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
															  Um::IGroup, Um::SetFill, farbe, Um::IFill);
		PageItem *currItem;
		for (int i = 0; i < selectedItemCount; ++i)
		{
			currItem = m_Selection->itemAt(i);
			currItem->setFillColor(farbe);
			currItem->update();
		}
		if (activeTransaction)
			activeTransaction.commit();
		m_updateManager.setUpdatesEnabled();
		changed();
	}
}

void ScribusDoc::itemSelection_SetItemBrushShade(int sha)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
														  Um::IGroup, Um::SetShade, QString("%1").arg(sha),
														  Um::IShade);
	PageItem *currItem;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setFillShade(sha);
		currItem->update();
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemPenShade(int sha)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (selectedItemCount > 1 && UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
														  Um::IGroup, Um::SetLineShade, QString("%1").arg(sha), Um::IShade);
	PageItem *currItem;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setLineShade(sha);
		currItem->update();
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemGradMask(int typ)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	if (selectedItemCount > 1 && UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::Selection,Um::IFill,Um::GradTypeMask,"",Um::IFill);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setMaskType(typ);
		if ((typ > 0) && (typ < 9))
			currItem->updateGradientVectors();
		currItem->update();
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemGradStroke(int typ)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::Selection,Um::IFill,Um::GradTypeMask,"",Um::IFill);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setStrokeGradientType(typ);
		if (currItem->strokeGradientType() == 0)
		{
			if (currItem->lineColor() != CommonStrings::None)
			{
				if (!PageColors.contains(currItem->lineColor()))
				{
					switch(currItem->itemType())
					{
						case PageItem::TextFrame:
						case PageItem::PathText:
							currItem->setLineColor(m_docPrefsData.itemToolPrefs.textLineColor);
							break;
						case PageItem::Line:
						case PageItem::PolyLine:
						case PageItem::Polygon:
						case PageItem::ImageFrame:
						case PageItem::LatexFrame:
						case PageItem::Spiral:
							currItem->setLineColor(m_docPrefsData.itemToolPrefs.shapeLineColor);
							break;
						default:
							break;
					}
				}
			}
		}
		if ((typ > 0) && (typ < 8))
			currItem->updateGradientVectors();
		currItem->update();
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemGradFill(int typ)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction trans;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	if (UndoManager::undoEnabled())
		trans = m_undoManager->beginTransaction(Um::Selection,Um::IPolygon,Um::SetFill,"",Um::IFill);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setGradientType(typ);
		switch (currItem->gradientType())
		{
			case 0:
				if (currItem->fillColor() != CommonStrings::None)
				{
					if (!PageColors.contains(currItem->fillColor()))
					{
						switch(currItem->itemType())
						{
							case PageItem::ImageFrame:
							case PageItem::LatexFrame:
								currItem->setFillColor(m_docPrefsData.itemToolPrefs.imageFillColor);
								break;
							case PageItem::TextFrame:
							case PageItem::PathText:
								currItem->setFillColor(m_docPrefsData.itemToolPrefs.textFillColor);
								break;
							case PageItem::Line:
							case PageItem::PolyLine:
							case PageItem::Polygon:
							case PageItem::Spiral:
								currItem->setFillColor(m_docPrefsData.itemToolPrefs.shapeFillColor);
								break;
							default:
								break;
						}
					}
				}
				break;
			case 1:
				currItem->setGradientStartX(0);
				currItem->setGradientStartY(currItem->height() / 2.0);
				currItem->setGradientEndX(currItem->width());
				currItem->setGradientEndY(currItem->height() / 2.0);
				break;
			case 2:
				currItem->setGradientStartX(currItem->width() / 2.0);
				currItem->setGradientStartY(0);
				currItem->setGradientEndX(currItem->width() / 2.0);
				currItem->setGradientEndY(currItem->height());
				break;
			case 3:
				currItem->setGradientStartX(0);
				currItem->setGradientStartY(0);
				currItem->setGradientEndX(currItem->width());
				currItem->setGradientEndY(currItem->height());
				break;
			case 4:
				currItem->setGradientStartX(0);
				currItem->setGradientStartY(currItem->height());
				currItem->setGradientEndX(currItem->width());
				currItem->setGradientEndY(0);
				break;
			case 5:
				currItem->setGradientStartX(currItem->width() / 2.0);
				currItem->setGradientStartY(currItem->height() / 2.0);
				if (currItem->width() >= currItem->height())
				{
					currItem->setGradientEndX(currItem->width());
					currItem->setGradientEndY(currItem->height() / 2.0);
				}
				else
				{
					currItem->setGradientEndX(currItem->width() / 2.0);
					currItem->setGradientEndY(currItem->height());
				}
				break;
			default:
				currItem->selectedMeshPointX = -1;
				currItem->selectedMeshPointY = 0;
				break;
		}
		if ((typ > 0) && (typ < 8))
			currItem->updateGradientVectors();
		if (currItem->gradientType() == 13)
			currItem->createConicalMesh();
		currItem->update();
	}
	if (trans)
		trans.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemPatternFill(QString pattern)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setPattern(pattern);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemPatternProps(double imageScaleX, double imageScaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setPatternTransform(imageScaleX, imageScaleY, offsetX, offsetY, rotation, skewX, skewY);
		currItem->setPatternFlip(mirrorX, mirrorY);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemStrokePattern(QString pattern)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setStrokePattern(pattern);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemStrokePatternProps(double imageScaleX, double imageScaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, double space, bool mirrorX, bool mirrorY)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setStrokePatternTransform(imageScaleX, imageScaleY, offsetX, offsetY, rotation, skewX, skewY, space);
		currItem->setStrokePatternFlip(mirrorX, mirrorY);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemStrokePatternType(bool type)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setStrokePatternToPath(type);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemPatternMask(QString pattern)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setPatternMask(pattern);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetItemPatternMaskProps(double imageScaleX, double imageScaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	PageItem *currItem;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		currItem->setMaskTransform(imageScaleX, imageScaleY, offsetX, offsetY, rotation, skewX, skewY);
		currItem->setMaskFlip(mirrorX, mirrorY);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_InsertTableRows()
{
	PageItem* item = m_Selection->itemAt(0);
	if (!item || !item->isTable())
		return;

	PageItem_Table* table = item->asTable();
	if (!table)
		return;

	QScopedPointer<InsertTableRowsDialog> dialog(new InsertTableRowsDialog(appMode, m_ScMW));
	if (dialog->exec() == QDialog::Accepted)
	{
		dontResize = true;
		/*
		 * In table edit mode we insert either before or after the active
		 * cell, otherwise we insert at beginning or end of table.
		 */
		int index = 0;
		const TableCell cell = table->activeCell();
		if (dialog->position() == InsertTableRowsDialog::Before)
			index = appMode == modeEditTable ? cell.row() : 0;
		else
			index = appMode == modeEditTable ? cell.row() + cell.rowSpan() : table->rows();

		// Insert the rows.
		table->insertRows(index, dialog->numberOfRows());
		table->clearSelection();
		table->adjustTable();
		table->update();

		m_ScMW->updateTableMenuActions();
		changed();
	}
}

void ScribusDoc::itemSelection_InsertTableColumns()
{
	PageItem* item = m_Selection->itemAt(0);
	if (!item || !item->isTable())
		return;

	PageItem_Table* table = item->asTable();
	if (!table)
		return;

	QPointer<InsertTableColumnsDialog> dialog = new InsertTableColumnsDialog(appMode, m_ScMW);
	if (dialog->exec() == QDialog::Accepted)
	{
		dontResize = true;
		/*
		 * In table edit mode we insert either before or after the active
		 * cell, otherwise we insert at beginning or end of table.
		 */
		int index = 0;
		const TableCell cell = table->activeCell();
		if (dialog->position() == InsertTableColumnsDialog::Before)
			index = appMode == modeEditTable ? cell.column() : 0;
		else
			index = appMode == modeEditTable ? cell.column() + cell.columnSpan() : table->columns();

		// Insert the columns.
		table->insertColumns(index, dialog->numberOfColumns());
		table->clearSelection();
		table->adjustTable();
		table->update();

		m_ScMW->updateTableMenuActions();
		changed();
	}

	delete dialog;
}

void ScribusDoc::itemSelection_DeleteTableRows()
{
	PageItem* item = m_Selection->itemAt(0);
	if (!item || !item->isTable())
		return;

	PageItem_Table* table = item->asTable();
	if (!table)
		return;

	if (appMode != modeEditTable)
		return;

	if (table->selectedRows().size() >= table->rows())
		return;

	dontResize = true;
	if (table->selectedRows().isEmpty())
	{
		// Remove rows spanned by active cell.
		TableCell activeCell = table->activeCell();
		table->removeRows(activeCell.row(), activeCell.rowSpan());
	}
	else
	{
		// Remove selected row(s).
		QList<int> selectedRows = table->selectedRows().toList();
		qSort(selectedRows.begin(), selectedRows.end(), qGreater<int>());

		int index = 0;
		int numRows = 1;
		for (int i = 0; i < selectedRows.size() - 1; ++i)
		{
			index = selectedRows[i];
			if (selectedRows[i] - 1 == selectedRows[i + 1])
			{
				index = selectedRows[i + 1];
				numRows++;
			}
			else
			{
				table->removeRows(index, numRows);
				numRows = 1;
			}
		}
		table->removeRows(index, numRows);
	}

	m_View->stopGesture(); // FIXME: Don't use m_View.
	table->adjustTable();
	table->update();

	m_ScMW->updateTableMenuActions();
	changed();
}

void ScribusDoc::itemSelection_DeleteTableColumns()
{
	PageItem* item = m_Selection->itemAt(0);
	if (!item || !item->isTable())
		return;

	PageItem_Table* table = item->asTable();
	if (!table)
		return;

	if (appMode != modeEditTable)
		return;

	if (table->selectedColumns().size() >= table->columns())
		return;

	dontResize = true;
	if (table->selectedColumns().isEmpty())
	{
		// Remove columns spanned by active cell.
		TableCell activeCell = table->activeCell();
		table->removeColumns(activeCell.column(), activeCell.columnSpan());
	}
	else
	{
		// Remove selected column(s).
		QList<int> selectedColumns = table->selectedColumns().toList();
		qSort(selectedColumns.begin(), selectedColumns.end(), qGreater<int>());

		int index = 0;
		int numColumns = 1;
		for (int i = 0; i < selectedColumns.size() - 1; ++i)
		{
			index = selectedColumns[i];
			if (selectedColumns[i] - 1 == selectedColumns[i + 1])
			{
				index = selectedColumns[i + 1];
				numColumns++;
			}
			else
			{
				table->removeColumns(index, numColumns);
				numColumns = 1;
			}
		}
		table->removeColumns(index, numColumns);
	}

	m_View->stopGesture(); // FIXME: Don't use m_View.
	table->adjustTable();
	table->update();

	m_ScMW->updateTableMenuActions();
	changed();
}

void ScribusDoc::itemSelection_MergeTableCells()
{
	PageItem* item = m_Selection->itemAt(0);
	if (!item || !item->isTable())
		return;

	PageItem_Table* table = item->asTable();
	if (!table)
		return;

	if (appMode != modeEditTable)
		return;

	if (table->selectedCells().size() < 2)
		return;

	QList<int> selectedRows = table->selectedRows().toList();
	QList<int> selectedColumns = table->selectedColumns().toList();
	qSort(selectedRows);
	qSort(selectedColumns);

	const int row = selectedRows.first();
	const int column = selectedColumns.first();
	const int numRows = selectedRows.last() - row + 1;
	const int numColumns = selectedColumns.last() - column + 1;

	dontResize = true;
	table->mergeCells(row, column, numRows, numColumns);

	m_View->stopGesture(); // FIXME: Don't use m_View.
	table->adjustTable();
	table->update();

	m_ScMW->updateTableMenuActions();
	changed();
}

void ScribusDoc::itemSelection_SetTableRowHeights()
{
	PageItem* item = m_Selection->itemAt(0);
	if (!item || !item->isTable())
		return;

	PageItem_Table* table = item->asTable();
	if (!table)
		return;

	QPointer<TableRowHeightsDialog> dialog = new TableRowHeightsDialog(this, m_ScMW);
	if (dialog->exec() == QDialog::Rejected)
		return;

	const qreal rowHeight = dialog->rowHeight();
	dontResize = true;
	if (appMode == modeEditTable)
	{
		if (table->selectedCells().isEmpty())
		{
			// Set height of rows spanned by active cell.
			TableCell activeCell = table->activeCell();
			int startRow = activeCell.row();
			int endRow = startRow + activeCell.rowSpan() - 1;
			for (int row = startRow; row <= endRow; ++row)
				table->resizeRow(row, rowHeight / unitRatio());
		}
		else
		{
			// Set height of selected rows.
			foreach (const int row, table->selectedRows())
				table->resizeRow(row, rowHeight / unitRatio());
		}
	}
	else
	{
		// Set height of all rows in table.
		for (int row = 0; row < table->rows(); ++row)
			table->resizeRow(row, rowHeight / unitRatio());
	}

	delete dialog;

	table->adjustTable();
	table->update();
	changed();
}

void ScribusDoc::itemSelection_SetTableColumnWidths()
{
	PageItem* item = m_Selection->itemAt(0);
	if (!item || !item->isTable())
		return;

	PageItem_Table* table = item->asTable();
	if (!table)
		return;

	QPointer<TableColumnWidthsDialog> dialog = new TableColumnWidthsDialog(this, m_ScMW);
	if (dialog->exec() == QDialog::Rejected)
		return;

	const qreal columnWidth = dialog->columnWidth();
	dontResize = true;
	if (appMode == modeEditTable)
	{
		if (table->selectedCells().isEmpty())
		{
			// Set width of columns spanned by active cell.
			TableCell activeCell = table->activeCell();
			int startColumn = activeCell.column();
			int endColumn = startColumn + activeCell.columnSpan() - 1;
			for (int column = startColumn; column <= endColumn; ++column)
				table->resizeColumn(column, columnWidth / unitRatio());
		}
		else
		{
			// Set width of selected columns.
			foreach (const int column, table->selectedColumns())
				table->resizeColumn(column, columnWidth / unitRatio());
		}
	}
	else
	{
		// Set width of all columns in table.
		for (int column = 0; column < table->columns(); ++column)
			table->resizeColumn(column, columnWidth / unitRatio());
	}

	delete dialog;

	table->adjustTable();
	table->update();
	changed();
}

void ScribusDoc::itemSelection_DistributeTableRowsEvenly()
{
	PageItem* item = m_Selection->itemAt(0);
	if (!item || !item->isTable())
		return;

	PageItem_Table* table = item->asTable();
	if (!table)
		return;

	dontResize = true;
	if (appMode == modeEditTable && !table->selectedRows().isEmpty())
	{
		// Distribute each contigous range of selected rows.
		QList<int> selectedRows = table->selectedRows().toList();
		qSort(selectedRows);
		int startRow = selectedRows.first();
		int endRow = startRow;
		for (int i = 0; i < selectedRows.size() - 1; ++i)
		{
			if (selectedRows[i + 1] == endRow + 1)
				endRow++; // Extend range.
			else
			{
				table->distributeRows(startRow, endRow);
				// Move range.
				startRow = selectedRows[i + 1];
				endRow = startRow;
			}
		}
		table->distributeRows(startRow, endRow);
	}
	else
	{
		// Distribute all rows in the table.
		table->distributeRows(0, table->rows() - 1);
	}

	table->adjustTable();
	table->update();
	changed();
}

void ScribusDoc::itemSelection_DistributeTableColumnsEvenly()
{
	PageItem* item = m_Selection->itemAt(0);
	if (!item || !item->isTable())
		return;

	PageItem_Table* table = item->asTable();
	if (!table)
		return;

	dontResize = true;
	if (appMode == modeEditTable && !table->selectedColumns().isEmpty())
	{
		// Distribute each contigous range of selected columns.
		QList<int> selectedColumns = table->selectedColumns().toList();
		qSort(selectedColumns);
		int startColumn = selectedColumns.first();
		int endColumn = startColumn;
		for (int i = 0; i < selectedColumns.size() - 1; ++i)
		{
			if (selectedColumns[i + 1] == endColumn + 1)
				endColumn++; // Extend range.
			else
			{
				table->distributeColumns(startColumn, endColumn);
				// Move range.
				startColumn = selectedColumns[i + 1];
				endColumn = startColumn;
			}
		}
		table->distributeColumns(startColumn, endColumn);
	}
	else
	{
		// Distribute all columns in the table.
		table->distributeColumns(0, table->columns() - 1);
	}

	table->adjustTable();
	table->update();
	changed();
}

void ScribusDoc::itemSelection_SetEffects(int s, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setFeatures(static_cast<StyleFlag>(s).featureList());
	itemSelection_ApplyCharStyle(newStyle, customSelection, "EFFECTS");
}

void ScribusDoc::itemSelection_SetOpticalMargins(int i, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setOpticalMargins(i);
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}


void ScribusDoc::itemSelection_resetOpticalMargins(Selection* customSelection)
{
	//TODO
//	ParagraphStyle newStyle;
//	newStyle.setOpticalMargins(i);
//	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetTracking(int kern, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setTracking(kern);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "TRACKING");
}

void ScribusDoc::itemSelection_SetLineSpacingMode(int m, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(m));
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetLanguage(QString m, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setLanguage(m);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "LANGUAGE");
}

void ScribusDoc::itemSetFont(const QString &newFont)
{
	QString nf2(newFont);
	if (!UsedFonts.contains(newFont))
	{
		if (!AddFont(newFont))
		{
			if (m_Selection->count() != 0)
			{
				PageItem *currItem = m_Selection->itemAt(0);
				nf2 = currItem->currentCharStyle().font().scName();
			}
		}
	}
	PageItem *i2 = m_Selection->itemAt(0);
	if (appMode == modeEditTable)
		i2 = m_Selection->itemAt(0)->asTable()->activeCell().textFrame();
	if (i2 != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		itemSelection_SetFont(nf2, &tempSelection);
	}
	m_View->DrawNew();
}

void ScribusDoc::itemSelection_SetFontSize(int size, Selection* customSelection)
{
	CharStyle newStyle;
	newStyle.setFontSize(size);
	itemSelection_ApplyCharStyle(newStyle, customSelection, "FONT_SIZE");
}

void ScribusDoc::itemSelection_ToggleBookMark(Selection *customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;

	for (uint a = 0; a < selectedItemCount; ++a)
	{
		UndoTransaction activeTransaction;
		if (UndoManager::undoEnabled())
			activeTransaction = m_undoManager->beginTransaction();
		PageItem* currItem = itemSelection->itemAt(a);
		if (currItem->asTextFrame())
		{
			if (currItem->OwnPage != -1)
			{
				bool old = currItem->isBookmark;
				currItem->setIsBookMark(!currItem->isBookmark);
				if (currItem->isBookmark)
				{
					currItem->setIsAnnotation(false);
					emit addBookmark(currItem);
				}
				else
				{
					if (old)
						emit deleteBookmark(currItem);
				}
			}
		}
		if (activeTransaction) {
			activeTransaction.commit(Um::Selection,
									 Um::IGroup,
									 Um::ActionPDF,
									 "",
									 Um::IGroup);
		}
	}
	m_ScMW->actionManager->setPDFActions(m_View);
	changed();
}

void ScribusDoc::itemSelection_ToggleAnnotation(Selection *customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;

	for (uint a = 0; a < selectedItemCount; ++a)
	{
		UndoTransaction activeTransaction;
		if (UndoManager::undoEnabled())
			activeTransaction = m_undoManager->beginTransaction();
		PageItem* currItem = itemSelection->itemAt(a);
		if (currItem->asTextFrame())
		{
			bool old = currItem->isBookmark;
			currItem->setIsAnnotation(!currItem->isAnnotation());
			if (currItem->isAnnotation())
			{
				currItem->AutoName = false;
				if (m_masterPageMode)
				{
					currItem->annotation().setType(Annotation::Link);
					currItem->annotation().setZiel(0);
					currItem->annotation().setAction("0 0");
				}
				if (old)
					emit deleteBookmark(currItem);
				currItem->isBookmark = false;
			}
		}
		if (activeTransaction) {
			activeTransaction.commit(Um::Selection,
									  Um::IGroup,
									  Um::ActionPDF,
									  "",
									  Um::IGroup);
		}
	}
	m_ScMW->actionManager->setPDFActions(m_View);
	changed();
}

void ScribusDoc::itemSelection_SetParagraphStyle(const ParagraphStyle & newStyle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SetStyle, newStyle.displayName(), Um::IFont);
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && ((appMode == modeEdit) || (appMode == modeEditTable)))
		{
			int start = currItem->firstInFrame();
			int stop = currItem->lastInFrame()+1;
			if ((appMode == modeEdit) || (appMode == modeEditTable))
			{
				start = currItem->itemText.startOfSelection();
				stop = currItem->itemText.endOfSelection();
				if (start >= stop)
					start = stop = currItem->itemText.normalizedCursorPosition();
			}
			for (int pos=start; pos < stop; ++pos)
			{
				if (currItem->itemText.text(pos) == SpecialChars::PARSEP)
				{
					currItem->itemText.setStyle(pos, newStyle);
				}
			}
			currItem->itemText.setStyle(stop, newStyle);
			currItem->invalid = true;
		}
		else
		{
			if (currItem->isNoteFrame())
				setNotesChanged(true);
			else if (currItem->isTextFrame())
				updateItemNotesFramesStyles(currItem, newStyle);
			else 
			currItem->itemText.setDefaultStyle(newStyle);
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
	}
	if (activeTransaction)
		activeTransaction.commit();
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_EraseParagraphStyle(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::RemoveTextStyle, tr( "remove direct paragraph formatting" ), Um::IFont);
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && ((appMode == modeEdit) || (appMode == modeEditTable)))
		{
			int start = currItem->firstInFrame();
			int stop = currItem->lastInFrame()+1;
			if ((appMode == modeEdit) || (appMode == modeEditTable))
			{
				start = currItem->itemText.startOfSelection();
				stop = currItem->itemText.endOfSelection();
				if (start >= stop)
					start = stop = currItem->itemText.normalizedCursorPosition();
			}
			for (int pos=start; pos < stop; ++pos)
			{
				if (currItem->itemText.text(pos) == SpecialChars::PARSEP)
				{
					ParagraphStyle newStyle;
					newStyle.setParent(currItem->itemText.paragraphStyle(pos).parent());
					if (UndoManager::undoEnabled())
					{
						ScItemState<QPair<ParagraphStyle,ParagraphStyle> > *is = new ScItemState<QPair <ParagraphStyle,ParagraphStyle> >(Um::SetStyle);
						is->set("SET_PARASTYLE");
						is->set("POS",pos);
						is->setItem(qMakePair(newStyle, currItem->itemText.paragraphStyle(pos)));
						m_undoManager->action(currItem, is);
					}
					currItem->itemText.setStyle(pos, newStyle);
				}
			}
			ParagraphStyle newStyle2;
			newStyle2.setParent(currItem->itemText.paragraphStyle(stop).parent());
			if (UndoManager::undoEnabled())
			{
				ScItemState<QPair<ParagraphStyle,ParagraphStyle> > *is = new ScItemState<QPair <ParagraphStyle,ParagraphStyle> >(Um::SetStyle);
				is->set("SET_PARASTYLE");
				is->set("POS",stop);
				is->setItem(qMakePair(newStyle2, currItem->itemText.paragraphStyle(stop)));
				m_undoManager->action(currItem, is);
			}
			currItem->itemText.setStyle(stop, newStyle2);
		}
		else
		{
			ParagraphStyle newStyle;
			//for notes frames apply style from master frame
			if (currItem->isNoteFrame() && (currItem->asNoteFrame()->masterFrame() != NULL))
			{
				newStyle.setParent(currItem->asNoteFrame()->masterFrame()->itemText.defaultStyle().parent());
				newStyle.applyStyle(currItem->asNoteFrame()->masterFrame()->currentStyle());
			}
			else
			newStyle.setParent(currItem->itemText.defaultStyle().parent());
			if (UndoManager::undoEnabled())
			{
				ScItemState<QPair<ParagraphStyle,ParagraphStyle> > *is = new ScItemState<QPair <ParagraphStyle,ParagraphStyle> >(Um::SetStyle);
				is->set("APPLY_DEFAULTPARASTYLE");
				is->setItem(qMakePair(newStyle, currItem->itemText.defaultStyle()));
				m_undoManager->action(currItem, is);
			}
			currItem->itemText.setDefaultStyle(newStyle);
			if (currItem->isTextFrame() && !currItem->isNoteFrame())
				updateItemNotesFramesStyles(currItem, newStyle);
			else if (currItem->isNoteFrame())
				setNotesChanged(true);
		}
		currItem->invalid = true;
		if (currItem->asPathText())
			currItem->updatePolyClip();
		if (currItem->isNoteFrame())
			currItem->asNoteFrame()->updateNotesText();
		else if (currItem->isTextFrame() && currItem->asTextFrame()->hasNoteFrame(NULL, true))
			setNotesChanged(true);
	}
	if (activeTransaction)
		activeTransaction.commit();
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_ClearBulNumStrings(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		if (currItem->itemText.length() == 0)
			continue;
		int currItemTextCount = currItem->itemText.length();
		if ((appMode != modeEdit) && (appMode != modeEditTable))
		{
			for (int pos = 0; pos < currItemTextCount; ++pos)
			{
				if (currItem->itemText.hasMark(pos))
				{
					Mark* mark = currItem->itemText.mark(pos);
					if (mark->isType(MARKBullNumType))
					{
						if (UndoManager::undoEnabled())
						{
							ScItemState<QPair<int,QString> > *is = new ScItemState<QPair <int,QString> >(Um::SetStyle);
							is->set("CLEARMARK");
							is->setItem(qMakePair(pos, mark->getString()));
							m_undoManager->action(currItem, is);
						}
						mark->setString(QString());
					}
				}
			}
		}
		else
		{
			int start;
			int stop;
			start = stop = currItem->itemText.normalizedCursorPosition();
			if (currItem->HasSel)
			{
				start = currItem->itemText.startOfSelection();
				stop = currItem->itemText.endOfSelection();
			}
			start = currItem->itemText.startOfParagraph(currItem->itemText.nrOfParagraph(start));
			stop = currItem->itemText.endOfParagraph(currItem->itemText.nrOfParagraph(stop));
			for (int pos=start; pos < stop; ++pos)
			{
				if (currItem->itemText.hasMark(pos))
				{
					Mark* mark = currItem->itemText.mark(pos);
					if (mark->isType(MARKBullNumType))
					{
						if (UndoManager::undoEnabled())
						{
							ScItemState<QPair<int,QString> > *is = new ScItemState<QPair <int,QString> >(Um::SetStyle);
							is->set("CLEARMARKSTRING");
							is->setItem(qMakePair(pos, mark->getString()));
							m_undoManager->action(currItem, is);
						}
						mark->setString(QString());
					}
				}
			}
		}
	}
	flag_Renumber = true;
}

void ScribusDoc::itemSelection_ApplyParagraphStyle(const ParagraphStyle & newStyle, Selection* customSelection, bool rmDirectFormatting)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	itemSelection_ClearBulNumStrings(itemSelection);
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
	{
		PageItem* currItem = itemSelection->itemAt(0);
		QString  targetName = (selectedItemCount > 1) ? Um::SelectionGroup : currItem->getUName();
		QPixmap* targetPixmap = (selectedItemCount > 1) ? Um::IGroup : currItem->getUPixmap();
		activeTransaction = m_undoManager->beginTransaction(targetName, targetPixmap, Um::ApplyTextStyle, newStyle.displayName(), Um::IFont);
	}
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		if (currItem->isTable() && appMode == modeEditTable)
			currItem = currItem->asTable()->activeCell().textFrame();
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount == 0) || ((appMode != modeEdit) && (appMode != modeEditTable)))
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.applyStyle(newStyle);
			if (UndoManager::undoEnabled())
			{
				ScItemState<QPair<ParagraphStyle,ParagraphStyle> > *is = new ScItemState<QPair <ParagraphStyle,ParagraphStyle> >(Um::SetStyle);
				is->set("APPLY_DEFAULTPARASTYLE");
				is->setItem(qMakePair(dstyle, currItem->itemText.defaultStyle()));
				m_undoManager->action(currItem, is);
			}
			currItem->itemText.setDefaultStyle(dstyle);
			if (currItem->isNoteFrame())
				setNotesChanged(true);
			else if (currItem->isTextFrame())
				updateItemNotesFramesStyles(currItem, dstyle);
		}
		if (currItemTextCount > 0)
		{
			int start = currItem->asPathText() ? currItem->firstInFrame() : 0;
			int stop  = currItem->asPathText() ? currItem->lastInFrame()+1 :  currItemTextCount;
			if ((appMode == modeEdit) || (appMode == modeEditTable))
			{
				start = currItem->itemText.startOfSelection();
				stop = currItem->itemText.endOfSelection();
				if (start >= stop)
					start = stop = currItem->itemText.normalizedCursorPosition();
			}
			for (int pos=start; pos < stop; ++pos)
			{
				if (currItem->itemText.text(pos) == SpecialChars::PARSEP)
				{
					if (UndoManager::undoEnabled())
					{
						ScItemState<QPair<ParagraphStyle,ParagraphStyle> > *is = new ScItemState<QPair <ParagraphStyle,ParagraphStyle> >(Um::SetStyle);
						is->set("APPLY_PARASTYLE");
						is->set("POS",pos);
						is->setItem(qMakePair(newStyle, currItem->itemText.paragraphStyle(pos)));
						m_undoManager->action(currItem, is);
					}
					currItem->itemText.applyStyle(pos, newStyle, rmDirectFormatting);
				}
			}
			if (UndoManager::undoEnabled())
			{
				ScItemState<QPair<ParagraphStyle,ParagraphStyle> > *is = new ScItemState<QPair <ParagraphStyle,ParagraphStyle> >(Um::SetStyle);
				is->set("APPLY_PARASTYLE");
				is->set("POS",stop);
				is->setItem(qMakePair(newStyle, currItem->itemText.paragraphStyle(stop)));
				m_undoManager->action(currItem, is);
			}
			currItem->itemText.applyStyle(stop, newStyle, rmDirectFormatting);
			currItem->invalid = true;
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
		if (currItem->isNoteFrame())
			currItem->asNoteFrame()->updateNotesText();
		else if (currItem->isTextFrame() && currItem->asTextFrame()->hasNoteFrame(NULL, true))
			setNotesChanged(true);
		currItem->invalidateLayout();
	}
	if (activeTransaction)
		activeTransaction.commit();
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_ApplyCharStyle(const CharStyle & newStyle, Selection* customSelection, QString ETEA)
{
	Selection* itemSelection = (customSelection != 0) ? customSelection : m_Selection;
	assert(itemSelection != 0);
	uint selectedItemCount = itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
	{
		PageItem* currItem = itemSelection->itemAt(0);
		QString  targetName = (selectedItemCount > 1) ? Um::SelectionGroup : currItem->getUName();
		QPixmap* targetPixmap = (selectedItemCount > 1) ? Um::IGroup : currItem->getUPixmap();
		activeTransaction = m_undoManager->beginTransaction(targetName, targetPixmap, Um::ApplyTextStyle, newStyle.asString(), Um::IFont);
	}
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		if (currItem->isTable() && appMode == modeEditTable)
			currItem = currItem->asTable()->activeCell().textFrame();
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && ((appMode == modeEdit) || (appMode == modeEditTable)))
		{
			int start = currItem->firstInFrame();
			int length = currItem->lastInFrame() - start + 1;
			if ((appMode == modeEdit) || (appMode == modeEditTable))
			{
				if (currItem->itemText.lengthOfSelection() > 0)
				{
					start = currItem->itemText.startOfSelection();
					length = currItem->itemText.endOfSelection() - start;
				}
				else
				{
					start = qMax(currItem->firstInFrame(), currItem->itemText.cursorPosition());
					length = start < currItem->itemText.length() ? 1 : 0;
				}
			}
			CharStyle lastParent = currItem->itemText.charStyle(start);
			int stop = start+qMax(0,length);
			int lastPos = start;
			for (int i = start; i <= stop; ++i)
			{
				const CharStyle& curParent(currItem->itemText.charStyle(i));
				if (!curParent.equiv(lastParent) || i==stop)
				{
					if (UndoManager::undoEnabled())
					{
						UndoState* state = m_undoManager->getLastUndo();
						ScItemState<QPair<CharStyle,CharStyle> > *is = NULL;
						SimpleState *ss = NULL;
						TransactionState *ts = NULL;
						while (state && state->isTransaction())
						{
							ts = dynamic_cast<TransactionState*>(state);
							ss = dynamic_cast<SimpleState*>(ts->last());
							state = ts->last();
						}
						if (ts && ss && ss->get("ETEA") == ETEA)
						{
							for (int i = ts->sizet() - 1; i >= 0; --i)
							{
								is = dynamic_cast<ScItemState<QPair<CharStyle, CharStyle> > *>(ts->at(i));
								if (!is || (is->get("ETEA") != ETEA))
									break;
								is->setItem(qMakePair(newStyle, is->getItem().second));
							}
						}
						else
						{
							is = new ScItemState<QPair <CharStyle,CharStyle> >(Um::ApplyTextStyle);
							is->set("APPLY_CHARSTYLE");
							is->set("START", lastPos);
							is->set("LENGTH", i - lastPos);
							is->set("ETEA", ETEA);
							is->setItem(qMakePair(newStyle, currItem->itemText.charStyle(lastPos)));
							m_undoManager->action(currItem, is);
						}
					}
					lastPos = i;
					lastParent = curParent;
				}
			}
			currItem->itemText.applyCharStyle(start, qMax(0, length), newStyle);
			currItem->invalid = true;
		}
		else
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.charStyle().applyCharStyle(newStyle);
			if (UndoManager::undoEnabled())
			{
				ScItemState<QPair<ParagraphStyle,ParagraphStyle> > *is = new ScItemState<QPair <ParagraphStyle,ParagraphStyle> >(Um::SetStyle);
				is->set("APPLY_DEFAULTPARASTYLE");
				is->setItem(qMakePair(dstyle, currItem->itemText.defaultStyle()));
				m_undoManager->action(currItem, is);
			}
			CharStyle lastParent = currItem->itemText.charStyle(0);
			int stop = currItem->itemText.length();
			int lastPos = 0;
			for (int i = 0; i <= stop; ++i)
			{
				const CharStyle& curParent(currItem->itemText.charStyle(i));
				if (!curParent.equiv(lastParent) || i==stop)
				{
					if (UndoManager::undoEnabled())
					{
						ScItemState<QPair<CharStyle,CharStyle> > *ist = new ScItemState<QPair <CharStyle,CharStyle> >(Um::ApplyTextStyle);
						ist->set("APPLY_CHARSTYLE");
						ist->set("START", lastPos);
						ist->set("LENGTH", i - lastPos);
						ist->setItem(qMakePair(newStyle, currItem->itemText.charStyle(lastPos)));
						m_undoManager->action(currItem, ist);
					}
					lastPos = i;
					lastParent = curParent;
				}
			}
			currItem->itemText.setDefaultStyle(dstyle);
			currItem->itemText.applyCharStyle(0, currItem->itemText.length(), newStyle);
			currItem->invalid = true;
			if (currItem->isNoteFrame())
				setNotesChanged(true);
			else if (currItem->isTextFrame())
				updateItemNotesFramesStyles(currItem, dstyle);
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
		if (currItem->isNoteFrame())
			currItem->asNoteFrame()->updateNotesText();
		currItem->invalidateLayout();
	}
	if (activeTransaction)
		activeTransaction.commit();
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_SetCharStyle(const CharStyle & newStyle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ApplyTextStyle, newStyle.asString(), Um::IFont);
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && ((appMode == modeEdit) || (appMode == modeEditTable)))
		{
			int start = currItem->firstInFrame();
			int length = currItem->lastInFrame() - start + 1;
			if ((appMode == modeEdit) || (appMode == modeEditTable))
			{
				if (currItem->itemText.lengthOfSelection() > 0)
				{
					start = currItem->itemText.startOfSelection();
					length = currItem->itemText.endOfSelection() - start;
				}
				else
				{
					start = qMax(currItem->firstInFrame(), currItem->itemText.cursorPosition());
					length = start < currItem->itemText.length() ? 1 : 0;
				}
			}
			if (UndoManager::undoEnabled())
			{
				ScItemState<QPair<CharStyle,CharStyle> > *is = new ScItemState<QPair <CharStyle,CharStyle> >(Um::ApplyTextStyle);
				is->set("SET_CHARSTYLE");
				is->set("START",start);
				is->set("LENGTH",length);
				is->setItem(qMakePair(newStyle, currItem->itemText.charStyle(start)));
				m_undoManager->action(currItem, is);
			}
			currItem->itemText.setCharStyle(start, length, newStyle);
			currItem->invalid = true;
		}
		else
		{
			ParagraphStyle dstyle(currItem->itemText.defaultStyle());
			dstyle.charStyle().setStyle(newStyle);
			if (UndoManager::undoEnabled())
			{
				ScItemState<QPair<ParagraphStyle,ParagraphStyle> > *is = new ScItemState<QPair <ParagraphStyle,ParagraphStyle> >(Um::SetStyle);
				is->set("APPLY_DEFAULTPARASTYLE");
				is->setItem(qMakePair(dstyle, currItem->itemText.defaultStyle()));
				m_undoManager->action(currItem, is);

				ScItemState<QPair<CharStyle,CharStyle> > *ist = new ScItemState<QPair <CharStyle,CharStyle> >(Um::ApplyTextStyle);
				ist->set("SET_CHARSTYLE");
				ist->set("START",0);
				ist->set("LENGTH",currItem->itemText.length());
				ist->setItem(qMakePair(newStyle, currItem->itemText.charStyle(0)));
				m_undoManager->action(currItem, ist);
			}
			currItem->itemText.setDefaultStyle(dstyle);
			currItem->itemText.setCharStyle(0, currItem->itemText.length(), newStyle);
			if (currItem->isNoteFrame())
				setNotesChanged(true);
			else if (currItem->isTextFrame())
				updateItemNotesFramesStyles(currItem, dstyle);
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
		if (currItem->isNoteFrame())
			currItem->asNoteFrame()->updateNotesText();
		currItem->invalidateLayout();
	}
	if (activeTransaction)
		activeTransaction.commit();
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_EraseCharStyle(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::RemoveTextStyle, tr( "remove direct char formatting" ), Um::IFont);
	for (uint aa = 0; aa < selectedItemCount; ++aa)
	{
		PageItem *currItem = itemSelection->itemAt(aa);
		int currItemTextCount = currItem->itemText.length();
		if ((currItemTextCount > 0) && ((appMode == modeEdit) || (appMode == modeEditTable)))
		{
			int start = currItem->firstInFrame();
			int length = currItem->lastInFrame() - start + 1;
			if ((appMode == modeEdit) || (appMode == modeEditTable))
			{
				if (currItem->itemText.lengthOfSelection() > 0)
				{
					start = currItem->itemText.startOfSelection();
					length = currItem->itemText.endOfSelection() - start;
				}
				else
				{
					start = qMax(currItem->firstInFrame(), currItem->itemText.cursorPosition());
					length = start < currItem->itemText.length() ? 1 : 0;
				}
			}
			QString lastParent;
			int stop = start+length;
			int lastPos = start;
			for (int i=start; i < stop; ++i)
			{
				const QString& curParent(currItem->itemText.charStyle(i).parent());
				if (curParent != lastParent)
				{
					if ( i-lastPos > 0)
					{
						CharStyle newStyle;
						newStyle.setParent(lastParent);
						if (UndoManager::undoEnabled())
						{
							ScItemState<QPair<CharStyle,CharStyle> > *is = new ScItemState<QPair <CharStyle,CharStyle> >(Um::ApplyTextStyle);
							is->set("SET_CHARSTYLE");
							is->set("START",lastPos);
							is->set("LENGTH",i-lastPos);
							is->setItem(qMakePair(newStyle, currItem->itemText.charStyle(lastPos)));
							m_undoManager->action(currItem, is);
						}
						currItem->itemText.setCharStyle(lastPos, i-lastPos, newStyle);
						lastPos = i;
					}
					lastParent = curParent;
				}
			}
			if (lastPos < stop)
			{
				CharStyle newStyle2;
				newStyle2.setParent(lastParent);
				if (UndoManager::undoEnabled())
				{
					ScItemState<QPair<CharStyle,CharStyle> > *is = new ScItemState<QPair <CharStyle,CharStyle> >(Um::ApplyTextStyle);
					is->set("SET_CHARSTYLE");
					is->set("START",lastPos);
					is->set("LENGTH",stop-lastPos);
					is->setItem(qMakePair(newStyle2, currItem->itemText.charStyle(lastPos)));
					m_undoManager->action(currItem, is);
				}
				currItem->itemText.setCharStyle(lastPos, stop-lastPos, newStyle2);
			}
		}
		else
		{
			ParagraphStyle defStyle = currItem->itemText.defaultStyle();
			CharStyle newStyle;
			newStyle.setParent(defStyle.charStyle().parent());
			defStyle.charStyle() = newStyle;
			if (UndoManager::undoEnabled())
			{
				ScItemState<QPair<ParagraphStyle,ParagraphStyle> > *is = new ScItemState<QPair <ParagraphStyle,ParagraphStyle> >(Um::SetStyle);
				is->set("APPLY_DEFAULTPARASTYLE");
				is->setItem(qMakePair(defStyle, currItem->itemText.defaultStyle()));
				m_undoManager->action(currItem, is);
			}
			currItem->itemText.setDefaultStyle(defStyle);
			if (currItem->isNoteFrame())
				setNotesChanged(true);
			else if (currItem->isTextFrame())
				updateItemNotesFramesStyles(currItem, defStyle);
		}
		if (currItem->asPathText())
			currItem->updatePolyClip();
		if (currItem->isNoteFrame())
			currItem->asNoteFrame()->updateNotesText();
		currItem->invalidateLayout();
	}
	if (activeTransaction)
		activeTransaction.commit();
	changed();
	regionsChanged()->update(QRectF());
}

//AV -> NodeEditContext
//CB removed looping, called by itemSelection_FlipH
void ScribusDoc::MirrorPolyH(PageItem* currItem)
{
	currItem->ClipEdited = true;
	QTransform ma;
	if (nodeEdit.isContourLine())
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
			ss->set("MIRROR_PATH_H");
			ss->set("IS_CONTOUR", true);
			m_undoManager->action(currItem, ss, Um::IBorder);
		}
		//FPoint tp2(getMinClipF(&currItem->ContourLine));
		FPoint tp(getMaxClipF(&currItem->ContourLine));
		ma.translate(qRound(tp.x()), 0);
		ma.scale(-1, 1);
		currItem->ContourLine.map(ma);
		regionsChanged()->update(QRectF());
		changed();
		return;
	}
	ma.scale(-1, 1);
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(currItem->width(), 0);
	if (currItem->asPathText())
		currItem->updatePolyClip();
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	setRedrawBounding(currItem);
	currItem->update();
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipH, "", Um::IFlipH);
		ss->set("MIRROR_PATH_H");
		ss->set("IS_CONTOUR", false);
		m_undoManager->action(currItem, ss, Um::IBorder);
	}
	changed();
}


//AV -> NodeEditContext
//CB removed looping, called by itemSelection_FlipV
void ScribusDoc::MirrorPolyV(PageItem* currItem)
{
	currItem->ClipEdited = true;
	QTransform ma;
	if (nodeEdit.isContourLine())
	{
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = new SimpleState(Um::FlipV, "", Um::IFlipV);
			ss->set("MIRROR_PATH_V");
			ss->set("IS_CONTOUR", true);
			m_undoManager->action(currItem, ss, Um::IBorder);
		}
		FPoint tp(getMaxClipF(&currItem->ContourLine));
		ma.translate(0, qRound(tp.y()));
		ma.scale(1, -1);
		currItem->ContourLine.map(ma);
		regionsChanged()->update(QRectF());
		changed();
		return;
	}
	ma.scale(1, -1);
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(0, currItem->height());
	if (currItem->asPathText())
		currItem->updatePolyClip();
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	setRedrawBounding(currItem);
	currItem->update();
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::FlipV, "", Um::IFlipV);
		ss->set("MIRROR_PATH_V");
		ss->set("IS_CONTOUR", false);
		m_undoManager->action(currItem, ss, Um::IBorder);
	}
	changed();
}

bool ScribusDoc::getItem(PageItem **currItem, int nr)
{
	int n=nr;
	if (n == -1)
		n=0;
	*(currItem) = m_Selection->itemAt(n);
	return (*(currItem)!=NULL);
}

void ScribusDoc::setFrameRect()
{
	nodeEdit.deselect();
	PageItem *currItem;
	if (getItem(&currItem))
	{
		currItem->SetRectFrame();
		setRedrawBounding(currItem);
		regionsChanged()->update(currItem->getRedrawBounding(1.0));
	}
}

void ScribusDoc::setFrameRounded()
{
	nodeEdit.deselect();
	PageItem *currItem;
	if (getItem(&currItem))
	{
		if (currItem->cornerRadius() == 0)
		{
			setFrameRect();
			return;
		}
		currItem->SetFrameRound();
		setRedrawBounding(currItem);
		regionsChanged()->update(currItem->getRedrawBounding(1.0));
	}
}

void ScribusDoc::setFrameOval()
{
	nodeEdit.deselect();
	PageItem *currItem;
	if (getItem(&currItem))
	{
		currItem->SetOvalFrame();
		setRedrawBounding(currItem);
		regionsChanged()->update(currItem->getRedrawBounding(1.0));
	}
}


void ScribusDoc::setRedrawBounding(PageItem *currItem)
{
	currItem->setRedrawBounding();
	FPoint maxSize(currItem->BoundingX+currItem->BoundingW+m_docPrefsData.displayPrefs.scratch.right(), currItem->BoundingY+currItem->BoundingH+m_docPrefsData.displayPrefs.scratch.bottom());
	FPoint minSize(currItem->BoundingX-m_docPrefsData.displayPrefs.scratch.left(), currItem->BoundingY-m_docPrefsData.displayPrefs.scratch.top());
	adjustCanvas(minSize, maxSize);
}


void ScribusDoc::adjustCanvas(FPoint minPos, FPoint maxPos, bool absolute)
{
	if (dontResize)
		return;
	double newMaxX, newMaxY, newMinX, newMinY;
	if (absolute)
	{
		newMaxX = maxPos.x();
		newMaxY = maxPos.y();
		newMinX = minPos.x();
		newMinY = minPos.y();
	}
	else
	{
		newMaxX = qMax(maxCanvasCoordinate.x(), maxPos.x());
		newMaxY = qMax(maxCanvasCoordinate.y(), maxPos.y());
		newMinX = qMin(minCanvasCoordinate.x(), minPos.x());
		newMinY = qMin(minCanvasCoordinate.y(), minPos.y());
	}
	if ((newMaxX != maxCanvasCoordinate.x()) || (newMaxY != maxCanvasCoordinate.y())
	|| (newMinX != minCanvasCoordinate.x()) || (newMinY != minCanvasCoordinate.y()))
	{
		maxCanvasCoordinate = FPoint(newMaxX, newMaxY);
		minCanvasCoordinate = FPoint(newMinX, newMinY);
		regionsChanged()->update(QRectF());
	}
}


void ScribusDoc::connectDocSignals()
{
	if (ScCore->usingGUI())
	{
		if (m_hasGUI)
		{
			connect(this, SIGNAL(docChanged()), m_ScMW, SLOT(slotDocCh()));
			connect(this, SIGNAL(firstSelectedItemType(int)), m_ScMW, SLOT(HaveNewSel()));
			connect(this->m_Selection, SIGNAL(selectionChanged()), m_ScMW, SLOT(HaveNewSel()));
			connect(autoSaveTimer, SIGNAL(timeout()), this, SLOT(slotAutoSave()));
		}
	}
}

void ScribusDoc::disconnectDocSignals()
{
	if (ScCore->usingGUI())
	{
		if (m_hasGUI)
		{
			disconnect(this, SIGNAL(docChanged()), m_ScMW, SLOT(slotDocCh()));
			disconnect(this, SIGNAL(firstSelectedItemType(int)), m_ScMW, SLOT(HaveNewSel()));
			disconnect(this->m_Selection, SIGNAL(selectionChanged()), m_ScMW, SLOT(HaveNewSel()));
			disconnect(autoSaveTimer, SIGNAL(timeout()), this, SLOT(slotAutoSave()));
		}
	}
}

//CB Same as RecalcPicturesRes apart from the name checking, which should be able to be removed
void ScribusDoc::updatePict(QString name)
{
	bool updated=false;
	QList<PageItem*> allItems;
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((!currItem->imageIsAvailable) || (currItem->Pfile != name))
				continue;
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
			updated=true;
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((!currItem->imageIsAvailable) || (currItem->Pfile != name))
				continue;
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
			updated = true;
		}
		allItems.clear();
	}
	for (QHash<int, PageItem*>::iterator it = FrameItems.begin(); it != FrameItems.end(); ++it)
	{
		PageItem *currItem = it.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((!currItem->imageIsAvailable) || (currItem->Pfile != name))
				continue;
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
			updated = true;
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		if (pa.items.count() <= 0)
			continue;
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if ((!currItem->imageIsAvailable) || (currItem->Pfile != name))
					continue;
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				double imgX = currItem->imageXOffset();
				double imgY = currItem->imageYOffset();
				loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
				currItem->setImageXOffset(imgX);
				currItem->setImageYOffset(imgY);
				updated = true;
			}
			allItems.clear();
		}
		PageItem *ite = pa.items.at(0);
		double minx =  std::numeric_limits<double>::max();
		double miny =  std::numeric_limits<double>::max();
		double maxx = -std::numeric_limits<double>::max();
		double maxy = -std::numeric_limits<double>::max();
		double x1, x2, y1, y2;
		ite->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
	}
	if (updated)
	{
		regionsChanged()->update(QRectF());
		changed();
	}
}

void ScribusDoc::updatePictDir(QString name)
{
	bool updated=false;
	QList<PageItem*> allItems;
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (!currItem->asImageFrame() || currItem->imageIsAvailable || currItem->Pfile.isEmpty())
				continue;
			QFileInfo fi(currItem->Pfile);
			if ((fi.absolutePath() != name) || !fi.exists())
				continue;
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
			ScCore->fileWatcher->addFile(currItem->Pfile);
			updated = true;
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (!currItem->asImageFrame() || currItem->imageIsAvailable || currItem->Pfile.isEmpty())
				continue;
			QFileInfo fi(currItem->Pfile);
			if ((fi.absolutePath() != name) || !fi.exists())
				continue;
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
			ScCore->fileWatcher->addFile(currItem->Pfile);
			updated = true;
		}
		allItems.clear();
	}
	for (QHash<int, PageItem*>::iterator it = FrameItems.begin(); it != FrameItems.end(); ++it)
	{
		PageItem *currItem = it.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (!currItem->asImageFrame() || currItem->imageIsAvailable || currItem->Pfile.isEmpty())
				continue;
			QFileInfo fi(currItem->Pfile);
			if ((fi.absolutePath() != name) || !fi.exists())
				continue;
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
			ScCore->fileWatcher->addFile(currItem->Pfile);
			updated = true;
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		if (pa.items.count() <= 0)
			continue;
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if (!currItem->asImageFrame() || currItem->imageIsAvailable || currItem->Pfile.isEmpty())
					continue;
				QFileInfo fi(currItem->Pfile);
				if ((fi.absolutePath() != name) || !fi.exists())
					continue;
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				double imgX = currItem->imageXOffset();
				double imgY = currItem->imageYOffset();
				loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
				currItem->setImageXOffset(imgX);
				currItem->setImageYOffset(imgY);
				ScCore->fileWatcher->addFile(currItem->Pfile);
				updated = true;
			}
			allItems.clear();
		}
		PageItem *ite = pa.items.at(0);
		double minx =  std::numeric_limits<double>::max();
		double miny =  std::numeric_limits<double>::max();
		double maxx = -std::numeric_limits<double>::max();
		double maxy = -std::numeric_limits<double>::max();
		double x1, x2, y1, y2;
		ite->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
	}
	if (updated)
	{
		regionsChanged()->update(QRectF());
		changed();
	}
}

//CB Same as updatePict apart from the name checking, this should be able to be removed
void ScribusDoc::recalcPicturesRes(bool applyNewRes)
{
	int ca = 0;
	ScGuardedPtr<ScribusDoc> docPtr = guardedPtr();
	m_ScMW->mainWindowProgressBar->reset();
	QList<PageItem*> allItems;
	QStringList patterns = docPatterns.keys();
	int cc = 0;
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->imageIsAvailable)
				cc++;
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->imageIsAvailable)
				cc++;
		}
		allItems.clear();
	}
	for (QHash<int, PageItem*>::iterator it = FrameItems.begin(); it != FrameItems.end(); ++it)
	{
		PageItem *currItem = it.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem->imageIsAvailable)
				cc++;
		}
		allItems.clear();
	}
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if (currItem->imageIsAvailable)
					cc++;
			}
			allItems.clear();
		}
	}
	m_ScMW->mainWindowProgressBar->setMaximum((cc > 0) ? cc : 1);
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (!currItem->imageIsAvailable)
				continue;
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			if (applyNewRes)
				currItem->pixm.imgInfo.lowResType = m_docPrefsData.itemToolPrefs.imageLowResType;
			if (currItem->asLatexFrame())
				currItem->asLatexFrame()->rerunApplication(false);
			else
				loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
			currItem->AdjustPictScale();
			ca++;
			m_ScMW->mainWindowProgressBar->setValue(ca);
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
			if (!docPtr) return;
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (!currItem->imageIsAvailable)
				continue;
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			if (applyNewRes)
				currItem->pixm.imgInfo.lowResType = m_docPrefsData.itemToolPrefs.imageLowResType;
			if (currItem->asLatexFrame())
				currItem->asLatexFrame()->rerunApplication(false);
			else
				loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
			currItem->AdjustPictScale();
			ca++;
			m_ScMW->mainWindowProgressBar->setValue(ca);
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
			if (!docPtr) return;
		}
		allItems.clear();
	}

	for (QHash<int, PageItem*>::iterator it = FrameItems.begin(); it != FrameItems.end(); ++it)
	{
		PageItem *currItem = it.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (!currItem->imageIsAvailable)
				continue;
			bool fho = currItem->imageFlippedH();
			bool fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			if (applyNewRes)
				currItem->pixm.imgInfo.lowResType = m_docPrefsData.itemToolPrefs.imageLowResType;
			if (currItem->asLatexFrame())
				currItem->asLatexFrame()->rerunApplication(false);
			else
				loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
			currItem->AdjustPictScale();
			ca++;
			m_ScMW->mainWindowProgressBar->setValue(ca);
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
			if (!docPtr) return;
		}
		allItems.clear();
	}
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		if (pa.items.count() <= 0)
			continue;
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if (!currItem->imageIsAvailable)
					continue;
				bool fho = currItem->imageFlippedH();
				bool fvo = currItem->imageFlippedV();
				double imgX = currItem->imageXOffset();
				double imgY = currItem->imageYOffset();
				if (applyNewRes)
					currItem->pixm.imgInfo.lowResType = m_docPrefsData.itemToolPrefs.imageLowResType;
				if (currItem->asLatexFrame())
					currItem->asLatexFrame()->rerunApplication(false);
				else
					loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
				currItem->setImageXOffset(imgX);
				currItem->setImageYOffset(imgY);
				currItem->AdjustPictScale();
				ca++;
				m_ScMW->mainWindowProgressBar->setValue(ca);
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				if (!docPtr) return;
			}
			allItems.clear();
		}
		PageItem *ite = pa.items.at(0);
		double minx =  std::numeric_limits<double>::max();
		double miny =  std::numeric_limits<double>::max();
		double maxx = -std::numeric_limits<double>::max();
		double maxy = -std::numeric_limits<double>::max();
		double x1, x2, y1, y2;
		ite->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
	}
	regionsChanged()->update(QRectF());
	changed();
	m_ScMW->mainWindowProgressBar->reset();
}

int ScribusDoc::previewQuality()
{
	return itemToolPrefs().imageLowResType;
}


void ScribusDoc::removePict(QString name)
{
	bool updated=false;
	QList<PageItem*> allItems;
	for (int a = 0; a < DocItems.count(); ++a)
	{
		PageItem *currItem = DocItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((currItem->imageIsAvailable) && (currItem->Pfile == name))
			{
				currItem->imageIsAvailable = false;
				currItem->pixm = ScImage();
				updated=true;
			}
		}
		allItems.clear();
	}
	for (int a = 0; a < MasterItems.count(); ++a)
	{
		PageItem *currItem = MasterItems.at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((currItem->imageIsAvailable) && (currItem->Pfile == name))
			{
				currItem->imageIsAvailable = false;
				currItem->pixm = ScImage();
				updated=true;
			}
		}
		allItems.clear();
	}
	for (QHash<int, PageItem*>::iterator it = FrameItems.begin(); it != FrameItems.end(); ++it)
	{
		PageItem *currItem = it.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if ((currItem->imageIsAvailable) && (currItem->Pfile == name))
			{
				currItem->imageIsAvailable = false;
				currItem->pixm = ScImage();
				updated=true;
			}
		}
		allItems.clear();
	}
	QStringList patterns = docPatterns.keys();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		if (pa.items.count() <= 0)
			continue;
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *currItem = pa.items.at(o);
			if (currItem->isGroup())
				allItems = currItem->getAllChildren();
			else
				allItems.append(currItem);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				currItem = allItems.at(ii);
				if ((currItem->imageIsAvailable) && (currItem->Pfile == name))
				{
					currItem->imageIsAvailable = false;
					currItem->pixm = ScImage();
					updated=true;
				}
			}
			allItems.clear();
		}
		PageItem *ite = pa.items.at(0);
		double minx =  std::numeric_limits<double>::max();
		double miny =  std::numeric_limits<double>::max();
		double maxx = -std::numeric_limits<double>::max();
		double maxy = -std::numeric_limits<double>::max();
		double x1, x2, y1, y2;
		ite->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
		docPatterns[patterns[c]].pattern = ite->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
	}
	if (updated)
	{
		regionsChanged()->update(QRectF());
		changed();
	}
}


void ScribusDoc::updatePic()
{
	//TODO? Getting the pointer with m_Selection->itemAt(i) over and over again in the loop 
	// seems to be a waste of ressources
	uint docSelectionCount = m_Selection->count();
	if (docSelectionCount <= 0)
		return;

	bool toUpdate=false;
	for (uint i = 0; i < docSelectionCount; ++i)
	{
		PageItem* currItem = m_Selection->itemAt(i);
		if (!currItem)
			continue;
		m_updateManager.setUpdatesDisabled();
		if (currItem->asLatexFrame())
		{
			PageItem_LatexFrame *latexframe = currItem->asLatexFrame();
			latexframe->rerunApplication();
			toUpdate = true;
		}
#ifdef HAVE_OSG
		else if ((currItem->asImageFrame()) || (currItem->asOSGFrame()))
#else
		else if (currItem->asImageFrame())
#endif
		{
			if (currItem->imageIsAvailable)
			{
				int fho = currItem->imageFlippedH();
				int fvo = currItem->imageFlippedV();
				double imgX = currItem->imageXOffset();
				double imgY = currItem->imageYOffset();
				loadPict(currItem->Pfile, currItem, true);
				currItem->setImageFlippedH(fho);
				currItem->setImageFlippedV(fvo);
				currItem->setImageXOffset(imgX);
				currItem->setImageYOffset(imgY);
				toUpdate = true;
			}
		}
		m_updateManager.setUpdatesEnabled();
	}
	if (toUpdate)
		regionsChanged()->update(QRectF());
}


//TODO replace with the ScribusDoc::deleteTaggedItems
void ScribusDoc::removeLayer(int l, bool dl)
{
	//FIXME: stop using m_View
	m_View->Deselect();
	Selection tmpSelection(this, false);
	int newLayerID = 0;
	// Find the new layer identifier
	const ScLayer* lcurr = Layers.layerByID(l);
	if (lcurr)
	{
		const ScLayer* lbelow = Layers.layerBelow(lcurr->Level);
		if (lcurr == lbelow)
		{
			const ScLayer* labove = Layers.layerAbove(lcurr->Level);
			if (labove)
				newLayerID = labove->ID;
		}
		else if (lbelow)
			newLayerID = lbelow->ID;
	}
	for (int b = 0; b < MasterItems.count(); ++b)
	{
		if (MasterItems.at(b)->LayerID == l)
		{
			if (dl)
			{
				tmpSelection.addItem(MasterItems.at(b));
				MasterItems.at(b)->setLocked(false);
			}
			else
				MasterItems.at(b)->setLayer(newLayerID);
		}
	}
	if (tmpSelection.count() != 0)
		itemSelection_DeleteItem(&tmpSelection);
	tmpSelection.clear();
	for (int b = 0; b < DocItems.count(); ++b)
	{
		PageItem* currItem = DocItems.at(b);
		if (currItem->LayerID == l)
		{
			if (dl)
			{
				tmpSelection.addItem(currItem);
				currItem->setLocked(false);
			}
			else
				currItem->setLayer(newLayerID);
		}
	}
	if (tmpSelection.count() != 0)
		itemSelection_DeleteItem(&tmpSelection);
	tmpSelection.clear();
	//FIXME signal these
	m_ScMW->rebuildLayersList();
	m_ScMW->updateLayerMenu();
	setActiveLayer(newLayerID);
}

void ScribusDoc::selectionChanged()
{
	if (m_ScMW->isObjectSpecificUndo())
	{
		uint docSelectionCount = m_Selection->count();
		if (docSelectionCount == 1)
			m_undoManager->showObject(m_Selection->itemAt(0)->getUId());
		else if (docSelectionCount == 0)
			m_undoManager->showObject(currentPage()->getUId());
		else
			m_undoManager->showObject(Um::NO_UNDO_STACK);
	}
}


void ScribusDoc::itemSelection_ToggleLock( )
{
	uint docSelectionCount=m_Selection->count();
	if (docSelectionCount != 0)
	{
		UndoTransaction activeTransaction;
		m_updateManager.setUpdatesDisabled();
		if (UndoManager::undoEnabled() && docSelectionCount > 1)
		{
			if (m_Selection->itemAt(0)->locked())
				activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::UnLock, 0, Um::IUnLock);
			else
				activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Lock, 0, Um::ILock);
		}
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			m_Selection->itemAt(a)->toggleLock();
			m_Selection->itemAt(a)->update();
		}
		if (activeTransaction)
			activeTransaction.commit();
		m_updateManager.setUpdatesEnabled();
		changed();
 		emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
	}
}


void ScribusDoc::itemSelection_ToggleSizeLock( )
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction activeTransaction;
		m_updateManager.setUpdatesDisabled();
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
		{
			if (m_Selection->itemAt(0)->sizeLocked())
				activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SizeUnLock, 0, Um::IUnLock);
			else
				activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SizeLock, 0, Um::ILock);
		}
		for (int i = 0; i < selectedItemCount; ++i)
		{
			m_Selection->itemAt(i)->toggleSizeLock();
			m_Selection->itemAt(i)->update();
		}
		if (activeTransaction)
			activeTransaction.commit();
		m_updateManager.setUpdatesEnabled();
		changed();
 		emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
	}
}


void ScribusDoc::itemSelection_ToggleImageShown()
{
	if (m_Selection->count() != 0)
	{
		m_updateManager.setUpdatesDisabled();
		for (int a = 0; a < m_Selection->count(); ++a)
		{
			PageItem_ImageFrame* imageItem=m_Selection->itemAt(a)->asImageFrame();
			if (imageItem==NULL)
				continue;
			imageItem->setImageVisible(!imageItem->imageVisible());
			imageItem->update();
		}
		m_updateManager.setUpdatesEnabled();
		changed();
		//Return to normal mode if in edit mode. We should not allow dragging of
		//an image in a frame if its not shown.
		if (appMode == modeEdit)
			m_ScMW->view->requestMode(modeNormal);
	}
}


void ScribusDoc::itemSelection_TogglePrintEnabled( )
{
	int docSelectionCount=m_Selection->count();
	if (docSelectionCount == 0)
		return;
	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (docSelectionCount > 1 && UndoManager::undoEnabled())
	{
		if (m_Selection->itemAt(0)->printEnabled())
			activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::DisablePrint, 0, Um::IDisablePrint);
		else
			activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::EnablePrint, 0, Um::IEnablePrint);
	}
	for (int i = 0; i < docSelectionCount; ++i)
	{
		m_Selection->itemAt(i)->togglePrintEnabled();
		m_Selection->itemAt(i)->update();
	}
	if (activeTransaction)
		activeTransaction.commit();
	m_updateManager.setUpdatesEnabled();
	changed();
	emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
}

void ScribusDoc::itemSelection_Transform(int nrOfCopies, QTransform matrix, int basepoint)
{
	int docSelectionCount=m_Selection->count();
	if (docSelectionCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	if (nrOfCopies == 0)
	{
		double gx, gy, gh, gw;
		PageItem *currItem = m_Selection->itemAt(0);
		if (m_Selection->count() == 1)
		{
			gx = currItem->xPos();
			gy = currItem->yPos();
			gw = currItem->width();
			gh = currItem->height();
		}
		else
			m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		for (int a = 0; a < m_Selection->count(); ++a)
		{
			PageItem *currItem = m_Selection->itemAt(a);
			double deltaX = currItem->xPos() - gx;
			double deltaY = currItem->yPos() - gy;
			QTransform matrixPre = QTransform();
			QTransform matrixAft = QTransform();
			switch (basepoint)
			{
			case 2:
				matrixPre.translate(-gw/2.0, -gh/2.0);
				matrixAft.translate(gw/2.0, gh/2.0);
				break;
			case 4:
				matrixPre.translate(-gw, -gh);
				matrixAft.translate(gw, gh);
				break;
			case 3:
				matrixPre.translate(0, -gh);
				matrixAft.translate(0, gh);
				break;
			case 1:
				matrixPre.translate(-gw, 0);
				matrixAft.translate(gw, 0);
				break;
			}
			if (UndoManager::undoEnabled())
			{
				ScItemState<QList<QTransform> > *state = new ScItemState<QList<QTransform> >(Um::Transform);
				state->set("TRANSFORM");
				state->set("DX",deltaX);
				state->set("DY",deltaY);
				state->set("POSX",currItem->xPos());
				state->set("POSY",currItem->yPos());
				QList<QTransform> l;
				l.append(matrixPre);
				l.append(matrix);
				l.append(matrixAft);
				state->setItem(l);
				m_undoManager->action(currItem, state);
			}

			currItem->PoLine.translate(deltaX, deltaY);
			currItem->PoLine.map(matrixPre);
			currItem->PoLine.map(matrix);
			currItem->PoLine.map(matrixAft);
			currItem->PoLine.translate(-deltaX, -deltaY);
			currItem->ContourLine.translate(deltaX, deltaY);
			currItem->ContourLine.map(matrixPre);
			currItem->ContourLine.map(matrix);
			currItem->ContourLine.map(matrixAft);
			currItem->ContourLine.translate(-deltaX, -deltaY);
//			currItem->Frame = false;
			currItem->ClipEdited = true;
//			currItem->FrameType = 3;
			m_undoManager->setUndoEnabled(false);
			adjustItemSize(currItem);
			m_undoManager->setUndoEnabled(true);
		}
	}
	else
	{
		QList<PageItem*> Elements;
		bool savedAlignGrid = SnapGrid;
		bool savedAlignGuides = SnapGuides;
		bool savedAlignElement = SnapElement;
		SnapGrid  = false;
		SnapGuides = false;
		SnapElement = false;
		DoDrawing = false;
		view()->updatesOn(false);
		m_Selection->delaySignalsOn();
		QTransform comulatedMatrix = matrix;
		PageItem *currItem = m_Selection->itemAt(0);
		Elements.append(currItem);
		int rotBack = rotationMode();
		setRotationMode ( 0 );
		ScriXmlDoc xmlDoc;
		QString copyBuffer = xmlDoc.WriteElem(this, m_Selection);
		view()->Deselect(true);
		for (int b = 0; b < nrOfCopies; b++)
		{
			uint ac = Items->count();
			xmlDoc.ReadElem(copyBuffer, m_appPrefsData.fontPrefs.AvailFonts, this, currentPage()->xOffset(), currentPage()->yOffset(), false, true, m_appPrefsData.fontPrefs.GFontSub);
			for (int as = ac; as < Items->count(); ++as)
			{
				PageItem* bItem = Items->at(as);
				m_Selection->addItem(bItem);
			}
			double gx, gy, gh, gw;
			currItem = m_Selection->itemAt(0);
			if (m_Selection->count() == 1)
			{
				gx = currItem->xPos();
				gy = currItem->yPos();
				gw = currItem->width();
				gh = currItem->height();
			}
			else
				m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			for (int a = 0; a < m_Selection->count(); ++a)
			{
				currItem = m_Selection->itemAt(a);
				double deltaX = currItem->xPos() - gx;
				double deltaY = currItem->yPos() - gy;
				QTransform matrixPre = QTransform();
				QTransform matrixAft = QTransform();
				switch (basepoint)
				{
				case 2:
					matrixPre.translate(-gw/2.0, -gh/2.0);
					matrixAft.translate(gw/2.0, gh/2.0);
					break;
				case 4:
					matrixPre.translate(-gw, -gh);
					matrixAft.translate(gw, gh);
					break;
				case 3:
					matrixPre.translate(0, -gh);
					matrixAft.translate(0, gh);
					break;
				case 1:
					matrixPre.translate(-gw, 0);
					matrixAft.translate(gw, 0);
					break;
				}

				if (UndoManager::undoEnabled())
				{
					ScItemState<QList<QTransform> > *state = new ScItemState<QList<QTransform> >(Um::Transform);
					state->set("TRANSFORM");
					state->set("DX",deltaX);
					state->set("DY",deltaY);
					state->set("POSX",currItem->xPos());
					state->set("POSY",currItem->yPos());
					QList<QTransform> l;
					l.append(matrixPre);
					l.append(matrix);
					l.append(matrixAft);
					state->setItem(l);
					m_undoManager->action(currItem, state);
				}
				currItem->PoLine.translate(deltaX, deltaY);
				currItem->PoLine.map(matrixPre);
				currItem->PoLine.map(comulatedMatrix);
				currItem->PoLine.map(matrixAft);
				currItem->PoLine.translate(-deltaX, -deltaY);
				currItem->ContourLine.translate(deltaX, deltaY);
				currItem->ContourLine.map(matrixPre);
				currItem->ContourLine.map(comulatedMatrix);
				currItem->ContourLine.map(matrixAft);
				currItem->ContourLine.translate(-deltaX, -deltaY);
				currItem->ClipEdited = true;
				m_undoManager->setUndoEnabled(false);
				adjustItemSize(currItem);
				m_undoManager->setUndoEnabled(true);
				Elements.append(currItem);
			}
			comulatedMatrix *= matrix;
			m_Selection->clear();
		}
		for (int c = 0; c < Elements.count(); ++c)
		{
			m_Selection->addItem(Elements.at(c), true);
		}
		m_Selection->setGroupRect();
		setRotationMode (rotBack);
		SnapGrid  = savedAlignGrid;
		SnapGuides = savedAlignGuides;
		SnapElement = savedAlignElement;
		DoDrawing = true;
		m_Selection->delaySignalsOff();
		view()->updatesOn(true);
		m_Selection->connectItemToGUI();
	}
	m_updateManager.setUpdatesEnabled();
	regionsChanged()->update(QRectF());
	changed();
}


void ScribusDoc::itemSelection_FlipH()
{
	int docSelectionCount=m_Selection->count();
	if (docSelectionCount == 0)
		return;
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::FlipH, 0, Um::IFlipH);
	if (docSelectionCount > 1)
	{
		double gx, gy, gh, gw, ix, iy, iw, ih;
		m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		for (int i = 0; i < docSelectionCount; ++i)
		{
			PageItem* currItem=m_Selection->itemAt(i);
			currItem->getBoundingRect(&ix, &iy, &iw, &ih);
			double dx =  ((gw / 2.0) -  ((ix - gx) + (iw - ix) / 2.0)) * 2.0;
			if (currItem->rotation() != 0.0)
			{
				double ix2, iy2, iw2, ih2;
				double newRotation = -currItem->rotation();
				if (currItem->itemType() == PageItem::Line)
					newRotation = 180 - currItem->rotation();
				while (newRotation < 0)
					newRotation += 360.0;
				while (newRotation >= 360.0)
					newRotation -= 360.0;
				currItem->setRotation(newRotation);
				currItem->setRedrawBounding();
				currItem->getBoundingRect(&ix2, &iy2, &iw2, &ih2);
				currItem->moveBy(ix-ix2, iy-iy2, false);
				currItem->setRedrawBounding();
			}
			if (currItem->isImageFrame() || currItem->isTextFrame() || currItem->isLatexFrame() || currItem->isOSGFrame() || currItem->isSymbol() || currItem->isGroup() || currItem->isSpiral())
				currItem->flipImageH();
			if (currItem->itemType() != PageItem::Line)
				MirrorPolyH(currItem);
			currItem->moveBy(dx, 0, false);
			currItem->GrStartX = currItem->width() - currItem->GrStartX;
			currItem->GrEndX = currItem->width() - currItem->GrEndX;
			if (currItem->isArc())
			{
				PageItem_Arc *ar = currItem->asArc();
				ar->arcStartAngle = (180 - ar->arcStartAngle) - ar->arcSweepAngle;
				if (ar->arcStartAngle < 0)
					ar->arcStartAngle += 360;
				else if (ar->arcStartAngle > 360)
					ar->arcStartAngle -= 360;
				ar->recalcPath();
				adjustItemSize(currItem);
				emit updateEditItem();
			}
			else if (currItem->isRegularPolygon())
			{
				PageItem_RegularPolygon *ar = currItem->asRegularPolygon();
				ar->polyRotation *= -1;
				ar->polyInnerRot *= -1;
				ar->recalcPath();
				emit updateEditItem();
			}
			else if (currItem->isSpiral())
				emit updateEditItem();
			currItem->setRedrawBounding();
		}
	}
	else
	{
		PageItem* currItem=m_Selection->itemAt(0);
		if (currItem->isImageFrame() || currItem->isTextFrame() || currItem->isLatexFrame() || currItem->isOSGFrame() || currItem->isSymbol() || currItem->isGroup() || currItem->isSpiral())
			currItem->flipImageH();
		if (currItem->itemType() != PageItem::Line)
			MirrorPolyH(currItem);
		else
		{
			double ix2, iy2, iw2, ih2, ix, iy, iw, ih;
			currItem->getBoundingRect(&ix, &iy, &iw, &ih);
			double newRotation = 180 - currItem->rotation();
			while (newRotation < 0)
				newRotation += 360.0;
			while (newRotation >= 360.0)
				newRotation -= 360.0;
			currItem->setRotation(newRotation);
			currItem->setRedrawBounding();
			currItem->getBoundingRect(&ix2, &iy2, &iw2, &ih2);
			currItem->moveBy(ix-ix2, iy-iy2, false);
			currItem->setRedrawBounding();
		}
		currItem->GrStartX = currItem->width() - currItem->GrStartX;
		currItem->GrEndX = currItem->width() - currItem->GrEndX;
		if (currItem->isArc())
		{
			PageItem_Arc *ar = currItem->asArc();
			ar->arcStartAngle = (180 - ar->arcStartAngle) - ar->arcSweepAngle;
			if (ar->arcStartAngle < 0)
				ar->arcStartAngle += 360;
			else if (ar->arcStartAngle > 360)
				ar->arcStartAngle -= 360;
			ar->recalcPath();
			adjustItemSize(currItem);
			emit updateEditItem();
		}
		else if (currItem->isRegularPolygon())
		{
			PageItem_RegularPolygon *ar = currItem->asRegularPolygon();
			ar->polyRotation *= -1;
			ar->polyInnerRot *= -1;
			ar->recalcPath();
			emit updateEditItem();
		}
		else if (currItem->isSpiral())
			emit updateEditItem();
	}
	if (trans)
		trans.commit();
	regionsChanged()->update(QRectF());
	changed();
	emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
}


void ScribusDoc::itemSelection_FlipV()
{
	int docSelectionCount=m_Selection->count();
	if (docSelectionCount == 0)
		return;
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::FlipV, 0, Um::IFlipV);
	if (docSelectionCount > 1)
	{
		double gx, gy, gh, gw, ix, iy, iw, ih;
		m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		for (int i = 0; i < docSelectionCount; ++i)
		{
			PageItem* currItem=m_Selection->itemAt(i);
			currItem->getBoundingRect(&ix, &iy, &iw, &ih);
			double dx =  ((gh / 2.0) -  ((iy - gy) + (ih - iy) / 2.0)) * 2.0;
			if (currItem->rotation() != 0.0)
			{
				double ix2, iy2, iw2, ih2;
				currItem->rotateBy(currItem->rotation() * -2.0);
				currItem->setRedrawBounding();
				currItem->getBoundingRect(&ix2, &iy2, &iw2, &ih2);
				currItem->moveBy(ix-ix2, iy-iy2, false);
				currItem->setRedrawBounding();
			}
			if (currItem->isImageFrame() || currItem->isTextFrame() || currItem->isLatexFrame() || currItem->isOSGFrame() || currItem->isSymbol() || currItem->isGroup() || currItem->isSpiral())
				currItem->flipImageV();
			if (currItem->itemType() != PageItem::Line)
				MirrorPolyV(currItem);
			currItem->moveBy(0, dx, false);
			currItem->GrStartY = currItem->height() - currItem->GrStartY;
			currItem->GrEndY = currItem->height() - currItem->GrEndY;
			if (currItem->isArc())
			{
				PageItem_Arc *ar = currItem->asArc();
				ar->arcStartAngle *= -1;
				ar->arcStartAngle -= ar->arcSweepAngle;
				if (ar->arcStartAngle < 0)
					ar->arcStartAngle += 360;
				else if (ar->arcStartAngle > 360)
					ar->arcStartAngle -= 360;
				ar->recalcPath();
				adjustItemSize(currItem);
				emit updateEditItem();
			}
			else if (currItem->isRegularPolygon())
			{
				PageItem_RegularPolygon *ar = currItem->asRegularPolygon();
				ar->polyRotation = 180.0 - ar->polyRotation;
				ar->polyRotation *= -1;
				ar->polyInnerRot *= -1;
				ar->recalcPath();
				emit updateEditItem();
			}
			else if (currItem->isSpiral())
				emit updateEditItem();
			currItem->setRedrawBounding();
		}
		regionsChanged()->update(QRectF());
	}
	else
	{
		PageItem* currItem=m_Selection->itemAt(0);
		if (currItem->isImageFrame() || currItem->isTextFrame() || currItem->isLatexFrame() || currItem->isOSGFrame() || currItem->isSymbol() || currItem->isGroup() || currItem->isSpiral())
			currItem->flipImageV();
		if (currItem->itemType() != PageItem::Line)
			MirrorPolyV(currItem);
		else
		{
			double ix2, iy2, iw2, ih2, ix, iy, iw, ih;
			currItem->getBoundingRect(&ix, &iy, &iw, &ih);
			currItem->rotateBy(currItem->rotation() * -2.0);
			currItem->setRedrawBounding();
			currItem->getBoundingRect(&ix2, &iy2, &iw2, &ih2);
			currItem->moveBy(ix-ix2, iy-iy2, false);
			currItem->setRedrawBounding();
		}
		currItem->GrStartY = currItem->height() - currItem->GrStartY;
		currItem->GrEndY = currItem->height() - currItem->GrEndY;
		if (currItem->isArc())
		{
			PageItem_Arc *ar = currItem->asArc();
			ar->arcStartAngle *= -1;
			ar->arcStartAngle -= ar->arcSweepAngle;
			if (ar->arcStartAngle < 0)
				ar->arcStartAngle += 360;
			else if (ar->arcStartAngle > 360)
				ar->arcStartAngle -= 360;
			ar->recalcPath();
			adjustItemSize(currItem);
			emit updateEditItem();
		}
		else if (currItem->isRegularPolygon())
		{
			PageItem_RegularPolygon *ar = currItem->asRegularPolygon();
			ar->polyRotation = 180.0 - ar->polyRotation;
			ar->polyRotation *= -1;
			ar->polyInnerRot *= -1;
			ar->recalcPath();
			emit updateEditItem();
		}
		else if (currItem->isSpiral())
			emit updateEditItem();
		regionsChanged()->update(QRectF());
	}
	if (trans)
		trans.commit();
	changed();
	emit firstSelectedItemType(m_Selection->itemAt(0)->itemType());
}

void ScribusDoc::itemSelection_Rotate(double angle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);

	if (itemSelection->count() == 0) return;
	
	if (itemSelection->count() > 1)
		rotateGroup(angle, itemSelection);
	else if (itemSelection->count() == 1)
		rotateItem(angle, itemSelection->itemAt(0));
	changed();
}

void ScribusDoc::itemSelection_ChangePreviewResolution(int id)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::Selection, Um::IGroup,Um::ResTyp, "", Um::IImageFrame);
	PageItem *currItem;
	bool found=false;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Selection->itemAt(i);
		if (!currItem || !currItem->isImageFrame())
			continue;
		currItem->setResolution(id);
		found = true;
	}
	if (activeTransaction)
		activeTransaction.commit();
	if (!found) //No image frames in the current selection!
		return;
	updatePic();
}

void ScribusDoc::allItems_ChangePreviewResolution(int id)
{
	bool found=false;
	QList<PageItem*> allItems;
	for (int c=0; c<DocItems.count(); ++c)
	{
		PageItem *currItem = DocItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem!=NULL)
			{
				if (currItem->asImageFrame())
				{
					currItem->pixm.imgInfo.lowResType = id;
					if (!found)
						found=true;
				}
			}
		}
		allItems.clear();
	}
	for (int c=0; c<MasterItems.count(); ++c)
	{
		PageItem *currItem = MasterItems.at(c);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem!=NULL)
			{
				if (currItem->asImageFrame())
				{
					currItem->pixm.imgInfo.lowResType = id;
					if (!found)
						found=true;
				}
			}
		}
		allItems.clear();
	}
	for (QHash<int, PageItem*>::iterator it = FrameItems.begin(); it != FrameItems.end(); ++it)
	{
		PageItem *currItem = it.value();
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (currItem!=NULL)
			{
				if (currItem->asImageFrame())
				{
					currItem->pixm.imgInfo.lowResType = id;
					if (!found)
						found=true;
				}
			}
		}
		allItems.clear();
	}
	
	if (!found) //No image frames in the current selection!
		return;
	recalcPicturesRes();
	changed();
}

void ScribusDoc::item_setFrameShape(PageItem* item, int frameType, int count, double* points)
{
	if ((item->itemType() == PageItem::PolyLine) || (item->itemType() == PageItem::PathText))
			return;
	UndoTransaction activeTransaction;
	if (m_undoManager->undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::Selection,Um::IImageFrame,Um::ChangeShapeType,"",Um::IBorder);

	if (frameType!=0)
		item->setCornerRadius(0);
	if (UndoManager::undoEnabled())
	{
		// Store shape info in this form:
		// CHANGE_SHAPE_TYPE - ID of the undo operation
		// OLD_FRAME_TYPE - original frame type
		// NEW_FRAME_TYPE - change of frame type
		// binary QPair<FPointArray, FPointArray> - .first original shape, .second new shape
		ScItemState<QPair<FPointArray, FPointArray> >* is = new ScItemState<QPair<FPointArray,FPointArray> >(Um::ChangeShapeType, "", Um::IBorder);
		is->set("CHANGE_SHAPE_TYPE");
		is->set("OLD_FRAME_TYPE", item->FrameType);
		is->set("NEW_FRAME_TYPE", frameType);
		// HACK: this is propably Evil Code (TM). I have to find better way...
		FPointArray newShape;
		int ix = 0;
		for (int i = 0; i < count/2; ++i)
		{
			double x = item->width()  * points[ix] / 100.0;
			double y = item->height() * points[ix+1] / 100.0;
			newShape.addPoint(x, y);
			ix += 2;
		}
		// HACK: end of hack
		is->setItem(qMakePair(item->shape(), newShape));
		UndoManager::instance()->action(item, is);
	}

	switch (frameType)
	{
		case 0:
			item->SetRectFrame();
			this->setRedrawBounding(item);
			break;
		case 1:
			item->SetOvalFrame();
			this->setRedrawBounding(item);
			break;
		default:
			item->SetFrameShape(count, points);
			if (item->isSymbol() || item->isGroup())
			{
				if (item->imageFlippedH())
				{
					QTransform ma;
					ma.scale(-1, 1);
					item->PoLine.map(ma);
					item->PoLine.translate(item->width(), 0);
				}
				if (item->imageFlippedV())
				{
					QTransform ma;
					ma.scale(1, -1);
					item->PoLine.map(ma);
					item->PoLine.translate(0, item->height());
				}
			}
			this->setRedrawBounding(item);
			item->FrameType = frameType + 2;
			break;
	}
	item->update();
	changed();
	if (activeTransaction)
		activeTransaction.commit();
}

void ScribusDoc::itemSelection_ClearItem(Selection* customSelection, bool useWarning)
{
	Selection* itemSelection = (customSelection != 0) ? customSelection : m_Selection;
	assert(itemSelection != 0);

	int selectedItemCount = itemSelection->count();
	if (selectedItemCount <= 0)
		return;
	if (ScCore->usingGUI() && useWarning)
	{
		int t = ScMessageBox::warning(m_ScMW, CommonStrings::trWarning, tr("Do you really want to clear the content of all selected frames?"),
							QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
							QMessageBox::No,	// GUI default
							QMessageBox::Yes);	// batch default
		if (t == QMessageBox::No)
			return;
	}
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem->asImageFrame())
		{
			if ((ScCore->fileWatcher->files().contains(currItem->Pfile) != 0) && (currItem->imageIsAvailable))
				ScCore->fileWatcher->removeFile(currItem->Pfile);
		}
		currItem->clearContents();
	}
	regionsChanged()->update(QRectF());
	changed();
}

void ScribusDoc::itemSelection_TruncateItem(Selection* customSelection)
{
	Selection* itemSelection = (customSelection != 0) ? customSelection : m_Selection;
	assert(itemSelection != 0);

	int selectedItemCount = itemSelection->count();
	if (selectedItemCount <= 0)
		return;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		currItem->truncateContents();
	}
	regionsChanged()->update(QRectF());
	changed();
}

QList<PageItem*>* ScribusDoc::GroupOfItem(QList<PageItem*>* itemList, PageItem* item)
{
	if (itemList->contains(item))
		return itemList;
	else
	{
		for (int i = 0; i < itemList->count(); i++)
		{
			if (itemList->at(i)->isGroup())
			{
				QList<PageItem*>* ite = GroupOfItem(&itemList->at(i)->groupItemList, item);
				if (ite != NULL)
					return ite;
			}
		}
		return NULL;
	}
	return NULL;
}

void ScribusDoc::itemSelection_DeleteItem(Selection* customSelection, bool forceDeletion)
{
	if (appMode == modeEditClip)
		return;
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	QList<PageItem*> delItems;
	QList<PageItem*> textInteractionItems;// text frames possibly interested in removal of selected items
	PageItem *currItem;
	QList<PageItem*>* itemList = Items;
	uint offs = 0;
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	itemSelection->delaySignalsOn();
	for (int de = 0; de < selectedItemCount; ++de)
	{
		currItem = itemSelection->itemAt(offs);
		if (((currItem->isSingleSel) && (!Items->contains(currItem))) || (currItem->locked()))
		{
			if (currItem->locked())
			{
				offs++;
				continue;
			}
		}
		//#12371 don't delete group items
		if(currItem->isGroupChild())
			continue;
		//CB FIXME remove this and include of storyeditor.h too
		if ((currItem->asTextFrame() || currItem->asPathText()) && currItem==m_ScMW->storyEditor->currentItem() && this==m_ScMW->storyEditor->currentDocument())
		{
			if (forceDeletion)
				m_ScMW->storyEditor->setCurrentDocumentAndItem(this, NULL);
			else
			{
				ScMessageBox::critical(m_ScMW, tr("Cannot Delete In-Use Item"), tr("The item %1 is currently being edited by Story Editor. The delete operation will be cancelled").arg(currItem->itemName()));
				itemSelection->delaySignalsOff();
				return;
			}
		}
		if (currItem->textFlowMode() != PageItem::TextFlowDisabled)
		{
			int id = Items->indexOf(currItem) - 1;
			for (int tIdx = id; tIdx >= 0; --tIdx)
			{
				if ( (Items->at(tIdx)->asTextFrame())
					&& (!itemSelection->containsItem(Items->at(tIdx)))
					&& (currItem->getBoundingRect().intersects(Items->at(tIdx)->getBoundingRect()))
					&& (!textInteractionItems.contains(Items->at(tIdx))) )
				{
					textInteractionItems.append( Items->at(tIdx) );
				}
			}
		}
		if (selectedItemCount <= Um::ItemsInvolvedLimit)
			tooltip += "\t" + currItem->getUName() + "\n";
		delItems.append(itemSelection->takeItem(offs));
	}
	if (delItems.count() == 0)
	{
		itemSelection->delaySignalsOff();
		return;
	}

	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled()) //always create transaction or check if item is reference for any mark or contains any mark or is welded etc
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
														  Um::Delete, tooltip, Um::IDelete);
	
	selectedItemCount = delItems.count();
	for (int de = 0; de < selectedItemCount; ++de)
	{
		currItem = delItems.at(selectedItemCount - (de + 1));
		itemList = GroupOfItem(Items, currItem);
		if (itemList == NULL)
			continue;
		if ((currItem->asImageFrame()) && ((ScCore->fileWatcher->files().contains(currItem->Pfile) != 0) && (currItem->imageIsAvailable)))
			ScCore->fileWatcher->removeFile(currItem->Pfile);
		//delete marks pointed to that item
		for (int a=0; a < m_docMarksList.count(); a++)
		{
			Mark* m = m_docMarksList.at(a);
			Q_ASSERT(m != NULL);
			if (m->isType(MARK2ItemType) && (m->getItemPtr() == currItem))
			{
				setUndoDelMark(m);
				eraseMark(m, true, NULL, true);
			}
		}
		if (currItem->isNoteFrame())
		{
			if (currItem->itemText.length() >0)
			{
				currItem->itemText.selectAll();
				currItem->asTextFrame()->deleteSelectedTextFromFrame();
				if (currItem->asNoteFrame()->masterFrame())
					currItem->asNoteFrame()->masterFrame()->invalid = true;
			}
			if (!UndoManager::undoEnabled() || forceDeletion || currItem->isAutoNoteFrame())
			{
				itemList->removeAll(currItem);
				delNoteFrame(currItem->asNoteFrame(), false, false);
				continue;
			}
		}
		else
		{
			if (currItem->asTextFrame() && !currItem->hasLinks())
			{
				currItem->itemText.selectAll();
				currItem->asTextFrame()->removeMarksFromText(true);
				currItem->asTextFrame()->delAllNoteFrames(false);
				currItem->itemText.deselectAll();
			}
			if (currItem->asTextFrame())
				currItem->dropLinks();
		}
		if (currItem->isWelded())
			currItem->unWeld();
		if (currItem->isBookmark)
			//CB From view   emit DelBM(currItem);
			m_ScMW->DelBookMark(currItem);
		if (UndoManager::undoEnabled() && (selectedItemCount > 0) && !forceDeletion)
		{
			ScItemState< QList<PageItem*> > *is = new ScItemState< QList<PageItem*> >(Um::Delete + " " + currItem->getUName(), "", Um::IDelete);
			is->setItem(delItems);
			is->set("DELETE_ITEM");
			is->set("ITEMID", itemList->indexOf(currItem));
			is->set("ID", selectedItemCount - (de + 1));
			m_undoManager->action(Pages->at(0), is, currItem->getUPixmap());
		}
		itemList->removeAll(currItem);
//		undoManager->action(Pages->at(0), is, currItem->getUPixmap());
		if (forceDeletion)
			delete currItem;
	}
	itemSelection->delaySignalsOff();
	if (activeTransaction)
		activeTransaction.commit();
	// JG resetting ElemToLink fixes #5629
	ElemToLink = NULL;
	
	foreach(PageItem* tii, textInteractionItems)
	{
		tii->update();
	}
	
	regionsChanged()->update(QRectF());
	if (m_View)
		m_View->setCursor(QCursor(Qt::ArrowCursor));

	if (itemSelection->isGUISelection() && !itemSelection->signalsDelayed())
	{
		if (itemSelection->count() == 0)
			emit firstSelectedItemType(-1);
		else
			itemSelection->itemAt(0)->emitAllToGUI();
	}
	changed();
}


void ScribusDoc::itemSelection_SetItemFillTransparency(double t)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = m_Selection->itemAt(i);
		if (currItem->isGroup())
			continue;
		currItem->setFillTransparency(t);
	}
	regionsChanged()->update(QRectF());
	changed();
}


void ScribusDoc::itemSelection_SetItemLineTransparency(double t)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = m_Selection->itemAt(i);
		currItem->setLineTransparency(t);
	}
	regionsChanged()->update(QRectF());
	changed();
}


void ScribusDoc::itemSelection_SetItemFillBlend(int t)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = m_Selection->itemAt(i);
		if (currItem->isGroup())
			continue;
		currItem->setFillBlendmode(t);
	}
	regionsChanged()->update(QRectF());
	changed();
	if (activeTransaction)
	{
		activeTransaction.commit(Um::Selection,
								 Um::IGroup,
								 Um::BlendMode,
								 "",
								 Um::IGroup);
	}
}


void ScribusDoc::itemSelection_SetItemLineBlend(int t)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = m_Selection->itemAt(i);
		currItem->setLineBlendmode(t);
	}
	regionsChanged()->update(QRectF());
	changed();
	if (activeTransaction)
	{
		activeTransaction.commit(Um::Selection,
								 Um::IGroup,
								 Um::BlendMode,
								 "",
								 Um::IGroup);
	}
}


void ScribusDoc::itemSelection_SetLineGradient(VGradient& newGradient, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem;
		currItem = itemSelection->itemAt(i);
		currItem->setStrokeGradient(newGradient);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetFillGradient(VGradient& newGradient, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem;
		currItem = itemSelection->itemAt(i);
		currItem->setFillGradient(newGradient);
		if (currItem->gradientType() == 13)
			currItem->createConicalMesh();
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetMaskGradient(VGradient& newGradient, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem;
		currItem = itemSelection->itemAt(i);
		currItem->setMaskGradient(newGradient);
		currItem->update();
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}


void ScribusDoc::itemSelection_SetOverprint(bool overprint, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	m_updateManager.setUpdatesDisabled();
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem* currItem = itemSelection->itemAt(i);
		currItem->setOverprint(overprint);
		currItem->update();
	}
	if (activeTransaction)
	{
		activeTransaction.commit(Um::Selection,
								 Um::IGroup,
							     Um::Overprint,
								 "",
								 Um::IGroup);
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_DoHyphenate()
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = m_Selection->itemAt(i);
		docHyphenator->slotHyphenate(currItem);
	}
	//FIXME: stop using m_View
	m_View->DrawNew(); //CB draw new until NLS for redraw through text chains
	changed();
}

void ScribusDoc::itemSelection_DoDeHyphenate()
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount == 0)
		return;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = m_Selection->itemAt(i);
		docHyphenator->slotDeHyphenate(currItem);
	}
	//FIXME: stop using m_View
	m_View->DrawNew(); //CB draw new until NLS for redraw through text chains
	changed();
}

void ScribusDoc::itemSelection_SendToLayer(int layerID)
{
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount != 0)
	{
		UndoTransaction activeTransaction;
		if (UndoManager::undoEnabled() && selectedItemCount > 1)
			activeTransaction = m_undoManager->beginTransaction();
		QString tooltip = Um::ItemsInvolved + "\n";
		if (selectedItemCount > Um::ItemsInvolvedLimit)
			tooltip = Um::ItemsInvolved2 + "\n";
		for (int i = 0; i < selectedItemCount; ++i)
		{
			PageItem *currItem = m_Selection->itemAt(i);
			currItem->setLayer(layerID);
			if (selectedItemCount <= Um::ItemsInvolvedLimit)
				tooltip += "\t" + currItem->getUName() + "\n";
		}
		if (activeTransaction)
		{
			activeTransaction.commit(Um::Selection,
									 Um::IGroup,
									 Um::SendToLayer,
									 tooltip,
									 Um::ILayerAction);
		}
	}
	m_View->Deselect(true);
	regionsChanged()->update(QRectF());
	changed();
}

void ScribusDoc::itemSelection_SetAlignment(int s, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(s));
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetDirection(int s, Selection* customSelection)
{
	ParagraphStyle newStyle;
	newStyle.setDirection(static_cast<ParagraphStyle::DirectionType>(s));
	itemSelection_ApplyParagraphStyle(newStyle, customSelection);
}

void ScribusDoc::itemSelection_SetImageOffset(double x, double y, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount = itemSelection->count();
	if (selectedItemCount <= 0)
		return;

	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction();
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		currItem->setImageXYOffset(x, y);
		if (currItem->imageClip.size() != 0)
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
			QTransform cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.rotate(currItem->imageRotation());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		if (selectedItemCount <= Um::ItemsInvolvedLimit)
			tooltip += "\t" + currItem->getUName() + "\n";
		currItem->update();
	}
	if (activeTransaction)
	{
		activeTransaction.commit(Um::Selection,
									Um::IGroup,
									Um::ImageOffset,
									tooltip,
									Um::IImageScaling);
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetImageScale(double x, double y, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount = itemSelection->count();
	if (selectedItemCount <= 0)
		return;

	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction();
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		currItem->setImageXYScale(x, y);
		if (currItem->imageClip.size() != 0)
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
			QTransform cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.rotate(currItem->imageRotation());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		if (selectedItemCount <= Um::ItemsInvolvedLimit)
			tooltip += "\t" + currItem->getUName() + "\n";
		currItem->update();
	}
	if (activeTransaction)
	{
		activeTransaction.commit(Um::Selection,
							Um::IGroup,
							Um::ImageScale,
							tooltip,
							Um::IImageScaling);
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetImageScaleAndOffset(double sx, double sy, double ox, double oy, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection !=0 );
	int selectedItemCount = itemSelection->count();
	if (selectedItemCount == 0)
		return;

	m_updateManager.setUpdatesDisabled();
	UndoTransaction outerTransaction;
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		outerTransaction = m_undoManager->beginTransaction();
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	for (int i = 0; i < selectedItemCount; ++i)
	{
		UndoTransaction activeTransaction;
		PageItem *currItem = itemSelection->itemAt(i);
		if (UndoManager::undoEnabled())
			activeTransaction = m_undoManager->beginTransaction();
		currItem->setImageXYScale(sx, sy);
		currItem->setImageXYOffset(ox/sx, oy/sy);
		if (currItem->imageClip.size() != 0)
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
			QTransform cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.rotate(currItem->imageRotation());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		if (selectedItemCount <= Um::ItemsInvolvedLimit)
			tooltip += "\t" + currItem->getUName() + "\n";
		if (activeTransaction)
		{
			activeTransaction.commit(Um::Selection,
									 Um::IImageFrame,
									 Um::ImageScale,
									 tooltip,
									 Um::IImageScaling);
		}
		currItem->update();
	}
	if (outerTransaction)
	{
		outerTransaction.commit(Um::Selection,
								Um::IGroup,
								Um::ImageScale,
								tooltip,
								Um::IImageScaling);
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetImageRotation(double rot, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection != 0);
	int selectedItemCount = itemSelection->count();
	if (selectedItemCount == 0)
		return;

	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = m_undoManager->beginTransaction(Um::Selection,Um::IImageFrame,Um::Rotate,"",Um::IRotate);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		currItem->setImageRotation(rot);
		if (currItem->imageClip.size() != 0)
		{
			currItem->imageClip = currItem->pixm.imgInfo.PDSpathData[currItem->pixm.imgInfo.usedPath].copy();
			QTransform cl;
			cl.translate(currItem->imageXOffset()*currItem->imageXScale(), currItem->imageYOffset()*currItem->imageYScale());
			cl.rotate(currItem->imageRotation());
			cl.scale(currItem->imageXScale(), currItem->imageYScale());
			currItem->imageClip.map(cl);
		}
		currItem->update();
	}
	if (trans)
		trans.commit();
	changed();
}

void ScribusDoc::buildAlignItemList(Selection* customSelection)
{
	Q_UNUSED(customSelection);
	//CB TODO Handling custom selections
	//Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Selection* itemSelection = m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	PageItem *currItem;
	struct AlignObjs Object;
	AObjects.clear();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = itemSelection->itemAt(i);
		currItem->getBoundingRect(&Object.x1, &Object.y1, &Object.x2, &Object.y2);
		Object.Group = 0;
		Object.ObjNr = Items->indexOf(currItem);
		Object.Object = currItem;
		AObjects.append(Object);
	}
	for (int i = 0; i < AObjects.count(); ++i)
	{
		AObjects[i].width = AObjects[i].x2 - AObjects[i].x1;
		AObjects[i].height = AObjects[i].y2 - AObjects[i].y1;
	}
}


bool ScribusDoc::startAlign(uint minObjects)
{
	buildAlignItemList();
	int alignObjectsCount = AObjects.count();
	if (alignObjectsCount < minObjects)
		return false;
		
	bool oneLocked=false;
	for (int i = 0; i < alignObjectsCount && !oneLocked; ++i)
	{
		if (AObjects[i].Object->locked())
			oneLocked = true;
	}
	int t = 2;
	if (oneLocked)
	{
		ScMessageBox msgBox;
		QPushButton *abortButton = msgBox.addButton(QMessageBox::Cancel);
		QPushButton *lockButton = msgBox.addButton(tr("&Unlock All"), QMessageBox::AcceptRole);
		QPushButton *unlockButton = msgBox.addButton(tr("&Skip locked objects"), QMessageBox::AcceptRole);
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(CommonStrings::trWarning);
		msgBox.setText( tr("Some objects are locked."));
		msgBox.setDefaultBatchButton(lockButton);
		msgBox.exec();
		if (msgBox.clickedButton() == abortButton)
			return false;
		else if (msgBox.clickedButton() == lockButton)
			t = 0;
		else if (msgBox.clickedButton() == unlockButton)
			t = 1;
	}
	
	QString targetTooltip;
	if (m_Selection->count() <= (int) Um::ItemsInvolvedLimit)
	{
		targetTooltip = Um::ItemsInvolved + "\n";
		for (int i = 0; i < m_Selection->count(); ++i)
			targetTooltip += "\t" + m_Selection->itemAt(i)->getUName() + "\n";
	}
	else
		targetTooltip = Um::ItemsInvolved2 + "\n";
	// Make the align action a single action in Action History
	m_alignTransaction = m_undoManager->beginTransaction(Um::Selection, 0, Um::AlignDistribute, targetTooltip, Um::IAlignDistribute);
	if (oneLocked && (t == 0))
	{
		for (int i = 0; i < alignObjectsCount; ++i)
		{
			if (AObjects[i].Object->locked())
				AObjects[i].Object->setLocked(false);
		}
	}
	return true;
}

void ScribusDoc::endAlign()
{
	changed();
	m_ScMW->HaveNewSel();
	for (int i = 0; i < m_Selection->count(); ++i)
	{
		setRedrawBounding(m_Selection->itemAt(i));
		m_Selection->itemAt(i)->invalidateLayout();
	}
	m_alignTransaction.commit(); // commit and send the action to the UndoManager
	m_alignTransaction.reset();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_AlignItemRight(int i, double newX, AlignMethod how)
{
	double diff=newX-AObjects[i].x2;
	double width=AObjects[i].x2-AObjects[i].x1;
	bool resize = (how == alignByResizing && diff > -width);
	if (!AObjects[i].Object->locked())
	{
		if (resize)
		{
			AObjects[i].Object->resizeBy(diff, 0.0);
			AObjects[i].Object->updateClip();
		}
		else
			AObjects[i].Object->moveBy(diff, 0.0);
	}
}

void ScribusDoc::itemSelection_AlignItemLeft(int i, double newX, AlignMethod how)
{
	double diff=newX-AObjects[i].x1;
	double width=AObjects[i].x2-AObjects[i].x1;
	bool resize = (how == alignByResizing && -diff > -width);
	if (!AObjects[i].Object->locked())
	{
		AObjects[i].Object->moveBy(diff, 0.0);
		if (resize)
		{
			AObjects[i].Object->resizeBy(-diff, 0.0);
			AObjects[i].Object->updateClip();
		}
	}
}

void ScribusDoc::itemSelection_AlignItemBottom(int i, double newY, AlignMethod how)
{
	double diff=newY-AObjects[i].y2;
	double height=AObjects[i].y2-AObjects[i].y1;
	bool resize = (how == alignByResizing && diff > -height);
	if (!AObjects[i].Object->locked())
	{
		if (resize)
		{
			AObjects[i].Object->resizeBy(0.0, diff);
			AObjects[i].Object->updateClip();
		}
		else AObjects[i].Object->moveBy(0.0, diff);
	}
}

void ScribusDoc::itemSelection_AlignItemTop(int i, double newY, AlignMethod how)
{
	double diff=newY-AObjects[i].y1;
	double height=AObjects[i].y2-AObjects[i].y1;
	bool resize = (how == alignByResizing && -diff > -height);
	if (!AObjects[i].Object->locked())
	{
		AObjects[i].Object->moveBy(0.0, diff);
		if (resize)
		{
			AObjects[i].Object->resizeBy(0.0, -diff);
			AObjects[i].Object->updateClip();
		}
	}
}

void ScribusDoc::itemSelection_AlignLeftOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newX = std::numeric_limits<double>::max();
	switch ( currAlignTo )
	{
		case alignFirst:
			newX = AObjects[0].x1;
			loopStart=1;
			break;
		case alignLast:
			newX = AObjects[alignObjectsCount-1].x1;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->Margins.left();
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			for (int i = 0; i < alignObjectsCount; ++i)
				newX = qMin(AObjects[i].x1, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemRight(i, newX, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignLeftIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newX = std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newX = AObjects[0].x1;
			loopStart=1;
			break;
		case alignLast:
			newX = AObjects[alignObjectsCount-1].x1;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->Margins.left();
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			for (int i = 0; i < alignObjectsCount; ++i)
				newX = qMin(AObjects[i].x1, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemLeft(i, newX, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignCenterHor(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newX = 0.0;
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newX = AObjects[0].x1 + (AObjects[0].width)/2;
			loopStart=1;
			break;
		case alignLast:
			{
				int objindex=alignObjectsCount-1;
				newX = AObjects[objindex].x1 + (AObjects[objindex].width)/2;
				loopEnd=alignObjectsCount-2;
			}
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			newX += currentPage()->width()/2;
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->Margins.left();
			newX += (currentPage()->width() - currentPage()->Margins.right() - currentPage()->Margins.left())/2;
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			double minX =  std::numeric_limits<double>::max();
			double maxX = -std::numeric_limits<double>::max();
			for (int i = 0; i < alignObjectsCount; ++i)
			{
				minX = qMin(AObjects[i].x1, minX);
				maxX = qMax(AObjects[i].x2, maxX);
			}
			newX = minX + (maxX-minX)/2;
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newX-AObjects[i].x1-(AObjects[i].width)/2;
		if (!AObjects[i].Object->locked())
			AObjects[i].Object->moveBy(diff, 0.0);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignRightIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newX = -std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newX = AObjects[0].x2;
			loopStart=1;
			break;
		case alignLast:
			newX = AObjects[alignObjectsCount-1].x2;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			newX += currentPage()->width();;
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->width();;
			newX -= currentPage()->Margins.right();
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			for (int i = 0; i < alignObjectsCount; ++i)
				newX = qMax(AObjects[i].x2, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemRight(i, newX, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignRightOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newX = -std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newX = AObjects[0].x2;
			loopStart=1;
			break;
		case alignLast:
			newX = AObjects[alignObjectsCount-1].x2;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newX = currentPage()->xOffset();
			newX += currentPage()->width();
			break;
		case alignMargins:
			newX = currentPage()->xOffset();
			newX += currentPage()->width();
			newX -= currentPage()->Margins.right();
			break;
		case alignGuide:
			newX=currentPage()->xOffset() + guidePosition;
			break;
		case alignSelection:
			for (int i = 0; i < alignObjectsCount; ++i)
				newX = qMax(AObjects[i].x2, newX);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemLeft(i, newX, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignTopOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newY = std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newY = AObjects[0].y1;
			loopStart=1;
			break;
		case alignLast:
			newY = AObjects[alignObjectsCount-1].y1;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->Margins.top();
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			for (int i = 0; i < alignObjectsCount; ++i)
				newY = qMin(AObjects[i].y1, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemBottom(i, newY, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignTopIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newY = std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newY = AObjects[0].y1;
			loopStart=1;
			break;
		case alignLast:
			newY = AObjects[alignObjectsCount-1].y1;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->Margins.top();
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			for (int i = 0; i < alignObjectsCount; ++i)
				newY = qMin(AObjects[i].y1, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemTop(i, newY, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignCenterVer(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newY = 0.0;
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newY = AObjects[0].y1 + (AObjects[0].height)/2;
			loopStart=1;
			break;
		case alignLast:
			{
				int objindex=alignObjectsCount-1;
				newY = AObjects[objindex].y1 + (AObjects[objindex].height)/2;
				loopEnd=alignObjectsCount-2;
			}
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			newY += currentPage()->height()/2;
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->Margins.top();
			newY += (currentPage()->height() - currentPage()->Margins.bottom() - currentPage()->Margins.top())/2;
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			double minY =  std::numeric_limits<double>::max();
			double maxY = -std::numeric_limits<double>::max();
			for (int i = 0; i < alignObjectsCount; ++i)
			{
				minY = qMin(AObjects[i].y1, minY);
				maxY = qMax(AObjects[i].y2, maxY);
			}
			newY = minY + (maxY-minY)/2;
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
	{
		double diff=newY-AObjects[i].y1-(AObjects[i].height)/2;
		if (!AObjects[i].Object->locked())
			AObjects[i].Object->moveBy(0.0, diff);
	}
	endAlign();
}


void ScribusDoc::itemSelection_AlignBottomIn(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newY = -std::numeric_limits<double>::max();
	switch ( currAlignTo ) 
	{
		case alignFirst:
			newY = AObjects[0].y2;
			loopStart=1;
			break;
		case alignLast:
			newY = AObjects[alignObjectsCount-1].y2;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			newY += currentPage()->height();
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->height();
			newY -= currentPage()->Margins.bottom();
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			for (int i = 0; i < alignObjectsCount; ++i)
				newY = qMax(AObjects[i].y2, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemBottom(i, newY, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_AlignBottomOut(AlignTo currAlignTo, AlignMethod currAlignMethod, double guidePosition)
{
	if (!startAlign())
		return;
	int alignObjectsCount = AObjects.count();
	int loopStart = 0, loopEnd = alignObjectsCount - 1;
	double newY = -std::numeric_limits<double>::max();
	switch ( currAlignTo )
	{
		case alignFirst:
			newY = AObjects[0].y2;
			loopStart=1;
			break;
		case alignLast:
			newY = AObjects[alignObjectsCount-1].y2;
			loopEnd=alignObjectsCount-2;
			break;
		case alignPage:
			newY = currentPage()->yOffset();
			newY += currentPage()->height();
			break;
		case alignMargins:
			newY = currentPage()->yOffset();
			newY += currentPage()->height();
			newY -= currentPage()->Margins.bottom();
			break;
		case alignGuide:
			newY=currentPage()->yOffset() + guidePosition;
			break;
		case alignSelection:
			for (int i = 0; i < alignObjectsCount; ++i)
				newY = qMax(AObjects[i].y2, newY);
			break;
	}
	for (int i = loopStart; i <= loopEnd; ++i)
		itemSelection_AlignItemTop(i, newY, currAlignMethod);
	endAlign();
}


void ScribusDoc::itemSelection_DistributeLeft()
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> Xsorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Xsorted.contains(AObjects[i].x1))
			Xsorted.insert(AObjects[i].x1, i);
	}
	QMap<double,uint>::Iterator it = Xsorted.begin();
	QMap<double,uint>::Iterator itend = Xsorted.end();
	double minX=it.key();
	double maxX=it.key();
	while ( it != itend)
	{
		if (minX>it.key())
			minX=it.key();
		if (maxX<it.key())
			maxX=it.key();
		++it;
	}
		
	double separation=(maxX-minX)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for (QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it)
	{
		double diff=minX + i*separation-AObjects[it.value()].x1;
		if (!AObjects[it.value()].Object->locked())
			AObjects[it.value()].Object->moveBy(diff, 0.0);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeCenterH()
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> Xsorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Xsorted.contains(AObjects[i].x1+(AObjects[i].width)/2))
			Xsorted.insert(AObjects[i].x1+(AObjects[i].width)/2, i);
	}
	QMap<double,uint>::Iterator it = Xsorted.begin();
	QMap<double,uint>::Iterator itend = Xsorted.end();
	double minX=it.key();
	double maxX=it.key();
	while ( it != itend)
	{
		if (minX>it.key())
			minX=it.key();
		if (maxX<it.key())
			maxX=it.key();
		++it;
	}
		
	double separation=(maxX-minX)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for (QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it)
	{
		double diff=minX + i*separation-AObjects[it.value()].x1-(AObjects[it.value()].width)/2;
		if (!AObjects[it.value()].Object->locked())
			AObjects[it.value()].Object->moveBy(diff, 0.0);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeRight()
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> Xsorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Xsorted.contains(AObjects[i].x2))
			Xsorted.insert(AObjects[i].x2, i);
	}
	QMap<double,uint>::Iterator it = Xsorted.begin();
	QMap<double,uint>::Iterator itend = Xsorted.end();
	double minX=it.key();
	double maxX=it.key();
	while ( it != itend)
	{
		if (minX>it.key())
			minX=it.key();
		if (maxX<it.key())
			maxX=it.key();
		++it;
	}
	
	double separation=(maxX-minX)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for (QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it)
	{
		double diff=minX + i*separation-AObjects[it.value()].x2;
		if (!AObjects[it.value()].Object->locked())
			AObjects[it.value()].Object->moveBy(diff, 0.0);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeDistH(bool usingDistance, double distance, bool reverseDistribute)
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> X1sorted, X2sorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!X1sorted.contains(AObjects[i].x1))
			X1sorted.insert(AObjects[i].x1, i);
		if (!X2sorted.contains(AObjects[i].x2))
			X2sorted.insert(AObjects[i].x2, i);
	}	
	uint left=X1sorted.begin().value();
	uint right=X2sorted[X2sorted.keys().back()];
	double minX=AObjects[left].x2;
	double maxX=AObjects[right].x2;
	double separation=0.0;
	if (!usingDistance)
	{
		double maxX=AObjects[right].x1;
		double totalSpace=maxX-minX;
		double totalWidth=0;
		uint insideObjectCount=0;
		for (int i = 0; i < alignObjectsCount; ++i)
		{
			if (i==left)
				continue;
			if (i==right)
				continue;
			totalWidth += AObjects[i].width;
			++insideObjectCount;
		}
		separation=(totalSpace-totalWidth)/(insideObjectCount+1);
	}
	else
		separation=value2pts(distance, unitIndex());
	if (!reverseDistribute)
	{
		double currX=minX;
		for (QMap<double,uint>::Iterator it = X1sorted.begin(); it != X1sorted.end(); ++it)
		{
			if (it.value()==left)
				continue;
			if (it.value()==right && !usingDistance)
				continue;
			currX+=separation;

			double diff=currX-AObjects[it.value()].x1;
			if (!AObjects[it.value()].Object->locked())
				AObjects[it.value()].Object->moveBy(diff, 0.0);
			currX+=AObjects[it.value()].width;
		}
	}
	else
	{
		QMapIterator<double,uint> it(X1sorted);
		it.toBack();
		double currX=maxX;
		while (it.hasPrevious())
		{
			it.previous();
			if (it.value()==right)
			{
				currX-=AObjects[it.value()].width;
				continue;
			}
			if (it.value()==left && !usingDistance)
				continue;
			currX-=separation;

			double diff=currX-AObjects[it.value()].x2;
			if (!AObjects[it.value()].Object->locked())
				AObjects[it.value()].Object->moveBy(diff, 0.0);
			currX-=AObjects[it.value()].width;
		}
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeBottom()
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> Ysorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Ysorted.contains(AObjects[i].y2))
			Ysorted.insert(AObjects[i].y2, i);
	}
	QMap<double,uint>::Iterator it = Ysorted.begin();
	QMap<double,uint>::Iterator itend = Ysorted.end();
	double minY=it.key();
	double maxY=it.key();
	while ( it != itend)
	{
		if (minY>it.key())
			minY=it.key();
		if (maxY<it.key())
			maxY=it.key();
		++it;
	}
		
	double separation=(maxY-minY)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for (QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it)
	{
		double diff=minY + i*separation-AObjects[it.value()].y2;
		if (!AObjects[it.value()].Object->locked())
			AObjects[it.value()].Object->moveBy(0.0, diff);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeCenterV()
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> Ysorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Ysorted.contains(AObjects[i].y1+(AObjects[i].height)/2))
			Ysorted.insert(AObjects[i].y1+(AObjects[i].height)/2, i);
	}
	QMap<double,uint>::Iterator it = Ysorted.begin();
	QMap<double,uint>::Iterator itend = Ysorted.end();
	double minY=it.key();
	double maxY=it.key();
	while ( it != itend)
	{
		if (minY>it.key())
			minY=it.key();
		if (maxY<it.key())
			maxY=it.key();
		++it;
	}
		
	double separation=(maxY-minY)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for (QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it)
	{
		double diff=minY + i*separation-AObjects[it.value()].y1-(AObjects[it.value()].height)/2;
		if (!AObjects[it.value()].Object->locked())
			AObjects[it.value()].Object->moveBy(0.0, diff);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeTop()
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> Ysorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Ysorted.contains(AObjects[i].y1))
			Ysorted.insert(AObjects[i].y1, i);
	}
	QMap<double,uint>::Iterator it = Ysorted.begin();
	QMap<double,uint>::Iterator itend = Ysorted.end();
	double minY=it.key();
	double maxY=it.key();
	while ( it != itend)
	{
		if (minY>it.key())
			minY=it.key();
		if (maxY<it.key())
			maxY=it.key();
		++it;
	}
		
	double separation=(maxY-minY)/static_cast<double>(alignObjectsCount-1);
	int i=0;
	for (QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it)
	{
		double diff=minY + i*separation-AObjects[it.value()].y1;
		if (!AObjects[it.value()].Object->locked())
			AObjects[it.value()].Object->moveBy(0.0,diff);
		i++;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeDistV(bool usingDistance, double distance, bool reverseDistribute)
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> Y1sorted, Y2sorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Y1sorted.contains(AObjects[i].y1))
			Y1sorted.insert(AObjects[i].y1, i);
		if (!Y2sorted.contains(AObjects[i].y2))
			Y2sorted.insert(AObjects[i].y2, i);
	}	
	uint top=Y1sorted.begin().value();
	uint bottom=Y2sorted[Y2sorted.keys().back()];
	double minY=AObjects[top].y2;
	double maxY=AObjects[bottom].y2;
	double separation=0.0;
	if (!usingDistance)
	{
		double maxY=AObjects[bottom].y1;
		double totalSpace=maxY-minY;
		double totalHeight=0;
		uint insideObjectCount=0;
		for (int i = 0; i < alignObjectsCount; ++i)
		{
			if (i==top)
				continue;
			if (i==bottom)
				continue;
			totalHeight += AObjects[i].height;
			++insideObjectCount;
		}
		separation=(totalSpace-totalHeight)/(insideObjectCount+1);
	}
	else
		separation=value2pts(distance, unitIndex());
	if (!reverseDistribute)
	{
		double currY=minY;
		for (QMap<double,uint>::Iterator it = Y1sorted.begin(); it != Y1sorted.end(); ++it)
		{
			if (it.value()==top)
				continue;
			if (it.value()==bottom && !usingDistance)
				continue;
			currY+=separation;

			double diff=currY-AObjects[it.value()].y1;
			if (!AObjects[it.value()].Object->locked())
				AObjects[it.value()].Object->moveBy(0.0,diff);
			currY+=AObjects[it.value()].height;
		}
	}
	else
	{
		QMapIterator<double,uint> it(Y1sorted);
		it.toBack();
		double currY=maxY;
		while (it.hasPrevious())
		{
			it.previous();
			if (it.value()==bottom)
			{
				currY-=AObjects[it.value()].height;
				continue;
			}
			if (it.value()==top && !usingDistance)
				continue;
			currY-=separation;

			double diff=currY-AObjects[it.value()].y2;
			if (!AObjects[it.value()].Object->locked())
				AObjects[it.value()].Object->moveBy(0.0, diff);
			currY-=AObjects[it.value()].height;
		}
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeAcrossPage(bool useMargins)
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> X1sorted, X2sorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!X1sorted.contains(AObjects[i].x1))
			X1sorted.insert(AObjects[i].x1, i);
		if (!X2sorted.contains(AObjects[i].x2))
			X2sorted.insert(AObjects[i].x2, i);
	}	
	
	double totalSpace=0.0;
	if (useMargins)
		totalSpace=currentPage()->width()-currentPage()->Margins.left()-currentPage()->Margins.right();
	else
		totalSpace=currentPage()->width();
	double totalWidth=0.0;
	uint insideObjectCount=0;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		totalWidth += AObjects[i].width;
		++insideObjectCount;
	}
	double separation=(totalSpace-totalWidth)/(insideObjectCount+1);
	double currX=currentPage()->xOffset();
	if (useMargins)
		currX+=currentPage()->Margins.left();
	//Handle when our items are too wide for the page.
	if (separation<0.0)
	{
		separation=(totalSpace-totalWidth)/(insideObjectCount-1);
		currX-=separation;
	}
		
	for (QMap<double,uint>::Iterator it = X1sorted.begin(); it != X1sorted.end(); ++it)
	{
		currX+=separation;
		double diff=currX-AObjects[it.value()].x1;
		if (!AObjects[it.value()].Object->locked())
			AObjects[it.value()].Object->moveBy(diff, 0.0);
		currX+=AObjects[it.value()].width;
	}
	endAlign();
}


void ScribusDoc::itemSelection_DistributeDownPage(bool useMargins)
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QMap<double,uint> Y1sorted, Y2sorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Y1sorted.contains(AObjects[i].y1))
			Y1sorted.insert(AObjects[i].y1, i);
		if (!Y2sorted.contains(AObjects[i].y2))
			Y2sorted.insert(AObjects[i].y2, i);
	}	
	
	double totalSpace=0.0;
	if (useMargins)
		totalSpace=currentPage()->height()-currentPage()->Margins.top()-currentPage()->Margins.bottom();
	else
		totalSpace=currentPage()->height();
	double totalHeight=0.0;
	uint insideObjectCount=0;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		totalHeight += AObjects[i].height;
		++insideObjectCount;
	}
	double separation=(totalSpace-totalHeight)/(insideObjectCount+1);
	double currY=currentPage()->yOffset();
	if (useMargins)
		currY+=currentPage()->Margins.top();
	//Handle when our items are too high for the page.
	if (separation<0.0)
	{
		separation=(totalSpace-totalHeight)/(insideObjectCount-1);
		currY-=separation;
	}
		
	for (QMap<double,uint>::Iterator it = Y1sorted.begin(); it != Y1sorted.end(); ++it)
	{
		currY+=separation;
		double diff=currY-AObjects[it.value()].y1;
		if (!AObjects[it.value()].Object->locked())
			AObjects[it.value()].Object->moveBy(0.0, diff);
		currY+=AObjects[it.value()].height;
	}
	endAlign();
}

void ScribusDoc::beginUpdate()
{
	m_docUpdater->beginUpdate();
	m_updateManager.setUpdatesEnabled(false);
}

void ScribusDoc::endUpdate()
{
	m_updateManager.setUpdatesEnabled(true);
	m_docUpdater->endUpdate();
}

void ScribusDoc::itemSelection_SwapLeft()
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QList<int> circleList;
	int circleListCounter = 0;
	//X
	QMap<double,uint> Xsorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Xsorted.contains(AObjects[i].x1))
			Xsorted.insert(AObjects[i].x1, i);
	}
	QMap<double,uint>::Iterator itX = Xsorted.begin();
	QMap<double,uint>::Iterator itXend = Xsorted.end();
	double minX = itX.key();
	double maxX = itX.key();
	while (itX != itXend)
	{
		if (minX > itX.key())
			minX = itX.key();
		if (maxX < itX.key())
			maxX = itX.key();
		++itX;
	}
	//Y
	QMap<double,uint> Ysorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Ysorted.contains(AObjects[i].y1))
			Ysorted.insert(AObjects[i].y1, i);
	}
	QMap<double,uint>::Iterator itY = Ysorted.begin();
	QMap<double,uint>::Iterator itYend = Ysorted.end();
	double minY = itY.key();
	double maxY = itY.key();
	while (itY != itYend)
	{
		if (minY > itY.key())
			minY = itY.key();
		if (maxY < itY.key())
			maxY = itY.key();
		++itY;
	}

	itX = Xsorted.begin(); //first item is left most
	int itemIndex = itX.value(); //get our first item's index in the AObjects array
//	bool found=false;
//	double itXX=itX.key();
	minY =  std::numeric_limits<double>::max();
	maxY = -std::numeric_limits<double>::max();
	int nextItemIndex = itemIndex;
	circleList.append(nextItemIndex);
	++circleListCounter;
	// find the next X item with the minimum Y

	QMap<double,uint>::Iterator itX2_1 = Xsorted.begin();
	QMap<double,uint>::Iterator itLast = Xsorted.begin();
	double xBeginYValue = AObjects[itX2_1.value()].y1;
	while (itX2_1 != Xsorted.end())
	{
		if (AObjects[itX2_1.value()].y1 < xBeginYValue)
		{
			circleList.append(itX2_1.value());
			++circleListCounter;
		}
		itLast = itX2_1;
		itX2_1++;
	}


	if (circleListCounter != static_cast<int>(alignObjectsCount)) //need to reverse back now
	{
		QMap<double,uint>::Iterator itX2_2 = itLast;
		while (itX2_2 != Xsorted.begin())
		{
			if (AObjects[itX2_2.value()].y1 >= xBeginYValue)
			{
				circleList.append(itX2_2.value());
				++circleListCounter;
			}
			itX2_2--;
		}
	}

	int i=0;
	double swapX = AObjects[i].x1;
	double swapY = AObjects[i].y1;
	if (!AObjects[i].Object->locked())
		AObjects[i].Object->moveBy(AObjects[circleListCounter-1].x1 - AObjects[i].x1, AObjects[circleListCounter-1].y1 - AObjects[i].y1);
	++i;
	while (i < circleListCounter-1)
	{
		double diffX = swapX-AObjects[i].x1;
		double diffY = swapY-AObjects[i].y1;
		swapX = AObjects[i].x1;
		swapY = AObjects[i].y1;
		if (!AObjects[i].Object->locked())
			AObjects[i].Object->moveBy(diffX, diffY);
		++i;
	}
	double diffX3 = swapX-AObjects[circleListCounter-1].x1;
	double diffY3 = swapY-AObjects[circleListCounter-1].y1;
	if (!AObjects[circleListCounter-1].Object->locked())
		AObjects[circleListCounter-1].Object->moveBy(diffX3, diffY3);
	endAlign();
}

void ScribusDoc::itemSelection_SwapRight()
{
	if (!startAlign(2))
		return;
	int alignObjectsCount = AObjects.count();
	QList<int> circleList;
	int circleListCounter = 0;
	//X
	QMap<double,uint> Xsorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Xsorted.contains(AObjects[i].x1))
			Xsorted.insert(AObjects[i].x1, i);
	}
	QMap<double,uint>::Iterator itX = Xsorted.begin();
	QMap<double,uint>::Iterator itXend = Xsorted.end();
	double minX = itX.key();
	double maxX = itX.key();
	while (itX != itXend)
	{
		if (minX > itX.key())
			minX = itX.key();
		if (maxX < itX.key())
			maxX = itX.key();
		++itX;
	}
	//Y
	QMap<double,uint> Ysorted;
	for (int i = 0; i < alignObjectsCount; ++i)
	{
		if (!Ysorted.contains(AObjects[i].y1))
			Ysorted.insert(AObjects[i].y1, i);
	}
	QMap<double,uint>::Iterator itY = Ysorted.begin();
	QMap<double,uint>::Iterator itYend = Ysorted.end();
	double minY = itY.key();
	double maxY = itY.key();
	while (itY != itYend)
	{
		if (minY > itY.key())
			minY = itY.key();
		if (maxY < itY.key())
			maxY = itY.key();
		++itY;
	}

	itX = Xsorted.begin(); //first item is left most
	int itemIndex = itX.value(); //get our first item's index in the AObjects array
//	bool found=false;
//	double itXX=itX.key();
	minY =  std::numeric_limits<double>::max();
	maxY = -std::numeric_limits<double>::max();
	int nextItemIndex=itemIndex;
	circleList.append(nextItemIndex);
	++circleListCounter;
	// find the next X item with the minimum Y

	QMap<double,uint>::Iterator itX2_1 = Xsorted.begin();
	QMap<double,uint>::Iterator itLast = Xsorted.begin();
	double xBeginYValue = AObjects[itX2_1.value()].y1;
	while (itX2_1!=Xsorted.end())
	{
		if (AObjects[itX2_1.value()].y1 < xBeginYValue)
		{
			circleList.append(itX2_1.value());
			++circleListCounter;
		}
		itLast = itX2_1;
		itX2_1++;
	}


	if (circleListCounter!=static_cast<int>(alignObjectsCount)) //need to reverse back now
	{
		QMap<double,uint>::Iterator itX2_2 = itLast;
		while (itX2_2 != Xsorted.begin())
		{
			if (AObjects[itX2_2.value()].y1 >= xBeginYValue)
			{
				circleList.append(itX2_2.value());
				++circleListCounter;
			}
			itX2_2--;
		}
	}

	int i = circleListCounter - 1;
	double swapX = AObjects[i].x1;
	double swapY = AObjects[i].y1;
	if (!AObjects[i].Object->locked())
		AObjects[i].Object->moveBy(AObjects[0].x1-AObjects[i].x1, AObjects[0].y1-AObjects[i].y1);
	--i;
	while (i > 0)
	{
		double diffX = swapX - AObjects[i].x1;
		double diffY = swapY - AObjects[i].y1;
		swapX = AObjects[i].x1;
		swapY = AObjects[i].y1;
		if (!AObjects[i].Object->locked())
			AObjects[i].Object->moveBy(diffX, diffY);
		--i;
	}
	double diffX3 = swapX - AObjects[0].x1;
	double diffY3 = swapY - AObjects[0].y1;
	if (!AObjects[0].Object->locked())
		AObjects[0].Object->moveBy(diffX3, diffY3);
	endAlign();
}

void ScribusDoc::changed()
{
	setModified(true);
	// Do not emit docChanged signal() unnecessarily
	// Processing of that signal is slowwwwwww and
	// DocUpdater will trigger it when necessary
	if (m_docUpdater->inUpdateSession())
	{
		m_docUpdater->setDocChangeNeeded();
		return;
	}
	emit docChanged();
}

void ScribusDoc::invalidateAll()
{
	QList<PageItem*> allItems;
	for (int c = 0; c < DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if (ite->isGroup())
			allItems = ite->getAllChildren();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			ite->invalidateLayout();
		}
		allItems.clear();
	}
	for (int c=0; c < MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		if (ite->isGroup())
			allItems = ite->getAllChildren();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			ite->invalidateLayout();
		}
		allItems.clear();
	}
	// for now hope that frameitems get invalidated by their parents layout() method.
}

void ScribusDoc::invalidateLayer(int layerID)
{
	QList<PageItem*> allItems;
	for (int c = 0; c < DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if (ite->isGroup())
			allItems = ite->getAllChildren();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			if (ite->LayerID == layerID)
					ite->invalidateLayout();
		}
		allItems.clear();
	}
	if (this->masterPageMode())
	{
		for (int c=0; c < MasterItems.count(); ++c)
		{
			PageItem *ite = MasterItems.at(c);
			if (ite->isGroup())
				allItems = ite->getAllChildren();
			else
				allItems.append(ite);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				ite = allItems.at(ii);
				if (ite->LayerID == layerID)
					ite->invalidateLayout();
			}
			allItems.clear();
		}
	}
	// for now hope that frameitems get invalidated by their parents layout() method.
}

void ScribusDoc::invalidateRegion(QRectF region)
{
	QList<PageItem*> allItems;
	for (int c=0; c<DocItems.count(); ++c)
	{
		PageItem *ite = DocItems.at(c);
		if (ite->isGroup())
			allItems = ite->getAllChildren();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			if (ite->getBoundingRect().intersects(region))
				ite->invalidateLayout();
		}
		allItems.clear();
	}
	for (int c=0; c<MasterItems.count(); ++c)
	{
		PageItem *ite = MasterItems.at(c);
		if (ite->isGroup())
			allItems = ite->getAllChildren();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
		// for now invalidate all masteritems, should be only necessary in masterpagemode
			ite->invalidateLayout();
		}
		allItems.clear();
	}
	// for now hope that frameitems get invalidated by their parents layout() method.
}


ScPage* ScribusDoc::currentPage()
{
	return m_currentPage;
}


void ScribusDoc::setCurrentPage(ScPage *newPage)
{
	if (newPage==NULL)
		return;
	m_currentPage = newPage;
	if (m_ScMW)
	{
		m_ScMW->guidePalette->setDoc(this);
		if (!isLoading())
		{
			// #9476 : call setupPage with false arg to setup only guidePalette GUI
			// Otherwise expect problems when applying masterpages with guides
			m_ScMW->guidePalette->setupPage(false);
			m_ScMW->pagePalette->markPage(newPage->pageNr());
		}
	}
	m_constants.insert("pagewidth", newPage->width());
	m_constants.insert("pageheight", newPage->height());
	m_constants.insert("marginleft", newPage->Margins.left());
	m_constants.insert("marginright", newPage->width() - newPage->Margins.right());
	m_constants.insert("margintop", newPage->Margins.top());
	m_constants.insert("marginbottom", newPage->height() - newPage->Margins.bottom());
}


QPoint ScribusDoc::ApplyGrid(const QPoint& in)
{
	QPoint np;
	int onp = OnPage(in.x(), in.y());
	if (SnapGrid && (onp != -1))
	{
		np.setX(static_cast<int>(qRound((in.x() - Pages->at(onp)->xOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->xOffset()));
		np.setY(static_cast<int>(qRound((in.y() - Pages->at(onp)->yOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->yOffset()));
	}
	else
		np = in;
	return np;
}


FPoint ScribusDoc::ApplyGridF(const FPoint& in)
{
	FPoint np;
	int onp = OnPage(in.x(), in.y());
	if (SnapGrid && (onp != -1))
	{
		np.setX(qRound((in.x() - Pages->at(onp)->xOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->xOffset());
		np.setY(qRound((in.y() - Pages->at(onp)->yOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->yOffset());
	}
	else
		np = in;
	return np;
}

QRectF ScribusDoc::ApplyGridF(const QRectF& in)
{
	QRectF nr(in);
	if (!SnapGrid)
		return nr;

	bool   onPage = false;
	double dX = m_docPrefsData.guidesPrefs.minorGridSpacing;
	double dY = m_docPrefsData.guidesPrefs.minorGridSpacing;
	double newX = in.x(), newY = in.y();

	FPoint fp1(in.x(), in.y());
	FPoint fp2(in.x(), in.y() + in.height());
	FPoint fp3(in.x() + in.width(), in.y());
	FPoint fp4(in.x() + in.width(), in.y() + in.height());

	int onp = OnPage(fp1.x(), fp1.y());
	if (onp >= 0)
	{
		newX = qRound((fp1.x() - Pages->at(onp)->xOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->xOffset();
		newY = qRound((fp1.y() - Pages->at(onp)->yOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->yOffset();
		if (fabs(newX - fp1.x()) < fabs(dX))
			dX = newX - fp1.x();
		if (fabs(newY - fp1.y()) < fabs(dY))
			dY = newY - fp1.y();
		onPage = true;
	}

	onp = OnPage(fp2.x(), fp2.y());
	if (onp >= 0)
	{
		newX = qRound((fp2.x() - Pages->at(onp)->xOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->xOffset();
		newY = qRound((fp2.y() - Pages->at(onp)->yOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->yOffset();
		if (fabs(newX - fp2.x()) < fabs(dX))
			dX = newX - fp2.x();
		if (fabs(newY - fp2.y()) < fabs(dY))
			dY = newY - fp2.y();
		onPage = true;
	}

	onp = OnPage(fp3.x(), fp3.y());
	if (onp >= 0)
	{
		newX = qRound((fp3.x() - Pages->at(onp)->xOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->xOffset();
		newY = qRound((fp3.y() - Pages->at(onp)->yOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->yOffset();
		if (fabs(newX - fp3.x()) < fabs(dX))
			dX = newX - fp3.x();
		if (fabs(newY - fp3.y()) < fabs(dY))
			dY = newY - fp3.y();
		onPage = true;
	}

	onp = OnPage(fp4.x(), fp4.y());
	if (onp >= 0)
	{
		newX = qRound((fp4.x() - Pages->at(onp)->xOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->xOffset();
		newY = qRound((fp4.y() - Pages->at(onp)->yOffset()) / m_docPrefsData.guidesPrefs.minorGridSpacing) * m_docPrefsData.guidesPrefs.minorGridSpacing + Pages->at(onp)->yOffset();
		if (fabs(newX - fp4.x()) < fabs(dX))
			dX = newX - fp4.x();
		if (fabs(newY - fp4.y()) < fabs(dY))
			dY = newY - fp4.y();
		onPage = true;
	}

	if (onPage)
		nr.translate(dX, dY);
	return nr;
}

void ScribusDoc::itemSelection_MultipleDuplicate(ItemMultipleDuplicateData& mdData)
{
	if ((mdData.type==0 && mdData.copyCount<1) || (mdData.type==1 && (mdData.gridRows==1 && mdData.gridCols==1)))
		return;
	QString tooltip;
	UndoTransaction activeTransaction;

	if (UndoManager::undoEnabled())
	{ // Make multiple duplicate a single action in the action history
		if (m_Selection->count() > 1)
			activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::MultipleDuplicate,"",Um::IMultipleDuplicate);
		else
		{
			PageItem* item=m_Selection->itemAt(0);
			activeTransaction = m_undoManager->beginTransaction(item->getUName(), item->getUPixmap(), Um::MultipleDuplicate, "", Um::IMultipleDuplicate);
		}
	}
	DoDrawing = false;
	view()->updatesOn(false);

	QList<PageItem*> selectedItems = m_Selection->items();
	qStableSort(selectedItems.begin(), selectedItems.end(), compareItemLevel);

	Selection selection(this, false);
	for (int i = 0; i < selectedItems.count(); ++i)
		selection.addItem(selectedItems.at(i));

	if (mdData.type==0) // Copy and offset or set a gap
	{
		double dH = mdData.copyShiftGapH / m_docUnitRatio;
		double dV = mdData.copyShiftGapV / m_docUnitRatio;
		double dH2 = dH;
		double dV2 = dV;
		double dR = mdData.copyRotation;
		double dR2 = dR;
		if (mdData.copyShiftOrGap==1)
		{
			if (dH != 0.0)
				dH2 += selection.width();
			if (dV != 0.0)
				dV2 += selection.height();
		}
		ScriXmlDoc ss;
		QString BufferS = ss.WriteElem(this, &selection);
		//FIXME: stop using m_View
		m_View->Deselect(true);
		for (int i=0; i<mdData.copyCount; ++i)
		{
			uint ac = Items->count();
			ss.ReadElem(BufferS, m_appPrefsData.fontPrefs.AvailFonts, this, currentPage()->xOffset(), currentPage()->yOffset(), false, true, m_appPrefsData.fontPrefs.GFontSub);
			m_Selection->delaySignalsOn();
			for (int as = ac; as < Items->count(); ++as)
			{
				PageItem* bItem = Items->at(as);
				bItem->setLocked(false);
				bItem->moveBy(dH2, dV2, true);
				if (bItem->isGroup())
					GroupOnPage(bItem);
				else
					bItem->OwnPage = OnPage(bItem);
				m_Selection->addItem(bItem);
			}
			m_Selection->delaySignalsOff();
			m_Selection->setGroupRect();
			if (dR != 0.0)
			{
				if (m_Selection->count() > 1)
					rotateGroup(dR2, FPoint(0,0)); //FIXME:av
				else
					rotateItem(dR2, m_Selection->itemAt(0));
			}
			dH2 += dH;
			dV2 += dV;
			if (mdData.copyShiftOrGap==1)
			{
				if (dH != 0.0)
					dH2 += m_Selection->width();
				if (dV != 0.0)
					dV2 += m_Selection->height();
			}
			dR2 += dR;
			if (m_Selection->count() > 0)
			{
				m_Selection->itemAt(0)->connectToGUI();
				m_Selection->itemAt(0)->emitAllToGUI();
			}
			m_Selection->clear();
		}
		QString unitSuffix = unitGetStrFromIndex(this->unitIndex());
		int unitPrecision = unitGetPrecisionFromIndex(this->unitIndex());
		QString hString = QString::number(dH * m_docUnitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString vString = QString::number(dV * m_docUnitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString dString = QString::number(dR) + " " + unitGetStrFromIndex(SC_DEGREES);
		tooltip = tr("Number of copies: %1\nHorizontal shift: %2\nVertical shift: %3\nRotation: %4").arg(mdData.copyCount).arg(hString).arg(vString).arg(dString);
	}
	else if (mdData.type==1) // Create a grid of duplicated items
	{
		int copyCount = mdData.gridRows * mdData.gridCols;
		double dX = mdData.gridGapH / m_docUnitRatio + selection.width();
		double dY = mdData.gridGapV / m_docUnitRatio + selection.height();
		ScriXmlDoc ss;
		QString BufferS = ss.WriteElem(this, &selection);
		for (int i = 0; i < mdData.gridRows; ++i) //skip 0, the item is the one we are copying
		{
			for (int j = 0; j < mdData.gridCols; ++j) //skip 0, the item is the one we are copying
			{
				// We can comment out this conditional jump if we use slotEditCut(), would not be cool? ;-)
				if (i==0 && j==0)
					continue;
				uint ac = Items->count();
				ss.ReadElem(BufferS, m_appPrefsData.fontPrefs.AvailFonts, this, currentPage()->xOffset(), currentPage()->yOffset(), false, true, m_appPrefsData.fontPrefs.GFontSub);
				for (int as = ac; as < Items->count(); ++as)
				{
					PageItem* bItem = Items->at(as);
					bItem->setLocked(false);
					bItem->moveBy(j*dX, i*dY, true);
					if (bItem->isGroup())
						GroupOnPage(bItem);
					else
						bItem->OwnPage = OnPage(bItem);
					bItem->connectToGUI();
					bItem->emitAllToGUI();
				}
			}
		}
		QString unitSuffix = unitGetStrFromIndex(this->unitIndex());
		int unitPrecision = unitGetPrecisionFromIndex(this->unitIndex());
		QString hString = QString::number(mdData.gridGapH, 'f', unitPrecision) + " " + unitSuffix;
		QString vString = QString::number(mdData.gridGapV, 'f', unitPrecision) + " " + unitSuffix;
		tooltip = tr("Number of copies: %1\nHorizontal gap: %2\nVertical gap: %3").arg(copyCount-1).arg(hString).arg(vString).arg(unitSuffix);
	}
	if (activeTransaction)
	{
		activeTransaction.commit("", 0, "", tooltip, 0);
	}
	DoDrawing = true;
	view()->updatesOn(true);
	//FIXME: stop using m_View
	m_View->Deselect(true);
	view()->DrawNew();
	changed();
}


void ScribusDoc::itemSelection_ApplyImageEffects(ScImageEffectList& newEffectList, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=NULL);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	PageItem *currItem = itemSelection->itemAt(0);
	ScImageEffectList oldEffects(currItem->effectsInUse);
	currItem->effectsInUse = newEffectList;
	updatePic();

	if (UndoManager::undoEnabled())
	{
		ScItemState<QPair<ScImageEffectList, ScImageEffectList> > *state =
				new ScItemState<QPair<ScImageEffectList, ScImageEffectList> >(
					Um::ImageEffects, "", currItem->getUPixmap());
		state->set("APPLY_IMAGE_EFFECTS");
		state->setItem(qMakePair(oldEffects, currItem->effectsInUse));
		m_undoManager->action(currItem, state);
	}
	changed();
}


void ScribusDoc::itemSelection_ApplyArrowHead(int startArrowID, int endArrowID, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection != 0);
	int selectedItemCount = itemSelection->count();
	if (selectedItemCount == 0)
		return;

	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction();
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (!(currItem->asLine() || currItem->asPolyLine() || currItem->asSpiral()))
			continue;
		if (startArrowID != -1)
			currItem->setStartArrowIndex(startArrowID);
		if (endArrowID != -1)
			currItem->setEndArrowIndex(endArrowID);
		if (selectedItemCount <= Um::ItemsInvolvedLimit)
			tooltip += "\t" + currItem->getUName() + "\n";
		currItem->update();
	}
	QString undoText;
	if (startArrowID!=-1 && endArrowID!=-1)
		undoText=Um::StartAndEndArrow;
	else
		undoText=(startArrowID!=-1) ? Um::StartArrow : Um::EndArrow;
	if (activeTransaction)
	{
		activeTransaction.commit(Um::Selection,
								 Um::IGroup,
								 undoText,
								 tooltip,
								 Um::IArrow);
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_ApplyArrowScale(int startArrowSc, int endArrowSc, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection != 0);
	int selectedItemCount = itemSelection->count();
	if (selectedItemCount == 0)
		return;

	UndoTransaction activeTransaction;
	m_updateManager.setUpdatesDisabled();
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		activeTransaction = m_undoManager->beginTransaction();
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (!(currItem->asLine() || currItem->asPolyLine() || currItem->asSpiral()))
			continue;
		if (startArrowSc !=  -1)
			currItem->setStartArrowScale(startArrowSc);
		if (endArrowSc != -1)
			currItem->setEndArrowScale(endArrowSc);
		if (selectedItemCount <= Um::ItemsInvolvedLimit)
			tooltip += "\t" + currItem->getUName() + "\n";
		currItem->update();
	}
	QString undoText;
	if (startArrowSc!=-1 && endArrowSc!=-1)
		undoText=Um::StartAndEndArrow;
	else
		undoText=(startArrowSc!=-1) ? Um::StartArrow : Um::EndArrow;
	if (activeTransaction)
	{
		activeTransaction.commit(Um::Selection,
								 Um::IGroup,
								 undoText,
								 tooltip,
								 Um::IArrow);
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}


void ScribusDoc::createDefaultMasterPages()
{
	int setcol = pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns;
	if (setcol == 1)
	{
		addMasterPage(0, CommonStrings::trMasterPageNormal);
	}
	else if (setcol == 2)
	{
		ScPage *lp = addMasterPage(0, CommonStrings::trMasterPageNormalLeft);
		lp->LeftPg = 1;
		lp->Margins.setLeft(lp->initialMargins.right());
		lp->Margins.setRight(lp->initialMargins.left());
		lp = addMasterPage(1, CommonStrings::trMasterPageNormalRight);
		lp->LeftPg = 0;
		lp->Margins.setRight(lp->initialMargins.right());
		lp->Margins.setLeft(lp->initialMargins.left());
	}
	else if ((setcol == 3) || (setcol == 4))
	{
		ScPage *lp = addMasterPage(0, CommonStrings::trMasterPageNormalLeft);
		lp->LeftPg = 1;
		lp->Margins.setLeft(lp->initialMargins.right());
		lp->Margins.setRight(lp->initialMargins.left());
		lp = addMasterPage(1, CommonStrings::trMasterPageNormalRight);
		lp->LeftPg = 0;
		lp->Margins.setRight(lp->initialMargins.right());
		lp->Margins.setLeft(lp->initialMargins.left());
		lp = addMasterPage(2, CommonStrings::trMasterPageNormalMiddle);
		lp->LeftPg = 2;
		lp->Margins.setLeft(lp->initialMargins.left());
		lp->Margins.setRight(lp->initialMargins.left());
	}
}


void ScribusDoc::createNewDocPages(int pageCount)
{
	int setcol = pageSets()[m_docPrefsData.docSetupPrefs.pagePositioning].Columns;
	int createCount=qMax(pageCount,1);
	if (setcol == 1)
	{
		for (int i = 0; i < createCount; ++i)
			addPage(i, CommonStrings::trMasterPageNormal, true);
	}
	else if (setcol == 2)
	{
		for (int i = 0; i < createCount; ++i)
		{
			PageLocation pageLoc = locationOfPage(i);
			if (pageLoc == LeftPage)
				addPage(i, CommonStrings::trMasterPageNormalLeft, true);
			else
				addPage(i, CommonStrings::trMasterPageNormalRight, true);
		}
	}
	else if ((setcol == 3) || (setcol == 4))
	{
		for (int i = 0; i < createCount; ++i)
		{
			PageLocation pageLoc = locationOfPage(i);
			if (pageLoc == LeftPage)
				addPage(i, CommonStrings::trMasterPageNormalLeft, true);
			else if (pageLoc == RightPage)
				addPage(i, CommonStrings::trMasterPageNormalRight, true);
			else
				addPage(i, CommonStrings::trMasterPageNormalMiddle, true);
		}
	}
}


void ScribusDoc::getClosestGuides(double xin, double yin, double *xout, double *yout, int *GxM, int *GyM, ScPage* refPage)
{
	*GxM = -1;
	*GyM = -1;
	ScPage* page = (refPage == NULL) ? currentPage() : refPage;
	QMap<double, uint> tmpGuidesSel;
	Guides tmpGuides = page->guides.horizontals(GuideManagerCore::Standard);
	Guides::iterator it;
	double viewScale=m_View->scale();
	uint yg = 0;
	uint xg = 0;
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++yg)
	{
		if (fabs((*it) + page->yOffset() - yin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs((*it) + page->yOffset() - yin), yg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GyM = tmpGuidesSel.begin().value();
		*yout = tmpGuides[*GyM];
	}
	tmpGuidesSel.clear();
	tmpGuides = page->guides.verticals(GuideManagerCore::Standard);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++xg)
	{
		if (fabs((*it) + page->xOffset() - xin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs((*it) + page->xOffset() - xin), xg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GxM = tmpGuidesSel.begin().value();
		*xout = tmpGuides[*GxM];
	}
	yg = 0;
	xg = 0;
	tmpGuidesSel.clear();
	tmpGuides = page->guides.horizontals(GuideManagerCore::Auto);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++yg)
	{
		if (fabs((*it) + page->yOffset() - yin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs((*it) + page->yOffset() - yin), yg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GyM = tmpGuidesSel.begin().value();
		*yout = tmpGuides[*GyM];
	}
	tmpGuidesSel.clear();
	tmpGuides = page->guides.verticals(GuideManagerCore::Auto);
	for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++xg)
	{
		if (fabs((*it) + page->xOffset() - xin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs((*it) + page->xOffset() - xin), xg);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GxM = tmpGuidesSel.begin().value();
		*xout = tmpGuides[*GxM];
	}
}

void ScribusDoc::getClosestElementBorder(double xin, double yin, double *xout, double *yout, int *GxM, int *GyM, ScPage* refPage)
{
	*GxM = -1;
	*GyM = -1;
	ScPage* page = (refPage == NULL) ? currentPage() : refPage;
	QMap<double, uint> tmpGuidesSel;
	double viewScale=m_View->scale();
	QList<PageItem*> item = getAllItems(*Items);
	PageItem *parentI = NULL;
	if (m_Selection->count() > 0)
		parentI = m_Selection->itemAt(0)->Parent;
	for (int i=0;i<item.size();i++)
	{
		if (m_Selection->containsItem(item.at(i)) || item.at(i)->OwnPage != OnPage(xin,yin))
			continue;
		else if (item.at(i)->Parent != parentI)
			continue;
		else if (fabs(item.at(i)->visualYPos() - yin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs(item.at(i)->visualYPos() - yin), i*3);
		else if (fabs(item.at(i)->visualYPos() + item.at(i)->visualHeight() - yin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs(item.at(i)->visualYPos() + item.at(i)->visualHeight() - yin), i*3+1);
		else if (fabs(item.at(i)->visualYPos() + item.at(i)->visualHeight()/2 - yin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs(item.at(i)->visualYPos() + item.at(i)->visualHeight()/2 - yin), i*3+2);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GyM = tmpGuidesSel.begin().value();
		if (*GyM%3==0)
			*yout = item.at(*GyM/3)->visualYPos() - page->yOffset();
		else if (*GyM%3==1)
			*yout = item.at(*GyM/3)->visualYPos() + item.at(*GyM/3)->visualHeight() -page->yOffset();
		else if (*GyM%3==2)
			*yout = item.at(*GyM/3)->visualYPos() + item.at(*GyM/3)->visualHeight()/2 -page->yOffset();
	}
	tmpGuidesSel.clear();
	for (int i=0;i<item.size();i++)
	{
		if (m_Selection->containsItem(item.at(i)) || item.at(i)->OwnPage != OnPage(xin,yin))
			continue;
		else if (item.at(i)->Parent != parentI)
			continue;
		else if (fabs(item.at(i)->visualXPos() - xin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs(item.at(i)->visualXPos() - xin), i*3);
		else if (fabs(item.at(i)->visualXPos() + item.at(i)->visualWidth() - xin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs(item.at(i)->visualXPos() + item.at(i)->visualWidth() - xin), i*3+1);
		else if (fabs(item.at(i)->visualXPos() + item.at(i)->visualWidth()/2 - xin) < (prefsData().guidesPrefs.guideRad / viewScale))
			tmpGuidesSel.insert(fabs(item.at(i)->visualXPos() + item.at(i)->visualWidth()/2 - xin), i*3+2);
	}
	if (tmpGuidesSel.count() != 0)
	{
		*GxM = tmpGuidesSel.begin().value();
		if (*GxM%3==0)
			*xout = item.at(*GxM/3)->visualXPos() -page->xOffset();
		else if (*GxM%3==1)
			*xout = item.at(*GxM/3)->visualXPos() + item.at(*GxM/3)->visualWidth() -page->xOffset();
		else if (*GxM%3==2)
			*xout = item.at(*GxM/3)->visualXPos() + item.at(*GxM/3)->visualWidth()/2 -page->xOffset();
	}
}

void ScribusDoc::SnapToGuides(PageItem *currItem)
{
	int pg = OnPage(currItem);
	double xout, yout;
	if (pg == -1)
		return;
	ScPage* page = Pages->at(pg);
	int GxM, GyM;

	getClosestGuides(0, currItem->yPos(), &xout, &yout, &GxM, &GyM);
	if (GyM != -1)
		currItem->setYPos(yout+page->yOffset());
	if (currItem->asLine())
	{
		QTransform ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
//		double my = ma.m22() * currItem->height() + ma.m12() * currItem->width() + ma.dy();
		double my = ma.m12() * currItem->width() + ma.dy();
		getClosestGuides(0, my, &xout, &yout, &GxM, &GyM);
		if (GyM != -1)
			currItem->moveBy(0.0, yout - my + page->yOffset());
	}
	else
	{
		getClosestGuides(0, currItem->yPos()+currItem->height(), &xout, &yout, &GxM, &GyM);
		if (GyM != -1)
			currItem->setYPos(yout-currItem->height()+page->yOffset());
	}
	getClosestGuides(currItem->xPos(), 0, &xout, &yout, &GxM, &GyM);
	if (GxM != -1)
		currItem->setXPos(xout+page->xOffset());
	if (currItem->asLine())
	{
		QTransform ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
		double mx = ma.m11() * currItem->width() + ma.dx();
//		double mx = ma.m11() * currItem->width() + ma.m21() * currItem->height() + ma.dx();
		getClosestGuides(mx,  0, &xout, &yout, &GxM, &GyM);
		if (GxM != -1)
			currItem->moveBy(xout - mx + page->xOffset(), 0.0);
	}
	else
	{
		getClosestGuides(currItem->xPos()+currItem->width(), 0, &xout, &yout, &GxM, &GyM);
		if (GxM != -1)
			currItem->setXPos(xout-currItem->width()+page->xOffset());
	}
}


bool ScribusDoc::ApplyGuides(double *x, double *y, bool elementSnap)
{
//	m_SnapCounter++;
	bool ret = false;
	double xout, yout;
	int pg = OnPage(*x, *y);
	if (pg == -1)
		return ret;
	ScPage* page = Pages->at(pg);
	int GxM, GyM;

	//	if ((SnapGuides) && (m_SnapCounter > 1))
	if ((SnapGuides && !elementSnap) || (SnapElement && elementSnap))
	{
//		m_SnapCounter = 0;
		if (!elementSnap)
			getClosestGuides(*x, *y, &xout, &yout, &GxM, &GyM, page);
		else
			getClosestElementBorder(*x, *y, &xout, &yout, &GxM, &GyM, page);
		if (GxM != -1)
		{
			*x = xout+page->xOffset();
			ret = true;
		}
		if (GyM != -1)
		{
			*y = yout+page->yOffset();
			ret = true;
		}
		double invViewScale=1/m_View->scale();
		if (fabs(page->Margins.left() + page->xOffset() - *x) < (m_docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*x = page->Margins.left()+page->xOffset();
			ret = true;
		}
		if (fabs((page->width() - page->Margins.right()) + page->xOffset() - *x) < (m_docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*x = page->width() - page->Margins.right()+page->xOffset();
			ret = true;
		}
		if (fabs(page->width() + page->xOffset() - *x) < (m_docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*x = page->width() + page->xOffset();
			ret = true;
		}
		if (fabs(page->xOffset() - *x) < (m_docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*x = page->xOffset();
			ret = true;
		}
		if (fabs(page->Margins.top() + page->yOffset() - *y) < (m_docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*y = page->Margins.top()+page->yOffset();
			ret = true;
		}
		if (fabs((page->height() - page->Margins.bottom())+page->yOffset() - *y) < (m_docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*y = page->height() - page->Margins.bottom()+page->yOffset();
			ret = true;
		}
		if (fabs(page->height() + page->yOffset() - *y) < (m_docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*y = page->height() + page->yOffset();
			ret = true;
		}
		if (fabs(page->yOffset() - *y) < (m_docPrefsData.guidesPrefs.guideRad * invViewScale))
		{
			*y = page->yOffset();
			ret = true;
		}
	}
//	if (m_SnapCounter > 10)
//		m_SnapCounter = 0;
	return ret;
}

bool ScribusDoc::ApplyGuides(FPoint* point, bool elementSnap)
{
	double newX = point->x();
	double newY = point->y();
	bool ret = ApplyGuides(&newX, &newY, elementSnap);
	point->setX(newX);
	point->setY(newY);

	return ret;
}

bool ScribusDoc::moveItem(double newX, double newY, PageItem* currItem)
{
	if (currItem->locked())
		return false;
	bool retw = false;
	double oldx = currItem->xPos();
	double oldy = currItem->yPos();
	currItem->moveBy(newX, newY);
	if ((currItem->xPos() != oldx) || (currItem->yPos() != oldy))
		retw = true;
	setRedrawBounding(currItem);
	currItem->OwnPage = OnPage(currItem);
	return retw;
}

void ScribusDoc::rotateItem(double angle, PageItem *currItem)
{
	if (currItem->locked())
		return;
	QRectF oldR = currItem->getBoundingRect();
//	if ((Doc->RotMode != 0) && (m_MouseButtonPressed))
	if (m_rotMode != 0)
	{
		QTransform ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.scale(1, 1);
		ma.rotate(currItem->rotation());
		double ro = angle - currItem->rotation();
		currItem->setRotation(angle);
		FPoint n(0,0);
		switch (m_rotMode)
		{
		case 2:
			ma.translate(currItem->width()/2.0, currItem->height()/2.0);
			n = FPoint(-currItem->width()/2.0, -currItem->height()/2.0);
			break;
		case 4:
			ma.translate(currItem->width(), currItem->height());
			n = FPoint(-currItem->width(), -currItem->height());
			break;
		case 3:
			ma.translate(0, currItem->height());
			n = FPoint(0, -currItem->height());
			break;
		case 1:
			ma.translate(currItem->width(), 0);
			n = FPoint(-currItem->width(), 0);
			break;
		}
		ma.rotate(ro);
		double x = ma.m11() * n.x() + ma.m21() * n.y() + ma.dx();
		double y = ma.m22() * n.y() + ma.m12() * n.x() + ma.dy();
		bool oldS = currItem->Sizing;
		currItem->Sizing = true;
		currItem->moveBy(x - currItem->xPos(), y - currItem->yPos(), true);
		setRedrawBounding(currItem);
		currItem->OwnPage = OnPage(currItem);
		currItem->Sizing = oldS;
		currItem->checkChanges();
	}
	else
	{
		currItem->setRotation(angle);
		setRedrawBounding(currItem);
	}
	if (!m_loading)
	{
		QRectF newR(currItem->getBoundingRect());
		regionsChanged()->update(newR.united(oldR));
	}
	//emit SetAngle(currItem->rotation());
}



void ScribusDoc::moveRotated(PageItem *currItem, FPoint npv)
{
	QTransform ma;
	ma.translate(currItem->xPos(), currItem->yPos());
	ma.rotate(currItem->rotation());
	double mxc = currItem->xPos() - (ma.m11() * npv.x() + ma.m21() * npv.y() + ma.dx());
	double myc = currItem->yPos() - (ma.m22() * npv.y() + ma.m12() * npv.x() + ma.dy());
	moveItem(-mxc, -myc, currItem);
}

bool ScribusDoc::sizeItem(double newX, double newY, PageItem *pi, bool fromMP, bool DoUpdateClip, bool redraw)
{
	PageItem *currItem = pi;
	if (currItem->locked())
		return false;
	QRectF oldR(currItem->getBoundingRect());
	//Uncomment for stopping resize in any direction
	//and remove the height/width <0 tests in item sizing switch
	/*
	if (!currItem->asLine())
	{
		newX = qMax(newX, 1);
		newY = qMax(newY, 1);
	}
	*/
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
	{
		QString unitSuffix = unitGetStrFromIndex(this->unitIndex());
		int unitPrecision  = unitGetPrecisionFromIndex(this->unitIndex());
		QString owString  = QString::number(currItem->oldWidth * m_docUnitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString ohString  = QString::number(currItem->oldHeight * m_docUnitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString nwString  = QString::number(newX * m_docUnitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString nhString  = QString::number(newY * m_docUnitRatio, 'f', unitPrecision) + " " + unitSuffix;
		QString transacDesc = QString(Um::ResizeFromTo).arg(owString).arg(ohString).arg(nwString).arg(nhString);
		activeTransaction = m_undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(), Um::Resize, transacDesc, Um::IResize);
	}
	int ph = static_cast<int>(qMax(1.0, currItem->lineWidth() / 2.0));
	QTransform ma;
	ma.rotate(currItem->rotation());
	double dX = ma.m11() * (currItem->width() - newX) + ma.m21() * (currItem->height() - newY) + ma.dx();
	double dY = ma.m22() * (currItem->height() - newY) + ma.m12() * (currItem->width() - newX) + ma.dy();
//	#8541, #8761: "when resizing with ALT-arrow, the size values in the PP aren't updated"
//	currItem->setWidthHeight(newX, newY, true);
	currItem->setWidthHeight(newX, newY);
	if ((m_rotMode != 0) && (fromMP) && (!isLoading()) && (appMode == modeNormal))
	{
		double moveX=dX, moveY=dY;
		if (m_rotMode==2)
		{
			moveX/=2.0;
			moveY/=2.0;
		}
		else if (m_rotMode==3)
			moveX=0.0;
		else if (m_rotMode==1)
			moveY=0.0;
		moveItem(moveX, moveY, currItem);
	}
	if ((currItem->asImageFrame()) && (!currItem->Sizing) && (appMode != modeEditClip))
	{
		currItem->AdjustPictScale();
	}
	if (currItem->asLine())
	{
		if (!fromMP)
		{
			FPoint g(currItem->xPos(), currItem->yPos());
			FPoint t(currItem->width(), 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
			t -= g;
			currItem->setRotation( atan2(t.y(), t.x()) * (180.0/M_PI));
			currItem->setWidthHeight( sqrt(pow(t.x(), 2) + pow(t.y(), 2)), 1.0);
			//currItem->setXYPos(currItem->xPos(), currItem->yPos());
		}
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
		                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
		                  -ph,static_cast<int>(currItem->height()+ph));
	}
	setRedrawBounding(currItem);
	currItem->OwnPage = OnPage(currItem);
	if (currItem->Sizing)
	{
		currItem->FrameOnly = true;

		if (currItem->FrameType != 0 && !currItem->asLine())
		{
			currItem->updateClip();
//			currItem->updateGradientVectors();
		}
			
		if (activeTransaction)
		{
			currItem->checkChanges();
			activeTransaction.commit();
		}
		return true;
	}
	if (DoUpdateClip)
	{
		// (JG) #5988 : break image position when resizing frame especially on undo
		/*double nX=0.0, nY=0.0;
		if (fromMP)
		{
			if (currItem->imageFlippedH())
				nX=-(currItem->width() - currItem->OldB2)/currItem->imageXScale();
			if (currItem->imageFlippedV())
				nY=-(currItem->height() - currItem->OldH2)/currItem->imageYScale();
		}
		else
		{
			if (!currItem->imageFlippedH())
				nX=(currItem->width() - currItem->OldB2)/currItem->imageXScale();
			if (!currItem->imageFlippedV())
				nY=(currItem->height() - currItem->OldH2)/currItem->imageYScale();
		}
		if (nX!=0.0 || nY!=0.0)
			currItem->moveImageInFrame(dX,dY);*/
		currItem->updateClip();
	}
//	currItem->updateGradientVectors();
	if (!m_loading)
	{
		QRectF newR(currItem->getBoundingRect());
		invalidateRegion(newR.united(oldR));
	}
	if ((redraw) && (!m_loading))
	{
		QRectF newR(currItem->getBoundingRect());
		regionsChanged()->update(newR.united(oldR));
	}
	if (!fromMP)
	{
		if (m_Selection->isMultipleSelection())
		{
			double gx, gy, gh, gw;
			m_Selection->setGroupRect();
			m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
//			qDebug() << "doc, emit w&h, when was this used?";
			emit widthAndHeight(gw, gh);
		}
	}
	currItem->setCornerRadius(qMin(currItem->cornerRadius(), qMin(currItem->width(),currItem->height())/2));
	if (activeTransaction)
	{
		currItem->checkChanges();
		activeTransaction.commit();
	}
	return true;
}

bool ScribusDoc::moveSizeItem(FPoint newX, FPoint newY, PageItem* currItem, bool fromMP, bool constrainRotation)
{
	QRectF oldR(currItem->getBoundingRect());
	if (currItem->asLine())
	{
		QTransform ma;
		ma.translate(currItem->xPos(), currItem->yPos());
		ma.rotate(currItem->rotation());
		double mx = ma.m11() * currItem->width() + ma.dx();
		double my = ma.m12() * currItem->width() + ma.dy();
		moveItem(newX.x(), newX.y(), currItem);
		double newRot=xy2Deg(mx - currItem->xPos(), my - currItem->yPos());
		currItem->setRotation(newRot);
		currItem->setWidthHeight(sqrt(pow(mx - currItem->xPos(),2)+pow(my - currItem->yPos(),2)), 1.0);
		currItem->updateClip();
		setRedrawBounding(currItem);
		QRectF newR(currItem->getBoundingRect());
		if (!m_loading)
		{
			regionsChanged()->update(oldR);
			regionsChanged()->update(newR);
		}
	}
	else
	{
		currItem->OldB2 = currItem->width();
		currItem->OldH2 = currItem->height();
		if (currItem->rotation() != 0)
		{
			FPoint npv(newX.x(), newX.y());
			QTransform ma3;
			ma3.translate(currItem->xPos(), currItem->yPos());
			ma3.rotate(currItem->rotation());
			double mxc3 = currItem->xPos() - (ma3.m11() * npv.x() + ma3.m21() * npv.y() + ma3.dx());
			double myc3 = currItem->yPos() - (ma3.m22() * npv.y() + ma3.m12() * npv.x() + ma3.dy());
			sizeItem(currItem->width() - newY.x(), currItem->height() - newY.y(), currItem, fromMP, true, false);
			moveItem(-mxc3, -myc3, currItem);
		}
		else
		{
			sizeItem(currItem->width() - newY.x(), currItem->height() - newY.y(), currItem, fromMP, true, false);
			moveItem(newX.x(), newX.y(), currItem);
		}
	}
	return true;
}


void ScribusDoc::adjustItemSize(PageItem *currItem, bool includeGroup, bool moveInGroup)
{
	if (currItem->isArc())
		return;
	m_undoManager->setUndoEnabled(false);
	bool siz = currItem->Sizing;
	currItem->Sizing = false;
	if ((!(currItem->isGroup() || currItem->isSymbol())) || includeGroup)
	{
		double oldX = currItem->xPos();
		double oldY = currItem->yPos();
		double oldW = currItem->width();
		double oldH = currItem->height();
		double oldgW = currItem->groupWidth;
		double oldgH = currItem->groupHeight;
		FPointArray Clip;
		Clip = currItem->PoLine;
		QRectF clipRect = Clip.toQPainterPath(false).boundingRect();
		FPoint tp2(clipRect.left(), clipRect.top());
		sizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem, true, false, false);
		Clip.translate(-tp2.x(), -tp2.y());
		if (currItem->rotation() != 0)
		{
			FPoint npv(tp2.x(), tp2.y());
			moveRotated(currItem, npv);
		}
		else
			moveItem(tp2.x(), tp2.y(), currItem);
		if (!currItem->imageFlippedH())
			currItem->moveImageInFrame(-tp2.x()/currItem->imageXScale(), 0);
		if (!currItem->imageFlippedV())
			currItem->moveImageInFrame(0, -tp2.y()/currItem->imageYScale());
		FPoint tp(clipRect.width(), clipRect.height());
		if (currItem->imageFlippedH())
			currItem->moveImageInFrame((currItem->width() - tp.x())/currItem->imageXScale(), 0);
		if (currItem->imageFlippedV())
			currItem->moveImageInFrame(0, (currItem->height() - tp.y())/currItem->imageYScale());
		sizeItem(clipRect.width(), clipRect.height(), currItem, true, false, false);
		currItem->PoLine = Clip.copy();
		if ((currItem->isGroup() || currItem->isSymbol()) && includeGroup)
		{
			currItem->groupWidth = oldgW * (currItem->width() / oldW);
			currItem->groupHeight = oldgH * (currItem->height() / oldH);
			double dx = (currItem->xPos() - oldX) / (currItem->width() / currItem->groupWidth);
			double dy = (currItem->yPos() - oldY) / (currItem->height() / currItem->groupHeight);
			for (int em = 0; em < currItem->groupItemList.count(); ++em)
			{
				PageItem* embedded = currItem->groupItemList.at(em);
				moveItem(-dx, -dy, embedded);
			}
			if (currItem->imageFlippedH())
			{
				if (oldX - currItem->xPos() == 0)
					moveItem(oldW - currItem->width(), 0, currItem);
				else
					moveItem((oldX - currItem->xPos()), 0, currItem);
			}
			if (currItem->imageFlippedV())
			{
				if (oldY- currItem->yPos() == 0)
					moveItem(0, oldH - currItem->height(), currItem);
				else
					moveItem(0, oldY - currItem->yPos(), currItem);
			}
		}
	}
	currItem->ClipEdited = true;
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if (currItem->asPolyLine() || currItem->asSpiral())
		currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2, 1.0)));
	else if (currItem->asPathText())
		currItem->updatePolyClip();
	else
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	currItem->updateGradientVectors();
	currItem->Sizing = siz;
	m_undoManager->setUndoEnabled(true);
}

void ScribusDoc::moveGroup(double x, double y, Selection* customSelection)
{
	double Scale = 1; //FIXME:av should all be in doc coordinates
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	int selectedItemCount = itemSelection->count();
	if (selectedItemCount == 0)
		return;
	if (x == 0.0 && y == 0.0)
		return;
	
	PageItem* currItem;
	double gx, gy, gw, gh;
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	QRectF OldRect = QRectF(gx, gy, gw, gh);
	QList<PageItem*> weldL;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = itemSelection->itemAt(i);
		if (!weldL.contains(currItem))
		{
			if (currItem->isWelded())
				weldL.append(currItem->itemsWeldedTo());
			moveItem(x, y, currItem);
		}
	}
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	if (itemSelection->isMultipleSelection())
	{
		// FIXME:av emit ItemPos(gx, gy);
		currItem = itemSelection->itemAt(0);
		GroupOnPage(currItem);
	}
	QPoint in2(qRound(gx*Scale), qRound(gy*Scale));
	OldRect = OldRect.united(QRectF(in2.x()/*+contentsX()*/, in2.y()/*+contentsY()*/, qRound(gw*Scale), qRound(gh*Scale))); //FIXME:av
	regionsChanged()->update(OldRect.adjusted(-10, -10, 20, 20));
}

void ScribusDoc::rotateGroup(double angle, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	if (itemSelection->count()==0)
		return;
	double gx, gy, gh, gw;
	FPoint rotationPoint(0, 0);
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	if (this->m_rotMode == 0)
		rotationPoint = FPoint(gx, gy);
	if (this->m_rotMode == 1)
		rotationPoint = FPoint(gx, gy);
	if (this->m_rotMode == 2)
		rotationPoint = FPoint(gx + gw / 2.0, gy + gh / 2.0);
	if (this->m_rotMode == 3)
		rotationPoint = FPoint(gx, gy+gh);
	if (this->m_rotMode == 4)
		rotationPoint = FPoint(gx+gw, gy+gh);
	rotateGroup(angle, rotationPoint, itemSelection);
}

void ScribusDoc::rotateGroup(double angle, FPoint RCenter, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	if (itemSelection->count()==0)
		return;
	double gxS, gyS, ghS, gwS;
	double sc = 1; // FIXME:av Scale;
	PageItem* currItem;
	itemSelection->getGroupRect(&gxS, &gyS, &gwS, &ghS);
	QTransform ma;
	ma.translate(RCenter.x(), RCenter.y());
	ma.scale(1, 1);
	ma.rotate(angle);
	QRect oldR = QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10));
	FPoint n;
	for (int a = 0; a < itemSelection->count(); ++a)
	{
		currItem = itemSelection->itemAt(a);
		n = FPoint(currItem->xPos() - RCenter.x(), currItem->yPos() - RCenter.y());
		currItem->setXYPos(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
		currItem->rotateBy(angle);
		setRedrawBounding(currItem);
	}
	currItem = itemSelection->itemAt(0);
	GroupOnPage(currItem);
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gxS, &gyS, &gwS, &ghS);
	regionsChanged()->update(QRectF(gxS-5, gyS-5, gwS+10, ghS+10).united(oldR));
}

void ScribusDoc::scaleGroup(double scx, double scy, bool scaleText, Selection* customSelection, bool scaleLine)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	Q_ASSERT(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	
	PageItem *bb;
	double gx, gy, gh, gw; //, x, y;
	int aa;
	double sc = 1; //FIXME:av Scale;
	int drm = m_rotMode;
	m_rotMode = 0;
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	QRect oldR(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10));
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	double origGW = gw;
	double origGH = gh;
	updateManager()->setUpdatesDisabled();

	for (int i = 0; i < selectedItemCount; ++i)
	{
		bb = itemSelection->itemAt(i);
		if ((bb->locked()) || (bb->sizeLocked()))
			continue;
		bb->OldB = bb->width();
		bb->OldH = bb->height();
		bb->OldB2 = bb->width();
		bb->OldH2 = bb->height();
		double dw = (bb->width() * scx) - bb->width();
		double dh = (bb->height() * scy) - bb->height();
		double dsch = 1.0;
		double dscw = 1.0;
		if (bb->isArc())
		{
			PageItem_Arc* item = bb->asArc();
			if (bb->height() != 0.0)
				dsch = item->arcHeight / bb->height();
			if (bb->width() != 0.0)
				dscw = item->arcWidth / bb->width();
		}
		bb->Sizing = false;
		double oldRot, oldLocalX, oldLocalY;
		oldRot = bb->rotation();
		oldLocalX = bb->imageXOffset();
		oldLocalY = bb->imageYOffset();
		FPointArray gr;
		gr.addPoint(bb->GrStartX, bb->GrStartY);
		gr.addPoint(bb->GrEndX, bb->GrEndY);
		gr.addPoint(bb->GrFocalX, bb->GrFocalY);
		gr.addPoint(bb->GrControl1);
		gr.addPoint(bb->GrControl2);
		gr.addPoint(bb->GrControl3);
		gr.addPoint(bb->GrControl4);
		gr.addPoint(bb->GrControl5);
		FPoint g(gx, gy);
		FPoint b(0, 0, bb->xPos(), bb->yPos(), bb->rotation(), 1, 1);
		b -= g;
		FPoint b1(b.x(), b.y(), 0, 0, 0, scx, scy);
		FPoint t(bb->width(), 0, bb->xPos(), bb->yPos(), bb->rotation(), 1, 1);
		t -= g;
		FPoint t1(t.x(), t.y(), 0, 0, 0, scx, scy);
		FPoint h(0, bb->height(), bb->xPos(), bb->yPos(), bb->rotation(), 1, 1);
		h -= g;
		FPoint h1(h.x(), h.y(), 0, 0, 0, scx, scy);
		if (bb->isGroup() || scaleLine)				// change the LineWidth only when the item is within a real Group
		{
			if (bb->lineWidth() != 0)				// don't try to scale hairlines
				bb->setLineWidth(qMax(bb->lineWidth()*((scx+scy)/2), 0.01));
		}
		if (bb->itemType() == PageItem::Line)
		{
			bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
			bb->setWidth(sqrt(pow(t1.x()-b1.x(),2)+pow(t1.y()-b1.y(),2)));
			bb->setXYPos(b1.x()+gx, b1.y()+gy);
		}
		else
		{
			FPoint oldPos(bb->xPos(), bb->yPos());
			QTransform ma;
			ma.rotate(bb->rotation());
			bb->PoLine.map(ma);
			QTransform ma2;
			ma2.translate(gx-bb->xPos(), gy-bb->yPos());
			ma2.scale(scx, scy);
			bb->PoLine.map(ma2);
			bb->setRotation(0.0);
			bb->ClipEdited = true;
			if (bb->isArc())
			{
				PageItem_Arc* item = bb->asArc();
				item->arcWidth += dw * dscw;
				item->arcHeight += dh * dsch;
				item->recalcPath();
			}
			if (bb->isSpiral())
			{
				PageItem_Spiral* item = bb->asSpiral();
				item->recalcPath();
			}
			else if (bb->isRegularPolygon())
			{
				PageItem_RegularPolygon* item = bb->asRegularPolygon();
				item->setWidthHeight(item->width() * scx, item->height() * scy, true);
				item->recalcPath();
			}
			else if (bb->isGroup() || bb->isSymbol())
			{
				double oldGW = bb->groupWidth;
				double oldGH = bb->groupHeight;
				adjustItemSize(bb, true, false);
				bb->groupWidth = oldGW;
				bb->groupHeight = oldGH;
			}
			else
				adjustItemSize(bb, true, false);
			if (bb->isArc() || bb->isSpiral() || bb->isRegularPolygon())
				bb->setXYPos(b1.x() + gx, b1.y() + gy);
			else
			{
				QTransform ma3;
				ma3.translate(gx, gy);
				ma3.scale(scx, scy);
				FPoint n(gx-oldPos.x(), gy-oldPos.y());
				double x = ma3.m11() * n.x() + ma3.m21() * n.y() + ma3.dx();
				double y = ma3.m22() * n.y() + ma3.m12() * n.x() + ma3.dy();
				//MoveItem(gx-x, gy-y, bb);
				bb->moveBy(gx - x, gy - y);
			}
			if (oldRot != 0)
			{
				bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
				if (!bb->isArc() && !bb->isSpiral() && !bb->isRegularPolygon())
				{
					QTransform ma;
					ma.rotate(-bb->rotation());
					bb->PoLine.map(ma);
				}
				if (bb->isGroup() || bb->isSymbol())
				{
					double oldGW = bb->groupWidth;
					double oldGH = bb->groupHeight;
					adjustItemSize(bb, true, false);
					bb->groupWidth = oldGW;
					bb->groupHeight = oldGH;
				}
				else
					adjustItemSize(bb, true, false);
			}
		}
		if (scaleText)
		{
			if (bb->itemText.length() != 0)
			{
				for (aa = 0; aa < bb->itemText.length(); ++aa)
				{
					CharStyle fsStyle;
					fsStyle.setFontSize(qMax(qRound(bb->itemText.charStyle(aa).fontSize()*((scx+scy)/2)), 1));
					bb->itemText.applyCharStyle(aa, 1, fsStyle);
				}
				if (bb->asPathText())
					bb->updatePolyClip();
			}
		}
		bb->setImageXYOffset(oldLocalX, oldLocalY);
		bb->OldB2 = bb->width();
		bb->OldH2 = bb->height();
		QTransform ma4;
		ma4.rotate(oldRot);
		ma4.scale(scx, scy);
		gr.map(ma4);
		for (int grow = 0; grow < bb->meshGradientArray.count(); grow++)
		{
			for (int gcol = 0; gcol < bb->meshGradientArray[grow].count(); gcol++)
			{
				bb->meshGradientArray[grow][gcol].transform(ma4);
			}
		}
		bb->ContourLine.map(ma4);
		bb->GrStartX = gr.point(0).x();
		bb->GrStartY = gr.point(0).y();
		bb->GrEndX = gr.point(1).x();
		bb->GrEndY = gr.point(1).y();
		bb->GrFocalX = gr.point(2).x();
		bb->GrFocalY = gr.point(2).y();
		bb->GrControl1 = gr.point(3);
		bb->GrControl2 = gr.point(4);
		bb->GrControl3 = gr.point(5);
		bb->GrControl4 = gr.point(6);
		bb->GrControl5 = gr.point(7);
		bb->updateGradientVectors();
	}
	bb = itemSelection->itemAt(0);
	GroupOnPage(bb);
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	m_rotMode = drm;
	if ((m_rotMode != 0) && (!isLoading()))
	{
		switch (m_rotMode)
		{
		case 2:
			moveGroup((origGW-gw) / 2.0, (origGH-gh) / 2.0);
			break;
		case 4:
			moveGroup(origGW-gw, origGH-gh);
			break;
		case 3:
			moveGroup(0.0, origGH-gh);
			break;
		case 1:
			moveGroup(origGW-gw, 0.0);
			break;
		}
	}
	updateManager()->setUpdatesEnabled();
	regionsChanged()->update(QRectF(gx-5, gy-5, gw+10, gh+10).united(oldR));
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		currItem->gXpos = currItem->xPos() - gx;
		currItem->gYpos = currItem->yPos() - gy;
		currItem->gWidth = gw;
		currItem->gHeight = gh;
	}
	// FIXME:av emit DocChanged();
}

PageItem* ScribusDoc::groupObjectsSelection(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	if (itemSelection->count() < 1)
		return NULL;
	int objectsLayer = itemSelection->objectsLayer();
	if (objectsLayer == -1)
		return NULL;
	PageItem *currItem;
	double x, y, w, h;
	int selectedItemCount = itemSelection->count();
	itemSelection->getVisualGroupRect(&x, &y, &w, &h);
	int lowestItem = 999999;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = itemSelection->itemAt(i);
		currItem->gXpos = currItem->xPos() - x;
		currItem->gYpos = currItem->yPos() - y;
		currItem->gWidth = w;
		currItem->gHeight = h;
		lowestItem = qMin(lowestItem, Items->indexOf(currItem));
	}
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem* currItem = itemSelection->itemAt(i);
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
	}
	double gx = minx;
	double gy = miny;
	double gw = maxx - minx;
	double gh = maxy - miny;
	int z = itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None);
	PageItem *groupItem = Items->takeAt(z);
	Items->insert(lowestItem, groupItem);
	groupItem->setItemName( tr("Group%1").arg(GroupCounter));
	groupItem->AutoName = false;
	groupItem->groupWidth = gw;
	groupItem->groupHeight = gh;
	groupItem->LayerID = objectsLayer;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = itemSelection->itemAt(i);
		int d = Items->indexOf(currItem);
		if (d >= 0)
			groupItem->groupItemList.append(Items->takeAt(d));
		else
			groupItem->groupItemList.append(currItem);
		currItem->Parent = groupItem;
	}
	groupItem->asGroupFrame()->adjustXYPosition();
	itemSelection->clear();
	itemSelection->addItem(groupItem);
	GroupCounter++;
	return groupItem;
}

PageItem* ScribusDoc::groupObjectsList(QList<PageItem*> &itemList)
{
	if (itemList.count() < 1)
		return NULL;
	PageItem *currItem;
	int selectedItemCount = itemList.count();
	int lowestItem = 999999;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = itemList.at(i);
		lowestItem = qMin(lowestItem, Items->indexOf(currItem));
	}
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = itemList.at(i);
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
	}
	double gx = minx;
	double gy = miny;
	double gw = maxx - minx;
	double gh = maxy - miny;
	int z = itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None);
	PageItem *groupItem = Items->takeAt(z);
	Items->insert(lowestItem, groupItem);
	groupItem->setItemName( tr("Group%1").arg(GroupCounter));
	groupItem->AutoName = false;
	groupItem->groupWidth = gw;
	groupItem->groupHeight = gh;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = itemList.at(i);
		int d = Items->indexOf(currItem);
		if (d >= 0)
			groupItem->groupItemList.append(Items->takeAt(d));
		else
			groupItem->groupItemList.append(currItem);
		currItem->gXpos = currItem->xPos() - minx;
		currItem->gYpos = currItem->yPos() - miny;
		currItem->gWidth = maxx - minx;
		currItem->gHeight = maxy - miny;
		currItem->Parent = groupItem;
	}
	groupItem->asGroupFrame()->adjustXYPosition();
	GroupCounter++;
	itemList.clear();
	itemList.append(groupItem);
	return groupItem;
}

void ScribusDoc::groupObjectsToItem(PageItem* groupItem, QList<PageItem*> &itemList)
{
	if (itemList.count() < 1)
		return;
	PageItem *currItem;
	uint selectedItemCount = itemList.count();
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	for (uint ep = 0; ep < selectedItemCount; ++ep)
	{
		currItem = itemList.at(ep);
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
	}
	if (groupItem->width() == 0)
		groupItem->setWidth(maxx - minx);
	if (groupItem->height() == 0)
		groupItem->setHeight(maxy - miny);
	groupItem->groupWidth = groupItem->width();
	groupItem->groupHeight = groupItem->height();
	for (uint c = 0; c < selectedItemCount; ++c)
	{
		currItem = itemList.at(c);
		int d = Items->indexOf(currItem);
		if (d >= 0)
			groupItem->groupItemList.append(Items->takeAt(d));
		else
			groupItem->groupItemList.append(currItem);
		currItem->gXpos = currItem->xPos() - groupItem->xPos();
		currItem->gYpos = currItem->yPos() - groupItem->yPos();
		currItem->gWidth = maxx - minx;
		currItem->gHeight = maxy - miny;
		currItem->Parent = groupItem;
	}
	GroupCounter++;
	groupItem->asGroupFrame()->adjustXYPosition();
	itemList.clear();
	itemList.append(groupItem);
}

PageItem * ScribusDoc::itemSelection_GroupObjects(bool changeLock, bool lock, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	if (itemSelection->count() < 1)
		return NULL;
	int objectsLayer = itemSelection->objectsLayer();
	if (objectsLayer == -1)
		return NULL;
	PageItem *currItem;
	double x, y, w, h;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::Selection, Um::IGroup, Um::Group, "", Um::IGroup);
	QList<PageItem*> selectedItems = itemSelection->items();
	qStableSort(selectedItems.begin(), selectedItems.end(), compareItemLevel);
	int selectedItemCount = selectedItems.count();
	QString tooltip = Um::ItemsInvolved + "\n";
	if (selectedItemCount > Um::ItemsInvolvedLimit)
		tooltip = Um::ItemsInvolved2 + "\n";
	if (changeLock)
	{
		for (int i = 0; i < selectedItemCount; ++i)
		{
			currItem = selectedItems.at(i);
			currItem->setLocked(lock);
			if (selectedItemCount <= Um::ItemsInvolvedLimit)
				tooltip += "\t" + currItem->getUName() + "\n";
		}
	}
	itemSelection->getVisualGroupRect(&x, &y, &w, &h);
	int lowestItem = std::numeric_limits<int>::max();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = selectedItems.at(i);
		currItem->gXpos = currItem->xPos() - x;
		currItem->gYpos = currItem->yPos() - y;
		currItem->gWidth = w;
		currItem->gHeight = h;
		lowestItem = qMin(lowestItem, Items->indexOf(currItem));
	}
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem* currItem = selectedItems.at(i);
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
	}
	double gx = minx;
	double gy = miny;
	double gw = maxx - minx;
	double gh = maxy - miny;
	m_undoManager->setUndoEnabled(false);
	int z = itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None);
	PageItem *groupItem = Items->takeAt(z);
	Items->insert(lowestItem, groupItem);
	groupItem->setItemName( tr("Group%1").arg(GroupCounter));
	groupItem->AutoName = false;
	groupItem->groupWidth = gw;
	groupItem->groupHeight = gh;
	groupItem->LayerID = objectsLayer;
	m_undoManager->setUndoEnabled(true);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = selectedItems.at(i);
		int d = Items->indexOf(currItem);
		groupItem->groupItemList.append(Items->takeAt(d));
		currItem->Parent = groupItem;
	}
	groupItem->asGroupFrame()->adjustXYPosition();

	if (UndoManager::undoEnabled())
	{
		ScItemState<QList<QPointer<PageItem> > > *is = new ScItemState<QList<QPointer<PageItem> > >(UndoManager::Group);
		is->set("GROUP");
		itemSelection->addItem(groupItem,true);
		is->setItem(itemSelection->selectionList());
		m_undoManager->action(this, is);
	}
	if (activeTransaction)
		activeTransaction.commit();
	itemSelection->clear();
	itemSelection->addItem(groupItem);

	GroupCounter++;
	regionsChanged()->update(QRectF(gx-5, gy-5, gw+10, gh+10));
	emit docChanged();
	if (m_ScMW && ScCore->usingGUI())
	{
		m_ScMW->scrActions["itemAttachTextToPath"]->setEnabled(false);
		m_ScMW->scrActions["itemGroup"]->setEnabled(false);
		m_ScMW->scrActions["itemUngroup"]->setEnabled(true);
	}
	return groupItem;
}

void ScribusDoc::itemSelection_UnGroupObjects(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	if (itemSelection->count() == 0)
		return;

	uint docSelectionCount = itemSelection->count();
	PageItem *currItem;
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::Selection, Um::IGroup, Um::Ungroup, "", Um::IGroup);
	QList<PageItem*> toDelete;
	for (uint a=0; a < docSelectionCount; ++a)
	{
		currItem = itemSelection->itemAt(a);
		if (currItem->isGroup())
			toDelete.append(currItem);
	}
	bool wasLoad = isLoading();
	// Remove group control objects
	setLoading(true);
	itemSelection->delaySignalsOn();
	dontResize = true;
	for (int b = 0; b < toDelete.count(); b++)
	{
		currItem = toDelete.at(b);
		QList<PageItem*> *list = Items;
		list = parentGroup(currItem, Items);
		int d = list->indexOf(currItem);
		if (d >= 0)
			list->removeAt(d);
		itemSelection->removeItem(currItem);
		int gcount = currItem->groupItemList.count();
		for (int c = 0; c < gcount; c++)
		{
			PageItem* gItem = currItem->groupItemList.last();
			removeFromGroup(gItem);
			if (currItem->isGroupChild())
			{
				addToGroup(currItem->Parent, gItem);
				list->insert(d, gItem);
			}
			else
			{
				Items->insert(d, gItem);
				gItem->OwnPage = OnPage(gItem);
			}
			itemSelection->addItem(gItem);
		}
		if (UndoManager::undoEnabled())
		{
			ScItemState<QList<QPointer<PageItem> > > *is = new ScItemState<QList<QPointer<PageItem> > >(UndoManager::Ungroup);
			is->set("UNGROUP");
			Selection tempSelection(this, false);
			tempSelection.addItem(currItem, true);
			is->setItem(tempSelection.selectionList());
			m_undoManager->action(this, is);
		}
	}
	dontResize = false;
	setLoading(wasLoad);
	itemSelection->delaySignalsOff();

	// Delete items after delaySignalsOff() call so that palette are updated before item deletion
	for (int i = 0; i < toDelete.count(); i++)
	{
		currItem = toDelete.at(i);
		if (currItem->isWelded())
			currItem->unWeld();
		delete currItem;
	}

	if (activeTransaction)
		activeTransaction.commit();

	double x, y, w, h;
	itemSelection->connectItemToGUI();
	itemSelection->getGroupRect(&x, &y, &w, &h);
	emit docChanged();
	if (itemSelection->count() > 0)
		m_ScMW->HaveNewSel();
	regionsChanged()->update(QRectF(x-5, y-5, w+10, h+10));
}

void ScribusDoc::addToGroup(PageItem* group, PageItem* item)
{
	QTransform groupTrans = group->getTransform();
	QTransform itemTrans = item->getTransform();
	QPointF grPos = groupTrans.map(QPointF(0, 0));
	QPointF itPos = itemTrans.map(QPointF(0, 0));
	double gRot = getRotationDFromMatrix(groupTrans);
	groupTrans.scale(group->width() / group->groupWidth, group->height() / group->groupHeight);
	double grScXi = 1.0;
	double grScYi = 1.0;
	getScaleFromMatrix(groupTrans, grScXi, grScYi);
	QTransform mm;
	mm.rotate(gRot);
	mm.scale(1.0 / grScXi, 1.0 / grScYi);
	QLineF d = QLineF(0.0, 0.0, itPos.x() - grPos.x(), itPos.y() - grPos.y());
	d = mm.map(d);
	item->gXpos = d.p2().x();
	item->gYpos = d.p2().y();
	sizeItem(item->width() * (1.0 / grScXi), item->height() * (1.0 / grScYi), item, false, true, false);
	if (item->isGroupChild())
		item->Parent->groupItemList.removeAll(item);
	else
		Items->removeAll(item);
	item->Parent = group;
	item->rotateBy(gRot);
	item->setLineWidth(item->lineWidth() / qMax(grScXi, grScYi));
	item->setImageXScale(item->imageXScale() / grScXi);
	item->setImageYScale(item->imageYScale() / grScYi);

	itemTrans = item->getTransform();
	if (itemTrans.m11() < 0)
	{
		item->gXpos -= item->width();
		if (item->isImageFrame() || item->isTextFrame() || item->isLatexFrame() || item->isOSGFrame() || item->isSymbol() || item->isGroup() || item->isSpiral())
			item->flipImageH();
		if (item->itemType() != PageItem::Line)
		{
			QTransform ma;
			ma.scale(-1, 1);
			item->PoLine.map(ma);
			item->PoLine.translate(item->width(), 0);
		}
	}
	if (itemTrans.m22() < 0)
	{
		item->gYpos -= item->height();
		if (item->isImageFrame() || item->isTextFrame() || item->isLatexFrame() || item->isOSGFrame() || item->isSymbol() || item->isGroup() || item->isSpiral())
			item->flipImageV();
		if (item->itemType() != PageItem::Line)
		{
			QTransform ma;
			ma.scale(1, -1);
			item->PoLine.map(ma);
			item->PoLine.translate(0, item->height());
		}
	}
}

void ScribusDoc::removeFromGroup(PageItem* item)
{
	if (!item->isGroupChild())
		return;
	PageItem* group = item->Parent;
	QTransform itemTrans = item->getTransform();
	QTransform groupTrans = group->getTransform();
	group->groupItemList.removeAll(item);
	item->Parent = NULL;
	double grScXi = 1.0;
	double grScYi = 1.0;
	getScaleFromMatrix(itemTrans, grScXi, grScYi);
	double gRot = getRotationDFromMatrix(groupTrans);
	sizeItem(item->width() * grScXi, item->height() * grScYi, item, false, true, false);
	if (group->imageFlippedH())
	{
		groupTrans.translate(group->width(), 0);
		groupTrans.translate(-item->width(), 0);
		groupTrans.scale(-1, 1);
		if (item->isImageFrame() || item->isTextFrame() || item->isLatexFrame() || item->isOSGFrame() || item->isSymbol() || item->isGroup() || item->isSpiral())
			item->flipImageH();
		if (item->itemType() != PageItem::Line)
		{
			QTransform ma;
			ma.scale(-1, 1);
			item->PoLine.map(ma);
			item->PoLine.translate(item->width(), 0);
		}
	}
	if (group->imageFlippedV())
	{
		groupTrans.translate(0, group->height());
		groupTrans.translate(0, -item->height());
		groupTrans.scale(1, -1);
		if (item->isImageFrame() || item->isTextFrame() || item->isLatexFrame() || item->isOSGFrame() || item->isSymbol() || item->isGroup() || item->isSpiral())
			item->flipImageV();
		if (item->itemType() != PageItem::Line)
		{
			QTransform ma;
			ma.scale(1, -1);
			item->PoLine.map(ma);
			item->PoLine.translate(0, item->height());
		}
	}
	QPointF itPos = groupTrans.map(QPointF(item->gXpos * grScXi, item->gYpos * grScYi));
	double nX = itPos.x();
	double nY = itPos.y();
	if (item->isTextFrame() || item->isPathText())
	{
		if (item->itemText.length() != 0)
		{
			for (int aa = 0; aa < item->itemText.length(); ++aa)
			{
				CharStyle fsStyle;
				fsStyle.setFontSize(qMax(qRound(item->itemText.charStyle(aa).fontSize()*((grScXi+grScYi)/2)), 1));
				item->itemText.applyCharStyle(aa, 1, fsStyle);
			}
			if (item->asPathText())
				item->updatePolyClip();
		}
	}
	item->setXYPos(nX, nY, true);
	item->rotateBy(-gRot);
	item->setLineWidth(item->lineWidth() * qMax(grScXi, grScYi));
	if (!item->ScaleType)
		item->AdjustPictScale();
	else
	{
		item->setImageXScale(item->imageXScale() * grScXi);
		item->setImageYScale(item->imageYScale() * grScYi);
	}
	if (item->GrType == 8)
	{
		double psx, psy, pox, poy, prot, pskx, psky;
		item->patternTransform(psx, psy, pox, poy, prot, pskx, psky);
		item->setPatternTransform(psx * grScXi, psy * grScYi, pox, poy, prot, pskx, psky);
	}
	if ((item->GrMask == 3) || (item->GrMask == 6) || (item->GrMask == 7) || (item->GrMask == 8))
	{
		double psx, psy, pox, poy, prot, pskx, psky;
		item->maskTransform(psx, psy, pox, poy, prot, pskx, psky);
		item->setMaskTransform(psx * grScXi, psy * grScYi, pox, poy, prot, pskx, psky);
	}
	if (item->isArc())
	{
		PageItem_Arc* ite = item->asArc();
		ite->arcWidth = ite->arcWidth * grScXi;
		ite->arcHeight = ite->arcHeight * grScYi;
		ite->recalcPath();
	}
	if (item->isSpiral())
	{
		PageItem_Spiral* ite = item->asSpiral();
		ite->recalcPath();
	}
	if (item->asPathText())
		item->updatePolyClip();
	else
		item->Clip = FlattenPath(item->PoLine, item->Segments);
	setRedrawBounding(item);
}

void ScribusDoc::rescaleGroup(PageItem* group, double scale)
{
	group->PoLine.scale(scale, scale);
	group->setWidthHeight(group->width() * scale, group->height() * scale);
	group->ClipEdited = true;
	group->FrameType = 3;
	adjustItemSize(group, true);
	group->OldB2 = group->width();
	group->OldH2 = group->height();
	if (group->isGroup())
		group->asGroupFrame()->adjustXYPosition();
	group->updateClip();
	setRedrawBounding(group);
}

void ScribusDoc::resizeGroupToContents(PageItem* group)
{
	PageItem_Group* currItem = group->asGroupFrame();
	if (currItem == NULL)
		return;
	QTransform groupTrans = group->getTransform();
	QPainterPath input1 = currItem->PoLine.toQPainterPath(true);
	if (currItem->fillEvenOdd())
		input1.setFillRule(Qt::OddEvenFill);
	else
		input1.setFillRule(Qt::WindingFill);
	input1 = groupTrans.map(input1);
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	double oldX = currItem->xPos();
	double oldY = currItem->yPos();
	double oldW = currItem->width();
	double oldH = currItem->height();
	int gcount = currItem->groupItemList.count();
	double scw = currItem->width() / currItem->groupWidth;
	double sch = currItem->height() / currItem->groupHeight;
	for (int c = 0; c < gcount; c++)
	{
		PageItem* gItem = currItem->groupItemList.at(c);
		gItem->setXYPos(oldX + gItem->gXpos, oldY + gItem->gYpos, true);
		double x1, x2, y1, y2;
		gItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
	}
	groupTrans.scale(scw, sch);
	QPointF newXY = groupTrans.map(QPointF(minx - oldX, miny - oldY));
	currItem->setXYPos(minx, miny, true);
	currItem->setWidthHeight((maxx - minx) * scw, (maxy - miny) * sch, true);
	currItem->groupWidth = maxx - minx;
	currItem->groupHeight = maxy - miny;
	for (int c = 0; c < gcount; c++)
	{
		PageItem* gItem = currItem->groupItemList.at(c);
		gItem->gXpos = (gItem->xPos() - currItem->xPos());
		gItem->gYpos = (gItem->yPos() - currItem->yPos());
		gItem->gWidth = maxx - minx;
		gItem->gHeight = maxy - miny;
	}
	currItem->setXYPos(newXY.x(), newXY.y(), true);
	currItem->SetRectFrame();
	currItem->ClipEdited = true;
	currItem->FrameType = 3;
	currItem->adjustXYPosition();
	if ((currItem->width() != oldW) || (currItem->height() != oldH))
	{
		QTransform groupTrans2 = group->getTransform();
		QPainterPath input2 = currItem->PoLine.toQPainterPath(true);
		if (currItem->fillEvenOdd())
			input2.setFillRule(Qt::OddEvenFill);
		else
			input2.setFillRule(Qt::WindingFill);
		input2 = groupTrans2.map(input2);
		QPainterPath result = input1.intersected(input2);
		result = groupTrans2.inverted().map(result);
		currItem->PoLine.fromQPainterPath(result, true);
	}
}

void ScribusDoc::itemSelection_resizeGroupToContents(Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	if (itemSelection->count() == 0)
		return;
	int docSelectionCount = itemSelection->count();
	for (int a = 0; a < docSelectionCount; ++a)
	{
		PageItem *group = itemSelection->itemAt(a);
		if (group->isGroup())
			resizeGroupToContents(group);
	}
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_UniteItems(Selection* /*customSelection*/)
{
	int docSelectionCount = m_Selection->count();
	if (docSelectionCount <= 1)
		return;
	PageItem *currItem;
	PageItem *bb;
	QList<QTransform> transform;
	QList<PageItem *> toDel;
	toDel.clear();
	UndoTransaction transaction;
	if (UndoManager::undoEnabled())
	{
		transaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::UniteItem, "", Um::IGroup);
		transform.clear();
	}
	m_undoManager->setUndoEnabled(false);
	currItem = m_Selection->itemAt(0);
	if (currItem->isGroup())
		return;
	m_Selection->delaySignalsOn();
	bool currClipEdited = currItem->ClipEdited;
	int currFrameType = currItem->FrameType;
	currItem->ClipEdited = true;
	currItem->FrameType = 3;
	for (int i = 1; i < docSelectionCount; ++i)
	{
		bb = m_Selection->itemAt(i);
		toDel.append(bb);
		QTransform ma;
		ma.translate(bb->xPos(), bb->yPos());
		ma.rotate(bb->rotation());
		QTransform ma2;
		ma2.translate(currItem->xPos(), currItem->yPos());
		ma2.rotate(currItem->rotation());
		ma2 = ma2.inverted();
		ma = ma * ma2;
		bb->PoLine.map(ma);
		m_undoManager->setUndoEnabled(true);
		if (UndoManager::undoEnabled())
			transform.append(ma);
		m_undoManager->setUndoEnabled(false);
		currItem->PoLine.setMarker();
		currItem->PoLine.putPoints(currItem->PoLine.size(), bb->PoLine.size(), bb->PoLine);
	}
	int oldRotMode = m_rotMode;
	m_rotMode = 0;
	adjustItemSize(currItem);
	m_rotMode = oldRotMode;
	currItem->ContourLine = currItem->PoLine.copy();
	m_undoManager->setUndoEnabled(true);
	//FIXME: stop using m_View
	if (UndoManager::undoEnabled())
	{
		ScItemState< QPair<QList<PageItem*> , QList<QTransform> > > *is = new ScItemState< QPair<QList<PageItem*> , QList<QTransform> > >(Um::UniteItem, "", Um::IGroup);
		is->setItem(qMakePair(toDel,transform));
		is->set("UNITEITEM");
		is->set("FRAMETYPE",currFrameType);
		is->set("CLIPEDITED",currClipEdited);
		m_undoManager->action(currItem, is);
	}
	m_View->Deselect(true);
	for (int c = 0; c < toDel.count(); ++c)
		m_View->SelectItem(toDel.at(c));
	m_Selection->delaySignalsOff();
	itemSelection_DeleteItem();
	regionsChanged()->update(QRectF());
	if (transaction)
		transaction.commit();
	m_Selection->addItem(currItem);
}

void ScribusDoc::itemSelection_SplitItems(Selection* /*customSelection*/)
{
	PageItem *bb;
	m_Selection->delaySignalsOn();
	UndoTransaction transaction;
	if (UndoManager::undoEnabled())
		transaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::SplitItem, "", Um::IGroup);
	m_undoManager->setUndoEnabled(false);
	int oldRotMode = m_rotMode;
	m_rotMode = 0;
	for (int i = 0; i < m_Selection->count(); ++i)
	{
		QList< int> itemsList;
		itemsList.clear();
		PageItem *currItem = m_Selection->itemAt(i);
		if (!currItem->isPolygon() || currItem->Segments.count() <= 0)
			continue;
		uint StartInd = 0;
		int currItemNr = Items->indexOf(currItem);
		uint EndInd = currItem->PoLine.size();
		m_Selection->clear();
		m_Selection->addItem(currItem);
		for (uint a = EndInd-1; a > 0; --a)
		{
			if (currItem->PoLine.isMarker(a))
			{
				StartInd = a + 1;
				bb = new PageItem_Polygon(*currItem);
				currItemNr++;
				itemsList.append(currItemNr);
				Items->insert(currItemNr, bb);
				bb->convertTo(PageItem::Polygon);
				bb->FrameType = 3;
				bb->PoLine.resize(0);
				bb->PoLine.putPoints(0, EndInd - StartInd, currItem->PoLine, StartInd);
				bb->setRotation(currItem->rotation());
				adjustItemSize(bb);
				bb->ContourLine = bb->PoLine.copy();
				bb->ClipEdited = true;
				m_Selection->addItem(bb, false);
				a -= 3;
				EndInd = StartInd - 4;
			}
		}
		currItem->PoLine.resize(StartInd-4);
		adjustItemSize(currItem);
		currItem->ContourLine = currItem->PoLine.copy();
		currItem->ClipEdited = true;
		m_undoManager->setUndoEnabled(true);
		if (UndoManager::undoEnabled())
		{
			ScItemState< QList<int> > *is = new ScItemState< QList<int> >(Um::SplitItem, "", Um::IGroup);
			is->setItem(QList<int>(itemsList));
			is->set("SPLITITEM");
			m_undoManager->action(currItem, is);
		}
		m_undoManager->setUndoEnabled(false);
	}
	m_rotMode = oldRotMode;
	m_undoManager->setUndoEnabled(true);
	m_Selection->delaySignalsOff();
	view()->Deselect(true);
	regionsChanged()->update(QRectF());
	if (transaction)
		transaction.commit();
}

void ScribusDoc::itemSelection_convertItemsTo(const PageItem::ItemType newType, Selection* restoredSelection, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//Create our copy selection as our item *s will be invalidated as we go through the loop and the selection index won't work
	//convertItemTo does this
	Selection tmpSel(*itemSelection);
	tmpSel.disconnectAllItemsFromGUI();
	tmpSel.delaySignalsOn();
	m_updateManager.setUpdatesDisabled();
	UndoTransaction convertTransaction;
	if (UndoManager::undoEnabled() && selectedItemCount > 1)
		convertTransaction = m_undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::ConvertTo, "", Um::IGroup);

	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem)
		{
			if (currItem->isGroup())  // don't convert group control items.
				restoredSelection->addItem(currItem);
			else
			{
				PageItem* newItem=convertItemTo(currItem, newType);
				if (newItem)
				{
					newItem->update();
					if (restoredSelection)
						restoredSelection->addItem(newItem);
				}
			}
		}
	}
	if (convertTransaction)
		convertTransaction.commit();
	tmpSel.delaySignalsOff();
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_convertItemsToSymbol(QString& patternName)
{
	int docSelectionCount = m_Selection->count();
	/* #11365 will be fixed once undo here is fixed
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled()) // && !m_itemCreationTransaction)
		activeTransaction = undoManager->beginTransaction();
	*/
	PageItem* currItem;
	Selection itemSelection(this, false);
	itemSelection.copy(*m_Selection, false);
	m_View->Deselect(true);
	if (docSelectionCount > 1)
		currItem = groupObjectsSelection(&itemSelection);
	else
		currItem = itemSelection.itemAt(0);
	QList<PageItem*> allItems;
	if (currItem->isGroup())
		allItems = currItem->getAllChildren();
	else
		allItems.append(currItem);
	QStringList results;
	for (int i = 0; i < allItems.count(); i++)
	{
		PageItem *item = allItems.at(i);
		if ((!results.contains(item->pattern())) && ((item->GrType == 8) || (item->itemType() == PageItem::Symbol)))
			results.append(item->pattern());
		if (!item->strokePattern().isEmpty())
		{
			if (!results.contains(item->strokePattern()))
				results.append(item->strokePattern());
		}
		if (!item->patternMask().isEmpty())
		{
			if (!results.contains(item->patternMask()))
				results.append(item->patternMask());
		}
	}
	m_ScMW->patternsDependingOnThis.clear();
	QStringList mainPatterns = docPatterns.keys();
	for (int i = 0; i < results.count(); i++)
	{
		QString temp = results[i];
		for (int j = 0; j < mainPatterns.count(); j++)
		{
			if (mainPatterns[j] != temp)
			{
				QStringList subPatterns;
				subPatterns = getUsedPatternsHelper(mainPatterns[j], subPatterns);
				if (subPatterns.contains(temp))
					m_ScMW->patternsDependingOnThis.prepend(mainPatterns[j]);
			}
		}
		m_ScMW->patternsDependingOnThis.prepend(temp);
	}
	 // #12753: We cannot replace currently edited symbol
	if (symbolEditMode())
		m_ScMW->patternsDependingOnThis.prepend(getEditedSymbol());
	allItems.clear();

	ScPattern pat = ScPattern();
	pat.setDoc(this);
	double minx =  std::numeric_limits<double>::max();
	double miny =  std::numeric_limits<double>::max();
	double maxx = -std::numeric_limits<double>::max();
	double maxy = -std::numeric_limits<double>::max();
	double x1, x2, y1, y2;
	currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
	minx = qMin(minx, x1);
	miny = qMin(miny, y1);
	maxx = qMax(maxx, x2);
	maxy = qMax(maxy, y2);
	pat.pattern = currItem->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
	pat.width = maxx - minx;
	pat.height = maxy - miny;
	pat.items.append(currItem);
	// #11274 : OwnPage is not meaningful for pattern items
	// We set consequently pattern item's OwnPage to -1
	QList<PageItem*> patternItems = pat.items;
	while (patternItems.count() > 0)
	{
		PageItem* patItem = patternItems.takeAt(0);
		if (patItem->isGroup())
			patternItems += patItem->groupItemList;
		patItem->OwnPage = -1;
	}
	if (docPatterns.contains(patternName))
		docPatterns.remove(patternName);
	addPattern(patternName, pat);
	int d = -1;
	double sx = minx;
	double sy = miny;
	if (currItem->isGroupChild())
	{
		sx = currItem->gXpos;
		sy = currItem->gYpos;
		d = currItem->parentGroup()->groupItemList.indexOf(currItem);
	}
	else
		d = Items->indexOf(currItem);
	currItem->gXpos = currItem->xPos() - minx;
	currItem->gYpos = currItem->yPos() - miny;
	currItem->setXYPos(currItem->gXpos, currItem->gYpos, true);
	int z = itemAdd(PageItem::Symbol, PageItem::Rectangle, sx, sy, maxx - minx, maxy - miny, 0, CommonStrings::None, CommonStrings::None);
	PageItem* groupItem = Items->takeAt(z);
	groupItem->setPattern(patternName);
	groupItem->Parent = currItem->Parent;
	if (currItem->isGroupChild())
		currItem->parentGroup()->groupItemList.replace(d, groupItem);
	else
		Items->replace(d, groupItem);
	/* #11365 will be fixed once undo here is fixed
	if (UndoManager::undoEnabled())
	{
		ScItemState<QPair<PageItem*, PageItem*> > *is = new ScItemState<QPair<PageItem*, PageItem*> >("Convert Item");
		is->set("CONVERT_ITEM_TO_SYMBOL");
		is->setItem(qMakePair(currItem, groupItem));
		is->setDescription(patternName);
		//Undo target rests with the Page for object specific undo
		UndoObject *target = Pages->at(0);
		if (groupItem->OwnPage > -1)
			target = Pages->at(groupItem->OwnPage);
		undoManager->action(target, is);
	}*/
	m_Selection->delaySignalsOff();
	/*if (activeTransaction)
		activeTransaction.commit();*/
}

//CB-->Doc
//Fix size/move item calls
//CB TODO Use the selection loop properly
void ScribusDoc::itemSelection_AdjustFrametoImageSize( Selection *customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	
	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::SelectionGroup,
															Um::IGroup,
															Um::AdjustFrameToImage, "", Um::IResize);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem!=NULL)
		{
			if (currItem->asImageFrame() && currItem->imageIsAvailable)
			{
				double w, h, x, y;
				w = currItem->OrigW * currItem->imageXScale();
				h = currItem->OrigH * currItem->imageYScale();
				x = currItem->imageXOffset() * currItem->imageXScale();
				y = currItem->imageYOffset() * currItem->imageYScale();
				if ((x != 0.0) || (y != 0.0)) // if the image frame has an offset, its assumed that the user wants the image to stay where it is
				{
					sizeItem(w, h, currItem);
					moveItem(x, y, currItem);
				}
				else
					sizeItem(w, h, currItem, true);
				currItem->setImageXYOffset(0.0, 0.0);
			}
		}
	}
	if (activeTransaction)
		activeTransaction.commit();
	regionsChanged()->update(QRectF());
	changed();
	itemSelection->itemAt(0)->emitAllToGUI();
}
void ScribusDoc::itemSelection_AdjustImagetoFrameSize( Selection *customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem!=NULL)
		{
			if (currItem->asImageFrame() && currItem->imageIsAvailable)
				currItem->setImageScalingMode(false, true);
		}
	}
	regionsChanged()->update(QRectF());
	changed();
	itemSelection->itemAt(0)->emitAllToGUI();
}

void ScribusDoc::itemSelection_AdjustFrameHeightToText( Selection *customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;

	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = m_undoManager->beginTransaction(Um::Selection, Um::ITextFrame, Um::Resize,"", Um::IResize);
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem!=NULL)
		{
			if (currItem->asTextFrame() && (currItem->itemText.length() > 0) && !currItem->isTableItem)
				currItem ->asTextFrame()->setTextFrameHeight();
		}
	}
	if (trans)
		trans.commit();
	regionsChanged()->update(QRectF());
	changed();
	itemSelection->itemAt(0)->emitAllToGUI();
}

void ScribusDoc::itemSelection_AdjustFrameToTable()
{
	// TODO: Do this in an undo transaction?
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount < 1)
		return;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *item = m_Selection->itemAt(i);
		if (item && item->isTable())
		{
			dontResize = true;
			item->asTable()->adjustFrameToTable();
			dontResize = false;
			setRedrawBounding(item);
		}
	}

	regionsChanged()->update(QRectF());
	changed();
}

void ScribusDoc::itemSelection_AdjustTableToFrame()
{
	// TODO: Do this in an undo transaction?
	int selectedItemCount = m_Selection->count();
	if (selectedItemCount < 1)
		return;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *item = m_Selection->itemAt(i);
		if (item && item->isTable())
		{
			dontResize = true;
			item->asTable()->adjustTableToFrame();
			dontResize = false;
			setRedrawBounding(item);
		}
	}

	regionsChanged()->update(QRectF());
	changed();
}



void ScribusDoc::itemSelection_SetColorProfile(const QString & profileName, Selection * customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//TODO Add Undo
	m_updateManager.setUpdatesDisabled();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem && currItem->itemType() == PageItem::ImageFrame)
		{
			currItem->IProfile = profileName;
			currItem->UseEmbedded = profileName.startsWith("Embedded");
			loadPict(currItem->Pfile, currItem, true);
			currItem->update();
		}
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}

void ScribusDoc::itemSelection_SetRenderIntent(int intentIndex, Selection * customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//TODO Add Undo
	m_updateManager.setUpdatesDisabled();
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem && currItem->itemType() == PageItem::ImageFrame)
		{
			currItem->IRender = (eRenderIntent) intentIndex;
			loadPict(currItem->Pfile, currItem, true);
			currItem->update();
		}
	}
	m_updateManager.setUpdatesEnabled();
	changed();
}
 
void ScribusDoc::itemSelection_SetCompressionMethod(int cmIndex, Selection * customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//TODO Add Undo
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem && currItem->itemType() == PageItem::ImageFrame)
		{
			if ((currItem->OverrideCompressionMethod = cmIndex >= 0))
				currItem->CompressionMethodIndex = cmIndex;
		}
	}
}

void ScribusDoc::itemSelection_SetCompressionQuality(int cqIndex, Selection * customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;
	//TODO Add Undo
	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (currItem && currItem->itemType() == PageItem::ImageFrame)
		{
			if ((currItem->OverrideCompressionQuality = cqIndex >= 0))
				currItem->CompressionQualityIndex = cqIndex;
		}
	}
}

QHash<PageItem*, QString> ScribusDoc::getDocItemNames(PageItem::ItemType itemType)
{
	QHash<PageItem*, QString> namesMap;
	QList<PageItem*> allItems;
	int docItemsCount = DocItems.count();
	for (int i = 0; i < docItemsCount; ++i)
	{
		PageItem* ite = DocItems.at(i);
		if (ite->isGroup())
			allItems = ite->getAllChildren();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			if (ite->itemType() == itemType && ite->nextInChain()==NULL && !ite->isAutoFrame())
				namesMap.insert(ite, ite->itemName());
		}
		allItems.clear();
	}
	return namesMap;
}

Serializer *ScribusDoc::serializer()
{
	if (!m_serializer) 
		m_serializer = new Serializer (*this);
	Q_ASSERT(m_serializer);
	return m_serializer;
}

Serializer *ScribusDoc::textSerializer()
{
	if (!m_tserializer) {
		m_tserializer = new Serializer (*this);
		StoryText::desaxeRules("/", *m_tserializer, "SCRIBUSTEXT");
		m_tserializer->addRule("/SCRIBUSTEXT", desaxe::Result<StoryText>());
	}
	Q_ASSERT(m_tserializer);
	return m_tserializer;
}


void ScribusDoc::setRotationMode(const int val)
{
	if (m_rotMode == val)
		return;
	m_rotMode = val;
	emit rotationMode(m_rotMode);
}

void ScribusDoc::setPageSetFirstPage(int layout, int fp)
{
	m_docPrefsData.pageSets[layout].FirstPage=fp;
}

void ScribusDoc::setNewPrefs(const ApplicationPrefs& prefsData, const ApplicationPrefs& oldPrefsData, bool resizePages, bool resizeMasterPages, bool resizePageMargins, bool resizeMasterPageMargins)
{
	m_docPrefsData=prefsData;
	double topDisplacement = prefsData.displayPrefs.scratch.top() - oldPrefsData.displayPrefs.scratch.top();
	double leftDisplacement = prefsData.displayPrefs.scratch.left() - oldPrefsData.displayPrefs.scratch.left();
	applyPrefsPageSizingAndMargins(resizePages, resizeMasterPages, resizePageMargins, resizeMasterPageMargins);

	int docItemsCount = MasterItems.count();
	for (int ite = 0; ite < docItemsCount; ++ite)
	{
		PageItem *item = MasterItems.at(ite);
		item->moveBy(leftDisplacement, topDisplacement);
		item->setRedrawBounding();
	}

//	bool viewToRecalcPictureRes = (m_docPrefsData.itemToolPrefs.imageLowResType==oldPrefsData.itemToolPrefs.imageLowResType);

	restartAutoSaveTimer();

/*	FIXME: scribus determines dict by charstyle now, so this setting should go into the doc's default charstyle
		currDoc->docHyphenator->slotNewDict(ScMW->GetLang(tabHyphenator->language->currentText()));
*/

	docHyphenator->slotNewSettings(m_docPrefsData.hyphPrefs.Automatic, m_docPrefsData.hyphPrefs.AutoCheck);
	docHyphenator->ignoredWords = m_docPrefsData.hyphPrefs.ignoredWords;
	docHyphenator->specialWords = m_docPrefsData.hyphPrefs.specialWords;
	if (ScCore->haveCMS())
	{
		bool cmsChanged = (
					(m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile != oldPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile) ||
					(m_docPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile != oldPrefsData.colorPrefs.DCMSset.DefaultImageCMYKProfile) ||
					(m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile != oldPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile) ||
					(m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile != oldPrefsData.colorPrefs.DCMSset.DefaultSolidColorCMYKProfile) ||
					(m_docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile != oldPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile) ||
					(m_docPrefsData.colorPrefs.DCMSset.DefaultIntentColors != oldPrefsData.colorPrefs.DCMSset.DefaultIntentColors) ||
					(m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages != oldPrefsData.colorPrefs.DCMSset.DefaultIntentImages) ||
					(m_docPrefsData.colorPrefs.DCMSset.SoftProofOn != oldPrefsData.colorPrefs.DCMSset.SoftProofOn) ||
					(m_docPrefsData.colorPrefs.DCMSset.SoftProofFullOn != oldPrefsData.colorPrefs.DCMSset.SoftProofFullOn) ||
					(m_docPrefsData.colorPrefs.DCMSset.GamutCheck != oldPrefsData.colorPrefs.DCMSset.GamutCheck) ||
					(m_docPrefsData.colorPrefs.DCMSset.BlackPoint != oldPrefsData.colorPrefs.DCMSset.BlackPoint) ||
					(m_docPrefsData.colorPrefs.DCMSset.CMSinUse != oldPrefsData.colorPrefs.DCMSset.CMSinUse)
					);
		if (cmsChanged)
		{
			m_ScMW->setStatusBarInfoText( tr("Adjusting Colors"));
			m_ScMW->mainWindowProgressBar->reset();
			int cc = PageColors.count() + MasterItems.count() + DocItems.count() + FrameItems.count();
			m_ScMW->mainWindowProgressBar->setMaximum(cc);
			qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
			bool newCM  = m_docPrefsData.colorPrefs.DCMSset.CMSinUse;
			bool updCol = false;
			m_docPrefsData.colorPrefs.DCMSset.CMSinUse = oldPrefsData.colorPrefs.DCMSset.CMSinUse;
			CloseCMSProfiles();
			m_docPrefsData.colorPrefs.DCMSset.CMSinUse = newCM;
			HasCMS = m_docPrefsData.colorPrefs.DCMSset.CMSinUse;
			SoftProofing = m_docPrefsData.colorPrefs.DCMSset.SoftProofOn;
			Gamut = m_docPrefsData.colorPrefs.DCMSset.GamutCheck;
			IntentColors = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
			IntentImages = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
			if (!m_docPrefsData.colorPrefs.DCMSset.CMSinUse)
			{
				HasCMS = false;
				if	(oldPrefsData.colorPrefs.DCMSset.CMSinUse)
				{
					SetDefaultCMSParams();
					updCol = true;
				}
			}
			else if ( OpenCMSProfiles(ScCore->InputProfiles, ScCore->InputProfilesCMYK, ScCore->MonitorProfiles, ScCore->PrinterProfiles) )
			{
				HasCMS = true;
				m_docPrefsData.pdfPrefs.SComp = m_docPrefsData.colorPrefs.DCMSset.ComponentsInput2;
				m_docPrefsData.pdfPrefs.SolidProf = m_docPrefsData.colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
				m_docPrefsData.pdfPrefs.ImageProf = m_docPrefsData.colorPrefs.DCMSset.DefaultImageRGBProfile;
				m_docPrefsData.pdfPrefs.PrintProf = m_docPrefsData.colorPrefs.DCMSset.DefaultPrinterProfile;
				m_docPrefsData.pdfPrefs.Intent  = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentColors;
				m_docPrefsData.pdfPrefs.Intent2 = m_docPrefsData.colorPrefs.DCMSset.DefaultIntentImages;
				updCol = true;
			}
			else
			{
				SetDefaultCMSParams();
				HasCMS = false;
			}
			if (updCol)
			{
				m_ScMW->recalcColors(m_ScMW->mainWindowProgressBar);
				RecalcPictures(&ScCore->InputProfiles, &ScCore->InputProfilesCMYK, m_ScMW->mainWindowProgressBar);
			}
			m_ScMW->mainWindowProgressBar->setValue(cc);
			qApp->restoreOverrideCursor();
			m_ScMW->setStatusBarInfoText("");
			m_ScMW->mainWindowProgressBar->reset();
		}
	}
	QStringList uf(UsedFonts.keys());
	QMap<QString,int>::Iterator it3;
	UsedFonts.clear();
	QStringList::Iterator it3a;
	QStringList::Iterator it3aend=uf.end();
	for (it3a = uf.begin(); it3a != it3aend; ++it3a)
		AddFont(*it3a);

	uint itemCount=Items->count();
	for (uint b=0; b<itemCount; ++b)
	{
		if (Items->at(b)->itemType() == PageItem::ImageFrame)
			Items->at(b)->setImageVisible(m_docPrefsData.guidesPrefs.showPic);
	}

	double oldBaseGridValue  = oldPrefsData.guidesPrefs.valueBaselineGrid;
	double oldBaseGridOffset = oldPrefsData.guidesPrefs.offsetBaselineGrid;
	if (oldBaseGridValue  != prefsData.guidesPrefs.valueBaselineGrid ||
		oldBaseGridOffset != prefsData.guidesPrefs.offsetBaselineGrid )
	{
		this->invalidateAll();
	}
}

void ScribusDoc::applyPrefsPageSizingAndMargins(bool resizePages, bool resizeMasterPages, bool resizePageMargins, bool resizeMasterPageMargins)
{
	for (int i = 0; i < Pages->count(); ++i)
	{
		ScPage *pp = Pages->at(i);
		if (resizePages)
		{
			pp->setInitialWidth(pageWidth());
			pp->setInitialHeight(pageHeight());
			pp->setHeight(pageHeight());
			pp->setWidth(pageWidth());
			pp->m_pageSize = pageSize();
			pp->setOrientation(pageOrientation());
		}
		if (resizePageMargins)
		{
			pp->initialMargins=m_docPrefsData.docSetupPrefs.margins;
			pp->marginPreset=m_docPrefsData.docSetupPrefs.marginPreset;
		}
		else
		if (resizeMasterPageMargins)
		{
			//CB #6796: find the master page (*mp) for the current page (*pp)
			//check if *pp's margins are the same as the *mp's current margins
			//apply new margins if same
			const int masterPageNumber = MasterNames[pp->MPageNam];
			const ScPage* mp = MasterPages.at(masterPageNumber);
			if (pp->initialMargins.left() == mp->initialMargins.left() &&
				pp->initialMargins.top() == mp->initialMargins.top() &&
				pp->initialMargins.right() == mp->initialMargins.right() &&
				pp->initialMargins.bottom() == mp->initialMargins.bottom())
			{
				pp->initialMargins=m_docPrefsData.docSetupPrefs.margins;
				pp->marginPreset=m_docPrefsData.docSetupPrefs.marginPreset;
			}
		}
	}
	for (int i = 0; i < MasterPages.count(); ++i)
	{
		ScPage *pp = MasterPages.at(i);
		if (resizeMasterPages)
		{
			pp->setInitialWidth(pageWidth());
			pp->setInitialHeight(pageHeight());
			pp->setHeight(pageHeight());
			pp->setWidth(pageWidth());
			pp->m_pageSize = pageSize();
			pp->setOrientation(pageOrientation());
		}
		if (resizeMasterPageMargins)
		{
			pp->initialMargins=m_docPrefsData.docSetupPrefs.margins;
			pp->marginPreset=m_docPrefsData.docSetupPrefs.marginPreset;
		}
		pp->setXOffset(scratch()->left());
		pp->setYOffset(scratch()->top());
	}
}

void ScribusDoc::itemSelection_UnlinkTextFrameAndKeepText( Selection *customSelection, bool cutText)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : m_Selection;
	assert(itemSelection!=0);
	int selectedItemCount = itemSelection->count();
	if (selectedItemCount == 0)
		return;

	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = itemSelection->itemAt(i);
		if (!currItem || !currItem->asTextFrame())
			continue;
		if (currItem->nextInChain() || currItem->prevInChain())
			currItem->unlinkWithText(cutText);
	}
	regionsChanged()->update(QRectF());
	changed();
	itemSelection->itemAt(0)->emitAllToGUI();
}

void ScribusDoc::itemSelection_UnlinkTextFrameAndCutText( Selection *customSelection)
{
	itemSelection_UnlinkTextFrameAndKeepText(customSelection, true);
}

void ScribusDoc::itemSelection_UnWeld()
{
	PageItem *currItem;
	for (int i = 0; i < m_Selection->count(); ++i)
	{
		currItem = m_Selection->itemAt(i);
		if (currItem->isWelded())
		{
			currItem->unWeld();
			changed();
		}
	}
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_Weld()
{
	if (m_Selection->count() != 2)
		return;
	PageItem * master = m_Selection->itemAt(1);
	PageItem * selItem = m_Selection->itemAt(0);
	selItem->weldTo(master);
	changed();
	regionsChanged()->update(QRectF());
}

void ScribusDoc::itemSelection_EditWeld()
{
	m_ScMW->view->requestMode(modeEditWeldPoint);
}

int ScribusDoc::addToInlineFrames(PageItem *item)
{
	int fIndex = qrand();
	while (FrameItems.contains(fIndex))
	{
		fIndex = qrand();
	}
	item->inlineCharID = fIndex;
	double lw = item->visualLineWidth() / 2.0;
	item->setXYPos(lw, lw, true);
	FrameItems.insert(fIndex, item);
	return fIndex;
}

void ScribusDoc::removeInlineFrame(int fIndex)
{
	QList<PageItem*> allItems;
	PageItem* it = NULL;
	int counter = 0;
	for (int lc = 0; lc < 2; ++lc)
	{
		switch (lc)
		{
			case 0:
				counter = MasterItems.count();
				break;
			case 1:
				counter = DocItems.count();
				break;
		}
		for (int d = 0; d < counter; ++d)
		{
			switch (lc)
			{
				case 0:
					it = MasterItems.at(d);
					break;
				case 1:
					it = DocItems.at(d);
					break;
			}
			if (it->isGroup())
				allItems = it->getAllChildren();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				if (it->isTable())
				{
					for (int row = 0; row < it->asTable()->rows(); ++row)
					{
						for (int col = 0; col < it->asTable()->columns(); col ++)
						{
							TableCell cell = it->asTable()->cellAt(row, col);
							if (cell.row() == row && cell.column() == col)
							{
								PageItem* textFrame = cell.textFrame();
								checkItemForFrames(textFrame, fIndex);
							}
						}
					}
				}
				else
					checkItemForFrames(it, fIndex);
			}
			allItems.clear();
		}
	}
	for (QHash<int, PageItem*>::iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		PageItem *ite = itf.value();
		if (ite->isGroup())
			allItems = ite->getAllChildren();
		else
			allItems.append(ite);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			ite = allItems.at(ii);
			if (ite->isTable())
			{
				for (int row = 0; row < ite->asTable()->rows(); ++row)
				{
					for (int col = 0; col < ite->asTable()->columns(); col ++)
					{
						TableCell cell = ite->asTable()->cellAt(row, col);
						if (cell.row() == row && cell.column() == col)
						{
							PageItem* textFrame = cell.textFrame();
							checkItemForFrames(textFrame, fIndex);
						}
					}
				}
			}
			else
				checkItemForFrames(ite, fIndex);
		}
		allItems.clear();
	}
	QStringList patterns = getUsedPatterns();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			it = pa.items.at(o);
			if (it->isGroup())
				allItems = it->getAllChildren();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				checkItemForFrames(it, fIndex);
			}
			allItems.clear();
		}
	}
	it = FrameItems.take(fIndex);
	delete it;
	changed();
	regionsChanged()->update(QRect());
}


void ScribusDoc::checkItemForFrames(PageItem *it, int fIndex)
{
	QList<int> deleteList;
	deleteList.clear();
	if (!it->isTextFrame() && !it->isPathText())
		return;
	int start = 0;
	int stop  = it->itemText.length();
	for (int e = start; e < stop; ++e)
	{
		if (it->itemText.hasObject(e))
		{
			if (it->itemText.object(e).getInlineCharID() == fIndex)
				deleteList.prepend(e);
		}
	}
	for (int a = 0; a < deleteList.count(); a++)
	{
		it->itemText.removeChars(deleteList[a], 1);
	}
	it->invalid = true;
}

bool ScribusDoc::hasPreflightErrors()
{
	return (
			(pageErrors.count() != 0) ||
			(docItemErrors.count() != 0) ||
			(masterItemErrors.count() != 0) ||
			(docLayerErrors.count() != 0)
			);
}

void ScribusDoc::itemResizeToMargin(PageItem* item, int direction)
{
	//FIX ME: for now avoid for rotated items
	if (item->rotation() != 0)
		return;
	Canvas::FrameHandle fh = (Canvas::FrameHandle) direction;
	ScPage *currPage = Pages->at(item->OwnPage);
	QMatrix ma;
	ma.translate(item->xPos(), item->yPos());
	double inX = ma.dx() - currentPage()->xOffset();
	double inY = ma.dy() - currentPage()->yOffset();
	if (fh == Canvas::NORTH || fh == Canvas::NORTHWEST || fh == Canvas::NORTHEAST)
	{
		double top = currPage->topMargin();
		double dY = inY - top;
		item->moveBy(0, -dY);
		item->setHeight(item->height() + dY);
	}
	if (fh == Canvas::SOUTH || fh == Canvas::SOUTHWEST || fh == Canvas::SOUTHEAST)
	{
		double bottom = currPage->height() - currPage->bottomMargin();
		double dY = bottom - (inY + item->height());
		item->setHeight(item->height() + dY);
	}
	if (fh == Canvas::EAST || fh == Canvas::NORTHEAST || fh == Canvas::SOUTHEAST)
	{
		double right = currPage->width() - currPage->rightMargin();
		double dX = right - (inX + item->width());
		item->setWidth(item->width() + dX);
	}
	if (fh == Canvas::WEST || fh == Canvas::NORTHWEST || fh == Canvas::SOUTHWEST)
	{
		double left = currPage->leftMargin();
		double dX = inX - left;
		item->moveBy(-dX, 0);
		item->setWidth(item->width() + dX);
	}

	item->updateClip();
	item->invalid = true;
	changed();
	regionsChanged()->update(QRect());
}

void ScribusDoc::restartAutoSaveTimer()
{
	autoSaveTimer->stop();
	if (m_docPrefsData.docSetupPrefs.AutoSave)
		autoSaveTimer->start(m_docPrefsData.docSetupPrefs.AutoSaveTime);
	emit updateAutoSaveClock();
}

void ScribusDoc::slotAutoSave()
{
	if (!isModified())
		return;
	autoSaveTimer->stop();
	QString base = tr("Document");
	QString path = m_docPrefsData.pathPrefs.documents;
	QString fileName;
	if (hasName)
	{
		QFileInfo fi(DocName);
		base = fi.baseName();
		path = fi.absolutePath();
	}
	QDateTime dat = QDateTime::currentDateTime();
	if ((!m_docPrefsData.docSetupPrefs.AutoSaveLocation) && (!m_docPrefsData.docSetupPrefs.AutoSaveDir.isEmpty()))
		path = m_docPrefsData.docSetupPrefs.AutoSaveDir;
	fileName = QDir::cleanPath(path + "/" + base + QString("_autosave_%1.sla").arg(dat.toString("dd_MM_yyyy_hh_mm")));
	FileLoader fl(fileName);
	if (fl.saveFile(fileName, this, 0))
	{
		scMW()->statusBar()->showMessage( tr("File %1 autosaved").arg(base), 5000);
		if (autoSaveFiles.count() >= m_docPrefsData.docSetupPrefs.AutoSaveCount)
		{
			QFile f(autoSaveFiles.first());
			f.remove();
			autoSaveFiles.removeFirst();
		}
		autoSaveFiles.append(fileName);
	}
	if (m_docPrefsData.docSetupPrefs.AutoSave)
		autoSaveTimer->start(m_docPrefsData.docSetupPrefs.AutoSaveTime);
}

void ScribusDoc::setupNumerations()
{
	QList<NumStruct*> numList = numerations.values();
	while (!numList.isEmpty())
		delete numList.takeFirst();
	numerations.clear();
	
	Numeration num;
	NumStruct * numS = NULL;
	for (int i=0; i < m_docParagraphStyles.count(); ++i)
	{
		ParagraphStyle &style = m_docParagraphStyles[i];
		if (!style.hasNum())
			continue;

		QString name = style.numName();
		if (numerations.contains(name))
			numS = numerations.value(name);
		else
		{
			numS = new NumStruct;
			numS->m_name = name;
		}
		num.numFormat = (NumFormat) style.numFormat();
		num.prefix = style.numPrefix();
		num.suffix = style.numSuffix();
		num.start = style.numStart();
		int level = style.numLevel();
		if (level >= numS->m_counters.count())
		{
			for (int i=numS->m_counters.count(); i <= level; ++i)
			{
				numS->m_nums.insert(i,num);
				numS->m_counters.insert(i, 0);
			}
		}
		numS->m_nums.replace(level, num);
		numS->m_counters.replace(level, num.start -1);
		numS->m_lastlevel = -1;
		numerations.insert(numS->m_name, numS);
	}

	if (!numerations.contains("default"))
	{
		//create default numeration
		numS = new NumStruct;
		numS->m_name = "default";
		Numeration newNum;
		numS->m_nums.insert(0, newNum);
		numS->m_counters.insert(0, 0);
		numS->m_lastlevel = -1;
		numerations.insert("default", numS);
	}

	flag_NumUpdateRequest = false;
	if (orgNumNames != numerations.keys())
	{
		orgNumNames = numerations.keys();
		flag_NumUpdateRequest = true;
	}
	flag_Renumber = true;
}

QString ScribusDoc::getNumberStr(QString numName, int level, bool reset, const ParagraphStyle &style)
{
	Q_ASSERT(numerations.contains(numName));
	NumStruct * numS = numerations.value(numName, (NumStruct*) 0);
	if (!numS)
		return QString();
	numS->m_lastlevel = level;
	if (level >= numS->m_nums.count())
		return QString();

	Numeration& num = numS->m_nums[level];
	num.numFormat = (NumFormat) style.numFormat();
	num.start = style.numStart();
	num.prefix = style.numPrefix();
	num.suffix = style.numSuffix();

	int currNum = numS->m_counters.at(level);
	if (reset)
		currNum = numS->m_nums[level].start -1;
	++currNum;
	setNumerationCounter(numName, level, currNum);

	QString result;
	for (int i=0; i <= level; ++i)
	{
		const Numeration& num = numS->m_nums.at(i);
		result.append(num.prefix);
		result.append(num.numString(numS->m_counters.at(i)));
		result.append(num.suffix);
	}
	return result;
}

void ScribusDoc::setNumerationCounter(QString numName, int level, int number)
{
	if (!numerations.contains(numName))
		return;
	NumStruct* numS = numerations.value(numName);
	if (!numS)
		return;
	if (level > numS->m_counters.count())
		numS->m_counters.insert(level, number);
	else
		numS->m_counters.replace(level, number);
}

bool ScribusDoc::updateLocalNums(StoryText& itemText)
{
	QVector<Numeration> m_nums;
	QList<int> m_counters;
	bool needUpdate = false;
	for (int pos = 0; pos < itemText.length(); ++pos)
	{
		if (pos != 0 && itemText.text(pos-1) != SpecialChars::PARSEP)
			continue;
		Mark* mark = itemText.mark(pos);
		if (mark != NULL && mark->isType(MARKBullNumType) && itemText.paragraphStyle(pos).hasNum())
		{
			ParagraphStyle style = itemText.paragraphStyle(pos);
			if (style.numName() == "<local block>")
			{
				int level = style.numLevel();
				while (m_counters.count() < (level + 1))
				{
					m_counters.append(0);
					Numeration num((NumFormat) style.numFormat());
					m_nums.append(num);
				}
				Numeration num = m_nums.at(level);
				num.prefix = style.numPrefix();
				num.suffix = style.numSuffix();
				num.start = style.numStart();
				num.numFormat = (NumFormat) style.numFormat();
				m_nums.replace(level, num);
				int count = m_counters.at(level);
				bool reset = false;
				if (pos == 0)
					reset = true;
				else if (pos > 0)
				{
					ParagraphStyle prevStyle;
					prevStyle = itemText.paragraphStyle(pos -1);
					reset = !prevStyle.hasNum()
							|| prevStyle.numName() != "<local block>"
							|| prevStyle.numLevel() < level
							|| prevStyle.numFormat() != style.numFormat();
				}
				if ((level == 0) && (style.numFormat() != (int) num.numFormat))
				{
					reset = true;
					m_counters.clear();
					m_counters.append(0);
					m_nums.clear();
					m_nums.append(num);
				}
				if (reset)
					count = style.numStart();
				else
					count++;
				m_counters.replace(level, count);
				//m_nums.insert(level, num);
				QString result;
				for (int i=0; i <= level; ++i)
				{
					result.append(m_nums.at(i).prefix);
					result.append(getStringFromNum(m_nums.at(i).numFormat, m_counters.at(i)));
					result.append(m_nums.at(i).suffix);
				}
				if (mark->getString() != result)
				{
					mark->setString(result);
					needUpdate = true;
				}
			}
		}
	}
	return needUpdate;
}

void ScribusDoc::updateNumbers(bool updateNumerations)
{
	if (updateNumerations)
		//after styles change reset all numerations settings
		setupNumerations();
	//reset ALL counters
	foreach (NumStruct * numS, numerations.values())
		for (int l = 0; l < numS->m_nums.count(); ++l)
			numS->m_counters[l] = numS->m_nums[l].start -1;

	flag_Renumber = false;
	//renumbering for doc, sections, page and frame range
	for (int sec = 0; sec < sections().count(); ++sec)
	{
		//reset section range counters
		foreach (NumStruct * numS, numerations.values())
			for (int l = 0; l < numS->m_nums.count(); ++l)
				if (numS->m_nums[l].range == NSRsection)
					numS->m_counters[l] = numS->m_nums[l].start -1;

		int start = sections().value(sec).fromindex;
		int stop = sections().value(sec).toindex;
		for (int page = start; page <= stop; ++page)
		{
			//reset page range counters
			foreach (NumStruct * numS, numerations.values())
				for (int l = 0; l < numS->m_nums.count(); ++l)
					if (numS->m_nums[l].range == NSRpage)
						numS->m_counters[l] = numS->m_nums[l].start -1;
			for (int i=0; i < DocItems.count(); ++i)
			{
				PageItem* item = DocItems.at(i);
				if (item->OwnPage != page)
					continue;
				if (!item->isTextFrame())
					continue;
				if (item->invalid)
					continue;

				//reset items and stories range counters
				foreach (NumStruct * numS, numerations.values())
					for (int l = 0; l < numS->m_nums.count(); ++l)
						if ((numS->m_nums[l].range == NSRframe) || ((numS->m_nums[l].range == NSRstory) && (item->prevInChain() == NULL)))
							numS->m_counters[l] = numS->m_nums[l].start -1;

				int pos = item->firstInFrame();
				int last = item->lastInFrame();
				if (pos > last) continue;

				if ((pos != 0) && (item->itemText.text(pos-1) != SpecialChars::PARSEP))
					pos = item->itemText.nextParagraph(pos)+1;
				int len = item->itemText.length();
				while (pos <= last)
				{
					if ((pos == 0) || (item->itemText.text(pos - 1) == SpecialChars::PARSEP))
					{
						const ParagraphStyle& style = item->itemText.paragraphStyle(pos);
						if (style.hasNum() && style.numName() != "<local block>")
						{
							if (!numerations.contains(style.numName()))
							{
								ParagraphStyle newStyle;
								newStyle.setNumName("<local block>");
								Selection tempSelection(this, false);
								tempSelection.addItem(item, true);
								itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
								continue;
							}
							Mark* mark = item->itemText.mark(pos);
							bool resetNums = false;
							if (numerations.value(style.numName())->m_lastlevel == -1)
								resetNums = true;
							else if (style.numOther())
							{
								ParagraphStyle preStyle = item->itemText.paragraphStyle(pos -1);
								//reset counter if prev style hasn't numeration or has other numeration
								if (!preStyle.hasNum() || (preStyle.numName() != style.numName()))
									resetNums = true;
							}
							else if (style.numHigher() && (style.numLevel() > numerations.value(style.numName())->m_lastlevel))
								resetNums = true;
							
							QString prefixStr = getNumberStr(style.numName(), style.numLevel(), resetNums, style);
							numerations.value(style.numName())->m_lastlevel = style.numLevel();
							if (mark == NULL)
							{
								BulNumMark* bnMark = new BulNumMark;
								item->itemText.insertMark(bnMark,pos);
								CharStyle emptyCS;
								item->itemText.setCharStyle(pos, 1, emptyCS);
							}
							if (mark && mark->getString() != prefixStr)
							{
								mark->setString(prefixStr);
								item->invalid = true;
								flag_Renumber = true;
							}
						}
						if (pos == last)
							break;
						if (item->itemText.text(pos) == SpecialChars::PARSEP)
							++pos;
						else
						{
							pos = item->itemText.nextParagraph(pos)+1;
							if (pos == len)
								break;
						}
					}
				}
			}
		}
	}

	//update local numbering
	foreach (PageItem* item, DocItems)
	{
		if (item->itemText.length() > 0)
		{
			PageItem* itemFirst = item->firstInChain();
			if (updateLocalNums(itemFirst->itemText))
			{
				if (itemFirst->isTextFrame())
					itemFirst->asTextFrame()->invalidateLayout(true);
				else
					itemFirst->invalidateLayout();
			}
		}
	}

}

QStringList ScribusDoc::marksLabelsList(MarkType type)
{
	QStringList nameList;
	for (int a=0; a < m_docMarksList.count(); ++a)
	{
		Mark* m = m_docMarksList.at(a);
		if (m == NULL)
			continue;
		if ((m != NULL) && m->isType(type))
			nameList.append(m->label);
	}
	return nameList;
}

Mark* ScribusDoc::getMark(QString l, MarkType t)
{
	foreach (Mark* mrk, m_docMarksList)
	{
		if ((mrk != NULL) && (mrk->label == l) && mrk->isType(t))
			return mrk;
	}
	return NULL;
}

Mark *ScribusDoc::newMark(Mark* mrk)
{
	Mark* newMrk = new Mark();
	if (mrk != NULL)
		*newMrk = *mrk;
	m_docMarksList.append(newMrk);
	return newMrk;
}

TextNote *ScribusDoc::newNote(NotesStyle* NS)
{
	TextNote* newNote = new TextNote(NS);
	m_docNotesList.append(newNote);
	setNotesChanged(true);
	return newNote;
}

PageItem* ScribusDoc::findMarkItem(Mark* mrk, int &lastItem)
{
	PageItem* item = NULL;
	for (int a = lastItem +1; a < DocItems.count(); ++a)
	{
		item = DocItems.at(a);
		if (!item || !item->isTextFrame() || (item->itemText.length() <= 0))
			continue;
	//	for (int i = item->firstInFrame(); i <= item->lastInFrame(); ++i)
		for (int i = 0; i < item->itemText.length(); ++i)
		{
			if (item->itemText.hasMark(i, mrk))
			{
				lastItem = a;
				return item;
			}
		}
	}
	lastItem = 0;
	return NULL;
}

int ScribusDoc::findMarkCPos(Mark* mrk, PageItem* &currItem, int Start)
{
	if (currItem == NULL)
		currItem = findFirstMarkItem(mrk);
	if (currItem == NULL)
	{
		foreach (PageItem* item, DocItems)
		{
			if (item->isTextFrame() && (item->prevInChain() == NULL))
			{
				for (int pos = 0; pos < item->itemText.length(); ++pos)
				{
					if (item->itemText.hasMark(pos, mrk))
					{
						currItem = item;
						return pos;
					}
				}
			}
		}
		return -1;
	}
	Q_ASSERT(currItem->isTextFrame());

	if (Start < currItem->firstInFrame())
		Start = currItem->firstInFrame();

	for (int i = Start; i < currItem->itemText.length(); ++i)
	{
		if (currItem->itemText.hasMark(i, mrk))
			return i;
	}
	return -1;
}

bool ScribusDoc::isMarkUsed(Mark* mrk, bool visible)
{
	foreach (PageItem* currItem, DocItems)
	{
		if (currItem->isTextFrame() && (currItem->itemText.length() > 0))
		{
			if (!visible && currItem->prevInChain() != NULL)
				//check in whole itemText only for first frames in chain
				continue;
			int i = 0;
			int end = currItem->itemText.length();
			if (visible)
			{
				//search only in visible text
				i = currItem->firstInFrame();
				end = currItem->lastInFrame() +1;
			}
			for (; i < end; ++i)
			{
				if (currItem->itemText.hasMark(i, mrk))
					return true;
			}
		}
	}
	return false;
}

void ScribusDoc::setCursor2MarkPos(Mark *mark)
{
	if (mark == NULL)
		return;
	PageItem* item = NULL;
	if (mark->isType(MARKNoteFrameType) || mark->isType(MARKNoteMasterType))
		item = mark->getItemPtr();
	if (item == NULL)
		item = getItemFromName(mark->getItemName());
	if (item == NULL)
		item = findFirstMarkItem(mark);
	if (item == NULL)
		return;

	int CPos = findMarkCPos(mark, item);
	if (CPos > -1)
	{
		scMW()->deselectAll();
		scMW()->selectItemFromOutlines(item, true, CPos +1);
	}
}

bool ScribusDoc::eraseMark(Mark *mrk, bool fromText, PageItem *item, bool force)
{
	bool found = false;
	if (fromText)
	{
		if (item != NULL)
		{
			int MPos = findMarkCPos(mrk, item);
			while (MPos > -1)
			{
				if (mrk->isType(MARKNoteFrameType) && MPos > 1 && item->itemText.text(MPos -1) == SpecialChars::PARSEP)
					item->itemText.removeChars(MPos-1,2);
				else
					item->itemText.removeChars(MPos,1);
				found = true;
				MPos = findMarkCPos(mrk, item);
			}
		}
		else
		{
			//find and delete all mark`s apperences in text
			int MPos = -1;
			int itemIndex = -1;
			item = findMarkItem(mrk, itemIndex);
			while (item != NULL)
			{
				MPos = findMarkCPos(mrk, item);
				while (MPos > -1)
				{
					item->itemText.removeChars(MPos,1);
					MPos = findMarkCPos(mrk, item);
				}
				found = true;
				item->asTextFrame()->invalidateLayout(false);
				item = findMarkItem(mrk, itemIndex);
			}
		}
	}
	//remove mark references
	for (int a=0; a < m_docMarksList.count(); ++a)
	{
		Mark* m = m_docMarksList.at(a);
		if (m == NULL)
			continue;
		if (m->isType(MARK2MarkType))
		{
			QString l;
			MarkType t;
			m->getMark(l, t);
			if (mrk == getMark(l, t))
			{
				setUndoDelMark(m);
				eraseMark(m, true, NULL, true);
			}
		}
	}
	//erase mark from marksMap
	if (mrk->isUnique() || force)
	{
		m_docMarksList.removeOne(mrk);
		delete mrk;
	}
	return found;
}

void ScribusDoc::setUndoDelMark(Mark *mrk)
{
	//used by MarksManager
	if (UndoManager::undoEnabled())
	{
		ScItemsState* ims = new ScItemsState(Um::DeleteMark, "", Um::IDelete);
		if (mrk->isUnique())
		{
			ims->set("MARK", QString("delete"));
			PageItem* master = findFirstMarkItem(mrk);
			if (master->isNoteFrame())
				ims->set("noteframeName", master->getUName());
			else
				ims->insertItem("inItem", master);
			ims->set("at", findMarkCPos(mrk, master));
			if (mrk->isType(MARK2MarkType))
			{
				QString dName;
				MarkType dType;
				mrk->getMark(dName, dType);
				ims->set("dName", dName);
				ims->set("dType", (int) dType);
			}
			if (mrk->isType(MARK2ItemType))
				ims->insertItem("itemPtr", mrk->getItemPtr());
		}
		else
		{
			ims->set("MARK", QString("delNonUnique"));
			int MPos = -1;
			int itemIndex = -1;
			//find all mark insertions
			PageItem* item = findMarkItem(mrk, itemIndex);
			while (item != NULL)
			{
				int num = 0; //shift of insertion position for undo
				MPos = findMarkCPos(mrk, item);
				while (MPos > -1)
				{
					ims->insertItemPos.append(QPair<void*, int>((void*) item, MPos - num)); //-num as while undo text will be shorter (without marks)
					//++num;
					MPos = findMarkCPos(mrk, item, MPos+1);
				}
				item = findMarkItem(mrk, itemIndex);
			}
		}
		ims->set("ETEA", mrk->label);
		ims->set("label", mrk->label);
		ims->set("type", (int) mrk->getType());
		ims->set("strtxt", mrk->getString());
		m_undoManager->action(this, ims);
	}
}

bool ScribusDoc::invalidateVariableTextFrames(Mark* mrk, bool forceUpdate)
{
	if (!mrk->isType(MARKVariableTextType))
		return false;
	int itemNo = -1;
	bool found = false;
	PageItem* mItem = findMarkItem(mrk, itemNo);
	while (mItem != NULL)
	{
		found = true;
		mItem->asTextFrame()->invalidateLayout(false);
		if (forceUpdate)
			mItem->layout();
		mItem = findMarkItem(mrk, itemNo);
	}
	return found;
}

//update strings (page number) for marks pointed to anchors and items
//and update marks list in Marks Manager
bool ScribusDoc::updateMarks(bool updateNotesMarks)
{
	if (updateNotesMarks && !notesList().isEmpty())
	{
		foreach (PageItem* item, DocItems)
		{
			if (!item->isTextFrame() || item->isNoteFrame())
				continue;
			if (item->prevInChain() != NULL)
				continue;
			item = item->lastInChain();
			int pos = item->lastInFrame() + 1;
			if (pos >= item->itemText.length())
				continue;
			for (int i = pos; i < item->itemText.length(); ++i)
			{
				if (item->itemText.hasMark(i) && item->itemText.mark(i)->isNoteType())
				{
					TextNote * note = item->itemText.mark(i)->getNotePtr();
					note->setNoteMark(NULL);
					note->masterMark()->setItemPtr(item);
					note->masterMark()->setItemName(item->itemName());
				}
			}
		}
	}
	if (m_docMarksList.isEmpty())
	{
		if ((!notesList().isEmpty() || notesChanged()) && updateNotesMarks && !isLoading())
			return notesFramesUpdate();
		return false;
	}
	Q_ASSERT(m_docMarksList.removeAll(NULL) == 0);

	bool docWasChanged = false;

	if (!isLoading())
	{
		//run for variable text (invalidate frames with variable text)
		foreach (Mark* mrk, m_docMarksList)
		{
			if (mrk->isType(MARKVariableTextType))
			{
				if (invalidateVariableTextFrames(mrk, false))
					docWasChanged = true;
			}
			else if (mrk->isUnique())
			{
				PageItem* mItem = findFirstMarkItem(mrk);
				mrk->OwnPage =(mItem != NULL) ? mItem->OwnPage : -1;
				mrk->setItemName((mItem != NULL) ? mItem->itemName() : QString(""));
			}
		}
	}
	
	//update marks for foot/endnotes
	if (updateNotesMarks)
	{
		//update notes numbers
		for (int i=0; i < m_docNotesStylesList.count(); ++i)
		{
			NotesStyle* NS = m_docNotesStylesList.at(i);
			//update nums
			if (updateNotesNums(NS))
			{
				//refresh endnotes content
				if (flag_updateEndNotes || isLoading())
					updateEndnotesFrames(NS);
			}
			if (flag_updateEndNotes)
				updateEndnotesFrames(NS);
			updateNotesFramesStyles(NS);
		}
		docWasChanged = notesFramesUpdate();
	}

	//for all marks
	foreach (Mark* mrk, m_docMarksList)
	{
		//set mark page number
		PageItem* mItem = findFirstMarkItem(mrk);
		if (mItem != NULL)
			mrk->OwnPage = mItem->OwnPage;
		else
			mrk->OwnPage = -1;
		if (mrk->isType(MARK2ItemType))
		{
			if (mrk->getItemPtr() != NULL)
			{
				mrk->setString(getSectionPageNumberForPageIndex(mrk->getItemPtr()->OwnPage));
				if (mItem != NULL)
				{
					mItem->asTextFrame()->invalidateLayout(false);
					docWasChanged = true;
				}
			}
			else
				mrk->setString("");
		}
		else if (mrk->isType(MARK2MarkType))
		{
			QString l;
			MarkType t;
			mrk->getMark(l,t);
			Mark* destMark = getMark(l,t);
			if (destMark != NULL)
			{
				PageItem* dItem = findFirstMarkItem(destMark);
				if (dItem == NULL)
				{
					destMark->OwnPage = -1;
					mrk->setString("");
					docWasChanged = true;
				}
				else
				{
					destMark->OwnPage = dItem->OwnPage;
					mrk->setString(getSectionPageNumberForPageIndex(destMark->OwnPage));
					if (mItem != NULL)
					{
						mItem->asTextFrame()->invalidateLayout(false);
						docWasChanged = true;
					}
				}
			}
			else
			{
				mrk->setString("");
				docWasChanged = true;
			}
		}
	}
	return docWasChanged;
}

NotesStyle* ScribusDoc::newNotesStyle(NotesStyle NS)
{
	QString nsName = NS.name();

	//if doc is loading overwrite current set
	NotesStyle* newNS = getNotesStyle(nsName);
	if (isLoading() && (newNS != NULL))
		*newNS = NS;
	else
	{
		//add new notes style
		if (!validateNSet(NS))
			return NULL;
		newNS = new NotesStyle();
		*newNS = NS;
		QStringList nsNames;
		for (int a = 0; a< m_docNotesStylesList.count(); ++a)
			nsNames.append(m_docNotesStylesList.at(a)->name());
		getUniqueName(nsName, nsNames, "_");
		newNS->setName(nsName);
		m_docNotesStylesList.append(newNS);
		if (m_undoManager->undoEnabled())
		{
			SimpleState* ss = new SimpleState(UndoManager::NewNotesStyle);
			ss->set("NSTYLE", QString("new"));
			undoSetNotesStyle(ss, newNS);
			m_undoManager->action(this, ss);
		}
	}
	return newNS;
}

void ScribusDoc::renameNotesStyle(NotesStyle* NS, QString newName)
{
	foreach (Mark* mrk, m_docMarksList)
		mrk->label.replace("_" + NS->name()+ "_", "_" + newName + "_");
	NS->setName(newName);
}

void ScribusDoc::deleteNotesStyle(QString nsName)
{
	NotesStyle* NS = getNotesStyle(nsName);
	assert(NS != NULL);
	//do not delete default notes style
	if (NS == m_docNotesStylesList.at(0))
		return;
	UndoTransaction activeTransaction;
	if (UndoManager::instance()->undoEnabled())
		activeTransaction = m_undoManager->beginTransaction(Um::DeleteNotesStyle, Um::IDelete, Um::Delete, "", Um::IDelete);
	QList<TextNote*> toDel;
	//search for notes to deleting
	foreach (TextNote* note, m_docNotesList)
	{
		if (note->notesStyle() == NS)
			toDel.append(note);
	}
	//deleting notes
	QList<PageItem*> master2Update;
	if (!toDel.isEmpty())
	{
		while (!toDel.isEmpty())
		{
			TextNote* note = toDel.takeFirst();
			master2Update.append(findFirstMarkItem(note->masterMark()));
			setUndoDelNote(note);
			deleteNote(note);
		}
		while (!master2Update.isEmpty())
			master2Update.takeFirst()->layout();
	}
	if (activeTransaction)
	{
		//undo is enabled
		SimpleState* ss = new SimpleState(UndoManager::DeleteNotesStyle);
		ss->set("NSTYLE", QString("delete"));
		undoSetNotesStyle(ss, NS);
		m_undoManager->action(this, ss);
		
		activeTransaction.commit();
	}
	flag_updateMarksLabels = true;
	if (NS->isEndNotes())
		flag_updateEndNotes = true;
	notesFramesUpdate();
	m_docNotesStylesList.removeOne(NS);
	scMW()->emitUpdateRequest(reqMarksUpdate);
	delete NS;
}

void ScribusDoc::undoSetNotesStyle(SimpleState* ss, NotesStyle *NS)
{
	ss->set("name", NS->name());
	ss->set("start", NS->start());
	ss->set("endNotes", NS->isEndNotes());
	ss->set("numFormat", (int) NS->getType());
	ss->set("range", (int) NS->range());
	ss->set("prefix", NS->prefix());
	ss->set("suffix", NS->suffix());
	ss->set("autoH", NS->isAutoNotesHeight());
	ss->set("autoW", NS->isAutoNotesWidth());
	ss->set("autoWeld", NS->isAutoWeldNotesFrames());
	ss->set("autoRemove", NS->isAutoRemoveEmptyNotesFrames());
	ss->set("superMaster", NS->isSuperscriptInMaster());
	ss->set("superNote", NS->isSuperscriptInNote());
	ss->set("marksChStyle", NS->marksChStyle());
	ss->set("notesParStyle", NS->notesParStyle());
}

NotesStyle* ScribusDoc::getNotesStyle(QString nsName)
{
	for (int i=0; i < m_docNotesStylesList.count(); ++i)
	{
		if (m_docNotesStylesList.at(i)->name() == nsName)
			return m_docNotesStylesList.at(i);
	}
	return NULL;
}

void ScribusDoc::deleteNote(TextNote* note)
{
	if (note == NULL)
		return;
	PageItem_NoteFrame* nF = NULL;
	if (note->noteMark() != NULL)
	{
		if (note->noteMark()->getItemPtr() != NULL)
			nF = note->noteMark()->getItemPtr()->asNoteFrame();
		if (nF == NULL)
			nF = findFirstMarkItem(note->noteMark())->asNoteFrame();
	}
	PageItem* master = note->masterMark()->getItemPtr();
	if (nF != NULL)
	{
		nF->removeNote(note);
		nF->invalid = true;
		master->invalid = true;
		if (nF->notesList().isEmpty() && nF->isAutoNoteFrame())
		{
			nF->deleteIt = true;
			master->asTextFrame()->removeNoteFrame(nF);
		}
	}
	if (note->masterMark() != NULL)
		eraseMark(note->masterMark(), true, master);
	if (note->noteMark() != NULL)
		eraseMark(note->noteMark(), true, nF);
	m_docNotesList.removeOne(note);
	setNotesChanged(true);
	if (note->isEndNote())
		flag_updateEndNotes = true;
	delete note;
}

void ScribusDoc::setUndoDelNote(TextNote *note)
{
	if (UndoManager::undoEnabled())
	{
		ScItemsState* ims = new ScItemsState(Um::DeleteNote,"",Um::IDelete);
		ims->set("DELETE_NOTE");
		PageItem* master = note->masterMark()->getItemPtr();
		int pos = findMarkCPos(note->masterMark(), master);
		Q_ASSERT(pos > -1);
		Q_ASSERT(master);
		ims->insertItem("inItem", master);
		ims->set("at", pos);
		ims->set("noteTXT", note->saxedText());
		ims->set("nStyle", note->notesStyle()->name());
		if (!note->notesStyle()->isAutoRemoveEmptyNotesFrames())
			ims->insertItem("noteframe", note->noteMark()->getItemPtr());
		m_undoManager->action(this, ims);
	}
}

void ScribusDoc::updateItemNotesNums(PageItem_TextFrame* frame, NotesStyle* nStyle, int &num)
{
	//update marks strings in master text and in notes frame (only numbers!)
	//check if notes should be added or removed from notes frame
	int noteNum = num;
	int index = 0;
	bool doUpdate = false;

	for (int pos = frame->firstInFrame(); pos <= frame->lastInFrame(); ++pos)
	{
		Mark* mark = frame->itemText.mark(pos);
		if (frame->itemText.hasMark(pos) && mark->isType(MARKNoteMasterType))
		{
			if (mark->getNotePtr() == NULL)
				continue;
			if (mark->getNotePtr()->notesStyle() == nStyle)
			{
				QString numStr = nStyle->prefix() + nStyle->numString(noteNum) + nStyle->suffix();
				QString mStr = mark->getString();
				QString label = mark->label;
				if ((mStr != numStr) || flag_updateMarksLabels || flag_updateEndNotes)
				{
					doUpdate = true;
					mark->setString(numStr);
					label = "NoteMark_" + nStyle->name();
					if (nStyle->range() != NSRdocument)
					{
						if (nStyle->range() == NSRsection)
							label += " in section " + getSectionNameForPageIndex(frame->OwnPage) + " page " + QString::number(frame->OwnPage +1);
						else if (nStyle->range() == NSRpage)
							label += " on page " + QString::number(frame->OwnPage +1);
						else if (nStyle->range() == NSRstory)
							label += " in " + frame->firstInChain()->itemName();
						else if (nStyle->range() == NSRframe)
							label += " in frame " + frame->itemName();
					}
					label += "_" + QString::number(noteNum);
					mark->label = label;
				}
				TextNote* note = mark->getNotePtr();
				note->setNum(noteNum);
				note->masterMark()->setItemPtr(frame);
				if (note->noteMark() != NULL)
				{
					note->noteMark()->setString(numStr);
					label = label.replace("NoteMark","NoteFrameMark");
					note->noteMark()->label = label;
				}
				++index;
				++noteNum;
			}
		}
	}
	PageItem_NoteFrame * nF;
	if (nStyle->isEndNotes())
		nF = endNoteFrame(nStyle);
	else
		nF = frame->itemNoteFrame(nStyle);

	if (doUpdate)
	{
		frame->invalidateLayout(false);
		if ((nF != NULL) && !nF->deleteIt)
		{
			if (nStyle->isEndNotes())
				m_docEndNotesFramesChanged.append(nF);
			else
				nF->invalidateLayout(true);
		}
	}
	num = noteNum;
	if (!nStyle->isEndNotes() && (index == 0) && (nF != NULL) && nF->isAutoNoteFrame())
		nF->deleteIt = true;
}

bool ScribusDoc::updateNotesNums(NotesStyle *nStyle)
{
	bool docWasChanged = false;
	flag_restartMarksRenumbering = false;
	flag_updateEndNotes = false;
	PageItem_NoteFrame* endNF = NULL;
	if (nStyle->isEndNotes() && nStyle->range() == NSRdocument)
	{
		endNF = endNoteFrame(nStyle);
		if (endNF != NULL)
			clearNotesInFrameList(endNF);
	}
	flag_layoutNotesFrames = false;  //do not layout notes frames while counting notes
	int num, i;
	int itemsCount = Items->count();
	if ((nStyle->range() == NSRdocument) || ((nStyle->range() == NSRsection) && m_docPrefsData.docSectionMap.isEmpty()))
	{
		//FIX ME: how to change frames order on page? and what about reverse page order?
		num = nStyle->start();
		for (int page = 0; page < Pages->count(); ++page)
		{
			for (i = 0; i < itemsCount; ++i)
			{
				PageItem* currItem = Items->at(i);
				if ((currItem->OwnPage == page) && currItem->isTextFrame() && !currItem->isNoteFrame() && (currItem->itemText.length() > 0))
				{
					if (!currItem->asTextFrame()->isValidChainFromBegin())
					{
						currItem->layout();
						if (flag_restartMarksRenumbering)
						{
							//restart whole update as items was changed
							if (endNF != NULL)
								clearNotesInFrameList(endNF);
							page = -1;
							itemsCount = Items->count();
							num = nStyle->start();
							docWasChanged = true;
							flag_restartMarksRenumbering = false;
							break;
						}
					}
					if (nStyle->isEndNotes() || currItem->asTextFrame()->hasNoteFrame(nStyle, false))
						updateItemNotesNums(currItem->asTextFrame(), nStyle, num);
					if (currItem->asTextFrame()->hasNoteFrame(nStyle, false) && currItem->asTextFrame()->itemNoteFrame(nStyle)->invalid)
						docWasChanged = true;
					if (currItem->invalid)
					{
						currItem->layout();
						docWasChanged = true;
					}
				}
			}
		}
		if ((num == nStyle->start()) && nStyle->isEndNotes())
		{
			PageItem_NoteFrame* nF = endNoteFrame(nStyle);
			if (nF != NULL && nF->isAutoNoteFrame())
				nF->deleteIt = true;
		}
	}
	else if (nStyle->range() == NSRsection)
	{
		Q_ASSERT(!m_docPrefsData.docSectionMap.isEmpty());
		foreach (DocumentSection section, m_docPrefsData.docSectionMap.values())
		{
			num = nStyle->start();
			for (int page = section.fromindex; page <= (int) section.toindex ; ++page)
			{
				if (nStyle->isEndNotes())
				{
					endNF = endNoteFrame(nStyle, getSectionKeyForPageIndex(page));
					if (endNF != NULL)
						clearNotesInFrameList(endNF);
				}
				for (i = 0; i < itemsCount; ++i)
				{
					PageItem* currItem = Items->at(i);
					if (currItem == NULL)
						continue;
					if ((currItem->OwnPage == page) && currItem->isTextFrame() && !currItem->isNoteFrame() && (currItem->itemText.length() > 0))
					{
						if (!currItem->asTextFrame()->isValidChainFromBegin())
						{
							currItem->layout();
							if (flag_restartMarksRenumbering)
							{
								//restart whole update as items was changed
								if (endNF != NULL)
									clearNotesInFrameList(endNF);
								page = section.fromindex -1;
								i = -1;
								itemsCount = Items->count();
								num = nStyle->start();
								docWasChanged = true;
								flag_restartMarksRenumbering = false;
								break;
							}
						}
						if (nStyle->isEndNotes() || currItem->asTextFrame()->hasNoteFrame(nStyle, false))
							updateItemNotesNums(currItem->asTextFrame(), nStyle, num);
						if (currItem->asTextFrame()->hasNoteFrame(nStyle, false) && currItem->asTextFrame()->itemNoteFrame(nStyle)->invalid)
							docWasChanged = true;
						if (currItem->invalid)
						{
							currItem->layout();
							docWasChanged = true;
						}
					}
				}
				if ((i != -1) && (num == nStyle->start()) && nStyle->isEndNotes())
				{
					PageItem_NoteFrame* nF = endNoteFrame(nStyle);
					if (nF != NULL && nF->isAutoNoteFrame())
						nF->deleteIt = true;
				}
			}
		}
	}
	else if (nStyle->range() == NSRstory || nStyle->range() == NSRframe)
	{
		for (i = 0; i < itemsCount; ++i)
		{
			PageItem* currItem = Items->at(i);
			if (currItem == NULL)
				continue;
			if (currItem->isTextFrame() && !currItem->isNoteFrame() && (currItem->itemText.length() > 0))
			{
				if (nStyle->isEndNotes() && nStyle->range() == NSRstory)
				{
					endNF = endNoteFrame(nStyle, (void*) currItem);
					if (endNF != NULL)
						clearNotesInFrameList(endNF);
				}
				if (!currItem->asTextFrame()->isValidChainFromBegin())
				{
					currItem->layout();
					if (flag_restartMarksRenumbering)
					{
						//restart whole update as items was changed
						if (endNF != NULL)
							clearNotesInFrameList(endNF);
						i = -1;
						itemsCount = Items->count();
						docWasChanged = true;
						flag_restartMarksRenumbering = false;
						continue;
					}
				}
				if (nStyle->range() == NSRstory)
				{
					//restart numeration for all first frames of story chain
					if (currItem->prevInChain() == NULL)
					{
						num = nStyle->start();
						PageItem* nextItem = currItem;
						while (nextItem != NULL)
						{
							if (nStyle->isEndNotes() || nextItem->asTextFrame()->hasNoteFrame(nStyle, false))
								updateItemNotesNums(nextItem->asTextFrame(), nStyle, num);
							nextItem = nextItem->nextInChain();
						}
					}
				}
				else	//restart numeration for each frame
				{
					num = nStyle->start();
					if (nStyle->isEndNotes() || currItem->asTextFrame()->hasNoteFrame(nStyle, false))
						updateItemNotesNums(currItem->asTextFrame(), nStyle, num);
				}
				if (currItem->asTextFrame()->hasNoteFrame(nStyle, false) && currItem->asTextFrame()->itemNoteFrame(nStyle)->invalid)
					docWasChanged = true;
				if (currItem->invalid)
				{
					currItem->layout();
					docWasChanged = true;
				}
			}
		}
	}
	else if (nStyle->range() == NSRpage)
	{
		for (int page = 0; page < Pages->count(); ++page)
		{
			if (nStyle->isEndNotes() && nStyle->range() == NSRpage)
			{
				endNF = endNoteFrame(nStyle, (void*) DocPages.at(page));
				if (endNF != NULL)
					clearNotesInFrameList(endNF);
			}
			//restart numeration for each page
			num = nStyle->start();

			for (i = 0; i < itemsCount; ++i)
			{
				PageItem* currItem = Items->at(i);
				if (currItem == NULL)
					continue;
				if ((currItem->OwnPage == page) && currItem->isTextFrame() && !currItem->isNoteFrame() && (currItem->itemText.length() > 0))
				{
					if (!currItem->asTextFrame()->isValidChainFromBegin())
					{
						currItem->layout();
						if (flag_restartMarksRenumbering)
						{
							//restart whole update as items was changed
							if (endNF != NULL)
								clearNotesInFrameList(endNF);
							page = -1;
							i = -1;
							itemsCount = Items->count();
							num = nStyle->start();
							docWasChanged = true;
							flag_restartMarksRenumbering = false;
							break;
						}
					}
					if (nStyle->isEndNotes() || currItem->asTextFrame()->hasNoteFrame(nStyle, false))
						updateItemNotesNums(currItem->asTextFrame(), nStyle, num);
					if (currItem->asTextFrame()->hasNoteFrame(nStyle, false) && currItem->asTextFrame()->itemNoteFrame(nStyle)->invalid)
						docWasChanged = true;
					if (currItem->invalid)
					{
						currItem->layout();
						docWasChanged = true;
					}
				}
			}
			if ((i != -1) && (num == nStyle->start()) && nStyle->isEndNotes())
			{
				PageItem_NoteFrame* nF = endNoteFrame(nStyle);
				if (nF != NULL && nF->isAutoNoteFrame())
					nF->deleteIt = true;
			}
		}
	}
	flag_layoutNotesFrames = true;
	return docWasChanged;
}

bool ScribusDoc::updateEndNotesNums()
{
	bool docWasChanged = false;
	foreach (NotesStyle* ns, m_docNotesStylesList)
	{
		if (ns->isEndNotes())
		{
			if (updateNotesNums(ns))
				docWasChanged = true;
		}
	}
	return docWasChanged;
}

void ScribusDoc::updateNotesFramesStyles(NotesStyle *nStyle)
{
	for (int i=0; i<Items->count(); ++i)
	{
		PageItem* item = Items->at(i);
		if (item->isNoteFrame() && (item->asNoteFrame()->notesStyle() == nStyle) && !item->asNoteFrame()->deleteIt)
		{
			ParagraphStyle newStyle;
			if (nStyle->notesParStyle().isEmpty() || (nStyle->notesParStyle() == tr("No Style")))
			{
				if (nStyle->isEndNotes())
					//set default doc style
					newStyle.setParent(paragraphStyles()[0].name());
				else
				{
					if (item->asNoteFrame()->masterFrame() != NULL)
					{
						//set back style from master frame
						newStyle.setParent(item->asNoteFrame()->masterFrame()->itemText.defaultStyle().parent());
						newStyle.applyStyle(item->asNoteFrame()->masterFrame()->currentStyle());
					}
					else
						newStyle.setParent(item->itemText.defaultStyle().parent());
				}
			}
			else
				newStyle.setParent(nStyle->notesParStyle());
			item->itemText.setDefaultStyle(newStyle);
			item->asTextFrame()->invalidateLayout(true);
		}
	}
}

void ScribusDoc::updateItemNotesFramesStyles(PageItem* item, const ParagraphStyle& newStyle)
{
	if (item->isTextFrame() && !item->isNoteFrame())
	{
		item = item->firstInChain();

		while (item != NULL)
		{
			foreach (PageItem_NoteFrame* nF, item->asTextFrame()->notesFramesList())
			{
				NotesStyle* nSet = nF->notesStyle();
				if (nSet->isEndNotes())
					continue;
				if (nSet->notesParStyle().isEmpty() || (nSet->notesParStyle() == tr("No Style")))
				{
					nF->itemText.setDefaultStyle(newStyle);
					//nF->itemText.applyCharStyle(0, nF->itemText.length(), newStyle.charStyle());
				}
				setNotesChanged(true);
			}
			item = item->nextInChain();
		}
	}
}

QList<PageItem_NoteFrame *> ScribusDoc::listNotesFrames(NotesStyle *NS)
{
	QList<PageItem_NoteFrame *> list;
	foreach(PageItem* item, DocItems)
	{
		if (item->isNoteFrame() && item->asNoteFrame()->notesStyle() == NS)
			list.append(item->asNoteFrame());
	}
	return list;
}

const ScPage *ScribusDoc::page4EndNotes(NotesStyle *NS, PageItem* item)
{
	ScPage* scP = NULL;
	if ((NS->range() == NSRdocument) || ((NS->range() == NSRsection) && m_docPrefsData.docSectionMap.isEmpty()))
		scP = DocPages.last();
	else if (item != NULL)
	{
		if (NS->range() == NSRpage)
			scP = DocPages.at(item->OwnPage);
		else if (NS->range() == NSRsection)
		{
			int section = getSectionKeyForPageIndex(item->OwnPage);
			DocumentSection sec = sections().find(section).value();
			scP = DocPages.at(sec.toindex);
		}
		else if (NS->range() == NSRstory)
			scP = DocPages.at(item->lastInChain()->OwnPage);
	}

	return scP;
}

bool ScribusDoc::notesFramesUpdate()
{
	bool removeEmptyNF = false;
	bool docWasChanged = false;
	int end = 0;
	do {
		setNotesChanged(false);
		end = Items->count();
		for (int i = 0; i < end; ++i)
		{
			PageItem* item = Items->at(i);
			if (!item->isTextFrame())
				continue;
			if (item->isNoteFrame())
			{
				if (item->asNoteFrame()->notesList().isEmpty())
				{
					if (item->isAutoNoteFrame())
						item->asNoteFrame()->deleteIt = true;
				}
				else
				{
					if (item->asNoteFrame()->isEndNotesFrame())
						updateEndNotesFrameContent(item->asNoteFrame());
					else
					{
						if (item->itemText.length() == 0 && !item->asNoteFrame()->notesList().isEmpty())
							item->asNoteFrame()->updateNotes(item->asNoteFrame()->notesList(), true);
						item->invalid = true;
						item->layout();
					}
				}
				if (item->asNoteFrame()->deleteIt)
					removeEmptyNF = true;
			}
			if (end != Items->count())
			{
				end = Items->count();
				setNotesChanged(true);
			}
			docWasChanged = docWasChanged || notesChanged();
			if (notesChanged())
				break;
		}
	} while (notesChanged());

	if (removeEmptyNF)
	{
		end = Items->count();
		QList<PageItem*> tmplist;
		for (int i = 0; i < end; ++i)
		{
			PageItem* item = Items->at(i);
			if (item->isAutoNoteFrame() && item->asNoteFrame()->deleteIt)
				tmplist.append(item);
		}
		foreach (PageItem* item, tmplist)
		{
			if (item->asNoteFrame()->masterFrame())
				item->asNoteFrame()->masterFrame()->invalid = true;
			delNoteFrame(item->asNoteFrame());
		}
		docWasChanged = true;
	}
	return docWasChanged;
}

void ScribusDoc::updateNotesFramesSettings(NotesStyle *NS)
{
	foreach (PageItem_NoteFrame* nF, listNotesFrames(NS))
	{
		if (nF->isWelded() && !NS->isAutoWeldNotesFrames())
			nF->unWeld();
		if (!nF->isWelded() && NS->isAutoWeldNotesFrames())
		{
			nF->setYPos(nF->masterFrame()->yPos() + nF->masterFrame()->height());
			nF->setXPos(nF->masterFrame()->xPos());
			nF->addWelded(nF->masterFrame());
			nF->masterFrame()->addWelded(nF);
			nF->masterFrame()->setWeldPoint(0, nF->masterFrame()->height(), nF);
			nF->setWeldPoint(0,0, nF->masterFrame());
			nF->invalidateLayout(false);
		}
		nF->setSizeVLocked(NS->isAutoNotesHeight());
		nF->setSizeHLocked(NS->isAutoNotesWidth());
		nF->setSizeLocked(NS->isAutoNotesHeight() && NS->isAutoNotesWidth());
		if (NS->isAutoNotesHeight() || NS->isAutoNotesWidth())
			nF->invalidateLayout(false);
	}
}

void ScribusDoc::updateEndnotesFrames(NotesStyle* nStyle, bool invalidate)
{
	if (m_docEndNotesFramesMap.isEmpty())
		return;
	if (nStyle == NULL)
	{
		int NScount = m_docNotesStylesList.count();
		for (int i=0; i < NScount; ++i)
			updateEndnotesFrames(m_docNotesStylesList.at(i));
	}
	else if (nStyle->isEndNotes())
	{
		foreach (PageItem_NoteFrame* nF, listNotesFrames(nStyle))
			updateEndNotesFrameContent(nF);
	}
	flag_updateEndNotes = false;
}

//for sorting purpose
bool ascendingSort(TextNote* nt1, TextNote* nt2) { return nt1->num() < nt2->num(); }

void ScribusDoc::updateEndNotesFrameContent(PageItem_NoteFrame *nF, bool invalidate)
{
	QList<TextNote*> nList;
	foreach (PageItem* item, DocItems)
	{
		if (item->isTextFrame() && !item->isNoteFrame())
		{
			QList<TextNote*> nL = item->asTextFrame()->notesList(nF);
			if (!nL.isEmpty())
				nList.append(nL);
			if (invalidate)
				item->invalid = true;
		}
	}

	if (nList.isEmpty())
	{
		if (nF->isAutoNoteFrame())
		{
			nF->deleteIt = true;
			m_docNotesInFrameMap.remove(nF);
			delNoteFrame(nF);
		}
	}
	else
	{
		NotesStyle* currNS = nF->notesStyle();
		if (currNS->isAutoNotesHeight() || currNS->isAutoNotesWidth())
			nF->invalidateLayout(false);
		qSort(nList.begin(), nList.end(), ascendingSort);
		if (nList == m_docNotesInFrameMap.value(nF))
			return;

		nF->updateNotes(nList);
		nF->invalid = true;
		nF->layout();
		//layout all endnotes frames with same range
		foreach (NotesStyle* NS, m_docNotesStylesList)
		{
			if ((NS != currNS) && (NS->isEndNotes() && NS->range() == currNS->range()))
				foreach (PageItem_NoteFrame* NF, listNotesFrames(NS))
					NF->layout();
		}
		m_docNotesInFrameMap.insert(nF, nList);
	}
}

void ScribusDoc::updateChangedEndNotesFrames()
{
	while (!m_docEndNotesFramesChanged.isEmpty())
	{
		PageItem_NoteFrame* nF = m_docEndNotesFramesChanged.first();
		m_docEndNotesFramesChanged.removeAll(nF);
		updateEndNotesFrameContent(nF);
		if (nF->deleteIt && nF->isAutoNoteFrame())
			delNoteFrame(nF);
	}
}

PageItem_NoteFrame *ScribusDoc::createNoteFrame(PageItem_TextFrame *inFrame, NotesStyle *nStyle, int index)
{
	PageItem_NoteFrame* nF = new PageItem_NoteFrame(inFrame, nStyle);
	if (nStyle->isEndNotes())
		m_docEndNotesFramesMap.insert(nF, rangeItem());
	m_docNotesInFrameMap.insert(nF, QList<TextNote*>());
	if (index > -1)
		DocItems.insert(index, nF);
	else
		DocItems.append(nF);
	return nF;
}

PageItem_NoteFrame *ScribusDoc::createNoteFrame(NotesStyle *nStyle, double x, double y, double w, double h, double w2, QString fill, QString outline)
{
	PageItem_NoteFrame* nF = new PageItem_NoteFrame(nStyle, this, x, y, w, h, w2, fill, outline);
	if (nStyle->isEndNotes())
		m_docEndNotesFramesMap.insert(nF, rangeItem());
	m_docNotesInFrameMap.insert(nF, QList<TextNote*>());
	DocItems.append(nF);
	return nF;
}

void ScribusDoc::delNoteFrame(PageItem_NoteFrame* nF, bool removeMarks, bool forceDeletion)
{
	Q_ASSERT(nF != NULL);

	//for all notes in noteFrame set notes marks to null
	foreach(TextNote* n, nF->notesList())
		n->setNoteMark(NULL);

	if (nF->itemText.length() > 0 && removeMarks)
		nF->removeMarksFromText(false);
		
	if (appMode == modeEdit && nF->isSelected())
	{
		view()->Deselect(true);
		if (!nF->isEndNotesFrame())
			view()->SelectItem(nF->masterFrame());
	}
	if (m_docEndNotesFramesMap.contains(nF))
	{
		m_docEndNotesFramesMap.remove(nF);
		m_docEndNotesFramesChanged.removeAll(nF);
		foreach (TextNote* note, nF->notesList())
		{
			note->masterMark()->getItemPtr()->asTextFrame()->removeNoteFrame(nF);
			note->masterMark()->getItemPtr()->invalid = true;
		}
	}
	else if (nF->masterFrame() != NULL)
	{
		nF->masterFrame()->removeNoteFrame(nF);
		nF->masterFrame()->invalid = true;
	}
	m_docNotesInFrameMap.remove(nF);

	nF->dropLinks();
	if (nF->isWelded())
		nF->unWeld(!nF->isAutoNoteFrame());
	//delete marks pointed to that item
	for (int a=0; a < m_docMarksList.count(); ++a)
	{
		Mark* m = m_docMarksList.at(a);
		Q_ASSERT(m != NULL);
		if (m->isType(MARK2ItemType) && (m->getItemPtr() == nF))
		{
			setUndoDelMark(m);
			eraseMark(m, true);
		}
	}
	m_Selection->delaySignalsOn();
	if (m_Selection->findItem(nF)!=-1)
	{
		if (appMode == modeEdit)
			view()->requestMode(modeNormal);
		m_Selection->removeItem(nF);
		if (m_Selection->isEmpty() && nF->masterFrame())
			m_Selection->addItem(nF->masterFrame());
	}
	m_Selection->delaySignalsOff();

	Items->removeOne(nF);

	QList<PageItem*> allItems = *Items;
	while (allItems.count() > 0)
	{
		PageItem* item = allItems.takeFirst();
		if (item->isGroup() || item->isTable())
		{
			allItems = item->getChildren() + allItems;
			continue;
		}
		if (item->isTextFrame())
			item->asTextFrame()->removeNoteFrame(nF);
	}
	setNotesChanged(true);
	if (forceDeletion)
		delete nF;
}

bool ScribusDoc::validateNSet(NotesStyle NS, QString newName)
{
	//check if chosen numbering type is available with chosen range, prefix and suffix
	QString errStr;
	foreach (NotesStyle* NS2, m_docNotesStylesList)
	{
		if (newName.isEmpty())
			//hack for validate nset while its name will change
			newName = NS.name();
		if (newName == NS2->name())
			continue;
		if (NS.range() == NS2->range())
		{
			if ((NS.getType() == NS2->getType()) && (NS.prefix() == NS2->prefix()) && (NS.suffix() == NS2->suffix()))
				errStr.append(tr("%1 note style has document as range and provide same numbering style as set %2").arg(NS.name()).arg(NS2->name()) + "\n");
		}
	}

	if (!errStr.isEmpty() && ScCore->usingGUI())
	{
		ScMessageBox::warning(this->scMW(), QObject::tr("Unacceptable settings for note style"), "<qt>"+ errStr +"</qt>");
		return false;
	}
	return true;
}

void ScribusDoc::invalidateNoteFrames(NotesStyle *nStyle)
{
	foreach (PageItem_NoteFrame* nF, listNotesFrames(nStyle))
		nF->invalid = true;
}

void ScribusDoc::invalidateMasterFrames(NotesStyle *nStyle)
{
	QList<PageItem*> toInvalidate;
	foreach (TextNote* note, m_docNotesList)
	{
		if (note->notesStyle() == nStyle)
			toInvalidate.append(note->masterMark()->getItemPtr());
	}
	while (!toInvalidate.isEmpty())
		toInvalidate.takeFirst()->invalid = true;
}

PageItem_NoteFrame *ScribusDoc::endNoteFrame(NotesStyle *nStyle, PageItem_TextFrame *master)
{
	if (nStyle->range() == NSRdocument)
		return endNoteFrame(nStyle);
	else if (nStyle->range() == NSRsection)
		return endNoteFrame(nStyle, getSectionKeyForPageIndex(master->OwnPage));
	else if (nStyle->range() == NSRpage)
		return endNoteFrame(nStyle, (void*) DocPages.at(master->OwnPage));
	else if (nStyle->range() == NSRstory)
		return endNoteFrame(nStyle, master->firstInChain());
	return NULL;
}

PageItem_NoteFrame* ScribusDoc::endNoteFrame(NotesStyle *nStyle, int sectIndex)
{
	if (nStyle->range() != NSRsection)
		return NULL;
	if (m_docEndNotesFramesMap.isEmpty())
		return NULL;

	QMap<PageItem_NoteFrame*, rangeItem>::Iterator it = m_docEndNotesFramesMap.begin();
	QMap<PageItem_NoteFrame*, rangeItem>::Iterator end = m_docEndNotesFramesMap.end();
	while (it != end)
	{
		PageItem_NoteFrame* nF = it.key();
		if (nF->notesStyle() == nStyle)
		{
			rangeItem rItem = it.value();
			if (rItem.sectionIndex == sectIndex)
				return nF;
		}
		++it;
	}
	return NULL;
}

PageItem_NoteFrame* ScribusDoc::endNoteFrame(NotesStyle *nStyle, void* item)
{
	if (m_docEndNotesFramesMap.isEmpty())
		return NULL;
	if ((nStyle->range() != NSRdocument) && (item == NULL))
		return NULL;

	QMap<PageItem_NoteFrame*, rangeItem>::Iterator it = m_docEndNotesFramesMap.begin();
	QMap<PageItem_NoteFrame*, rangeItem>::Iterator end = m_docEndNotesFramesMap.end();

	while (it != end)
	{
		PageItem_NoteFrame* nF = it.key();
		rangeItem rItem = it.value();

		if (nF->notesStyle() == nStyle)
		{
			if ((nStyle->range() == NSRdocument) || (item == rItem.P))
				return nF;
		}
		++it;
	}
	return NULL;
}

/* Functions for PDF Form Actions */

void ScribusDoc::SubmitForm()
{
}

void ScribusDoc::ImportData()
{
}

void ScribusDoc::ResetFormFields()
{
	QList<PageItem*> allItems;
	PageItem* it = NULL;
	uint counter = 0;
	for (uint lc = 0; lc < 2; ++lc)
	{
		switch (lc)
		{
			case 0:
				counter = MasterItems.count();
				break;
			case 1:
				counter = DocItems.count();
				break;
		}
		for (uint d = 0; d < counter; ++d)
		{
			switch (lc)
			{
				case 0:
					it = MasterItems.at(d);
					break;
				case 1:
					it = DocItems.at(d);
					break;
			}
			if (it->isGroup())
				allItems = it->getAllChildren();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				if (it->isAnnotation())
				{
					if ((it->annotation().Type() == Annotation::RadioButton) || (it->annotation().Type() == Annotation::Checkbox))
						it->annotation().setCheckState(it->annotation().IsChk());
					it->annotation().setOnState(false);
					it->annotation().setOpen(false);
					if (it->annotation().Type() == Annotation::Text)
						it->asTextFrame()->setTextAnnotationOpen(false);
					it->update();
				}
			}
			allItems.clear();
		}
	}
	for (QHash<int, PageItem*>::iterator itf = FrameItems.begin(); itf != FrameItems.end(); ++itf)
	{
		it = itf.value();
		if (it->isGroup())
			allItems = it->getAllChildren();
		else
			allItems.append(it);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			it = allItems.at(ii);
			if (it->isAnnotation())
			{
				if ((it->annotation().Type() == Annotation::RadioButton) || (it->annotation().Type() == Annotation::Checkbox))
					it->annotation().setCheckState(it->annotation().IsChk());
				it->annotation().setOnState(false);
				it->annotation().setOpen(false);
				if (it->annotation().Type() == Annotation::Text)
					it->asTextFrame()->setTextAnnotationOpen(false);
				it->update();
			}
		}
		allItems.clear();
	}
	QStringList patterns = getUsedPatterns();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			it = pa.items.at(o);
			if (it->isGroup())
				allItems = it->getAllChildren();
			else
				allItems.append(it);
			for (int ii = 0; ii < allItems.count(); ii++)
			{
				it = allItems.at(ii);
				if (it->isAnnotation())
				{
					if ((it->annotation().Type() == Annotation::RadioButton) || (it->annotation().Type() == Annotation::Checkbox))
						it->annotation().setCheckState(it->annotation().IsChk());
					it->annotation().setOnState(false);
					it->annotation().setOpen(false);
					if (it->annotation().Type() == Annotation::Text)
						it->asTextFrame()->setTextAnnotationOpen(false);
					it->update();
				}
			}
			allItems.clear();
		}
	}
	changed();
	regionsChanged()->update(QRect());
}
