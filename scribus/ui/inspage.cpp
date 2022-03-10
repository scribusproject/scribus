/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "inspage.h"

#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QScopedPointer>

#include "commonstrings.h"
#include "iconmanager.h"
#include "pagesize.h"
#include "scpage.h"
#include "scribusdoc.h"
#include "scrspinbox.h"
#include "units.h"
#include "util.h"

InsPage::InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages)
	: QDialog(parent, Qt::WindowFlags())
{
	m_doc = currentDoc;
	masterPageCombos.clear();

	setModal(true);
	setWindowTitle( tr( "Insert Page" ) );
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));

	dialogLayout = new QVBoxLayout(this);
	dialogLayout->setSpacing(6);
	dialogLayout->setContentsMargins(9, 9, 9, 9);

	whereLayout = new QGridLayout();
	whereLayout->setSpacing(6);
	whereLayout->setContentsMargins(0, 0, 0, 0);

	insCountData = new QSpinBox(this);
	insCountData->setMinimum(1);
	insCountData->setMaximum(999);
	insCountData->setValue(1);
	insCountLabel = new QLabel(tr( "&Insert" ), this );
	insCountLabel->setBuddy(insCountData);
	whereLayout->addWidget(insCountLabel, 0, 0);
	whereLayout->addWidget(insCountData, 0, 1);

	pagesLabel = new QLabel( tr( "Page(s)" ), this);
	whereLayout->addWidget(pagesLabel, 0, 2);

	insWhereData = new QComboBox( this );
	insWhereData->addItem( tr("before Page"));
	insWhereData->addItem( tr("after Page"));
	insWhereData->addItem( tr("at End"));
	insWhereData->setCurrentIndex(2);
	whereLayout->addWidget(insWhereData, 1, 0, 1, 2);

	insWherePageData = new QSpinBox(this);
	insWherePageData->setMinimum(1);
	insWherePageData->setMaximum(maxPages);
	insWherePageData->setValue(currentPage + 1);
	insWherePageData->setDisabled(true);

	whereLayout->addWidget(insWherePageData, 1, 2);
	whereLayout->addItem(new QSpacerItem(insCountLabel->fontMetrics().horizontalAdvance( tr( "&Insert" )), 0), 0, 0);
	dialogLayout->addLayout(whereLayout);
	
	masterPageLabel = nullptr;
	masterPageGroup = new QGroupBox( this);
	masterPageGroup->setTitle( tr( "Master Pages" ) );
	masterPageLayout = new QGridLayout( masterPageGroup );
	masterPageLayout->setAlignment(Qt::AlignTop);
	masterPageLayout->setSpacing(6);
	masterPageLayout->setContentsMargins(9, 9, 9, 9);
	if (m_doc->pagePositioning() == 0)
	{
		QComboBox* pageData = new QComboBox(masterPageGroup);
		for (QMap<QString, int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
		{
			pageData->addItem(it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key(), it.key());
		}
		if (m_doc->MasterNames.contains( CommonStrings::trMasterPageNormal))
			setCurrentComboItem(pageData, CommonStrings::trMasterPageNormal);
		masterPageLabel = new QLabel(tr("&Master Page:"), masterPageGroup);
		masterPageLabel->setBuddy(pageData);
		masterPageLayout->addWidget(masterPageLabel, 0, 0);
		masterPageLayout->addWidget(pageData, 0, 1);
		masterPageCombos.append(pageData);
	}
	else
	{
		int row = 0;
		const PageSet& currentPageSet = m_doc->pageSets()[m_doc->pagePositioning()];
		for (int mp = 0; mp < currentPageSet.pageNames.count(); ++mp)
		{
			QComboBox* pageData = new QComboBox(masterPageGroup);
//			for (QMap<QString, int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
//				pageData->insertItem(it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key());
			if (mp == 0)
			{
				bool conNam = m_doc->MasterNames.contains(CommonStrings::trMasterPageNormalLeft);
				for (QMap<QString, int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
				{
					if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
						pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
					else if ((it.key() == CommonStrings::trMasterPageNormal) && (!conNam))
						pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
					else
					{
						if (m_doc->MasterPages.at(it.value())->LeftPg == 1)
							pageData->addItem(it.key(), it.key());
					}
				}
				if (m_doc->MasterNames.contains(CommonStrings::trMasterPageNormalLeft))
					setCurrentComboItem(pageData, CommonStrings::trMasterPageNormalLeft);
			}
			else if (mp == 1)
			{
				if (currentPageSet.pageNames.count() > 2)
				{
					bool conNam = m_doc->MasterNames.contains( CommonStrings::trMasterPageNormalMiddle);
					for (QMap<QString, int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
					{
						if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
						else if ((it.key() == CommonStrings::trMasterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
						else
						{
							if ((m_doc->MasterPages.at(it.value())->LeftPg != 0) && (m_doc->MasterPages.at(it.value())->LeftPg != 1))
								pageData->addItem(it.key(), it.key());
						}
					}
					if (m_doc->MasterNames.contains(CommonStrings::trMasterPageNormalMiddle))
						setCurrentComboItem(pageData, CommonStrings::trMasterPageNormalMiddle);
				}
				else
				{
					bool conNam = m_doc->MasterNames.contains( CommonStrings::trMasterPageNormalRight);
					for (QMap<QString, int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
					{
						if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
						else if ((it.key() == CommonStrings::trMasterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
						else
						{
							if (m_doc->MasterPages.at(it.value())->LeftPg == 0)
								pageData->addItem(it.key(), it.key());
						}
					}
					if (m_doc->MasterNames.contains(CommonStrings::trMasterPageNormalRight))
						setCurrentComboItem(pageData, CommonStrings::trMasterPageNormalRight);
				}
			}
			else if (mp == 2)
			{
				if (currentPageSet.pageNames.count() > 3)
				{
					bool conNam = m_doc->MasterNames.contains(CommonStrings::trMasterPageNormalMiddle);
					for (QMap<QString, int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
					{
						if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
						else if ((it.key() == CommonStrings::trMasterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
						else
						{
							if ((m_doc->MasterPages.at(it.value())->LeftPg != 0) && (m_doc->MasterPages.at(it.value())->LeftPg != 1))
								pageData->addItem(it.key(), it.key());
						}
					}
					if (m_doc->MasterNames.contains(CommonStrings::trMasterPageNormalMiddle))
						setCurrentComboItem(pageData, CommonStrings::trMasterPageNormalMiddle);
				}
				else
				{
					bool conNam = m_doc->MasterNames.contains(CommonStrings::trMasterPageNormalRight);
					for (QMap<QString, int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
					{
						if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
						else if ((it.key() == CommonStrings::trMasterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
						else
						{
							if (m_doc->MasterPages.at(it.value())->LeftPg == 0)
								pageData->addItem(it.key(), it.key());
						}
					}
					if (m_doc->MasterNames.contains( CommonStrings::trMasterPageNormalRight))
						setCurrentComboItem(pageData, CommonStrings::trMasterPageNormalRight);
				}
			}
			else if (mp == 3)
			{
				bool conNam = m_doc->MasterNames.contains( CommonStrings::trMasterPageNormalRight);
				for (QMap<QString, int>::Iterator it = m_doc->MasterNames.begin(); it != m_doc->MasterNames.end(); ++it)
				{
					if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
						pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
					else if ((it.key() == CommonStrings::trMasterPageNormal) && (!conNam))
						pageData->addItem(CommonStrings::trMasterPageNormal, it.key());
					else
					{
						if (m_doc->MasterPages.at(it.value())->LeftPg == 0)
							pageData->addItem(it.key(), it.key());
					}
				}
				if (m_doc->MasterNames.contains( CommonStrings::trMasterPageNormalRight))
					setCurrentComboItem(pageData, CommonStrings::trMasterPageNormalRight);
			}
			QString transLabel = currentPageSet.pageNames[mp];
			QLabel* pageLabel = new QLabel(CommonStrings::translatePageSetLocString(transLabel), masterPageGroup);
			pageLabel->setBuddy(pageData);
			masterPageLayout->addWidget(pageLabel, row, 0 );
			masterPageLayout->addWidget(pageData, row, 1);
			row++;
			masterPageCombos.append(pageData);
		}
	}
	dialogLayout->addWidget(masterPageGroup);
	overrideMPSizingCheckBox = new QCheckBox( tr("Override Master Page Sizing"));
	dialogLayout->addWidget(overrideMPSizingCheckBox);
	dsGroupBox7 = new QGroupBox( this );
	dsGroupBox7->setTitle( tr( "Page Size" ) );
	dsGroupBox7Layout = new QGridLayout(dsGroupBox7);
	dsGroupBox7Layout->setSpacing(6);
	dsGroupBox7Layout->setContentsMargins(9, 9, 9, 9);
	textLabel1 = new QLabel( tr( "&Size:" ), dsGroupBox7);
	dsGroupBox7Layout->addWidget(textLabel1, 0, 0);

	QScopedPointer<PageSize> ps(new PageSize(m_doc->pageSize()));
	prefsPageSizeName = ps->name();
	sizeQComboBox = new QComboBox(dsGroupBox7);
	QStringList insertList(ps->activeSizeTRList());
	if (insertList.indexOf(prefsPageSizeName) == -1)
		insertList << prefsPageSizeName;
	insertList.sort();
	insertList << CommonStrings::trCustomPageSize;
	sizeQComboBox->addItems(insertList);
	int sizeIndex = insertList.indexOf(ps->nameTR());
	if (sizeIndex != -1)
		sizeQComboBox->setCurrentIndex(sizeIndex);
	else
		sizeQComboBox->setCurrentIndex(sizeQComboBox->count() - 1);

	textLabel1->setBuddy(sizeQComboBox);
	dsGroupBox7Layout->addWidget(sizeQComboBox, 0, 1, 1, 3);

	textLabel2 = new QLabel( tr( "Orie&ntation:" ), dsGroupBox7);
	dsGroupBox7Layout->addWidget(textLabel2, 1, 0);

	orientationQComboBox = new QComboBox(dsGroupBox7);
	orientationQComboBox->addItem( tr( "Portrait" ) );
	orientationQComboBox->addItem( tr( "Landscape" ) );
	orientationQComboBox->setCurrentIndex(m_doc->pageOrientation() );
	textLabel2->setBuddy(orientationQComboBox);
	dsGroupBox7Layout->addWidget(orientationQComboBox, 1, 1, 1, 3);

	widthSpinBox = new ScrSpinBox(1, 10000, dsGroupBox7, m_doc->unitIndex());
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7);
	widthSpinBox->setValue(m_doc->pageWidth() * m_doc->unitRatio());
	widthQLabel->setBuddy(widthSpinBox);
	dsGroupBox7Layout->addWidget(widthQLabel, 2, 0);
	dsGroupBox7Layout->addWidget(widthSpinBox, 2, 1);

	heightSpinBox = new ScrSpinBox(1, 10000, dsGroupBox7, m_doc->unitIndex());
	heightSpinBox->setValue(m_doc->pageHeight() * m_doc->unitRatio());
	heightQLabel = new QLabel( tr( "&Height:" ), dsGroupBox7);
	heightQLabel->setBuddy(heightSpinBox);
	dsGroupBox7Layout->addWidget(heightQLabel, 3, 0);
	dsGroupBox7Layout->addWidget(heightSpinBox, 3, 1);

	moveObjectsCheckBox = new QCheckBox( dsGroupBox7);
	moveObjectsCheckBox->setText( tr( "Move Objects with their Page" ) );
	moveObjectsCheckBox->setChecked(true);
	dsGroupBox7Layout->addWidget(moveObjectsCheckBox, 4, 0, 1, 4);
	dialogLayout->addWidget(dsGroupBox7);

	dsGroupBox7->setEnabled(false);
	bool b = (sizeQComboBox->currentText() == CommonStrings::trCustomPageSize);
	heightSpinBox->setEnabled(b);
	widthSpinBox->setEnabled(b);

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	dialogLayout->addWidget(buttonBox);
	setMaximumSize(sizeHint());
	m_unitRatio = m_doc->unitRatio();

	// signals and slots connections
	connect(insWhereData, SIGNAL( activated(int) ), this, SLOT( insWherePageDataDisable(int) ) );
	connect(buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
	connect(buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
	connect(orientationQComboBox, SIGNAL(activated(int)), this, SLOT(setOrientation(int)));
	connect(sizeQComboBox, SIGNAL(activated(const QString &)), this, SLOT(setSize(const QString &)));
	connect(overrideMPSizingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableSizingControls(int)));
}

void InsPage::setSize(const QString & gr)
{
	PageSize ps2(gr);

	widthSpinBox->setEnabled(false);
	heightSpinBox->setEnabled(false);
	prefsPageSizeName = ps2.name();
	if (gr == CommonStrings::trCustomPageSize)
	{
		widthSpinBox->setEnabled(true);
		heightSpinBox->setEnabled(true);
		prefsPageSizeName = CommonStrings::customPageSize;
	}
	else
	{
		widthSpinBox->setValue(ps2.width() * m_unitRatio);
		heightSpinBox->setValue(ps2.height() * m_unitRatio);
	}
}

void InsPage::setOrientation(int ori)
{
	double br;
	setSize(sizeQComboBox->currentText());
	if (ori == 0)
	{
		if (sizeQComboBox->currentText() == CommonStrings::trCustomPageSize)
		{
			br = widthSpinBox->value();
			widthSpinBox->setValue(heightSpinBox->value());
			heightSpinBox->setValue(br);
		}
	}
	else
	{
		br = widthSpinBox->value();
		widthSpinBox->setValue(heightSpinBox->value());
		heightSpinBox->setValue(br);
	}
}

QStringList InsPage::getMasterPages() const
{
	QStringList ret;
	for (int i = 0; i < masterPageCombos.count(); ++i)
	{
		int currentIndex = masterPageCombos.at(i)->currentIndex();
		QVariant pageVar = masterPageCombos.at(i)->itemData(currentIndex);
		ret.append(pageVar.toString());
	}
	return ret;
}

QString InsPage::getMasterPageN(uint n) const
{
	QComboBox* comboBox = masterPageCombos.at(n);
	int currentIndex = comboBox->currentIndex();
	return comboBox->itemData(currentIndex).toString();
}

int InsPage::getWhere() const
{
	return insWhereData->currentIndex();
}

int InsPage::getWherePage() const
{
	if (insWhereData->currentIndex() == 2)
		return m_doc->Pages->count();
	return insWherePageData->value();
}

int InsPage::getCount() const
{
	return insCountData->value();
}

double InsPage::pageWidth() const
{
	return widthSpinBox->value() / m_unitRatio;
}

double InsPage::pageHeight() const
{
	return heightSpinBox->value() / m_unitRatio;
}

int InsPage::orientation() const
{
	return orientationQComboBox->currentIndex();
}

bool InsPage::overrideMasterPageSizing() const
{
	return (overrideMPSizingCheckBox->checkState() == Qt::Checked);
}

bool InsPage::moveObjects() const
{
	return moveObjectsCheckBox->isChecked();
}

void InsPage::insWherePageDataDisable(int index)
{
	insWherePageData->setDisabled((index == 2));
}

void InsPage::enableSizingControls(int state)
{
	dsGroupBox7->setEnabled(state == Qt::Checked);
}
