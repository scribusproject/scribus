/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "contentpalette_default.h"

#include <QObject>
#include <QWidget>

#include "units.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "pageitem.h"
#include "selection.h"

ContentPalette_Default::ContentPalette_Default( QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	languageChange();
}

void ContentPalette_Default::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;

	connect(m_ScMW, &ScribusMainWindow::UpdateRequest, this, &ContentPalette_Default::handleUpdateRequest);
	// connect(m_ScMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
}

void ContentPalette_Default::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}
	
	m_doc  = d;
	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	m_haveDoc  = true;
	m_haveItem = false;

	setLabelText();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void ContentPalette_Default::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_haveDoc  = false;
	m_haveItem = false;
	m_doc   = nullptr;

	setLabelText();
	setEnabled(false);
}

void ContentPalette_Default::unsetItem()
{
	m_haveItem = false;
	handleSelectionChanged();
}

void ContentPalette_Default::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (m_doc->m_Selection->count() >= 1)
		m_haveItem = true;
	setLabelText();
}

void ContentPalette_Default::handleUpdateRequest(int updateFlags)
{
}

void ContentPalette_Default::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (!m_doc)
		setDoc(item->doc());

	m_haveItem = true;
}

void ContentPalette_Default::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}


void ContentPalette_Default::languageChange()
{
	retranslateUi(this);
	setLabelText();
}

void ContentPalette_Default::unitChange()
{
	if (!m_doc)
		return;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();
}

void ContentPalette_Default::setLabelText()
{
	if (!m_haveDoc || !m_doc)
	{
		label->setText( tr("<html><body><p>Open a document</p><p>and select an item to see its properties</p></body></html>"));
		return;
	}

	int selectionCount = m_doc->m_Selection->count();
	if (selectionCount > 1)
		label->setText( tr("Select a single item to see its properties"));
	else if (selectionCount == 0)
		label->setText( tr("Select an item to see its properties"));
}
