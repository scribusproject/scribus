/***************************************************************************
                          mspinbox.cpp  -  description
                             -------------------
    begin                : Sat Jun 16 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "mspinbox.h"
#include "qapplication.h"

MSpinBox::MSpinBox(QWidget *pa, int s):QSpinBox(pa)
{
	switch (s)
		{
		case 1:
			Decimals = 10;
			Width = 1;
			break;
		case 2:
			Decimals = 100;
			Width = 2;
		default:
			Decimals = 100;
			Width = 2;
			break;
		}
	setValidator(0);
}

bool MSpinBox::eventFilter( QObject* ob, QEvent* ev )
{
	bool retval = FALSE;
	if ( ev->type() == QEvent::KeyPress )
		{
		QKeyEvent* k = (QKeyEvent*)ev;
		if (k->key() == Key_Shift)
			{
			setLineStep(Decimals / 10);
			retval = true;
	    qApp->sendEvent( this, ev );
			return retval;
			}
		}
	if ( ev->type() == QEvent::KeyRelease )
		{
		QKeyEvent* k = (QKeyEvent*)ev;
		if (k->key() == Key_Shift)
			{
			setLineStep(Decimals);
			retval = true;
	    qApp->sendEvent( this, ev );
			return retval;
			}
		}
	return QSpinBox::eventFilter(ob, ev);
}

QString MSpinBox::mapValueToText(int value)
{
	double dez;
	if (Width == 1)
		dez = 10.0;
	else
		dez = 100.0;
	return QString::number(static_cast<double>(value) / dez, 'f', Width);
}

int MSpinBox::mapTextToValue(bool *)
{
	float dez;
	if (Width == 1)
		dez = 10.0;
	else
		dez = 100.0;
	return int(qRound(text().toFloat()*dez));
}

void MSpinBox::setDecimals(int deci)
{
	Decimals = deci;
	setLineStep(Decimals);
	if (deci > 9)
		Width = 1;
	if (deci > 99)
		Width = 2;
}
