/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smwidgets.h"
#include "smwidgets.moc"


/***********************************************************************/
/***********************************************************************/

SMMSpinBox::SMMSpinBox(QWidget *pa, int s)
: MSpinBox(pa, s),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0.0)
{
	
}

SMMSpinBox::SMMSpinBox(double minValue, double maxValue, QWidget *pa, int s)
: MSpinBox(minValue, maxValue, pa, s),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0.0)
{
	
}

SMMSpinBox::SMMSpinBox(QWidget *parent, const char * name)
: MSpinBox(parent, name),
  hasParent_(false),
  useParentValue_(false),
  pValue_(0.0)
{
	
}

void SMMSpinBox::setValue(double val)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	hasParent_ = false;
	pValue_ = 0.0;
	setFont(false);

	MSpinBox::setValue(val);
}

void SMMSpinBox::setValue(double val, bool isParentVal)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	hasParent_ = true;
	pValue_ = val;
	setFont(!isParentVal);

	MSpinBox::setValue(val);
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
}

void SMMSpinBox::setParentValue(double val)
{
	hasParent_ = true;
	pValue_ = val;
}

bool SMMSpinBox::useParentValue()
{
	bool ret = useParentValue_;
	useParentValue_ = false;
	return ret;
}

void SMMSpinBox::interpretText()
{
	QString t = text();
	if (hasParent_ && (t == "" || t.isEmpty() || t == QString::null))
	{
		useParentValue_ = true;
		setValue(pValue_, true);
	}
}

void SMMSpinBox::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	MSpinBox::setFont(f);
}

void SMMSpinBox::slotValueChanged()
{
	if(hasParent_)
		setFont(true);
}

/***********************************************************************/
/***********************************************************************/

