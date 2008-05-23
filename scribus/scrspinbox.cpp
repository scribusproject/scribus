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

#include "commonstrings.h"
#include "scrspinbox.h"
#include "units.h"
#include "fparser.h"

ScrSpinBox::ScrSpinBox(QWidget *parent, int unitIndex) : QDoubleSpinBox(parent), m_constants(NULL)
{
	init(unitIndex);
}

ScrSpinBox::ScrSpinBox(double minValue, double maxValue, QWidget *pa, int unitIndex) : QDoubleSpinBox(pa), m_constants(NULL)
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
	lineEdit()->setValidator(0);
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
	setSingleStep(1.0);
	m_unitIndex=unitIndex;
 	setValue(oldVal * newUnitRatio);
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

void ScrSpinBox::setConstants(const QMap<QString, double>* constants)
{
	m_constants = constants;
}



static const QString FinishTag("\xA0");

double ScrSpinBox::valueFromText ( const QString & text ) const
{
	//Get all our units strings
//CB: Replaced by new CommonStrings versions
// 	QString trStrPT=unitGetStrFromIndex(SC_PT);
// 	QString trStrMM=unitGetStrFromIndex(SC_MM);
// 	QString trStrIN=unitGetStrFromIndex(SC_IN);
// 	QString trStrP =unitGetStrFromIndex(SC_P);
// 	QString trStrCM=unitGetStrFromIndex(SC_CM);
// 	QString trStrC =unitGetStrFromIndex(SC_C);
// 	QString strPT=unitGetUntranslatedStrFromIndex(SC_PT);
// 	QString strMM=unitGetUntranslatedStrFromIndex(SC_MM);
// 	QString strIN=unitGetUntranslatedStrFromIndex(SC_IN);
// 	QString strP =unitGetUntranslatedStrFromIndex(SC_P);
// 	QString strCM=unitGetUntranslatedStrFromIndex(SC_CM);
// 	QString strC =unitGetUntranslatedStrFromIndex(SC_C);
	//Get a copy for use
	QString ts = text.trimmed();
	//Find our suffix
	QString su(suffix().trimmed());
	//Get the index of our suffix
	int toConvertToIndex=unitIndexFromString(su);
	//Replace our pica XpY.Z format with (X*12+Y.Z)pt
	if (CommonStrings::trStrP.localeAwareCompare(CommonStrings::strP)!=0)
		ts.replace(CommonStrings::trStrP, CommonStrings::strP);
	QRegExp rxP("\\b(\\d+)"+CommonStrings::strP+"(\\d+\\.?\\d*)?\\b");
	int posP = 0;
	while (posP >= 0)
	{
// 		qDebug() << "#";
		posP = rxP.indexIn(ts, posP);
		if (posP >= 0)
		{
// 			qDebug() << rxP.cap(1);
// 			qDebug() << rxP.cap(2);
			QString replacement = QString("%1%2").arg(rxP.cap(1).toDouble()*(static_cast<double>(unitGetBaseFromIndex(SC_PICAS))) + rxP.cap(2).toDouble()).arg(CommonStrings::strPT);
			ts.replace(posP, rxP.cap(0).length(), replacement);
// 			qDebug() << ts;
		}
	}
// 	qDebug() << "##" << ts;
	
	ts.replace(",", ".");
	ts.replace("%", "");
	ts.replace("°", "");
	ts.replace(FinishTag, "");
	ts = ts.trimmed();

	if (ts.endsWith(su))
		ts = ts.left(ts.length()-su.length());
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
	//CB FParser doesn't handle unicode well/at all.
	//So, instead of just getting the translated strings and
	//sticking them in as variables in the parser, if they are
	//not the same as the untranslated version, then we replace them.
	//We lose the ability for now to have some strings in languages 
	//that might use them in variables.
	//To return to previous functionality, remove the follow replacement ifs,
	//S&R in the trStr* assignments trStrPT->strPT and remove the current str* ones. 
	//IE, send the translated strings through to the regexp.
	if (CommonStrings::trStrPT.localeAwareCompare(CommonStrings::strPT)!=0)
		ts.replace(CommonStrings::trStrPT, CommonStrings::strPT);
	if (CommonStrings::trStrMM.localeAwareCompare(CommonStrings::strMM)!=0)
		ts.replace(CommonStrings::trStrMM, CommonStrings::strMM);
	if (CommonStrings::trStrIN.localeAwareCompare(CommonStrings::strIN)!=0)
		ts.replace(CommonStrings::trStrIN, CommonStrings::strIN);
	if (CommonStrings::trStrCM.localeAwareCompare(CommonStrings::strCM)!=0)
		ts.replace(CommonStrings::trStrCM, CommonStrings::strCM);
	if (CommonStrings::trStrC.localeAwareCompare(CommonStrings::strPT)!=0)
		ts.replace(CommonStrings::trStrC, CommonStrings::strC);
	//Replace in our typed text all of the units strings with *unitstring
	QRegExp rx("\\b(\\d+)\\s*("+CommonStrings::strPT+"|"+CommonStrings::strMM+"|"+CommonStrings::strC+"|"+CommonStrings::strCM+"|"+CommonStrings::strIN+")\\b");
	pos = 0;
	while (pos >= 0) {
		pos = rx.indexIn(ts, pos);
		if (pos >= 0) {
			QString replacement = rx.cap(1) + "*" + rx.cap(2);
			ts.replace(pos, rx.cap(0).length(), replacement);
		}
	}

	//Add in the fparser constants using our unit strings, and the conversion factors.
	FunctionParser fp;
// 	setFPConstants(fp);
	fp.AddConstant(CommonStrings::strPT.toStdString(), value2value(1.0, SC_PT, toConvertToIndex));
	fp.AddConstant(CommonStrings::strMM.toStdString(), value2value(1.0, SC_MM, toConvertToIndex));
	fp.AddConstant(CommonStrings::strIN.toStdString(), value2value(1.0, SC_IN, toConvertToIndex));
	fp.AddConstant(CommonStrings::strP.toStdString(), value2value(1.0, SC_P, toConvertToIndex));
	fp.AddConstant(CommonStrings::strCM.toStdString(), value2value(1.0, SC_CM, toConvertToIndex));
	fp.AddConstant(CommonStrings::strC.toStdString(), value2value(1.0, SC_C, toConvertToIndex));

	fp.AddConstant("old", value());
	if (m_constants)
	{
		QMap<QString, double>::ConstIterator itend = m_constants->constEnd();
		QMap<QString, double>::ConstIterator it = m_constants->constBegin();
		while(it != itend)
		{
			fp.AddConstant(it.key().toStdString(), it.value());
			++it;
		}
	}
	
	int ret = fp.Parse(ts.toStdString(), "", true);
//	qDebug() << "fp return =" << ret;
	if (ret >= 0)
		return 0;
	double erg = fp.Eval(NULL);
//	qDebug() << "fp value =" << erg;
	return erg;
}

QString ScrSpinBox::textFromValue ( double value ) const
{
	if (m_unitIndex==SC_PICAS)
	{
// 		QString r=QString("%1%2%3").arg((static_cast<int>(value))/12).arg(unitGetStrFromIndex(m_unitIndex)).arg(fabs(fmod(value, 12)));
		int a=(static_cast<int>(value))/12;
		double b=fabs(fmod(value, 12));
		QString prefix((a==0 && value < 0.0) ? "-" : "");
		return QString("%1%2%3%4").arg(prefix).arg(a).arg(unitGetStrFromIndex(m_unitIndex)).arg(b);
	}
	return QDoubleSpinBox::textFromValue ( value );
}

QValidator::State ScrSpinBox::validate ( QString & input, int & pos ) const
{
	if (input.endsWith(FinishTag))
	{
//		qDebug() << "spinbox validate acceptable:" << input;
		return QValidator::Acceptable;
	}
	else
	{
//		qDebug() << "spinbox validate intermediate:" << input;
		return QValidator::Intermediate;
	}
}

void ScrSpinBox::fixup ( QString & input ) const
{
	if (!input.endsWith(FinishTag))
		input += FinishTag;
}


void ScrSpinBox::textChanged()
{
// 	qDebug() << "v:" << value() << "t:" << text() << "ct:" << cleanText();
}

bool ScrSpinBox::eventFilter( QObject* watched, QEvent* event )
{
	bool retval = false;
/* Adding this to be sure that the IM* events are processed correctly i.e not intercepted by our KeyPress/Release handlers */
 	if (event->type() == QEvent::InputMethod)
 		return QDoubleSpinBox::eventFilter(watched, event);
	/*
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
			qDebug("boo1");
			setSingleStep(0.01);
			retval = QWidget::event(event);
		}
		else if ((k->key() == Qt::Key_Return) || (k->key() == Qt::Key_Enter) || (k->key() == Qt::Key_Tab))
		{
 			if (!m_tabAdvance)
			{
//				qDebug() << "eventFilter: interpretText";
				QDoubleSpinBox::interpretText();
				return true;
			}
		}
	}
	else if (event->type() == QEvent::KeyRelease )
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
	*/
	if ( event->type() == QEvent::Wheel )
	{
		//If read only dont spin
		if (isReadOnly())
			return false;
		QWheelEvent* k = (QWheelEvent*)event;
		bool shiftB=k->modifiers() & Qt::ShiftModifier;
		bool altB=k->modifiers() & Qt::AltModifier;
		if (shiftB && !altB)
		{
			setSingleStep(0.1);
			retval=QWidget::event(event);
		} 
		else if (!shiftB && altB)
		{
			setSingleStep(10.0);
			retval=QWidget::event(event);
		}
		else if (shiftB && altB)
		{
			setSingleStep(0.01);
			retval=QWidget::event(event);
		}
		else if (!shiftB && !altB)
		{
			setSingleStep(1.0);
			retval=QWidget::event(event);
		}
	}
	else
		return QDoubleSpinBox::eventFilter(watched, event);
	return retval;
}

// void ScrSpinBox::setSingleStepM(int val)
// {
// 	QDoubleSpinBox::setSingleStep( val );
// //	currLineStep = val * Decimals;
// }

void ScrSpinBox::setTabAdvance(bool enable)
{
	m_tabAdvance = enable;
}
