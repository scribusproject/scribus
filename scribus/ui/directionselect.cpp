/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "directionselect.h"
#include <QEvent>
#include <QPixmap>
#include <QToolTip>

#include "iconmanager.h"
#include "scribusapp.h"

DirectionSelect::DirectionSelect(QWidget* parent) : FormWidget(parent)
{
	buttonGroup = new QButtonGroup(this);

	LTR = new QToolButton( this );
	LTR->setCheckable( true );
	LTR->setChecked( true );
	addWidget( LTR );
	buttonGroup->addButton(LTR, 0);

	RTL = new QToolButton( this );
	RTL->setCheckable( true );
	addWidget( RTL );
	buttonGroup->addButton(RTL, 1);

	resize(minimumSizeHint());

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(buttonGroup, SIGNAL(idClicked(int)), this, SLOT(setTypeStyle(int)));
}

void DirectionSelect::setStyle(int s)
{
	if ((s >= 0) && (s < 2))
		buttonGroup->button(s)->setChecked(true);
}

int DirectionSelect::getStyle()
{
	int ret = 0;
	if (LTR->isChecked())
		ret = 0;
	if (RTL->isChecked())
		ret = 1;
	return ret;
}

void DirectionSelect::setTypeStyle(int a)
{
	m_selected = a;
	emit State(a);
}

int DirectionSelect::selectedId()
{
	return m_selected;
}

void DirectionSelect::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void DirectionSelect::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();
	LTR->setIcon(iconManager.loadIcon("16/text-direction-ltr.png"));
	RTL->setIcon(iconManager.loadIcon("16/text-direction-rtl.png"));
}

void DirectionSelect::languageChange()
{
	LTR->setToolTip( tr("Left to right paragraph"));
	RTL->setToolTip( tr("Right to left paragraph"));
	setText( tr("&Direction"));
}

