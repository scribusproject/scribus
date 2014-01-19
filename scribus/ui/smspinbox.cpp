/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smspinbox.h"

SMSpinBox::SMSpinBox(QWidget *parent)
: QSpinBox(parent),
  m_hasParent(false),
  m_useParentValue(false),
  m_pValue(0)
{

}

void SMSpinBox::setValue(int val)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	m_hasParent = false;
	m_pValue = 0;
	setFont(false);

	QSpinBox::setValue(val);
}

void SMSpinBox::setValue(int val, bool isParentVal)
{
	disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged()));
	m_hasParent = true;
	m_pValue = val;
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
	m_hasParent = true;
	m_pValue = val;
}

bool SMSpinBox::useParentValue()
{
	bool ret = m_useParentValue;
	m_useParentValue = false;
	return ret;
}

void SMSpinBox::interpretText()
{
// 	QString t = text();
// 	if (m_hasParent && (t == "" || t.isEmpty() || t == QString::null))
	if (m_hasParent && text().isEmpty())
	{
		m_useParentValue = true;
		setValue(m_pValue, true);
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
	if(m_hasParent)
		setFont(true);
}
