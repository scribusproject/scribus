/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smstyleselect.h"


SMStyleSelect::SMStyleSelect(QWidget *parent)
: StyleSelect(parent),
  hasParent_(false),
  useParentStyle_(false),
  pStyle_(0)
{
	parentButton = new QToolButton(this);
	parentButton->setMaximumSize(QSize(22, 22));
	parentButton->setMinimumSize(QSize(22, 22));
	parentButton->setText( tr("P", "P as in Parent"));
	parentButton->setToolTip( tr("Use parent style's effects instead of overriding them"));
	ssLayout->addWidget(parentButton);
	resize(minimumSizeHint());
	parentButton->hide();
}

void SMStyleSelect::setStyle(int i)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(ShadowVal->Xoffset, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(ShadowVal->Yoffset, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(OutlineVal->LWidth, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(UnderlineVal->LPos, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(UnderlineVal->LWidth, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(styleChanged()));
	disconnect(StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	setFont(false);
	hasParent_ = false;
	pStyle_ = 0;
	parentButton->hide();
	StyleSelect::setStyle(i);
}

void SMStyleSelect::setStyle(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	disconnect(ShadowVal->Xoffset, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(ShadowVal->Yoffset, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(OutlineVal->LWidth, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(UnderlineVal->LPos, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(UnderlineVal->LWidth, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	disconnect(StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(styleChanged()));
	disconnect(StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(styleChanged()));
	disconnect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
	hasParent_ = true;
	pStyle_ = i;
	setFont(!isParentValue);
	if (isParentValue)
		parentButton->hide();
	else
		parentButton->show();
	parentButton->setChecked(true);
	StyleSelect::setStyle(i);
	connect(this, SIGNAL(State(int)), this, SLOT(styleChanged()));
	connect(ShadowVal->Xoffset, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	connect(ShadowVal->Yoffset, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	connect(OutlineVal->LWidth, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	connect(UnderlineVal->LPos, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	connect(UnderlineVal->LWidth, SIGNAL(valueChanged(double)),this, SLOT(styleChanged()));
	connect(StrikeVal->LPos, SIGNAL(valueChanged(double)), this, SLOT(styleChanged()));
	connect(StrikeVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(styleChanged()));
	connect(parentButton, SIGNAL(pressed()), this, SLOT(pbPressed()));
}

void SMStyleSelect::setParentItem(int i)
{
	hasParent_ = true;
	pStyle_ = i;
}

bool SMStyleSelect::useParentValue()
{
	bool ret = useParentStyle_;
	useParentStyle_ = false;
	if (ret)
		setStyle(pStyle_, true);

	return ret;
}

void SMStyleSelect::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	parentButton->setFont(f);
	ShadowVal->Xoffset->setFont(f);
	ShadowVal->Yoffset->setFont(f);
	OutlineVal->LWidth->setFont(f);
	UnderlineVal->LPos->setFont(f);
	UnderlineVal->LWidth->setFont(f);
	StrikeVal->LPos->setFont(f);
	StrikeVal->LWidth->setFont(f);
	StyleSelect::setFont(f);
}

void SMStyleSelect::styleChanged()
{
	if (hasParent_)
	{
		setFont(true);
		parentButton->show();
	}
}

void SMStyleSelect::pbPressed()
{
	useParentStyle_ = true;
	emit State(getStyle());
}
