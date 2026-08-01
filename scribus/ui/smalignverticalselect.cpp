/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QToolTip>
#include "smalignverticalselect.h"
#include "iconmanager.h"
#include "scribusapp.h"

SMAlignVerticalSelect::SMAlignVerticalSelect(QWidget *parent)
	: AlignVerticalSelect(parent)
{
	parentButton = new QToolButton(this);
	parentButton->setText("");
	parentButton->setToolTip( tr("Use parent style's vertical alignment instead of overriding it"));
	addWidget(parentButton);
	resize(minimumSizeHint());
	parentButton->hide();

	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void SMAlignVerticalSelect::setStyle(int s)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	m_hasParent = false;
	m_pStyle = 0;
	parentButton->hide();
	AlignVerticalSelect::setStyle(s);
}

void SMAlignVerticalSelect::setStyle(int s, bool isParentValue)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	m_hasParent = true;
	m_pStyle = s;
	if (isParentValue)
		parentButton->hide();
	else
		parentButton->show();

	AlignVerticalSelect::setStyle(s);
	connect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

void SMAlignVerticalSelect::setParentItem(int s)
{
	m_hasParent = true;
	m_pStyle = s;
}

bool SMAlignVerticalSelect::useParentValue()
{
	bool ret = m_useParentStyle;
	m_useParentStyle = false;
	if (ret)
		setStyle(m_pStyle, true);

	return ret;
}

void SMAlignVerticalSelect::styleChanged()
{
	if (m_hasParent)
		parentButton->show();
}

void SMAlignVerticalSelect::pbPressed()
{
	m_useParentStyle = true;
}

void SMAlignVerticalSelect::iconSetChange()
{
	IconManager &im = IconManager::instance();
	parentButton->setIcon(im.loadIcon("reset-style-changes"));
}
