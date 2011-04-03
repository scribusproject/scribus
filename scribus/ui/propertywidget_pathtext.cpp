/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_pathtext.h"

#include "scribusdoc.h"
#include "units.h"
#include "util_icon.h"

PropertyWidget_PathText::PropertyWidget_PathText(QWidget* parent) : QFrame(parent)
{
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

void PropertyWidget_PathText::setDoc(ScribusDoc *d)
{
	if(d == (ScribusDoc*) m_doc)
		return;

	m_doc  = d;

	if (m_doc.isNull())
		return;

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	startOffset->setMaximum( 30000 );
	startOffset->setMinimum( 0 );
	startOffset->setSingleStep(10);
	distFromCurve->setMaximum( 300 );
	distFromCurve->setMinimum( -300 );
	distFromCurve->setSingleStep(10);
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

void PropertyWidget_PathText::unitChange()
{
	if (!m_doc)
		return;

	double oldRatio = m_unitRatio;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	startOffset->setNewUnit( m_unitIndex );
	distFromCurve->setNewUnit( m_unitIndex );
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