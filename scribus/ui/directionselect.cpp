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

DirectionSelect::DirectionSelect(QWidget* parent) : QWidget(parent)
{
        selected = 0;

        IconManager* im=IconManager::instance();
        buttonGroup = new QButtonGroup(this);

        GroupSelectLayout = new QHBoxLayout( this );
        GroupSelectLayout->setSpacing( 0 );
        GroupSelectLayout->setMargin( 5 );
        GroupSelectLayout->setAlignment(Qt::AlignRight);

        LTR = new QToolButton( this );
        LTR->setMaximumSize( QSize( 22, 22 ) );
        LTR->setIcon(im->loadIcon("16/text-direction-ltr.png"));
        LTR->setCheckable( true );
        LTR->setChecked( true );
        GroupSelectLayout->addWidget( LTR );
        buttonGroup->addButton(LTR, 0);

        RTL = new QToolButton( this );
        RTL->setMaximumSize( QSize( 22, 22 ) );
        RTL->setIcon(im->loadIcon("16/text-direction-rtl.png"));
        RTL->setCheckable( true );
        GroupSelectLayout->addWidget( RTL );
        buttonGroup->addButton(RTL, 1);

        resize(minimumSizeHint());
        connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(setTypeStyle(int)));
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
        selected = a;
        emit State(a);
}

int DirectionSelect::selectedId()
{
        return selected;
}

void DirectionSelect::changeEvent(QEvent *e)
{
        if (e->type() == QEvent::LanguageChange)
        {
                languageChange();
        }
}

void DirectionSelect::languageChange()
{
        LTR->setToolTip("");
        RTL->setToolTip("");


        LTR->setToolTip( tr("Left to right paragraph"));
        RTL->setToolTip( tr("Right to left paragraph"));

}

