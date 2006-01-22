/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include <qregexp.h>
#include <cmath>
#include "fparser.h"
#include "units.h"

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
	edited = false;
    connect( ed, SIGNAL(textChanged(const QString&)), SLOT(textChanged()) );
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
	edited = false;
    connect( ed, SIGNAL(textChanged(const QString&)), SLOT(textChanged()) );
}

void MSpinBox::setParameters( int s )
{
	if (s>=0 && s <=unitGetMaxIndex())
	{
		Width=s;
		Decimals=static_cast<int>(pow(10, s));
	}
	else
	{
		Width = 2;
		Decimals = 100;
	}
	edited = false;
}

bool MSpinBox::eventFilter( QObject* ob, QEvent* ev )
{
	bool retval = false;
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
		else if (k->key() == Key_Control)
		{
			setLineStep(QMAX(Decimals * 10, 1));
			retval = true;
		    qApp->sendEvent( this, ev );
			return retval;
		}
	}
	if (ev->type() == QEvent::KeyRelease )
	{
		QKeyEvent* k = (QKeyEvent*)ev;
		if ((k->key() == Key_Shift) || (k->key() == Key_Control))
		{
			setLineStep(Decimals);
			retval = true;
		    qApp->sendEvent( this, ev );
			return retval;
		}
	}
	if ( ev->type() == QEvent::Wheel )
	{
		//If read only dont spin
		if (readOnly)
			return false;
		QWheelEvent* k = (QWheelEvent*)ev;
		bool shiftB=k->state() & ShiftButton;
		bool controlB=k->state() & ControlButton;
		if (shiftB && !controlB)
		{
			setLineStep(QMAX(Decimals / 10, 1));
			retval = true;
			qApp->sendEvent( this, ev );
			return retval;
		} 
		else if (!shiftB && controlB)
		{
			setLineStep(QMAX(Decimals * 10, 1));
			retval = true;
		    qApp->sendEvent( this, ev );
			return retval;
		}
		else if (shiftB && controlB)
		{
			setLineStep(QMAX(Decimals / 100, 1));
			retval = true;
		    qApp->sendEvent( this, ev );
			return retval;
		}
		else
		if (!shiftB && !controlB)
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
 \brief Maps the Text of the Spinbox to the Value, does Unit Conversion and Calculations
 \param None
 \retval The Value
 */

int MSpinBox::mapTextToValue(bool *)
{
	FunctionParser fp;
	setFPConstants(fp);
	QString ts = text();
	QString su = suffix().stripWhiteSpace();
	ts.replace(",", ".");
	ts.replace("%", "");
	//Get all our units strings
	QString strPT=unitGetStrFromIndex(SC_PT);
	QString strMM=unitGetStrFromIndex(SC_MM);
	QString strIN=unitGetStrFromIndex(SC_IN);
	QString strP =unitGetStrFromIndex(SC_P);
	QString strCM=unitGetStrFromIndex(SC_CM);
	QString strC =unitGetStrFromIndex(SC_C);
	//Replace in our typed text all of the units strings with *unitstring
	QRegExp rx("\\b(\\d+)\\s*("+strPT+"|"+strP+"|"+strMM+"|"+strC+"|"+strCM+"|"+strIN+")\\b");
	int pos = 0;
	while (pos >= 0) {
		pos = rx.search(ts, pos);
		if (pos >= 0) {
			QString replacement = rx.cap(1) + "*" + rx.cap(2);
			ts.replace(pos, rx.cap(0).length(), replacement);
		}
	}
	//Get the index of our suffix
	int toConvertToIndex=unitIndexFromString(su);
	//Add in the fparser constants using our unit strings, and the conversion factors.
	fp.AddConstant(strPT, value2value(1.0, SC_PT, toConvertToIndex));
	fp.AddConstant(strMM, value2value(1.0, SC_MM, toConvertToIndex));
	fp.AddConstant(strIN, value2value(1.0, SC_IN, toConvertToIndex));
	fp.AddConstant(strP, value2value(1.0, SC_P, toConvertToIndex));
	fp.AddConstant(strCM, value2value(1.0, SC_CM, toConvertToIndex));
	fp.AddConstant(strC, value2value(1.0, SC_C, toConvertToIndex));

	int ret = fp.Parse(ts.latin1(), "", true);
	if (ret >= 0)
		return 0;
	double erg = fp.Eval(NULL);
	return qRound(erg*Decimals);
}

void MSpinBox::textChanged()
{
	edited = true;
}

void MSpinBox::stepDown()
{
	if ( edited )
		QSpinBox::interpretText();
	if ( wrapping() && ( QSpinBox::value()-lineStep() < QSpinBox::minValue() ) )
		QSpinBox::setValue( QSpinBox::maxValue() - (QSpinBox::maxValue() % lineStep()));
	else
		QSpinBox::subtractLine();
}

void MSpinBox::setValues(double min, double max, int deci, double val)
{
	setDecimals(deci);
	setMinValue(min);
	setMaxValue(max);
	setValue(val);
	edited = false;
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
	edited = false;
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

void MSpinBox::setConstants(const QMap<QString, double>& newConstants)
{
	functionParserConstants=newConstants;
}

void MSpinBox::setFPConstants(FunctionParser &fp)
{
	if (functionParserConstants.isEmpty())
		return;
		
	fp.AddConstant("old", static_cast<double>(QSpinBox::value()) / Decimals);
	QMap<QString, double>::Iterator itend=functionParserConstants.end();
	QMap<QString, double>::Iterator it=functionParserConstants.begin();
	while(it!=itend)
	{
		fp.AddConstant(it.key(), it.data());
		++it;
	}
}
