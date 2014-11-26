/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smshadebutton.h"


SMShadeButton::SMShadeButton(QWidget *parent)
: ShadeButton(parent),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0)
{
	
}

void SMShadeButton::setValue(int i)
{
	disconnect(this, SIGNAL(pressed()), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pValue_ = 0;
	ShadeButton::setValue(i);
}

void SMShadeButton::setValue(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(pressed()), this, SLOT(currentChanged()));
	hasParent_ = true;
	pValue_ = i;
	setFont(!isParentValue);
	ShadeButton::setValue(i);
	connect(this, SIGNAL(pressed()), this, SLOT(currentChanged()));
}

void SMShadeButton::setParentValue(int i)
{
	hasParent_ = true;
	pValue_ = i;
}

bool SMShadeButton::useParentValue()
{
	bool ret = useParentValue_;
	useParentValue_ = false;

	if (ret)
	{
		setValue(pValue_, true);
		QList<QAction*> actList = FillSh->actions();
		if (actList.last()->text() == useParentValueText())
			FillSh->removeAction(actList.last());
	}

	return ret;
}

QString SMShadeButton::useParentValueText()
{
	QString trText = tr("Use Parent Value");
	return trText;
}

void SMShadeButton::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	FillSh->setFont(f);
	ShadeButton::setFont(f);
}

void SMShadeButton::currentChanged()
{
	if (hasParent_)
	{
		setFont(true);
		QString upv = useParentValueText();
		QList<QAction*> actList = FillSh->actions();
		if (actList.last()->text() != upv)
			FillSh->addAction(upv, this, SLOT(slotUseParent()));
	}
}

void SMShadeButton::slotUseParent()
{
	useParentValue_ = true;
	QList<QAction*> actList = FillSh->actions();
	if (actList.last()->text() == useParentValueText())
		FillSh->removeAction(actList.last());
	emit clicked();
}
