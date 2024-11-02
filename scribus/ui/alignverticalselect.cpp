/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "alignverticalselect.h"

#include <QEvent>
#include <QPixmap>
#include <QToolTip>

#include "iconmanager.h"
#include "scribusapp.h"

AlignVerticalSelect::AlignVerticalSelect(QWidget* parent) : FormWidget(parent)
{
	buttonGroup = new QButtonGroup(this);
	buttonGroup->setExclusive(true);

	TextT = new QToolButton( this );
	TextT->setCheckable( true );
	TextT->setChecked( true );
	buttonGroup->addButton(TextT, 0);

	TextC = new QToolButton( this );
	TextC->setCheckable( true );
	buttonGroup->addButton(TextC, 1);

	TextB = new QToolButton( this );
	TextB->setCheckable( true );
	buttonGroup->addButton(TextB, 2);

	addWidget(TextT);
	addWidget(TextC);
	addWidget(TextB);

	iconSetChange();
	languageChange();

	resize(minimumSizeHint());

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(buttonGroup, SIGNAL(idClicked(int)), this, SLOT(setTypeStyle(int)));
}

void AlignVerticalSelect::setStyle(int s)
{
	if ((s >= 0) && (s < 3))
		buttonGroup->button(s)->setChecked(true);
}

int AlignVerticalSelect::getStyle() const
{
	return buttonGroup->checkedId();
}

void AlignVerticalSelect::setTypeStyle(int a)
{
	emit State(a);
}

void AlignVerticalSelect::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void AlignVerticalSelect::iconSetChange()
{
	IconManager& im = IconManager::instance();

	TextT->setIcon(im.loadIcon("paragraph-align-top"));
	TextC->setIcon(im.loadIcon("paragraph-align-vertical-center"));
	TextB->setIcon(im.loadIcon("paragraph-align-bottom"));
}

void AlignVerticalSelect::languageChange()
{
	TextT->setToolTip( tr("Align Text Top"));
	TextC->setToolTip( tr("Align Text Vertical Centered"));
	TextB->setToolTip( tr("Align Text Bottom"));
	setText( tr("&Vertical"));
}
