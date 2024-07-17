#include "propertiespalette_attributes.h"
#include "ui_propertiespalette_attributes.h"

#include "commonstrings.h"
#include "iconmanager.h"
#include "pageitem.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"

PropertiesPalette_Attributes::PropertiesPalette_Attributes(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	nameEdit->setFocusPolicy(Qt::ClickFocus);

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));


	connect(nameEdit , SIGNAL(Leaved()) , this, SLOT(handleNewName()));
	connect(noPrint  , SIGNAL(clicked()), this, SLOT(handlePrint()));

}

void PropertiesPalette_Attributes::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;
}

void PropertiesPalette_Attributes::setDoc(ScribusDoc *d)
{
	if ((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = nullptr;

	m_haveDoc = true;
	m_haveItem = false;

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Attributes::unsetDoc()
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
	nameEdit->clear();

	setEnabled(false);
}

void PropertiesPalette_Attributes::unsetItem()
{
	m_haveItem = false;
	m_item     = nullptr;
	handleSelectionChanged();
}


PageItem* PropertiesPalette_Attributes::currentItemFromSelection()
{
	PageItem *currentItem = nullptr;

	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 0)
			currentItem = m_doc->m_Selection->itemAt(0);
	}

	return currentItem;
}

void PropertiesPalette_Attributes::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (!m_doc)
		setDoc(item->doc());

	disconnect(nameEdit, SIGNAL(Leaved()), this, SLOT(handleNewName()));
	disconnect(noPrint, SIGNAL(clicked()), this, SLOT(handlePrint()));

	m_haveItem = false;
	m_item = item;

	nameEdit->setText(m_item->itemName());
	noPrint->setChecked(!item->printEnabled());

	connect(nameEdit, SIGNAL(Leaved()), this, SLOT(handleNewName()));
	connect(noPrint , SIGNAL(clicked()), this, SLOT(handlePrint()), Qt::UniqueConnection);

	m_haveItem = true;

}

void PropertiesPalette_Attributes::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

//	nameEdit->setEnabled(m_doc->m_Selection->count() == 1);

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		nameEdit->setEnabled(false);

		setEnabled(true);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;

		m_haveItem = (itemType!=-1);

		nameEdit->setEnabled(true);

		setEnabled(true);

	}
	if (currItem)
	{
		setCurrentItem(currItem);
	}
	updateGeometry();
}

void PropertiesPalette_Attributes::toggleLabelVisibility(bool visibility)
{
	formWidget->setLabelVisibility(visibility);
	labelExport->setLabelVisibility(visibility);
}

void PropertiesPalette_Attributes::showPrintingEnabled(bool isPrintingEnabled)
{
	noPrint->setChecked(!isPrintingEnabled);
}


void PropertiesPalette_Attributes::handlePrint()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_ScMW->scrActions["itemPrintingEnabled"]->toggle();
}

void PropertiesPalette_Attributes::handleNewName()
{
	if (m_ScMW->scriptIsRunning() || !m_haveDoc || !m_haveItem)
		return;
	QString NameOld = m_item->itemName();
	QString NameNew = nameEdit->text();
	if (NameNew.isEmpty())
	{
		nameEdit->setText(NameOld);
		return;
	}
	bool found = false;
	QList<PageItem*> allItems;
	for (int a = 0; a < m_doc->Items->count(); ++a)
	{
		PageItem *currItem = m_doc->Items->at(a);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			PageItem* item = allItems.at(ii);
			if ((NameNew == item->itemName()) && (item != m_item))
			{
				found = true;
				break;
			}
		}
		allItems.clear();
	}
	if (found)
	{
		ScMessageBox::warning(this, CommonStrings::trWarning, "<qt>"+ tr("Name \"%1\" isn't unique.<br/>Please choose another.").arg(NameNew)+"</qt>");
		nameEdit->setText(NameOld);
		nameEdit->setFocus();
	}
	else
	{
		if (m_item->itemName() != nameEdit->text())
		{
			m_item->setItemName(nameEdit->text());
			m_doc->changed();
		}
	}
}

void PropertiesPalette_Attributes::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void PropertiesPalette_Attributes::iconSetChange()
{
	IconManager& im = IconManager::instance();

	QIcon a2;
	a2.addPixmap(im.loadPixmap("NoPrint.png"), QIcon::Normal, QIcon::On);
	a2.addPixmap(im.loadPixmap("16/document-print.png"), QIcon::Normal, QIcon::Off);
	noPrint->setIcon(a2);

}

void PropertiesPalette_Attributes::languageChange()
{
	retranslateUi(this);
}
