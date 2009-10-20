/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_documentsetup.h"
#include "commonstrings.h"
#include "pagesize.h"
#include "prefsstructs.h"
#include "units.h"
#include "util.h"

Prefs_DocumentSetup::Prefs_DocumentSetup(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	pageWidthSpinBox->setMaximum(16777215);
	pageHeightSpinBox->setMaximum(16777215);
	languageChange();
}

Prefs_DocumentSetup::~Prefs_DocumentSetup()
{
}

void Prefs_DocumentSetup::unitChange()
{
	int docUnitIndex = pageUnitsComboBox->currentIndex();
	pageWidthSpinBox->setNewUnit(docUnitIndex);
	pageHeightSpinBox->setNewUnit(docUnitIndex);
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
	pageWidthSpinBox->setValue(pageW * unitRatio);
	pageHeightSpinBox->setValue(pageH * unitRatio);
}

void Prefs_DocumentSetup::languageChange()
{
	int i=0;

	pageOrientationComboBox->currentIndex();
	pageOrientationComboBox->clear();
	pageOrientationComboBox->addItem( tr( "Portrait" ) );
	pageOrientationComboBox->addItem( tr( "Landscape" ) );
	pageOrientationComboBox->setCurrentIndex(i<0?0:i);

	i=pageUnitsComboBox->currentIndex();
	pageUnitsComboBox->clear();
	pageUnitsComboBox->addItems(unitGetTextUnitList());
	pageUnitsComboBox->setCurrentIndex(i<0?0:i);
}

void Prefs_DocumentSetup::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	unitRatio = unitGetRatioFromIndex(prefsData->docSetupPrefs.docUnitIndex);
	prefsPageSizeName = prefsData->docSetupPrefs.pageSize;
	PageSize *ps=new PageSize(prefsPageSizeName);
	pageSizeComboBox->addItems(ps->sizeTRList());
	pageSizeComboBox->addItem( CommonStrings::trCustomPageSize );
	if (prefsData->docSetupPrefs.pageSize == CommonStrings::customPageSize)
		setCurrentComboItem(pageSizeComboBox, CommonStrings::trCustomPageSize);
	else
		setCurrentComboItem(pageSizeComboBox, prefsPageSizeName);

	pageOrientationComboBox->setCurrentIndex(prefsData->docSetupPrefs.pageOrientation);
	pageUnitsComboBox->setCurrentIndex(prefsData->docSetupPrefs.docUnitIndex);
	pageW = prefsData->docSetupPrefs.pageWidth;
	pageH = prefsData->docSetupPrefs.pageHeight;
	pageWidthSpinBox->setValue(pageW * unitRatio);
	pageHeightSpinBox->setValue(pageH * unitRatio);

	/*
	docLayout->selectItem(prefsData->docSetupPrefs.pagePositioning);
	docLayout->firstPage->setCurrentIndex(prefsData->pageSets[prefsData->docSetupPrefs.pagePositioning].FirstPage);


	marginGroup->setNewMargins(prefsData->docSetupPrefs.margins);
	marginGroup->setPageWidthHeight(prefsData->docSetupPrefs.pageWidth, prefsData->docSetupPrefs.pageHeight);
	marginGroup->setPageSize(prefsPageSizeName);
	marginGroup->setNewBleeds(prefsData->docSetupPrefs.bleeds);
	marginGroup->setMarginPreset(prefsData->docSetupPrefs.marginPreset);
	GroupAS->setChecked( prefsData->docSetupPrefs.AutoSave );
	ASTime->setValue(prefsData->docSetupPrefs.AutoSaveTime / 1000 / 60);
	connect(pageWidth, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(pageHeight, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	*/
	unitChange();
}

void Prefs_DocumentSetup::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	/*
	pageSizeComboBox
			pageOrientationComboBox
			pageUnitsComboBox
			pageWidthSpinBox
			pageHeightSpinBox
			*/
}


