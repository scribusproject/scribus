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
#include <qapplication.h>
#include "fparser.h"

MSpinBox::MSpinBox(QWidget *pa, int s):QSpinBox(pa)
{
	switch (s)
		{
		case 0:
			Decimals = 1;
			Width = 0;
			break;
		case 1:
			Decimals = 10;
			Width = 1;
			break;
		case 2:
			Decimals = 100;
			Width = 2;
			break;
		case 3:
			Decimals = 1000;
			Width = 3;
			break;
		case 4:
			Decimals = 10000;
			Width = 3;
			break;
		default:
			Decimals = 100;
			Width = 2;
			break;
		}
	setValidator(0);
	ed = editor();
}

bool MSpinBox::eventFilter( QObject* ob, QEvent* ev )
{
	bool retval = FALSE;
	if ( ev->type() == QEvent::KeyPress )
		{
		QKeyEvent* k = (QKeyEvent*)ev;
		if (k->key() == Key_Shift)
			{
			setLineStep(QMAX(Decimals / 10, 1));
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
//	double dez = Width == 1 ? 10.0 : 100.0;
	return QString::number(static_cast<double>(value) / Decimals, 'f', Width);
}

int MSpinBox::mapTextToValue(bool *)
{
//	double dez = Width == 1 ? 10.0 : 100.0;
  FunctionParser fp;
	QString ts = text();
	QString su = suffix();
	ts.replace(",", ".");
	if (su == tr( " pt" ))
		{
		ts.replace("pt", "");
		ts.replace("mm", "/25.4*72");
		ts.replace("in", "*72");
		ts.replace("p", "*12");
		}
	else if (su == tr( " mm" ))
		{
		ts.replace("pt", "/72*25.4");
		ts.replace("mm", "");
		ts.replace("in", "*25.4");
		ts.replace("p", "/12*25.4");
		}
	else if (su == tr( " in" ))
		{
		ts.replace("pt", "/72");
		ts.replace("mm", "/25.4");
		ts.replace("in", "");
		ts.replace("p", "/6");
		}
	else if (su == tr( " p" ))
		{
		ts.replace("pt", "/12");
		ts.replace("mm", "/25.4*6");
		ts.replace("in", "*6");
		ts.replace("p", "");
		}
	else if (su != "")
		{
		ts.replace(su, " ");
		}
  int ret = fp.Parse(ts.latin1(), "", true);
	if (ret >= 0)
		return 0;
  double erg = fp.Eval(NULL);
	return qRound(erg*Decimals);
}

void MSpinBox::setDecimals(int deci)
{
	Decimals = deci;
	setLineStep(Decimals);
	if (deci < 10)
		Width = 0;
	if (deci > 9 && deci < 100)
		Width = 1;
	if (deci > 99 && deci < 1000)
		Width = 2;
	if (deci > 999 && deci < 10000)
		Width = 3;
	if (deci > 9999)
		Width = 4;
}
