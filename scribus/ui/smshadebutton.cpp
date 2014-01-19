/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smshadebutton.h"


SMShadeButton::SMShadeButton(QWidget *parent)
: ShadeButton(parent),
  m_hasParent(false),
  m_useParentValue(false),
  m_pValue(0)
{
	
}

void SMShadeButton::setValue(int i)
{
	disconnect(this, SIGNAL(pressed()), this, SLOT(currentChanged()));
	setFont(false);
	m_hasParent = false;
	m_pValue = 0;
	ShadeButton::setValue(i);
}

void SMShadeButton::setValue(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(pressed()), this, SLOT(currentChanged()));
	m_hasParent = true;
	m_pValue = i;
	setFont(!isParentValue);
	ShadeButton::setValue(i);
	connect(this, SIGNAL(pressed()), this, SLOT(currentChanged()));
}

void SMShadeButton::setParentValue(int i)
{
	m_hasParent = true;
	m_pValue = i;
}

bool SMShadeButton::useParentValue()
{
	bool ret = m_useParentValue;
	m_useParentValue = false;

	if (ret)
	{
		setValue(m_pValue, true);
		QList<QAction*> actList = FillSh->actions();
		FillSh->removeAction(actList.last());
	}
	

	return ret;
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
	if (m_hasParent)
	{
		setFont(true);
		QString upv = tr("Use Parent Value");
		QList<QAction*> actList = FillSh->actions();
		if (actList.last()->text() != upv)
			FillSh->addAction(upv, this, SLOT(slotUseParent()));
	}
}

void SMShadeButton::slotUseParent()
{
	m_useParentValue = true;
	QList<QAction*> actList = FillSh->actions();
	FillSh->removeAction(actList.last());
	emit clicked();
}
