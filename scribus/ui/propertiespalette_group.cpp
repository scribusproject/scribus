/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertiespalette_group.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QEvent>
#include <QFocusEvent>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QListView>
#include <QTransform>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExp>
#include <QSpacerItem>
#include <QSpinBox>
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

#include "appmodes.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "scraction.h"
#include "scribuscore.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/autoform.h"
#include "ui/nodeeditpalette.h"
#include "ui/propertiespalette.h"
#include "ui/propertiespalette_utils.h"
#include "ui/propertiespalette_xyz.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"

PropertiesPalette_Group::PropertiesPalette_Group( QWidget* parent)
	: QWidget(parent)
{
	m_tmpSelection = new Selection(this, false);
	m_tmpSelection->clear();

	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	languageChange();

	connect(clipGroups   , SIGNAL(clicked())                 , this, SLOT(handleClipping()) );
}

PageItem* PropertiesPalette_Group::currentItemFromSelection()
{
	PageItem *currentItem = nullptr;

	if (m_doc && m_doc->m_Selection->count()>0)
		currentItem = m_doc->m_Selection->itemAt(0);

	return currentItem;
}

void PropertiesPalette_Group::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Group::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}
	
	m_doc  = d;
	m_item = nullptr;
	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	m_haveDoc  = true;
	m_haveItem = false;

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Group::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_haveDoc  = false;
	m_haveItem = false;
	m_doc   = nullptr;
	m_item  = nullptr;

	setEnabled(false);
}

void PropertiesPalette_Group::unsetItem()
{
	m_haveItem = false;
	m_item     = nullptr;
	handleSelectionChanged();
}

void PropertiesPalette_Group::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		bool widgetEnabled = false;
		if (m_haveItem && m_item)
		{
			if ((m_item->isGroup()) && (!m_item->isSingleSel))
				widgetEnabled = true;
		}
		setEnabled(widgetEnabled);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;
		m_haveItem = (itemType != -1);

		if (itemType == PageItem::Symbol)
			setEnabled(true);
	}
	if (currItem)
	{
		setCurrentItem(currItem);
	}
	updateGeometry();
}

void PropertiesPalette_Group::handleUpdateRequest(int updateFlags)
{
}

void PropertiesPalette_Group::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be nullptr.
	//if (m_item == i)
	//	return;

	if (!m_doc)
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	clipGroups->setChecked(m_item->groupClipping());

	if (((m_item->isGroup()) && (!m_item->isSingleSel)) || m_item->asSymbol())
		setEnabled(true);
	else
		setEnabled(false);

	m_haveItem = true;
}

void PropertiesPalette_Group::handleClipping()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->setGroupClipping(clipGroups->isChecked());
	m_item->update();
}

void PropertiesPalette_Group::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}


void PropertiesPalette_Group::languageChange()
{
	retranslateUi(this);
}

void PropertiesPalette_Group::unitChange()
{
	if (!m_doc)
		return;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();
}
