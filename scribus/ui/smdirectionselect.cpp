#include "smdirectionselect.h"
#include "iconmanager.h"
#include "scribusapp.h"

SMDirectionSelect::SMDirectionSelect(QWidget *parent)
: DirectionSelect(parent)
{
	parentButton = new QToolButton(this);
	parentButton->setText( "" );
	parentButton->setToolTip( tr("Use parent style's direction instead of overriding it"));
	addWidget( parentButton );
	resize(minimumSizeHint());
	parentButton->hide();

	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void SMDirectionSelect::setStyle(int i)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
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

void SMDirectionSelect::styleChanged()
{
	if (m_hasParent)
		parentButton->show();
}

void SMDirectionSelect::pbPressed()
{
	m_useParentStyle = true;
}

void SMDirectionSelect::iconSetChange()
{
	IconManager &im = IconManager::instance();
	parentButton->setIcon(im.loadIcon("reset-style-changes"));
}
