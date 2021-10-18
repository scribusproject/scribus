/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QSignalBlocker>

#include "propertywidget_hyphenation.h"

#include "appmodes.h"
#include "pageitem_table.h"
#include "iconmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "selection.h"

PropertyWidget_Hyphenation::PropertyWidget_Hyphenation(QWidget* parent)
	: QFrame(parent)
{
	setupUi(this);

	hyphenCharLineEdit->setMaxLength(1);
	layout()->setAlignment( Qt::AlignTop );
	languageChange();
}

void PropertyWidget_Hyphenation::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;
}

void PropertyWidget_Hyphenation::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_Hyphenation::languageChange()
{
	retranslateUi(this);
}

void PropertyWidget_Hyphenation::handleWordMin(int minWord)
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetHyphenWordMin(minWord, &tempSelection);
}

void PropertyWidget_Hyphenation::handleConsecutiveLines(int consecutiveLines)
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetHyphenConsecutiveLines(consecutiveLines, &tempSelection);
}

void PropertyWidget_Hyphenation::handleHyphenChar(const QString& hyphenText)
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	Selection tempSelection(this, false);
	uint hyphenChar;
	if (hyphenText.isEmpty())
		hyphenChar = 0;
	else
		hyphenChar = hyphenText.toUcs4()[0];
	qDebug() << hyphenChar;
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetHyphenChar(hyphenChar, &tempSelection);
}

void PropertyWidget_Hyphenation::setDoc(ScribusDoc *d)
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

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_Hyphenation::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
	repaint();
}

void PropertyWidget_Hyphenation::updateCharStyle(const CharStyle& charStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	QSignalBlocker smallestWordSpinBoxBlocker(smallestWordSpinBox);
	QSignalBlocker hyphenCharLineEditBlocker(hyphenCharLineEdit);

	smallestWordSpinBox->setValue(charStyle.hyphenWordMin());
	uint hyphenChar = charStyle.hyphenChar();
	QString hyphenText;
	if (hyphenChar)
		hyphenText = QString::fromUcs4(&hyphenChar, 1);
	hyphenCharLineEdit->setText(hyphenText);
}

void PropertyWidget_Hyphenation::updateStyle(const ParagraphStyle& paraStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	const CharStyle& charStyle = paraStyle.charStyle();
	updateCharStyle(charStyle);

	QSignalBlocker blocker(maxConsecutiveCountSpinBox);
	maxConsecutiveCountSpinBox->setValue(paraStyle.hyphenConsecutiveLines());
}

void PropertyWidget_Hyphenation::connectSignals()
{
	connect(smallestWordSpinBox,        SIGNAL(valueChanged(int)), this, SLOT(handleWordMin(int)));
	connect(maxConsecutiveCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(handleConsecutiveLines(int)));
	connect(hyphenCharLineEdit,         SIGNAL(textChanged(const QString&)), this, SLOT(handleHyphenChar(const QString&)));
}

void PropertyWidget_Hyphenation::disconnectSignals()
{
	disconnect(smallestWordSpinBox,        SIGNAL(valueChanged(int)), this, SLOT(handleWordMin(int)));
	disconnect(maxConsecutiveCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(handleConsecutiveLines(int)));
	disconnect(hyphenCharLineEdit,         SIGNAL(textChanged(const QString&)), this, SLOT(handleHyphenChar(const QString&)));
}

void PropertyWidget_Hyphenation::configureWidgets()
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		if (m_item->isPathText() || m_item->isTextFrame() || m_item->isTable())
			enabled = true;
		if ((m_item->isGroup()) && (!m_item->isSingleSel))
			enabled = false;
		if (m_item->isOSGFrame() || m_item->isSymbol())
			enabled = false;
		if (m_doc->m_Selection->count() > 1)
			enabled = false;
	}
	setEnabled(enabled);
}

void PropertyWidget_Hyphenation::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (item && m_doc.isNull())
		setDoc(item->doc());

	m_item = item;

	disconnectSignals();
	configureWidgets();

	if (m_item)
	{
		if (m_item->isTextFrame() || m_item->isPathText() || m_item->isTable())
		{
			ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
			if (m_doc->appMode == modeEdit)
				m_item->currentTextProps(parStyle);
			else if (m_doc->appMode == modeEditTable)
				m_item->asTable()->activeCell().textFrame()->currentTextProps(parStyle);
			updateStyle(parStyle);
		}
		connectSignals();
	}
}
