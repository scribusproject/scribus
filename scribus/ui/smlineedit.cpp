/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "smlineedit.h"

SMLineEdit::SMLineEdit(QWidget *parent)
	: QLineEdit(parent)
{

}

void SMLineEdit::setValue(const QString& val)
{
	disconnect(this, SIGNAL(textChanged(QString)), this, SLOT(slotValueChanged()));
	m_hasParent = false;
	m_pValue.clear();
	setFont(false);

	QLineEdit::setText(val);
}

void SMLineEdit::setValue(const QString& val, bool isParentVal)
{
	disconnect(this, SIGNAL(textChanged(QString)), this, SLOT(slotValueChanged()));
	m_hasParent = true;
	m_pValue = val;
	setFont(!isParentVal);

	QLineEdit::setText(val);
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(slotValueChanged()));
}

void SMLineEdit::clear()
{
	disconnect(this, SIGNAL(textChanged(QString)), this, SLOT(slotValueChanged()));
	QLineEdit::clear();
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(slotValueChanged()));
}

void SMLineEdit::setParentValue(const QString& val)
{
	m_hasParent = true;
	m_pValue = val;
}

bool SMLineEdit::useParentValue()
{
	bool ret = m_useParentValue;
	m_useParentValue = false;
	return ret;
}

void SMLineEdit::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	QLineEdit::setFont(f);
}

void SMLineEdit::slotValueChanged()
{
	if(m_hasParent)
		setFont(true);
}
