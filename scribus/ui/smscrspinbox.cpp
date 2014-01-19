/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "smscrspinbox.h"


SMScrSpinBox::SMScrSpinBox(QWidget *pa, int s)
: ScrSpinBox(pa, s),
  m_hasParent(false),
  m_useParentValue(false),
  m_pValue(0.0)
{
	
}

SMScrSpinBox::SMScrSpinBox(double minValue, double maxValue, QWidget *pa, int s)
: ScrSpinBox(minValue, maxValue, pa, s),
  m_hasParent(false),
  m_useParentValue(false),
  m_pValue(0.0)
{
	
}

SMScrSpinBox::SMScrSpinBox(QWidget *parent, const char * name)
: ScrSpinBox(parent, 0),
  m_hasParent(false),
  m_useParentValue(false),
  m_pValue(0.0)
{
	
}

void SMScrSpinBox::setValue(double val)
{
	disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(slotValueChanged()));
	m_hasParent = false;
	m_pValue = 0.0;
	setFont(false);

	ScrSpinBox::setValue(val);
}

void SMScrSpinBox::setValue(double val, bool isParentVal)
{
	disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(slotValueChanged()));
	m_hasParent = true;
	m_pValue = val;
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
	m_hasParent = true;
	m_pValue = val;
}

bool SMScrSpinBox::useParentValue()
{
	bool ret = m_useParentValue;
	m_useParentValue = false;
	return ret;
}

void SMScrSpinBox::interpretText()
{
// 	QString t = text();
// 	if (m_hasParent && (t == "" || t.isEmpty() || t == QString::null))
	if (m_hasParent && text().isEmpty())
	{
		m_useParentValue = true;
		setValue(m_pValue, true);
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
	if(m_hasParent)
		setFont(true);
}
