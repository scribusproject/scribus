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

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "arrowchooser.h"
#include "autoform.h"
#include "basepointwidget.h"
#include "commonstrings.h"
#include "colorlistbox.h"
#include "sccolorengine.h"
#include "cpalette.h"
#include "pageitem_textframe.h"
#include "propertiespalette_group.h"
#include "propertiespalette_image.h"
#include "propertiespalette_line.h"
#include "propertiespalette_shape.h"
#include "propertiespalette_text.h"
#include "propertiespalette_utils.h"
#include "propertiespalette_xyz.h"
#include "sccombobox.h"
#include "scfonts.h"
#include "scribus.h"
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
#include "util_icon.h"
#include "util_math.h"
#include "text/nlsconfig.h"
#include "dasheditor.h"
#include "fontcombo.h"
#include "colorcombo.h"

//using namespace std;

PropertiesPalette::PropertiesPalette( QWidget* parent) : ScDockPalette( parent, "PropertiesPalette", 0)
{
	m_ScMW=0;
	m_doc=0;
	m_haveDoc = false;
	m_haveItem = false;
	m_unitRatio = 1.0;

	//setObjectName(QString::fromLocal8Bit("PropertiesPalette"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	MpalLayout = new QVBoxLayout();
	MpalLayout->setMargin(5);
	MpalLayout->setSpacing(1);
	setLayout(MpalLayout);

	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);

	TabStack = new QToolBox( this );

	page = new QWidget( TabStack );
	
	pageLayout = new QVBoxLayout( page );
	pageLayout->setMargin(0);
	pageLayout->setSpacing(5);

	xyzPal = new PropertiesPalette_XYZ( page );
	pageLayout->addWidget( xyzPal );

	QSpacerItem* spacer13 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout->addItem( spacer13 );
	idXYZItem = TabStack->addItem( page, "X, Y, &Z" );

	page_2 = new QWidget( TabStack );
	pageLayout_2 = new QVBoxLayout( page_2 );
	pageLayout_2->setSpacing( 5 );
	pageLayout_2->setMargin( 0 );

	shapePal = new PropertiesPalette_Shape( page_2 );
	pageLayout_2->addWidget( shapePal );

	QSpacerItem* spacer6 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout_2->addItem( spacer6 );
	idShapeItem = TabStack->addItem( page_2, "&Shape" );

	page_group = new QWidget(TabStack);
	page_group_layout = new QVBoxLayout( page_group );
	page_group_layout->setSpacing( 5 );
	page_group_layout->setMargin( 0 );

	groupPal = new PropertiesPalette_Group( page_group );
	page_group_layout->addWidget( groupPal );

	QSpacerItem* spacerTr2 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	page_group_layout->addItem( spacerTr2 );
	idGroupItem = TabStack->addItem(page_group, "Groups");

	page_3 = new QWidget( TabStack );
	pageLayout_3 = new QVBoxLayout( page_3 );
	pageLayout_3->setSpacing( 5 );
	pageLayout_3->setMargin( 0 );
	pageLayout_3->setAlignment( Qt::AlignLeft );

	textPal = new PropertiesPalette_Text( page_group );
	pageLayout_3->addWidget( textPal );

	idTextItem = TabStack->addItem( page_3, "&Text" );

	page_4 = new QWidget( TabStack );
	pageLayout_4 = new QVBoxLayout( page_4 );
	pageLayout_4->setSpacing( 5 );
	pageLayout_4->setMargin( 0 );

	imagePal = new PropertiesPalette_Image(page_4);
	pageLayout_4->addWidget(imagePal);

	QSpacerItem* spacer9 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pageLayout_4->addItem( spacer9 );
	idImageItem=TabStack->addItem( page_4, "&Image" );

	page_5 = new QWidget( TabStack );
	pageLayout_5 = new QVBoxLayout( page_5 );
	pageLayout_5->setSpacing( 5 );
	pageLayout_5->setMargin( 0 );

	linePal = new PropertiesPalette_Line(page_5);
	pageLayout_5->addWidget(linePal);

	idLineItem=TabStack->addItem( page_5, "&Line" );
	
	page_6 = new QWidget( TabStack );
	pageLayout_6 = new QVBoxLayout( page_6 );
	pageLayout_6->setSpacing( 5 );
	pageLayout_6->setMargin( 0 );
	Cpal = new Cpalette(page_6);
	pageLayout_6->addWidget( Cpal );
	idColorsItem = TabStack->addItem(page_6, "&Colors" );
	
	page_7 = new QWidget( TabStack );
	pageLayout_7 = new QVBoxLayout( page_7 );
	pageLayout_7->setSpacing( 5 );
	pageLayout_7->setMargin( 0 );
	Tpal = new Tpalette(page_7);
	pageLayout_7->addWidget( Tpal );
	idTransparencyItem = TabStack->addItem(page_7, "&Transparency" );

	MpalLayout->addWidget( TabStack );

	setWidget( TabStack );

	languageChange();

	connect(linePal, SIGNAL(lineModeChanged(int)), this, SLOT(NewLineMode(int)));

	connect(groupPal, SIGNAL(shapeChanged(int)) , this, SLOT(handleNewShape(int)));
	connect(groupPal, SIGNAL(shapeEditStarted()), this, SLOT(handleShapeEdit()));

	connect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));

	connect(Cpal, SIGNAL(NewSpecial(double, double, double, double, double, double, double, double, double, double)), this, SLOT(NewSpGradient(double, double, double, double, double, double, double, double, double, double )));
	connect(Cpal, SIGNAL(editGradient(int)), this, SLOT(toggleGradientEdit(int)));
	connect(Tpal, SIGNAL(NewSpecial(double, double, double, double, double, double, double, double, double, double)), this, SLOT(NewSpGradientM(double, double, double, double, double, double, double, double )));
	connect(Tpal, SIGNAL(editGradient()), this, SLOT(toggleGradientEditM()));

	m_haveItem = false;
	for (int ws = 1; ws < 8; ++ws)
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
	this->shapePal->setMainWindow(mw);
	this->groupPal->setMainWindow(mw);
	this->imagePal->setMainWindow(mw);
	this->linePal->setMainWindow(mw);
	this->textPal->setMainWindow(mw);

	connect(this, SIGNAL(DocChanged()), m_ScMW, SLOT(slotDocCh()));
	//connect(this->Cpal, SIGNAL(gradientChanged()), m_ScMW, SLOT(updtGradFill()));
	//connect(this->Cpal, SIGNAL(strokeGradientChanged()), m_ScMW, SLOT(updtGradStroke()));
	connect(this->Tpal, SIGNAL(gradientChanged()), m_ScMW, SLOT(updtGradMask()));
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
	Cpal->setDocument(m_doc);
	Cpal->setCurrentItem(NULL);
	Tpal->setDocument(m_doc);
	Tpal->setCurrentItem(NULL);

	m_unitRatio=m_doc->unitRatio();
	m_unitIndex=m_doc->unitIndex();
	int precision = unitGetPrecisionFromIndex(m_unitIndex);
//qt4 FIXME here
	double maxXYWHVal= 16777215 * m_unitRatio;
	double minXYVal= -16777215 * m_unitRatio;
	m_haveDoc = true;
	m_haveItem = false;

	xyzPal->setDoc(m_doc);
	shapePal->setDoc(m_doc);
	groupPal->setDoc(m_doc);
	imagePal->setDoc(m_doc);
	linePal->setDoc(m_doc);
	textPal->setDoc(m_doc);

	updateColorList();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_haveDoc = false;
	m_haveItem = false;
	m_doc=NULL;
	m_item = NULL;

	xyzPal->unsetItem();
	xyzPal->unsetDoc();
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

	Cpal->setCurrentItem(NULL);
	Cpal->setDocument(NULL);
	Tpal->setCurrentItem(NULL);
	Tpal->setDocument(NULL);

	m_haveItem = false;
	for (int ws = 1; ws < 8; ++ws)
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
	handleSelectionChanged();
}

void PropertiesPalette::setTextFlowMode(PageItem::TextFlowMode mode)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveItem)
		return;
	shapePal->displayTextFlowMode(mode);
	groupPal->displayTextFlowMode(mode);
}

PageItem* PropertiesPalette::currentItemFromSelection()
{
	PageItem *currentItem = NULL;

	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 1)
		{
			uint lowestItem = 999999;
			for (int a=0; a<m_doc->m_Selection->count(); ++a)
			{
				currentItem = m_doc->m_Selection->itemAt(a);
				lowestItem = qMin(lowestItem, currentItem->ItemNr);
			}
			currentItem = m_doc->Items->at(lowestItem);
		}
		else if (m_doc->m_Selection->count() == 1)
		{
			currentItem = m_doc->m_Selection->itemAt(0);
		}
	}

	return currentItem;
}

void PropertiesPalette::setCurrentItem(PageItem *i)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldnt really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This wont work until when a canvas deselect happens, m_item must be NULL.
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

	disconnect(TabStack, SIGNAL(currentChanged(int)) , this, SLOT(SelTab(int)));
	disconnect(linePal , SIGNAL(lineModeChanged(int)), this, SLOT(NewLineMode(int)));

	m_haveItem = false;
	m_item = i;

	/*xyzPal->handleSelectionChanged();
	shapePal->handleSelectionChanged();
	groupPal->handleSelectionChanged();
	imagePal->handleSelectionChanged();
	linePal->handleSelectionChanged();
	textPal->handleSelectionChanged();*/

	/*Cpal->setCurrentItem(m_item);
	Cpal->updateFromItem();*/
	Tpal->setCurrentItem(m_item);
	Tpal->updateFromItem();

	setTextFlowMode(m_item->textFlowMode());

	connect(linePal , SIGNAL(lineModeChanged(int)), this, SLOT(NewLineMode(int)));

//CB replaces old emits from PageItem::emitAllToGUI()
	setLocked(i->locked());

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idShapeItem, false);
		TabStack->setItemEnabled(idGroupItem, true);
		TabStack->setItemEnabled(idLineItem, false);
		TabStack->setItemEnabled(idColorsItem, false);
		TabStack->setItemEnabled(idTextItem, false);
		TabStack->setItemEnabled(idImageItem, false);
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

	xyzPal->handleSelectionChanged();
	shapePal->handleSelectionChanged();
	groupPal->handleSelectionChanged();
	imagePal->handleSelectionChanged();
	linePal->handleSelectionChanged();
	textPal->handleSelectionChanged();

	if (m_item->asOSGFrame())
	{
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idShapeItem, true);
		TabStack->setItemEnabled(idGroupItem, false);
		TabStack->setItemEnabled(idLineItem, false);
		TabStack->setItemEnabled(idColorsItem, true);
		TabStack->setItemEnabled(idTransparencyItem, false);
		TabStack->setItemEnabled(idTextItem, false);
		TabStack->setItemEnabled(idImageItem, false);
	}
	if (m_item->asSymbolFrame())
	{
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idShapeItem, false);
		TabStack->setItemEnabled(idGroupItem, true);
		TabStack->setItemEnabled(idLineItem, false);
		TabStack->setItemEnabled(idColorsItem, false);
		TabStack->setItemEnabled(idTextItem, false);
		TabStack->setItemEnabled(idImageItem, false);
		TabStack->setItemEnabled(idTransparencyItem, false);
	}
	connect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));
}

void  PropertiesPalette::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	int currentTab = TabStack->currentIndex();
	disconnect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));

	qDebug() << "PropertiesPalette::handleSelectionChanged()";

	/*xyzPal->handleSelectionChanged();
	shapePal->handleSelectionChanged();
	groupPal->handleSelectionChanged();
	imagePal->handleSelectionChanged();
	linePal->handleSelectionChanged();
	textPal->handleSelectionChanged();*/

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		for (int ws = 1; ws < 8; ++ws)
			TabStack->setItemEnabled(ws, false);
		TabStack->widget(0)->setEnabled(true);
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idLineItem, false);
		TabStack->setItemEnabled(idColorsItem, true);
		TabStack->setItemEnabled(idTransparencyItem, true);
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

		int visID = TabStack->currentIndex();
		TabStack->widget(0)->setEnabled(true);
		TabStack->setItemEnabled(idXYZItem, true);
		TabStack->setItemEnabled(idColorsItem, true);
		TabStack->setItemEnabled(idTransparencyItem, true);
		switch (itemType)
		{
		case -1:
			m_haveItem = false;
			for (int ws = 1; ws < 8; ++ws)
				TabStack->setItemEnabled(ws, false);
			TabStack->widget(0)->setEnabled(false);
			TabStack->setItemEnabled(idXYZItem, false);
			Cpal->displayGradient(0);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
			if (currItem->asOSGFrame())
			{
				TabStack->setItemEnabled(idXYZItem, true);
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
				TabStack->setItemEnabled(idShapeItem, true);
				TabStack->setItemEnabled(idTextItem, false);
				TabStack->setItemEnabled(idImageItem, true);
				TabStack->setItemEnabled(idLineItem, true);
			}
			break;
		case PageItem::TextFrame:
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, true);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			break;
		case PageItem::Line:
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
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			break;
		case PageItem::PolyLine:
		case PageItem::Spiral:
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			break;
		case PageItem::PathText:
			TabStack->setItemEnabled(idShapeItem, true);
			TabStack->setItemEnabled(idTextItem, true);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, true);
			break;
		case PageItem::Symbol:
		case PageItem::Group:
			TabStack->setItemEnabled(idShapeItem, false);
			TabStack->setItemEnabled(idTextItem, false);
			TabStack->setItemEnabled(idImageItem, false);
			TabStack->setItemEnabled(idLineItem, false);
			TabStack->setItemEnabled(idGroupItem, true);
			TabStack->setItemEnabled(idColorsItem, false);
			TabStack->setItemEnabled(idTransparencyItem, false);
			break;
		}
	}
	if (TabStack->isItemEnabled(currentTab) && (TabStack->currentIndex() != currentTab))
		TabStack->setCurrentIndex(currentTab);
	updateGeometry();
	repaint();
	connect(TabStack, SIGNAL(currentChanged(int)), this, SLOT(SelTab(int)));

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
	xyzPal->displayWH(m_item->width(), m_item->height());
	updateGeometry();
	repaint();
}

void PropertiesPalette::NewAlignement(int a)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetAlignment(a);
}

void PropertiesPalette::handleNewShape(int frameType)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if ((m_item->itemType() == PageItem::PolyLine) || (m_item->itemType() == PageItem::PathText))
			return;

		if ((m_item->itemType() == PageItem::ImageFrame) || (m_item->itemType() == PageItem::TextFrame))
		{
			shapePal->setRoundRectEnabled(frameType == 0);
			return;
		}
//		m_item->convertTo(PageItem::Polygon);
//		newSelection(6);
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
		if (m_ScMW->view->editStrokeGradient == 1)
		{
			m_item->GrStrokeStartX = x1 / m_unitRatio;
			m_item->GrStrokeStartY = y1 / m_unitRatio;
			m_item->GrStrokeEndX = x2 / m_unitRatio;
			m_item->GrStrokeEndY = y2 / m_unitRatio;
			m_item->GrStrokeFocalX = fx / m_unitRatio;
			m_item->GrStrokeFocalY = fy / m_unitRatio;
			m_item->GrStrokeScale = sg;
			m_item->GrStrokeSkew = sk;
			if (m_item->GrTypeStroke == 6)
			{
				m_item->GrStrokeFocalX = m_item->GrStrokeStartX;
				m_item->GrStrokeFocalY = m_item->GrStrokeStartY;
			}
			m_item->update();
			upRect = QRectF(QPointF(m_item->GrStrokeStartX, m_item->GrStrokeStartY), QPointF(m_item->GrStrokeEndX, m_item->GrStrokeEndY));
			double radEnd = distance(m_item->GrStrokeEndX - m_item->GrStrokeStartX, m_item->GrStrokeEndY - m_item->GrStrokeStartY);
			double rotEnd = xy2Deg(m_item->GrStrokeEndX - m_item->GrStrokeStartX, m_item->GrStrokeEndY - m_item->GrStrokeStartY);
			QTransform m;
			m.translate(m_item->GrStrokeStartX, m_item->GrStrokeStartY);
			m.rotate(rotEnd);
			m.rotate(-90);
			m.rotate(m_item->GrStrokeSkew);
			m.translate(radEnd * m_item->GrStrokeScale, 0);
			QPointF shP = m.map(QPointF(0,0));
			upRect = upRect.united(QRectF(shP, QPointF(m_item->GrStrokeEndX, m_item->GrStrokeEndY)).normalized());
			upRect = upRect.united(QRectF(shP, QPointF(m_item->GrStrokeStartX, m_item->GrStrokeStartY)).normalized());
			upRect |= QRectF(shP, QPointF(0, 0)).normalized();
			upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		}
		else if (m_ScMW->view->editStrokeGradient == 3)
		{
			m_item->GrControl1 = FPoint(x1 / m_unitRatio, y1 / m_unitRatio);
			m_item->GrControl2 = FPoint(x2 / m_unitRatio, y2 / m_unitRatio);
			m_item->GrControl3 = FPoint(fx / m_unitRatio, fy / m_unitRatio);
			m_item->GrControl4 = FPoint(sg / m_unitRatio, sk / m_unitRatio);
			m_item->update();
			upRect = QRectF(QPointF(-m_item->width(), -m_item->height()), QPointF(m_item->width() * 2, m_item->height() * 2)).normalized();
		}
		else if (m_ScMW->view->editStrokeGradient == 4)
		{
			m_item->GrControl1 = FPoint(x1 / m_unitRatio, y1 / m_unitRatio);
			m_item->GrControl2 = FPoint(x2 / m_unitRatio, y2 / m_unitRatio);
			m_item->GrControl3 = FPoint(fx / m_unitRatio, fy / m_unitRatio);
			m_item->GrControl4 = FPoint(sg / m_unitRatio, sk / m_unitRatio);
			m_item->GrControl5 = FPoint(cx / m_unitRatio, cy / m_unitRatio);
			m_item->update();
			upRect = QRectF(QPointF(-m_item->width(), -m_item->height()), QPointF(m_item->width() * 2, m_item->height() * 2)).normalized();
		}
		else
		{
			m_item->GrStartX = x1 / m_unitRatio;
			m_item->GrStartY = y1 / m_unitRatio;
			m_item->GrEndX = x2 / m_unitRatio;
			m_item->GrEndY = y2 / m_unitRatio;
			m_item->GrFocalX = fx / m_unitRatio;
			m_item->GrFocalY = fy / m_unitRatio;
			m_item->GrScale = sg;
			m_item->GrSkew = sk;
			if (m_item->GrTypeStroke == 6)
			{
				m_item->GrFocalX = m_item->GrStartX;
				m_item->GrFocalY = m_item->GrStartY;
			}
			m_item->update();
			upRect = QRectF(QPointF(m_item->GrStartX, m_item->GrStartY), QPointF(m_item->GrEndX, m_item->GrEndY));
			double radEnd = distance(m_item->GrEndX - m_item->GrStartX, m_item->GrEndY - m_item->GrStartY);
			double rotEnd = xy2Deg(m_item->GrEndX - m_item->GrStartX, m_item->GrEndY - m_item->GrStartY);
			QTransform m;
			m.translate(m_item->GrStartX, m_item->GrStartY);
			m.rotate(rotEnd);
			m.rotate(-90);
			m.rotate(m_item->GrSkew);
			m.translate(radEnd * m_item->GrScale, 0);
			QPointF shP = m.map(QPointF(0,0));
			upRect |= QRectF(shP, QPointF(m_item->GrEndX, m_item->GrEndY)).normalized();
			upRect |= QRectF(shP, QPointF(m_item->GrStartX, m_item->GrStartY)).normalized();
			upRect |= QRectF(shP, QPointF(0, 0)).normalized();
			upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		}
		upRect.translate(m_item->xPos(), m_item->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
		emit DocChanged();
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
		m_item->GrMaskStartX = x1 / m_unitRatio;
		m_item->GrMaskStartY = y1 / m_unitRatio;
		m_item->GrMaskEndX = x2 / m_unitRatio;
		m_item->GrMaskEndY = y2 / m_unitRatio;
		m_item->GrMaskFocalX = fx / m_unitRatio;
		m_item->GrMaskFocalY = fy / m_unitRatio;
		m_item->GrMaskScale = sg;
		m_item->GrMaskSkew = sk;
		if ((m_item->GrMask == 1) || (m_item->GrMask == 4))
		{
			m_item->GrMaskFocalX = m_item->GrMaskStartX;
			m_item->GrMaskFocalY = m_item->GrMaskStartY;
		}
		m_item->update();
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
		emit DocChanged();
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

	Cpal->updateColorList();
	Tpal->updateColorList();

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
	TabStack->setItemText(idShapeItem, tr("&Shape"));
	TabStack->setItemText(idLineItem, tr("&Line"));
	TabStack->setItemText(idColorsItem, tr("&Colors"));
	TabStack->setItemText(idGroupItem, tr("&Group"));
	TabStack->setItemText(idTransparencyItem, tr("&Transparency"));

	xyzPal->languageChange();
	shapePal->languageChange();
	groupPal->languageChange();
	imagePal->languageChange();
	linePal->languageChange();
	textPal->languageChange();
}

const VGradient PropertiesPalette::getMaskGradient()
{
	return Tpal->gradEdit->gradient();
}

const VGradient PropertiesPalette::getMaskGradientGroup()
{
	return groupPal->getMaskGradientGroup();
}

void PropertiesPalette::setGradientEditMode(bool on)
{
	Cpal->gradEditButton->setChecked(on);
}

void PropertiesPalette::updateColorSpecialGradient()
{
	if (!m_haveDoc)
		return;
	if(m_doc->m_Selection->isEmpty())
		return;

	groupPal->updateColorSpecialGradient();

	double dur=m_doc->unitRatio();
	PageItem *currItem=m_doc->m_Selection->itemAt(0);
	if (currItem)
	{
		if (m_ScMW->view->editStrokeGradient == 0)
			Cpal->setSpecialGradient(currItem->GrStartX * dur, currItem->GrStartY * dur, currItem->GrEndX * dur, currItem->GrEndY * dur, currItem->GrFocalX * dur, currItem->GrFocalY * dur, currItem->GrScale, currItem->GrSkew, 0, 0);
		else if (m_ScMW->view->editStrokeGradient == 1)
			Cpal->setSpecialGradient(currItem->GrStrokeStartX * dur, currItem->GrStrokeStartY * dur, currItem->GrStrokeEndX * dur, currItem->GrStrokeEndY * dur, currItem->GrStrokeFocalX * dur, currItem->GrStrokeFocalY * dur, currItem->GrStrokeScale, currItem->GrStrokeSkew, 0, 0);
		else if (m_ScMW->view->editStrokeGradient == 3)
			Cpal->setSpecialGradient(currItem->GrControl1.x() * dur, currItem->GrControl1.y() * dur, currItem->GrControl2.x() * dur, currItem->GrControl2.y() * dur, currItem->GrControl3.x() * dur, currItem->GrControl3.y() * dur, currItem->GrControl4.x() * dur, currItem->GrControl4.y() * dur, 0, 0);
		else if (m_ScMW->view->editStrokeGradient == 4)
			Cpal->setSpecialGradient(currItem->GrControl1.x() * dur, currItem->GrControl1.y() * dur, currItem->GrControl2.x() * dur, currItem->GrControl2.y() * dur, currItem->GrControl3.x() * dur, currItem->GrControl3.y() * dur, currItem->GrControl4.x() * dur, currItem->GrControl4.y() * dur, currItem->GrControl5.x(), currItem->GrControl5.y());
		else if ((m_ScMW->view->editStrokeGradient == 5) || (m_ScMW->view->editStrokeGradient == 6))
			Cpal->setMeshPoint();
		else if (m_ScMW->view->editStrokeGradient == 7)
			Cpal->setMeshControlPoint();
		else if (!currItem->isGroup())
			Tpal->setSpecialGradient(currItem->GrMaskStartX * dur, currItem->GrMaskStartY * dur, currItem->GrMaskEndX * dur, currItem->GrMaskEndY * dur, currItem->GrMaskFocalX * dur, currItem->GrMaskFocalY * dur, currItem->GrMaskScale, currItem->GrMaskSkew);
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
	{
		shapePal->setRoundRectEnabled(false);
	}
}
