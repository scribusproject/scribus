/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QToolTip>
#include "smalignselect.h"
#include "iconmanager.h"
#include "scribusapp.h"

SMAlignSelect::SMAlignSelect(QWidget *parent)
	: AlignSelect(parent)
{
	parentButton = new QToolButton(this);
	parentButton->setText( "");
	parentButton->setToolTip( tr("Use parent style's alignment instead of overriding it"));
	addWidget( parentButton );
	resize(minimumSizeHint());
	parentButton->hide();

	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void SMAlignSelect::setStyle(int a, int d)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	m_hasParent = false;
	m_pStyle = 0;
	m_pDirection = 0;
	parentButton->hide();
	AlignSelect::setStyle(a, d);
}

void SMAlignSelect::setStyle(int a, int d, bool isParentValue)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	m_hasParent = true;
	m_pStyle = a;
	m_pDirection = d;
	if (isParentValue)
		parentButton->hide();
	else
		parentButton->show();

	AlignSelect::setStyle(a, d);
	connect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

void SMAlignSelect::setParentItem(int a, int d)
{
	m_hasParent = true;
	m_pStyle = a;
	m_pDirection = d;
}

bool SMAlignSelect::useParentValue()
{
	bool ret = m_useParentStyle;
	m_useParentStyle = false;
	if (ret)
		setStyle(m_pStyle, m_pDirection, true);

	return ret;
}

void SMAlignSelect::styleChanged()
{
	if (m_hasParent)
		parentButton->show();
}

void SMAlignSelect::pbPressed()
{
	m_useParentStyle = true;
}

void SMAlignSelect::iconSetChange()
{
	IconManager &im = IconManager::instance();
	parentButton->setIcon(im.loadIcon("reset-style-changes"));
}
