/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qstring.h>

#include "tabdocument.h"
#include "tabdocument.moc"
#include "prefsmanager.h"
#include "units.h"
#include "marginWidget.h"
#include "mspinbox.h"
#include "pagelayout.h"
#include "pagesize.h"
#include "undomanager.h"
#include "sccombobox.h"


TabDocument::TabDocument(QWidget* parent, const char* name)
	: QWidget(parent, name, 0)
{
	ApplicationPrefs* prefsData=&(PrefsManager::instance()->appPrefs);
	unitRatio = unitGetRatioFromIndex(prefsData->docUnitIndex);
	int decimals = unitGetPrecisionFromIndex(prefsData->docUnitIndex);
	customText="Custom";
	customTextTR=tr( "Custom" );

	tabLayout_7 = new QHBoxLayout( this, 0, 5, "tabLayout_7");
	Layout21 = new QVBoxLayout( 0, 0, 6, "Layout21");
	dsLayout4p = new QHBoxLayout;
	dsLayout4p->setSpacing( 5 );
	dsLayout4p->setMargin( 0 );
	dsLayout4p->setAlignment( Qt::AlignLeft );
	docLayout = new PageLayouts(this, prefsData->pageSets);
	dsLayout4p->addWidget( docLayout );
	dsLayout4pv = new QVBoxLayout;
	dsLayout4pv->setSpacing( 5 );
	dsLayout4pv->setMargin( 0 );

	GroupSize = new QButtonGroup( tr( "Page Size" ), this, "GroupSize" );
	GroupSize->setColumnLayout(0, Qt::Vertical );
	GroupSize->layout()->setSpacing( 6 );
	GroupSize->layout()->setMargin( 10 );
	GroupSizeLayout = new QVBoxLayout( GroupSize->layout() );
	GroupSizeLayout->setAlignment( Qt::AlignTop );
	Layout6 = new QGridLayout( 0, 1, 1, 0, 6, "Layout6");

	pageSizeComboBox = new QComboBox( true, GroupSize, "pageSizeComboBox" );

	PageSize *ps=new PageSize(prefsData->pageSize);
	pageSizeComboBox->insertStringList(ps->sizeTRList());
	pageSizeComboBox->insertItem( customTextTR );
	pageSizeComboBox->setEditable(false);

	QStringList pageSizes=ps->sizeList();
	int sizeIndex=pageSizes.findIndex(ps->nameTR());
	if (sizeIndex!=-1)
		pageSizeComboBox->setCurrentItem(sizeIndex);
	else
		pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);
	GZText1 = new QLabel( pageSizeComboBox, tr( "&Size:" ), GroupSize, "GZText1" );
	Layout6->addWidget( GZText1, 0, 0 );
	Layout6->addWidget( pageSizeComboBox, 0, 1 );

	pageOrientationComboBox = new QComboBox( true, GroupSize, "pageOrientationComboBox" );
	pageOrientationComboBox->insertItem( tr( "Portrait" ) );
	pageOrientationComboBox->insertItem( tr( "Landscape" ) );
	pageOrientationComboBox->setEditable(false);
	GZText2 = new QLabel( pageOrientationComboBox, tr( "Orie&ntation:" ), GroupSize, "GZText2" );
	Layout6->addWidget( GZText2, 1, 0 );
	Layout6->addWidget( pageOrientationComboBox, 1, 1 );
	unitCombo = new QComboBox( true, GroupSize, "unitCombo" );
	unitCombo->insertStringList(unitGetTextUnitList());
	unitCombo->setEditable(false);
	unitComboText = new QLabel( unitCombo, tr( "Units:" ), GroupSize, "unitComboText" );
	Layout6->addWidget( unitComboText, 2, 0 );
	Layout6->addWidget( unitCombo, 2, 1 );

	GroupSizeLayout->addLayout( Layout6 );

	Layout5_2 = new QHBoxLayout( 0, 0, 6, "Layout5_2");

	pageWidth = new MSpinBox( 1, 100000, GroupSize, decimals );
	pageWidth->setMinimumSize( QSize( 70, 20 ) );
	GZText3 = new QLabel( pageWidth, tr( "&Width:" ), GroupSize, "GZText3" );
	Layout5_2->addWidget( GZText3 );
	Layout5_2->addWidget( pageWidth );

	pageHeight = new MSpinBox( 1, 100000, GroupSize, decimals );
	pageHeight->setMinimumSize( QSize( 70, 20 ) );
	GZText4 = new QLabel( pageHeight, tr( "&Height:" ), GroupSize, "GZText4" );
	Layout5_2->addWidget( GZText4 );
	Layout5_2->addWidget( pageHeight );

	GroupSizeLayout->addLayout( Layout5_2 );
	dsLayout4pv->addWidget( GroupSize );

	struct MarginStruct marg;
	marg.Top = prefsData->RandOben;
	marg.Bottom = prefsData->RandUnten;
	marg.Left = prefsData->RandLinks;
	marg.Right = prefsData->RandRechts;
	marginGroup = new MarginWidget(this,  tr( "Margin Guides" ), &marg, prefsData->docUnitIndex);
	dsLayout4pv->addWidget( marginGroup );
	dsLayout4p->addLayout( dsLayout4pv );
	Layout21->addLayout( dsLayout4p );
	QBoxLayout *asurLayout = new QHBoxLayout( 0, 0, 6, "asurLayout");

	GroupAS = new QGroupBox( tr( "Autosave" ), this, "GroupAS" );
	GroupAS->setCheckable( true );
	GroupAS->setColumnLayout(0, Qt::Vertical );
	GroupAS->layout()->setSpacing( 5 );
	GroupAS->layout()->setMargin( 10 );
	GroupASLayout = new QHBoxLayout( GroupAS->layout() );
	GroupASLayout->setAlignment( Qt::AlignTop );
	ASTime = new QSpinBox( GroupAS, "Time" );
	ASTime->setMaxValue( 60 );
	ASTime->setMinValue( 1 );
	ASTime->setSuffix( " " + tr("min") );
	ASText = new QLabel( ASTime, tr( "&Interval:" ), GroupAS, "ASText" );
	GroupASLayout->addWidget( ASText);
	GroupASLayout->addWidget( ASTime );
	asurLayout->addWidget(GroupAS);

	urGroup = new QGroupBox( tr("Undo/Redo"), this, "urGroup");
	urGroup->setColumnLayout(0, Qt::Vertical);
	urGroup->layout()->setSpacing(5);
	urGroup->layout()->setMargin(10);
	QGridLayout *urGroupLayout = new QGridLayout(urGroup->layout());
	urGroupLayout->setAlignment(Qt::AlignTop);
	urSpinBox = new QSpinBox(urGroup, "urSpinBox");
	urSpinBox->setMinValue(0);
	urSpinBox->setMaxValue(1000);
	int urSBValue = UndoManager::instance()->getHistoryLength();
	if (urSBValue == -1)
		urSpinBox->setEnabled(false);
	else
		urSpinBox->setValue(urSBValue);
	urLabel = new QLabel(urSpinBox, tr("Action history length"), urGroup, "urLabel");
	urGroupLayout->addWidget(urLabel, 0, 0);
	urGroupLayout->addWidget(urSpinBox, 0, 1);
	asurLayout->addWidget(urGroup);
	Layout21->addLayout(asurLayout);

	tabLayout_7->addLayout( Layout21 );

	QToolTip::add( pageWidth, "<qt>" + tr( "Width of document pages, editable if you have chosen a custom page size" ) + "</qt>" );
	QToolTip::add( pageHeight, "<qt>" + tr( "Height of document pages, editable if you have chosen a custom page size" ) + "</qt>" );
	QToolTip::add( pageSizeComboBox, "<qt>" + tr( "Default page size, either a standard size or a custom size" ) + "</qt>" );
	QToolTip::add( pageOrientationComboBox, "<qt>" + tr( "Default orientation of document pages" ) + "</qt>" );
	QToolTip::add( unitCombo, "<qt>" + tr( "Default unit of measurement for document editing" ) + "</qt>" );
	QToolTip::add( GroupAS, "<qt>" + tr( "When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses" ) + "</qt>" );
	QToolTip::add( ASTime, "<qt>" + tr( "Time period between saving automatically" ) + "</qt>" );
	QToolTip::add( urSpinBox, "<qt>" + tr("Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.") + "</qt>");

	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	//connect(docLayout, SIGNAL( selectedLayout(int) ), this, SLOT( setDS(int) ) );
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
}

void TabDocument::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	unitRatio = unitGetRatioFromIndex(prefsData->docUnitIndex);

	setSize(prefsData->pageSize);
	setOrien(prefsData->pageOrientation);

	docLayout->selectItem(prefsData->FacingPages);
	docLayout->firstPage->setCurrentItem(prefsData->pageSets[prefsData->FacingPages].FirstPage);
	pageOrientationComboBox->setCurrentItem(prefsData->pageOrientation);
	unitCombo->setCurrentItem(prefsData->docUnitIndex);
	pageWidth->setValue(prefsData->PageWidth * unitRatio);
	pageHeight->setValue(prefsData->PageHeight * unitRatio);
	marginGroup->setNewMargins(prefsData->RandOben, prefsData->RandUnten,
							   prefsData->RandLinks, prefsData->RandRechts);
	marginGroup->setPageWidthHeight(prefsData->PageWidth, prefsData->PageHeight);
	GroupAS->setChecked( prefsData->AutoSave );
	ASTime->setValue(prefsData->AutoSaveTime / 1000 / 60);
}

void TabDocument::unitChange(int docUnitIndex)
{
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));

	double oldUnitRatio = unitRatio;
	double oldB, oldBM, oldH, oldHM, val;
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
	int decimals = unitGetDecimalsFromIndex(docUnitIndex);
	QString suffix = unitGetSuffixFromIndex(docUnitIndex);
	
	pageWidth->getValues(&oldB, &oldBM, &decimals, &val);
	oldB /= oldUnitRatio;
	oldBM /= oldUnitRatio;
	pageHeight->getValues(&oldH, &oldHM, &decimals, &val);
	oldH /= oldUnitRatio;
	oldHM /= oldUnitRatio;

	pageWidth->setSuffix(suffix);
	pageHeight->setSuffix(suffix);

	pageWidth->setValues(oldB * unitRatio, oldBM * unitRatio, decimals, Pagebr * unitRatio);
	pageHeight->setValues(oldH * unitRatio, oldHM * unitRatio, decimals, Pageho * unitRatio);
	marginGroup->unitChange(unitRatio, decimals, suffix);
	marginGroup->setPageHeight(Pageho);
	marginGroup->setPageWidth(Pagebr);

	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
}

void TabDocument::setPageWidth(int)
{
	Pagebr = pageWidth->value() / unitRatio;
	marginGroup->setPageWidth(Pagebr);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=customTextTR && psText!=customText)
		pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);
}

void TabDocument::setPageHeight(int)
{
	Pageho = pageHeight->value() / unitRatio;
	marginGroup->setPageHeight(Pageho);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=customTextTR && psText!=customText)
		pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);
}

void TabDocument::setSize(const QString & gr)
{
	Pagebr = pageWidth->value() / unitRatio;
	Pageho = pageHeight->value() / unitRatio;
	PageSize *ps2=new PageSize(gr);

	prefsPageSizeName=ps2->name();
	if (gr != customTextTR)
	{
		Pagebr = ps2->width();
		Pageho = ps2->height();
	}
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	pageWidth->setValue(Pagebr * unitRatio);
	pageHeight->setValue(Pageho * unitRatio);
	marginGroup->setPageHeight(Pageho);
	marginGroup->setPageWidth(Pagebr);
	marginGroup->setPageSize(gr);
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	delete ps2;
}

void TabDocument::setOrien(int ori)
{
	double br;
	setSize(pageSizeComboBox->currentText());
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	if (ori == 0)
	{
		//if (pageSizeComboBox->currentItem() == 30)
		if (pageSizeComboBox->currentText() == customTextTR)
		{
			br = pageWidth->value();
			pageWidth->setValue(pageHeight->value());
			pageHeight->setValue(br);
		}
	}
	else
	{
		br = pageWidth->value();
		pageWidth->setValue(pageHeight->value());
		pageHeight->setValue(br);
	}
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
}

void TabDocument::setPageSize()
{
	setOrien(pageOrientationComboBox->currentItem());
}
