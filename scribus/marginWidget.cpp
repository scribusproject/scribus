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

	presetCombo = new PresetLayout(this, "presetCombo");
	presetLabel = new QLabel(presetCombo, tr("Preset Layouts:"), this, "presetLabel");

	leftR = new MSpinBox( 0, 1000, this, decimals );
	leftR->setSuffix( einh );
	leftR->setValue(RandL * unitRatio);

	rightR = new MSpinBox( 0, 1000, this, decimals );
	rightR->setSuffix( einh );
	rightR->setValue(RandR * unitRatio);

	topR = new MSpinBox( 0, 1000, this, decimals );
	topR->setSuffix( einh );
	topR->setValue(RandT * unitRatio);

	bottomR = new MSpinBox( 0, 1000, this, decimals );
	bottomR->setSuffix( einh );
	bottomR->setValue(RandB * unitRatio);

	bText = new QLabel( bottomR, tr( "&Bottom:" ), this, "bText" );
	tText = new QLabel( topR, tr( "&Top:" ), this, "tText" );
	rText = new QLabel( rightR, tr( "&Right:" ), this, "rText" );
	lText = new QLabel( leftR, tr( "&Left:" ), this, "lText" );
	// layout
	GroupLayout = new QGridLayout( this->layout() );
	GroupLayout->setAlignment( Qt::AlignTop );
	GroupLayout->addWidget(presetLabel, 0, 0);
	GroupLayout->addWidget(presetCombo, 0, 1);
	GroupLayout->addWidget( leftR, 1, 1 );
	GroupLayout->addWidget( rightR, 2, 1 );
	GroupLayout->addWidget( topR, 3, 1 );
	GroupLayout->addWidget( bottomR, 4, 1 );
	GroupLayout->addWidget( lText, 1, 0 );
	GroupLayout->addWidget( rText, 2, 0 );
	GroupLayout->addWidget( tText, 3, 0 );
	GroupLayout->addWidget( bText, 4, 0 );

	// hints
	QToolTip::add( topR, "<qt>" + tr( "Distance between the top margin guide and the edge of the page" ) + "</qt>");
	QToolTip::add( bottomR, "<qt>" + tr( "Distance between the bottom margin guide and the edge of the page" ) + "</qt>");
	QToolTip::add( leftR, "<qt>" + tr( "Distance between the left margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding") + "</qt>");
	QToolTip::add( rightR, "<qt>" +tr( "Distance between the right margin guide and the edge of the page.\nIf Facing Pages is selected, this margin space can be used to achieve the correct margins for binding") + "</qt>");
	// signals&slots
	connect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight()));
	connect(presetCombo, SIGNAL(activated(int)), this, SLOT(setPreset()));
}

void MarginWidget::setFacingPages(bool facing)
{
	lText->setText(facing == true ? tr( "&Inside:" ) : tr( "&Left:" ));
	rText->setText(facing == true ? tr( "O&utside:" ) : tr( "&Right:" ));
	setPreset();
}

void MarginWidget::setPageWidthHeight(double width, double height)
{
	rightR->setMaxValue(width * unitRatio - leftR->value());
	leftR->setMaxValue(width * unitRatio - rightR->value());
	pageWidth = width;
	topR->setMaxValue(height * unitRatio - bottomR->value());
	bottomR->setMaxValue(height * unitRatio - topR->value());
	pageHeight = height;
	setPreset();
}

void MarginWidget::setPageWidth(double width)
{
	rightR->setMaxValue(width * unitRatio - leftR->value());
	leftR->setMaxValue(width * unitRatio - rightR->value());
	pageWidth = width;
	setPreset();
}

void MarginWidget::setPageHeight(double height)
{
	topR->setMaxValue(height * unitRatio - bottomR->value());
	bottomR->setMaxValue(height * unitRatio - topR->value());
	pageHeight = height;
	setPreset();
}

void MarginWidget::setTop()
{
	RandT = topR->value() / unitRatio;
	bottomR->setMaxValue(pageHeight * unitRatio - topR->value());
	setPreset();
}

void MarginWidget::setBottom()
{
	RandB = bottomR->value() / unitRatio;
	topR->setMaxValue(pageHeight * unitRatio - bottomR->value());
	setPreset();
}

void MarginWidget::setLeft()
{
	RandL = leftR->value() / unitRatio;
	rightR->setMaxValue(pageWidth * unitRatio - leftR->value());
	setPreset();
}

void MarginWidget::setRight()
{
	RandR = rightR->value() / unitRatio;
	leftR->setMaxValue(pageWidth * unitRatio - rightR->value());
	setPreset();
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

void MarginWidget::setPreset()
{
	disconnect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop()));
	disconnect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom()));
	disconnect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft()));
	disconnect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight()));
	int item = presetCombo->currentItem();
	MarginStruct marg = presetCombo->getMargins(item, pageWidth, pageHeight, leftR->value());
	if (presetCombo->needUpdate())
	{
		leftR->setValue(marg.Left);
		rightR->setValue(marg.Right);
		topR->setValue(marg.Top);
		bottomR->setValue(marg.Bottom);
		RandT = topR->value() / unitRatio;
		RandB = bottomR->value() / unitRatio;
		RandL = leftR->value() / unitRatio;
		RandR = rightR->value() / unitRatio;
		bottomR->setMaxValue(pageHeight * unitRatio - topR->value());
		topR->setMaxValue(pageHeight * unitRatio - bottomR->value());
		rightR->setMaxValue(pageWidth * unitRatio - leftR->value());
		leftR->setMaxValue(pageWidth * unitRatio - rightR->value());
		rightR->setEnabled(false);
		topR->setEnabled(false);
		bottomR->setEnabled(false);
	}
	else
	{
		rightR->setEnabled(true);
		topR->setEnabled(true);
		bottomR->setEnabled(true);
	}
	connect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight()));
}

/*
 * presets
 */
PresetLayout::PresetLayout(QWidget *parent, const char * name) : QComboBox(parent, name)
{
	insertItem(tr("None"), PresetLayout::none);
	insertItem(tr("Book"), PresetLayout::book);
	insertItem(tr("Magazine"), PresetLayout::magazine);
	setCurrentItem(PresetLayout::none);

	QToolTip::add(this, "<qt>" +tr("You can select predefined page layout here. 'None' leave margins as is, 'Book' sets margins classically (Gutenberg). 'Book' is proposed for two-sided documents. 'Magazine' sets all margins for same value. Leading is Left/Inside value.") + "</qt>");
}

MarginStruct PresetLayout::getMargins(int index, double pageWidth, double pageHeight, double leftMargin)
{
	MarginStruct ret;

	updateMargins = true;

	switch (index)
	{
		case PresetLayout::magazine:
			ret.Top = ret.Bottom = ret.Left = ret.Right = leftMargin;
			break;
		case PresetLayout::book:
			{
				double ratio = pageHeight / pageWidth;
				ret.Left = leftMargin;
				ret.Top = leftMargin * ratio;
				ret.Right = leftMargin * 2.0;
				ret.Bottom = ret.Right * ratio;
			}
			break;
		default:
			updateMargins = false;
			ret.Top = ret.Bottom = ret.Left = ret.Right = -1.0;
	}
	return ret;
}

bool PresetLayout::needUpdate()
{
	return updateMargins;
}
