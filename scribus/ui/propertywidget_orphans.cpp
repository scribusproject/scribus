/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_orphans.h"

#include "scribus.h"

PropertyWidget_Orphans::PropertyWidget_Orphans(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);
	layout()->setAlignment( Qt::AlignTop );
	keepLinesStart->setDecimals(0);
	keepLinesEnd->setDecimals(0);

	languageChange();
}

void PropertyWidget_Orphans::connectSignals()
{
	connect(keepLinesStart, SIGNAL(valueChanged(double)), this, SLOT(handleKeepLinesStart()));
	connect(keepLinesEnd, SIGNAL(valueChanged(double)), this, SLOT(handleKeepLinesEnd()));
	connect(keepTogether, SIGNAL(stateChanged(int)), this, SLOT(handleKeepTogether()));
	connect(keepWithNext, SIGNAL(stateChanged(int)), this, SLOT(handleKeepWithNext()));
}

void PropertyWidget_Orphans::disconnectSignals()
{
	disconnect(keepLinesStart, SIGNAL(valueChanged(double)), this, SLOT(handleKeepLinesStart()));
	disconnect(keepLinesEnd, SIGNAL(valueChanged(double)), this, SLOT(handleKeepLinesEnd()));
	disconnect(keepTogether, SIGNAL(stateChanged(int)), this, SLOT(handleKeepTogether()));
	disconnect(keepWithNext, SIGNAL(stateChanged(int)), this, SLOT(handleKeepWithNext()));
}

void PropertyWidget_Orphans::handleKeepLinesStart()
{
	if (!m_doc) return;
	ParagraphStyle newStyle;
	newStyle.setKeepLinesStart (keepLinesStart->value());
	m_doc->itemSelection_ApplyParagraphStyle(newStyle);
}

void PropertyWidget_Orphans::handleKeepLinesEnd()
{
	if (!m_doc) return;
	ParagraphStyle newStyle;
	newStyle.setKeepLinesEnd (keepLinesEnd->value());
	m_doc->itemSelection_ApplyParagraphStyle(newStyle);
}

void PropertyWidget_Orphans::handleKeepTogether()
{
	if (!m_doc) return;
	ParagraphStyle newStyle;
	newStyle.setKeepTogether (keepTogether->isChecked());
	m_doc->itemSelection_ApplyParagraphStyle(newStyle);
}

void PropertyWidget_Orphans::handleKeepWithNext()
{
	if (!m_doc) return;
	ParagraphStyle newStyle;
	newStyle.setKeepWithNext (keepWithNext->isChecked());
	m_doc->itemSelection_ApplyParagraphStyle(newStyle);
}

void PropertyWidget_Orphans::updateStyle(const ParagraphStyle& newCurrent)
{
	disconnectSignals ();
	keepLinesStart->setValue (newCurrent.keepLinesStart());
	keepLinesEnd->setValue (newCurrent.keepLinesEnd());
	keepTogether->setChecked (newCurrent.keepTogether());
	keepWithNext->setChecked (newCurrent.keepWithNext());
	connectSignals ();
}

void PropertyWidget_Orphans::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_Orphans::languageChange()
{
	keepLabelStart->setText (tr ("Don't separate first"));
	keepLabelEnd->setText (tr ("Don't separate last"));
	keepLinesStart->setSuffix (tr (" lines"));
	keepLinesEnd->setSuffix (tr (" lines"));
	keepTogether->setText (tr ("Do not split paragraph"));
	keepWithNext->setText (tr ("Keep with next paragraph"));

	keepLinesStart->setToolTip ("<qt>" + tr ("Ensure that first lines of a paragraph won't end up separated from the rest (known as widow/orphan control)") + "</qt>");
	keepLinesEnd->setToolTip ("<qt>" + tr ("Ensure that last lines of a paragraph won't end up separated from the rest (known as widow/orphan control)") + "</qt>");
	keepLabelStart->setToolTip (keepLinesStart->toolTip());
	keepLabelEnd->setToolTip (keepLinesEnd->toolTip());
	keepTogether->setToolTip ("<qt>" + tr ("If checked, ensures that the paragraph won't be split across multiple pages or columns") + "</qt>");
	keepWithNext->setToolTip ("<qt>" + tr ("If checked, automatically moves the paragraph to the next column or page if the next paragraph isn't on the same page or column") + "</qt>");
}

