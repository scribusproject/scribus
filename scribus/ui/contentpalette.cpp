/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "contentpalette.h"

#include <QWidget>
#include <QStackedWidget>

#include "appmodehelper.h" // for AppModeChanged (if needed)

#include "propertiespalette_image.h"
#include "propertiespalette_text.h"
#include "pageitem_imageframe.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribus.h" // ScribusMainWindow
#include "selection.h"
#include "styles/paragraphstyle.h"
#include "styles/charstyle.h"

ContentPalette::ContentPalette(QWidget* parent) :
	ScDockPalette(parent, "ContentPalette", nullptr)
{
	setObjectName(QString::fromLocal8Bit("ContentPalette"));

	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);


	stackedWidget = new QStackedWidget(this);

	auto emptyPal = new QWidget();
	stackedWidget->addWidget(emptyPal);

	textPal = new PropertiesPalette_Text(this);
	stackedWidget->addWidget(textPal);

	imagePal = new PropertiesPalette_Image(this);
	stackedWidget->addWidget(imagePal);

	setWidget(stackedWidget);

	stackedWidget->setCurrentIndex((int) Panel::empty);

	languageChange();
}

void ContentPalette::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;

	textPal->setMainWindow(mw);
	imagePal->setMainWindow(mw);

	connect(m_ScMW->appModeHelper, &AppModeHelper::AppModeChanged, this, &ContentPalette::AppModeChanged);
}

void ContentPalette::setDoc(ScribusDoc *doc)
{
	if((doc == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, &Selection::selectionChanged, this, &ContentPalette::handleSelectionChanged);
		disconnect(m_doc, &ScribusDoc::docChanged, this, &ContentPalette::handleSelectionChanged);
	}

	m_doc = doc;
	m_item = nullptr;
	setEnabled(!m_doc->drawAsPreview);

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();
	m_haveDoc = true;
	m_haveItem = false;

	textPal->setDoc(m_doc);
	imagePal->setDoc(m_doc);

	updateColorList();

	connect(m_doc->m_Selection, &Selection::selectionChanged, this, &ContentPalette::handleSelectionChanged);
	connect(m_doc, &ScribusDoc::docChanged, this, &ContentPalette::handleSelectionChanged);

	// Handle properties update when switching document
	handleSelectionChanged();
}

void ContentPalette::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, &Selection::selectionChanged, this, &ContentPalette::handleSelectionChanged);
		disconnect(m_doc, &ScribusDoc::docChanged, this, &ContentPalette::handleSelectionChanged);
	}

	setEnabled(true);
	m_haveDoc = false;
	m_haveItem = false;
	m_doc=nullptr;
	m_item = nullptr;

	textPal->unsetItem();
	textPal->unsetDoc();
	imagePal->unsetItem();
	imagePal->unsetDoc();

	stackedWidget->setCurrentIndex((int) Panel::empty);
}

void ContentPalette::unsetItem()
{
	m_haveItem = false;
	m_item = nullptr;

	imagePal->unsetItem();
	textPal->unsetItem();

	handleSelectionChanged();
}

PageItem* ContentPalette::currentItemFromSelection()
{
	if (m_doc && m_doc->m_Selection->count() > 0)
	{
		return m_doc->m_Selection->itemAt(0);
	}

	return nullptr;
}

void ContentPalette::AppModeChanged()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if ((m_haveDoc) && (m_haveItem))
	{
		if (m_item->isTable())
		{
			textPal->setEnabled(m_doc->appMode == modeEditTable);
			if (m_doc->appMode == modeEditTable)
			{
				connect(m_item->asTable(), &PageItem_Table::selectionChanged, this, &ContentPalette::handleSelectionChanged);
			}
			else
			{
				disconnect(m_item->asTable(), &PageItem_Table::selectionChanged, this, &ContentPalette::handleSelectionChanged);
			}
		}
		textPal->handleSelectionChanged();
	}
}

void ContentPalette::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (!item)
	{
		unsetItem();
		return;
	}

	if (!m_doc)
	{
		setDoc(item->doc());
	}

	m_haveItem = true;
	m_item = item;

	// TODO: in PropertiesPalette there is a a funny if for the groups: take care of it when adding the group panel

	if (!sender() || (m_doc->appMode == modeEditTable))
	{
		textPal->handleSelectionChanged();
		imagePal->handleSelectionChanged();
	}
}

void  ContentPalette::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	auto currentPanel = (Panel) stackedWidget->currentIndex();
	auto newPanel{currentPanel};

	PageItem* currItem = currentItemFromSelection();

	// TODO: should me move this to setCurrentIndex()?
	if (!currItem)
	{
		newPanel = Panel::empty;
		m_haveItem = false;
	} else if (m_doc->m_Selection->count() > 1)
	{
		newPanel = Panel::empty;
	}
	else
	{
		m_haveItem = true;

		switch (currItem->itemType())
		{
		case PageItem::ImageFrame:
			newPanel = Panel::image;
			break;
		case PageItem::TextFrame:
		case PageItem::PathText:
			newPanel = Panel::text;
			break;
		case PageItem::Table:
			newPanel = m_doc->appMode == modeEditTable ? Panel::text : Panel::empty;
			break;
		default:
			newPanel = Panel::empty;
			break;
		}
		setCurrentItem(currItem);
	}
	if (currentPanel != newPanel)
	{
		stackedWidget->setCurrentIndex((int) newPanel);
		updatePanelTitle();
	}
	updateGeometry();
	ScDockPalette::update();
}

void ContentPalette::unitChange()
{
	if (!m_haveDoc)
		return;

	bool tmp = m_haveItem;
	m_haveItem = false;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	textPal->unitChange();
	imagePal->unitChange();

	m_haveItem = tmp;
}

void ContentPalette::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	textPal->updateColorList();

	assert (m_doc->PageColors.document());
}

bool ContentPalette::userActionOn()
{
	return imagePal->userActionOn();
}

void ContentPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	ScDockPalette::changeEvent(e);
}

void ContentPalette::updatePanelTitle()
{
	switch ((Panel) stackedWidget->currentIndex())
	{
		case Panel::empty:
			setWindowTitle( tr("Content Properties"));
			break;
		case Panel::text:
			setWindowTitle( tr("Text Properties"));
			break;
		case Panel::image:
			setWindowTitle( tr("Image Properties"));
			break;
	}
}

void ContentPalette::languageChange()
{
	updatePanelTitle();
	textPal->languageChange();
	imagePal->languageChange();
}

void ContentPalette::update(PageItem_ImageFrame* image)
{
	imagePal->showScaleAndOffset(image->imageXScale(), image->imageYScale(), image->imageXOffset(), image->imageYOffset());
}

void ContentPalette::update(const ParagraphStyle& style)
{
	textPal->updateStyle(style);
}

void ContentPalette::update(const CharStyle& style)
{
	textPal->updateCharStyle(style);
}

void ContentPalette::update(PageItem_TextFrame* text)
{
}

void ContentPalette::updateTextStyles()
{
	textPal->updateParagraphStyles();
}

void ContentPalette::updateTextAlignment(int i)
{
	textPal->showAlignment(i);
}

void ContentPalette::updateTextDirection(int i)
{
	textPal->showDirection(i);
}

void ContentPalette::updateTextFontSize(int i)
{
	textPal->showFontSize(i);
}

void ContentPalette::updateTextLanguage(const QString& language)
{
	textPal->showLanguage(language);
}
