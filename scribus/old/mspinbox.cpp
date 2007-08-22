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
//#include "mspinbox.moc"
#include <QApplication>
#include <QLineEdit>
#include <QRegExp>
//Added by qt3to4:
#include <QWheelEvent>
#include <QKeyEvent>
#include <QEvent>
#include <cmath>
#include "fparser.h"
#include "units.h"


MSpinBox::MSpinBox(QWidget *pa, int s):QSpinBox(pa)
{
	setParameters(s);
	setValidator(0);
	ed = editor();
	QSpinBox::setLineStep(Decimals);
	oldLineStep = 0;
	currLineStep = Decimals;
	readOnly=false;
	edited = false;
    connect( ed, SIGNAL(textChanged(const QString&)), SLOT(textChanged()) );
}

MSpinBox::MSpinBox(double minValue, double maxValue, QWidget *pa, int s):QSpinBox(pa)
{
	setParameters(s);
	setValidator(0);
	ed = editor();
	QSpinBox::setLineStep(Decimals);
	oldLineStep=0;
	currLineStep = Decimals;
	setMinValue(minValue);
	setMaxValue(maxValue);
	readOnly=false;
	edited = false;
    connect( ed, SIGNAL(textChanged(const QString&)), SLOT(textChanged()) );
}

MSpinBox::MSpinBox(QWidget *parent, const char * name): QSpinBox(parent, name)
{
	setParameters(666); // pv - dummy setup for designer's constructor
	setValidator(0);
	ed = editor();
	QSpinBox::setLineStep(Decimals); // pv - dummy setup for designer's constructor
	oldLineStep=0;
	currLineStep = Decimals;
	readOnly=false;
	edited = false;
	connect( ed, SIGNAL(textChanged(const QString&)), SLOT(textChanged()) );
}

MSpinBox::~MSpinBox()
{
	if (ed)
		disconnect( ed, SIGNAL(textChanged(const QString&)));
}

void MSpinBox::setParameters( int s )
{
	if (s>=0 && s <=unitGetMaxIndex())
	{
		Width=s;
		Decimals=static_cast<int>(pow(10.0, s));
	}
	else
	{
		Width = 2;
		Decimals = 100;
	}
	edited = false;
	m_tabAdvance = true;
}

void MSpinBox::setTabAdvance(bool enable)
{
	m_tabAdvance = enable;
}

bool MSpinBox::eventFilter( QObject* ob, QEvent* ev )
{
	bool retval = false;
/* Adding this to be sure that the IM* events are processed correctly i.e not intercepted by our KeyPress/Release handlers */
	if ((ev->type() == QEvent::IMStart) || (ev->type() == QEvent::IMCompose) || (ev->type() == QEvent::IMEnd))
		return QSpinBox::eventFilter(ob, ev);
	if ( ev->type() == QEvent::KeyPress )
	{
		QKeyEvent* k = (QKeyEvent*)ev;
		bool shiftB=k->state() & Qt::ShiftButton;
		bool controlB=k->state() & Qt::ControlButton;
		if (k->key() == Qt::Key_Shift && !controlB)
		{
			QSpinBox::setLineStep(qMax(currLineStep / 10, 1));
			retval = true;
			qApp->sendEvent( this, ev );
			return retval;
		}
		else if (k->key() == Qt::Key_Control && !shiftB)
		{
			QSpinBox::setLineStep(qMax(currLineStep * 10, 1));
			retval = true;
			qApp->sendEvent( this, ev );
			return retval;
		}
		else if ((k->key() == Qt::Key_Control && shiftB) || (k->key() == Qt::Key_Shift && controlB))
		{
			QSpinBox::setLineStep(qMax(currLineStep / 100, 1));
			retval = true;
			qApp->sendEvent( this, ev );
			return retval;
		}
		else if ((k->key() == Qt::Key_Return) || (k->key() == Qt::Key_Enter) || (k->key() == Qt::Key_Tab))
		{
			if (!m_tabAdvance)
			{
				QSpinBox::interpretText();
				return true;
			}
		}
	}
	if (ev->type() == QEvent::KeyRelease )
	{
		QKeyEvent* k = (QKeyEvent*)ev;
		bool shiftB=k->stateAfter() & Qt::ShiftButton;
		bool controlB=k->stateAfter() & Qt::ControlButton;
		if ((k->key() == Qt::Key_Shift && !controlB) || (k->key() == Qt::Key_Control && !shiftB))
		{
			QSpinBox::setLineStep(currLineStep);
			retval = true;
		    qApp->sendEvent( this, ev );
			return retval;
		}
		else if (k->key() == Qt::Key_Shift && controlB)
		{
			QSpinBox::setLineStep(qMax(currLineStep * 10, 1));
			retval = true;
			qApp->sendEvent( this, ev );
			return retval;
		}
		else if (k->key() == Qt::Key_Control && shiftB)
		{
			QSpinBox::setLineStep(qMax(currLineStep / 10, 1));
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
		bool shiftB=k->state() & Qt::ShiftButton;
		bool controlB=k->state() & Qt::ControlButton;
		if (shiftB && !controlB)
		{
			QSpinBox::setLineStep(qMax(currLineStep / 10, 1));
			retval = true;
			qApp->sendEvent( this, ev );
			return retval;
		} 
		else if (!shiftB && controlB)
		{
			QSpinBox::setLineStep(qMax(currLineStep * 10, 1));
			retval = true;
		    qApp->sendEvent( this, ev );
			return retval;
		}
		else if (shiftB && controlB)
		{
			QSpinBox::setLineStep(qMax(currLineStep / 100, 1));
			retval = true;
		    qApp->sendEvent( this, ev );
			return retval;
		}
		else
		if (!shiftB && !controlB)
		{
			QSpinBox::setLineStep(currLineStep);
			retval = true;
			qApp->sendEvent( this, ev );
			return retval;
		}
	}
	return QSpinBox::eventFilter(ob, ev);
}

QString MSpinBox::mapValueToText(int value)
{
	QString s;
// 	debug outputs for strange designer's behaviour. See (parent, name) constructor for more info - PV
// 	qDebug("setting s");
// 	qDebug(QString("params %1 %2 %3 %4<-").arg(value).arg(Decimals).arg(Width).arg(name()));
	s.setNum(static_cast<double>(value) / Decimals, 'f', Width);
	return s;
}

int MSpinBox::mapTextToValue(bool *)
{
	FunctionParser fp;
	setFPConstants(fp);
	QString ts = text().toLower();
	QString su = suffix().trimmed().toLower();
	ts.replace(",", ".");
	ts.replace("%", "");
	int pos = ts.length();
	while (pos > 0)
	{
		pos = ts.lastIndexOf(".", pos);
		if (pos >= 0) 
		{
			if (pos < static_cast<int>(ts.length()))
			{
				if (!ts[pos+1].isDigit())
					ts.insert(pos+1, "0 ");
			}
			pos--;
		}
	}
	if (ts.endsWith("."))
		ts.append("0");
	
	//Get all our units strings
	QString trStrPT=unitGetStrFromIndex(SC_PT);
	QString trStrMM=unitGetStrFromIndex(SC_MM);
	QString trStrIN=unitGetStrFromIndex(SC_IN);
	QString trStrP =unitGetStrFromIndex(SC_P);
	QString trStrCM=unitGetStrFromIndex(SC_CM);
	QString trStrC =unitGetStrFromIndex(SC_C);
	QString strPT=unitGetUntranslatedStrFromIndex(SC_PT);
	QString strMM=unitGetUntranslatedStrFromIndex(SC_MM);
	QString strIN=unitGetUntranslatedStrFromIndex(SC_IN);
	QString strP =unitGetUntranslatedStrFromIndex(SC_P);
	QString strCM=unitGetUntranslatedStrFromIndex(SC_CM);
	QString strC =unitGetUntranslatedStrFromIndex(SC_C);
	//CB FParser doesn't handle unicode well/at all.
	//So, instead of just getting the translated strings and
	//sticking them in as variables in the parser, if they are
	//not the same as the untranslated version, then we replace them.
	//We lose the ability for now to have some strings in languages 
	//that might use them in variables.
	//To return to previous functionality, remove the follow replacement ifs,
	//S&R in the trStr* assignments trStrPT->strPT and remove the current str* ones. 
	//IE, send the translated strings through to the regexp.
	if (trStrPT.localeAwareCompare(strPT)!=0)
		ts.replace(trStrPT, strPT);
	if (trStrMM.localeAwareCompare(strMM)!=0)
		ts.replace(trStrMM, strMM);
	if (trStrIN.localeAwareCompare(strIN)!=0)
		ts.replace(trStrIN, strIN);
	if (trStrP.localeAwareCompare(strP)!=0)
		ts.replace(trStrP, strP);
	if (trStrCM.localeAwareCompare(strCM)!=0)
		ts.replace(trStrCM, strCM);
	if (trStrC.localeAwareCompare(strC)!=0)
		ts.replace(trStrC, strC);
	//Replace in our typed text all of the units strings with *unitstring
	QRegExp rx("\\b(\\d+)\\s*("+strPT+"|"+strP+"|"+strMM+"|"+strC+"|"+strCM+"|"+strIN+")\\b");
	pos = 0;
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
	fp.AddConstant(strPT.local8Bit().data(), value2value(1.0, SC_PT, toConvertToIndex));
	fp.AddConstant(strMM.local8Bit().data(), value2value(1.0, SC_MM, toConvertToIndex));
	fp.AddConstant(strIN.local8Bit().data(), value2value(1.0, SC_IN, toConvertToIndex));
	fp.AddConstant(strP.local8Bit().data(), value2value(1.0, SC_P, toConvertToIndex));
	fp.AddConstant(strCM.local8Bit().data(), value2value(1.0, SC_CM, toConvertToIndex));
	fp.AddConstant(strC.local8Bit().data(), value2value(1.0, SC_C, toConvertToIndex));
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
	currLineStep = Decimals;
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

void MSpinBox::setMaxValue(double val)
{
	QSpinBox::setMaxValue(qRound(val*Decimals));
}

void MSpinBox::setMinValue(double val)
{
	QSpinBox::setMinValue(qRound(val*Decimals));
}

void MSpinBox::setValue(double val)
{
	QSpinBox::setValue(qRound(val*Decimals));
	edited = false;
}

void MSpinBox::setLineStepM(int val)
{
	QSpinBox::setLineStep( val * Decimals );
	currLineStep = val * Decimals;
}

double MSpinBox::value()
{
	return static_cast<double>(QSpinBox::value()) / Decimals;
}

double MSpinBox::minValue()
{
	return static_cast<double>(QSpinBox::minValue()) / Decimals;
}

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

void MSpinBox::setNewUnit(double oldUnitRatio, double newUnitRatio, int unitIndex)
{
	double oldVal = value() / oldUnitRatio;
	double oldMax = maxValue() / oldUnitRatio;
	double oldMin = minValue() / oldUnitRatio;
	setSuffix(unitGetSuffixFromIndex(unitIndex));
	setDecimals(unitGetDecimalsFromIndex(unitIndex));
	setMinValue(oldMin * newUnitRatio);
	setMaxValue(oldMax * newUnitRatio);
	setValue(oldVal * newUnitRatio);
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
		fp.AddConstant(it.key().local8Bit().data(), it.data());
		++it;
	}
}
