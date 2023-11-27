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
#include "sclimits.h"

#include <QColor>
#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QEvent>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QImageReader>
#include <QList>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPixmap>
#include <QPolygon>
#include <QStack>
#include <QStringList>
#include <QStyleOptionRubberBand>
#include <QWidgetAction>

#include <cstdio>
#include <cstdlib>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <QUrl>
#include <QDir>
#include <QSizeGrip>

#include "actionmanager.h"
#include "appmodehelper.h"
#include "appmodes.h"
#include "canvas.h"
#include "canvasgesture.h"
#include "canvasmode.h"
#include "canvasmode_imageimport.h"
#include "canvasmode_objimport.h"
#include "commonstrings.h"
#include "fileloader.h"
#include "filewatcher.h"
#include "hyphenator.h"
#include "iconmanager.h"
#include "loadsaveplugin.h"
#include "text/textlayoutpainter.h"
#include "pageitem.h"
#include "pageitem_group.h"
#include "pageitem_imageframe.h"
#include "pageitem_latexframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "plugins/formatidlist.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scclocale.h"
#include "scmimedata.h"
#include "scpage.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribuswin.h"
#include "scribusXml.h"
#include "selection.h"
#include "selectionrubberband.h"
#include "serializer.h"
#include "ui/extimageprops.h"
#include "ui/guidemanager.h"
#include "ui/hruler.h"
#include "ui/insertTable.h"
#include "ui/pageitemattributes.h"
#include "ui/pageselector.h"
#include "ui/propertiespalette.h"
#include "ui/propertiespalette_line.h"
#include "ui/rulermover.h"
#include "ui/scrapbookpalette.h"
#include "ui/storyeditor.h"
#include "ui/symbolpalette.h"
#include "ui/viewtoolbar.h"
#include "ui/vruler.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_math.h"

using namespace std;

ScribusView::ScribusView(QWidget* win, ScribusMainWindow* mw, ScribusDoc *doc) :
	QScrollArea(win),
	m_doc(doc),
	m_canvas(new Canvas(doc, this)),
	Prefs(&(PrefsManager::instance().appPrefs)),
	undoManager(UndoManager::instance()),
	m_ScMW(mw)
{
	setObjectName("s");
	QPalette p = palette();
	p.setBrush(QPalette::Window, PrefsManager::instance().appPrefs.displayPrefs.scratchColor);
	setPalette(p);
	setAttribute(Qt::WA_StaticContents);
	setAttribute(Qt::WA_InputMethodEnabled, true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setViewportMargins(m_vhRulerHW, m_vhRulerHW, 0, 0);
	setWidgetResizable(false);
	m_canvasMode = CanvasMode::createForAppMode(this, m_doc->appMode);
	setWidget(m_canvas);
	//already done by QScrollArea: widget()->installEventFilter(this);
	installEventFilter(this); // FIXME:av
	//	viewport()->setBackgroundMode(Qt::PaletteBackground);
	setFocusPolicy(Qt::ClickFocus);
	QFont fo = QFont(font());
	// #8058: Better not use too small font size on Windows
	// in case ClearType is not enabled
	int posi = fo.pointSize() - (ScCore->isWinGUI() ? 1 : 2);
	fo.setPointSize(posi);

	horizRuler = new Hruler(this, m_doc);
	vertRuler = new Vruler(this, m_doc);
	horizRuler->installEventFilter(this);
	vertRuler->installEventFilter(this);

	rulerMover = new RulerMover(this);
	rulerMover->setFocusPolicy(Qt::NoFocus);
	horizRuler->setGeometry(m_vhRulerHW, 1, width()-m_vhRulerHW-1, m_vhRulerHW);
	vertRuler->setGeometry(1, m_vhRulerHW, m_vhRulerHW, height()-m_vhRulerHW-1);
	rulerMover->setGeometry(1, 1, m_vhRulerHW, m_vhRulerHW);

	m_ready = true;
	m_canvas->setMouseTracking(true);
	setAcceptDrops(true);
	m_canvas->setAcceptDrops(true);
	// FIXME setDragAutoScroll(false);
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_doc->SubMode = -1;
	storedFramesShown = m_doc->guidesPrefs().framesShown;
	storedShowControls = m_doc->guidesPrefs().showControls;
	setRulersShown(m_doc->guidesPrefs().rulersShown);

	m_canvas->m_viewMode.viewAsPreview = false;
	m_canvas->setPreviewVisual(-1);
	m_previousMode = -1;

	redrawMarker = new SelectionRubberBand(QRubberBand::Rectangle, this);
	redrawMarker->hide();

	m_canvas->newRedrawPolygon();
	m_canvas->resetRenderMode();
	//  #13101 : "viewPreviewMode" is checked if necessary in ScribusMainWindow::newActWin()
	//  At this point the view and parent window may not be able to perform all what is necessary to
	//  enable preview mode anyway, especially when loading an existing doc.
	//	m_ScMW->scrActions["viewPreviewMode"]->setChecked(m_canvas->m_viewMode.viewAsPreview);
	m_mousePointDoc = FPoint(0,0);
	m_doc->regionsChanged()->connectObserver(this);
	connect(this, SIGNAL(HaveSel()), m_doc, SLOT(selectionChanged()));
	// Commented out to fix bug #7865
	//	m_dragTimer = new QTimer(this);
	//	connect(m_dragTimer, SIGNAL(timeout()), this, SLOT(dragTimerTimeOut()));
	//	m_dragTimer->stop();
	m_dragTimerFired = false;

	clockLabel = new ClockWidget(this, m_doc);
	clockLabel->setGeometry(m_vhRulerHW + 1, height() - m_vhRulerHW - 61, 60, 60);
	clockLabel->setVisible(false);

	endEditButton = new QPushButton(IconManager::instance().loadIcon("22/exit.png"), tr("End Edit"), this);
	endEditButton->setGeometry(m_vhRulerHW + 1, height() - m_vhRulerHW - endEditButton->minimumSizeHint().height() - 1, endEditButton->minimumSizeHint().width(), endEditButton->minimumSizeHint().height());
	endEditButton->setVisible(false);

	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(endEditButton, SIGNAL(clicked()), m_ScMW, SLOT(slotEndSpecialEdit()));
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
		QFrame::changeEvent(e);
}

void ScribusView::nativeGestureEvent(QNativeGestureEvent *e)
{
	if (e->gestureType() == Qt::ZoomNativeGesture)
	{
		double delta = 1 + e->value();
		FPoint mp = m_canvas->globalToCanvas(e->globalPos());
		zoom(mp.x(), mp.y(), m_canvas->scale() * delta, true);
	}
	if (e->gestureType() == Qt::SmartZoomNativeGesture)
	{
		static bool zoomTo100 = false;
		FPoint mp = m_canvas->globalToCanvas(e->globalPos());
		if (zoomTo100)
		{
			zoom(mp.x(), mp.y(), Prefs->displayPrefs.displayScale, true);
		}
		else
		{
			zoom(mp.x(), mp.y(), Prefs->displayPrefs.displayScale*0.75, true);
		}
		zoomTo100 = !zoomTo100;
	}
	e->accept();
}

void ScribusView::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();
	endEditButton->setIcon(iconManager.loadIcon("22/exit.png"));
}

void ScribusView::languageChange()
{
	endEditButton->setToolTip( tr("Click here to leave this special edit mode"));
}

void ScribusView::toggleCMS(bool cmsOn)
{
	m_doc->enableCMS(cmsOn);
	m_ScMW->requestUpdate(reqCmsOptionsUpdate);
	DrawNew();
}


void ScribusView::switchPreviewVisual(int vis)
{
	m_canvas->setPreviewVisual(vis);
	m_doc->viewAsPreview = m_canvas->usePreviewVisual();
	m_doc->previewVisual = m_canvas->previewVisual();
	m_doc->recalculateColors();
	m_doc->recalcPicturesRes();
	DrawNew();
}

void ScribusView::togglePreviewEdit(bool edit)
{
	m_doc->editOnPreview = edit;
	m_ScMW->setPreviewToolbar();
	m_EditModeWasOn = true;
	DrawNew();
}

void ScribusView::togglePreview(bool inPreview)
{
	this->requestMode(modeNormal);
	deselectItems(true);
	undoManager->setUndoEnabled(false);
	m_canvas->m_viewMode.viewAsPreview = inPreview;
	m_doc->drawAsPreview = inPreview;
	bool recalc = false;
	m_doc->editOnPreview = false;
	m_ScMW->scrActions["viewEditInPreview"]->setChecked(false);
	m_AnnotChanged = false;
	m_EditModeWasOn = false;
	m_ChangedState = m_doc->isModified();

	if (inPreview)
	{
		m_ScMW->scrActions["viewEditInPreview"]->setEnabled(true);
		storedFramesShown = m_doc->guidesPrefs().framesShown;
		m_doc->guidesPrefs().framesShown = false;
		storedShowControls = m_doc->guidesPrefs().showControls;
		m_doc->guidesPrefs().showControls = false;
		m_canvas->m_viewMode.previewVisual = 0;
		m_doc->previewVisual = 0;
		// warning popping up in case colour management and out-of-gamut-display are active
		// as from #4346: Add a preview for daltonian - PV
		if (m_doc->HasCMS && m_doc->Gamut)
		{
			ScMessageBox::information(m_ScMW, tr("Preview Mode"),
									  "<qt>" + tr("Out of gamut colors display is active. Therefore the color display may not match the perception by visually impaired. Disable the option \"Mark colors out of gamut\" in Document Setup > Color Management if this function is not required.") + "</qt>");
		}
	}
	else
	{
		if (m_AnnotChanged)
			m_doc->ResetFormFields();
		m_ScMW->scrActions["viewEditInPreview"]->setEnabled(false);
		m_doc->guidesPrefs().framesShown = storedFramesShown;
		m_doc->guidesPrefs().showControls = storedShowControls;
		m_canvas->m_viewMode.previewVisual = 0;
		m_doc->previewVisual = 0;
		if (m_ScMW->viewToolBar->visualMenu->currentIndex() != m_doc->previewVisual)
			recalc = true;
		m_ScMW->viewToolBar->setDoc(m_doc);
	}
	m_ScMW->appModeHelper->setPreviewMode(inPreview);
	m_ScMW->setPreviewToolbar();
	m_ScMW->viewToolBar->setViewPreviewMode(inPreview);
	ScGuardedPtr<ScribusDoc> docPtr = m_doc->guardedPtr();
	if (recalc)
	{
		m_doc->recalculateColors();
		m_doc->recalcPicturesRes();
	}
	if (docPtr) // document may have been destroyed in-between
	{
		DrawNew();
		if ((!m_EditModeWasOn) && (!m_AnnotChanged))
			m_doc->setModified(m_ChangedState);
	}
	undoManager->setUndoEnabled(true);
}

void ScribusView::changed(QRectF re, bool)
{
	double scale = m_canvas->scale();
	int newCanvasWidth  = qRound((m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x()) * scale);
	int newCanvasHeight = qRound((m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y()) * scale);
	QSize newCanvasSize(newCanvasWidth, newCanvasHeight);
	if (!re.isValid() && /* don't check this all the time */
			(m_oldCanvasSize != newCanvasSize))
	{
		QSize maxViewport = maximumViewportSize();
		horizontalScrollBar()->setRange(0, newCanvasWidth - maxViewport.width());
		verticalScrollBar()->setRange(0, newCanvasHeight - maxViewport.height());
		/*	qDebug() << "adjustCanvas [" << m_oldCanvasSize.width() << m_oldCanvasSize.height() << " ] -> [" << newCanvasWidth << newCanvasHeight 
				<< "] (" << Doc->minCanvasCoordinate.x() << Doc->minCanvasCoordinate.y() << ") - ("
				<< Doc->maxCanvasCoordinate.x() << Doc->maxCanvasCoordinate.y() << ") @" << scale << maxViewport;
		*/
		widget()->resize(newCanvasWidth, newCanvasHeight);
		m_oldCanvasSize = newCanvasSize;
	}
	if (!m_doc->isLoading() && !m_ScMW->scriptIsRunning())
	{
// 		qDebug() << "ScribusView-changed(): changed region:" << re;
		m_canvas->setForcedRedraw(true);
		updateCanvas(re);
	}
}

bool ScribusView::handleObjectImport(QMimeData* mimeData, TransactionSettings* trSettings)
{
	requestMode(modeImportObject);
	CanvasMode_ObjImport* objImport = qobject_cast<CanvasMode_ObjImport*>(m_canvasMode);
	if (objImport)
	{
		objImport->setMimeData(mimeData);
		objImport->setTransactionSettings(trSettings);
		return true;
	}
	delete trSettings;
	delete mimeData;
	return false;
}

void ScribusView::startGesture(CanvasGesture* gesture)
{
	//qDebug() << "start gesture" << typeid(*m_canvasMode).name()
	//		<< "---->"
	//		<< typeid(*gesture).name();
	if (m_canvasMode != gesture)
	{
		m_canvasMode->deactivate(true);
		gesture->setDelegate(m_canvasMode);
		m_canvasMode = gesture;
		m_canvasMode->activate(false);
		if (m_doc->appMode != modeEditClip)
			m_canvas->repaint();
	}
}

void ScribusView::stopGesture()
{
	//qDebug() << "stop gesture" << typeid(*m_canvasMode).name() << (m_canvasMode->delegate() != 0);
	if (m_canvasMode->delegate())
	{
		m_canvasMode->deactivate(false);
		m_canvasMode = m_canvasMode->delegate();
		m_canvasMode->activate(true);
		if (PrefsManager::instance().appPrefs.uiPrefs.stickyTools)
		{
			m_canvas->setForcedRedraw(true);
			//			Doc->m_Selection->clear();
			//			emit HaveSel();
			m_canvas->resetRenderMode();
			updateContents();
		}
		else
			m_canvas->repaint();
	}
}

/**
switches between appmodes:
 - for submodes, activate the appropriate dialog or palette
 - set a new CanvasMode if necessary
 - call ScribusMainWindow::setAppMode(), which de/activates actions
 */
void ScribusView::requestMode(int appMode)
{
	bool updateNecessary = false;
	//	qDebug() << "request mode:" << appMode;
	switch (appMode) // filter submodes
	{
		case submodePaintingDone:   // return to normal mode
			appMode = modeNormal;
			m_previousMode = -1;
			updateNecessary = true;
			break;
		case submodeEndNodeEdit:     // return from node/shape editing
			appMode = modeNormal;
			m_previousMode = -1;
			updateNecessary = true;
			break;
		case submodeLoadPic:         // open GetImage dialog
			m_ScMW->slotGetContent();
			appMode = m_doc->appMode;
			m_previousMode = appMode;
			break;
		case submodeStatusPic:       // open ManageImages dialog
			appMode = m_doc->appMode;
			m_previousMode = appMode;
			m_ScMW->StatusPic();
			break;
		case submodeEditExternal:    // open external image editor
			appMode = m_doc->appMode;
			m_previousMode = appMode;
			m_ScMW->callImageEditor();
			break;
		case submodeAnnotProps:
			appMode = m_doc->appMode;
			m_previousMode = appMode;
			m_ScMW->ModifyAnnot();
			break;
		case submodeEditSymbol:
			appMode = m_doc->appMode;
			m_previousMode = appMode;
			m_ScMW->editSelectedSymbolStart();
			break;
		default:
			if (appMode < 0 || appMode > submodeFirstSubmode)
				appMode = modeNormal;
			m_previousMode = appMode;
			break;
	}

	if (m_doc->appMode != appMode)
	{
		m_ScMW->appModeHelper->setApplicationMode(m_ScMW, m_doc, appMode);
		CanvasMode* newCanvasMode = modeInstances.value(appMode);
		if (!newCanvasMode)
		{
			newCanvasMode = CanvasMode::createForAppMode(this, appMode);
			modeInstances[appMode] = newCanvasMode;
		}
		if (newCanvasMode)
		{
			m_canvasMode->deactivate(false);
			m_canvasMode = newCanvasMode;
			m_canvasMode->activate(false);
		}
		updateNecessary = true;
	}
	else
	{
		m_ScMW->appModeHelper->setApplicationMode(m_ScMW, m_doc, appMode);
	}
	if (updateNecessary)
		updateCanvas();
}

void ScribusView::enterEvent(QEvent * e)
{
	m_canvasMode->enterEvent(e);
}

void ScribusView::leaveEvent(QEvent *e)
{
	m_canvasMode->leaveEvent(e);
}

void ScribusView::contentsDragEnterEvent(QDragEnterEvent *e)
{
	QString text;
	bool /* dataFound = false, */ fromFile = false;
	const ScElemMimeData* elemData = qobject_cast<const ScElemMimeData*>(e->mimeData());
	if (elemData)
		text = elemData->scribusElem();
	else if (e->mimeData()->hasUrls())
	{
		QList<QUrl> urls = e->mimeData()->urls();
		if (!urls.isEmpty())
		{
			QUrl url = urls[0];
			QFileInfo fi(url.toLocalFile());
			if (fi.exists())
			{
				fromFile = true;
				text = url.toLocalFile();
			}
		}
	}

	bool hasSupportedFormat = !text.isEmpty();
	hasSupportedFormat |= e->mimeData()->hasFormat("text/inline");
	hasSupportedFormat |= e->mimeData()->hasFormat("text/symbol");
	hasSupportedFormat |= e->mimeData()->hasUrls();
	if (!hasSupportedFormat)
	{
		e->ignore();
		return;
	}

	e->accept();
	e->acceptProposedAction();

	if (text.isEmpty())
		return;

	double gx, gy, gw, gh;
	ScriXmlDoc ss;
	if (ss.readElemHeader(text, fromFile, &gx, &gy, &gw, &gh))
	{
		FPoint dragPosDoc = m_canvas->globalToCanvas(widget()->mapToGlobal(e->pos()));
		dragX = dragPosDoc.x();
		dragY = dragPosDoc.y();
		dragW = gw;
		dragH = gh;
		DraggedGroup = true;
		getDragRectScreen(&gx, &gy, &gw, &gh);
		//		QPoint evP = viewport()->mapToGlobal(e->pos());
		//		evP -= QPoint(contentsX(), contentsY());
		//		redrawMarker->setGeometry(QRect(evP.x() + 1, evP.y() + 1, qRound(gw), qRound(gh)).normalized());
		//		if (!redrawMarker->isVisible())
		//			redrawMarker->show();
		emit ItemGeom();
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
			emit MousePos(dragX, dragY); //+Doc->minCanvasCoordinate.x(), dragY+Doc->minCanvasCoordinate.y());
			QPoint pos = m_canvas->canvasToLocal(dragPosDoc);
			horizRuler->draw(pos.x());
			vertRuler->draw(pos.y());
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
			item = SelItem.at(0);
			if (img)
			{
				if (item->PType != 2)
					Deselect(true);
			}
			else
			{
				if (item->PType != 4)
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
	UndoTransaction activeTransaction;
	bool img = false;
	bool selectedItemByDrag = false;
	int re = 0;

	m_canvas->resetRenderMode();
	e->accept();
	
	FPoint dropPosDoc = m_canvas->globalToCanvas(widget()->mapToGlobal(e->pos()));
	QPointF dropPosDocQ(dropPosDoc.x(), dropPosDoc.y());

	//Loop through all items and see which one(s) were under the drop point on the current layer
	//Should make a nice function for this.
	//#9051: loop in reverse order so that items in front of others are prioritized
	m_doc->m_Selection->delaySignalsOn();
	for (int i = m_doc->Items->count() - 1; i >= 0 ; --i)
	{
		PageItem* item = m_doc->Items->at(i);
		if (item->m_layerID != m_doc->activeLayer())
			continue;
		if ((m_doc->masterPageMode())  && (!((item->OwnPage == -1) || (item->OwnPage == m_doc->currentPage()->pageNr()))))
			continue;
		if (m_canvas->frameHitTest(dropPosDocQ, item) >= Canvas::INSIDE)
		{
			deselectItems(false);
			m_doc->m_Selection->addItem(item);
			selectedItemByDrag = true;
			break;
		}
	}
	m_doc->m_Selection->delaySignalsOff();

	QStringList imageFormatList;
	QList<QByteArray> qImageReaderSupportedFormatList = QImageReader::supportedImageFormats();
	for (int i = 0; i < qImageReaderSupportedFormatList.count(); ++i )
	{
		imageFormatList.append(QString(qImageReaderSupportedFormatList.at(i)).toUpper());
	}
	QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::IMAGESIMGFRAME, 1).toUpper());
	imageFormatList += formatD.split("|");

	if (e->mimeData()->hasUrls())
	{
		if ((e->mimeData()->urls().count() == 1) || selectedItemByDrag)
		{
			url = e->mimeData()->urls().at(0);
			text.clear();
		}
		else
		{
			int dropOffsetX = 0;
			int dropOffsetY = 0;
			QList<QUrl> fileUrls = e->mimeData()->urls();
			for (int a = 0; a < fileUrls.count(); a++)
			{
				url = fileUrls[a];
				QFileInfo fi(url.toLocalFile());
				QString ext = fi.suffix().toUpper();
				if (ext == "JPG")
					ext = "JPEG";
				if (!imageFormatList.contains(ext) || !fi.exists() || selectedItemByDrag)
					continue;
				int z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, dropPosDoc.x() + dropOffsetX, dropPosDoc.y() + dropOffsetY, 1, 1, m_doc->itemToolPrefs().shapeLineWidth, m_doc->itemToolPrefs().imageFillColor, m_doc->itemToolPrefs().imageStrokeColor);
				PageItem *item = m_doc->Items->at(z);
				item->m_layerID = m_doc->activeLayer();
				m_doc->loadPict(url.toLocalFile(), item);
				double iw = static_cast<double>(item->OrigW * 72.0 / item->pixm.imgInfo.xres);
				double ih = static_cast<double>(item->OrigH * 72.0 / item->pixm.imgInfo.yres);
				if (iw > ih)
				{
					double pw = m_doc->currentPage()->width();
					if (iw > pw)
					{
						ih = pw * (ih / iw);
						iw = pw;
					}
				}
				else
				{
					double ph = m_doc->currentPage()->height();
					if (ih > ph)
					{
						iw = ph * (iw / ih);
						ih = ph;
					}
				}
				item->setWidth(iw);
				item->setHeight(ih);
				item->OldB2 = item->width();
				item->OldH2 = item->height();
				item->updateClip();
				item->adjustPictScale();
				item->update();
				dropOffsetX += m_doc->opToolPrefs().dispX;
				dropOffsetY += m_doc->opToolPrefs().dispY;
			}
			emit DocChanged();
			update();
			return;
		}
	}
	else if (e->mimeData()->hasFormat("application/x-scribus-elem"))
	{
		const ScElemMimeData* scMimeData = qobject_cast<const ScElemMimeData*>(e->mimeData());
		if (scMimeData)
			text = scMimeData->scribusElem();
	}
	else if (e->mimeData()->hasFormat("text/symbol"))
	{
		e->acceptProposedAction();
		activateWindow();
		if (!m_ScMW->scriptIsRunning())
			raise();
		m_ScMW->newActWin((dynamic_cast<ScribusWin*>(m_doc->WinHan))->getSubWin());
		updateContents();
		QString patternVal = e->mimeData()->data("text/symbol");
		m_doc->m_Selection->delaySignalsOn();
		for (int i = m_doc->Items->count() - 1; i >= 0 ; --i)
		{
			PageItem* item = m_doc->Items->at(i);
			if (item->m_layerID != m_doc->activeLayer())
				continue;
			if ((m_doc->masterPageMode()) && (!((item->OwnPage == -1) || (item->OwnPage == m_doc->currentPage()->pageNr()))))
				continue;
			if ((m_canvas->frameHitTest(dropPosDocQ, item) >= Canvas::INSIDE) && (item->itemType() == PageItem::Symbol))
			{
				deselectItems(false);
				m_doc->m_Selection->addItem(item);
				item->setPattern(patternVal);
				selectedItemByDrag = true;
				break;
			}
		}
		m_doc->m_Selection->delaySignalsOff();
		if (!selectedItemByDrag)
		{
			int z = m_doc->itemAdd(PageItem::Symbol, PageItem::Unspecified, dropPosDoc.x(), dropPosDoc.y(), 1, 1, 0, CommonStrings::None, CommonStrings::None);
			PageItem *item = m_doc->Items->at(z);
			item->m_layerID = m_doc->activeLayer();
			ScPattern pat = m_doc->docPatterns[patternVal];
			item->setWidth(pat.width);
			item->setHeight(pat.height);
			item->OldB2 = item->width();
			item->OldH2 = item->height();
			item->setPattern(patternVal);
			item->updateClip();
			deselectItems(false);
			m_doc->m_Selection->addItem(item);
		}
		emit DocChanged();
		update();
		return;
	}
	else if (e->mimeData()->hasFormat("text/inline"))
	{
		if (((m_doc->appMode == modeEditTable) || (m_doc->appMode == modeEdit)) && (!m_doc->m_Selection->isEmpty()))
		{
			PageItem *item = m_doc->m_Selection->itemAt(0);
			if (item->isTextFrame() || item->isTable())
			{
				e->acceptProposedAction();
				activateWindow();
				if (!m_ScMW->scriptIsRunning())
					raise();
				m_ScMW->newActWin((dynamic_cast<ScribusWin*>(m_doc->WinHan))->getSubWin());
				updateContents();
				QString patternVal = e->mimeData()->data("text/inline");
				int id = patternVal.toInt();
				PageItem_TextFrame *cItem;
				if (m_doc->appMode == modeEditTable)
					cItem = item->asTable()->activeCell().textFrame();
				else
					cItem = item->asTextFrame();
				if (cItem->HasSel)
					cItem->deleteSelectedTextFromFrame();
				cItem->invalidateLayout(false);
				cItem->itemText.insertObject(id);
				if (item->isTable())
					item->asTable()->update();
				else
					item->update();
				emit DocChanged();
				update();
				return;
			}
		}
	}

	//	qDebug() << "ScribusView::contentsDropEvent" << e->mimeData()->formats() << url;
	if (url.isEmpty() && text.isEmpty())
	{
		e->ignore();
		return;
	}

	e->acceptProposedAction();
	//<<#3524
	activateWindow();
	if (!m_ScMW->scriptIsRunning())
		raise();

	ScribusWin* sw = dynamic_cast<ScribusWin*>(m_doc->WinHan);
	if (!sw)
	{
		qFatal("ScribusView::contentsDropEvent !sw");
		return;
	}
	m_ScMW->newActWin(sw->getSubWin());
	updateContents();
	//>>
	QFileInfo fi;
	QString ext;
	if (!e->mimeData()->hasFormat("application/x-scribus-elem"))
	{
		fi.setFile(url.toLocalFile());
		ext = fi.suffix().toUpper();
	}
	if (ext == "JPG")
		ext = "JPEG";
	img = imageFormatList.contains(ext);
	bool vectorFile = false;
	if (fi.exists())
	{
		if (fi.suffix().toLower() == "sce")
			vectorFile = true;
		else
		{
			FileLoader *fileLoader = new FileLoader(url.toLocalFile());
			int testResult = fileLoader->testFile();
			delete fileLoader;
			if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
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
	if ((fi.exists()) && img && !selectedItemByDrag && !vectorFile)// && (!SeleItemPos(e->pos())))
	{
		int z = m_doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified, dropPosDoc.x(), dropPosDoc.y(), 1, 1, m_doc->itemToolPrefs().shapeLineWidth, m_doc->itemToolPrefs().imageFillColor, m_doc->itemToolPrefs().imageStrokeColor);
		PageItem *item = m_doc->Items->at(z);
		item->m_layerID = m_doc->activeLayer();
		m_doc->loadPict(url.toLocalFile(), item);

		double iw = static_cast<double>(item->OrigW * 72.0 / item->pixm.imgInfo.xres);
		double ih = static_cast<double>(item->OrigH * 72.0 / item->pixm.imgInfo.yres);
		if (iw > ih)
		{
			double pw = m_doc->currentPage()->width();
			if (iw > pw)
			{
				ih = pw * (ih / iw);
				iw = pw;
			}
		}
		else
		{
			double ph = m_doc->currentPage()->height();
			if (ih > ph)
			{
				iw = ph * (iw / ih);
				ih = ph;
			}
		}

		item->setWidth(iw);
		item->setHeight(ih);
		item->OldB2 = item->width();
		item->OldH2 = item->height();
		item->updateClip();
		item->adjustPictScale();
		item->update();
		emit DocChanged();
		update();
		return;
	}
	if (selectedItemByDrag && (m_canvas->frameHitTest(dropPosDocQ, m_doc->m_Selection->itemAt(0)) >= Canvas::INSIDE) && (!vectorFile || img))
	{
		PageItem *item = m_doc->m_Selection->itemAt(0);
		if (item->itemType() == PageItem::ImageFrame)
		{
			if (fi.exists() && img)
				m_doc->loadPict(url.toLocalFile(), item);
		}
		else if (item->itemType() == PageItem::TextFrame)
		{
			if (fi.exists() && !img)
			{
				gtGetText* gt = new gtGetText(m_doc);
				QStringList exts = gt->getSupportedTypes();
				if (exts.contains(fi.suffix().toLower()))
				{
					ImportSetup impsetup;
					impsetup.runDialog = true;
					impsetup.encoding = "";
					impsetup.prefixNames = true;
					impsetup.textOnly = false;
					impsetup.importer = -1;
					impsetup.filename = url.toLocalFile();
					if (item->itemText.length() != 0)
					{
						int t = ScMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your text?"),
														QMessageBox::Yes | QMessageBox::No,
														QMessageBox::No,	// GUI default
														QMessageBox::Yes);	// batch default
						if (t == QMessageBox::No)
						{
							delete gt;
							return;
						}
					}
					gt->launchImporter(impsetup.importer, impsetup.filename, impsetup.textOnly, impsetup.encoding, false, impsetup.prefixNames, item);
					m_ScMW->updateFromDrop();
				}
				else
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
						item->itemText.insertChars(txt, true);
					}
				}
				if (m_doc->docHyphenator->autoCheck())
					m_doc->docHyphenator->slotHyphenate(item);
				item->invalidateLayout();
				item->update();
				delete gt;
			}
		}
		emit DocChanged();
		update();
	}
	else
	{
		deselectItems(true);
		int oldDocItemCount = m_doc->Items->count();
		if ((!img || vectorFile) && (m_doc->DraggedElem == nullptr))
		{
			activeTransaction = undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Create, "", Um::ICreate);
			bool fromScrapbook = false;
			if (fi.exists())
			{
				if (fi.suffix().toLower() == "sce")
				{
					fromScrapbook = true;
					emit LoadElem(url.toLocalFile(), dropPosDoc.x(), dropPosDoc.y(), true, false, m_doc, this);
				}
				else
				{
					FileLoader *fileLoader = new FileLoader(url.toLocalFile());
					int testResult = fileLoader->testFile();
					delete fileLoader;
					if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
					{
						const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
						if (fmt)
						{
							// We disable undo here as we are only interested by the item creation undo actions
							// We create them manually after import
							undoManager->setUndoEnabled(false);
							m_doc->dontResize = true;
							fmt->loadFile(url.toLocalFile(), LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
							undoManager->setUndoEnabled(true);
							if (m_doc->m_Selection->count() > 0)
							{
								if (UndoManager::undoEnabled())
								{
									// Create undo actions for created items
									for (int i = 0; i < m_doc->m_Selection->count(); ++i)
									{
										PageItem* newItem = m_doc->m_Selection->itemAt(i);
										ScItemState<PageItem*> *is = new ScItemState<PageItem*>("Create PageItem");
										is->set("CREATE_ITEM");
										is->setItem(newItem);
										//Undo target rests with the Page for object specific undo
										int pindex = (newItem->OwnPage > -1) ? newItem->OwnPage : 0;
										UndoObject *target = m_doc->Pages->at(pindex);
										undoManager->action(target, is);
									}
								}
								double x2, y2, w, h;
								// We disable undo temporarily as move actions are not necessary
								// to perform undo correctly here
								undoManager->setUndoEnabled(false);
								m_doc->m_Selection->getGroupRect(&x2, &y2, &w, &h);
								m_doc->moveGroup(dropPosDoc.x() - x2, dropPosDoc.y() - y2);
								m_ScMW->requestUpdate(reqColorsUpdate | reqSymbolsUpdate | reqTextStylesUpdate | reqLineStylesUpdate);
								undoManager->setUndoEnabled(true);
							}
							m_doc->dontResize = false;
						}
					}
				}
			}
			else
			{
				emit LoadElem(QString(text), dropPosDoc.x(), dropPosDoc.y(), false, false, m_doc, this);
			}
			Selection tmpSelection(this, false);
			tmpSelection.copy(*m_doc->m_Selection, true);
			for (int i = oldDocItemCount; i < m_doc->Items->count(); ++i)
			{
				currItem = m_doc->Items->at(i);
				m_doc->setRedrawBounding(currItem);
				tmpSelection.addItem(currItem, true);
				if (currItem->isBookmark)
					emit AddBM(currItem);
			}
			m_doc->m_Selection->copy(tmpSelection, false);
			if (!fromScrapbook && m_doc->m_Selection->count() == 1)
			{
				PageItem *newItem = m_doc->m_Selection->itemAt(0);
				if ((newItem->width() > m_doc->currentPage()->width()) || (newItem->height() > m_doc->currentPage()->height()))
				{
					m_doc->rescaleGroup(newItem, qMin(qMin(m_doc->currentPage()->width() / newItem->width(), m_doc->currentPage()->height() / newItem->height()), 1.0));
					newItem->update();
				}
			}
			activeTransaction.commit();
			activeTransaction.reset();
		}
		else
		{
			if (m_doc->DraggedElem != nullptr)
			{
				if (!m_doc->leaveDrag)
				{
					QMenu *pmen = new QMenu();
					pmen->addAction( tr("Copy Here"));
					QAction* mov = pmen->addAction( tr("Move Here"));
					pmen->addAction( tr("Cancel"));
					for (int i=0; i<m_doc->DragElements.count(); ++i)
					{
						if (m_doc->DragElements[i]->locked())
						{
							mov->setEnabled(false);
							break;
						}
					}
					re = pmen->actions().indexOf(pmen->exec(QCursor::pos()));
					delete pmen;
					pmen = nullptr;
				}
				else
					re = 1;
				if ((re == 2) || (re == -1))
				{
					updateContents();
					return;
				}
				if ((re == 1) || (m_doc->leaveDrag))
				{
					QList<PageItem*> pasted;
					emit LoadElem(QString(text), dropPosDoc.x(), dropPosDoc.y(), false, false, m_doc, this);
					for (int i = oldDocItemCount; i < m_doc->Items->count(); ++i)
					{
						pasted.append(m_doc->Items->at(i));
					}
					Selection tmpSelection(this, false);
					tmpSelection.copy(*m_doc->m_Selection, true);
					for (int i = 0; i < m_doc->DragElements.count(); ++i)
					{
						tmpSelection.addItem(m_doc->DragElements[i], true);
					}
					m_doc->m_Selection->copy(tmpSelection, false);
					PageItem* bb;
					int fin;
					for (int i = 0; i < m_doc->DragElements.count(); ++i)
					{
						bb = pasted.at(i);
						currItem = m_doc->m_Selection->itemAt(i);
						if ((currItem->isTextFrame()) && ((currItem->nextInChain() != nullptr) || (currItem->prevInChain() != nullptr)))
						{
							PageItem* before = currItem->prevInChain();
							PageItem* after = currItem->nextInChain();
							currItem->unlink();
							if (before != nullptr)
							{
								fin = m_doc->m_Selection->findItem(before);
								if (fin != -1)
									before = pasted.at(fin);
								before->unlink();
								before->link(bb);
							}
							if (after != nullptr)
							{
								fin = m_doc->m_Selection->findItem(after);
								if (fin != -1)
									after = pasted.at(fin);
								bb->link(after);
							}
						}
					}
					pasted.clear();
					m_doc->itemSelection_DeleteItem();
				}
			}
			if (!img && (re == 0))
				emit LoadElem(QString(text), dropPosDoc.x(), dropPosDoc.y(), false, false, m_doc, this);
			m_doc->DraggedElem = nullptr;
			m_doc->DragElements.clear();
			Selection tmpSelection(this, false);
			tmpSelection.copy(*m_doc->m_Selection, true);
			for (int i = oldDocItemCount; i < m_doc->Items->count(); ++i)
			{
				currItem = m_doc->Items->at(i);
				m_doc->setRedrawBounding(currItem);
				tmpSelection.addItem(currItem, true);
				if (currItem->isBookmark)
					emit AddBM(currItem);
			}
			m_doc->m_Selection->copy(tmpSelection, false);
		}
		if (m_doc->m_Selection->count() > 1)
		{
			m_doc->m_Selection->connectItemToGUI();
			double gx, gy, gh, gw;
			m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			double nx = gx;
			double ny = gy;
			if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
			{
				FPoint npx;
				npx = m_doc->ApplyGridF(FPoint(nx, ny));
				nx = npx.x();
				ny = npx.y();
			}
			activeTransaction = undoManager->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::Move, "", Um::IMove);
			m_doc->moveGroup(nx - gx, ny - gy);
			m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			nx = gx + gw;
			ny = gy + gh;
			m_doc->ApplyGuides(&nx, &ny);
			m_doc->ApplyGuides(&nx, &ny,true);
			m_doc->moveGroup(nx - (gx + gw), ny - (gy + gh));
			m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
			for (int i = 0; i < m_doc->m_Selection->count(); ++i)
			{
				PageItem *currItem = m_doc->m_Selection->itemAt(i);
				currItem->m_layerID = m_doc->activeLayer();
				currItem->gXpos = currItem->xPos() - gx;
				currItem->gYpos = currItem->yPos() - gy;
				currItem->gWidth = gw;
				currItem->gHeight = gh;
			}
			activeTransaction.commit();
			activeTransaction.reset();
			emit ItemGeom();
		}
		else if (m_doc->m_Selection->count() == 1)
		{
			m_doc->m_Selection->connectItemToGUI();
			currItem = m_doc->m_Selection->itemAt(0);
			currItem->m_layerID = m_doc->activeLayer();
			if (m_doc->SnapGrid)
			{
				double nx = currItem->xPos();
				double ny = currItem->yPos();
				if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
				{
					FPoint npx;
					npx = m_doc->ApplyGridF(FPoint(nx, ny));
					nx = npx.x();
					ny = npx.y();
				}
				m_doc->moveItem(nx - currItem->xPos(), ny - currItem->yPos(), currItem);
			}
		}
		if ((m_doc->m_Selection->count() > 0) && (m_doc->appMode != modeNormal))
			this->requestMode(modeNormal);
		updateContents();
	}
	if (!m_doc->masterPageMode())
	{
		int docPagesCount = m_doc->Pages->count();
		int docCurrPageNo = m_doc->currentPageNumber();
		for (int i = 0; i < docPagesCount; ++i)
		{
			double x = m_doc->Pages->at(i)->xOffset();
			double y = m_doc->Pages->at(i)->yOffset();
			double w = m_doc->Pages->at(i)->width();
			double h = m_doc->Pages->at(i)->height();
			if (QRectF(x, y, w, h).contains(dropPosDocQ))
			{
				if (docCurrPageNo != i)
				{
					m_doc->setCurrentPage(m_doc->Pages->at(i));
					m_ScMW->slotSetCurrentPage(i);
					DrawNew();
				}
				break;
			}
		}
		setRulerPos(contentsX(), contentsY());
	}
}

void ScribusView::getDragRectScreen(double *x, double *y, double *w, double *h)
{
	QPoint in(qRound(dragX * m_canvas->scale()), qRound(dragY * m_canvas->scale()));
	//	in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
	QPoint out = contentsToViewport(in);
	*x = static_cast<double>(out.x());
	*y = static_cast<double>(out.y());
	*w = dragW * m_canvas->scale();
	*h = dragH * m_canvas->scale();
}

void ScribusView::getGroupRectScreen(double *x, double *y, double *w, double *h)
{
	double gx, gy, gh, gw;
	m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
	QPoint in(qRound(gx * m_canvas->scale()), qRound(gy * m_canvas->scale()));
	//	in -= QPoint(qRound(Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(Doc->minCanvasCoordinate.y() * m_canvas->scale()));
	QPoint out = contentsToViewport(in);
	*x = static_cast<double>(out.x());
	*y = static_cast<double>(out.y());
	*w = gw * m_canvas->scale();
	*h = gh * m_canvas->scale();
}



void ScribusView::RefreshGradient(PageItem *currItem, double dx, double dy)
{
	QTransform matrix;
	QRect rect = currItem->getRedrawBounding(m_canvas->scale());
	m_canvas->Transform(currItem, matrix);
	FPointArray fpNew;
	if (editStrokeGradient)
		fpNew.setPoints(2, currItem->GrStrokeStartX, currItem->GrStrokeStartY, currItem->GrStrokeEndX, currItem->GrStrokeEndY);
	else
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
		for (int i = an.y(); i < en.y(); ++i)
		{
			if (MArea.contains(an.x(), i))
				return true;
		}
	}
	if (Start.y() == End.y())
	{
		an = Start.x() > End.x() ? End : Start;
		en = an == End ? Start : End;
		for (int i = an.x(); i < en.x(); ++i)
		{
			if (MArea.contains(i, an.y()))
				return true;
		}
	}
	if (abs(Start.x() - End.x()) > abs(Start.y() - End.y()))
	{
		an = Start.x() > End.x() ? End : Start;
		en = an == End ? Start : End;
		double stg = (en.y() - an.y()) / static_cast<double>((en.x() - an.x()));
		for (int i = an.x(); i < en.x(); ++i)
		{
			if (MArea.contains(i, an.y() + qRound((i - an.x()) * stg)))
				return true;
		}
	}
	else
	{
		an = Start.y() > End.y() ? End : Start;
		en = an == End ? Start : End;
		double stg = (en.x() - an.x()) / static_cast<double>((en.y() - an.y()));
		for (int i = an.y(); i < en.y(); ++i)
		{
			if (MArea.contains(an.x() + qRound((i - an.y()) * stg), i))
				return true;
		}
	}
	return false;
}

//CB-->Doc??
void ScribusView::TransformPoly(int mode, int rot, double scaling)
{
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	currItem->ClipEdited = true;
	QTransform ma;
	undoManager->setUndoEnabled(false);
	if (m_doc->nodeEdit.isContourLine())
	{
		FPoint tp2(getMinClipF(&currItem->ContourLine));
		FPoint tp(getMaxClipF(&currItem->ContourLine));
		FPoint tpS = currItem->ContourLine.widthHeight();
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
				ma.scale(1.0 - (scaling / tpS.x()), 1.0 - (scaling / tpS.y()));
				break;
			case 9:
				ma.scale(1.0 + (scaling / tpS.x()), 1.0 + (scaling / tpS.y()));
				break;
				//10-13 are for scaling the contour line in shape edit mode
			case 10:
				{
					double s = 1.0 - (scaling / (tp2.x() - tp.x()));
					ma.scale(s, 1);
					ma.translate(-scaling / s / 2, 0);
				}
				break;
			case 11:
				{
					double s = 1.0 - (scaling / (tp2.x() - tp.x()));
					ma.scale(s, 1);
					ma.translate(scaling / s / 2, 0);
				}
				break;
			case 12:
				{
					double s = 1.0 - (scaling / (tp2.y() - tp.y()));
					ma.scale(1, s);
					ma.translate(0, -scaling / s / 2);
				}
				break;
			case 13:
				{
					double s = 1.0 - (scaling/(tp2.y() - tp.y()));
					ma.scale(1, s);
					ma.translate(0, scaling / s / 2);
				}
				break;
		}
		currItem->ContourLine.map(ma);
		currItem->ContourLine.translate(qRound((tp.x() + tp2.x()) / 2.0), qRound((tp.y() + tp2.y()) / 2.0));
		updateContents();
		currItem->FrameOnly = true;
		m_doc->regionsChanged()->update(QRect());
		undoManager->setUndoEnabled(true);
		if (UndoManager::undoEnabled())
		{
			undoManager->setUndoEnabled(false);
			currItem->checkChanges(true);
			undoManager->setUndoEnabled(true);
			auto *ss = new SimpleState(Um::EditContourLine, "", Um::IBorder);
			ss->set("EDIT_CONTOUR");
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
	double oldWidth = currItem->width();
	double oldHeight = currItem->height();
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
			ma.scale(1.0 - (scaling / oldWidth), 1.0 - (scaling / oldHeight));
			break;
		case 9:
			ma.scale(1.0 + (scaling / oldWidth), 1.0 + (scaling / oldHeight));
			break;
	}
	currItem->PoLine.map(ma);
	currItem->PoLine.translate(offsX, offsY);
	m_doc->adjustItemSize(currItem);
	QTransform ma2;
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
			ma2.scale(1.0 - (scaling / oldWidth), 1.0 - (scaling / oldHeight));
			break;
		case 9:
			ma2.scale(1.0 + (scaling / oldWidth), 1.0 + (scaling / oldHeight));
			break;
	}
	double x = ma2.m11() * n.x() + ma2.m21() * n.y() + ma2.dx();
	double y = ma2.m22() * n.y() + ma2.m12() * n.x() + ma2.dy();
	m_doc->moveItem(x-oldPos.x(), y-oldPos.y(), currItem);
	if (currItem->isPathText())
		currItem->updatePolyClip();
	m_doc->setRedrawBounding(currItem);
	m_doc->regionsChanged()->update(QRect());
	currItem->update();
	currItem->FrameType = 3;
	undoManager->setUndoEnabled(true);
	if (UndoManager::undoEnabled())
	{
		undoManager->setUndoEnabled(false);
		currItem->checkChanges(true);
		undoManager->setUndoEnabled(true);
		auto *ss = new SimpleState(Um::EditShape, "", Um::IBorder);
		ss->set("EDIT_SHAPE");
		ss->set("MODE", mode);
		ss->set("ROT", rot);
		ss->set("SCALING", scaling);
		undoManager->action(currItem, ss);
	}
	emit DocChanged();
}

bool ScribusView::slotSetCurs(int x, int y)
{
	PageItem *item;
	if (!m_doc->getItem(&item))
		return false;

	PageItem_TextFrame *textFrame;
	QPointF canvasPoint;
	QTransform mm = item->getTransform();
	QPointF textFramePoint = mm.map(QPointF(0, 0));
	if (item->isTextFrame())
	{
		textFrame = item->asTextFrame();
		canvasPoint = m_canvas->globalToCanvas(QPoint(x,y)).toQPointF();
	}
	else if (item->isTable())
	{
		// Move to cell under cursor and position the text cursor.
		PageItem_Table *table = item->asTable();
		QPointF tablePoint = m_canvas->globalToCanvas(QPoint(x, y)).toQPointF();
		table->moveTo(table->cellAt(tablePoint));
		textFrame = table->activeCell().textFrame();
		mm = textFrame->getTransform();
		canvasPoint = table->getTransform().inverted().map(tablePoint) - table->gridOffset();
	}
	else if (item->isImageFrame())
		return true;
	else
		return false;

	if (m_canvas->frameHitTest(canvasPoint, textFrame) == Canvas::INSIDE)
	{
		// #9592 : layout must be valid here, or screenToPosition() may crash
		if (textFrame->invalid)
			textFrame->layout();

		double px = canvasPoint.x() - textFramePoint.x();
		double py = canvasPoint.y() - textFramePoint.y();
		FPoint point(px, py);
		if (mm.isInvertible() && textFrame->itemText.length() > 0)
		{
			qreal tx = 0, ty = 0;
			mm.inverted().map(canvasPoint.x(), canvasPoint.y(), &tx, &ty);
			point.setXY(tx, ty);
		}
		if (textFrame->imageFlippedH())
			point.setX(textFrame->width() - point.x());
		if (textFrame->imageFlippedV())
			point.setY(textFrame->height() - point.y());
		if (textFrame->itemText.length() == 0)
			textFrame->itemText.setCursorPosition(0);
		else
		{
			int result = textFrame->textLayout.pointToPosition(point.toQPointF());
			if (result >= 0)
				textFrame->itemText.setCursorPosition(result);
		}

		if (textFrame->itemText.length() > 0)
		{
			int pos = qMax(0, qMin(textFrame->itemText.cursorPosition(), textFrame->itemText.length()));
			if (textFrame->itemText.hasSelection())
			{
				int firstSelected = textFrame->itemText.startOfSelection();
				int lastSelected  = qMax(textFrame->itemText.endOfSelection() - 1, 0);
				pos = qMax(firstSelected, qMin(pos, lastSelected));
			}
			m_doc->currentStyle.charStyle() = textFrame->currentCharStyle();
			emit ItemCharStyle(m_doc->currentStyle.charStyle());
			emit ItemTextEffects(m_doc->currentStyle.charStyle().effects());
			emit ItemTextAlign(textFrame->itemText.paragraphStyle(pos).alignment());
			return true;
		}
		m_doc->currentStyle.charStyle() = textFrame->itemText.defaultStyle().charStyle();
		emit ItemCharStyle(textFrame->itemText.defaultStyle().charStyle());
		emit ItemTextEffects(textFrame->itemText.defaultStyle().charStyle().effects());
		emit ItemTextAlign(0);
		return true;
	}
	return false;
}


void ScribusView::dragTimerTimeOut()
{
	m_dragTimerFired = true;
	// #0007865
	// 	qApp->changeOverrideCursor(QCursor(loadIcon("dragpix.png")));
}

Qt::CursorShape ScribusView::getResizeCursor(PageItem *currItem, QRect mpo, Qt::CursorShape cursorShape)
{
	QTransform ma;
	m_canvas->Transform(currItem, ma);
	QPoint tx = ma.map(QPoint(static_cast<int>(currItem->width()), 0));
	QPoint tx2 = ma.map(QPoint(0, static_cast<int>(currItem->height())));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(currItem->rotation());
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >=315.0) && (rr <= 360.0)))
			cursorShape = Qt::SizeBDiagCursor;
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			cursorShape = Qt::SizeFDiagCursor;
	}
	tx = ma.map(QPoint(static_cast<int>(currItem->width()), static_cast<int>(currItem->height())/2));
	tx2 = ma.map(QPoint(0, static_cast<int>(currItem->height())/2));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(currItem->rotation());
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			cursorShape = Qt::SizeHorCursor;
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			cursorShape = Qt::SizeVerCursor;
	}
	tx = ma.map(QPoint(static_cast<int>(currItem->width())/2, 0));
	tx2 = ma.map(QPoint(static_cast<int>(currItem->width())/2, static_cast<int>(currItem->height())));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		double rr = fabs(currItem->rotation());
		if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) || ((rr >= 315.0) && (rr <= 360.0)))
			cursorShape = Qt::SizeVerCursor;
		if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
			cursorShape = Qt::SizeHorCursor;
	}
	tx = ma.map(QPoint(static_cast<int>(currItem->width()), static_cast<int>(currItem->height())));
	tx2 = ma.map(QPoint(0, 0));
	if (mpo.contains(tx) || mpo.contains(tx2))
	{
		if (!currItem->sizeHLocked() && ! currItem->sizeVLocked())
		{
			double rr = fabs(currItem->rotation());
			if (((rr >= 0.0) && (rr < 45.0)) || ((rr >= 135.0) && (rr < 225.0)) ||
					((rr >= 315.0) && (rr <= 360.0)))
				cursorShape = Qt::SizeFDiagCursor;
			if (((rr >= 45.0) && (rr < 135.0)) || ((rr >= 225.0) && (rr < 315.0)))
				cursorShape = Qt::SizeBDiagCursor;
		}
	}
	return cursorShape;
}

void ScribusView::selectItemByNumber(int nr, bool draw, bool single)
{
	if (nr < m_doc->Items->count())
		selectItem(m_doc->Items->at(nr), draw, single);
}

//CB-->Doc/Fix
void ScribusView::selectItem(PageItem *currItem, bool draw, bool single)
{
	if (!currItem->isSelected())
	{
		if (single)
		{
			m_doc->m_Selection->addItem(currItem);
			currItem->isSingleSel = true;
			updateContents();
		}
		else
		{
			m_doc->m_Selection->addItem(currItem);
			if (draw)
				updateContents(currItem->getRedrawBounding(m_canvas->scale()));
		}
	}
	if (draw && !m_doc->m_Selection->isEmpty())
	{
		double x, y, w, h;
		m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
		getGroupRectScreen(&x, &y, &w, &h);
		updateContents();
		emit ItemGeom();
		emit HaveSel();
	}
}

//CB Remove bookmark interaction here, item/doc should do it
void ScribusView::deselectItems(bool /*prop*/)
{
	if (m_doc->m_Selection->isEmpty())
		return;

	const double scale = m_canvas->scale();
	PageItem* currItem = nullptr;
	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
	{
		currItem = m_doc->m_Selection->itemAt(i);
		if ((currItem->isTextFrame()) && (currItem->isBookmark))
			emit ChBMText(currItem);
	}
	if (!m_doc->m_Selection->isMultipleSelection())
	{
		currItem = m_doc->m_Selection->itemAt(0);
		if (currItem != nullptr)
		{
			currItem->itemText.deselectAll();
			currItem->HasSel = false;
		}
	}
	double x, y, w, h;
	m_doc->m_Selection->getVisualGroupRect(&x, &y, &w, &h);
	m_doc->m_Selection->clear();
	updateCanvas(x - 5 / scale, y - 5 / scale, w + 10 / scale, h + 10 / scale);
}

// FIXME:av -> CanvasMode_legacy / Doc
//CB Remove emit/start pasting objects
void ScribusView::PasteToPage()
{
	UndoTransaction activeTransaction;
	int ac = m_doc->Items->count();
	if (UndoManager::undoEnabled())
		activeTransaction = undoManager->beginTransaction(m_doc->currentPage()->getUName(), nullptr, Um::Paste, "", Um::IPaste);

	QString buffer = ScMimeData::clipboardScribusElem();
	emit LoadElem(buffer, dragX, dragY, false, false, m_doc, this);

	m_doc->DraggedElem = nullptr;
	m_doc->DragElements.clear();
	updateContents();
	Selection newObjects(this, false);
	for (int as = ac; as < m_doc->Items->count(); ++as)
	{
		PageItem* currItem = m_doc->Items->at(as);
		if (currItem->isBookmark)
			emit AddBM(currItem);
		newObjects.addItem(currItem);
		currItem->m_layerID = m_doc->activeLayer();
	}
	if (newObjects.count() > 1)
	{
		double gx, gy, gh, gw;
		newObjects.getGroupRect(&gx, &gy, &gw, &gh);
		m_doc->moveGroup(dragX - gx, dragY - gy, &newObjects);
		newObjects.getGroupRect(&gx, &gy, &gw, &gh);
		double nx = gx;
		double ny = gy;
		if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
		{
			FPoint npx;
			npx = m_doc->ApplyGridF(FPoint(nx, ny));
			nx = npx.x();
			ny = npx.y();
		}
		m_doc->moveGroup(nx - gx, ny - gy, &newObjects);
		newObjects.getGroupRect(&gx, &gy, &gw, &gh);
		nx = gx + gw;
		ny = gy + gh;
		m_doc->ApplyGuides(&nx, &ny);
		m_doc->ApplyGuides(&nx, &ny,true);
		m_doc->moveGroup(nx - (gx + gw), ny - (gy + gh), &newObjects);
		newObjects.getGroupRect(&gx, &gy, &gw, &gh);
		emit ItemGeom();
		emit HaveSel();
	}
	else if (newObjects.count() == 1)
	{
		PageItem *currItem = newObjects.itemAt(0);
		if (m_doc->SnapGrid)
		{
			double nx = currItem->xPos();
			double ny = currItem->yPos();
			if (!m_doc->ApplyGuides(&nx, &ny) && !m_doc->ApplyGuides(&nx, &ny,true))
			{
				FPoint npx;
				npx = m_doc->ApplyGridF(FPoint(nx, ny));
				nx = npx.x();
				ny = npx.y();
			}
			m_doc->moveItem(nx - currItem->xPos(), ny - currItem->yPos(), currItem);
		}
		currItem->emitAllToGUI();
	}
	else // newObjects.count() == 0
	{
		if (activeTransaction)
		{
			activeTransaction.cancel();
			activeTransaction.reset();
		}
		return;
	}
	newObjects.clear();
	if (activeTransaction)
	{
		activeTransaction.commit();
		activeTransaction.reset();
	}
	emit DocChanged();
}

void ScribusView::resizeEvent ( QResizeEvent * event )
{
	// 	qDebug() << "ScribusView::resizeEvent";
	QScrollArea::resizeEvent(event);
	horizRuler->setGeometry(m_vhRulerHW, 1, width()-m_vhRulerHW-1, m_vhRulerHW);
	vertRuler->setGeometry(1, m_vhRulerHW, m_vhRulerHW, height()-m_vhRulerHW-1);
	rulerMover->setGeometry(1, 1, m_vhRulerHW, m_vhRulerHW);
	if (clockLabel->isExpanded())
	{
		clockLabel->setGeometry(m_vhRulerHW + 1, height() - m_vhRulerHW - 61, 60, 60);
		clockLabel->setFixedSize(60, 60);
	}
	else
	{
		clockLabel->setGeometry(m_vhRulerHW + 1, height() - m_vhRulerHW - 16, 15, 15);
		clockLabel->setFixedSize(15, 15);
	}
	endEditButton->setGeometry(m_vhRulerHW + 1, height() - m_vhRulerHW - endEditButton->minimumSizeHint().height() - 1, endEditButton->minimumSizeHint().width(), endEditButton->minimumSizeHint().height());
	m_canvas->setForcedRedraw(true);
	m_canvas->resetRenderMode();
	// Per Qt doc, not painting should be done in a resizeEvent,
	// a paint event will be emitted right afterwards
	// m_canvas->update();
}

void ScribusView::setHBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x, y, w, h);
	if (m_ready)
		horizRuler->setGeometry(m_vhRulerHW, 1, w-m_vhRulerHW-1, m_vhRulerHW);
}

void ScribusView::setVBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x, y, w, h);
	if (m_ready)
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
	Selection* itemSelection = (customSelection != nullptr) ? customSelection : m_doc->m_Selection;
	assert(itemSelection != nullptr);
	int selectedItemCount = itemSelection->count();
	Q_ASSERT(selectedItemCount > 0);
	if (!m_groupTransaction)
	{
		QString tooltip = description;
		QString target = Um::SelectionGroup;
		QPixmap* targetIcon = Um::IGroup;
		if (tooltip.isEmpty() && selectedItemCount > 1)
		{
			if (selectedItemCount <= Um::ItemsInvolvedLimit)
			{
				tooltip = Um::ItemsInvolved + "\n";
				for (int i = 0; i < selectedItemCount; ++i)
					tooltip += "\t" + itemSelection->itemAt(i)->getUName() + "\n";
			}
			else
			{
				tooltip = Um::ItemsInvolved2 + "\n";
			}
		}
		if (selectedItemCount == 1)
		{
			target = itemSelection->itemAt(0)->getUName();
			targetIcon = itemSelection->itemAt(0)->getUPixmap();
		}
		m_groupTransaction = undoManager->beginTransaction(target, targetIcon,
														   action, tooltip, actionIcon);
	}
	++m_groupTransactions;
}


/**

*/
void ScribusView::endGroupTransaction()
{
	if (m_groupTransactions > 0)
	{
		--m_groupTransactions;
	}
	if (m_groupTransaction && m_groupTransactions == 0)
	{
		m_groupTransaction.commit();
		m_groupTransaction.reset();
	}
}

/**
   Always cancels the toplevel transaction and all enclosed ones
 */
void ScribusView::cancelGroupTransaction()
{
	if (m_groupTransaction && m_groupTransactions == 1)
	{
		m_groupTransaction.cancel();
		m_groupTransaction.reset();
	}
	else if (m_groupTransaction)
	{
		m_groupTransaction.markFailed();
	}
	if (m_groupTransactions > 0)
		--m_groupTransactions;
}


// jjsa 27-03-2004 add for better setting while zooming
//CB find a new name
void ScribusView::rememberOldZoomLocation(int mx, int my)
{
	m_oldZoomX = mx;
	m_oldZoomY = my;
}

void ScribusView::setRulerPos(int x, int y)
{
	if (m_ScMW->scriptIsRunning())
		return;
	if (m_doc->guidesPrefs().rulerMode)
	{
		horizRuler->shift(x / m_canvas->scale() - m_doc->currentPage()->xOffset());
		vertRuler->shift(y / m_canvas->scale() - m_doc->currentPage()->yOffset());
	}
	else
	{
		horizRuler->shift(x / m_canvas->scale());
		vertRuler->shift(y / m_canvas->scale());
	}
	//	horizRuler->offs += qRound(Doc->minCanvasCoordinate.x() - 1 - Doc->rulerXoffset);
	//	vertRuler->offs += qRound(Doc->minCanvasCoordinate.y() - 1 - Doc->rulerYoffset);
	horizRuler->shiftRel(0*m_doc->minCanvasCoordinate.x() - m_doc->rulerXoffset);
	vertRuler->shiftRel(0*m_doc->minCanvasCoordinate.y() - m_doc->rulerYoffset);
	horizRuler->update();
	vertRuler->update();
	QString newStatusBarText(" ");
	if ((verticalScrollBar()->isSliderDown()) || (horizontalScrollBar()->isSliderDown()))
	{
		QList<int> pag;
		pag.clear();
		int docPageCount = m_doc->Pages->count();
		for (int i = 0; i < docPageCount; ++i)
		{
			int xs = static_cast<int>(m_doc->Pages->at(i)->xOffset() * m_canvas->scale());
			int ys = static_cast<int>(m_doc->Pages->at(i)->yOffset() * m_canvas->scale());
			int ws = static_cast<int>(m_doc->Pages->at(i)->width() * m_canvas->scale());
			int hs = static_cast<int>(m_doc->Pages->at(i)->height() * m_canvas->scale());
			QRect drawRect(x, y, visibleWidth(), visibleHeight());
			//			drawRect.moveBy(qRound(-Doc->minCanvasCoordinate.x() * m_canvas->scale()), qRound(-Doc->minCanvasCoordinate.y() * m_canvas->scale()));
			if (drawRect.intersects(QRect(xs, ys, ws, hs)))
				pag.append(i+1);
		}
		if (!pag.isEmpty())
			newStatusBarText=( tr("Page %1 to %2").arg(pag.first()).arg(pag.last()));
	}
	m_ScMW->setStatusBarInfoText(newStatusBarText);
}


//CB This MUST now be called AFTER a call to doc->addPage or doc->addMasterPage as it
//does NOT create a page anymore.
ScPage* ScribusView::addPage(int nr, bool mov)
{
	ScPage* fe = m_doc->Pages->at(nr);
	Q_ASSERT(fe != nullptr);
	if (fe == nullptr)
		return nullptr;
	//Note this picks up the new page or master page depending on the mode.
	//	reformPages(mov);
	m_doc->reformPages(mov);
	m_ScMW->slotSetCurrentPage(nr);
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_doc->DragP = false;
	m_doc->leaveDrag = false;
	m_canvas->m_viewMode.operItemMoving = false;
	MidButt = false;
	HaveSelRect = false;
	Magnify = false;
	m_doc->nodeEdit.setEdPoints(true);
	DraggedGroup = false;
	//FIXME:av	MoveGY = false;
	//FIXME:av	MoveGX = false;
	m_doc->nodeEdit.setIsContourLine(false);
	return fe;
}

void ScribusView::reformPages(bool moveObjects)
{
	m_doc->reformPages(moveObjects);
	reformPagesView();
}

void ScribusView::reformPagesView()
{
	if (!m_ScMW->scriptIsRunning())
		setContentsPos(qRound((m_doc->currentPage()->xOffset() - 10 - 0 * m_doc->minCanvasCoordinate.x()) * m_canvas->scale()), qRound((m_doc->currentPage()->yOffset() - 10 - 0 * m_doc->minCanvasCoordinate.y()) * m_canvas->scale()));
	if (!m_doc->isLoading())
	{
		setRulerPos(contentsX(), contentsY());
		m_ScMW->slotSetCurrentPage(m_doc->currentPage()->pageNr());
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
		upperLeft.setX(qMax(0, upperLeft.x() - 10));
		upperLeft.setY(qMax(0, upperLeft.y() - 10));
		lowerRight.setX(qMax(0, lowerRight.x() + 10));
		lowerRight.setY(qMax(0, lowerRight.y() + 10));
		//		qDebug() << "updateCanvas:" << upperLeft << lowerRight;
		m_canvas->update(upperLeft.x(), upperLeft.y(), lowerRight.x() - upperLeft.x(), lowerRight.y() - upperLeft.y());
	}
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
	int newCanvasWidth  = qRound((m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x()) * scale);
	int newCanvasHeight = qRound((m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y()) * scale);
	horizontalScrollBar()->setRange(0, newCanvasWidth - maxViewport.width());
	verticalScrollBar()->setRange(0, newCanvasHeight - maxViewport.height());
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
	verticalScrollBar()->setValue(qRound(verticalScrollBar()->value() + deltaY * scale));
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
QRectF ScribusView::visibleCanvasRect() const
{
	double scale = m_canvas->scale();
	return QRectF(horizontalScrollBar()->value() / scale + m_doc->minCanvasCoordinate.x(),
				  verticalScrollBar()->value() / scale + m_doc->minCanvasCoordinate.y(),
				  viewport()->width() / scale,
				  viewport()->height() / scale);
}

void ScribusView::setZoom()
{
	double x = qMax(contentsX() / m_canvas->scale(), m_doc->minCanvasCoordinate.x());
	double y = qMax(contentsY() / m_canvas->scale(), m_doc->minCanvasCoordinate.y());
	double w = qMin(visibleWidth() / m_canvas->scale(), m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x());
	double h = qMin(visibleHeight() / m_canvas->scale(), m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y());

	int zoomX = qRound(x + w / 2);
	int zoomY = qRound(y + h / 2);
	rememberOldZoomLocation(zoomX, zoomY);

	zoom(m_oldZoomX, m_oldZoomY, m_ScMW->zoomSpinBox->value() / 100.0 * Prefs->displayPrefs.displayScale, false);
	setFocus();
}

void ScribusView::slotZoom100()
{
	double x = qMax(contentsX() / m_canvas->scale(), m_doc->minCanvasCoordinate.x());
	double y = qMax(contentsY() / m_canvas->scale(), m_doc->minCanvasCoordinate.y());
	double w = qMin(visibleWidth() / m_canvas->scale(), m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x());
	double h = qMin(visibleHeight() / m_canvas->scale(), m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y());

	int zoomX = qRound(x + w / 2);
	int zoomY = qRound(y + h / 2);
	rememberOldZoomLocation(zoomX, zoomY);

	int zoomPointX(m_oldZoomX), zoomPointY(m_oldZoomY);
	if (m_doc->m_Selection->count() != 0)
	{
		QRectF selRect = m_doc->m_Selection->getVisualGroupRect();
		zoomPointX = qRound(selRect.x() + selRect.width() / 2.0);
		zoomPointY = qRound(selRect.y() + selRect.height() / 2.0);
	}
	else if (m_doc->currentPage() != nullptr)
	{
		ScPage* currentPage = m_doc->currentPage();
		zoomPointX = qRound(currentPage->xOffset() + currentPage->width() / 2.0);
		zoomPointY = qRound(currentPage->yOffset() + currentPage->height() / 2.0);
	}
	zoom(zoomPointX, zoomPointY, Prefs->displayPrefs.displayScale, false);
}

void ScribusView::slotZoomIn(int mx, int my, bool preservePoint)
{
	// FIXME : mx and my should really be ScribusView local coordinates or global coordinates
	int oldZoomX(mx), oldZoomY(my);
	if ((mx == 0) && (my == 0))
	{
		double x = qMax(contentsX() / m_canvas->scale(), m_doc->minCanvasCoordinate.x());
		double y = qMax(contentsY() / m_canvas->scale(), m_doc->minCanvasCoordinate.y());
		double w = qMin(visibleWidth() / m_canvas->scale(), m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x());
		double h = qMin(visibleHeight() / m_canvas->scale(), m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y());
		oldZoomX = qRound(w / 2.0 + x);
		oldZoomY = qRound(h / 2.0 + y);
	}
	rememberOldZoomLocation(oldZoomX, oldZoomY);

	double newScale = m_canvas->scale() * (1 + static_cast<double>(m_doc->opToolPrefs().magStep) / 100.0);
	if (static_cast<int>(newScale * 100.0) > static_cast<int>(100.0 * static_cast<double>(m_doc->opToolPrefs().magStep) * Prefs->displayPrefs.displayScale / 100.0))
		newScale = m_canvas->scale() + static_cast<double>(m_doc->opToolPrefs().magStep) * Prefs->displayPrefs.displayScale / 100.0;

	int zoomPointX(m_oldZoomX), zoomPointY(m_oldZoomY);
	if (!preservePoint)
	{
		if (m_doc->m_Selection->count() != 0)
		{
			QRectF selRect = m_doc->m_Selection->getVisualGroupRect();
			zoomPointX = qRound(selRect.x() + selRect.width() / 2.0);
			zoomPointY = qRound(selRect.y() + selRect.height() / 2.0);
		}
	}
	zoom(zoomPointX, zoomPointY, newScale, preservePoint);
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut(int mx, int my, bool preservePoint)
{
	// FIXME : mx and my should really be ScribusView local coordinates or global coordinates
	int oldZoomX(mx);
	int oldZoomY(my);
	if ((mx == 0) && (my == 0))
	{
		double x = qMax(contentsX() / m_canvas->scale(), m_doc->minCanvasCoordinate.x());
		double y = qMax(contentsY() / m_canvas->scale(), m_doc->minCanvasCoordinate.y());
		double w = qMin(visibleWidth() / m_canvas->scale(), m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x());
		double h = qMin(visibleHeight() / m_canvas->scale(), m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y());
		oldZoomX = qRound(w / 2.0 + x);
		oldZoomY = qRound(h / 2.0 + y);
	}
	rememberOldZoomLocation(oldZoomX, oldZoomY);

	double newScale = m_canvas->scale() - static_cast<double>(m_doc->opToolPrefs().magStep) * Prefs->displayPrefs.displayScale / 100.0;
	if (newScale <= Prefs->displayPrefs.displayScale / 100.0)
		newScale = m_canvas->scale() / (1 + static_cast<double>(m_doc->opToolPrefs().magStep) / 100.0);
	if (newScale <= Prefs->displayPrefs.displayScale / 100.0)
		newScale = m_canvas->scale();

	int zoomPointX(m_oldZoomX), zoomPointY(m_oldZoomY);
	if (!preservePoint)
	{
		if (m_doc->m_Selection->count() != 0)
		{
			QRectF selRect = m_doc->m_Selection->getVisualGroupRect();
			zoomPointX = qRound(selRect.x() + selRect.width() / 2.0);
			zoomPointY = qRound(selRect.y() + selRect.height() / 2.0);
		}
	}
	zoom(zoomPointX, zoomPointY, newScale, preservePoint);
}

void ScribusView::DrawNew()
{
	// 	qDebug("ScribusView::DrawNew");
	// 	printBacktrace(24);
	if (m_ScMW->scriptIsRunning())
		return;
	m_canvas->setForcedRedraw(true);
	m_canvas->resetRenderMode();
	updateContents();
	setRulerPos(contentsX(), contentsY());
	m_ScMW->slotSetCurrentPage(m_doc->currentPage()->pageNr());
}

void ScribusView::setCanvasCenterPos(double x, double y)
{
	if (m_ScMW->scriptIsRunning())
		return;
	QPoint nx = m_canvas->canvasToLocal(FPoint(x, y));
	QSize viewsize = viewport()->size();
	//	qDebug() << "setCCPo" << nx << viewsize;
	setContentsPos(nx.x() - viewsize.width() / 2, nx.y() - viewsize.height() / 2);
}

void ScribusView::setCanvasPos(double x, double y)
{
	if (m_ScMW->scriptIsRunning())
		return;
	QPoint nx = m_canvas->canvasToLocal(FPoint(x, y));
	setContentsPos(nx.x(), nx.y());
}

void ScribusView::GotoLayer(int l)
{
	int level = m_doc->layerCount()-l-1;
	int layerID = m_doc->layerIDFromLevel(level);
	if (layerID == -1)
		return;
	m_doc->setActiveLayer(layerID);
	//CB TODO fix this to use view calls after 1.3.2 release
	m_ScMW->changeLayer(m_doc->activeLayer());
	emit layerChanged(layerID);
}

void ScribusView::ChgUnit(int unitIndex)
{
	emit unitChanged(unitIndex);
	unitChange();
	vertRuler->update();
	horizRuler->update();
}


void ScribusView::GotoPa(int pageNumber)
{
	deselectItems();
	GotoPage(pageNumber - 1);
	setFocus();
}

void ScribusView::GotoPage(int pageIndex)
{
	QRectF newTrimRect = m_doc->Pages->at(pageIndex)->trimRect();
	QRectF oldTrimRect = m_doc->currentPage() ? m_doc->currentPage()->trimRect() : newTrimRect;
	m_doc->setCurrentPage(m_doc->Pages->at(pageIndex));
	if (m_ScMW->scriptIsRunning())
		return;
	QRect updateRect = m_canvas->canvasToLocal(oldTrimRect.united(newTrimRect));
	m_ScMW->slotSetCurrentPage(pageIndex);
	m_canvas->setForcedRedraw(true);
	m_canvas->resetRenderMode();
	setCanvasPos(m_doc->currentPage()->xOffset() - 10, m_doc->currentPage()->yOffset() - 10);
	updateContents(updateRect.adjusted(-5, -5, 10, 10));
	m_ScMW->HaveNewSel();
}

void ScribusView::showMasterPage(int nr)
{
	// #9684 : we need Deselect() to emit HaveSel() when switching masterpage
	deselectItems(true);
	OldScale = m_canvas->scale();
	if (!m_doc->masterPageMode())
		this->requestMode(modeNormal);
	m_doc->setMasterPageMode(true);
	m_doc->setCurrentPage(m_doc->Pages->at(nr));
	m_ScMW->pageSelector->setEnabled(false);
	updateOn = false;
	setZoom();
	m_oldZoomX = qRound(m_doc->currentPage()->xOffset()- 10);
	m_oldZoomY = qRound(m_doc->currentPage()->yOffset()- 10);
	setCanvasPos(m_doc->currentPage()->xOffset() - 10, m_doc->currentPage()->yOffset() - 10);
	updateOn = true;
	endEditButton->setVisible(true);
	DrawNew();
}

void ScribusView::hideMasterPage()
{
	deselectItems(true);
	if (m_doc->masterPageMode())
		this->requestMode(modeNormal);
	m_doc->setMasterPageMode(false);
	m_ScMW->pageSelector->setEnabled(true);
	endEditButton->setVisible(false);
	resizeContents(qRound((m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x()) * m_canvas->scale()), qRound((m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y()) * m_canvas->scale()));
}

void ScribusView::showSymbolPage(const QString& symbolName)
{
	deselectItems(false);
	OldScale = m_canvas->scale();
	if (!m_doc->symbolEditMode())
		this->requestMode(modeNormal);
	m_doc->setSymbolEditMode(true, symbolName);
	m_doc->setCurrentPage(m_doc->Pages->at(0));
	m_ScMW->pageSelector->setEnabled(false);
	m_ScMW->layerMenu->setEnabled(false);
	updateOn = false;
	setZoom();
	m_oldZoomX = qRound(m_doc->currentPage()->xOffset()- 10);
	m_oldZoomY = qRound(m_doc->currentPage()->yOffset()- 10);
	setCanvasPos(m_doc->currentPage()->xOffset() - 10, m_doc->currentPage()->yOffset() - 10);
	updateOn = true;
	endEditButton->setVisible(true);
	DrawNew();
}

void ScribusView::hideSymbolPage()
{
	updatesOn(false);
	deselectItems(true);
	if (m_doc->symbolEditMode())
		this->requestMode(modeNormal);
	m_doc->setSymbolEditMode(false);
	updatesOn(true);
	endEditButton->setVisible(false);
	m_doc->setCurrentPage(m_doc->Pages->at(0));
	m_ScMW->pageSelector->setEnabled(true);
	m_ScMW->layerMenu->setEnabled(true);
	resizeContents(qRound((m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x()) * m_canvas->scale()), qRound((m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y()) * m_canvas->scale()));
}

void ScribusView::showInlinePage(int id)
{
	deselectItems(false);
	OldScale = m_canvas->scale();
	if (!m_doc->inlineEditMode())
		this->requestMode(modeNormal);
	m_doc->setInlineEditMode(true, id);
	m_doc->setCurrentPage(m_doc->Pages->at(0));
	m_ScMW->pageSelector->setEnabled(false);
	m_ScMW->layerMenu->setEnabled(false);
	updateOn = false;
	setZoom();
	m_oldZoomX = qRound(m_doc->currentPage()->xOffset() - 10);
	m_oldZoomY = qRound(m_doc->currentPage()->yOffset() - 10);
	setCanvasPos(m_doc->currentPage()->xOffset() - 10, m_doc->currentPage()->yOffset() - 10);
	updateOn = true;
	endEditButton->setVisible(true);
	DrawNew();
}

void ScribusView::hideInlinePage()
{
	updatesOn(false);
	deselectItems(true);
	if (m_doc->inlineEditMode())
		this->requestMode(modeNormal);
	m_doc->setInlineEditMode(false);
	updatesOn(true);
	endEditButton->setVisible(false);
	m_doc->setCurrentPage(m_doc->Pages->at(0));
	m_ScMW->pageSelector->setEnabled(true);
	m_ScMW->layerMenu->setEnabled(true);
	resizeContents(qRound((m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x()) * m_canvas->scale()), qRound((m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y()) * m_canvas->scale()));
}

QImage ScribusView::MPageToPixmap(const QString& name, int maxGr, bool drawFrame)
{
	ScLayer layer;
	layer.isViewable = false;
	int Nr = m_doc->MasterNames[name];
	int clipx = static_cast<int>(m_doc->scratch()->left());
	int clipy = static_cast<int>(m_doc->scratch()->top());
	int clipw = qRound(m_doc->MasterPages.at(Nr)->width());
	int cliph = qRound(m_doc->MasterPages.at(Nr)->height());
	if (clipw <= 0 || cliph <= 0)
		return QImage();

	double sca = m_canvas->scale();
	bool   frs = m_doc->guidesPrefs().framesShown;
	double cx = m_doc->minCanvasCoordinate.x();
	double cy = m_doc->minCanvasCoordinate.y();

	m_doc->minCanvasCoordinate = FPoint(0, 0);
	ScPage* act = m_doc->currentPage();
	bool mMode = m_doc->masterPageMode();
	m_doc->setMasterPageMode(true);
	m_doc->setCurrentPage(m_doc->MasterPages.at(Nr));
	bool ctrls = m_doc->guidesPrefs().showControls;
	m_doc->guidesPrefs().showControls = false;
	m_doc->guidesPrefs().framesShown = false;
	setScale(1.0);
	m_canvas->setPreviewMode(true);
	m_canvas->setForcedRedraw(true);
	QImage pm(clipw, cliph, QImage::Format_ARGB32_Premultiplied);
	ScPainter *painter = new ScPainter(&pm, pm.width(), pm.height(), 1.0, 0);
	painter->clear(m_doc->paperColor());
	painter->translate(-clipx, -clipy);
	painter->setLineWidth(1);
	if (drawFrame)
	{
		painter->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		painter->setBrush(m_doc->paperColor());
		painter->drawRect(clipx, clipy, clipw, cliph);
	}
	painter->beginLayer(1.0, 0);
	int layerCount = m_doc->layerCount();
	for (int layerLevel = 0; layerLevel < layerCount; ++layerLevel)
	{
		m_doc->Layers.levelToLayer(layer, layerLevel);
		m_canvas->DrawPageItems(painter, layer, QRect(clipx, clipy, clipw, cliph), false);
		m_canvas->DrawPageItems(painter, layer, QRect(clipx, clipy, clipw, cliph), true);
	}
	painter->endLayer();
	painter->end();
	double sx = pm.width() / static_cast<double>(maxGr);
	double sy = pm.height() / static_cast<double>(maxGr);
	QImage im;
	if (sy < sx)
		im = pm.scaled(static_cast<int>(pm.width() / sx), static_cast<int>(pm.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	else
		im = pm.scaled(static_cast<int>(pm.width() / sy), static_cast<int>(pm.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	delete painter;
	painter = nullptr;
	m_canvas->setPreviewMode(false);
	m_canvas->setForcedRedraw(false);
	m_doc->guidesPrefs().framesShown = frs;
	m_doc->guidesPrefs().showControls = ctrls;
	setScale(sca);
	m_doc->setMasterPageMode(mMode);
	m_doc->setCurrentPage(act);
	m_doc->minCanvasCoordinate = FPoint(cx, cy);

	return im;
}

QImage ScribusView::PageToPixmap(int Nr, int maxGr, PageToPixmapFlags flags)
{
	QImage im;
	double sx = maxGr / m_doc->DocPages.at(Nr)->width();
	double sy = maxGr / m_doc->DocPages.at(Nr)->height();
	double sc = qMin(sx, sy);
	int clipx = static_cast<int>(m_doc->DocPages.at(Nr)->xOffset() * sc);
	int clipy = static_cast<int>(m_doc->DocPages.at(Nr)->yOffset() * sc);
	int clipw = qRound(m_doc->DocPages.at(Nr)->width() * sc);
	int cliph = qRound(m_doc->DocPages.at(Nr)->height() * sc);
	if ((clipw <=0) || (cliph <= 0))
		return im;
	im = QImage(clipw, cliph, QImage::Format_ARGB32_Premultiplied);
	if (im.isNull())
		return im;
	im.fill( qRgba(0, 0, 0, 0) );
	int oldAppMode = m_doc->appMode;
	requestMode(modeNormal);
	double oldScale = m_canvas->scale();
	double cx = m_doc->minCanvasCoordinate.x();
	double cy = m_doc->minCanvasCoordinate.y();
	m_doc->minCanvasCoordinate = FPoint(0, 0);
	bool oldFramesShown  = m_doc->guidesPrefs().framesShown;
	bool oldShowControls = m_doc->guidesPrefs().showControls;
	bool oldDrawAsPreview = m_doc->drawAsPreview;
	m_doc->guidesPrefs().framesShown = false;
	m_doc->guidesPrefs().showControls = false;
	bool cmsCorr = false;
	if ((m_doc->cmsSettings().CMSinUse) && (m_doc->cmsSettings().GamutCheck))
	{
		cmsCorr = true;
		m_doc->cmsSettings().GamutCheck = false;
		m_doc->enableCMS(true);
	}
	m_doc->drawAsPreview = true;
	m_canvas->setScale(sc);
	m_canvas->setPreviewMode(true);
	m_canvas->setForcedRedraw(true);
	ScPage* act = m_doc->currentPage();
	bool mMode = m_doc->masterPageMode();
	m_doc->setMasterPageMode(false);
	m_doc->setLoading(true);
	m_doc->setCurrentPage(m_doc->DocPages.at(Nr));
	ScPainter *painter = new ScPainter(&im, im.width(), im.height(), 1.0, 0);
	if (flags & Pixmap_DrawBackground)
		painter->clear(m_doc->paperColor());
	else if (flags & Pixmap_DrawWhiteBackground)
		painter->clear(QColor(255, 255, 255));
	painter->translate(-clipx, -clipy);
	painter->setFillMode(ScPainter::Solid);
	if (flags & Pixmap_DrawFrame)
	{
		painter->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		painter->setBrush(m_doc->paperColor());
		painter->drawRect(clipx, clipy, clipw, cliph);
	}
	painter->beginLayer(1.0, 0);
	painter->setZoomFactor(m_canvas->scale());

	QList<QPair<PageItem*, int> > changedList;
	ScPage* page = m_doc->DocPages.at(Nr);
	PageItem* currItem;
	if ((page->FromMaster.count() != 0) && !flags.testFlag(Pixmap_DontReloadImages))
	{
		QList<PageItem*> itemList = page->FromMaster;
		while (itemList.count() > 0)
		{
			currItem = itemList.takeFirst();
			if (currItem->isGroup())
			{
				itemList = currItem->getChildren() + itemList;
				continue;
			}
			if (!currItem->isImageFrame() || !currItem->imageIsAvailable)
				continue;
			if (currItem->pixm.imgInfo.lowResType == 0)
				continue;
			changedList.append(qMakePair(currItem, currItem->pixm.imgInfo.lowResType));
			currItem->pixm.imgInfo.lowResType = 0;
			int fho = currItem->imageFlippedH();
			int fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			m_doc->loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
		}
	}
	if ((m_doc->Items->count() != 0) && !flags.testFlag(Pixmap_DontReloadImages))
	{
		FPoint orig = m_canvas->localToCanvas(QPoint(clipx, clipy));
		QRectF cullingArea(orig.x(), orig.y(), qRound(clipw / sc + 0.5), qRound(cliph / sc + 0.5));
		QList<PageItem*> itemList = *(m_doc->Items);
		while (itemList.count() > 0)
		{
			currItem = itemList.takeFirst();
			if (currItem->isGroup())
			{
				itemList = currItem->getChildren() + itemList;
				continue;
			}
			double w = currItem->visualWidth();
			double h = currItem->visualHeight();
			double x = -currItem->visualLineWidth() / 2.0;
			double y = -currItem->visualLineWidth() / 2.0;
			QRectF boundingRect = currItem->getTransform().mapRect(QRectF(x, y, w, h));
			if (!cullingArea.intersects(boundingRect.adjusted(0.0, 0.0, 1.0, 1.0)))
				continue;
			if (!currItem->isImageFrame() || !currItem->imageIsAvailable)
				continue;
			if (currItem->pixm.imgInfo.lowResType == 0)
				continue;
			changedList.append(qMakePair(currItem, currItem->pixm.imgInfo.lowResType));
			currItem->pixm.imgInfo.lowResType = 0;
			int fho = currItem->imageFlippedH();
			int fvo = currItem->imageFlippedV();
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			m_doc->loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
		}
	}

	ScLayer layer;
	layer.isViewable = false;
	int layerCount = m_doc->layerCount();
	for (int layerLevel = 0; layerLevel < layerCount; ++layerLevel)
	{
		m_doc->Layers.levelToLayer(layer, layerLevel);
		m_canvas->DrawMasterItems(painter, m_doc->DocPages.at(Nr), layer, QRect(clipx, clipy, clipw, cliph));
		m_canvas->DrawPageItems(painter, layer, QRect(clipx, clipy, clipw, cliph), false);
		m_canvas->DrawPageItems(painter, layer, QRect(clipx, clipy, clipw, cliph), true);
	}
	painter->endLayer();
	painter->end();
	delete painter;
	painter = nullptr;

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
			double imgX = currItem->imageXOffset();
			double imgY = currItem->imageYOffset();
			m_doc->loadPict(currItem->Pfile, currItem, true);
			currItem->setImageFlippedH(fho);
			currItem->setImageFlippedV(fvo);
			currItem->setImageXOffset(imgX);
			currItem->setImageYOffset(imgY);
		}
	}
	if (cmsCorr)
	{
		m_doc->cmsSettings().GamutCheck = true;
		m_doc->enableCMS(true);
	}
	m_doc->drawAsPreview = oldDrawAsPreview;
	m_doc->guidesPrefs().framesShown  = oldFramesShown;
	m_doc->guidesPrefs().showControls = oldShowControls;
	m_canvas->setScale(oldScale);
	m_doc->setMasterPageMode(mMode);
	m_doc->setCurrentPage(act);
	m_doc->setLoading(false);
	m_canvas->setPreviewMode(m_doc->drawAsPreview);
	m_canvas->setForcedRedraw(false);
	m_doc->minCanvasCoordinate = FPoint(cx, cy);
	requestMode(oldAppMode);
	return im;
}

void ScribusView::setNewRulerOrigin(QMouseEvent *m)
{
	QPoint py = viewport()->mapFromGlobal(m->globalPos());
	m_doc->rulerXoffset = (py.x() + contentsX()) / m_canvas->scale() + 0*m_doc->minCanvasCoordinate.x();
	m_doc->rulerYoffset = (py.y() + contentsY()) / m_canvas->scale() + 0*m_doc->minCanvasCoordinate.y();
	if (m_doc->guidesPrefs().rulerMode)
	{
		m_doc->rulerXoffset -= m_doc->currentPage()->xOffset();
		m_doc->rulerYoffset -= m_doc->currentPage()->yOffset();
	}
	setRulerPos(contentsX(), contentsY());
	m_canvas->newRedrawPolygon();
	int docSelectionCount = m_doc->m_Selection->count();
	if (docSelectionCount != 0)
	{
		if (docSelectionCount > 1)
		{
			double x, y, w, h;
			m_doc->m_Selection->getGroupRect(&x, &y, &w, &h);
			emit ItemGeom();
		}
		else
			m_doc->m_Selection->itemAt(0)->emitAllToGUI();
	}
}

void ScribusView::editExtendedImageProperties()
{
	if (m_doc->m_Selection->isEmpty())
		return;
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	if (!currItem->pixm.imgInfo.valid)
		return;
	QScopedPointer<ExtImageProps> dia(new ExtImageProps(this, &currItem->pixm.imgInfo, currItem, this));
	static_cast<void>(dia->exec());
	m_ScMW->propertiesPalette->setTextFlowMode(currItem->textFlowMode());
}

void ScribusView::ToPicFrame()
{
	Selection tempSelection(*m_doc->m_Selection);
	m_doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	deselectItems(true);
	Selection restoreSelection(this);
	m_doc->itemSelection_convertItemsTo(PageItem::ImageFrame, &restoreSelection, &tempSelection);
	m_doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	m_doc->m_Selection->delaySignalsOff();
}

void ScribusView::ToPolyFrame()
{
	Selection tempSelection(*m_doc->m_Selection);
	m_doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	deselectItems(true);
	Selection restoreSelection(this);
	m_doc->itemSelection_convertItemsTo(PageItem::Polygon, &restoreSelection, &tempSelection);
	m_doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	m_doc->m_Selection->delaySignalsOff();
}

void ScribusView::ToTextFrame()
{
	Selection tempSelection(*m_doc->m_Selection);
	m_doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	deselectItems(true);
	Selection restoreSelection(this);
	m_doc->itemSelection_convertItemsTo(PageItem::TextFrame, &restoreSelection, &tempSelection);
	m_doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	m_doc->m_Selection->delaySignalsOff();
}

void ScribusView::ToBezierFrame()
{
	Selection tempSelection(*m_doc->m_Selection);
	m_doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	deselectItems(true);
	Selection restoreSelection(this);
	m_doc->itemSelection_convertItemsTo(PageItem::PolyLine, &restoreSelection, &tempSelection);
	m_doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	m_doc->m_Selection->delaySignalsOff();
}

void ScribusView::Bezier2Poly()
{
	Selection tempSelection(*m_doc->m_Selection);
	m_doc->m_Selection->delaySignalsOn();
	updatesOn(false);
	deselectItems(true);
	Selection restoreSelection(this);
	m_doc->itemSelection_convertItemsTo(PageItem::Polygon, &restoreSelection, &tempSelection);
	m_doc->m_Selection->copy(restoreSelection, true);
	updatesOn(true);
	m_doc->m_Selection->delaySignalsOff();
}

void ScribusView::ToPathText()
{
	if (m_doc->m_Selection->count() <= 1)
		return;

	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	PageItem *polyLineItem;
	if (currItem->isTextFrame())
		polyLineItem = m_doc->m_Selection->itemAt(1);
	else
	{
		polyLineItem = m_doc->m_Selection->itemAt(0);
		currItem = m_doc->m_Selection->itemAt(1);
	}
	ParagraphStyle dstyle(currItem->itemText.defaultStyle());
	if (polyLineItem->asPolyLine() || polyLineItem->asPolygon() || polyLineItem->asSpiral() || polyLineItem->asArc() || polyLineItem->asRegularPolygon())
	{
		deselectItems(true);
		PageItem* newItem = m_doc->convertItemTo(currItem, PageItem::PathText, polyLineItem);
		newItem->itemText.setDefaultStyle(dstyle);
		newItem->itemText.applyCharStyle(0, newItem->itemText.length(), dstyle.charStyle());
		newItem->invalid = true;
		newItem->update();
		selectItem(newItem);
		emit DocChanged();
	}
}

void ScribusView::FromPathText()
{
	PageItem *currItem;
	if (!m_doc->getItem(&currItem))
		return;

	deselectItems(true);
	PageItem* newItem = m_doc->convertItemTo(currItem, PageItem::TextFrame);
	selectItem(newItem);
	m_doc->bringItemSelectionToFront();
	update();
}

// FIXME: the following code is untested, but it should give an idea for
// anyone who wants to fix and knows how to test it.
class TextToPathPainter: public TextLayoutPainter
{
		ScribusView* m_view;
		PageItem* m_item;
		QList<PageItem*> &m_group;
		int m_counter { 0 };

	public:
		TextToPathPainter(ScribusView* view, PageItem* item, QList<PageItem*> &group)
			: m_view(view)
			, m_item(item)
			, m_group(group)
		{}

		void drawGlyph(const GlyphCluster& gc) override
		{
			double current_x = 0.0;
			for (const GlyphLayout& gl : gc.glyphs())
			{
				if (gl.glyph >= ScFace::CONTROL_GLYPHS)
				{
					current_x += gl.xadvance * gl.scaleH;
					continue;
				}

				FPointArray outline = font().glyphOutline(gl.glyph);
				if (outline.size() < 4)
					return;
				QTransform transform;
				if (m_item->isPathText())
					transform = matrix();
				if (m_item->imageFlippedH())
				{
					transform.translate(m_item->width(), 0);
					transform.scale(-1, 1);
				}
				if (m_item->imageFlippedV())
				{
					transform.translate(0, m_item->height());
					transform.scale(1, -1);
				}
				transform.translate(x() + gl.xoffset + current_x , y() + gl.yoffset);
				transform.translate(0, -(fontSize() * gl.scaleV));
				transform.scale(gl.scaleH * fontSize() / 10.0, gl.scaleV * fontSize() / 10.0);
				outline.map(transform);
				int z = m_view->m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_item->xPos(), m_item->yPos(), m_item->width(), m_item->height(), 0, fillColor().color, CommonStrings::None);
				PageItem* item = m_view->m_doc->Items->at(z);
				m_view->undoManager->setUndoEnabled(false);
				item->setTextFlowMode(m_item->textFlowMode());
				item->setSizeLocked(m_item->sizeLocked());
				item->setLocked(m_item->locked());
				item->NamedLStyle = m_item->NamedLStyle;
				item->setItemName(m_item->itemName() + "+U" + QString::number(m_counter++));
				item->PoLine = outline.copy();
				if (!m_item->asPathText())
					item->setRotation(m_item->rotation());
				item->setFillColor(fillColor().color);
				item->setFillShade(fillColor().shade);
				m_view->m_doc->adjustItemSize(item);
				item->ContourLine = item->PoLine.copy();
				item->ClipEdited = true;
				item->FrameType = 3;
				item->OldB2 = item->width();
				item->OldH2 = item->height();
				m_view->m_doc->setRedrawBounding(item);
				m_view->undoManager->setUndoEnabled(true);
				m_group.append(m_view->m_doc->Items->takeAt(z));

				current_x += gl.xadvance * gl.scaleH;
			}
		}

		void drawGlyphOutline(const GlyphCluster& gc, bool fill) override
		{
			double current_x = 0.0;
			for (const GlyphLayout& gl : gc.glyphs())
			{
				if (gl.glyph >= ScFace::CONTROL_GLYPHS)
				{
					current_x += gl.xadvance * gl.scaleH;
					continue;
				}

				FPointArray outline = font().glyphOutline(gl.glyph);
				if (outline.size() < 4)
					return;
				QTransform transform;
				if (m_item->isPathText())
					transform = matrix();
				if (m_item->imageFlippedH())
				{
					transform.translate(m_item->width(), 0);
					transform.scale(-1, 1);
				}
				if (m_item->imageFlippedV())
				{
					transform.translate(0, m_item->height());
					transform.scale(1, -1);
				}
				transform.translate(x() + gl.xoffset + current_x , y() + gl.yoffset);
				transform.translate(0, -(fontSize() * gl.scaleV));
				transform.scale(gl.scaleH * fontSize() / 10.0, gl.scaleV * fontSize() / 10.0);
				outline.map(transform);
				int z = m_view->m_doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_item->xPos(), m_item->yPos(), m_item->width(), m_item->height(), strokeWidth(), fillColor().color, strokeColor().color);
				PageItem* item = m_view->m_doc->Items->at(z);
				m_view->undoManager->setUndoEnabled(false);
				item->setTextFlowMode(m_item->textFlowMode());
				item->setSizeLocked(m_item->sizeLocked());
				item->setLocked(m_item->locked());
				item->NamedLStyle = m_item->NamedLStyle;
				item->setItemName(m_item->itemName() + "+U" + QString::number(m_counter++));
				item->PoLine = outline.copy();
				if (!m_item->asPathText())
					item->setRotation(m_item->rotation());
				item->setFillColor(fillColor().color);
				item->setFillShade(fillColor().shade);
				item->setLineColor(strokeColor().color);
				item->setLineShade(strokeColor().shade);
				item->setLineWidth(strokeWidth());
				m_view->m_doc->adjustItemSize(item);
				item->ContourLine = item->PoLine.copy();
				item->ClipEdited = true;
				item->FrameType = 3;
				item->OldB2 = item->width();
				item->OldH2 = item->height();
				m_view->m_doc->setRedrawBounding(item);
				m_view->undoManager->setUndoEnabled(true);
				m_group.append(m_view->m_doc->Items->takeAt(z));

				current_x += gl.xadvance * gl.scaleH;
			}
		}

		void drawLine(const QPointF& start, const QPointF& end) override
		{
			QTransform transform = matrix();
			transform.translate(x(), y());
			int z = m_view->m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, m_item->xPos(), m_item->yPos(), m_item->width(), m_item->height(), m_item->lineWidth(), m_item->lineColor(), m_item->fillColor());
			PageItem* item = m_view->m_doc->Items->at(z);
			m_view->undoManager->setUndoEnabled(false);
			item->setTextFlowMode(m_item->textFlowMode());
			item->setSizeLocked(m_item->sizeLocked());
			item->setLocked(m_item->locked());
			item->NamedLStyle = m_item->NamedLStyle;
			item->setItemName(m_item->itemName() + "+U" + QString::number(m_counter++));
			item->PoLine.resize(0);
			item->PoLine.addQuadPoint(FPoint(start), FPoint(start), FPoint(end), FPoint(end));
			item->PoLine.map(transform);
			item->setLineColor(fillColor().color);
			item->setLineShade(fillColor().shade);
			item->setLineWidth(strokeWidth());
			m_view->m_doc->adjustItemSize(item);
			item->ContourLine = item->PoLine.copy();
			item->ClipEdited = true;
			item->FrameType = 3;
			item->OldB2 = item->width();
			item->OldH2 = item->height();
			m_view->m_doc->setRedrawBounding(item);
			m_view->undoManager->setUndoEnabled(true);
			m_group.append(m_view->m_doc->Items->takeAt(z));
		}

		void drawRect(const QRectF& rect) override {}
		void drawObject(PageItem* item) override {}
		void drawObjectDecoration(PageItem* item) override {}
};

void ScribusView::TextToPath()
{
	if (m_doc->appMode == modeEditClip)
		requestMode(submodeEndNodeEdit);
	Selection tmpSelection(this, false);
	tmpSelection.copy(*m_doc->m_Selection, false);
	PageItem *currItem = tmpSelection.itemAt(0);
	if ((currItem->prevInChain() != nullptr) || (currItem->nextInChain() != nullptr))
	{
		// select whole chain
		PageItem *backItem = currItem;
		while (backItem->prevInChain() != nullptr)
			backItem = backItem->prevInChain();
		currItem = backItem;
		deselectItems(true);
		tmpSelection.addItem(currItem);
		backItem = currItem->nextInChain();
		while (backItem != nullptr)
		{
			tmpSelection.addItem(backItem);
			if (backItem->nextInChain() != nullptr)
				backItem = backItem->nextInChain();
			else
				break;
		}
	}

	QList<PageItem*> delItems, newGroupedItems;
	int selectedItemCount = tmpSelection.count();
	if (selectedItemCount <= 0)
		return;

	UndoTransaction trans(undoManager->beginTransaction(currItem->getUName(), currItem->getUPixmap(), Um::ToOutlines, "", nullptr));
	int offset = 0;

	int oldRotMode = m_doc->rotationMode();
	m_doc->setRotationMode(0);

	for (int i = 0; i < selectedItemCount; ++i)
	{
		PageItem *currItem = tmpSelection.itemAt(offset);
		bool cont = false;
		if ((!((currItem->isTextFrame()) || (currItem->isPathText()))) || (currItem->locked()) || currItem->itemText.length() == 0)
			cont = true;
		if (currItem == m_ScMW->storyEditor->currentItem() && m_doc == m_ScMW->storyEditor->currentDocument())
		{
			ScMessageBox::information(m_ScMW, tr("Cannot Convert In-Use Item"), "<qt>" + tr("The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped").arg(currItem->itemName()) + "</qt>");
			cont = true;
		}
		//Deselect();
		if (cont)
		{
			++offset;
			continue;
		}

		// We usually don't need any of the undo actions created by TextToPathPainter. If we did take them into account,
		// the created items would reappear when redoing an undone TextToPath action
		int textLen = currItem->itemText.length();
		if (textLen > 1)
			undoManager->setUndoEnabled(false);
		TextToPathPainter p(this, currItem, newGroupedItems);
		currItem->textLayout.render(&p);
		if (textLen > 1)
			undoManager->setUndoEnabled(true);

		if ((currItem->isPathText()) && (currItem->PoShow))
		{
			int z = m_doc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, currItem->xPos(), currItem->yPos(), currItem->width(), currItem->height(), currItem->lineWidth(), CommonStrings::None, currItem->lineColor());
			PageItem *bb = m_doc->Items->at(z);
			undoManager->setUndoEnabled(false);
			bb->PoLine = currItem->PoLine.copy();
			bb->ClipEdited = true;
			bb->FrameType = 3;
			bb->OldB2 = bb->width();
			bb->OldH2 = bb->height();
			bb->setRotation(currItem->rotation());
			m_doc->adjustItemSize(bb);
			undoManager->setUndoEnabled(true);
			newGroupedItems.append(m_doc->Items->takeAt(z));
		}
		if (currItem->isTextFrame())
		{
			if ((!currItem->NamedLStyle.isEmpty()) || (currItem->lineColor() != CommonStrings::None) || (!currItem->strokePattern().isEmpty()) || (!currItem->strokeGradient().isEmpty()))
			{
				PageItem* newItem = new PageItem_Polygon(*currItem);
				newItem->convertTo(PageItem::Polygon);
				newItem->ClipEdited = true;
				newItem->FrameType = 3;
				newItem->OldB2 = newItem->width();
				newItem->OldH2 = newItem->height();
				newItem->Clip = flattenPath(newItem->PoLine, newItem->Segments);
				newItem->ContourLine = newItem->PoLine.copy();
				newGroupedItems.prepend(newItem);
			}
		}
		delItems.append(tmpSelection.takeItem(offset));
	}

	tmpSelection.clear();
	int ind = -1;
	if (currItem->isGroupChild())
		ind = currItem->parentGroup()->groupItemList.indexOf(currItem);
	else
		ind = m_doc->Items->indexOf(currItem);
	if (newGroupedItems.count() > 1)
	{
		double minx =  std::numeric_limits<double>::max();
		double miny =  std::numeric_limits<double>::max();
		double maxx = -std::numeric_limits<double>::max();
		double maxy = -std::numeric_limits<double>::max();
		for (int ep = 0; ep < newGroupedItems.count(); ++ep)
		{
			double x1, x2, y1, y2;
			newGroupedItems.at(ep)->getVisualBoundingRect(&x1, &y1, &x2, &y2);
			minx = qMin(minx, x1);
			miny = qMin(miny, y1);
			maxx = qMax(maxx, x2);
			maxy = qMax(maxy, y2);
		}
		double gx = minx;
		double gy = miny;
		double gw = maxx - minx;
		double gh = maxy - miny;
		int z = m_doc->itemAdd(PageItem::Group, PageItem::Rectangle, gx, gy, gw, gh, 0, CommonStrings::None, CommonStrings::None);
		PageItem *gItem = m_doc->Items->takeAt(z);
		m_doc->groupObjectsToItem(gItem, newGroupedItems);
		gItem->Parent = currItem->Parent;
		gItem->gXpos = currItem->gXpos;
		gItem->gYpos = currItem->gYpos;
		if (currItem->isGroupChild())
			currItem->parentGroup()->groupItemList.insert(ind+1, gItem);
		else
			m_doc->Items->insert(ind+1, gItem);
	}
	else if (newGroupedItems.count() > 0)
	{
		newGroupedItems.at(0)->Parent = currItem->Parent;
		if (currItem->isGroupChild())
			currItem->parentGroup()->groupItemList.insert(ind+1, newGroupedItems.at(0));
		else
			m_doc->Items->insert(ind+1, newGroupedItems.at(0));
	}

	int toDeleteItemCount = delItems.count();
	if (toDeleteItemCount != 0)
	{
		tmpSelection.clear();
		for (int i = 0; i < toDeleteItemCount; ++i)
			tmpSelection.addItem(delItems.takeAt(0)); //yes, 0, remove the first
		m_doc->itemSelection_DeleteItem(&tmpSelection);
	}

	m_doc->setRotationMode(oldRotMode);
	m_ScMW->HaveNewSel();
	deselectItems(true);
	trans.commit();
}

void ScribusView::keyPressEvent(QKeyEvent *k)
{
	if (m_canvasMode)
		m_canvasMode->keyPressEvent(k);
}

void ScribusView::keyReleaseEvent(QKeyEvent *k)
{
	if (m_canvasMode)
		m_canvasMode->keyReleaseEvent(k);
}

void ScribusView::inputMethodEvent(QInputMethodEvent * event)
{
	//qDebug() << "IME" << event->commitString() << event->preeditString() << "attributes:" << event->attributes().count();
	// #9682 : Avoid parameter type ambiguity in QKeyEvent constructor with Qt3Support enabled Qt builds
	Qt::KeyboardModifiers modifiers = Qt::NoModifier;

	const QString& commitString = event->commitString();
	for (int i = 0; i < commitString.length(); ++i)
	{
		QKeyEvent ev(QEvent::KeyPress, 0, modifiers, commitString.mid(i, 1));
		keyPressEvent(&ev);
	}
}

QVariant ScribusView::inputMethodQuery(Qt::InputMethodQuery query) const
{
	//	qDebug() << "IMQ" << query;
	return QVariant();
}

void ScribusView::wheelEvent(QWheelEvent *w)
{
	if (w->modifiers() == Qt::ControlModifier)
	{
		FPoint mp = m_canvas->globalToCanvas(w->globalPos());
		w->delta() > 0 ? slotZoomIn(mp.x(), mp.y() , true) : slotZoomOut(mp.x(), mp.y(), true);
	}
	else
	{
		int dX = 0, dY = 0;
		int moveBy = (w->delta() < 0) ? Prefs->uiPrefs.wheelJump : -Prefs->uiPrefs.wheelJump;
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
	if (!m_ScMW->HaveDoc)
		return;

	//qDebug(QString("%1 %2").arg((int)m_ScMW).arg(m_ScMW->scrActions.contains("editActionMode")));
	m_ScMW->scrActions["editActionMode"]->setChecked(true);
	int docSelectionCount = m_doc->m_Selection->count();
	if (docSelectionCount == 1)
		undoManager->showObject(m_doc->m_Selection->itemAt(0)->getUId());
	else if (docSelectionCount > 1)
		undoManager->showObject(Um::NO_UNDO_STACK);
	else if (docSelectionCount == 0)
		undoManager->showObject(m_doc->currentPage()->getUId());
}

void ScribusView::setGlobalUndoMode()
{
	if (!m_ScMW->HaveDoc)
		return;

	m_ScMW->scrActions["editActionMode"]->setChecked(!_isGlobalMode);
	if (_isGlobalMode)
		undoManager->showObject(Um::GLOBAL_UNDO_MODE);
	else
	{
		int docSelectionCount = m_doc->m_Selection->count();
		if (docSelectionCount == 1)
			undoManager->showObject(m_doc->m_Selection->itemAt(0)->getUId());
		else if (docSelectionCount > 1)
			undoManager->showObject(Um::NO_UNDO_STACK);
		else if (docSelectionCount == 0)
			undoManager->showObject(m_doc->currentPage()->getUId());
	}
}

void ScribusView::unitChange()
{
	vertRuler->unitChange();
	horizRuler->unitChange();
}

void ScribusView::setRulersShown(bool isShown)
{
	vertRuler->setVisible(isShown);
	horizRuler->setVisible(isShown);
	rulerMover->setVisible(isShown);
	int newTopLeftMargin = isShown ? m_vhRulerHW : 0;
	setViewportMargins(newTopLeftMargin, newTopLeftMargin, 0, 0);
}

void ScribusView::setScale(double newScale)
{
	double canvasScale = newScale;
	double v = m_doc->opToolPrefs().magMin * Prefs->displayPrefs.displayScale / 100.0;
	if (canvasScale < v)
		canvasScale = v;
	double v2 = m_doc->opToolPrefs().magMax * Prefs->displayPrefs.displayScale / 100.0;
	if (canvasScale > v2)
		canvasScale = v2;
	double v3 = 320 * Prefs->displayPrefs.displayScale;
	if (canvasScale > v3)
		canvasScale = v3;

	m_canvas->setScale(canvasScale);

	bool sigBlocked = m_ScMW->zoomSpinBox->blockSignals(true);
	m_ScMW->zoomSpinBox->setValue(m_canvas->scale() / Prefs->displayPrefs.displayScale * 100);
	m_ScMW->zoomSpinBox->blockSignals(sigBlocked);

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
		auto* m = dynamic_cast<QMouseEvent*> (event);
		m_mousePointDoc = m_canvas->globalToCanvas(m->globalPos());
		FPoint p = m_canvas->localToCanvas(QPoint(m->x(),m->y()));
		emit MousePos(p.x(),p.y());
		horizRuler->draw(m->x() + qRound(m_doc->minCanvasCoordinate.x() * m_canvas->scale())); //  - 2 * contentsX());
		vertRuler->draw(m->y() + qRound(m_doc->minCanvasCoordinate.y() * m_canvas->scale()));
		m_canvasMode->mouseMoveEvent(m);
		return true;
	}
	if (obj == widget() && event->type() == QEvent::MouseButtonRelease)
	{
		auto* m = dynamic_cast<QMouseEvent*> (event);
		m_canvasMode->mouseReleaseEvent(m);
		m_canvas->m_viewMode.m_MouseButtonPressed = false;
		if (linkAfterDraw)
		{
			//user creates new frame using linking tool
			PageItem * secondFrame = m_doc->m_Selection->itemAt(0);
			if (secondFrame && firstFrame)
			{
				firstFrame->link(secondFrame);
				firstFrame = nullptr;
				secondFrame->emitAllToGUI();
			}
			linkAfterDraw = false;
		}
		if (ImageAfterDraw)
		{
			//user creates new frame using linking tool
			PageItem * frame = m_doc->m_Selection->itemAt(0);
			requestMode(modeImportImage);
			if (frame)
			{
				auto* cm = qobject_cast<CanvasMode_ImageImport*>(canvasMode());
				if (!cm)
				{
					qFatal("ScribusView::eventFilter cm nullptr");
					return true;
				}
				cm->setImage(frame);
				cm->updateList();
			}
			ImageAfterDraw = false;
		}
		return true;
	}
	if (obj == widget() && event->type() == QEvent::MouseButtonPress)
	{
		auto* m = dynamic_cast<QMouseEvent*> (event);
		bool linkmode = (m_doc->appMode == modeLinkFrames);
		firstFrame = m_doc->m_Selection->itemAt(0);
		m_canvasMode->mousePressEvent(m);
		//if user don't click any frame he want to draw new frame and link it
		bool requestDrawMode = (m_doc->ElemToLink == nullptr);
		requestDrawMode &= (firstFrame && !firstFrame->nextInChain());
		if (linkmode && requestDrawMode)
		{
			//switch to drawing new text frame
			linkAfterDraw = true;
			requestMode(modeDrawText);
			m_canvasMode->mousePressEvent(m);
		}
		else
			firstFrame = nullptr;
		if(m_doc->appMode == modeImportImage && ImageAfterDraw)
		{
			//switch to drawing new text frame
			requestMode(modeDrawImage);
			m_canvasMode->mousePressEvent(m);
		}
		m_canvas->m_viewMode.m_MouseButtonPressed = true;
		return true;
	}
	if (obj == widget() && event->type() == QEvent::MouseButtonDblClick)
	{
		auto* m = dynamic_cast<QMouseEvent*> (event);
		m_canvasMode->mouseDoubleClickEvent(m);
		return true;
	}
	if (event->type() == QEvent::KeyPress)
	{
		auto* k = dynamic_cast<QKeyEvent*> (event);
		m_canvasMode->keyPressEvent(k);
		return true;
	}
	if (event->type() == QEvent::KeyRelease)
	{
		auto* m = dynamic_cast<QKeyEvent*> (event);
		m_canvasMode->keyReleaseEvent(m);
		return true;
	}
	if (obj == widget() && event->type() == QEvent::DragEnter)
	{
		auto* d = dynamic_cast<QDragEnterEvent*> (event);
		contentsDragEnterEvent(d);
		return true;
	}
	if (obj == widget() && event->type() == QEvent::DragLeave)
	{
		auto* d = dynamic_cast<QDragLeaveEvent*> (event);
		contentsDragLeaveEvent(d);
		return true;
	}
	if (obj == widget() && event->type() == QEvent::DragMove)
	{
		auto* d = dynamic_cast<QDragMoveEvent*> (event);
		contentsDragMoveEvent(d);
		return true;
	}
	if (obj == widget() && event->type() == QEvent::Drop)
	{
		auto* d = dynamic_cast<QDropEvent*> (event);
		contentsDropEvent(d);
		return true;
	}
	if (event->type() == QEvent::NativeGesture)
	{
		auto *ng = static_cast<QNativeGestureEvent*>(event);
		nativeGestureEvent(ng);
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
	updateContents(QRect(x, y, w, h));
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
	int originX = qRound(m_doc->minCanvasCoordinate.x() * scale());
	int originY = qRound(m_doc->minCanvasCoordinate.y() * scale());
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
	int originX = qRound(m_doc->minCanvasCoordinate.x() * scale());
	return horizontalScrollBar()->value() + originX;
}

int ScribusView::contentsY() // deprecated
{
	int originY = qRound(m_doc->minCanvasCoordinate.y() * scale());
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
	double zPointX = m_oldZoomX;
	double zPointY = m_oldZoomY;
	if (scale <= 0.0)
		scale = m_canvas->scale();
	if (!m_doc->m_Selection->isEmpty())
	{
		PageItem *currItem = m_doc->m_Selection->itemAt(0);
		zPointX = currItem->xPos() + currItem->width() / 2.0;
		zPointY = currItem->yPos() + currItem->height() / 2.0;
	}
	zoom( qRound(zPointX), qRound(zPointY), scale, false);
}

void ScribusView::zoom(int canvasX, int canvasY, double scale, bool preservePoint)
{
	QPoint globalPoint = m_canvas->canvasToGlobal(QPointF(canvasX, canvasY));
	double oldScale = m_canvas->scale();
	double newScale = (scale > (Prefs->opToolPrefs.magMax / 100.0) * Prefs->displayPrefs.displayScale) ? ((Prefs->opToolPrefs.magMax / 100.0) * Prefs->displayPrefs.displayScale) : scale;
	undoManager->setUndoEnabled(false);
	updatesOn(false);
	setScale(newScale);

	FPoint minPos = m_doc->minCanvasCoordinate;
	FPoint maxPos = m_doc->maxCanvasCoordinate;
	if (newScale > oldScale)
	{
		// Try to set scrollBars values in a reasonable way after important zoom out 
		QRectF optimalRect = m_doc->canvasOptimalRect();
		double minX = qMax(minPos.x(), optimalRect.left());
		double minY = qMax(minPos.y(), optimalRect.top());
		double maxX = qMin(maxPos.x(), optimalRect.right());
		double maxY = qMin(maxPos.y(), optimalRect.bottom());
		minPos.setXY(minX, minY);
		maxPos.setXY(maxX, maxY);
	}

	// FIXME : find a way to avoid adjustCanvas/resizeContents, cause unnecessary paintEvents despite updates disabled
	if (minPos != m_doc->minCanvasCoordinate || maxPos != m_doc->maxCanvasCoordinate)
		m_doc->adjustCanvas(minPos, maxPos, true);
	else
	{
		int nw = qMax(qRound((m_doc->maxCanvasCoordinate.x() - m_doc->minCanvasCoordinate.x()) * m_canvas->scale()), visibleWidth());
		int nh = qMax(qRound((m_doc->maxCanvasCoordinate.y() - m_doc->minCanvasCoordinate.y()) * m_canvas->scale()), visibleHeight());
		resizeContents(nw, nh);
	}

	QPoint localPoint = m_canvas->canvasToLocal( QPointF(canvasX, canvasY) );

	QPoint canvasPoint;
	if (preservePoint)
		canvasPoint = viewport()->mapFromGlobal(globalPoint);
	else
		canvasPoint = QPoint(viewport()->width() / 2, viewport()->height() / 2);

	int contentPosX = localPoint.x() - canvasPoint.x();
	int contentPosY = localPoint.y() - canvasPoint.y();

	QSize maxViewport = maximumViewportSize();
	minPos = m_canvas->localToCanvas(QPoint(horizontalScrollBar()->minimum(), verticalScrollBar()->minimum()));
	maxPos = m_canvas->localToCanvas(QPoint(horizontalScrollBar()->maximum() + maxViewport.width(), verticalScrollBar()->maximum() + maxViewport.height()));

	int hMin = qMin(contentPosX, horizontalScrollBar()->minimum());
	int hMax = qMax(contentPosX, horizontalScrollBar()->maximum());
	int vMin = qMin(contentPosY, verticalScrollBar()->minimum());
	int vMax = qMax(contentPosY, verticalScrollBar()->maximum());
	if ((hMin < horizontalScrollBar()->minimum()) || (hMax > horizontalScrollBar()->maximum()) ||
		(vMin < verticalScrollBar()->minimum())   || (vMax > verticalScrollBar()->maximum()))
	{
		QSize maxViewport = maximumViewportSize();
		minPos = m_canvas->localToCanvas(QPoint(hMin, vMin));
		maxPos = m_canvas->localToCanvas(QPoint(hMax + maxViewport.width(), vMax + maxViewport.height()));
	}

	m_doc->adjustCanvas(minPos, maxPos);
	localPoint = m_canvas->canvasToLocal(QPointF(canvasX, canvasY));
	if (preservePoint)
		canvasPoint = viewport()->mapFromGlobal(globalPoint);
	else
		canvasPoint = QPoint(viewport()->width() / 2, viewport()->height() / 2);
	contentPosX = localPoint.x() - canvasPoint.x();
	contentPosY = localPoint.y() - canvasPoint.y();
	setContentsPos(contentPosX, contentPosY);
	
	updatesOn(true);
	undoManager->setUndoEnabled(true);
}

void ScribusView::saveViewState()
{
	ViewState viewState;
	viewState.currentPage = m_doc->currentPageNumber();
	viewState.contentX = horizontalScrollBar()->value();
	viewState.contentY = verticalScrollBar()->value();
	viewState.canvasScale = m_canvas->scale();
	viewState.minCanvasCoordinate = m_doc->minCanvasCoordinate;
	viewState.maxCanvasCoordinate = m_doc->maxCanvasCoordinate;
	m_viewStates.push(viewState);
}

void ScribusView::restoreViewState()
{
	if (m_viewStates.isEmpty())
		return;

	ViewState viewState = m_viewStates.pop();
	m_doc->minCanvasCoordinate = viewState.minCanvasCoordinate;
	m_doc->maxCanvasCoordinate = viewState.maxCanvasCoordinate;
	setScale(viewState.canvasScale);

	// #12857 : the number of pages may change when undoing/redoing
	// page addition/deletion while in edit mode, so take some extra
	// care so that storedPageNum is in appropriate range
	int currentPage = qMin(viewState.currentPage, m_doc->DocPages.count() - 1);
	m_doc->setCurrentPage(m_doc->DocPages.at(currentPage));
	setContentsPos(viewState.contentX, viewState.contentY);
}

void ScribusView::stopAllDrags() // deprecated
{
	m_canvas->m_viewMode.m_MouseButtonPressed = false;
	m_canvas->m_viewMode.operItemMoving = false;
	m_canvas->m_viewMode.operItemResizing = false;
	MidButt = false;
}

void ScribusView::setRedrawMarkerShown(bool shown)
{
	if (shown != redrawMarker->isVisible())
		redrawMarker->setVisible(shown);
}
