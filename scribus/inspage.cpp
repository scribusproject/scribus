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
#include "scribusdoc.h"
#include "page.h"
#include "units.h"
#include "scrspinbox.h"
#include "pagesize.h"
#include "commonstrings.h"
#include "util_icon.h"

InsPage::InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages)
		: QDialog( parent, "InsPage", true, 0 )
{
	masterPageCombos.clear();
	setModal(true);
	setCaption( tr( "Insert Page" ) );
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout(this);
	dialogLayout->setSpacing( 5 );
	dialogLayout->setMargin( 5 );
	whereLayout = new QGridLayout();
	whereLayout->setSpacing( 5 );
	whereLayout->setMargin( 5 );
	insCountData = new QSpinBox( 1, 999, 1, this, "insCountData" );
	insCountData->setValue( 1 );
	insCountLabel = new QLabel( insCountData, tr( "&Insert" ), this, "insCountLabel" );
	whereLayout->addWidget( insCountLabel, 0, 0 );
	whereLayout->addWidget( insCountData, 0, 1 );
	pagesLabel = new QLabel( tr( "Page(s)" ), this, "pagesLabel" );
	whereLayout->addWidget( pagesLabel, 0, 2 );

	insWhereData = new QComboBox( false, this, "insWhereData" );
	insWhereData->addItem( tr("before Page"));
	insWhereData->addItem( tr("after Page"));
	insWhereData->addItem( tr("at End"));
	insWhereData->setCurrentIndex(2);
	whereLayout->addWidget( insWhereData, 1, 0, 1, 2 );

	insWherePageData = new QSpinBox( 1, maxPages, 1, this, "insWherePageData" );
	insWherePageData->setValue( currentPage+1 );
	insWherePageData->setDisabled( true );

	whereLayout->addWidget( insWherePageData, 1, 2 );

	whereLayout->addColSpacing(0, insCountLabel->fontMetrics().width( tr( "&Insert" )));
	dialogLayout->addLayout( whereLayout );
	
	masterPageGroup = new QGroupBox( this, "masterPageGroup" );
	masterPageGroup->setTitle( tr( "Master Pages" ) );
	masterPageLayout = new QGridLayout( masterPageGroup );
	masterPageLayout->setAlignment( Qt::AlignTop );
	masterPageLayout->setSpacing( 5 );
	masterPageLayout->setMargin( 5 );
	if (currentDoc->currentPageLayout == 0)
	{
		QComboBox* pageData = new QComboBox(false, masterPageGroup, "masterPageData");
		for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
			pageData->addItem(it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key());
		masterPageLabel = new QLabel(pageData,  tr("&Master Page:"), masterPageGroup, "text");
		masterPageLayout->addWidget( masterPageLabel, 0, 0 );
		masterPageLayout->addWidget(pageData, 0, 1);
		masterPageCombos.append(pageData);
	}
	else
	{
		int row = 0;
		for (int mp = 0; mp < currentDoc->pageSets[currentDoc->currentPageLayout].pageNames.count(); ++mp)
		{
			QComboBox* pageData = new QComboBox(false, masterPageGroup, "pageData");
//			for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
//				pageData->insertItem(it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key());
			if (mp == 0)
			{
				bool conNam = currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalLeft);
				for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
				{
					if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
						pageData->addItem(CommonStrings::trMasterPageNormal);
					else
					{
						if (currentDoc->MasterPages.at(it.data())->LeftPg == 1)
							pageData->addItem(it.key());
					}
				}
				if (currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalLeft))
					pageData->setCurrentText( CommonStrings::trMasterPageNormalLeft);
			}
			else if (mp == 1)
			{
				if (currentDoc->pageSets[currentDoc->currentPageLayout].pageNames.count() > 2)
				{
					bool conNam = currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalMiddle);
					for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
					{
						if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal);
						else
						{
							if ((currentDoc->MasterPages.at(it.data())->LeftPg != 0) && (currentDoc->MasterPages.at(it.data())->LeftPg != 1))
								pageData->addItem(it.key());
						}
					}
					if (currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalMiddle))
						pageData->setCurrentText( CommonStrings::trMasterPageNormalMiddle);
				}
				else
				{
					bool conNam = currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalRight);
					for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
					{
						if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal);
						else
						{
							if (currentDoc->MasterPages.at(it.data())->LeftPg == 0)
								pageData->addItem(it.key());
						}
					}
					if (currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalRight))
						pageData->setCurrentText( CommonStrings::trMasterPageNormalRight);
				}
			}
			else if (mp == 2)
			{
				if (currentDoc->pageSets[currentDoc->currentPageLayout].pageNames.count() > 3)
				{
					bool conNam = currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalMiddle);
					for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
					{
						if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal);
						else
						{
							if ((currentDoc->MasterPages.at(it.data())->LeftPg != 0) && (currentDoc->MasterPages.at(it.data())->LeftPg != 1))
								pageData->addItem(it.key());
						}
					}
					if (currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalMiddle))
						pageData->setCurrentText( CommonStrings::trMasterPageNormalMiddle);
				}
				else
				{
					bool conNam = currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalRight);
					for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
					{
						if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
							pageData->addItem(CommonStrings::trMasterPageNormal);
						else
						{
							if (currentDoc->MasterPages.at(it.data())->LeftPg == 0)
								pageData->addItem(it.key());
						}
					}
					if (currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalRight))
						pageData->setCurrentText( CommonStrings::trMasterPageNormalRight);
				}
			}
			else if (mp == 3)
			{
				bool conNam = currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalRight);
				for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
				{
					if ((it.key() == CommonStrings::masterPageNormal) && (!conNam))
						pageData->addItem(CommonStrings::trMasterPageNormal);
					else
					{
						if (currentDoc->MasterPages.at(it.data())->LeftPg == 0)
							pageData->addItem(it.key());
					}
				}
				if (currentDoc->MasterNames.contains( CommonStrings::trMasterPageNormalRight))
					pageData->setCurrentText( CommonStrings::trMasterPageNormalRight);
			}
			QString transLabel = currentDoc->pageSets[currentDoc->currentPageLayout].pageNames[mp];
			QLabel* pageLabel = new QLabel(pageData,  CommonStrings::translatePageSetLocString(transLabel), masterPageGroup, "text");
			masterPageLayout->addWidget(pageLabel, row, 0 );
			masterPageLayout->addWidget(pageData, row, 1);
			row++;
			masterPageCombos.append(pageData);
		}
	}
	dialogLayout->addWidget(masterPageGroup);

	dsGroupBox7 = new QGroupBox( this, "GroupBox7" );
	dsGroupBox7->setTitle( tr( "Page Size" ) );
	dsGroupBox7Layout = new QGridLayout( dsGroupBox7 );
	dsGroupBox7Layout->setSpacing( 5 );
	dsGroupBox7Layout->setMargin( 5 );
	TextLabel1 = new QLabel( tr( "&Size:" ), dsGroupBox7, "TextLabel1" );
	dsGroupBox7Layout->addWidget( TextLabel1, 0, 0);
	PageSize *ps=new PageSize(currentDoc->m_pageSize);
	sizeQComboBox = new QComboBox( true, dsGroupBox7, "ComboBox1" );
	sizeQComboBox->setEditable(false);
	QStringList pageSizes=ps->sizeList();
	sizeQComboBox->addItems(ps->sizeTRList());
	sizeQComboBox->addItem(CommonStrings::trCustomPageSize);
	prefsPageSizeName=ps->name();
	int sizeIndex = pageSizes.findIndex(ps->nameTR());
	if (sizeIndex != -1)
		sizeQComboBox->setCurrentIndex(sizeIndex);
	else
		sizeQComboBox->setCurrentIndex(sizeQComboBox->count()-1);
	TextLabel1->setBuddy(sizeQComboBox);
	dsGroupBox7Layout->addWidget(sizeQComboBox, 0, 1, 1, 3);
	TextLabel2 = new QLabel( tr( "Orie&ntation:" ), dsGroupBox7, "TextLabel2" );
	dsGroupBox7Layout->addWidget( TextLabel2, 1, 0);
	orientationQComboBox = new QComboBox( true, dsGroupBox7, "ComboBox2" );
	orientationQComboBox->addItem( tr( "Portrait" ) );
	orientationQComboBox->addItem( tr( "Landscape" ) );
	orientationQComboBox->setEditable(false);
	orientationQComboBox->setCurrentIndex(currentDoc->PageOri );
	TextLabel2->setBuddy(orientationQComboBox);
	dsGroupBox7Layout->addWidget( orientationQComboBox, 1, 1, 1, 3 );
	widthSpinBox = new ScrSpinBox( 1, 10000, dsGroupBox7, currentDoc->unitIndex() );
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7, "widthLabel" );
	widthSpinBox->setValue(currentDoc->pageWidth * currentDoc->unitRatio());
	widthQLabel->setBuddy(widthSpinBox);
	dsGroupBox7Layout->addWidget( widthQLabel, 2, 0 );
	dsGroupBox7Layout->addWidget( widthSpinBox, 2, 1 );
	heightSpinBox = new ScrSpinBox( 1, 10000, dsGroupBox7, currentDoc->unitIndex() );
	heightSpinBox->setValue(currentDoc->pageHeight * currentDoc->unitRatio());
	heightQLabel = new QLabel(heightSpinBox,  tr( "&Height:" ), dsGroupBox7, "heightLabel" );
	dsGroupBox7Layout->addWidget( heightQLabel, 2, 2 );
	dsGroupBox7Layout->addWidget( heightSpinBox, 2, 3 );
	moveObjects = new QCheckBox( dsGroupBox7, "moveObjects" );
	moveObjects->setText( tr( "Move Objects with their Page" ) );
	moveObjects->setChecked( true );
	dsGroupBox7Layout->addWidget( moveObjects, 3, 0, 1, 4 );
	dialogLayout->addWidget( dsGroupBox7 );
	bool b=(sizeQComboBox->currentText() == CommonStrings::trCustomPageSize);
	heightSpinBox->setEnabled( b );
	widthSpinBox->setEnabled( b );
	delete ps;

	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 5 );
	okCancelLayout->setMargin( 5 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );

	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton" );
	okButton->setDefault( true );
	okCancelLayout->addWidget( okButton );

	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "cancelButton" );
	okCancelLayout->addWidget( cancelButton );
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());
	unitRatio = currentDoc->unitRatio();

	// signals and slots connections
	connect( insWhereData, SIGNAL( activated(int) ), this, SLOT( insWherePageDataDisable(int) ) );
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(orientationQComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(sizeQComboBox, SIGNAL(activated(const QString &)), this, SLOT(setSize(const QString &)));
}

void InsPage::setSize(const QString & gr)
{
	widthSpinBox->setEnabled(false);
	heightSpinBox->setEnabled(false);
	PageSize *ps2 = new PageSize(gr);
	prefsPageSizeName = ps2->name();
	if (gr == CommonStrings::trCustomPageSize)
	{
		widthSpinBox->setEnabled(true);
		heightSpinBox->setEnabled(true);
		prefsPageSizeName = CommonStrings::customPageSize;
	}
	else
	{
		widthSpinBox->setValue(ps2->width() * unitRatio);
		heightSpinBox->setValue(ps2->height() * unitRatio);
	}
	delete ps2;
}

void InsPage::setOrien(int ori)
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

const QStringList InsPage::getMasterPages()
{
	QStringList ret;
	for (int n = 0; n < masterPageCombos.count(); ++n)
	{
		ret.append(masterPageCombos.at(n)->currentText());
	}
	return ret;
}

const QString InsPage::getMasterPageN(uint n)
{
	return masterPageCombos.at(n)->currentText();
}

const int InsPage::getWhere()
{
	return insWhereData->currentIndex();
}

const int InsPage::getWherePage()
{
	return insWherePageData->value();
}

const int InsPage::getCount()
{
	return insCountData->value();
}

void InsPage::insWherePageDataDisable(int index)
{
	insWherePageData->setDisabled((index==2));
}
