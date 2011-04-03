/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_optmargins.h"

#include "scribusdoc.h"
#include "units.h"
#include "util_icon.h"

PropertyWidget_OptMargins::PropertyWidget_OptMargins(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);
	layout()->setAlignment( Qt::AlignTop );

	languageChange();
}

void PropertyWidget_OptMargins::setDoc(ScribusDoc *d)
{
	if(d == (ScribusDoc*) m_doc)
		return;

	m_doc  = d;

	if (m_doc.isNull())
		return;
}

void PropertyWidget_OptMargins::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_OptMargins::languageChange()
{
	optMarginRadioNone->setText( tr("None","optical margins") );
	optMarginRadioBoth->setText( tr("Both Sides","optical margins") );
	optMarginRadioLeft->setText( tr("Left Only","optical margins") );
	optMarginRadioRight->setText( tr("Right Only","optical margins") );
	optMarginResetButton->setText( tr("Reset") );
}