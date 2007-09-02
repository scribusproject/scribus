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
#include <QFont>
#include <QFontMetrics>
#include <QPixmap>
#include <QPolygon>
#include <QStringList>
#include <QImage>
#include <QFileInfo>
#include <QFile>
#include <QStack>
//Added by qt3to4:
#include <QWheelEvent>
#include <QPaintEvent>
#include <QDrag>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QMimeData>
#include <QLabel>
#include <QDropEvent>
#include <QMenu>
#include <QMenu>
#include <QDragEnterEvent>
#include <QList>
#include <QMouseEvent>
#include <QImageReader>
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
#include "canvasmode.h"
#include "actionmanager.h"
#include "commonstrings.h"
#include "extimageprops.h"
#include "filewatcher.h"
#include "guidemanager.h"
#include "hruler.h"
#include "hyphenator.h"
#include "insertTable.h"
#include "mpalette.h"
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
#include "rulermover.h"
#include "scmessagebox.h"
#include "scpaths.h"
#include "scrap.h"
#include "scribusXml.h"
#include "selection.h"
#include "serializer.h"
#include "stencilreader.h"
#include "story.h"
#include "text/nlsconfig.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"
#include "util_math.h"
#include "vruler.h"
#include <tiffio.h>
#include CMS_INC


using namespace std;



ScribusView::ScribusView(QWidget* win, ScribusMainWindow* mw, ScribusDoc *doc) :
	QScrollArea(win),
	Doc(doc),
	m_canvas(new Canvas(doc, this)),
	Prefs(&(PrefsManager::instance()->appPrefs)),
	undoManager(UndoManager::instance()),
	OldScale(0),
	dragX(0), dragY(0), dragW(0), dragH(0),
	oldW(-1), oldCp(-1),
	Mxp(-1), Myp(-1), Dxp(-1), Dyp(-1),
	frameResizeHandle(-1),
	SeRx(-1), SeRy(-1), GyM(-1), GxM(-1),
//	ClRe(-1), ClRe2(-1),
//	SegP1(-1), SegP2(-1),
	RotMode(0),
	DrHY(-1), DrVX(-1),
//	EdPoints(true),
//	m_MouseButtonPressed(false),	
//	operItemMoving(false),
	MoveGY(false), MoveGX(false),
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
	RecordP(),
	Ready(false),
	oldX(0), oldY(0),
	_groupTransactionStarted(false),
	_isGlobalMode(true),
	evSpon(false),
	forceRedraw(false),
//	Scale(Prefs->DisScale),
//	oldClip(0),
	m_vhRulerHW(17),
	m_cursorVisible(false),
	m_ScMW(mw)
{
		setName("s");
		setAttribute(Qt::WA_StaticContents);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setViewportMargins(m_vhRulerHW, m_vhRulerHW, 0, 0);
	setWidgetResizable(false);
	m_canvasMode = CanvasMode::createForAppMode(this, Doc->appMode);
	setWidget(m_canvas);
	//already done by QScrollArea: widget()->installEventFilter(this);
	viewport()->setBackgroundMode(Qt::PaletteBackground);
	QFont fo = QFont(font());
	int posi = fo.pointSize()-2;
	fo.setPointSize(posi);
	unitSwitcher = new QComboBox( false, this, "unitSwitcher" );
	unitSwitcher->setFocusPolicy(Qt::NoFocus);
	unitSwitcher->setFont(fo);
	int maxUindex = unitGetMaxIndex() - 2;
	for (int i = 0; i <= maxUindex; ++i)
		unitSwitcher->insertItem(unitGetStrFromIndex(i));
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
	cmsToolbarButton->setToggleButton(true);
	QIcon ic2;
	ic2.addPixmap(loadIcon("cmsOff.png"), QIcon::Normal, QIcon::Off);
	ic2.addPixmap(loadIcon("cmsOn.png"), QIcon::Normal, QIcon::On);
	cmsToolbarButton->setIcon(ic2);
	previewToolbarButton->setAutoRaise(OPTION_FLAT_BUTTON);
	previewToolbarButton->setToggleButton(true);
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
	//zoomDefaultToolbarButton->setText("1:1");
	zoomDefaultToolbarButton->setPixmap(loadIcon("16/zoom-original.png"));
	zoomOutToolbarButton->setPixmap(loadIcon("16/zoom-out.png"));
	zoomInToolbarButton->setPixmap(loadIcon("16/zoom-in.png"));
	pageSelector = new PageSelector(this, Doc->Pages->count());
	pageSelector->setFont(fo);
	pageSelector->setFocusPolicy(Qt::ClickFocus);
	layerMenu = new QComboBox( true, this, "LY" );
	layerMenu->setEditable(false);
	layerMenu->setFont(fo);
	layerMenu->setFocusPolicy(Qt::NoFocus);
	visualMenu = new QComboBox( false, this, "visualMenu" );
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
	viewport()->setMouseTracking(true);
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	// FIXME setDragAutoScroll(false);
	Doc->DragP = false;
	Doc->leaveDrag = false;
	Doc->SubMode = -1;
	storedFramesShown = Doc->guidesSettings.framesShown;
	storedShowControls = Doc->guidesSettings.showControls;
	m_canvas->m_viewMode.viewAsPreview = false;
	m_canvas->setPreviewVisual(-1);
	shiftSelItems = false;
	inItemCreation = false;
	m_previousMode = -1;
	redrawMode = 0;
	redrawCount = 0;
	redrawMarker = new QRubberBand(QRubberBand::Rectangle);
	redrawMarker->hide();
	m_canvas->newRedrawPolygon();
	m_canvas->resetRenderMode();
	m_ScMW->scrActions["viewFitPreview"]->setOn(m_canvas->m_viewMode.viewAsPreview);
	m_SnapCounter = 0;
	
	Doc->regionsChanged()->connectObserver(this);
	connect(zoomOutToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoomOut()));
	connect(zoomInToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoomIn()));
	connect(zoomDefaultToolbarButton, SIGNAL(clicked()), this, SLOT(slotZoom100()));
	connect(zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom()));
	connect(pageSelector, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
	connect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	connect(unitSwitcher, SIGNAL(activated(int)), this, SLOT(ChgUnit(int)));
	connect(previewToolbarButton, SIGNAL(clicked()), this, SLOT(togglePreview()));
	connect(cmsToolbarButton, SIGNAL(clicked()), this, SLOT(toggleCMS()));
	connect(visualMenu, SIGNAL(activated(int)), this, SLOT(switchPreviewVisual(int)));
	connect(this, SIGNAL(contentsMoving(int, int)), this, SLOT(setRulerPos(int, int)));
	connect(this, SIGNAL(HaveSel(int)), this, SLOT(selectionChanged()));
	languageChange();
	dragTimer = new QTimer(this);
	connect(dragTimer, SIGNAL(timeout()), this, SLOT(dragTimerTimeOut()));
	dragTimer->stop();
	dragTimerFired = false;
}

void ScribusView::languageChange()
{
	cmsToolbarButton->setToolTip("");
	previewToolbarButton->setToolTip("");
	visualMenu->setToolTip("");
	cmsToolbarButton->setToolTip( tr("Switches Color Management on or off"));
	previewToolbarButton->setToolTip( tr("Enables the Preview Mode"));
	visualMenu->setToolTip( tr("Here you can select the visual appearance of the display\nYou can choose between normal and several color blindness forms"));
	disconnect(visualMenu, SIGNAL(activated(int)), this, SLOT(switchPreviewVisual(int)));
	visualMenu->clear();
	visualMenu->insertItem(CommonStrings::trVisionNormal);
	visualMenu->insertItem(CommonStrings::trVisionProtanopia);
	visualMenu->insertItem(CommonStrings::trVisionDeuteranopia);
	visualMenu->insertItem(CommonStrings::trVisionTritanopia);
	visualMenu->insertItem(CommonStrings::trVisionFullColorBlind);
	visualMenu->setCurrentItem(m_canvas->previewVisual());
	connect(visualMenu, SIGNAL(activated(int)), this, SLOT(switchPreviewVisual(int)));
}

void ScribusView::toggleCMS()
{
	Doc->enableCMS(!Doc->HasCMS);
	repaintContents(QRect());
}

void ScribusView::switchPreviewVisual(int vis)
{
	m_canvas->setPreviewVisual(vis);
	Doc->recalculateColors();
	Doc->recalcPicturesRes();
	repaintContents(QRect());
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
	m_ScMW->scrActions["viewFitPreview"]->setOn(m_canvas->m_viewMode.viewAsPreview);
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
	previewToolbarButton->setOn(m_canvas->m_viewMode.viewAsPreview);
#endif
	visualMenu->setEnabled(m_canvas->m_viewMode.viewAsPreview);
	Doc->recalculateColors();
	Doc->recalcPicturesRes();
	repaintContents(QRect());
}

void ScribusView::changed(QRect re)
{
	qDebug() << "ScribusView: changed region:" << re;
	if (re.isValid())
		m_canvas->repaint(re);
	else
		m_canvas->repaint();
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
	qDebug() << "request mode:" << appMode;
	switch(appMode) // filter submodes
	{
		case submodePaintingDone:   // return to normal mode
			appMode = m_previousMode < 0 ? modeNormal : m_previousMode;
			m_previousMode = -1;
			updateNecessary = true;
			break;			
		case submodeEndNodeEdit:     // return from node/shape editing
			appMode = m_previousMode < 0 ? modeNormal : m_previousMode;
			m_previousMode = -1;
			updateNecessary = true;
			return;
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
				qDebug() << "request mode: UNKNOWN" << appMode;
				appMode = modeNormal;
			}
			m_previousMode = appMode;			
			break;
	}

	qDebug() << "request mode" << Doc->appMode << "-->" << appMode;
	if (Doc->appMode != appMode)
	{
		m_ScMW->setAppMode(appMode);
		CanvasMode* newCanvasMode = modeInstances.value(appMode);
		if (!newCanvasMode)
		{
			newCanvasMode = CanvasMode::createForAppMode(this, appMode);
			modeInstances[appMode] = newCanvasMode;
		}
		qDebug() << "request canvas mode" << newCanvasMode;
		if (newCanvasMode)
		{
			m_canvasMode = newCanvasMode;
		}
		updateNecessary = true;
	}
	if (updateNecessary)
		updateContents();
}



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
			case modeDrawPicture:
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
//	e->accept(Q3TextDrag::canDecode(e));
	e->accept();
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
		text = e->mimeData()->text();
		double gx, gy, gw, gh;
		/*<< #3524
		setActiveWindow();
		raise();
		m_ScMW->newActWin(Doc->WinHan);
		updateContents();
		>>*/
//		SeleItemPos(e->pos());
		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (fi.exists())
			text = ur.path();
		if(ss->ReadElemHeader(text,fi.exists(), &gx, &gy, &gw, &gh))
		{
			dragX = e->pos().x() / m_canvas->scale();
			dragY = e->pos().y() / m_canvas->scale();
			dragW = gw;
			dragH = gh;
			DraggedGroup = true;
			DraggedGroupFirst = true;
			getDragRectScreen(&gx, &gy, &gw, &gh);
//			gx += Doc->minCanvasCoordinate.x();
//			gy += Doc->minCanvasCoordinate.y();
			QPoint evP = viewport()->mapToGlobal(e->pos());
			evP -= QPoint(contentsX(), contentsY());
			redrawMarker->setGeometry(QRect(evP.x() + 1, evP.y() + 1, qRound(gw), qRound(gh)).normalized());
			if (!redrawMarker->isVisible())
				redrawMarker->show();
			emit ItemGeom(gw, gh);
		}
		delete ss;
		ss=NULL;
	}
}

void ScribusView::contentsDragMoveEvent(QDragMoveEvent *e)
{
	QString text;
//	PageItem *currItem;
//	bool img;
//	e->accept(Q3TextDrag::canDecode(e));
	e->accept();
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
		text = e->mimeData()->text();
		if (DraggedGroup)
		{
			double gx, gy, gw, gh;
			dragX = e->pos().x() / m_canvas->scale();
			dragY = e->pos().y() / m_canvas->scale();
			getDragRectScreen(&gx, &gy, &gw, &gh);
//			gx += Doc->minCanvasCoordinate.x();
//			gy += Doc->minCanvasCoordinate.y();
			QPoint evP = viewport()->mapToGlobal(e->pos());
			evP -= QPoint(contentsX(), contentsY());
			redrawMarker->setGeometry(QRect(evP.x() + 2, evP.y() + 2, qRound(gw - 2), qRound(gh - 2)).normalized());
			if (!redrawMarker->isVisible())
				redrawMarker->show();
			DraggedGroupFirst = false;
			emit MousePos(dragX, dragY); //+Doc->minCanvasCoordinate.x(), dragY+Doc->minCanvasCoordinate.y());
			horizRuler->Draw(e->pos().x());
			vertRuler->Draw(e->pos().y());
//			return;
		}
/*		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
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
		redrawMarker->hide();
		updateContents();
	}
}

void ScribusView::contentsDropEvent(QDropEvent *e)
{
	QString text;
	PageItem *currItem;
	bool img = false;
	m_canvas->resetRenderMode();
	redrawMode = 0;
	redrawMarker->hide();
//	struct ScText *hg;
//	uint a;
	int re = 0;
//	e->accept(Q3TextDrag::canDecode(e));
	e->accept();
	DraggedGroupFirst = false;
	int ex = qRound(e->pos().x()/m_canvas->scale());// + Doc->minCanvasCoordinate.x());
		int ey = qRound(e->pos().y()/m_canvas->scale());// + Doc->minCanvasCoordinate.y());
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
		text = e->mimeData()->text();
		//<<#3524
		setActiveWindow();
		raise();
		m_ScMW->newActWin(Doc->WinHan);
		updateContents();
		//>>
		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).toUpper();
		QStringList imfo;
		QList<QByteArray> imgs = QImageReader::supportedImageFormats();
		for (int i = 0; i < imgs.count(); ++i )
		{
			imfo.append(QString(imgs.at(i)));
		}
		if (ext == "JPG")
			ext = "JPEG";
		//CB Need to handle this ugly file extension list elsewhere... some capabilities class perhaps
		img = ((imfo.contains(ext)) || extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext) || extensionIndicatesTIFF(ext) || extensionIndicatesPSD(ext));
		bool selectedItemByDrag=false;
		int pscx=qRound(e->pos().x()/m_canvas->scale()), pscy=qRound(e->pos().y()/m_canvas->scale());
		//Loop through all items and see which one(s) were under the drop point on the current layer
		//Should make a nice function for this.
		for (int i=0; i<Doc->Items->count(); ++i)
		{
			if (Doc->Items->at(i)->LayerNr==Doc->activeLayer())
			{
				if (Doc->Items->at(i)->pointWithinItem(pscx, pscy))
				{
					Deselect(false);
					SelectItem(Doc->Items->at(i));
					selectedItemByDrag=true;
					break;
				}
			}
		}
		//CB When we drag an image to a page from outside
		//SeleItemPos is from 1.2.x. Needs reenabling for dragging *TO* a frame
		if ((fi.exists()) && (img) && !selectedItemByDrag)// && (!SeleItemPos(e->pos())))
		{
			int z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, ex, ey, 1, 1, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, true);
			PageItem *b = Doc->Items->at(z);
			Doc->LoadPict(ur.path(), b->ItemNr);
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
		if (Doc->m_Selection->count()>0 && Doc->m_Selection->itemAt(0)->pointWithinItem(pscx, pscy) && (!text.startsWith("<SCRIBUSELEM")))
		{
			PageItem *b = Doc->m_Selection->itemAt(0);
			if (b->itemType() == PageItem::ImageFrame)
			{
				if ((fi.exists()) && (img))
				{
					Doc->LoadPict(ur.path(), b->ItemNr);
					updateContents();
				}
			}
			/* CB leaving this out for now...
			if (b->PType == 4)
			{
				if ((b->BackBox != 0) && (b->itemText.count() == 0))
					return;
				if ((fi.exists()) && (!img) && (fi.size() < 500000))
				{
					Serializer *ss = new Serializer(ur.path());
					if (ss->Read())
					{
						int st = doku->currentParaStyle;
						ss->GetText(b, st, doku->docParagraphStyles[st].Font, doku->docParagraphStyles[st].FontSize, true);
						emit DocChanged();
					}
					delete ss;
					ss=NULL;
					update();
				}
				else
				{
					slotDoCurs(false);
					slotSetCurs(e->pos().x(), e->pos().y());
					if (text.startsWith("<SCRIBUSELEM"))
						return;
					for (a=0; a<text.length(); ++a)
					{
						hg = new ScText;
						hg->ch = text.at(a);
						if (hg->ch == QChar(10))
							hg->ch = QChar(13);
						if (hg->ch == QChar(4))
							hg->ch = QChar(9);
						if (hg->ch == QChar(5))
							hg->ch = QChar(13);
						hg->cfont = b->IFont;
						hg->csize = b->ISize;
						hg->ccolor = b->TxtFill;
						hg->cshade = b->ShTxtFill;
						hg->cstroke = b->TxtStroke;
						hg->cshade2 = b->ShTxtStroke;
						hg->cselect = false;
						hg->cscale = b->TxtScale;
						hg->cextra = 0;
						hg->cstyle = 0;
						hg->cab = 0;
						hg->xp = 0;
						hg->yp = 0;
						hg->PRot = 0;
						hg->PtransX = 0;
						hg->PtransY = 0;
						b->itemText.insert(b->CPos, hg);
						b->CPos += 1;
					}
					emit DocChanged();
					update();
				}
			}*/
		}
		else
		{
//			for (uint as = 0; as < Doc->Items->count(); ++as)
//			{
//				Doc->Items->at(as)->setSelected(false);
//			}
			Deselect(true);
			uint oldDocItemCount = Doc->Items->count();
			if ((!img) && (Doc->DraggedElem == 0))
			{
				undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Create,"",Um::ICreate);
				if ((fi.exists()) && (!img))
				{
					QString data;
					if (fi.extension(true).toLower() == "sml")
					{
						QString f = "";
						loadText(ur.path(), &f);
						StencilReader *pre = new StencilReader();
						data = pre->createObjects(f);
						delete pre;
						emit LoadElem(data, ex, ey, false, false, Doc, this);
					}
					else if (fi.extension(true).toLower() == "shape")
					{
						QString f = "";
						loadText(ur.path(), &f);
						StencilReader *pre = new StencilReader();
						data = pre->createShape(f);
						delete pre;
						emit LoadElem(data, ex, ey, false, false, Doc, this);
					}
					else
						emit LoadElem(ur.path(), ex, ey, true, false, Doc, this);
				}
				else
					emit LoadElem(QString(text), ex, ey, false, false, Doc, this);
				Selection tmpSelection(this, false);
				tmpSelection.copy(*Doc->m_Selection, false, true);
				for (int as = oldDocItemCount; as < Doc->Items->count(); ++as)
				{
					currItem = Doc->Items->at(as);
					Doc->setRedrawBounding(currItem);
					tmpSelection.addItem(currItem, true);
					if (currItem->isBookmark)
						emit AddBM(currItem);
				}
				Doc->m_Selection->copy(tmpSelection, false, false);
				undoManager->commit();
			}
			else
			{
				if (Doc->DraggedElem != 0)
				{
					if (!Doc->leaveDrag)
					{
						QMenu *pmen = new QMenu();
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
						pmen->insertItem( tr("Copy Here"));
						int mov = pmen->insertItem( tr("Move Here"));
						pmen->insertItem( tr("Cancel"));
						for (int dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							if (Doc->Items->at(Doc->DragElements[dre])->locked())
							{
								pmen->setItemEnabled(mov, false);
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
						emit LoadElem(QString(text), ex, ey, false, false, Doc, this);
						for (int as = oldDocItemCount; as < Doc->Items->count(); ++as)
						{
							pasted.append(Doc->Items->at(as));
						}
						Selection tmpSelection(this, false);
						tmpSelection.copy(*Doc->m_Selection, false, true);
						for (int dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							tmpSelection.addItem(Doc->Items->at(Doc->DragElements[dre]), true);
						}
						Doc->m_Selection->copy(tmpSelection, false, false);
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
/*						for (uint dre=0; dre<Doc->DragElements.count(); ++dre)
						{
							currItem = Doc->m_Selection->itemAt(dre);
							currItem->NextBox = 0;
							currItem->BackBox = 0;
						}
*/
						pasted.clear();
						Doc->itemSelection_DeleteItem();
					}
				}
				if ((!img) && ((re == 0)))
					emit LoadElem(QString(text), ex, ey, false, false, Doc, this);
				Doc->DraggedElem = 0;
				Doc->DragElements.clear();
				Selection tmpSelection(this, false);
				tmpSelection.copy(*Doc->m_Selection, false, true);
				for (int as = oldDocItemCount; as < Doc->Items->count(); ++as)
				{
					currItem = Doc->Items->at(as);
					Doc->setRedrawBounding(currItem);
					tmpSelection.addItem(currItem, true);
					if (currItem->isBookmark)
						emit AddBM(currItem);
				}
				Doc->m_Selection->copy(tmpSelection, false, false);
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
				moveGroup(nx-gx, ny-gy, false);
				Doc->m_Selection->setGroupRect();
				Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				nx = gx+gw;
				ny = gy+gh;
				Doc->ApplyGuides(&nx, &ny);
				moveGroup(nx-(gx+gw), ny-(gy+gh), false);
				Doc->m_Selection->setGroupRect();
				Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				for (int a = 0; a < Doc->m_Selection->count(); ++a)
				{
					PageItem *currItem = Doc->m_Selection->itemAt(a);
					currItem->gXpos = currItem->xPos() - gx;
					currItem->gYpos = currItem->yPos() - gy;
					currItem->gWidth = gw;
					currItem->gHeight = gh;
				}
				emit ItemPos(gx, gy);
				emit ItemGeom(gw, gh);
			}
			else
			{
				Doc->m_Selection->connectItemToGUI();
				currItem = Doc->m_Selection->itemAt(0);
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
				int x = static_cast<int>(Doc->Pages->at(i)->xOffset());
				int y = static_cast<int>(Doc->Pages->at(i)->yOffset());
				int w = static_cast<int>(Doc->Pages->at(i)->width());
				int h = static_cast<int>(Doc->Pages->at(i)->height());
				if (QRect(x, y, w, h).contains(ex, ey))
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


#if 0
void ScribusView::contentsMouseDoubleClickEvent(QMouseEvent *m)
{}
{
	m->accept();
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	dragTimer->stop();
	PageItem *currItem = 0;
	if (Doc->appMode == modeEditClip)
	{
		emit EndNodeEdit();
		return;
	}
	if ((Doc->m_Selection->isMultipleSelection()) || (Doc->appMode != modeNormal))
	{
		if ((Doc->m_Selection->isMultipleSelection()) && (Doc->appMode == modeNormal))
		{
			if (GetItem(&currItem))
			{
				if (currItem->isTableItem)
				{
					Deselect(false);
					Doc->m_Selection->addItem(currItem);
					currItem->isSingleSel = true;
					//CB FIXME dont call this if the added item is item 0
					if (!Doc->m_Selection->primarySelectionIs(currItem))
						currItem->emitAllToGUI();
					updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				}
			}
			return;
		}
		else
		{
			if (!(GetItem(&currItem) && (Doc->appMode == modeEdit) && currItem->asTextFrame()))
			{
				contentsMousePressEvent(m);
				return;
			}
		}
	}
	if (GetItem(&currItem))
	{
		if ((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::PolyLine) || (currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::PathText))
		{
			if ((currItem->locked()) || (!currItem->ScaleType))
			{
//				contentsMousePressEvent(m);
				return;
			}
			//If we double click on an image frame and theres no image assigned, open the
			//load picture dialog, else put it into edit mode if the frame is set to show the image
			if (currItem->itemType() == PageItem::ImageFrame)
			{
				if (currItem->Pfile.isEmpty())
					emit LoadPic();
				else if (!currItem->PicAvail)
					emit StatusPic();
				else if (currItem->imageShown())
					emit Amode(modeEdit);
 			}
 			else
 				emit Amode(modeEdit);
		}
		else
			if (currItem->itemType() == PageItem::TextFrame)
			{
				//CB old code
				//emit currItem->isAnnotation() ? AnnotProps() : Amode(modeEdit);
				//contentsMousePressEvent(m);
				//CB if annotation, open the annotation dialog
				if (currItem->isAnnotation())
				{
					emit AnnotProps();
//					contentsMousePressEvent(m);
				}
				//else if not in mode edit, set mode edit
				else if (Doc->appMode != modeEdit)
				{
					emit Amode(modeEdit);
					//CB ignore the double click and go with a single one
					//if we werent in mode edit before.
					//unsure if this is correct, but its ok given we had no
					//double click select until now.
					contentsMousePressEvent(m);
				}
				//otherwise, select between the whitespace
				else
				{	//Double click in a frame to select a word
					PageItem_TextFrame *cItem=currItem->asTextFrame();
					bool inText = slotSetCurs(m->x(), m->y());
					if (!inText)
					{
						Deselect(true);
						slotDoCurs(true);
						emit Amode(modeNormal);
						return;
					}
					int a=cItem->CPos;
					while(a>0)
					{
						if (cItem->itemText.text(a-1).isLetterOrNumber())
							--a;
						else
							break;
					}
					int b=cItem->CPos;
					while(b<cItem->itemText.length())
					{
						if (cItem->itemText.text(b).isLetterOrNumber())
							++b;
						else
							break;
					}
					oldCp = a;
					cItem->CPos=b;
					cItem->ExpandSel(1, oldCp);
					slotDoCurs(true);
				}
			}
	}
}
#endif

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

#if 0
void ScribusView::contentsMouseReleaseEvent(QMouseEvent *m)
{}
{
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->resetRenderMode();
	m->accept();
	dragTimer->stop();
	if ((Doc->appMode == modeEditClip || Doc->appMode == modeNormal) && Doc->guidesSettings.guidesShown)
	{
		bool foundGuide = false;
		double nx = translateToDoc(m->x(), m->y()).x();
		double ny = translateToDoc(m->x(), m->y()).y();
		double grabRadScale=Doc->guidesSettings.grabRad / m_canvas->scale();
		if (Doc->currentPage()->guides.isMouseOnHorizontal(ny + grabRadScale, ny - grabRadScale, GuideManagerCore::Standard)
			|| Doc->currentPage()->guides.isMouseOnVertical(nx + grabRadScale, nx - grabRadScale, GuideManagerCore::Standard))
			foundGuide = true;
		if ((foundGuide) && (m->button() == Qt::RightButton) && (!GetItem(&currItem)))
		{
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			MoveGY = false;
			MoveGX = false;
			return;
		}
		if (MoveGY)
		{
			SetYGuide(m, GyM);
			MoveGY = false;
			redrawMarker->hide();
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			updateContents();
			GyM = -1;
			return;
		}
		if (MoveGX)
		{
			SetXGuide(m, GxM);
			MoveGX = false;
			redrawMarker->hide();
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			updateContents();
			GxM = -1;
			return;
		}
	}
	if (Doc->appMode == modeEditGradientVectors)
		return;
	if (Doc->appMode == modeCopyProperties)
		return;
	if (Doc->appMode == modeMeasurementTool)
	{
		m_canvas->newRedrawPolygon();
		updateContents(QRect(QPoint(Dxp, Dyp), QPoint(Mxp, Myp)).normalized().adjusted(-10, -10, 20, 20));
//		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		//emit PaintingDone();
		return;
	}
	if (Doc->appMode == modePanning)
	{
		if ((m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
		{
			m_ScMW->setAppMode(modeNormal);
		}
		return;
	}
	if (Doc->appMode == modeDrawTable)
	{
		if ((Doc->m_Selection->count() == 0) && (HaveSelRect) && (!MidButt))
		{
			QRect AreaR = QRect(static_cast<int>(Mxp), static_cast<int>(Myp), static_cast<int>(SeRx-Mxp), static_cast<int>(SeRy-Myp)).normalized();
			HaveSelRect = false;
			double Tx, Ty, Tw, Th;
			FPoint np2 = Doc->ApplyGrid(QPoint(Mxp, Myp));
			Tx = np2.x();
			Ty = np2.y();
			Doc->ApplyGuides(&Tx, &Ty);
			Mxp = qRound(Tx);
			Myp = qRound(Ty);
			np2 = Doc->ApplyGrid(QPoint(SeRx, SeRy));
			Tw = np2.x();
			Th = np2.y();
			Doc->ApplyGuides(&Tw, &Th);
			SeRx = qRound(Tw);
			SeRy = qRound(Th);
			Tw = Tw - Tx;
			Th = Th - Ty;
			int z;
			int Cols, Rows;
			double deltaX, deltaY, offX, offY;
			if ((Th < 6) || (Tw < 6))
			{
				redrawMarker->hide();
				Doc->appMode = modeNormal;
				emit PaintingDone();
				return;
			}
			InsertTable *dia = new InsertTable(this, static_cast<int>(Th / 6), static_cast<int>(Tw / 6));
			if (!dia->exec())
			{
				redrawMarker->hide();
				Doc->appMode = modeNormal;
				emit PaintingDone();
				delete dia;
				dia=NULL;
				return;
			}
			redrawMarker->hide();
			Cols = dia->Cols->value();
			Rows = dia->Rows->value();
			delete dia;
			dia=NULL;
			deltaX = Tw / Cols;
			deltaY = Th / Rows;
			offX = 0.0;
			offY = 0.0;
			Doc->m_Selection->clear();
			if (UndoManager::undoEnabled())
				undoManager->beginTransaction(Doc->currentPage()->getUName(),
											  Um::ITable, Um::CreateTable,
											  QString(Um::RowsCols).arg(Rows).arg(Cols),
											  Um::ICreate);
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					//z = PaintText(Tx + offX, Ty + offY, deltaX, deltaY, Doc->toolSettings.dWidth, Doc->toolSettings.dPenText);
					z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Tx + offX, Ty + offY, deltaX, deltaY, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_canvas->m_viewMode.m_MouseButtonPressed);
					currItem = Doc->Items->at(z);
					currItem->isTableItem = true;
					//currItem->setTextFlowsAroundFrame(true);
					//currItem->setTextFlowUsesBoundingBox(true);
					currItem->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
					Doc->m_Selection->addItem(currItem);
					offX += deltaX;
				}
				offY += deltaY;
				offX = 0.0;
			}
			for (int rc = 0; rc < Rows; ++rc)
			{
				for (int cc = 0; cc < Cols; ++cc)
				{
					currItem = Doc->m_Selection->itemAt((rc * Cols) + cc);
					if (rc == 0)
						currItem->TopLink = 0;
					else
						currItem->TopLink = Doc->m_Selection->itemAt(((rc-1)*Cols)+cc);
					if (rc == Rows-1)
						currItem->BottomLink = 0;
					else
						currItem->BottomLink = Doc->m_Selection->itemAt(((rc+1)*Cols)+cc);
					if (cc == 0)
						currItem->LeftLink = 0;
					else
						currItem->LeftLink = Doc->m_Selection->itemAt((rc*Cols)+cc-1);
					if (cc == Cols-1)
						currItem->RightLink = 0;
					else
						currItem->RightLink = Doc->m_Selection->itemAt((rc*Cols)+cc+1);
				}
			}
			emit DoGroup();
			if (UndoManager::undoEnabled())
				undoManager->commit();
		}
		if (!Prefs->stickyTools)
		{
			Doc->appMode = modeNormal;
			emit PaintingDone();
		}
		else
			emit Amode(Doc->appMode);
		emit DocChanged();
		updateContents();
		return;
	}
	if (Doc->appMode == modeDrawFreehandLine)
	{
		if (RecordP.size() > 1)
		{
			uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, Mxp, Myp, 1, 1, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenLine, !m_canvas->m_viewMode.m_MouseButtonPressed);
			currItem = Doc->Items->at(z);
			currItem->PoLine.resize(0);
			currItem->PoLine.addPoint(RecordP.point(0));
			currItem->PoLine.addPoint(RecordP.point(0));
			for (uint px = 1; px < RecordP.size()-1; ++px)
			{
				currItem->PoLine.addPoint(RecordP.point(px));
				currItem->PoLine.addPoint(RecordP.point(px));
				currItem->PoLine.addPoint(RecordP.point(px));
				currItem->PoLine.addPoint(RecordP.point(px));
			}
			currItem->PoLine.addPoint(RecordP.point(RecordP.size()-1));
			currItem->PoLine.addPoint(RecordP.point(RecordP.size()-1));
			FPoint tp2(getMinClipF(&currItem->PoLine));
			currItem->setXYPos(tp2.x(), tp2.y(), true);
			currItem->PoLine.translate(-tp2.x(), -tp2.y());
			FPoint tp(getMaxClipF(&currItem->PoLine));
			Doc->SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
			Doc->AdjustItemSize(currItem);
			Doc->m_Selection->clear();
			Doc->m_Selection->addItem(currItem);
			currItem->ClipEdited = true;
			currItem->FrameType = 3;
			currItem->OwnPage = Doc->OnPage(currItem);
			m_canvas->m_viewMode.operItemMoving = false;
			m_canvas->m_viewMode.operItemResizing = false;
			inItemCreation = false;
			updateContents(currItem->getRedrawBounding(m_canvas->scale()).adjusted(-10, -10, 20, 20));
		}
		if (!Prefs->stickyTools)
		{
			Doc->appMode = modeNormal;
			emit PaintingDone();
		}
		else
			emit Amode(Doc->appMode);
		emit DocChanged();
		return;
	}
	if ((Doc->appMode == modeEditClip) && !Doc->nodeEdit.hasNodeSelected() && (HaveSelRect))
	{
		double sc = m_canvas->scale();
		currItem = Doc->m_Selection->itemAt(0);
		Doc->nodeEdit.SelNode.clear();
		QRect Sele = QRect(Dxp, Dyp, SeRx-Dxp, SeRy-Dyp).normalized();
		FPointArray Clip;
		m_canvas->setScale(1.0);
		if (Doc->nodeEdit.isContourLine)
			Clip = currItem->ContourLine;
		else
			Clip = currItem->PoLine;
		for (uint a = 0; a < Clip.size(); ++a)
		{
			if (Clip.point(a).x() > 900000)
				continue;
			FPoint np = Clip.point(a);
			FPoint npf2 = np.transformPoint(currItem->xPos(), currItem->yPos(), currItem->rotation(), 1.0, 1.0, false);
			QPoint npf = QPoint(qRound(npf2.x()), qRound(npf2.y()));
			if ((Sele.contains(npf)) && ((a == 0) || (((a-2) % 4) == 0)))
			{
				Doc->nodeEdit.ClRe = a;
				Doc->nodeEdit.SelNode.append(a);
				Doc->nodeEdit.update(QPointF(Clip.point(a).x(), Clip.point(a).y()));
			}
		}
		m_canvas->setScale(sc);
		HaveSelRect = false;
		redrawMarker->hide();
		RefreshItem(currItem);
		Doc->nodeEdit.finishTransaction(currItem);
		m_SnapCounter = 0;
		return;
	}
/*	if (moveTimerElapsed() && (Doc->appMode == modeEditClip) && (SegP1 == -1) && (SegP2 == -1))
	{
		currItem = Doc->m_Selection->itemAt(0);
		if (operItemMoving)
		{
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			double nx = m->x()/m_canvas->scale() + Doc->minCanvasCoordinate.x();
			double ny = m->y()/m_canvas->scale() + Doc->minCanvasCoordinate.y();
			if (!Doc->ApplyGuides(&nx, &ny))
			{
				FPoint npg(ApplyGridF(FPoint(nx, ny)));
				nx = npg.x();
				ny = npg.y();
			}
			FPoint np(nx, ny, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
			Doc->nodeEdit.moveClipPoint(currItem, np);
		}
		Doc->AdjustItemSize(currItem);
		emit DocChanged();
		updateContents();
		operItemMoving = false;
		return;
	}
	if (moveTimerElapsed() && (Doc->appMode == modeEditClip) && (SegP1 != -1) && (SegP2 != -1)) */
	if (moveTimerElapsed() && (Doc->appMode == modeEditClip))
	{
		dragTimer->stop();
		Doc->nodeEdit.SegP1 = -1;
		Doc->nodeEdit.SegP2 = -1;
		currItem = Doc->m_Selection->itemAt(0);
		m_canvas->m_viewMode.operItemMoving = false;
		double xposOrig = currItem->xPos();
		double yposOrig = currItem->yPos();

		ItemState<QPair<FPointArray, FPointArray> > *state = NULL;
		
		state = Doc->nodeEdit.finishTransaction1(currItem);
		xposOrig = currItem->xPos();
		yposOrig = currItem->yPos();
		if (Doc->nodeEdit.hasNodeSelected())
		{
			double newX = m->x();
			double newY = m->y();
			FPoint np(newX-Mxp, newY-Myp, 0, 0, currItem->rotation(), 1, 1, true);
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			FPointArray Clip;
			if (Doc->nodeEdit.isContourLine)
				Clip = currItem->ContourLine;
			else
				Clip = currItem->PoLine;
			FPoint npf = FPoint(Clip.point(Doc->nodeEdit.ClRe).x() + np.x() / m_canvas->scale(), Clip.point(Doc->nodeEdit.ClRe).y() + np.y() / m_canvas->scale());
			double nx = npf.x();
			double ny = npf.y();
			nx += currItem->xPos();
			ny += currItem->yPos();
			if (!Doc->ApplyGuides(&nx, &ny))
				npf = Doc->ApplyGridF(FPoint(nx, ny));
			else
				npf = FPoint(nx, ny);
			npf = FPoint(npf.x() - currItem->xPos(), npf.y() - currItem->yPos());
			Doc->nodeEdit.moveClipPoint(currItem, npf);
		}

		Doc->AdjustItemSize(currItem);
		if (!Doc->nodeEdit.isContourLine)
			currItem->ContourLine.translate(xposOrig - currItem->xPos(), yposOrig - currItem->yPos());
		emit DocChanged();
		updateContents();
		if (state)
		{
			Doc->nodeEdit.finishTransaction2(currItem, state);
		}
		m_SnapCounter = 0;
		return;
	}
	if ((!GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!Doc->DragP) && (Doc->appMode == modeNormal))
	{
		QMenu *pmen = new QMenu();
		if ((m_ScMW->Buffer2.startsWith("<SCRIBUSELEM")) || (m_ScMW->Buffer2.contains("<SCRIBUSFRAGMENT")) || (m_ScMW->scrapbookPalette->tempBView->objectMap.count() > 0))
		{
			Mxp = m->x();
			Myp = m->y();
			if ((m_ScMW->Buffer2.startsWith("<SCRIBUSELEM")) || (m_ScMW->Buffer2.contains("<SCRIBUSFRAGMENT")))
				pmen->insertItem( tr("&Paste") , this, SLOT(PasteToPage()));
			if (m_ScMW->scrapbookPalette->tempBView->objectMap.count() > 0)
			{
				pmen3 = new QMenu();
				QMap<QString,BibView::Elem>::Iterator it;
				it = m_ScMW->scrapbookPalette->tempBView->objectMap.end();
				it--;
				for (int m = 0; m < m_ScMW->scrapbookPalette->tempBView->objectMap.count(); ++m)
				{
					QString strippedName = it.key();
					QPixmap pm = it.value().Preview;
// Qt4					pmen3->insertItem(pm, strippedName);
					pmen3->addAction(pm, strippedName);
					it--;
				}
				connect(pmen3, SIGNAL(activated(int)), this, SLOT(PasteRecentToPage(int)));
				pmen->insertItem( tr("Paste Recent"), pmen3);
			}
			pmen->insertSeparator();
		}
		setObjectUndoMode();
		pmen->addAction(m_ScMW->scrActions["editUndoAction"]);
		pmen->addAction(m_ScMW->scrActions["editRedoAction"]);
		pmen->insertSeparator();
		pmen->addAction(m_ScMW->scrActions["viewShowMargins"]);
		pmen->addAction(m_ScMW->scrActions["viewShowFrames"]);
		pmen->addAction(m_ScMW->scrActions["viewShowLayerMarkers"]);
		pmen->addAction(m_ScMW->scrActions["viewShowImages"]);
		pmen->addAction(m_ScMW->scrActions["viewShowGrid"]);
		pmen->addAction(m_ScMW->scrActions["viewShowGuides"]);
		pmen->addAction(m_ScMW->scrActions["viewShowBaseline"]);
		pmen->addAction(m_ScMW->scrActions["viewShowTextChain"]);
		pmen->addAction(m_ScMW->scrActions["viewRulerMode"]);
		pmen->insertSeparator();
		pmen->addAction(m_ScMW->scrActions["viewSnapToGrid"]);
		pmen->addAction(m_ScMW->scrActions["viewSnapToGuides"]);
		int pgNum = -1;
		int docPageCount = static_cast<int>(Doc->Pages->count() - 1);
		double bleedRight = 0.0;
		double bleedLeft = 0.0;
		double bleedBottom = 0.0;
		double bleedTop = 0.0;
		bool drawBleed = false;
		int x2 = static_cast<int>(translateToDoc(m->x(), m->y()).x());
		int y2 = static_cast<int>(translateToDoc(m->x(), m->y()).y());
		if (((Doc->bleeds.Bottom != 0.0) || (Doc->bleeds.Top != 0.0) || (Doc->bleeds.Left != 0.0) || (Doc->bleeds.Right != 0.0)) && (Doc->guidesSettings.showBleed))
			drawBleed = true;
		for (int a = docPageCount; a > -1; a--)
		{
			if (drawBleed)
				Doc->getBleeds(a, &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
			int x = static_cast<int>(Doc->Pages->at(a)->xOffset() - bleedLeft);
			int y = static_cast<int>(Doc->Pages->at(a)->yOffset() - bleedTop);
			int w = static_cast<int>(Doc->Pages->at(a)->width() + bleedLeft + bleedRight);
			int h = static_cast<int>(Doc->Pages->at(a)->height() + bleedBottom + bleedTop);
			if (QRect(x, y, w, h).contains(x2, y2))
			{
				pgNum = static_cast<int>(a);
				if (drawBleed)  // check again if its really on the correct page
				{
					for (int a2 = docPageCount; a2 > -1; a2--)
					{
						int xn = static_cast<int>(Doc->Pages->at(a2)->xOffset());
						int yn = static_cast<int>(Doc->Pages->at(a2)->yOffset());
						int wn = static_cast<int>(Doc->Pages->at(a2)->width());
						int hn = static_cast<int>(Doc->Pages->at(a2)->height());
						if (QRect(xn, yn, wn, hn).contains(x2, y2))
						{
							pgNum = static_cast<int>(a2);
							break;
						}
					}
				}
				break;
			}
		}
		if (pgNum != -1)
		{
			pmen->insertSeparator();
			pmen->addAction(m_ScMW->scrActions["pageApplyMasterPage"]);
			pmen->addAction(m_ScMW->scrActions["pageManageGuides"]);
			pmen->addAction(m_ScMW->scrActions["pageManageMargins"]);
			pmen->insertSeparator();
			pmen->addAction(m_ScMW->scrActions["pageDelete"]);
		}
		pmen->exec(QCursor::pos());
		setGlobalUndoMode();
		delete pmen;
		pmen=NULL;
		if (m_ScMW->scrapbookPalette->tempBView->objectMap.count() > 0)
		{
			delete pmen3;
			pmen3=NULL;
		}
		return;
	}
	if ((Doc->appMode != modeMagnifier) && (!Doc->appMode != modeEditClip) && (Doc->appMode != modeDrawBezierLine))
	{
		if ((GetItem(&currItem)) && (m->button() == Qt::RightButton) && (!Doc->DragP))
		{
			QMenu *pmen = new QMenu();
			QMenu *pmen2 = new QMenu();
			pmen3 = new QMenu();
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			QMenu *pmen4 = new QMenu();
			QMenu *pmenEditContents = new QMenu();
			QMenu *pmenLevel = new QMenu();
			QMenu *pmenPDF = new QMenu();
			pmenResolution = new QMenu();
			setObjectUndoMode();
			if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::PathText))
			{
				QFrame *InfoGroup = new QFrame( this );
				InfoGroup->setFrameShape( QFrame::NoFrame );
				QGridLayout *InfoGroupLayout = new QGridLayout( InfoGroup );
				InfoGroupLayout->setAlignment( Qt::AlignTop );
				InfoGroupLayout->setSpacing( 2 );
				InfoGroupLayout->setMargin( 0 );
				QString txtC, txtC2;
				QLabel *InfoT = new QLabel(InfoGroup, "ct");
				QLabel *LinCT = new QLabel(InfoGroup, "lt");
				QLabel *LinC = new QLabel(InfoGroup, "lc");
				QLabel *ParCT = new QLabel(InfoGroup, "pt");
				QLabel *ParC = new QLabel(InfoGroup, "pc");
				QLabel *WordCT = new QLabel(InfoGroup, "wt");
				QLabel *WordC = new QLabel(InfoGroup, "wc");
				QLabel *CharCT = new QLabel(InfoGroup, "ct");
				QLabel *CharC = new QLabel(InfoGroup, "cc");
				QLabel *ColCT = new QLabel(InfoGroup, "ct");
				QLabel *ColC = new QLabel(InfoGroup, "cc");
				QLabel *PrintCT = new QLabel(InfoGroup, "nt"); // <a.l.e>
				QLabel *PrintC = new QLabel(InfoGroup, "nc"); // </a.l.e>
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					//TODO (Herm): Handle latex frame (show plain tex, etc)
					LinC->hide();
					LinCT->hide();
					if (currItem->PicAvail)
					{
						QFileInfo fi = QFileInfo(currItem->Pfile);
						InfoT->setText( tr("Picture"));
						InfoGroupLayout->addWidget( InfoT, 0, 0, 1, 2, Qt::AlignHCenter );
						ParCT->setText( tr("File: "));
						InfoGroupLayout->addWidget( ParCT, 1, 0, Qt::AlignRight );
						ParC->setText(fi.fileName());
						InfoGroupLayout->addWidget( ParC, 1, 1 );
						WordCT->setText( tr("Original PPI: "));
						InfoGroupLayout->addWidget( WordCT, 2, 0, Qt::AlignRight );
						WordC->setText(txtC.setNum(qRound(currItem->pixm.imgInfo.xres))+" x "+txtC2.setNum(qRound(currItem->pixm.imgInfo.yres)));
						InfoGroupLayout->addWidget( WordC, 2, 1 );
						CharCT->setText( tr("Actual PPI: "));
						InfoGroupLayout->addWidget( CharCT, 3, 0, Qt::AlignRight );
						CharC->setText(txtC.setNum(qRound(72.0 / currItem->imageXScale()))+" x "+ txtC2.setNum(qRound(72.0 / currItem->imageYScale())));
						InfoGroupLayout->addWidget( CharC, 3, 1 );
						ColCT->setText( tr("Colorspace: "));
						InfoGroupLayout->addWidget( ColCT, 4, 0, Qt::AlignRight );
						QString cSpace;
						QString ext = fi.extension(false).toLower();
						if ((extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext)) && (currItem->pixm.imgInfo.type != 7))
							cSpace = tr("Unknown");
						else
							cSpace=colorSpaceText(currItem->pixm.imgInfo.colorspace);
						ColC->setText(cSpace);
						InfoGroupLayout->addWidget( ColC, 4, 1 );
					}
					else
					{
						InfoT->setText( tr("Picture"));
						InfoGroupLayout->addWidget( InfoT, 0, 0, 1, 2, Qt::AlignHCenter );
						ParCT->setText( tr("No Image Loaded"));
						InfoGroupLayout->addWidget( ParCT, 1, 0, 1, 2, Qt::AlignHCenter );
						ParC->hide();
						WordCT->hide();
						WordC->hide();
						CharCT->hide();
						CharC->hide();
						ColCT->hide();
						ColC->hide();
					}
				}
				if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
				{
					int Parag = 0;
					int Words = 0;
					int Chara = 0;
					int ParagN = 0;
					int WordsN = 0;
					int CharaN = 0;
					ColC->hide();
					ColCT->hide();
					if (currItem->itemType() == PageItem::TextFrame)
					{
						if ((currItem->nextInChain() != 0) || (currItem->prevInChain() != 0))
							InfoT->setText( tr("Linked Text"));
						else
							InfoT->setText( tr("Text Frame"));
					}
					else
						InfoT->setText( tr("Text on a Path"));
					InfoGroupLayout->addWidget( InfoT, 0, 0, 1, 2, Qt::AlignCenter );
					WordAndPara(currItem, &Words, &Parag, &Chara, &WordsN, &ParagN, &CharaN);
					ParCT->setText( tr("Paragraphs: "));
					InfoGroupLayout->addWidget( ParCT, 1, 0, Qt::AlignRight );
					if (ParagN != 0)
						ParC->setText(txtC.setNum(Parag+ParagN)+" ("+txtC2.setNum(ParagN)+")");
					else
						ParC->setText(txtC.setNum(Parag));
					InfoGroupLayout->addWidget( ParC, 1, 1 );
					LinCT->setText( tr("Lines: "));
					InfoGroupLayout->addWidget( LinCT, 2, 0, Qt::AlignRight );
					LinC->setText(txtC.setNum(currItem->itemText.lines()));
					InfoGroupLayout->addWidget( LinC, 2, 1 );
					WordCT->setText( tr("Words: "));
					InfoGroupLayout->addWidget( WordCT, 3, 0, Qt::AlignRight );
					if (WordsN != 0)
						WordC->setText(txtC.setNum(Words+WordsN)+" ("+txtC2.setNum(WordsN)+")");
					else
						WordC->setText(txtC.setNum(Words));
					InfoGroupLayout->addWidget( WordC, 3, 1 );
					CharCT->setText( tr("Chars: "));
					InfoGroupLayout->addWidget( CharCT, 4, 0, Qt::AlignRight );
					if (CharaN != 0)
						CharC->setText(txtC.setNum(Chara+CharaN)+" ("+txtC2.setNum(CharaN)+")");
					else
						CharC->setText(txtC.setNum(Chara));
					InfoGroupLayout->addWidget( CharC, 4, 1 );
				}

				int row = InfoGroupLayout->rowCount(); // <a.l.e>

				PrintCT->setText( tr("Print: "));
				InfoGroupLayout->addWidget( PrintCT, row, 0, Qt::AlignRight );
				if (currItem->printEnabled())
					PrintC->setText( tr("Enabled"));
				else
					PrintC->setText( tr("Disabled"));
				InfoGroupLayout->addWidget( PrintC, row, 1 ); // </a.l.e>
				QWidgetAction* MenAct = new QWidgetAction(this);
				MenAct->setDefaultWidget(InfoGroup);
				pmen4->addAction(MenAct);

// Qt4				pmen4->insertItem(InfoGroup);
				if ((currItem->itemType() == PageItem::ImageFrame) && (currItem->pixm.imgInfo.exifDataValid))
					pmen4->addAction(m_ScMW->scrActions["itemImageInfo"]);
				pmen->insertItem( tr("In&fo"), pmen4);
			}
			pmen->insertSeparator();
			pmen->addAction(m_ScMW->scrActions["editUndoAction"]);
			pmen->addAction(m_ScMW->scrActions["editRedoAction"]);
			if (currItem->itemType() == PageItem::ImageFrame ||
				currItem->itemType() == PageItem::TextFrame ||
				currItem->itemType() == PageItem::PathText)
			{
				/* Common submenu for latex and image frames*/
				pmenResolution->addAction(m_ScMW->scrActions["itemImageIsVisible"]);
				pmenResolution->insertSeparator();
				pmenResolution->addAction(m_ScMW->scrActions["itemPreviewLow"]);
				pmenResolution->addAction(m_ScMW->scrActions["itemPreviewNormal"]);
				pmenResolution->addAction(m_ScMW->scrActions["itemPreviewFull"]);
				pmen->insertSeparator();
				if (currItem->asLatexFrame()) 
				{
					pmen->addAction(m_ScMW->scrActions["itemAdjustFrameToImage"]);
					pmen->addAction(m_ScMW->scrActions["itemUpdateImage"]);
					pmen->addAction(m_ScMW->scrActions["editEditWithLatexEditor"]);
					pmen->insertItem( tr("Preview Settings"), pmenResolution);

				} else if (currItem->itemType() == PageItem::ImageFrame)
				{
					pmen->addAction(m_ScMW->scrActions["fileImportImage"]);
					if (currItem->PicAvail)
					{
						if (!currItem->isTableItem)
							pmen->addAction(m_ScMW->scrActions["itemAdjustFrameToImage"]);
						if (currItem->pixm.imgInfo.valid)
							pmen->addAction(m_ScMW->scrActions["itemExtendedImageProperties"]);
						pmen->addAction(m_ScMW->scrActions["itemUpdateImage"]);
					}
					pmen->insertItem( tr("Preview Settings"), pmenResolution);
					if (currItem->PicAvail && currItem->isRaster)
					{
						pmen->addAction(m_ScMW->scrActions["styleImageEffects"]);
						pmen->addAction(m_ScMW->scrActions["editEditWithImageEditor"]);
					}
				}
				if (currItem->itemType() == PageItem::TextFrame)
				{
					pmen->addAction(m_ScMW->scrActions["fileImportText"]);
					pmen->addAction(m_ScMW->scrActions["fileImportAppendText"]);
					pmen->addAction(m_ScMW->scrActions["toolsEditWithStoryEditor"]);
					pmen->addAction(m_ScMW->scrActions["insertSampleText"]);
				}
				if (currItem->itemType() == PageItem::PathText)
					pmen->addAction(m_ScMW->scrActions["toolsEditWithStoryEditor"]);
			}
			if (Doc->m_Selection->count() == 1)
			{
				pmen->insertSeparator();
				m_ScMW->scrActions["itemAttributes"]->addTo(pmen);
			}	
			if (currItem->itemType() == PageItem::TextFrame)
			{
				if (Doc->currentPage()->pageName().isEmpty())
				{
					pmenPDF->addAction(m_ScMW->scrActions["itemPDFIsAnnotation"]);
					pmenPDF->addAction(m_ScMW->scrActions["itemPDFIsBookmark"]);
					if (currItem->isAnnotation())
					{
						if ((currItem->annotation().Type() == 0) || (currItem->annotation().Type() == 1) || (currItem->annotation().Type() > 9))
							pmenPDF->addAction(m_ScMW->scrActions["itemPDFAnnotationProps"]);
						else
							pmenPDF->addAction(m_ScMW->scrActions["itemPDFFieldProps"]);
					}
				}
				pmen->insertItem( tr("&PDF Options"), pmenPDF);
			}
			pmen->insertSeparator();
			pmen->addAction(m_ScMW->scrActions["itemLock"]);
			pmen->addAction(m_ScMW->scrActions["itemLockSize"]);
			if (!currItem->isSingleSel)
			{
				pmen->addAction(m_ScMW->scrActions["itemSendToScrapbook"]);
				pmen->addAction(m_ScMW->scrActions["itemSendToPattern"]);
				if (Doc->layerCount() > 1)
				{
					QMap<int,int> layerMap;
					for (ScLayers::iterator it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it)
						layerMap.insert((*it).Level, (*it).LNr);
					int i=layerMap.count()-1;
					while (i>=0)
					{
						if (Doc->layerLocked(layerMap[i]))
							m_ScMW->scrLayersActions[QString::number(layerMap[i])]->setEnabled(false);
						else
							m_ScMW->scrLayersActions[QString::number(layerMap[i])]->setEnabled(true);
						m_ScMW->scrLayersActions[QString::number(layerMap[i--])]->addTo(pmen3);
					}

					pmen->insertItem( tr("Send to La&yer"), pmen3);
				}
			}
			if (Doc->m_Selection->count() > 1)
			{
				bool isGroup = true;
				int firstElem = -1;
				if (currItem->Groups.count() != 0)
					firstElem = currItem->Groups.top();
				for (int bx = 0; bx < Doc->m_Selection->count(); ++bx)
				{
					if (Doc->m_Selection->itemAt(bx)->Groups.count() != 0)
					{
						if (Doc->m_Selection->itemAt(bx)->Groups.top() != firstElem)
							isGroup = false;
					}
					else
						isGroup = false;
				}
				if (!isGroup)
					pmen->addAction(m_ScMW->scrActions["itemGroup"]);
			}
			if (currItem->Groups.count() != 0)
				pmen->addAction(m_ScMW->scrActions["itemUngroup"]);
			if (!currItem->locked())
			{
				if ((!currItem->isTableItem) && (!currItem->isSingleSel))
				{
					pmen->insertItem( tr("Le&vel"), pmenLevel);
					pmenLevel->addAction(m_ScMW->scrActions["itemRaiseToTop"]);
					pmenLevel->addAction(m_ScMW->scrActions["itemRaise"]);
					pmenLevel->addAction(m_ScMW->scrActions["itemLower"]);
					pmenLevel->addAction(m_ScMW->scrActions["itemLowerToBottom"]);
				}
			}
			if (Doc->appMode != modeEdit && (Doc->m_Selection->itemsAreSameType() || currItem->isSingleSel)) //Create convertTo Menu
			{
				bool insertConvertToMenu=false;
				if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
				{
					insertConvertToMenu=true;
					if (currItem->itemType() == PageItem::PathText)
						pmen2->addAction(m_ScMW->scrActions["itemConvertToOutlines"]);
					else
					{
						if (currItem->isTableItem)
						{
							m_ScMW->scrActions["itemConvertToImageFrame"]->setEnabled(true);
							pmen2->addAction(m_ScMW->scrActions["itemConvertToImageFrame"]);
						}
						if (!currItem->isTableItem)
						{
							if ((currItem->prevInChain() == 0) && (currItem->nextInChain() == 0))
								pmen->addAction(m_ScMW->scrActions["itemConvertToImageFrame"]);
							pmen2->addAction(m_ScMW->scrActions["itemConvertToOutlines"]);
							if ((currItem->prevInChain() == 0) && (currItem->nextInChain() == 0))
								pmen2->addAction(m_ScMW->scrActions["itemConvertToPolygon"]);
						}
					}
				}
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					insertConvertToMenu=true;
					m_ScMW->scrActions["itemConvertToTextFrame"]->setEnabled(true);
					pmen2->addAction(m_ScMW->scrActions["itemConvertToTextFrame"]);
					if (!currItem->isTableItem)
						pmen2->addAction(m_ScMW->scrActions["itemConvertToPolygon"]);
				}
				if (currItem->itemType() == PageItem::Polygon)
				{
					insertConvertToMenu=true;
					pmen2->addAction(m_ScMW->scrActions["itemConvertToBezierCurve"]);
					pmen2->addAction(m_ScMW->scrActions["itemConvertToImageFrame"]);
					pmen2->addAction(m_ScMW->scrActions["itemConvertToTextFrame"]);
				}
				bool insertedMenusEnabled = false;
				for (uint pc = 0; pc < pmen2->count(); pc++)
				{
					if (pmen2->isItemEnabled(pmen2->idAt(pc)))
						insertedMenusEnabled = true;
				}
				if ((insertConvertToMenu) && (insertedMenusEnabled))
					pmen->insertItem( tr("Conve&rt to"), pmen2);
			}
			pmen->insertSeparator();
			if (!currItem->locked() && !(currItem->isSingleSel))
				pmen->addAction(m_ScMW->scrActions["editCut"]);
			if (!(currItem->isSingleSel))
				pmen->addAction(m_ScMW->scrActions["editCopy"]);
			if ((Doc->appMode == modeEdit) && (m_ScMW->Buffer2.startsWith("<SCRIBUSTEXT")) && (currItem->itemType() == PageItem::TextFrame))
				pmen->addAction(m_ScMW->scrActions["editPaste"]);
			if (!currItem->locked() && (Doc->appMode != modeEdit) && (!(currItem->isSingleSel)))
				pmen->insertItem( tr("&Delete"), Doc, SLOT(itemSelection_DeleteItem()));
			if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
			{
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					if (currItem->PicAvail)
						pmenEditContents->addAction(m_ScMW->scrActions["editCopyContents"]);
					if (m_ScMW->contentsBuffer.sourceType==PageItem::ImageFrame)
					{
						pmenEditContents->addAction(m_ScMW->scrActions["editPasteContents"]);
						pmenEditContents->addAction(m_ScMW->scrActions["editPasteContentsAbs"]);
					}
					if (currItem->PicAvail)
						pmenEditContents->addAction(m_ScMW->scrActions["editClearContents"]);
					if ((currItem->PicAvail) || (m_ScMW->contentsBuffer.sourceType==PageItem::ImageFrame))
						pmen->insertItem( tr("Contents"), pmenEditContents);
				}
				else
				{
					if (currItem->itemText.lines() != 0)
					{
						pmenEditContents->addAction(m_ScMW->scrActions["editClearContents"]);
						pmen->insertItem( tr("Contents"), pmenEditContents);
					}
				}
			}
			pmen->insertSeparator();
			m_ScMW->scrActions["toolsProperties"]->addTo(pmen);

			pmen->exec(QCursor::pos());
			setGlobalUndoMode();
			delete pmen;
			delete pmen2;
			delete pmen3;
			delete pmen4;
			delete pmenEditContents;
			delete pmenLevel;
			delete pmenPDF;
			delete pmenResolution;
			pmen=NULL;
			pmen2=NULL;
			pmen3=NULL;
			pmen4=NULL;
			pmenEditContents=NULL;
			pmenLevel=NULL;
			pmenPDF=NULL;
			pmenResolution=NULL;
		}
		if ((Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
		{
			updateContents();
			if (Doc->ElemToLink != 0)
				return;
			else
			{
				Doc->appMode = modeNormal;
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				emit PaintingDone();
				return;
			}
		}
		if (inItemCreation)
		{
			currItem = Doc->m_Selection->itemAt(0);
			double itemX = 0.0;
			double itemY = 0.0;
			if (Doc->appMode == modeDrawLine)
			{
				QMatrix p;
				m_canvas->Transform(currItem, p);
				QPoint np = m->pos() * p.inverted();
//				np += QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
				np = Doc->ApplyGrid(np);
				itemX = sqrt(pow(np.x(),2.0)+pow(np.y(),2.0));
				itemY = 1.0;
			}
			else if (Doc->appMode == modeDrawRegularPolygon)
			{
				FPoint np1(translateToDoc(m->x(), m->y()));
				np1 = Doc->ApplyGridF(np1);
				itemX = fabs(np1.x() - currItem->xPos());
				itemY = fabs(np1.y() - currItem->yPos());
			}
			else
			{
				itemX = fabs(currItem->width());
				itemY = fabs(currItem->height());
			}
			if ((!moveTimerElapsed()) || ((itemX < 2.0) && (itemY < 2.0)) || ((Doc->appMode == modeDrawLine) && (itemX < 2.0)))
			{
				int lmode = 0;
				if (Doc->appMode == modeDrawLine)
					lmode = 1;
				PrefsContext* sizes = PrefsManager::instance()->prefsFile->getContext("ObjectSize");
				double xSize, ySize;
				int originPoint;
				if (lmode == 0)
				{
					xSize = sizes->getDouble("defWidth", 100.0);
					ySize = sizes->getDouble("defHeight", 100.0);
					originPoint = sizes->getInt("Origin", 0);
				}
				else
				{
					xSize = sizes->getDouble("defLength", 100.0);
					ySize = sizes->getDouble("defAngle", 0.0);
					originPoint = sizes->getInt("OriginL", 0);
				}
				bool doRemember = sizes->getBool("Remember", true);
				bool doCreate = false;
				if (m->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier))
					doCreate = true;
				else
				{
					OneClick *dia = new OneClick(this, tr("Enter Object Size"), Doc->unitIndex(), xSize, ySize, doRemember, originPoint, lmode);
					if (dia->exec())
					{
						doRemember = dia->checkRemember->isChecked();
						if (lmode == 0)
						{
							xSize = dia->spinWidth->value() / unitGetRatioFromIndex(Doc->unitIndex());
							ySize = dia->spinHeight->value() / unitGetRatioFromIndex(Doc->unitIndex());
							originPoint = dia->RotationGroup->checkedId();
							if (doRemember)
							{
								sizes->set("defWidth", xSize);
								sizes->set("defHeight", ySize);
								sizes->set("Origin", originPoint);
							}
						}
						else
						{
							xSize = dia->spinWidth->value() / unitGetRatioFromIndex(Doc->unitIndex());
							ySize = dia->spinHeight->value();
							originPoint = dia->RotationGroup->checkedId();
							if (doRemember)
							{
								sizes->set("defLength", xSize);
								sizes->set("defAngle", ySize);
								sizes->set("OriginL", originPoint);
							}
						}
						sizes->set("Remember", doRemember);
						doCreate = true;
					}
					else
					{
						Deselect(false);
						Doc->Items->removeAt(currItem->ItemNr);
					}
					delete dia;
				}
				if (doCreate)
				{
					bool oldSnap = Doc->SnapGuides;
					Doc->SnapGuides = false;
					if (Doc->appMode == modeDrawLine)
					{
						currItem->setWidthHeight(xSize, 1);
						currItem->setRotation(-ySize);
						currItem->Sizing = false;
						currItem->updateClip();
					}
					else if (Doc->appMode == modeDrawRegularPolygon)
					{
						currItem->setWidthHeight(xSize, ySize);
						FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), Doc->toolSettings.polyC, Doc->toolSettings.polyS, Doc->toolSettings.polyF, Doc->toolSettings.polyR);
						FPoint np(cli.point(0));
						currItem->PoLine.resize(2);
						currItem->PoLine.setPoint(0, np);
						currItem->PoLine.setPoint(1, np);
						for (uint ax = 1; ax < cli.size(); ++ax)
						{
							np = FPoint(cli.point(ax));
							currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
						}
						np = FPoint(cli.point(0));
						currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
						FPoint tp2(getMinClipF(&currItem->PoLine));
						if ((tp2.x() > -1) || (tp2.y() > -1))
							Doc->SizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem->ItemNr, false, false, false);
						FPoint tp(getMaxClipF(&currItem->PoLine));
						Doc->SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
//						currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
						Doc->AdjustItemSize(currItem);
					}
					else
					{
						Doc->SizeItem(xSize, ySize, currItem->ItemNr, false, false, false);
						currItem->updateClip();
					}
					currItem->ContourLine = currItem->PoLine.copy();
					switch (originPoint)
					{
						case 0:
							break;
						case 1:
							if (lmode == 0)
								Doc->MoveItem(-currItem->width(), 0.0, currItem, false);
							else
								Doc->MoveRotated(currItem, FPoint(-currItem->width() / 2.0, 0.0), false);
							break;
						case 2:
							if (lmode == 0)
								Doc->MoveItem(-currItem->width() / 2.0, -currItem->height() / 2.0, currItem, false);
							else
								Doc->MoveRotated(currItem, FPoint(-currItem->width(), 0.0), false);
							break;
						case 3:
							Doc->MoveItem(0.0, -currItem->height(), currItem, false);
							break;
						case 4:
							Doc->MoveItem(-currItem->width(), -currItem->height(), currItem, false);
							break;
					}
					Doc->setRedrawBounding(currItem);
					currItem->OwnPage = Doc->OnPage(currItem);
					currItem->OldB2 = currItem->width();
					currItem->OldH2 = currItem->height();
					currItem->Sizing = false;
					Doc->SnapGuides = oldSnap;
					if (!Prefs->stickyTools)
					{
						Doc->SubMode = -1;
						Doc->appMode = modeNormal;
						emit PaintingDone();
					}
					else
					{
						if ((inItemCreation) && (Doc->appMode == modeNormal))
						{
							currItem = Doc->m_Selection->itemAt(0);
							if (currItem->asTextFrame())
								Doc->appMode = modeDrawText;
							else if (currItem->asLatexFrame())
								Doc->appMode = modeDrawLatex;
							else if (currItem->asImageFrame())
								Doc->appMode = modeDrawPicture;
							else if (Doc->SubMode != -1)
								Doc->appMode = modeDrawShapes;
						}
						emit Amode(Doc->appMode);
					}
					emit DocChanged();
				}
				else
				{
					Doc->SubMode = -1;
					Doc->appMode = modeNormal;
					emit PaintingDone();
					emit HaveSel(-1);
				}
				inItemCreation = false;
				Doc->DragP = false;
				Doc->leaveDrag = false;
				m_canvas->m_viewMode.operItemMoving = false;
				m_canvas->m_viewMode.operItemResizing = false;
				m_canvas->setRenderModeUseBuffer(false);
				MidButt = false;
				shiftSelItems = false;
				m_SnapCounter = 0;
				updateContents();
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			}
		}
		if ((Doc->appMode == modeDrawLine) && (inItemCreation))
		{
			currItem = Doc->m_Selection->itemAt(0);
			QMatrix p;
			m_canvas->Transform(currItem, p);
			QPoint np = m->pos() * p.inverted();
//			np += QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
			np = Doc->ApplyGrid(np);
			double newRot=xy2Deg(np.x(), np.y());
			//Constrain rotation angle, when the mouse is released from drawing a line
			if (m->modifiers() & Qt::ControlModifier)
				newRot=constrainAngle(newRot, Doc->toolSettings.constrain);
			currItem->setRotation(newRot);
			currItem->setWidthHeight(sqrt(pow(np.x(),2.0)+pow(np.y(),2.0)), 1.0);
			currItem->Sizing = false;
			currItem->updateClip();
			Doc->setRedrawBounding(currItem);
			currItem->OwnPage = Doc->OnPage(currItem);
			m_canvas->m_viewMode.operItemResizing = false;
			m_canvas->setRenderModeUseBuffer(false);
			updateContents();
		}
		if ((Doc->appMode == modeDrawRegularPolygon) && (inItemCreation))
		{
			currItem = Doc->m_Selection->itemAt(0);
			FPoint np1(translateToDoc(m->x(), m->y()));
			np1 = Doc->ApplyGridF(np1);
			double w = np1.x() - currItem->xPos();
			double h = np1.y()- currItem->yPos();
			currItem->setWidthHeight(fabs(w), fabs(h));
			if (w < 0.0)
				currItem->setXPos(currItem->xPos() - fabs(w), true);
			if (h < 0.0)
				currItem->setYPos(currItem->yPos() - fabs(h), true);
			FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), Doc->toolSettings.polyC, Doc->toolSettings.polyS, Doc->toolSettings.polyF, Doc->toolSettings.polyR);
			FPoint np(cli.point(0));
			currItem->PoLine.resize(2);
			currItem->PoLine.setPoint(0, np);
			currItem->PoLine.setPoint(1, np);
			for (uint ax = 1; ax < cli.size(); ++ax)
			{
				np = FPoint(cli.point(ax));
				currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
			}
			np = FPoint(cli.point(0));
			currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
			FPoint tp2(getMinClipF(&currItem->PoLine));
			if ((tp2.x() > -1) || (tp2.y() > -1))
			{
				Doc->SizeItem(currItem->width() - tp2.x(), currItem->height() - tp2.y(), currItem->ItemNr, false, false, false);
			}
			FPoint tp(getMaxClipF(&currItem->PoLine));
			Doc->SizeItem(tp.x(), tp.y(), currItem->ItemNr, false, false, false);
//			currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
			Doc->AdjustItemSize(currItem);
			currItem->ContourLine = currItem->PoLine.copy();
			Doc->setRedrawBounding(currItem);
			currItem->OwnPage = Doc->OnPage(currItem);
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			m_canvas->m_viewMode.operItemResizing = false;
			m_canvas->setRenderModeUseBuffer(false);
			updateContents();
		}
		if (moveTimerElapsed() && (GetItem(&currItem)))
		{
			dragTimer->stop();
			m_canvas->setRenderModeUseBuffer(false);
			if (Doc->m_Selection->isMultipleSelection())
			{
				if (m_canvas->m_viewMode.operItemResizing)
				{
					double gx, gy, gh, gw, nx, ny, scx, scy;
					Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					double sc = m_canvas->scale();
					scx = sc;
					scy = sc;
					QPoint np2;
					double newXF = translateToDoc(m->x(), m->y()).x();
					double newYF = translateToDoc(m->x(), m->y()).y();
					if (m->modifiers() & Qt::ControlModifier)
						np2 = QPoint(qRound(newXF), qRound(gy+(gh * ((newXF-gx) / gw))));
					else
						np2 = QPoint(qRound(newXF), qRound(newYF));
					nx = np2.x();
					ny = np2.y();
					if (!Doc->ApplyGuides(&nx, &ny))
					{
						np2 = Doc->ApplyGrid(np2);
						nx = np2.x();
						ny = np2.y();
					}
					switch (frameResizeHandle)
					{
						case 1:
							scy = fabs(ny-gy) / gh;
							scx = fabs(nx-gx) / gw;
							break;
						case 2:
							scx = fabs(nx-(gx+gw)) / gw;
							scy = fabs(ny-(gy+gh)) / gh;
							break;
						case 3:
							scx = fabs(nx-gx) / gw;
							scy = fabs(ny-(gy+gh)) / gh;
							break;
						case 4:
							scx = fabs(nx-(gx+gw)) / gw;
							scy = fabs(ny-gy) / gh;
							break;
						case 5:
							scx = 1.0;
							scy = fabs(ny-gy) / gh;
							break;
						case 6:
							scx = fabs(nx-gx) / gw;
							scy = 1.0;
							break;
						case 7:
							scx = fabs(nx-(gx+gw)) / gw;
							scy = 1.0;
							break;
						case 8:
							scx = 1.0;
							scy = fabs(ny-(gy+gh)) / gh;
							break;
					}
					RotMode = Doc->RotMode;
					Doc->RotMode = 0;
					//CB #3012 only scale text in a group if alt is pressed
					if ((currItem->itemType() == PageItem::TextFrame) && (m->modifiers() & Qt::AltModifier))
						scaleGroup(scx, scy, true);
					else
						scaleGroup(scx, scy, false);
					if ((frameResizeHandle == 3) || (frameResizeHandle == 8))
						moveGroup(0, ny-gy);
					if (frameResizeHandle == 2)
						moveGroup(nx-gx, ny-gy);
					if ((frameResizeHandle == 7) || (frameResizeHandle == 4))
						moveGroup(nx-gx, 0);
					Doc->RotMode = RotMode;
					evSpon = false;
					m_canvas->m_viewMode.operItemResizing = false;
					updateContents();
					emit DocChanged();
				}
			}
			else
			{
				//Where a single frame resize stops when the mouse buttno is released
				//FIXME: reduce code!
				if (currItem->Sizing)
				{
					FPoint npx;
					double nx = translateToDoc(m->x(), m->y()).x();
					double ny = translateToDoc(m->x(), m->y()).y();
					if (Doc->SnapGuides)
					{
						Doc->ApplyGuides(&nx, &ny);
						npx = FPoint(nx, ny, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
					}
					else
					{
						npx = Doc->ApplyGridF(FPoint(nx, ny));
						npx = FPoint(npx.x(), npx.y(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
					}
					if ((frameResizeHandle == 1) && !(currItem->asLine()) && (Doc->SnapGuides))
						Doc->SizeItem(npx.x(), npx.y(), currItem->ItemNr);
					bool sav = Doc->SnapGuides;
					Doc->SnapGuides = false;
					if (UndoManager::undoEnabled())
					{
						undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(),
							Um::Resize, QString(Um::ResizeFromTo).arg(currItem->width()).arg(currItem->height()).arg(currItem->width() - npx.x()).arg(currItem->height() - npx.y()), Um::IResize);
					}
					m_canvas->m_viewMode.operItemResizing = false;
					switch (frameResizeHandle)
					{
					case 1:
						if (!currItem->asLine())
						{
							if (currItem->isTableItem)
							{
								double dist;
								if (currItem->LeftLink != 0)
									dist = npx.y() - currItem->LeftLink->height();
								else if (currItem->RightLink != 0)
									dist = npx.y() - currItem->RightLink->height();
								else
									dist = npx.y() - currItem->height();
								PageItem* bb2;
								PageItem* bb = currItem;
								while (bb->LeftLink != 0)
								{
									bb = bb->LeftLink;
								}
								while (bb->RightLink != 0)
								{
									bb2 = bb;
									while (bb2->BottomLink != 0)
									{
										Doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
										bb2 = bb2->BottomLink;
									}
									if (bb != currItem)
										Doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
									bb = bb->RightLink;
								}
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									Doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								if (bb != currItem)
									Doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = currItem;
								if (currItem->TopLink != 0)
									dist = npx.x() - currItem->TopLink->width();
								else if (currItem->BottomLink != 0)
									dist = npx.x() - currItem->BottomLink->width();
								else
									dist = npx.x() - currItem->width();
								while (bb->TopLink != 0)
								{
									bb = bb->TopLink;
								}
								while (bb->BottomLink != 0)
								{
									bb2 = bb;
									while (bb2->RightLink != 0)
									{
										Doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
										bb2 = bb2->RightLink;
									}
									if (bb != currItem)
										Doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
									bb = bb->BottomLink;
								}
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									Doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								if (bb != currItem)
									Doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
							}
							//<<Swap location if width/height is <0
							if (currItem->width()<0)
							{
								currItem->setWidth(-currItem->width());
								currItem->setXPos(currItem->xPos()-currItem->width());
							}
							if (currItem->height()<0)
							{
								currItem->setHeight(-currItem->height());
								currItem->setYPos(currItem->yPos()-currItem->height());
							}
							//>>
							if (currItem->imageFlippedH())
								currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
							if (currItem->imageFlippedV())
								currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						}
						else
						{
							if (sav)
							{
								double nx = translateToDoc(m->x(), m->y()).x();
								double ny = translateToDoc(m->x(), m->y()).y();
								if (Doc->useRaster)
								{
									FPoint ra(Doc->ApplyGridF(FPoint(nx, ny)));
									nx = ra.x();
									ny = ra.y();
								}
								Doc->SnapGuides = sav;
								Doc->ApplyGuides(&nx, &ny);
								Doc->SnapGuides = false;
								double r = atan2(ny - currItem->yPos(), nx - currItem->xPos())*(180.0/M_PI);
								Doc->RotateItem(r, currItem->ItemNr);
								double w = sqrt(pow(nx - currItem->xPos(), 2) + pow(ny - currItem->yPos(),2));
								Doc->SizeItem(w, currItem->height(), currItem->ItemNr, true);
							}
						}
						currItem->Sizing = false;
						break;
					case 2:
						if (!currItem->asLine())
						{
							if (currItem->isTableItem)
							{
								PageItem* bb2;
								PageItem* bb = currItem;
								while (bb->TopLink != 0)
								{
									bb = bb->TopLink;
								}
								while (bb->BottomLink != 0)
								{
									bb2 = bb;
									while (bb2->LeftLink != 0)
									{
										Doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
										bb2 = bb2->LeftLink;
									}
									Doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
									bb = bb->BottomLink;
								}
								bb2 = bb;
								while (bb2->LeftLink != 0)
								{
									Doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
									bb2 = bb2->LeftLink;
								}
								Doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
								bb = currItem;
								while (bb->LeftLink != 0)
								{
									bb = bb->LeftLink;
								}
								while (bb->RightLink != 0)
								{
									bb2 = bb;
									while (bb2->TopLink != 0)
									{
										Doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
										bb2 = bb2->TopLink;
									}
									Doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
									bb = bb->RightLink;
								}
								bb2 = bb;
								while (bb2->TopLink != 0)
								{
									Doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
									bb2 = bb2->TopLink;
								}
								Doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
							}
							else
								Doc->MoveSizeItem(npx, npx, currItem->ItemNr);
								
							//<<Swap location if width/height is <0
							if (currItem->width()<0)
							{
								currItem->setWidth(-currItem->width());
								currItem->setXPos(currItem->xPos()-currItem->width());
							}
							if (currItem->height()<0)
							{
								currItem->setHeight(-currItem->height());
								currItem->setYPos(currItem->yPos()-currItem->height());
							}
							//>>
								
							currItem->Sizing = false;
							if (!currItem->imageFlippedH())
							{
								currItem->moveImageInFrame((currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
								updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							}
							if (!currItem->imageFlippedV())
							{
								currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
								updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							}
						}
						else
						{
							if (sav)
							{
								double nx = translateToDoc(m->x(), m->y()).x();
								double ny = translateToDoc(m->x(), m->y()).y();
								if (Doc->useRaster)
								{
									FPoint ra(Doc->ApplyGridF(FPoint(nx, ny)));
									nx = ra.x();
									ny = ra.y();
								}
								Doc->SnapGuides = sav;
								Doc->ApplyGuides(&nx, &ny);
								Doc->SnapGuides = false;
								QMatrix ma;
								ma.translate(currItem->xPos(), currItem->yPos());
								ma.rotate(currItem->rotation());
								double mx = ma.m11() * currItem->width() + ma.m21() * currItem->height() + ma.dx();
								double my = ma.m22() * currItem->height() + ma.m12() * currItem->width() + ma.dy();
								double r = atan2(my-ny,mx-nx)*(180.0/M_PI);
								double w = sqrt(pow(mx-nx,2)+pow(my-ny,2));
								Doc->MoveItem(nx - currItem->xPos(), ny - currItem->yPos(), currItem, true);
								Doc->SizeItem(w, currItem->height(), currItem->ItemNr, true);
								Doc->RotateItem(r, currItem->ItemNr);
								currItem->Sizing = false;
							}
						}
						break;
					case 3:
						if (currItem->isTableItem)
						{
							double dist = npx.x() - currItem->width();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									Doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								Doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->RightLink != 0)
							{
								Doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
								bb2 = bb2->RightLink;
							}
							Doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
							bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->TopLink != 0)
								{
									Doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
									bb2 = bb2->TopLink;
								}
								Doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->TopLink != 0)
							{
								Doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
								bb2 = bb2->TopLink;
							}
							Doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
						}
						else
							Doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(currItem->width() - npx.x(), npx.y()), currItem->ItemNr);
							
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (currItem->imageFlippedH())
							currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (!currItem->imageFlippedV())
							currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					case 4:
					
						if (currItem->isTableItem)
						{
							double dist = npx.y() - currItem->height();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									Doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								Doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->BottomLink != 0)
							{
								Doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
								bb2 = bb2->BottomLink;
							}
							Doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
							bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->LeftLink != 0)
								{
									Doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
									bb2 = bb2->LeftLink;
								}
								Doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->LeftLink != 0)
							{
								Doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
								bb2 = bb2->LeftLink;
							}
							Doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
						}
						else
							Doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), currItem->height() - npx.y()), currItem->ItemNr);
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (!currItem->imageFlippedH())
							currItem->moveImageInFrame((currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (currItem->imageFlippedV())
							currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					case 5:
						if (currItem->isTableItem)
						{
							double dist = npx.y() - currItem->height();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->BottomLink != 0)
								{
									Doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
									bb2 = bb2->BottomLink;
								}
								Doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->BottomLink != 0)
							{
								Doc->MoveRotated(bb2->BottomLink, FPoint(0, dist));
								bb2 = bb2->BottomLink;
							}
							Doc->MoveSizeItem(FPoint(0, 0), FPoint(0, -dist), bb->ItemNr);
						}
						else
							Doc->MoveSizeItem(FPoint(0, 0), FPoint(0, currItem->height() - npx.y()), currItem->ItemNr);
						if (currItem->imageFlippedV())
							currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						break;
					case 6:
						if (currItem->isTableItem)
						{
							double dist = npx.x() - currItem->width();
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->RightLink != 0)
								{
									Doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
									bb2 = bb2->RightLink;
								}
								Doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->RightLink != 0)
							{
								Doc->MoveRotated(bb2->RightLink, FPoint(dist, 0));
								bb2 = bb2->RightLink;
							}
							Doc->MoveSizeItem(FPoint(0, 0), FPoint(-dist, 0), bb->ItemNr);
						}
						else
							Doc->MoveSizeItem(FPoint(0, 0), FPoint(currItem->width() - npx.x(), 0), currItem->ItemNr);
						if (currItem->imageFlippedH())
							currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
						
						currItem->Sizing = false;
						break;
					case 7:
						if (currItem->isTableItem)
						{
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->TopLink != 0)
							{
								bb = bb->TopLink;
							}
							while (bb->BottomLink != 0)
							{
								bb2 = bb;
								while (bb2->LeftLink != 0)
								{
									Doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
									bb2 = bb2->LeftLink;
								}
								Doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
								bb = bb->BottomLink;
							}
							bb2 = bb;
							while (bb2->LeftLink != 0)
							{
								Doc->MoveRotated(bb2->LeftLink, FPoint(npx.x(), 0));
								bb2 = bb2->LeftLink;
							}
							Doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), bb->ItemNr);
						}
						else
							Doc->MoveSizeItem(FPoint(npx.x(), 0), FPoint(npx.x(), 0), currItem->ItemNr);
						
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (!currItem->imageFlippedH())
							currItem->moveImageInFrame((currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (currItem->imageFlippedV())
							currItem->moveImageInFrame(0, -(currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					case 8:
						if (currItem->isTableItem)
						{
							PageItem* bb2;
							PageItem* bb = currItem;
							while (bb->LeftLink != 0)
							{
								bb = bb->LeftLink;
							}
							while (bb->RightLink != 0)
							{
								bb2 = bb;
								while (bb2->TopLink != 0)
								{
									Doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
									bb2 = bb2->TopLink;
								}
								Doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
								bb = bb->RightLink;
							}
							bb2 = bb;
							while (bb2->TopLink != 0)
							{
								Doc->MoveRotated(bb2->TopLink, FPoint(0, npx.y()));
								bb2 = bb2->TopLink;
							}
							Doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), bb->ItemNr);
						}
						else
							Doc->MoveSizeItem(FPoint(0, npx.y()), FPoint(0, npx.y()), currItem->ItemNr);
							
						//<<Swap location if width/height is <0
						if (currItem->width()<0)
						{
							currItem->setWidth(-currItem->width());
							currItem->setXPos(currItem->xPos()-currItem->width());
						}
						if (currItem->height()<0)
						{
							currItem->setHeight(-currItem->height());
							currItem->setYPos(currItem->yPos()-currItem->height());
						}
						//>>
							
						currItem->Sizing = false;
						if (currItem->imageFlippedH())
							currItem->moveImageInFrame(-(currItem->width() - currItem->OldB2)/currItem->imageXScale(), 0);
						if (!currItem->imageFlippedV())
							currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
						break;
					}

					//TextFrame resize - Resize text with resize of frame
					//alt resize, free resize with text scaling
					//shift alt, square resize with text scaling
					//control alt, proportional resize with text scaling
					//if ((currItem->itemType() == PageItem::TextFrame) && (m->state() & Qt::ShiftModifier) && (m->state() & Qt::ControlModifier))
					//NOTE: this does not cover groups, strangely enough. Thats done in scaleGroup()
					if ((currItem->itemType() == PageItem::TextFrame) && (m->modifiers() & Qt::AltModifier))
					{
						double scx = currItem->width() / currItem->OldB2;
						double scy = currItem->height() / currItem->OldH2;
						scx = scx != scy ? scx / scy : 1.0;
						if (currItem->itemText.length() != 0)
						{
//							currItem->setFontSize(qMax(qRound(currItem->fontSize() * scy), 1));
//							currItem->setLineSpacing(((currItem->fontSize() / 10.0)* static_cast<double>(Doc->typographicSettings.autoLineSpacing) / 100) + (currItem->fontSize() / 10.0));
//							currItem->TxtScale = qMin(qMax(qRound(currItem->TxtScale * scx), 25), 400);
/*							Doc->currTextScale = currItem->TxtScale;
							Doc->CurrFontSize = currItem->fontSize();
							Doc->CurrTextBase = currItem->TxtBase;
							Doc->CurrTextShadowX = currItem->TxtShadowX;
							Doc->CurrTextShadowY = currItem->TxtShadowY;
							Doc->CurrTextOutline = currItem->TxtOutline;
							Doc->CurrTextUnderPos = currItem->TxtUnderPos;
							Doc->CurrTextUnderWidth = currItem->TxtUnderWidth;
							Doc->CurrTextStrikePos = currItem->TxtStrikePos;
							Doc->CurrTextStrikeWidth = currItem->TxtStrikeWidth;
							emit ItemTextStrike(currItem->TxtStrikePos, currItem->TxtStrikeWidth);
							emit ItemTextUnderline(currItem->TxtUnderPos, currItem->TxtUnderWidth);
							emit ItemTextOutline(currItem->TxtOutline);
							emit ItemTextShadow(currItem->TxtShadowX, currItem->TxtShadowY);
							//emit ItemTextAttr(currItem->LineSp);
							emit ItemTextCols(currItem->Cols, currItem->ColGap);
							emit ItemTextSize(currItem->fontSize());
							emit ItemTextSca(currItem->TxtScale);
							emit ItemTextScaV(currItem->TxtScaleV);
							emit ItemTextBase(currItem->TxtBase);
*/
#ifndef NLS_PROTO
							for (int aa = 0; aa < currItem->itemText.length(); ++aa)
							{
								currItem->itemText.item(aa)->setFontSize(qMax(qRound(currItem->itemText.item(aa)->fontSize()*scy), 1));
								currItem->itemText.item(aa)->setScaleH(qMax(qMin(qRound(currItem->itemText.item(aa)->scaleH()*scx), 4000), 100));
							}
#endif
						}
					}
					if (currItem->itemType() == PageItem::ImageFrame)
					{
						currItem->AdjustPictScale();
						/*Dont need this now adjustpictscale will emit for 1st selected item
						emit SetLocalValues(currItem->LocalScX, currItem->LocalScY, currItem->LocalX, currItem->LocalY );
						*/
					}
					if (currItem->itemType() == PageItem::TextFrame)
						currItem->invalid = true;
					currItem->updateClip();
					emit ItemTextCols(currItem->Cols, currItem->ColGap);
					Doc->SnapGuides = sav;
					evSpon = false;
					updateContents();
					emit DocChanged();
					currItem->checkChanges();
					if (UndoManager::undoEnabled())
						undoManager->commit();
				}
				Doc->setRedrawBounding(currItem);
				currItem->OwnPage = Doc->OnPage(currItem);
				m_canvas->m_viewMode.operItemResizing = false;
				if (currItem->asLine())
					updateContents();
			}
			if (m_canvas->m_viewMode.operItemMoving)
			{
				updatesOn(false);
				evSpon = false;
				if (Doc->m_Selection->isMultipleSelection())
				{
					Doc->m_Selection->setGroupRect();
					double gx, gy, gh, gw;
					Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					double nx = gx;
					double ny = gy;
					if (!Doc->ApplyGuides(&nx, &ny))
					{
							FPoint npx = Doc->ApplyGridF(FPoint(gx, gy));
							FPoint npw = Doc->ApplyGridF(FPoint(gx+gw, gy+gh));
							if ((fabs(gx-npx.x())) > (fabs((gx+gw)-npw.x())))
								nx = npw.x() - gw;
							else
								nx = npx.x();
							if ((fabs(gy-npx.y())) > (fabs((gy+gh)-npw.y())))
								ny = npw.y() - gh;
							else
								ny = npx.y();
					}
					moveGroup(nx-gx, ny-gy, false);
					Doc->m_Selection->setGroupRect();
					Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					nx = gx+gw;
					ny = gy+gh;
					if (Doc->ApplyGuides(&nx, &ny))
						moveGroup(nx-(gx+gw), ny-(gy+gh), false);
					Doc->m_Selection->setGroupRect();
				}
				else
				{
					currItem = Doc->m_Selection->itemAt(0);
/*					if (!currItem->asLine())
					{
						if (fabs(currItem->width()) < 5)
							currItem->setWidth(5.0);
						if (fabs(currItem->height()) < 5)
							currItem->setHeight(5.0);
					} */
					if (Doc->useRaster)
					{
						double nx = currItem->xPos();
						double ny = currItem->yPos();
						if (!Doc->ApplyGuides(&nx, &ny))
						{
							Doc->m_Selection->setGroupRect();
							double gx, gy, gh, gw;
							Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
							FPoint npx = Doc->ApplyGridF(FPoint(gx, gy));
							FPoint npw = Doc->ApplyGridF(FPoint(gx+gw, gy+gh));
							if ((fabs(gx-npx.x())) > (fabs((gx+gw)-npw.x())))
								nx = npw.x() - gw;
							else
								nx = npx.x();
							if ((fabs(gy-npx.y())) > (fabs((gy+gh)-npw.y())))
								ny = npw.y() - gh;
							else
								ny = npx.y();
						}
						Doc->MoveItem(nx-currItem->xPos(), ny-currItem->yPos(), currItem);
					}
					else
						Doc->MoveItem(0, 0, currItem, false);
				}
				m_canvas->m_viewMode.operItemMoving = false;
				if (Doc->m_Selection->isMultipleSelection())
				{
					double gx, gy, gh, gw;
					Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					FPoint maxSize(gx+gw+Doc->scratch.Right, gy+gh+Doc->scratch.Bottom);
					FPoint minSize(gx-Doc->scratch.Left, gy-Doc->scratch.Top);
					Doc->adjustCanvas(minSize, maxSize);
				}
				Doc->setRedrawBounding(currItem);
				currItem->OwnPage = Doc->OnPage(currItem);
				if (currItem->OwnPage != -1)
				{
					Doc->setCurrentPage(Doc->Pages->at(currItem->OwnPage));
					setMenTxt(currItem->OwnPage);
				}
				//CB done with emitAllToGUI
				//emit HaveSel(currItem->itemType());
				//EmitValues(currItem);
				//CB need this for? a moved item will send its new data with the new xpos/ypos emits
				//CB TODO And what if we have dragged to a new page. Items X&Y are not updated anyway now
				//currItem->emitAllToGUI();
				updatesOn(true);
				updateContents();
				emit DocChanged();
			}
		}
		//CB Drag selection performed here
		if (((Doc->m_Selection->count() == 0) && (HaveSelRect) && (!MidButt)) || ((shiftSelItems) && (HaveSelRect) && (!MidButt)))
		{
			QRectF Sele = QRectF(Dxp, Dyp, SeRx-Dxp, SeRy-Dyp).normalized();
			if (!Doc->masterPageMode())
			{
				uint docPagesCount=Doc->Pages->count();
				uint docCurrPageNo=Doc->currentPageNumber();
				for (uint i = 0; i < docPagesCount; ++i)
				{
					if (QRectF(Doc->Pages->at(i)->xOffset(), Doc->Pages->at(i)->yOffset(), Doc->Pages->at(i)->width(), Doc->Pages->at(i)->height()).intersects(Sele))
					{
						if (docCurrPageNo != i)
						{
							Doc->setCurrentPage(Doc->Pages->at(i));
							setMenTxt(i);
						}
						break;
					}
				}
				setRulerPos(contentsX(), contentsY());
			}
			int docItemCount=Doc->Items->count();
			if (docItemCount != 0)
			{
				Doc->m_Selection->setIsGUISelection(false);
				for (int a = 0; a < docItemCount; ++a)
				{
					PageItem* docItem = Doc->Items->at(a);
					QMatrix p;
					m_canvas->Transform(docItem, p);
					QRegion apr = QRegion(docItem->Clip * p);
					QRect apr2(docItem->getRedrawBounding(1.0));
					if ((Doc->masterPageMode()) && (docItem->OnMasterPage != Doc->currentPage()->pageName()))
						continue;
					//CB Finally Items are selected here
					if (((Sele.contains(apr.boundingRect())) || (Sele.contains(apr2))) && (docItem->LayerNr == Doc->activeLayer()) && (!Doc->layerLocked(docItem->LayerNr)))
					{
					//CB set draw to true to (dis)enable some actions via emit to HaveNewSel in scapp.
					//CB FIXME emit from selection when multiple selected instead
						//CB #3523 bool redrawSelection=(a==docItemCount-1);
						bool redrawSelection=false;
						SelectItemNr(a, redrawSelection);
					}
				}
				Doc->m_Selection->setIsGUISelection(true);
				Doc->m_Selection->connectItemToGUI();
				if (Doc->m_Selection->count() > 1)
				{
					Doc->m_Selection->setGroupRect();
					double x, y, w, h;
					Doc->m_Selection->getGroupRect(&x, &y, &w, &h);
					emit ItemPos(x, y);
					emit ItemGeom(w, h);
					getGroupRectScreen(&x, &y, &w, &h);
				}
				if (Doc->m_Selection->count() > 0)
					emit HaveSel(Doc->m_Selection->itemAt(0)->itemType());
			}
			HaveSelRect = false;
			shiftSelItems = false;
			redrawMarker->hide();
			updateContents();
		}
		if (Doc->appMode != modeEdit)
		{
			if (Doc->appMode == modeRotation)
				Doc->RotMode = RotMode;
			if (!Prefs->stickyTools)
			{
				Doc->appMode = modeNormal;
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				emit PaintingDone();
			}
			else
			{
				if ((inItemCreation) && (Doc->appMode == modeNormal))
				{
					currItem = Doc->m_Selection->itemAt(0);
					if (currItem->asTextFrame())
						Doc->appMode = modeDrawText;
					else if (currItem->asLatexFrame())
						Doc->appMode = modeDrawLatex;
					else if (currItem->asImageFrame())
						Doc->appMode = modeDrawPicture;
					else if (Doc->SubMode != -1)
						Doc->appMode = modeDrawShapes;
				}
				emit Amode(Doc->appMode);
			}
		}
		if (GetItem(&currItem))
		{
			if (Doc->m_Selection->count() > 1)
			{
				Doc->m_Selection->setGroupRect();
				//CB shouldnt need this i think
				//emit HaveSel(currItem->itemType());
				double x, y, w, h;
				Doc->m_Selection->getGroupRect(&x, &y, &w, &h);
				emit ItemPos(x, y);
				emit ItemGeom(w, h);
				getGroupRectScreen(&x, &y, &w, &h);
				
				m_canvas->m_viewMode.operItemMoving = false;
				m_canvas->m_viewMode.operItemResizing = false;
				updateContents(QRect(static_cast<int>(x+contentsX()-5), static_cast<int>(y+contentsY()-5), static_cast<int>(w+10), static_cast<int>(h+10)));
			}
			else
				currItem->emitAllToGUI();
				
			/*else
			{
				//CB Dont think we need this here with the new selection code
				//For a select, deselect operation, this will cause 2x emit
				//emit HaveSel(currItem->itemType());
				//EmitValues(currItem);
				//currItem->emitAllToGUI();
			}*/
		}
	}
	
	m_canvas->m_viewMode.operItemResizeInEditMode = false;
	if (Doc->appMode == modeEdit)
	{
		currItem = Doc->m_Selection->itemAt(0);
		if (currItem->asTextFrame())
		{
			if (oldCp == currItem->CPos)
			{
				currItem->itemText.deselectAll();
				currItem->HasSel = false;
				emit HasNoTextSel();
			}
		}
		else
			emit DocChanged();
		RefreshItem(currItem);
	}
	if (Doc->appMode == modeMagnifier)
	{
		double sc = m_canvas->scale();
		if (HaveSelRect)
		{
			QRect geom = redrawMarker->geometry().normalized();
			QPoint xp = viewport()->mapFromGlobal(QPoint(geom.x() + geom.width() / 2, geom.y() + geom.height() / 2));
			FPoint nx = translateToDoc(xp.x()+contentsX(), xp.y()+contentsY());
			double scaleAdjust = visibleWidth() / static_cast<double>(qMax(geom.width(), 1));
			setScale(m_canvas->scale() * scaleAdjust);
			slotDoZoom();
			SetCCPo(nx.x(), nx.y());
			if (sc == m_canvas->scale())
			{
				HaveSelRect = false;
				redrawMarker->hide();
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				Doc->appMode = modeNormal;
				emit PaintingDone();
			}
			redrawMarker->hide();
		}
		else
		{
			FPoint nx = translateToDoc(m->x(), m->y());
			int mx = qRound(nx.x());
			int my = qRound(nx.y());
			Magnify ? slotZoomIn(mx,my) : slotZoomOut(mx,my);
			if (sc == m_canvas->scale())
			{
				HaveSelRect = false;
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				Doc->appMode = modeNormal;
				emit PaintingDone();
			}
			else
			{
				if (m->modifiers() & Qt::ShiftModifier)
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
				else
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
			}
		}
	}
	m_canvas->setRenderModeUseBuffer(false);
	if ((Doc->appMode == modeDrawBezierLine) && (m->button() == Qt::LeftButton))
	{
		m_canvas->setRenderModeUseBuffer(true);
		currItem = Doc->m_Selection->itemAt(0);
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		QMatrix pm;
//		pm.translate(-Doc->minCanvasCoordinate.x()*m_canvas->scale(), -Doc->minCanvasCoordinate.y()*m_canvas->scale());
		m_canvas->Transform(currItem, pm);
		FPoint npf = FPoint(m->pos() * pm.inverted());
		npf = Doc->ApplyGridF(npf);
		currItem->PoLine.addPoint(npf);
		bool ssiz = currItem->Sizing;
		currItem->Sizing = true;
		if ((currItem->PoLine.size() % 4 == 0) && (currItem->PoLine.size() > 3))
		{
			FPoint lxy(currItem->PoLine.point(currItem->PoLine.size()-2));
			FPoint lk(currItem->PoLine.point(currItem->PoLine.size()-1));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			currItem->PoLine.addPoint(lxy);
			currItem->PoLine.addPoint(lk);
		}
		FPoint np2(getMinClipF(&currItem->PoLine));
		if (np2.x() < 0)
		{
			currItem->PoLine.translate(-np2.x(), 0);
			Doc->MoveItem(np2.x(), 0, currItem);
		}
		if (np2.y() < 0)
		{
			currItem->PoLine.translate(0, -np2.y());
			Doc->MoveItem(0, np2.y(), currItem);
		}
		if (FirstPoly)
		{
			FirstPoly = false;
			currItem->Sizing = ssiz;
		}
		else
		{
			Doc->SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false, false);
			Doc->AdjustItemSize(currItem);
			currItem->Sizing = ssiz;
			currItem->ContourLine = currItem->PoLine.copy();
			m_canvas->setRenderModeUseBuffer(false);
			repaintContents(currItem->getRedrawBounding(m_canvas->scale()));
		}
		m_canvas->setRenderModeFillBuffer();
		int newX = qRound(translateToDoc(m->x(), m->y()).x());
		int newY = qRound(translateToDoc(m->x(), m->y()).y());
		m_canvas->newRedrawPolygon() << QPoint(newX - qRound(currItem->xPos()), newY - qRound(currItem->yPos()));
		updateContents();
	}
	if ((Doc->appMode == modeDrawBezierLine) && (m->button() == Qt::RightButton))
	{
		currItem = Doc->m_Selection->itemAt(0);
		if (currItem!=0)
		{
			currItem->PoLine.resize(currItem->PoLine.size()-2);
			if (currItem->PoLine.size() < 4)
			{
	//			emit DelObj(Doc->currentPage->pageNr(), currItem->ItemNr);
				Doc->Items->removeAt(currItem->ItemNr);
				Doc->m_Selection->removeFirst();
				emit HaveSel(-1);
			}
			else
			{
				Doc->SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false);
//				currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2.0, 1)));
				Doc->AdjustItemSize(currItem);
				currItem->ContourLine = currItem->PoLine.copy();
			}
		}
		if (!Prefs->stickyTools)
		{
			Doc->appMode = modeNormal;
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			emit PaintingDone();
		}
		else
			emit Amode(Doc->appMode);
		emit DocChanged();
		FirstPoly = true;
		m_canvas->setRenderModeUseBuffer(false);
		updateContents();
	}
	Doc->DragP = false;
	Doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	MidButt = false;
	shiftSelItems = false;
	inItemCreation = false;
	m_SnapCounter = 0;
//	Doc->SubMode = -1;
	if (_groupTransactionStarted)
	{
		for (int i = 0; i < Doc->m_Selection->count(); ++i)
			Doc->m_Selection->itemAt(i)->checkChanges(true);
		undoManager->commit();
		_groupTransactionStarted = false;
	}

	for (int i = 0; i < Doc->m_Selection->count(); ++i)
		Doc->m_Selection->itemAt(i)->checkChanges(true);

	//Commit drag created items to undo manager.
	if (Doc->m_Selection->itemAt(0)!=NULL)
	{
		Doc->itemAddCommit(Doc->m_Selection->itemAt(0)->ItemNr);
	}
	//Make sure the Zoom spinbox and page selector dont have focus if we click on the canvas
	zoomSpinBox->clearFocus();
	pageSelector->clearFocus();
	if (Doc->m_Selection->itemAt(0) != 0) // is there the old clip stored for the undo action
	{
		currItem = Doc->m_Selection->itemAt(0);
		Doc->nodeEdit.finishTransaction(currItem);
	}
	else
	{
		//delete oldClip;
		//oldClip = 0;
	}
}
#endif

#if 0
void ScribusView::contentsMouseMoveEvent(QMouseEvent *m)
{}
{
	int newX, newY;
	double nx, ny;
	PageItem *currItem;
	QPoint np, np2, mop;
	FPoint npf, npf2;
	QPainter p;
	QRect tx;
	bool erf = false;
	double sc = m_canvas->scale();
	horizRuler->Draw(m->x());
	vertRuler->Draw(m->y());
	m->accept();
	emit MousePos(m->x()/m_canvas->scale(),// + Doc->minCanvasCoordinate.x(), 
				  m->y()/m_canvas->scale()); // + Doc->minCanvasCoordinate.y());
	if (Doc->guidesSettings.guidesShown)
	{
		if (MoveGY)
		{
			FromHRuler(m);
			return;
		}
		if (MoveGX)
		{
			FromVRuler(m);
			return;
		}
	}
/*	if ((!Doc->DragP) && (m_canvas->m_viewMode.m_MouseButtonPressed) && (SelItem.count() != 0))
	{
		int cx = Anz->contentsX() - Anz->childX(doku->currentPage->parentWidget());
		int cy = Anz->contentsY() - Anz->childY(doku->currentPage->parentWidget());
		int cw = Anz->visibleWidth();
		int ch = Anz->visibleHeight();
		if ((m->y() < cy+10) || ((ch - (m->y() - cy)) - 10 < 0) || (m->x() < cx + 10) || ((cw - (m->x() - cx)) - 10 < 0))
		{
			if (m->y() < cy+10)
				Anz->scrollBy(0, m->y() - cy - 10);
			else if ((ch - (m->y() - cy)) - 10 < 0)
				Anz->scrollBy(0, m->y() - cy - ch +10);
			else if (m->x() < cx + 10)
				Anz->scrollBy(m->x() - cx - 10, 0);
			else if ((cw - (m->x() - cx)) - 10 < 0)
				Anz->scrollBy(m->x() - cx - cw + 10, 0);
			BlockLeave = true;
			update();
		}
	else
		BlockLeave = false;
	} */

	if (m_canvas->m_viewMode.m_MouseButtonPressed && (Doc->appMode == modeEditGradientVectors))
	{
		PageItem *currItem = Doc->m_Selection->itemAt(0);
		newX = m->x();
		newY = m->y();
		double dx = abs(Mxp - newX) + 5;
		double dy = abs(Myp - newY) + 5;
		if (m->buttons() & Qt::LeftButton)
		{
			currItem->GrStartX -= (Mxp - newX) / m_canvas->scale();
			currItem->GrStartY -= (Myp - newY) / m_canvas->scale();
		}
		if (m->buttons() & Qt::RightButton)
		{
			currItem->GrEndX -= (Mxp - newX) / m_canvas->scale();
			currItem->GrEndY -= (Myp - newY) / m_canvas->scale();
		}
		Mxp = newX;
		Myp = newY;
		RefreshGradient(currItem, dx, dy);
		m_ScMW->propertiesPalette->updateColorSpecialGradient();
		return;
	}
	if (m_canvas->m_viewMode.m_MouseButtonPressed && (Doc->appMode == modeMeasurementTool))
	{
		newX = m->x();
		newY = m->y();
		m_canvas->newRedrawPolygon() << QPoint(Dxp, Dyp) << QPoint(newX, newY);
		updateContents(QRect(QPoint(Dxp, Dyp), QPoint(newX, newY)).normalized().adjusted(-10, -10, 20, 20));
		Mxp = newX;
		Myp = newY;
		double dxp = Dxp / sc - Doc->currentPage()->xOffset();//+ Doc->minCanvasCoordinate.x();
			double dyp = Dyp / sc - Doc->currentPage()->yOffset();//+ Doc->minCanvasCoordinate.y();
				double nxp = newX / sc - Doc->currentPage()->xOffset();//+ Doc->minCanvasCoordinate.x();
					double nyp = newY / sc - Doc->currentPage()->yOffset();//+ Doc->minCanvasCoordinate.y();
		emit MVals(dxp, dyp, nxp, nyp, -xy2Deg(newX/sc - Dxp*sc, newY/sc - Dyp/sc), sqrt(pow(newX/sc - Dxp/sc,2)+pow(newY/sc - Dyp/sc,2)), Doc->unitIndex());
		return;
	}
	if (m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::RightButton) && (m->modifiers() & Qt::ControlModifier))
	{
		m_ScMW->setAppMode(modePanning);
	}
	if (m_canvas->m_viewMode.m_MouseButtonPressed && (Doc->appMode == modePanning))
	{
		int scroX = m->x() - qRound((Mxp * sc));
		int scroY = m->y() - qRound((Myp * sc));
		scrollBy(-scroX, -scroY);
		Mxp = static_cast<int>((m->x()-scroX)/sc);
		Myp = static_cast<int>((m->y()-scroY)/sc);
		return;
	}
	if (m_canvas->m_viewMode.m_MouseButtonPressed && (Doc->appMode == modeDrawFreehandLine))
	{
		newX = m->x();
		newY = m->y();
		double newXF = translateToDoc(m->x(), m->y()).x();
		double newYF = translateToDoc(m->x(), m->y()).y();
		if (RecordP.size() > 0)
		{
			if (FPoint(newXF, newYF) != RecordP.point(RecordP.size()-1))
				RecordP.addPoint(FPoint(newXF, newYF));
		}
		else
			RecordP.addPoint(FPoint(newXF, newYF));
		QPolygon& redrawPolygon(m_canvas->newRedrawPolygon());
		for (uint pp = 0; pp < RecordP.size(); pp++)
		{
			redrawPolygon << RecordP.pointQ(pp);
		}
		m_canvas->m_viewMode.operItemResizing = true;
		QRect bRect = m_canvas->redrawPolygon().boundingRect();
		QPoint in(qRound(bRect.x()*m_canvas->scale()), qRound(bRect.y()*m_canvas->scale()));
//		in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
		QPoint out = contentsToViewport(in);
		updateContents(QRect(out.x()+0*contentsX(), out.y()+0*contentsY(), qRound(bRect.width()*m_canvas->scale()), qRound(bRect.height()*m_canvas->scale())).adjusted(-10, -10, 20, 20));
		return;
	}
	if ((GetItem(&currItem)) && (!shiftSelItems))
	{
		newX = qRound(translateToDoc(m->x(), m->y()).x());
		newY = qRound(translateToDoc(m->x(), m->y()).y());
		if ((((dragTimerFired) && (m->buttons() & Qt::LeftButton)) || (moveTimerElapsed() && (m->buttons() & Qt::RightButton)))
			&& (m_canvas->m_viewMode.m_MouseButtonPressed)
			&& (!Doc->DragP) 
			&& (Doc->appMode == modeNormal) 
			&& (!(currItem->isSingleSel)))
		{
			dragTimer->stop();
			if ((abs(Dxp - newX) > 10) || (abs(Dyp - newY) > 10))
			{
				m_canvas->resetRenderMode();
				dragTimerFired = false;
				Doc->DragP = true;
				Doc->leaveDrag = false;
				Doc->DraggedElem = currItem;
				Doc->DragElements.clear();
				for (int dre=0; dre<Doc->m_Selection->count(); ++dre)
					Doc->DragElements.append(Doc->m_Selection->itemAt(dre)->ItemNr);
				ScriXmlDoc *ss = new ScriXmlDoc();
				//Q3DragObject *dr = new Q3TextDrag(ss->WriteElem(Doc, this, Doc->m_Selection), this);
				QMimeData* md = new QMimeData();
				md->setText(ss->WriteElem(Doc, this, Doc->m_Selection));
				QDrag* dr = new QDrag(this);
				dr->setMimeData(md);
				dr->setDragCursor(loadIcon("DragPix.xpm"), Qt::ActionMask);
				dr->start();
//				QImage drImg = currItem->DrawObj_toImage();
//				QPixmap pm;
//				pm=QPixmap::fromImage(drImg);
//				dr->setPixmap(pm);
//				dr->setPixmap(loadIcon("DragPix.xpm"));
//				dr->drag();
//				if (!dr->drag())
//					qDebug("ScribusView::contentsMouseMoveEvent: couldn't start drag operation!");
/* commented out the code above as the debug message is incorrect,
   see the Qt Reference: "The function returns TRUE if the caller should delete the original copy
    of the dragged data (but see target()); otherwise returns FALSE." */
				delete ss;
				ss=NULL;
				Doc->DragP = false;
				Doc->leaveDrag = false;
				m_canvas->m_viewMode.m_MouseButtonPressed = false;
				Doc->DraggedElem = 0;
				Doc->DragElements.clear();
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				updateContents();
			}
			return;
		}
		if (Doc->DragP)
			return;
		if (moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed && (Doc->appMode == modeRotation))
		{
			dragTimer->stop();
			double newW = xy2Deg(m->x()/sc - RCenter.x(), m->y()/sc - RCenter.y());
			if (m->modifiers() & Qt::ControlModifier)
			{
				newW=constrainAngle(newW, Doc->toolSettings.constrain);
				oldW=constrainAngle(oldW, Doc->toolSettings.constrain);
				//RotateGroup uses MoveBy so its pretty hard to constrain the result
				if (Doc->m_Selection->isMultipleSelection())
					RotateGroup(newW-oldW);
				else
					Doc->RotateItem(newW, currItem->ItemNr);
			}
			else
			{
				if (Doc->m_Selection->isMultipleSelection())
					RotateGroup(newW - oldW);
				else
					Doc->RotateItem(currItem->rotation() - (oldW - newW), currItem->ItemNr);
			}
			oldW = newW;
			emit DocChanged();
		}
		if (Doc->appMode == modeDrawBezierLine)
		{
			if ((Doc->useRaster) && (Doc->OnPage(currItem) != -1))
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			m_canvas->newRedrawPolygon() << QPoint(newX - qRound(currItem->xPos()), newY - qRound(currItem->yPos()));
			updateContents();
			Mxp = newX;
			Myp = newY;
		}
		if (m_canvas->m_viewMode.m_MouseButtonPressed && (Doc->appMode == modeDrawRegularPolygon))
		{
			if (m->modifiers() & Qt::ShiftModifier)
			{
				mop = QPoint(m->x(), static_cast<int>((currItem->yPos() + (newX - currItem->xPos())) * sc));
				QCursor::setPos(mapToGlobal(mop));
				newY = static_cast<int>(mop.y()/sc);
			}
			if ((Doc->useRaster) && (Doc->OnPage(currItem) != -1))
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			QMatrix mp;
			mp.translate(currItem->xPos(),// - Doc->minCanvasCoordinate.x(), 
						 currItem->yPos()); // - Doc->minCanvasCoordinate.y());
			mp.rotate(currItem->rotation());
			np2 = QPoint(newX, newY) * mp.inverted();
			QMatrix pm;
			m_canvas->Transform(currItem, pm);
			m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), np2)));
			emit ItemGeom(newX - currItem->xPos(), newY - currItem->yPos());
			Mxp = newX;
			Myp = newY;
		}
		if (m_canvas->m_viewMode.m_MouseButtonPressed && (Doc->appMode == modeDrawLine))
		{
			if (Doc->useRaster)
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			double newRot=xy2Deg(newX - currItem->xPos(), newY - currItem->yPos());
			//Constrain rotation angle, when the mouse is being dragged around for a new line
			if (m->modifiers() & Qt::ControlModifier)
			{
				//Flip our angles around here
				if (newRot<0.0)
					newRot=-newRot;
				else
					newRot=360-newRot;
				newRot=constrainAngle(newRot, Doc->toolSettings.constrain);
				double hlen=sqrt(pow(newX - currItem->xPos(),2)+pow(newY - currItem->yPos(),2));
				newX = qRound(currItem->xPos()+(hlen * cos(newRot/(180.0/M_PI))));
				newY = qRound(currItem->yPos()-(hlen * sin(newRot/(180.0/M_PI))));
			}
			emit SetAngle(newRot);
			emit ItemGeom(sqrt(pow(newX - currItem->xPos(),2)+pow(newY - currItem->yPos(),2)), 0);
			m_canvas->newRedrawPolygon() << QPoint(qRound(currItem->xPos()), qRound(currItem->yPos())) << QPoint(newX, newY);
			newX = m->x();
			newY = m->y();
			updateContents(QRect(QPoint(Dxp, Dyp), QPoint(newX, newY)).normalized().adjusted(-10, -10, 20, 20));
			Mxp = newX;
			Myp = newY;
		}
		if (m_canvas->m_viewMode.m_MouseButtonPressed && (Doc->appMode == modeEdit) && (!m_canvas->m_viewMode.operItemResizeInEditMode))
		{
			if (currItem->asImageFrame())
			{
				QMatrix ro;
				ro.rotate(-currItem->rotation());
				QPoint rota = ro.map(QPoint(newX-Mxp,newY-Myp));
				currItem->moveImageInFrame(rota.x()/currItem->imageXScale(), rota.y()/currItem->imageYScale());
				updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				Mxp = newX;
				Myp = newY;
			}
			if (currItem->asTextFrame())
			{
				currItem->itemText.deselectAll();
				currItem->HasSel = false;
				slotSetCurs(m->x(), m->y());
				//Make sure we dont go here if the old cursor position was not set
				if (oldCp!=-1 && currItem->itemText.length() > 0)
				{
					if (currItem->CPos < oldCp)
					{
						currItem->itemText.select(currItem->CPos, oldCp - currItem->CPos);
						currItem->HasSel = true;
					}
					if (currItem->CPos > oldCp)
					{
						currItem->itemText.select(oldCp, currItem->CPos - oldCp);
						currItem->HasSel = true;
					}
				}
				RefreshItem(currItem);
				emit currItem->HasSel ? HasTextSel() : HasNoTextSel();
			}
		}
		//Operations run here:
		//Item resize, esp after creating a new one
		if (moveTimerElapsed() && m_canvas->m_viewMode.m_MouseButtonPressed && (m->buttons() & Qt::LeftButton) && ((Doc->appMode == modeNormal) || ((Doc->appMode == modeEdit) && m_canvas->m_viewMode.operItemResizeInEditMode)) && (!currItem->locked()))
		{
			dragTimer->stop();
			if (Doc->appMode == modeEditClip)
			{
//				handleNodeEditDrag(m, currItem);
				return;
			}
			if (m_canvas->m_viewMode.operItemResizing)
			{
				newX = static_cast<int>(m->x()/sc);
				newY = static_cast<int>(m->y()/sc);
				m_canvas->m_viewMode.operItemMoving = false;
				if (Doc->m_Selection->isMultipleSelection())
				{
					newX = qRound(translateToDoc(m->x(), m->y()).x());
					newY = qRound(translateToDoc(m->x(), m->y()).y());
					double gx, gy, gh, gw;
					Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					int ox1 = qRound(gx*sc);
					int oy1 = qRound(gy*sc);
					int ox2 = qRound((gx+gw)*sc);
					int oy2 = qRound((gy+gh)*sc);
					np2 = QPoint(newX, newY);
					np2 = Doc->ApplyGrid(np2);
					double nx = np2.x();
					double ny = np2.y();
					Doc->ApplyGuides(&nx, &ny);
					np2 = QPoint(qRound(nx*sc), qRound(ny*sc));
					QMatrix pm;
					switch (frameResizeHandle)
					{
					case 1:
						if (m->modifiers() & Qt::ControlModifier)
							np2 = QPoint(qRound(newX), qRound(gy+(gh * ((newX-gx) / gw))));
						else
							np2 = QPoint(qRound(newX), qRound(newY));
						np2 = Doc->ApplyGrid(np2);
						nx = np2.x();
						ny = np2.y();
						Doc->ApplyGuides(&nx, &ny);
						np2 = QPoint(qRound(nx*sc), qRound(ny*sc));
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(qRound(gx*sc), qRound(gy*sc)), np2)));
						break;
					case 2:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(ox2,oy2))));
						break;
					case 3:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(ox1, oy2))));
						break;
					case 4:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(ox2, oy1))));
						break;
					case 5:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(ox1, oy1), QPoint(ox2, np2.y()))));
						break;
					case 6:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(np2.x(), oy2), QPoint(ox1,oy1))));
						break;
					case 7:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(np2.x(), oy1), QPoint(ox2, oy2))));
						break;
					case 8:
						m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(ox1, qRound(np2.y())), QPoint(ox2, oy2))));
						break;
					}
					Mxp = qRound(np2.x());
					Myp = qRound(np2.y());
				}
				else
				{
					//qDebug(QString("frameResizeHandle %1").arg(frameResizeHandle));
					for (int a = 0; a < Doc->m_Selection->count(); ++a)
					{
						currItem = Doc->m_Selection->itemAt(0);
						double nh = currItem->height();
						if ((frameResizeHandle == 1) || (frameResizeHandle == 2))
						{
							QMatrix mp;
							switch (frameResizeHandle)
							{
							case 1:
//								mp.translate(-Doc->minCanvasCoordinate.x() * m_canvas->scale(),-Doc->minCanvasCoordinate.y() * m_canvas->scale());
								m_canvas->Transform(currItem, mp);
								//Shift proportional square resize
								if ((m->modifiers() & Qt::ShiftModifier) && (!(m->modifiers() & Qt::ControlModifier)))
								{
									QMatrix ma;
									ma.translate(currItem->xPos(), currItem->yPos());
									ma.rotate(currItem->rotation());
									ma = ma.invert();
									nh = ma.m11() * newX + ma.m21() * newY + ma.dx();
									mop = QPoint(m->x(), m->y());
								}
								else
								{
									//Control proportional resize
									if ((m->modifiers() & Qt::ControlModifier) && (!(m->modifiers() & Qt::ShiftModifier)))
									{
										QMatrix ma;
										ma.translate(currItem->xPos(), currItem->yPos());
										ma.rotate(currItem->rotation());
										ma = ma.invert();
										double nX = ma.m11() * newX + ma.m21() * newY + ma.dx();
										nh = nX / currItem->OldB2 * currItem->OldH2;
										mop = QPoint(m->x(), m->y());
									}
									else
										mop = QPoint(m->x(), m->y());
								}
								np = mop * mp.inverted();
								nx = np.x();
								ny = np.y();
								if (!currItem->asLine())
								{
									if ((Doc->useRaster) && (Doc->OnPage(currItem) != -1))
									{
										nx += currItem->xPos();
										ny += currItem->yPos();
										npf = Doc->ApplyGridF(FPoint(nx, ny));
										nx = npf.x() - currItem->xPos();
										ny = npf.y() - currItem->yPos();
									}
									if (Doc->SnapGuides)
									{
										nx += currItem->xPos();
										ny += currItem->yPos();
										Doc->ApplyGuides(&nx, &ny);
										nx -= currItem->xPos();
										ny -= currItem->yPos();
									}
									if ((m->modifiers() & Qt::ControlModifier) || ((m->modifiers() & Qt::ShiftModifier)))
										erf = Doc->SizeItem(nx, nh, currItem->ItemNr);
									else
										erf = Doc->SizeItem(nx, ny, currItem->ItemNr);
									QMatrix mp;
									mp.translate(currItem->xPos(),// - Doc->minCanvasCoordinate.x(), 
												 currItem->yPos());// - Doc->minCanvasCoordinate.y());
									mp.rotate(currItem->rotation());
									np2 = QPoint(qRound(nx), qRound(ny));
									QMatrix pm;
									m_canvas->Transform(currItem, pm);
									m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), np2)));
								}
								else
								{
									double rba = currItem->rotation();
									currItem->setRotation(0.0);
									QMatrix mp;
									m_canvas->Transform(currItem, mp);
//									mp.translate(-Doc->minCanvasCoordinate.x(), -Doc->minCanvasCoordinate.y());
									np = QPoint(m->x(), m->y()) * mp.inverted();
									double sizeItemX=np.x(), sizeItemY=np.y();
									//Constrain rotation angle, when the mouse is moving the non-origin point of a line
									if (m->modifiers() & Qt::ControlModifier)
									{
										double newRot=xy2Deg(np.x(), np.y());
										rba=constrainAngle(newRot, Doc->toolSettings.constrain);
										double hlen=sqrt(pow(newX - currItem->xPos(),2)+pow(newY - currItem->yPos(),2));
										sizeItemX = hlen * cos(rba/(180.0/M_PI));
										sizeItemY = hlen * sin(rba/(180.0/M_PI));
									}
									currItem->setRotation(rba);
									np = Doc->ApplyGrid(np);
									erf = Doc->SizeItem(sizeItemX, sizeItemY, currItem->ItemNr);
									QMatrix pm;
//									pm.translate(-Doc->minCanvasCoordinate.x() * m_canvas->scale(),-Doc->minCanvasCoordinate.y() * m_canvas->scale());
									m_canvas->Transform(currItem, pm);
									m_canvas->newRedrawPolygon() << pm.map(QPoint(0, 0)) << pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())));
									updateContents(QRect(pm.map(QPoint(0, 0)), pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())))).normalized().adjusted(-10, -10, 20, 20));
								}
								break;
							case 2:
								if (currItem->asLine())
								{
									double sav = Doc->SnapGuides;
									npf2 = FPoint(newX-Mxp, newY-Myp);
									//Constrain rotation on left point move, disabled for now in movesizeitem
									erf = Doc->MoveSizeItem(npf2, FPoint(0, 0), currItem->ItemNr, false, (m->modifiers() & Qt::ControlModifier));
									Doc->SnapGuides = sav;
									if (sav)
										currItem->Sizing = true;
									QMatrix pm;
//									pm.translate(-Doc->minCanvasCoordinate.x() * m_canvas->scale(),-Doc->minCanvasCoordinate.y() * m_canvas->scale());
									m_canvas->Transform(currItem, pm);
									m_canvas->newRedrawPolygon() << pm.map(QPoint(0, 0)) << pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())));
									updateContents(QRect(pm.map(QPoint(0, 0)), pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())))).normalized().adjusted(-10, -10, 20, 20));
								}
								else
								{
									newX = qRound(translateToDoc(m->x(), m->y()).x());
									newY = qRound(translateToDoc(m->x(), m->y()).y());
									np2 = QPoint(newX, newY);
									np2 = Doc->ApplyGrid(np2);
									double nx = np2.x();
									double ny = np2.y();
									Doc->ApplyGuides(&nx, &ny);
									QMatrix mp;
//									mp.translate(currItem->xPos() - Doc->minCanvasCoordinate.x(), currItem->yPos() - Doc->minCanvasCoordinate.y());
									mp.rotate(currItem->rotation());
									np2 = QPoint(qRound(nx), qRound(ny)) * mp.inverted();
									QMatrix pm;
									m_canvas->Transform(currItem, pm);
									m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(qRound(currItem->width()), qRound(currItem->height())))));
//									p.end();
								}
								break;
							}
						}
						else
						{
							np2 = QPoint(newX, newY);
							np2 = Doc->ApplyGrid(np2);
							double nx = np2.x();
							double ny = np2.y();
							Doc->ApplyGuides(&nx, &ny);
							QMatrix mp;
//							mp.translate(currItem->xPos() - Doc->minCanvasCoordinate.x(), currItem->yPos() - Doc->minCanvasCoordinate.y());
							mp.rotate(currItem->rotation());
							np2 = QPoint(qRound(nx), qRound(ny)) * mp.inverted();
							QMatrix pm;
							m_canvas->Transform(currItem, pm);
							switch (frameResizeHandle)
							{
							case 0:
								break;
							case 3:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(0, qRound(currItem->height())))));
								break;
							case 4:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(qRound(currItem->width()), 0))));
								break;
							case 5:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), QPoint(qRound(currItem->width()), np2.y()))));
								break;
							case 6:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), QPoint(np2.x(), qRound(currItem->height())))));
								break;
							case 7:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(np2.x(), 0), QPoint(qRound(currItem->width()), qRound(currItem->height())))));
								break;
							case 8:
								m_canvas->PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, np2.y()), QPoint(qRound(currItem->width()), qRound(currItem->height())))));
								break;
							}
						}
					}
				}
				if ((erf) || (frameResizeHandle > 1))
				{
					Mxp = newX;
					Myp = newY;
				}
			}
			else
			{
				//Dragging an item (plus more?)
				newX = static_cast<int>(m->x()/sc);
				newY = static_cast<int>(m->y()/sc);
				m_canvas->m_viewMode.operItemMoving = true;
				erf = false;
				int dX=newX-Mxp, dY=newY-Myp;
				if (!Doc->m_Selection->isMultipleSelection())
				{
					erf=true;
					currItem = Doc->m_Selection->itemAt(0);
					//Control Alt drag image in frame without being in edit mode
					if (currItem->asImageFrame() && (m->modifiers() & Qt::ControlModifier) && (m->modifiers() & Qt::AltModifier))
					{
						currItem->moveImageInFrame(dX/currItem->imageXScale(),dY/currItem->imageYScale());
						updateContents(currItem->getRedrawBounding(m_canvas->scale()));
					}
					else
					{
						//Dragging orthogonally - Ctrl Drag
						if ((m->modifiers() & Qt::ControlModifier) && !(m->modifiers() & Qt::ShiftModifier) && !(m->modifiers() & Qt::AltModifier))
						{
							if (abs(dX)>abs(dY))
								dY=0;
							else
							if (abs(dY)>abs(dX))
								dX=0;
							erf=false;
							dX+=qRound(dragConstrainInitPtX-currItem->xPos());
							dY+=qRound(dragConstrainInitPtY-currItem->yPos());
						}
						if (!(currItem->isTableItem && currItem->isSingleSel))
						{
							Doc->m_Selection->setGroupRect();
							double gx, gy, gh, gw;
							Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
							moveGroup(dX, dY, false);
							if (Doc->SnapGuides)
							{
								double nx = gx;
								double ny = gy;
								Doc->ApplyGuides(&nx, &ny);
								moveGroup(nx-gx, ny-gy, false);
								Doc->m_Selection->setGroupRect();
								Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
								nx = gx+gw;
								ny = gy+gh;
								Doc->ApplyGuides(&nx, &ny);
								moveGroup(nx-(gx+gw), ny-(gy+gh), false);
							}
							if (Doc->useRaster)
							{
								Doc->m_Selection->setGroupRect();
								double gx, gy, gh, gw, gxo, gyo;
								Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
								gxo = gx;
								gyo = gy;
								FPoint npx = Doc->ApplyGridF(FPoint(gx, gy));
								FPoint npw = Doc->ApplyGridF(FPoint(gx+gw, gy+gh));
								if ((fabs(gx-npx.x())) > (fabs((gx+gw)-npw.x())))
									gx = npw.x() - gw;
								else
									gx = npx.x();
								if ((fabs(gy-npx.y())) > (fabs((gy+gh)-npw.y())))
									gy = npw.y() - gh;
								else
									gy = npx.y();
								if ((fabs(gx - gxo) < (Doc->guidesSettings.guideRad / 2.0) / m_canvas->scale()) && (fabs(gy - gyo) < (Doc->guidesSettings.guideRad / 2.0) / m_canvas->scale()))
									moveGroup(gx-gxo, gy-gyo, false);
							}
						}
					}
				}
				else
				{
					Doc->m_Selection->setGroupRect();
					double gx, gy, gh, gw;
					Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					int dX=newX-Mxp, dY=newY-Myp;
					erf = true;
					if (m->modifiers() & Qt::ControlModifier)
					{
						if (abs(dX)>abs(dY))
							dY=0;
						else
						if (abs(dY)>abs(dX))
							dX=0;
						erf=false;
						dX+=dragConstrainInitPtX-qRound(gx);
						dY+=dragConstrainInitPtY-qRound(gy);
					}
					moveGroup(dX, dY, false);
					if (Doc->SnapGuides)
					{
						double nx = gx;
						double ny = gy;
						Doc->ApplyGuides(&nx, &ny);
						moveGroup(nx-gx, ny-gy, false);
						Doc->m_Selection->setGroupRect();
						Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
						nx = gx+gw;
						ny = gy+gh;
						Doc->ApplyGuides(&nx, &ny);
						moveGroup(nx-(gx+gw), ny-(gy+gh), false);
					}
					Doc->m_Selection->setGroupRect();
					if (Doc->useRaster)
					{
						Doc->m_Selection->setGroupRect();
						double gx, gy, gh, gw, gxo, gyo;
						Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
						gxo = gx;
						gyo = gy;
						FPoint npx = Doc->ApplyGridF(FPoint(gx, gy));
						FPoint npw = Doc->ApplyGridF(FPoint(gx+gw, gy+gh));
						if ((fabs(gx-npx.x())) > (fabs((gx+gw)-npw.x())))
							gx = npw.x() - gw;
						else
							gx = npx.x();
						if ((fabs(gy-npx.y())) > (fabs((gy+gh)-npw.y())))
							gy = npw.y() - gh;
						else
							gy = npx.y();
						if ((fabs(gx - gxo) < (Doc->guidesSettings.guideRad / 2.0) / m_canvas->scale()) && (fabs(gy - gyo) < (Doc->guidesSettings.guideRad / 2.0) / m_canvas->scale()))
							moveGroup(gx-gxo, gy-gyo, false);
						Doc->m_Selection->setGroupRect();
					}
				}
				if (erf)
				{
					Mxp = newX;
					Myp = newY;
				}
			}
		}
		if ((!m_canvas->m_viewMode.m_MouseButtonPressed) && (Doc->appMode != modeDrawBezierLine))
		{
			if (Doc->m_Selection->isMultipleSelection())
			{
				QRect mpo = QRect(qRound(m->x()/m_canvas->scale())-Doc->guidesSettings.grabRad, qRound(m->y()/m_canvas->scale())-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
//				mpo.moveBy(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
				double gx, gy, gh, gw;
				Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					&& ((Doc->appMode == modeNormal) || (Doc->appMode == modeRotation)))
				{
					int how = 0;
//					gx -= Doc->minCanvasCoordinate.x();
//					gy -= Doc->minCanvasCoordinate.y();
					QMap<double,int> distance;
					double d1 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
					if (d1 < Doc->guidesSettings.grabRad)
						distance.insert(d1, 1);
					double d2 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
					if (d2 < Doc->guidesSettings.grabRad)
						distance.insert(d2, 2);
					double d3 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
					if (d3 < Doc->guidesSettings.grabRad)
						distance.insert(d3, 3);
					double d4 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
					if (d4 < Doc->guidesSettings.grabRad)
						distance.insert(d4, 4);
					double d5 = sqrt(pow(((gx+gw/2) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
					if (d5 < Doc->guidesSettings.grabRad)
						distance.insert(d5, 5);
					double d6 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh/2) * m_canvas->scale()) - m->y(),2));
					if (d6 < Doc->guidesSettings.grabRad)
						distance.insert(d6, 6);
					double d7 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow(((gy+gh/2) * m_canvas->scale()) - m->y(),2));
					if (d7 < Doc->guidesSettings.grabRad)
						distance.insert(d7, 7);
					double d8 = sqrt(pow(((gx+gw/2) * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
					if (d8 < Doc->guidesSettings.grabRad)
						distance.insert(d8, 8);
					QList<int> result = distance.values();
					if (result.count() != 0)
					{
						how = result[0];
						switch (how)
						{
							case 1:
							case 2:
								qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
								break;
							case 3:
							case 4:
								qApp->changeOverrideCursor(QCursor(Qt::SizeBDiagCursor));
								break;
							case 5:
							case 8:
								qApp->changeOverrideCursor(QCursor(Qt::SizeVerCursor));
								break;
							case 6:
							case 7:
								qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
								break;
							default:
								qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
								break;
						}
					}
					else
						qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
					if (Doc->appMode == modeRotation)
						qApp->changeOverrideCursor(QCursor(loadIcon("Rotieren2.png")));
				}
				else
				{
					switch (Doc->appMode)
					{
						case modeDrawShapes:
							qApp->changeOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")));
							break;
						case modeDrawPicture:
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
						default:
							qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
						break;
					}
				}
				return;
			}
			for (int a = 0; a < Doc->m_Selection->count(); ++a)
			{
				currItem = Doc->m_Selection->itemAt(a);
				if (currItem->locked())
					break;
				QMatrix p;
				m_canvas->Transform(currItem, p);
				QRect mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
//				mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
				if (Doc->appMode == modeEditClip)
				{
//					handleNodeEditMove(m, mpo, currItem, p);
				}
				if ((QRegion(p.map(QPolygon(QRect(-3, -3, static_cast<int>(currItem->width()+6), static_cast<int>(currItem->height()+6))))).contains(mpo))
					&& ((Doc->appMode == modeNormal) || (Doc->appMode == modeRotation) || (Doc->appMode == modeEdit)))
				{
					tx = p.map(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
					if ((tx.intersects(mpo)) && (!currItem->locked()))
					{
						if (Doc->appMode == modeRotation)
							qApp->changeOverrideCursor(QCursor(loadIcon("Rotieren2.png")));
						else
						if (Doc->appMode == modeEdit)
						{
							if (currItem->asTextFrame())
								qApp->changeOverrideCursor(QCursor(Qt::ibeamCursor));
							if (currItem->asImageFrame())
								qApp->changeOverrideCursor(QCursor(loadIcon("HandC.xpm")));
						}
						else
							qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
						if (!currItem->sizeLocked())
							HandleCurs(currItem, mpo);
					}
				}
				else
				{
					switch (Doc->appMode)
					{
						case modeDrawShapes:
							qApp->changeOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")));
							break;
						case modeDrawPicture:
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
						default:
							qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
						break;
					}
				}
			}
		}
	}
	else
	{
		if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (m->state() & Qt::LeftButton) && (GyM == -1) && (GxM == -1))
		{
			newX = qRound(translateToDoc(m->x(), m->y()).x());
			if (Doc->appMode == modeMagnifier)
				newY = qRound(Myp + ((SeRx - Mxp) * visibleHeight()) / visibleWidth());
			else
				newY = qRound(translateToDoc(m->x(), m->y()).y());
			if (Doc->appMode == modeDrawTable)
			{
				FPoint np2 = Doc->ApplyGrid(QPoint(newX, newY));
				double nx = np2.x();
				double ny = np2.y();
				Doc->ApplyGuides(&nx, &ny);
				newX = qRound(nx);
				newY = qRound(ny);
				GyM = -1;
				GxM = -1;
			}
			SeRx = newX;
			SeRy = newY;
			if (Doc->appMode == modeDrawTable)
				redrawMarker->setGeometry(QRect(Dxp, Dyp, m->globalPos().x() - Dxp, m->globalPos().y() - Dyp).normalized());
			else
				redrawMarker->setGeometry(QRect(Mxp, Myp, m->globalPos().x() - Mxp, m->globalPos().y() - Myp).normalized());
			if (!redrawMarker->isVisible())
				redrawMarker->show();
			HaveSelRect = true;
			return;
		}
		if ((Doc->guidesSettings.guidesShown) && (Doc->appMode == modeNormal || Doc->appMode == modeEditClip) && (!Doc->GuideLock) && (Doc->OnPage(m->x()/sc, m->y()/sc) != -1) && (!GetItem(&currItem)))
		{
			Guides::iterator it;
			Guides tmpGuides = Doc->currentPage()->guides.horizontals(GuideManagerCore::Standard);
			for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it)
			{
				if (((*it)+Doc->currentPage()->yOffset()- 0*Doc->minCanvasCoordinate.y() < ((m->y()+Doc->guidesSettings.grabRad) / sc)) &&
								   ((*it)+Doc->currentPage()->yOffset()- 0*Doc->minCanvasCoordinate.y() > ((m->y()-Doc->guidesSettings.grabRad) / sc)))
				{
					if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (GyM != -1))
						MoveGY = true;
					if (((m->x()/sc) < Doc->currentPage()->xOffset()- 0*Doc->minCanvasCoordinate.x()) || ((m->x()/sc) >= Doc->currentPage()->width()-1+Doc->currentPage()->xOffset()- 0*Doc->minCanvasCoordinate.x()))
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					else
						qApp->changeOverrideCursor(QCursor(SPLITHC));
					return;
				}
			}
//			qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
			tmpGuides = Doc->currentPage()->guides.verticals(GuideManagerCore::Standard);
			for (it = tmpGuides.begin(); it!= tmpGuides.end(); ++it)
			{
				if (((*it)+Doc->currentPage()->xOffset()- 0*Doc->minCanvasCoordinate.x() < ((m->x()+Doc->guidesSettings.grabRad) / sc)) &&
								   ((*it)+Doc->currentPage()->xOffset()- 0*Doc->minCanvasCoordinate.x() > ((m->x()-Doc->guidesSettings.grabRad) / sc)))
				{
					if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (GxM != -1))
						MoveGX = true;
					if (((m->y()/sc) < Doc->currentPage()->yOffset()- 0*Doc->minCanvasCoordinate.x()) || ((m->y()/sc) >= Doc->currentPage()->height()-1+Doc->currentPage()->yOffset()- 0*Doc->minCanvasCoordinate.y()))
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					else
						qApp->changeOverrideCursor(QCursor(SPLITVC));
					return;
				}
			}
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		}
	}
}
#endif

#if 0
void ScribusView::contentsMousePressEvent(QMouseEvent *m)
{}
{
	m_SnapCounter = 0;
	bool inText;
	int z;
	double Rxp = 0;
	double Ryp = 0;
	double Rxpd = 0;
	double Rypd = 0;
	PageItem *currItem;
	PageItem *bb;
	QPainter p;
	m_canvas->PaintSizeRect(QRect());
	FPoint npf, npf2;
	QRect tx;
	QMatrix pm;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	m_canvas->m_viewMode.operItemMoving = false;
	HaveSelRect = false;
	Doc->DragP = false;
	Doc->leaveDrag = false;
	inItemCreation = false;
//	oldClip = 0;
	m->accept();
	moveTimer.start();
	dragTimerFired = false;
	Mxp = qRound(m->x()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.x());
	Myp = qRound(m->y()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
	QRect mpo(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
	if (Doc->appMode != modeEdit)
	{
		Rxp = Doc->ApplyGrid(QPoint(Mxp, Myp)).x();
		Rxpd = Mxp - Rxp;
		Mxp = qRound(Rxp);
		Ryp = Doc->ApplyGrid(QPoint(Mxp, Myp)).y();
		Rypd = Myp - Ryp;
		Myp = qRound(Ryp);
	}
	else
	{
		Ryp = Myp;
		Rxp = Mxp;
	}
	SeRx = Mxp;
	SeRy = Myp;
	if (m->button() == Qt::MidButton)
	{
		MidButt = true;
		DrawNew();
		return;
	}
	switch (Doc->appMode)
	{
		case modeNormal:
		case modeEditClip:
			Mxp = qRound(m->x()/m_canvas->scale());
			Myp = qRound(m->y()/m_canvas->scale());
			SeRx = Mxp;
			SeRy = Myp;
			if ((Doc->appMode == modeEditClip) && (Doc->m_Selection->count() != 0))
			{
//				handleNodeEditPress(m, mpo);
				return;
			}			
			
			if (GetItem(&currItem))
			{
				if (Doc->m_Selection->isMultipleSelection())
				{
					m_canvas->PaintSizeRect(QRect());
					double gx, gy, gh, gw;
					bool shiftSel = true;
					Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					dragConstrainInitPtX = qRound(gx);
					dragConstrainInitPtY = qRound(gy);
					mpo = QRect(qRound(m->x() / m_canvas->scale()) - Doc->guidesSettings.grabRad, qRound(m->y() / m_canvas->scale()) - Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
//					mpo.moveBy(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
					if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					      && (m->modifiers() != (Qt::ControlModifier | Qt::AltModifier)) && (m->modifiers() != Qt::ShiftModifier))
					{
						frameResizeHandle = 0;
						QMap<double,int> distance;
//						gx -= Doc->minCanvasCoordinate.x();
//						gy -= Doc->minCanvasCoordinate.y();
						double d1 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
						if (d1 < Doc->guidesSettings.grabRad)
							distance.insert(d1, 1);
						double d2 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
						if (d2 < Doc->guidesSettings.grabRad)
							distance.insert(d2, 2);
						double d3 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
						if (d3 < Doc->guidesSettings.grabRad)
							distance.insert(d3, 3);
						double d4 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
						if (d4 < Doc->guidesSettings.grabRad)
							distance.insert(d4, 4);
						double d5 = sqrt(pow(((gx+gw/2) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh) * m_canvas->scale()) - m->y(),2));
						if (d5 < Doc->guidesSettings.grabRad)
							distance.insert(d5, 5);
						double d6 = sqrt(pow(((gx+gw) * m_canvas->scale()) - m->x(),2)+pow(((gy+gh/2) * m_canvas->scale()) - m->y(),2));
						if (d6 < Doc->guidesSettings.grabRad)
							distance.insert(d6, 6);
						double d7 = sqrt(pow((gx * m_canvas->scale()) - m->x(),2)+pow(((gy+gh/2) * m_canvas->scale()) - m->y(),2));
						if (d7 < Doc->guidesSettings.grabRad)
							distance.insert(d7, 7);
						double d8 = sqrt(pow(((gx+gw/2) * m_canvas->scale()) - m->x(),2)+pow((gy * m_canvas->scale()) - m->y(),2));
						if (d8 < Doc->guidesSettings.grabRad)
							distance.insert(d8, 8);
						QList<int> result = distance.values();
						if (result.count() != 0)
						{
							frameResizeHandle = result[0];
							switch (frameResizeHandle)
							{
								case 1:
								case 2:
									qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
									break;
								case 3:
								case 4:
									qApp->changeOverrideCursor(QCursor(Qt::SizeBDiagCursor));
									break;
								case 5:
								case 8:
									qApp->changeOverrideCursor(QCursor(Qt::SizeVerCursor));
									break;
								case 6:
								case 7:
									qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
									break;
								default:
									qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
									break;
							}
						}
						if (currItem->sizeLocked())
						{
							qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
							frameResizeHandle = 0;
						}
						if (frameResizeHandle != 0)
							m_canvas->m_viewMode.operItemResizing = true;
					}
					else
						shiftSel = SeleItem(m);
					if (((Doc->m_Selection->count() == 0) || (!shiftSel)) && (m->modifiers() == Qt::ShiftModifier))
					{
						shiftSelItems = true;
						Mxp = qRound(m->x()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.x());
						Myp = qRound(m->y()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
						SeRx = Mxp;
						SeRy = Myp;
					}
					else
						shiftSelItems = false;
					m_canvas->setRenderModeFillBuffer();
				}
				else
				{
					dragConstrainInitPtX = qRound(currItem->xPos());
					dragConstrainInitPtY = qRound(currItem->yPos());
					SeleItem(m); //Where we send the mouse press event to select an item
					if (Doc->m_Selection->count() != 0)
					{
						currItem = Doc->m_Selection->itemAt(0);
						if (!currItem->locked())
						{
							HandleSizer(currItem, mpo, m);
							if (frameResizeHandle != 0)
							{
								if (!currItem->asLine())
									currItem->Sizing = true;
								m_canvas->m_viewMode.operItemResizing = true;
							}
						}
					}
					else
					{
						Mxp = qRound(m->x()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.x());
						Myp = qRound(m->y()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
						SeRx = Mxp;
						SeRy = Myp;
					}
				}
				m_canvas->setRenderModeFillBuffer();
			}
			else
			{
				SeleItem(m);
				if (Doc->m_Selection->count() == 0)
				{
					Mxp = qRound(m->x()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.x());
					Myp = qRound(m->y()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
					SeRx = Mxp;
					SeRy = Myp;
					redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
					redrawMarker->show();
					Mxp = m->globalPos().x();
					Myp = m->globalPos().y();
					Dxp = qRound(m->x()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.x());
					Dyp = qRound(m->y()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
				}
				else
				{
					m_canvas->setRenderModeFillBuffer();
				}
			}
/*			if (m->button() == MidButton)
			{
				MidButt = true;
				if (Doc->m_Selection->count() != 0)
					Deselect(true);
				DrawNew();
			} */
			if ((Doc->m_Selection->count() != 0) && (m->button() == Qt::RightButton))
			{
				m_canvas->m_viewMode.m_MouseButtonPressed = true;
				Dxp = Mxp;
				Dyp = Myp;
			}
			if ((Doc->m_Selection->count() != 0) && (m->button() == Qt::LeftButton) && (frameResizeHandle == 0))
			{
				dragTimer->setSingleShot(true);
				dragTimer->start(1000);			// set Timeout for starting a Drag operation to 1 sec.
			}
			break;
		case modeDrawShapes:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			switch (Doc->SubMode)
			{
			case 0:
				if (m->modifiers() == Qt::ShiftModifier)
				{
					z = Doc->itemAddArea(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					Doc->Items->at(z)->FrameType = 0;
					SetupDrawNoResize(z);
				}
				else
				{
					Doc->ApplyGuides(&Rxp, &Ryp);
					z = Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					Doc->Items->at(z)->FrameType = 0;
					SetupDraw(z);
				}
				break;
			case 1:
				if (m->modifiers() == Qt::ShiftModifier)
				{
					z = Doc->itemAddArea(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					Doc->Items->at(z)->FrameType = 1;
					SetupDrawNoResize(z);
				}
				else
				{
					Doc->ApplyGuides(&Rxp, &Ryp);
					z = Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					Doc->Items->at(z)->FrameType = 1;
					SetupDraw(z);
				}
				break;
			default:
				if (m->modifiers() == Qt::ShiftModifier)
				{
					z = Doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					Doc->Items->at(z)->SetFrameShape(Doc->ValCount, Doc->ShapeValues);
					Doc->setRedrawBounding(Doc->Items->at(z));
					Doc->Items->at(z)->FrameType = Doc->SubMode+2;
					SetupDrawNoResize(z);
				}
				else
				{
					Doc->ApplyGuides(&Rxp, &Ryp);
					z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
					Doc->Items->at(z)->SetFrameShape(Doc->ValCount, Doc->ShapeValues);
					Doc->setRedrawBounding(Doc->Items->at(z));
					Doc->Items->at(z)->FrameType = Doc->SubMode+2;
					SetupDraw(z);
				}
				break;
			}
			break;
		case modeDrawPicture:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->modifiers() == Qt::ShiftModifier)
			{
				z = Doc->itemAddArea(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}
			else
			{
				Doc->ApplyGuides(&Rxp, &Ryp);
				z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeDrawLatex:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->modifiers() == Qt::ShiftModifier)
			{
				z = Doc->itemAddArea(PageItem::LatexFrame, PageItem::Unspecified, Rxp, Ryp, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}
			else
			{
				Doc->ApplyGuides(&Rxp, &Ryp);
				z = Doc->itemAdd(PageItem::LatexFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeDrawText:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->modifiers() == Qt::ShiftModifier)
			{
				z = Doc->itemAddArea(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}	
			else
			{
				Doc->ApplyGuides(&Rxp, &Ryp);
				z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_canvas->m_viewMode.m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeMagnifier:
			m_canvas->m_viewMode.m_MouseButtonPressed = true;
			if ((m->modifiers() == Qt::ShiftModifier) || (m->button() == Qt::RightButton))
			{
				Magnify = false;
				qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
			}
			else
			{
				Magnify = true;
				qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
			}
			Mxp = m->globalPos().x();
			Myp = m->globalPos().y();
			SeRx = Mxp;
			SeRy = Myp;
			redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
			break;
		case modeEdit:
			{
				frameResizeHandle = 0;
				m_canvas->m_viewMode.operItemResizeInEditMode = false;
				int oldP=0;
				if (GetItem(&currItem))
				{
					slotDoCurs(false);
					if (!currItem->locked())
					{
						frameResizeHandle = HandleSizer(currItem, mpo, m);
						if (frameResizeHandle != 0)
						{
							m_canvas->m_viewMode.operItemResizeInEditMode = true;
							slotDoCurs(true);
							m_canvas->setRenderModeFillBuffer();
							return;
						}
					}
					oldP = currItem->CPos;
				}
				//CB Where we set the cursor for a click in text frame
				inText = slotSetCurs(m->x(), m->y());
				//CB If we clicked outside a text frame to go out of edit mode and deselect the frame
				if (!inText)
				{
					//<<CB Redraw subsequent frames after being in edit mode
					//No intelligence, may slow things down when drawing unnecessarily
					PageItem *nextItem=currItem;
					while (nextItem != 0)
					{
						if (nextItem->nextInChain() != 0)
						{
							nextItem = nextItem->nextInChain();
							RefreshItem(nextItem);
						}
						else
							break;
					}
					//>>
					Deselect(true);
					slotDoCurs(true);
					emit Amode(modeNormal);
					return;
				}
				//<<CB Add in shift select to text frames
				if (m->modifiers() & Qt::ShiftModifier)
				{
					int dir=1;
					if (oldCp>currItem->CPos)
						dir=-1;
					if (currItem->asTextFrame())
						currItem->asTextFrame()->ExpandSel(dir, oldP);
					oldCp = oldP;
				}
				else //>>CB
					oldCp = currItem->CPos;
				currItem = Doc->m_Selection->itemAt(0);
				slotDoCurs(true);
				if ((!inText) && ((currItem->asTextFrame()) || (currItem->asImageFrame())))
				{
					Deselect(true);
					if (SeleItem(m))
					{
						currItem = Doc->m_Selection->itemAt(0);
						if ((currItem->asTextFrame()) || (currItem->asImageFrame()))
							emit Amode(modeEdit);
						else
						{
							emit PaintingDone();
							qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
						}
					}
					else
					{
						emit PaintingDone();
						qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
					}
				}
				else
				{
					if ((m->button() == Qt::MidButton) && (currItem->asTextFrame()))
					{
						m_canvas->m_viewMode.m_MouseButtonPressed = false;
						MidButt = false;
						QString cc;
						cc = QApplication::clipboard()->text(QClipboard::Selection);
						if (cc.isNull())
							cc = QApplication::clipboard()->text(QClipboard::Clipboard);
						if (!cc.isNull())
						{
							// K.I.S.S.:
							currItem->itemText.insertChars(0, cc, true);
							if (Doc->docHyphenator->AutoCheck)
								Doc->docHyphenator->slotHyphenate(currItem);
							emit ChBMText(currItem);
//							m_ScMW->outlinePalette->BuildTree();
						}
						else
						{
							if (m_ScMW->Buffer2.startsWith("<SCRIBUSTEXT"))
								m_ScMW->slotEditPaste();
						}
						RefreshItem(currItem);
					}
					if (currItem->asImageFrame() && !tx.contains(m->x(), m->y()))
					{
						Deselect(true);
						if (SeleItem(m))
						{
							currItem = Doc->m_Selection->itemAt(0);
							if ((currItem->asTextFrame()) || (currItem->asImageFrame()))
								emit Amode(modeEdit);
							else
							{
								emit PaintingDone();
								qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
							}
						}
						else
						{
							emit PaintingDone();
							qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
						}
					}
				}
			}
			break;
		case modeDrawLine:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			Doc->ApplyGuides(&Rxp, &Ryp);
			z = Doc->itemAdd(PageItem::Line, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, Rypd, Doc->toolSettings.dWidthLine, CommonStrings::None, Doc->toolSettings.dPenLine, !m_canvas->m_viewMode.m_MouseButtonPressed);
			currItem = Doc->Items->at(z);
			qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
			Doc->m_Selection->clear();
			Doc->m_Selection->addItem(currItem);
			updateContents(currItem->getRedrawBounding(m_canvas->scale()));
			m_canvas->m_viewMode.operItemMoving = true;
			inItemCreation = true;
			m_canvas->m_viewMode.operItemResizing = false;
			m_canvas->setRenderModeFillBuffer();
			Dxp = m->x();
			Dyp = m->y();
			Mxp = m->x();
			Myp = m->y();
			break;
		case modeRotation:
			if (m->button() != Qt::LeftButton)
				break;
			if (GetItem(&currItem))
			{
				RotMode = Doc->RotMode;
				if (Doc->m_Selection->isMultipleSelection())
				{
					double gx, gy, gh, gw;
					double gxR, gyR, ghR, gwR;
					getGroupRectScreen(&gx, &gy, &gw, &gh);
					Doc->m_Selection->getGroupRect(&gxR, &gyR, &gwR, &ghR);
					if (QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					{
						Doc->RotMode = 2;
						RCenter = FPoint(gxR+gwR/2.0, gyR+ghR/2.0);
						if (QRect(static_cast<int>(gx+gw)-6, static_cast<int>(gy+gh)-6, 6, 6).intersects(mpo))
						{
							RCenter = FPoint(gxR, gyR);
							Doc->RotMode = 0;
						}
					}
					oldW = xy2Deg(m->x()/m_canvas->scale() - RCenter.x(), m->y()/m_canvas->scale() - RCenter.y());
				}
				else
				{
					QMatrix mat;
					m_canvas->Transform(currItem, mat);
					Doc->RotMode = 2;
//					RCenter = FPoint(currItem->xPos()+currItem->width()/2, currItem->yPos()+currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, true);
					RCenter = FPoint(currItem->width()/2, currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, false);
//					if (!currItem->asLine())
//					{
						if (QRegion(mat.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo))
						{
							if (mat.map(QRect(0, 0, 6, 6)).intersects(mpo))
							{
//								RCenter = FPoint(currItem->xPos()+currItem->width(), currItem->yPos()+currItem->height(), 0, 0, currItem->rotation(), 1, 1, true);
								RCenter = FPoint(currItem->width(), currItem->height(), 0, 0, currItem->rotation(), 1, 1, false);
								Doc->RotMode = 4;
							}
							else if (mat.map(QRect(static_cast<int>(currItem->width())-6, 0, 6, 6)).intersects(mpo))
							{
//								RCenter = FPoint(currItem->xPos(), currItem->yPos()+currItem->height(), 0, 0, currItem->rotation(), 1, 1, true);
								RCenter = FPoint(0, currItem->height(), 0, 0, currItem->rotation(), 1, 1, false);
								Doc->RotMode = 3;
							}
							else if (mat.map(QRect(static_cast<int>(currItem->width())-6, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
							{
//								RCenter = FPoint(currItem->xPos(), currItem->yPos());
								RCenter = FPoint(0, 0);
								Doc->RotMode = 0;
							}
							else if (mat.map(QRect(0, static_cast<int>(currItem->height())-6, 6, 6)).intersects(mpo))
							{
//								RCenter = FPoint(currItem->xPos()+currItem->width(), currItem->yPos(), 0, 0, currItem->rotation(), 1, 1, true);
								RCenter = FPoint(currItem->width(), 0, 0, 0, currItem->rotation(), 1, 1, false);
								Doc->RotMode = 1;
							}
						}
//					}
					RCenter = FPoint(currItem->xPos()+RCenter.x(), currItem->yPos()+ RCenter.y(), 0, 0, 0, 1, 1, true);
					oldW = xy2Deg(m->x()/m_canvas->scale() - RCenter.x(), m->y()/m_canvas->scale() - RCenter.y());
				}
			}
			break;
		case modeLinkFrames:
			if (m->button() != Qt::LeftButton)
				break;
			currItem = Doc->ElemToLink;
			if (currItem==NULL)
				break;
			SeleItem(m);
			if (GetItem(&bb) && (bb->asTextFrame()))
			{
				PageItem* bblast = bb;
				while (bblast->nextInChain())
					bblast = bblast->nextInChain();
				
				if (currItem->nextInChain() == 0 && bb->prevInChain() == 0 && currItem != bblast)
				{
					currItem->link(bb);
					// CB We need to do this because we draw in the order of the item list
					// Which is also item number list.. but #3488: we must also renumber the items
					if (bb->ItemNr < currItem->ItemNr)
					{
						Doc->Items->insert(currItem->ItemNr+1, bb);
						bb = Doc->Items->takeAt(bb->ItemNr);
						Doc->renumberItemsInListOrder();
					}
					updateContents();
					emit DocChanged();
					Doc->ElemToLink = bb;
				}
				else if (currItem == bblast)
				{
					//CB Mouse is released when this messagebox takes focus
					m_canvas->m_viewMode.m_MouseButtonPressed = false;
					QMessageBox::warning(this, tr("Linking Text Frames"),
											 "<qt>" + tr("You are trying to link a frame to itself.") + "</qt>");
				}
				else
				{
					//CB Mouse is released when this messagebox takes focus
					m_canvas->m_viewMode.m_MouseButtonPressed = false;
					QMessageBox::warning(this, tr("Linking Text Frames"),
										 "<qt>" + tr("You are trying to link a frame which is already linked.") + "</qt>");
				}
			}
			else
				Doc->ElemToLink = NULL;
			break;
		case modeUnlinkFrames:
			if (m->button() != Qt::LeftButton)
				break;
			SeleItem(m);
			if (GetItem(&currItem) && (currItem->asTextFrame()))
			{
				if (currItem->prevInChain() != 0)
				{
					currItem->prevInChain()->unlink();
				}
				emit DocChanged();
				updateContents();
			}
			break;
		case modeDrawRegularPolygon:
			{
				if (m->button() != Qt::LeftButton)
					break;
				selectPage(m);
				if (m->modifiers() == Qt::ShiftModifier)
					z = Doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
				else
				{
					Doc->ApplyGuides(&Rxp, &Ryp);
					z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_canvas->m_viewMode.m_MouseButtonPressed);
				}
				currItem = Doc->Items->at(z);
				FPointArray cli = RegularPolygonF(currItem->width(), currItem->height(), Doc->toolSettings.polyC, Doc->toolSettings.polyS, Doc->toolSettings.polyF, Doc->toolSettings.polyR);
				FPoint np(cli.point(0));
				currItem->PoLine.resize(2);
				currItem->PoLine.setPoint(0, np);
				currItem->PoLine.setPoint(1, np);
				for (uint ax = 1; ax < cli.size(); ++ax)
				{
					np = FPoint(cli.point(ax));
					currItem->PoLine.putPoints(currItem->PoLine.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
				}
				np = FPoint(cli.point(0));
				currItem->PoLine.putPoints(currItem->PoLine.size(), 2, np.x(), np.y(), np.x(), np.y());
				currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
				qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
				Doc->m_Selection->clear();
				Doc->m_Selection->addItem(currItem);
				updateContents(currItem->getRedrawBounding(m_canvas->scale()));
				inItemCreation = true;
				m_canvas->m_viewMode.operItemResizing = true;
				if (m->modifiers() == Qt::ShiftModifier)
				{
					Doc->appMode = modeNormal;
					emit DocChanged();
					currItem->Sizing =  currItem->asLine() ? false : true;
					inItemCreation = false;
					updateContents();
				}
				else
				{
					m_canvas->m_viewMode.operItemMoving = true;
					m_canvas->setRenderModeFillBuffer();
				}
				break;
			}
		case modeDrawBezierLine:
			if (m->button() == Qt::RightButton)
				break;
			if (FirstPoly)
			{
				selectPage(m);
				z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenLine, !m_canvas->m_viewMode.m_MouseButtonPressed);
				currItem = Doc->Items->at(z);
				Doc->m_Selection->clear();
				Doc->m_Selection->addItem(currItem);
				qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
				m_canvas->setRenderModeFillBuffer();
			}
			currItem = Doc->m_Selection->itemAt(0);
//			pm.translate(-Doc->minCanvasCoordinate.x()*m_canvas->scale(), -Doc->minCanvasCoordinate.y()*m_canvas->scale());
			m_canvas->Transform(currItem, pm);
			npf = FPoint(m->pos() * pm.inverted());
			npf = Doc->ApplyGridF(npf);
			currItem->PoLine.addPoint(npf);
			npf2 = getMinClipF(&currItem->PoLine);
			if (npf2.x() < 0)
			{
				currItem->PoLine.translate(-npf2.x(), 0);
				Doc->MoveItem(npf2.x(), 0, currItem);
			}
			if (npf2.y() < 0)
			{
				currItem->PoLine.translate(0, -npf2.y());
				Doc->MoveItem(0, npf2.y(), currItem);
			}
			Doc->SizeItem(currItem->PoLine.WidthHeight().x(), currItem->PoLine.WidthHeight().y(), currItem->ItemNr, false, false, false);
			currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2, 1.0)));
			m_canvas->newRedrawPolygon();
			break;
		case modeInsertPDFButton:
		case modeInsertPDFTextfield:
		case modeInsertPDFCheckbox:
		case modeInsertPDFCombobox:
		case modeInsertPDFListbox:
		case modeInsertPDFTextAnnotation:
		case modeInsertPDFLinkAnnotation:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			Doc->ApplyGuides(&Rxp, &Ryp);
			z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_canvas->m_viewMode.m_MouseButtonPressed);
			currItem = Doc->Items->at(z);
			currItem->setIsAnnotation(true);
			currItem->AutoName = false;
			switch (Doc->appMode)
			{
			case modeInsertPDFButton:
				currItem->annotation().setType(2);
				currItem->annotation().setFlag(65536);
				break;
			case modeInsertPDFTextfield:
				currItem->annotation().setType(3);
				break;
			case modeInsertPDFCheckbox:
				currItem->annotation().setType(4);
				break;
			case modeInsertPDFCombobox:
				currItem->annotation().setType(5);
				currItem->annotation().setFlag(131072);
				break;
			case modeInsertPDFListbox:
				currItem->annotation().setType(6);
				break;
			case modeInsertPDFTextAnnotation:
				currItem->annotation().setType(10);
				break;
			case modeInsertPDFLinkAnnotation:
				currItem->annotation().setType(11);
				currItem->annotation().setZiel(Doc->currentPage()->pageNr());
				currItem->annotation().setAction("0 0");
				currItem->setTextFlowMode(PageItem::TextFlowDisabled);
				break;
			}
			SetupDraw(z);
			break;
		case modeDrawFreehandLine:
			if (m->button() != Qt::LeftButton)
				break;
			RecordP.resize(0);
			Deselect(false);
			Mxp = qRound(m->x()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.x());
			Myp = qRound(m->y()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
			SeRx = Mxp;
			SeRy = Myp;
			m_canvas->setRenderModeFillBuffer();
			break;
		case modeDrawTable:
			if (m->button() != Qt::LeftButton)
				break;
			Deselect(false);
//			Mxp = qRound(m->x()/m_canvas->scale() + Doc->minCanvasCoordinate.x());
//			Myp = qRound(m->y()/m_canvas->scale() + Doc->minCanvasCoordinate.y());
			Rxp = m->x()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.x();
			Ryp = m->y()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.y();
			npf = Doc->ApplyGridF(FPoint(Rxp, Ryp));
			Rxp = npf.x();
			Ryp = npf.y();
			Doc->ApplyGuides(&Rxp, &Ryp);
			GyM = -1;
			GxM = -1;
			Mxp = qRound(Rxp);
			Myp = qRound(Ryp);
			SeRx = Mxp;
			SeRy = Myp;
			Dxp = m->globalPos().x();
			Dyp = m->globalPos().y();
			redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
			redrawMarker->show();
			break;
		case modePanning:
			break;
		case modeMeasurementTool:
			m_canvas->setRenderModeFillBuffer();
		case modeEditGradientVectors:
			if (m->button() != Qt::LeftButton)
				break;
			m_canvas->m_viewMode.m_MouseButtonPressed = true;
			qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
			Dxp = m->x();
			Dyp = m->y();
			Mxp = m->x();
			Myp = m->y();
			break;
		case modeCopyProperties:
			if (m->button() != Qt::LeftButton)
				break;
			SeleItem(m);
			if (GetItem(&currItem))
			{
				double gsx, gsy, gex, gey;
				double scaleX, scaleY, offsetX, offsetY, rotation;
				currItem->ColGap = Doc->ElemToLink->ColGap;
				currItem->Cols = Doc->ElemToLink->Cols;
				currItem->BottomLine = Doc->ElemToLink->BottomLine;
				currItem->TopLine = Doc->ElemToLink->TopLine;
				currItem->LeftLine = Doc->ElemToLink->LeftLine;
				currItem->RightLine = Doc->ElemToLink->RightLine;
				currItem->setTextToFrameDist(Doc->ElemToLink->textToFrameDistLeft(), Doc->ElemToLink->textToFrameDistRight(), Doc->ElemToLink->textToFrameDistTop(), Doc->ElemToLink->textToFrameDistBottom());
				currItem->setLineStyle(Doc->ElemToLink->lineStyle());
				currItem->setLineWidth(Doc->ElemToLink->lineWidth());
				currItem->setLineTransparency(Doc->ElemToLink->lineTransparency());
				currItem->setLineBlendmode(Doc->ElemToLink->lineBlendmode());
				currItem->setLineShade(Doc->ElemToLink->lineShade());
				currItem->setLineColor(Doc->ElemToLink->lineColor());
				currItem->setLineEnd(Doc->ElemToLink->lineEnd());
				currItem->setLineJoin(Doc->ElemToLink->lineJoin());
				currItem->setCustomLineStyle(Doc->ElemToLink->customLineStyle());
				currItem->setEndArrowIndex(Doc->ElemToLink->endArrowIndex());
				currItem->setStartArrowIndex(Doc->ElemToLink->startArrowIndex());
				currItem->setFillColor(Doc->ElemToLink->fillColor());
				currItem->setFillShade(Doc->ElemToLink->fillShade());
				currItem->setFillTransparency(Doc->ElemToLink->fillTransparency());
				currItem->setFillBlendmode(Doc->ElemToLink->fillBlendmode());
				currItem->fill_gradient = Doc->ElemToLink->fill_gradient;
				currItem->setGradientType(Doc->ElemToLink->gradientType());
				Doc->ElemToLink->gradientVector(gsx, gsy, gex, gey);
				currItem->setGradientVector(gsx, gsy, gex, gey);
				currItem->setPattern(Doc->ElemToLink->pattern());
				Doc->ElemToLink->patternTransform(scaleX, scaleY, offsetX, offsetY, rotation);
				currItem->setPatternTransform(scaleX, scaleY, offsetX, offsetY, rotation);
				Doc->ElemToLink = currItem;
				emit DocChanged();
				updateContents();
			}
			else
			{
				Doc->ElemToLink = NULL;
				Doc->appMode = modeNormal;
				emit PaintingDone();
			}
			break;
	}
}
#endif


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

void ScribusView::ToView(QPainter *p)
{
	QPoint out(contentsToViewport(QPoint(0, 0)));
//	p->translate(qRound(-Doc->minCanvasCoordinate.x()*m_canvas->scale()), qRound(-Doc->minCanvasCoordinate.y()*m_canvas->scale()));
	p->translate(out.x(), out.y());
}

void ScribusView::ToView(QMatrix& m)
{
	QPoint out(contentsToViewport(QPoint(0, 0)));
//	m.translate(qRound(-Doc->minCanvasCoordinate.x()*m_canvas->scale()), qRound(-Doc->minCanvasCoordinate.y()*m_canvas->scale()));
	m.translate(out.x(), out.y());
}

void ScribusView::RefreshItem(PageItem *currItem)
{
	updateContents(currItem->getRedrawBounding(m_canvas->scale()));
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
	RefreshItem(currItem);
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

void ScribusView::Reset1Control()
{
	Doc->nodeEdit.reset1Control(Doc->m_Selection->itemAt(0));
}

void ScribusView::ResetControl()
{
	PageItem *currItem = Doc->m_Selection->itemAt(0);
	Doc->nodeEdit.resetControl(currItem);
}



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
	//		m_cursorVisible = true;
#if 0
//#ifndef NLS_PROTO
			//Work out which column we are in
			double colWidth=currItem->columnWidth();
			double colGap=currItem->ColGap;
			int currCol=0;
			int cp=static_cast<int>(currItem->xPos()+currItem->textToFrameDistLeft()+qRound(colGap)+qRound(colWidth));
			while (xP>cp)
			{
				++currCol;
				cp+=qRound(colGap);
				cp+=qRound(colWidth);
			};
			int a, xp=0, yp=0, h;
			bool breakAndReturn=false;
			int currItemTextCount=currItem->itemText.length();
			for (a = currItem->firstInFrame(); a <= currItem->lastInFrame(); ++a)
			{
				xp = static_cast<int>(currItem->itemText.item(a)->glyph.xoffset);
				//If x pos of curr char is less than left position of current column, continue
				if (xp+currItem->xPos()<cp-colWidth-colGap)
					continue;
				yp = static_cast<int>(currItem->itemText.item(a)->glyph.yoffset);
				h = static_cast<int>(currItem->itemText.paragraphStyle(a).lineSpacing());

				if (a<currItemTextCount-1)
				{
					if ((xp+currItem->xPos()+1>=xP) && (yp+currItem->yPos()>=yP))
					{
						//beginning of new paragraph or not
						if (((currItem->itemText.text(a) == m_canvas->setRenderModeFillBufferChars::PARSEP) || (currItem->itemText.text(a) == SpecialChars::LINEBREAK)))
							currItem->CPos = a+1;
						else
							currItem->CPos = a;
						p.end();
						breakAndReturn=true;
						break;
					}
					if (yp-h+currItem->yPos()<=yP && yp+currItem->yPos()>=yP)
					{
						//click where next char is on next line in same or next column
						if (static_cast<int>(currItem->itemText.item(a+1)->glyph.yoffset)!=yp)
						{
							if (((currItem->itemText.text(a+1) == SpecialChars::PARSEP) || (currItem->itemText.text(a+1) == SpecialChars::LINEBREAK)))
								currItem->CPos = a+1;
							else
								currItem->CPos = a;
							p.end();
							breakAndReturn=true;
							break;
						}
					}
				}

				/* CB old, rather complicated code...
				chx = currItem->itemText.at(a)->ch;
				if (chx == QChar(30))
					chx = currItem->ExpandToken(a);
				if (chx == QChar(29))
					chx = " ";
				chs = currItem->itemText.at(a)->csize;
				currItem->SetZeichAttr(currItem->itemText.charStyle(a), &chs, &chx);
				if ((chx == QChar(13)) || (chx == QChar(9)) || (chx == QChar(27)))
					w = 1;
				else
					w = qRound(Cwidth(Doc, currItem->itemText.at(a)->cfont, chx, chs)*(currItem->itemText.at(a)->cscale / 1000.0));
				h = static_cast<int>(Doc->docParagraphStyles[currItem->itemText.at(a)->cab].LineSpa);
				//If we click within text
				if (QRegion(p.xForm(QRect(xp-1, yp-h, w+1, h))).contains(QPoint(x, y)))
				{
					currItem->CPos = a;
					p.end();
					breakAndReturn=true;
					break;
				}
				//If we are not at the end of the text and the next characters y position > current one
				//IE, we are at the end of the line
				else if ((a<currItemTextCount-1) && (static_cast<int>(currItem->itemText.at(a+1)->yp)>yp))
				{
					//If one column only and we are within the width of the frame
					//Yes, some people want selection outside the frame to work too, this works ok for now
					if ((currItem->Cols==1) && (QRegion(p.xForm(QRect(xp-1, yp-h, static_cast<int>(currItem->width()-xp-1), h))).contains(QPoint(x, y))))
					{
						currItem->CPos = a;
						p.end();
						breakAndReturn=true;
						break;
					}
					//If columns>1, then work out what column we are in and place our cursor
					//TODO: Move this column placement code into PageItem_TextFrame.
					else if (currItem->Cols>1)
					{
						double colWidth=currItem->columnWidth();
						int currCol=0;
						int cp=static_cast<int>(currItem->xPos()+currItem->textToFrameDistLeft());
						do
						{
							++currCol;
							cp+=qRound(currItem->ColGap);
							if (currCol>1)
								cp+=qRound(colWidth);
						} while (xp>cp);
						if (QRegion(p.xForm(QRect(xp-1, yp-h, static_cast<int>(cp-xp-1), h))).contains(QPoint(x, y)))
						{
							currItem->CPos = a;
							p.end();
							breakAndReturn=true;
							break;
						}
					}
				}*/
			}
			if (breakAndReturn)
			{
				Doc->currentStyle.charStyle() = currItem->itemText.charStyle(a);
				emit ItemTextStrike(currItem->itemText.charStyle(a).strikethruOffset(), currItem->itemText.charStyle(a).strikethruWidth());
				emit ItemTextUnderline(currItem->itemText.charStyle(a).underlineOffset(), currItem->itemText.charStyle(a).underlineWidth());
				emit ItemTextOutline(currItem->itemText.charStyle(a).outlineWidth());
				emit ItemTextShadow(currItem->itemText.charStyle(a).shadowXOffset(), currItem->itemText.charStyle(a).shadowYOffset());
				emit ItemTextBase(currItem->itemText.charStyle(a).baselineOffset());
				emit ItemTextSca(currItem->itemText.charStyle(a).scaleH());
				emit ItemTextScaV(currItem->itemText.charStyle(a).scaleV());
				emit ItemTextFont(currItem->itemText.charStyle(a).font().scName());
				emit ItemTextSize(currItem->itemText.charStyle(a).fontSize());
				emit ItemTextUSval(currItem->itemText.charStyle(a).tracking());
				emit ItemTextStil(currItem->itemText.charStyle(a).effects());
				emit ItemTextAbs(currItem->itemText.paragraphStyle(a).alignment());
				emit ItemTextFarben(currItem->itemText.charStyle(a).strokeColor(), currItem->itemText.charStyle(a).fillColor(), currItem->itemText.charStyle(a).strokeShade(), currItem->itemText.charStyle(a).fillShade());
				return true;
			}

			/*currItemTextCount=currItem->itemText.count();
			qDebug(QString("%1").arg(2));
			for (a=0; a<currItemTextCount; ++a)
			{
				xp = static_cast<int>(currItem->itemText.at(a)->xp);
				yp = static_cast<int>(currItem->itemText.at(a)->yp);
				QPoint np = p.xForm(QPoint(xp, yp - static_cast<int>(Doc->docParagraphStyles[currItem->itemText.at(a)->cab].LineSpa)));
				if (yP < np.y())
				{
					currItem->CPos = a;
					p.end();
					uint i=a;
					if (a > 0)
						i--;
					Doc->CurrFont = currItem->itemText.at(i)->cfont->scName();
					Doc->CurrFontSize = currItem->itemText.at(i)->csize;
					Doc->CurrTextFill = currItem->itemText.at(i)->ccolor;
					Doc->CurrTextFillSh = currItem->itemText.at(i)->cshade;
					Doc->CurrTextStroke = currItem->itemText.at(i)->cstroke;
					Doc->CurrTextStrokeSh = currItem->itemText.at(i)->cshade2;
					Doc->CurrTextScale = currItem->itemText.at(i)->cscale;
					Doc->CurrTextScaleV = currItem->itemText.at(i)->cscalev;
					Doc->CurrTextBase = currItem->itemText.at(i)->cbase;
					Doc->CurrTextShadowX = currItem->itemText.at(i)->cshadowx;
					Doc->CurrTextShadowY = currItem->itemText.at(i)->cshadowy;
					Doc->CurrTextOutline = currItem->itemText.at(i)->coutline;
					Doc->CurrTextUnderPos = currItem->itemText.at(i)->cunderpos;
					Doc->CurrTextUnderWidth = currItem->itemText.at(i)->cunderwidth;
					Doc->CurrTextStrikePos = currItem->itemText.at(i)->cstrikepos;
					Doc->CurrTextStrikeWidth = currItem->itemText.at(i)->cstrikewidth;
					emit ItemTextStrike(currItem->itemText.at(i)->cstrikepos, currItem->itemText.at(i)->cstrikewidth);
					emit ItemTextUnderline(currItem->itemText.at(i)->cunderpos, currItem->itemText.at(i)->cunderwidth);
					emit ItemTextOutline(currItem->itemText.at(i)->coutline);
					emit ItemTextShadow(currItem->itemText.at(i)->cshadowx, currItem->itemText.at(i)->cshadowy);
					emit ItemTextSca(currItem->itemText.at(i)->cscale);
					emit ItemTextScaV(currItem->itemText.at(i)->cscalev);
					emit ItemTextFarben(currItem->itemText.at(i)->cstroke, currItem->itemText.at(i)->ccolor, currItem->itemText.at(i)->cshade2, currItem->itemText.at(i)->cshade);
					emit ItemTextFont(currItem->itemText.at(i)->cfont->scName());
					emit ItemTextSize(currItem->itemText.at(i)->csize);
					emit ItemTextUSval(currItem->itemText.at(i)->cextra);
					emit ItemTextStil(currItem->itemText.at(i)->cstyle);
					emit ItemTextAbs(currItem->itemText.at(i)->cab);
					emit ItemTextBase(currItem->itemText.at(i)->cbase);
					return true;
				}
			}
			*/
			//End of text in frame
			if (currItem->itemText.length() != 0)
			{
				a=currItem->lastInFrame();
				int w = qRound(currItem->itemText.charStyle(a).font().charWidth(currItem->itemText.text(a), currItem->itemText.charStyle(a).fontSize())*(currItem->itemText.charStyle(a).scaleH() / 1000.0));
				if (xp+currItem->xPos()+w<xP || yp+currItem->yPos()<yP)
					currItem->CPos = a+1;
				else
					currItem->CPos = a;
			}
			else
				currItem->CPos = 0;
			p.end();

#else
			FPoint point((x + 0*Doc->minCanvasCoordinate.x()) / m_canvas->scale() - currItem->xPos(), 
						 (y + 0*Doc->minCanvasCoordinate.x()) / m_canvas->scale() - currItem->yPos());
			currItem->CPos = currItem->itemText.length() == 0 ? 0 :
				currItem->itemText.screenToPosition(point);			
#endif
			
			if (currItem->itemText.length() > 0)
			{
				int b=currItem->CPos-1;
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

void ScribusView::slotDoCurs(bool draw)
{
//	disabling that function for now
	return;

	PageItem *currItem;
	if (GetItem(&currItem))
	{
		PageItem_TextFrame * textframe = currItem->asTextFrame();
		if ( !textframe )
			return;
		
		// don't mess around with itemText when layout() is about to happen
		if (currItem->invalid)
			return;
		
		//              textframe->lastTextItem = qMin(textframe->lastTextItem, 
		//                                             signed(textframe->itemText.nrOfItems()) - 1);
		
		if (textframe->firstInFrame() >= 0 
			&& ((textframe->lastInFrame() >= textframe->firstInFrame()
				 && textframe->CPos >= textframe->itemText.startOfItem(textframe->firstInFrame())
				 && textframe->CPos <= textframe->itemText.endOfItem(textframe->lastInFrame())
				 ) 
				|| 
				textframe->CPos >= textframe->itemText.length()
				)
			)
		{
			QPainter p;
			p.begin(viewport());
			ToView(&p);
			m_canvas->Transform(textframe, &p);
			m_canvas->TransformM(textframe, &p);
			
			if (Doc->CurTimer != 0)
				Doc->CurTimer->stop();
			
			int x, y, y1;
#ifdef NLS_PROTO
			if (textframe->CPos == textframe->itemText.endOfItem(textframe->lastInFrame()))
			{
				ScScriptItem * last = textframe->itemText.item(textframe->lastInFrame());
				x = static_cast<int>(last->x + last->naturalWidth);
				y = static_cast<int>(last->y);
				y1 = static_cast<int>(last->y - 12);
				qDebug(QString("cursor at end (%1,%2)").arg(x).arg(y));
			}
			else
			{
				glyph_metrics_t bbox = textframe->itemText.boundingBox(textframe->CPos);
				x = static_cast<int>(bbox.x);
				y = static_cast<int>(bbox.y);
				y1 = static_cast<int>(bbox.y - bbox.height);
//				qDebug(QString("cursor at (%1,%2) + %3").arg(bbox.x).arg(bbox.y).arg(bbox.height));
			}
#else
			if (textframe->CPos > textframe->itemText.length())
			{
				textframe->CPos = textframe->itemText.length();
			}
			if (textframe->lastInFrame() >= signed(textframe->itemText.nrOfItems()) 
				|| textframe->itemText.length() == 0)
			{
				x = 0;
				y = 0;
				y1 = static_cast<int>(textframe->itemText.defaultStyle().charStyle().fontSize() / 10);
			}
			else if ( textframe->CPos > textframe->itemText.endOfItem(textframe->lastInFrame())
					  || (textframe->CPos >= textframe->itemText.length() && textframe->itemText.text(textframe->itemText.length()-1) != SpecialChars::PARSEP) )
			{
				FRect bbox = textframe->itemText.boundingBox(qMax(0,qMin(textframe->lastInFrame(), textframe->itemText.length()-1)));
				x = static_cast<int>(bbox.x() + textframe->itemText.item(qMax(0,qMin(textframe->lastInFrame(), textframe->itemText.length()-1)))->glyph.wide());
				y = static_cast<int>(bbox.y());
				if (bbox.height() <= 2)
					y1 = static_cast<int>(bbox.y() + textframe->itemText.defaultStyle().charStyle().fontSize() / 30);
				else
					y1 = static_cast<int>(bbox.y() + bbox.height());
			}
			else
			{
				FRect bbox = textframe->itemText.boundingBox(qMax(0,qMin(textframe->CPos, textframe->itemText.length())));
				x = static_cast<int>(bbox.x());
				y = static_cast<int>(bbox.y());
				if (bbox.height() <= 2) 
					y1 = static_cast<int>(bbox.y() + textframe->itemText.charStyle(textframe->CPos).fontSize() / 30);
				else
					y1 = static_cast<int>(bbox.y() + bbox.height());
			}
#endif
			// avoid displaying the cursor on the frameborder
			if (x < 1)
				x = 1;
			p.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			p.setCompositionMode(QPainter::CompositionMode_Xor);
			if (draw)
			{
				p.drawLine(x, qMin(qMax(y,0),static_cast<int>(currItem->height())), 
						   x, qMin(qMax(y1,0),static_cast<int>(currItem->height())));
				m_cursorVisible = !m_cursorVisible;
			if (Doc->CurTimer != 0)
				{
					if (!Doc->CurTimer->isActive())
						Doc->CurTimer->start(500);
				}
			}
			else
			{
				if (Doc->CurTimer != 0)
					Doc->CurTimer->stop();
				if (m_cursorVisible)
					p.drawLine(x, qMin(qMax(y,0),static_cast<int>(currItem->height())), 
							   x, qMin(qMax(y1,0),static_cast<int>(currItem->height())));
				m_cursorVisible = false;
			}
			p.end();
		}
	}
}

void ScribusView::blinkCursor()
{
//	disabling that function for now
	return;
	slotDoCurs(true);
}

void ScribusView::dragTimerTimeOut()
{
	dragTimerFired = true;
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

void ScribusView::selectPage(QMouseEvent *m)
{
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	Mxp = static_cast<int>(m->x()/m_canvas->scale());
	Myp = static_cast<int>(m->y()/m_canvas->scale());
	QRect mpo(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
//	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
	Doc->nodeEdit.deselect();
	Deselect(false);
	if (!Doc->masterPageMode())
	{
		int i = Doc->OnPage(Mxp + 0*Doc->minCanvasCoordinate.x(), Myp + 0*Doc->minCanvasCoordinate.y());
		if (i!=-1)
		{
			uint docCurrPageNo=Doc->currentPageNumber();
			uint j=static_cast<uint>(i);
			if (docCurrPageNo != j)
			{
				Doc->setCurrentPage(Doc->Pages->at(j));
				setMenTxt(j);
				DrawNew();
			}
		}
/*		uint docPagesCount=Doc->Pages->count();
		uint docCurrPageNo=Doc->currentPageNumber();
		for (uint i = 0; i < docPagesCount; ++i)
		{
			int x = static_cast<int>(Doc->Pages->at(i)->xOffset() * m_canvas->scale());
			int y = static_cast<int>(Doc->Pages->at(i)->yOffset() * m_canvas->scale());
			int w = static_cast<int>(Doc->Pages->at(i)->width() * m_canvas->scale());
			int h = static_cast<int>(Doc->Pages->at(i)->height() * m_canvas->scale());
			if (QRect(x, y, w, h).intersects(mpo))
			{
				if (docCurrPageNo != i)
				{
					Doc->setCurrentPage(Doc->Pages->at(i));
					setMenTxt(i);
					DrawNew();
				}
				break;
			}
		} */
		setRulerPos(contentsX(), contentsY());
	}
}

//CB-->Doc/Fix
bool ScribusView::SeleItem(QMouseEvent *m)
{
	QMatrix p;
	QRectF tx, mpo;
	PageItem *currItem;
	m_canvas->m_viewMode.m_MouseButtonPressed = true;
	Mxp = static_cast<int>(m->x()/m_canvas->scale());
	Myp = static_cast<int>(m->y()/m_canvas->scale());
	int MxpS = static_cast<int>(m->x()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.x());
	int MypS = static_cast<int>(m->y()/m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
	mpo = QRectF(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
//	mpo.translate(Doc->minCanvasCoordinate.x() * m_canvas->scale(), Doc->minCanvasCoordinate.y() * m_canvas->scale());
	Doc->nodeEdit.deselect();
	int a;
	if (!Doc->masterPageMode())
	{
		int pgNum = -1;
		int docPageCount = static_cast<int>(Doc->Pages->count() - 1);
		double bleedRight = 0.0;
		double bleedLeft = 0.0;
		double bleedBottom = 0.0;
		double bleedTop = 0.0;
		bool drawBleed = false;
		if (((Doc->bleeds.Bottom != 0.0) || (Doc->bleeds.Top != 0.0) || (Doc->bleeds.Left != 0.0) || (Doc->bleeds.Right != 0.0)) && (Doc->guidesSettings.showBleed))
			drawBleed = true;
		for (int a = docPageCount; a > -1; a--)
		{
			if (drawBleed)
				Doc->getBleeds(a, &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
			int x = static_cast<int>(Doc->Pages->at(a)->xOffset() - bleedLeft);
			int y = static_cast<int>(Doc->Pages->at(a)->yOffset() - bleedTop);
			int w = static_cast<int>(Doc->Pages->at(a)->width() + bleedLeft + bleedRight);
			int h = static_cast<int>(Doc->Pages->at(a)->height() + bleedBottom + bleedTop);
			if (QRect(x, y, w, h).contains(MxpS, MypS))
			{
				pgNum = static_cast<int>(a);
				if (drawBleed)  // check again if its really on the correct page
				{
					for (int a2 = docPageCount; a2 > -1; a2--)
					{
						int xn = static_cast<int>(Doc->Pages->at(a2)->xOffset());
						int yn = static_cast<int>(Doc->Pages->at(a2)->yOffset());
						int wn = static_cast<int>(Doc->Pages->at(a2)->width());
						int hn = static_cast<int>(Doc->Pages->at(a2)->height());
						if (QRect(xn, yn, wn, hn).contains(MxpS, MypS))
						{
							pgNum = static_cast<int>(a2);
							break;
						}
					}
				}
				break;
			}
		}
		if (pgNum!=-1)
		{
			uint docCurrPageNo=Doc->currentPageNumber();
			uint j=static_cast<uint>(pgNum);
			if (docCurrPageNo != j)
			{
				Doc->setCurrentPage(Doc->Pages->at(j));
				setMenTxt(j);
				DrawNew();
			}
		}
		setRulerPos(contentsX(), contentsY());
	}
	if (m->modifiers() == (Qt::ControlModifier | Qt::AltModifier))
		Deselect(false);

	if ((m->modifiers() == (Qt::ShiftModifier | Qt::AltModifier)) && (!Doc->masterPageMode()) && (Doc->currentPage()->FromMaster.count() != 0))
	{
		Page* Mp = Doc->MasterPages.at(Doc->MasterNames[Doc->currentPage()->MPageNam]);
		currItem = Doc->currentPage()->FromMaster.at(Doc->currentPage()->FromMaster.count()-1);
		int currNr = Doc->currentPage()->FromMaster.count()-1;
		for (a = 0; a < Doc->currentPage()->FromMaster.count(); ++a)
		{
			p = QMatrix();
			if ((currItem->LayerNr == Doc->activeLayer()) && (!Doc->layerLocked(currItem->LayerNr)))
			{
				double OldX = currItem->xPos();
				double OldY = currItem->yPos();
				if (!currItem->ChangedMasterItem)
				{
					currItem->moveBy(-Mp->xOffset() + Doc->currentPage()->xOffset(), -Mp->yOffset() + Doc->currentPage()->yOffset());
				}
				m_canvas->Transform(currItem, p);
				if ((QRegion(p.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo.toRect())) ||
						(QRegion(currItem->Clip * p).contains(mpo.toRect())))
				{
					Doc->m_Selection->setIsGUISelection(false);
					if (!currItem->isSelected())
					{
						if ((m->modifiers() != Qt::ShiftModifier) || (Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
							Deselect(false);
						if (currItem->Groups.count() != 0)
						{
							if (Doc->m_Selection->count() != 0)
							{
								if (Doc->m_Selection->findItem(currItem) == -1)
									Doc->m_Selection->addItem(currItem, true);
							}
							else
								Doc->m_Selection->addItem(currItem, true);
							if (m->modifiers() != (Qt::ControlModifier | Qt::AltModifier))
							{
								for (int ga=0; ga<Doc->Items->count(); ++ga)
								{
									if (Doc->Items->at(ga)->Groups.count() != 0)
									{
										if (Doc->Items->at(ga)->Groups.top() == currItem->Groups.top())
										{
											if (Doc->Items->at(ga)->ItemNr != currItem->ItemNr)
											{
												if (Doc->m_Selection->findItem(Doc->Items->at(ga)) == -1)
												{
													Doc->m_Selection->addItem(Doc->Items->at(ga), true);
												}

											}
											Doc->Items->at(ga)->isSingleSel = false;
										}
									}
								}
							}
							else
							{
								currItem->isSingleSel = true;
								updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							}
						}
						else
						{
							Doc->m_Selection->addItem(currItem, true);
							updateContents(currItem->getRedrawBounding(m_canvas->scale()));
						}
					}
					else
					{
						if (Doc->m_Selection->count() > 1)
						{
							PageItem *bb = Doc->m_Selection->itemAt(0);
							Doc->m_Selection->removeItem(currItem);
							Doc->m_Selection->prependItem(currItem);
							updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							updateContents(bb->getRedrawBounding(m_canvas->scale()));
						}
					}
					Doc->m_Selection->setIsGUISelection(true);
					Doc->m_Selection->connectItemToGUI();
					if (Doc->m_Selection->count() > 1)
					{
						for (int aa = 0; aa < Doc->m_Selection->count(); ++aa)
						{
							PageItem *bb = Doc->m_Selection->itemAt(aa);
							updateContents(bb->getRedrawBounding(m_canvas->scale()));
						}
						Doc->m_Selection->setGroupRect();
						double x, y, w, h;
						Doc->m_Selection->getGroupRect(&x, &y, &w, &h);
						emit ItemPos(x, y);
						emit ItemGeom(w, h);
						getGroupRectScreen(&x, &y, &w, &h);
						updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
						emit HaveSel(currItem->itemType());
					}
					else
					{
						currItem->emitAllToGUI();
					}
					if (!currItem->ChangedMasterItem)
					{
						currItem->setXYPos(OldX, OldY);
					}
					return true;
				}
				Doc->m_Selection->setIsGUISelection(true);
				Doc->m_Selection->connectItemToGUI();
				if (!currItem->ChangedMasterItem)
				{
					currItem->setXYPos(OldX, OldY);
				}
			}
			currNr--;
			currItem = Doc->currentPage()->FromMaster.at(currNr);
		}
	}
	if (Doc->Items->count() != 0)
	{
		if ((Doc->m_Selection->count() != 0) && (m->modifiers() == Qt::ControlModifier))
			currItem = Doc->m_Selection->itemAt(0);
		else
			currItem = Doc->Items->at(Doc->Items->count()-1);
		if ((m->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) && (Doc->m_Selection->count() != 0))
		{
			int currNr = Doc->Items->count();
			for (a = 0; a < Doc->Items->count(); ++a)
			{
				if (currItem->isSelected())
				{
					if (currItem->ItemNr == 0)
					{
						currItem = Doc->Items->at(Doc->Items->count()-1);
						break;
					}
					currNr--;
					currItem = Doc->Items->at(currNr);
					break;
				}
				currNr--;
				currItem = Doc->Items->at(currNr);
			}
		}
		Doc->m_Selection->setIsGUISelection(false);
		if (currItem == NULL)
		{
			Doc->m_Selection->setIsGUISelection(true);
			Doc->m_Selection->connectItemToGUI();
			Deselect(true);
		}
		else
		{
			//Where all basic selection occurs having found the click location and the current page
			int currNr = Doc->Items->indexOf(currItem);
			for (a = currNr; a > -1; a--)
			{
				currItem = Doc->Items->at(a);
				if ((Doc->masterPageMode())  && (!((currItem->OwnPage == -1) || (currItem->OwnPage == static_cast<int>(Doc->currentPage()->pageNr())))))
					continue;
				if ((currItem->LayerNr == Doc->activeLayer()) && (!Doc->layerLocked(currItem->LayerNr)))
				{
					p = QMatrix();
					m_canvas->Transform(currItem, p);
					if ((QRegion(p.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo.toRect())) ||
							(QRegion(currItem->Clip * p).contains(mpo.toRect())))
					{
						//If the clicked on item is not tagged as selected
						if (!currItem->isSelected())
						{
							if ((m->modifiers() != Qt::ShiftModifier) || (Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
								Deselect(false);
							//If we are selecting an item that is part of a group...
							if (currItem->Groups.count() != 0)
							{
								if (Doc->m_Selection->count() != 0)
								{
									if (Doc->m_Selection->findItem(currItem) == -1)
										Doc->m_Selection->addItem(currItem, true);
								}
								else
									Doc->m_Selection->addItem(currItem, true);
								//CB This is where we add the items of an unselected group
								if (m->modifiers() != (Qt::ControlModifier | Qt::AltModifier))
								{
									for (int ga=0; ga<Doc->Items->count(); ++ga)
									{
										if (Doc->Items->at(ga)->Groups.count() != 0)
										{
											if (Doc->Items->at(ga)->Groups.top() == currItem->Groups.top())
											{
												if (Doc->Items->at(ga)->ItemNr != currItem->ItemNr)
												{
													if (Doc->m_Selection->findItem(Doc->Items->at(ga)) == -1)
														Doc->m_Selection->addItem(Doc->Items->at(ga), true);
												}
												Doc->Items->at(ga)->isSingleSel = false;
											}
										}
									}
								}
								else
								{
									currItem->isSingleSel = true;
									updateContents(currItem->getRedrawBounding(m_canvas->scale()));
								}
							}
							else
							//If we are just selecting one item
							{
								//CB 301206 We shouldnt be ignoring the GUI here...
								//Doc->m_Selection->addItem(currItem, true);
								Doc->m_Selection->addItem(currItem);
								updateContents(currItem->getRedrawBounding(m_canvas->scale()));
							}
						}
						else //If the clicked on item is tagged as selected
						{
							if (Doc->m_Selection->count() > 1)
							{
								PageItem *bb = Doc->m_Selection->itemAt(0);
								Doc->m_Selection->removeItem(currItem);
								Doc->m_Selection->prependItem(currItem);
								updateContents(currItem->getRedrawBounding(m_canvas->scale()));
								//CB dont think we need to paint here when we paint below
								//CB With the change of 301206, perhaps we need to?
								updateContents(bb->getRedrawBounding(m_canvas->scale()));
							}
						}
						Doc->m_Selection->setIsGUISelection(true);
						Doc->m_Selection->connectItemToGUI();
						if (Doc->m_Selection->count() > 1)
						{
							for (int aa = 0; aa < Doc->m_Selection->count(); ++aa)
							{
								PageItem *bb = Doc->m_Selection->itemAt(aa);
								updateContents(bb->getRedrawBounding(m_canvas->scale()));
							}
							Doc->m_Selection->setGroupRect();
							double x, y, w, h;
							Doc->m_Selection->getGroupRect(&x, &y, &w, &h);
							emit ItemPos(x, y);
							emit ItemGeom(w, h);
							getGroupRectScreen(&x, &y, &w, &h);
							updateContents(QRect(static_cast<int>(x-5), static_cast<int>(y-5), static_cast<int>(w+10), static_cast<int>(h+10)));
							emit HaveSel(currItem->itemType());
						}
		//CB 301206 Unsure why we need this if the above is no longer ignoring the GUI
		// 				else
		// 				{
		// 					Doc->m_Selection->connectItemToGUI();
		// 					//CB Dont need this as creating the 0th selection does this
		// 					currItem->paintObj();
		// 				}
						if (Doc->m_Selection->count() == 1)
						{
							HandleSizer(currItem, mpo.toRect(), m);
							if ((frameResizeHandle == 0) && (!currItem->locked()))
								qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
						}
						else
						{
							qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
							m_canvas->m_viewMode.operItemResizing = false;
						}
						return true;
					}
				}
			}
		}
	}
	if ((Doc->guidesSettings.guidesShown) && (Doc->appMode == modeNormal) && (!Doc->GuideLock) && (Doc->OnPage(MxpS, MypS) != -1) && (Doc->m_Selection->count() == 0))
	{
		GxM = -1;
		GyM = -1;
		QMap<double, uint> tmpGuidesSel;
		Guides tmpGuides = Doc->currentPage()->guides.horizontals(GuideManagerCore::Standard);
		Guides::iterator it;
		uint yg = 0;
		uint xg = 0;
		double lowX = ((m->x() - Doc->guidesSettings.grabRad) / m_canvas->scale()) + 0*Doc->minCanvasCoordinate.x();
		double highX = ((m->x() + Doc->guidesSettings.grabRad) / m_canvas->scale()) + 0*Doc->minCanvasCoordinate.x();
		double lowY = ((m->y() - Doc->guidesSettings.grabRad) / m_canvas->scale()) + 0*Doc->minCanvasCoordinate.y();
		double highY = ((m->y() + Doc->guidesSettings.grabRad) / m_canvas->scale()) + 0*Doc->minCanvasCoordinate.y();
		for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++yg)
		{
			if (((*it) + Doc->currentPage()->yOffset() < highY) && ((*it)+Doc->currentPage()->yOffset() > lowY))
				tmpGuidesSel.insert(fabs(((*it)+Doc->currentPage()->yOffset()) - MypS), yg);
		}
		if (tmpGuidesSel.count() != 0)
		{
			GyM = tmpGuidesSel.begin().value();
			QPoint py = viewport()->mapFromGlobal(m->globalPos());
			DrHY = py.y();
		}
		tmpGuidesSel.clear();
		tmpGuides = Doc->currentPage()->guides.verticals(GuideManagerCore::Standard);
		for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++xg)
		{
			if (((*it) + Doc->currentPage()->xOffset() < highX) && ((*it)+Doc->currentPage()->xOffset() > lowX))
				tmpGuidesSel.insert(fabs(((*it)+Doc->currentPage()->xOffset()) - MypS), xg);
		}
		if (tmpGuidesSel.count() != 0)
		{
			GxM = tmpGuidesSel.begin().value();
			QPoint py = viewport()->mapFromGlobal(m->globalPos());
			DrVX = py.x();
		}
		if (GxM!=-1 || GyM!=-1)
		{
			if (GxM==-1)
			{
				// Horizontal Guide
				MoveGY = true;
				emit signalGuideInformation(0, qRound(Doc->currentPage()->guides.horizontal(GyM, GuideManagerCore::Standard) * 10000.0) / 10000.0);
			}
			else
			{
				// Vertical Guide
				MoveGX = true;
				emit signalGuideInformation(1, qRound(Doc->currentPage()->guides.vertical(GxM, GuideManagerCore::Standard) * 10000.0) / 10000.0);
			}
		}
	}
	Doc->m_Selection->setIsGUISelection(true);
	Doc->m_Selection->connectItemToGUI();
	if ((m->modifiers() != Qt::ShiftModifier) || (Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
		Deselect(true);
	return false;
}

//CB Fix item->old* stuff
int ScribusView::HandleSizer(PageItem *currItem, QRect mpo, QMouseEvent *m)
{
	currItem->OldB = currItem->width();
	currItem->OldH = currItem->height();
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	frameResizeHandle = 0;
	if (currItem->sizeLocked())
		return 0;
	m_canvas->PaintSizeRect(QRect());
	double d1;
	QMap<double,int> distance;
	FPoint n1(currItem->width(), currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
//	n1 -= QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
	d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 1);
	n1 = FPoint(0, 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
//	n1 -= QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
	d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 2);
	if (!currItem->asLine())
	{
		QPoint docMinCanvasCoordinate(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
		n1 = FPoint(currItem->width(), 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 3);
		n1 = FPoint(0, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 4);
		n1 = FPoint(currItem->width()/2, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 5);
		n1 = FPoint(currItem->width(), currItem->height()/2, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 6);
		n1 = FPoint(0, currItem->height()/2, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 7);
		n1 = FPoint(currItem->width()/2, 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
//		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * m_canvas->scale() - m->x(),2)+pow(n1.y() * m_canvas->scale() - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 8);
	}
	QList<int> result = distance.values();
	if (result.count() != 0)
		frameResizeHandle = result[0];
//	mpo.moveBy(qRound(-Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
	HandleCurs(currItem, mpo);
	if (frameResizeHandle != 0)
	{
		if (!currItem->asLine())
			currItem->Sizing = true;
		m_canvas->m_viewMode.operItemResizing = true;
	}
	return frameResizeHandle;
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
//			x -= Doc->minCanvasCoordinate.x();
//			y -= Doc->minCanvasCoordinate.y();
			updateContents(static_cast<int>(x*m_canvas->scale()-5), static_cast<int>(y*m_canvas->scale()-5), static_cast<int>(w*m_canvas->scale()+10), static_cast<int>(h*m_canvas->scale()+10));
		}
		else
		{
			currItem=Doc->m_Selection->itemAt(0);
			Doc->m_Selection->clear();
			if (currItem!=NULL)
				updateContents(currItem->getRedrawBounding(m_canvas->scale()));
		}
	}
	if (prop)
		emit HaveSel(-1);
}

void ScribusView::SetupDraw(int nr)
{
	PageItem* currItem = Doc->Items->at(nr);
//	currItem->setFont(Doc->toolSettings.defFont);
//	currItem->setFontSize(Doc->toolSettings.defSize);
	m_canvas->m_viewMode.operItemResizing = true;
	frameResizeHandle = 1;
	qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
	Doc->m_Selection->setIsGUISelection(true);
	Doc->m_Selection->clear();
	Doc->m_Selection->addItem(currItem);
	Doc->m_Selection->connectItemToGUI();
	updateContents(currItem->getRedrawBounding(m_canvas->scale()));
	m_canvas->m_viewMode.operItemMoving = true;
	Doc->appMode = modeNormal;
	emit DocChanged();
	currItem->Sizing =  currItem->asLine() ? false : true;
	inItemCreation = true;
	m_canvas->setRenderModeFillBuffer();
//	moveTimer = moveTimer.addSecs(1500);
}

void ScribusView::SetupDrawNoResize(int nr)
{
	PageItem* currItem = Doc->Items->at(nr);
//	currItem->setFont(Doc->toolSettings.defFont);
//	currItem->setFontSize(Doc->toolSettings.defSize);
	Doc->m_Selection->setIsGUISelection(true);
	Doc->m_Selection->clear();
	Doc->m_Selection->addItem(currItem);
	Doc->m_Selection->connectItemToGUI();
	updateContents(currItem->getRedrawBounding(m_canvas->scale()));
	if (!Prefs->stickyTools)
		Doc->appMode = modeNormal;
	emit DocChanged();
	currItem->Sizing =  currItem->asLine() ? false : true;
	inItemCreation = false;
	moveTimer = moveTimer.addSecs(1500);
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
		forceRedraw = true;
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
		forceRedraw = true;
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
		bool wasGUISelection=Doc->m_Selection->isGUISelection();
		if (wasGUISelection)
		{
			Doc->m_Selection->setIsGUISelection(false);
			Doc->m_Selection->disconnectAllItemsFromGUI();
		}
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
		if (wasGUISelection)
			tempSelection.setIsGUISelection(true);
		*Doc->m_Selection=tempSelection;
		emit LevelChanged(Doc->m_Selection->itemAt(0)->ItemNr);
		emit DocChanged();
		forceRedraw = true;
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
		bool wasGUISelection=Doc->m_Selection->isGUISelection();
		if (wasGUISelection)
		{
			Doc->m_Selection->setIsGUISelection(false);
			Doc->m_Selection->disconnectAllItemsFromGUI();
		}
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
		if (wasGUISelection)
			tempSelection.setIsGUISelection(true);
		*Doc->m_Selection=tempSelection;
		emit LevelChanged(Doc->m_Selection->itemAt(0)->ItemNr);
		emit DocChanged();
		forceRedraw = true;
		updateContents();
	}
}

//CB Remove emit/start pasting objects
void ScribusView::PasteToPage()
{
	int ac = Doc->Items->count();
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(Doc->currentPage()->getUName(), 0, Um::Paste, "", Um::IPaste);
	if (m_ScMW->Buffer2.contains("<SCRIBUSFRAGMENT"))
	{
		bool savedAlignGrid = Doc->useRaster;
		bool savedAlignGuides = Doc->SnapGuides;
		Selection pastedObjects = Serializer(*Doc).deserializeObjects(m_ScMW->Buffer2.toUtf8());
		Doc->useRaster = savedAlignGrid;
		Doc->SnapGuides = savedAlignGuides;
		pastedObjects.setGroupRect();
		double gx, gy, gh, gw;
		pastedObjects.getGroupRect(&gx, &gy, &gw, &gh);
		moveGroup(Mxp / m_canvas->scale() - gx, Myp / m_canvas->scale() - gy, false, &pastedObjects);
		Doc->m_Selection->clear();
	}
	else
		emit LoadElem(m_ScMW->Buffer2, Mxp / m_canvas->scale(), Myp / m_canvas->scale(), false, false, Doc, this);
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
		moveGroup(nx-gx, ny-gy, false, &newObjects);
		newObjects.setGroupRect();
		newObjects.getGroupRect(&gx, &gy, &gw, &gh);
		nx = gx+gw;
		ny = gy+gh;
		Doc->ApplyGuides(&nx, &ny);
		moveGroup(nx-(gx+gw), ny-(gy+gh), false, &newObjects);
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
	if (UndoManager::undoEnabled())
		undoManager->commit();
}

void ScribusView::PasteRecentToPage(int id)
{
	if (UndoManager::undoEnabled())
		undoManager->beginTransaction(Doc->currentPage()->getUName(), 0, Um::Paste, "", Um::IPaste);
	QString nam = pmen3->text(id);
	QString data = m_ScMW->scrapbookPalette->tempBView->objectMap[nam].Data;
	QFileInfo fi(data);
	if (fi.extension(true).toLower() == "sml")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createObjects(f);
		delete pre;
	}
	else if (fi.extension(true).toLower() == "shape")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createShape(f);
		delete pre;
	}
	else if (fi.extension(true).toLower() == "sce")
	{
		QString f = "";
		loadText(data, &f);
		data = f;
	}
	int ac = Doc->Items->count();
	emit LoadElem(data, Mxp / m_canvas->scale(), Myp / m_canvas->scale(), false, false, Doc, this);
	Doc->DraggedElem = 0;
	Doc->DragElements.clear();
	updateContents();
	for (int as = ac; as < Doc->Items->count(); ++as)
	{
		PageItem* currItem = Doc->Items->at(as);
		if (currItem->isBookmark)
			emit AddBM(currItem);
		Doc->m_Selection->addItem(currItem);
	}
	if (Doc->m_Selection->count() > 1)
	{
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
		moveGroup(nx-gx, ny-gy, false);
		Doc->m_Selection->setGroupRect();
		Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		nx = gx+gw;
		ny = gy+gh;
		Doc->ApplyGuides(&nx, &ny);
		moveGroup(nx-(gx+gw), ny-(gy+gh), false);
		Doc->m_Selection->setGroupRect();
		Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		emit ItemPos(gx, gy);
		emit ItemGeom(gw, gh);
		emit HaveSel(Doc->m_Selection->itemAt(0)->itemType());
	}
	else
	{
		PageItem *currItem = Doc->m_Selection->itemAt(0);
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
	if (UndoManager::undoEnabled())
		undoManager->commit();
}


void ScribusView::resizeEvent ( QResizeEvent * event )
{
	QScrollArea::resizeEvent(event);
	horizRuler->setGeometry(m_vhRulerHW, 1, width()-m_vhRulerHW-1, m_vhRulerHW);
	vertRuler->setGeometry(1, m_vhRulerHW, m_vhRulerHW, height()-m_vhRulerHW-1);
	rulerMover->setGeometry(1, 1, m_vhRulerHW, m_vhRulerHW);	
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

bool ScribusView::groupTransactionStarted()
{
	return _groupTransactionStarted;
}

void ScribusView::setGroupTransactionStarted(bool isOn)
{
	_groupTransactionStarted = isOn;
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
	evSpon = true;
	QString newStatusBarText(" ");
	if ((verticalScrollBar()->draggingSlider()) || (horizontalScrollBar()->draggingSlider()))
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
	reformPages(mov);
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
	MoveGY = false;
	MoveGX = false;
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

void ScribusView::adjustCanvas(double width, double height, double dX, double dY)
{
	if (!m_canvas->m_viewMode.operItemMoving)
	{
		bool updback = updateOn;
		updatesOn(false);
		disconnect(this, SIGNAL(contentsMoving(int, int)), this, SLOT(setRulerPos(int, int)));
		int oldDX = contentsX();
		int oldDY = contentsY();
		int nw = qMax(qRound(width * m_canvas->scale()), contentsWidth() + qRound(dX * m_canvas->scale()) * 2);
		int nh = qMax(qRound(height * m_canvas->scale()), contentsHeight() + qRound(dY * m_canvas->scale()) * 2);
		resizeContents(qMax(nw, visibleWidth() + qRound(dX * m_canvas->scale()) * 2), qMax(nh, visibleHeight() + qRound(dY * m_canvas->scale()) * 2));
		setContentsPos(oldDX + qRound(dX * m_canvas->scale()), oldDY + qRound(dY * m_canvas->scale()));
		setRulerPos(contentsX(), contentsY());
		connect(this, SIGNAL(contentsMoving(int, int)), this, SLOT(setRulerPos(int, int)));
		updatesOn(updback);
	}
	evSpon = false;
}

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

/** Fuehrt die Vergroesserung/Verkleinerung aus */
void ScribusView::slotDoZoom()
{
	undoManager->setUndoEnabled(false);
	if (m_canvas->scale() > 32*Prefs->DisScale)
	{
		setScale(32*Prefs->DisScale);
		return;
	}
	updatesOn(false);
	int nw = qMax(qRound((Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()) * m_canvas->scale()), visibleWidth());
	int nh = qMax(qRound((Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()) * m_canvas->scale()), visibleHeight());
	resizeContents(nw, nh);
	if (Doc->m_Selection->count() != 0)
	{
		PageItem *currItem = Doc->m_Selection->itemAt(0);
		SetCCPo(currItem->xPos() + currItem->width() / 2.0, currItem->yPos() + currItem->height() / 2.0);
	}
	else
		SetCCPo(oldX, oldY);
	updatesOn(true);
	DrawNew();
	undoManager->setUndoEnabled(true);
}

void ScribusView::setZoom()
{
	int x = qRound(qMax(contentsX() / m_canvas->scale(), 0.0));
	int y = qRound(qMax(contentsY() / m_canvas->scale(), 0.0));
	int w = qRound(qMin(visibleWidth() / m_canvas->scale(), Doc->currentPage()->width()));
	int h = qRound(qMin(visibleHeight() / m_canvas->scale(), Doc->currentPage()->height()));
	rememberOldZoomLocation(w / 2 + x,h / 2 + y);
	setScale(zoomSpinBox->value() / 100.0 * Prefs->DisScale);
	slotDoZoom();
	m_ScMW->setFocus();
}

void ScribusView::slotZoom100()
{
	int x = qRound(qMax(contentsX() / m_canvas->scale(), Doc->minCanvasCoordinate.x()));
	int y = qRound(qMax(contentsY() / m_canvas->scale(), Doc->minCanvasCoordinate.y()));
	int w = qRound(qMin(visibleWidth() / m_canvas->scale(), Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
	int h = qRound(qMin(visibleHeight() / m_canvas->scale(), Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
	rememberOldZoomLocation(w / 2 + x,h / 2 + y);
	setScale(Prefs->DisScale);
	slotDoZoom();
}

void ScribusView::slotZoomIn(int mx,int my)
{
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
	setScale(m_canvas->scale() * static_cast<double>(Doc->toolSettings.magStep)/100.0);
	slotDoZoom();
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut(int mx,int my)
{
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
	setScale(m_canvas->scale() / (static_cast<double>(Doc->toolSettings.magStep)/100.0));
	slotDoZoom();
}

FPoint ScribusView::translateToView(double x, double y)
{
	return FPoint((x - 0*Doc->minCanvasCoordinate.x()) * m_canvas->scale(), (y - 0*Doc->minCanvasCoordinate.y())* m_canvas->scale());
}

FPoint ScribusView::translateToView(FPoint in)
{
	return translateToView(in.x(), in.y());
}

FPoint ScribusView::translateToDoc(double x, double y)
{
	return FPoint(x / m_canvas->scale() + 0*Doc->minCanvasCoordinate.x(), y / m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
}

FPoint ScribusView::translateToDoc(FPoint in)
{
	return translateToDoc(in.x(), in.y());
}

FPoint ScribusView::translateFromViewport(double x, double y)
{
	return FPoint((x + contentsX()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.x(), (y + contentsY()) / m_canvas->scale() + 0*Doc->minCanvasCoordinate.y());
}

FPoint ScribusView::translateFromViewport(FPoint in)
{
	return translateFromViewport(in.x(), in.y());
}

FPoint ScribusView::translateToViewport(double x, double y)
{
	return FPoint((x - 0*Doc->minCanvasCoordinate.x()) * m_canvas->scale() - contentsX(), (y - 0*Doc->minCanvasCoordinate.y())* m_canvas->scale() - contentsY());
}

FPoint ScribusView::translateToViewport(FPoint in)
{
	return translateToViewport(in.x(), in.y());
}

void ScribusView::DrawNew()
{
	if (m_ScMW->ScriptRunning)
		return;
	evSpon = false;
	forceRedraw = true;
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
	FPoint nx = translateToView(x, y);
	QSize viewsize = viewport()->size();
	setContentsPos(qRound(nx.x()) + viewsize.width() / 2, qRound(nx.y()) + viewsize.height() / 2);
	setRulerPos(contentsX(), contentsY());
}

void ScribusView::SetCPo(double x, double y)
{
	if (m_ScMW->ScriptRunning)
		return;
	FPoint nx = translateToView(x, y);
	setContentsPos(qRound(nx.x()), qRound(nx.y()));
	setRulerPos(contentsX(), contentsY());
}

void ScribusView::updateLayerMenu()
{
	disconnect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	layerMenu->clear();
	QStringList newNames;
	Doc->orderedLayerList(&newNames);
	for (QStringList::Iterator it=newNames.begin(); it!=newNames.end(); ++it)
        layerMenu->insertItem(*it);
	connect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
}

void ScribusView::setLayerMenuText(const QString &layerName)
{
	disconnect(layerMenu, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	if (layerMenu->count() != 0)
		layerMenu->setCurrentText(layerName);
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

void ScribusView::GotoPa(int Seite)
{
	Deselect();
	GotoPage(Seite-1);
	m_ScMW->setFocus();
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
	slotDoZoom();
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
		forceRedraw = true;
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
		forceRedraw = false;
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
	double sx = maxGr / Doc->Pages->at(Nr)->width();
	double sy = maxGr / Doc->Pages->at(Nr)->height();
	double sc = qMin(sx, sy);
	int clipx = static_cast<int>(Doc->Pages->at(Nr)->xOffset() * sc);
	int clipy = static_cast<int>(Doc->Pages->at(Nr)->yOffset() * sc);
	int clipw = qRound(Doc->Pages->at(Nr)->width() * sc);
	int cliph = qRound(Doc->Pages->at(Nr)->height() * sc);
	if ((clipw > 0) && (cliph > 0))
	{
		double oldScale = m_canvas->scale();
		double cx = Doc->minCanvasCoordinate.x();
		double cy = Doc->minCanvasCoordinate.y();
		Doc->minCanvasCoordinate = FPoint(0, 0);
		bool frs = Doc->guidesSettings.framesShown;
		bool ctrls = Doc->guidesSettings.showControls;
		Doc->guidesSettings.framesShown = false;
		Doc->guidesSettings.showControls = false;
		m_canvas->setScale(sc);
		m_canvas->m_viewMode.previewMode = true;
		forceRedraw = true;
		Page* act = Doc->currentPage();
		Doc->setLoading(true);
		Doc->setCurrentPage(Doc->Pages->at(Nr));
		im = QImage(clipw, cliph, QImage::Format_ARGB32);
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
		m_canvas->DrawMasterItems(painter, Doc->Pages->at(Nr), QRect(clipx, clipy, clipw, cliph));
		m_canvas->DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		painter->endLayer();
		painter->end();
		Doc->guidesSettings.framesShown = frs;
		Doc->guidesSettings.showControls = ctrls;
		m_canvas->setScale(oldScale);
		Doc->setCurrentPage(act);
		Doc->setLoading(false);
		delete painter;
		painter=NULL;
		m_canvas->m_viewMode.previewMode = false;
		forceRedraw = false;
		Doc->minCanvasCoordinate = FPoint(cx, cy);
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

// //CB-->Doc
// //Fix size/move item calls
// void ScribusView::adjustFrametoImageSize()
// {
// 	uint docSelectionCount=Doc->m_Selection->count();
// 	if (docSelectionCount > 0)
// 	{
// 		bool toUpdate=false;
// 		for (uint i = 0; i < docSelectionCount; ++i)
// 		{
// 			PageItem *currItem = Doc->m_Selection->itemAt(i);
// 			if (currItem!=NULL)
// 			{
// 				if (currItem->asImageFrame() && currItem->PicAvail && !currItem->isTableItem)
// 				{
// 					undoManager->beginTransaction(Doc->m_Selection->count() == 1 ?
// 					                              currItem->getUName() : Um::SelectionGroup,
// 					                              Doc->m_Selection->count() == 1 ?
// 					                              currItem->getUPixmap() : Um::IGroup,
// 					                              Um::AdjustFrameToImage,"",Um::IResize);
// 					double w, h, x, y;
// 					w = currItem->OrigW * currItem->imageXScale();
// 					h = currItem->OrigH * currItem->imageYScale();
// 					x = currItem->imageXOffset() * currItem->imageXScale();
// 					y = currItem->imageYOffset() * currItem->imageYScale();
// 					Doc->SizeItem(w, h, currItem->ItemNr);
// 					Doc->MoveItem(x, y, currItem);
// 					currItem->setImageXYOffset(0.0, 0.0);
// 					toUpdate=true;
// 				}
// 			}
// 		}
// 		if (toUpdate)
// 		{
// 			updateContents();
// 			emit DocChanged();
// 			undoManager->commit();
// 		}
// 	}
// }

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
	qDebug() << "Pasting frame of type " << Buffer->PType;
	switch (Buffer->PType)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType1:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_canvas->m_viewMode.m_MouseButtonPressed);
		break;
	//
	case PageItem::ImageFrame:
		z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, x, y, w, h, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
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
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_canvas->m_viewMode.m_MouseButtonPressed);
		break;
	//
	case PageItem::PathText:
	case PageItem::TextFrame:
#ifndef NLS_PROTO
		if (Buffer->PType == PageItem::PathText)
			z = Doc->itemAdd(PageItem::PathText, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Buffer->Pcolor, !m_canvas->m_viewMode.m_MouseButtonPressed);
		else
			z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Buffer->Pcolor, !m_canvas->m_viewMode.m_MouseButtonPressed);
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
				wt = QStringList::split("\t", cc);
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
			pstyle.setLineSpacing(Buffer->LineSp);
			pstyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(Buffer->LineSpMode));
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
		z = Doc->itemAdd(PageItem::Line, PageItem::Unspecified, x, y, w ,0, pw, CommonStrings::None, Buffer->Pcolor2, !m_canvas->m_viewMode.m_MouseButtonPressed);
		break;
	case PageItem::Polygon:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_canvas->m_viewMode.m_MouseButtonPressed);
		break;
	case PageItem::PolyLine:
		z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_canvas->m_viewMode.m_MouseButtonPressed);
		break;
	case PageItem::Multiple:
		Q_ASSERT(false);
		break;
	case PageItem::LatexFrame:
		z = Doc->itemAdd(PageItem::LatexFrame, PageItem::Unspecified, x, y, w, h, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_canvas->m_viewMode.m_MouseButtonPressed);
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
		/*Doc->Items->at(z)->BBoxX = Buffer->BBoxX;
		Doc->Items->at(z)->BBoxH = Buffer->BBoxH; */
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
						currItem->SetFarbe(&tmp, Buffer->GrColor, Buffer->GrShade);
					currItem->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor, Buffer->GrShade);
					if ((Buffer->GrColor2 != CommonStrings::None) && (!Buffer->GrColor2.isEmpty()))
						currItem->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
					currItem->fill_gradient.addStop(tmp, 1.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
				}
				else
				{
					if ((Buffer->GrColor2 != CommonStrings::None) && (!Buffer->GrColor2.isEmpty()))
						currItem->SetFarbe(&tmp, Buffer->GrColor2, Buffer->GrShade2);
					currItem->fill_gradient.addStop(tmp, 0.0, 0.5, 1.0, Buffer->GrColor2, Buffer->GrShade2);
					if ((Buffer->GrColor != CommonStrings::None) && (!Buffer->GrColor.isEmpty()))
						currItem->SetFarbe(&tmp, Buffer->GrColor, Buffer->GrShade);
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

//CB get this out of here, remove the need completely if possible. Used for cpal on mpalette
/*
void ScribusView::QueryFarben()
{
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		emit ItemFarben(currItem->lineColor(), currItem->fillColor(), currItem->lineShade(), currItem->fillShade());
		emit ItemGradient(currItem->GrType);
		emit ItemTrans(currItem->fillTransparency(), currItem->lineTransparency());
		emit ItemBlend(currItem->fillBlendmode(), currItem->lineBlendmode());
	}
}
*/
void ScribusView::ToPicFrame()
{
	PageItem *currItem = Doc->m_Selection->itemAt(0);
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::ImageFrame);
	RefreshItem(newItem);
	SelectItem(newItem);
	emit DocChanged();
}

void ScribusView::ToPolyFrame()
{
	PageItem *currItem = Doc->m_Selection->itemAt(0);
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::Polygon);
	RefreshItem(newItem);
	SelectItem(newItem);
	emit DocChanged();
}

void ScribusView::ToTextFrame()
{
	PageItem *currItem = Doc->m_Selection->itemAt(0);
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::TextFrame);
	RefreshItem(newItem);
	SelectItem(newItem);
	emit DocChanged();
}

void ScribusView::ToBezierFrame()
{
	PageItem *currItem = Doc->m_Selection->itemAt(0);
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::PolyLine);
	RefreshItem(newItem);
	SelectItem(newItem);
	emit DocChanged();
}

void ScribusView::Bezier2Poly()
{
	PageItem *currItem = Doc->m_Selection->itemAt(0);
	Deselect(true);
	PageItem* newItem=Doc->convertItemTo(currItem, PageItem::Polygon);
	RefreshItem(newItem);
	SelectItem(newItem);
	emit DocChanged();
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
		if (polyLineItem->asPolyLine() || polyLineItem->asPolygon())
		{
			Deselect(true);
			PageItem* newItem=Doc->convertItemTo(currItem, PageItem::PathText, polyLineItem);
			RefreshItem(newItem);
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
	tmpSelection.copy(*Doc->m_Selection, false, false);
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
		undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(), Um::ToOutlines, "", 0);
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
					if ((chstr == QChar(13)) || (chstr == QChar(32)) || (chstr == QChar(29)))
					{
//						if ((chstr == QChar(32)) || (chstr == QChar(29)))
//							CurX += hl->glyph.wide();
						continue;
					}
					if (chstr == QChar(30))
					{
						chstr = currItem->ExpandToken(a);
						if (chstr == QChar(32))
						{
//							CurX += hl->glyph.wide();
							continue;
						}
					}
					double chs = charStyle.fontSize();
					if (hl->effects() & ScStyle_SmallCaps)
					{
						if (chstr.toUpper() != chstr)
						{
							chs = qMax(static_cast<int>(hl->fontSize() * Doc->typographicSettings.valueSmallCaps / 100), 1);
							chstr = chstr.toUpper();
						}
					}
//					double csi = static_cast<double>(chs) / 100.0;
					uint chr = chstr[0].unicode();
					QMatrix chma, chma2, chma3, chma4, chma6;
					QMatrix trafo = QMatrix( 1, 0, 0, -1, -hl->PRot, 0 );
					trafo *= QMatrix( hl->PtransX, hl->PtransY, hl->PtransY, -hl->PtransX, hl->glyph.xoffset, hl->glyph.yoffset);
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
					if (hl->baselineOffset() != 0)
						chma6.translate(0, (-charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0));
					uint gl = hl->font().char2CMap(chr);
					pts = hl->font().glyphOutline(gl);
					if (pts.size() < 4)
						continue;
					pts.map(chma * chma2 * chma3 * chma4 * chma6);
					pts.map(trafo);
					if (currItem->rotation() != 0)
					{
						QMatrix sca;
						sca.translate(currItem->xPos(), currItem->yPos());
						pts.map(sca);
					}
					uint z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), !m_canvas->m_viewMode.m_MouseButtonPressed);
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
//					CurX += hl->glyph.wide();
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
						if ((chstr == QChar(13)) || (chstr == QChar(32)) || (chstr == QChar(29)))
						{
							if ((chstr == QChar(32)) || (chstr == QChar(29)))
								CurX += hl->glyph.wide();
							continue;
						}
						if (chstr == QChar(30))
						{
							chstr = currItem->ExpandToken(a);
							if (chstr == QChar(32))
							{
								CurX += hl->glyph.wide();
								continue;
							}
						}
						double chs = charStyle.fontSize();
						if (hl->effects() & ScStyle_SmallCaps)
						{
							if (chstr.toUpper() != chstr)
							{
								chs = qMax(static_cast<int>(hl->fontSize() * Doc->typographicSettings.valueSmallCaps / 100), 1);
								chstr = chstr.toUpper();
							}
						}
						double csi = static_cast<double>(chs) / 100.0;
						uint chr = chstr[0].unicode();
						QMatrix chma, chma2, chma3, chma4, chma6;
						uint gl = hl->font().char2CMap(chr);
						pts = hl->font().glyphOutline(gl);
						if (pts.size() < 4)
							continue;
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
							uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), !m_canvas->m_viewMode.m_MouseButtonPressed);
							bb = Doc->Items->at(z);
							bb->setTextFlowMode(currItem->textFlowMode());
							bb->setSizeLocked(currItem->sizeLocked());
							bb->setLocked(currItem->locked());
							bb->NamedLStyle = currItem->NamedLStyle;
							bb->setItemName(currItem->itemName()+"+U"+ccounter.setNum(a));
							bb->AutoName = false;
							bb->setRotation(currItem->rotation());
							bb->PoLine.addQuadPoint(FPoint(0, 0), FPoint(0, 0), FPoint(hl->glyph.xadvance, 0), FPoint(hl->glyph.xadvance, 0));
							bb->setLineColor(hl->strokeColor());
							bb->setLineShade(hl->strokeShade());
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
						chma = QMatrix();
						chma.scale(hl->glyph.scaleH * charStyle.fontSize() / 100.00, hl->glyph.scaleV * charStyle.fontSize() / 100.0);
						pts.map(chma);
						chma = QMatrix();
						if (currItem->imageFlippedH() && (!currItem->reversed()))
							chma.scale(-1, 1);
						if (currItem->imageFlippedV())
							chma.scale(1, -1);
						pts.map(chma);
						uint z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), !m_canvas->m_viewMode.m_MouseButtonPressed);
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
						if (charStyle.effects() & ScStyle_Subscript)
							textY -= hl->glyph.yoffset;
						if (charStyle.effects() & ScStyle_Superscript)
							textY += hl->glyph.yoffset;
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
							uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), !m_canvas->m_viewMode.m_MouseButtonPressed);
							bb = Doc->Items->at(z);
							bb->setTextFlowMode(currItem->textFlowMode());
							bb->setSizeLocked(currItem->sizeLocked());
							bb->setLocked(currItem->locked());
							bb->NamedLStyle = currItem->NamedLStyle;
							bb->setItemName(currItem->itemName()+"+S"+ccounter.setNum(a));
							bb->AutoName = false;
							bb->setRotation(currItem->rotation());
							bb->PoLine.addQuadPoint(FPoint(0, 0), FPoint(0, 0), FPoint(hl->glyph.xadvance, 0), FPoint(hl->glyph.xadvance, 0));
							bb->setLineColor(hl->strokeColor());
							bb->setLineShade(hl->strokeShade());
							bb->setLineWidth(lw);
							FPoint tp2(getMinClipF(&bb->PoLine));
							bb->PoLine.translate(-tp2.x(), -tp2.y());
							FPoint tp(getMaxClipF(&bb->PoLine));
							bb->setWidthHeight(tp.x(), tp.y());
							bb->Clip = FlattenPath(bb->PoLine, bb->Segments);
							double textX = CurX;
							double textY = ls.y - st; // + hl->glyph.yoffset;
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
				uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), CommonStrings::None, currItem->lineColor(), !m_canvas->m_viewMode.m_MouseButtonPressed);
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
				Doc->Items->insert(currItem->ItemNr+1, newGroupedItems.at(ag));
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
		Doc->m_Selection->copy(tmpSelection, false, true);
		undoManager->commit();
	}
#endif
}

void ScribusView::contentsWheelEvent(QWheelEvent *w)
{
	//QScrollArea::contentsWheelEvent(w);
	evSpon = true;
	if ((m_canvas->m_viewMode.m_MouseButtonPressed) && (MidButt) || ( w->state() & Qt::ControlButton ))
	{
		w->delta() > 0 ? slotZoomIn() : slotZoomOut();
	}
	else
	{
		int dX=0,dY=0;
		int moveBy=(w->delta() < 0) ? Prefs->Wheelval : -Prefs->Wheelval;
		if ((w->orientation() != Qt::Vertical) || ( w->modifiers() & Qt::ShiftModifier ))
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
		m_ScMW->scrActions["editActionMode"]->setOn(true);
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
		m_ScMW->scrActions["editActionMode"]->setOn(!_isGlobalMode);
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

void ScribusView::slotUpdateContents()
{
	updateContents();
}

void ScribusView::slotUpdateContents(const QRect &r)
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
	
	unitChange();
}



bool ScribusView::eventFilter(QObject *obj, QEvent *event)
{
//	if (obj == horizRuler || obj == vertRuler || obj == rulerMover)
//		return true; // FIXME:av
	
	if (obj == widget() && event->type() == QEvent::MouseMove)
	{
		QMouseEvent* m = static_cast<QMouseEvent*> (event);
		qDebug() << "ScribusView::eventFilter MouseMove" << m->x() << m->y();
		emit MousePos(m->x()/m_canvas->scale(),// + m_doc->minCanvasCoordinate.x(), 
					  m->y()/m_canvas->scale()); // + m_doc->minCanvasCoordinate.y());
		horizRuler->Draw(m->x());
		vertRuler->Draw(m->y());
		m_canvasMode->mouseMoveEvent(m);
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent* m = static_cast<QMouseEvent*> (event);
		m_canvasMode->mouseReleaseEvent(m);
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* m = static_cast<QMouseEvent*> (event);
		m_canvasMode->mousePressEvent(m);
		return true;
	}
	else if (obj == widget() && event->type() == QEvent::MouseButtonDblClick)
	{
		QMouseEvent* m = static_cast<QMouseEvent*> (event);
		m_canvasMode->mouseDoubleClickEvent(m);
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
	else if (false) //obj == widget() && event->type() == QEvent::Paint)
	{
		qDebug() << "filtered paint event" ;
//		QPaintEvent* ev = static_cast<QPaintEvent *>(event);
		QPainter qp(viewport());
		QString iconFilePath(QString("%1%2").arg(ScPaths::instance().iconDir()).arg("scribus_logo.png"));
		QImage image(iconFilePath);
		qp.drawImage(0, 200, image);
//		drawContents(&qp, ev->rect().x(), ev->rect().y(), ev->rect().width(), ev->rect().height());
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

void ScribusView::resizeContents(int w, int h)
{
	widget()->resize(w,h);
}

QPoint ScribusView::contentsToViewport(QPoint p)
{
	return p + viewport()->pos();
}

QPoint ScribusView::viewportToContents(QPoint p)
{
	return p - viewport()->pos();
}

int ScribusView::contentsX()
{
	return horizontalScrollBar()->value();
}

int ScribusView::contentsY()
{
	return verticalScrollBar()->value();
}

int ScribusView::contentsWidth()
{
	return horizontalScrollBar()->maximum();
}

int ScribusView::contentsHeight()
{
	return verticalScrollBar()->maximum();
}

void ScribusView::setContentsPos(int x, int y)
{
	horizontalScrollBar()->setValue(x);
	verticalScrollBar()->setValue(y);
}

void ScribusView::scrollBy(int x, int y)
{
	setContentsPos(horizontalScrollBar()->value() + x, verticalScrollBar()->value() + y);
}

double ScribusView::scale() const 
{ 
	return m_canvas->scale(); 
}

#if 0
void ScribusView::handleNodeEditPress(QMouseEvent* m, QRect mpo)
{
	assert (false);
	
	FPoint npf, npf2;
	
	PageItem* currItem = Doc->m_Selection->itemAt(0);
	FPointArray Clip = Doc->nodeEdit.beginTransaction(currItem);
	bool edited = false;
	bool pfound = false;
	QMatrix pm;
//	pm.translate(-Doc->minCanvasCoordinate.x()*m_canvas->scale(), -Doc->minCanvasCoordinate.y()*m_canvas->scale());
	m_canvas->Transform(currItem, pm);
	npf2 = FPoint(m->pos() * pm.inverted());
	QMatrix pm2;
	m_canvas->Transform(currItem, pm2);
	for (int a=0; a < signed(Clip.size()); ++a)
	{
		if (((Doc->nodeEdit.EdPoints) && (a % 2 != 0)) || ((!Doc->nodeEdit.EdPoints) && (a % 2 == 0)))
				continue;
			npf = FPoint(Clip.pointQ(a) * pm2);
			QRect tx = QRect(static_cast<int>(npf.x()-3), static_cast<int>(npf.y()-3), 6, 6);
			if (tx.intersects(mpo))
			{
				Doc->nodeEdit.ClRe = a;
				if ((Doc->nodeEdit.EdPoints) && (Doc->nodeEdit.SelNode.contains(a) == 0))
				{
					if (m->state() == Qt::ShiftButton)
						Doc->nodeEdit.SelNode.append(a);
					else
					{
						Doc->nodeEdit.SelNode.clear();
						Doc->nodeEdit.SelNode.append(a);
					}
				}
				Doc->nodeEdit.update(QPointF(Clip.point(a).x(), Clip.point(a).y()));
				pfound = true;
				break;
			}
		}
		if ((!pfound) || (!Doc->nodeEdit.EdPoints))
			Doc->nodeEdit.SelNode.clear();
		
		if ((Doc->nodeEdit.submode == NodeEditContext::MOVE_POINT) && (Doc->nodeEdit.ClRe2 != -1) && !Doc->nodeEdit.hasNodeSelected())
		{
			Doc->nodeEdit.SegP1 = Doc->nodeEdit.ClRe2;
			Doc->nodeEdit.SegP2 = Doc->nodeEdit.ClRe2+2;
			Doc->nodeEdit.ClRe = Doc->nodeEdit.ClRe2;
		}
		FPointArray cli;
		uint EndInd = Clip.size();
		uint StartInd = 0;
		for (uint n = Doc->nodeEdit.ClRe; n < Clip.size(); ++n)
		{
			if (Clip.point(n).x() > 900000)
			{
				EndInd = n;
				break;
			}
		}
		if (Doc->nodeEdit.ClRe > 0)
		{
			for (uint n2 = Doc->nodeEdit.ClRe; n2 > 0; n2--)
			{
				if (n2 == 0)
					break;
				if (Clip.point(n2).x() > 900000)
				{
					StartInd = n2 + 1;
					break;
				}
					}
		}
		if (Doc->nodeEdit.submode == NodeEditContext::SPLIT_PATH)
		{
			if (!Doc->nodeEdit.EdPoints)
						return;
			if (!Doc->nodeEdit.hasNodeSelected())	// We don't have a Point, try to add one onto the current curve segment
			{
				bool foundP = false;
				uint seg = 0;
				double absDist = 9999999999.9;
				FPoint point = FPoint(0, 0);
				FPoint normal = FPoint(0, 0);
				FPoint tangent = FPoint(0, 0);
				FPoint nearPoint = FPoint(0, 0);
				double nearT = 0.0;
				QRect mpo2(0, 0, Doc->guidesSettings.grabRad*3, Doc->guidesSettings.grabRad*3);
				mpo2.moveCenter(QPoint(qRound(npf2.x()), qRound(npf2.y())));
				for (uint poi=0; poi<Clip.size()-3; poi += 4)
				{
					FPoint a1 = Clip.point(poi);
					FPoint a2 = Clip.point(poi+1);
					FPoint a3 = Clip.point(poi+3);
					FPoint a4 = Clip.point(poi+2);
					QPainterPath Bez;
					Bez.moveTo(a1.x(), a1.y());
					Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
					QPolygon cli2 = Bez.toFillPolygon().toPolygon();
					for (int clp = 0; clp < cli2.size()-1; ++clp)
					{
						if (PointOnLine(cli2.point(clp), cli2.point(clp+1), mpo2))
						{
							seg = poi;
							double sp = 0.0;
							double spadd = 1.0 / (Clip.lenPathSeg(seg) * m_canvas->scale());
							while (sp < 1.0)
							{
								Clip.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
								double d1 = fabs(sqrt(pow(point.x() - npf2.x(), 2) + pow(point.y() - npf2.y() ,2)));
								if (d1 < absDist)
								{
									foundP = true;
									nearPoint = point;
									nearT = sp;
									absDist = d1;
								}
								sp += spadd;
							}
						}
					}
				}
				cli.putPoints(0, Doc->nodeEdit.ClRe2+2, Clip);
				if (foundP)
				{
					npf2 = nearPoint;
					FPoint base = cli.point(cli.size()-2);
					FPoint c1 = cli.point(cli.size()-1);
					FPoint base2 =  Clip.point(Doc->nodeEdit.ClRe2+2);
					FPoint c2 = Clip.point(Doc->nodeEdit.ClRe2+3);
					if ((base == c1) && (base2 == c2))
					{
						cli.resize(cli.size()+4);
						cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
						cli.putPoints(cli.size(), Clip.size()-(Doc->nodeEdit.ClRe2 + 2), Clip, Doc->nodeEdit.ClRe2+2);
					}
					else
					{
						FPoint cn1 = (1.0 - nearT) * base + nearT * c1;
						FPoint cn2 = (1.0 - nearT) * cn1 + nearT * ((1.0 - nearT) * c1 + nearT * c2);
						FPoint cn3 = (1.0 - nearT) * ((1.0 - nearT) * c1 + nearT * c2) + nearT * ((1.0 - nearT) * c2 + nearT * base2);
						FPoint cn4 = (1.0 - nearT) * c2 + nearT * base2;
						cli.setPoint(cli.size()-1, cn1);
						cli.resize(cli.size()+4);
						uint basind = cli.size()+1;
						cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), cn2.x(), cn2.y(), npf2.x(), npf2.y(), cn3.x(), cn3.y());
						cli.putPoints(cli.size(), Clip.size()-(Doc->nodeEdit.ClRe2 + 2), Clip, Doc->nodeEdit.ClRe2+2);
						cli.setPoint(basind, cn4);
					}
					Clip = cli.copy();
					cli.resize(0);
					Doc->nodeEdit.ClRe = Doc->nodeEdit.ClRe2+2;
					Doc->nodeEdit.ClRe2 = -1;
					EndInd = Clip.size();
					StartInd = 0;
					for (uint n = Doc->nodeEdit.ClRe; n < Clip.size(); ++n)
					{
						if (Clip.point(n).x() > 900000)
						{
							EndInd = n;
							break;
						}
					}
					if (Doc->nodeEdit.ClRe > 0)
					{
						for (uint n2 = Doc->nodeEdit.ClRe; n2 > 0; n2--)
						{
							if (n2 == 0)
								break;
							if (Clip.point(n2).x() > 900000)
							{
								StartInd = n2 + 1;
								break;
							}
						}
					}
				}
				else
					Doc->nodeEdit.deselect();
			}
			if (Doc->nodeEdit.hasNodeSelected())
			{
				if (currItem->asPolygon())
				{
					if ((Doc->nodeEdit.ClRe != 0) && (Doc->nodeEdit.ClRe != static_cast<int>(EndInd-2)))
					{
						if (currItem->Segments.count() == 0)
						{
							cli.putPoints(0, EndInd-(Doc->nodeEdit.ClRe+2), Clip, Doc->nodeEdit.ClRe+2);
							cli.putPoints(cli.size(), Doc->nodeEdit.ClRe+2, Clip);
						}
						else
						{
							cli.putPoints(0, EndInd-StartInd, Clip, StartInd);
							int z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor(), true);
							PageItem* bb = Doc->Items->at(z);
							if (Doc->nodeEdit.isContourLine)
								bb->ContourLine.resize(0);
							else
								bb->PoLine.resize(0);
							if (StartInd != 0)
							{
								if (Doc->nodeEdit.isContourLine)
								{
									bb->ContourLine.putPoints(0, StartInd - 4, Clip);
									bb->ContourLine.putPoints(bb->ContourLine.size(), Clip.size()-EndInd, Clip, EndInd);
								}
								else
								{
									bb->PoLine.putPoints(0, StartInd - 4, Clip);
									bb->PoLine.putPoints(bb->PoLine.size(), Clip.size()-EndInd, Clip, EndInd);
								}
							}
							else
							{
								if (Doc->nodeEdit.isContourLine)
									bb->ContourLine.putPoints(0, Clip.size()-EndInd-4, Clip, EndInd+4);
								else
									bb->PoLine.putPoints(0, Clip.size()-EndInd-4, Clip, EndInd+4);
							}
							bb->setRotation(currItem->rotation());
							Doc->AdjustItemSize(bb);
							bb->ClipEdited = true;
							PageItem *bx = Doc->Items->takeAt(bb->ItemNr);
							Doc->Items->insert(bb->ItemNr-1, bx);
						}
						currItem->PoLine = cli.copy();
					}
					Doc->nodeEdit.deselect();
					currItem->ClipEdited = true;
					edited = true;
					Doc->nodeEdit.submode = NodeEditContext::MOVE_POINT;
					PageItem* newItem=Doc->convertItemTo(currItem, PageItem::PolyLine);
					currItem=newItem;
					emit PolyOpen();
				}
				else
				{
					if ((currItem->asPolyLine()) || (currItem->asPathText()))
					{
						if ((Doc->nodeEdit.ClRe > 1) && (Doc->nodeEdit.ClRe < static_cast<int>(Clip.size()-2)))
						{
							int z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor(), true);
							PageItem* bb = Doc->Items->at(z);
							if (Doc->nodeEdit.isContourLine)
								bb->ContourLine.putPoints(0, Clip.size()-(Doc->nodeEdit.ClRe+2), Clip, Doc->nodeEdit.ClRe+2);
							else
								bb->PoLine.putPoints(0, Clip.size()-(Doc->nodeEdit.ClRe+2), Clip, Doc->nodeEdit.ClRe+2);
							bb->setRotation(currItem->rotation());
							Doc->AdjustItemSize(bb);
							bb->ClipEdited = true;
							cli.resize(0);
							cli.putPoints(0, Doc->nodeEdit.ClRe+2, Clip);
							currItem->PoLine = cli.copy();
						}
						Doc->nodeEdit.deselect();
						currItem->ClipEdited = true;
						edited = true;
						Doc->nodeEdit.submode = NodeEditContext::MOVE_POINT;
						currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2.0, 1.0)));
						emit PolyOpen();
					}
				}
			}
		}
		if ((Doc->nodeEdit.submode == NodeEditContext::DEL_POINT) && Doc->nodeEdit.hasNodeSelected())
		{
			if (!Doc->nodeEdit.EdPoints)
				return;
			if ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame()))
			{
				if ((currItem->Segments.count() == 0) && (Clip.size() < 13))
					return;
			}
			else
			{
				if (Clip.size() < 5)
					return;
			}
			if ((currItem->Segments.count() != 0) && ((EndInd - StartInd) < 13))
			{
				if (StartInd != 0)
					cli.putPoints(0, StartInd-4, Clip);
				cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
			}
			else
			{
				if (Doc->nodeEdit.ClRe == static_cast<int>(StartInd))
				{
					if ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame()))
					{
						FPoint kp(Clip.point(EndInd-3));
						cli.putPoints(0, StartInd, Clip);
						cli.putPoints(cli.size(), EndInd - StartInd - 4, Clip, StartInd);
						cli.setPoint(StartInd, cli.point(cli.size()-2));
						cli.setPoint(StartInd + 1, kp);
						cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
					}
					else
					{
						cli.putPoints(0, StartInd, Clip);
						cli.putPoints(cli.size(), EndInd - StartInd - 4, Clip, StartInd+4);
						cli.putPoints(cli.size(), Clip.size() - EndInd, Clip, EndInd);
					}
				}
				else
				{
					if (Doc->nodeEdit.ClRe != 0)
						cli.putPoints(0, Doc->nodeEdit.ClRe, Clip);
					cli.putPoints(cli.size(), Clip.size()-(Doc->nodeEdit.ClRe + 4), Clip, Doc->nodeEdit.ClRe+4);
				}
			}
			if (Doc->nodeEdit.isContourLine)
				currItem->ContourLine = cli.copy();
			else
				currItem->PoLine = cli.copy();
			Doc->nodeEdit.deselect();
			currItem->ClipEdited = true;
			edited = true;
		}
		if ((Doc->nodeEdit.submode == NodeEditContext::ADD_POINT) && (Doc->nodeEdit.ClRe2 != -1))
		{
			bool foundP = false;
			uint seg = 0;
			double absDist = 9999999999.9;
			FPoint point = FPoint(0, 0);
			FPoint normal = FPoint(0, 0);
			FPoint tangent = FPoint(0, 0);
			FPoint nearPoint = FPoint(0, 0);
			double nearT = 0.0;
			QRect mpo2(0, 0, Doc->guidesSettings.grabRad*3, Doc->guidesSettings.grabRad*3);
			mpo2.moveCenter(QPoint(qRound(npf2.x()), qRound(npf2.y())));
			for (uint poi=0; poi<Clip.size()-3; poi += 4)
			{
				FPoint a1 = Clip.point(poi);
				FPoint a2 = Clip.point(poi+1);
				FPoint a3 = Clip.point(poi+3);
				FPoint a4 = Clip.point(poi+2);
				QPainterPath Bez;
				Bez.moveTo(a1.x(), a1.y());
				Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
				QPolygon cli2 = Bez.toFillPolygon().toPolygon();
				for (int clp = 0; clp < cli2.size()-1; ++clp)
				{
					if (PointOnLine(cli2.point(clp), cli2.point(clp+1), mpo2))
					{
						seg = poi;
						double sp = 0.0;
						double spadd = 1.0 / (Clip.lenPathSeg(seg) * m_canvas->scale());
						while (sp < 1.0)
						{
							Clip.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
							double d1 = fabs(sqrt(pow(point.x() - npf2.x(), 2) + pow(point.y() - npf2.y() ,2)));
							if (d1 < absDist)
							{
								foundP = true;
								nearPoint = point;
								nearT = sp;
								absDist = d1;
							}
							sp += spadd;
						}
					}
				}
			}
			cli.putPoints(0, Doc->nodeEdit.ClRe2+2, Clip);
			if (foundP)
			{
				npf2 = nearPoint;
				FPoint base = cli.point(cli.size()-2);
				FPoint c1 = cli.point(cli.size()-1);
				FPoint base2 =  Clip.point(Doc->nodeEdit.ClRe2+2);
				FPoint c2 = Clip.point(Doc->nodeEdit.ClRe2+3);
				if ((base == c1) && (base2 == c2))
				{
					cli.resize(cli.size()+4);
					cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
							cli.putPoints(cli.size(), Clip.size()-(Doc->nodeEdit.ClRe2 + 2), Clip, Doc->nodeEdit.ClRe2+2);
				}
				else
				{
					FPoint cn1 = (1.0 - nearT) * base + nearT * c1;
					FPoint cn2 = (1.0 - nearT) * cn1 + nearT * ((1.0 - nearT) * c1 + nearT * c2);
					FPoint cn3 = (1.0 - nearT) * ((1.0 - nearT) * c1 + nearT * c2) + nearT * ((1.0 - nearT) * c2 + nearT * base2);
					FPoint cn4 = (1.0 - nearT) * c2 + nearT * base2;
					cli.setPoint(cli.size()-1, cn1);
					cli.resize(cli.size()+4);
					uint basind = cli.size()+1;
					cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), cn2.x(), cn2.y(), npf2.x(), npf2.y(), cn3.x(), cn3.y());
					cli.putPoints(cli.size(), Clip.size()-(Doc->nodeEdit.ClRe2 + 2), Clip, Doc->nodeEdit.ClRe2+2);
					cli.setPoint(basind, cn4);
				}
			}
			else
			{
				cli.resize(cli.size()+4);
				cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
				cli.putPoints(cli.size(), Clip.size()-(Doc->nodeEdit.ClRe2 + 2), Clip, Doc->nodeEdit.ClRe2+2);
			}
			if (Doc->nodeEdit.isContourLine)
				currItem->ContourLine = cli.copy();
			else
				currItem->PoLine = cli.copy();
			Doc->nodeEdit.ClRe2 = -1;
			currItem->ClipEdited = true;
			edited = true;
		}
		if (edited)
		{
			currItem->FrameType = 3;
			Doc->AdjustItemSize(currItem);
			updateContents();
			emit DocChanged();
		}
		if ((Doc->nodeEdit.SelNode.count() != 0) || ((Doc->nodeEdit.SegP1 != -1) && (Doc->nodeEdit.SegP2 != -1)) || (Doc->nodeEdit.hasNodeSelected() && (!Doc->nodeEdit.EdPoints)))
		{
			Mxp = m->x();
			Myp = m->y();
			m_canvas->setRenderModeFillBuffer();
		}
		else
		{
			redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
			redrawMarker->show();
			Mxp = m->globalPos().x();
			Myp = m->globalPos().y();
			Dxp = qRound(m->x()/m_canvas->scale());// + Doc->minCanvasCoordinate.x());
			Dyp = qRound(m->y()/m_canvas->scale());// + Doc->minCanvasCoordinate.y());
		}
	
}
#endif




void ScribusView::stopAllDrags()
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	inItemCreation = false;
	MidButt = false;
}
