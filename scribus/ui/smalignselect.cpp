/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QToolTip>
#include "smalignselect.h"


SMAlignSelect::SMAlignSelect(QWidget *parent)
: AlignSelect(parent),
  m_hasParent(false),
  m_useParentStyle(false),
  m_pStyle(0)
{
	parentButton = new QToolButton(this);
	parentButton->setMaximumSize( QSize( 22, 22 ) );
	parentButton->setCheckable( true );
	parentButton->setText( tr("P", "P as in Parent"));
	parentButton->setToolTip( tr("Use parent style's alignment instead of overriding it"));
	GroupAlignLayout->addWidget( parentButton );
	resize(minimumSizeHint());
	parentButton->hide();
}

void SMAlignSelect::setStyle(int i)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	setFont(false);
	m_hasParent = false;
	m_pStyle = 0;
	parentButton->hide();
	AlignSelect::setStyle(i);
}

void SMAlignSelect::setStyle(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	m_hasParent = true;
	m_pStyle = i;
	setFont(!isParentValue);
	if (isParentValue)
		parentButton->hide();
	else
		parentButton->show();

	AlignSelect::setStyle(i);
	connect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

void SMAlignSelect::setParentItem(int i)
{
	m_hasParent = true;
	m_pStyle = i;
}

bool SMAlignSelect::useParentValue()
{
	bool ret = m_useParentStyle;
	m_useParentStyle = false;
	if (ret)
		setStyle(m_pStyle, true);

	return ret;
}

void SMAlignSelect::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	parentButton->setFont(f);
}

void SMAlignSelect::styleChanged()
{
	if (m_hasParent)
	{
		setFont(true);
		parentButton->show();
	}
}

void SMAlignSelect::pbPressed()
{
	m_useParentStyle = true;
}
