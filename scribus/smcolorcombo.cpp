/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smcolorcombo.h"
#include "util.h"


SMColorCombo::SMColorCombo(QWidget *parent)
: ColorCombo(parent),
  hasParent_(false),
  useParentValue_(false),
  pItem_(0)
{
	
}

SMColorCombo::SMColorCombo(bool rw, QWidget* parent)
: ColorCombo(rw, parent),
  hasParent_(false),
  useParentValue_(false),
  pItem_(0),
  pText_(QString::null)
{
	
}

void SMColorCombo::setCurrentItem(int i)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pItem_ = 0;
	pText_ = QString::null;
	ColorCombo::setCurrentIndex(i);
}

void SMColorCombo::setCurrentItem(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	hasParent_ = true;
	pItem_ = i;
	pText_ = QString::null;
	ColorCombo::setCurrentIndex(i);
	setFont(!isParentValue);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMColorCombo::setCurrentText(const QString &s)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pItem_ = -1;
	pText_ = s;
	setCurrentComboItem(this, s);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMColorCombo::setCurrentText(const QString &s, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	hasParent_ = true;
	pItem_ = -1;
	pText_ = s;
	setCurrentComboItem(this, s);
	setFont(!isParentValue);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMColorCombo::setParentItem(int i)
{
	hasParent_ = true;
	pItem_ = i;
	pText_ = QString::null;
}

void SMColorCombo::setParentText(const QString &s)
{
	hasParent_ = true;
	pText_ = s;
}

bool SMColorCombo::useParentValue()
{
	bool ret = false;

	if (useParentValue_ && hasParent_)
	{
		ret = currentIndex() == (count() - 1);
		if (ret)
		{
			removeItem(count() - 1);
			setFont(false);
			if (!pText_.isNull())
				setCurrentText(pText_, true);
			else
				setCurrentItem(pItem_, true);
			useParentValue_ = false;
		}
	}

	return ret;
}

void SMColorCombo::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	ColorCombo::setFont(f);
}

void SMColorCombo::currentChanged()
{
	if (hasParent_ && !useParentValue_)
	{
		setFont(true);
		addItem( tr("Use Parent Value"));
		useParentValue_ = true;
	}
}
