/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scribusview.cpp  -  description
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

#include "scribusview.h"

#include "scconfig.h"

#include <QColor>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QImageReader>
#include <QLabel>
#include <QList>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QPolygon>
#include <QStack>
#include <QStringList>
#include <QWheelEvent>
#include <QWidgetAction>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <QUrl>
#include <QDir>
#include <QEvent>
#include <QSizeGrip>
#if QT_VERSION  > 0x030102
	#define SPLITVC Qt::SplitHCursor
	#define SPLITHC Qt::SplitVCursor
#else
	#define SPLITVC Qt::SplitVCursor
	#define SPLITHC Qt::SplitHCursor
#endif
#include "scribus.h"

#include "canvas.h"
#include "canvasgesture.h"
#include "canvasmode.h"
#include "canvasmode_objimport.h"
#include "actionmanager.h"
#include "adjustcmsdialog.h"
#include "commonstrings.h"
#include "extimageprops.h"
#include "filewatcher.h"
#include "guidemanager.h"
#include "hruler.h"
#include "hyphenator.h"
#include "insertTable.h"
#include "oneclick.h"
#include "page.h"
#include "pageitem_imageframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_textframe.h"
#include "pageitem_latexframe.h"
#include "pageitemattributes.h"
#include "pageselector.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "rulermover.h"
#include "scmessagebox.h"
#include "scmimedata.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scrapbookpalette.h"
#include "scribusXml.h"
#include "selection.h"
#include "serializer.h"
#include "stencilreader.h"
#include "storyeditor.h"
#include "text/nlsconfig.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"
#include "vruler.h"
#include "loadsaveplugin.h"
#include "fileloader.h"
#include "plugins/formatidlist.h"
#include <tiffio.h>
#include CMS_INC


using namespace std;



ScribusView::ScribusView(QWidget* win, ScribusMainWindow* mw, ScribusDoc *doc) :
	QScrollArea(win),
	Doc(doc),
	m_canvas(new Canvas(doc, this)),
	Prefs(&(PrefsManager::instance()->appPrefs)),
	undoManager(UndoManager::instance()),
	m_ScMW(mw),
	OldScale(0),
	dragX(0), dragY(0), dragW(0), dragH(0),
	oldW(-1), // oldCp(-1),
//	Mxp(-1), Myp(-1), Dxp(-1), Dyp(-1),
//	frameResizeHandle(-1),
//	SeRx(-1), SeRy(-1), GyM(-1), GxM(-1),
//	ClRe(-1), ClRe2(-1),
//	SegP1(-1), SegP2(-1),
	RotMode(0),
	DrHY(-1), DrVX(-1),
//	EdPoints(true),
//	m_MouseButtonPressed(false),
//	operItemMoving(false),
//	MoveGY(false), MoveGX(false),
	HaveSelRect(false),
//	operItemResizing(false),
//	EditContour(false),
	//GroupSel(false),
	DraggedGroup(false),
	DraggedGroupFirst(false),
//	operItemResizeInEditMode(false),
	MidButt(false),
	updateOn(true),
	FirstPoly(true),
	Magnify(false),
//	MoveSym(false),
//	previewMode(false),
	RCenter(-1,-1),
//	RecordP(),
	Ready(false),
	oldX(0), oldY(0),
	m_groupTransactions(0),
	m_groupTransaction(NULL),
	_isGlobalMode(true),
//	evSpon(false),
//	forceRedraw(false),
//	Scale(Prefs->DisScale),
//	oldClip(0),
	m_vhRulerHW(17)
{
	setObjectName("s");
	setAttribute(Qt::WA_StaticContents);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setViewportMargins(m_vhRulerHW, m_vhRulerHW, 0, 0);
	setWidgetResizable(false);
	m_canvasMode = CanvasMode::createForAppMode(this, Doc->appMode);
	setWidget(m_canvas);
	//already done by QScrollArea: widget()->installEventFilter(this);
	installEventFilter(this); // FIXME:av
//	viewport()->setBackgroundMode(Qt::PaletteBackground);
	setFocusPolicy(Qt::ClickFocus);
	QFont fo = QFont(font());
	int posi = fo.pointSize()-2;
	fo.setPointSize(posi);
	unitSwitcher = new QComboBox( this );
	unitSwitcher->setFocusPolicy(Qt::NoFocus);
	unitSwitcher->setFont(fo);
	int maxUindex = unitGetMaxIndex() - 2;
	for (int i = 0; i <= maxUindex; ++i)
		unitSwitcher->addItem(unitGetStrFromIndex(i));
	previewQualitySwitcher = new QComboBox( this );
	previewQualitySwitcher->setFocusPolicy(Qt::NoFocus);
	previewQualitySwitcher->setFont(fo);
	previewQualitySwitcher->addItem(tr("High"));
	previewQualitySwitcher->addItem(tr("Normal"));
	previewQualitySwitcher->addItem(tr("Low"));
// 	setCurrentComboItem(previewQualitySwitcher, tr("Normal"));
	previewQualitySwitcher->setCurrentIndex(Prefs->toolSettings.lowResType);

	zoomSpinBox = new ScrSpinBox( 10, 3200, this, 6 );
	zoomSpinBox->setTabAdvance(false);
	zoomSpinBox->setFont(fo);
	zoomSpinBox->setValue( 100 );
	zoomSpinBox->setSingleStep(10);
	zoomSpinBox->setFocusPolicy(Qt::ClickFocus);
	zoomSpinBox->setSuffix( tr( " %" ) );
#if OPTION_USE_QTOOLBUTTON
	zoomOutToolbarButton = new QToolButton(this);
	zoomDefaultToolbarButton = new QToolButton(this);
	zoomInToolbarButton = new QToolButton(this);
	cmsToolbarButton = new QToolButton(this);
	previewToolbarButton = new QToolButton(this);
	zoomDefaultToolbarButton->setAutoRaise(OPTION_FLAT_BUTTON);
	zoomOutToolbarButton->setAutoRaise(OPTION_FLAT_BUTTON);
	zoomInToolbarButton->setAutoRaise(OPTION_FLAT_BUTTON);
	cmsToolbarButton->setAutoRaise(OPTION_FLAT_BUTTON);
	cmsToolbarButton->setCheckable(true);
	QIcon ic2;
	ic2.addPixmap(loadIcon("cmsOff.png"), QIcon::Normal, QIcon::Off);
	ic2.addPixmap(loadIcon("cmsOn.png"), QIcon::Normal, QIcon::On);
	cmsToolbarButton->setIcon(ic2);
	previewToolbarButton->setAutoRaise(OPTION_FLAT_BUTTON);
	previewToolbarButton->setCheckable(true);
	QIcon ic;
	ic.addPixmap(loadIcon("previewOff.png"), QIcon::Normal, QIcon::Off);
	ic.addPixmap(loadIcon("previewOn.png"), QIcon::Normal, QIcon::On);
	previewToolbarButton->setIcon(ic);
#else
	zoomDefaultToolbarButton = new QPushButton(this);
	zoomDefaultToolbarButton->setFocusPolicy(Qt::NoFocus);
	zoomDefaultToolbarButton->setDefault( false );
	zoomDefaultToolbarButton->setAutoDefault( false );
	zoomDefaultToolbarButton->setFlat(OPTION_FLAT_BUTTON);
	zoomOutToolbarButton = new QPushButton(this);
	zoomOutToolbarButton->setFocusPolicy(Qt::NoFocus);
	zoomOutToolbarButton->setDefault( false );
	zoomOutToolbarButton->setAutoDefault( false );
	zoomOutToolbarButton->setFlat(OPTION_FLAT_BUTTON);
	zoomInToolbarButton = new QPushButton(this);
	zoomInToolbarButton->setFocusPolicy(Qt::NoFocus);
	zoomInToolbarButton->setDefault( false );
	zoomInToolbarButton->setAutoDefault( false );
	zoomInToolbarButton->setFlat(OPTION_FLAT_BUTTON);
	previewToolbarButton = new QPushButton(this);
	cmsToolbarButton->setFocusPolicy(Qt::NoFocus);
	cmsToolbarButton->setDefault( false );
	cmsToolbarButton->setAutoDefault( false );
	cmsToolbarButton->setFlat(OPTION_FLAT_BUTTON);
	cmsToolbarButton->setPixmap(loadIcon("cmsOn.png"));
	previewToolbarButton = new QPushButton(this);
	previewToolbarButton->setFocusPolicy(Qt::NoFocus);
	previewToolbarButton->setDefault( false );
	previewToolbarButton->setAutoDefault( false );
	previewToolbarButton->setFlat(OPTION_FLAT_BUTTON);
	previewToolbarButton->setPixmap(loadIcon("previewOn.png"));
#endif
	cmsAdjustMenu = new QMenu();
	idCmsAdjustMenu = cmsAdjustMenu->addAction( "Configure CMS...", this, SLOT(adjustCMS()));
	cmsToolbarButton->setMenu(cmsAdjustMenu);
#if OPTION_USE_QTOOLBUTTON
	cmsToolbarButton->setPopupMode(QToolButton::DelayedPopup);
#endif
	//zoomDefaultToolbarButton->setText("1:1");
	zoomDefaultToolbarButton->setIcon(QIcon(loadIcon("16/zoom-original.png")));
	zoomOutToolbarButton->setIcon(QIcon(loadIcon("16/zoom-out.png")));
	zoomInToolbarButton->setIcon(QIcon(loadIcon("16/zoom-in.png")));
	pageSelector = new PageSelector(this, Doc->Pages->count());
	pageSelector->setFont(fo);
	pageSelector->setFocusPolicy(Qt::ClickFocus);
	layerMenu = new QComboBox( this );
	layerMenu->setEditable(false);
	layerMenu->setFont(fo);
	layerMenu->setFocusPolicy(Qt::NoFocus);
	visualMenu = new QComboBox( this );
	visualMenu->setFocusPolicy(Qt::NoFocus);
	visualMenu->setFont(fo);
	visualMenu->setEnabled(false);
	horizRuler = new Hruler(this, Doc);
	vertRuler = new Vruler(this, Doc);
	horizRuler->installEventFilter(this);
	vertRuler->installEventFilter(this);
	rulerMover = new RulerMover(this);
	rulerMover->setFocusPolicy(Qt::NoFocus);
	horizRuler->setGeometry(m_vhRulerHW, 1, width()-m_vhRulerHW-1, m_vhRulerHW);
	vertRuler->setGeometry(1, m_vhRulerHW, m_vhRulerHW, height()-m_vhRulerHW-1);
	rulerMover->setGeometry(1, 1, m_vhRulerHW, m_vhRulerHW);
	Ready = true;
	m_canvas->setMouseTracking(true);
	setAcceptDrops(true);
	m_canvas->setAcceptDrops(true);
	// FIXME setDragAutoScroll(false);
	Doc->DragP = false;
	Doc->leaveDrag = false;
	Doc->SubMode = -1;
	storedFramesShown = Doc->guidesSettings.framesShown;
	storedShowControls = Doc->guidesSettings.showControls;
	m_canvas->m_viewMode.viewAsPreview = false;
	m_canvas->setPreviewVisual(-1);
//	shiftSelItems = false;
//	inItemCreation = false;
	m_previousMode = -1;
	redrawMode = 0;
	redrawCount = 0;
	redrawMarker = new QRubberBand(QRubberBand::Rectangle);
	redrawMarker->hide();
	m_canvas->newRedrawPolygon();
	m_canvas->resetRenderMode();
	m_ScMW->scrActions["viewFitPreview"]->setChecked(m_canvas->m_viewMode.viewAsPreview);
//	m_SnapCounter = 0;

	Doc->regionsChanged()->connectObserver(this);
	connect(zoomOutToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoomOut()));
	connect(zoomInToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoomIn()));
	connect(zoomDefaultToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoom100()));
	connect(zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom()));
	connect(pageSelector, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
	connect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	connect(unitSwitcher, SIGNAL(activated(int)), this, SLOT(ChgUnit(int)));
	connect(previewQualitySwitcher, SIGNAL(activated(int)), this, SLOT(changePreviewQuality(int)));
	connect(previewToolbarButton, SIGNAL(clicked()), this, SLOT(togglePreview()));
	connect(cmsToolbarButton, SIGNAL(clicked()), this, SLOT(toggleCMS()));
	connect(visualMenu, SIGNAL(activated(int)), this, SLOT(switchPreviewVisual(int)));
	connect(this, SIGNAL(HaveSel(int)), this, SLOT(selectionChanged()));
	languageChange();
	m_dragTimer = new QTimer(this);
	connect(m_dragTimer, SIGNAL(timeout()), this, SLOT(dragTimerTimeOut()));
	m_dragTimer->stop();
	m_dragTimerFired = false;
}

ScribusView::~ScribusView()
{
	while (m_canvasMode)
	{
		m_canvasMode->deactivate(false);
		m_canvasMode = m_canvasMode->delegate();
	}
}

void ScribusView::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void ScribusView::languageChange()
{
	zoomDefaultToolbarButton->setToolTip( tr("Zoom to 100%"));
	zoomOutToolbarButton->setToolTip( tr("Zoom out by the stepping value in Tools preferences"));
	zoomInToolbarButton->setToolTip( tr("Zoom in by the stepping value in Tools preferences"));
	zoomSpinBox->setToolTip( tr("Current zoom level"));
	cmsToolbarButton->setToolTip("");
	previewToolbarButton->setToolTip("");
	layerMenu->setToolTip( tr("Select the current layer"));
	unitSwitcher->setToolTip( tr("Select the current unit"));
	previewQualitySwitcher->setToolTip( tr("Select the image preview quality"));
	visualMenu->setToolTip("");
	cmsToolbarButton->setToolTip( tr("Enable/disable Color Management"));
	idCmsAdjustMenu->setText( tr("Configure CMS..."));
	previewToolbarButton->setToolTip( tr("Enable/disable the Preview Mode"));
	visualMenu->setToolTip( tr("Select the visual appearance of the display. You can choose between normal and several color blindness forms"));
	disconnect(visualMenu, SIGNAL(activated(int)), this, SLOT(switchPreviewVisual(int)));
	visualMenu->clear();
	visualMenu->addItem(CommonStrings::trVisionNormal);
	visualMenu->addItem(CommonStrings::trVisionProtanopia);
	visualMenu->addItem(CommonStrings::trVisionDeuteranopia);
	visualMenu->addItem(CommonStrings::trVisionTritanopia);
	visualMenu->addItem(CommonStrings::trVisionFullColorBlind);
	visualMenu->setCurrentIndex(m_canvas->previewVisual());
	connect(visualMenu, SIGNAL(activated(int)), this, SLOT(switchPreviewVisual(int)));
}

void ScribusView::toggleCMS()
{
	Doc->enableCMS(!Doc->HasCMS);
	m_ScMW->propertiesPalette->ShowCMS();
	DrawNew();
}

void ScribusView::adjustCMS()
{
	AdjustCmsDialog* dia = new AdjustCmsDialog(this, Doc);
	if (dia->exec())
	{
		dia->tabColorManagement->updateDocSettings(Doc);
		if (dia->tabColorManagement->changed)
		{
			Doc->enableCMS(Doc->CMSSettings.CMSinUse);
			cmsToolbarButton->setChecked(Doc->HasCMS);
			m_ScMW->propertiesPalette->ShowCMS();
			DrawNew();
		}
	}
	delete dia;
}

void ScribusView::switchPreviewVisual(int vis)
{
	m_canvas->setPreviewVisual(vis);
	Doc->recalculateColors();
	Doc->recalcPicturesRes();
	DrawNew();
}

void ScribusView::togglePreview()
{
	m_canvas->m_viewMode.viewAsPreview = !m_canvas->m_viewMode.viewAsPreview;
	if (m_canvas->m_viewMode.viewAsPreview)
	{
		storedFramesShown = Doc->guidesSettings.framesShown;
		Doc->guidesSettings.framesShown = false;
		storedShowControls = Doc->guidesSettings.showControls;
		Doc->guidesSettings.showControls = false;
		// warning popping up in case colour management and out-of-gamut-display are active
		// as from #4346: Add a preview for daltonian - PV
		if (Doc->HasCMS && Doc->Gamut)
			QMessageBox::information(m_ScMW, tr("Preview Mode"),
						"<qt>" + tr("CMS is active. Therefore the color display may not match the perception by visually impaired") + "</qt>",
						QMessageBox::Ok);
	}
	else
	{
		Doc->guidesSettings.framesShown = storedFramesShown;
		Doc->guidesSettings.showControls = storedShowControls;
	}
	m_ScMW->scrActions["viewFitPreview"]->setChecked(m_canvas->m_viewMode.viewAsPreview);
	m_ScMW->scrActions["viewShowMargins"]->setEnabled(!m_canvas->m_viewMode.viewAsPreview);
	m_ScMW->scrActions["viewShowFrames"]->setEnabled(!m_canvas->m_viewMode.viewAsPreview);
	m_ScMW->scrActions["viewShowLayerMarkers"]->setEnabled(!m_canvas->m_viewMode.viewAsPreview);
	m_ScMW->scrActions["viewShowGrid"]->setEnabled(!m_canvas->m_viewMode.viewAsPreview);
	m_ScMW->scrActions["viewShowGuides"]->setEnabled(!m_canvas->m_viewMode.viewAsPreview);
	m_ScMW->scrActions["viewShowColumnBorders"]->setEnabled(!m_canvas->m_viewMode.viewAsPreview);
	m_ScMW->scrActions["viewShowBaseline"]->setEnabled(!m_canvas->m_viewMode.viewAsPreview);
	m_ScMW->scrActions["viewShowTextChain"]->setEnabled(!m_canvas->m_viewMode.viewAsPreview);
	m_ScMW->scrActions["viewShowTextControls"]->setEnabled(!m_canvas->m_viewMode.viewAsPreview);
#if OPTION_USE_QTOOLBUTTON
	previewToolbarButton->setChecked(m_canvas->m_viewMode.viewAsPreview);
#endif
	visualMenu->setEnabled(m_canvas->m_viewMode.viewAsPreview);
	Doc->recalculateColors();
	Doc->recalcPicturesRes();
//	repaintContents(QRect());
	DrawNew();
}

void ScribusView::changed(QRectF re)
{
	double scale = m_canvas->scale();
	int newCanvasWidth = qRound((Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()) * scale);
	int newCanvasHeight = qRound((Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()) * scale);
	if (!re.isValid() && // dont check this all the time
		( m_oldCanvasWidth != newCanvasWidth || m_oldCanvasHeight != newCanvasHeight))
	{
		QSize maxViewport = maximumViewportSize();
		horizontalScrollBar()->setRange(qRound(Doc->minCanvasCoordinate.x() * scale),
										qRound(Doc->maxCanvasCoordinate.x() * scale) - maxViewport.width());
		verticalScrollBar()->setRange(qRound(Doc->minCanvasCoordinate.y() * scale),
									  qRound(Doc->maxCanvasCoordinate.y() * scale) - maxViewport.height());
		/*	qDebug() << "adjustCanvas [" << m_oldCanvasWidth << m_oldCanvasHeight << " ] -> [" << newCanvasWidth << newCanvasHeight 
				<< "] (" << Doc->minCanvasCoordinate.x() << Doc->minCanvasCoordinate.y() << ") - ("
				<< Doc->maxCanvasCoordinate.x() << Doc->maxCanvasCoordinate.y() << ") @" << scale << maxViewport;
		*/
		widget()->resize(newCanvasWidth, newCanvasHeight);	
		m_oldCanvasWidth = newCanvasWidth;
		m_oldCanvasHeight = newCanvasHeight;
	}
	if (!Doc->isLoading() && !m_ScMW->ScriptRunning)
	{
// 		qDebug() << "ScribusView-changed(): changed region:" << re;
		m_canvas->m_viewMode.forceRedraw = true;
		updateCanvas(re);
	}
}

bool ScribusView::handleObjectImport(QMimeData* mimeData)
{
	requestMode(modeImportObject);
	CanvasMode_ObjImport* objImport = dynamic_cast<CanvasMode_ObjImport*>(m_canvasMode);
	if (objImport)
	{
		objImport->setMimeData(mimeData);
		return true;
	}
	delete mimeData;
	return false;
}

void ScribusView::startGesture(CanvasGesture* gesture)
{
//	qDebug() << "start gesture" << typeid(*m_canvasMode).name()
//			<< "---->"
//			<< typeid(*gesture).name();
	m_canvasMode->deactivate(true);
	gesture->setDelegate(m_canvasMode);
	m_canvasMode = gesture;
	m_canvasMode->activate(false);
	if (Doc->appMode != modeEditClip)
		m_canvas->repaint();
}

void ScribusView::stopGesture()
{
// 	qDebug() << "stop gesture" << typeid(*m_canvasMode).name() << (m_canvasMode->delegate() != 0);
	if (m_canvasMode->delegate())
	{
		m_canvasMode->deactivate(false);
		m_canvasMode = m_canvasMode->delegate();
		m_canvasMode->activate(true);
		if (PrefsManager::instance()->appPrefs.stickyTools)
		{
			m_canvas->m_viewMode.forceRedraw = true;
//			Doc->m_Selection->clear();
//			emit HaveSel(-1);
			m_canvas->resetRenderMode();
			updateContents();
		}
		else
			m_canvas->repaint();
	}
}

/**
switches between appmodes:
 - for submodes, activate the appropiate dialog or palette
 - set a new CanvasMode if necessary
 - call ScribusMainWindow::setAppMode(), which de/activates actions
 TODO:av make this simpler
 */
void ScribusView::requestMode(int appMode)
{
	bool updateNecessary = false;
//	qDebug() << "request mode:" << appMode;
	switch(appMode) // filter submodes
	{
		case submodePaintingDone:   // return to normal mode
//			appMode = m_previousMode < 0 ? modeNormal : m_previousMode;
			appMode = modeNormal;
			m_previousMode = -1;
			updateNecessary = true;
			break;
		case submodeEndNodeEdit:     // return from node/shape editing
//			appMode = m_previousMode < 0 ? modeNormal : m_previousMode;
			appMode = modeNormal;
			m_previousMode = -1;
			updateNecessary = true;
			break;
			/*
			 case submodeToggleNodeEdit:
				 if (Doc->appMode == modeEditClip)
				 {
					 m_ScMW->setAppMode(m_previousMode < 0 ? modeNormal : m_previousMode);
					 m_previousMode = -1;
				 }
				 else
				 {
					 m_previousMode = Doc->appMode;
					 m_ScMW->setAppMode(modeEditClip);
				 }
				 return;
				 */
		case submodeLoadPic:         // open GetImage dialog
			appMode = Doc->appMode;
			m_previousMode = appMode;
			m_ScMW->slotGetContent();
			break;
		case submodeStatusPic:       // open ManageImages dialog
			appMode = Doc->appMode;
			m_previousMode = appMode;
			m_ScMW->StatusPic();
			break;
		case submodeEditExternal:    // open external image editor
			appMode = Doc->appMode;
			m_previousMode = appMode;
			m_ScMW->callImageEditor();
			break;
		case submodeAnnotProps:
			appMode = Doc->appMode;
			m_previousMode = appMode;
			m_ScMW->ModifyAnnot();
			break;
		default:
			if (appMode < 0 || appMode > submodeFirstSubmode)
			{
//				qDebug() << "request mode: UNKNOWN" << appMode;
				appMode = modeNormal;
			}
			m_previousMode = appMode;
			break;
	}

//	qDebug() << "request mode" << Doc->appMode << "-->" << appMode;
	if (Doc->appMode != appMode)
	{
		m_ScMW->setAppMode(appMode);
		CanvasMode* newCanvasMode = modeInstances.value(appMode);
		if (!newCanvasMode)
		{
			newCanvasMode = CanvasMode::createForAppMode(this, appMode);
			modeInstances[appMode] = newCanvasMode;
		}
		if (newCanvasMode)
		{
//			qDebug() << "request canvas mode" << typeid(*newCanvasMode).name();
			m_canvasMode->deactivate(false);
			m_canvasMode = newCanvasMode;
			m_canvasMode->activate(false);
		}
		updateNecessary = true;
	}
	else
		m_ScMW->setAppMode(appMode);
//	if (appMode == modeEdit)
//	{
//		m_ScMW->activateWindow();
//	}
	if (updateNecessary)
		updateCanvas();
}



/*
void ScribusView::paintEvent ( QPaintEvent * p )
{
	#ifndef _WIN32
	if (p->spontaneous())
		evSpon = true;
	#endif
	QScrollArea::paintEvent(p);
//	QPainter qp(viewport());
//	drawContents(&qp, p->rect().x(), p->rect().y(), p->rect().width(), p->rect().height());
}
*/

void ScribusView::enterEvent(QEvent * e)
{
	m_canvasMode->enterEvent(e);
	return;
	/*
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
	{
		switch (Doc->appMode)
		{
			case modeDrawShapes:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")));
				break;
			case modeDrawImage:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")));
				break;
			case modeDrawLatex:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawLatexFrame.xpm")));
				break;
			case modeDrawText:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawTextFrame.xpm")));
				break;
			case modeDrawTable:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawTable.xpm")));
				break;
			case modeDrawRegularPolygon:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawPolylineFrame.xpm")));
				break;
			case modeDrawLine:
			case modeDrawBezierLine:
				qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
				break;
			case modeDrawFreehandLine:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawFreeLine.png"), 0, 32));
				break;
			case modeMagnifier:
				if (Magnify)
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
				else
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
				break;
			case modePanning:
				qApp->changeOverrideCursor(QCursor(loadIcon("HandC.xpm")));
				break;
			case modeMeasurementTool:
			case modeEditGradientVectors:
			case modeInsertPDFButton:
			case modeInsertPDFTextfield:
			case modeInsertPDFCheckbox:
			case modeInsertPDFCombobox:
			case modeInsertPDFListbox:
			case modeInsertPDFTextAnnotation:
			case modeInsertPDFLinkAnnotation:
				qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
				break;
			default:
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			break;
		}
	}*/
}

void ScribusView::leaveEvent(QEvent *e)
{
	m_canvasMode->leaveEvent(e);
	return;

/*	if (BlockLeave)
		return; */
	if (!m_canvas->m_viewMode.m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
/*	else
	{
		if ((SelItem.count() != 0) && (m_canvas->m_viewMode.m_MouseButtonPressed) && (!doku->DragP) && (doku->appMode == 1))
		{
			PageItem *currItem = SelItem.at(0);
			if ((b->Locked) || (b->Sizing))
				return;
			doku->DragP = true;
			doku->leaveDrag = true;
			doku->DraggedElem = b;
			doku->DragElements.clear();
			for (uint dre=0; dre<SelItem.count(); ++dre)
				doku->DragElements.append(SelItem.at(dre)->ItemNr);
			ScriXmlDoc *ss = new ScriXmlDoc();
			QDragObject *dr = new QTextDrag(ss->WriteElem(&SelItem, doku), this);
			dr->setPixmap(loadIcon("DragPix.xpm"));
			dr->drag();
			delete ss;
			ss=NULL;
			doku->DragP = false;
			doku->leaveDrag = false;
			m_canvas->m_viewMode.m_MouseButtonPressed = false;
			doku->DraggedElem = 0;
			doku->DragElements.clear();
		}
	} */
}

void ScribusView::contentsDragEnterEvent(QDragEnterEvent *e)
{
	QString text;
	bool /* dataFound = false, */ fromFile = false;
	const ScElemMimeData* elemData = dynamic_cast<const ScElemMimeData*>(e->mimeData());
	e->accept();
	if (elemData)
		text = elemData->scribusElem();
	else if (e->mimeData()->hasUrls())
	{
		QUrl url = e->mimeData()->urls().at(0);
		QFileInfo fi(url.toLocalFile());
		if (fi.exists())
		{
			fromFile = true;
			text = url.toLocalFile();
		}
	}
	if (!text.isEmpty())
	{
		e->acceptProposedAction();
		double gx, gy, gw, gh;
		ScriXmlDoc *ss = new ScriXmlDoc();
		if(ss->ReadElemHeader(text, fromFile, &gx, &gy, &gw, &gh))
		{
			FPoint dragPosDoc = m_canvas->globalToCanvas(widget()->mapToGlobal(e->pos()));
			dragX = dragPosDoc.x(); //e->pos().x() / m_canvas->scale();
			dragY = dragPosDoc.y(); //e->pos().y() / m_canvas->scale();
			dragW = gw;
			dragH = gh;
			DraggedGroup = true;
			DraggedGroupFirst = true;
			getDragRectScreen(&gx, &gy, &gw, &gh);
//			QPoint evP = viewport()->mapToGlobal(e->pos());
//			evP -= QPoint(contentsX(), contentsY());
//			redrawMarker->setGeometry(QRect(evP.x() + 1, evP.y() + 1, qRound(gw), qRound(gh)).normalized());
//			if (!redrawMarker->isVisible())
//				redrawMarker->show();
			emit ItemGeom(gw, gh);
		}
		delete ss;
		ss=NULL;
	}
}

void ScribusView::contentsDragMoveEvent(QDragMoveEvent *e)
{
	QString text;
	e->accept();
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
		text = e->mimeData()->text();
		if (DraggedGroup)
		{
//			double gx, gy, gw, gh;
			FPoint dragPosDoc = m_canvas->globalToCanvas(widget()->mapToGlobal(e->pos()));
			dragX = dragPosDoc.x(); //e->pos().x() / m_canvas->scale();
			dragY = dragPosDoc.y(); //e->pos().y() / m_canvas->scale();
//			getDragRectScreen(&gx, &gy, &gw, &gh);
//			QPoint evP = viewport()->mapToGlobal(e->pos());
//			evP -= QPoint(contentsX(), contentsY());
//			redrawMarker->setGeometry(QRect(evP.x() + 2, evP.y() + 2, qRound(gw - 2), qRound(gh - 2)).normalized());
//			if (!redrawMarker->isVisible())
//				redrawMarker->show();
			DraggedGroupFirst = false;
			emit MousePos(dragX, dragY); //+Doc->minCanvasCoordinate.x(), dragY+Doc->minCanvasCoordinate.y());
			QPoint pos = m_canvas->canvasToLocal(dragPosDoc);
			horizRuler->Draw(pos.x());
			vertRuler->Draw(pos.y());
//			return;
		}
/*		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.toLocalFile());
		QString ext = fi.extension(false).toUpper();
		QStrList imfo = QImageIO::inputFormats();
		if (ext == "JPG")
			ext = "JPEG";
		img = ((imfo.contains(ext))||(ext=="PS")||(ext=="EPS")||(ext=="TIF"));
		if (!SeleItemPos(e->pos()))
		{
			if (SelItem.count() != 0)
				Deselect(true);
		}
		else
		{
			b = SelItem.at(0);
			if (img)
			{
				if (b->PType != 2)
					Deselect(true);
			}
			else
			{
				if (b->PType != 4)
					Deselect(true);
			}
		} */
	}
}

void ScribusView::contentsDragLeaveEvent(QDragLeaveEvent *)
{
	if (DraggedGroup)
	{
		DraggedGroup = false;
		DraggedGroupFirst = false;
		m_canvas->resetRenderMode();
//		redrawMarker->hide();
		updateContents();
	}
}

void ScribusView::contentsDropEvent(QDropEvent *e)
{
	QString text;
	QUrl url;
	PageItem *currItem;
	UndoTransaction* activeTransaction = NULL;
	bool img = false;
	m_canvas->resetRenderMode();
	redrawMode = 0;
//	redrawMarker->hide();
//	struct ScText *hg;
//	uint a;
	int re = 0;
//	e->accept(Q3TextDrag::canDecode(e));
	e->accept();
	DraggedGroupFirst = false;
	FPoint dropPosDoc = m_canvas->globalToCanvas(widget()->mapToGlobal(e->pos()));
	QPointF dropPosDocQ(dropPosDoc.x(), dropPosDoc.y());
//	int ex = qRound(e->pos().x()/m_canvas->scale());// + Doc->minCanvasCoordinate.x());
//		int ey = qRound(e->pos().y()/m_canvas->scale());// + Doc->minCanvasCoordinate.y());

	if (ScMimeData::clipboardHasScribusElem())
	{
		text = ScMimeData::clipboardScribusElem();
		url  = QUrl(text);
	}
	else if (e->mimeData()->hasText())
	{
		text = e->mimeData()->text();
		url = QUrl(text);
	}
	else if (e->mimeData()->hasUrls())
	{
		url = e->mimeData()->urls().at(0);
		text = "";
	}
//	qDebug() << "ScribusView::contentsDropEvent" << e->mimeData()->formats() << url;
	if (!url.isEmpty())
	{
		e->acceptProposedAction();
		//<<#3524
		activateWindow();
		if (!m_ScMW->ScriptRunning)
			raise();
		m_ScMW->newActWin(Doc->WinHan);
		updateContents();
		//>>
		QFileInfo fi(url.toLocalFile());
		QString ext = fi.suffix().toUpper();
		QStringList imfo;
		QList<QByteArray> imgs = QImageReader::supportedImageFormats();
		for (int i = 0; i < imgs.count(); ++i )
		{
			imfo.append(QString(imgs.at(i)).toUpper());
		}
		if (ext == "JPG")
			ext = "JPEG";
		//CB Need to handle this ugly file extension list elsewhere... some capabilities class perhaps
		img = ((imfo.contains(ext)) || extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext) || extensionIndicatesTIFF(ext) || extensionIndicatesJPEG(ext) || extensionIndicatesPSD(ext));
		bool selectedItemByDrag=false;
//		int pscx=qRound(e->pos().x()/m_canvas->scale()), pscy=qRound(e->pos().y()/m_canvas->scale());
		//Loop through all items and see which one(s) were under the drop point on the current layer
		//Should make a nice function for this.
		for (int i=0; i<Doc->Items->count(); ++i)
		{
			if (Doc->Items->at(i)->LayerNr==Doc->activeLayer())
			{
				if (m_canvas->frameHitTest(dropPosDocQ, Doc->Items->at(i)) >= Canvas::INSIDE)
				{
					Deselect(false);
					SelectItem(Doc->Items->at(i));
					selectedItemByDrag=true;
					break;
				}
			}
		}
		bool vectorFile = false;
		if (fi.exists())
		{
			if ((fi.suffix().toLower() == "sml") || (fi.suffix().toLower() == "shape") || (fi.suffix().toLower() == "sce"))
				vectorFile = true;
			else
			{
				FileLoader *fileLoader = new FileLoader(url.toLocalFile());
				int testResult = fileLoader->TestFile();
				delete fileLoader;
				if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT))
					vectorFile = true;
			}
		}
		else
		{
			if ((text.startsWith("<SCRIBUSELEM")) || (text.startsWith("SCRIBUSFRAGMENT")))
				vectorFile = true;
		}
//		qDebug() << "drop - img:" << img << "file:" << fi.exists() << "suffix:" << fi.suffix() << "select by drag:" << selectedItemByDrag;
		//CB When we drag an image to a page from outside
		//SeleItemPos is from 1.2.x. Needs reenabling for dragging *TO* a frame
		if ((fi.exists()) && (img) && !selectedItemByDrag && !vectorFile)// && (!SeleItemPos(e->pos())))
		{
			int z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, dropPosDoc.x(), dropPosDoc.y(), 1, 1, Doc->toolSettings.dWidth, Doc->toolSettings.dBrushPict, CommonStrings::None, true);
			PageItem *b = Doc->Items->at(z);
			b->LayerNr = Doc->activeLayer();
			Doc->LoadPict(url.toLocalFile(), b->ItemNr);
			b->setWidth(static_cast<double>(b->OrigW * 72.0 / b->pixm.imgInfo.xres));
			b->setHeight(static_cast<double>(b->OrigH * 72.0 / b->pixm.imgInfo.yres));
			b->OldB2 = b->width();
			b->OldH2 = b->height();
			b->updateClip();
			emit DocChanged();
			update();
			return;
		}
		//if ((SeleItemPos(e->pos())) && (!text.startsWith("<SCRIBUSELEM")))
//		if (Doc->m_Selection->count()>0 && (m_canvas->frameHitTest(dropPosDocQ, Doc->m_Selection->itemAt(0)) >= Canvas::INSIDE) && !vectorFile) // && (img))
		if (selectedItemByDrag && (m_canvas->frameHitTest(dropPosDocQ, Doc->m_Selection->itemAt(0)) >= Canvas::INSIDE) && ((!vectorFile) || (img)))
		{
			PageItem *b = Doc->m_Selection->itemAt(0);
			if (b->itemType() == PageItem::ImageFrame)
			{
				if ((fi.exists()) && (img))
					Doc->LoadPict(url.toLocalFile(), b->ItemNr);
			}
			else if (b->itemType() == PageItem::TextFrame)
			{
				if ((fi.exists()) && (!img))
				{
					QByteArray file;
					QTextCodec *codec = QTextCodec::codecForLocale();
					// TODO create a Dialog for selecting the codec
					if (loadRawText(url.toLocalFile(), file))
					{
						QString txt = codec->toUnicode( file.data() );
						txt.replace(QRegExp("\r"), QChar(13));
						txt.replace(QRegExp("\n"), QChar(13));
						txt.replace(QRegExp("\t"), QChar(9));
						b->itemText.insertChars(b->CPos, txt, true);
						if (Doc->docHyphenator->AutoCheck)
							Doc->docHyphenator->slotHyphenate(b);
						b->invalidateLayout();
						b->update();
					}
				}
			}
			emit DocChanged();
			update();
		}
		else
		{
			Deselect(true);
			uint oldDocItemCount = Doc->Items->count();
			if (((!img) || (vectorFile)) && (Doc->DraggedElem == 0))
			{
				activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Create,"",Um::ICreate));
				if (fi.exists())
				{
					QString data;
					if (fi.suffix().toLower() == "sml")
					{
						QByteArray cf;
						loadRawText(url.toLocalFile(), cf);
						StencilReader *pre = new StencilReader();
						QString f = QString::fromUtf8(cf.data());
						data = pre->createObjects(f);
						delete pre;
						emit LoadElem(data, dropPosDoc.x(), dropPosDoc.y(), false, false, Doc, this);
					}
					else if (fi.suffix().toLower() == "shape")
					{
						QByteArray cf;
						loadRawText(url.toLocalFile(), cf);
						StencilReader *pre = new StencilReader();
						QString f = QString::fromUtf8(cf.data());
						data = pre->createShape(f);
						delete pre;
						emit LoadElem(data, dropPosDoc.x(), dropPosDoc.y(), false, false, Doc, this);
					}
					else if (fi.suffix().toLower() == "sce")
					{
						emit LoadElem(url.toLocalFile(), dropPosDoc.x(), dropPosDoc.y(), true, false, Doc, this);
					}
					else
					{
						FileLoader *fileLoader = new FileLoader(url.toLocalFile());
						int testResult = fileLoader->TestFile();
						delete fileLoader;
						if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT))
						{
							const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
							if( fmt )
							{
								fmt->loadFile(url.toLocalFile(), LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
								if (Doc->m_Selection->count() > 0)
								{
									double x2, y2, w, h;
									Doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
									Doc->moveGroup(dropPosDoc.x() - x2, dropPosDoc.y() - y2);
									m_ScMW->propertiesPalette->updateColorList();
									m_ScMW->propertiesPalette->paraStyleCombo->updateFormatList();
									m_ScMW->propertiesPalette->charStyleCombo->updateFormatList();
									m_ScMW->propertiesPalette->SetLineFormats(Doc);
								}
							}
						}
					}
				}
				else
				{
					emit LoadElem(QString(text), dropPosDoc.x(), dropPosDoc.y(), false, false, Doc, this);
				}
				Selection tmpSelection(this, false);
				tmpSelection.copy(*Doc->m_Selection, true);
				for (int as = oldDocItemCount; as < Doc->Items->count(); ++as)
				{
					currItem = Doc->Items->at(as);
					Doc->setRedrawBounding(currItem);
					tmpSelection.addItem(currItem, true);
					if (currItem->isBookmark)
						emit AddBM(currItem);
				}
				Doc->m_Selection->copy(tmpSelection, false);
				activeTransaction->commit();
				delete activeTransaction;
				activeTransaction = NULL;
			}
			else
			{
				if (Doc->DraggedElem != 0)
				{
					if (!Doc->leaveDrag)
					{
						QMenu *pmen = new QMenu();
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
						pmen->addAction( tr("Copy Here"));
						QAction* mov = pmen->addAction( tr("Move Here"));
						pmen->addAction( tr("Cancel"));
						for (int dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							if (Doc->Items->at(Doc->DragElements[dre])->locked())
							{
								mov->setEnabled(false);
								break;
							}
						}
						re = pmen->actions().indexOf(pmen->exec(QCursor::pos()));
						delete pmen;
						pmen=NULL;
					}
					else
						re = 1;
					if ((re == 2) || (re == -1))
					{
						updateContents();
						return;
					}
					if ((re == 1) || (Doc->leaveDrag))
					{
						QList<PageItem*> pasted;
						emit LoadElem(QString(text), dropPosDoc.x(), dropPosDoc.y(), false, false, Doc, this);
						for (int as = oldDocItemCount; as < Doc->Items->count(); ++as)
						{
							pasted.append(Doc->Items->at(as));
						}
						Selection tmpSelection(this, false);
						tmpSelection.copy(*Doc->m_Selection, true);
						for (int dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							tmpSelection.addItem(Doc->Items->at(Doc->DragElements[dre]), true);
						}
						Doc->m_Selection->copy(tmpSelection, false);
						PageItem* bb;
						int fin;
						for (int dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							bb = pasted.at(dre);
							currItem = Doc->m_Selection->itemAt(dre);
							if ((currItem->asTextFrame()) && ((currItem->nextInChain() != 0) || (currItem->prevInChain() != 0)))
							{
								PageItem* before = currItem->prevInChain();
								PageItem* after = currItem->nextInChain();
								currItem->unlink();
								if (before != 0)
								{
									fin = Doc->m_Selection->findItem(before);
									if (fin != -1)
										before = pasted.at(fin);
									before->unlink();
									before->link(bb);
								}
								if (after != 0)
								{
									fin = Doc->m_Selection->findItem(after);
									if (fin != -1)
										after = pasted.at(fin);
									bb->link(after);
								}
							}
						}
						pasted.clear();
						Doc->itemSelection_DeleteItem();
					}
				}
				if ((!img) && ((re == 0)))
					emit LoadElem(QString(text), dropPosDoc.x(), dropPosDoc.y(), false, false, Doc, this);
				Doc->DraggedElem = 0;
				Doc->DragElements.clear();
				Selection tmpSelection(this, false);
				tmpSelection.copy(*Doc->m_Selection, true);
				for (int as = oldDocItemCount; as < Doc->Items->count(); ++as)
				{
					currItem = Doc->Items->at(as);
					Doc->setRedrawBounding(currItem);
					tmpSelection.addItem(currItem, true);
					if (currItem->isBookmark)
						emit AddBM(currItem);
				}
				Doc->m_Selection->copy(tmpSelection, false);
			}
			if (Doc->m_Selection->count() > 1)
			{
				Doc->m_Selection->connectItemToGUI();
				Doc->m_Selection->setGroupRect();
				double gx, gy, gh, gw;
				Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				double nx = gx;
				double ny = gy;
				if (!Doc->ApplyGuides(&nx, &ny))
				{
					FPoint npx;
					npx = Doc->ApplyGridF(FPoint(nx, ny));
					nx = npx.x();
					ny = npx.y();
				}
				activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Move,"",Um::IMove));
				Doc->moveGroup(nx-gx, ny-gy, false);
				Doc->m_Selection->setGroupRect();
				Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				nx = gx+gw;
				ny = gy+gh;
				Doc->ApplyGuides(&nx, &ny);
				Doc->moveGroup(nx-(gx+gw), ny-(gy+gh), false);
				Doc->m_Selection->setGroupRect();
				Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				for (int a = 0; a < Doc->m_Selection->count(); ++a)
				{
					PageItem *currItem = Doc->m_Selection->itemAt(a);
					currItem->LayerNr = Doc->activeLayer();
					currItem->gXpos = currItem->xPos() - gx;
					currItem->gYpos = currItem->yPos() - gy;
					currItem->gWidth = gw;
					currItem->gHeight = gh;
				}
				activeTransaction->commit();
				delete activeTransaction;
				activeTransaction = NULL;
				emit ItemPos(gx, gy);
				emit ItemGeom(gw, gh);
			}
			else if (Doc->m_Selection->count() == 1)
			{
				Doc->m_Selection->connectItemToGUI();
				currItem = Doc->m_Selection->itemAt(0);
				currItem->LayerNr = Doc->activeLayer();
				if (Doc->useRaster)
				{
					double nx = currItem->xPos();
					double ny = currItem->yPos();
					if (!Doc->ApplyGuides(&nx, &ny))
					{
						FPoint npx;
						npx = Doc->ApplyGridF(FPoint(nx, ny));
						nx = npx.x();
						ny = npx.y();
					}
					Doc->MoveItem(nx-currItem->xPos(), ny-currItem->yPos(), currItem);
				}
			}
			updateContents();
		}
		if (!Doc->masterPageMode())
		{
			uint docPagesCount=Doc->Pages->count();
			uint docCurrPageNo=Doc->currentPageNumber();
			for (uint i = 0; i < docPagesCount; ++i)
			{
				double x = Doc->Pages->at(i)->xOffset();
				double y = Doc->Pages->at(i)->yOffset();
				double w = Doc->Pages->at(i)->width();
				double h = Doc->Pages->at(i)->height();
				if (QRectF(x, y, w, h).contains(dropPosDocQ))
				{
					if (docCurrPageNo != i)
					{
						Doc->setCurrentPage(Doc->Pages->at(i));
						setMenTxt(i);
						DrawNew();
					}
					break;
				}
			}
			setRulerPos(contentsX(), contentsY());
		}
	}
}


/*
void ScribusView::normalizeSelectionRect() // unsused
{
	if(Mxp > SeRx)
	{
		int tmp = SeRx;
		SeRx = Mxp;
		Mxp = tmp;
	}
	if(Myp > SeRy)
	{
		int tmp = SeRy;
		SeRy = Myp;
		Myp = tmp;
	}
}
*/


void ScribusView::getDragRectScreen(double *x, double *y, double *w, double *h)
{
	QPoint in(qRound(dragX*m_canvas->scale()), qRound(dragY*m_canvas->scale()));
//	in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
	QPoint out = contentsToViewport(in);
	*x = static_cast<double>(out.x());
	*y = static_cast<double>(out.y());
	*w = dragW*m_canvas->scale();
	*h = dragH*m_canvas->scale();
}

void ScribusView::getGroupRectScreen(double *x, double *y, double *w, double *h)
{
	double gx, gy, gh, gw;
	Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
	QPoint in(qRound(gx*m_canvas->scale()), qRound(gy*m_canvas->scale()));
//	in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
	QPoint out = contentsToViewport(in);
	*x = static_cast<double>(out.x());
	*y = static_cast<double>(out.y());
	*w = gw*m_canvas->scale();
	*h = gh*m_canvas->scale();
}



void ScribusView::RefreshGradient(PageItem *currItem, double dx, double dy)
{
	QMatrix matrix;
	QRect rect = currItem->getRedrawBounding(m_canvas->scale());
	m_canvas->Transform(currItem, matrix);
	FPointArray fpNew;
	fpNew.setPoints(2, currItem->GrStartX, currItem->GrStartY, currItem->GrEndX, currItem->GrEndY);
	fpNew.map(matrix);
	if (dx < 8.0) dx = 8.0;
	if (dy < 8.0) dy = 8.0;
	int grl = (int) floor( qMin(fpNew.point(0).x(), fpNew.point(1).x()) - dx );
	int grr = (int) ceil ( qMax(fpNew.point(0).x(), fpNew.point(1).x()) + dx );
	int grb = (int) ceil ( qMax(fpNew.point(0).y(), fpNew.point(1).y()) + dy );
	int grt = (int) floor( qMin(fpNew.point(0).y(), fpNew.point(1).y()) - dy );
	rect |= QRect(grl, grt, grr-grl, grb-grt);
	updateContents(rect);
}


//CB-->elsewhere, util, however, only used in the view for now
bool ScribusView::PointOnLine(QPoint Start, QPoint End, QRect MArea)
{
	QPoint an, en;
	if (Start.x() == End.x())
	{
		an = Start.y() > End.y() ? End : Start;
		en = an == End ? Start : End;
		for (int a=an.y(); a<en.y(); ++a)
		{
			if (MArea.contains(an.x(), a))
				return true;
		}
	}
	if (Start.y() == End.y())
	{
		an = Start.x() > End.x() ? End : Start;
		en = an == End ? Start : End;
		for (int a=an.x(); a<en.x(); ++a)
		{
			if (MArea.contains(a, an.y()))
				return true;
		}
	}
	if (abs(Start.x() - End.x()) > abs(Start.y() - End.y()))
	{
		an = Start.x() > End.x() ? End : Start;
		en = an == End ? Start : End;
		double stg = (en.y() - an.y()) / static_cast<double>((en.x() - an.x()));
		for (int a = an.x(); a < en.x(); ++a)
		{
			if (MArea.contains(a, an.y()+qRound((a-an.x())*stg)))
				return true;
		}
	}
	else
	{
		an = Start.y() > End.y() ? End : Start;
		en = an == End ? Start : End;
		double stg = (en.x() - an.x()) / static_cast<double>((en.y() - an.y()));
		for (int a = an.y(); a < en.y(); ++a)
		{
			if (MArea.contains(an.x()+qRound((a-an.y())*stg), a))
				return true;
		}
	}
	return false;
}

//CB-->Doc??
void ScribusView::TransformPoly(int mode, int rot, double scaling)
{
	PageItem *currItem = Doc->m_Selection->itemAt(0);
	currItem->ClipEdited = true;
	QMatrix ma;
	if (Doc->nodeEdit.isContourLine)
	{
		FPoint tp2(getMinClipF(&currItem->ContourLine));
		FPoint tp(getMaxClipF(&currItem->ContourLine));
		currItem->ContourLine.translate(-qRound((tp.x() + tp2.x()) / 2.0), -qRound((tp.y() + tp2.y()) / 2.0));
		switch (mode)
		{
		case 0:
			ma.rotate(-rot);
			break;
		case 1:
			ma.rotate(rot);
			break;
		case 2:
			ma.scale(1.0 - (scaling / 100.0), 1.0 - (scaling / 100.0));
			break;
		case 3:
			ma.scale(1.0 + (scaling / 100.0), 1.0 + (scaling / 100.0));
			break;
		case 4:
			ma.shear(0.017455, 0);
			break;
		case 5:
			ma.shear(-0.017455, 0);
			break;
		case 6:
			ma.shear(0, -0.017455);
			break;
		case 7:
			ma.shear(0, 0.017455);
			break;
		case 8:
			ma.scale(1.0 - (scaling / 100.0), 1.0 - (scaling / 100.0));
			break;
		case 9:
			ma.scale(1.0 + (scaling / 100.0), 1.0 + (scaling / 100.0));
			break;
		//10-13 are for scaling the contour line in shape edit mode
		case 10:
			{
				double s=1.0 - (scaling/(tp2.x() - tp.x()));
				ma.scale(s, 1);
				ma.translate(-scaling/s/2,0);
			}
			break;
		case 11:
			{
				double s=1.0 - (scaling/(tp2.x() - tp.x()));
				ma.scale(s, 1);
				ma.translate(scaling/s/2,0);
			}
			break;
		case 12:
			{
				double s=1.0 - (scaling/(tp2.y() - tp.y()));
				ma.scale(1, s);
				ma.translate(0,-scaling/s/2);
			}
			break;
		case 13:
			{
				double s=1.0 - (scaling/(tp2.y() - tp.y()));
				ma.scale(1, s);
				ma.translate(0,scaling/s/2);
			}
			break;
		}
		currItem->ContourLine.map(ma);
		currItem->ContourLine.translate(qRound((tp.x() + tp2.x()) / 2.0), qRound((tp.y() + tp2.y()) / 2.0));
		updateContents();
		currItem->FrameOnly = true;
		updateContents(currItem->getRedrawBounding(m_canvas->scale()));
		if (UndoManager::undoEnabled())
		{
			undoManager->setUndoEnabled(false);
			currItem->checkChanges(true);
			undoManager->setUndoEnabled(true);
			SimpleState *ss = new SimpleState(Um::EditContourLine, "", Um::IBorder);
			ss->set("EDIT_CONTOUR", "edit_contour");
			ss->set("MODE", mode);
			ss->set("ROT", rot);
			ss->set("SCALING", scaling);
			undoManager->action(currItem, ss);
		}
		emit DocChanged();
		return;
	}
	FPoint oldPos(currItem->xyPos());
	double offsX = currItem->width() / 2.0;
	double offsY = currItem->height() / 2.0;
	ma.translate(-offsX, -offsY);
	switch (mode)
	{
	case 0:
		ma.rotate(-rot);
		break;
	case 1:
		ma.rotate(rot);
		break;
	case 2:
		ma.scale(1.0 - (scaling / 100.0), 1.0 - (scaling / 100.0));
		break;
	case 3:
		ma.scale(1.0 + (scaling / 100.0), 1.0 + (scaling / 100.0));
		break;
	case 4:
		ma.shear(0.017455, 0);
		break;
	case 5:
		ma.shear(-0.017455, 0);
		break;
	case 6:
		ma.shear(0, -0.017455);
		break;
	case 7:
		ma.shear(0, 0.017455);
		break;
	case 8:
		ma.scale(1.0 - (scaling / currItem->width()),1.0 - (scaling / currItem->height()));
		break;
	case 9:
		ma.scale(1.0 + (scaling / currItem->width()),1.0 + (scaling / currItem->height()));
		break;
	}
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(offsX, offsY);
	Doc->AdjustItemSize(currItem);
	QMatrix ma2;
	ma2.translate(oldPos.x(), oldPos.y());
	ma2.scale(1, 1);
	ma2.translate(offsX, offsY);
	FPoint n(-offsX, -offsY);
	switch (mode)
	{
	case 0:
		ma2.rotate(-rot);
		break;
	case 1:
		ma2.rotate(rot);
		break;
	case 2:
		ma2.scale(1.0 - (scaling / 100.0), 1.0 - (scaling / 100.0));
		break;
	case 3:
		ma2.scale(1.0 + (scaling / 100.0), 1.0 + (scaling / 100.0));
		break;
	case 4:
		ma2.shear(0.017455, 0);
		break;
	case 5:
		ma2.shear(-0.017455, 0);
		break;
	case 6:
		ma2.shear(0, -0.017455);
		break;
	case 7:
		ma2.shear(0, 0.017455);
		break;
	case 8:
		ma2.scale(1.0 - (scaling / currItem->width()),1.0 - (scaling / currItem->height()));
		break;
	case 9:
		ma2.scale(1.0 + (scaling / currItem->width()),1.0 + (scaling / currItem->height()));
		break;
	}
	double x = ma2.m11() * n.x() + ma2.m21() * n.y() + ma2.dx();
	double y = ma2.m22() * n.y() + ma2.m12() * n.x() + ma2.dy();
	Doc->MoveItem(x-oldPos.x(), y-oldPos.y(), currItem);
	if (currItem->asPathText())
		currItem->updatePolyClip();
	Doc->setRedrawBounding(currItem);
	currItem->update();
//	MarkClip(currItem, currItem->PoLine, true);
	currItem->FrameType = 3;
	if (UndoManager::undoEnabled())
	{
		undoManager->setUndoEnabled(false);
		currItem->checkChanges(true);
		undoManager->setUndoEnabled(true);
		SimpleState *ss = new SimpleState(Um::EditShape, "", Um::IBorder);
		ss->set("EDIT_SHAPE", "edit_shape");
		ss->set("MODE", mode);
		ss->set("ROT", rot);
		ss->set("SCALING", scaling);
		undoManager->action(currItem, ss);
	}
	emit DocChanged();
}

/*
void ScribusView::Reset1Control()
{
	Doc->nodeEdit.reset1Control(Doc->m_Selection->itemAt(0));
}

void ScribusView::ResetControl()
{
	PageItem *currItem = Doc->m_Selection->itemAt(0);
	Doc->nodeEdit.resetControl(currItem);
}
*/

/*
//CB-->Doc
void ScribusView::moveGroup(double x, double y, bool fromMP, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : Doc->m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;

	if (!_groupTransactionStarted && selectedItemCount > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint i = 0; i < selectedItemCount; ++i)
			tooltip += "\t" + itemSelection->itemAt(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Move, tooltip, Um::IMove);
		_groupTransactionStarted = true;
	}

	Doc->moveGroup(x, y, fromMP, itemSelection);
}

//CB-->Doc
void ScribusView::RotateGroup(double win)
{
	if (!_groupTransactionStarted && Doc->m_Selection->count() > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (int i = 0; i < Doc->m_Selection->count(); ++i)
			tooltip += "\t" + Doc->m_Selection->itemAt(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Rotate, tooltip, Um::IRotate);
		_groupTransactionStarted = true;
	}

	Doc->rotateGroup(win, RCenter);
}



void ScribusView::scaleGroup(double scx, double scy, bool scaleText, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : Doc->m_Selection;
	Q_ASSERT(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	if (selectedItemCount == 0)
		return;

	if (!_groupTransactionStarted && selectedItemCount > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint i = 0; i < selectedItemCount; ++i)
			tooltip += "\t" + itemSelection->itemAt(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Resize, tooltip, Um::IResize);
		_groupTransactionStarted = true;
	}

	Doc->scaleGroup(scx, scy, scaleText, itemSelection);
	emit DocChanged();
}
*/


bool ScribusView::slotSetCurs(int x, int y)
{
	PageItem *currItemGeneric;
	if (GetItem(&currItemGeneric))
	{
		if (!((currItemGeneric->asTextFrame()) || (currItemGeneric->asImageFrame())))
			return false;
		// unsed by gcc warning - PV
		//int xP = qRound(x/m_canvas->scale() + Doc->minCanvasCoordinate.x());
		//int yP = qRound(y/m_canvas->scale() + Doc->minCanvasCoordinate.y());
 		QMatrix p;
//		p.begin(this);
		m_canvas->Transform(currItemGeneric, p);
//		p.translate(qRound(-Doc->minCanvasCoordinate.x()), qRound(-Doc->minCanvasCoordinate.y()));
		if (currItemGeneric->asImageFrame())
			return true;
		PageItem_TextFrame *currItem=currItemGeneric->asTextFrame();
		if (currItem==0)
			return false;
		QRect mpo(x - Doc->guidesSettings.grabRad, y - Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
		if ((QRegion(p.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo)) ||
		        (QRegion(p.map(currItem->Clip)).contains(mpo)))
		{
			if(currItem->reversed())
			{ //handle Right to Left writing
				FPoint point(currItem->width()-((x + 0*Doc->minCanvasCoordinate.x()) / m_canvas->scale() - currItem->xPos()),
							 (y + 0*Doc->minCanvasCoordinate.x()) / m_canvas->scale() - currItem->yPos());
				currItem->CPos = currItem->itemText.length() == 0 ? 0 :
					currItem->itemText.screenToPosition(point);
			}
			else
			{
				FPoint point((x + 0*Doc->minCanvasCoordinate.x()) / m_canvas->scale() - currItem->xPos(),
							 (y + 0*Doc->minCanvasCoordinate.x()) / m_canvas->scale() - currItem->yPos());
				currItem->CPos = currItem->itemText.length() == 0 ? 0 :
					currItem->itemText.screenToPosition(point);
			}

			if (currItem->itemText.length() > 0)
			{
				int b=qMin(currItem->CPos-1, currItem->itemText.length());
				if (b<0)
					b=0;
				Doc->currentStyle.charStyle() = currItem->itemText.charStyle(b);
				emit ItemTextStrike(Doc->currentStyle.charStyle().strikethruOffset(), Doc->currentStyle.charStyle().strikethruWidth());
				emit ItemTextUnderline(Doc->currentStyle.charStyle().underlineOffset(), Doc->currentStyle.charStyle().underlineWidth());
				emit ItemTextOutline(Doc->currentStyle.charStyle().outlineWidth());
				emit ItemTextShadow(Doc->currentStyle.charStyle().shadowXOffset(), Doc->currentStyle.charStyle().shadowYOffset());
				emit ItemTextSca(Doc->currentStyle.charStyle().scaleH());
				emit ItemTextScaV(Doc->currentStyle.charStyle().scaleV());
				emit ItemTextFarben(Doc->currentStyle.charStyle().strokeColor(), Doc->currentStyle.charStyle().fillColor(), Doc->currentStyle.charStyle().strokeShade(), Doc->currentStyle.charStyle().fillShade());
				emit ItemTextFont(Doc->currentStyle.charStyle().font().scName());
				emit ItemTextSize(Doc->currentStyle.charStyle().fontSize());
				emit ItemTextUSval(Doc->currentStyle.charStyle().tracking());
				emit ItemTextStil(Doc->currentStyle.charStyle().effects());
				emit ItemTextAbs(currItem->itemText.paragraphStyle(b).alignment());
				emit ItemTextBase(Doc->currentStyle.charStyle().baselineOffset());
				return true;
			}
			else
			{
				Doc->currentStyle.charStyle() = currItem->itemText.defaultStyle().charStyle();
				emit ItemTextStrike(currItem->itemText.defaultStyle().charStyle().strikethruOffset(), currItem->itemText.defaultStyle().charStyle().strikethruWidth());
				emit ItemTextUnderline(currItem->itemText.defaultStyle().charStyle().underlineOffset(), currItem->itemText.defaultStyle().charStyle().underlineWidth());
				emit ItemTextOutline(currItem->itemText.defaultStyle().charStyle().outlineWidth());
				emit ItemTextShadow(currItem->itemText.defaultStyle().charStyle().shadowXOffset(), currItem->itemText.defaultStyle().charStyle().shadowYOffset());
				emit ItemTextSca(currItem->itemText.defaultStyle().charStyle().scaleH());
				emit ItemTextScaV(currItem->itemText.defaultStyle().charStyle().scaleV());
				emit ItemTextFarben(currItem->itemText.defaultStyle().charStyle().strokeColor(), currItem->itemText.defaultStyle().charStyle().fillColor(), currItem->itemText.defaultStyle().charStyle().strokeShade(), currItem->itemText.defaultStyle().charStyle().fillShade());
				emit ItemTextFont(currItem->itemText.defaultStyle().charStyle().font().scName());
				emit ItemTextSize(currItem->itemText.defaultStyle().charStyle().fontSize());
				emit ItemTextUSval(currItem->itemText.defaultStyle().charStyle().tracking());
				emit ItemTextStil(currItem->itemText.defaultStyle().charStyle().effects());
				emit ItemTextAbs( 0 );
				emit ItemTextBase(currItem->itemText.defaultStyle().charStyle().baselineOffset());
				return true;
			}
		}
	}
	return false;
}


void ScribusView::dragTimerTimeOut()
{
	m_dragTimerFired = true;
	qApp->changeOverrideCursor(QCursor(loadIcon("DragPix.xpm")));
}

void ScribusView::HandleCurs(PageItem *currItem, QRect mpo)
{
	QPoint tx, tx2;
	QMatrix ma;
//	ma.translate(-Doc->minCanvasCoordinate.x()*m_canvas->scale(), -Doc->minCanvasCoordinate.y()*m_canvas->scale());
	m_canvas->Transform(currItem, ma);
	tx = ma.map(QPoint(static_cast<int>(currItem->width()), 0));
	tx2 = ma.map(QPoint(0, static_cast<int>(currItem->height())));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (Doc->appMode == modeRotation)
			qApp->changeOverrideCursor(QCursor(loadIcon("Rotieren2.png")));
		else
		{
			double rr = fabs(currItem->rotation());
			if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >=315.0) && (rr <= 360.0)))
				qApp->changeOverrideCursor(QCursor(Qt::SizeBDiagCursor));
			if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
				qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
		}
	}
	tx = ma.map(QPoint(static_cast<int>(currItem->width()), static_cast<int>(currItem->height())/2));
	tx2 = ma.map(QPoint(0, static_cast<int>(currItem->height())/2));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(currItem->rotation());
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			qApp->changeOverrideCursor(QCursor(Qt::SizeVerCursor));
	}
	tx = ma.map(QPoint(static_cast<int>(currItem->width())/2, 0));
	tx2 = ma.map(QPoint(static_cast<int>(currItem->width())/2, static_cast<int>(currItem->height())));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(currItem->rotation());
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			qApp->changeOverrideCursor(QCursor(Qt::SizeVerCursor));
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
	}
	tx = ma.map(QPoint(static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
	tx2 = ma.map(QPoint(0, 0));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (Doc->appMode == modeRotation)
			qApp->changeOverrideCursor(QCursor(loadIcon("Rotieren2.png")));
		else
		{
			double rr = fabs(currItem->rotation());
			if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) ||
			        ((rr >= 315.0) && (rr <= 360.0)))
				qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
			if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
				qApp->changeOverrideCursor(QCursor(Qt::SizeBDiagCursor));
		}
	}
	if (Doc->appMode == modeEditClip)
		qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
}

void ScribusView::SelectItemNr(uint nr, bool draw, bool single)
{
	if (nr < static_cast<uint>(Doc->Items->count()))
		SelectItem(Doc->Items->at(nr), draw, single);
}

//CB-->Doc/Fix
void ScribusView::SelectItem(PageItem *currItem, bool draw, bool single)
{
	if (!currItem->isSelected())
	{
		if (single)
		{
			Doc->m_Selection->addItem(currItem);
			currItem->isSingleSel = true;
			updateContents(currItem->getRedrawBounding(m_canvas->scale()));
		}
		else
		{
			if (currItem->Groups.count() != 0)
			{
				if (Doc->m_Selection->count() != 0)
				{
					if (Doc->m_Selection->findItem(currItem) == -1)
						Doc->m_Selection->addItem(currItem);
				}
				else
					Doc->m_Selection->addItem(currItem);

				for (int ga=0; ga<Doc->Items->count(); ++ga)
				{
					if (Doc->Items->at(ga)->Groups.count() != 0)
					{
						if (Doc->Items->at(ga)->Groups.top() == currItem->Groups.top())
						{
							if (Doc->Items->at(ga)->ItemNr != currItem->ItemNr)
							{
								if (Doc->m_Selection->findItem(Doc->Items->at(ga)) == -1)
									Doc->m_Selection->addItem(Doc->Items->at(ga));
							}
							if (draw)
							{
								updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							}
						}
					}
				}
			}
			else
			{
				Doc->m_Selection->addItem(currItem);
				if (draw)
				{
					updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				}
			}
			//CB FIXME/TODO We are surely prepending here and we have turned off
			//emitting in prepend below so do it here.
			//Doc->m_Selection->itemAt(0)->emitAllToGUI();
		}
	}
	else
	{
	//CB Prepend used to occur to enable level changes to work properly, however with
	//current selection code we dont seem to need that anymore
	/*
		if (Doc->m_Selection->count() > 1)
		{
			PageItem *bb = Doc->m_Selection->itemAt(0);
			Doc->m_Selection->removeItem(currItem);
			Doc->m_Selection->prependItem(currItem, false);
			currItem->paintObj();
			bb->paintObj();
		}*/
	}
	if (draw)
	{
		if (Doc->m_Selection->count() > 1)
		{
			Doc->m_Selection->setGroupRect();
			double x, y, w, h;
			Doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			emit ItemPos(x, y);
			emit ItemGeom(w, h);
			getGroupRectScreen(&x, &y, &w, &h);
			updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
			//CB move in here as the emitAllToGUI will do it otherwise
			emit HaveSel(currItem->itemType());
		}
		//CB done by addItem for single selection or the frame data is already there
		//else
			//EmitValues(currItem);
			//currItem->emitAllToGUI();
	}
}

void ScribusView::selectionChanged()
{
	if (m_ScMW->isObjectSpecificUndo())
	{
		uint docSelectionCount=Doc->m_Selection->count();
		if (docSelectionCount == 1)
			undoManager->showObject(Doc->m_Selection->itemAt(0)->getUId());
		else if (docSelectionCount == 0)
			undoManager->showObject(Doc->currentPage()->getUId());
		else
			undoManager->showObject(Um::NO_UNDO_STACK);
	}
}

bool ScribusView::GetItem(PageItem **currItem, int nr)
{
	int n=nr;
	if (n == -1)
		n=0;
	*(currItem) = Doc->m_Selection->itemAt(n);
	return (*(currItem)!=NULL);
}

//CB Remove bookmark interaction here, item/doc should do it
void ScribusView::Deselect(bool prop)
{
	if (!Doc->m_Selection->isEmpty())
	{
		const double scale = m_canvas->scale();
		PageItem* currItem=NULL;
		for (int a = 0; a < Doc->m_Selection->count(); ++a)
		{
			currItem = Doc->m_Selection->itemAt(a);
			if ((currItem->asTextFrame()) && (currItem->isBookmark))
				emit ChBMText(currItem);
		}
		if (Doc->m_Selection->isMultipleSelection())
		{
			double x, y, w, h;
			Doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			Doc->m_Selection->clear();
			updateCanvas(x - 5/scale, y - 5/scale, w + 10/scale, h + 10/scale);
		}
		else
		{
			currItem = Doc->m_Selection->itemAt(0);
			if (currItem != NULL)
			{
				currItem->itemText.deselectAll();
				currItem->HasSel = false;
			}
			Doc->m_Selection->clear();
			if (currItem != NULL)
				updateContents(currItem->getRedrawBounding(scale));
		}
	}
	if (prop)
		emit HaveSel(-1);
}


//CB-->Doc/Fix
void ScribusView::ToggleBookmark()
{
	uint docSelectionCount=Doc->m_Selection->count();
	if (docSelectionCount != 0)
	{
		for (uint a = 0; a < docSelectionCount; ++a)
		{
			PageItem* currItem = Doc->m_Selection->itemAt(a);
			if (currItem->asTextFrame())
			{
				if (currItem->OwnPage != -1)
				{
					bool old = currItem->isBookmark;
					currItem->isBookmark = !currItem->isBookmark;
					if (currItem->isBookmark)
					{
						currItem->setIsAnnotation(false);
						emit AddBM(currItem);
					}
					else
					{
						if (old)
							emit DelBM(currItem);
					}
				}
			}
		}
		m_ScMW->actionManager->setPDFActions(this);
		emit DocChanged();
	}
}

//CB-->Doc/Fix
void ScribusView::ToggleAnnotation()
{
	if (Doc->m_Selection->count() != 0)
	{
		for (int a = 0; a < Doc->m_Selection->count(); ++a)
		{
			PageItem* currItem = Doc->m_Selection->itemAt(a);
			if (currItem->asTextFrame())
			{
				bool old = currItem->isBookmark;
				currItem->setIsAnnotation(!currItem->isAnnotation());
				if (currItem->isAnnotation())
				{
					currItem->AutoName = false;
					if (Doc->masterPageMode())
					{
						currItem->annotation().setType(11);
						currItem->annotation().setZiel(0);
						currItem->annotation().setAction("0 0");
					}
					if (old)
						emit DelBM(currItem);
					currItem->isBookmark = false;
				}
			}
		}
		m_ScMW->actionManager->setPDFActions(this);
		emit DocChanged();
	}
}

//CB Unused
// void ScribusView::sentToScrap()
// {
// 	ScriXmlDoc *ss = new ScriXmlDoc();
// 	emit ToScrap(ss->WriteElem(Doc, this, Doc->m_Selection));
// 	delete ss;
// 	ss=NULL;
// }

//CB-->Doc/Fix
void ScribusView::ToBack()
{
	if (Doc->sendItemSelectionToBack())
	{
//		m_ScMW->outlinePalette->BuildTree();
		emit LevelChanged(0);
		emit DocChanged();
		m_canvas->m_viewMode.forceRedraw = true;
		updateContents();
	}
}

//CB-->Doc/Fix
void ScribusView::ToFront()
{
	if (Doc->bringItemSelectionToFront())
	{
//		m_ScMW->outlinePalette->BuildTree();
		emit LevelChanged(Doc->m_Selection->itemAt(0)->ItemNr);
		emit DocChanged();
		m_canvas->m_viewMode.forceRedraw = true;
		updateContents();
	}
}

//CB-->Doc/Fix
void ScribusView::LowerItem()
{
	uint low = Doc->Items->count();
	uint high = 0;
	int d;
	QMap<int, uint> ObjOrder;
	PageItem *currItem;
	PageItem *b2;
	uint docSelectionCount=Doc->m_Selection->count();
	if ((Doc->Items->count() > 1) && (docSelectionCount != 0))
	{
		for (uint c = 0; c < docSelectionCount; ++c)
		{
			currItem = Doc->m_Selection->itemAt(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			low = qMin(currItem->ItemNr, low);
			high = qMax(currItem->ItemNr, high);
		}
		if (low == 0)
			return;
		/*bool wasGUISelection=Doc->m_Selection->isGUISelection();
		if (wasGUISelection)
		{
			Doc->m_Selection->setIsGUISelection(false);
			Doc->m_Selection->disconnectAllItemsFromGUI();
		}*/
		bool wasSignalDelayed = !Doc->m_Selection->signalsDelayed();
		Doc->m_Selection->delaySignalsOn();
		if (!wasSignalDelayed)
			Doc->m_Selection->disconnectAllItemsFromGUI();
		Selection tempSelection(*Doc->m_Selection);
		b2 = Doc->Items->at(high);
		Doc->m_Selection->clear();
		SelectItemNr(low-1, false);
		for (int c = 0; c < Doc->m_Selection->count(); ++c)
		{
			currItem = Doc->m_Selection->itemAt(c);
			ObjOrder.insert(currItem->ItemNr, c);
			d = Doc->Items->indexOf(currItem);
			Doc->Items->takeAt(d);
		}
		d = Doc->Items->indexOf(b2);
		QList<uint> Oindex = ObjOrder.values();
		for (int c = static_cast<int>(Oindex.count()-1); c > -1; c--)
		{
			Doc->Items->insert(d+1, Doc->m_Selection->itemAt(Oindex[c]));
		}
		Doc->m_Selection->clear();
		Doc->renumberItemsInListOrder();
//		m_ScMW->outlinePalette->BuildTree();
		/*if (wasGUISelection)
			tempSelection.setIsGUISelection(true);*/
		*Doc->m_Selection=tempSelection;
		Doc->m_Selection->delaySignalsOff();
		emit LevelChanged(Doc->m_Selection->itemAt(0)->ItemNr);
		emit DocChanged();
		m_canvas->m_viewMode.forceRedraw = true;
		updateContents();
	}
}

//CB-->Doc/Fix
void ScribusView::RaiseItem()
{
	uint low = Doc->Items->count();
	uint high = 0;
	int d;
	QMap<int, uint> ObjOrder;
	PageItem *currItem;
	PageItem *b2;
	uint docSelectionCount=Doc->m_Selection->count();
	if ((Doc->Items->count() > 1) && (docSelectionCount != 0))
	{
		for (uint c = 0; c < docSelectionCount; ++c)
		{
			currItem = Doc->m_Selection->itemAt(c);
			if (currItem->isTableItem && currItem->isSingleSel)
				return;
			low = qMin(currItem->ItemNr, low);
			high = qMax(currItem->ItemNr, high);
		}
		if (high == static_cast<uint>(Doc->Items->count()-1))
			return;
		/*bool wasGUISelection=Doc->m_Selection->isGUISelection();
		if (wasGUISelection)
		{
			Doc->m_Selection->setIsGUISelection(false);
			Doc->m_Selection->disconnectAllItemsFromGUI();
		}*/
		bool wasSignalDelayed = !Doc->m_Selection->signalsDelayed();
		Doc->m_Selection->delaySignalsOn();
		if (!wasSignalDelayed)
			Doc->m_Selection->disconnectAllItemsFromGUI();
		Selection tempSelection(*Doc->m_Selection);
		b2 = Doc->Items->at(low);
		Doc->m_Selection->clear();
		SelectItemNr(high+1, false);
		for (int c = 0; c < Doc->m_Selection->count(); ++c)
		{
			currItem = Doc->m_Selection->itemAt(c);
			ObjOrder.insert(currItem->ItemNr, c);
			d = Doc->Items->indexOf(currItem);
			Doc->Items->takeAt(d);
		}
		QList<uint> Oindex = ObjOrder.values();
		for (int c = 0; c <static_cast<int>(Oindex.count()); ++c)
		{
			d = Doc->Items->indexOf(b2);
			if (d==-1)
				d=0;
			Doc->Items->insert(d, Doc->m_Selection->itemAt(Oindex[c]));
		}
		Doc->m_Selection->clear();
		Doc->renumberItemsInListOrder();
//		m_ScMW->outlinePalette->BuildTree();
		/*if (wasGUISelection)
			tempSelection.setIsGUISelection(true);*/
		*Doc->m_Selection=tempSelection;
		Doc->m_Selection->delaySignalsOff();
		emit LevelChanged(Doc->m_Selection->itemAt(0)->ItemNr);
		emit DocChanged();
		m_canvas->m_viewMode.forceRedraw = true;
		updateContents();
	}
}

 // FIXME:av -> CanvasMode_legacy / Doc
//CB Remove emit/start pasting objects
void ScribusView::PasteToPage()
{
	UndoTransaction* activeTransaction = NULL;
	int ac = Doc->Items->count();
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Doc->currentPage()->getUName(), 0, Um::Paste, "", Um::IPaste));
	if (ScMimeData::clipboardHasScribusFragment())
	{
		bool savedAlignGrid = Doc->useRaster;
		bool savedAlignGuides = Doc->SnapGuides;
		QByteArray fragment   = ScMimeData::clipboardScribusFragment();
		Selection pastedObjects = Serializer(*Doc).deserializeObjects(fragment);
		Doc->useRaster = savedAlignGrid;
		Doc->SnapGuides = savedAlignGuides;
		pastedObjects.setGroupRect();
		double gx, gy, gh, gw;
		pastedObjects.getGroupRect(&gx, &gy, &gw, &gh);
		Doc->moveGroup(dragX - gx, dragY - gy, false, &pastedObjects);
		Doc->m_Selection->clear();
	}
	else
	{
		QString buffer = ScMimeData::clipboardScribusElem();
		emit LoadElem(buffer, dragX, dragY, false, false, Doc, this);
	}
	Doc->DraggedElem = 0;
	Doc->DragElements.clear();
	updateContents();
	Selection newObjects(this, false);
	for (int as = ac; as < Doc->Items->count(); ++as)
	{
		PageItem* currItem = Doc->Items->at(as);
		if (currItem->isBookmark)
			emit AddBM(currItem);
		newObjects.addItem(currItem);
		currItem->LayerNr = Doc->activeLayer();
	}
	if (newObjects.count() > 1)
	{
		newObjects.setGroupRect();
		double gx, gy, gh, gw;
		newObjects.getGroupRect(&gx, &gy, &gw, &gh);
		double nx = gx;
		double ny = gy;
		if (!Doc->ApplyGuides(&nx, &ny))
		{
			FPoint npx;
			npx = Doc->ApplyGridF(FPoint(nx, ny));
			nx = npx.x();
			ny = npx.y();
		}
		Doc->moveGroup(nx-gx, ny-gy, false, &newObjects);
		newObjects.setGroupRect();
		newObjects.getGroupRect(&gx, &gy, &gw, &gh);
		nx = gx+gw;
		ny = gy+gh;
		Doc->ApplyGuides(&nx, &ny);
		Doc->moveGroup(nx-(gx+gw), ny-(gy+gh), false, &newObjects);
		newObjects.setGroupRect();
		newObjects.getGroupRect(&gx, &gy, &gw, &gh);
		emit ItemPos(gx, gy);
		emit ItemGeom(gw, gh);
		emit HaveSel(newObjects.itemAt(0)->itemType());
	}
	else
	{
		Q_ASSERT(newObjects.count()==1);
		PageItem *currItem = newObjects.itemAt(0);
		if (Doc->useRaster)
		{
			double nx = currItem->xPos();
			double ny = currItem->yPos();
			if (!Doc->ApplyGuides(&nx, &ny))
			{
				FPoint npx;
				npx = Doc->ApplyGridF(FPoint(nx, ny));
				nx = npx.x();
				ny = npx.y();
			}
			Doc->MoveItem(nx-currItem->xPos(), ny-currItem->yPos(), currItem);
		}
		currItem->emitAllToGUI();
	}
	newObjects.clear();
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
}

void ScribusView::resizeEvent ( QResizeEvent * event )
{
// 	qDebug() << "ScribusView::resizeEvent";
	QScrollArea::resizeEvent(event);
	horizRuler->setGeometry(m_vhRulerHW, 1, width()-m_vhRulerHW-1, m_vhRulerHW);
	vertRuler->setGeometry(1, m_vhRulerHW, m_vhRulerHW, height()-m_vhRulerHW-1);
	rulerMover->setGeometry(1, 1, m_vhRulerHW, m_vhRulerHW);
	m_canvas->m_viewMode.forceRedraw = true;
	m_canvas->resetRenderMode();
	// Per Qt doc, not painting should be done in a resizeEvent,
	// a paint event will be emitted right afterwards
	// m_canvas->update();
}


/** Waagrechter Scrollbalken */
void ScribusView::setHBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x, y, w, h);
	if (Ready)
		horizRuler->setGeometry(m_vhRulerHW, 1, w-m_vhRulerHW-1, m_vhRulerHW);
}

/** Senkrechter Scrollbalken */
void ScribusView::setVBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x, y, w, h);
	if (Ready)
	{
		vertRuler->setGeometry(1, m_vhRulerHW, m_vhRulerHW, h-m_vhRulerHW-1);
		rulerMover->setGeometry(1, 1, m_vhRulerHW, m_vhRulerHW);
	}
}

bool ScribusView::mousePressed()
{
	return m_canvas->m_viewMode.m_MouseButtonPressed;
}

void ScribusView::resetMousePressed()
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
}


void ScribusView::startGroupTransaction(const QString& action, const QString& description, QPixmap* actionIcon, Selection* customSelection)
{
	Selection* itemSelection = (customSelection!=0) ? customSelection : Doc->m_Selection;
	assert(itemSelection!=0);
	uint selectedItemCount=itemSelection->count();
	Q_ASSERT(selectedItemCount > 0);
	if (!m_groupTransaction)
	{
		QString tooltip = description;
		QString target = Um::SelectionGroup;
		QPixmap* targetIcon = Um::IGroup;
		if (tooltip.isEmpty() && selectedItemCount > 1)
		{
			tooltip = Um::ItemsInvolved + "\n";
			for (uint i = 0; i < selectedItemCount; ++i)
				tooltip += "\t" + itemSelection->itemAt(i)->getUName() + "\n";
		}
		if (selectedItemCount == 1)
		{
			target = itemSelection->itemAt(0)->getUName();
			targetIcon = itemSelection->itemAt(0)->getUPixmap();
		}
		m_groupTransaction = new UndoTransaction(undoManager->beginTransaction(target, targetIcon,
																			   action, tooltip, actionIcon));
	}
	++m_groupTransactions;
}


/**

*/
void ScribusView::endGroupTransaction()
{
	if(m_groupTransactions > 0)
	{
		--m_groupTransactions;
	}
	if (m_groupTransaction && m_groupTransactions == 0)
	{
		m_groupTransaction->commit();
		delete m_groupTransaction;
		m_groupTransaction = NULL;
	}
}

/**
   Always cancels the toplevel transaction and all enclosed ones
 */
void ScribusView::cancelGroupTransaction()
{
	if(m_groupTransaction && m_groupTransactions == 1)
	{
		m_groupTransaction->cancel();
		delete m_groupTransaction;
		m_groupTransaction = NULL;
	}
	else if (m_groupTransaction)
	{
		m_groupTransaction->markFailed();
	}
	if (m_groupTransactions > 0)
		--m_groupTransactions;
}


// jjsa 27-03-2004 add for better setting while zooming
//CB find a new name
void ScribusView::rememberOldZoomLocation(int mx, int my)
{
	oldX = mx;
	oldY = my;
}

void ScribusView::setRulerPos(int x, int y)
{
	if (m_ScMW->ScriptRunning)
		return;
	if (Doc->guidesSettings.rulerMode)
	{
		horizRuler->offs = x / m_canvas->scale() - Doc->currentPage()->xOffset();
		vertRuler->offs = y / m_canvas->scale() - Doc->currentPage()->yOffset();
	}
	else
	{
		horizRuler->offs = x / m_canvas->scale();
		vertRuler->offs = y / m_canvas->scale();
	}
//	horizRuler->offs += qRound(Doc->minCanvasCoordinate.x() - 1 - Doc->rulerXoffset);
//	vertRuler->offs += qRound(Doc->minCanvasCoordinate.y() - 1 - Doc->rulerYoffset);
	horizRuler->offs += 0*Doc->minCanvasCoordinate.x()  - Doc->rulerXoffset;
	vertRuler->offs += 0*Doc->minCanvasCoordinate.y()  - Doc->rulerYoffset;
	horizRuler->repaint();
	vertRuler->repaint();
//	evSpon = true;
	QString newStatusBarText(" ");
	if ((verticalScrollBar()->isSliderDown()) || (horizontalScrollBar()->isSliderDown()))
	{
		QList<int> pag;
		pag.clear();
		uint docPageCount=Doc->Pages->count();
		for (uint a = 0; a < docPageCount; ++a)
		{
			int xs = static_cast<int>(Doc->Pages->at(a)->xOffset() * m_canvas->scale());
			int ys = static_cast<int>(Doc->Pages->at(a)->yOffset() * m_canvas->scale());
			int ws = static_cast<int>(Doc->Pages->at(a)->width() * m_canvas->scale());
			int hs = static_cast<int>(Doc->Pages->at(a)->height() * m_canvas->scale());
			QRect drawRect = QRect(x, y, visibleWidth(), visibleHeight());
//			drawRect.moveBy(qRound(-Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(-Doc->minCanvasCoordinate.y() * m_canvas->scale()));
			if (drawRect.intersects(QRect(xs, ys, ws, hs)))
				pag.append(a+1);
		}
		if (!pag.isEmpty())
			newStatusBarText=( tr("Page %1 to %2").arg(pag.first()).arg(pag.last()));
	}
	m_ScMW->setStatusBarInfoText(newStatusBarText);
}


//CB This MUST now be called AFTER a call to doc->addPage or doc->addMasterPage as it
//does NOT create a page anymore.
Page* ScribusView::addPage(int nr, bool mov)
{
	Page* fe=Doc->Pages->at(nr);
	Q_ASSERT(fe!=0);
	if (fe==0)
		return 0;
	//Note this picks up the new page or master page depending on the mode.
//	reformPages(mov);
	Doc->reformPages(mov);
	setMenTxt(nr);
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	Doc->DragP = false;
	Doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	MidButt = false;
	HaveSelRect = false;
	Magnify = false;
	FirstPoly = true;
	Doc->nodeEdit.EdPoints = true;
	//GroupSel = false;
	DraggedGroup = false;
//FIXME:av	MoveGY = false;
//FIXME:av	MoveGX = false;
	Doc->nodeEdit.isContourLine = false;
	return fe;
}

void ScribusView::reformPages(bool moveObjects)
{
	Doc->reformPages(moveObjects);
	if (!m_ScMW->ScriptRunning)
		setContentsPos(qRound((Doc->currentPage()->xOffset()-10 - 0*Doc->minCanvasCoordinate.x()) * m_canvas->scale()), qRound((Doc->currentPage()->yOffset()-10 - 0*Doc->minCanvasCoordinate.y()) * m_canvas->scale()));
	if (!Doc->isLoading())
	{
		setRulerPos(contentsX(), contentsY());
		setMenTxt(Doc->currentPage()->pageNr());
	}
}

void ScribusView::updatesOn(bool on)
{
	updateOn = on;
	setUpdatesEnabled(on);
	viewport()->setUpdatesEnabled(on);
}


/*!
  paints the canvas inside the box given in canvas coordinates
 */
void ScribusView::updateCanvas(QRectF box)
{
	if (box.isValid())
	{
		QPoint upperLeft = m_canvas->canvasToLocal(box.topLeft());
		QPoint lowerRight = m_canvas->canvasToLocal(box.bottomRight());
		upperLeft.setX(qMax(0, upperLeft.x()-10));
		upperLeft.setY(qMax(0, upperLeft.y()-10));
		lowerRight.setX(qMax(0, lowerRight.x()+10));
		lowerRight.setY(qMax(0, lowerRight.y()+10));
//		qDebug() << "updateCanvas:" << upperLeft << lowerRight;
		m_canvas->update(upperLeft.x(), upperLeft.y(), lowerRight.x()-upperLeft.x(), lowerRight.y()-upperLeft.y());
/*		double scale = m_canvas->scale();
		double x = box.x() * scale;
		double y = box.y() * scale;
		double w = box.width() * scale;
		double h = box.height() * scale;
		double sbx = horizontalScrollBar()->value();
		double sby = verticalScrollBar()->value();
		if (x < sbx)
		{
			w -= (sbx - x);
			x = sbx;
		}
		else
		{
			w += (x - static_cast<int>(x));  // since x will be rounded down below
		}
		if (y < sby)
		{
			h -= (sby - y);
			y = sby;
		}
		else
		{
			h += (y - static_cast<int>(y));  // since y will be rounded down below
		}
		// enlarge the rectangle when rounding to int:
		m_canvas->update(static_cast<int>(x), static_cast<int>(y),
						 qMin(qRound(h + 0.5), viewport()->width()),
						 qMin(qRound(w + 0.5), viewport()->height()));
*/	}
	else
	{
		m_canvas->update(horizontalScrollBar()->value(), verticalScrollBar()->value(), viewport()->width(), viewport()->height());
	}
}


/*!
  Scrolls the canvas so (x,y) becomes the origin
 */
void ScribusView::setCanvasOrigin(double x, double y)
{
	double scale = m_canvas->scale();
	horizontalScrollBar()->setValue(qRound(x * scale));
	verticalScrollBar()->setValue(qRound(y * scale));
	// fix ranges
	QSize maxViewport = maximumViewportSize();
	horizontalScrollBar()->setRange(qRound(Doc->minCanvasCoordinate.x() * scale),
									qRound(Doc->maxCanvasCoordinate.x() * scale) - maxViewport.width());
	verticalScrollBar()->setRange(qRound(Doc->minCanvasCoordinate.y() * scale),
								  qRound(Doc->maxCanvasCoordinate.y() * scale) - maxViewport.height());
}


/*!
  Scrolls the canvas so (x,y) is in the center of the viewport
 */
void ScribusView::setCanvasCenter(double x, double y)
{
	double scale = m_canvas->scale();
	setCanvasOrigin(x - viewport()->width() / scale,
					y - viewport()->height() / scale);
}

/*!
  Scrolls canvas relatively
 */
void ScribusView::scrollCanvasBy(double deltaX, double deltaY)
{
	double scale = m_canvas->scale();
	horizontalScrollBar()->setValue(qRound(horizontalScrollBar()->value() + deltaX * scale));
	verticalScrollBar()->setValue(qRound(verticalScrollBar()->value() + deltaX * scale));
}


/*!
  returns the canvas's origin in canvas coordinates.
  Doc->minCanvasCoordinate <= result <= Doc->maxCanvasCoordinate
 */
FPoint ScribusView::canvasOrigin() const
{
	double scale = m_canvas->scale();
	return FPoint(horizontalScrollBar()->value() / scale, verticalScrollBar()->value() / scale);
}


/*!
  returns the visible part of the canvas in canvas coordinates
 */
QRectF ScribusView::visibleCanvas() const
{
	double scale = m_canvas->scale();
	return QRectF(horizontalScrollBar()->value() / scale,
				  verticalScrollBar()->value() / scale,
				  viewport()->width() / scale,
				  viewport()->height() / scale);
}


/*!
  Legacy.
  Now ignores the parameters and just adjusts the canvas widget according to min/maxCanvasCoordinate
 */
/*
void ScribusView::adjustCanvas(double width, double height, double dX, double dY)
{
	QSize maxViewport = maximumViewportSize();
	double scale = m_canvas->scale();
	horizontalScrollBar()->setRange(qRound(Doc->minCanvasCoordinate.x() * scale),
									qRound(Doc->maxCanvasCoordinate.x() * scale) - maxViewport.width());
	verticalScrollBar()->setRange(qRound(Doc->minCanvasCoordinate.y() * scale),
								  qRound(Doc->maxCanvasCoordinate.y() * scale) - maxViewport.height());
//	qDebug() << "adjustCanvas" << width << height << dX << dY
//		<< "(" << Doc->minCanvasCoordinate.x() << Doc->minCanvasCoordinate.y() << ") - ("
//		<< Doc->maxCanvasCoordinate.x() << Doc->maxCanvasCoordinate.y() << ") @" << scale << maxViewport;
	widget()->resize(qRound((Doc->maxCanvasCoordinate.x()-Doc->minCanvasCoordinate.x())*scale),
				   qRound((Doc->maxCanvasCoordinate.y()-Doc->minCanvasCoordinate.y())*scale));

	if (false && !m_canvas->m_viewMode.operItemMoving)
	{
		bool updback = updateOn;
		updatesOn(false);
		int oldDX = contentsX();
		int oldDY = contentsY();
		int nw = qMax(qRound(width * scale), contentsWidth() + qRound(dX * scale) * 2);
		int nh = qMax(qRound(height * scale), contentsHeight() + qRound(dY * scale) * 2);
		resizeContents(qMax(nw, visibleWidth() + qRound(dX * scale) * 2), qMax(nh, visibleHeight() + qRound(dY * scale) * 2));
		setContentsPos(oldDX + qRound(dX * scale), oldDY + qRound(dY * scale));
//		setRulerPos(contentsX(), contentsY());
		updatesOn(updback);
	}
//	evSpon = false;
}*/

void ScribusView::setMenTxt(int Seite)
{
	if (m_ScMW->ScriptRunning)
		return;
	disconnect(pageSelector, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
	pageSelector->setMaximum(Doc->masterPageMode() ? 1 : Doc->Pages->count());
	if ((!Doc->isLoading()) && (!Doc->masterPageMode()))
		pageSelector->GotoPg(Seite);
	connect(pageSelector, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
}

void ScribusView::setZoom()
{
	int x = qRound(qMax(contentsX() / m_canvas->scale(), 0.0));
	int y = qRound(qMax(contentsY() / m_canvas->scale(), 0.0));
	int w = qRound(qMin(visibleWidth() / m_canvas->scale(), Doc->currentPage()->width()));
	int h = qRound(qMin(visibleHeight() / m_canvas->scale(), Doc->currentPage()->height()));
	rememberOldZoomLocation(w / 2 + x,h / 2 + y);
	zoom(oldX, oldY, zoomSpinBox->value() / 100.0 * Prefs->DisScale, false);
	setFocus();
}

void ScribusView::slotZoom100()
{
	int x = qRound(qMax(contentsX() / m_canvas->scale(), Doc->minCanvasCoordinate.x()));
	int y = qRound(qMax(contentsY() / m_canvas->scale(), Doc->minCanvasCoordinate.y()));
	int w = qRound(qMin(visibleWidth() / m_canvas->scale(), Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
	int h = qRound(qMin(visibleHeight() / m_canvas->scale(), Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
	rememberOldZoomLocation(w / 2 + x,h / 2 + y);
	zoom(oldX, oldY, Prefs->DisScale, false);
}

void ScribusView::slotZoomIn(int mx,int my)
{
	// FIXME : mx and my should really be ScribusView local coordinates or global coordinates
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(qMax(contentsX() / m_canvas->scale(), Doc->minCanvasCoordinate.x()));
		int y = qRound(qMax(contentsY() / m_canvas->scale(), Doc->minCanvasCoordinate.y()));
		int w = qRound(qMin(visibleWidth() / m_canvas->scale(), Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
		int h = qRound(qMin(visibleHeight() / m_canvas->scale(), Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
		rememberOldZoomLocation(w/2+x, h/2+y);
	}
	else
		rememberOldZoomLocation(mx,my);
	double newScale = m_canvas->scale() * static_cast<double>(Doc->toolSettings.magStep)/100.0;
	zoom(oldX, oldY, newScale, true);
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut(int mx,int my)
{
	// FIXME : mx and my should really be ScribusView local coordinates or global coordinates
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(qMax(contentsX() / m_canvas->scale(), Doc->minCanvasCoordinate.x()));
		int y = qRound(qMax(contentsY() / m_canvas->scale(), Doc->minCanvasCoordinate.y()));
		int w = qRound(qMin(visibleWidth() / m_canvas->scale(), Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
		int h = qRound(qMin(visibleHeight() / m_canvas->scale(), Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
		rememberOldZoomLocation(w/2+x, h/2+y);
	}
	else
		rememberOldZoomLocation(mx,my);
	double newScale = m_canvas->scale() / (static_cast<double>(Doc->toolSettings.magStep)/100.0);
	zoom(oldX, oldY, newScale, true);
}

#if 0
FPoint ScribusView::translateToView(double x, double y) // deprecated
{
	return FPoint((x - 0*Doc->minCanvasCoordinate.x()) * m_canvas->scale(), (y - 0*Doc->minCanvasCoordinate.y())* m_canvas->scale());
}

FPoint ScribusView::translateToView(FPoint in) // deprecated
{
	return translateToView(in.x(), in.y());
}

FPoint ScribusView::translateToDoc(double x, double y) // deprecated
{
	return FPoint(x / m_canvas->scale() + 0*Doc->minCanvasCoordinate.x(), y / m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
}

FPoint ScribusView::translateToDoc(FPoint in) // deprecated
{
	return translateToDoc(in.x(), in.y());
}

FPoint ScribusView::translateFromViewport(double x, double y) // deprecated
{
	return FPoint((x + contentsX()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.x(), (y + contentsY()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
}

FPoint ScribusView::translateFromViewport(FPoint in) // deprecated
{
	return translateFromViewport(in.x(), in.y());
}

FPoint ScribusView::translateToViewport(double x, double y) // deprecated
{
	return FPoint((x - 0*Doc->minCanvasCoordinate.x()) * m_canvas->scale() - contentsX(), (y - 0*Doc->minCanvasCoordinate.y())* m_canvas->scale() - contentsY());
}

FPoint ScribusView::translateToViewport(FPoint in) // deprecated
{
	return translateToViewport(in.x(), in.y());
}
#endif


void ScribusView::DrawNew()
{
// 	qDebug("ScribusView::DrawNew");
// 	printBacktrace(24);
	if (m_ScMW->ScriptRunning)
		return;
	m_canvas->m_viewMode.forceRedraw = true;
	m_canvas->resetRenderMode();
	updateContents();
	setRulerPos(contentsX(), contentsY());
	setMenTxt(Doc->currentPage()->pageNr());
	disconnect(zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom()));
	zoomSpinBox->setValue(m_canvas->scale()/Prefs->DisScale*100);
	connect(zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom()));
}

void ScribusView::SetCCPo(double x, double y)
{
	if (m_ScMW->ScriptRunning)
		return;
	QPoint nx = m_canvas->canvasToLocal(FPoint(x, y));
	QSize viewsize = viewport()->size();
//	qDebug() << "setCCPo" << nx << viewsize;
	setContentsPos(nx.x() - viewsize.width() / 2, nx.y() - viewsize.height() / 2);
}

void ScribusView::SetCPo(double x, double y)
{
	if (m_ScMW->ScriptRunning)
		return;
	QPoint nx = m_canvas->canvasToLocal(FPoint(x, y));
	setContentsPos(nx.x(), nx.y());
}

void ScribusView::updateLayerMenu()
{
	disconnect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	layerMenu->clear();
	QStringList newNames;
	Doc->orderedLayerList(&newNames);
	for (QStringList::Iterator it=newNames.begin(); it!=newNames.end(); ++it)
	{
		QPixmap pm(20,15);
		pm.fill(Doc->Layers.layerByName(*it)->markerColor);
		layerMenu->addItem(pm, *it);
	}
	connect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
}

void ScribusView::setLayerMenuText(const QString &layerName)
{
	disconnect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	if (layerMenu->count() != 0)
		setCurrentComboItem(layerMenu, layerName);
	connect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
}

void ScribusView::GotoLa(int l)
{
	int level = Doc->layerCount()-l-1;
	int layerNumber=Doc->layerNumberFromLevel(level);
	if (layerNumber==-1)
		return;
	Doc->setActiveLayer(layerNumber);
	//CB TODO fix this to use view calls after 1.3.2 release
	m_ScMW->changeLayer(Doc->activeLayer());
	emit changeLA(layerNumber);
}

void ScribusView::ChgUnit(int art)
{
	emit changeUN(art);
	unitChange();
	vertRuler->repaint();
	horizRuler->repaint();
}

void ScribusView::changePreviewQuality(int index)
{
	Doc->allItems_ChangePreviewResolution(index);
	DrawNew();
}

void ScribusView::GotoPa(int Seite)
{
	Deselect();
	GotoPage(Seite-1);
	setFocus();
}

void ScribusView::GotoPage(int Seite)
{
	Doc->setCurrentPage(Doc->Pages->at(Seite));
	if (m_ScMW->ScriptRunning)
		return;
	setMenTxt(Seite);
	SetCPo(Doc->currentPage()->xOffset() - 10, Doc->currentPage()->yOffset() - 10);
}

void ScribusView::showMasterPage(int nr)
{
	Deselect(false);
	OldScale = m_canvas->scale();
	Doc->setMasterPageMode(true);
	Doc->setCurrentPage(Doc->Pages->at(nr));
	pageSelector->setEnabled(false);
	updateOn = false;
//	reformPages();
	zoom();
	oldX = qRound(Doc->currentPage()->xOffset()- 10);
	oldY = qRound(Doc->currentPage()->yOffset()- 10);
	SetCPo(Doc->currentPage()->xOffset() - 10, Doc->currentPage()->yOffset() - 10);
	updateOn = true;
	DrawNew();
}

void ScribusView::hideMasterPage()
{
	Deselect(true);
	Doc->setMasterPageMode(false);
//	Doc->currentPage = Doc->Pages->at(0);
	pageSelector->setEnabled(true);
	resizeContents(qRound((Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()) * m_canvas->scale()), qRound((Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()) * m_canvas->scale()));
//	setScale(OldScale);
//	updateOn = false;
//	GotoPage(0);
//	slotDoZoom();
//	oldX = qRound(Doc->currentPage->xOffset()- 10);
//	oldY = qRound(Doc->currentPage->yOffset()- 10);
//	SetCPo(oldX, oldY);
//	updateOn = true;
//	DrawNew();
}

QImage ScribusView::MPageToPixmap(QString name, int maxGr, bool drawFrame)
{
	QImage pm;
	QImage im;
	int Nr = Doc->MasterNames[name];
	int clipx = static_cast<int>(Doc->scratch.Left);
	int clipy = static_cast<int>(Doc->scratch.Top);
	int clipw = qRound(Doc->MasterPages.at(Nr)->width());
	int cliph = qRound(Doc->MasterPages.at(Nr)->height());
	if ((clipw > 0) && (cliph > 0))
	{
		double sca = m_canvas->scale();
		bool frs = Doc->guidesSettings.framesShown;
		double cx = Doc->minCanvasCoordinate.x();
		double cy = Doc->minCanvasCoordinate.y();
		Doc->minCanvasCoordinate = FPoint(0, 0);
		Page* act = Doc->currentPage();
		bool mMode = Doc->masterPageMode();
		Doc->setMasterPageMode(true);
		Doc->setCurrentPage(Doc->MasterPages.at(Nr));
		bool ctrls = Doc->guidesSettings.showControls;
		Doc->guidesSettings.showControls = false;
		Doc->guidesSettings.framesShown = false;
		setScale(1.0);
		m_canvas->m_viewMode.previewMode = true;
		m_canvas->m_viewMode.forceRedraw = true;
		pm = QImage(clipw, cliph, QImage::Format_ARGB32);
		ScPainter *painter = new ScPainter(&pm, pm.width(), pm.height(), 1.0, 0);
		painter->clear(Doc->papColor);
		painter->translate(-clipx, -clipy);
		painter->setLineWidth(1);
		if (drawFrame)
		{
			painter->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			painter->setBrush(Doc->papColor);
			painter->drawRect(clipx, clipy, clipw, cliph);
		}
		painter->beginLayer(1.0, 0);
		m_canvas->DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		painter->endLayer();
		painter->end();
		double sx = pm.width() / static_cast<double>(maxGr);
		double sy = pm.height() / static_cast<double>(maxGr);
		if (sy < sx)
			im = pm.scaled(static_cast<int>(pm.width() / sx), static_cast<int>(pm.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		else
			im = pm.scaled(static_cast<int>(pm.width() / sy), static_cast<int>(pm.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		delete painter;
		painter=NULL;
		m_canvas->m_viewMode.previewMode = false;
		m_canvas->m_viewMode.forceRedraw = false;
		Doc->guidesSettings.framesShown = frs;
		Doc->guidesSettings.showControls = ctrls;
		setScale(sca);
		Doc->setMasterPageMode(mMode);
		Doc->setCurrentPage(act);
		Doc->minCanvasCoordinate = FPoint(cx, cy);
	}
	return im;
}

QImage ScribusView::PageToPixmap(int Nr, int maxGr, bool drawFrame)
{
	QImage im;
	double sx = maxGr / Doc->DocPages.at(Nr)->width();
	double sy = maxGr / Doc->DocPages.at(Nr)->height();
	double sc = qMin(sx, sy);
	int clipx = static_cast<int>(Doc->DocPages.at(Nr)->xOffset() * sc);
	int clipy = static_cast<int>(Doc->DocPages.at(Nr)->yOffset() * sc);
	int clipw = qRound(Doc->DocPages.at(Nr)->width() * sc);
	int cliph = qRound(Doc->DocPages.at(Nr)->height() * sc);
	if ((clipw > 0) && (cliph > 0))
	{
		im = QImage(clipw, cliph, QImage::Format_ARGB32);
		if (!im.isNull())
		{
			double oldScale = m_canvas->scale();
			double cx = Doc->minCanvasCoordinate.x();
			double cy = Doc->minCanvasCoordinate.y();
			Doc->minCanvasCoordinate = FPoint(0, 0);
			bool oldFramesShown  = Doc->guidesSettings.framesShown;
			bool oldShowControls = Doc->guidesSettings.showControls;
			Doc->guidesSettings.framesShown = false;
			Doc->guidesSettings.showControls = false;
			m_canvas->setScale(sc);
			m_canvas->m_viewMode.previewMode = true;
			m_canvas->m_viewMode.forceRedraw = true;
			Page* act = Doc->currentPage();
			bool mMode = Doc->masterPageMode();
			Doc->setMasterPageMode(false);
			Doc->setLoading(true);
			Doc->setCurrentPage(Doc->DocPages.at(Nr));
			ScPainter *painter = new ScPainter(&im, im.width(), im.height(), 1.0, 0);
			painter->clear(Doc->papColor);
			painter->translate(-clipx, -clipy);
			painter->setFillMode(ScPainter::Solid);
			if (drawFrame)
			{
				painter->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				painter->setBrush(Doc->papColor);
				painter->drawRect(clipx, clipy, clipw, cliph);
			}
			painter->beginLayer(1.0, 0);
			painter->setZoomFactor(m_canvas->scale());

			QList<QPair<PageItem*, int> > changedList;
			Page* page = Doc->DocPages.at(Nr);
			PageItem* currItem;
			if (page->FromMaster.count() != 0)
			{
				uint pageFromMasterCount = page->FromMaster.count();
				for (uint a = 0; a < pageFromMasterCount; ++a)
				{
					currItem = page->FromMaster.at(a);
					if (currItem->asImageFrame())
					{
						if (currItem->PicAvail)
						{
							if (currItem->pixm.imgInfo.lowResType != 0)
							{
								changedList.append(qMakePair(currItem, currItem->pixm.imgInfo.lowResType));
								currItem->pixm.imgInfo.lowResType = 0;
								int fho = currItem->imageFlippedH();
								int fvo = currItem->imageFlippedV();
								Doc->loadPict(currItem->Pfile, currItem, true);
								currItem->setImageFlippedH(fho);
								currItem->setImageFlippedV(fvo);
								currItem->AdjustPictScale();
							}
						}
					}
				}
			}
			if (Doc->Items->count() != 0)
			{
				FPoint orig = m_canvas->localToCanvas(QPoint(clipx, clipy));
				QRectF cullingArea = QRectF(orig.x(), orig.y(), qRound(clipw / sc + 0.5), qRound(cliph / sc + 0.5));
				for (int it = 0; it < Doc->Items->count(); ++it)
				{
					currItem = Doc->Items->at(it);
					if (cullingArea.intersects(currItem->getBoundingRect().adjusted(0.0, 0.0, 1.0, 1.0)))
					{
						if (currItem->asImageFrame())
						{
							if (currItem->PicAvail)
							{
								if (currItem->pixm.imgInfo.lowResType != 0)
								{
									changedList.append(qMakePair(currItem, currItem->pixm.imgInfo.lowResType));
									currItem->pixm.imgInfo.lowResType = 0;
									int fho = currItem->imageFlippedH();
									int fvo = currItem->imageFlippedV();
									Doc->loadPict(currItem->Pfile, currItem, true);
									currItem->setImageFlippedH(fho);
									currItem->setImageFlippedV(fvo);
									currItem->AdjustPictScale();
								}
							}
						}
					}
				}
			}

			m_canvas->DrawMasterItems(painter, Doc->DocPages.at(Nr), QRect(clipx, clipy, clipw, cliph));
			m_canvas->DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
			painter->endLayer();
			painter->end();
			delete painter;
			painter=NULL;

			if (changedList.count() != 0)
			{
				QPair<PageItem*, int> itemPair;
				for (int it = 0; it < changedList.count(); it++)
				{
					itemPair = changedList.at(it);
					currItem = itemPair.first;
					currItem->pixm.imgInfo.lowResType = itemPair.second;
					int fho = currItem->imageFlippedH();
					int fvo = currItem->imageFlippedV();
					Doc->loadPict(currItem->Pfile, currItem, true);
					currItem->setImageFlippedH(fho);
					currItem->setImageFlippedV(fvo);
					currItem->AdjustPictScale();
				}
			}

			Doc->guidesSettings.framesShown  = oldFramesShown;
			Doc->guidesSettings.showControls = oldShowControls;
			m_canvas->setScale(oldScale);
			Doc->setMasterPageMode(mMode);
			Doc->setCurrentPage(act);
			Doc->setLoading(false);
			m_canvas->m_viewMode.previewMode = false;
			m_canvas->m_viewMode.forceRedraw = false;
			Doc->minCanvasCoordinate = FPoint(cx, cy);
		}
	}
	return im;
}

void ScribusView::rulerMove(QMouseEvent *m)
{
	QPoint py(viewport()->mapFromGlobal(m->globalPos()));
	int newY = py.y();
	int newX = py.x();
	QPoint out = viewportToContents(py);
	emit MousePos((py.x() + contentsX())/m_canvas->scale(), (py.y() + contentsY())/m_canvas->scale());
	horizRuler->Draw(out.x());
	vertRuler->Draw(out.y());
	m_canvas->newRedrawPolygon() << QPoint(newX, newY);
	updateContents();
	DrHY = newY;
	DrVX = newX;
}

void ScribusView::setNewRulerOrigin(QMouseEvent *m)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	Doc->rulerXoffset = (py.x() + contentsX()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.x();
	Doc->rulerYoffset = (py.y() + contentsY()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.y();
	if (Doc->guidesSettings.rulerMode)
	{
		Doc->rulerXoffset -= Doc->currentPage()->xOffset();
		Doc->rulerYoffset -= Doc->currentPage()->yOffset();
	}
	setRulerPos(contentsX(), contentsY());
	m_canvas->newRedrawPolygon();
	int docSelectionCount=Doc->m_Selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
		{
			double x, y, w, h;
			Doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			emit ItemPos(x, y);
			emit ItemGeom(w, h);
		}
		else
			Doc->m_Selection->itemAt(0)->emitAllToGUI();
	}
}

#if 0
void ScribusView::FromHRuler(QMouseEvent *m)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	int newY = py.y();
	QPoint out = viewportToContents(py);
	emit MousePos((py.x() + contentsX())/m_canvas->scale(), (py.y() + 2 + contentsY())/m_canvas->scale());
	horizRuler->Draw(out.x());
	vertRuler->Draw(out.y() + 2);
	if (!redrawMarker->isVisible())
		redrawMarker->show();
	if (QRect(0, 0, visibleWidth(), visibleHeight()).contains(py))
		redrawMarker->setGeometry(QRect(viewport()->mapToGlobal(QPoint(0, 0)).x(), m->globalPos().y(), visibleWidth(), 1));
	DrHY = newY;
	double newXp = (py.x() + contentsX()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.x();
	double newYp = (py.y() + contentsY()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.y();
	int pg = Doc->OnPage(newXp, newYp);
	if ((pg == -1) || (!QRect(0, 0, visibleWidth(), visibleHeight()).contains(py)))
		qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png")));
	else
		qApp->changeOverrideCursor(QCursor(SPLITHC));
}

void ScribusView::FromVRuler(QMouseEvent *m)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	int newY = py.x();
	QPoint out = viewportToContents(py);
	emit MousePos((py.x() + 2 + contentsX())/m_canvas->scale(), (py.y() + contentsY())/m_canvas->scale());
	horizRuler->Draw(out.x() + 2);
	vertRuler->Draw(out.y());
	if (!redrawMarker->isVisible())
		redrawMarker->show();
	if (QRect(0, 0, visibleWidth(), visibleHeight()).contains(py))
		redrawMarker->setGeometry(QRect(m->globalPos().x(), viewport()->mapToGlobal(QPoint(0, 0)).y(), 1, visibleHeight()));
	DrVX = newY;
	double newXp = (py.x() + contentsX()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.x();
	double newYp = (py.y() + contentsY()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.y();
	int pg = Doc->OnPage(newXp, newYp);
	if ((pg == -1) || (!QRect(0, 0, visibleWidth(), visibleHeight()).contains(py)))
		qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png")));
	else
		qApp->changeOverrideCursor(QCursor(SPLITVC));
}
#endif

#if 0
// TODO: PV - guides refactoring
void ScribusView::SetYGuide(QMouseEvent *m, int oldIndex)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	double newX = (py.x() + contentsX()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.x();
	double newY = (py.y() + contentsY()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.y();
	int pg = Doc->OnPage(newX, newY);
	if ((pg != -1) && (QRect(0, 0, visibleWidth(), visibleHeight()).contains(py)))
	{
		if (oldIndex < 0)
		{
			Doc->Pages->at(pg)->guides.addHorizontal(newY-Doc->Pages->at(pg)->yOffset(), GuideManagerCore::Standard);
			emit signalGuideInformation(0, qRound((newY-Doc->Pages->at(pg)->yOffset()) * 10000.0) / 10000.0);
		}
		else
		{
			if (pg == Doc->currentPageNumber())
			{
				Doc->currentPage()->guides.moveHorizontal( Doc->currentPage()->guides.horizontal(oldIndex, GuideManagerCore::Standard),
																							newY-Doc->currentPage()->yOffset(), GuideManagerCore::Standard);
				emit signalGuideInformation(0, qRound((newY-Doc->Pages->at(pg)->yOffset()) * 10000.0) / 10000.0);
			}
			else
			{
				Doc->currentPage()->guides.deleteHorizontal( Doc->currentPage()->guides.horizontal(oldIndex, GuideManagerCore::Standard), GuideManagerCore::Standard);
				emit signalGuideInformation(-1, 0.0);
				Doc->Pages->at(pg)->guides.addHorizontal(newY-Doc->Pages->at(pg)->yOffset(), GuideManagerCore::Standard);
			}
		}
		m_ScMW->guidePalette->clearRestoreHorizontalList();
	}
	else
	{
		if (oldIndex >= 0)
		{
			Doc->currentPage()->guides.deleteHorizontal( Doc->currentPage()->guides.horizontal(oldIndex, GuideManagerCore::Standard), GuideManagerCore::Standard);
			emit signalGuideInformation(-1, 0.0);
			m_ScMW->guidePalette->clearRestoreHorizontalList();
		}
	}
	emit DocChanged();
/*
	if ((pg != -1) && (pg == Doc->currentPageNumber()) && (QRect(0, 0, visibleWidth(), visibleHeight()).contains(py)))
	{
		if (oldIndex < 0)
			Doc->Pages->at(pg)->guides.addHorizontal(newY-Doc->Pages->at(pg)->yOffset(), GuideManagerCore::Standard);
		else
			Doc->Pages->at(pg)->guides.moveHorizontal(
					Doc->Pages->at(pg)->guides.horizontal(oldIndex, GuideManagerCore::Standard),
					newY-Doc->Pages->at(pg)->yOffset(),
					GuideManagerCore::Standard);
		m_ScMW->guidePalette->clearRestoreHorizontalList();
		emit signalGuideInformation(0, qRound((newY-Doc->Pages->at(pg)->yOffset()) * 10000.0) / 10000.0);
	}
	else if (oldIndex >= 0)
	{
		Doc->currentPage()->guides.deleteHorizontal( Doc->currentPage()->guides.horizontal(oldIndex, GuideManagerCore::Standard), GuideManagerCore::Standard);
		emit signalGuideInformation(-1, 0.0);
		m_ScMW->guidePalette->clearRestoreHorizontalList();
	}
	emit DocChanged();
*/
}

void ScribusView::SetXGuide(QMouseEvent *m, int oldIndex)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	double newX = (py.x() + contentsX()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.x();
	double newY = (py.y() + contentsY()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.y();
	int pg = Doc->OnPage(newX, newY);
	if ((pg != -1) && (QRect(0, 0, visibleWidth(), visibleHeight()).contains(py)))
	{
		if (oldIndex < 0)
		{
			Doc->Pages->at(pg)->guides.addVertical(newX-Doc->Pages->at(pg)->xOffset(), GuideManagerCore::Standard);
			emit signalGuideInformation(0, qRound((newX-Doc->Pages->at(pg)->xOffset()) * 10000.0) / 10000.0);
		}
		else
		{
			if (pg == Doc->currentPageNumber())
			{
				Doc->currentPage()->guides.moveVertical( Doc->currentPage()->guides.vertical(oldIndex, GuideManagerCore::Standard),
																							newX-Doc->currentPage()->xOffset(), GuideManagerCore::Standard);
				emit signalGuideInformation(0, qRound((newX-Doc->Pages->at(pg)->xOffset()) * 10000.0) / 10000.0);
			}
			else
			{
				Doc->currentPage()->guides.deleteVertical( Doc->currentPage()->guides.vertical(oldIndex, GuideManagerCore::Standard), GuideManagerCore::Standard);
				emit signalGuideInformation(-1, 0.0);
				Doc->Pages->at(pg)->guides.addVertical(newX-Doc->Pages->at(pg)->xOffset(), GuideManagerCore::Standard);
			}
		}
		m_ScMW->guidePalette->clearRestoreVerticalList();
	}
	else
	{
		if (oldIndex >= 0)
		{
			Doc->currentPage()->guides.deleteVertical( Doc->currentPage()->guides.vertical(oldIndex, GuideManagerCore::Standard), GuideManagerCore::Standard);
			emit signalGuideInformation(-1, 0.0);
			m_ScMW->guidePalette->clearRestoreVerticalList();
		}
	}
	emit DocChanged();
/*
	if ((pg != -1) && (pg == Doc->currentPageNumber()) && (QRect(0, 0, visibleWidth(), visibleHeight()).contains(py)))
	{
		if (oldIndex < 0)
			Doc->Pages->at(pg)->guides.addVertical(newX-Doc->Pages->at(pg)->xOffset(), GuideManagerCore::Standard);
		else
			Doc->Pages->at(pg)->guides.moveVertical(
					Doc->Pages->at(pg)->guides.vertical(oldIndex, GuideManagerCore::Standard),
					newX-Doc->Pages->at(pg)->xOffset(),
					GuideManagerCore::Standard);
		m_ScMW->guidePalette->clearRestoreVerticalList();
		emit signalGuideInformation(1, qRound((newX-Doc->Pages->at(pg)->xOffset()) * 10000.0) / 10000.0);
	}
	else if (oldIndex >= 0)
	{
		Doc->currentPage()->guides.deleteVertical( Doc->currentPage()->guides.vertical(oldIndex, GuideManagerCore::Standard), GuideManagerCore::Standard);
		emit signalGuideInformation(-1, 0.0);
		m_ScMW->guidePalette->clearRestoreVerticalList();
	}
	emit DocChanged();
*/
}
#endif

//CB-->Doc
void ScribusView::SetFrameRect()
{
	Doc->nodeEdit.deselect();
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		currItem->SetRectFrame();
		Doc->setRedrawBounding(currItem);
		updateContents(currItem->getRedrawBounding(m_canvas->scale()));
	}
}

//CB-->Doc
void ScribusView::SetFrameRounded()
{
	Doc->nodeEdit.deselect();
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		if (currItem->cornerRadius() == 0)
		{
			SetFrameRect();
			return;
		}
		currItem->SetFrameRound();
		Doc->setRedrawBounding(currItem);
		updateContents(currItem->getRedrawBounding(m_canvas->scale()));
	}
}

//CB-->Doc
void ScribusView::SetFrameOval()
{
	Doc->nodeEdit.deselect();
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		currItem->SetOvalFrame();
		Doc->setRedrawBounding(currItem);
		updateContents(currItem->getRedrawBounding(m_canvas->scale()));
	}
}

void ScribusView::editExtendedImageProperties()
{
	if (Doc->m_Selection->count() != 0)
	{
		PageItem *currItem = Doc->m_Selection->itemAt(0);
		if (currItem->pixm.imgInfo.valid)
		{
			ExtImageProps* dia = new ExtImageProps(this, &currItem->pixm.imgInfo, currItem, this);
			dia->exec();
			delete dia;
			dia=NULL;
			m_ScMW->propertiesPalette->setTextFlowMode(currItem->textFlowMode());
		}
	}
}

//CB Stop using this for putting items on pages apart from pasting
//IE write a more generic function in the doc
void ScribusView::PasteItem(struct CopyPasteBuffer *Buffer, bool loading, bool drag, bool resize)
{
	QColor tmp;
	if (!loading)
		Deselect(true);
	double x = Buffer->Xpos;
	double y = Buffer->Ypos;
	double w = Buffer->Width;
	double h = Buffer->Height;
	double pw = Buffer->Pwidth;
	int z = 0;
//	qDebug() << "Pasting frame of type " << Buffer->PType;
	switch (Buffer->PType)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType1:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, true);
		break;
	//
	case PageItem::ImageFrame:
		z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, x, y, w, h, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, true);
		Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
		Doc->Items->at(z)->setImageXYOffset(Buffer->LocalX, Buffer->LocalY);
		Doc->Items->at(z)->Pfile = Buffer->Pfile;
		Doc->Items->at(z)->IProfile = Buffer->IProfile;
		Doc->Items->at(z)->EmProfile = Buffer->EmProfile;
		Doc->Items->at(z)->IRender = Buffer->IRender;
		Doc->Items->at(z)->UseEmbedded = Buffer->UseEmbedded;
		if (!Doc->Items->at(z)->Pfile.isEmpty())
			Doc->LoadPict(Doc->Items->at(z)->Pfile, z);
		Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
		Doc->Items->at(z)->setImageShown(Buffer->PicArt);
/*		Doc->Items->at(z)->BBoxX = Buffer->BBoxX;
		Doc->Items->at(z)->BBoxH = Buffer->BBoxH; */
		Doc->Items->at(z)->ScaleType = Buffer->ScaleType;
		Doc->Items->at(z)->AspectRatio = Buffer->AspectRatio;
		Doc->Items->at(z)->setLineWidth(Buffer->Pwidth);
		break;
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType3:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, true);
		break;
	//
	case PageItem::PathText:
	case PageItem::TextFrame:
#ifndef NLS_PROTO
		if (Buffer->PType == PageItem::PathText)
			z = Doc->itemAdd(PageItem::PathText, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Buffer->Pcolor, true);
		else
			z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Buffer->Pcolor, true);
		if ((Buffer->m_isAnnotation) && (Buffer->m_annotation.UseIcons()))
		{
			Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
			Doc->Items->at(z)->setImageXYOffset(Buffer->LocalX, Buffer->LocalY);
			Doc->Items->at(z)->Pfile = Buffer->Pfile;
			Doc->Items->at(z)->Pfile2 = Buffer->Pfile2;
			Doc->Items->at(z)->Pfile3 = Buffer->Pfile3;
			Doc->Items->at(z)->IProfile = Buffer->IProfile;
			Doc->Items->at(z)->EmProfile = Buffer->EmProfile;
			Doc->Items->at(z)->IRender = Buffer->IRender;
			Doc->Items->at(z)->UseEmbedded = Buffer->UseEmbedded;
			Doc->LoadPict(Doc->Items->at(z)->Pfile, z);
			Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
			Doc->Items->at(z)->setImageShown(Buffer->PicArt);
		}
		if (!Buffer->itemText.isEmpty())
		{
			QTextStream t(&Buffer->itemText, QIODevice::ReadOnly);
			QString cc;
			while (!t.atEnd())
			{
				cc = t.readLine();
				if (cc.isEmpty())
					continue;
				QStringList wt;
				QStringList::Iterator it;
				wt = cc.split("\t", QString::SkipEmptyParts);
				it = wt.begin();
				CharStyle nstyle;
				QString ch = (*it);
				if (ch == QChar(5))
					ch = SpecialChars::PARSEP;
				if (ch == QChar(4))
					ch = SpecialChars::TAB;
				it++;
				nstyle.setFont((*Doc->AllFonts)[*it]);
				it++;
				nstyle.setFontSize(qRound((*it).toDouble() * 10));
				it++;
				nstyle.setFillColor(*it);
				it++;
				nstyle.setTracking((*it).toInt());
				it++;
				nstyle.setFillShade((*it).toInt());
				it++;
				nstyle.setFeatures(static_cast<StyleFlag>(it == wt.end() ? 0 : (*it).toInt()).featureList());
				it++;
				int cab = it == wt.end() ? 0 : (*it).toInt();
				it++;
				nstyle.setStrokeColor(it == wt.end() ? CommonStrings::None : *it);
				it++;
				nstyle.setStrokeShade(it == wt.end() ? 100 : (*it).toInt());
				it++;
				if (it == wt.end())
					nstyle.setScaleH(1000);
				else
					nstyle.setScaleH(qMin(qMax((*it).toInt(), 100), 4000));
				it++;
				if (it == wt.end())
					nstyle.setScaleV(1000);
				else
					nstyle.setScaleV(qMin(qMax((*it).toInt(), 100), 4000));
				it++;
				nstyle.setBaselineOffset(it == wt.end() ? 0 : (*it).toInt());
				it++;
				nstyle.setShadowXOffset(it == wt.end() ? 50 : (*it).toInt());
				it++;
				nstyle.setShadowYOffset(it == wt.end() ? -50 : (*it).toInt());
				it++;
				nstyle.setOutlineWidth(it == wt.end() ? 10 : (*it).toInt());
				it++;
				nstyle.setUnderlineOffset(it == wt.end() ? -1 : (*it).toInt());
				it++;
				nstyle.setUnderlineWidth(it == wt.end() ? -1 : (*it).toInt());
				it++;
				nstyle.setStrikethruOffset(it == wt.end() ? -1 : (*it).toInt());
				it++;
				nstyle.setStrikethruWidth(it == wt.end() ? -1 : (*it).toInt());
				uint pos = Doc->Items->at(z)->itemText.length();
				Doc->Items->at(z)->itemText.insertChars(pos, ch);
				if (ch == SpecialChars::PARSEP && cab > 0) {
					ParagraphStyle pstyle;
					pstyle.setParent(Doc->paragraphStyles()[cab].name());
					Doc->Items->at(z)->itemText.applyStyle(pos, pstyle);
				}
				else {
					Doc->Items->at(z)->itemText.applyCharStyle(pos, 1, nstyle);
				}
			}
		}
		{
			ParagraphStyle pstyle;
			pstyle.setAlignment(static_cast<ParagraphStyle::AlignmentType>(Buffer->textAlignment));
			pstyle.setLineSpacing(Buffer->LineSp);
			pstyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(Buffer->LineSpMode));
			if (Doc->AllFonts->contains(Buffer->IFont))
				pstyle.charStyle().setFont((*Doc->AllFonts)[Buffer->IFont]);
			else
				pstyle.charStyle().setFont((*Doc->AllFonts)[Doc->toolSettings.defFont]);
			pstyle.charStyle().setFontSize(Buffer->ISize);
			pstyle.charStyle().setFillColor(Buffer->TxtFill);
			pstyle.charStyle().setStrokeColor(Buffer->TxtStroke);
			pstyle.charStyle().setFillShade(Buffer->ShTxtFill);
			pstyle.charStyle().setStrokeShade(Buffer->ShTxtStroke);
			pstyle.charStyle().setScaleH(Buffer->TxtScale);
			pstyle.charStyle().setScaleV(Buffer->TxtScaleV);
			pstyle.charStyle().setBaselineOffset(Buffer->TxTBase);
			pstyle.charStyle().setFeatures(StyleFlag(Buffer->TxTStyle).featureList());
			pstyle.charStyle().setShadowXOffset(Buffer->TxtShadowX);
			pstyle.charStyle().setShadowYOffset(Buffer->TxtShadowY);
			pstyle.charStyle().setOutlineWidth(Buffer->TxtOutline);
			pstyle.charStyle().setUnderlineOffset(Buffer->TxtUnderPos);
			pstyle.charStyle().setUnderlineWidth(Buffer->TxtUnderWidth);
			pstyle.charStyle().setStrikethruOffset(Buffer->TxtStrikePos);
			pstyle.charStyle().setStrikethruWidth(Buffer->TxtStrikeWidth);
			Doc->Items->at(z)->itemText.setDefaultStyle(pstyle);
		}
//		Doc->Items->at(z)->convertTo(Buffer->PType);
#endif
		break;
	case PageItem::Line:
		z = Doc->itemAdd(PageItem::Line, PageItem::Unspecified, x, y, w ,0, pw, CommonStrings::None, Buffer->Pcolor2, true);
		break;
	case PageItem::Polygon:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, true);
		break;
	case PageItem::PolyLine:
		z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, true);
		break;
	case PageItem::Multiple:
		Q_ASSERT(false);
		break;
	case PageItem::LatexFrame:
		z = Doc->itemAdd(PageItem::LatexFrame, PageItem::Unspecified, x, y, w, h, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, true);
		Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
		Doc->Items->at(z)->setImageXYOffset(Buffer->LocalX, Buffer->LocalY);
		Doc->Items->at(z)->Pfile = Buffer->Pfile;
		Doc->Items->at(z)->IProfile = Buffer->IProfile;
		Doc->Items->at(z)->EmProfile = Buffer->EmProfile;
		Doc->Items->at(z)->IRender = Buffer->IRender;
		Doc->Items->at(z)->UseEmbedded = Buffer->UseEmbedded;
		if (!Doc->Items->at(z)->Pfile.isEmpty())
			Doc->LoadPict(Doc->Items->at(z)->Pfile, z);
		Doc->Items->at(z)->setImageXYScale(Buffer->LocalScX, Buffer->LocalScY);
		Doc->Items->at(z)->setImageShown(Buffer->PicArt);
		Doc->Items->at(z)->ScaleType = Buffer->ScaleType;
		Doc->Items->at(z)->AspectRatio = Buffer->AspectRatio;
		Doc->Items->at(z)->setLineWidth(Buffer->Pwidth);
		PageItem_LatexFrame *latexframe = Doc->Items->at(z)->asLatexFrame();
		latexframe->setFormula(Buffer->itemText); //itemText seems to be a good choice...
		break;

	}
	PageItem *currItem = Doc->Items->at(z);
/*FIXME
	currItem->setLineSpacingMode(Buffer->LineSpMode);
	if (currItem->lineSpacingMode() == 3)
	{
		currItem->setLineSpacing(Doc->typographicSettings.valueBaseGrid-1);
	}
	*/
	currItem->setImageFlippedH(Buffer->flippedH);
	currItem->setImageFlippedV(Buffer->flippedV);
	currItem->setCornerRadius(Buffer->RadRect);
	currItem->FrameType = Buffer->FrameType;
	currItem->ClipEdited = Buffer->ClipEdited;
	currItem->setFillColor(Buffer->Pcolor);
	currItem->setLineColor(Buffer->Pcolor2);
	currItem->setFillShade(Buffer->Shade);
	currItem->setLineShade(Buffer->Shade2);
	currItem->fillRule = Buffer->FillRule;
/*	currItem->TxtStroke = Buffer->TxtStroke;
	currItem->TxtFill = Buffer->TxtFill;
	currItem->ShTxtStroke = Buffer->ShTxtStroke;
	currItem->ShTxtFill = Buffer->ShTxtFill;
	currItem->TxtScale = Buffer->TxtScale;
	currItem->TxtScaleV = Buffer->TxtScaleV;
	currItem->TxTStyle = Buffer->TxTStyle;
	currItem->TxtShadowX = Buffer->TxtShadowX;
	currItem->TxtShadowY = Buffer->TxtShadowY;
	currItem->TxtOutline = Buffer->TxtOutline;
	currItem->TxtUnderPos = Buffer->TxtUnderPos;
	currItem->TxtUnderWidth = Buffer->TxtUnderWidth;
	currItem->TxtStrikePos = Buffer->TxtStrikePos;
	currItem->TxtStrikeWidth = Buffer->TxtStrikeWidth;
*/
	currItem->setRotation(Buffer->Rot);
	currItem->setTextToFrameDist(Buffer->Extra, Buffer->RExtra, Buffer->TExtra, Buffer->BExtra);
	currItem->PLineArt = Qt::PenStyle(Buffer->PLineArt);
	currItem->PLineEnd = Qt::PenCapStyle(Buffer->PLineEnd);
	currItem->PLineJoin = Qt::PenJoinStyle(Buffer->PLineJoin);
	currItem->setPrintEnabled(Buffer->isPrintable);
	currItem->isBookmark = Buffer->isBookmark;
//	currItem->BMnr = Buffer->BMnr;
	currItem->Groups = Buffer->Groups;
	currItem->setIsAnnotation(Buffer->m_isAnnotation);
	currItem->setAnnotation(Buffer->m_annotation);
	if (!Buffer->AnName.isEmpty())
	{
		if (!drag)
		{
			if (currItem->itemName() == Buffer->AnName)
				currItem->AutoName = true;
			else
			{
				currItem->setItemName(Buffer->AnName);
				currItem->AutoName = false;
			}
		}
		else
		{
//			currItem->setItemName(currItem->generateUniqueCopyName(Buffer->AnName));
			currItem->setItemName(Buffer->AnName);
			currItem->AutoName = false;
		}
	}
	else
	{
		if (currItem->isGroupControl)
			currItem->setItemName( tr("Group%1").arg(currItem->Groups.top()));
	}

	currItem->TopLine = Buffer->TopLine;
	currItem->RightLine = Buffer->RightLine;
	currItem->LeftLine = Buffer->LeftLine;
	currItem->BottomLine = Buffer->BottomLine;
	currItem->isTableItem = Buffer->isTableItem;
	currItem->TopLinkID = Buffer->TopLinkID;
	currItem->LeftLinkID = Buffer->LeftLinkID;
	currItem->RightLinkID = Buffer->RightLinkID;
	currItem->BottomLinkID = Buffer->BottomLinkID;
	currItem->Clip = Buffer->Clip; //irrelevant, overwritten below
	currItem->PoShow = Buffer->PoShow;
	currItem->BaseOffs = Buffer->BaseOffs;
	currItem->textPathFlipped = Buffer->textPathFlipped;
	currItem->textPathType = Buffer->textPathType;
	//currItem->setTextFlowsAroundFrame(Buffer->Textflow);
	//currItem->setTextFlowUsesBoundingBox(Buffer->Textflow2);
	currItem->setTextFlowMode((PageItem::TextFlowMode) Buffer->TextflowMode);
//	currItem->textAlignment = Buffer->textAlignment;
//	currItem->setFont(Buffer->IFont);
//	currItem->setFontSize(Buffer->ISize);
//	currItem->ExtraV = Buffer->ExtraV;
//	currItem->TabValues = Buffer->TabValues;
	currItem->DashValues = Buffer->DashValues;
	currItem->DashOffset = Buffer->DashOffset;
	currItem->setLocked(Buffer->Locked);
	currItem->setSizeLocked(Buffer->LockRes);
	currItem->setFillTransparency(Buffer->Transparency);
	currItem->setLineTransparency(Buffer->TranspStroke);
	currItem->setFillBlendmode(Buffer->TransBlend);
	currItem->setLineBlendmode(Buffer->TransBlendS);
	currItem->setStartArrowIndex(Buffer->startArrowIndex);
	currItem->setEndArrowIndex(Buffer->endArrowIndex);
	currItem->setReversed(Buffer->Reverse);
	currItem->NamedLStyle = Buffer->NamedLStyle;
//	currItem->Language = m_ScMW->GetLang(Buffer->Language);
	currItem->Cols = Buffer->Cols;
	currItem->ColGap = Buffer->ColGap;
	currItem->setFirstLineOffset(Buffer->firstLineOffsetP);
	if (Buffer->LayerNr != -1)
		currItem->LayerNr = Buffer->LayerNr;
	currItem->PoLine = Buffer->PoLine.copy();
	//currItem->setTextFlowUsesContourLine(Buffer->UseContour);
	currItem->setTextFlowMode((PageItem::TextFlowMode) Buffer->TextflowMode);
	if (Buffer->ContourLine.size() == 0)
		currItem->ContourLine = currItem->PoLine.copy();
	else
		currItem->ContourLine = Buffer->ContourLine.copy();
	if (!currItem->asLine())
	{
		// OBSOLETE CR 2005-02-06
		if ((currItem->PoLine.size() == 0) && (currItem->itemType() != PageItem::ItemType1))
			currItem->convertClip();
		else
			//
			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
	}
	else
	{
		int ph = static_cast<int>(qMax(1.0, currItem->lineWidth() / 2.0));
		currItem->Segments.clear();
		currItem->PoLine.resize(0);
		currItem->Clip.setPoints(4, -ph,-ph, static_cast<int>(currItem->width()+ph),-ph,
		                  static_cast<int>(currItem->width()+ph),static_cast<int>(currItem->height()+ph),
		                  -ph,static_cast<int>(currItem->height()+ph));
		currItem->setHeight(1.0);
	}
	// OBSOLETE CR 2005-02-06
	if (currItem->itemType() == PageItem::ItemType1)
	{
		currItem->SetOvalFrame();
		Doc->setRedrawBounding(currItem);
	}
	// OBSOLETE CR 2005-02-06
	if (currItem->itemType() == PageItem::ItemType3)
	{
		if (currItem->cornerRadius() != 0.0)
		{
			Doc->nodeEdit.deselect();
			currItem->SetFrameRound();
			Doc->setRedrawBounding(currItem);
		}
		else
		{
			currItem->SetRectFrame();
			Doc->setRedrawBounding(currItem);
		}
		currItem->ClipEdited = true;
	}
	if (currItem->asImageFrame())
		currItem->AdjustPictScale();
//	if (!(currItem->asTextFrame()) && !(currItem->asPathText()))
//		currItem->setFont(Doc->toolSettings.defFont);
	if (currItem->asPathText())
	{
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		currItem->updatePolyClip();
		currItem->Frame = true;
	}
	if (Buffer->GrType != 0)
	{
		if (Buffer->GrType == 8)
		{
			currItem->setPattern(Buffer->pattern);
			currItem->GrType = Buffer->GrType;
			currItem->setPatternTransform(Buffer->patternScaleX, Buffer->patternScaleY, Buffer->patternOffsetX, Buffer->patternOffsetY, Buffer->patternRotation);
		}
		else
		{
			if ((!Buffer->GrColor.isEmpty()) && (!Buffer->GrColor2.isEmpty()))
			{
				currItem->fill_gradient.clearStops();
				if (Buffer->GrType == 5)
				{
					if ((Buffer->GrColor != CommonStrings::None) && (!Buffer->GrColor.isEmpty()))
						currItem->SetQColor(&tmp, Buffer->GrColor, Buffer->GrShade);
					currItem->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor, Buffer->GrShade);
					if ((Buffer->GrColor2 != CommonStrings::None) && (!Buffer->GrColor2.isEmpty()))
						currItem->SetQColor(&tmp, Buffer->GrColor2, Buffer->GrShade2);
					currItem->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
				}
				else
				{
					if ((Buffer->GrColor2 != CommonStrings::None) && (!Buffer->GrColor2.isEmpty()))
						currItem->SetQColor(&tmp, Buffer->GrColor2, Buffer->GrShade2);
					currItem->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
					if ((Buffer->GrColor != CommonStrings::None) && (!Buffer->GrColor.isEmpty()))
						currItem->SetQColor(&tmp, Buffer->GrColor, Buffer->GrShade);
					currItem->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, Buffer->GrColor, Buffer->GrShade);
				}
			}
			else
				currItem->fill_gradient = Buffer->fill_gradient;
			currItem->GrType = Buffer->GrType;
			currItem->GrStartX = Buffer->GrStartX;
			currItem->GrStartY = Buffer->GrStartY;
			currItem->GrEndX = Buffer->GrEndX;
			currItem->GrEndY = Buffer->GrEndY;
			currItem->updateGradientVectors();
		}
	}
	currItem->setObjectAttributes(&(Buffer->pageItemAttributes));
	if (resize)
		Doc->setRedrawBounding(currItem);
	currItem->OwnPage = Doc->OnPage(currItem);
	if (!loading)
	{
		Doc->m_Selection->addItem(currItem);
		emit HaveSel(currItem->itemType());
		currItem->emitAllToGUI();
		emit DocChanged();
		updateContents();
	}
}

void ScribusView::ToPicFrame()
{
	Selection tempSelection(*Doc->m_Selection);
	Doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	Deselect(true);
	Selection restoreSelection(this);
	Doc->itemSelection_convertItemsTo(PageItem::ImageFrame, &restoreSelection, &tempSelection);
	Doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	Doc->m_Selection->delaySignalsOff();
}

void ScribusView::ToPolyFrame()
{
	Selection tempSelection(*Doc->m_Selection);
	Doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	Deselect(true);
	Selection restoreSelection(this);
	Doc->itemSelection_convertItemsTo(PageItem::Polygon, &restoreSelection, &tempSelection);
	Doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	Doc->m_Selection->delaySignalsOff();
}

void ScribusView::ToTextFrame()
{
	Selection tempSelection(*Doc->m_Selection);
	Doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	Deselect(true);
	Selection restoreSelection(this);
	Doc->itemSelection_convertItemsTo(PageItem::TextFrame, &restoreSelection, &tempSelection);
	Doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	Doc->m_Selection->delaySignalsOff();
}

void ScribusView::ToBezierFrame()
{
	Selection tempSelection(*Doc->m_Selection);
	Doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	Deselect(true);
	Selection restoreSelection(this);
	Doc->itemSelection_convertItemsTo(PageItem::PolyLine, &restoreSelection, &tempSelection);
	Doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	Doc->m_Selection->delaySignalsOff();
}

void ScribusView::Bezier2Poly()
{
	Selection tempSelection(*Doc->m_Selection);
	Doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	Deselect(true);
	Selection restoreSelection(this);
	Doc->itemSelection_convertItemsTo(PageItem::Polygon, &restoreSelection, &tempSelection);
	Doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	Doc->m_Selection->delaySignalsOff();
}

void ScribusView::ToPathText()
{
	if (Doc->m_Selection->count() > 1)
	{
		PageItem* currItem=Doc->m_Selection->itemAt(0);
		PageItem *polyLineItem;
		if (currItem->asTextFrame())
			polyLineItem=Doc->m_Selection->itemAt(1);
		else
		{
			polyLineItem=Doc->m_Selection->itemAt(0);
			currItem=Doc->m_Selection->itemAt(1);
		}
		ParagraphStyle dstyle(currItem->itemText.defaultStyle());
		if (polyLineItem->asPolyLine() || polyLineItem->asPolygon())
		{
			Deselect(true);
			PageItem* newItem=Doc->convertItemTo(currItem, PageItem::PathText, polyLineItem);
			newItem->itemText.setDefaultStyle(dstyle);
			newItem->itemText.applyCharStyle(0, newItem->itemText.length(), dstyle.charStyle());
			newItem->invalid = true;
			newItem->update();
			SelectItem(newItem);
			emit DocChanged();
		}
	}
}

void ScribusView::FromPathText()
{
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		Deselect(true);
		PageItem* newItem=Doc->convertItemTo(currItem, PageItem::TextFrame);
		SelectItem(newItem);
		ToFront();
		update();
	}
}

void ScribusView::TextToPath()
{
#ifndef NLS_PROTO
	if (Doc->appMode == modeEditClip)
		requestMode(submodeEndNodeEdit);
	Selection tmpSelection(this, false);
	tmpSelection.copy(*Doc->m_Selection, false);
	PageItem *currItem = tmpSelection.itemAt(0);
	if ((currItem->prevInChain() != 0) || (currItem->nextInChain() != 0))
	{
		// select whole chain
		PageItem *backItem = currItem;
		while (backItem->prevInChain() != 0)
			backItem = backItem->prevInChain();
		currItem = backItem;
		Deselect(true);
		tmpSelection.addItem(currItem);
		backItem = currItem->nextInChain();
		while (backItem != 0)
		{
			tmpSelection.addItem(backItem);
			if (backItem->nextInChain() != 0)
				backItem = backItem->nextInChain();
			else
				break;
		}
	}
	QList<PageItem*> delItems,newGroupedItems;
	newGroupedItems.clear();
	uint selectedItemCount = tmpSelection.count();
	if (selectedItemCount != 0)
	{
		UndoTransaction trans(undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(), Um::ToOutlines, "", 0));
		uint offset=0;
		for(uint i=0; i<selectedItemCount; ++i)
		{
			PageItem *currItem = tmpSelection.itemAt(offset);
			bool cont=false;
			if ((!((currItem->asTextFrame()) || (currItem->asPathText()))) || (currItem->isTableItem && currItem->isSingleSel) || (currItem->locked()) || currItem->itemText.length() == 0)
				cont=true;
			if (currItem==m_ScMW->storyEditor->currentItem() && Doc==m_ScMW->storyEditor->currentDocument())
			{
				QMessageBox::information(m_ScMW, tr("Cannot Convert In-Use Item"), "<qt>" + tr("The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped").arg(currItem->itemName()) + "</qt>", QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
				cont=true;
			}
			//Deselect();
			if (cont)
			{
				++offset;
				continue;
			}
//			newGroupedItems.clear();
			FPointArray pts;
			double x, y, wide;
			QString chstr, ccounter;
			PageItem* bb;

			if (currItem->asPathText())
			{
				for (int a = 0; a < currItem->itemText.length(); ++a)
				{
					pts.resize(0);
					x = 0.0;
					y = 0.0;
					ScText * hl = currItem->itemText.item(a);
					const CharStyle& charStyle(currItem->itemText.charStyle(a));
					chstr = currItem->itemText.text(a,1);
					if ((chstr == QChar(13)) || (chstr == QChar(29)))
						continue;
					if (chstr == QChar(30))
						chstr = currItem->ExpandToken(a);
					double chs = charStyle.fontSize();
					if (hl->effects() & ScStyle_SmallCaps)
					{
						if (chstr.toUpper() != chstr)
						{
							chs = qMax(static_cast<int>(hl->fontSize() * Doc->typographicSettings.valueSmallCaps / 100), 1);
							chstr = chstr.toUpper();
						}
					}
					else if (hl->effects() & ScStyle_AllCaps)
						chstr = chstr.toUpper();
//					double csi = static_cast<double>(chs) / 100.0;
					uint chr = chstr[0].unicode();
					QPointF tangt = QPointF( cos(hl->PRot), sin(hl->PRot) );
					QMatrix chma, chma2, chma3, chma4, chma6;
					QMatrix trafo = QMatrix( 1, 0, 0, -1, -hl->PDx, 0 );
					if (currItem->textPathFlipped)
						trafo *= QMatrix(1, 0, 0, -1, 0, 0);
					if (currItem->textPathType == 0)
						trafo *= QMatrix( tangt.x(), tangt.y(), tangt.y(), -tangt.x(), hl->PtransX, hl->PtransY );
					else if (currItem->textPathType == 1)
						trafo *= QMatrix(1, 0, 0, -1, hl->PtransX, hl->PtransY );
					else if (currItem->textPathType == 2)
					{
						double a = 1;
						double b = -1;
						if (tangt.x() < 0)
						{
							a = -1;
							b = 1;
						}
						if (fabs(tangt.x()) > 0.1)
							trafo *= QMatrix( a, (tangt.y() / tangt.x()) * b, 0, -1, hl->PtransX, hl->PtransY ); // ID's Skew mode
						else
							trafo *= QMatrix( a, 6 * b, 0, -1, hl->PtransX, hl->PtransY );
					}
					//trafo *= QMatrix( hl->PtransX, hl->PtransY, hl->PtransY, -hl->PtransX, hl->glyph.xoffset, hl->glyph.yoffset);
					if (currItem->rotation() != 0)
					{
						QMatrix sca;
						sca.translate(-currItem->xPos(), -currItem->yPos());
						sca.rotate(currItem->rotation());
						trafo *= sca;
					}
					chma.scale(hl->glyph.scaleH * charStyle.fontSize() / 100.00, hl->glyph.scaleV * charStyle.fontSize() / 100.0);
					if (currItem->reversed())
					{
						if (a < currItem->itemText.length()-1)
							wide = hl->font().charWidth(chstr[0], hl->fontSize(), currItem->itemText.text(a+1));
						else
							wide = hl->font().charWidth(chstr[0], hl->fontSize());
						chma3.scale(-1, 1);
						chma3.translate(-wide, 0);
					}
					chma4.translate(0, currItem->BaseOffs - (charStyle.fontSize() / 10.0) * hl->glyph.scaleV);
					if (charStyle.effects() & (ScStyle_Subscript | ScStyle_Superscript))
						chma6.translate(0, hl->glyph.yoffset);
					if (hl->baselineOffset() != 0)
						chma6.translate(0, (-charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0));
					uint gl = hl->font().char2CMap(chr);
					QMatrix finalMat = QMatrix(chma * chma2 * chma3 * chma4 * chma6 * trafo);
					if (currItem->rotation() != 0)
					{
						QMatrix sca;
						sca.translate(currItem->xPos(), currItem->yPos());
						pts.map(sca);
					}
					QChar chstc = hl->ch;
					if (((charStyle.effects() & ScStyle_Underline) && !SpecialChars::isBreak(chstc))
						|| ((charStyle.effects() & ScStyle_UnderlineWords) && !chstc.isSpace() && !SpecialChars::isBreak(chstc)))
					{
						QMatrix stro = QMatrix(chma2 * chma3 * chma6 * trafo);
						if (currItem->rotation() != 0)
						{
							QMatrix sca;
							sca.translate(currItem->xPos(), currItem->yPos());
							stro *= sca;
						}
						double Ulen = hl->glyph.xadvance;
						double Upos, Uwid, kern;
						if (hl->effects() & ScStyle_StartOfLine)
							kern = 0;
						else
							kern = charStyle.fontSize() * charStyle.tracking() / 10000.0;
						if ((charStyle.underlineOffset() != -1) || (charStyle.underlineWidth() != -1))
						{
							if (charStyle.underlineOffset() != -1)
								Upos = (charStyle.underlineOffset() / 1000.0) * (charStyle.font().descent(charStyle.fontSize() / 10.0));
							else
								Upos = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
							if (charStyle.underlineWidth() != -1)
								Uwid = (charStyle.underlineWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
							else
								Uwid = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
						}
						else
						{
							Upos = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
							Uwid = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
						}
						if (charStyle.baselineOffset() != 0)
							Upos += (charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0);
						uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), true);
						bb = Doc->Items->at(z);
						bb->setTextFlowMode(currItem->textFlowMode());
						bb->setSizeLocked(currItem->sizeLocked());
						bb->setLocked(currItem->locked());
						bb->NamedLStyle = currItem->NamedLStyle;
						bb->setItemName(currItem->itemName()+"+U"+ccounter.setNum(a));
						bb->AutoName = false;
						FPoint start, stop;
						if (charStyle.effects() & ScStyle_Subscript)
						{
							start = FPoint(hl->glyph.xoffset-kern, -Upos);
							stop = FPoint(hl->glyph.xoffset+Ulen, -Upos);
						}
						else
						{
							start = FPoint(hl->glyph.xoffset-kern, -(Upos + hl->glyph.yoffset));
							stop = FPoint(hl->glyph.xoffset+Ulen, -(Upos + hl->glyph.yoffset));
						}
						bb->PoLine.resize(0);
						bb->PoLine.addQuadPoint(start, start, stop, stop);
						bb->PoLine.map(stro);
						bb->setLineColor(hl->fillColor());
						bb->setLineShade(hl->fillShade());
						bb->setLineWidth(Uwid);
						Doc->AdjustItemSize(bb);
						bb->ContourLine = bb->PoLine.copy();
						bb->ClipEdited = true;
						Doc->setRedrawBounding(bb);
						newGroupedItems.append(Doc->Items->takeAt(z));
					}
					if (chstr != QChar(32))
					{
						pts = hl->font().glyphOutline(gl);
						if (pts.size() < 4)
							continue;
						if ((charStyle.effects() & ScStyle_Shadowed) && (charStyle.strokeColor() != CommonStrings::None))
						{
							double glxTr = charStyle.fontSize() * charStyle.shadowXOffset() / 10000.0;
							double glyTr = -charStyle.fontSize() * charStyle.shadowYOffset() / 10000.0;
							uint z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), true);
							bb = Doc->Items->at(z);
							bb->setTextFlowMode(currItem->textFlowMode());
							bb->setSizeLocked(currItem->sizeLocked());
							bb->setLocked(currItem->locked());
							bb->NamedLStyle = currItem->NamedLStyle;
							bb->setItemName(currItem->itemName()+"+Sh"+ccounter.setNum(a));
							bb->AutoName = false;
							bb->PoLine = pts.copy();
							QMatrix shmap;
							shmap.translate(glxTr, glyTr);
							bb->PoLine.map(finalMat * shmap);
							bb->setFillColor(hl->strokeColor());
							bb->setFillShade(hl->strokeShade());
							if (currItem->itemText.charStyle(a).effects() & ScStyle_Outline)
							{
								bb->setLineColor(hl->strokeColor());
								bb->setLineShade(hl->strokeShade());
							}
							else
							{
								bb->setLineColor(CommonStrings::None);
								bb->setLineShade(100);
							}
							bb->setLineWidth(chs * hl->outlineWidth() / 10000.0);
							Doc->AdjustItemSize(bb);
							bb->ContourLine = bb->PoLine.copy();
							bb->ClipEdited = true;
							Doc->setRedrawBounding(bb);
							newGroupedItems.append(Doc->Items->takeAt(z));
						}
						pts.map(finalMat);
						uint z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), true);
						bb = Doc->Items->at(z);
						//bb->setTextFlowsAroundFrame(currItem->textFlowsAroundFrame());
						//bb->setTextFlowUsesBoundingBox(currItem->textFlowUsesBoundingBox());
						bb->setTextFlowMode(currItem->textFlowMode());
						bb->setSizeLocked(currItem->sizeLocked());
						bb->setLocked(currItem->locked());
						bb->NamedLStyle = currItem->NamedLStyle;
						bb->setItemName(currItem->itemName()+"+"+ccounter.setNum(a));
						bb->AutoName = false;
						bb->PoLine = pts.copy();
						if (!currItem->asPathText())
							bb->setRotation(currItem->rotation());
						bb->setFillColor(hl->fillColor());
						bb->setFillShade(hl->fillShade());
						if (currItem->itemText.charStyle(a).effects() & ScStyle_Outline)
						{
							bb->setLineColor(hl->strokeColor());
							bb->setLineShade(hl->strokeShade());
						}
						else
						{
							bb->setLineColor(CommonStrings::None);
							bb->setLineShade(100);
						}
						bb->setLineWidth(chs * hl->outlineWidth() / 10000.0);
						Doc->AdjustItemSize(bb);
						bb->ContourLine = bb->PoLine.copy();
						bb->ClipEdited = true;
						Doc->setRedrawBounding(bb);
						newGroupedItems.append(Doc->Items->takeAt(z));
					}
					if (charStyle.effects() & ScStyle_Strikethrough)
					{
						QMatrix stro = QMatrix(chma2 * chma3 * chma6 * trafo);
						if (currItem->rotation() != 0)
						{
							QMatrix sca;
							sca.translate(currItem->xPos(), currItem->yPos());
							stro *= sca;
						}
						double Ulen = hl->glyph.xadvance;
						double Upos, Uwid, kern;
						if (hl->effects() & ScStyle_StartOfLine)
							kern = 0;
						else
							kern = charStyle.fontSize() * charStyle.tracking() / 10000.0;
						if ((charStyle.strikethruOffset() != -1) || (charStyle.strikethruWidth() != -1))
						{
							if (charStyle.strikethruOffset() != -1)
								Upos = (charStyle.strikethruOffset() / 1000.0) * (charStyle.font().ascent(charStyle.fontSize() / 10.0));
							else
								Upos = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
							if (charStyle.strikethruWidth() != -1)
								Uwid = (charStyle.strikethruWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
							else
								Uwid = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
						}
						else
						{
							Upos = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
							Uwid = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
						}
						if (charStyle.baselineOffset() != 0)
							Upos += (charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0);
						uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), true);
						bb = Doc->Items->at(z);
						bb->setTextFlowMode(currItem->textFlowMode());
						bb->setSizeLocked(currItem->sizeLocked());
						bb->setLocked(currItem->locked());
						bb->NamedLStyle = currItem->NamedLStyle;
						bb->setItemName(currItem->itemName()+"+S"+ccounter.setNum(a));
						bb->AutoName = false;
						FPoint start = FPoint(hl->glyph.xoffset-kern, -Upos);
						FPoint stop = FPoint(hl->glyph.xoffset+Ulen, -Upos);
						bb->PoLine.resize(0);
						bb->PoLine.addQuadPoint(start, start, stop, stop);
						bb->PoLine.map(stro);
						bb->setLineColor(hl->fillColor());
						bb->setLineShade(hl->fillShade());
						bb->setLineWidth(Uwid);
						Doc->AdjustItemSize(bb);
						bb->ContourLine = bb->PoLine.copy();
						bb->ClipEdited = true;
						Doc->setRedrawBounding(bb);
						bb->setItemName(currItem->itemName()+"+S"+ccounter.setNum(a));
						newGroupedItems.append(Doc->Items->takeAt(z));
					}
				}
			}
			else
			{
				for (uint ll=0; ll < currItem->itemText.lines(); ++ll)
				{
					LineSpec ls = currItem->itemText.line(ll);
					double CurX = ls.x;
					for (int a = ls.firstItem; a <= ls.lastItem; ++a)
					{
						pts.resize(0);
						x = 0.0;
						y = 0.0;
						ScText * hl = currItem->itemText.item(a);
						const CharStyle& charStyle(currItem->itemText.charStyle(a));
						chstr = currItem->itemText.text(a,1);
						if ((chstr == QChar(13)) || (chstr == QChar(29)))
						{
							if (chstr == QChar(29))
								CurX += hl->glyph.wide();
							continue;
						}
						if (chstr == QChar(30))
							chstr = currItem->ExpandToken(a);
						double chs = charStyle.fontSize();
						if (hl->effects() & ScStyle_SmallCaps)
						{
							if (chstr.toUpper() != chstr)
							{
								chs = qMax(static_cast<int>(hl->fontSize() * Doc->typographicSettings.valueSmallCaps / 100), 1);
								chstr = chstr.toUpper();
							}
						}
						else if (hl->effects() & ScStyle_AllCaps)
							chstr = chstr.toUpper();
						double csi = static_cast<double>(chs) / 100.0;
						uint chr = chstr[0].unicode();
						QMatrix chma, chma2, chma3, chma4, chma6;
						uint gl = hl->font().char2CMap(chr);
						FPoint origin = hl->font().glyphOrigin(gl);
						x = origin.x() * csi;
						y = origin.y() * csi;
						if ((charStyle.effects() & ScStyle_Underline)
											   || ((charStyle.effects() & ScStyle_UnderlineWords)
											   // Qt4 added toInt() ???
											   && chstr.toUInt() != charStyle.font().char2CMap(QChar(' '))))
						{
							double st, lw;
							if ((charStyle.underlineOffset() != -1) || (charStyle.underlineWidth() != -1))
							{
								if (charStyle.underlineOffset() != -1)
									st = (charStyle.underlineOffset() / 1000.0) * (charStyle.font().descent(charStyle.fontSize() / 10.0));
								else
									st = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
								if (charStyle.underlineWidth() != -1)
									lw = (charStyle.underlineWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
								else
									lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
							}
							else
							{
								st = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
								lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
							}
							if (charStyle.baselineOffset() != 0)
								st += (charStyle.fontSize() / 10.0) * hl->glyph.scaleV * (charStyle.baselineOffset() / 1000.0);
							uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), true);
							bb = Doc->Items->at(z);
							bb->setTextFlowMode(currItem->textFlowMode());
							bb->setSizeLocked(currItem->sizeLocked());
							bb->setLocked(currItem->locked());
							bb->NamedLStyle = currItem->NamedLStyle;
							bb->setItemName(currItem->itemName()+"+U"+ccounter.setNum(a));
							bb->AutoName = false;
							bb->setRotation(currItem->rotation());
							bb->PoLine.addQuadPoint(FPoint(0, 0), FPoint(0, 0), FPoint(hl->glyph.xadvance, 0), FPoint(hl->glyph.xadvance, 0));
							bb->setLineColor(hl->fillColor());
							bb->setLineShade(hl->fillShade());
							bb->setLineWidth(lw);
							FPoint tp2(getMinClipF(&bb->PoLine));
							bb->PoLine.translate(-tp2.x(), -tp2.y());
							FPoint tp(getMaxClipF(&bb->PoLine));
							bb->setWidthHeight(tp.x(), tp.y());
							bb->Clip = FlattenPath(bb->PoLine, bb->Segments);
							double textX = CurX;
							double textY = ls.y - st;  // + hl->glyph.yoffset;
							if (charStyle.effects() & ScStyle_Subscript)
								textY += hl->glyph.yoffset;
							if (charStyle.baselineOffset() != 0)
								textY -= (charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0);
							if (a < currItem->itemText.length()-1)
								wide = hl->font().charWidth(chstr[0], charStyle.fontSize(), currItem->itemText.text(a+1));
							else
								wide = hl->font().charWidth(chstr[0], charStyle.fontSize());
							if (currItem->imageFlippedH())
								textX = currItem->width() - textX - wide;
							if (currItem->imageFlippedV())
								textY = currItem->height() - textY + y - (bb->height() - y);
							FPoint npo(textX, textY, 0.0, 0.0, currItem->rotation(), 1.0, 1.0);
							bb->moveBy(npo.x(),npo.y());
							bb->ContourLine = bb->PoLine.copy();
							bb->ClipEdited = true;
							bb->FrameType = 3;
							Doc->setRedrawBounding(bb);
							newGroupedItems.append(Doc->Items->takeAt(z));
						}
						if (chstr != QChar(32))
						{
							pts = hl->font().glyphOutline(gl);
							if (pts.size() < 4)
								continue;
							chma = QMatrix();
							chma.scale(hl->glyph.scaleH * charStyle.fontSize() / 100.00, hl->glyph.scaleV * charStyle.fontSize() / 100.0);
							pts.map(chma);
							chma = QMatrix();
							if (currItem->imageFlippedH() && (!currItem->reversed()))
								chma.scale(-1, 1);
							if (currItem->imageFlippedV())
								chma.scale(1, -1);
							pts.map(chma);
							if ((charStyle.effects() & ScStyle_Shadowed) && (charStyle.strokeColor() != CommonStrings::None))
							{
								double glxTr = charStyle.fontSize() * charStyle.shadowXOffset() / 10000.0;
								double glyTr = -charStyle.fontSize() * charStyle.shadowYOffset() / 10000.0;
								uint z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos() + glxTr, currItem->yPos() + glyTr, currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), true);
								bb = Doc->Items->at(z);
								bb->setTextFlowMode(currItem->textFlowMode());
								bb->setSizeLocked(currItem->sizeLocked());
								bb->setLocked(currItem->locked());
								bb->NamedLStyle = currItem->NamedLStyle;
								bb->setItemName(currItem->itemName()+"+Sh"+ccounter.setNum(a));
								bb->AutoName = false;
								bb->PoLine = pts.copy();
								bb->setRotation(currItem->rotation());
								bb->setFillColor(hl->strokeColor());
								bb->setFillShade(hl->strokeShade());
								bb->setLineColor(CommonStrings::None);
								bb->setLineShade(100);
								bb->setLineWidth(chs * hl->outlineWidth() / 10000.0);
								FPoint tp2(getMinClipF(&bb->PoLine));
								bb->PoLine.translate(-tp2.x(), -tp2.y());
								FPoint tp(getMaxClipF(&bb->PoLine));
								bb->setWidthHeight(tp.x(), tp.y());
								bb->Clip = FlattenPath(bb->PoLine, bb->Segments);
								double textX = CurX + hl->glyph.xoffset;
								double textY = ls.y;  // + hl->glyph.yoffset;
								if (charStyle.effects() & (ScStyle_Subscript | ScStyle_Superscript))
								{
									textY += hl->glyph.yoffset;
									x *= hl->glyph.scaleH;
									y *= hl->glyph.scaleV;
								}
								chma6 = QMatrix();
								if (charStyle.baselineOffset() != 0)
									textY -= (charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0);
								if (a < currItem->itemText.length()-1)
									wide = hl->font().charWidth(chstr[0], charStyle.fontSize(), currItem->itemText.text(a+1));
								else
									wide = hl->font().charWidth(chstr[0], charStyle.fontSize());
								if (currItem->imageFlippedH())
									textX = currItem->width() - textX - wide;
								if (currItem->imageFlippedV())
									textY = currItem->height() - textY + y - (bb->height() - y);
								FPoint npo(textX+x, textY-y, 0.0, 0.0, currItem->rotation(), 1.0, 1.0);
								bb->moveBy(npo.x(),npo.y());
								bb->ContourLine = bb->PoLine.copy();
								bb->ClipEdited = true;
								Doc->setRedrawBounding(bb);
								newGroupedItems.append(Doc->Items->takeAt(z));
							}
							uint z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), true);
							bb = Doc->Items->at(z);
							//bb->setTextFlowsAroundFrame(currItem->textFlowsAroundFrame());
							//bb->setTextFlowUsesBoundingBox(currItem->textFlowUsesBoundingBox());
							bb->setTextFlowMode(currItem->textFlowMode());
							bb->setSizeLocked(currItem->sizeLocked());
							bb->setLocked(currItem->locked());
							bb->NamedLStyle = currItem->NamedLStyle;
							bb->setItemName(currItem->itemName()+"+"+ccounter.setNum(a));
							bb->AutoName = false;
							bb->PoLine = pts.copy();
							bb->setRotation(currItem->rotation());
							bb->setFillColor(hl->fillColor());
							bb->setFillShade(hl->fillShade());
							if (currItem->itemText.charStyle(a).effects() & ScStyle_Outline)
							{
								bb->setLineColor(hl->strokeColor());
								bb->setLineShade(hl->strokeShade());
							}
							else
							{
								bb->setLineColor(CommonStrings::None);
								bb->setLineShade(100);
							}
							bb->setLineWidth(chs * hl->outlineWidth() / 10000.0);
							FPoint tp2(getMinClipF(&bb->PoLine));
							bb->PoLine.translate(-tp2.x(), -tp2.y());
							FPoint tp(getMaxClipF(&bb->PoLine));
							bb->setWidthHeight(tp.x(), tp.y());
							bb->Clip = FlattenPath(bb->PoLine, bb->Segments);
							double textX = CurX + hl->glyph.xoffset;
							double textY = ls.y;  // + hl->glyph.yoffset;
							if (charStyle.effects() & (ScStyle_Subscript | ScStyle_Superscript))
							{
								textY += hl->glyph.yoffset;
								x *= hl->glyph.scaleH;
								y *= hl->glyph.scaleV;
							}
							chma6 = QMatrix();
							if (charStyle.baselineOffset() != 0)
								textY -= (charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0);
							if (a < currItem->itemText.length()-1)
								wide = hl->font().charWidth(chstr[0], charStyle.fontSize(), currItem->itemText.text(a+1));
							else
								wide = hl->font().charWidth(chstr[0], charStyle.fontSize());
							if (currItem->imageFlippedH())
								textX = currItem->width() - textX - wide;
							if (currItem->imageFlippedV())
								textY = currItem->height() - textY + y - (bb->height() - y);
							FPoint npo(textX+x, textY-y, 0.0, 0.0, currItem->rotation(), 1.0, 1.0);
							bb->moveBy(npo.x(),npo.y());
							bb->ContourLine = bb->PoLine.copy();
							bb->ClipEdited = true;
							Doc->setRedrawBounding(bb);
							newGroupedItems.append(Doc->Items->takeAt(z));
						}
						if (charStyle.effects() & ScStyle_Strikethrough)
						{
							double st, lw;
							if ((charStyle.strikethruOffset() != -1) || (charStyle.strikethruWidth() != -1))
							{
								if (charStyle.strikethruOffset() != -1)
									st = (charStyle.strikethruOffset() / 1000.0) * (charStyle.font().ascent(charStyle.fontSize() / 10.0));
								else
									st = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
								if (charStyle.strikethruWidth() != -1)
									lw = (charStyle.strikethruWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
								else
									lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
							}
							else
							{
								st = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
								lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
							}
							if (charStyle.baselineOffset() != 0)
								st += (charStyle.fontSize() / 10.0) * hl->glyph.scaleV * (charStyle.baselineOffset() / 1000.0);
							uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), true);
							bb = Doc->Items->at(z);
							bb->setTextFlowMode(currItem->textFlowMode());
							bb->setSizeLocked(currItem->sizeLocked());
							bb->setLocked(currItem->locked());
							bb->NamedLStyle = currItem->NamedLStyle;
							bb->setItemName(currItem->itemName()+"+S"+ccounter.setNum(a));
							bb->AutoName = false;
							bb->setRotation(currItem->rotation());
							bb->PoLine.addQuadPoint(FPoint(0, 0), FPoint(0, 0), FPoint(hl->glyph.xadvance, 0), FPoint(hl->glyph.xadvance, 0));
							bb->setLineColor(hl->fillColor());
							bb->setLineShade(hl->fillShade());
							bb->setLineWidth(lw);
							FPoint tp2(getMinClipF(&bb->PoLine));
							bb->PoLine.translate(-tp2.x(), -tp2.y());
							FPoint tp(getMaxClipF(&bb->PoLine));
							bb->setWidthHeight(tp.x(), tp.y());
							bb->Clip = FlattenPath(bb->PoLine, bb->Segments);
							double textX = CurX;
							double textY = ls.y - st + hl->glyph.yoffset;
							if (charStyle.baselineOffset() != 0)
								textY -= (charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0);
							if (a < currItem->itemText.length()-1)
								wide = hl->font().charWidth(chstr[0], charStyle.fontSize(), currItem->itemText.text(a+1));
							else
								wide = hl->font().charWidth(chstr[0], charStyle.fontSize());
							if (currItem->imageFlippedH())
								textX = currItem->width() - textX - wide;
							if (currItem->imageFlippedV())
								textY = currItem->height() - textY + y - (bb->height() - y);
							FPoint npo(textX, textY, 0.0, 0.0, currItem->rotation(), 1.0, 1.0);
							bb->moveBy(npo.x(),npo.y());
							bb->ContourLine = bb->PoLine.copy();
							bb->ClipEdited = true;
							bb->FrameType = 3;
							Doc->setRedrawBounding(bb);
							newGroupedItems.append(Doc->Items->takeAt(z));
						}
						CurX += hl->glyph.wide();
					}
				}
			}
			if ((currItem->asPathText()) && (currItem->PoShow))
			{
				uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), CommonStrings::None, currItem->lineColor(), true);
				PageItem *bb = Doc->Items->at(z);
				bb->PoLine = currItem->PoLine.copy();
				bb->ClipEdited = true;
				bb->FrameType = 3;
				bb->setRotation(currItem->rotation());
//				bb->setPolyClip(qRound(qMax(bb->lineWidth() / 2, 1)));
				Doc->AdjustItemSize(bb);
				newGroupedItems.append(Doc->Items->takeAt(z));
			}
			delItems.append(tmpSelection.takeItem(offset));
		}
		tmpSelection.clear();
		if (newGroupedItems.count() > 1)
		{
			for (int ag = 0; ag < newGroupedItems.count(); ++ag)
			{
				Doc->Items->insert(currItem->ItemNr+1+ag, newGroupedItems.at(ag));
				tmpSelection.addItem(newGroupedItems.at(ag));
			}
			Doc->renumberItemsInListOrder();
			Doc->itemSelection_GroupObjects(true, false, &tmpSelection);
		}
		else if (newGroupedItems.count() > 0)
		{
			Doc->Items->insert(currItem->ItemNr+1, newGroupedItems.at(0));
			Doc->renumberItemsInListOrder();
		}
		int toDeleteItemCount=delItems.count();
		if (toDeleteItemCount != 0)
		{
			tmpSelection.clear();
			for(int i=0; i<toDeleteItemCount; ++i)
				tmpSelection.addItem(delItems.takeAt(0)); //yes, 0, remove the first
			Doc->itemSelection_DeleteItem(&tmpSelection);
		}
		Doc->m_Selection->copy(tmpSelection, true);
		m_ScMW->HaveNewSel(-1);
		trans.commit();
	}
#endif
}

void ScribusView::keyPressEvent(QKeyEvent *k)
{
	m_ScMW->keyPressEvent(k);
}

void ScribusView::keyReleaseEvent(QKeyEvent *k)
{
	m_ScMW->keyReleaseEvent(k);
}

void ScribusView::wheelEvent(QWheelEvent *w)
{
	//QScrollArea::contentsWheelEvent(w);
//	evSpon = true;
	

	if (w->modifiers() == Qt::ControlModifier)
	{
		FPoint mp = m_canvas->globalToCanvas(w->globalPos());
		w->delta() > 0 ? slotZoomIn(mp.x(), mp.y()) : slotZoomOut(mp.x(), mp.y());
		
	}
	else
	{
		int dX=0,dY=0;
		int moveBy=(w->delta() < 0) ? Prefs->Wheelval : -Prefs->Wheelval;
		if ((w->orientation() != Qt::Vertical) || ( w->modifiers() == Qt::ShiftModifier ))
			dX = moveBy;
		else
			dY = moveBy;
		scrollBy(dX, dY);
	}
	w->accept();
}

void ScribusView::setObjectUndoMode()
{
	_isGlobalMode = undoManager->isGlobalMode();
	if (m_ScMW->HaveDoc)
	{
		//qDebug(QString("%1 %2").arg((int)m_ScMW).arg(m_ScMW->scrActions.contains("editActionMode")));
		m_ScMW->scrActions["editActionMode"]->setChecked(true);
		uint docSelectionCount=Doc->m_Selection->count();
		if (docSelectionCount == 1)
			undoManager->showObject(Doc->m_Selection->itemAt(0)->getUId());
		else if (docSelectionCount > 1)
			undoManager->showObject(Um::NO_UNDO_STACK);
		else if (docSelectionCount == 0)
			undoManager->showObject(Doc->currentPage()->getUId());
	}
}

void ScribusView::setGlobalUndoMode()
{
	if (m_ScMW->HaveDoc)
	{
		m_ScMW->scrActions["editActionMode"]->setChecked(!_isGlobalMode);
		if (_isGlobalMode)
			undoManager->showObject(Um::GLOBAL_UNDO_MODE);
		else
		{
			uint docSelectionCount=Doc->m_Selection->count();
			if (docSelectionCount == 1)
				undoManager->showObject(Doc->m_Selection->itemAt(0)->getUId());
			else if (docSelectionCount > 1)
				undoManager->showObject(Um::NO_UNDO_STACK);
			else if (docSelectionCount == 0)
				undoManager->showObject(Doc->currentPage()->getUId());
		}
	}
}

void ScribusView::unitChange()
{
	vertRuler->unitChange();
	horizRuler->unitChange();
}

void ScribusView::setRulersShown(bool isShown)
{
	vertRuler->setShown(isShown);
	horizRuler->setShown(isShown);
	rulerMover->setShown(isShown);
	int newTopLeftMargin = isShown ? m_vhRulerHW : 0;
	setViewportMargins(newTopLeftMargin, newTopLeftMargin, 0, 0);
}

void ScribusView::slotUpdateContents()  // deprecated
{
	updateContents();
}

void ScribusView::slotUpdateContents(const QRect &r) // deprecated
{
	updateContents(r);
}


void ScribusView::setScale(const double newScale)
{
	double Scale=newScale;
	if (Scale < Doc->toolSettings.magMin*Prefs->DisScale/100.0)
		Scale=Doc->toolSettings.magMin*Prefs->DisScale/100.0;

	if (Scale > Doc->toolSettings.magMax*Prefs->DisScale/100.0)
		Scale=Doc->toolSettings.magMax*Prefs->DisScale/100.0;

	if (Scale > 32*Prefs->DisScale)
		Scale=32*Prefs->DisScale;

	m_canvas->setScale(Scale);

	zoomSpinBox->blockSignals(true);
	zoomSpinBox->setValue(m_canvas->scale()/Prefs->DisScale*100);
	zoomSpinBox->blockSignals(false);

	unitChange();
}



double ScribusView::scale() const
{
	return m_canvas->scale();
}



bool ScribusView::eventFilter(QObject *obj, QEvent *event)
{
//	if (obj == horizRuler || obj == vertRuler || obj == rulerMover)
//		return true; // FIXME:av

	if (obj == widget() && event->type() == QEvent::MouseMove)
	{
		QMouseEvent* m = static_cast<QMouseEvent*> (event);
		emit MousePos(m->x() / m_canvas->scale() + Doc->minCanvasCoordinate.x(),
					  m->y() / m_canvas->scale() + Doc->minCanvasCoordinate.y());
		horizRuler->Draw(m->x() + qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale())); //  - 2 * contentsX());
		vertRuler->Draw(m->y() + qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
		m_canvasMode->mouseMoveEvent(m);
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent* m = static_cast<QMouseEvent*> (event);
		m_canvasMode->mouseReleaseEvent(m);
		m_canvas->m_viewMode.m_MouseButtonPressed = false;
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* m = static_cast<QMouseEvent*> (event);
		m_canvasMode->mousePressEvent(m);
		m_canvas->m_viewMode.m_MouseButtonPressed = true;
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::MouseButtonDblClick)
	{
		QMouseEvent* m = static_cast<QMouseEvent*> (event);
		m_canvasMode->mouseDoubleClickEvent(m);
		return true;
	}
	else if (/* obj == widget() && */ event->type() == QEvent::KeyPress)
	{
		QKeyEvent* m = static_cast<QKeyEvent*> (event);
		m_ScMW->keyPressEvent(m);
		return true;
	}
	else if (/* obj == widget() && */ event->type() == QEvent::KeyRelease)
	{
		QKeyEvent* m = static_cast<QKeyEvent*> (event);
		m_ScMW->keyReleaseEvent(m);
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::DragEnter)
	{
		QDragEnterEvent* d = static_cast<QDragEnterEvent*> (event);
		contentsDragEnterEvent(d);
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::DragLeave)
	{
		QDragLeaveEvent* d = static_cast<QDragLeaveEvent*> (event);
		contentsDragLeaveEvent(d);
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::DragMove)
	{
		QDragMoveEvent* d = static_cast<QDragMoveEvent*> (event);
		contentsDragMoveEvent(d);
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::Drop)
	{
		QDropEvent* d = static_cast<QDropEvent*> (event);
		contentsDropEvent(d);
		return true;
	}

	// rulermover events
	// hruler events
	// vruler events

	return QScrollArea::eventFilter(obj, event);
}

//Legacy

void ScribusView::updateContents(QRect box)
{
	if (box.isValid())
		m_canvas->update(box);
	else
		m_canvas->update();
}

void ScribusView::updateContents(int x, int y, int w, int h)
{
	updateContents(QRect(x,y,w,h));
}

void ScribusView::repaintContents(QRect box)
{
	if (box.isValid())
		m_canvas->repaint(box);
	else
		m_canvas->repaint();
}

void ScribusView::resizeContents(int w, int h)  // deprecated
{
//	qDebug() << "ScribusView::resizeContents(" << w << "," << h << ")";
	int originX = qRound(Doc->minCanvasCoordinate.x() * scale());
	int originY = qRound(Doc->minCanvasCoordinate.y() * scale());
	widget()->resize(w - 0*originX, h - 0*originY);
}

QPoint ScribusView::contentsToViewport(QPoint p) // deprecated
{
	return p + viewport()->pos();
}

QPoint ScribusView::viewportToContents(QPoint p) // deprecated
{
	return p - viewport()->pos();
}

int ScribusView::contentsX() // deprecated
{
	int originX = qRound(Doc->minCanvasCoordinate.x() * scale());
	return horizontalScrollBar()->value() + originX;
}

int ScribusView::contentsY() // deprecated
{
	int originY = qRound(Doc->minCanvasCoordinate.y() * scale());
	return verticalScrollBar()->value() + originY;
}

int ScribusView::contentsWidth()
{
	return horizontalScrollBar()->maximum() - horizontalScrollBar()->minimum();
}

int ScribusView::contentsHeight()
{
	return verticalScrollBar()->maximum() - verticalScrollBar()->minimum();
}

void ScribusView::setContentsPos(int x, int y)
{
	horizontalScrollBar()->setValue(x);
	verticalScrollBar()->setValue(y);
	setRulerPos(contentsX(), contentsY());
}


void ScribusView::scrollContentsBy(int dx, int dy)
{
	QScrollArea::scrollContentsBy (dx, dy);
	setRulerPos(contentsX(), contentsY());
}

void ScribusView::scrollBy(int x, int y) // deprecated
{
	setContentsPos(horizontalScrollBar()->value() + x, verticalScrollBar()->value() + y);
}

void ScribusView::zoom(double scale)
{
	double zPointX = oldX, zPointY = oldY;
	if (scale <= 0.0)
		scale = m_canvas->scale();
	if (Doc->m_Selection->count() != 0)
	{
		PageItem *currItem = Doc->m_Selection->itemAt(0);
		zPointX = currItem->xPos() + currItem->width() / 2.0;
		zPointY = currItem->yPos() + currItem->height() / 2.0;
	}
	zoom( qRound(zPointX), qRound(zPointY), scale, false);
}

void ScribusView::zoom(int canvasX, int canvasY, double scale, bool preservePoint)
{
	QPoint canvasPoint;
	QPoint globalPoint = m_canvas->canvasToGlobal(QPointF(canvasX, canvasY));
	double newScale    = (scale > 32*Prefs->DisScale) ? (32*Prefs->DisScale) : scale;
	undoManager->setUndoEnabled(false);
	updatesOn(false);
	setScale(newScale);
	QPoint localPoint = m_canvas->canvasToLocal( QPointF(canvasX, canvasY) );
	int nw = qMax(qRound((Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()) * m_canvas->scale()), visibleWidth());
	int nh = qMax(qRound((Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()) * m_canvas->scale()), visibleHeight());
	resizeContents(nw, nh); // FIXME : should be avoided here, cause an unnecessary paintEvent despite updates disabled
	if (preservePoint)
		canvasPoint = viewport()->mapFromGlobal(globalPoint);
	else
	{
		QSize viewsize = viewport()->size();
		canvasPoint = QPoint(viewsize.width() / 2, viewsize.height() / 2);
	}
	setContentsPos(localPoint.x() - canvasPoint.x(), localPoint.y() - canvasPoint.y());
	updatesOn(true);
	undoManager->setUndoEnabled(true);
}


void ScribusView::stopAllDrags() // deprecated
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
//FIXME:av	inItemCreation = false;
	MidButt = false;
}
