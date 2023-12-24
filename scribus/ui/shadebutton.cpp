/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <array>

#include "shadebutton.h"
#include "query.h"

ShadeButton::ShadeButton(QWidget* parent) : QToolButton(parent)
{
	std::array<QString, 11> tmp = {"0 %", "10 %", "20 %", "30 %", "40 %", "50 %", "60 %", "70 %", "80 %", "90 %", "100 %"};
	fillShadeMenu = new QMenu();
	fillShadeMenu->addAction( tr("Other..."))->setCheckable(true);
	for (size_t i = 0; i < tmp.size(); ++i)
		fillShadeMenu->addAction(tmp[i])->setCheckable(true);
	setMenu(fillShadeMenu);
	setPopupMode(QToolButton::InstantPopup);
	setText("100 %");
	fillShadeMenu->actions().at(11)->setChecked(true);
	connect( fillShadeMenu, SIGNAL(triggered(QAction*)), this, SLOT(setShade(QAction*)));
}

void ShadeButton::setShade(QAction *act)
{
	for (int i = 0; i < fillShadeMenu->actions().count(); ++i)
		fillShadeMenu->actions().at(i)->setChecked(false);
	act->setChecked(true);
	QList<QAction*> actList = fillShadeMenu->actions();
	int b = 100;
	int c = actList.indexOf(act);
	if (c < 0)
		return;
	if (c > 0)
		b = (c-1) * 10;
	if (b > 100)
		return; // no need for > 100%, fix needed by SM, Riku
	if (c == 0)
	{
		Query dia(this, "New", true, tr("&Shade:"), tr("Shade"));
		if (dia.exec())
		{
			bool ok = false;
			c = dia.getEditText().toInt(&ok);
			if (ok)
				b = qMax(qMin(c, 100),0);
			else
				b = 100;
		}
		else
			return;
	}
	setText(QString::number(b)+" %");
	emit clicked();
}

int ShadeButton::getValue() const
{
	int l = text().length();
	QString tx(text().remove(l-2, 2));
	return tx.toInt();
}

void ShadeButton::setValue(int val)
{
	QList<QAction*> fillActions = fillShadeMenu->actions();
	for (int i = 0; i < fillActions.count(); ++i)
		fillActions[i]->setChecked(false);
	if ((val % 10) == 0)
		fillActions[val / 10 + 1]->setChecked(true);
	else
		fillActions[0]->setChecked(true);
	setText(QString::number(val)+" %");
}


