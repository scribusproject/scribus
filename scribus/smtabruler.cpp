/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QToolButton>

#include "smtabruler.h"
#include "smscrspinbox.h"
#include "units.h"


SMTabruler::SMTabruler(QWidget* parent, bool haveFirst, int dEin,
					   QList<ParagraphStyle::TabRecord> Tabs, double wid)
: Tabruler(parent, haveFirst, dEin, Tabs, wid)
{
	parentButton_ = new QToolButton(this);
	Q_CHECK_PTR(parentButton_);
	parentButton_->setText( tr(" Parent Tabs "));
	indentLayout->addWidget(parentButton_);
	parentButton_->hide();
	QFont f(font());
	f.setBold(true);
	parentButton_->setFont(f);
	connect(parentButton_, SIGNAL(clicked()), this, SLOT(pbClicked()));
	hasParent_ = false;
	tabsChanged_ = false;
	useParentTabs_ = false;
	first_ = new SMScrSpinBox(-3000, 4000, this, dEin);
	Q_CHECK_PTR(first_);
	left_ = new SMScrSpinBox(0, 4000, this, dEin);
	Q_CHECK_PTR(left_);
	right_ = new SMScrSpinBox(0, 4000, this, dEin);
	Q_CHECK_PTR(right_);

	indentLayout->removeWidget(firstLineData);
	firstLineData->hide();
	indentLayout->insertWidget(1, first_);
	first_->show();

	layout4->removeWidget(leftIndentData);
	leftIndentData->hide();
	layout4->insertWidget(1, left_);
	left_->show();

	indentLayout->removeWidget(rightIndentData);
	rightIndentData->hide();
	indentLayout->insertWidget(3, right_);
	right_->show();

	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));

	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::unitChange(int unitIndex)
{
	this->blockSignals(true);
	Tabruler::setTabs(ruler->tabValues, unitIndex);
	Tabruler::repaint();

	first_->blockSignals(true);
	left_->blockSignals(true);
	right_->blockSignals(true);
	tabData->blockSignals(true);

	first_->setNewUnit(unitIndex);
	left_->setNewUnit(unitIndex);
	right_->setNewUnit(unitIndex);
	tabData->setNewUnit(unitIndex);

	first_->blockSignals(false);
	left_->blockSignals(false);
	right_->blockSignals(false);
	tabData->blockSignals(false);

	m_unitIndex = unitIndex;
	this->blockSignals(false);
}

void SMTabruler::setTabs(QList<ParagraphStyle::TabRecord> Tabs, int unitIndex)
{
	disconnect(this, SIGNAL(tabsChanged()), this, SLOT(slotTabsChanged()));
	disconnect(this, SIGNAL(mouseReleased()), this, SLOT(slotTabsChanged()));
	hasParent_ = false;
	parentButton_->hide();
	Tabruler::setTabs(Tabs, unitIndex);
	Tabruler::repaint();
	first_->setNewUnit(unitIndex);
	left_->setNewUnit(unitIndex);
	right_->setNewUnit(unitIndex);
	tabData->setNewUnit(unitIndex);
}

void SMTabruler::setTabs(QList<ParagraphStyle::TabRecord> Tabs, int unitIndex, bool isParentValue)
{
	disconnect(this, SIGNAL(tabsChanged()), this, SLOT(slotTabsChanged()));
	disconnect(this, SIGNAL(mouseReleased()), this, SLOT(slotTabsChanged()));
	hasParent_  = true;
	m_unitIndex = unitIndex;
	if (isParentValue)
		parentButton_->hide();
	else
		parentButton_->show();
	Tabruler::setTabs(Tabs, unitIndex);
	Tabruler::repaint();
	first_->setNewUnit(unitIndex);
	left_->setNewUnit(unitIndex);
	right_->setNewUnit(unitIndex);
	tabData->setNewUnit(unitIndex);

	connect(this, SIGNAL(tabsChanged()), this, SLOT(slotTabsChanged()));
	connect(this, SIGNAL(mouseReleased()), this, SLOT(slotTabsChanged()));
}

void SMTabruler::setParentTabs(QList<ParagraphStyle::TabRecord> Tabs)
{
	hasParent_ = true;
	pTabs_ = Tabs;
}

void SMTabruler::setFirstLineValue(double t)
{
	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
	first_->setValue(t);
	firstLineData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setFirstLineValue(double t, bool isParentValue)
{
	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
	isSetupFirst_ = true;
	first_->setValue(t, isParentValue);
	firstLineData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setParentFirstLine(double t)
{
	first_->setParentValue(t);
}

void SMTabruler::setLeftIndentValue(double t)
{
	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
	left_->setValue(t);
	leftIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setLeftIndentValue(double t, bool isParentValue)
{
	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
	isSetupLeft_ = true;
	left_->setValue(t, isParentValue);
	leftIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setParentLeftIndent(double t)
{
	left_->setParentValue(t);
}

void SMTabruler::setRightIndentValue(double t)
{
	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
	right_->setValue(t);
	rightIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setRightIndentValue(double t, bool isParentValue)
{
	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	disconnect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
	isSetupRight_ = true;
	right_->setValue(t, isParentValue);
	rightIndentData->setValue(t);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::setParentRightIndent(double t)
{
	right_->setParentValue(t);
}

bool SMTabruler::useParentTabs()
{
	bool ret = useParentTabs_;
	if (ret && hasParent_)
	{
		setTabs(pTabs_, m_unitIndex, true);
		Tabruler::repaint();
		parentButton_->hide();
	}
	else if (hasParent_)
	{
		parentButton_->show();
	}

	return ret;
}

bool SMTabruler::useParentFirstLine()
{
	return first_->useParentValue();
}

bool SMTabruler::useParentLeftIndent()
{
	return left_->useParentValue();
}

bool SMTabruler::useParentRightIndent()
{
	return right_->useParentValue();
}

void SMTabruler::slotTabsChanged()
{
	if (hasParent_)
	{
		useParentTabs_ = false;
		tabsChanged_ = true;
	}
}

void SMTabruler::pbClicked()
{
	if (hasParent_)
	{
		useParentTabs_ = true;
		emit mouseReleased();
	}
}

void SMTabruler::leftDataChanged()
{
	disconnect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
	double a, b, value;
	int c;
	leftIndentData->getValues(&a, &b, &c, &value);
	if (hasParent_ && !isSetupLeft_)
		left_->setValue(value, false);
	else if (!hasParent_)
		left_->setValue(value);

	isSetupLeft_ = false;
	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::rightDataChanged()
{
	disconnect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
	double a, b, value;
	int c;
	rightIndentData->getValues(&a, &b, &c, &value);
	if (hasParent_ && !isSetupRight_)
		right_->setValue(value, false);
	else if (!hasParent_)
		right_->setValue(value);

	isSetupRight_ = false;
	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::firstDataChanged()
{
	disconnect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	disconnect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	disconnect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
	double a, b, value;
	int c;
	firstLineData->getValues(&a, &b, &c, &value);
	if (hasParent_ && !isSetupFirst_)
		first_->setValue(value, false);
	else if (!hasParent_)
		first_->setValue(value);

	isSetupFirst_ = false;
	connect(first_, SIGNAL(valueChanged(double)), this, SLOT(firstValueChanged()));
	connect(right_, SIGNAL(valueChanged(double)), this, SLOT(rightValueChanged()));
	connect(left_, SIGNAL(valueChanged(double)), this, SLOT(leftValueChanged()));
}

void SMTabruler::firstValueChanged()
{
//	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
//	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
//	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	double a, b, value;
	int c;
	first_->getValues(&a, &b, &c, &value);
	setFirstLineData(value / docUnitRatio);
	setFirstLine();
	setLeftIndent();
	setRightIndent();
	isSetupFirst_ = true;
//	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
//	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
//	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
}

void SMTabruler::leftValueChanged()
{
//	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
//	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	double a, b, value;
	int c;
	left_->getValues(&a, &b, &c, &value);
	setLeftIndentData(value / docUnitRatio);
	setLeftIndent();
//	setFirstLine();
	setRightIndent();
	isSetupLeft_ = true;
//	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
//	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
}

void SMTabruler::rightValueChanged()
{
	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
	double a, b, value;
	int c;
	right_->getValues(&a, &b, &c, &value);
	setRightIndentData(value / docUnitRatio);
	setLeftIndent();
	setFirstLine();
	setRightIndent();
	isSetupRight_ = true;
	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(firstDataChanged()));
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(rightDataChanged()));
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(leftDataChanged()));
}
