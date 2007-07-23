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
#include <Q3PointArray>
#include <QStringList>
#include <QImage>
#include <Q3CString>
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
#include <Q3StrList>
#include <QLabel>
#include <QDropEvent>
#include <QMenu>
#include <QMenu>
#include <QDragEnterEvent>
#include <QList>
#include <QMouseEvent>
#include <Q3GridLayout>
#include <QImageReader>
#include <QWidgetAction>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <Q3Url>
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
	Q3ScrollView(win, "s", Qt::WNoAutoErase | Qt::WStaticContents),
	Doc(doc),
	Prefs(&(PrefsManager::instance()->appPrefs)),
	undoManager(UndoManager::instance()),
	OldScale(0),
	dragX(0), dragY(0), dragW(0), dragH(0),
	oldW(-1), oldCp(-1),
	Mxp(-1), Myp(-1), Dxp(-1), Dyp(-1),
	frameResizeHandle(-1),
	SeRx(-1), SeRy(-1), GyM(-1), GxM(-1),
	ClRe(-1), ClRe2(-1),
	SegP1(-1), SegP2(-1),
	RotMode(0),
	DrHY(-1), DrVX(-1),
	EdPoints(true),
	m_MouseButtonPressed(false),	
	operItemMoving(false),
	MoveGY(false), MoveGX(false),
	HaveSelRect(false),
	operItemResizing(false),
	EditContour(false),
	//GroupSel(false),
	DraggedGroup(false),
	DraggedGroupFirst(false),
	operItemResizeInEditMode(false),
	MidButt(false),
	updateOn(true),
	FirstPoly(true),
	Magnify(false),
	MoveSym(false),
	previewMode(false),
	RCenter(-1,-1),
	RecordP(),
	Ready(false),
	oldX(0), oldY(0),
	_groupTransactionStarted(false),
	_isGlobalMode(true),
	evSpon(false),
	forceRedraw(false),
	Scale(Prefs->DisScale),
	oldClip(0),
	m_vhRulerHW(17),
	m_cursorVisible(false),
	m_ScMW(mw)
{
	setHScrollBarMode(Q3ScrollView::AlwaysOn);
	setVScrollBarMode(Q3ScrollView::AlwaysOn);
	setMargins(m_vhRulerHW, m_vhRulerHW, 0, 0);
	setResizePolicy(Manual);
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
	zoomSpinBox->setLineStepM(10);
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
	rulerMover = new RulerMover(this);
	rulerMover->setFocusPolicy(Qt::NoFocus);
	Ready = true;
	viewport()->setMouseTracking(true);
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setDragAutoScroll(false);
	Doc->DragP = false;
	Doc->leaveDrag = false;
	Doc->SubMode = -1;
	storedFramesShown = Doc->guidesSettings.framesShown;
	storedShowControls = Doc->guidesSettings.showControls;
	viewAsPreview = false;
	previewVisual = 0;
	shiftSelItems = false;
	inItemCreation = false;
	redrawMode = 0;
	redrawCount = 0;
	redrawMarker = new QRubberBand(QRubberBand::Rectangle);
	redrawMarker->hide();
	redrawPolygon.clear();
	specialRendering = false;
	firstSpecial = false;
	m_buffer = QPixmap();
	m_ScMW->scrActions["viewFitPreview"]->setOn(viewAsPreview);
	m_SnapCounter = 0;
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
	QToolTip::remove(cmsToolbarButton);
	QToolTip::remove(previewToolbarButton);
	QToolTip::remove(visualMenu);
	QToolTip::add(cmsToolbarButton, tr("Switches Color Management on or off"));
	QToolTip::add(previewToolbarButton, tr("Enables the Preview Mode"));
	QToolTip::add(visualMenu, tr("Here you can select the visual appearance of the display\nYou can choose between normal and several color blindness forms"));
	disconnect(visualMenu, SIGNAL(activated(int)), this, SLOT(switchPreviewVisual(int)));
	visualMenu->clear();
	visualMenu->insertItem(CommonStrings::trVisionNormal);
	visualMenu->insertItem(CommonStrings::trVisionProtanopia);
	visualMenu->insertItem(CommonStrings::trVisionDeuteranopia);
	visualMenu->insertItem(CommonStrings::trVisionTritanopia);
	visualMenu->insertItem(CommonStrings::trVisionFullColorBlind);
	visualMenu->setCurrentItem(previewVisual);
	connect(visualMenu, SIGNAL(activated(int)), this, SLOT(switchPreviewVisual(int)));
}

void ScribusView::toggleCMS()
{
	Doc->enableCMS(!Doc->HasCMS);
	updateContents();
}

void ScribusView::switchPreviewVisual(int vis)
{
	previewVisual = vis;
	Doc->recalculateColors();
	Doc->recalcPicturesRes();
	updateContents();
}

void ScribusView::togglePreview()
{
	viewAsPreview = !viewAsPreview;
	if (viewAsPreview)
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
	m_ScMW->scrActions["viewFitPreview"]->setOn(viewAsPreview);
	m_ScMW->scrActions["viewShowMargins"]->setEnabled(!viewAsPreview);
	m_ScMW->scrActions["viewShowFrames"]->setEnabled(!viewAsPreview);
	m_ScMW->scrActions["viewShowLayerMarkers"]->setEnabled(!viewAsPreview);
	m_ScMW->scrActions["viewShowGrid"]->setEnabled(!viewAsPreview);
	m_ScMW->scrActions["viewShowGuides"]->setEnabled(!viewAsPreview);
	m_ScMW->scrActions["viewShowColumnBorders"]->setEnabled(!viewAsPreview);
	m_ScMW->scrActions["viewShowBaseline"]->setEnabled(!viewAsPreview);
	m_ScMW->scrActions["viewShowTextChain"]->setEnabled(!viewAsPreview);
	m_ScMW->scrActions["viewShowTextControls"]->setEnabled(!viewAsPreview);
#if OPTION_USE_QTOOLBUTTON
	previewToolbarButton->setOn(viewAsPreview);
#endif
	visualMenu->setEnabled(viewAsPreview);
	Doc->recalculateColors();
	Doc->recalcPicturesRes();
	updateContents();
}

void ScribusView::viewportPaintEvent ( QPaintEvent * p )
{
	#ifndef _WIN32
	if (p->spontaneous())
		evSpon = true;
	#endif
	Q3ScrollView::viewportPaintEvent(p);
}

void ScribusView::drawContents(QPainter *psx, int clipx, int clipy, int clipw, int cliph)
{
	if (Doc->isLoading())
		return;
	if (!updateOn)
		return;
//	QTime tim;
//	tim.start();
	psx->setRenderHint(QPainter::Antialiasing, false);
	psx->setRenderHint(QPainter::SmoothPixmapTransform, false);
	if ((clipw > 0) && (cliph > 0))
	{

		if ((specialRendering) || (Doc->EditClip && specialRendering))
		{
			if (firstSpecial)
			{
#ifdef Q_WS_MAC
				QPoint viewportOrigin = mapToGlobal(QPoint(0,0));
				m_buffer = QPixmap::grabWindow(viewport()->winId(), viewportOrigin.x(), viewportOrigin.y(), viewport()->width(), viewport()->height());
				//m_buffer.resize(viewport()->width(), viewport()->height());
				//viewport()->render(m_buffer);
//				qDebug(QString("drawContents:viewport (%1,%2) %3x%4 -> %5x%6").arg(viewportOrigin.x()).arg(viewportOrigin.y())
//					   .arg(viewport()->width()).arg(viewport()->height()).arg(m_buffer.width()).arg(m_buffer.height()));
#else
				m_buffer = QPixmap::grabWindow(viewport()->winId(), 0, 0);
#endif
				firstSpecial = false;
			}
			QPainter pp;
			QPixmap ppx = m_buffer;
			pp.begin(&ppx);
			pp.setRenderHint(QPainter::Antialiasing, true);
			if (redrawMode == 1)
			{
				pp.resetMatrix();
				pp.setBrush(Qt::NoBrush);
				pp.setPen(QPen(Qt::black, 1.0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
				QPoint nXY = redrawPolygon.point(0);
				pp.drawLine(0, nXY.y(), viewport()->width(), nXY.y());
				pp.drawLine(nXY.x(), 0, nXY.x(), viewport()->height());
				redrawPolygon.clear();
			}
			if ((Doc->appMode == modeDrawBezierLine) && (!redrawPolygon.isEmpty()) && (Doc->m_Selection->count() != 0))
			{
				pp.setBrush(Qt::NoBrush);
				pp.setPen(QPen(Qt::black, 1.0 / Scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
				pp.resetMatrix();
				QPoint out = contentsToViewport(QPoint(0, 0));
				pp.translate(out.x(), out.y());
				pp.translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
				PageItem *currItem = Doc->m_Selection->itemAt(0);
				Transform(currItem, &pp);
				Q3PointArray Bez(4);
				if (currItem->PoLine.size() > 1)
				{
					QPoint nXY = redrawPolygon.point(0);
					if (!m_MouseButtonPressed)
					{
						QPoint a1 = currItem->PoLine.pointQ(currItem->PoLine.size()-2);
						QPoint a2 = currItem->PoLine.pointQ(currItem->PoLine.size()-1);
						BezierPoints(&Bez, a1, a2, nXY, nXY);
						pp.drawCubicBezier(Bez);
					}
					else
					{
						QPoint a2 = currItem->PoLine.pointQ(currItem->PoLine.size()-1);
						if (currItem->PoLine.size() > 2)
						{
							QPoint a1 = currItem->PoLine.pointQ(currItem->PoLine.size()-2);
							QPoint a3 = currItem->PoLine.pointQ(currItem->PoLine.size()-3);
							BezierPoints(&Bez, a3, a1, nXY, a2);
							pp.drawCubicBezier(Bez);
						}
						pp.drawLine(a2, nXY);
					}
				}
				else
				{
					QPoint a2 = currItem->PoLine.pointQ(currItem->PoLine.size()-1);
					QPoint nXY = redrawPolygon.point(0);
					pp.drawLine(a2, nXY);
				}
				redrawPolygon.clear();
			}
			if (m_MouseButtonPressed && (Doc->appMode == modeMeasurementTool))
			{
				pp.resetMatrix();
				QPoint out = contentsToViewport(QPoint(0, 0));
				pp.translate(out.x(), out.y());
				pp.setBrush(Qt::NoBrush);
				pp.setPen(QPen(Qt::black, 1.0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
				pp.drawPolyline(redrawPolygon);
				redrawPolygon.clear();
			}
			if (m_MouseButtonPressed && (Doc->appMode == modeDrawLine))
			{
				QPoint out = contentsToViewport(QPoint(0, 0));
				pp.resetMatrix();
				pp.translate(out.x(), out.y());
				pp.translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
				pp.scale(Scale, Scale);
				pp.setBrush(Qt::NoBrush);
				pp.setPen(QPen(Qt::black, 1.0 / Scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
				pp.drawPolyline(redrawPolygon);
				redrawPolygon.clear();
			}
			if (operItemMoving || operItemResizing)
			{
				if (operItemResizing)
				{
					if (!redrawPolygon.isEmpty())
					{
						if (m_MouseButtonPressed && (Doc->appMode == modeDrawFreehandLine))
						{
							pp.resetMatrix();
							QPoint out = contentsToViewport(QPoint(0, 0));
							pp.translate(out.x(), out.y());
							pp.translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
							pp.scale(Scale, Scale);
							pp.setBrush(Qt::NoBrush);
							pp.setPen(QPen(Qt::black, 1.0 / Scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
							pp.drawPolyline(redrawPolygon);
							redrawPolygon.clear();
						}
						else
						{
							QColor drawColor = qApp->palette().color(QPalette::Active, QColorGroup::Highlight);
							pp.setPen(QPen(drawColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
							drawColor.setAlpha(64);
							pp.resetMatrix();
							QPoint out = contentsToViewport(QPoint(0, 0));
							pp.translate(out.x(), out.y());
//							pp.translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
							pp.setBrush(drawColor);
							pp.drawPolygon(redrawPolygon);
							redrawPolygon.clear();
						}
					}
				}
				else
				{
					if (Doc->m_Selection->count() != 0)
					{
						uint selectedItemCount = Doc->m_Selection->count();
						PageItem *currItem = Doc->m_Selection->itemAt(0);
						if (selectedItemCount < moveWithBoxesOnlyThreshold)
						{
							for (uint cu = 0; cu < selectedItemCount; cu++)
							{
								currItem = Doc->m_Selection->itemAt(cu);
								pp.resetMatrix();
								QPoint out = contentsToViewport(QPoint(0, 0));
								pp.translate(out.x(), out.y());
								pp.translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
								Transform(currItem, &pp);
								pp.setBrush(Qt::NoBrush);
								pp.setPen(QPen(Qt::black, 1.0 / Scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
								if (selectedItemCount < moveWithFullOutlinesThreshold)
								{
									if (!(currItem->asLine()))
										currItem->DrawPolyL(&pp, currItem->Clip);
									else
									{
										if (currItem->asLine())
										{
											int lw2 = 1;
											int lw = 1;
											Qt::PenCapStyle le = Qt::FlatCap;
											if (currItem->NamedLStyle.isEmpty())
											{
												lw2 = qRound(currItem->lineWidth()  / 2.0);
												lw = qRound(qMax(currItem->lineWidth(), 1.0));
												le = currItem->PLineEnd;
											}
											else
											{
												multiLine ml = Doc->MLineStyles[currItem->NamedLStyle];
												lw2 = qRound(ml[ml.size()-1].Width  / 2.0);
												lw = qRound(qMax(ml[ml.size()-1].Width, 1.0));
												le = static_cast<Qt::PenCapStyle>(ml[ml.size()-1].LineEnd);
											}
											if (le != Qt::FlatCap)
												pp.drawRect(-lw2, -lw2, qRound(currItem->width())+lw, lw);
											else
												pp.drawRect(-1, -lw2, qRound(currItem->width()), lw);
										}
									}
								}
								else
									pp.drawRect(0, 0, static_cast<int>(currItem->width())+1, static_cast<int>(currItem->height())+1);
							}
						}
						else
						{
							double gx, gy, gw, gh;
							Doc->m_Selection->setGroupRect();
							Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
							QPoint out = contentsToViewport(QPoint(0, 0));
							pp.resetMatrix();
							pp.translate(out.x(), out.y());
							pp.translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
							pp.scale(Scale, Scale);
							pp.translate(qRound(gx), qRound(gy));
							pp.drawRect(QRect(0, 0, qRound(gw), qRound(gh)));
						}
					}
				}
			}
			else
			{
				if ((Doc->m_Selection->count() != 0) && (Doc->appMode != modeDrawBezierLine))
				{
					PageItem *currItem = Doc->m_Selection->itemAt(0);
					if ((Doc->EditClip) && (currItem->isSelected()))
					{
						if (EditContour)
							MarkClip(&pp, currItem, currItem->ContourLine, true);
						else
							MarkClip(&pp, currItem, currItem->PoLine, true);
					}
					else
					{
						if (Doc->m_Selection->isMultipleSelection())
						{
							pp.resetMatrix();
							QPoint out = contentsToViewport(QPoint(0, 0));
							pp.translate(out.x(), out.y());
							pp.translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
							double x, y, w, h;
							Doc->m_Selection->setGroupRect();
							Doc->m_Selection->getGroupRect(&x, &y, &w, &h);
							x *= Scale;
							y *= Scale;
							w *= Scale;
							h *= Scale;
							pp.setPen(QPen(Qt::red, 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
							pp.setBrush(Qt::NoBrush);
							pp.drawRect(QRectF(x, y, w, h));
							pp.setBrush(Qt::red);
							pp.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
							pp.drawRect(QRectF(x+w-6, y+h-6, 6, 6));
							pp.drawRect(QRectF(x+w/2 - 3, y+h-6, 6, 6));
							pp.drawRect(QRectF(x+w/2 - 3, y, 6, 6));
							pp.drawRect(QRectF(x+w-6, y+h/2 - 3, 6, 6));
							pp.drawRect(QRectF(x+w-6, y, 6, 6));
							pp.drawRect(QRectF(x, y, 6, 6));
							pp.drawRect(QRectF(x, y+h/2 - 3, 6, 6));
							pp.drawRect(QRectF(x, y+h-6, 6, 6));
						}
						else
						{
							pp.resetMatrix();
							QPoint out = contentsToViewport(QPoint(0, 0));
							pp.translate(out.x(), out.y());
							pp.translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
							Transform(currItem, &pp);
							currItem->paintObj(&pp);
						}
					}
				}
			}
			pp.end();
			psx->resetMatrix();
			psx->drawPixmap(0,0,ppx);
			return;
		}

		QPoint vr = contentsToViewport(QPoint(clipx, clipy));
		ScPainter *painter=0;
		QImage img = QImage(clipw, cliph, QImage::Format_ARGB32);
		painter = new ScPainter(&img, img.width(), img.height(), 1.0, 0);
		painter->clear(paletteBackgroundColor());
		painter->newPath();
		painter->moveTo(0, 0);
		painter->lineTo(clipw, 0);
		painter->lineTo(clipw, cliph);
		painter->lineTo(0, cliph);
		painter->closePath();
		painter->setClipPath();
		painter->translate(-clipx, -clipy);
		painter->setZoomFactor(Scale);
		painter->translate(-Doc->minCanvasCoordinate.x(), -Doc->minCanvasCoordinate.y());
		painter->setLineWidth(1);
		painter->setFillMode(ScPainter::Solid);
/* Draw Page Outlines */
		if (!Doc->masterPageMode())
		{
			uint docPagesCount=Doc->Pages->count();
			if (!viewAsPreview)
			{
				painter->setBrush(QColor(128,128,128));
				painter->setAntialiasing(false);
				painter->beginLayer(1.0, 0);
				painter->setPen(Qt::black, 1.0 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				Page *actPg;
				for (int a = 0; a < static_cast<int>(docPagesCount); ++a)
				{
					actPg = Doc->Pages->at(a);
					double bleedRight = 0.0;
					double bleedLeft = 0.0;
					double bleedBottom = 0.0;
					double bleedTop = 0.0;
					Doc->getBleeds(actPg, &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
					double blx = (actPg->xOffset() - bleedLeft) * Scale;
					double bly = (actPg->yOffset() - bleedTop) * Scale;
					double blw = (actPg->width() + bleedLeft + bleedRight) * Scale;
					double blh = (actPg->height() + bleedBottom + bleedTop) * Scale;
	
					QRectF drawRect = QRectF(blx-1, bly-1, blw+6, blh+6);
					drawRect.translate(-Doc->minCanvasCoordinate.x() * Scale, -Doc->minCanvasCoordinate.y() * Scale);
					if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
					{
						painter->setFillMode(ScPainter::Solid);
						double blx2 = actPg->xOffset();
						double bly2 = actPg->yOffset();
						double blw2 = actPg->width();
						double blh2 = actPg->height();
						if (Doc->guidesSettings.showBleed)
						{
							blx2 -= bleedLeft;
							bly2 -= bleedTop;
							blw2 += bleedLeft + bleedRight;
							blh2 += bleedBottom + bleedTop;
						}
						painter->drawRect(blx2 + 5 / Scale, bly2 + 5 / Scale, blw2, blh2);
						if (((Doc->bleeds.Bottom != 0.0) || (Doc->bleeds.Top != 0.0) || (Doc->bleeds.Left != 0.0) || (Doc->bleeds.Right != 0.0)) && (Doc->guidesSettings.showBleed))
						{
							painter->setFillMode(ScPainter::None);
							painter->setPen(Qt::black, 1.0 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
							painter->drawRect(blx2, bly2 - 1 / Scale, blw2 + 1 / Scale, blh2 + 2 / Scale);
						}
					}
				}
				painter->endLayer();
				painter->setAntialiasing(true);
			}
			painter->setFillMode(ScPainter::Solid);
			Page *actPg;
			for (int a = 0; a < static_cast<int>(docPagesCount); ++a)
			{
				actPg = Doc->Pages->at(a);
				double x = actPg->xOffset();
				double y = actPg->yOffset();
				double w = actPg->width();
				double h = actPg->height();
				double bleedRight = 0.0;
				double bleedLeft = 0.0;
				double bleedBottom = 0.0;
				double bleedTop = 0.0;
				bool drawBleed = false;
				if (((Doc->bleeds.Bottom != 0.0) || (Doc->bleeds.Top != 0.0) || (Doc->bleeds.Left != 0.0) || (Doc->bleeds.Right != 0.0)) && (Doc->guidesSettings.showBleed))
				{
					drawBleed = true;
					Doc->getBleeds(a, &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
				}
				double blx = (actPg->xOffset() - bleedLeft) * Scale;
				double bly = (actPg->yOffset() - bleedTop) * Scale;
				double blw = (actPg->width() + bleedLeft + bleedRight) * Scale;
				double blh = (actPg->height() + bleedBottom + bleedTop) * Scale;

				QRectF drawRect = QRectF(blx, bly, blw+5, blh+5);
				drawRect.translate(-Doc->minCanvasCoordinate.x() * Scale, -Doc->minCanvasCoordinate.y() * Scale);
				if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
				{
					painter->setFillMode(ScPainter::Solid);
					painter->setPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					painter->setAntialiasing(false);
					painter->beginLayer(1.0, 0);
					painter->setLineWidth(0.0);
					painter->setBrush(Doc->papColor);
					if (!viewAsPreview)
					{
						double blx2 = actPg->xOffset() - bleedLeft;
						double bly2 = actPg->yOffset() - bleedTop;
						double blw2 = actPg->width() + bleedLeft + bleedRight;
						double blh2 = actPg->height() + bleedBottom + bleedTop;
						painter->drawRect(blx2, bly2, blw2, blh2);
						if (drawBleed)
							painter->drawRect(x, y, w, h);
					}
					else
						painter->drawRect(x, y, w, h);
					painter->endLayer();
					painter->setAntialiasing(true);
				}
			}
			if (viewAsPreview)
			{
				FPointArray PoLine;
				bool first = true;
				Page *actPg;
				for (int a = 0; a < static_cast<int>(docPagesCount); ++a)
				{
					if (!first)
						PoLine.setMarker();
					first = false;
					actPg = Doc->Pages->at(a);
					double x = actPg->xOffset();
					double y = actPg->yOffset();
					double w = actPg->width();
					double h = actPg->height();
					static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
													1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
													0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
					for (int a = 0; a < 29; a += 4)
					{
						PoLine.addPoint(x + w * rect[a], y + h * rect[a+1]);
						PoLine.addPoint(x + w * rect[a+2], y + h * rect[a+3]);
					}
				}
				painter->beginLayer(1.0, 0, &PoLine);
			}
			else
				painter->beginLayer(1.0, 0);
			if ((Doc->guidesSettings.before) && (!viewAsPreview))
			{
				Page *actPg;
				for (uint a = 0; a < docPagesCount; ++a)
				{
					actPg = Doc->Pages->at(a);
					double x = actPg->xOffset() * Scale;
					double y = actPg->yOffset() * Scale;
					double w = actPg->width() * Scale;
					double h = actPg->height() * Scale;
					QRectF drawRect = QRectF(x, y, w+5, h+5);
					drawRect.translate(-Doc->minCanvasCoordinate.x() * Scale, -Doc->minCanvasCoordinate.y() * Scale);
					if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
						DrawPageMarks(painter, Doc->Pages->at(a), QRect(clipx, clipy, clipw, cliph));
				}
			}
			for (uint a = 0; a < docPagesCount; ++a)
			{
				DrawMasterItems(painter, Doc->Pages->at(a), QRect(clipx, clipy, clipw, cliph));
			}
			DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
			painter->endLayer();
			if ((!Doc->guidesSettings.before) && (!viewAsPreview))
			{
				Page *actPg;
				for (uint a = 0; a < docPagesCount; ++a)
				{
					actPg = Doc->Pages->at(a);
					double x = actPg->xOffset() * Scale;
					double y = actPg->yOffset() * Scale;
					double w = actPg->width() * Scale;
					double h = actPg->height() * Scale;
					QRectF drawRect = QRectF(x, y, w+5, h+5);
					drawRect.translate(-Doc->minCanvasCoordinate.x() * Scale, -Doc->minCanvasCoordinate.y() * Scale);
					if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
						DrawPageMarks(painter, Doc->Pages->at(a), QRect(clipx, clipy, clipw, cliph));
				}
			}
		}
		else
		{
			double x = Doc->scratch.Left * Scale;
			double y = Doc->scratch.Top * Scale;
			double w = Doc->currentPage()->width() * Scale;
			double h = Doc->currentPage()->height() * Scale;
			QRectF drawRect = QRectF(x, y, w+5, h+5);
			drawRect.translate(-Doc->minCanvasCoordinate.x() * Scale, -Doc->minCanvasCoordinate.y() * Scale);
			if (drawRect.intersects(QRectF(clipx, clipy, clipw, cliph)))
			{
				painter->setFillMode(ScPainter::Solid);
				painter->setBrush(QColor(128,128,128));
				double bleedRight = 0.0;
				double bleedLeft = 0.0;
				double bleedBottom = 0.0;
				double bleedTop = 0.0;
				Doc->getBleeds(Doc->currentPage(), &bleedTop, &bleedBottom, &bleedLeft, &bleedRight);
				painter->beginLayer(1.0, 0);
				painter->setAntialiasing(false);
				painter->setPen(Qt::black, 1 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				if (((Doc->bleeds.Bottom != 0.0) || (Doc->bleeds.Top != 0.0) || (Doc->bleeds.Left != 0.0) || (Doc->bleeds.Right != 0.0)) && (Doc->guidesSettings.showBleed))
				{
					painter->drawRect(Doc->scratch.Left - bleedLeft+5 / Scale, Doc->scratch.Top - bleedTop+5 / Scale, Doc->currentPage()->width() + bleedLeft + bleedRight, Doc->currentPage()->height() + bleedBottom + bleedTop);
					painter->setBrush(Doc->papColor);
					painter->drawRect(Doc->scratch.Left - bleedLeft, Doc->scratch.Top - bleedTop, Doc->currentPage()->width() + bleedLeft + bleedRight, Doc->currentPage()->height() + bleedBottom + bleedTop);
				}
				else
				{
					painter->drawRect(Doc->scratch.Left+5 / Scale, Doc->scratch.Top+5 / Scale, Doc->currentPage()->width(), Doc->currentPage()->height());
					painter->setBrush(Doc->papColor);
					painter->drawRect(Doc->scratch.Left, Doc->scratch.Top, Doc->currentPage()->width(), Doc->currentPage()->height());
				}
				painter->setAntialiasing(true);
				if (Doc->guidesSettings.before)
					DrawPageMarks(painter, Doc->currentPage(), QRect(clipx, clipy, clipw, cliph));
				painter->endLayer();
			}
			painter->beginLayer(1.0, 0);
			DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
			painter->endLayer();
			if ((!Doc->guidesSettings.before) && (drawRect.intersects(QRect(clipx, clipy, clipw, cliph))))
				DrawPageMarks(painter, Doc->currentPage(), QRect(clipx, clipy, clipw, cliph));
		}
		if (((Doc->m_Selection->count() != 0) || (linkedFramesToShow.count() != 0))  && (!viewAsPreview))
		{
			painter->save();
			PageItem *currItem;
			if ((Doc->guidesSettings.linkShown) && (linkedFramesToShow.count() != 0))
				currItem = linkedFramesToShow.at(0);
			else
			{
				if (linkedFramesToShow.count() != 0)
					currItem = linkedFramesToShow.at(0);
				else
					currItem = Doc->m_Selection->itemAt(0);
			}
			//Draw the frame links
			if ((((Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
				   && (currItem->itemType() == PageItem::TextFrame)) || (Doc->guidesSettings.linkShown))
			{
				PageItem *nextItem = currItem;
				if (Doc->guidesSettings.linkShown)
				{
					for (int lks = 0; lks < linkedFramesToShow.count(); ++lks)
					{
						nextItem = linkedFramesToShow.at(lks);
						while (nextItem != 0)
						{
							//Calculate the link points of the frames
							double x11 = nextItem->xPos();
							double y11 = nextItem->yPos();
							double x12 = x11+nextItem->width();
							double y12 = y11+nextItem->height();
							double x1mid = x11+(x12-x11)/2;
							double y1mid = y11+(y12-y11)/2;

							if (nextItem->rotation()!=0.000)
							{
								FPoint tempPoint(0,0, x11, y11, nextItem->rotation(), 1, 1);
								x11=tempPoint.x();
								y11=tempPoint.y();
								FPoint tempPoint2(0,0, x12, y12, nextItem->rotation(), 1, 1);
								x12=tempPoint2.x();
								y12=tempPoint2.y();
								FPoint tempPoint3(0,0, x1mid, y1mid, nextItem->rotation(), 1, 1);
								x1mid=tempPoint3.x();
								y1mid=tempPoint3.y();
							}


							double a1, b1, a2, b2;
							a1 = a2 = b1 = b2 = 0;
							if (nextItem->nextInChain() != NULL)
							{
								double x21 = nextItem->nextInChain()->xPos();
								double y21 = nextItem->nextInChain()->yPos();
								double x22 = x21+nextItem->nextInChain()->width();
								double y22 = y21+nextItem->nextInChain()->height();
								double x2mid = x21 + nextItem->nextInChain()->width()/2;
								double y2mid = y21 + nextItem->nextInChain()->height()/2;
								//x2mid = x21+(x22-x21)/2;
								//y2mid = y21+(y22-y21)/2;

								if (nextItem->nextInChain()->rotation()!=0.000)
								{
									FPoint tempPoint(0,0, x21, y21, nextItem->nextInChain()->rotation(), 1, 1);
									x21=tempPoint.x();
									y21=tempPoint.y();
									FPoint tempPoint2(0,0, x22, y22, nextItem->nextInChain()->rotation(), 1, 1);
									x22=tempPoint2.x();
									y22=tempPoint2.y();
									FPoint tempPoint3(0,0, x2mid, y2mid, nextItem->nextInChain()->rotation(), 1, 1);
									x2mid=tempPoint3.x();
									y2mid=tempPoint3.y();
								}

								if (x22<x11) { a1 = x11; a2 = x22; }
								if (x21>x12) { a1 = x12; a2 = x21; }
								if (y22<y11) { b1 = y11; b2 = y22; }
								if (y21>y12) { b1 = y12; b2 = y21; }

								if (x21<x12 && x21>x11) { a1 = x1mid; a2 = x2mid; }
								if (x21<x11 && x22>x11) { a1 = x1mid; a2 = x2mid; }

								if (y21<y12 && y21>y11) { b1 = y1mid; b2 = y2mid; }
								if (y21<y11 && y22>y11) { b1 = y1mid; b2 = y2mid; }

							}
							//Draw the link frame lines
							FPoint Start(a1-nextItem->xPos(), b1-nextItem->yPos(), nextItem->xPos(), nextItem->yPos(), nextItem->rotation(), 1, 1);
							//FPoint Start = transformPoint(FPoint(nextItem->Width/2, nextItem->Height), nextItem->xPos(), nextItem->yPos(), nextItem->Rot, 1, 1);
							nextItem = nextItem->nextInChain();
							if (nextItem != NULL)
							{
								FPoint End(a2-nextItem->xPos(), b2-nextItem->yPos(), nextItem->xPos(), nextItem->yPos(), nextItem->rotation(), 1, 1);
								drawLinkFrameLine(painter, Start, End);
							}
						}
					}
				}
				else
				{
					while (nextItem != 0)
					{
						if (nextItem->prevInChain() != 0)
							nextItem = nextItem->prevInChain();
						else
							break;
					}
					while (nextItem != 0)
					{
						FPoint Start(nextItem->width()/2, nextItem->height(), nextItem->xPos(), nextItem->yPos(), nextItem->rotation(), 1, 1);
						nextItem = nextItem->nextInChain();
						//Draw the link frame indicator for a new link
						//CB unsure if we need to do this
						if (nextItem != 0)
						{
							FPoint End(nextItem->width()/2, 0, nextItem->xPos(), nextItem->yPos(), nextItem->rotation(), 1, 1);
							drawLinkFrameLine(painter, Start, End);
						}
					}
				}
			}
			painter->setLineWidth(1);
			painter->setPenOpacity(1.0);
			painter->restore();
		}
		painter->end();
		psx->drawImage(clipx, clipy, img);
		delete painter;
		painter=NULL;
	}
/*	psx->setRenderHint(QPainter::Antialiasing, true);
	if (operItemMoving || operItemResizing)
	{
		if (operItemResizing)
		{
			if (!redrawPolygon.isEmpty())
			{
				if (m_MouseButtonPressed && (Doc->appMode == modeDrawFreehandLine))
				{
					psx->resetMatrix();
					QPoint out = contentsToViewport(QPoint(0, 0));
					psx->translate(out.x(), out.y());
					psx->translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
					psx->scale(Scale, Scale);
					psx->setBrush(Qt::NoBrush);
					psx->setPen(QPen(Qt::black, 1.0 / Scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
					psx->drawPolyline(redrawPolygon);
					redrawPolygon.clear();
				}
				else
				{
					QColor drawColor = qApp->palette().color(QPalette::Active, QColorGroup::Highlight);
					psx->setPen(QPen(drawColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
					drawColor.setAlpha(64);
					psx->setBrush(drawColor);
					psx->drawPolygon(redrawPolygon);
					redrawPolygon.clear();
				}
			}
		}
		else
		{
			if (Doc->m_Selection->count() != 0)
			{
				uint selectedItemCount = Doc->m_Selection->count();
				PageItem *currItem = Doc->m_Selection->itemAt(0);
				if (selectedItemCount < moveWithBoxesOnlyThreshold)
				{
					for (uint cu = 0; cu < selectedItemCount; cu++)
					{
						currItem = Doc->m_Selection->itemAt(cu);
						psx->resetMatrix();
						QPoint out = contentsToViewport(QPoint(0, 0));
						psx->translate(out.x(), out.y());
						psx->translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
						Transform(currItem, psx);
						psx->setBrush(Qt::NoBrush);
						psx->setPen(QPen(Qt::black, 1.0 / Scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
						if (selectedItemCount < moveWithFullOutlinesThreshold)
						{
							if (!(currItem->asLine()))
								currItem->DrawPolyL(psx, currItem->Clip);
							else
							{
								if (currItem->asLine())
								{
									int lw2 = 1;
									int lw = 1;
									Qt::PenCapStyle le = Qt::FlatCap;
									if (currItem->NamedLStyle.isEmpty())
									{
										lw2 = qRound(currItem->lineWidth()  / 2.0);
										lw = qRound(qMax(currItem->lineWidth(), 1.0));
										le = currItem->PLineEnd;
									}
									else
									{
										multiLine ml = Doc->MLineStyles[currItem->NamedLStyle];
										lw2 = qRound(ml[ml.size()-1].Width  / 2.0);
										lw = qRound(qMax(ml[ml.size()-1].Width, 1.0));
										le = static_cast<Qt::PenCapStyle>(ml[ml.size()-1].LineEnd);
									}
									if (le != Qt::FlatCap)
										psx->drawRect(-lw2, -lw2, qRound(currItem->width())+lw, lw);
									else
										psx->drawRect(-1, -lw2, qRound(currItem->width()), lw);
								}
							}
						}
						else
							psx->drawRect(0, 0, static_cast<int>(currItem->width())+1, static_cast<int>(currItem->height())+1);
					}
				}
				else
				{
					double gx, gy, gw, gh;
					Doc->m_Selection->setGroupRect();
					getGroupRectScreen(&gx, &gy, &gw, &gh);
					QPoint out = contentsToViewport(QPoint(0, 0));
					psx->resetMatrix();
					psx->translate(-out.x(), -out.y());
					psx->translate(qRound(gx), qRound(gy));
					psx->scale(Scale, Scale);
					psx->drawRect(QRect(0, 0, qRound(gw), qRound(gh)));
				}
			}
		}
	}
	else
	{ */
		if ((Doc->m_Selection->count() != 0) && !(operItemMoving || operItemResizing) && (Doc->appMode != modeDrawBezierLine))
		{
			PageItem *currItem = Doc->m_Selection->itemAt(0);
			if ((Doc->EditClip) && (currItem->isSelected()) && (!specialRendering))
			{
				if (EditContour)
					MarkClip(psx, currItem, currItem->ContourLine, true);
				else
					MarkClip(psx, currItem, currItem->PoLine, true);
			}
			else
			{
				if (Doc->m_Selection->isMultipleSelection())
				{
				
					psx->resetMatrix();
					QPoint out = contentsToViewport(QPoint(0, 0));
					psx->translate(out.x(), out.y());
					psx->translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
					if (Doc->m_Selection->count() != 0)
					{
						uint docSelectionCount = Doc->m_Selection->count();
						PageItem *currItem;
						for (uint a=0; a<docSelectionCount; ++a)
						{
							currItem = Doc->m_Selection->itemAt(a);
							psx->save();
							Transform(currItem, psx);
	 						currItem->paintObj(psx);
	 						psx->restore();
	 					}
	 				}
					psx->resetMatrix();
					psx->translate(out.x(), out.y());
					psx->translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
					double x, y, w, h;
					Doc->m_Selection->setGroupRect();
					Doc->m_Selection->getGroupRect(&x, &y, &w, &h);
					x *= Scale;
					y *= Scale;
					w *= Scale;
					h *= Scale;
					psx->setPen(QPen(Qt::red, 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
					psx->setBrush(Qt::NoBrush);
					psx->drawRect(QRectF(x, y, w, h));
					psx->setBrush(Qt::red);
					psx->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
					psx->drawRect(QRectF(x+w-6, y+h-6, 6, 6));
					psx->drawRect(QRectF(x+w/2 - 3, y+h-6, 6, 6));
					psx->drawRect(QRectF(x+w/2 - 3, y, 6, 6));
					psx->drawRect(QRectF(x+w-6, y+h/2 - 3, 6, 6));
					psx->drawRect(QRectF(x+w-6, y, 6, 6));
					psx->drawRect(QRectF(x, y, 6, 6));
					psx->drawRect(QRectF(x, y+h/2 - 3, 6, 6));
					psx->drawRect(QRectF(x, y+h-6, 6, 6));
				}
				else
				{
					psx->resetMatrix();
					QPoint out = contentsToViewport(QPoint(0, 0));
					psx->translate(out.x(), out.y());
					psx->translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
					Transform(currItem, psx);
	 				currItem->paintObj(psx);
	 			}
	 		}
		}
/*	} */
	if (Doc->appMode == modeEdit)
		slotDoCurs(true);
	if (Doc->appMode == modeEditGradientVectors)
	{
		PageItem *currItem = Doc->m_Selection->itemAt(0);
		psx->resetMatrix();
		QPoint out = contentsToViewport(QPoint(0, 0));
		psx->translate(out.x(), out.y());
		psx->translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
		Transform(currItem, psx);
		psx->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		psx->setBrush(Qt::NoBrush);
		psx->drawLine(QPointF(currItem->GrStartX, currItem->GrStartY), QPointF(currItem->GrEndX, currItem->GrEndY));
		psx->setPen(QPen(Qt::magenta, 8, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
		psx->drawPoint(QPointF(currItem->GrStartX, currItem->GrStartY));
		psx->drawPoint(QPointF(currItem->GrEndX, currItem->GrEndY));
	}
	evSpon = false;
	forceRedraw = false;
//	qDebug( "Time elapsed: %d ms", tim.elapsed() );
}

void ScribusView::DrawMasterItems(ScPainter *painter, Page *page, QRect clip)
{
	QRect oldR;
	QStack<PageItem*> groupStack;
	QStack<PageItem*> groupStack2;
	if (!page->MPageNam.isEmpty())
	{
		Page* Mp = Doc->MasterPages.at(Doc->MasterNames[page->MPageNam]);
		if (page->FromMaster.count() != 0)
		{
			int Lnr;
			ScLayer ll;
			PageItem *currItem;
			ll.isViewable = false;
			ll.LNr = 0;
			Lnr = 0;
			uint layerCount=Doc->layerCount();
			for (uint la = 0; la < layerCount; ++la)
			{
				Doc->Layers.levelToLayer(ll, Lnr);
				bool pr = true;
				if ((previewMode) && (!ll.isPrintable))
					pr = false;
				if ((viewAsPreview) && (!ll.isPrintable))
					pr = false;
				if ((ll.isViewable) && (pr))
				{
					if ((layerCount > 1) || (ll.transparency != 1.0))
						painter->beginLayer(ll.transparency, ll.blendMode);
					uint pageFromMasterCount=page->FromMaster.count();
					for (uint a = 0; a < pageFromMasterCount; ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->pageNr())))
							continue;
						if ((previewMode) && (!currItem->printEnabled()))
							continue;
						if ((viewAsPreview) && (!currItem->printEnabled()))
							continue;
						double OldX = currItem->xPos();
						double OldY = currItem->yPos();
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->moveBy(-Mp->xOffset() + page->xOffset(), -Mp->yOffset() + page->yOffset(), true);
							currItem->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
							currItem->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
						}
						oldR = currItem->getRedrawBounding(Scale);
						if (currItem->isGroupControl)
						{
							painter->save();
							currItem->savedOwnPage = currItem->OwnPage;
							currItem->OwnPage = page->pageNr();
							if ((clip.intersects(oldR)) && (Doc->guidesSettings.layerMarkersShown) && (Doc->layerCount() > 1))
								currItem->DrawObj(painter, clip);
							FPointArray cl = currItem->PoLine.copy();
							QMatrix mm;
							mm.translate(currItem->xPos(), currItem->yPos());
							mm.rotate(currItem->rotation());
							cl.map( mm );
							painter->beginLayer(1.0 - currItem->fillTransparency(), currItem->fillBlendmode(), &cl);
							groupStack.push(currItem->groupsLastItem);
							groupStack2.push(currItem);
							currItem->OwnPage = currItem->savedOwnPage;
							if (!currItem->ChangedMasterItem)
							{
								//Hack to not check for undo changes, indicate drawing only
								currItem->setXYPos(OldX, OldY, true);
								currItem->BoundingX = OldBX;
								currItem->BoundingY = OldBY;
							}
							continue;
						}
						currItem->savedOwnPage = currItem->OwnPage;
						currItem->OwnPage = page->pageNr();
						if (!evSpon || forceRedraw)
							currItem->invalid = true;
						if (clip.intersects(oldR))
						{
							if (!((operItemMoving || operItemResizeInEditMode) && (currItem->isSelected())))
								currItem->DrawObj(painter, clip);
						}
						currItem->OwnPage = currItem->savedOwnPage;
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->setXYPos(OldX, OldY, true);
							currItem->BoundingX = OldBX;
							currItem->BoundingY = OldBY;
						}
						if (groupStack.count() != 0)
						{
							while (currItem == groupStack.top())
							{
								painter->endLayer();
								painter->restore();
								PageItem *cite = groupStack2.pop();
								double OldX = cite->xPos();
								double OldY = cite->yPos();
								double OldBX = cite->BoundingX;
								double OldBY = cite->BoundingY;
								if (!cite->ChangedMasterItem)
								{
									//Hack to not check for undo changes, indicate drawing only
									cite->moveBy(-Mp->xOffset() + page->xOffset(), -Mp->yOffset() + page->yOffset(), true);
									cite->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
									cite->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
								}
								oldR = cite->getRedrawBounding(Scale);
								if ((clip.intersects(oldR)) && (Doc->guidesSettings.layerMarkersShown) && (Doc->layerCount() > 1))
									cite->DrawObj(painter, clip);
								cite->OwnPage = cite->savedOwnPage;
								if (!currItem->ChangedMasterItem)
								{
									//Hack to not check for undo changes, indicate drawing only
									cite->setXYPos(OldX, OldY, true);
									cite->BoundingX = OldBX;
									cite->BoundingY = OldBY;
								}
								groupStack.pop();
								if (groupStack.count() == 0)
									break;
							}
						}
					}
					for (uint a = 0; a < pageFromMasterCount; ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if (!currItem->isTableItem)
							continue;
						if ((previewMode) && (!currItem->printEnabled()))
							continue;
						if ((viewAsPreview) && (!currItem->printEnabled()))
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->pageNr())))
							continue;
						if (currItem->isGroupControl)
							continue;
						double OldX = currItem->xPos();
						double OldY = currItem->yPos();
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->setXYPos(OldX - Mp->xOffset() + page->xOffset(), OldY - Mp->yOffset() + page->yOffset(), true);
							currItem->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
							currItem->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
						}
						oldR = currItem->getRedrawBounding(Scale);
						if (clip.intersects(oldR))
						{
							painter->save();
							painter->translate(currItem->xPos(), currItem->yPos());
							painter->rotate(currItem->rotation());
							if ((currItem->lineColor() != CommonStrings::None) && (currItem->lineWidth() != 0.0))
							{
								QColor tmp;
								currItem->SetFarbe(&tmp, currItem->lineColor(), currItem->lineShade());
								if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
								{
									painter->setPen(tmp, currItem->lineWidth(), currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
									if (currItem->TopLine)
										painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->width(), 0.0));
									if (currItem->RightLine)
										painter->drawLine(FPoint(currItem->width(), 0.0), FPoint(currItem->width(), currItem->height()));
									if (currItem->BottomLine)
										painter->drawLine(FPoint(currItem->width(), currItem->height()), FPoint(0.0, currItem->height()));
									if (currItem->LeftLine)
										painter->drawLine(FPoint(0.0, currItem->height()), FPoint(0.0, 0.0));
								}
							}
							painter->restore();
						}
						if (!currItem->ChangedMasterItem)
						{
							//Hack to not check for undo changes, indicate drawing only
							currItem->setXYPos(OldX, OldY, true);
							currItem->BoundingX = OldBX;
							currItem->BoundingY = OldBY;
						}
					}
					if ((layerCount > 1) || (ll.transparency != 1.0))
						painter->endLayer();
				}
				Lnr++;
			}
		}
	}
}

void ScribusView::DrawPageItems(ScPainter *painter, QRect clip)
{
	linkedFramesToShow.clear();
	QRect oldR;
	QStack<PageItem*> groupStack;
	QStack<PageItem*> groupStack2;
	if (Doc->Items->count() != 0)
	{
		int Lnr=0;
		ScLayer ll;
		PageItem *currItem;
		ll.isViewable = false;
		ll.LNr = 0;
		uint layerCount=Doc->layerCount();
		int docCurrPageNo=static_cast<int>(Doc->currentPageNumber());
		for (uint la2 = 0; la2 < layerCount; ++la2)
		{
			Doc->Layers.levelToLayer(ll, Lnr);
			bool pr = true;
			if ((previewMode) && (!ll.isPrintable))
				pr = false;
			if ((viewAsPreview) && (!ll.isPrintable))
				pr = false;
			if ((ll.isViewable) && (pr))
			{
				if ((layerCount > 1) || (ll.transparency != 1.0))
					painter->beginLayer(ll.transparency, ll.blendMode);
//				QListIterator<PageItem*> docItem(*Doc->Items);
//				while ( (currItem = docItem.current()) != 0)
//				{
//					++docItem;
				for (int it = 0; it < Doc->Items->count(); ++it)
				{
					currItem = Doc->Items->at(it);
					if (currItem->LayerNr != ll.LNr)
						continue;
					if ((previewMode) && (!currItem->printEnabled()))
						continue;
					if ((viewAsPreview) && (!currItem->printEnabled()))
						continue;
					if ((Doc->masterPageMode()) && ((currItem->OwnPage != -1) && (currItem->OwnPage != docCurrPageNo)))
						continue;
					if (!Doc->masterPageMode() && !currItem->OnMasterPage.isEmpty())
					{
						if (currItem->OnMasterPage != Doc->currentPage()->pageName())
							continue;
					}
					oldR = currItem->getRedrawBounding(Scale);
					if (currItem->isGroupControl)
					{
						painter->save();
						FPointArray cl = currItem->PoLine.copy();
						QMatrix mm;
						mm.translate(currItem->xPos(), currItem->yPos());
						mm.rotate(currItem->rotation());
						cl.map( mm );
						painter->beginLayer(1.0 - currItem->fillTransparency(), currItem->fillBlendmode(), &cl);
						groupStack.push(currItem->groupsLastItem);
						groupStack2.push(currItem);
						continue;
					}
					if (clip.intersects(oldR))
					{
						if (!evSpon || forceRedraw) 
							currItem->invalid = true;
//						if ((!m_MouseButtonPressed) || (Doc->EditClip))
						if (!((operItemMoving || operItemResizeInEditMode) && (currItem->isSelected())))
							currItem->DrawObj(painter, clip);
//						currItem->Redrawn = true;
						if ((currItem->asTextFrame()) && ((currItem->nextInChain() != 0) || (currItem->prevInChain() != 0)))
						{
							PageItem *nextItem = currItem;
							while (nextItem != 0)
							{
								if (nextItem->prevInChain() != 0)
									nextItem = nextItem->prevInChain();
								else
									break;
							}
							if (!linkedFramesToShow.contains(nextItem))
								linkedFramesToShow.append(nextItem);
						}
						if ((Doc->appMode == modeEdit) && (currItem->isSelected()) && (currItem->itemType() == PageItem::TextFrame))
						{
							//CB 230305 Stop redrawing the horizontal ruler if it hasnt changed when typing text!!!
							if ((qRound(horizRuler->ItemPos*10000) != qRound((currItem->xPos())*10000)) || (qRound(horizRuler->ItemEndPos*10000) != qRound(((currItem->xPos()+currItem->width()) )*10000)))
							{
								horizRuler->setItem(currItem);
								if (currItem->lineColor() != CommonStrings::None)
									horizRuler->lineCorr = currItem->lineWidth() / 2.0;
								else
									horizRuler->lineCorr = 0;
								horizRuler->ColGap = currItem->ColGap;
								horizRuler->Cols = currItem->Cols;
								horizRuler->Extra = currItem->textToFrameDistLeft();
								horizRuler->RExtra = currItem->textToFrameDistRight();
								horizRuler->First = currItem->currentStyle().firstIndent();
								horizRuler->Indent = currItem->currentStyle().leftMargin();
								double columnWidth = (currItem->width() - (currItem->columnGap() * (currItem->columns() - 1)) 
									- currItem->textToFrameDistLeft() - currItem->textToFrameDistLeft() 
									- 2*horizRuler->lineCorr) / currItem->columns();
								horizRuler->RMargin = columnWidth - currItem->currentStyle().rightMargin();
								if (currItem->imageFlippedH() || (currItem->reversed()))
									horizRuler->Revers = true;
								else
									horizRuler->Revers = false;
								horizRuler->ItemPosValid = true;
								horizRuler->TabValues = currItem->currentStyle().tabValues();
								horizRuler->update();
							}
						}
					}
					if (groupStack.count() != 0)
					{
						while (currItem == groupStack.top())
						{
							painter->endLayer();
							painter->restore();
							PageItem *cite = groupStack2.pop();
							oldR = cite->getRedrawBounding(Scale);
							if ((clip.intersects(oldR)) && (((Doc->guidesSettings.layerMarkersShown) && (Doc->layerCount() > 1)) || (cite->textFlowUsesContourLine())))
								cite->DrawObj(painter, clip);
							groupStack.pop();
							if (groupStack.count() == 0)
								break;
						}
					}
				}
//				QListIterator<PageItem*> docItem2(*Doc->Items);
//				while ( (currItem = docItem2.current()) != 0 )
//				{
//					++docItem2;
				for (int it = 0; it < Doc->Items->count(); ++it)
				{
					currItem = Doc->Items->at(it);
					if (currItem->LayerNr != ll.LNr)
						continue;
					if (!currItem->isTableItem)
						continue;
					if ((previewMode) && (!currItem->printEnabled()))
						continue;
					if ((viewAsPreview) && (!currItem->printEnabled()))
						continue;
					oldR = currItem->getRedrawBounding(Scale);
					if (clip.intersects(oldR))
					{
						painter->save();
						painter->translate(currItem->xPos(), currItem->yPos());
						painter->rotate(currItem->rotation());
						if ((currItem->lineColor() != CommonStrings::None) && (currItem->lineWidth() != 0.0))
						{
							QColor tmp;
							currItem->SetFarbe(&tmp, currItem->lineColor(), currItem->lineShade());
							if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
							{
								painter->setPen(tmp, currItem->lineWidth(), currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
								if (currItem->TopLine)
									painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->width(), 0.0));
								if (currItem->RightLine)
									painter->drawLine(FPoint(currItem->width(), 0.0), FPoint(currItem->width(), currItem->height()));
								if (currItem->BottomLine)
									painter->drawLine(FPoint(currItem->width(), currItem->height()), FPoint(0.0, currItem->height()));
								if (currItem->LeftLine)
									painter->drawLine(FPoint(0.0, currItem->height()), FPoint(0.0, 0.0));
							}
						}
						painter->restore();
					}
				}
				if ((layerCount > 1) || (ll.transparency != 1.0))
					painter->endLayer();
			}
			Lnr++;
		}
	}
}

void ScribusView::DrawPageMarks(ScPainter *p, Page *page, QRect clip)
{
	p->save();
	p->setAntialiasing(false);
	p->translate(page->xOffset(), page->yOffset());
	double lineWidth = 1.0 / Scale;
	double pageHeight=page->height();
	double pageWidth=page->width();
	p->setFillMode(ScPainter::None);
	p->setPen(Qt::black, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->drawRect(0, 0, pageWidth, pageHeight);
	//Draw the margins
	if (Doc->guidesSettings.marginsShown)
	{
		p->setPen(Doc->guidesSettings.margColor);
		if (Doc->marginColored)
		{
			p->setBrush(Doc->guidesSettings.margColor);
			p->drawRect(0, 0, pageWidth, page->Margins.Top);
			p->drawRect(0, page->Margins.Top, page->Margins.Left, pageHeight - page->Margins.Top);
			p->drawRect(page->Margins.Left, pageHeight - page->Margins.Bottom, pageWidth - page->Margins.Right - page->Margins.Left, page->Margins.Bottom);
			p->drawRect(pageWidth - page->Margins.Right, page->Margins.Top, page->Margins.Right, pageHeight-page->Margins.Top);
		}
//		p->setPen(Doc->guidesSettings.margColor);
		p->setFillMode(ScPainter::None);
		p->drawRect(page->Margins.Left, page->Margins.Top, pageWidth - page->Margins.Left - page->Margins.Right, pageHeight - page->Margins.Top - page->Margins.Bottom);
//		p->drawLine(FPoint(0, page->Margins.Top), FPoint(pageWidth, page->Margins.Top));
//		p->drawLine(FPoint(0, pageHeight - page->Margins.Bottom), FPoint(pageWidth, pageHeight - page->Margins.Bottom));
//		p->drawLine(FPoint(page->Margins.Left, 0), FPoint(page->Margins.Left, pageHeight));
//		p->drawLine(FPoint(pageWidth - page->Margins.Right, 0), FPoint(pageWidth - page->Margins.Right, pageHeight));
	}
	//Draw the baseline grid
	if (Doc->guidesSettings.baseShown)
	{
		p->setPen(Doc->guidesSettings.baseColor, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		for (double yg = Doc->typographicSettings.offsetBaseGrid; yg < pageHeight; yg += Doc->typographicSettings.valueBaseGrid)
			p->drawLine(FPoint(0, yg), FPoint(pageWidth, yg));
	}
	//Draw the grid lines
	if (Doc->guidesSettings.gridShown)
	{
		double lowerBx = qMax(clip.x() / Scale + Doc->minCanvasCoordinate.x() - page->xOffset(), 0.0);
		double lowerBy = qMax(clip.y() / Scale + Doc->minCanvasCoordinate.y() - page->yOffset(), 0.0);
		double highBx = qMin(lowerBx + clip.width() / Scale, pageWidth);
		double highBy = qMin(lowerBy + clip.height() / Scale, pageHeight);
		if (Scale > 0.49)
		{
			double i,start;
			i = Doc->guidesSettings.majorGrid;
			p->setPen(Doc->guidesSettings.majorColor, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			start=floor(lowerBy/i);
			start*=i;
			for (double b = start; b <= highBy; b+=i)
			{
				p->drawLine(FPoint(qMax(lowerBx, 0.0), b), FPoint(qMin(pageWidth, highBx), b));
			}
			start=floor(lowerBx/i);
			start*=i;
			for (double b = start; b <= highBx; b+=i)
			{
				p->drawLine(FPoint(b, qMax(lowerBy, 0.0)), FPoint(b, qMin(pageHeight, highBy)));
			}
			i = Doc->guidesSettings.minorGrid;
			p->setPen(Doc->guidesSettings.minorColor, lineWidth, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
			start=floor(lowerBy/i);
			start*=i;
			for (double b = start; b <= highBy; b+=i)
			{
				p->drawLine(FPoint(qMax(lowerBx, 0.0), b), FPoint(qMin(pageWidth, highBx), b));
			}
			start=floor(lowerBx/i);
			start*=i;
			for (double b = start; b <= highBx; b+=i)
			{
				p->drawLine(FPoint(b, qMax(lowerBy, 0.0)), FPoint(b, qMin(pageHeight, highBy)));
			}
		}
	}
	//Draw the guides
	if (Doc->guidesSettings.guidesShown)
		page->guides.drawPage(p, Doc, lineWidth);
	if (Doc->currentPage() == page)
	{
		p->setPen(Prefs->DPageBorderColor, 2 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawRect(0, 0, pageWidth, pageHeight);
	}
	p->setAntialiasing(true);

	p->restore();
}

void ScribusView::enterEvent(QEvent *)
{
	if (!m_MouseButtonPressed)
	{
		switch (Doc->appMode)
		{
			case modeDrawShapes:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawFrame.xpm")));
				break;
			case modeDrawPicture:
				qApp->changeOverrideCursor(QCursor(loadIcon("DrawImageFrame.xpm")));
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
	}
}

void ScribusView::leaveEvent(QEvent *)
{
/*	if (BlockLeave)
		return; */
	if (!m_MouseButtonPressed)
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
/*	else
	{
		if ((SelItem.count() != 0) && (m_MouseButtonPressed) && (!doku->DragP) && (doku->appMode == 1))
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
			m_MouseButtonPressed = false;
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
		Q3Url ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (fi.exists())
			text = ur.path();
		if(ss->ReadElemHeader(text,fi.exists(), &gx, &gy, &gw, &gh))
		{
			dragX = e->pos().x() / Scale;
			dragY = e->pos().y() / Scale;
			dragW = gw;
			dragH = gh;
			DraggedGroup = true;
			DraggedGroupFirst = true;
			getDragRectScreen(&gx, &gy, &gw, &gh);
			gx += Doc->minCanvasCoordinate.x();
			gy += Doc->minCanvasCoordinate.y();
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
			dragX = e->pos().x() / Scale;
			dragY = e->pos().y() / Scale;
			getDragRectScreen(&gx, &gy, &gw, &gh);
			gx += Doc->minCanvasCoordinate.x();
			gy += Doc->minCanvasCoordinate.y();
			QPoint evP = viewport()->mapToGlobal(e->pos());
			evP -= QPoint(contentsX(), contentsY());
			redrawMarker->setGeometry(QRect(evP.x() + 2, evP.y() + 2, qRound(gw - 2), qRound(gh - 2)).normalized());
			if (!redrawMarker->isVisible())
				redrawMarker->show();
			DraggedGroupFirst = false;
			emit MousePos(dragX+Doc->minCanvasCoordinate.x(), dragY+Doc->minCanvasCoordinate.y());
			horizRuler->Draw(e->pos().x());
			vertRuler->Draw(e->pos().y());
//			return;
		}
/*		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).upper();
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
		specialRendering = false;
		firstSpecial = false;
		redrawMarker->hide();
		updateContents();
	}
}

void ScribusView::contentsDropEvent(QDropEvent *e)
{
	QString text;
	PageItem *currItem;
	bool img = false;
	specialRendering = false;
	firstSpecial = false;
	redrawMode = 0;
	redrawMarker->hide();
//	struct ScText *hg;
//	uint a;
	int re = 0;
//	e->accept(Q3TextDrag::canDecode(e));
	e->accept();
	DraggedGroupFirst = false;
	int ex = qRound(e->pos().x()/Scale + Doc->minCanvasCoordinate.x());
	int ey = qRound(e->pos().y()/Scale + Doc->minCanvasCoordinate.y());
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
		Q3Url ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).upper();
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
		int pscx=qRound(e->pos().x()/Scale), pscy=qRound(e->pos().y()/Scale);
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
					update();
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
					if (fi.extension(true).lower() == "sml")
					{
						QString f = "";
						loadText(ur.path(), &f);
						StencilReader *pre = new StencilReader();
						data = pre->createObjects(f);
						delete pre;
						emit LoadElem(data, ex, ey, false, false, Doc, this);
					}
					else if (fi.extension(true).lower() == "shape")
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
				for (uint a = 0; a < Doc->m_Selection->count(); ++a)
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

void ScribusView::contentsMouseDoubleClickEvent(QMouseEvent *m)
{
	m->accept();
	m_MouseButtonPressed = false;
	specialRendering = false;
	firstSpecial = false;
	dragTimer->stop();
	PageItem *currItem = 0;
	if (Doc->EditClip)
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
					updateContents(currItem->getRedrawBounding(Scale));
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

void ScribusView::normalizeSelectionRect()
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

void ScribusView::contentsMouseReleaseEvent(QMouseEvent *m)
{
	PageItem *currItem;
	m_MouseButtonPressed = false;
	specialRendering = false;
	firstSpecial = false;
	m->accept();
	dragTimer->stop();
	if (Doc->appMode == modeNormal && Doc->guidesSettings.guidesShown)
	{
		bool foundGuide = false;
		double nx = translateToDoc(m->x(), m->y()).x();
		double ny = translateToDoc(m->x(), m->y()).y();
		double grabRadScale=Doc->guidesSettings.grabRad / Scale;
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
		redrawPolygon.clear();
		updateContents(QRect(QPoint(Dxp, Dyp), QPoint(Mxp, Mxp)).normalized().adjusted(-10, -10, 20, 20));
//		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		//emit PaintingDone();
		return;
	}
	if (Doc->appMode == modePanning)
	{
		if ((m->state() & Qt::RightButton) && (m->state() & Qt::ControlButton))
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
					z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Tx + offX, Ty + offY, deltaX, deltaY, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_MouseButtonPressed);
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
			uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, Mxp, Myp, 1, 1, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenLine, !m_MouseButtonPressed);
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
			operItemMoving = false;
			operItemResizing = false;
			inItemCreation = false;
			updateContents(currItem->getRedrawBounding(Scale).adjusted(-10, -10, 20, 20));
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
	if ((Doc->EditClip) && (ClRe == -1) && (HaveSelRect))
	{
		double sc = Scale;
		currItem = Doc->m_Selection->itemAt(0);
		SelNode.clear();
		QRect Sele = QRect(Dxp, Dyp, SeRx-Dxp, SeRy-Dyp).normalized();
		FPointArray Clip;
		Scale = 1.0;
		if (EditContour)
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
				ClRe = a;
				SelNode.append(a);
				emit ClipPo(Clip.point(a).x(), Clip.point(a).y());
			}
		}
		Scale = sc;
		HaveSelRect = false;
		redrawMarker->hide();
		RefreshItem(currItem);
		if (oldClip) // is there the old clip stored for the undo action
		{
			FPointArray newClip(isContourLine ? currItem->ContourLine : currItem->PoLine);
			if (*oldClip != newClip)
			{
				QString name = isContourLine ? Um::EditContour : Um::EditShape;
				ItemState<QPair<FPointArray, FPointArray> > *state =
						new ItemState<QPair<FPointArray, FPointArray> >(name);
				state->set("EDIT_SHAPE_OR_CONTOUR", "edit_shape_or_contour");
				state->set("IS_CONTOUR", isContourLine);
				state->setItem(QPair<FPointArray, FPointArray>(*oldClip, newClip));
				state->set("OLD_X", oldItemX);
				state->set("OLD_Y", oldItemY);
				state->set("NEW_X", currItem->xPos());
				state->set("NEW_Y", currItem->yPos());
				undoManager->action(currItem, state);
				undoManager->commit();
			}
			else
				undoManager->cancelTransaction();

			delete oldClip;
			oldClip = 0;
		}
		m_SnapCounter = 0;
		return;
	}
/*	if (moveTimerElapsed() && (Doc->EditClip) && (SegP1 == -1) && (SegP2 == -1))
	{
		currItem = Doc->m_Selection->itemAt(0);
		if (operItemMoving)
		{
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			double nx = m->x()/Scale + Doc->minCanvasCoordinate.x();
			double ny = m->y()/Scale + Doc->minCanvasCoordinate.y();
			if (!Doc->ApplyGuides(&nx, &ny))
			{
				FPoint npg(ApplyGridF(FPoint(nx, ny)));
				nx = npg.x();
				ny = npg.y();
			}
			FPoint np(nx, ny, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1, true);
			MoveClipPoint(currItem, np);
		}
		Doc->AdjustItemSize(currItem);
		emit DocChanged();
		updateContents();
		operItemMoving = false;
		return;
	}
	if (moveTimerElapsed() && (Doc->EditClip) && (SegP1 != -1) && (SegP2 != -1)) */
	if (moveTimerElapsed() && (Doc->EditClip))
	{
		dragTimer->stop();
		SegP1 = -1;
		SegP2 = -1;
		currItem = Doc->m_Selection->itemAt(0);
		operItemMoving = false;
		double xposOrig = currItem->xPos();
		double yposOrig = currItem->yPos();

		ItemState<QPair<FPointArray, FPointArray> > *state = NULL;
		if (oldClip) // is there the old clip stored for the undo action
		{
			FPointArray newClip(isContourLine ? currItem->ContourLine : currItem->PoLine);
			if (*oldClip != newClip)
			{
				QString name = isContourLine ? Um::EditContour : Um::EditShape;
				state = new ItemState<QPair<FPointArray, FPointArray> >(name);
				state->set("EDIT_SHAPE_OR_CONTOUR", "edit_shape_or_contour");
				state->set("IS_CONTOUR", isContourLine);
				state->setItem(QPair<FPointArray, FPointArray>(*oldClip, newClip));
				undoManager->setUndoEnabled(false);
			}
			else
			{
				delete oldClip;
				oldClip = 0;
				undoManager->cancelTransaction();
			}
		}
		xposOrig = currItem->xPos();
		yposOrig = currItem->yPos();
		if (ClRe != -1)
		{
			double newX = m->x();
			double newY = m->y();
			FPoint np(newX-Mxp, newY-Myp, 0, 0, currItem->rotation(), 1, 1, true);
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			FPointArray Clip;
			if (EditContour)
				Clip = currItem->ContourLine;
			else
				Clip = currItem->PoLine;
			FPoint npf = FPoint(Clip.point(ClRe).x() + np.x() / Scale, Clip.point(ClRe).y() + np.y() / Scale);
			double nx = npf.x();
			double ny = npf.y();
			nx += currItem->xPos();
			ny += currItem->yPos();
			if (!Doc->ApplyGuides(&nx, &ny))
				npf = Doc->ApplyGridF(FPoint(nx, ny));
			else
				npf = FPoint(nx, ny);
			npf = FPoint(npf.x() - currItem->xPos(), npf.y() - currItem->yPos());
			MoveClipPoint(currItem, npf);
		}

		Doc->AdjustItemSize(currItem);
		if (!EditContour)
			currItem->ContourLine.translate(xposOrig - currItem->xPos(),yposOrig - currItem->yPos());
		emit DocChanged();
		updateContents();
		if (oldClip)
		{
			state->set("OLD_X", oldItemX);
			state->set("OLD_Y", oldItemY);
			state->set("NEW_X", currItem->xPos());
			state->set("NEW_Y", currItem->yPos());
			undoManager->setUndoEnabled(true);
			undoManager->action(currItem, state);
			undoManager->commit();
			delete oldClip;
			oldClip = 0;
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
					QPixmap pm = it.data().Preview;
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
	if ((Doc->appMode != modeMagnifier) && (!Doc->EditClip) && (Doc->appMode != modeDrawBezierLine))
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
						QString ext = fi.extension(false).lower();
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
				pmen->insertSeparator();
				if (currItem->itemType() == PageItem::ImageFrame)
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
					pmenResolution->addAction(m_ScMW->scrActions["itemImageIsVisible"]);
					pmenResolution->insertSeparator();
					pmenResolution->addAction(m_ScMW->scrActions["itemPreviewLow"]);
					pmenResolution->addAction(m_ScMW->scrActions["itemPreviewNormal"]);
					pmenResolution->addAction(m_ScMW->scrActions["itemPreviewFull"]);
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
				for (uint bx = 0; bx < Doc->m_Selection->count(); ++bx)
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
				Transform(currItem, p);
				QPoint np = m->pos() * p.inverted();
				np += QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
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
				if (m->state() & (Qt::ShiftButton | Qt::ControlButton))
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
				operItemMoving = false;
				operItemResizing = false;
				specialRendering = false;
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
			Transform(currItem, p);
			QPoint np = m->pos() * p.inverted();
			np += QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
			np = Doc->ApplyGrid(np);
			double newRot=xy2Deg(np.x(), np.y());
			//Constrain rotation angle, when the mouse is released from drawing a line
			if (m->state() & Qt::ControlButton)
				newRot=constrainAngle(newRot, Doc->toolSettings.constrain);
			currItem->setRotation(newRot);
			currItem->setWidthHeight(sqrt(pow(np.x(),2.0)+pow(np.y(),2.0)), 1.0);
			currItem->Sizing = false;
			currItem->updateClip();
			Doc->setRedrawBounding(currItem);
			currItem->OwnPage = Doc->OnPage(currItem);
			operItemResizing = false;
			specialRendering = false;
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
			operItemResizing = false;
			specialRendering = false;
			updateContents();
		}
		if (moveTimerElapsed() && (GetItem(&currItem)))
		{
			dragTimer->stop();
			specialRendering = false;
			if (Doc->m_Selection->isMultipleSelection())
			{
				if (operItemResizing)
				{
					double gx, gy, gh, gw, nx, ny, scx, scy;
					Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					double sc = Scale;
					scx = sc;
					scy = sc;
					QPoint np2;
					double newXF = translateToDoc(m->x(), m->y()).x();
					double newYF = translateToDoc(m->x(), m->y()).y();
					if (m->state() & Qt::ControlButton)
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
					if ((currItem->itemType() == PageItem::TextFrame) && (m->state() & Qt::AltButton))
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
					operItemResizing = false;
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
					operItemResizing = false;
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
								updateContents(currItem->getRedrawBounding(Scale));
							}
							if (!currItem->imageFlippedV())
							{
								currItem->moveImageInFrame(0, (currItem->height() - currItem->OldH2)/currItem->imageYScale());
								updateContents(currItem->getRedrawBounding(Scale));
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
					//if ((currItem->itemType() == PageItem::TextFrame) && (m->state() & Qt::ShiftButton) && (m->state() & Qt::ControlButton))
					//NOTE: this does not cover groups, strangely enough. Thats done in scaleGroup()
					if ((currItem->itemType() == PageItem::TextFrame) && (m->state() & Qt::AltButton))
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
				operItemResizing = false;
				if (currItem->asLine())
					updateContents();
			}
			if (operItemMoving)
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
				operItemMoving = false;
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
					Transform(docItem, p);
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
	if ((Doc->appMode == modeEdit) && !operItemResizeInEditMode)
	{
		currItem = Doc->m_Selection->itemAt(0);
		if (currItem->asTextFrame())
		{
			if (oldCp == currItem->CPos)
			{
				currItem->itemText.deselectAll();
				currItem->HasSel = false;
				emit HasNoTextSel();
				RefreshItem(currItem);
			}
		}
		else
			emit DocChanged();
	}
	if (Doc->appMode == modeMagnifier)
	{
		double sc = Scale;
		if (HaveSelRect)
		{
			QRect geom = redrawMarker->geometry().normalized();
			QPoint xp = viewport()->mapFromGlobal(QPoint(geom.x() + geom.width() / 2, geom.y() + geom.height() / 2));
			FPoint nx = translateToDoc(xp.x()+contentsX(), xp.y()+contentsY());
			double scaleAdjust = visibleWidth() / static_cast<double>(qMax(geom.width(), 1));
			setScale(Scale * scaleAdjust);
			slotDoZoom();
			SetCCPo(nx.x(), nx.y());
			if (sc == Scale)
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
			if (sc == Scale)
			{
				HaveSelRect = false;
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				Doc->appMode = modeNormal;
				emit PaintingDone();
			}
			else
			{
				if (m->state() & Qt::ShiftButton)
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZm.xpm")));
				else
					qApp->changeOverrideCursor(QCursor(loadIcon("LupeZ.xpm")));
			}
		}
	}
	specialRendering = false;
	if ((Doc->appMode == modeDrawBezierLine) && (m->button() == Qt::LeftButton))
	{
		specialRendering = true;
		currItem = Doc->m_Selection->itemAt(0);
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		QMatrix pm;
		pm.translate(-Doc->minCanvasCoordinate.x()*Scale, -Doc->minCanvasCoordinate.y()*Scale);
		Transform(currItem, pm);
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
			specialRendering = false;
			repaintContents(currItem->getRedrawBounding(Scale));
		}
		specialRendering = true;
		firstSpecial = true;
		redrawPolygon.clear();
		int newX = qRound(translateToDoc(m->x(), m->y()).x());
		int newY = qRound(translateToDoc(m->x(), m->y()).y());
		redrawPolygon << QPoint(newX - qRound(currItem->xPos()), newY - qRound(currItem->yPos()));
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
		specialRendering = false;
		updateContents();
	}
	Doc->DragP = false;
	Doc->leaveDrag = false;
	operItemMoving = false;
	operItemResizing = false;
	MidButt = false;
	shiftSelItems = false;
	inItemCreation = false;
	m_SnapCounter = 0;
//	Doc->SubMode = -1;
	if (_groupTransactionStarted)
	{
		for (uint i = 0; i < Doc->m_Selection->count(); ++i)
			Doc->m_Selection->itemAt(i)->checkChanges(true);
		undoManager->commit();
		_groupTransactionStarted = false;
	}

	for (uint i = 0; i < Doc->m_Selection->count(); ++i)
		Doc->m_Selection->itemAt(i)->checkChanges(true);

	//Commit drag created items to undo manager.
	if (Doc->m_Selection->itemAt(0)!=NULL)
	{
		Doc->itemAddCommit(Doc->m_Selection->itemAt(0)->ItemNr);
	}
	//Make sure the Zoom spinbox and page selector dont have focus if we click on the canvas
	zoomSpinBox->clearFocus();
	pageSelector->clearFocus();
	if (oldClip && Doc->m_Selection->itemAt(0) != 0) // is there the old clip stored for the undo action
	{
		currItem = Doc->m_Selection->itemAt(0);
		FPointArray newClip(isContourLine ? currItem->ContourLine : currItem->PoLine);
		if (*oldClip != newClip)
		{
			QString name = isContourLine ? Um::EditContour : Um::EditShape;
			ItemState<QPair<FPointArray, FPointArray> > *state =
					new ItemState<QPair<FPointArray, FPointArray> >(name);
			state->set("EDIT_SHAPE_OR_CONTOUR", "edit_shape_or_contour");
			state->set("IS_CONTOUR", isContourLine);
			state->setItem(QPair<FPointArray, FPointArray>(*oldClip, newClip));
			state->set("OLD_X", oldItemX);
			state->set("OLD_Y", oldItemY);
			state->set("NEW_X", currItem->xPos());
			state->set("NEW_Y", currItem->yPos());
			undoManager->action(currItem, state);
			undoManager->commit();
		}
		else
			undoManager->cancelTransaction();

		delete oldClip;
		oldClip = 0;
	}

	delete oldClip;
	oldClip = 0;
}

void ScribusView::contentsMouseMoveEvent(QMouseEvent *m)
{
	int newX, newY;
	double nx, ny;
	uint a;
	PageItem *currItem;
	QPoint np, np2, mop;
	FPoint npf, npf2;
	QPainter p;
	QRect tx;
	Q3PointArray Bez(4);
	bool erf = false;
	double sc = Scale;
	horizRuler->Draw(m->x());
	vertRuler->Draw(m->y());
	m->accept();
	emit MousePos(m->x()/Scale + Doc->minCanvasCoordinate.x(), m->y()/Scale + Doc->minCanvasCoordinate.y());
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
/*	if ((!Doc->DragP) && (m_MouseButtonPressed) && (SelItem.count() != 0))
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

	if (m_MouseButtonPressed && (Doc->appMode == modeEditGradientVectors))
	{
		PageItem *currItem = Doc->m_Selection->itemAt(0);
		newX = m->x();
		newY = m->y();
		double dx = abs(Mxp - newX) + 5;
		double dy = abs(Myp - newY) + 5;
		if (m->state() == Qt::LeftButton)
		{
			currItem->GrStartX -= (Mxp - newX) / Scale;
			currItem->GrStartY -= (Myp - newY) / Scale;
		}
		if (m->state() == Qt::RightButton)
		{
			currItem->GrEndX -= (Mxp - newX) / Scale;
			currItem->GrEndY -= (Myp - newY) / Scale;
		}
		Mxp = newX;
		Myp = newY;
		RefreshGradient(currItem, dx, dy);
		m_ScMW->propertiesPalette->updateColorSpecialGradient();
		return;
	}
	if (m_MouseButtonPressed && (Doc->appMode == modeMeasurementTool))
	{
		newX = m->x();
		newY = m->y();
		redrawPolygon.clear();
		redrawPolygon << QPoint(Dxp, Dyp) << QPoint(newX, newY);
		updateContents(QRect(QPoint(Dxp, Dyp), QPoint(newX, newY)).normalized().adjusted(-10, -10, 20, 20));
		Mxp = newX;
		Myp = newY;
		double dxp = Dxp / sc - Doc->currentPage()->xOffset()+ Doc->minCanvasCoordinate.x();
		double dyp = Dyp / sc - Doc->currentPage()->yOffset()+ Doc->minCanvasCoordinate.y();
		double nxp = newX / sc - Doc->currentPage()->xOffset()+ Doc->minCanvasCoordinate.x();
		double nyp = newY / sc - Doc->currentPage()->yOffset()+ Doc->minCanvasCoordinate.y();
		emit MVals(dxp, dyp, nxp, nyp, -xy2Deg(newX/sc - Dxp*sc, newY/sc - Dyp/sc), sqrt(pow(newX/sc - Dxp/sc,2)+pow(newY/sc - Dyp/sc,2)), Doc->unitIndex());
		return;
	}
	if (m_MouseButtonPressed && (m->state() & Qt::RightButton) && (m->state() & Qt::ControlButton))
	{
		m_ScMW->setAppMode(modePanning);
	}
	if (m_MouseButtonPressed && (Doc->appMode == modePanning))
	{
		int scroX = m->x() - qRound((Mxp * sc));
		int scroY = m->y() - qRound((Myp * sc));
		scrollBy(-scroX, -scroY);
		Mxp = static_cast<int>((m->x()-scroX)/sc);
		Myp = static_cast<int>((m->y()-scroY)/sc);
		return;
	}
	if (m_MouseButtonPressed && (Doc->appMode == modeDrawFreehandLine))
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
		redrawPolygon.clear();
		for (uint pp = 0; pp < RecordP.size(); pp++)
		{
			redrawPolygon << RecordP.pointQ(pp);
		}
		operItemResizing = true;
		QRect bRect = redrawPolygon.boundingRect();
		QPoint in(qRound(bRect.x()*Scale), qRound(bRect.y()*Scale));
		in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
		QPoint out = contentsToViewport(in);
		updateContents(QRect(out.x()+contentsX(), out.y()+contentsY(), qRound(bRect.width()*Scale), qRound(bRect.height()*Scale)).adjusted(-10, -10, 20, 20));
		return;
	}
	if ((GetItem(&currItem)) && (!shiftSelItems))
	{
		newX = qRound(translateToDoc(m->x(), m->y()).x());
		newY = qRound(translateToDoc(m->x(), m->y()).y());
		if ((((dragTimerFired) && (m->state() == Qt::LeftButton)) || (moveTimerElapsed() && (m->state() == Qt::RightButton)))
			&& (m_MouseButtonPressed)
			&& (!Doc->DragP) 
			&& (Doc->appMode == modeNormal) 
			&& (!(currItem->isSingleSel)))
		{
			dragTimer->stop();
			if ((abs(Dxp - newX) > 10) || (abs(Dyp - newY) > 10))
			{
				specialRendering = false;
				firstSpecial = false;
				dragTimerFired = false;
				Doc->DragP = true;
				Doc->leaveDrag = false;
				Doc->DraggedElem = currItem;
				Doc->DragElements.clear();
				for (uint dre=0; dre<Doc->m_Selection->count(); ++dre)
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
//				pm.convertFromImage(drImg);
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
				m_MouseButtonPressed = false;
				Doc->DraggedElem = 0;
				Doc->DragElements.clear();
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
				updateContents();
			}
			return;
		}
		if (Doc->DragP)
			return;
		if (moveTimerElapsed() && m_MouseButtonPressed && (Doc->appMode == modeRotation))
		{
			dragTimer->stop();
			double newW = xy2Deg(m->x()/sc - RCenter.x(), m->y()/sc - RCenter.y());
			if (m->state() & Qt::ControlButton)
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
			redrawPolygon.clear();
			redrawPolygon << QPoint(newX - qRound(currItem->xPos()), newY - qRound(currItem->yPos()));
			updateContents();
			Mxp = newX;
			Myp = newY;
		}
		if (m_MouseButtonPressed && (Doc->appMode == modeDrawRegularPolygon))
		{
			if (m->state() & Qt::ShiftButton)
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
			mp.translate(currItem->xPos() - Doc->minCanvasCoordinate.x(), currItem->yPos() - Doc->minCanvasCoordinate.y());
			mp.rotate(currItem->rotation());
			np2 = QPoint(newX, newY) * mp.inverted();
			QMatrix pm;
			Transform(currItem, pm);
			PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), np2)));
			emit ItemGeom(newX - currItem->xPos(), newY - currItem->yPos());
			Mxp = newX;
			Myp = newY;
		}
		if (m_MouseButtonPressed && (Doc->appMode == modeDrawLine))
		{
			if (Doc->useRaster)
			{
				newX = static_cast<int>(qRound(newX / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
				newY = static_cast<int>(qRound(newY / Doc->guidesSettings.minorGrid) * Doc->guidesSettings.minorGrid);
			}
			double newRot=xy2Deg(newX - currItem->xPos(), newY - currItem->yPos());
			//Constrain rotation angle, when the mouse is being dragged around for a new line
			if (m->state() & Qt::ControlButton)
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
			redrawPolygon.clear();
			redrawPolygon << QPoint(qRound(currItem->xPos()), qRound(currItem->yPos())) << QPoint(newX, newY);
			newX = m->x();
			newY = m->y();
			updateContents(QRect(QPoint(Dxp, Dyp), QPoint(newX, newY)).normalized().adjusted(-10, -10, 20, 20));
			Mxp = newX;
			Myp = newY;
		}
		if (m_MouseButtonPressed && (Doc->appMode == modeEdit) && (!operItemResizeInEditMode))
		{
			if (currItem->asImageFrame())
			{
				QMatrix ro;
				ro.rotate(-currItem->rotation());
				QPoint rota = ro.map(QPoint(newX-Mxp,newY-Myp));
				currItem->moveImageInFrame(rota.x()/currItem->imageXScale(), rota.y()/currItem->imageYScale());
				updateContents(currItem->getRedrawBounding(Scale));
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
		if (moveTimerElapsed() && m_MouseButtonPressed && (m->state() & Qt::LeftButton) && ((Doc->appMode == modeNormal) || ((Doc->appMode == modeEdit) && operItemResizeInEditMode)) && (!currItem->locked()))
		{
			dragTimer->stop();
			if (Doc->EditClip)
			{
				if ((m_MouseButtonPressed) && (ClRe == -1) && (SegP1 == -1) && (SegP2 == -1))
				{
					newX = qRound(translateToDoc(m->x(), m->y()).x());
					newY = qRound(translateToDoc(m->x(), m->y()).y());
					redrawMarker->setGeometry(QRect(Mxp, Myp, m->globalPos().x() - Mxp, m->globalPos().y() - Myp).normalized());
					if (!redrawMarker->isVisible())
						redrawMarker->show();
					HaveSelRect = true;
					SeRx = newX;
					SeRy = newY;
					return;
				}
				newX = m->x();
				newY = m->y();
				FPoint np(newX-Mxp, newY-Myp, 0, 0, currItem->rotation(), 1, 1, true);
				operItemMoving = true;
				m_SnapCounter++;
				if (m_SnapCounter > 3)
					m_SnapCounter = 0;
				currItem = Doc->m_Selection->itemAt(0);
				currItem->OldB2 = currItem->width();
				currItem->OldH2 = currItem->height();
				FPointArray Clip;
				if (EditContour)
					Clip = currItem->ContourLine;
				else
					Clip = currItem->PoLine;
				npf.setX(Clip.point(ClRe).x() + np.x() / Scale);
				npf.setY(Clip.point(ClRe).y() + np.y() / Scale);
				if ((SegP1 != -1) && (SegP2 != -1))
				{
					npf.setX(Clip.point(SegP2).x() + np.x() / Scale);
					npf.setY(Clip.point(SegP2).y() + np.y() / Scale);
					ClRe = SegP2;
					MoveClipPoint(currItem, npf);
					currItem->OldB2 = currItem->width();
					currItem->OldH2 = currItem->height();
					if (EditContour)
						Clip = currItem->ContourLine;
					else
						Clip = currItem->PoLine;
					ClRe = SegP1;
					npf2.setX(Clip.point(SegP1).x() + np.x() / Scale);
					npf2.setY(Clip.point(SegP1).y() + np.y() / Scale);
					MoveClipPoint(currItem, npf2);
					Mxp = newX;
					Myp = newY;
				}
				else
				{
					if ((SelNode.count() != 0) && (EdPoints))
					{
						int storedClRe = ClRe;
						for (int itm = 0; itm < SelNode.count(); ++itm)
						{
							if (EditContour)
								Clip = currItem->ContourLine;
							else
								Clip = currItem->PoLine;
							npf.setX(Clip.point(SelNode.at(itm)).x() + np.x() / Scale);
							npf.setY(Clip.point(SelNode.at(itm)).y() + np.y() / Scale);
							ClRe = SelNode.at(itm);
							currItem->OldB2 = currItem->width();
							currItem->OldH2 = currItem->height();
							if (((ClRe != 0) && (SelNode.count() > 1)) || (SelNode.count() == 1))
							{
								if ((SelNode.count() == 1) && (m_SnapCounter > 2))
								{
									double nx = npf.x();
									double ny = npf.y();
									nx += currItem->xPos();
									ny += currItem->yPos();
									if (!Doc->ApplyGuides(&nx, &ny))
										npf = Doc->ApplyGridF(FPoint(nx, ny));
									else
										npf = FPoint(nx, ny);
									npf = FPoint(npf.x() - currItem->xPos(), npf.y() - currItem->yPos());
									m_SnapCounter = 0;
								}
								MoveClipPoint(currItem, npf);
							}
						}
						currItem->OldB2 = currItem->width();
						currItem->OldH2 = currItem->height();
						ClRe = storedClRe;
					}
					else
					{
						if (m_SnapCounter > 2)
						{
							double nx = npf.x();
							double ny = npf.y();
							nx += currItem->xPos();
							ny += currItem->yPos();
							if (!Doc->ApplyGuides(&nx, &ny))
								npf = Doc->ApplyGridF(FPoint(nx, ny));
							else
								npf = FPoint(nx, ny);
							npf = FPoint(npf.x() - currItem->xPos(), npf.y() - currItem->yPos());
							m_SnapCounter = 0;
						}
						MoveClipPoint(currItem, npf);
					}
					Mxp = newX;
					Myp = newY;
				}
				operItemMoving = false;
				updateContents();
				return;
			}
			if (operItemResizing)
			{
				newX = static_cast<int>(m->x()/sc);
				newY = static_cast<int>(m->y()/sc);
				operItemMoving = false;
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
						if (m->state() & Qt::ControlButton)
							np2 = QPoint(qRound(newX), qRound(gy+(gh * ((newX-gx) / gw))));
						else
							np2 = QPoint(qRound(newX), qRound(newY));
						np2 = Doc->ApplyGrid(np2);
						nx = np2.x();
						ny = np2.y();
						Doc->ApplyGuides(&nx, &ny);
						np2 = QPoint(qRound(nx*sc), qRound(ny*sc));
						PaintSizeRect(pm.mapToPolygon(QRect(QPoint(qRound(gx*sc), qRound(gy*sc)), np2)));
						break;
					case 2:
						PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(ox2,oy2))));
						break;
					case 3:
						PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(ox1, oy2))));
						break;
					case 4:
						PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(ox2, oy1))));
						break;
					case 5:
						PaintSizeRect(pm.mapToPolygon(QRect(QPoint(ox1, oy1), QPoint(ox2, np2.y()))));
						break;
					case 6:
						PaintSizeRect(pm.mapToPolygon(QRect(QPoint(np2.x(), oy2), QPoint(ox1,oy1))));
						break;
					case 7:
						PaintSizeRect(pm.mapToPolygon(QRect(QPoint(np2.x(), oy1), QPoint(ox2, oy2))));
						break;
					case 8:
						PaintSizeRect(pm.mapToPolygon(QRect(QPoint(ox1, qRound(np2.y())), QPoint(ox2, oy2))));
						break;
					}
					Mxp = qRound(np2.x());
					Myp = qRound(np2.y());
				}
				else
				{
					//qDebug(QString("frameResizeHandle %1").arg(frameResizeHandle));
					for (a = 0; a < Doc->m_Selection->count(); ++a)
					{
						currItem = Doc->m_Selection->itemAt(0);
						double nh = currItem->height();
						if ((frameResizeHandle == 1) || (frameResizeHandle == 2))
						{
							QMatrix mp;
							switch (frameResizeHandle)
							{
							case 1:
								mp.translate(-Doc->minCanvasCoordinate.x() * Scale,-Doc->minCanvasCoordinate.y() * Scale);
								Transform(currItem, mp);
								//Shift proportional square resize
								if ((m->state() & Qt::ShiftButton) && (!(m->state() & Qt::ControlButton)))
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
									if ((m->state() & Qt::ControlButton) && (!(m->state() & Qt::ShiftButton)))
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
									if ((m->state() & Qt::ControlButton) || ((m->state() & Qt::ShiftButton)))
										erf = Doc->SizeItem(nx, nh, currItem->ItemNr);
									else
										erf = Doc->SizeItem(nx, ny, currItem->ItemNr);
									QMatrix mp;
									mp.translate(currItem->xPos() - Doc->minCanvasCoordinate.x(), currItem->yPos() - Doc->minCanvasCoordinate.y());
									mp.rotate(currItem->rotation());
									np2 = QPoint(qRound(nx), qRound(ny));
									QMatrix pm;
									Transform(currItem, pm);
									PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), np2)));
								}
								else
								{
									double rba = currItem->rotation();
									currItem->setRotation(0.0);
									QMatrix mp;
									Transform(currItem, mp);
									mp.translate(-Doc->minCanvasCoordinate.x(), -Doc->minCanvasCoordinate.y());
									np = QPoint(m->x(), m->y()) * mp.inverted();
									double sizeItemX=np.x(), sizeItemY=np.y();
									//Constrain rotation angle, when the mouse is moving the non-origin point of a line
									if (m->state() & Qt::ControlButton)
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
									pm.translate(-Doc->minCanvasCoordinate.x() * Scale,-Doc->minCanvasCoordinate.y() * Scale);
									Transform(currItem, pm);
									redrawPolygon.clear();
									redrawPolygon << pm.map(QPoint(0, 0)) << pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())));
									updateContents(QRect(pm.map(QPoint(0, 0)), pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())))).normalized().adjusted(-10, -10, 20, 20));
								}
								break;
							case 2:
								if (currItem->asLine())
								{
									double sav = Doc->SnapGuides;
									npf2 = FPoint(newX-Mxp, newY-Myp);
									//Constrain rotation on left point move, disabled for now in movesizeitem
									erf = Doc->MoveSizeItem(npf2, FPoint(0, 0), currItem->ItemNr, false, (m->state() & Qt::ControlButton));
									Doc->SnapGuides = sav;
									if (sav)
										currItem->Sizing = true;
									QMatrix pm;
									pm.translate(-Doc->minCanvasCoordinate.x() * Scale,-Doc->minCanvasCoordinate.y() * Scale);
									Transform(currItem, pm);
									redrawPolygon.clear();
									redrawPolygon << pm.map(QPoint(0, 0)) << pm.map(QPoint(qRound(currItem->width()), qRound(currItem->height())));
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
									mp.translate(currItem->xPos() - Doc->minCanvasCoordinate.x(), currItem->yPos() - Doc->minCanvasCoordinate.y());
									mp.rotate(currItem->rotation());
									np2 = QPoint(qRound(nx), qRound(ny)) * mp.inverted();
									QMatrix pm;
									Transform(currItem, pm);
									PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(qRound(currItem->width()), qRound(currItem->height())))));
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
							mp.translate(currItem->xPos() - Doc->minCanvasCoordinate.x(), currItem->yPos() - Doc->minCanvasCoordinate.y());
							mp.rotate(currItem->rotation());
							np2 = QPoint(qRound(nx), qRound(ny)) * mp.inverted();
							QMatrix pm;
							Transform(currItem, pm);
							switch (frameResizeHandle)
							{
							case 0:
								break;
							case 3:
								PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(0, qRound(currItem->height())))));
								break;
							case 4:
								PaintSizeRect(pm.mapToPolygon(QRect(np2, QPoint(qRound(currItem->width()), 0))));
								break;
							case 5:
								PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), QPoint(qRound(currItem->width()), np2.y()))));
								break;
							case 6:
								PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, 0), QPoint(np2.x(), qRound(currItem->height())))));
								break;
							case 7:
								PaintSizeRect(pm.mapToPolygon(QRect(QPoint(np2.x(), 0), QPoint(qRound(currItem->width()), qRound(currItem->height())))));
								break;
							case 8:
								PaintSizeRect(pm.mapToPolygon(QRect(QPoint(0, np2.y()), QPoint(qRound(currItem->width()), qRound(currItem->height())))));
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
				operItemMoving = true;
				erf = false;
				int dX=newX-Mxp, dY=newY-Myp;
				if (!Doc->m_Selection->isMultipleSelection())
				{
					erf=true;
					currItem = Doc->m_Selection->itemAt(0);
					//Control Alt drag image in frame without being in edit mode
					if ((currItem->asImageFrame()) && (m->state() & Qt::ControlButton) && (m->state() & Qt::AltButton))
					{
						currItem->moveImageInFrame(dX/currItem->imageXScale(),dY/currItem->imageYScale());
						updateContents(currItem->getRedrawBounding(Scale));
						
					}
					else
					{
						//Dragging orthogonally - Ctrl Drag
						if ((m->state() & Qt::ControlButton) && !(m->state() & Qt::ShiftButton) && !(m->state() & Qt::AltButton))
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
								if ((fabs(gx - gxo) < (Doc->guidesSettings.guideRad / 2.0) / Scale) && (fabs(gy - gyo) < (Doc->guidesSettings.guideRad / 2.0) / Scale))
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
					if (m->state() & Qt::ControlButton)
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
						if ((fabs(gx - gxo) < (Doc->guidesSettings.guideRad / 2.0) / Scale) && (fabs(gy - gyo) < (Doc->guidesSettings.guideRad / 2.0) / Scale))
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
		if ((!m_MouseButtonPressed) && (Doc->appMode != modeDrawBezierLine))
		{
			if (Doc->m_Selection->isMultipleSelection())
			{
				QRect mpo = QRect(qRound(m->x()/Scale)-Doc->guidesSettings.grabRad, qRound(m->y()/Scale)-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
				mpo.moveBy(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
				double gx, gy, gh, gw;
				Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					&& ((Doc->appMode == modeNormal) || (Doc->appMode == modeRotation)))
				{
					int how = 0;
					gx -= Doc->minCanvasCoordinate.x();
					gy -= Doc->minCanvasCoordinate.y();
					QMap<double,int> distance;
					double d1 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
					if (d1 < Doc->guidesSettings.grabRad)
						distance.insert(d1, 1);
					double d2 = sqrt(pow((gx * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
					if (d2 < Doc->guidesSettings.grabRad)
						distance.insert(d2, 2);
					double d3 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
					if (d3 < Doc->guidesSettings.grabRad)
						distance.insert(d3, 3);
					double d4 = sqrt(pow((gx * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
					if (d4 < Doc->guidesSettings.grabRad)
						distance.insert(d4, 4);
					double d5 = sqrt(pow(((gx+gw/2) * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
					if (d5 < Doc->guidesSettings.grabRad)
						distance.insert(d5, 5);
					double d6 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow(((gy+gh/2) * Scale) - m->y(),2));
					if (d6 < Doc->guidesSettings.grabRad)
						distance.insert(d6, 6);
					double d7 = sqrt(pow((gx * Scale) - m->x(),2)+pow(((gy+gh/2) * Scale) - m->y(),2));
					if (d7 < Doc->guidesSettings.grabRad)
						distance.insert(d7, 7);
					double d8 = sqrt(pow(((gx+gw/2) * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
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
			for (a = 0; a < Doc->m_Selection->count(); ++a)
			{
				currItem = Doc->m_Selection->itemAt(a);
				if (currItem->locked())
					break;
				QMatrix p;
				Transform(currItem, p);
				QRect mpo = QRect(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
				mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
				if (Doc->EditClip)
				{
					FPointArray Clip;
					Q3PointArray cli;
					ClRe2 = -1;
					SegP1 = -1;
					SegP2 = -1;
					if (EditContour)
						Clip = currItem->ContourLine;
					else
						Clip = currItem->PoLine;
					if ((Doc->EditClipMode == 2) || (Doc->EditClipMode == 0) || (Doc->EditClipMode == 3))
					{
						for (a=0; a<Clip.size(); ++a)
						{
							if (((EdPoints) && (a % 2 != 0)) || ((!EdPoints) && (a % 2 == 0)))
								continue;
							np = p.map(Clip.pointQ(a));
							tx = QRect(np.x()-3, np.y()-3, 6, 6);
							if (tx.intersects(mpo))
							{
								if (Doc->EditClipMode == 0)
									qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
								if (Doc->EditClipMode == 2)
									qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png"), 1, 1));
								if (Doc->EditClipMode == 3)
									qApp->changeOverrideCursor(QCursor(loadIcon("Split.png"), 1, 1));
								return;
							}
						}
						qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
					}
					if ((Doc->EditClipMode == 1) || (Doc->EditClipMode == 0) || (Doc->EditClipMode == 3) && (EdPoints))
					{
						for (uint poi=0; poi<Clip.size()-3; poi += 4)
						{
							BezierPoints(&Bez, Clip.pointQ(poi), Clip.pointQ(poi+1), Clip.pointQ(poi+3), Clip.pointQ(poi+2));
							cli = Bez.cubicBezier();
							for (int clp = 0; clp < cli.size()-1; ++clp)
							{
								if (PointOnLine(cli.point(clp), cli.point(clp+1), p.inverted().map(mpo)))
								{
									if (Doc->EditClipMode == 0)
										qApp->changeOverrideCursor(QCursor(loadIcon("HandC.xpm")));
									if (Doc->EditClipMode == 1)
										qApp->changeOverrideCursor(QCursor(loadIcon("AddPoint.png"), 1, 1));
									if (Doc->EditClipMode == 3)
										qApp->changeOverrideCursor(QCursor(loadIcon("Split.png"), 1, 1));
									ClRe2 = poi;
									return;
								}
							}
							qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
						}
					}
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
		if ((m_MouseButtonPressed) && (m->state() & Qt::LeftButton) && (GyM == -1) && (GxM == -1))
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
		if ((Doc->guidesSettings.guidesShown) && (Doc->appMode == modeNormal) && (!Doc->GuideLock) && (Doc->OnPage(m->x()/sc, m->y()/sc) != -1) && (!GetItem(&currItem)))
		{
			Guides::iterator it;
			Guides tmpGuides = Doc->currentPage()->guides.horizontals(GuideManagerCore::Standard);
			for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it)
			{
				if (((*it)+Doc->currentPage()->yOffset()- Doc->minCanvasCoordinate.y() < ((m->y()+Doc->guidesSettings.grabRad) / sc)) &&
								   ((*it)+Doc->currentPage()->yOffset()- Doc->minCanvasCoordinate.y() > ((m->y()-Doc->guidesSettings.grabRad) / sc)))
				{
					if ((m_MouseButtonPressed) && (GyM != -1))
						MoveGY = true;
					if (((m->x()/sc) < Doc->currentPage()->xOffset()- Doc->minCanvasCoordinate.x()) || ((m->x()/sc) >= Doc->currentPage()->width()-1+Doc->currentPage()->xOffset()- Doc->minCanvasCoordinate.x()))
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
				if (((*it)+Doc->currentPage()->xOffset()- Doc->minCanvasCoordinate.x() < ((m->x()+Doc->guidesSettings.grabRad) / sc)) &&
								   ((*it)+Doc->currentPage()->xOffset()- Doc->minCanvasCoordinate.x() > ((m->x()-Doc->guidesSettings.grabRad) / sc)))
				{
					if ((m_MouseButtonPressed) && (GxM != -1))
						MoveGX = true;
					if (((m->y()/sc) < Doc->currentPage()->yOffset()- Doc->minCanvasCoordinate.x()) || ((m->y()/sc) >= Doc->currentPage()->height()-1+Doc->currentPage()->yOffset()- Doc->minCanvasCoordinate.y()))
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

void ScribusView::contentsMousePressEvent(QMouseEvent *m)
{
	m_SnapCounter = 0;
	bool inText;
	uint a;
	int z;
	double Rxp = 0;
	double Ryp = 0;
	double Rxpd = 0;
	double Rypd = 0;
	PageItem *currItem;
	PageItem *bb;
	QPainter p;
	PaintSizeRect(QRect());
	FPoint npf, npf2;
	Q3PointArray Bez(4);
	QRect tx;
	QMatrix pm;
	m_MouseButtonPressed = true;
	operItemMoving = false;
	HaveSelRect = false;
	Doc->DragP = false;
	Doc->leaveDrag = false;
	inItemCreation = false;
	oldClip = 0;
	m->accept();
	moveTimer.start();
	dragTimerFired = false;
	Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
	Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
	QRect mpo(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
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
			Mxp = qRound(m->x()/Scale);
			Myp = qRound(m->y()/Scale);
			SeRx = Mxp;
			SeRy = Myp;
			if ((Doc->EditClip) && (Doc->m_Selection->count() != 0))
			{
				currItem = Doc->m_Selection->itemAt(0);
				FPointArray Clip;
				bool edited = false;
				bool pfound = false;
				QString uAction;
				if (EditContour)
				{
					Clip = currItem->ContourLine;
					isContourLine = true;
					uAction = Um::EditContour;
				}
				else
				{
					Clip = currItem->PoLine;
					isContourLine = false;
					uAction = Um::EditShape;
				}
				oldClip = new FPointArray(Clip);
				oldItemX = currItem->xPos();
				oldItemY = currItem->yPos();
				undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(), uAction);
				QMatrix pm;
				pm.translate(-Doc->minCanvasCoordinate.x()*Scale, -Doc->minCanvasCoordinate.y()*Scale);
				Transform(currItem, pm);
				npf2 = FPoint(m->pos() * pm.inverted());
				QMatrix pm2;
				Transform(currItem, pm2);
				ClRe = -1;
				for (a=0; a<Clip.size(); ++a)
				{
					if (((EdPoints) && (a % 2 != 0)) || ((!EdPoints) && (a % 2 == 0)))
						continue;
					npf = FPoint(Clip.pointQ(a) * pm2);
					tx = QRect(static_cast<int>(npf.x()-3), static_cast<int>(npf.y()-3), 6, 6);
					if (tx.intersects(mpo))
					{
						ClRe = a;
						if ((EdPoints) && (SelNode.contains(a) == 0))
						{
							if (m->state() == Qt::ShiftButton)
								SelNode.append(a);
							else
							{
								SelNode.clear();
								SelNode.append(a);
							}
						}
						emit ClipPo(Clip.point(a).x(), Clip.point(a).y());
						pfound = true;
						break;
					}
				}
				if ((!pfound) || (!EdPoints))
					SelNode.clear();
				if ((Doc->EditClipMode == 0) && (ClRe2 != -1) && (ClRe == -1))
				{
					SegP1 = ClRe2;
					SegP2 = ClRe2+2;
					ClRe = ClRe2;
				}
				FPointArray cli;
				uint EndInd = Clip.size();
				uint StartInd = 0;
				for (uint n = ClRe; n < Clip.size(); ++n)
				{
					if (Clip.point(n).x() > 900000)
					{
						EndInd = n;
						break;
					}
				}
				if (ClRe > 0)
				{
					for (uint n2 = ClRe; n2 > 0; n2--)
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
				if (Doc->EditClipMode == 3)
				{
					if (!EdPoints)
						return;
					if (ClRe == -1)	// We don't have a Point, try to add one onto the current curve segment
					{
						bool foundP = false;
						Q3PointArray Bez(4);
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
							BezierPoints(&Bez, Clip.pointQ(poi), Clip.pointQ(poi+1), Clip.pointQ(poi+3), Clip.pointQ(poi+2));
							Q3PointArray cli2 = Bez.cubicBezier();
							for (int clp = 0; clp < cli2.size()-1; ++clp)
							{
								if (PointOnLine(cli2.point(clp), cli2.point(clp+1), mpo2))
								{
									seg = poi;
									double sp = 0.0;
									double spadd = 1.0 / (Clip.lenPathSeg(seg) * Scale);
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
						cli.putPoints(0, ClRe2+2, Clip);
						if (foundP)
						{
							npf2 = nearPoint;
							FPoint base = cli.point(cli.size()-2);
							FPoint c1 = cli.point(cli.size()-1);
							FPoint base2 =  Clip.point(ClRe2+2);
							FPoint c2 = Clip.point(ClRe2+3);
							if ((base == c1) && (base2 == c2))
							{
								cli.resize(cli.size()+4);
								cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
								cli.putPoints(cli.size(), Clip.size()-(ClRe2 + 2), Clip, ClRe2+2);
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
								cli.putPoints(cli.size(), Clip.size()-(ClRe2 + 2), Clip, ClRe2+2);
								cli.setPoint(basind, cn4);
							}
							Clip = cli.copy();
							cli.resize(0);
							ClRe = ClRe2+2;
							ClRe2 = -1;
							EndInd = Clip.size();
							StartInd = 0;
							for (uint n = ClRe; n < Clip.size(); ++n)
							{
								if (Clip.point(n).x() > 900000)
								{
									EndInd = n;
									break;
								}
							}
							if (ClRe > 0)
							{
								for (uint n2 = ClRe; n2 > 0; n2--)
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
							ClRe = -1;
					}
					if (ClRe != -1)
					{
						if (currItem->asPolygon())
						{
							if ((ClRe != 0) && (ClRe != static_cast<int>(EndInd-2)))
							{
								if (currItem->Segments.count() == 0)
								{
									cli.putPoints(0, EndInd-(ClRe+2), Clip, ClRe+2);
									cli.putPoints(cli.size(), ClRe+2, Clip);
								}
								else
								{
									cli.putPoints(0, EndInd-StartInd, Clip, StartInd);
									z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor(), true);
									bb = Doc->Items->at(z);
									if (EditContour)
										bb->ContourLine.resize(0);
									else
										bb->PoLine.resize(0);
									if (StartInd != 0)
									{
										if (EditContour)
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
										if (EditContour)
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
							ClRe = -1;
							currItem->ClipEdited = true;
							edited = true;
							Doc->EditClipMode = 0;
							PageItem* newItem=Doc->convertItemTo(currItem, PageItem::PolyLine);
							currItem=newItem;
							emit PolyOpen();
						}
						else
						{
							if ((currItem->asPolyLine()) || (currItem->asPathText()))
							{
								if ((ClRe > 1) && (ClRe < static_cast<int>(Clip.size()-2)))
								{
									z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->fillColor(), currItem->lineColor(), true);
									bb = Doc->Items->at(z);
									if (EditContour)
										bb->ContourLine.putPoints(0, Clip.size()-(ClRe+2), Clip, ClRe+2);
									else
										bb->PoLine.putPoints(0, Clip.size()-(ClRe+2), Clip, ClRe+2);
									bb->setRotation(currItem->rotation());
									Doc->AdjustItemSize(bb);
									bb->ClipEdited = true;
									cli.resize(0);
									cli.putPoints(0, ClRe+2, Clip);
									currItem->PoLine = cli.copy();
								}
								ClRe = -1;
								currItem->ClipEdited = true;
								edited = true;
								Doc->EditClipMode = 0;
								currItem->setPolyClip(qRound(qMax(currItem->lineWidth() / 2.0, 1.0)));
								emit PolyOpen();
							}
						}
					}
				}
				if ((Doc->EditClipMode == 2) && (ClRe != -1))
				{
					if (!EdPoints)
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
						if (ClRe == static_cast<int>(StartInd))
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
							if (ClRe != 0)
								cli.putPoints(0, ClRe, Clip);
							cli.putPoints(cli.size(), Clip.size()-(ClRe + 4), Clip, ClRe+4);
						}
					}
					if (EditContour)
						currItem->ContourLine = cli.copy();
					else
						currItem->PoLine = cli.copy();
					ClRe = -1;
					currItem->ClipEdited = true;
					edited = true;
				}
				if ((Doc->EditClipMode == 1) && (ClRe2 != -1))
				{
					bool foundP = false;
					Q3PointArray Bez(4);
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
						BezierPoints(&Bez, Clip.pointQ(poi), Clip.pointQ(poi+1), Clip.pointQ(poi+3), Clip.pointQ(poi+2));
						Q3PointArray cli2 = Bez.cubicBezier();
						for (int clp = 0; clp < cli2.size()-1; ++clp)
						{
							if (PointOnLine(cli2.point(clp), cli2.point(clp+1), mpo2))
							{
								seg = poi;
								double sp = 0.0;
								double spadd = 1.0 / (Clip.lenPathSeg(seg) * Scale);
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
					cli.putPoints(0, ClRe2+2, Clip);
					if (foundP)
					{
						npf2 = nearPoint;
						FPoint base = cli.point(cli.size()-2);
						FPoint c1 = cli.point(cli.size()-1);
						FPoint base2 =  Clip.point(ClRe2+2);
						FPoint c2 = Clip.point(ClRe2+3);
						if ((base == c1) && (base2 == c2))
						{
							cli.resize(cli.size()+4);
							cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
							cli.putPoints(cli.size(), Clip.size()-(ClRe2 + 2), Clip, ClRe2+2);
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
							cli.putPoints(cli.size(), Clip.size()-(ClRe2 + 2), Clip, ClRe2+2);
							cli.setPoint(basind, cn4);
						}
					}
					else
					{
						cli.resize(cli.size()+4);
						cli.putPoints(cli.size()-4, 4, npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y(), npf2.x(), npf2.y());
						cli.putPoints(cli.size(), Clip.size()-(ClRe2 + 2), Clip, ClRe2+2);
					}
					if (EditContour)
						currItem->ContourLine = cli.copy();
					else
						currItem->PoLine = cli.copy();
					ClRe2 = -1;
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
				if ((SelNode.count() != 0) || ((SegP1 != -1) && (SegP2 != -1)) || ((ClRe != -1) && (!EdPoints)))
				{
					Mxp = m->x();
					Myp = m->y();
					specialRendering = true;
					firstSpecial = true;
				}
				else
				{
					redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
					redrawMarker->show();
					Mxp = m->globalPos().x();
					Myp = m->globalPos().y();
					Dxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
					Dyp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
				}
				return;
			}
			if (GetItem(&currItem))
			{
				if (Doc->m_Selection->isMultipleSelection())
				{
					PaintSizeRect(QRect());
					double gx, gy, gh, gw;
					bool shiftSel = true;
					Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					dragConstrainInitPtX = qRound(gx);
					dragConstrainInitPtY = qRound(gy);
					mpo = QRect(qRound(m->x() / Scale) - Doc->guidesSettings.grabRad, qRound(m->y() / Scale) - Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
					mpo.moveBy(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
					if ((QRect(static_cast<int>(gx), static_cast<int>(gy), static_cast<int>(gw), static_cast<int>(gh)).intersects(mpo))
					      && (m->state() != (Qt::ControlButton | Qt::AltButton)) && (m->state() != Qt::ShiftButton))
					{
						frameResizeHandle = 0;
						QMap<double,int> distance;
						gx -= Doc->minCanvasCoordinate.x();
						gy -= Doc->minCanvasCoordinate.y();
						double d1 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
						if (d1 < Doc->guidesSettings.grabRad)
							distance.insert(d1, 1);
						double d2 = sqrt(pow((gx * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
						if (d2 < Doc->guidesSettings.grabRad)
							distance.insert(d2, 2);
						double d3 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
						if (d3 < Doc->guidesSettings.grabRad)
							distance.insert(d3, 3);
						double d4 = sqrt(pow((gx * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
						if (d4 < Doc->guidesSettings.grabRad)
							distance.insert(d4, 4);
						double d5 = sqrt(pow(((gx+gw/2) * Scale) - m->x(),2)+pow(((gy+gh) * Scale) - m->y(),2));
						if (d5 < Doc->guidesSettings.grabRad)
							distance.insert(d5, 5);
						double d6 = sqrt(pow(((gx+gw) * Scale) - m->x(),2)+pow(((gy+gh/2) * Scale) - m->y(),2));
						if (d6 < Doc->guidesSettings.grabRad)
							distance.insert(d6, 6);
						double d7 = sqrt(pow((gx * Scale) - m->x(),2)+pow(((gy+gh/2) * Scale) - m->y(),2));
						if (d7 < Doc->guidesSettings.grabRad)
							distance.insert(d7, 7);
						double d8 = sqrt(pow(((gx+gw/2) * Scale) - m->x(),2)+pow((gy * Scale) - m->y(),2));
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
							operItemResizing = true;
					}
					else
						shiftSel = SeleItem(m);
					if (((Doc->m_Selection->count() == 0) || (!shiftSel)) && (m->state() == Qt::ShiftButton))
					{
						shiftSelItems = true;
						Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
						Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
						SeRx = Mxp;
						SeRy = Myp;
					}
					else
						shiftSelItems = false;
					specialRendering = true;
					firstSpecial = true;
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
								operItemResizing = true;
							}
						}
					}
					else
					{
						Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
						Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
						SeRx = Mxp;
						SeRy = Myp;
					}
				}
				specialRendering = true;
				firstSpecial = true;
			}
			else
			{
				SeleItem(m);
				if (Doc->m_Selection->count() == 0)
				{
					Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
					Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
					SeRx = Mxp;
					SeRy = Myp;
					redrawMarker->setGeometry(m->globalPos().x(), m->globalPos().y(), 1, 1);
					redrawMarker->show();
					Mxp = m->globalPos().x();
					Myp = m->globalPos().y();
					Dxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
					Dyp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
				}
				else
				{
					specialRendering = true;
					firstSpecial = true;
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
				m_MouseButtonPressed = true;
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
				if (m->state() == Qt::ShiftButton)
				{
					z = Doc->itemAddArea(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
					Doc->Items->at(z)->FrameType = 0;
					SetupDrawNoResize(z);
				}
				else
				{
					Doc->ApplyGuides(&Rxp, &Ryp);
					z = Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
					Doc->Items->at(z)->FrameType = 0;
					SetupDraw(z);
				}
				break;
			case 1:
				if (m->state() == Qt::ShiftButton)
				{
					z = Doc->itemAddArea(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
					Doc->Items->at(z)->FrameType = 1;
					SetupDrawNoResize(z);
				}
				else
				{
					Doc->ApplyGuides(&Rxp, &Ryp);
					z = Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
					Doc->Items->at(z)->FrameType = 1;
					SetupDraw(z);
				}
				break;
			default:
				if (m->state() == Qt::ShiftButton)
				{
					z = Doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
					Doc->Items->at(z)->SetFrameShape(Doc->ValCount, Doc->ShapeValues);
					Doc->setRedrawBounding(Doc->Items->at(z));
					Doc->Items->at(z)->FrameType = Doc->SubMode+2;
					SetupDrawNoResize(z);
				}
				else
				{
					Doc->ApplyGuides(&Rxp, &Ryp);
					z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
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
			if (m->state() == Qt::ShiftButton)
			{
				z = Doc->itemAddArea(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}
			else
			{
				Doc->ApplyGuides(&Rxp, &Ryp);
				z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeDrawText:
			if (m->button() != Qt::LeftButton)
				break;
			selectPage(m);
			if (m->state() == Qt::ShiftButton)
			{
				z = Doc->itemAddArea(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_MouseButtonPressed);
				SetupDrawNoResize(z);
			}	
			else
			{
				Doc->ApplyGuides(&Rxp, &Ryp);
				z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_MouseButtonPressed);
				SetupDraw(z);
			}
			break;
		case modeMagnifier:
			m_MouseButtonPressed = true;
			if ((m->state() == Qt::ShiftButton) || (m->button() == Qt::RightButton))
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
				operItemResizeInEditMode = false;
				int oldP=0;
				if (GetItem(&currItem))
				{
					slotDoCurs(false);
					if (!currItem->locked())
					{
						HandleSizer(currItem, mpo, m);
						if (frameResizeHandle != 0)
						{
							operItemResizeInEditMode = true;
							slotDoCurs(true);
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
				if (m->state() & Qt::ShiftButton)
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
						m_MouseButtonPressed = false;
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
			z = Doc->itemAdd(PageItem::Line, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, Rypd, Doc->toolSettings.dWidthLine, CommonStrings::None, Doc->toolSettings.dPenLine, !m_MouseButtonPressed);
			currItem = Doc->Items->at(z);
			qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
			Doc->m_Selection->clear();
			Doc->m_Selection->addItem(currItem);
			updateContents(currItem->getRedrawBounding(Scale));
			operItemMoving = true;
			inItemCreation = true;
			operItemResizing = false;
			specialRendering = true;
			firstSpecial = true;
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
					oldW = xy2Deg(m->x()/Scale - RCenter.x(), m->y()/Scale - RCenter.y());
				}
				else
				{
					QMatrix mat;
					Transform(currItem, mat);
					Doc->RotMode = 2;
//					RCenter = FPoint(currItem->xPos()+currItem->width()/2, currItem->yPos()+currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, true);
					RCenter = FPoint(currItem->width()/2, currItem->height()/2, 0, 0, currItem->rotation(), 1, 1, false);
//					if (!currItem->asLine())
//					{
						if (QRegion(mat.map(Q3PointArray(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo))
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
					oldW = xy2Deg(m->x()/Scale - RCenter.x(), m->y()/Scale - RCenter.y());
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
					m_MouseButtonPressed = false;
					QMessageBox::warning(this, tr("Linking Text Frames"),
											 "<qt>" + tr("You are trying to link a frame to itself.") + "</qt>");
				}
				else
				{
					//CB Mouse is released when this messagebox takes focus
					m_MouseButtonPressed = false;
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
				if (m->state() == Qt::ShiftButton)
					z = Doc->itemAddArea(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
				else
				{
					Doc->ApplyGuides(&Rxp, &Ryp);
					z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, Doc->toolSettings.dBrush, Doc->toolSettings.dPen, !m_MouseButtonPressed);
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
				updateContents(currItem->getRedrawBounding(Scale));
				inItemCreation = true;
				operItemResizing = true;
				if (m->state() == Qt::ShiftButton)
				{
					Doc->appMode = modeNormal;
					emit DocChanged();
					currItem->Sizing =  currItem->asLine() ? false : true;
					inItemCreation = false;
					updateContents();
				}
				else
				{
					operItemMoving = true;
					specialRendering = true;
					firstSpecial = true;
				}
				break;
			}
		case modeDrawBezierLine:
			if (m->button() == Qt::RightButton)
				break;
			if (FirstPoly)
			{
				selectPage(m);
				z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenLine, !m_MouseButtonPressed);
				currItem = Doc->Items->at(z);
				Doc->m_Selection->clear();
				Doc->m_Selection->addItem(currItem);
				qApp->changeOverrideCursor(QCursor(Qt::CrossCursor));
				specialRendering = true;
				firstSpecial = true;
			}
			currItem = Doc->m_Selection->itemAt(0);
			pm.translate(-Doc->minCanvasCoordinate.x()*Scale, -Doc->minCanvasCoordinate.y()*Scale);
			Transform(currItem, pm);
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
			redrawPolygon.clear();
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
			z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, Rxp, Ryp, 1+Rxpd, 1+Rypd, Doc->toolSettings.dWidth, CommonStrings::None, Doc->toolSettings.dPenText, !m_MouseButtonPressed);
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
			Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
			Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
			SeRx = Mxp;
			SeRy = Myp;
			specialRendering = true;
			firstSpecial = true;
			break;
		case modeDrawTable:
			if (m->button() != Qt::LeftButton)
				break;
			Deselect(false);
//			Mxp = qRound(m->x()/Scale + Doc->minCanvasCoordinate.x());
//			Myp = qRound(m->y()/Scale + Doc->minCanvasCoordinate.y());
			Rxp = m->x()/Scale + Doc->minCanvasCoordinate.x();
			Ryp = m->y()/Scale + Doc->minCanvasCoordinate.y();
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
			specialRendering = true;
			firstSpecial = true;
		case modeEditGradientVectors:
			if (m->button() != Qt::LeftButton)
				break;
			m_MouseButtonPressed = true;
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

void ScribusView::getDragRectScreen(double *x, double *y, double *w, double *h)
{
	QPoint in(qRound(dragX*Scale), qRound(dragY*Scale));
	in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	QPoint out = contentsToViewport(in);
	*x = static_cast<double>(out.x());
	*y = static_cast<double>(out.y());
	*w = dragW*Scale;
	*h = dragH*Scale;
}

void ScribusView::getGroupRectScreen(double *x, double *y, double *w, double *h)
{
	double gx, gy, gh, gw;
	Doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
	QPoint in(qRound(gx*Scale), qRound(gy*Scale));
	in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	QPoint out = contentsToViewport(in);
	*x = static_cast<double>(out.x());
	*y = static_cast<double>(out.y());
	*w = gw*Scale;
	*h = gh*Scale;
}

void ScribusView::paintGroupRect(bool norm)
{
	double x, y, w, h;
	getGroupRectScreen(&x, &y, &w, &h);
	QPainter pgc;
	pgc.begin(viewport());
	pgc.setPen(QPen((norm ? Qt::red : Qt::black), 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
	pgc.setBrush(Qt::NoBrush);
	pgc.drawRect(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h));
	pgc.setBrush(Qt::red);
	if (norm)
	{
		pgc.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		pgc.drawRect(qRound(x+w-6), qRound(y+h-6), 6, 6);
		pgc.drawRect(qRound(x+w/2 - 3), qRound(y+h-6), 6, 6);
		pgc.drawRect(qRound(x+w/2 - 3), qRound(y), 6, 6);
		pgc.drawRect(qRound(x+w-6), qRound(y+h/2 - 3), 6, 6);
		pgc.drawRect(qRound(x+w-6), qRound(y), 6, 6);
		pgc.drawRect(qRound(x), qRound(y), 6, 6);
		pgc.drawRect(qRound(x), qRound(y+h/2 - 3), 6, 6);
		pgc.drawRect(qRound(x), qRound(y+h-6), 6, 6);
	}
	pgc.end();
}

void ScribusView::PaintSizeRect(QRect newRect)
{
	static QRect oldRect;
	if (!newRect.isNull())
	{
		if (!oldRect.isNull())
			newRect.unite(oldRect);
		updateContents(newRect.adjusted(-10, -10, 20, 20));
	}
	oldRect = newRect;
}

void ScribusView::PaintSizeRect(QPolygon newRect)
{
	static QRect oldRect;
	QRect newR = newRect.boundingRect();
	if (!newR.isNull())
	{
		if (!oldRect.isNull())
			newR.unite(oldRect);
		newR.moveBy(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(-Doc->minCanvasCoordinate.y() * Scale));
		redrawPolygon = newRect;
		redrawPolygon.translate(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(-Doc->minCanvasCoordinate.y() * Scale));
		updateContents(newR.adjusted(-20, -20, 40, 40));
	}
	oldRect = newR;
}

void ScribusView::ToView(QPainter *p)
{
	QPoint out(contentsToViewport(QPoint(0, 0)));
	p->translate(qRound(-Doc->minCanvasCoordinate.x()*Scale), qRound(-Doc->minCanvasCoordinate.y()*Scale));
	p->translate(out.x(), out.y());
}

void ScribusView::ToView(QMatrix& m)
{
	QPoint out(contentsToViewport(QPoint(0, 0)));
	m.translate(qRound(-Doc->minCanvasCoordinate.x()*Scale), qRound(-Doc->minCanvasCoordinate.y()*Scale));
	m.translate(out.x(), out.y());
}

void ScribusView::RefreshItem(PageItem *currItem)
{
	updateContents(currItem->getRedrawBounding(Scale));
}

void ScribusView::RefreshGradient(PageItem *currItem, double dx, double dy)
{
	QMatrix matrix;
	QRect rect = currItem->getRedrawBounding(Scale);
	Transform(currItem, matrix);
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

void ScribusView::MarkClip(QPainter *p, PageItem *currItem, FPointArray cli, bool)
{
	double x, y;
	Q3PointArray Bez(4);
	p->save();
	p->resetMatrix();
	QPoint out = contentsToViewport(QPoint(0, 0));
	p->translate(out.x(), out.y());
	p->translate(-qRound(Doc->minCanvasCoordinate.x()*Scale), -qRound(Doc->minCanvasCoordinate.y()*Scale));
	Transform(currItem, p);
	p->setPen(QPen(Qt::blue, 1 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p->setBrush(Qt::NoBrush);
	if ((EditContour) && (currItem->ContourLine.size() != 0))
		cli = currItem->ContourLine;
	else
		cli = currItem->PoLine;
	if (cli.size() > 3)
	{
		for (uint poi=0; poi<cli.size()-3; poi += 4)
		{
			if (cli.point(poi).x() > 900000)
				continue;
			p->setPen(QPen(Qt::blue, 1 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			BezierPoints(&Bez, QPoint(qRound(cli.point(poi).x()), qRound(cli.point(poi).y())),
												QPoint(qRound(cli.point(poi+1).x()), qRound(cli.point(poi+1).y())),
												QPoint(qRound(cli.point(poi+3).x()), qRound(cli.point(poi+3).y())),
												QPoint(qRound(cli.point(poi+2).x()), qRound(cli.point(poi+2).y())));
			p->drawCubicBezier(Bez);
			p->setPen(QPen(Qt::blue, 1 / Scale, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
			p->drawLine(Bez.point(0), Bez.point(1));
			p->drawLine(Bez.point(2), Bez.point(3));
		}
	}
	for (uint a=0; a<cli.size()-1; a += 2)
	{
		if (cli.point(a).x() > 900000)
			continue;
		if (EdPoints)
		{
			if (ClRe == static_cast<int>(a+1))
				p->setPen(QPen(Qt::red, 8 / Scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::magenta, 8 / Scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a+1, &x, &y);
			p->drawPoint(QPointF(x, y));
			if (ClRe == static_cast<int>(a))
				p->setPen(QPen(Qt::red, 8 / Scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::blue, 8 / Scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a, &x, &y);
			p->drawPoint(QPointF(x, y));
		}
		else
		{
			if (ClRe == static_cast<int>(a))
				p->setPen(QPen(Qt::red, 8 / Scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::blue, 8 / Scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a, &x, &y);
			p->drawPoint(QPointF(x, y));
			if (ClRe == static_cast<int>(a+1))
				p->setPen(QPen(Qt::red, 8 / Scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			else
				p->setPen(QPen(Qt::magenta, 8 / Scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
			cli.point(a+1, &x, &y);
			p->drawPoint(QPointF(x, y));
		}
	}
	if (ClRe != -1)
	{
		p->setPen(QPen(Qt::red, 8 / Scale, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
		cli.point(ClRe, &x, &y);
		p->drawPoint(QPointF(x, y));
		QList<int>::Iterator itm;
		for (itm = SelNode.begin(); itm != SelNode.end(); ++itm)
		{
			cli.point((*itm), &x, &y);
			p->drawPoint(QPointF(x, y));
		}
		emit HavePoint(true, MoveSym);
	}
	else
		emit HavePoint(false, MoveSym);
	p->restore();
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
	if (EditContour)
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
		updateContents(currItem->getRedrawBounding(Scale));
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
	// do no record anything else but the core reset points action
	undoManager->setUndoEnabled(false);

	PageItem *currItem = Doc->m_Selection->itemAt(0);

	oldClip = new FPointArray(EditContour ? currItem->ContourLine.copy() : currItem->PoLine.copy());
	oldItemX = currItem->xPos();
	oldItemY = currItem->yPos();
	isContourLine = EditContour;

	currItem->ClipEdited = true;
	FPoint np;
	if (EditContour)
		np = currItem->ContourLine.point(ClRe-1);
	else
		np = currItem->PoLine.point(ClRe-1);
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if (EditContour)
	{
		currItem->ContourLine.setPoint(ClRe, np);
		updateContents();
//		currItem->Tinput = true;
		currItem->FrameOnly = true;
		updateContents(currItem->getRedrawBounding(Scale));
	}
	else
	{
		currItem->PoLine.setPoint(ClRe, np);
		Doc->AdjustItemSize(currItem);
	}
	FPointArray cli;
	if ((EditContour) && (currItem->ContourLine.size() != 0))
		cli = currItem->ContourLine;
	else
		cli = currItem->PoLine;
//	MarkClip(currItem, cli, true);
	emit DocChanged();

	undoManager->setUndoEnabled(true);
	FPointArray newClip(isContourLine ? currItem->ContourLine : currItem->PoLine);
	if (*oldClip != newClip)
	{
		ItemState<QPair<FPointArray, FPointArray> > *state =
				new ItemState<QPair<FPointArray, FPointArray> >(Um::ResetControlPoint, "",
																currItem->getUPixmap());
		state->set("EDIT_SHAPE_OR_CONTOUR", "edit_shape_or_contour");
		state->set("IS_CONTOUR", EditContour);
		state->setItem(QPair<FPointArray, FPointArray>(*oldClip, newClip));
		state->set("OLD_X", oldItemX);
		state->set("OLD_Y", oldItemY);
		state->set("NEW_X", currItem->xPos());
		state->set("NEW_Y", currItem->yPos());
		undoManager->action(currItem, state);
	}
	delete oldClip;
	oldClip = 0;
}

void ScribusView::ResetControl()
{
	// do no record anything else but the core reset points action
	undoManager->setUndoEnabled(false);

	PageItem *currItem = Doc->m_Selection->itemAt(0);
	oldClip = new FPointArray(EditContour ? currItem->ContourLine.copy() : currItem->PoLine.copy());
	oldItemX = currItem->xPos();
	oldItemY = currItem->yPos();
	isContourLine = EditContour;

	currItem->ClipEdited = true;
	FPoint np;
	if (EditContour)
		np = currItem->ContourLine.point(ClRe);
	else
		np = currItem->PoLine.point(ClRe);

	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	if ((ClRe == 0) || (ClRe == static_cast<int>(currItem->PoLine.size()-2)))
	{
		if (EditContour)
			currItem->ContourLine.setPoint(ClRe+1, np);
		else
			currItem->PoLine.setPoint(ClRe+1, np);
	}
	else
	{
		if (EditContour)
		{
			currItem->ContourLine.setPoint(ClRe+1, np);
			currItem->ContourLine.setPoint((ClRe % 4 != 0 ? ClRe + 3 : ClRe - 1), np);
		}
		else
		{
			currItem->PoLine.setPoint(ClRe+1, np);
			currItem->PoLine.setPoint((ClRe % 4 != 0 ? ClRe + 3 : ClRe - 1), np);
		}
	}
	if (!EditContour)
		Doc->AdjustItemSize(currItem);
	else
	{
		updateContents();
//		currItem->Tinput = true;
//		currItem->FrameOnly = true;
//		updateContents(currItem->getRedrawBounding(Scale));
	}
	FPointArray cli;
	if ((EditContour) && (currItem->ContourLine.size() != 0))
		cli = currItem->ContourLine;
	else
		cli = currItem->PoLine;
//	MarkClip(currItem, cli, true);

	emit DocChanged();

	undoManager->setUndoEnabled(true);
	FPointArray newClip(isContourLine ? currItem->ContourLine : currItem->PoLine);
	if (*oldClip != newClip)
	{
		ItemState<QPair<FPointArray, FPointArray> > *state =
				new ItemState<QPair<FPointArray, FPointArray> >(Um::ResetControlPoints, "",
																currItem->getUPixmap());
		state->set("EDIT_SHAPE_OR_CONTOUR", "edit_shape_or_contour");
		state->set("IS_CONTOUR", EditContour);
		state->setItem(QPair<FPointArray, FPointArray>(*oldClip, newClip));
		state->set("OLD_X", oldItemX);
		state->set("OLD_Y", oldItemY);
		state->set("NEW_X", currItem->xPos());
		state->set("NEW_Y", currItem->yPos());
		undoManager->action(currItem, state);
	}
	delete oldClip;
	oldClip = 0;
}

//CB-->Doc
void ScribusView::MoveClipPoint(PageItem *currItem, FPoint ip)
{
	if (((EdPoints) && (ClRe % 2 != 0)) || ((!EdPoints) && (ClRe % 2 == 0)))
		return;
	double xposOrig = currItem->xPos();
	double yposOrig = currItem->yPos();
	currItem->ClipEdited = true;
	FPointArray Clip;
//	RefreshItem(currItem);
	if (EditContour)
	{
//		MarkClip(currItem, currItem->ContourLine);
		Clip = currItem->ContourLine.copy();
	}
	else
	{
//		MarkClip(currItem, currItem->PoLine);
		Clip = currItem->PoLine.copy();
	}
	currItem->FrameType = 3;
	uint EndInd = Clip.size();
	uint StartInd = 0;
	for (uint n = ClRe; n < Clip.size(); ++n)
	{
		if (Clip.point(n).x() > 900000)
		{
			EndInd = n;
			break;
		}
	}
	if (ClRe > 0)
	{
		for (uint n2 = ClRe; n2 > 0; n2--)
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
	FPoint np(ip);
	if (ClRe != -1)
	{
		if ((np.x() < 0) && (!EditContour))
		{
			Doc->SizeItem(currItem->width() - np.x(), currItem->height(), currItem->ItemNr, false, false, false);
			if (currItem->rotation() != 0)
			{
				FPoint npv(np.x(), 0);
				Doc->MoveRotated(currItem, npv);
			}
			else
				Doc->MoveItem(np.x(), 0, currItem);
			Clip.translate(-np.x(), 0);
			if (!currItem->imageFlippedH())
				currItem->moveImageInFrame(-np.x()/currItem->imageXScale(), 0);
			np.setX(0);
		}
		if ((np.y() < 0) && (!EditContour))
		{
			Doc->SizeItem(currItem->width(), currItem->height() - np.y(), currItem->ItemNr, false, false, false);
			if (currItem->rotation() != 0)
			{
				FPoint npv(0, np.y());
				Doc->MoveRotated(currItem, npv);
			}
			else
				Doc->MoveItem(0, np.y(), currItem);
			Clip.translate(0, -np.y());
			if (!currItem->imageFlippedV())
				currItem->moveImageInFrame(0, -np.y()/currItem->imageYScale());
			np.setY(0);
		}
		emit ClipPo(np.x(), np.y());
		if ((ClRe+1 < static_cast<int>(EndInd)) && (ClRe % 2 == 0))
		{
			FPoint ap(Clip.point(ClRe));
			FPoint ap2(Clip.point(ClRe+1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe+1, ap2);
		}
		Clip.setPoint(ClRe, np);
		if (((ClRe % 4 != 0) && (ClRe % 2 == 0)) && (ClRe+3 < static_cast<int>(EndInd)) && (ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(ClRe+2));
			FPoint ap2(Clip.point(ClRe+3));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe+3, ap2);
			Clip.setPoint(ClRe+2, np);
		}
		if ((ClRe % 4 == 0) && (ClRe+3 < static_cast<int>(EndInd)) && (ClRe != static_cast<int>(StartInd)))
		{
			FPoint ap(Clip.point(ClRe-2));
			FPoint ap2(Clip.point(ClRe-1));
			ap2.setX(ap2.x() - (ap.x() - np.x()));
			ap2.setY(ap2.y() - (ap.y() - np.y()));
			Clip.setPoint(ClRe-1, ap2);
			Clip.setPoint(ClRe-2, np);
		}
		/*
		{
			qDebug(QString("is a text %1").arg((currItem->asTextFrame()!=0)));
			qDebug(QString("is a image %1").arg((currItem->asImageFrame()!=0)));
			qDebug(QString("is a line %1").arg((currItem->asLine()!=0)));
			qDebug(QString("is a pathtext %1").arg((currItem->asPathText()!=0)));
			qDebug(QString("is a polygon %1").arg((currItem->asPolygon()!=0)));
			qDebug(QString("is a polyline %1").arg((currItem->asPolyLine()!=0)));

			qDebug(QString("item type is %1").arg(currItem->itemType()));
		}
		*/
		if (((ClRe == static_cast<int>(StartInd)) || (ClRe == static_cast<int>(EndInd-2))) &&
//		        ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame())))
	((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)))
		{
			if (ClRe == static_cast<int>(StartInd))
			{
				FPoint ap(Clip.point(EndInd-2));
				FPoint ap2(Clip.point(EndInd-1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(EndInd-2, Clip.point(StartInd));
				Clip.setPoint(EndInd-1, ap2);
			}
			else
			{
				FPoint ap(Clip.point(StartInd));
				FPoint ap2(Clip.point(StartInd + 1));
				ap2.setX(ap2.x() - (ap.x() - np.x()));
				ap2.setY(ap2.y() - (ap.y() - np.y()));
				Clip.setPoint(StartInd, Clip.point(EndInd-2));
				Clip.setPoint(StartInd + 1, ap2);
			}
		}
		if (((ClRe == static_cast<int>(StartInd+1)) || (ClRe == static_cast<int>(EndInd-1))) &&
//		        ((currItem->asPolygon()) || (currItem->asTextFrame()) || (currItem->asImageFrame())) && (MoveSym))
((currItem->itemType() == PageItem::Polygon) || (currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame)) && (MoveSym))
		{
			uint kon = 0;
			if (ClRe == static_cast<int>(StartInd+1))
				kon = EndInd-1;
			else
				kon = StartInd + 1;
			FPoint lxy(Clip.point(ClRe-1));
			FPoint lk(Clip.point(ClRe));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			Clip.setPoint(kon, lk);
		}
		if ((ClRe % 2 != 0) && (MoveSym) && (ClRe != static_cast<int>(StartInd + 1)) && (ClRe != static_cast<int>(EndInd-1)))
		{
			uint kon = 0;
			if ((ClRe+1) % 4 == 0)
				kon = ClRe + 2;
			else
				kon = ClRe - 2;
			FPoint lxy(Clip.point(ClRe-1));
			FPoint lk(Clip.point(ClRe));
			double dx = lxy.x() - lk.x();
			double dy = lxy.y() - lk.y();
			lk.setX(lk.x() + dx*2);
			lk.setY(lk.y() + dy*2);
			Clip.setPoint(kon, lk);
		}
		if (EditContour)
			currItem->ContourLine = Clip.copy();
		else
			currItem->PoLine = Clip.copy();
		currItem->Clip = FlattenPath(currItem->PoLine, currItem->Segments);
//		RefreshItem(currItem);
//		MarkClip(currItem, Clip);
	}
	if (!EditContour)
		currItem->ContourLine.translate(xposOrig - currItem->xPos(), yposOrig - currItem->yPos());
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
	PageItem* currItem;
	double gx, gy, gw, gh;
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	QPoint in(qRound(gx*Scale), qRound(gy*Scale));
	in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	QPoint out = contentsToViewport(in);
	QRect OldRect = QRect(out.x(), out.y(), qRound(gw*Scale), qRound(gh*Scale));
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		currItem = itemSelection->itemAt(a);
		Doc->MoveItem(x, y, currItem, fromMP);
	}
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	if (itemSelection->isMultipleSelection())
	{
		emit ItemPos(gx, gy);
		currItem = itemSelection->itemAt(0);
		Doc->GroupOnPage(currItem);
	}
	QPoint in2(qRound(gx*Scale), qRound(gy*Scale));
	in2 -= QPoint(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	OldRect = OldRect.united(QRect(in2.x()+contentsX(), in2.y()+contentsY(), qRound(gw*Scale), qRound(gh*Scale)));
	updateContents(OldRect.adjusted(-10, -10, 20, 20));
}

//CB-->Doc
void ScribusView::RotateGroup(double win)
{
	if (!_groupTransactionStarted && Doc->m_Selection->count() > 1)
	{
		QString tooltip = Um::ItemsInvolved + "\n";
		for (uint i = 0; i < Doc->m_Selection->count(); ++i)
			tooltip += "\t" + Doc->m_Selection->itemAt(i)->getUName() + "\n";
		undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup,
									  Um::Rotate, tooltip, Um::IRotate);
		_groupTransactionStarted = true;
	}
	double gxS, gyS, ghS, gwS;
	double sc = Scale;
	PageItem* currItem;
	Doc->m_Selection->getGroupRect(&gxS, &gyS, &gwS, &ghS);
	QMatrix ma;
	ma.translate(RCenter.x(), RCenter.y());
	ma.scale(1, 1);
	ma.rotate(win);
	gxS -= Doc->minCanvasCoordinate.x();
	gyS -= Doc->minCanvasCoordinate.y();
	QRect oldR = QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10));
	FPoint n;
	for (uint a = 0; a < Doc->m_Selection->count(); ++a)
	{
		currItem = Doc->m_Selection->itemAt(a);
		n = FPoint(currItem->xPos() - RCenter.x(), currItem->yPos() - RCenter.y());
		currItem->setXYPos(ma.m11() * n.x() + ma.m21() * n.y() + ma.dx(), ma.m22() * n.y() + ma.m12() * n.x() + ma.dy());
		currItem->rotateBy(win);
		Doc->setRedrawBounding(currItem);
	}
	currItem = Doc->m_Selection->itemAt(0);
	Doc->GroupOnPage(currItem);
	Doc->m_Selection->setGroupRect();
	Doc->m_Selection->getGroupRect(&gxS, &gyS, &gwS, &ghS);
	gxS -= Doc->minCanvasCoordinate.x();
	gyS -= Doc->minCanvasCoordinate.y();
	updateContents(QRect(static_cast<int>(gxS*sc-5), static_cast<int>(gyS*sc-5), static_cast<int>(gwS*sc+10), static_cast<int>(ghS*sc+10)).unite(oldR));
}

//CB-->Doc
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
	PageItem *bb;
	double gx, gy, gh, gw, x, y;
	int aa;
	double sc = Scale;
	int drm = Doc->RotMode;
	Doc->RotMode = 0;
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	gx -= Doc->minCanvasCoordinate.x();
	gy -= Doc->minCanvasCoordinate.y();
	QRect oldR = QRect(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10));
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	double origGW = gw;
	double origGH = gh;
	updatesOn(false);

	for (uint a = 0; a < selectedItemCount; ++a)
	{
		bb = itemSelection->itemAt(a);
		if ((bb->locked()) || (bb->sizeLocked()))
			continue;
		bb->OldB = bb->width();
		bb->OldH = bb->height();
		bb->OldB2 = bb->width();
		bb->OldH2 = bb->height();
		bb->Sizing = false;
		double oldRot, oldLocalX, oldLocalY;
		oldRot = bb->rotation();
		oldLocalX = bb->imageXOffset();
		oldLocalY = bb->imageYOffset();
		FPointArray gr;
		gr.addPoint(bb->GrStartX, bb->GrStartY);
		gr.addPoint(bb->GrEndX, bb->GrEndY);
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
		if (bb->Groups.count() != 0)				// change the LineWidth only when the item is within a real Group
			bb->setLineWidth(qMax(bb->lineWidth()*((scx+scy)/2), 0.01));
		if (bb->itemType() == PageItem::Line)
		{
			bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
			bb->setWidth(sqrt(pow(t1.x()-b1.x(),2)+pow(t1.y()-b1.y(),2)));
			bb->setXYPos(b1.x()+gx, b1.y()+gy);
		}
		else
		{
			FPoint oldPos(bb->xPos(), bb->yPos());
			QMatrix ma;
			ma.rotate(bb->rotation());
			bb->PoLine.map(ma);
			QMatrix ma2;
			ma2.translate(gx-bb->xPos(), gy-bb->yPos());
			ma2.scale(scx, scy);
			bb->PoLine.map(ma2);
			bb->setRotation(0.0);
			bb->ClipEdited = true;
			Doc->AdjustItemSize(bb);
			QMatrix ma3;
			ma3.translate(gx, gy);
			ma3.scale(scx, scy);
			FPoint n(gx-oldPos.x(), gy-oldPos.y());
			x = ma3.m11() * n.x() + ma3.m21() * n.y() + ma3.dx();
			y = ma3.m22() * n.y() + ma3.m12() * n.x() + ma3.dy();
			Doc->MoveItem(gx-x, gy-y, bb, true);
			if (oldRot != 0)
			{
				bb->setRotation(atan2(t1.y()-b1.y(),t1.x()-b1.x())*(180.0/M_PI));
				QMatrix ma;
				ma.rotate(-bb->rotation());
				bb->PoLine.map(ma);
				Doc->AdjustItemSize(bb);
			}
		}
		if (scaleText)
		{
#ifndef NLS_PROTO
//			bb->setFontSize(qMax(qRound(bb->fontSize()*((scx+scy)/2)), 1));
			if ((bb->itemText.length() != 0) && (!bb->isTableItem))
			{
//				bb->setLineSpacing(((bb->fontSize() / 10.0) * static_cast<double>(Doc->typographicSettings.autoLineSpacing) / 100) + (bb->fontSize() / 10.0));
				for (aa = 0; aa < bb->itemText.length(); ++aa)
					bb->itemText.item(aa)->setFontSize(qMax(qRound(bb->itemText.item(aa)->fontSize()*((scx+scy)/2)), 1));
				if (bb->asPathText())
					bb->updatePolyClip();
			}
#endif
		}
		bb->setImageXYOffset(oldLocalX, oldLocalY);
		bb->OldB2 = bb->width();
		bb->OldH2 = bb->height();
		QMatrix ma4;
		ma4.rotate(oldRot);
		ma4.scale(scx, scy);
		gr.map(ma4);
		bb->ContourLine.map(ma4);
		bb->GrStartX = gr.point(0).x();
		bb->GrStartY = gr.point(0).y();
		bb->GrEndX = gr.point(1).x();
		bb->GrEndY = gr.point(1).y();
		bb->updateGradientVectors();
	}
	bb = itemSelection->itemAt(0);
	Doc->GroupOnPage(bb);
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	Doc->RotMode = drm;
	if ((Doc->RotMode != 0) && (!Doc->isLoading()))
	{
		switch (Doc->RotMode)
		{
		case 2:
			moveGroup((origGW-gw) / 2.0, (origGH-gh) / 2.0, true);
			break;
		case 4:
			moveGroup(origGW-gw, origGH-gh, true);
			break;
		case 3:
			moveGroup(0.0, origGH-gh, true);
			break;
		case 1:
			moveGroup(origGW-gw, 0.0, true);
			break;
		}
	}
	gx -= Doc->minCanvasCoordinate.x();
	gy -= Doc->minCanvasCoordinate.y();
	updatesOn(true);
	updateContents(QRect(static_cast<int>(gx*sc-5), static_cast<int>(gy*sc-5), static_cast<int>(gw*sc+10), static_cast<int>(gh*sc+10)).unite(oldR));
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	for (uint a = 0; a < selectedItemCount; ++a)
	{
		PageItem *currItem = itemSelection->itemAt(a);
		currItem->gXpos = currItem->xPos() - gx;
		currItem->gYpos = currItem->yPos() - gy;
		currItem->gWidth = gw;
		currItem->gHeight = gh;
	}
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
		//int xP = qRound(x/Scale + Doc->minCanvasCoordinate.x());
		//int yP = qRound(y/Scale + Doc->minCanvasCoordinate.y());
 		QMatrix p;
//		p.begin(this);
		Transform(currItemGeneric, p);
		p.translate(qRound(-Doc->minCanvasCoordinate.x()), qRound(-Doc->minCanvasCoordinate.y()));
		if (currItemGeneric->asImageFrame())
			return true;
		PageItem_TextFrame *currItem=currItemGeneric->asTextFrame();
		if (currItem==0)
			return false;
		QRect mpo(x - Doc->guidesSettings.grabRad, y - Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
		if ((QRegion(p.map(Q3PointArray(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo)) ||
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
						if (((currItem->itemText.text(a) == SpecialChars::PARSEP) || (currItem->itemText.text(a) == SpecialChars::LINEBREAK)))
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
			FPoint point((x + Doc->minCanvasCoordinate.x()) / Scale - currItem->xPos(), 
						 (y + Doc->minCanvasCoordinate.x()) / Scale - currItem->yPos());
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
			Transform(textframe, &p);
			TransformM(textframe, &p);
			
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
				FRect bbox = textframe->itemText.boundingBox(QMAX(0,QMIN(textframe->CPos, textframe->itemText.length())));
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
//	ma.translate(-Doc->minCanvasCoordinate.x()*Scale, -Doc->minCanvasCoordinate.y()*Scale);
	Transform(currItem, ma);
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
	if (Doc->EditClip)
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
			updateContents(currItem->getRedrawBounding(Scale));
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
								updateContents(currItem->getRedrawBounding(Scale));
						}
					}
				}
			}
			else
			{
				Doc->m_Selection->addItem(currItem);
				if (draw)
					updateContents(currItem->getRedrawBounding(Scale));
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
	m_MouseButtonPressed = true;
	Mxp = static_cast<int>(m->x()/Scale);
	Myp = static_cast<int>(m->y()/Scale);
	QRect mpo(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	mpo.moveBy(qRound(Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	ClRe = -1;
	Deselect(false);
	if (!Doc->masterPageMode())
	{
		int i = Doc->OnPage(Mxp + Doc->minCanvasCoordinate.x(), Myp + Doc->minCanvasCoordinate.y());
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
			int x = static_cast<int>(Doc->Pages->at(i)->xOffset() * Scale);
			int y = static_cast<int>(Doc->Pages->at(i)->yOffset() * Scale);
			int w = static_cast<int>(Doc->Pages->at(i)->width() * Scale);
			int h = static_cast<int>(Doc->Pages->at(i)->height() * Scale);
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
	m_MouseButtonPressed = true;
	Mxp = static_cast<int>(m->x()/Scale);
	Myp = static_cast<int>(m->y()/Scale);
	int MxpS = static_cast<int>(m->x()/Scale + Doc->minCanvasCoordinate.x());
	int MypS = static_cast<int>(m->y()/Scale + Doc->minCanvasCoordinate.y());
	mpo = QRectF(m->x()-Doc->guidesSettings.grabRad, m->y()-Doc->guidesSettings.grabRad, Doc->guidesSettings.grabRad*2, Doc->guidesSettings.grabRad*2);
	mpo.translate(Doc->minCanvasCoordinate.x() * Scale, Doc->minCanvasCoordinate.y() * Scale);
	ClRe = -1;
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
	if (m->state() == (Qt::ControlButton | Qt::AltButton))
		Deselect(false);

	if ((m->state() == (Qt::ShiftButton | Qt::AltButton)) && (!Doc->masterPageMode()) && (Doc->currentPage()->FromMaster.count() != 0))
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
				Transform(currItem, p);
				if ((QRegion(p.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo.toRect())) ||
						(QRegion(currItem->Clip * p).contains(mpo.toRect())))
				{
					Doc->m_Selection->setIsGUISelection(false);
					if (!currItem->isSelected())
					{
						if ((m->state() != Qt::ShiftButton) || (Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
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
							if (m->state() != (Qt::ControlButton | Qt::AltButton))
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
								updateContents(currItem->getRedrawBounding(Scale));
							}
						}
						else
						{
							Doc->m_Selection->addItem(currItem, true);
							updateContents(currItem->getRedrawBounding(Scale));
						}
					}
					else
					{
						if (Doc->m_Selection->count() > 1)
						{
							PageItem *bb = Doc->m_Selection->itemAt(0);
							Doc->m_Selection->removeItem(currItem);
							Doc->m_Selection->prependItem(currItem);
							updateContents(currItem->getRedrawBounding(Scale));
							updateContents(bb->getRedrawBounding(Scale));
						}
					}
					Doc->m_Selection->setIsGUISelection(true);
					Doc->m_Selection->connectItemToGUI();
					if (Doc->m_Selection->count() > 1)
					{
						for (uint aa = 0; aa < Doc->m_Selection->count(); ++aa)
						{
							PageItem *bb = Doc->m_Selection->itemAt(aa);
							updateContents(bb->getRedrawBounding(Scale));
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
	if (Doc->Items->count() == 0)
		return false;
	if ((Doc->m_Selection->count() != 0) && (m->state() == Qt::ControlButton))
		currItem = Doc->m_Selection->itemAt(0);
	else
		currItem = Doc->Items->at(Doc->Items->count()-1);
	if ((m->state() == (Qt::ControlButton | Qt::ShiftButton)) && (Doc->m_Selection->count() != 0))
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
		return false;
	}
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
			Transform(currItem, p);
			if ((QRegion(p.map(QPolygon(QRect(0, 0, static_cast<int>(currItem->width()), static_cast<int>(currItem->height()))))).contains(mpo.toRect())) ||
			        (QRegion(currItem->Clip * p).contains(mpo.toRect())))
			{
				//If the clicked on item is not tagged as selected
				if (!currItem->isSelected())
				{
					if ((m->state() != Qt::ShiftButton) || (Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
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
						if (m->state() != (Qt::ControlButton | Qt::AltButton))
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
							updateContents(currItem->getRedrawBounding(Scale));
						}
					}
					else
					//If we are just selecting one item
					{
						//CB 301206 We shouldnt be ignoring the GUI here...
						//Doc->m_Selection->addItem(currItem, true);
						Doc->m_Selection->addItem(currItem);
						updateContents(currItem->getRedrawBounding(Scale));
					}
				}
				else //If the clicked on item is tagged as selected
				{
					if (Doc->m_Selection->count() > 1)
					{
						PageItem *bb = Doc->m_Selection->itemAt(0);
						Doc->m_Selection->removeItem(currItem);
						Doc->m_Selection->prependItem(currItem);
						updateContents(currItem->getRedrawBounding(Scale));
						//CB dont think we need to paint here when we paint below
						//CB With the change of 301206, perhaps we need to?
						updateContents(bb->getRedrawBounding(Scale));
					}
				}
				Doc->m_Selection->setIsGUISelection(true);
				Doc->m_Selection->connectItemToGUI();
				if (Doc->m_Selection->count() > 1)
				{
					for (uint aa = 0; aa < Doc->m_Selection->count(); ++aa)
					{
						PageItem *bb = Doc->m_Selection->itemAt(aa);
						updateContents(bb->getRedrawBounding(Scale));
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
					operItemResizing = false;
				}
				return true;
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
		double lowX = ((m->x() - Doc->guidesSettings.grabRad) / Scale) + Doc->minCanvasCoordinate.x();
		double highX = ((m->x() + Doc->guidesSettings.grabRad) / Scale) + Doc->minCanvasCoordinate.x();
		double lowY = ((m->y() - Doc->guidesSettings.grabRad) / Scale) + Doc->minCanvasCoordinate.y();
		double highY = ((m->y() + Doc->guidesSettings.grabRad) / Scale) + Doc->minCanvasCoordinate.y();
		for (it = tmpGuides.begin(); it != tmpGuides.end(); ++it, ++yg)
		{
			if (((*it) + Doc->currentPage()->yOffset() < highY) && ((*it)+Doc->currentPage()->yOffset() > lowY))
				tmpGuidesSel.insert(fabs(((*it)+Doc->currentPage()->yOffset()) - MypS), yg);
		}
		if (tmpGuidesSel.count() != 0)
		{
			GyM = tmpGuidesSel.begin().data();
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
			GxM = tmpGuidesSel.begin().data();
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
	if ((m->state() != Qt::ShiftButton) || (Doc->appMode == modeLinkFrames) || (Doc->appMode == modeUnlinkFrames))
		Deselect(true);
	return false;
}

//CB Fix item->old* stuff
void ScribusView::HandleSizer(PageItem *currItem, QRect mpo, QMouseEvent *m)
{
	currItem->OldB = currItem->width();
	currItem->OldH = currItem->height();
	currItem->OldB2 = currItem->width();
	currItem->OldH2 = currItem->height();
	frameResizeHandle = 0;
	if (currItem->sizeLocked())
		return;
	PaintSizeRect(QRect());
	double d1;
	QMap<double,int> distance;
	FPoint n1(currItem->width(), currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
	n1 -= QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
	d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 1);
	n1 = FPoint(0, 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
	n1 -= QPoint(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
	d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
	if (d1 < Doc->guidesSettings.grabRad)
		distance.insert(d1, 2);
	if (!currItem->asLine())
	{
		QPoint docMinCanvasCoordinate(qRound(Doc->minCanvasCoordinate.x()), qRound(Doc->minCanvasCoordinate.y()));
		n1 = FPoint(currItem->width(), 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 3);
		n1 = FPoint(0, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 4);
		n1 = FPoint(currItem->width()/2, currItem->height(), currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 5);
		n1 = FPoint(currItem->width(), currItem->height()/2, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 6);
		n1 = FPoint(0, currItem->height()/2, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 7);
		n1 = FPoint(currItem->width()/2, 0, currItem->xPos(), currItem->yPos(), currItem->rotation(), 1, 1);
		n1 -= docMinCanvasCoordinate;
		d1 = sqrt(pow(n1.x() * Scale - m->x(),2)+pow(n1.y() * Scale - m->y(),2));
		if (d1 < Doc->guidesSettings.grabRad)
			distance.insert(d1, 8);
	}
	QList<int> result = distance.values();
	if (result.count() != 0)
		frameResizeHandle = result[0];
	mpo.moveBy(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(Doc->minCanvasCoordinate.y() * Scale));
	HandleCurs(currItem, mpo);
	if (frameResizeHandle != 0)
	{
		if (!currItem->asLine())
			currItem->Sizing = true;
		operItemResizing = true;
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
		PageItem* currItem=NULL;
		for (uint a = 0; a < Doc->m_Selection->count(); ++a)
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
			x -= Doc->minCanvasCoordinate.x();
			y -= Doc->minCanvasCoordinate.y();
			updateContents(static_cast<int>(x*Scale-5), static_cast<int>(y*Scale-5), static_cast<int>(w*Scale+10), static_cast<int>(h*Scale+10));
		}
		else
		{
			currItem=Doc->m_Selection->itemAt(0);
			Doc->m_Selection->clear();
			if (currItem!=NULL)
				updateContents(currItem->getRedrawBounding(Scale));
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
	operItemResizing = true;
	frameResizeHandle = 1;
	qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
	Doc->m_Selection->setIsGUISelection(true);
	Doc->m_Selection->clear();
	Doc->m_Selection->addItem(currItem);
	Doc->m_Selection->connectItemToGUI();
	updateContents(currItem->getRedrawBounding(Scale));
	operItemMoving = true;
	Doc->appMode = modeNormal;
	emit DocChanged();
	currItem->Sizing =  currItem->asLine() ? false : true;
	inItemCreation = true;
	specialRendering = true;
	firstSpecial = true;
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
	updateContents(currItem->getRedrawBounding(Scale));
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
		for (uint a = 0; a < Doc->m_Selection->count(); ++a)
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
		for (uint c = 0; c < Doc->m_Selection->count(); ++c)
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
		for (uint c = 0; c < Doc->m_Selection->count(); ++c)
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
		Selection pastedObjects = Serializer(*Doc).deserializeObjects(m_ScMW->Buffer2.utf8());
		Doc->useRaster = savedAlignGrid;
		Doc->SnapGuides = savedAlignGuides;
		pastedObjects.setGroupRect();
		double gx, gy, gh, gw;
		pastedObjects.getGroupRect(&gx, &gy, &gw, &gh);
		moveGroup(Mxp / Scale - gx, Myp / Scale - gy, false, &pastedObjects);
		Doc->m_Selection->clear();
	}
	else
		emit LoadElem(m_ScMW->Buffer2, Mxp / Scale, Myp / Scale, false, false, Doc, this);
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
	if (fi.extension(true).lower() == "sml")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createObjects(f);
		delete pre;
	}
	else if (fi.extension(true).lower() == "shape")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createShape(f);
		delete pre;
	}
	else if (fi.extension(true).lower() == "sce")
	{
		QString f = "";
		loadText(data, &f);
		data = f;
	}
	int ac = Doc->Items->count();
	emit LoadElem(data, Mxp / Scale, Myp / Scale, false, false, Doc, this);
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
	return m_MouseButtonPressed;
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
		horizRuler->offs = x / Scale - Doc->currentPage()->xOffset();
		vertRuler->offs = y / Scale - Doc->currentPage()->yOffset();
	}
	else
	{
		horizRuler->offs = x / Scale;
		vertRuler->offs = y / Scale;
	}
//	horizRuler->offs += qRound(Doc->minCanvasCoordinate.x() - 1 - Doc->rulerXoffset);
//	vertRuler->offs += qRound(Doc->minCanvasCoordinate.y() - 1 - Doc->rulerYoffset);
	horizRuler->offs += Doc->minCanvasCoordinate.x()  - Doc->rulerXoffset;
	vertRuler->offs += Doc->minCanvasCoordinate.y()  - Doc->rulerYoffset;
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
			int xs = static_cast<int>(Doc->Pages->at(a)->xOffset() * Scale);
			int ys = static_cast<int>(Doc->Pages->at(a)->yOffset() * Scale);
			int ws = static_cast<int>(Doc->Pages->at(a)->width() * Scale);
			int hs = static_cast<int>(Doc->Pages->at(a)->height() * Scale);
			QRect drawRect = QRect(x, y, visibleWidth(), visibleHeight());
			drawRect.moveBy(qRound(-Doc->minCanvasCoordinate.x() * Scale), qRound(-Doc->minCanvasCoordinate.y() * Scale));
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
	m_MouseButtonPressed = false;
	Doc->DragP = false;
	Doc->leaveDrag = false;
	operItemMoving = false;
	MidButt = false;
	HaveSelRect = false;
	Magnify = false;
	FirstPoly = true;
	EdPoints = true;
	//GroupSel = false;
	DraggedGroup = false;
	MoveGY = false;
	MoveGX = false;
	EditContour = false;
	return fe;
}

void ScribusView::reformPages(bool moveObjects)
{
	Doc->reformPages(moveObjects);
	if (!m_ScMW->ScriptRunning)
		setContentsPos(qRound((Doc->currentPage()->xOffset()-10 - Doc->minCanvasCoordinate.x()) * Scale), qRound((Doc->currentPage()->yOffset()-10 - Doc->minCanvasCoordinate.y()) * Scale));
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
	if (!operItemMoving)
	{
		bool updback = updateOn;
		updatesOn(false);
		disconnect(this, SIGNAL(contentsMoving(int, int)), this, SLOT(setRulerPos(int, int)));
		int oldDX = contentsX();
		int oldDY = contentsY();
		int nw = qMax(qRound(width * Scale), contentsWidth() + qRound(dX * Scale) * 2);
		int nh = qMax(qRound(height * Scale), contentsHeight() + qRound(dY * Scale) * 2);
		resizeContents(qMax(nw, visibleWidth() + qRound(dX * Scale) * 2), qMax(nh, visibleHeight() + qRound(dY * Scale) * 2));
		setContentsPos(oldDX + qRound(dX * Scale), oldDY + qRound(dY * Scale));
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
	pageSelector->setMaxValue(Doc->masterPageMode() ? 1 : Doc->Pages->count());
	if ((!Doc->isLoading()) && (!Doc->masterPageMode()))
		pageSelector->GotoPg(Seite);
	connect(pageSelector, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
}

/** Fuehrt die Vergroesserung/Verkleinerung aus */
void ScribusView::slotDoZoom()
{
	undoManager->setUndoEnabled(false);
	if (Scale > 32*Prefs->DisScale)
	{
		setScale(32*Prefs->DisScale);
		return;
	}
	updatesOn(false);
	int nw = qMax(qRound((Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()) * Scale), visibleWidth());
	int nh = qMax(qRound((Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()) * Scale), visibleHeight());
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
	int x = qRound(qMax(contentsX() / Scale, 0.0));
	int y = qRound(qMax(contentsY() / Scale, 0.0));
	int w = qRound(qMin(visibleWidth() / Scale, Doc->currentPage()->width()));
	int h = qRound(qMin(visibleHeight() / Scale, Doc->currentPage()->height()));
	rememberOldZoomLocation(w / 2 + x,h / 2 + y);
	setScale(zoomSpinBox->value() / 100.0 * Prefs->DisScale);
	slotDoZoom();
	m_ScMW->setFocus();
}

void ScribusView::slotZoom100()
{
	int x = qRound(qMax(contentsX() / Scale, Doc->minCanvasCoordinate.x()));
	int y = qRound(qMax(contentsY() / Scale, Doc->minCanvasCoordinate.y()));
	int w = qRound(qMin(visibleWidth() / Scale, Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
	int h = qRound(qMin(visibleHeight() / Scale, Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
	rememberOldZoomLocation(w / 2 + x,h / 2 + y);
	setScale(Prefs->DisScale);
	slotDoZoom();
}

void ScribusView::slotZoomIn(int mx,int my)
{
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(qMax(contentsX() / Scale, Doc->minCanvasCoordinate.x()));
		int y = qRound(qMax(contentsY() / Scale, Doc->minCanvasCoordinate.y()));
		int w = qRound(qMin(visibleWidth() / Scale, Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
		int h = qRound(qMin(visibleHeight() / Scale, Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
		rememberOldZoomLocation(w/2+x, h/2+y);
	}
	else
		rememberOldZoomLocation(mx,my);
	setScale(Scale * static_cast<double>(Doc->toolSettings.magStep)/100.0);
	slotDoZoom();
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut(int mx,int my)
{
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(qMax(contentsX() / Scale, Doc->minCanvasCoordinate.x()));
		int y = qRound(qMax(contentsY() / Scale, Doc->minCanvasCoordinate.y()));
		int w = qRound(qMin(visibleWidth() / Scale, Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()));
		int h = qRound(qMin(visibleHeight() / Scale, Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()));
		rememberOldZoomLocation(w/2+x, h/2+y);
	}
	else
		rememberOldZoomLocation(mx,my);
	setScale(Scale / (static_cast<double>(Doc->toolSettings.magStep)/100.0));
	slotDoZoom();
}

FPoint ScribusView::translateToView(double x, double y)
{
	return FPoint((x - Doc->minCanvasCoordinate.x()) * Scale, (y - Doc->minCanvasCoordinate.y())* Scale);
}

FPoint ScribusView::translateToView(FPoint in)
{
	return translateToView(in.x(), in.y());
}

FPoint ScribusView::translateToDoc(double x, double y)
{
	return FPoint(x / Scale + Doc->minCanvasCoordinate.x(), y / Scale + Doc->minCanvasCoordinate.y());
}

FPoint ScribusView::translateToDoc(FPoint in)
{
	return translateToDoc(in.x(), in.y());
}

FPoint ScribusView::translateFromViewport(double x, double y)
{
	return FPoint((x + contentsX()) / Scale + Doc->minCanvasCoordinate.x(), (y + contentsY()) / Scale + Doc->minCanvasCoordinate.y());
}

FPoint ScribusView::translateFromViewport(FPoint in)
{
	return translateFromViewport(in.x(), in.y());
}

FPoint ScribusView::translateToViewport(double x, double y)
{
	return FPoint((x - Doc->minCanvasCoordinate.x()) * Scale - contentsX(), (y - Doc->minCanvasCoordinate.y())* Scale - contentsY());
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
	zoomSpinBox->setValue(Scale/Prefs->DisScale*100);
	connect(zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom()));
}

void ScribusView::SetCCPo(double x, double y)
{
	if (m_ScMW->ScriptRunning)
		return;
	FPoint nx = translateToView(x, y);
	center(qRound(nx.x()), qRound(nx.y()));
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
	OldScale = Scale;
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
	resizeContents(qRound((Doc->maxCanvasCoordinate.x() - Doc->minCanvasCoordinate.x()) * Scale), qRound((Doc->maxCanvasCoordinate.y() - Doc->minCanvasCoordinate.y()) * Scale));
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
		double sca = Scale;
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
		previewMode = true;
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
		DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		painter->endLayer();
		painter->end();
		double sx = pm.width() / static_cast<double>(maxGr);
		double sy = pm.height() / static_cast<double>(maxGr);
		if (sy < sx)
			im = pm.smoothScale(static_cast<int>(pm.width() / sx), static_cast<int>(pm.height() / sx));
		else
			im = pm.smoothScale(static_cast<int>(pm.width() / sy), static_cast<int>(pm.height() / sy));
		delete painter;
		painter=NULL;
		previewMode = false;
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
		double sca = Scale;
		double cx = Doc->minCanvasCoordinate.x();
		double cy = Doc->minCanvasCoordinate.y();
		Doc->minCanvasCoordinate = FPoint(0, 0);
		bool frs = Doc->guidesSettings.framesShown;
		bool ctrls = Doc->guidesSettings.showControls;
		Doc->guidesSettings.framesShown = false;
		Doc->guidesSettings.showControls = false;
		Scale = sc;
		previewMode = true;
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
		painter->setZoomFactor(Scale);
		DrawMasterItems(painter, Doc->Pages->at(Nr), QRect(clipx, clipy, clipw, cliph));
		DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
		painter->endLayer();
		painter->end();
		Doc->guidesSettings.framesShown = frs;
		Doc->guidesSettings.showControls = ctrls;
		Scale = sca;
		Doc->setCurrentPage(act);
		Doc->setLoading(false);
		delete painter;
		painter=NULL;
		previewMode = false;
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
	emit MousePos((py.x() + contentsX())/Scale, (py.y() + contentsY())/Scale);
	horizRuler->Draw(out.x());
	vertRuler->Draw(out.y());
	redrawPolygon.clear();
	redrawPolygon << QPoint(newX, newY);
	updateContents();
	DrHY = newY;
	DrVX = newX;
}

void ScribusView::setNewRulerOrigin(QMouseEvent *m)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	Doc->rulerXoffset = (py.x() + contentsX()) / Scale + Doc->minCanvasCoordinate.x();
	Doc->rulerYoffset = (py.y() + contentsY()) / Scale + Doc->minCanvasCoordinate.y();
	if (Doc->guidesSettings.rulerMode)
	{
		Doc->rulerXoffset -= Doc->currentPage()->xOffset();
		Doc->rulerYoffset -= Doc->currentPage()->yOffset();
	}
	setRulerPos(contentsX(), contentsY());
	updateContents();
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
	emit MousePos((py.x() + contentsX())/Scale, (py.y() + 2 + contentsY())/Scale);
	horizRuler->Draw(out.x());
	vertRuler->Draw(out.y() + 2);
	if (!redrawMarker->isVisible())
		redrawMarker->show();
	if (QRect(0, 0, visibleWidth(), visibleHeight()).contains(py))
		redrawMarker->setGeometry(QRect(viewport()->mapToGlobal(QPoint(0, 0)).x(), m->globalPos().y(), visibleWidth(), 1));
	DrHY = newY;
	double newXp = (py.x() + contentsX()) / Scale + Doc->minCanvasCoordinate.x();
	double newYp = (py.y() + contentsY()) / Scale + Doc->minCanvasCoordinate.y();
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
	emit MousePos((py.x() + 2 + contentsX())/Scale, (py.y() + contentsY())/Scale);
	horizRuler->Draw(out.x() + 2);
	vertRuler->Draw(out.y());
	if (!redrawMarker->isVisible())
		redrawMarker->show();
	if (QRect(0, 0, visibleWidth(), visibleHeight()).contains(py))
		redrawMarker->setGeometry(QRect(m->globalPos().x(), viewport()->mapToGlobal(QPoint(0, 0)).y(), 1, visibleHeight()));
	DrVX = newY;
	double newXp = (py.x() + contentsX()) / Scale + Doc->minCanvasCoordinate.x();
	double newYp = (py.y() + contentsY()) / Scale + Doc->minCanvasCoordinate.y();
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
	double newX = (py.x() + contentsX()) / Scale + Doc->minCanvasCoordinate.x();
	double newY = (py.y() + contentsY()) / Scale + Doc->minCanvasCoordinate.y();
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
	double newX = (py.x() + contentsX()) / Scale + Doc->minCanvasCoordinate.x();
	double newY = (py.y() + contentsY()) / Scale + Doc->minCanvasCoordinate.y();
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

void ScribusView::Transform(PageItem *currItem, QPainter *p)
{
	p->translate(static_cast<int>(currItem->xPos()*Scale), static_cast<int>(currItem->yPos()*Scale));
	p->scale(Scale, Scale);
	p->rotate(currItem->rotation());
}

void ScribusView::Transform(PageItem *currItem, QMatrix& m)
{
	m.translate(static_cast<int>(currItem->xPos()*Scale), static_cast<int>(currItem->yPos()*Scale));
	m.scale(Scale, Scale);
	m.rotate(currItem->rotation());
}

void ScribusView::TransformM(PageItem *currItem, QPainter *p)
{
	if (currItem->imageFlippedH())
	{
		p->translate(currItem->width(), 0);
		p->scale(-1, 1);
	}
	if (currItem->imageFlippedV())
	{
		p->translate(0, currItem->height());
		p->scale(1, -1);
	}
}

//CB-->Doc
void ScribusView::SetFrameRect()
{
	ClRe = -1;
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		currItem->SetRectFrame();
		Doc->setRedrawBounding(currItem);
		updateContents(currItem->getRedrawBounding(Scale));
	}
}

//CB-->Doc
void ScribusView::SetFrameRounded()
{
	ClRe = -1;
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
		updateContents(currItem->getRedrawBounding(Scale));
	}
}

//CB-->Doc
void ScribusView::SetFrameOval()
{
	ClRe = -1;
	PageItem *currItem;
	if (GetItem(&currItem))
	{
		currItem->SetOvalFrame();
		Doc->setRedrawBounding(currItem);
		updateContents(currItem->getRedrawBounding(Scale));
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
	switch (Buffer->PType)
	{
	// OBSOLETE CR 2005-02-06
	case PageItem::ItemType1:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Ellipse, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	//
	case PageItem::ImageFrame:
		z = Doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, x, y, w, h, 1, Doc->toolSettings.dBrushPict, CommonStrings::None, !m_MouseButtonPressed);
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
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	//
	case PageItem::PathText:
	case PageItem::TextFrame:
#ifndef NLS_PROTO
		if (Buffer->PType == PageItem::PathText)
			z = Doc->itemAdd(PageItem::PathText, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Buffer->Pcolor, !m_MouseButtonPressed);
		else
			z = Doc->itemAdd(PageItem::TextFrame, PageItem::Unspecified, x, y, w, h, pw, CommonStrings::None, Buffer->Pcolor, !m_MouseButtonPressed);
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
		z = Doc->itemAdd(PageItem::Line, PageItem::Unspecified, x, y, w ,0, pw, CommonStrings::None, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	case PageItem::Polygon:
		z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	case PageItem::PolyLine:
		z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, x, y, w, h, pw, Buffer->Pcolor, Buffer->Pcolor2, !m_MouseButtonPressed);
		break;
	case PageItem::Multiple:
		Q_ASSERT(false);
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
	currItem->Clip = Buffer->Clip.copy(); //irrelevant, overwritten below
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
			ClRe = -1;
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
	m_ScMW->NoFrameEdit();
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
						if (chstr.upper() != chstr)
						{
							chs = qMax(static_cast<int>(hl->fontSize() * Doc->typographicSettings.valueSmallCaps / 100), 1);
							chstr = chstr.upper();
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
					uint z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), !m_MouseButtonPressed);
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
							if (chstr.upper() != chstr)
							{
								chs = qMax(static_cast<int>(hl->fontSize() * Doc->typographicSettings.valueSmallCaps / 100), 1);
								chstr = chstr.upper();
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
							uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), !m_MouseButtonPressed);
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
						uint z = Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), !m_MouseButtonPressed);
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
							uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), currItem->lineColor(), currItem->fillColor(), !m_MouseButtonPressed);
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
				uint z = Doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), CommonStrings::None, currItem->lineColor(), !m_MouseButtonPressed);
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
	Q3ScrollView::contentsWheelEvent(w);
	evSpon = true;
	if ((m_MouseButtonPressed) && (MidButt) || ( w->state() & Qt::ControlButton ))
	{
		w->delta() > 0 ? slotZoomIn() : slotZoomOut();
	}
	else
	{
		int dX=0,dY=0;
		int moveBy=(w->delta() < 0) ? Prefs->Wheelval : -Prefs->Wheelval;
		if ((w->orientation() != Qt::Vertical) || ( w->state() & Qt::ShiftButton ))
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
	setMargins(newTopLeftMargin, newTopLeftMargin, 0, 0);
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
	Scale=newScale;
	if (Scale < Doc->toolSettings.magMin*Prefs->DisScale/100.0)
		Scale=Doc->toolSettings.magMin*Prefs->DisScale/100.0;

	if (Scale > Doc->toolSettings.magMax*Prefs->DisScale/100.0)
		Scale=Doc->toolSettings.magMax*Prefs->DisScale/100.0;

	if (Scale > 32*Prefs->DisScale)
		Scale=32*Prefs->DisScale;

	unitChange();
}

void ScribusView::drawLinkFrameLine(ScPainter* painter, FPoint &start, FPoint &end)
{
//CB FIXME Add some checking that the painter is setup?
	Q_ASSERT(painter!=NULL);
	painter->setPen(Qt::black, 1.0 / Scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	painter->setPenOpacity(1.0);
	painter->drawLine(start, end);
	QMatrix arrowTrans;
	arrowTrans.translate(end.x(), end.y());
	double r = atan2(end.y()-start.y(), end.x()-start.x())*(180.0/M_PI);
	arrowTrans.rotate(r);
	double sc = 0.8 / Scale;
	arrowTrans.scale(sc, sc);
	FPointArray arrow;
	arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
	arrow.addQuadPoint(-15, -5, -15, -5, -15, -5, -15, -5);
	arrow.addQuadPoint(0, 0, 0, 0, 0, 0, 0, 0);
	arrow.addQuadPoint(-15, 5, -15, 5, -15, 5, -15, 5);
	arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
	arrow.map(arrowTrans);
	painter->setBrush(painter->pen());
	painter->setBrushOpacity(1.0);
	painter->setLineWidth(0);
	painter->setFillMode(ScPainter::Solid);
	painter->setupPolygon(&arrow);
	painter->fillPath();
}

inline bool ScribusView::moveTimerElapsed()
{
	return (moveTimer.elapsed() > Prefs->moveTimeout);
}
