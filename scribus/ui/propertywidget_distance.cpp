/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_distance.h"

#include "scribusdoc.h"
#include "units.h"
#include "util_icon.h"

PropertyWidget_Distance::PropertyWidget_Distance(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);

	layout()->setAlignment( Qt::AlignTop );

	columnsLabel->setBuddy(columns);
	columnGap->setValues(0, 300, 2, 0);

	topDistance->setValues(0, 300, 2, 0);
	topLabel->setBuddy(topDistance);

	bottomDistance->setValues(0, 300, 2, 0);
	bottomLabel->setBuddy(bottomDistance);

	leftDistance->setValues(0, 300, 2, 0);
	leftLabel->setBuddy(leftDistance);

	rightDistance->setValues(0, 300, 2, 0);
	rightLabel->setBuddy(rightDistance);

	languageChange();

	columnGapLabel->setCurrentIndex(0);
}

void PropertyWidget_Distance::setDoc(ScribusDoc *d)
{
	if(d == (ScribusDoc*) m_doc)
		return;

	m_doc  = d;

	if (m_doc.isNull())
		return;

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	columnGap->setDecimals(2);
	topDistance->setDecimals(2);
	leftDistance->setDecimals(2);
	bottomDistance->setDecimals(2);
	rightDistance->setDecimals(2);
}

void PropertyWidget_Distance::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_Distance::displayTextDistances(double left, double top, double bottom, double right)
{
	leftDistance->showValue(left*m_unitRatio);
	topDistance->showValue(top*m_unitRatio);
	bottomDistance->showValue(bottom*m_unitRatio);
	rightDistance->showValue(right*m_unitRatio);
}

void PropertyWidget_Distance::unitChange()
{
	if (!m_doc)
		return;

	double oldRatio = m_unitRatio;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	columnGap->blockSignals(true);
	leftDistance->blockSignals(true);
	topDistance->blockSignals(true);
	bottomDistance->blockSignals(true);
	rightDistance->blockSignals(true);

	columnGap->setNewUnit( m_unitIndex );
	leftDistance->setNewUnit( m_unitIndex );
	topDistance->setNewUnit( m_unitIndex );
	bottomDistance->setNewUnit( m_unitIndex );
	rightDistance->setNewUnit( m_unitIndex );

	columnGap->blockSignals(false);
	leftDistance->blockSignals(false);
	topDistance->blockSignals(false);
	bottomDistance->blockSignals(false);
	rightDistance->blockSignals(false);
}

void PropertyWidget_Distance::languageChange()
{
	columnsLabel->setText( tr("Colu&mns:"));

	int oldcolgapLabel = columnGapLabel->currentIndex();
	columnGapLabel->clear();
	columnGapLabel->addItem( tr("Gap:"));
	columnGapLabel->addItem( tr("Width:"));
	columnGapLabel->setCurrentIndex(oldcolgapLabel);

	topLabel->setText( tr("To&p:"));
	bottomLabel->setText( tr("&Bottom:"));
	leftLabel->setText( tr("&Left:"));
	rightLabel->setText( tr("&Right:"));

	tabsButton->setText( tr("T&abulators..."));

	QString ptSuffix = tr(" pt");

	QString suffix = (m_doc) ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;

	columnGap->setSuffix(suffix);
	leftDistance->setSuffix(suffix);
	topDistance->setSuffix(suffix);
	bottomDistance->setSuffix(suffix);
	rightDistance->setSuffix(suffix);

	columns->setToolTip( tr("Number of columns in text frame"));
	columnGapLabel->setToolTip( tr("Switches between Gap or Column width"));
	columnGap->setToolTip( tr("Distance between columns"));
	topDistance->setToolTip( tr("Distance of text from top of frame"));
	bottomDistance->setToolTip( tr("Distance of text from bottom of frame"));
	leftDistance->setToolTip( tr("Distance of text from left of frame"));
	rightDistance->setToolTip( tr("Distance of text from right of frame"));
	tabsButton->setToolTip( tr("Edit tab settings of text frame..."));
}