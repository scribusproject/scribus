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
#include "mspinbox.moc"
#include <qapplication.h>
#include <qlineedit.h>
#include "fparser.h"

/*!
 \fn MSpinBox(QWidget *pa, int s)
 \author Franz Schmid
 \date
 \brief Constructor
 \param pa Parent Widget
 \param s Number of Decimals
 \retval None
 */

MSpinBox::MSpinBox(QWidget *pa, int s):QSpinBox(pa)
{
	setParameters(s);
	setValidator(0);
	ed = editor();
	QSpinBox::setLineStep(Decimals);
	oldLineStep=0;
	readOnly=false;
}
/*!
 \fn MSpinBox(double minValue, double maxValue, QWidget *pa, int s)
 \author Franz Schmid
 \date
 \brief Constructor
 \param pa Parent Widget
 \param s Number of Decimals
 \retval None
 */

MSpinBox::MSpinBox(double minValue, double maxValue, QWidget *pa, int s):QSpinBox(pa)
{
	setParameters(s);
	setValidator(0);
	ed = editor();
	QSpinBox::setLineStep(Decimals);
	oldLineStep=0;
	setMinValue(minValue);
	setMaxValue(maxValue);
	readOnly=false;
}

void MSpinBox::setParameters( int s )
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
			Width = 4;
			break;
		default:
			Decimals = 100;
			Width = 2;
			break;
	}
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
	return QString::number(static_cast<double>(value) / Decimals, 'f', Width);
}

/*!
 \fn MSpinBox::mapTextToValue(bool *)
 \author Franz Schmid
 \date
 \brief Maps the Text fo the Spinbox to the Value, does Unit Conversion and Calculations
 \param None
 \retval The Value
 */

int MSpinBox::mapTextToValue(bool *)
{
	FunctionParser fp;
	QString ts = text();
	QString su = suffix().stripWhiteSpace();
	ts.replace(",", ".");
	ts.replace("%", "");
	if ((su == tr( " pt" )) || (su == tr( "pt" )))
	{
		ts.replace(tr( "pt"), "");
		ts.replace(tr( "mm"), "/25.4*72");
		ts.replace(tr( "in"), "*72");
		ts.replace(tr( "p"), "*12");
	}
	else 
		if ((su == tr( " mm" )) || (su == tr( "mm" )))
		{
			ts.replace(tr( "pt"), "/72*25.4");
			ts.replace(tr( "mm"), "");
			ts.replace(tr( "in"), "*25.4");
			ts.replace(tr( "p"), "/12*25.4");
		}
		else 
			if ((su == tr( " in" )) || (su == tr( "in" )))
			{
				ts.replace(tr( "pt"), "/72");
				ts.replace(tr( "mm"), "/25.4");
				ts.replace(tr( "in"), "");
				ts.replace(tr( "p"), "/6");
			}
			else 
				if ((su == tr( " p" )) || (su == tr( "p" )))
				{
					ts.replace(tr( "pt"), "/12");
					ts.replace(tr( "mm"), "/25.4*6");
					ts.replace(tr( "in"), "*6");
					ts.replace(tr( "p"), "");
				}
	if (su != "")
		ts.replace(su, " ");
	int ret = fp.Parse(ts.latin1(), "", true);
	if (ret >= 0)
		return 0;
	double erg = fp.Eval(NULL);
	return qRound(erg*Decimals);
}

void MSpinBox::setValues(double min, double max, int deci, double val)
{
	setDecimals(deci);
	setMinValue(min);
	setMaxValue(max);
	setValue(val);
}

void MSpinBox::getValues(double *min, double *max, int *deci, double *val)
{
	*deci = Decimals;
	*min = static_cast<double>(QSpinBox::minValue()) / Decimals;
	*max = static_cast<double>(QSpinBox::maxValue()) / Decimals;
	*val = static_cast<double>(QSpinBox::value()) / Decimals;
}

void MSpinBox::setDecimals(int deci)
{
	Decimals = deci;
	QSpinBox::setLineStep(Decimals);
	if (deci < 10)
		Width = 0;
	if ((deci > 9) && (deci < 100))
		Width = 1;
	if ((deci > 99) && (deci < 1000))
		Width = 2;
	if ((deci > 999) && (deci < 10000))
		Width = 3;
	if (deci > 9999)
		Width = 4;
}

/*!
 \fn MSpinBox::setMaxValue()
 \author Franz Schmid
 \date
 \brief Sets the Maximum Value of the Spinbox
 \param val new Value
 \retval None
 */

void MSpinBox::setMaxValue(double val)
{
	QSpinBox::setMaxValue(qRound(val*Decimals));
}

/*!
 \fn MSpinBox::setMinValue()
 \author Franz Schmid
 \date
 \brief Sets the Minimum Value of the Spinbox
 \param val new Value
 \retval None
 */

void MSpinBox::setMinValue(double val)
{
	QSpinBox::setMinValue(qRound(val*Decimals));
}

/*!
 \fn MSpinBox::setValue()
 \author Franz Schmid
 \date
 \brief Sets the Value of the Spinbox
 \param val new Value
 \retval None
 */

void MSpinBox::setValue(double val)
{
	QSpinBox::setValue(qRound(val*Decimals));
}

/*!
 \fn MSpinBox::value()
 \author Franz Schmid
 \date
 \brief Returns the current Value of the SpinBox
 \param None
 \retval The Value
 */

double MSpinBox::value()
{
	return static_cast<double>(QSpinBox::value()) / Decimals;
}

/*!
 \fn MSpinBox::minValue()
 \author Franz Schmid
 \date
 \brief Gets the Minimum Value of the Spinbox
 \param None
 \retval The Value
 */

double MSpinBox::minValue()
{
	return static_cast<double>(QSpinBox::minValue()) / Decimals;
}

/*!
 \fn MSpinBox::maxValue()
 \author Franz Schmid
 \date
 \brief Gets the Maximum Value of the Spinbox
 \param None
 \retval The Value
 */

double MSpinBox::maxValue()
{
	return static_cast<double>(QSpinBox::maxValue()) / Decimals;
}

void MSpinBox::setReadOnly( bool ro )
{
	if (readOnly!=ro) 
	{
		if (!readOnly && ro) {
			oldLineStep=QSpinBox::lineStep();
			QSpinBox::setLineStep( 0 );
		}
		else if (readOnly && !ro) {
			QSpinBox::setLineStep( oldLineStep );
			oldLineStep=0;
		}
		ed->setReadOnly( ro );
		readOnly=ro;
	}
}

bool MSpinBox::isReadOnly() const
{
	return readOnly;
}

