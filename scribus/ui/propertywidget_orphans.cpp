/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_orphans.h"

#include "iconmanager.h"
#include "scribusapp.h"
#include "scribusdoc.h"

PropertyWidget_Orphans::PropertyWidget_Orphans(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	layout()->setAlignment(Qt::AlignTop);
	keepLinesStart->setDecimals(0);
	keepLinesEnd->setDecimals(0);

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void PropertyWidget_Orphans::connectSignals()
{
	connect(keepLinesStart, SIGNAL(valueChanged(double)), this, SLOT(handleKeepLinesStart()));
	connect(keepLinesEnd, SIGNAL(valueChanged(double)), this, SLOT(handleKeepLinesEnd()));
	connect(keepTogether, SIGNAL(toggled(bool)), this, SLOT(handleKeepTogether()));
	connect(keepWithNext, SIGNAL(toggled(bool)), this, SLOT(handleKeepWithNext()));
}

void PropertyWidget_Orphans::disconnectSignals()
{
	disconnect(keepLinesStart, SIGNAL(valueChanged(double)), this, SLOT(handleKeepLinesStart()));
	disconnect(keepLinesEnd, SIGNAL(valueChanged(double)), this, SLOT(handleKeepLinesEnd()));
	disconnect(keepTogether, SIGNAL(toggled(bool)), this, SLOT(handleKeepTogether()));
	disconnect(keepWithNext, SIGNAL(toggled(bool)), this, SLOT(handleKeepWithNext()));
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

void PropertyWidget_Orphans::iconSetChange()
{
	IconManager &im = IconManager::instance();

	keepLabelStart->setPixmap(im.loadPixmap("paragraph-orphan"));
	keepLabelEnd->setPixmap(im.loadPixmap("paragraph-widow"));
}

void PropertyWidget_Orphans::languageChange()
{
	retranslateUi(this);
}

