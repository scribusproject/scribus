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

AlignSelect::AlignSelect(QWidget* parent) : QWidget(parent)
{
	IconManager& im = IconManager::instance();
	buttonGroup = new QButtonGroup(this);

	GroupAlignLayout = new QHBoxLayout( this );
	GroupAlignLayout->setSpacing(3);
	GroupAlignLayout->setContentsMargins(0, 0, 0, 0);

	TextL = new QToolButton( this );
	TextL->setIcon(im.loadIcon("16/format-justify-left.png"));
	TextL->setCheckable( true );
	TextL->setChecked( true );
	GroupAlignLayout->addWidget( TextL );
	buttonGroup->addButton(TextL, 0);

	TextC = new QToolButton( this );
	TextC->setIcon(im.loadIcon("16/format-justify-center.png"));
	TextC->setCheckable( true );
	GroupAlignLayout->addWidget( TextC );
	buttonGroup->addButton(TextC, 1);

	TextR = new QToolButton( this );
	TextR->setIcon(im.loadIcon("16/format-justify-right.png"));
	TextR->setCheckable( true );
	GroupAlignLayout->addWidget( TextR );
	buttonGroup->addButton(TextR, 2);

	TextB = new QToolButton( this );
	TextB->setIcon(im.loadIcon("16/format-justify-fill-block.png"));
	TextB->setCheckable( true );
	GroupAlignLayout->addWidget( TextB );
	buttonGroup->addButton(TextB, 3);

	TextF = new QToolButton( this );
	TextF->setIcon(im.loadIcon("16/format-justify-fill.png"));
	TextF->setCheckable( true );
	GroupAlignLayout->addWidget( TextF );
	buttonGroup->addButton(TextF, 4);
	resize(minimumSizeHint());

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(setTypeStyle(int)));
}

void AlignSelect::setStyle(int s, int d)
{
	if ((s >= 0) && (s < 5))
		buttonGroup->button(s)->setChecked(true);

	if (d == ParagraphStyle::RTL)
		TextB->setIcon(IconManager::instance().loadIcon("16/format-justify-fill-block-right.png"));
	else
		TextB->setIcon(IconManager::instance().loadIcon("16/format-justify-fill-block.png"));
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

	TextL->setIcon(im.loadIcon("16/format-justify-left.png"));
	TextC->setIcon(im.loadIcon("16/format-justify-center.png"));
	TextR->setIcon(im.loadIcon("16/format-justify-right.png"));
	TextB->setIcon(im.loadIcon("16/format-justify-fill-block.png"));
	TextF->setIcon(im.loadIcon("16/format-justify-fill.png"));
}

void AlignSelect::languageChange()
{
	TextL->setToolTip( tr("Align Text Left"));
	TextR->setToolTip( tr("Align Text Right"));
	TextC->setToolTip( tr("Align Text Center"));
	TextB->setToolTip( tr("Align Text Justified"));
	TextF->setToolTip( tr("Align Text Forced Justified"));
}
