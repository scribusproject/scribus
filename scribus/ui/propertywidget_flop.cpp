/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_flop.h"

#include "scribusdoc.h"
#include "units.h"

PropertyWidget_Flop::PropertyWidget_Flop(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	layout()->setAlignment(Qt::AlignTop);

	flopRealHeight->setChecked(true);

	flopGroup->setId(flopRealHeight,  RealHeightID);
	flopGroup->setId(flopFontAscent,  FontAscentID);
	flopGroup->setId(flopLineSpacing, LineSpacingID);
	flopGroup->setId(flopBaselineGrid, BaselineGridID);

	languageChange();
}

void PropertyWidget_Flop::setDoc(ScribusDoc *d)
{
	if(d == (ScribusDoc*) m_doc)
		return;

	m_doc  = d;

	if (m_doc.isNull())
		return;
}

void PropertyWidget_Flop::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_Flop::languageChange()
{
	retranslateUi(this);
}
