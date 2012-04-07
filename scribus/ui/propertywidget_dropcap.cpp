/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pageitem.h"
#include "pageitem_table.h"
#include "propertywidget_dropcap.h"
#include "scribus.h"
#include "selection.h"
#include "units.h"
#include "util.h"

PropertyWidget_DropCap::PropertyWidget_DropCap(QWidget *parent) : QFrame(parent)
{
	m_item = 0;
	m_ScMW = 0;

    setupUi(this);
	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);
	layout()->setAlignment( Qt::AlignTop );

	languageChange();
	dropCapLines->setDecimals(0);

	if (!m_doc) return;
	dropCapCharStyleCombo->updateFormatList();
}

void PropertyWidget_DropCap::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(AppModeChanged(int, int)), this, SLOT(handleAppModeChanged(int, int)));
	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this  , SLOT(handleUpdateRequest(int)));
}

void PropertyWidget_DropCap::setDoc(ScribusDoc *doc)
{
	if(doc == (ScribusDoc*) m_doc)
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc = doc;
	dropCapCharStyleCombo->setDoc(doc);

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();
	dropCapOffset->setSuffix(unitGetSuffixFromIndex(0));

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_DropCap::setCurrentItem(PageItem *item)
{
	if (item && m_doc.isNull())
		setDoc(item->doc());

	m_item = item;

	disconnectSignals();
	configureWidgets();

	if (!m_item) return;

	if (m_item->asTextFrame() || m_item->asPathText() || m_item->asTable())
	{
		ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
		if (m_doc->appMode == modeEdit)
			m_item->currentTextProps(parStyle);
		else if (m_doc->appMode == modeEditTable)
			m_item->asTable()->activeCell().textFrame()->currentTextProps(parStyle);
		updateStyle(parStyle);

		connectSignals();
	}
}

void PropertyWidget_DropCap::unitChange()
{
	if (!m_doc)
		return;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	dropCapOffset->blockSignals(true);
	dropCapOffset->setNewUnit( m_unitIndex );
	dropCapOffset->blockSignals(false);
}

void PropertyWidget_DropCap::updateCharStyles()
{
	dropCapCharStyleCombo->updateFormatList();
}

void PropertyWidget_DropCap::displayCharStyle(const QString& name)
{
	bool blocked = dropCapCharStyleCombo->blockSignals(true);
	dropCapCharStyleCombo->setFormat(name);
	dropCapCharStyleCombo->blockSignals(blocked);
}

void PropertyWidget_DropCap::enableDropCap(bool enable)
{
	dropCapLines->setEnabled(enable);
	dropCapOffset->setEnabled(enable);
	dropCapCharStyleCombo->setEnabled(enable);
}

void PropertyWidget_DropCap::updateStyle(const ParagraphStyle& newCurrent)
{
	disconnectSignals ();
	dropCapBox->setChecked(newCurrent.hasDropCap());
	dropCapLines->setValue(newCurrent.dropCapLines());
	dropCapOffset->setValue(newCurrent.dropCapOffset() * m_unitRatio);
	displayCharStyle(newCurrent.dcCharStyleName());
	connectSignals ();
}

void PropertyWidget_DropCap::connectSignals()
{
	connect(dropCapBox, SIGNAL(stateChanged(int)), this, SLOT(handleDropCapUse()), Qt::UniqueConnection);
	connect(dropCapLines, SIGNAL(valueChanged(double)), this, SLOT(handleDropCapLines()), Qt::UniqueConnection);
	connect(dropCapOffset, SIGNAL(valueChanged(double)), this, SLOT(handleDropCapOffset()), Qt::UniqueConnection);
	connect(dropCapCharStyleCombo, SIGNAL(activated(int)), this, SLOT(handleDropCapCharStyle()), Qt::UniqueConnection);
}

void PropertyWidget_DropCap::disconnectSignals()
{
	disconnect(dropCapBox, SIGNAL(stateChanged(int)), this, SLOT(handleDropCapUse()));
	disconnect(dropCapLines, SIGNAL(valueChanged(double)), this, SLOT(handleDropCapLines()));
	disconnect(dropCapOffset, SIGNAL(valueChanged(double)), this, SLOT(handleDropCapOffset()));
	disconnect(dropCapCharStyleCombo, SIGNAL(activated(int)), this, SLOT(handleDropCapCharStyle()));
}

void PropertyWidget_DropCap::configureWidgets(void)
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		PageItem_TextFrame *textItem = m_item->asTextFrame();
		if (m_doc->appMode == modeEditTable)
			textItem = m_item->asTable()->activeCell().textFrame();
		if (textItem || m_item->asPathText())
			enabled = true;
	}
	setEnabled(enabled);
}

void PropertyWidget_DropCap::handleAppModeChanged(int oldMode, int mode)
{
	if (oldMode == modeEditTable || mode == modeEditTable)
	{
		setCurrentItem(m_item);
	}
}

void PropertyWidget_DropCap::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
	repaint();
}

void PropertyWidget_DropCap::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqCharStylesUpdate)
		updateCharStyles();
	if (updateFlags & reqStyleComboDocUpdate)
		setDoc(m_doc ? m_doc : 0);
}

void PropertyWidget_DropCap::handleDropCapUse()
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setHasDropCap(dropCapBox->isChecked());
	PageItem *item = m_item;
	if (m_doc->appMode == modeEditTable)
		item = item->asTable()->activeCell().textFrame();
	if (item != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(item, true);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
	}
	enableDropCap(dropCapBox->isChecked());
}

void PropertyWidget_DropCap::handleDropCapLines()
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setDropCapLines(static_cast<int>(dropCapLines->value()));
	newStyle.setHasDropCap(dropCapBox->isChecked());
	PageItem *item = m_doc->m_Selection->itemAt(0);
	if (m_doc->appMode == modeEditTable)
		item = item->asTable()->activeCell().textFrame();
	if (item != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(item, true);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
	}
}

void PropertyWidget_DropCap::handleDropCapOffset()
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	newStyle.setDropCapOffset(dropCapOffset->value());
	newStyle.setHasDropCap(dropCapBox->isChecked());
	PageItem *item = m_item;
	if (m_doc->appMode == modeEditTable)
		item = item->asTable()->activeCell().textFrame();
	if (item != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(item, true);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
	}
}

void PropertyWidget_DropCap::handleDropCapCharStyle()
{
	if (!m_doc || !m_item)
		return;
	ParagraphStyle newStyle;
	QString name = dropCapCharStyleCombo->currentText();
	if (!name.isEmpty())
		newStyle.setDcCharStyleName(name);
	newStyle.setHasDropCap(dropCapBox->isChecked());
	PageItem *item = m_item;
	if (m_doc->appMode == modeEditTable)
		item = item->asTable()->activeCell().textFrame();
	if (item != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(item, true);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
	}
}

void PropertyWidget_DropCap::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_DropCap::languageChange()
{
	dropCapBox->setText(tr("Use Drop Caps"));
	dropCapLinesLabel->setText(tr("Drop Caps lines"));
	dropCapOffsetLabel->setText(tr("Drop Caps offset"));
	dropCapCharStyleLabel->setText(tr("Drop Cap use character style..."));
	dropCapCharStyleCombo->setToolTip("<qt>" + tr("Choose chracter style or leave blank for use default paragraph style"));
}

