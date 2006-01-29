/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "marginWidget.h"
#include "marginWidget.moc"

#include <qcheckbox.h>
#include <qtooltip.h>

#include "mspinbox.h"
#include "units.h"
#include "useprintermarginsdialog.h"


MarginWidget::MarginWidget( QWidget* parent, QString title, MarginStruct* margs, int unitIndex, bool showChangeAll) : QGroupBox( title, parent, "marginWidget")
{
	setColumnLayout(0, Qt::Vertical );
	layout()->setSpacing( 5 );
	layout()->setMargin( 10 );
	RandT = margs->Top;
	RandB = margs->Bottom;
	RandR = margs->Right;
	RandL = margs->Left;
	
	m_docUnitIndex=unitIndex;
	m_unitRatio = unitGetRatioFromIndex(unitIndex);
	m_suffix = unitGetSuffixFromIndex(unitIndex);
	int decimals = unitGetDecimalsFromIndex(unitIndex);
	
	presetCombo = new PresetLayout(this, "presetCombo");
	presetLabel = new QLabel(presetCombo, tr("Preset Layouts:"), this, "presetLabel");

	leftR = new MSpinBox( 0, 1000, this, decimals );
	leftR->setSuffix( m_suffix );
	leftR->setValue(RandL * m_unitRatio);

	rightR = new MSpinBox( 0, 1000, this, decimals );
	rightR->setSuffix( m_suffix );
	rightR->setValue(RandR * m_unitRatio);

	topR = new MSpinBox( 0, 1000, this, decimals );
	topR->setSuffix( m_suffix );
	topR->setValue(RandT * m_unitRatio);

	bottomR = new MSpinBox( 0, 1000, this, decimals );
	bottomR->setSuffix( m_suffix );
	bottomR->setValue(RandB * m_unitRatio);

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
	if (showChangeAll)
	{
		marginsForAllPages = new QCheckBox( this, "moveObjects" );
		marginsForAllPages->setText( tr( "Apply margin settings to all pages" ) );
		marginsForAllPages->setChecked( false );
		//GroupLayout->addMultiCellWidget( marginsForAllPages, 5, 5, 0, 1);
		GroupLayout->addWidget( marginsForAllPages, 5, 0 );
		QToolTip::add( marginsForAllPages, "<qt>" + tr( "Apply the margin changes to all existing pages in the document" ) + "</qt>" );
		
	}
	else
		marginsForAllPages=NULL;
	usePrinterMarginsButton=NULL;
	#ifdef HAVE_CUPS
	usePrinterMarginsButton=new QPushButton( tr("Printer Margins..."),this, "usePrinterMarginsButton" );
	GroupLayout->addWidget( usePrinterMarginsButton, 5, 1 );
	#endif
	// hints
	QToolTip::add( topR, "<qt>" + tr( "Distance between the top margin guide and the edge of the page" ) + "</qt>");
	QToolTip::add( bottomR, "<qt>" + tr( "Distance between the bottom margin guide and the edge of the page" ) + "</qt>");
	QToolTip::add( leftR, "<qt>" + tr( "Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding") + "</qt>");
	QToolTip::add( rightR, "<qt>" +tr( "Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding") + "</qt>");
	QToolTip::add( usePrinterMarginsButton, "<qt>" +tr( "Import the margins from the selected printer." ) + "</qt>");
		// signals&slots
	connect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight()));
	connect(presetCombo, SIGNAL(activated(int)), this, SLOT(setPreset()));
	connect(usePrinterMarginsButton, SIGNAL(clicked()), this, SLOT(setMarginsToPrinterMargins()));
}

void MarginWidget::setFacingPages(bool facing)
{
	lText->setText(facing == true ? tr( "&Inside:" ) : tr( "&Left:" ));
	rText->setText(facing == true ? tr( "O&utside:" ) : tr( "&Right:" ));
	setPreset();
}

void MarginWidget::setPageWidthHeight(double width, double height)
{
	rightR->setMaxValue(width * m_unitRatio - leftR->value());
	leftR->setMaxValue(width * m_unitRatio - rightR->value());
	pageWidth = width;
	topR->setMaxValue(height * m_unitRatio - bottomR->value());
	bottomR->setMaxValue(height * m_unitRatio - topR->value());
	pageHeight = height;
	setPreset();
}

void MarginWidget::setPageWidth(double width)
{
	rightR->setMaxValue(width * m_unitRatio - leftR->value());
	leftR->setMaxValue(width * m_unitRatio - rightR->value());
	pageWidth = width;
	setPreset();
}

void MarginWidget::setPageHeight(double height)
{
	topR->setMaxValue(height * m_unitRatio - bottomR->value());
	bottomR->setMaxValue(height * m_unitRatio - topR->value());
	pageHeight = height;
	setPreset();
}

void MarginWidget::setTop()
{
	RandT = topR->value() / m_unitRatio;
	bottomR->setMaxValue(pageHeight * m_unitRatio - topR->value());
	setPreset();
}

void MarginWidget::setBottom()
{
	RandB = bottomR->value() / m_unitRatio;
	topR->setMaxValue(pageHeight * m_unitRatio - bottomR->value());
	setPreset();
}

void MarginWidget::setLeft()
{
	RandL = leftR->value() / m_unitRatio;
	rightR->setMaxValue(pageWidth * m_unitRatio - leftR->value());
	setPreset();
}

void MarginWidget::setRight()
{
	RandR = rightR->value() / m_unitRatio;
	leftR->setMaxValue(pageWidth * m_unitRatio - rightR->value());
	setPreset();
}

void MarginWidget::unitChange(double newUnit, int newDecimals, QString newSuffix)
{
	disconnect(topR, SIGNAL(valueChanged(int)), this, SLOT(setTop()));
	disconnect(bottomR, SIGNAL(valueChanged(int)), this, SLOT(setBottom()));
	disconnect(leftR, SIGNAL(valueChanged(int)), this, SLOT(setLeft()));
	disconnect(rightR, SIGNAL(valueChanged(int)), this, SLOT(setRight()));
	int decimalsOld;
	double oldUnitRatio = m_unitRatio;
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
	m_unitRatio = newUnit;
	m_suffix=newSuffix;
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
	MarginStruct marg = presetCombo->getMargins(item, pageWidth * m_unitRatio, pageHeight * m_unitRatio, leftR->value());
	if (presetCombo->needUpdate())
	{
		leftR->setValue(marg.Left);
		rightR->setValue(marg.Right);
		topR->setValue(marg.Top);
		bottomR->setValue(marg.Bottom);
		RandT = topR->value() / m_unitRatio;
		RandB = bottomR->value() / m_unitRatio;
		RandL = leftR->value() / m_unitRatio;
		RandR = rightR->value() / m_unitRatio;
		bottomR->setMaxValue(pageHeight * m_unitRatio - topR->value());
		topR->setMaxValue(pageHeight * m_unitRatio - bottomR->value());
		rightR->setMaxValue(pageWidth * m_unitRatio - leftR->value());
		leftR->setMaxValue(pageWidth * m_unitRatio - rightR->value());
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

void MarginWidget::setPageSize(const QString& pageSize)
{
	m_pageSize=pageSize;
}

void MarginWidget::setMarginsToPrinterMargins()
{
	UsePrinterMarginsDialog upm(parentWidget(), m_pageSize, m_unitRatio, m_suffix);
	if (upm.exec())
	{
		double t,b,l,r;
		upm.getNewPrinterMargins(t,b,l,r);
		presetCombo->setCurrentItem(PresetLayout::none);
		topR->setValue(t * m_unitRatio);
		bottomR->setValue(b * m_unitRatio);
		leftR->setValue(l * m_unitRatio);
		rightR->setValue(r * m_unitRatio);
		
		RandT = t;
		RandB = b;
		RandL = l;
		RandR = r;
		
		bottomR->setMaxValue((pageHeight - t) * m_unitRatio);
		topR->setMaxValue((pageHeight - b) * m_unitRatio);
		rightR->setMaxValue((pageWidth - l) * m_unitRatio);
		leftR->setMaxValue((pageWidth - r) * m_unitRatio);
		
		rightR->setEnabled(true);
		topR->setEnabled(true);
		bottomR->setEnabled(true);
	}
}


/*
 * presets
 */
PresetLayout::PresetLayout(QWidget *parent, const char * name) : QComboBox(parent, name)
{
	insertItem(tr("None"), PresetLayout::none);
	insertItem(tr("Gutenberg"), PresetLayout::gutenberg);
	insertItem(tr("Magazine"), PresetLayout::magazine);
	insertItem(tr("Fibonacci"), PresetLayout::fibonacci);
	insertItem(tr("Golden Mean"), PresetLayout::goldencut);
	insertItem(tr("Nine Parts"), PresetLayout::nineparts);
	setCurrentItem(PresetLayout::none);

	QToolTip::add(this, "<qt>" +tr("You can select predefined page layout here. 'None' leave margins as is, Gutenberg sets margins classically. 'Magazine' sets all margins for same value. Leading is Left/Inside value.") + "</qt>");
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
		case PresetLayout::gutenberg:
			{
				double ratio = pageHeight / pageWidth;
				ret.Left = leftMargin;
				ret.Top = leftMargin * ratio;
				ret.Right = leftMargin * 2.0;
				ret.Bottom = ret.Right * ratio;
			}
			break;
		case PresetLayout::fibonacci:
			ret.Left = leftMargin;
			ret.Top = leftMargin / 2.0 * 3.0;
			ret.Right = leftMargin / 2.0 * 5.0;
			ret.Bottom = leftMargin / 2.0 * 8.0;
			break;
		case PresetLayout::goldencut:
			ret.Left = leftMargin;
			ret.Top = leftMargin / 2.0 * 3.4;
			ret.Right = leftMargin / 2.0 * 4.8;
			ret.Bottom = leftMargin / 2.0 * 6.8;
			break;
		case PresetLayout::nineparts:
			ret.Left = pageWidth / 9.0;
			ret.Top = pageHeight / 9.0;
			ret.Right = pageWidth / 9.0 * 2.0;
			ret.Bottom = pageHeight / 9.0 * 2.0;
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
