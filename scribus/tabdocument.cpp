/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tabdocument.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QString>
#include <QToolTip>

#include "commonstrings.h"
#include "prefsmanager.h"
#include "units.h"
#include "marginwidget.h"
#include "scrspinbox.h"
#include "pagelayout.h"
#include "pagesize.h"
#include "undomanager.h"
#include "sccombobox.h"
#include "prefsfile.h"
#include "scribusdoc.h"
#include "util.h"


TabDocument::TabDocument(QWidget* parent, const char* name, const bool reform)
	: QWidget(parent)
{
	ApplicationPrefs* prefsData=&(PrefsManager::instance()->appPrefs);
	unitRatio = unitGetRatioFromIndex(prefsData->docUnitIndex);

	tabLayout_7 = new QHBoxLayout( this );
	tabLayout_7->setSpacing( 5 );
	tabLayout_7->setMargin( 0 );
	Layout21 = new QVBoxLayout;
	Layout21->setSpacing( 5 );
	Layout21->setMargin( 0 );
	dsLayout4p = new QHBoxLayout;
	dsLayout4p->setSpacing( 5 );
	dsLayout4p->setMargin( 0 );
	dsLayout4p->setAlignment( Qt::AlignLeft );
	docLayout = new PageLayouts(this, prefsData->pageSets);
	dsLayout4p->addWidget( docLayout );
	dsLayout4pv = new QVBoxLayout;
	dsLayout4pv->setSpacing( 5 );
	dsLayout4pv->setMargin( 0 );

	GroupSize = new QGroupBox( tr( "Page Size" ), this );
	GroupSize->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));
	GroupSizeLayout = new QVBoxLayout( GroupSize );
	GroupSizeLayout->setSpacing( 5 );
	GroupSizeLayout->setMargin( 10 );
	GroupSizeLayout->setAlignment( Qt::AlignTop );
	Layout6 = new QGridLayout;
	Layout6->setSpacing( 5 );
	Layout6->setMargin( 0 );

	pageSizeComboBox = new QComboBox(GroupSize);

	PageSize *ps=new PageSize(prefsData->pageSize);
	pageSizeComboBox->addItems(ps->sizeTRList());
	pageSizeComboBox->addItem( CommonStrings::trCustomPageSize );
	pageSizeComboBox->setEditable(false);

	QStringList pageSizes=ps->sizeList();
	int sizeIndex=pageSizes.indexOf(ps->nameTR());
	if (sizeIndex!=-1)
		pageSizeComboBox->setCurrentIndex(sizeIndex);
	else
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);
	GZText1 = new QLabel( tr( "&Size:" ), GroupSize );
	GZText1->setBuddy(pageSizeComboBox);
	Layout6->addWidget( GZText1, 0, 0 );
	Layout6->addWidget( pageSizeComboBox, 0, 1 );

	pageOrientationComboBox = new QComboBox(GroupSize);
	pageOrientationComboBox->addItem( tr( "Portrait" ) );
	pageOrientationComboBox->addItem( tr( "Landscape" ) );
	pageOrientationComboBox->setEditable(false);
	GZText2 = new QLabel( tr( "Orie&ntation:" ), GroupSize );
	GZText2->setBuddy(pageOrientationComboBox);
	Layout6->addWidget( GZText2, 1, 0 );
	Layout6->addWidget( pageOrientationComboBox, 1, 1 );
	unitCombo = new QComboBox( GroupSize );
	unitCombo->addItems(unitGetTextUnitList());
	unitCombo->setEditable(false);
	unitComboText = new QLabel( tr( "Units:" ), GroupSize );
	unitComboText->setBuddy(unitCombo);
	Layout6->addWidget( unitComboText, 2, 0 );
	Layout6->addWidget( unitCombo, 2, 1 );
	GroupSizeLayout->addLayout( Layout6 );

	Layout5_2 = new QHBoxLayout;
	Layout5_2->setSpacing( 5 );
	Layout5_2->setMargin( 0 );

	pageWidth = new ScrSpinBox( 1, 16777215, GroupSize, prefsData->docUnitIndex );
	pageWidth->setMinimumSize( QSize( 90, 20 ) );
	GZText3 = new QLabel( tr( "&Width:" ), GroupSize );
	GZText3->setBuddy(pageWidth);
	Layout5_2->addWidget( GZText3 );
	Layout5_2->addWidget( pageWidth );

	pageHeight = new ScrSpinBox( 1, 16777215, GroupSize, prefsData->docUnitIndex );
	pageHeight->setMinimumSize( QSize( 90, 20 ) );
	GZText4 = new QLabel( tr( "&Height:" ), GroupSize );
	GZText4->setBuddy(pageHeight);
	Layout5_2->addWidget( GZText4 );
	Layout5_2->addWidget( pageHeight );
	GroupSizeLayout->addLayout( Layout5_2 );
	
	QHBoxLayout *sizePagesLayout = new QHBoxLayout;
	sizePagesLayout->setSpacing( 5 );
	sizePagesLayout->setMargin( 0 );
	sizePages = new QLabel( tr( "Apply settings to:" ), GroupSize );
	sizePagesLayout->addWidget(sizePages);
	sizeAllPages = new QCheckBox( GroupSize );
	sizeAllPages->setText( tr( "All Document Pages" ) );
	sizeAllPages->setChecked( false );
	sizePagesLayout->addWidget(sizeAllPages);
	sizeAllMasterPages = new QCheckBox( GroupSize );
	sizeAllMasterPages->setText( tr( "All Master Pages" ) );
	sizeAllMasterPages->setChecked( false );
	sizePagesLayout->addWidget(sizeAllMasterPages);
	GroupSizeLayout->addLayout(sizePagesLayout);
	sizePages->hide();
	sizeAllPages->hide();
	sizeAllMasterPages->hide();
	
	dsLayout4pv->addWidget( GroupSize );

	marginGroup = new MarginWidget(this,  tr( "Margin Guides" ), &(prefsData->margins), prefsData->docUnitIndex, reform);

	dsLayout4pv->addWidget( marginGroup );

	dsLayout4p->addLayout( dsLayout4pv );
	Layout21->addLayout( dsLayout4p );
	QHBoxLayout *asurLayout = new QHBoxLayout;
	asurLayout->setSpacing( 5 );
	asurLayout->setMargin( 0 );

	GroupAS = new QGroupBox( tr( "Autosave" ), this );
	GroupAS->setCheckable( true );
	GroupASLayout = new QHBoxLayout( GroupAS );
	GroupASLayout->setSpacing( 5 );
	GroupASLayout->setMargin( 10 );
	GroupASLayout->setAlignment( Qt::AlignTop );
	ASTime = new QSpinBox( GroupAS );
	ASTime->setMaximum( 60 );
	ASTime->setMinimum( 1 );
	ASTime->setSuffix( " " + tr("min") );
	ASText = new QLabel( tr( "&Interval:" ), GroupAS);
	ASText->setBuddy(ASTime);
	GroupASLayout->addWidget( ASText);
	GroupASLayout->addWidget( ASTime );
	asurLayout->addWidget(GroupAS);

	urGroup = new QGroupBox( tr("Undo/Redo"), this);
	urGroup->setCheckable(true);
	QGridLayout *urGroupLayout = new QGridLayout(urGroup);
	urGroupLayout->setSpacing( 5 );
	urGroupLayout->setMargin( 10 );
	urGroupLayout->setAlignment(Qt::AlignTop);
	urSpinBox = new QSpinBox(urGroup);
	urSpinBox->setMinimum(0);
	urSpinBox->setMaximum(1000);
	int urSBValue = UndoManager::instance()->getHistoryLength();
	if (urSBValue == -1)
		urSpinBox->setEnabled(false);
	else
		urSpinBox->setValue(urSBValue);
	urLabel = new QLabel( tr("Action history length"), urGroup);
	urLabel->setBuddy(urSpinBox);
	urGroupLayout->addWidget(urLabel, 0, 0);
	urGroupLayout->addWidget(urSpinBox, 0, 1);
	asurLayout->addWidget(urGroup);
	Layout21->addLayout(asurLayout);
	urGroup->setChecked((PrefsManager::instance()->prefsFile->getContext("undo")->getBool("enabled", true)));

	tabLayout_7->addLayout( Layout21 );

	pageWidth->setToolTip( "<qt>" + tr( "Width of document pages, editable if you have chosen a custom page size" ) + "</qt>" );
	pageHeight->setToolTip( "<qt>" + tr( "Height of document pages, editable if you have chosen a custom page size" ) + "</qt>" );
	pageSizeComboBox->setToolTip( "<qt>" + tr( "Default page size, either a standard size or a custom size" ) + "</qt>" );
	pageOrientationComboBox->setToolTip( "<qt>" + tr( "Default orientation of document pages" ) + "</qt>" );
	unitCombo->setToolTip( "<qt>" + tr( "Default unit of measurement for document editing" ) + "</qt>" );
	GroupAS->setToolTip( "<qt>" + tr( "When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses" ) + "</qt>" );
	ASTime->setToolTip( "<qt>" + tr( "Time period between saving automatically" ) + "</qt>" );
	urSpinBox->setToolTip( "<qt>" + tr("Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.") + "</qt>");
	sizeAllPages->setToolTip( "<qt>" + tr( "Apply the page size changes to all existing pages in the document" ) + "</qt>" );
	sizeAllMasterPages->setToolTip( "<qt>" + tr( "Apply the page size changes to all existing master pages in the document" ) + "</qt>" );
	connect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	connect(urGroup, SIGNAL(toggled(bool)), this, SLOT(slotUndo(bool)));
}

void TabDocument::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	disconnect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	disconnect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	disconnect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	disconnect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	unitRatio = unitGetRatioFromIndex(prefsData->docUnitIndex);

//	setSize(prefsData->pageSize);
	if (prefsData->pageSize == CommonStrings::customPageSize)
		setCurrentComboItem(pageSizeComboBox, CommonStrings::trCustomPageSize);
	else
		setCurrentComboItem(pageSizeComboBox, prefsData->pageSize);
	prefsPageSizeName = prefsData->pageSize;
//	setOrien(prefsData->pageOrientation);

	docLayout->selectItem(prefsData->FacingPages);
	docLayout->firstPage->setCurrentIndex(prefsData->pageSets[prefsData->FacingPages].FirstPage);
	pageOrientationComboBox->setCurrentIndex(prefsData->pageOrientation);
	unitCombo->setCurrentIndex(prefsData->docUnitIndex);
	pageWidth->setValue(prefsData->PageWidth * unitRatio);
	pageHeight->setValue(prefsData->PageHeight * unitRatio);
	pageW = prefsData->PageWidth;
	pageH = prefsData->PageHeight;
	marginGroup->setNewMargins(prefsData->margins);
	marginGroup->setPageWidthHeight(prefsData->PageWidth, prefsData->PageHeight);
	marginGroup->setPageSize(prefsPageSizeName);
	marginGroup->setNewBleeds(prefsData->bleeds);
	marginGroup->setMarginPreset(prefsData->marginPreset);
	GroupAS->setChecked( prefsData->AutoSave );
	ASTime->setValue(prefsData->AutoSaveTime / 1000 / 60);
	connect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	unitChange();
}

void TabDocument::restoreDefaults(ScribusDoc *prefsData)
{
	disconnect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	disconnect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	disconnect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	disconnect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	unitRatio = unitGetRatioFromIndex(prefsData->unitIndex());

//	setSize(prefsData->m_pageSize);
	if (prefsData->m_pageSize == CommonStrings::customPageSize)
		setCurrentComboItem(pageSizeComboBox, CommonStrings::trCustomPageSize);
	else
		setCurrentComboItem(pageSizeComboBox, prefsData->m_pageSize);
	prefsPageSizeName = prefsData->m_pageSize;
//	setOrien(prefsData->PageOri);

	docLayout->selectItem(prefsData->currentPageLayout);
	docLayout->firstPage->setCurrentIndex(prefsData->pageSets[prefsData->currentPageLayout].FirstPage);
	pageOrientationComboBox->setCurrentIndex(prefsData->PageOri);
	unitCombo->setCurrentIndex(prefsData->unitIndex());
	pageWidth->setValue(prefsData->pageWidth * unitRatio);
	pageHeight->setValue(prefsData->pageHeight * unitRatio);
	pageW = prefsData->pageWidth;
	pageH = prefsData->pageHeight;
	marginGroup->setNewBleeds(prefsData->bleeds);
	marginGroup->setNewMargins(prefsData->pageMargins);
	marginGroup->setPageWidthHeight(prefsData->pageWidth, prefsData->pageHeight);
	marginGroup->setPageSize(prefsPageSizeName);
	marginGroup->setMarginPreset(prefsData->marginPreset);
	GroupAS->setChecked( prefsData->AutoSave );
	ASTime->setValue(prefsData->AutoSaveTime / 1000 / 60);
	connect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	unitChange();
}

void TabDocument::unitChange()
{
	disconnect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	disconnect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));

	int docUnitIndex = unitCombo->currentIndex();
	pageWidth->setNewUnit(docUnitIndex);
	pageHeight->setNewUnit(docUnitIndex);
	unitRatio = unitGetRatioFromIndex(docUnitIndex);	
	pageWidth->setValue(pageW * unitRatio);
	pageHeight->setValue(pageH * unitRatio);
	marginGroup->setNewUnit(docUnitIndex);
	marginGroup->setPageHeight(pageH);
	marginGroup->setPageWidth(pageW);

	connect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
}

void TabDocument::setPageWidth(double)
{
	pageW = pageWidth->value() / unitRatio;
	marginGroup->setPageWidth(pageW);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);
	int newOrientation = (pageWidth->value() > pageHeight->value()) ? landscapePage : portraitPage;
	if (newOrientation != pageOrientationComboBox->currentIndex())
	{
		pageOrientationComboBox->blockSignals(true);
		pageOrientationComboBox->setCurrentIndex(newOrientation);
		pageOrientationComboBox->blockSignals(false);
	}
}

void TabDocument::setPageHeight(double)
{
	pageH = pageHeight->value() / unitRatio;
	marginGroup->setPageHeight(pageH);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);
	int newOrientation = (pageWidth->value() > pageHeight->value()) ? landscapePage : portraitPage;
	if (newOrientation != pageOrientationComboBox->currentIndex())
	{
		pageOrientationComboBox->blockSignals(true);
		pageOrientationComboBox->setCurrentIndex(newOrientation);
		pageOrientationComboBox->blockSignals(false);
	}
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
	else
		prefsPageSizeName = CommonStrings::customPageSize;
	disconnect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	disconnect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	pageWidth->setValue(pageW * unitRatio);
	pageHeight->setValue(pageH * unitRatio);
	marginGroup->setPageHeight(pageH);
	marginGroup->setPageWidth(pageW);
	marginGroup->setPageSize(gr);
	connect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	delete ps2;
}

void TabDocument::setOrien(int ori)
{
	double br;
	setSize(pageSizeComboBox->currentText());
	disconnect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	disconnect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
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
	connect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
}

void TabDocument::setPageSize()
{
	setOrien(pageOrientationComboBox->currentIndex());
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
	UndoManager::instance()->setAllHistoryLengths(urSpinBox->value());
}
