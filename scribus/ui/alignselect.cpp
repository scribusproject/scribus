/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "alignselect.h"

#include <QEvent>
#include <QPixmap>
#include <QToolTip>

#include "iconmanager.h"
#include "scribusapp.h"

AlignSelect::AlignSelect(QWidget* parent) : FormWidget(parent)
{
	buttonGroup = new QButtonGroup(this);

	TextL = new QToolButton( this );
	TextL->setCheckable( true );
	TextL->setChecked( true );
	buttonGroup->addButton(TextL, 0);

	TextC = new QToolButton( this );
	TextC->setCheckable( true );
	buttonGroup->addButton(TextC, 1);

	TextR = new QToolButton( this );
	TextR->setCheckable( true );
	buttonGroup->addButton(TextR, 2);

	TextB = new QToolButton( this );
	TextB->setCheckable( true );
	buttonGroup->addButton(TextB, 3);

	TextF = new QToolButton( this );
	TextF->setCheckable( true );
	buttonGroup->addButton(TextF, 4);

	addWidget(TextL);
	addWidget(TextC);
	addWidget(TextR);
	addWidget(TextB);
	addWidget(TextF);

	iconSetChange();
	languageChange();

	resize(minimumSizeHint());

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(buttonGroup, SIGNAL(idClicked(int)), this, SLOT(setTypeStyle(int)));
}

void AlignSelect::setStyle(int s, int d)
{
	if ((s >= 0) && (s < 5))
		buttonGroup->button(s)->setChecked(true);

	if (d == ParagraphStyle::RTL)
		TextB->setIcon(IconManager::instance().loadIcon("format-justify-fill-block-right"));
	else
		TextB->setIcon(IconManager::instance().loadIcon("format-justify-fill-block"));
}

int AlignSelect::getStyle() const
{
	int ret = 0;
	if (TextL->isChecked())
		ret = 0;
	if (TextR->isChecked())
		ret = 2;
	if (TextC->isChecked())
		ret = 1;
	if (TextB->isChecked())
		ret = 3;
	if (TextF->isChecked())
		ret = 4;
	return ret;
}

void AlignSelect::setTypeStyle(int a)
{
	selected = a;
	emit State(a);
}

int AlignSelect::selectedId() const
{
	return selected;
}

void AlignSelect::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void AlignSelect::iconSetChange()
{
	IconManager& im = IconManager::instance();

	TextL->setIcon(im.loadIcon("format-justify-left"));
	TextC->setIcon(im.loadIcon("format-justify-center"));
	TextR->setIcon(im.loadIcon("format-justify-right"));
	TextB->setIcon(im.loadIcon("format-justify-fill-block"));
	TextF->setIcon(im.loadIcon("format-justify-fill"));
}

void AlignSelect::languageChange()
{
	TextL->setToolTip( tr("Align Text Left"));
	TextR->setToolTip( tr("Align Text Right"));
	TextC->setToolTip( tr("Align Text Center"));
	TextB->setToolTip( tr("Align Text Justified"));
	TextF->setToolTip( tr("Align Text Forced Justified"));
	setText( tr("&Horizontal"));
}
