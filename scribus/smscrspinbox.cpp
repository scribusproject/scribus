/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "smscrspinbox.h"


SMScrSpinBox::SMScrSpinBox(QWidget *pa, int s)
: ScrSpinBox(pa, s),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0.0)
{
	
}

SMScrSpinBox::SMScrSpinBox(double minValue, double maxValue, QWidget *pa, int s)
: ScrSpinBox(minValue, maxValue, pa, s),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0.0)
{
	
}

SMScrSpinBox::SMScrSpinBox(QWidget *parent, const char * name)
: ScrSpinBox(parent, 0),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0.0)
{
	
}

void SMScrSpinBox::setValue(double val)
{
	disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(slotValueChanged()));
	hasParent_ = false;
	pValue_ = 0.0;
	setFont(false);

	ScrSpinBox::setValue(val);
}

void SMScrSpinBox::setValue(double val, bool isParentVal)
{
	disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(slotValueChanged()));
	hasParent_ = true;
	pValue_ = val;
	setFont(!isParentVal);

	ScrSpinBox::setValue(val);
	connect(this, SIGNAL(valueChanged(double)), this, SLOT(slotValueChanged()));
}

void SMScrSpinBox::clear()
{
	disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(slotValueChanged()));
	ScrSpinBox::clear();
	connect(this, SIGNAL(valueChanged(double)), this, SLOT(slotValueChanged()));
}

void SMScrSpinBox::setParentValue(double val)
{
	hasParent_ = true;
	pValue_ = val;
}

bool SMScrSpinBox::useParentValue()
{
	bool ret = useParentValue_;
	useParentValue_ = false;
	return ret;
}

void SMScrSpinBox::interpretText()
{
// 	QString t = text();
// 	if (hasParent_ && (t == "" || t.isEmpty() || t == QString::null))
	if (hasParent_ && text().isEmpty())
	{
		useParentValue_ = true;
		setValue(pValue_, true);
	}
	ScrSpinBox::interpretText();
}

void SMScrSpinBox::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	ScrSpinBox::setFont(f);
}

void SMScrSpinBox::slotValueChanged()
{
	if(hasParent_)
		setFont(true);
}
