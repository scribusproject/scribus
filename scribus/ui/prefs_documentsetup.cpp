/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QButtonGroup>

#include "ui/prefs_documentsetup.h"
#include "commonstrings.h"
#include "ui/newmarginwidget.h"
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

	pageLayoutButtonGroup->setId(singlePageRadioButton,0);
	pageLayoutButtonGroup->setId(doublePageRadioButton,1);
	pageLayoutButtonGroup->setId(threeFoldRadioButton,2);
	pageLayoutButtonGroup->setId(fourFoldRadioButton,3);

	connect(pageSizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	connect(pageOrientationComboBox, SIGNAL(activated(int)), this, SLOT(setPageOrientation(int)));
	connect(pageWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(pageHeightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	connect(pageLayoutButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(pageLayoutChanged(int)));
	connect(pageUnitsComboBox, SIGNAL(activated(int)), this, SLOT(unitChange()));
}

Prefs_DocumentSetup::~Prefs_DocumentSetup()
{
}

void Prefs_DocumentSetup::unitChange()
{
	pageWidthSpinBox->blockSignals(true);
	pageHeightSpinBox->blockSignals(true);

	int docUnitIndex = pageUnitsComboBox->currentIndex();
	pageWidthSpinBox->setNewUnit(docUnitIndex);
	pageHeightSpinBox->setNewUnit(docUnitIndex);
	unitRatio = unitGetRatioFromIndex(docUnitIndex);
	pageWidthSpinBox->setValue(pageW * unitRatio);
	pageHeightSpinBox->setValue(pageH * unitRatio);
	marginsWidget->setNewUnitIndex(docUnitIndex);
	marginsWidget->setPageHeight(pageH);
	marginsWidget->setPageWidth(pageW);

	pageWidthSpinBox->blockSignals(false);
	pageHeightSpinBox->blockSignals(false);
}

void Prefs_DocumentSetup::languageChange()
{
	int i=0;

	i=pageOrientationComboBox->currentIndex();
	pageOrientationComboBox->clear();
	pageOrientationComboBox->addItem( tr( "Portrait" ) );
	pageOrientationComboBox->addItem( tr( "Landscape" ) );
	pageOrientationComboBox->setCurrentIndex(i<0?0:i);

	i=pageUnitsComboBox->currentIndex();
	pageUnitsComboBox->clear();
	pageUnitsComboBox->addItems(unitGetTextUnitList());
	pageUnitsComboBox->setCurrentIndex(i<0?0:i);

	setupPageSets();

}

void Prefs_DocumentSetup::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	pageWidthSpinBox->blockSignals(true);
	pageHeightSpinBox->blockSignals(true);
	pageOrientationComboBox->blockSignals(true);
	pageSizeComboBox->blockSignals(true);

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

	pageSets=prefsData->pageSets;
	switch (prefsData->docSetupPrefs.pagePositioning)
	{
		case 0:
			singlePageRadioButton->setChecked(true);
			break;
		case 1:
			doublePageRadioButton->setChecked(true);
			break;
		case 2:
			threeFoldRadioButton->setChecked(true);
			break;
		case 3:
			fourFoldRadioButton->setChecked(true);
			break;
	}
	setupPageSets();

	layoutFirstPageIsComboBox->setCurrentIndex(prefsData->pageSets[prefsData->docSetupPrefs.pagePositioning].FirstPage);

	pageWidthSpinBox->blockSignals(false);
	pageHeightSpinBox->blockSignals(false);
	pageOrientationComboBox->blockSignals(false);
	pageSizeComboBox->blockSignals(false);

/*
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
	prefsData->docSetupPrefs.pageSize=prefsPageSizeName;
	prefsData->docSetupPrefs.pageOrientation=pageOrientationComboBox->currentIndex();
	prefsData->docSetupPrefs.pageWidth=pageW;
	prefsData->docSetupPrefs.pageHeight=pageH;
	prefsData->docSetupPrefs.pagePositioning=pageLayoutButtonGroup->checkedId();
	prefsData->pageSets[prefsData->docSetupPrefs.pagePositioning].FirstPage=layoutFirstPageIsComboBox->currentIndex();
}

void Prefs_DocumentSetup::setupPageSets()
{
	int i=layoutFirstPageIsComboBox->currentIndex();
	layoutFirstPageIsComboBox->clear();
	int currIndex=pageLayoutButtonGroup->checkedId()<0?0:pageLayoutButtonGroup->checkedId();
	if (currIndex>=0 && currIndex<pageSets.count())
		for(QStringList::Iterator pNames = pageSets[currIndex].pageNames.begin(); pNames != pageSets[currIndex].pageNames.end(); ++pNames )
			layoutFirstPageIsComboBox->addItem(CommonStrings::translatePageSetLocString(*pNames));
	layoutFirstPageIsComboBox->setCurrentIndex(i<0?0:i);
}

void Prefs_DocumentSetup::pageLayoutChanged(int /*i*/)
{
	setupPageSets();
	layoutFirstPageIsComboBox->setCurrentIndex(pageSets[pageLayoutButtonGroup->checkedId()].FirstPage);
}

void Prefs_DocumentSetup::setPageWidth(double w)
{
	pageW = pageWidthSpinBox->value() / unitRatio;
	marginsWidget->setPageWidth(pageW);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);
	int newOrientation = (pageWidthSpinBox->value() > pageHeightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != pageOrientationComboBox->currentIndex())
	{
		pageOrientationComboBox->blockSignals(true);
		pageOrientationComboBox->setCurrentIndex(newOrientation);
		pageOrientationComboBox->blockSignals(false);
	}
}

void Prefs_DocumentSetup::setPageHeight(double h)
{
	pageH = pageHeightSpinBox->value() / unitRatio;
	marginsWidget->setPageHeight(pageH);
	QString psText=pageSizeComboBox->currentText();
	if (psText!=CommonStrings::trCustomPageSize && psText!=CommonStrings::customPageSize)
		pageSizeComboBox->setCurrentIndex(pageSizeComboBox->count()-1);
	int newOrientation = (pageWidthSpinBox->value() > pageHeightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != pageOrientationComboBox->currentIndex())
	{
		pageOrientationComboBox->blockSignals(true);
		pageOrientationComboBox->setCurrentIndex(newOrientation);
		pageOrientationComboBox->blockSignals(false);
	}
}

void Prefs_DocumentSetup::setPageOrientation(int orientation)
{
	setSize(pageSizeComboBox->currentText());
	pageWidthSpinBox->blockSignals(true);
	pageHeightSpinBox->blockSignals(true);
	if (orientation == 0)
	{
		if (pageSizeComboBox->currentText() == CommonStrings::trCustomPageSize)
		{
			double w = pageWidthSpinBox->value(), h = pageHeightSpinBox->value();
			pageWidthSpinBox->setValue((orientation == portraitPage) ? qMin(w, h) : qMax(w, h));
			pageHeightSpinBox->setValue((orientation == portraitPage) ? qMax(w, h) : qMin(w, h));
		}
	}
	else
	{
		double w = pageWidthSpinBox->value(), h = pageHeightSpinBox->value();
		pageWidthSpinBox->setValue((orientation == portraitPage) ? qMin(w, h) : qMax(w, h));
		pageHeightSpinBox->setValue((orientation == portraitPage) ? qMax(w, h) : qMin(w, h));
	}
	pageW = pageWidthSpinBox->value() / unitRatio;
	pageH = pageHeightSpinBox->value() / unitRatio;
	pageWidthSpinBox->blockSignals(false);
	pageHeightSpinBox->blockSignals(false);
}

void Prefs_DocumentSetup::setPageSize()
{
	setPageOrientation(pageOrientationComboBox->currentIndex());
}

void Prefs_DocumentSetup::setSize(const QString &newSize)
{
	pageW = pageWidthSpinBox->value() / unitRatio;
	pageH = pageHeightSpinBox->value() / unitRatio;
	PageSize *ps2=new PageSize(newSize);

	prefsPageSizeName=ps2->name();
	if (newSize != CommonStrings::trCustomPageSize)
	{
		pageW = ps2->width();
		pageH = ps2->height();
	}
	else
		prefsPageSizeName = CommonStrings::customPageSize;
	pageWidthSpinBox->blockSignals(true);
	pageHeightSpinBox->blockSignals(true);
	pageWidthSpinBox->setValue(pageW * unitRatio);
	pageHeightSpinBox->setValue(pageH * unitRatio);
	marginsWidget->setPageHeight(pageH);
	marginsWidget->setPageWidth(pageW);
	marginsWidget->setPageSize(newSize);
	pageWidthSpinBox->blockSignals(false);
	pageHeightSpinBox->blockSignals(false);
	delete ps2;
}
