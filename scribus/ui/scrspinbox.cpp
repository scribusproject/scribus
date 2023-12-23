/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Craig Bradney, cbradney@zip.com.au                                    *
 ***************************************************************************/

#include <algorithm>
#include <cmath>

#include <QtDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QLocale>
#include <QRegExp>

#include "commonstrings.h"
#include "localemgr.h"
#include "scrspinbox.h"
#include "units.h"
#include "third_party/fparser/fparser.hh"

static const QString FinishTag("\xA0");

ScrSpinBox::ScrSpinBox(QWidget *parent, int unitIndex) : QDoubleSpinBox(parent), m_constants(nullptr)
{
	init(unitIndex);
}

ScrSpinBox::ScrSpinBox(double minValue, double maxValue, QWidget *pa, int unitIndex) : QDoubleSpinBox(pa), m_constants(nullptr)
{
	init(unitIndex);
	setMinimum(minValue);
	setMaximum(maxValue);
}

void ScrSpinBox::init(int unitIndex)
{
	m_unitIndex = unitIndex;
	setSuffix(unitGetSuffixFromIndex(m_unitIndex));
	setDecimals(unitGetPrecisionFromIndex(m_unitIndex));
	setLocale(LocaleManager::instance().userPreferredLocale());
	setSingleStep(1.0);
	lineEdit()->setValidator(nullptr);
	disconnect(this, SIGNAL(valueChanged(const QString &)), this, SLOT(textChanged()));
	connect(this, SIGNAL(valueChanged(const QString &)), this, SLOT(textChanged()));
	installEventFilter(this);
}

double ScrSpinBox::unitRatio() const
{
	return unitGetRatioFromIndex(m_unitIndex); 
}

void ScrSpinBox::setValue(int val)
{
	QDoubleSpinBox::setValue(val);
}

void ScrSpinBox::setValue(double val)
{
	QDoubleSpinBox::setValue(val);
}

void ScrSpinBox::showValue(double val)
{
	bool sigBlocked = this->blockSignals(true);
	setValue(val);
	this->blockSignals(sigBlocked);
}

void ScrSpinBox::stepBy(int steps)
{
	if (m_unitIndex == SC_DEGREES)
	{
		double angle = this->value();
		angle += steps * singleStep();
		double angleRange = maximum() - minimum();
		if (angleRange == 360.0)
		{
			while (angle < minimum())
				angle += 360.0;
			while (angle > maximum())
				angle -= 360.0;
		}
		else
			angle = std::max(minimum(), std::min(angle, maximum()));
		setValue(angle);
		return;
	}
	QDoubleSpinBox::stepBy(steps);
}

void ScrSpinBox::setParameters( int s )
{
	if (s >= 0 && s <= unitGetMaxIndex())
		setDecimals(static_cast<int>(pow(10.0, s)));
	else
		setDecimals(100);
}

void ScrSpinBox::setNewUnit(int unitIndex)
{
	double oldUnitRatio = unitGetRatioFromIndex(m_unitIndex);
	double oldVal = value() / oldUnitRatio;
	double oldMax = maximum() / oldUnitRatio;
	double oldMin = minimum() / oldUnitRatio;
	setSuffix(unitGetSuffixFromIndex(unitIndex));
	setDecimals(unitGetPrecisionFromIndex(unitIndex));
	double newUnitRatio = unitGetRatioFromIndex(unitIndex);
	setMinimum(oldMin * newUnitRatio);
	setMaximum(oldMax * newUnitRatio);
	setSingleStep(1.0);
	m_unitIndex = unitIndex;
 	setValue(oldVal * newUnitRatio);
}

void ScrSpinBox::setValues(double min, double max, int deci, double val)
{
	setDecimals(deci);
	setMinimum(min);
	setMaximum(max);
	setValue(val);
}

void ScrSpinBox::getValues(double *min, double *max, int *deci, double *val) const
{
	*deci = decimals();
	*min = minimum();
	*max = maximum();
	*val = value();
}

double ScrSpinBox::getValue(int unitIndex) const
{
	double val = value() / unitGetRatioFromIndex(m_unitIndex);
	if (unitIndex == 0)
		return val;
	return val * unitGetRatioFromIndex(unitIndex);
}

void ScrSpinBox::setConstants(const QMap<QString, double>* constants)
{
	m_constants = constants;
}

double ScrSpinBox::valueFromText(const QString & text) const
{
	//Get a copy for use
	QString ts = text.trimmed();
	//Find our suffix
	QString su(unitGetStrFromIndex(m_unitIndex));
	//Replace our pica XpY.Z format with (X*12+Y.Z)pt
	if (CommonStrings::trStrP.localeAwareCompare(CommonStrings::strP) != 0)
		ts.replace(CommonStrings::trStrP, CommonStrings::strP);

//	const lconv * lc(localeconv());
	QString cSepDecimal(QLocale::c().decimalPoint());
	QString cSepGroup(QLocale::c().groupSeparator());
//	QString sysSepDecimal(QLocale::system().decimalPoint());
//	qDebug()<<"sysSepDecimal"<<sysSepDecimal;
//	QString sysSepGroup(QLocale::system().groupSeparator());
//	qDebug()<<"sysSepGroup"<<sysSepGroup;
//	QString crtSepDecimal(QString::fromLocal8Bit( lc->decimal_point ));
//	QString crtSepGroup(QString::fromLocal8Bit( lc->thousands_sep ));
//	QString crtSepDecimal(LocaleManager::instance().userPreferredLocale().decimalPoint());
	QString crtSepGroup(LocaleManager::instance().userPreferredLocale().groupSeparator());
//	qDebug()<<"crtSepGroup"<<crtSepGroup;
	QString crtSepDecimal(LocaleManager::instance().userPreferredLocale().decimalPoint());
//	qDebug()<<"crtSepDecimal"<<crtSepDecimal;
	QRegExp rxP;
	if (m_unitIndex == SC_PICAS)
		rxP.setPattern("\\b(\\d+)" + CommonStrings::strP + "?(\\d+\\" + crtSepDecimal + "?\\d*)?\\b");
	else
		rxP.setPattern("\\b(\\d+)" + CommonStrings::strP + "(\\d+\\" + crtSepDecimal + "?\\d*)?\\b");
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

/*
	// this could be hardcoded: "."

	if (sysSepGroup != sysSepDecimal)
	{
		qDebug()<<"Removing "<<sysSepGroup;
		ts.remove(sysSepGroup);
	}
	ts.replace(sysSepDecimal, crtSepDecimal);
	qDebug()<<"Replacing "<<sysSepDecimal<<"by"<<crtSepDecimal;
	if (crtSepGroup != crtSepDecimal)
	{
		qDebug()<<"Removing "<<crtSepGroup;
		ts.remove(crtSepGroup);
	}
	ts.replace(crtSepDecimal, cSepDecimal);
	qDebug()<<"Replacing "<<crtSepDecimal<<"by"<<cSepDecimal;
	*/
	if (crtSepGroup != cSepGroup)
	{
//		qDebug()<<"Removing "<<crtSepGroup;
		ts.remove(crtSepGroup);
	}
	if (crtSepDecimal != cSepDecimal)
	{
		ts.replace(crtSepDecimal, cSepDecimal);
//		qDebug()<<"Replacing "<<crtSepDecimal<<"by"<<cSepDecimal;
	}
	ts.replace(CommonStrings::trStrPX, "");
	ts.replace("%", "");
	ts.replace("°", "");
	ts.replace(FinishTag, "");
	ts = ts.trimmed();

	if (ts.endsWith(su))
		ts.chop(su.length());
	int pos = ts.length();
	while (pos > 0)
	{
		pos = ts.lastIndexOf(cSepDecimal, pos);
		if (pos >= 0) 
		{
			if (pos < ts.length())
			{
				if (!ts[pos + 1].isDigit())
					ts.insert(pos + 1, "0 ");
			}
			pos--;
		}
	}
	if (ts.endsWith(cSepDecimal))
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
	if (CommonStrings::trStrPT.localeAwareCompare(CommonStrings::strPT) != 0)
		ts.replace(CommonStrings::trStrPT, CommonStrings::strPT);
	if (CommonStrings::trStrMM.localeAwareCompare(CommonStrings::strMM) != 0)
		ts.replace(CommonStrings::trStrMM, CommonStrings::strMM);
	if (CommonStrings::trStrIN.localeAwareCompare(CommonStrings::strIN) != 0)
		ts.replace(CommonStrings::trStrIN, CommonStrings::strIN);
	if (CommonStrings::trStrCM.localeAwareCompare(CommonStrings::strCM) != 0)
		ts.replace(CommonStrings::trStrCM, CommonStrings::strCM);
	if (CommonStrings::trStrC.localeAwareCompare(CommonStrings::strC) != 0)
		ts.replace(CommonStrings::trStrC, CommonStrings::strC);

	//Add in the fparser constants using our unit strings, and the conversion factors.
	FunctionParser fp;
// 	setFPConstants(fp);
	fp.AddUnit(CommonStrings::strPT.toStdString(), value2value(1.0, SC_PT, m_unitIndex));
	fp.AddUnit(CommonStrings::strMM.toStdString(), value2value(1.0, SC_MM, m_unitIndex));
	fp.AddUnit(CommonStrings::strIN.toStdString(), value2value(1.0, SC_IN, m_unitIndex));
	fp.AddUnit(CommonStrings::strP.toStdString(), value2value(1.0, SC_P, m_unitIndex));
	fp.AddUnit(CommonStrings::strCM.toStdString(), value2value(1.0, SC_CM, m_unitIndex));
	fp.AddUnit(CommonStrings::strC.toStdString(), value2value(1.0, SC_C, m_unitIndex));

	fp.AddConstant("old", value());
	if (m_constants)
	{
		QMap<QString, double>::ConstIterator itend = m_constants->constEnd();
		QMap<QString, double>::ConstIterator it = m_constants->constBegin();
		while (it != itend)
		{
			fp.AddConstant(it.key().toStdString(), it.value() * unitGetRatioFromIndex(m_unitIndex));
			++it;
		}
	}
	std::string str(ts.toLocal8Bit().data());
	double erg = this->value();
	int ret = fp.Parse(str, "", true);
	if(ret < 0)
		erg = fp.Eval(nullptr);
	//qDebug() << "fp value =" << erg ;
	return erg;
}

QString ScrSpinBox::textFromValue(double value) const
{
	if (m_unitIndex == SC_PICAS)
	{
		int a = (static_cast<int>(value)) / 12;
		double b = fabs(fmod(value, 12));
		QString prefix((a == 0 && value < 0.0) ? "-" : "");
		return QString("%1%2%3%4").arg(prefix).arg(a).arg(unitGetStrFromIndex(m_unitIndex)).arg(b);
	}
	return QDoubleSpinBox::textFromValue(value);
}

QValidator::State ScrSpinBox::validate(QString & input, int & pos) const
{
//		qDebug() << "spinbox validate intermediate:" << input;
	if (input.endsWith(FinishTag))
		return QValidator::Acceptable;
	return QValidator::Intermediate;
}

void ScrSpinBox::fixup(QString & input) const
{
	if (!input.endsWith(FinishTag))
		input += FinishTag;
}

void ScrSpinBox::textChanged()
{
// 	qDebug() << "v:" << value() << "t:" << text() << "ct:" << cleanText();
}

bool ScrSpinBox::eventFilter(QObject* watched, QEvent* event)
{
	bool retval = false;
/* Adding this to be sure that the IM* events are processed correctly i.e not intercepted by our KeyPress/Release handlers */
 	if (event->type() == QEvent::InputMethod)
 		return QDoubleSpinBox::eventFilter(watched, event);

	if (event->type() == QEvent::Wheel)
	{
		//If read only don't spin
		if (isReadOnly())
			return false;
		auto* wheelEvent = dynamic_cast<QWheelEvent*>(event);
		bool shiftB = wheelEvent->modifiers() & Qt::ShiftModifier;
		bool altB = wheelEvent->modifiers() & Qt::AltModifier;
		if (shiftB && !altB)
		{
			setSingleStep(0.1);
			retval = QAbstractSpinBox::event(event);
		} 
		else if (!shiftB && altB)
		{
			setSingleStep(10.0);
			retval = QAbstractSpinBox::event(event);
		}
		else if (shiftB && altB)
		{
			setSingleStep(0.01);
			retval = QAbstractSpinBox::event(event);
		}
		else
		{
			setSingleStep(1.0);
			retval = QAbstractSpinBox::event(event);
		}
		return retval;
	}

	if (event->type() == QEvent::LocaleChange)
	{
		setLocale(LocaleManager::instance().userPreferredLocale());
	}

	return QDoubleSpinBox::eventFilter(watched, event);
}
