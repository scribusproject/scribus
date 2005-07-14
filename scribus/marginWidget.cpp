#include "marginWidget.h"
#include "marginWidget.moc"
#include "mspinbox.h"
#include <qtooltip.h>


MarginWidget::MarginWidget( QWidget* parent, QString title, MarginStruct* margs, int decimals, double unit, QString einh) : QGroupBox( title, parent, "marginWidget")
{
	setColumnLayout(0, Qt::Vertical );
	layout()->setSpacing( 5 );
	layout()->setMargin( 10 );
	RandT = margs->Top;
	RandB = margs->Bottom;
	RandR = margs->Right;
	RandL = margs->Left;
	unitRatio = unit;
	GroupLayout = new QGridLayout( this->layout() );
	GroupLayout->setAlignment( Qt::AlignTop );
	topR = new MSpinBox( 0, 1000, this, decimals );
	topR->setSuffix( einh );
	topR->setValue(RandT * unitRatio);
	GroupLayout->addWidget( topR, 0, 1 );
	bottomR = new MSpinBox( 0, 1000, this, decimals );
	bottomR->setSuffix( einh );
	bottomR->setValue(RandB * unitRatio);
	GroupLayout->addWidget( bottomR, 1, 1 );
	leftR = new MSpinBox( 0, 1000, this, decimals );
	leftR->setSuffix( einh );
	leftR->setValue(RandL * unitRatio);
	GroupLayout->addWidget( leftR, 0, 3 );
	rightR = new MSpinBox( 0, 1000, this, decimals );
	rightR->setSuffix( einh );
	rightR->setValue(RandR * unitRatio);
	GroupLayout->addWidget( rightR, 1, 3 );	
	GRText2 = new QLabel( bottomR, tr( "&Bottom:" ), this, "GRText2" );
	GroupLayout->addWidget( GRText2, 1, 0 );
	GRText1 = new QLabel( topR, tr( "&Top:" ), this, "GRText1" );
	GroupLayout->addWidget( GRText1, 0, 0 );
	GRText4 = new QLabel( rightR, tr( "&Right:" ), this, "GRText4" );
	GroupLayout->addWidget( GRText4, 1, 2 );
	GRText3 = new QLabel( leftR, tr( "&Left:" ), this, "GRText3" );
	GroupLayout->addWidget( GRText3, 0, 2 );
	QToolTip::add( topR, tr( "Distance between the top margin guide and the edge of the page" ) );
	QToolTip::add( bottomR, tr( "Distance between the bottom margin guide and the edge of the page" ) );
	QToolTip::add( leftR, tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	QToolTip::add( rightR, tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding" ) );
	connect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight()));
}

void MarginWidget::setFacingPages(bool facing)
{
	GRText3->setText(facing == true ? tr( "&Inside:" ) : tr( "&Left:" ));
	GRText4->setText(facing == true ? tr( "O&utside:" ) : tr( "&Right:" ));
}

void MarginWidget::setPageWidth(double width)
{
	rightR->setMaxValue(width * unitRatio - leftR->value());
	leftR->setMaxValue(width * unitRatio - rightR->value());
	pageWidth = width;
}

void MarginWidget::setPageHeight(double height)
{
	topR->setMaxValue(height * unitRatio - bottomR->value());
	bottomR->setMaxValue(height * unitRatio - topR->value());
	pageHeight = height;
}

void MarginWidget::setTop()
{
	RandT = topR->value() / unitRatio;
	bottomR->setMaxValue(pageHeight * unitRatio - topR->value());
}

void MarginWidget::setBottom()
{
	RandB = bottomR->value() / unitRatio;
	topR->setMaxValue(pageHeight * unitRatio - bottomR->value());
}

void MarginWidget::setLeft()
{
	RandL = leftR->value() / unitRatio;
	rightR->setMaxValue(pageWidth * unitRatio - leftR->value());
}

void MarginWidget::setRight()
{
	RandR = rightR->value() / unitRatio;
	leftR->setMaxValue(pageWidth * unitRatio - rightR->value());
}

void MarginWidget::unitChange(double newUnit, int newDecimals, QString newSuffix)
{
	disconnect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop()));
	disconnect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom()));
	disconnect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft()));
	disconnect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight()));
	int decimalsOld;
	double oldUnitRatio = unitRatio;
	double oldMin, oldMax, val;
	topR->setSuffix(newSuffix);
	bottomR->setSuffix(newSuffix);
	leftR->setSuffix(newSuffix);
	rightR->setSuffix(newSuffix);
	double invUnitConversion = 1.0 / oldUnitRatio * newUnit;
	topR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	topR->setValues(0, oldMax * invUnitConversion, newDecimals, val * invUnitConversion);
	bottomR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	bottomR->setValues(0, oldMax * invUnitConversion, newDecimals, val * invUnitConversion);
	leftR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	leftR->setValues(0, oldMax * invUnitConversion, newDecimals, val * invUnitConversion);
	rightR->getValues(&oldMin, &oldMax, &decimalsOld, &val);
	rightR->setValues(0, oldMax * invUnitConversion, newDecimals, val * invUnitConversion);
	unitRatio = newUnit;
	connect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight()));
}
