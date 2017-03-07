/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "textpalette.h"

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
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "propertiespalette_text.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "selection.h"
#include "undomanager.h"


TextPalette::TextPalette( QWidget* parent) : ScDockPalette( parent, "TextPalette", 0)
{
	undoManager = UndoManager::instance();
	m_ScMW=0;
	m_doc=0;
	m_haveDoc = false;
	m_haveItem = false;
	m_unitRatio = 1.0;

	setObjectName(QString::fromLocal8Bit("TextPalette"));
///	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);

	textPal = new PropertiesPalette_Text( this );
	setWidget( textPal );

	languageChange();

	m_haveItem = false;
}

void TextPalette::closeEvent(QCloseEvent *closeEvent)
{
	ScDockPalette::closeEvent(closeEvent);
}

void TextPalette::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	// For some reason, the mapToGlobal() / mapFromGlobal() roundtrip
	// performed below does not give always good results, causing TP to
	// not display in some situations. Moreover the reparenting is useless 
	// as TP is already created with ScribusMainWindow as parent.
	/*QPoint p1 = mapToGlobal(pos());
	QPoint p2 = m_ScMW->mapFromGlobal(p1);
	setParent(m_ScMW);
	move(p2);*/

	this->textPal->setMainWindow(mw);

	connect(m_ScMW->appModeHelper, SIGNAL(AppModeChanged(int,int)), this, SLOT(AppModeChanged()));
}


void TextPalette::setDoc(ScribusDoc *d)
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

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();
//qt4 FIXME here
	m_haveDoc = true;
	m_haveItem = false;

	textPal->setDoc(m_doc);

	updateColorList();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));

	// Handle properties update when switching document
	handleSelectionChanged();
}

void TextPalette::unsetDoc()
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

	textPal->unsetItem();
	textPal->unsetDoc();

	m_haveItem = false;
}

void TextPalette::unsetItem()
{
	m_haveItem = false;
	m_item     = NULL;
	textPal->unsetItem();
	handleSelectionChanged();
}

PageItem* TextPalette::currentItemFromSelection()
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

void TextPalette::AppModeChanged()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if (m_item->isTable())
		{
			textPal->setEnabled(m_doc->appMode == modeEditTable);
			if (m_doc->appMode == modeEditTable)
				connect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
			else
				disconnect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		}
		textPal->handleSelectionChanged();
	}
}

void TextPalette::setCurrentItem(PageItem *i)
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

	if (!m_doc)
		setDoc(i->doc());

	m_haveItem = false;
	m_item = i;

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
		textPal->setEnabled(false);

	m_haveItem = true;

	if (!sender() || (m_doc->appMode == modeEditTable))
	{
		textPal->handleSelectionChanged();
	}

	if (m_item->asOSGFrame())
	{
		textPal->setEnabled(false);
	}
	if (m_item->asSymbolFrame())
	{
		textPal->setEnabled(false);
	}
}

void  TextPalette::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		textPal->setEnabled(false);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;
		m_haveItem   = (itemType != -1);

		switch (itemType)
		{
		case -1:
			m_haveItem = false;
			textPal->setEnabled(false);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
			textPal->setEnabled(false);
			break;
		case PageItem::TextFrame:
			textPal->setEnabled(true);
			break;
		case PageItem::Line:
			textPal->setEnabled(false);
			break;
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::RegularPolygon:
		case PageItem::Arc:
			textPal->setEnabled(false);
			break;
		case PageItem::PolyLine:
		case PageItem::Spiral:
			textPal->setEnabled(false);
			break;
		case PageItem::PathText:
			textPal->setEnabled(true);
			break;
		case PageItem::Symbol:
		case PageItem::Group:
			textPal->setEnabled(false);
			break;
		case PageItem::Table:
			textPal->setEnabled(m_doc->appMode == modeEditTable);
			break;
		}
	}

	updateGeometry();
	update();

	if (currItem)
		setCurrentItem(currItem);
}

void TextPalette::unitChange()
{
	if (!m_haveDoc)
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	textPal->unitChange();
	m_haveItem = tmp;
}

void TextPalette::NewLineMode(int mode)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	updateGeometry();
	repaint();
}

void TextPalette::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	textPal->updateColorList();

	assert (m_doc->PageColors.document());
}

bool TextPalette::userActionOn()
{
	bool userActionOn = false;
	return userActionOn;
}

void TextPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	ScDockPalette::changeEvent(e);
}

void TextPalette::languageChange()
{
	setWindowTitle( tr("Text Properties"));
	textPal->languageChange();
}


