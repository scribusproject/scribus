/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QToolButton>
#include <QSignalBlocker>

#include "smtabruler.h"
#include "smscrspinbox.h"
#include "units.h"


SMTabruler::SMTabruler(QWidget* parent, bool haveFirst, int dEin, QList<ParagraphStyle::TabRecord> Tabs, double wid)
	: Tabruler(parent, haveFirst, dEin, Tabs, wid)
{
	m_parentButton = new QToolButton(this);
	Q_CHECK_PTR(m_parentButton);
	m_parentButton->setText( tr(" Parent Tabs "));
	indentLayout->addWidget(m_parentButton);
	m_parentButton->hide();
	QFont f(font());
	f.setBold(true);
	m_parentButton->setFont(f);
	connect(m_parentButton, SIGNAL(clicked()), this, SLOT(pbClicked()));
	firstLineSpin = new SMScrSpinBox(-3000, 4000, this, dEin);
	Q_CHECK_PTR(firstLineSpin);
	leftIndentSpin = new SMScrSpinBox(0, 4000, this, dEin);
	Q_CHECK_PTR(leftIndentSpin);
	rightIndentSpin = new SMScrSpinBox(0, 4000, this, dEin);
	Q_CHECK_PTR(rightIndentSpin);

	indentLayout->removeWidget(firstLineData);
	firstLineData->hide();
	indentLayout->insertWidget(1, firstLineSpin);
	firstLineSpin->show();

	indentLayout->removeWidget(leftIndentData);
	leftIndentData->hide();
	indentLayout->insertWidget(3, leftIndentSpin);
	leftIndentSpin->show();

	indentLayout->removeWidget(rightIndentData);
	rightIndentData->hide();
	indentLayout->insertWidget(5, rightIndentSpin);
	rightIndentSpin->show();

	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));

	connect(firstLineSpin, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(rightIndentSpin, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(leftIndentSpin, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::unitChange(int unitIndex)
{
	this->blockSignals(true);
	Tabruler::setTabs(ruler->tabValues, unitIndex);
	Tabruler::repaint();

	firstLineSpin->blockSignals(true);
	leftIndentSpin->blockSignals(true);
	rightIndentSpin->blockSignals(true);
	tabData->blockSignals(true);

	firstLineSpin->setNewUnit(unitIndex);
	leftIndentSpin->setNewUnit(unitIndex);
	rightIndentSpin->setNewUnit(unitIndex);
	tabData->setNewUnit(unitIndex);

	firstLineSpin->blockSignals(false);
	leftIndentSpin->blockSignals(false);
	rightIndentSpin->blockSignals(false);
	tabData->blockSignals(false);

	m_unitIndex = unitIndex;
	this->blockSignals(false);
}

void SMTabruler::setTabs(const QList<ParagraphStyle::TabRecord>& Tabs, int unitIndex)
{
	disconnect(this, SIGNAL(tabsChanged()), this, SLOT(slotTabsChanged()));
	disconnect(this, SIGNAL(mouseReleased()), this, SLOT(slotTabsChanged()));
	m_hasParent = false;
	m_parentButton->hide();
	Tabruler::setTabs(Tabs, unitIndex);
	Tabruler::repaint();
	firstLineSpin->setNewUnit(unitIndex);
	leftIndentSpin->setNewUnit(unitIndex);
	rightIndentSpin->setNewUnit(unitIndex);
	tabData->setNewUnit(unitIndex);
}

void SMTabruler::setTabs(const QList<ParagraphStyle::TabRecord>& Tabs, int unitIndex, bool isParentValue)
{
	disconnect(this, SIGNAL(tabsChanged()), this, SLOT(slotTabsChanged()));
	disconnect(this, SIGNAL(mouseReleased()), this, SLOT(slotTabsChanged()));
	m_hasParent  = true;
	m_unitIndex = unitIndex;
	if (isParentValue)
		m_parentButton->hide();
	else
		m_parentButton->show();
	Tabruler::setTabs(Tabs, unitIndex);
	Tabruler::repaint();
	firstLineSpin->setNewUnit(unitIndex);
	leftIndentSpin->setNewUnit(unitIndex);
	rightIndentSpin->setNewUnit(unitIndex);
	tabData->setNewUnit(unitIndex);

	connect(this, SIGNAL(tabsChanged()), this, SLOT(slotTabsChanged()));
	connect(this, SIGNAL(mouseReleased()), this, SLOT(slotTabsChanged()));
}

void SMTabruler::setParentTabs(const QList<ParagraphStyle::TabRecord>& Tabs)
{
	m_hasParent = true;
	m_parentTabs = Tabs;
}

void SMTabruler::setFirstLineValue(double t)
{
	QSignalBlocker firstLineDataBlocker(firstLineData);
	QSignalBlocker rightIndentDataBlocker(rightIndentData);
	QSignalBlocker leftIndentDataBlocker(leftIndentData);
	QSignalBlocker firstLineSpinBlocker(firstLineSpin);
	QSignalBlocker rightIndentSpinBlocker(rightIndentSpin);
	QSignalBlocker leftIndentSpinBlocker(leftIndentSpin);

	firstLineSpin->setValue(t);
	firstLineData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
}

void SMTabruler::setFirstLineValue(double t, bool isParentValue)
{
	QSignalBlocker firstLineDataBlocker(firstLineData);
	QSignalBlocker rightIndentDataBlocker(rightIndentData);
	QSignalBlocker leftIndentDataBlocker(leftIndentData);
	QSignalBlocker firstLineSpinBlocker(firstLineSpin);
	QSignalBlocker rightIndentSpinBlocker(rightIndentSpin);
	QSignalBlocker leftIndentSpinBlocker(leftIndentSpin);

	m_isSetupFirst = true;
	firstLineSpin->setValue(t, isParentValue);
	firstLineData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
}

void SMTabruler::setParentFirstLine(double t)
{
	firstLineSpin->setParentValue(t);
}

void SMTabruler::setLeftIndentValue(double t)
{
	QSignalBlocker firstLineDataBlocker(firstLineData);
	QSignalBlocker rightIndentDataBlocker(rightIndentData);
	QSignalBlocker leftIndentDataBlocker(leftIndentData);
	QSignalBlocker firstLineSpinBlocker(firstLineSpin);
	QSignalBlocker rightIndentSpinBlocker(rightIndentSpin);
	QSignalBlocker leftIndentSpinBlocker(leftIndentSpin);

	leftIndentSpin->setValue(t);
	leftIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
}

void SMTabruler::setLeftIndentValue(double t, bool isParentValue)
{
	QSignalBlocker firstLineDataBlocker(firstLineData);
	QSignalBlocker rightIndentDataBlocker(rightIndentData);
	QSignalBlocker leftIndentDataBlocker(leftIndentData);
	QSignalBlocker firstLineSpinBlocker(firstLineSpin);
	QSignalBlocker rightIndentSpinBlocker(rightIndentSpin);
	QSignalBlocker leftIndentSpinBlocker(leftIndentSpin);

	m_isSetupLeft = true;
	leftIndentSpin->setValue(t, isParentValue);
	leftIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
}

void SMTabruler::setParentLeftIndent(double t)
{
	leftIndentSpin->setParentValue(t);
}

void SMTabruler::setRightIndentValue(double t)
{
	QSignalBlocker firstLineDataBlocker(firstLineData);
	QSignalBlocker rightIndentDataBlocker(rightIndentData);
	QSignalBlocker leftIndentDataBlocker(leftIndentData);
	QSignalBlocker firstLineSpinBlocker(firstLineSpin);
	QSignalBlocker rightIndentSpinBlocker(rightIndentSpin);
	QSignalBlocker leftIndentSpinBlocker(leftIndentSpin);

	rightIndentSpin->setValue(t);
	rightIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
}

void SMTabruler::setRightIndentValue(double t, bool isParentValue)
{
	QSignalBlocker firstLineDataBlocker(firstLineData);
	QSignalBlocker rightIndentDataBlocker(rightIndentData);
	QSignalBlocker leftIndentDataBlocker(leftIndentData);
	QSignalBlocker firstLineSpinBlocker(firstLineSpin);
	QSignalBlocker rightIndentSpinBlocker(rightIndentSpin);
	QSignalBlocker leftIndentSpinBlocker(leftIndentSpin);

	m_isSetupRight = true;
	rightIndentSpin->setValue(t, isParentValue);
	rightIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
}

void SMTabruler::setParentRightIndent(double t)
{
	rightIndentSpin->setParentValue(t);
}

bool SMTabruler::useParentTabs()
{
	bool ret = m_useParentTabs;
	if (ret && m_hasParent)
	{
		setTabs(m_parentTabs, m_unitIndex, true);
		Tabruler::repaint();
		m_parentButton->hide();
		m_useParentTabs = false;
	}
	else if (m_hasParent)
	{
		m_parentButton->show();
	}

	return ret;
}

bool SMTabruler::useParentFirstLine()
{
	return firstLineSpin->useParentValue();
}

bool SMTabruler::useParentLeftIndent()
{
	return leftIndentSpin->useParentValue();
}

bool SMTabruler::useParentRightIndent()
{
	return rightIndentSpin->useParentValue();
}

void SMTabruler::slotTabsChanged()
{
	if (m_hasParent)
	{
		m_useParentTabs = false;
		m_tabsChanged = true;
	}
}

void SMTabruler::pbClicked()
{
	if (m_hasParent)
	{
		m_useParentTabs = true;
		emit mouseReleased();
	}
}

void SMTabruler::leftDataChanged()
{
	QSignalBlocker firstLineSpinBlocker(firstLineSpin);
	QSignalBlocker rightIndentSpinBlocker(rightIndentSpin);
	QSignalBlocker leftIndentSpinBlocker(leftIndentSpin);

	double a, b, value;
	int c;
	leftIndentData->getValues(&a, &b, &c, &value);
	if (m_hasParent && !m_isSetupLeft)
		leftIndentSpin->setValue(value, false);
	else if (!m_hasParent)
		leftIndentSpin->setValue(value);

	m_isSetupLeft = false;
}

void SMTabruler::rightDataChanged()
{
	QSignalBlocker firstLineSpinBlocker(firstLineSpin);
	QSignalBlocker rightIndentSpinBlocker(rightIndentSpin);
	QSignalBlocker leftIndentSpinBlocker(leftIndentSpin);

	double a, b, value;
	int c;
	rightIndentData->getValues(&a, &b, &c, &value);
	if (m_hasParent && !m_isSetupRight)
		rightIndentSpin->setValue(value, false);
	else if (!m_hasParent)
		rightIndentSpin->setValue(value);

	m_isSetupRight = false;
}

void SMTabruler::firstDataChanged()
{
	QSignalBlocker firstLineSpinBlocker(firstLineSpin);
	QSignalBlocker rightIndentSpinBlocker(rightIndentSpin);
	QSignalBlocker leftIndentSpinBlocker(leftIndentSpin);

	double a, b, value;
	int c;
	firstLineData->getValues(&a, &b, &c, &value);
	if (m_hasParent && !m_isSetupFirst)
		firstLineSpin->setValue(value, false);
	else if (!m_hasParent)
		firstLineSpin->setValue(value);

	m_isSetupFirst = false;
}

void SMTabruler::firstValueChanged()
{
//	QSignalBlocker firstLineDataBlocker(firstLineData);
//	QSignalBlocker rightIndentDataBlocker(rightIndentData);
//	QSignalBlocker leftIndentDataBlocker(leftIndentData);

	double a, b, value;
	int c;
	firstLineSpin->getValues(&a, &b, &c, &value);
	setFirstLineData(value / m_docUnitRatio);
	setFirstLine();
	setLeftIndent();
	setRightIndent();
	m_isSetupFirst = true;
}

void SMTabruler::leftValueChanged()
{
//	QSignalBlocker firstLineDataBlocker(firstLineData);
//	QSignalBlocker rightIndentDataBlocker(rightIndentData);
	QSignalBlocker leftIndentDataBlocker(leftIndentData);

	double a, b, value;
	int c;
	leftIndentSpin->getValues(&a, &b, &c, &value);
	setLeftIndentData(value / m_docUnitRatio);
	setLeftIndent();
//	setFirstLine();
	setRightIndent();
	m_isSetupLeft = true;
}

void SMTabruler::rightValueChanged()
{
	QSignalBlocker firstLineDataBlocker(firstLineData);
	QSignalBlocker rightIndentDataBlocker(rightIndentData);
	QSignalBlocker leftIndentDataBlocker(leftIndentData);

	double a, b, value;
	int c;
	rightIndentSpin->getValues(&a, &b, &c, &value);
	setRightIndentData(value / m_docUnitRatio);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	m_isSetupRight = true;
}
