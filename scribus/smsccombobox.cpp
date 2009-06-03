/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smsccombobox.h"


SMScComboBox::SMScComboBox(QWidget *parent)
: ScComboBox(parent),
  hasParent_(false),
  useParentValue_(false),
  pItem_(0)
{
	
}

void SMScComboBox::setCurrentItem(int i)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pItem_ = 0;
	ScComboBox::setCurrentIndex(i);
}

void SMScComboBox::setCurrentItem(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	hasParent_ = true;
	pItem_ = i;
	setFont(!isParentValue);
	if (!isParentValue)
	{
		useParentValue_ = true;
		addItem( tr("Use Parent Value"));
	}

	ScComboBox::setCurrentIndex(i);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMScComboBox::setCurrentItemByData(int i)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pItem_ = 0;
	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toInt() == i)
			ScComboBox::setCurrentIndex(idx);
	}
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMScComboBox::setCurrentItemByData(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	hasParent_ = true;
	setFont(!isParentValue);
	if (!isParentValue)
	{
		useParentValue_ = true;
		addItem( tr("Use Parent Value"));
	}

	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toInt() == i)
		{
			ScComboBox::setCurrentIndex(idx);
			pItem_ = idx;
		}
	}
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMScComboBox::setCurrentItemByData(double d)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	hasParent_ = false;
	pItem_ = 0;
	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toDouble() == d)
			ScComboBox::setCurrentIndex(idx);
	}
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMScComboBox::setCurrentItemByData(double d, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	hasParent_ = true;
	setFont(!isParentValue);
	if (!isParentValue)
	{
		useParentValue_ = true;
		addItem( tr("Use Parent Value"));
	}

	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toDouble() == d)
		{
			ScComboBox::setCurrentIndex(idx);
			pItem_ = idx;
		}
	}
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

int SMScComboBox::getItemIndexForData(int i)
{
	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toInt() == i)
		{
			return idx;
		}
	}
	return 0;
}

int SMScComboBox::getItemIndexForData(double d)
{
	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toDouble() == d)
		{
			return idx;
		}
	}
	return 0;
}



void SMScComboBox::setParentItem(int i)
{
	hasParent_ = true;
	pItem_ = i;
}

bool SMScComboBox::useParentValue()
{
	bool ret = false;

	if (useParentValue_ && hasParent_)
	{
		ret = currentIndex() == (count() - 1);
		if (ret)
		{
			removeItem(count() - 1);
			setFont(false);
			setCurrentItem(pItem_, true);
			useParentValue_ = false;
		}
	}

	return ret;
}

void SMScComboBox::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	ScComboBox::setFont(f);
}

void SMScComboBox::currentChanged()
{
	if (hasParent_ && !useParentValue_)
	{
		setFont(true);
		addItem( tr("Use Parent Value"));
		useParentValue_ = true;
	}
}



