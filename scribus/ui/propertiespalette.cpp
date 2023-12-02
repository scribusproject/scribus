/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "propertiespalette.h"

#include <QCloseEvent>
#include <QComboBox>
#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QObject>
#include <QPoint>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QTimer>
#include <QToolBox>
#include <QToolTip>
#include <QTransform>
#include <QVBoxLayout>
#include <QValidator>
#include <QWidget>

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "appmodehelper.h"
#include "appmodes.h"
#include "colorpalette.h"
#include "iconmanager.h"
#include "pageitem_table.h"
#include "propertiespalette_line.h"
#include "propertiespalette_shadow.h"
#include "propertiespalette_shape.h"
#include "propertiespalette_xyz.h"
#include "transparencypalette.h"
#include "scribus.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"
#include "util_math.h"


PropertiesPalette::PropertiesPalette(QWidget *parent) : DockPanelBase("PropertiesPalette", parent)
{
	undoManager = UndoManager::instance();

	setObjectName(QString::fromLocal8Bit("PropertiesPalette"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);

	// XZY
	xyzPal = new PropertiesPalette_XYZ( this );
	scXYZ = new SectionContainer("X, Y, &Z");
	scXYZ->setWidget(xyzPal);

	// Drop Shadow
	shadowPal = new PropertiesPalette_Shadow( this );
	scShadow = new SectionContainer("Drop Shadow");
	scShadow->setWidget(shadowPal);
	scShadow->setIsCollapsed(true);

	// Shape
	shapePal = new PropertiesPalette_Shape( this );
	scShape = new SectionContainer("&Shape");
	scShape->setWidget(shapePal);
	scShape->setIsCollapsed(true);

	// Line
	linePal = new PropertiesPalette_Line(this);
	scLine = new SectionContainer("&Line");
	scLine->setWidget(linePal);
	scLine->setIsCollapsed(true);

	// Color
	colorPalette = new ColorPalette(this);
	scColor = new SectionContainer("&Colors");
	scColor->setWidget(colorPalette);
	scColor->setIsCollapsed(true);

	// Transparency
	transparencyPalette = new TransparencyPalette(this);
	scTransparency = new SectionContainer("&Transparency");
	scTransparency->setWidget(transparencyPalette);
	scTransparency->setIsCollapsed(true);

	// Layout stack
	QVBoxLayout * lyt = new QVBoxLayout();
	lyt->setContentsMargins(0, 0, 0, 0);
	lyt->setSpacing(0);
	lyt->addWidget(scXYZ);
	lyt->addWidget(scShadow);
	lyt->addWidget(scShape);
	lyt->addWidget(scLine);
	lyt->addWidget(scColor);
	lyt->addWidget(scTransparency);
	lyt->addStretch(1);

	QWidget * wdg = new QWidget();
	wdg->setLayout(lyt);
	setWidget(wdg);

	languageChange();
	enablePalettes(false);

	connect(colorPalette, SIGNAL(NewSpecial(double,double,double,double,double,double,double,double,double,double)), this, SLOT(newSpGradient(double,double,double,double,double,double,double,double,double,double)));
	connect(colorPalette, SIGNAL(editGradient(int)), this, SLOT(toggleGradientEdit(int)));
	connect(transparencyPalette, SIGNAL(NewSpecial(double,double,double,double,double,double,double,double,double,double)), this, SLOT(newSpGradientM(double,double,double,double,double,double,double,double)));
	connect(transparencyPalette, SIGNAL(editGradient()), this, SLOT(toggleGradientEditM()));

	m_haveItem = false;

}

void PropertiesPalette::closeEvent(QCloseEvent *closeEvent)
{
	if (m_ScMW && !m_ScMW->scriptIsRunning())
	{
		if (m_haveDoc && m_haveItem && colorPalette->gradEditButton->isChecked())
		{
			m_ScMW->view->requestMode(modeNormal);
			m_ScMW->view->RefreshGradient(m_item);
		}
	}
	DockPanelBase::closeEvent(closeEvent);
}

void PropertiesPalette::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	// For some reason, the mapToGlobal() / mapFromGlobal() roundtrip
	// performed below does not give always good results, causing PP to
	// not display in some situations. Moreover the reparenting is useless
	// as PP is already created with ScribusMainWindow as parent.
	/*QPoint p1 = mapToGlobal(pos());
	QPoint p2 = m_ScMW->mapFromGlobal(p1);
	setParent(m_ScMW);
	move(p2);*/

	this->xyzPal->setMainWindow(mw);
	this->shadowPal->setMainWindow(mw);
	this->shapePal->setMainWindow(mw);
	this->linePal->setMainWindow(mw);

	//connect(this->Cpal, SIGNAL(gradientChanged()), m_ScMW, SLOT(updtGradFill()));
	//connect(this->Cpal, SIGNAL(strokeGradientChanged()), m_ScMW, SLOT(updtGradStroke()));
	connect(this->transparencyPalette, SIGNAL(gradientChanged()), this, SLOT(handleGradientChanged()));
	connect(m_ScMW->appModeHelper, SIGNAL(AppModeChanged(int,int)), this, SLOT(AppModeChanged()));
}

void PropertiesPalette::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));
	}

	m_doc = d;
	m_item = nullptr;
	setEnabled(!m_doc->drawAsPreview);
	colorPalette->setDocument(m_doc);
	colorPalette->setCurrentItem(nullptr);
	transparencyPalette->setDocument(m_doc);
	transparencyPalette->setCurrentItem(nullptr);

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();
	m_haveDoc = true;
	m_haveItem = false;

	xyzPal->setDoc(m_doc);
	shadowPal->setDoc(m_doc);
	shapePal->setDoc(m_doc);
	linePal->setDoc(m_doc);

	updateColorList();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));

	// Handle properties update when switching document
	handleSelectionChanged();
}

void PropertiesPalette::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));
	}
	setEnabled(true);
	m_haveDoc = false;
	m_haveItem = false;
	m_doc = nullptr;
	m_item = nullptr;

	xyzPal->unsetItem();
	xyzPal->unsetDoc();
	shadowPal->unsetItem();
	shadowPal->unsetDoc();
	shapePal->unsetItem();
	shapePal->unsetDoc();
	linePal->unsetItem();
	linePal->unsetDoc();

	colorPalette->setCurrentItem(nullptr);
	colorPalette->setDocument(nullptr);
	transparencyPalette->setCurrentItem(nullptr);
	transparencyPalette->setDocument(nullptr);

	m_haveItem = false;
	enablePalettes(false);
}

void PropertiesPalette::unsetItem()
{
	m_haveItem = false;
	m_item = nullptr;
	colorPalette->setCurrentItem(nullptr);
	transparencyPalette->setCurrentItem(nullptr);
	shapePal->unsetItem();
	linePal->unsetItem();
	handleSelectionChanged();
}

void PropertiesPalette::setTextFlowMode(PageItem::TextFlowMode mode)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveItem)
		return;
	shapePal->showTextFlowMode(mode);
}

PageItem* PropertiesPalette::currentItemFromSelection()
{
	if (m_doc && m_doc->m_Selection->count() > 0)
		return m_doc->m_Selection->itemAt(0);
	return nullptr;
}

void PropertiesPalette::enablePalettes(bool enable)
{
	scXYZ->setEnabled(enable);
	scShape->setEnabled(enable);
	scLine->setEnabled(enable);
	scColor->setEnabled(enable);
	scTransparency->setEnabled(enable);
	scShadow->setEnabled(enable);
}

void PropertiesPalette::AppModeChanged()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		// TODO: not sure that this is still needed...
		if (m_item->isTable())
		{
			if (m_doc->appMode == modeEditTable)
				connect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
			else
				disconnect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		}
	}
}

void PropertiesPalette::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be nullptr.
	//if (m_item == item)
	//	return;

	if (!item)
	{
		unsetItem();
		return;
	}

	if (!m_doc)
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	transparencyPalette->setCurrentItem(m_item);

	setTextFlowMode(m_item->textFlowMode());

//CB replaces old emits from PageItem::emitAllToGUI()
	setLocked(item->locked());

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		scXYZ->setEnabled(true);
		scShadow->setEnabled(true);
		scShape->setEnabled(true);
		scLine->setEnabled(false);
		scColor->setEnabled(false);
		scTransparency->setEnabled(true);

	}

	m_haveItem = true;

	if (!sender() || (m_doc->appMode == modeEditTable))
	{
		xyzPal->handleSelectionChanged();
		shadowPal->handleSelectionChanged();
		shapePal->handleSelectionChanged();
		linePal->handleSelectionChanged();
		colorPalette->handleSelectionChanged();
	}

	if (m_item->isOSGFrame())
	{
		scXYZ->setEnabled(true);
		scShadow->setEnabled(true);
		scShape->setEnabled(true);
		scLine->setEnabled(false);
		scColor->setEnabled(true);
		scTransparency->setEnabled(false);
	}
	if (m_item->asSymbol())
	{
		scXYZ->setEnabled(true);
		scShadow->setEnabled(true);
		scShape->setEnabled(false);
		scLine->setEnabled(false);
		scColor->setEnabled(false);
		scTransparency->setEnabled(false);
	}
}

void PropertiesPalette::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	enablePalettes(false);

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		scXYZ->setEnabled(true);
		scShadow->setEnabled(true);
		scLine->setEnabled(true);
		scColor->setEnabled(true);
		scTransparency->setEnabled(true);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;
		m_haveItem   = (itemType != -1);

		switch (itemType)
		{
		case -1:
			m_haveItem = false;
			colorPalette->showGradient(0);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
			if (currItem->isOSGFrame())
			{
				scXYZ->setEnabled(true);
				scShadow->setEnabled(true);
				scShape->setEnabled(true);
				scColor->setEnabled(true);
			}
			else
			{
				enablePalettes(true);
			}
			break;
		case PageItem::Line:
			scXYZ->setEnabled(true);
			scShadow->setEnabled(true);
			scLine->setEnabled(true);
			scColor->setEnabled(true);
			scTransparency->setEnabled(true);
			break;
		case PageItem::Group:
		case PageItem::Symbol:
		case PageItem::Table:
		case PageItem::TextFrame:
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::RegularPolygon:
		case PageItem::Arc:
		case PageItem::PolyLine:
		case PageItem::Spiral:
		case PageItem::PathText:
			enablePalettes(true);
			break;
		}
	}

	updateGeometry();
	update();

	if (currItem)
	{
		setCurrentItem(currItem);
	}
}

void PropertiesPalette::unitChange()
{
	if (!m_haveDoc)
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	double oldRatio = m_unitRatio;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	xyzPal->unitChange();
	shadowPal->unitChange();
	shapePal->unitChange();
	linePal->unitChange();

	colorPalette->unitChange(oldRatio, m_unitRatio, m_doc->unitIndex());
	transparencyPalette->unitChange(oldRatio, m_unitRatio, m_doc->unitIndex());
	m_haveItem = tmp;
}

void PropertiesPalette::handleNewShape(int frameType)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if ((m_item->itemType() == PageItem::PolyLine) || (m_item->itemType() == PageItem::PathText))
			return;
		shapePal->setRoundRectEnabled(frameType == 0);
	}
}

void PropertiesPalette::newSpGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk, double cx, double cy)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		QRectF upRect;
		UndoTransaction trans;
		if (UndoManager::undoEnabled())
			trans = undoManager->beginTransaction(Um::Selection, Um::ILine, Um::GradPos + "p", "", Um::ILine);
		if (m_ScMW->view->editStrokeGradient == 1)
		{
			m_item->setGradientStrokeStart(x1 / m_unitRatio, y1 / m_unitRatio);
			m_item->setGradientStrokeEnd(x2 / m_unitRatio, y2 / m_unitRatio);
			m_item->setGradientStrokeFocal(fx / m_unitRatio, fy / m_unitRatio);
			m_item->setGradientStrokeScale(sg);
			m_item->setGradientStrokeSkew(sk);
			if (m_item->strokeGradientType() == 6)
			{
				m_item->setGradientStrokeFocal(m_item->gradientStrokeStartX(), m_item->gradientStrokeStartY());
			}
			m_item->update();
			upRect = QRectF(m_item->gradientStrokeStart(), m_item->gradientStrokeEnd());
			double radEnd = distance(m_item->gradientStrokeEndX() - m_item->gradientStrokeStartX(), m_item->gradientStrokeEndY() - m_item->gradientStrokeStartY());
			double rotEnd = xy2Deg(m_item->gradientStrokeEndX() - m_item->gradientStrokeStartX(), m_item->gradientStrokeEndY() - m_item->gradientStrokeStartY());
			QTransform m;
			m.translate(m_item->gradientStrokeStartX(), m_item->gradientStrokeStartY());
			m.rotate(rotEnd);
			m.rotate(-90);
			m.rotate(m_item->gradientStrokeSkew());
			m.translate(radEnd * m_item->gradientStrokeScale(), 0);
			QPointF shP = m.map(QPointF(0,0));
			upRect = upRect.united(QRectF(shP, m_item->gradientStrokeEnd()).normalized());
			upRect = upRect.united(QRectF(shP, m_item->gradientStrokeStart()).normalized());
			upRect |= QRectF(shP, QPointF(0, 0)).normalized();
			upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		}
		else if (m_ScMW->view->editStrokeGradient == 3)
		{
			m_item->setGradientControl1(FPoint(x1 / m_unitRatio, y1 / m_unitRatio));
			m_item->setGradientControl2(FPoint(x2 / m_unitRatio, y2 / m_unitRatio));
			m_item->setGradientControl3(FPoint(fx / m_unitRatio, fy / m_unitRatio));
			m_item->setGradientControl4(FPoint(sg / m_unitRatio, sk / m_unitRatio));
			m_item->update();
			upRect = QRectF(QPointF(-m_item->width(), -m_item->height()), QPointF(m_item->width() * 2, m_item->height() * 2)).normalized();
		}
		else if (m_ScMW->view->editStrokeGradient == 4)
		{
			m_item->setGradientControl1(FPoint(x1 / m_unitRatio, y1 / m_unitRatio));
			m_item->setGradientControl2(FPoint(x2 / m_unitRatio, y2 / m_unitRatio));
			m_item->setGradientControl3(FPoint(fx / m_unitRatio, fy / m_unitRatio));
			m_item->setGradientControl4(FPoint(sg / m_unitRatio, sk / m_unitRatio));
			m_item->setGradientControl5(FPoint(cx / m_unitRatio, cy / m_unitRatio));
			m_item->update();
			upRect = QRectF(QPointF(-m_item->width(), -m_item->height()), QPointF(m_item->width() * 2, m_item->height() * 2)).normalized();
		}
		else
		{
			if (m_item->gradientType() == 13 && UndoManager::undoEnabled())
			{
				SimpleState *ss= new SimpleState("Refresh");
				ss->set("UNDO_UPDATE_CONICAL");
				undoManager->action(m_item,ss);
			}
			m_item->setGradientStart(x1 / m_unitRatio, y1 / m_unitRatio);
			m_item->setGradientEnd(x2 / m_unitRatio, y2 / m_unitRatio);
			m_item->setGradientFocal(fx / m_unitRatio, fy / m_unitRatio);
			m_item->setGradientScale(sg);
			m_item->setGradientSkew(sk);
			if (m_item->strokeGradientType() == 6)
			{
				m_item->setGradientFocal(m_item->gradientStartX(), m_item->gradientStartY());
			}
			if (m_item->gradientType() == 13 && UndoManager::undoEnabled())
			{
				m_item->createConicalMesh();
				SimpleState *ss= new SimpleState("Refresh");
				ss->set("REDO_UPDATE_CONICAL");
				undoManager->action(m_item,ss);
			}
			m_item->update();
			upRect = QRectF(m_item->gradientStart(), m_item->gradientEnd());
			double radEnd = distance(m_item->gradientEndX() - m_item->gradientStartX(), m_item->gradientEndY() - m_item->gradientStartY());
			double rotEnd = xy2Deg(m_item->gradientEndX() - m_item->gradientStartX(), m_item->gradientEndY() - m_item->gradientStartY());
			QTransform m;
			m.translate(m_item->gradientStartX(), m_item->gradientStartY());
			m.rotate(rotEnd);
			m.rotate(-90);
			m.rotate(m_item->gradientSkew());
			m.translate(radEnd * m_item->gradientScale(), 0);
			QPointF shP = m.map(QPointF(0,0));
			upRect |= QRectF(shP, m_item->gradientEnd()).normalized();
			upRect |= QRectF(shP, m_item->gradientStart()).normalized();
			upRect |= QRectF(shP, QPointF(0, 0)).normalized();
			upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		}
		if (trans)
			trans.commit();
		upRect.translate(m_item->xPos(), m_item->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
		m_doc->changed();
		m_doc->changedPagePreview();
	}
}

void PropertiesPalette::toggleGradientEdit(int stroke)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		m_ScMW->view->editStrokeGradient = stroke;
		if (stroke == 1)
		{
			if (colorPalette->gradEditButtonStroke->isChecked())
				m_ScMW->view->requestMode(modeEditGradientVectors);
			else
				m_ScMW->view->requestMode(modeNormal);
		}
		else
		{
			if ((colorPalette->gradEditButton->isChecked()) || (colorPalette->editMeshColors->isChecked()))
			{
				if ((stroke == 5) || (stroke == 6) || (stroke == 7))
					m_ScMW->view->requestMode(modeEditMeshGradient);
				else if ((stroke == 8) || (stroke == 9) || (stroke == 10) || (stroke == 11))
					m_ScMW->view->requestMode(modeEditMeshPatch);
				else
					m_ScMW->view->requestMode(modeEditGradientVectors);
			}
			else
				m_ScMW->view->requestMode(modeNormal);
		}
		m_ScMW->view->RefreshGradient(m_item);
	}
}

void PropertiesPalette::newSpGradientM(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		QRectF upRect;
		UndoTransaction trans;
		if (UndoManager::undoEnabled())
			trans = undoManager->beginTransaction(Um::Selection, Um::ILine, Um::GradPos + "o", "", Um::ILine);
		m_item->setGradientMaskStart(x1 / m_unitRatio, y1 / m_unitRatio);
		m_item->setGradientMaskEnd(x2 / m_unitRatio, y2 / m_unitRatio);
		m_item->setGradientMaskFocal(fx / m_unitRatio, fy / m_unitRatio);
		m_item->setGradientMaskScale(sg);
		m_item->setGradientMaskSkew(sk);
		if ((m_item->GrMask == GradMask_Linear) || (m_item->GrMask == GradMask_LinearLumAlpha))
		{
			m_item->setGradientMaskFocal(m_item->GrMaskStartX, m_item->GrMaskStartY);
		}
		m_item->update();
		if (trans)
		{
			trans.commit();
		}
		upRect = QRectF(m_item->gradientMaskStart(), m_item->gradientMaskEnd());
		double radEnd = distance(m_item->GrMaskEndX - m_item->GrMaskStartX, m_item->GrMaskEndY - m_item->GrMaskStartY);
		double rotEnd = xy2Deg(m_item->GrMaskEndX - m_item->GrMaskStartX, m_item->GrMaskEndY - m_item->GrMaskStartY);
		QTransform m;
		m.translate(m_item->GrMaskStartX, m_item->GrMaskStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(m_item->GrMaskSkew);
		m.translate(radEnd * m_item->GrMaskScale, 0);
		QPointF shP = m.map(QPointF(0,0));
		upRect |= QRectF(shP, m_item->gradientMaskEnd()).normalized();
		upRect |= QRectF(shP, m_item->gradientMaskStart()).normalized();
		upRect |= QRectF(shP, QPointF(0, 0)).normalized();
		upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		upRect.translate(m_item->xPos(), m_item->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
		m_doc->changed();
		m_doc->changedPagePreview();
	}
}

void PropertiesPalette::toggleGradientEditM()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		m_ScMW->view->editStrokeGradient = 2;
		if (transparencyPalette->gradEditButton->isChecked())
			m_ScMW->view->requestMode(modeEditGradientVectors);
		else
			m_ScMW->view->requestMode(modeNormal);
	}
}

void PropertiesPalette::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	colorPalette->updateColorList();
	transparencyPalette->updateColorList();
	shadowPal->updateColorList();

	assert (m_doc->PageColors.document());
}

bool PropertiesPalette::userActionOn()
{
	return xyzPal->userActionOn();
}

void PropertiesPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	DockPanelBase::changeEvent(e);
}

void PropertiesPalette::languageChange()
{
	setWindowTitle( tr("Properties"));

	scXYZ->setText(tr("X, Y, &Z"));
	scShadow->setText(tr("Drop Shadow"));
	scShape->setText(tr("&Shape"));
	scLine->setText(tr("&Line"));
	scColor->setText(tr("&Colors"));
	scTransparency->setText(tr("&Transparency"));

	xyzPal->languageChange();
	shadowPal->languageChange();
	shapePal->languageChange();
	colorPalette->languageChange();
	linePal->languageChange();
}

void PropertiesPalette::setGradientEditMode(bool on)
{
	colorPalette->gradEditButton->setChecked(on);
}

void PropertiesPalette::endPatchAdd()
{
	colorPalette->endPatchAdd();
}

void PropertiesPalette::updateColorSpecialGradient()
{
	if (!m_haveDoc)
		return;
	if(m_doc->m_Selection->isEmpty())
		return;

	PageItem* currItem = m_doc->m_Selection->itemAt(0);
	if (currItem)
	{
		if (m_ScMW->view->editStrokeGradient == 0)
			colorPalette->setSpecialGradient(currItem->GrStartX, currItem->GrStartY, currItem->GrEndX, currItem->GrEndY, currItem->GrFocalX, currItem->GrFocalY, currItem->GrScale, currItem->GrSkew, 0, 0);
		else if (m_ScMW->view->editStrokeGradient == 1)
			colorPalette->setSpecialGradient(currItem->GrStrokeStartX, currItem->GrStrokeStartY, currItem->GrStrokeEndX, currItem->GrStrokeEndY, currItem->GrStrokeFocalX, currItem->GrStrokeFocalY, currItem->GrStrokeScale, currItem->GrStrokeSkew, 0, 0);
		else if (m_ScMW->view->editStrokeGradient == 3)
			colorPalette->setSpecialGradient(currItem->GrControl1.x(), currItem->GrControl1.y(), currItem->GrControl2.x(), currItem->GrControl2.y(), currItem->GrControl3.x(), currItem->GrControl3.y(), currItem->GrControl4.x(), currItem->GrControl4.y(), 0, 0);
		else if (m_ScMW->view->editStrokeGradient == 4)
			colorPalette->setSpecialGradient(currItem->GrControl1.x(), currItem->GrControl1.y(), currItem->GrControl2.x(), currItem->GrControl2.y(), currItem->GrControl3.x(), currItem->GrControl3.y(), currItem->GrControl4.x(), currItem->GrControl4.y(), currItem->GrControl5.x(), currItem->GrControl5.y());
		else if ((m_ScMW->view->editStrokeGradient == 5) || (m_ScMW->view->editStrokeGradient == 6))
			colorPalette->setMeshPoint();
		else if (m_ScMW->view->editStrokeGradient == 8)
			colorPalette->setMeshPatchPoint();
		else if (m_ScMW->view->editStrokeGradient == 9)
			colorPalette->setMeshPatch();
		else if (currItem->isGroup())
			transparencyPalette->setSpecialGradient(currItem->GrMaskStartX, currItem->GrMaskStartY, currItem->GrMaskEndX, currItem->GrMaskEndY, currItem->GrMaskFocalX, currItem->GrMaskFocalY, currItem->GrMaskScale, currItem->GrMaskSkew);
	}
}

void PropertiesPalette::setLocked(bool isLocked)
{
	shapePal->setLocked(isLocked);
}

void PropertiesPalette::handleShapeEdit()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
		shapePal->setRoundRectEnabled(false);
}

void PropertiesPalette::handleGradientChanged()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		VGradient vg(transparencyPalette->gradEdit->gradient());
		m_doc->itemSelection_SetMaskGradient(vg);
	}
}
