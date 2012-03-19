/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_pathtext.h"

#include "scribusdoc.h"
#include "scribus.h"
#include "selection.h"
#include "units.h"
#include "util_icon.h"

PropertyWidget_PathText::PropertyWidget_PathText(QWidget* parent) : QFrame(parent)
{
	m_ScMW = 0;
	m_doc  = 0;
	m_unitRatio  = 1.0;

	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);
	layout()->setAlignment( Qt::AlignTop );
	
	startOffset->setValues(0, 30000, 2, 0);
	startOffset->setSingleStep(10);

	distFromCurve->setValues(-300, 300, 2, 0);
	distFromCurve->setSingleStep(10);

	languageChange();
}

void PropertyWidget_PathText::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this  , SLOT(handleUpdateRequest(int)));
}

void PropertyWidget_PathText::setDoc(ScribusDoc *d)
{
	if(d == (ScribusDoc*) m_doc)
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = NULL;

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	startOffset->setMaximum( 30000 );
	startOffset->setMinimum( 0 );
	startOffset->setSingleStep(10);
	distFromCurve->setMaximum( 300 );
	distFromCurve->setMinimum( -300 );
	distFromCurve->setSingleStep(10);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_PathText::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldnt really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This wont work until when a canvas deselect happens, m_item must be NULL.
	//if (m_item == i)
	//	return;

	if (item && m_doc.isNull())
		setDoc(item->doc());

	m_item = item;

	disconnectSignals();
	configureWidgets();

	if (m_item)
	{
		if (m_item->asPathText())
		{
			pathTextType->setCurrentIndex(m_item->textPathType);
			flippedPathText->setChecked(m_item->textPathFlipped);
			showCurveCheckBox->setChecked(m_item->PoShow);
			distFromCurve->setValue(m_item->BaseOffs * -1);
			startOffset->setValue(m_item->textToFrameDistLeft());
		}
		connectSignals();
	}
}

void PropertyWidget_PathText::connectSignals()
{
	connect(showCurveCheckBox, SIGNAL(clicked())     , this, SLOT(handlePathLine()));
	connect(pathTextType     , SIGNAL(activated(int)), this, SLOT(handlePathType()));
	connect(flippedPathText  , SIGNAL(clicked())     , this, SLOT(handlePathFlip()));
	connect(startOffset      , SIGNAL(valueChanged(double)), this, SLOT(handlePathDist()));
	connect(distFromCurve    , SIGNAL(valueChanged(double)), this, SLOT(handlePathOffs()));
}

void PropertyWidget_PathText::disconnectSignals()
{
	disconnect(showCurveCheckBox, SIGNAL(clicked())     , this, SLOT(handlePathLine()));
	disconnect(pathTextType     , SIGNAL(activated(int)), this, SLOT(handlePathType()));
	disconnect(flippedPathText  , SIGNAL(clicked())     , this, SLOT(handlePathFlip()));
	disconnect(startOffset      , SIGNAL(valueChanged(double)), this, SLOT(handlePathDist()));
	disconnect(distFromCurve    , SIGNAL(valueChanged(double)), this, SLOT(handlePathOffs()));
}

void PropertyWidget_PathText::configureWidgets(void)
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		enabled  = m_item->isPathText();
		enabled &= (m_doc->m_Selection->count() == 1);
	}
	setEnabled(enabled);
}

void PropertyWidget_PathText::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
	repaint();
}

void PropertyWidget_PathText::handleUpdateRequest(int /*updateFlags*/)
{
	// Nothing to do
}

void PropertyWidget_PathText::handlePathDist()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->setTextToFrameDistLeft(startOffset->value());
	m_doc->AdjustItemSize(m_item);
	m_item->updatePolyClip();
	m_item->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertyWidget_PathText::handlePathFlip()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->textPathFlipped = flippedPathText->isChecked();
	m_item->updatePolyClip();
	m_item->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertyWidget_PathText::handlePathLine()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->PoShow = showCurveCheckBox->isChecked();
	m_item->update();
}

void PropertyWidget_PathText::handlePathOffs()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->BaseOffs = -distFromCurve->value();
	m_doc->AdjustItemSize(m_item);
	m_item->updatePolyClip();
	m_item->update();
}

void PropertyWidget_PathText::handlePathType()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->textPathType = pathTextType->currentIndex();
	m_item->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertyWidget_PathText::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_PathText::languageChange()
{
	int oldPathType = pathTextType->currentIndex();
	pathTextType->clear();
	pathTextType->addItem( tr("Default"));
	pathTextType->addItem( tr("Stair Step"));
	pathTextType->addItem( tr("Skew"));
	pathTextType->setCurrentIndex(oldPathType);
	
	flippedPathText->setText( tr("Flip Text"));
	showCurveCheckBox->setText( tr("Show Curve"));
	pathTextTypeLabel->setText( tr("Type:"));
	startOffsetLabel->setText( tr("Start Offset:"));
	distFromCurveLabel->setText( tr("Distance from Curve:"));
	
	QString ptSuffix = tr(" pt");
	startOffset->setSuffix(ptSuffix);
	distFromCurve->setSuffix(ptSuffix);
}

void PropertyWidget_PathText::unitChange()
{
	if (!m_doc)
		return;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	startOffset->setNewUnit( m_unitIndex );
	distFromCurve->setNewUnit( m_unitIndex );
}
