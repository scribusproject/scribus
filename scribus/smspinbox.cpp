/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smspinbox.h"

SMSpinBox::SMSpinBox(QWidget *parent)
: QSpinBox(parent),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0)
{

}

void SMSpinBox::setValue(int val)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	hasParent_ = false;
	pValue_ = 0;
	setFont(false);

	QSpinBox::setValue(val);
}

void SMSpinBox::setValue(int val, bool isParentVal)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	hasParent_ = true;
	pValue_ = val;
	setFont(!isParentVal);

	QSpinBox::setValue(val);
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
}

void SMSpinBox::clear()
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	QSpinBox::clear();
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
}

void SMSpinBox::setParentValue(int val)
{
	hasParent_ = true;
	pValue_ = val;
}

bool SMSpinBox::useParentValue()
{
	bool ret = useParentValue_;
	useParentValue_ = false;
	return ret;
}

void SMSpinBox::interpretText()
{
// 	QString t = text();
// 	if (hasParent_ && (t == "" || t.isEmpty() || t == QString::null))
	if (hasParent_ && text().isEmpty())
	{
		useParentValue_ = true;
		setValue(pValue_, true);
	}
	QSpinBox::interpretText();
}

void SMSpinBox::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	QSpinBox::setFont(f);
}

void SMSpinBox::slotValueChanged()
{
	if(hasParent_)
		setFont(true);
}
