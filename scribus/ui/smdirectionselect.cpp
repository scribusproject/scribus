#include "smdirectionselect.h"

SMDirectionSelect::SMDirectionSelect(QWidget *parent)
: DirectionSelect(parent),
  m_hasParent(false),
  m_useParentStyle(false),
  m_pStyle(0)
{
	parentButton = new QToolButton(this);
	parentButton->setMaximumSize( QSize( 22, 22 ) );
	parentButton->setCheckable( true );
	parentButton->setText( tr("P", "P as in Parent"));
	parentButton->setToolTip( tr("Use parent style's direction instead of overriding it"));
	GroupSelectLayout->addWidget( parentButton );
	resize(minimumSizeHint());
	parentButton->hide();
}

void SMDirectionSelect::setStyle(int i)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	setFont(false);
	m_hasParent = false;
	m_pStyle = 0;
	parentButton->hide();
	DirectionSelect::setStyle(i);
}

void SMDirectionSelect::setStyle(int i, bool isParentValue)
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

	DirectionSelect::setStyle(i);
	connect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

void SMDirectionSelect::setParentItem(int i)
{
	m_hasParent = true;
	m_pStyle = i;
}

bool SMDirectionSelect::useParentValue()
{
	bool ret = m_useParentStyle;
	m_useParentStyle = false;
	if (ret)
		setStyle(m_pStyle, true);

	return ret;
}

void SMDirectionSelect::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	parentButton->setFont(f);
}

void SMDirectionSelect::styleChanged()
{
	if (m_hasParent)
	{
		setFont(true);
		parentButton->show();
	}
}

void SMDirectionSelect::pbPressed()
{
	m_useParentStyle = true;
}
