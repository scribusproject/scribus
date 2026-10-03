/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "contentpalette.h"

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "appmodehelper.h" // for AppModeChanged (if needed)

#include "propertiespalette_group.h"
#include "propertiespalette_image.h"
#include "contentpalette_default.h"
#include "contentpalette_page.h"
#include "propertiespalette_table.h"
#include "propertiespalette_text.h"
#include "pageitem_imageframe.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribus.h" // ScribusMainWindow
#include "selection.h"
#include "styles/paragraphstyle.h"
#include "styles/charstyle.h"
#include "widgets/inspector_header.h"
#include "widgets/section_container.h"

ContentPalette::ContentPalette(QWidget *parent) :
	DockPanelBase("ContentPalette", "inspector-content", parent)
{
	setObjectName(QString::fromLocal8Bit("ContentPalette"));

	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);

	stackedWidget = new StackedContainer(this);

	defaultPal = new ContentPalette_Default(this);
	stackedWidget->addWidget(defaultPal);

	groupPal = new PropertiesPalette_Group(this);
	stackedWidget->addWidget(groupPal);

	imagePal = new PropertiesPalette_Image(this);
	stackedWidget->addWidget(imagePal);

	pagePal = new ContentPalette_Page(this);
	stackedWidget->addWidget(pagePal);

	tablePal = new PropertiesPalette_Table(this);
	stackedWidget->addWidget(tablePal);

	textPal = new PropertiesPalette_Text(this);
	stackedWidget->addWidget(textPal);

	auto* panel = new QWidget(this);
	auto* layout = new QVBoxLayout(panel);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	m_inspectorHeader = new InspectorHeader(QStringLiteral("inspector-content"), panel);
	layout->addWidget(m_inspectorHeader);
	layout->addWidget(stackedWidget, 1);
	setWidget(panel);

	const auto sections = panel->findChildren<SectionContainer*>();
	for (auto* section : sections)
	{
		section->setProperty("inspectorSection", true);
		section->setAttribute(Qt::WA_StyledBackground, true);
		section->setHeaderSize(SectionContainerHeader::Normal);
		section->setHeaderType(SectionContainerHeader::Header);
		section->setHasStyle(true);
	}

	for (auto* contentPanel : { static_cast<QWidget*>(defaultPal), static_cast<QWidget*>(groupPal),
		static_cast<QWidget*>(imagePal), static_cast<QWidget*>(pagePal),
		static_cast<QWidget*>(tablePal), static_cast<QWidget*>(textPal) })
	{
		if (contentPanel->layout())
		{
			contentPanel->layout()->setContentsMargins(6, 6, 6, 6);
			contentPanel->layout()->setSpacing(6);
		}
	}

	stackedWidget->setCurrentIndex((int) Panel::empty);

	languageChange();
}

void ContentPalette::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;

	defaultPal->setMainWindow(mw);
	groupPal->setMainWindow(mw);
	imagePal->setMainWindow(mw);
	pagePal->setMainWindow(mw);
	tablePal->setMainWindow(mw);
	textPal->setMainWindow(mw);

	connect(m_ScMW->appModeHelper, &AppModeHelper::AppModeChanged, this, &ContentPalette::AppModeChanged);
}

void ContentPalette::setDoc(ScribusDoc *doc)
{
	if((doc == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_textSelectionConnection);
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

	defaultPal->setDoc(m_doc);
	groupPal->setDoc(m_doc);
	imagePal->setDoc(m_doc);
	pagePal->setDoc(m_doc);
	tablePal->setDocument(m_doc);
	textPal->setDoc(m_doc);

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
		disconnect(m_textSelectionConnection);
		disconnect(m_doc->m_Selection, &Selection::selectionChanged, this, &ContentPalette::handleSelectionChanged);
		disconnect(m_doc, &ScribusDoc::docChanged, this, &ContentPalette::handleSelectionChanged);
	}

	setEnabled(true);
	m_haveDoc = false;
	m_haveItem = false;
	m_doc = nullptr;
	m_item = nullptr;

	defaultPal->unsetItem();
	defaultPal->unsetDoc();
	groupPal->unsetItem();
	groupPal->unsetDoc();
	imagePal->unsetItem();
	imagePal->unsetDoc();
	pagePal->unsetItem();
	pagePal->unsetDoc();
	tablePal->unsetItem();
	tablePal->unsetDocument();
	textPal->unsetItem();
	textPal->unsetDoc();

	stackedWidget->setCurrentIndex((int) Panel::empty);
	updatePanelTitle();
	emit inspectorTargetChanged(InspectorContent);
}

void ContentPalette::unsetItem()
{
	disconnect(m_textSelectionConnection);
	m_haveItem = false;
	m_item = nullptr;

	defaultPal->unsetItem();
	groupPal->unsetItem();
	imagePal->unsetItem();
	tablePal->unsetItem();
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

	if (m_haveDoc && m_haveItem)
	{
		if (m_item->isTable())
		{
			// TODO: a.l.e: fix the table selection
			// when a cell is selected it should show the table pane for now
			// you need to enter the text edit mode to format text.
			// in the future we will probably add a multiple panes at once
			// (on top of each other? switching?)
			// tablePal->setEnabled(m_doc->appMode == modeEditTable);
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
	handleSelectionChanged();
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

	if (item != m_item)
	{
		disconnect(m_textSelectionConnection);
		if (item->asTextFrame())
			m_textSelectionConnection = connect(&item->itemText, &StoryText::selectionChanged,
				this, &ContentPalette::handleSelectionChanged, Qt::QueuedConnection);
	}
	m_haveItem = true;
	m_item = item;

	tablePal->setItem(m_item);

	// TODO: in PropertiesPalette there is a a funny if for the groups: take care of it when adding the group panel

	if (!sender() || (m_doc->appMode == modeEditTable))
	{
		defaultPal->handleSelectionChanged();
		groupPal->handleSelectionChanged();
		imagePal->handleSelectionChanged();
		tablePal->handleSelectionChanged();
		textPal->handleSelectionChanged();
	}
}

void  ContentPalette::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	auto currentPanel = (Panel) stackedWidget->currentIndex();
	auto newPanel{currentPanel};
	auto inspectorTarget = InspectorContent;

	PageItem* currItem = currentItemFromSelection();
	PageItem *selectedAnchor = m_doc->appMode == modeEdit && currItem && currItem->asTextFrame()
		? currItem->asTextFrame()->selectedAnchoredObject() : nullptr;

	// TODO: should me move this to setCurrentIndex()?
	if (!currItem)
	{
		newPanel = Panel::page;
		m_haveItem = false;
	}
	else if (m_doc->m_Selection->count() > 1)
	{
		newPanel = Panel::empty;
		inspectorTarget = InspectorAlignment;
		m_haveItem = false;
	}
	else
	{
		m_haveItem = true;

		if (selectedAnchor && selectedAnchor->isImageFrame())
			newPanel = Panel::image;
		else if (selectedAnchor && selectedAnchor->isTable())
			newPanel = Panel::table;
		else
		{
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
				newPanel = m_doc->appMode == modeEditTable && !static_cast<PageItem_Table*>(currItem)->hasSelection() ? Panel::text : Panel::table;
				break;
			case PageItem::Group:
				newPanel = Panel::group;
				break;
			default:
				newPanel = Panel::empty;
				inspectorTarget = InspectorAppearance;
				break;
			}
		}
		setCurrentItem(currItem);
		if (selectedAnchor && selectedAnchor->isImageFrame())
			imagePal->handleSelectionChanged();
		else if (selectedAnchor && selectedAnchor->isTable())
			tablePal->handleSelectionChanged();
	}
	if (currentPanel != newPanel)
	{
		stackedWidget->setCurrentIndex((int) newPanel);
		updatePanelTitle();
	}
	emit inspectorTargetChanged(inspectorTarget);
	updateGeometry();
	DockPanelBase::update();
}

void ContentPalette::unitChange()
{
	if (!m_haveDoc)
		return;

	bool tmp = m_haveItem;
	m_haveItem = false;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	groupPal->unitChange();
	imagePal->unitChange();
	pagePal->unitChange();
	textPal->unitChange();
	tablePal->unitChange();

	m_haveItem = tmp;
}

void ContentPalette::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	tablePal->updateColorList();

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
	DockPanelBase::changeEvent(e);
}

void ContentPalette::updatePanelTitle()
{
	setWindowTitle(tr("Content"));

	switch ((Panel) stackedWidget->currentIndex())
	{
		case Panel::empty:
			m_inspectorHeader->setTitle(tr("Content"));
			m_inspectorHeader->setSubtitle(tr("Select an object to edit its content"));
			break;
		case Panel::group:
			m_inspectorHeader->setTitle(tr("Group"));
			m_inspectorHeader->setSubtitle(tr("Grouped object content and options"));
			break;
		case Panel::image:
			m_inspectorHeader->setTitle(tr("Image"));
			m_inspectorHeader->setSubtitle(tr("Fitting, crop, resolution, and colour"));
			break;
		case Panel::page:
			m_inspectorHeader->setTitle(tr("Page"));
			m_inspectorHeader->setSubtitle(tr("Document and page settings"));
			break;
		case Panel::table:
			m_inspectorHeader->setTitle(tr("Table"));
			m_inspectorHeader->setSubtitle(tr("Table and cell content"));
			break;
		case Panel::text:
			m_inspectorHeader->setTitle(tr("Text"));
			m_inspectorHeader->setSubtitle(tr("Typography, columns, spacing, and flow"));
			break;
	}
}

void ContentPalette::languageChange()
{
	updatePanelTitle();
	defaultPal->languageChange();
	groupPal->languageChange();
	imagePal->languageChange();
	tablePal->languageChange();
	textPal->languageChange();
}

void ContentPalette::update(PageItem_ImageFrame* image)
{
	imagePal->showScaleAndOffset(image->imageXScale(), image->imageYScale(), image->imageXOffset(), image->imageYOffset());
}

void ContentPalette::update(const ParagraphStyle& style)
{
	textPal->updateParagraphStyle(style);
}

void ContentPalette::update(const CharStyle& style)
{
	textPal->updateCharStyle(style);
}

void ContentPalette::update(PageItem_TextFrame* /*text*/)
{
}

void ContentPalette::updateTextStyles()
{
	textPal->updateTextStyles();
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
