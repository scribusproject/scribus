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
#include <qcheckbox.h>

#include "tabdocument.h"
#include "tabdocument.moc"

#include "commonstrings.h"
#include "prefsmanager.h"
#include "units.h"
#include "marginWidget.h"
#include "mspinbox.h"
#include "pagelayout.h"
#include "pagesize.h"
#include "undomanager.h"
#include "sccombobox.h"
#include "prefsfile.h"
#include "scribusdoc.h"


TabDocument::TabDocument(QWidget* parent, const char* name, const bool reform)
	: QWidget(parent, name, 0)
{
	ApplicationPrefs* prefsData=&(PrefsManager::instance()->appPrefs);
	unitRatio = unitGetRatioFromIndex(prefsData->docUnitIndex);
	int decimals = unitGetPrecisionFromIndex(prefsData->docUnitIndex);

	tabLayout_7 = new QHBoxLayout( this, 0, 5, "tabLayout_7");
	Layout21 = new QVBoxLayout( 0, 0, 5, "Layout21");
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
	GroupSize->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));
	GroupSize->setColumnLayout(0, Qt::Vertical );
	GroupSize->layout()->setSpacing( 5 );
	GroupSize->layout()->setMargin( 10 );
	GroupSizeLayout = new QVBoxLayout( GroupSize->layout() );
	GroupSizeLayout->setAlignment( Qt::AlignTop );
	Layout6 = new QGridLayout( 0, 1, 1, 0, 5, "Layout6");

	pageSizeComboBox = new QComboBox( true, GroupSize, "pageSizeComboBox" );

	PageSize *ps=new PageSize(prefsData->pageSize);
	pageSizeComboBox->insertStringList(ps->sizeTRList());
	pageSizeComboBox->insertItem( CommonStrings::trCustomPageSize );
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
	pageWidth->setMinimumSize( QSize( 90, 20 ) );
	GZText3 = new QLabel( pageWidth, tr( "&Width:" ), GroupSize, "GZText3" );
	Layout5_2->addWidget( GZText3 );
	Layout5_2->addWidget( pageWidth );

	pageHeight = new MSpinBox( 1, 100000, GroupSize, decimals );
	pageHeight->setMinimumSize( QSize( 90, 20 ) );
	GZText4 = new QLabel( pageHeight, tr( "&Height:" ), GroupSize, "GZText4" );
	Layout5_2->addWidget( GZText4 );
	Layout5_2->addWidget( pageHeight );
	GroupSizeLayout->addLayout( Layout5_2 );
	
	QBoxLayout *sizePagesLayout = new QHBoxLayout( 0, 0, 5, "sizePagesLayout");
	sizePages = new QLabel( tr( "Apply settings to:" ), GroupSize, "sizePages" );
	sizePagesLayout->addWidget(sizePages);
	sizeAllPages = new QCheckBox( GroupSize, "sizeAllPages" );
	sizeAllPages->setText( tr( "All Document Pages" ) );
	sizeAllPages->setChecked( false );
	sizePagesLayout->addWidget(sizeAllPages);
	sizeAllMasterPages = new QCheckBox( GroupSize, "sizeAllMasterPages" );
	sizeAllMasterPages->setText( tr( "All Master Pages" ) );
	sizeAllMasterPages->setChecked( false );
	sizePagesLayout->addWidget(sizeAllMasterPages);
	GroupSizeLayout->addLayout(sizePagesLayout);
	sizePages->hide();
	sizeAllPages->hide();
	sizeAllMasterPages->hide();
	
	dsLayout4pv->addWidget( GroupSize );

	struct MarginStruct marg;
	marg.Top = prefsData->RandOben;
	marg.Bottom = prefsData->RandUnten;
	marg.Left = prefsData->RandLinks;
	marg.Right = prefsData->RandRechts;
	marginGroup = new MarginWidget(this,  tr( "Margin Guides" ), &marg, prefsData->docUnitIndex, reform);

	dsLayout4pv->addWidget( marginGroup );

	dsLayout4p->addLayout( dsLayout4pv );
	Layout21->addLayout( dsLayout4p );
	QHBoxLayout *asurLayout = new QHBoxLayout( 0, 0, 5, "asurLayout");

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
	urGroup->setCheckable(true);
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
	urGroup->setChecked((PrefsManager::instance()->prefsFile->getContext("undo")->getBool("enabled", true)));

	tabLayout_7->addLayout( Layout21 );

	QToolTip::add( pageWidth, "<qt>" + tr( "Width of document pages, editable if you have chosen a custom page size" ) + "</qt>" );
	QToolTip::add( pageHeight, "<qt>" + tr( "Height of document pages, editable if you have chosen a custom page size" ) + "</qt>" );
	QToolTip::add( pageSizeComboBox, "<qt>" + tr( "Default page size, either a standard size or a custom size" ) + "</qt>" );
	QToolTip::add( pageOrientationComboBox, "<qt>" + tr( "Default orientation of document pages" ) + "</qt>" );
	QToolTip::add( unitCombo, "<qt>" + tr( "Default unit of measurement for document editing" ) + "</qt>" );
	QToolTip::add( GroupAS, "<qt>" + tr( "When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses" ) + "</qt>" );
	QToolTip::add( ASTime, "<qt>" + tr( "Time period between saving automatically" ) + "</qt>" );
	QToolTip::add( urSpinBox, "<qt>" + tr("Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.") + "</qt>");
	QToolTip::add( sizeAllPages, "<qt>" + tr( "Apply the page size changes to all existing pages in the document" ) + "</qt>" );
	QToolTip::add( sizeAllMasterPages, "<qt>" + tr( "Apply the page size changes to all existing master pages in the document" ) + "</qt>" );
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	connect(urGroup, SIGNAL(toggled(bool)), this, SLOT(slotUndo(bool)));
}

void TabDocument::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	disconnect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	disconnect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	unitRatio = unitGetRatioFromIndex(prefsData->docUnitIndex);

	setSize(prefsData->pageSize);
	pageSizeComboBox->setCurrentText(prefsData->pageSize);
	prefsPageSizeName = prefsData->pageSize;
	setOrien(prefsData->pageOrientation);

	docLayout->selectItem(prefsData->FacingPages);
	docLayout->firstPage->setCurrentItem(prefsData->pageSets[prefsData->FacingPages].FirstPage);
	pageOrientationComboBox->setCurrentItem(prefsData->pageOrientation);
	unitCombo->setCurrentItem(prefsData->docUnitIndex);
	pageWidth->setValue(prefsData->PageWidth * unitRatio);
	pageHeight->setValue(prefsData->PageHeight * unitRatio);
	marginGroup->setNewMargins(prefsData->RandOben, prefsData->RandUnten, prefsData->RandLinks, prefsData->RandRechts);
	marginGroup->setPageWidthHeight(prefsData->PageWidth, prefsData->PageHeight);
	marginGroup->setNewBleeds(prefsData->BleedTop,prefsData->BleedBottom, prefsData->BleedLeft, prefsData->BleedRight);
	GroupAS->setChecked( prefsData->AutoSave );
	ASTime->setValue(prefsData->AutoSaveTime / 1000 / 60);
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
}

void TabDocument::restoreDefaults(ScribusDoc *prefsData)
{
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	disconnect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	disconnect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	unitRatio = unitGetRatioFromIndex(prefsData->unitIndex());

	setSize(prefsData->m_pageSize);
	pageSizeComboBox->setCurrentText(prefsData->m_pageSize);
	prefsPageSizeName = prefsData->m_pageSize;
	setOrien(prefsData->PageOri);

	docLayout->selectItem(prefsData->currentPageLayout);
	docLayout->firstPage->setCurrentItem(prefsData->pageSets[prefsData->currentPageLayout].FirstPage);
	pageOrientationComboBox->setCurrentItem(prefsData->PageOri);
	unitCombo->setCurrentItem(prefsData->unitIndex());
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	pageWidth->setValue(prefsData->pageWidth * unitRatio);
	pageHeight->setValue(prefsData->pageHeight * unitRatio);
	marginGroup->setNewBleeds(prefsData->BleedTop,prefsData->BleedBottom, prefsData->BleedLeft, prefsData->BleedRight);
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	marginGroup->setNewMargins(prefsData->pageMargins.Top, prefsData->pageMargins.Bottom,
							   prefsData->pageMargins.Left, prefsData->pageMargins.Right);
	marginGroup->setPageWidthHeight(prefsData->pageWidth, prefsData->pageHeight);
	GroupAS->setChecked( prefsData->AutoSave );
	ASTime->setValue(prefsData->AutoSaveTime / 1000 / 60);
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
}

void TabDocument::unitChange()
{
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));

	int docUnitIndex = unitCombo->currentItem();
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

	pageWidth->setValues(oldB * unitRatio, oldBM * unitRatio, decimals, pageW * unitRatio);
	pageHeight->setValues(oldH * unitRatio, oldHM * unitRatio, decimals, pageH * unitRatio);
	marginGroup->unitChange(unitRatio, decimals, suffix);
	marginGroup->setPageHeight(pageH);
	marginGroup->setPageWidth(pageW);

	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
}

void TabDocument::setPageWidth(int)
{
	pageW = pageWidth->value() / unitRatio;
	marginGroup->setPageWidth(pageW);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);
}

void TabDocument::setPageHeight(int)
{
	pageH = pageHeight->value() / unitRatio;
	marginGroup->setPageHeight(pageH);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentItem(pageSizeComboBox->count()-1);
}

void TabDocument::setSize(const QString & gr)
{
	pageW = pageWidth->value() / unitRatio;
	pageH = pageHeight->value() / unitRatio;
	PageSize *ps2=new PageSize(gr);

	prefsPageSizeName=ps2->name();
	if (gr != CommonStrings::trCustomPageSize)
	{
		pageW = ps2->width();
		pageH = ps2->height();
	}
	disconnect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	disconnect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
	pageWidth->setValue(pageW * unitRatio);
	pageHeight->setValue(pageH * unitRatio);
	marginGroup->setPageHeight(pageH);
	marginGroup->setPageWidth(pageW);
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
		if (pageSizeComboBox->currentText() == CommonStrings::trCustomPageSize)
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
	pageW = pageWidth->value() / unitRatio;
	pageH = pageHeight->value() / unitRatio;
	connect(pageWidth, SIGNAL(valueChanged(int)), this, SLOT(setPageWidth(int)));
	connect(pageHeight, SIGNAL(valueChanged(int)), this, SLOT(setPageHeight(int)));
}

void TabDocument::setPageSize()
{
	setOrien(pageOrientationComboBox->currentItem());
}

void TabDocument::hideReform()
{
	urGroup->hide();
	sizePages->show();
	sizeAllPages->show();
	sizeAllMasterPages->show();
}

void TabDocument::slotUndo(bool isEnabled)
{
	static PrefsContext *undoPrefs = PrefsManager::instance()->prefsFile->getContext("undo");

	if (!isEnabled)
		UndoManager::instance()->clearStack();
	UndoManager::instance()->setUndoEnabled(isEnabled);
	undoPrefs->set("enabled", isEnabled);
}
