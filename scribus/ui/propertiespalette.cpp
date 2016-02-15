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
#include <QTransform>
#include <QObject>
#include <QPoint>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QTimer>
#include <QToolBox>
#include <QToolTip>
#include <QVBoxLayout>
#include <QValidator>
#include <QWidget>

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "appmodehelper.h"
#include "appmodes.h"
#include "arrowchooser.h"
#include "autoform.h"
#include "basepointwidget.h"
#include "commonstrings.h"
#include "colorlistbox.h"
#include "sccolorengine.h"
#include "cpalette.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "propertiespalette_group.h"
#include "propertiespalette_image.h"
#include "propertiespalette_line.h"
#include "propertiespalette_shadow.h"
#include "propertiespalette_shape.h"
#include "propertiespalette_table.h"
#include "propertiespalette_text.h"
#include "propertiespalette_utils.h"
#include "propertiespalette_xyz.h"
#include "sccombobox.h"
#include "scfonts.h"

#include "scribuscore.h"
#include "scraction.h"
#include "scribusview.h"
#include "selection.h"
#include "spalette.h"
#include "styleselect.h"
#include "tabmanager.h"
#include "transparencypalette.h"
#include "units.h"
#include "undomanager.h"
#include "util.h"
#include "util_math.h"
#include "dasheditor.h"
#include "fontcombo.h"
#include "colorcombo.h"

//using namespace std;

PropertiesPalette::PropertiesPalette( QWidget* parent) : ScDockPalette( parent, "PropertiesPalette", 0)
{
	undoManager = UndoManager::instance();
	m_ScMW=0;
	m_doc=0;
	m_haveDoc = false;
	m_haveItem = false;
	m_unitRatio = 1.0;

	setObjectName(QString::fromLocal8Bit("PropertiesPalette"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);

	TabStack = new ScTreeWidget( this );
//	Uncomment this line if the new behaviour makes any trouble.
//	TabStack->setToolBoxMode(true);

	xyzPal = new PropertiesPalette_XYZ( this );
	idXYZItem = TabStack->addItem( xyzPal, "X, Y, &Z" );

	shadowPal = new PropertiesPalette_Shadow( this );
	idShadowItem = TabStack->addItem( shadowPal, "Drop Shadow" );

	shapePal = new PropertiesPalette_Shape( this );
	idShapeItem = TabStack->addItem( shapePal, "&Shape" );

	groupPal = new PropertiesPalette_Group( this );
	idGroupItem = TabStack->addItem(groupPal, "Groups");

	textPal = new PropertiesPalette_Text( this );
	idTextItem = TabStack->addItem( textPal, "&Text" );

	imagePal = new PropertiesPalette_Image(this);
	idImageItem=TabStack->addItem( imagePal, "&Image" );

	linePal = new PropertiesPalette_Line(this);
	idLineItem=TabStack->addItem( linePal, "&Line" );

	Cpal = new Cpalette(this);
	idColorsItem = TabStack->addItem(Cpal, "&Colors" );

	Tpal = new Tpalette(this);
	idTransparencyItem = TabStack->addItem(Tpal, "&Transparency" );

	tablePal = new PropertiesPalette_Table(this);
	idTableItem = TabStack->addItem(tablePal, "T&able" );

	setWidget( TabStack );

	languageChange();

	connect(linePal, SIGNAL(lineModeChanged(int)), this, SLOT(NewLineMode(int)));

	connect(groupPal, SIGNAL(shapeChanged(int)) , this, SLOT(handleNewShape(int)));
	connect(groupPal, SIGNAL(shapeEditStarted()), this, SLOT(handleShapeEdit()));

	connect(TabStack, SIGNAL(currentChanged2(int)), this, SLOT(SelTab(int)));

	connect(Cpal, SIGNAL(NewSpecial(double, double, double, double, double, double, double, double, double, double)), this, SLOT(NewSpGradient(double, double, double, double, double, double, double, double, double, double )));
	connect(Cpal, SIGNAL(editGradient(int)), this, SLOT(toggleGradientEdit(int)));
	connect(Tpal, SIGNAL(NewSpecial(double, double, double, double, double, double, double, double, double, double)), this, SLOT(NewSpGradientM(double, double, double, double, double, double, double, double )));
	connect(Tpal, SIGNAL(editGradient()), this, SLOT(toggleGradientEditM()));

	m_haveItem = false;
	for (int ws = 1; ws < 10; ++ws)
		TabStack->setItemEnabled(ws, false);
	TabStack->setCurrentIndex(0);
	TabStack->widget(0)->setEnabled(false);
	TabStack->setItemEnabled(0, false);
}

void PropertiesPalette::closeEvent(QCloseEvent *closeEvent)
{
	if (m_ScMW && !m_ScMW->scriptIsRunning())
	{
		if ((m_haveDoc) && (m_haveItem))
		{
			if (Cpal->gradEditButton->isChecked())
			{
				m_ScMW->view->requestMode(modeNormal);
				m_ScMW->view->RefreshGradient(m_item);
			}
		}
	}
	ScDockPalette::closeEvent(closeEvent);
}

void PropertiesPalette::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW=mw;
	QPoint p1 = mapToGlobal(pos());
	QPoint p2 = m_ScMW->mapFromGlobal(p1);
	//Qt4 reparent(m_ScMW, this->getWFlags(), p2);
	setParent(m_ScMW);
	move(p2);

	this->xyzPal->setMainWindow(mw);
	this->shadowPal->setMainWindow(mw);
	this->shapePal->setMainWindow(mw);
	this->groupPal->setMainWindow(mw);
	this->imagePal->setMainWindow(mw);
	this->linePal->setMainWindow(mw);
	this->textPal->setMainWindow(mw);
	this->tablePal->setMainWindow(mw);

	//connect(this->Cpal, SIGNAL(gradientChanged()), m_ScMW, SLOT(updtGradFill()));
	//connect(this->Cpal, SIGNAL(strokeGradientChanged()), m_ScMW, SLOT(updtGradStroke()));
	connect(this->Tpal, SIGNAL(gradientChanged()), this, SLOT(handleGradientChanged()));
	connect(m_ScMW->appModeHelper, SIGNAL(AppModeChanged(int,int)), this, SLOT(AppModeChanged()));
}

void PropertiesPalette::SelTab(int t)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	foreach (QObject *o, TabStack->widget(t)->children())
	{
		// Layouts, boxes etc aren't widgets at all
		// so let's skip them silently...
		QWidget *w = qobject_cast<QWidget*>(o);
		if (w)
		{
			QWidget *i = TabStack->widget(t);
			while ((i = i->nextInFocusChain()) != TabStack->widget(t))
			{
				if (((i->focusPolicy() & Qt::TabFocus) == Qt::TabFocus) && !i->focusProxy() && i->isEnabled())
				{
					i->setFocus();
					break;
				}
			}
		}
	}
}

void PropertiesPalette::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc = d;
	m_item = NULL;
	setEnabled(!m_doc->drawAsPreview);
	Cpal->setDocument(m_doc);
	Cpal->setCurrentItem(NULL);
	Tpal->setDocument(m_doc);
	Tpal->setCurrentItem(NULL);

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();
//qt4 FIXME here
	m_haveDoc = true;
	m_haveItem = false;

	xyzPal->setDoc(m_doc);
	shadowPal->setDoc(m_doc);
	shapePal->setDoc(m_doc);
	groupPal->setDoc(m_doc);
	imagePal->setDoc(m_doc);
	linePal->setDoc(m_doc);
	textPal->setDoc(m_doc);
	tablePal->setDocument(m_doc);

	updateColorList();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));

	// Handle properties update when switching document
	handleSelectionChanged();
}

void PropertiesPalette::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}
	setEnabled(true);
	m_haveDoc = false;
	m_haveItem = false;
	m_doc=NULL;
	m_item = NULL;

	imagePal->unsetItem();
	xyzPal->unsetDoc();
	shadowPal->unsetItem();
	shadowPal->unsetDoc();
	shapePal->unsetItem();
	shapePal->unsetDoc();
	groupPal->unsetItem();
	groupPal->unsetDoc();
	imagePal->unsetItem();
	imagePal->unsetDoc();
	linePal->unsetItem();
	linePal->unsetDoc();
	textPal->unsetItem();
	textPal->unsetDoc();
	tablePal->unsetItem();
	tablePal->unsetDocument();

	Cpal->setCurrentItem(NULL);
	Cpal->setDocument(NULL);
	Tpal->setCurrentItem(NULL);
	Tpal->setDocument(NULL);

	m_haveItem = false;
	for (int ws = 1; ws < 10; ++ws)
		TabStack->setItemEnabled(ws, false);
	TabStack->widget(0)->setEnabled(false);
	TabStack->setItemEnabled(idXYZItem, false);
}

void PropertiesPalette::unsetItem()
{
	m_haveItem = false;
	m_item     = NULL;
	Cpal->setCurrentItem(NULL);
	Tpal->setCurrentItem(NULL);
	imagePal->unsetItem();
	tablePal->unsetItem();
	shapePal->unsetItem();
	groupPal->unsetItem();
	textPal->unsetItem();
	shadowPal->unsetItem();
	imagePal->unsetItem();
	linePal->unsetItem();
	handleSelectionChanged();
}

void PropertiesPalette::setTextFlowMode(PageItem::TextFlowMode mode)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveItem)
		return;
	shapePal->showTextFlowMode(mode);
	groupPal->showTextFlowMode(mode);
}

PageItem* PropertiesPalette::currentItemFromSelection()
{
	PageItem *currentItem = NULL;

	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 0)
			currentItem = m_doc->m_Selection->itemAt(0);
	/*	if (m_doc->m_Selection->count() > 1)
		{
			int lowestItem = 999999;
			for (int a=0; a<m_doc->m_Selection->count(); ++a)
			{
				currentItem = m_doc->m_Selection->itemAt(a);
				lowestItem = qMin(lowestItem, m_doc->Items->indexOf(currentItem));
			}
			currentItem = m_doc->Items->at(lowestItem);
		}
		else if (m_doc->m_Selection->count() == 1)
		{
			currentItem = m_doc->m_Selection->itemAt(0);
		} */
	}

	return currentItem;
}

void PropertiesPalette::AppModeChanged()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if (m_item->isTable())
		{
			TabStack->setItemEnabled(idTextItem, m_doc->appMode == modeEditTable);
			if (m_doc->appMode == modeEditTable)
				connect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
			else
				disconnect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		}
		textPal->handleSelectionChanged();
	}
}

void PropertiesPalette::setCurrentItem(PageItem *i)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be NULL.
	//if (m_item == i)
	//	return;

	if (!i)
	{
		unsetItem();
		return;
	}

	int oldTabIndex = TabStack->currentIndex();

	if (!m_doc)
		setDoc(i->doc());

	disconnect(TabStack, SIGNAL(currentChanged2(int)) , this, SLOT(SelTab(int)));
	disconnect(linePal , SIGNAL(lineModeChanged(int)), this, SLOT(NewLineMode(int)));

	m_haveItem = false;
	m_item = i;

	tablePal->setItem(m_item);

	Tpal->setCurrentItem(m_item);

	setTextFlowMode(m_item->textFlowMode());

	connect(linePal , SIGNAL(lineModeChanged(int)), this, SLOT(NewLineMode(int)));

//CB replaces old emits from PageItem::emitAllToGUI()
	setLocked(i->locked());

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idShadowItem, true);
		TabStack->setItemEnabled(idShapeItem, false);
		TabStack->setItemEnabled(idGroupItem, true);
		TabStack->setItemEnabled(idLineItem, false);
		TabStack->setItemEnabled(idColorsItem, false);
		TabStack->setItemEnabled(idTextItem, false);
		TabStack->setItemEnabled(idImageItem, false);
		TabStack->setItemEnabled(idTableItem, false);
	}
	else
		TabStack->setItemEnabled(idGroupItem, false);

	m_haveItem = true;

	if (oldTabIndex != TabStack->currentIndex())
	{
		//Must be called only when necessary : cause focus problem
		//in spinboxes when processing valueChanged() signals
		SelTab(TabStack->currentIndex());
	}

	if (!sender() || (m_doc->appMode == modeEditTable))
	{
		xyzPal->handleSelectionChanged();
		shadowPal->handleSelectionChanged();
		shapePal->handleSelectionChanged();
		groupPal->handleSelectionChanged();
		imagePal->handleSelectionChanged();
		linePal->handleSelectionChanged();
		textPal->handleSelectionChanged();
		tablePal->handleSelectionChanged();
		Cpal->handleSelectionChanged();
	}

	if (m_item->asOSGFrame())
	{
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idShadowItem, true);
		TabStack->setItemEnabled(idShapeItem, true);
		TabStack->setItemEnabled(idGroupItem, false);
		TabStack->setItemEnabled(idLineItem, false);
		TabStack->setItemEnabled(idColorsItem, true);
		TabStack->setItemEnabled(idTableItem, false);
		TabStack->setItemEnabled(idTransparencyItem, false);
		TabStack->setItemEnabled(idTextItem, false);
		TabStack->setItemEnabled(idImageItem, false);
		TabStack->setItemEnabled(idTableItem, false);
	}
	if (m_item->asSymbolFrame())
	{
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idShadowItem, true);
		TabStack->setItemEnabled(idShapeItem, false);
		TabStack->setItemEnabled(idGroupItem, true);
		TabStack->setItemEnabled(idLineItem, false);
		TabStack->setItemEnabled(idColorsItem, false);
		TabStack->setItemEnabled(idTextItem, false);
		TabStack->setItemEnabled(idImageItem, false);
		TabStack->setItemEnabled(idTransparencyItem, false);
		TabStack->setItemEnabled(idTableItem, false);
	}
	connect(TabStack, SIGNAL(currentChanged2(int)), this, SLOT(SelTab(int)));
}

void  PropertiesPalette::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	int currentTab = TabStack->currentIndex();
	disconnect(TabStack, SIGNAL(currentChanged2(int)), this, SLOT(SelTab(int)));

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		for (int ws = 1; ws < 10; ++ws)
			TabStack->setItemEnabled(ws, false);
		TabStack->widget(0)->setEnabled(true);
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idShadowItem, true);
		TabStack->setItemEnabled(idLineItem, true);
		TabStack->setItemEnabled(idColorsItem, true);
		TabStack->setItemEnabled(idTransparencyItem, true);
		TabStack->setItemEnabled(idTableItem, false); // At least not for now.
		if (m_haveItem && m_item)
		{
			if ((m_item->isGroup()) && (!m_item->isSingleSel))
				TabStack->setItemEnabled(idGroupItem, true);
		}
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;
		m_haveItem   = (itemType != -1);

//		int visID = TabStack->currentIndex();
		TabStack->widget(0)->setEnabled(true);
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idColorsItem, true);
		TabStack->setItemEnabled(idTransparencyItem, true);
		TabStack->setItemEnabled(idTableItem, false);
		switch (itemType)
		{
		case -1:
			m_haveItem = false;
			for (int ws = 1; ws < 10; ++ws)
				TabStack->setItemEnabled(ws, false);
			TabStack->widget(0)->setEnabled(false);
			TabStack->setItemEnabled(idXYZItem, false);
			Cpal->showGradient(0);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
			if (currItem->asOSGFrame())
			{
				TabStack->setItemEnabled(idXYZItem, true);
				TabStack->setItemEnabled(idShadowItem, true);
				TabStack->setItemEnabled(idShapeItem, true);
				TabStack->setItemEnabled(idGroupItem, false);
				TabStack->setItemEnabled(idLineItem, false);
				TabStack->setItemEnabled(idColorsItem, true);
				TabStack->setItemEnabled(idTransparencyItem, false);
				TabStack->setItemEnabled(idTextItem, false);
				TabStack->setItemEnabled(idImageItem, false);
			}
			else
			{
				TabStack->setItemEnabled(idXYZItem, true);
				TabStack->setItemEnabled(idShadowItem, true);
				TabStack->setItemEnabled(idShapeItem, true);
				TabStack->setItemEnabled(idTextItem, false);
				TabStack->setItemEnabled(idImageItem, true);
				TabStack->setItemEnabled(idLineItem, true);
			}
			break;
		case PageItem::TextFrame:
			TabStack->setItemEnabled(idShadowItem, true);
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, true);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			break;
		case PageItem::Line:
			TabStack->setItemEnabled(idShadowItem, true);
			TabStack->setItemEnabled(idShapeItem, false);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			break;
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::RegularPolygon:
		case PageItem::Arc:
			TabStack->setItemEnabled(idShadowItem, true);
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			break;
		case PageItem::PolyLine:
		case PageItem::Spiral:
			TabStack->setItemEnabled(idShadowItem, true);
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			break;
		case PageItem::PathText:
			TabStack->setItemEnabled(idShadowItem, true);
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, true);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			break;
		case PageItem::Symbol:
		case PageItem::Group:
			TabStack->setItemEnabled(idShadowItem, true);
			TabStack->setItemEnabled(idShapeItem, false);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, false);
			TabStack->setItemEnabled(idGroupItem, true);
			TabStack->setItemEnabled(idColorsItem, false);
			TabStack->setItemEnabled(idTransparencyItem, false);
			break;
		case PageItem::Table:
			TabStack->setItemEnabled(idTableItem, true);
			TabStack->setItemEnabled(idShadowItem, true);
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, m_doc->appMode == modeEditTable);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, false);
			TabStack->setItemEnabled(idGroupItem, false);
			TabStack->setItemEnabled(idColorsItem, false);
			TabStack->setItemEnabled(idTransparencyItem, false);
			break;
		}
	}
	if (TabStack->isItemEnabled(currentTab) && (TabStack->currentIndex() != currentTab))
		TabStack->setCurrentIndex(currentTab);
	updateGeometry();
	repaint();
	connect(TabStack, SIGNAL(currentChanged2(int)), this, SLOT(SelTab(int)));

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
	groupPal->unitChange();
	imagePal->unitChange();
	linePal->unitChange();
	textPal->unitChange();

	Cpal->unitChange(oldRatio, m_unitRatio, m_doc->unitIndex());
	Tpal->unitChange(oldRatio, m_unitRatio, m_doc->unitIndex());
	m_haveItem = tmp;
}

void PropertiesPalette::NewLineMode(int mode)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	xyzPal->setLineMode(mode);
	xyzPal->showWH(m_item->width(), m_item->height());
	updateGeometry();
	repaint();
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

void PropertiesPalette::NewSpGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk, double cx, double cy)
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
			m_item->setGradientStrokeStartX(x1 / m_unitRatio);
			m_item->setGradientStrokeStartY(y1 / m_unitRatio);
			m_item->setGradientStrokeEndX(x2 / m_unitRatio);
			m_item->setGradientStrokeEndY(y2 / m_unitRatio);
			m_item->setGradientStrokeFocalX(fx / m_unitRatio);
			m_item->setGradientStrokeFocalY(fy / m_unitRatio);
			m_item->setGradientStrokeScale(sg);
			m_item->setGradientStrokeSkew(sk);
			if (m_item->strokeGradientType() == 6)
			{
				m_item->setGradientStrokeFocalX(m_item->gradientStrokeStartX());
				m_item->setGradientStrokeFocalY(m_item->gradientStrokeStartY());
			}
			m_item->update();
			upRect = QRectF(QPointF(m_item->gradientStrokeStartX(), m_item->gradientStrokeStartY()), QPointF(m_item->gradientStrokeEndX(), m_item->gradientStrokeEndY()));
			double radEnd = distance(m_item->gradientStrokeEndX() - m_item->gradientStrokeStartX(), m_item->gradientStrokeEndY() - m_item->gradientStrokeStartY());
			double rotEnd = xy2Deg(m_item->gradientStrokeEndX() - m_item->gradientStrokeStartX(), m_item->gradientStrokeEndY() - m_item->gradientStrokeStartY());
			QTransform m;
			m.translate(m_item->gradientStrokeStartX(), m_item->gradientStrokeStartY());
			m.rotate(rotEnd);
			m.rotate(-90);
			m.rotate(m_item->gradientStrokeSkew());
			m.translate(radEnd * m_item->gradientStrokeScale(), 0);
			QPointF shP = m.map(QPointF(0,0));
			upRect = upRect.united(QRectF(shP, QPointF(m_item->gradientStrokeEndX(), m_item->gradientStrokeEndY())).normalized());
			upRect = upRect.united(QRectF(shP, QPointF(m_item->gradientStrokeStartX(), m_item->gradientStrokeStartY())).normalized());
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
			m_item->setGradientStartX(x1 / m_unitRatio);
			m_item->setGradientStartY(y1 / m_unitRatio);
			m_item->setGradientEndX(x2 / m_unitRatio);
			m_item->setGradientEndY(y2 / m_unitRatio);
			m_item->setGradientFocalX(fx / m_unitRatio);
			m_item->setGradientFocalY(fy / m_unitRatio);
			m_item->setGradientScale(sg);
			m_item->setGradientSkew(sk);
			if (m_item->strokeGradientType() == 6)
			{
				m_item->setGradientFocalX(m_item->gradientStartX());
				m_item->setGradientFocalY(m_item->gradientStartY());
			}
			if (m_item->gradientType() == 13 && UndoManager::undoEnabled())
			{
				m_item->createConicalMesh();
				SimpleState *ss= new SimpleState("Refresh");
				ss->set("REDO_UPDATE_CONICAL");
				undoManager->action(m_item,ss);
			}
			m_item->update();
			upRect = QRectF(QPointF(m_item->gradientStartX(), m_item->gradientStartY()), QPointF(m_item->gradientEndX(), m_item->gradientEndY()));
			double radEnd = distance(m_item->gradientEndX() - m_item->gradientStartX(), m_item->gradientEndY() - m_item->gradientStartY());
			double rotEnd = xy2Deg(m_item->gradientEndX() - m_item->gradientStartX(), m_item->gradientEndY() - m_item->gradientStartY());
			QTransform m;
			m.translate(m_item->gradientStartX(), m_item->gradientStartY());
			m.rotate(rotEnd);
			m.rotate(-90);
			m.rotate(m_item->gradientSkew());
			m.translate(radEnd * m_item->gradientScale(), 0);
			QPointF shP = m.map(QPointF(0,0));
			upRect |= QRectF(shP, QPointF(m_item->gradientEndX(), m_item->gradientEndY())).normalized();
			upRect |= QRectF(shP, QPointF(m_item->gradientStartX(), m_item->gradientStartY())).normalized();
			upRect |= QRectF(shP, QPointF(0, 0)).normalized();
			upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		}
		if (trans)
			trans.commit();
		upRect.translate(m_item->xPos(), m_item->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
		m_doc->changed();
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
			if (Cpal->gradEditButtonStroke->isChecked())
				m_ScMW->view->requestMode(modeEditGradientVectors);
			else
				m_ScMW->view->requestMode(modeNormal);
		}
		else
		{
			if ((Cpal->gradEditButton->isChecked()) || (Cpal->editMeshColors->isChecked()))
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

void PropertiesPalette::NewSpGradientM(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		QRectF upRect;
		UndoTransaction trans;
		if (UndoManager::undoEnabled())
			trans = undoManager->beginTransaction(Um::Selection, Um::ILine, Um::GradPos + "o", "", Um::ILine);
		m_item->setGradientMaskStartX(x1 / m_unitRatio);
		m_item->setGradientMaskStartY(y1 / m_unitRatio);
		m_item->setGradientMaskEndX(x2 / m_unitRatio);
		m_item->setGradientMaskEndY(y2 / m_unitRatio);
		m_item->setGradientMaskFocalX(fx / m_unitRatio);
		m_item->setGradientMaskFocalY(fy / m_unitRatio);
		m_item->setGradientMaskScale(sg);
		m_item->setGradientMaskSkew(sk);
		if ((m_item->GrMask == 1) || (m_item->GrMask == 4))
		{
			m_item->setGradientMaskFocalX(m_item->GrMaskStartX);
			m_item->setGradientMaskFocalY(m_item->GrMaskStartY);
		}
		m_item->update();
		if (trans)
		{
			trans.commit();
		}
		upRect = QRectF(QPointF(m_item->GrMaskStartX, m_item->GrMaskStartY), QPointF(m_item->GrMaskEndX, m_item->GrMaskEndY));
		double radEnd = distance(m_item->GrMaskEndX - m_item->GrMaskStartX, m_item->GrMaskEndY - m_item->GrMaskStartY);
		double rotEnd = xy2Deg(m_item->GrMaskEndX - m_item->GrMaskStartX, m_item->GrMaskEndY - m_item->GrMaskStartY);
		QTransform m;
		m.translate(m_item->GrMaskStartX, m_item->GrMaskStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(m_item->GrMaskSkew);
		m.translate(radEnd * m_item->GrMaskScale, 0);
		QPointF shP = m.map(QPointF(0,0));
		upRect |= QRectF(shP, QPointF(m_item->GrMaskEndX, m_item->GrMaskEndY)).normalized();
		upRect |= QRectF(shP, QPointF(m_item->GrMaskStartX, m_item->GrMaskStartY)).normalized();
		upRect |= QRectF(shP, QPointF(0, 0)).normalized();
		upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		upRect.translate(m_item->xPos(), m_item->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
		m_doc->changed();
	}
}

void PropertiesPalette::toggleGradientEditM()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		m_ScMW->view->editStrokeGradient = 2;
		if (Tpal->gradEditButton->isChecked())
			m_ScMW->view->requestMode(modeEditGradientVectors);
		else
			m_ScMW->view->requestMode(modeNormal);
	}
}

void PropertiesPalette::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	groupPal->updateColorList();
	textPal->updateColorList();
	tablePal->updateColorList();
	Cpal->updateColorList();
	Tpal->updateColorList();
	shadowPal->updateColorList();

	assert (m_doc->PageColors.document());
}

bool PropertiesPalette::userActionOn()
{
	bool userActionOn = false;
	userActionOn  = xyzPal->userActionOn();
	userActionOn |= imagePal->userActionOn();
	return userActionOn;
}

void PropertiesPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	ScDockPalette::changeEvent(e);
}

void PropertiesPalette::languageChange()
{
	setWindowTitle( tr("Properties"));

	TabStack->setItemText(idXYZItem, tr("X, Y, &Z"));
	TabStack->setItemText(idTextItem, tr("&Text"));
	TabStack->setItemText(idImageItem, tr("&Image"));
	TabStack->setItemText(idShadowItem, tr("Drop Shadow"));
	TabStack->setItemText(idShapeItem, tr("&Shape"));
	TabStack->setItemText(idLineItem, tr("&Line"));
	TabStack->setItemText(idColorsItem, tr("&Colors"));
	TabStack->setItemText(idGroupItem, tr("&Group"));
	TabStack->setItemText(idTransparencyItem, tr("&Transparency"));
	TabStack->setItemText(idTableItem, tr("T&able"));

	xyzPal->languageChange();
	shadowPal->languageChange();
	shapePal->languageChange();
	groupPal->languageChange();
	imagePal->languageChange();
	Cpal->languageChange();
	linePal->languageChange();
	textPal->languageChange();
	tablePal->languageChange();
}

void PropertiesPalette::setGradientEditMode(bool on)
{
	Cpal->gradEditButton->setChecked(on);
}

void PropertiesPalette::endPatchAdd()
{
	Cpal->endPatchAdd();
}

void PropertiesPalette::updateColorSpecialGradient()
{
	if (!m_haveDoc)
		return;
	if(m_doc->m_Selection->isEmpty())
		return;

	groupPal->updateColorSpecialGradient();

	PageItem *currItem=m_doc->m_Selection->itemAt(0);
	if (currItem)
	{
		if (m_ScMW->view->editStrokeGradient == 0)
			Cpal->setSpecialGradient(currItem->GrStartX, currItem->GrStartY, currItem->GrEndX, currItem->GrEndY, currItem->GrFocalX, currItem->GrFocalY, currItem->GrScale, currItem->GrSkew, 0, 0);
		else if (m_ScMW->view->editStrokeGradient == 1)
			Cpal->setSpecialGradient(currItem->GrStrokeStartX, currItem->GrStrokeStartY, currItem->GrStrokeEndX, currItem->GrStrokeEndY, currItem->GrStrokeFocalX, currItem->GrStrokeFocalY, currItem->GrStrokeScale, currItem->GrStrokeSkew, 0, 0);
		else if (m_ScMW->view->editStrokeGradient == 3)
			Cpal->setSpecialGradient(currItem->GrControl1.x(), currItem->GrControl1.y(), currItem->GrControl2.x(), currItem->GrControl2.y(), currItem->GrControl3.x(), currItem->GrControl3.y(), currItem->GrControl4.x(), currItem->GrControl4.y(), 0, 0);
		else if (m_ScMW->view->editStrokeGradient == 4)
			Cpal->setSpecialGradient(currItem->GrControl1.x(), currItem->GrControl1.y(), currItem->GrControl2.x(), currItem->GrControl2.y(), currItem->GrControl3.x(), currItem->GrControl3.y(), currItem->GrControl4.x(), currItem->GrControl4.y(), currItem->GrControl5.x(), currItem->GrControl5.y());
		else if ((m_ScMW->view->editStrokeGradient == 5) || (m_ScMW->view->editStrokeGradient == 6))
			Cpal->setMeshPoint();
		else if (m_ScMW->view->editStrokeGradient == 8)
			Cpal->setMeshPatchPoint();
		else if (m_ScMW->view->editStrokeGradient == 9)
			Cpal->setMeshPatch();
		else if (!currItem->isGroup())
			Tpal->setSpecialGradient(currItem->GrMaskStartX, currItem->GrMaskStartY, currItem->GrMaskEndX, currItem->GrMaskEndY, currItem->GrMaskFocalX, currItem->GrMaskFocalY, currItem->GrMaskScale, currItem->GrMaskSkew);
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
		VGradient vg(Tpal->gradEdit->gradient());
		m_doc->itemSelection_SetMaskGradient(vg);
	}
}
