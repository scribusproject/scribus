/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Craig Bradney, cbradney@zip.com.au                                    *
 ***************************************************************************/

#include <cmath>

#include <QtDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QRegExp>

#include "scrspinbox.h"
#include "units.h"

ScrSpinBox::ScrSpinBox(QWidget *parent, int unitIndex) : QDoubleSpinBox(parent)
{
	init(unitIndex);
}

ScrSpinBox::ScrSpinBox(double minValue, double maxValue, QWidget *pa, int unitIndex):QDoubleSpinBox(pa)
{
	init(unitIndex);
	setMinimum(minValue);
	setMaximum(maxValue);
}

void ScrSpinBox::init(int unitIndex)
{
	m_unitIndex=unitIndex;
	m_tabAdvance=true;
	setSuffix(unitGetSuffixFromIndex(m_unitIndex));
	setDecimals(unitGetPrecisionFromIndex(m_unitIndex));
	setSingleStep(1.0);
	disconnect(this, SIGNAL(valueChanged(const QString &)), this, SLOT(textChanged()));
	connect(this, SIGNAL(valueChanged(const QString &)), this, SLOT(textChanged()));
	installEventFilter(this);
}

ScrSpinBox::~ScrSpinBox()
{
}

void ScrSpinBox::setParameters( int s )
{
	if (s>=0 && s <=unitGetMaxIndex())
		setDecimals(static_cast<int>(pow(10.0, s)));
	else
		setDecimals(100);
}

void ScrSpinBox::setNewUnit(int unitIndex)
{
	double oldUnitRatio=unitGetRatioFromIndex(m_unitIndex);
	double oldVal = value() / oldUnitRatio;
	double oldMax = maximum() / oldUnitRatio;
	double oldMin = minimum() / oldUnitRatio;
	setSuffix(unitGetSuffixFromIndex(unitIndex));
	setDecimals(unitGetPrecisionFromIndex(unitIndex));
	double newUnitRatio=unitGetRatioFromIndex(unitIndex);
	setMinimum(oldMin * newUnitRatio);
	setMaximum(oldMax * newUnitRatio);
	setValue(oldVal * newUnitRatio);
	setSingleStep(1.0);
	m_unitIndex=unitIndex;
}

void ScrSpinBox::setValues(double min, double max, int deci, double val)
{
	setDecimals(deci);
	setMinimum(min);
	setMaximum(max);
	setValue(val);
}

void ScrSpinBox::getValues(double *min, double *max, int *deci, double *val)
{
	*deci = decimals();
	*min = minimum();
	*max = maximum();
	*val = value();
}

double ScrSpinBox::getValue(int unitIndex)
{
	double val=value() / unitGetRatioFromIndex(m_unitIndex);
	if (unitIndex==0)
		return val;
	return val * unitGetRatioFromIndex(unitIndex);
}

double ScrSpinBox::valueFromText ( const QString & text ) const
{
//	qDebug() << "vft:" << text;
// 	return QDoubleSpinBox::valueFromText(text);
	
	FunctionParser fp;
//  	setFPConstants(fp);
	QString ts = text;
	QString su = suffix().trimmed();
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
	if (trStrC.localeAwareCompare(strPT)!=0)
		ts.replace(trStrC, strC);
	//Replace in our typed text all of the units strings with *unitstring
	QRegExp rx("\\b(\\d+)\\s*("+strPT+"|"+strP+"|"+strMM+"|"+strC+"|"+strCM+"|"+strIN+")\\b");
	pos = 0;
	while (pos >= 0) {
		pos = rx.indexIn(ts, pos);
		if (pos >= 0) {
			QString replacement = rx.cap(1) + "*" + rx.cap(2);
			ts.replace(pos, rx.cap(0).length(), replacement);
		}
	}
	//Get the index of our suffix
	int toConvertToIndex=unitIndexFromString(su);
	//Add in the fparser constants using our unit strings, and the conversion factors.
	fp.AddConstant(strPT.toStdString(), value2value(1.0, SC_PT, toConvertToIndex));
	fp.AddConstant(strMM.toStdString(), value2value(1.0, SC_MM, toConvertToIndex));
	fp.AddConstant(strIN.toStdString(), value2value(1.0, SC_IN, toConvertToIndex));
	fp.AddConstant(strP.toStdString(), value2value(1.0, SC_P, toConvertToIndex));
	fp.AddConstant(strCM.toStdString(), value2value(1.0, SC_CM, toConvertToIndex));
	fp.AddConstant(strC.toStdString(), value2value(1.0, SC_C, toConvertToIndex));
	//int ret = fp.Parse(ts.toLatin1().toStdString(), "", true);
	int ret = fp.Parse(ts.toStdString(), "", true);
	if (ret >= 0)
		return 0;
	double erg = fp.Eval(NULL);
	return erg;
}

QValidator::State ScrSpinBox::validate ( QString & input, int & pos ) const
{
	return QValidator::Acceptable;
}

void ScrSpinBox::textChanged()
{
	//qDebug() << "v:" << value() << "t:" << text() << "ct:" << cleanText();
}

bool ScrSpinBox::eventFilter( QObject* watched, QEvent* event )
{
// 	qDebug() << "eventFilter";
	bool retval = false;
/* Adding this to be sure that the IM* events are processed correctly i.e not intercepted by our KeyPress/Release handlers */
 	if (event->type() == QEvent::InputMethod)
 		return QDoubleSpinBox::eventFilter(watched, event);
	if ( event->type() == QEvent::KeyPress )
	{
		QKeyEvent* k = (QKeyEvent*)event;
		bool shiftB=k->modifiers() & Qt::ShiftModifier;
		bool controlB=k->modifiers() & Qt::ControlModifier;
		if (k->key() == Qt::Key_Shift && !controlB)
		{
			setSingleStep(0.1);
			retval = QWidget::event(event);
		}
		else if (k->key() == Qt::Key_Control && !shiftB)
		{
			setSingleStep(10.0);
			retval = QWidget::event(event);
		}
		else if ((k->key() == Qt::Key_Control && shiftB) || (k->key() == Qt::Key_Shift && controlB))
		{
			setSingleStep(0.01);
			retval = QWidget::event(event);
		}
		else if ((k->key() == Qt::Key_Return) || (k->key() == Qt::Key_Enter) || (k->key() == Qt::Key_Tab))
		{
 			if (!m_tabAdvance)
			{
				QDoubleSpinBox::interpretText();
				return true;
			}
		}
	}
	else
	if (event->type() == QEvent::KeyRelease )
	{
		QKeyEvent* k = (QKeyEvent*)event;
		bool shiftB=k->modifiers() & Qt::ShiftModifier;
		bool controlB=k->modifiers() & Qt::ControlModifier;
		if ((k->key() == Qt::Key_Shift && !controlB) || (k->key() == Qt::Key_Control && !shiftB))
		{
			setSingleStep(1.0);
			retval = QWidget::event(event);
		}
		else if (k->key() == Qt::Key_Shift && controlB)
		{
			setSingleStep(10.0);
			retval = QWidget::event(event);
		}
		else if (k->key() == Qt::Key_Control && shiftB)
		{
			setSingleStep(0.1);
			retval = QWidget::event(event);
		}
	}
	else
	if ( event->type() == QEvent::Wheel )
	{
		//If read only dont spin
		if (isReadOnly())
			return false;
		QWheelEvent* k = (QWheelEvent*)event;
		bool shiftB=k->modifiers() & Qt::ShiftModifier;
		bool controlB=k->modifiers() & Qt::ControlModifier;
		if (shiftB && !controlB)
		{
			setSingleStep(0.1);
			retval=QWidget::event(event);
		} 
		else if (!shiftB && controlB)
		{
			setSingleStep(10.0);
			retval=QWidget::event(event);
		}
		else if (shiftB && controlB)
		{
			setSingleStep(0.01);
			retval=QWidget::event(event);
		}
		else
		if (!shiftB && !controlB)
		{
 			setSingleStep(1.0);
			retval=QWidget::event(event);
		}
	}
	else
		return QDoubleSpinBox::eventFilter(watched, event);
	return retval;
}

void ScrSpinBox::setLineStepM(int val)
{
	QDoubleSpinBox::setSingleStep( val );
//	currLineStep = val * Decimals;
}

void ScrSpinBox::setTabAdvance(bool enable)
{
	m_tabAdvance = enable;
}
