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
  hasParent_(false),
  useParentStyle_(false),
  pStyle_(0)
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
	hasParent_ = false;
	pStyle_ = 0;
	parentButton->hide();
	AlignSelect::setStyle(i);
}

void SMAlignSelect::setStyle(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	hasParent_ = true;
	pStyle_ = i;
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
	hasParent_ = true;
	pStyle_ = i;
}

bool SMAlignSelect::useParentValue()
{
	bool ret = useParentStyle_;
	useParentStyle_ = false;
	if (ret)
		setStyle(pStyle_, true);

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
	if (hasParent_)
	{
		setFont(true);
		parentButton->show();
	}
}

void SMAlignSelect::pbPressed()
{
	useParentStyle_ = true;
}
