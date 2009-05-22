/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "marginwidget.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QToolTip>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>

#include "scrspinbox.h"
#include "units.h"
#include "ui/useprintermarginsdialog.h"


MarginWidget::MarginWidget( QWidget* parent, QString /*title*/, const MarginStruct* margs, int unitIndex, bool showChangeAll, bool showBleeds) : QTabWidget(parent),
pageType(0)
{
	marginData=*margs;
	savedMargins=*margs;
	savedPresetItem=PresetLayout::none;//we dont recheck if we are using a layout but always start at none
	facingPages = false;
	useBleeds = showBleeds;
	marginPage = new QWidget(this);

	presetCombo = new PresetLayout(marginPage);
	presetLabel = new QLabel( tr("Preset Layouts:"), marginPage);
	presetLabel->setBuddy(presetCombo);
	m_unitIndex=unitIndex;
	m_unitRatio=unitGetRatioFromIndex(unitIndex);
	leftR = new ScrSpinBox( 0, 1000, marginPage, unitIndex );
	rightR = new ScrSpinBox( 0, 1000, marginPage, unitIndex );
	topR = new ScrSpinBox( 0, 1000, marginPage, unitIndex );
	bottomR = new ScrSpinBox( 0, 1000, marginPage, unitIndex );
	updateMarginSpinValues();

	bText = new QLabel( tr( "&Bottom:" ), marginPage);
	bText->setBuddy(bottomR);
	tText = new QLabel( tr( "&Top:" ), marginPage);
	tText->setBuddy(topR);
	rText = new QLabel( tr( "&Right:" ), marginPage);
	rText->setBuddy(rightR);
	lText = new QLabel( tr( "&Left:" ), marginPage);
	lText->setBuddy(leftR);

	linkMargins = new LinkButton( marginPage );
	linkMargins->setCheckable( true );
	linkMargins->setChecked(false);
	linkMargins->setAutoRaise( true );
	linkMargins->setMinimumSize( QSize( 15, 0 ) );
	linkMargins->setMaximumSize( QSize( 15, 32767 ) );

	// layout
	GroupLayout = new QGridLayout( marginPage );
	GroupLayout->setSpacing( 5 );
	GroupLayout->setMargin( 10 );
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
	GroupLayout->addWidget( linkMargins, 1, 2, 4, 1 );
	if (showChangeAll)
	{
		marginsForPagesLayout = new QHBoxLayout;
		marginsForPagesLayout->setMargin(5);
		marginsForPagesLayout->setSpacing(5);
		marginsForPages = new QLabel( tr( "Apply settings to:" ), marginPage );
		marginsForPagesLayout->addWidget(marginsForPages);
		marginsForAllPages = new QCheckBox( marginPage );
		marginsForAllPages->setText( tr( "All Document Pages" ) );
		marginsForAllPages->setChecked( false );
		marginsForPagesLayout->addWidget(marginsForAllPages);
		marginsForAllMasterPages = new QCheckBox( marginPage );
		marginsForAllMasterPages->setText( tr( "All Master Pages" ) );
		marginsForAllMasterPages->setChecked( false );
		marginsForPagesLayout->addWidget(marginsForAllMasterPages);
		GroupLayout->addLayout( marginsForPagesLayout, 6, 0, 1, 2 );
		marginsForAllPages->setToolTip( "<qt>" + tr( "Apply the margin changes to all existing pages in the document" ) + "</qt>" );
		marginsForAllMasterPages->setToolTip( "<qt>" + tr( "Apply the margin changes to all existing master pages in the document" ) + "</qt>" );
	}
	else
	{
		marginsForPages=NULL;
		marginsForAllPages=NULL;
		marginsForAllMasterPages=NULL;
	}

	usePrinterMarginsButton=NULL;
#if defined(HAVE_CUPS) || defined(_WIN32)
	usePrinterMarginsButton=new QPushButton( tr("Printer Margins..."),marginPage );
	GroupLayout->addWidget( usePrinterMarginsButton, 5, 1 );
	usePrinterMarginsButton->setToolTip( "<qt>" + tr( "Import the margins for the selected page size from the available printers." ) + "</qt>");
	connect(usePrinterMarginsButton, SIGNAL(clicked()), this, SLOT(setMarginsToPrinterMargins()));
#endif

	addTab(marginPage, tr("Margin Guides"));

	if (useBleeds)
	{
		bleedPage = new QWidget(this);
		BleedGroupLayout = new QGridLayout( bleedPage );
		BleedGroupLayout->setSpacing( 5 );
		BleedGroupLayout->setMargin( 10 );
		BleedGroupLayout->setAlignment( Qt::AlignTop );
		BleedTxt3 = new QLabel( bleedPage );
		BleedGroupLayout->addWidget( BleedTxt3, 0, 0 );
		BleedLeft = new ScrSpinBox( 0, 3000*m_unitRatio, bleedPage, unitIndex );
		BleedGroupLayout->addWidget( BleedLeft, 0, 1 );
		BleedTxt4 = new QLabel( bleedPage );
		BleedGroupLayout->addWidget( BleedTxt4, 1, 0 );
		BleedRight = new ScrSpinBox( 0, 3000*m_unitRatio, bleedPage, unitIndex );
		BleedGroupLayout->addWidget( BleedRight, 1, 1 );
		BleedTxt1 = new QLabel( bleedPage );
		BleedTxt1->setText( tr( "Top:" ) );
		BleedGroupLayout->addWidget( BleedTxt1, 2, 0 );
		BleedTop = new ScrSpinBox( 0, 3000*m_unitRatio, bleedPage, unitIndex );
		BleedGroupLayout->addWidget( BleedTop, 2, 1 );
		BleedTxt2 = new QLabel( bleedPage );
		BleedTxt2->setText( tr( "Bottom:" ) );
		BleedGroupLayout->addWidget( BleedTxt2, 3, 0 );
		BleedBottom = new ScrSpinBox( 0, 3000*m_unitRatio, bleedPage, unitIndex );
		BleedGroupLayout->addWidget( BleedBottom, 3, 1 );
		linkBleeds = new LinkButton( bleedPage );
/** Fix Me to move this to the prefsmanager.cpp someday */		
		linkBleeds->setCheckable( true );
		linkBleeds->setChecked(true);
		linkBleeds->setAutoRaise( true );
		linkBleeds->setMinimumSize( QSize( 15, 0 ) );
		linkBleeds->setMaximumSize( QSize( 15, 32767 ) );
		BleedGroupLayout->addWidget( linkBleeds, 0, 2, 4, 1 );
		BleedTop->setToolTip( "<qt>" + tr( "Distance for bleed from the top of the physical page" ) + "</qt>" );
		BleedBottom->setToolTip( "<qt>" + tr( "Distance for bleed from the bottom of the physical page" ) + "</qt>" );
		BleedLeft->setToolTip( "<qt>" + tr( "Distance for bleed from the left of the physical page" ) + "</qt>" );
		BleedRight->setToolTip( "<qt>" + tr( "Distance for bleed from the right of the physical page" )  + "</qt>");
		connect(linkBleeds, SIGNAL(clicked()), this, SLOT(slotLinkBleeds()));
		connect(BleedLeft, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		connect(BleedRight, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		connect(BleedTop, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		connect(BleedBottom, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		addTab(bleedPage, tr("Bleeds"));
	}

	// hints
	topR->setToolTip( "<qt>" + tr( "Distance between the top margin guide and the edge of the page" ) + "</qt>");
	bottomR->setToolTip( "<qt>" + tr( "Distance between the bottom margin guide and the edge of the page" ) + "</qt>");
	leftR->setToolTip( "<qt>" + tr( "Distance between the left margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding") + "</qt>");
	rightR->setToolTip( "<qt>" + tr( "Distance between the right margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding") + "</qt>");

		// signals&slots
	connect(topR, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
	connect(presetCombo, SIGNAL(activated(int)), this, SLOT(setPreset()));
	connect(linkMargins, SIGNAL(clicked()), this, SLOT(slotLinkMargins()));
}

void MarginWidget::slotLinkBleeds()
{
	disconnect(BleedLeft, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	disconnect(BleedRight, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	disconnect(BleedTop, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	disconnect(BleedBottom, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	if (linkBleeds->isChecked())
	{
		BleedTop->setValue(BleedLeft->value());
		BleedBottom->setValue(BleedLeft->value());
		BleedRight->setValue(BleedLeft->value());
	}
	connect(BleedLeft, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	connect(BleedRight, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	connect(BleedTop, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	connect(BleedBottom, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
}

void MarginWidget::changeBleeds()
{
	if (linkBleeds->isChecked())
	{
		double val = 0.0;
		if (BleedTop == sender())
			val = BleedTop->value();
		else if (BleedBottom == sender())
			val = BleedBottom->value();
		else if (BleedLeft == sender())
			val = BleedLeft->value();
		else if (BleedRight == sender())
			val = BleedRight->value();
		val = val / m_unitRatio;
		MarginStruct v(val,val,val,val);
		setNewBleeds(v);
	}
}

void MarginWidget::setFacingPages(bool facing, int pagetype)
{
	facingPages = facing;
	pageType = pagetype;
	lText->setText(facing == true ? tr( "&Inside:" ) : tr( "&Left:" ));
	rText->setText(facing == true ? tr( "O&utside:" ) : tr( "&Right:" ));
	if (useBleeds)
	{
		if (facing)
		{
			BleedTxt3->setText( tr( "Inside:" ) );
			BleedTxt4->setText( tr( "Outside:" ) );
		}
		else
		{
			BleedTxt3->setText( tr( "Left:" ) );
			BleedTxt4->setText( tr( "Right:" ) );
		}
	}
	setPreset();
}

void MarginWidget::setPageWidthHeight(double width, double height)
{
	rightR->setMaximum(width * m_unitRatio - leftR->value());
	leftR->setMaximum(width * m_unitRatio - rightR->value());
	pageWidth = width;
	topR->setMaximum(height * m_unitRatio - bottomR->value());
	bottomR->setMaximum(height * m_unitRatio - topR->value());
	pageHeight = height;
	setPreset();
}

void MarginWidget::setPageWidth(double width)
{
	rightR->setMaximum(qMax(0.0, width * m_unitRatio - leftR->value()));
	leftR->setMaximum(qMax(0.0,width * m_unitRatio - rightR->value()));
	pageWidth = width;
	setPreset();
}

void MarginWidget::setPageHeight(double height)
{
	topR->setMaximum(qMax(0.0, height * m_unitRatio - bottomR->value()));
	bottomR->setMaximum(qMax(0.0,height * m_unitRatio - topR->value()));
	pageHeight = height;
	setPreset();
}

void MarginWidget::setTop()
{
	double newVal=topR->value() / m_unitRatio;
	bottomR->setMaximum(qMax(0.0, pageHeight * m_unitRatio - topR->value()));
	if (linkMargins->isChecked() && savedPresetItem==PresetLayout::none)
	{
		marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		marginData.Top = newVal;
	setPreset();
}

void MarginWidget::setBottom()
{
	double newVal = bottomR->value() / m_unitRatio;
	topR->setMaximum(qMax(0.0, pageHeight * m_unitRatio - bottomR->value()));
	if (linkMargins->isChecked() && savedPresetItem==PresetLayout::none)
	{
		marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		marginData.Bottom = newVal;
	setPreset();
}

void MarginWidget::setLeft()
{
	double newVal = leftR->value() / m_unitRatio;
	rightR->setMaximum(qMax(0.0, pageWidth * m_unitRatio - leftR->value()));
	if (linkMargins->isChecked() && savedPresetItem==PresetLayout::none)
	{
		marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		marginData.Left = newVal;
	setPreset();
}

void MarginWidget::setRight()
{
	double newVal = rightR->value() / m_unitRatio;
	leftR->setMaximum(qMax(0.0, pageWidth * m_unitRatio - rightR->value()));
	if (linkMargins->isChecked() && savedPresetItem==PresetLayout::none)
	{
		marginData.set(newVal, newVal, newVal, newVal);
		updateMarginSpinValues();
	}
	else
		marginData.Right = newVal;
	setPreset();
}

void MarginWidget::setNewUnit(int newUnitIndex)
{
	disconnect(topR, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	disconnect(bottomR, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	disconnect(leftR, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	disconnect(rightR, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
	m_unitIndex=newUnitIndex;
	m_unitRatio=unitGetRatioFromIndex(newUnitIndex);
	topR->setNewUnit(newUnitIndex);
	bottomR->setNewUnit(newUnitIndex);
	leftR->setNewUnit(newUnitIndex);
	rightR->setNewUnit(newUnitIndex);
	
	if (useBleeds)
	{
		disconnect(BleedLeft, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		disconnect(BleedRight, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		disconnect(BleedTop, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		disconnect(BleedBottom, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		BleedBottom->setNewUnit(newUnitIndex);
		BleedTop->setNewUnit(newUnitIndex);
		BleedRight->setNewUnit(newUnitIndex);
		BleedLeft->setNewUnit(newUnitIndex);
		connect(BleedLeft, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		connect(BleedRight, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		connect(BleedTop, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
		connect(BleedBottom, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	}
	connect(topR, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
}

void MarginWidget::setPreset()
{
	disconnect(topR, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	disconnect(bottomR, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	disconnect(leftR, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	disconnect(rightR, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
	if (savedPresetItem==PresetLayout::none)
		savedMargins=marginData;
	int item = presetCombo->currentIndex();

	MarginStruct marg = presetCombo->getMargins(item, pageWidth, pageHeight, leftR->value() / m_unitRatio);
	facingPages ? presetCombo->setEnabled(true) : presetCombo->setEnabled(false);
	bool restoringValues=false;
	if (item==PresetLayout::none && savedPresetItem!=PresetLayout::none)
	{
		marg=savedMargins;
		restoringValues=true;
	}
	if (restoringValues || (presetCombo->needUpdate() && facingPages))
	{
		marginData.set(qMax(0.0, marg.Top), qMax(0.0, marg.Left), qMax(0.0, marg.Bottom), qMax(0.0, marg.Right));
		updateMarginSpinValues();
		
		bottomR->setMaximum(qMax(0.0, pageHeight * m_unitRatio - topR->value()));
		topR->setMaximum(qMax(0.0, pageHeight * m_unitRatio - bottomR->value()));
		rightR->setMaximum(qMax(0.0, pageWidth * m_unitRatio - leftR->value()));
		leftR->setMaximum(qMax(0.0, pageWidth * m_unitRatio - rightR->value()));
		rightR->setEnabled(restoringValues);
		topR->setEnabled(restoringValues);
		bottomR->setEnabled(restoringValues);
	}
	else
	{
		rightR->setEnabled(true);
		topR->setEnabled(true);
		bottomR->setEnabled(true);
	}
	if (pageType == 1)
		rightR->setEnabled(false);
	leftR->setEnabled(item != PresetLayout::nineparts);
	if (item!=PresetLayout::none)
		linkMargins->setChecked(false);
	linkMargins->setEnabled(item==PresetLayout::none);
	connect(topR, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
	savedPresetItem=item;
}

void MarginWidget::setPageSize(const QString& pageSize)
{
	m_pageSize=pageSize;
}

void MarginWidget::setMarginsToPrinterMargins()
{
	UsePrinterMarginsDialog upm(parentWidget(), m_pageSize, unitGetRatioFromIndex(m_unitIndex), unitGetSuffixFromIndex(m_unitIndex));
	if (upm.exec())
	{
		double t,b,l,r;
		upm.getNewPrinterMargins(t,b,l,r);
		presetCombo->setCurrentIndex(PresetLayout::none);
		
		marginData.Top = t;
		marginData.Bottom = b;
		marginData.Left = l;
		marginData.Right = r;
		updateMarginSpinValues();

		bottomR->setMaximum((qMax(0.0, pageHeight - t) * m_unitRatio));
		topR->setMaximum((qMax(0.0, pageHeight - b) * m_unitRatio));
		rightR->setMaximum((qMax(0.0, pageWidth - l) * m_unitRatio));
		leftR->setMaximum((qMax(0.0, pageWidth - r) * m_unitRatio));

		rightR->setEnabled(true);
		topR->setEnabled(true);
		bottomR->setEnabled(true);
	}
}

double MarginWidget::top() const
{
	return marginData.Top;
}

double MarginWidget::bottom() const
{
	return marginData.Bottom;
}

double MarginWidget::left() const
{
	return marginData.Left;
}

double MarginWidget::right() const
{
	return marginData.Right;
}

void MarginWidget::setNewMargins(MarginStruct &m)
{
	disconnect(topR, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	disconnect(bottomR, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	disconnect(leftR, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	disconnect(rightR, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
	marginData=m;
	updateMarginSpinValues();
	connect(topR, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
}

void MarginWidget::setMarginPreset(int p)
{
	disconnect(presetCombo, SIGNAL(activated(int)), this, SLOT(setPreset()));
	savedPresetItem = p;
	presetCombo->setCurrentIndex(p);
	if (savedPresetItem==PresetLayout::none)
		savedMargins=marginData;
	int item = presetCombo->currentIndex();
	facingPages ? presetCombo->setEnabled(true) : presetCombo->setEnabled(false);
	bool restoringValues=false;
	if (item==PresetLayout::none && savedPresetItem!=PresetLayout::none)
	{
		restoringValues=true;
	}
	if (restoringValues || (presetCombo->needUpdate() && facingPages))
	{
		rightR->setEnabled(restoringValues);
		topR->setEnabled(restoringValues);
		bottomR->setEnabled(restoringValues);
	}
	else
	{
		rightR->setEnabled(true);
		topR->setEnabled(true);
		bottomR->setEnabled(true);
	}
	if (pageType == 1)
		rightR->setEnabled(false);
	leftR->setEnabled(item != PresetLayout::nineparts);
	if (item!=PresetLayout::none)
		linkMargins->setChecked(false);
	linkMargins->setEnabled(item==PresetLayout::none);
	connect(presetCombo, SIGNAL(activated(int)), this, SLOT(setPreset()));
}

int MarginWidget::getMarginPreset()
{
	return presetCombo->currentIndex();
}

bool MarginWidget::getMarginsForAllPages() const
{
	return marginsForAllPages->isChecked();
}

bool MarginWidget::getMarginsForAllMasterPages() const
{
	return marginsForAllMasterPages->isChecked();
}

void MarginWidget::setNewBleeds(MarginStruct& b)
{
	disconnect(BleedLeft, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	disconnect(BleedRight, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	disconnect(BleedTop, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	disconnect(BleedBottom, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	BleedTop->setValue(b.Top * m_unitRatio);
	BleedBottom->setValue(b.Bottom * m_unitRatio);
	BleedLeft->setValue(b.Left * m_unitRatio);
	BleedRight->setValue(b.Right * m_unitRatio);
	connect(BleedLeft, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	connect(BleedRight, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	connect(BleedTop, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
	connect(BleedBottom, SIGNAL(valueChanged(double)), this, SLOT(changeBleeds()));
}

double MarginWidget::topBleed() const
{
	return BleedTop->value() / m_unitRatio;
}

double MarginWidget::bottomBleed() const
{
	return BleedBottom->value() / m_unitRatio;
}

double MarginWidget::leftBleed() const
{
	return BleedLeft->value() / m_unitRatio;
}

double MarginWidget::rightBleed() const
{
	return BleedRight->value() / m_unitRatio;
}


/*
 * presets
 */
PresetLayout::PresetLayout(QWidget *parent) : QComboBox(parent)
{
	addItem( tr("None", "layout type"), PresetLayout::none);
	addItem( tr("Gutenberg"), PresetLayout::gutenberg);
	addItem( tr("Magazine"), PresetLayout::magazine);
	addItem( tr("Fibonacci"), PresetLayout::fibonacci);
	addItem( tr("Golden Mean"), PresetLayout::goldencut);
	addItem( tr("Nine Parts"), PresetLayout::nineparts);
	setCurrentIndex(PresetLayout::none);

	this->setToolTip( "<qt>" + tr("When you have selected a Document Layout other than Single Page, you can select a predefined page layout here. 'None' leaves margins as is, Gutenberg sets margins classically. 'Magazine' sets all margins to the same value. Leading is Left/Inside value.") + "</qt>");
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

bool PresetLayout::needUpdate() const
{
	return updateMargins;
}

const MarginStruct & MarginWidget::margins() const
{
	return marginData;
}

void MarginWidget::slotLinkMargins()
{
	disconnect(topR, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	disconnect(bottomR, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	disconnect(leftR, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	disconnect(rightR, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
	if (linkMargins->isChecked())
	{
		bottomR->setValue(leftR->value());
		topR->setValue(leftR->value());
		rightR->setValue(leftR->value());
		double newVal=leftR->value() / m_unitRatio;
		marginData.set(newVal, newVal, newVal, newVal);
	}	
	connect(topR, SIGNAL(valueChanged(double)), this, SLOT(setTop()));
	connect(bottomR, SIGNAL(valueChanged(double)), this, SLOT(setBottom()));
	connect(leftR, SIGNAL(valueChanged(double)), this, SLOT(setLeft()));
	connect(rightR, SIGNAL(valueChanged(double)), this, SLOT(setRight()));
	
}

void MarginWidget::updateMarginSpinValues()
{
	topR->setValue(marginData.Top * m_unitRatio);
	rightR->setValue(marginData.Right * m_unitRatio);
	bottomR->setValue(marginData.Bottom * m_unitRatio);
	leftR->setValue(marginData.Left * m_unitRatio);
}
