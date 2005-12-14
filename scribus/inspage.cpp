#include <qdialog.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include "scribusdoc.h"

#include "inspage.h"
#include "inspage.moc"
#include "page.h"
#include "units.h"
#include "mspinbox.h"
#include "pagesize.h"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

InsPage::InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages)
		: QDialog( parent, "InsPage", true, 0 )
{
	masterPageCombos.clear();
	setCaption( tr( "Insert Page" ) );
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this, 5, 5 );
	whereLayout = new QGridLayout;
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
	insWhereData->insertItem( tr("before Page"));
	insWhereData->insertItem( tr("after Page"));
	insWhereData->insertItem( tr("at End"));
	insWhereData->setCurrentItem(2);
	whereLayout->addMultiCellWidget( insWhereData, 1, 1, 0, 1 );

	insWherePageData = new QSpinBox( 1, maxPages, 1, this, "insWherePageData" );
	insWherePageData->setValue( currentPage+1 );
	insWherePageData->setDisabled( true );

	whereLayout->addWidget( insWherePageData, 1, 2 );

	whereLayout->addColSpacing(0, insCountLabel->fontMetrics().width( tr( "&Insert" )));
	dialogLayout->addLayout( whereLayout );
	
	masterPageGroup = new QGroupBox( this, "masterPageGroup" );
	masterPageGroup->setTitle( tr( "Master Pages" ) );
	masterPageGroup->setColumnLayout(0, Qt::Vertical );
	masterPageGroup->layout()->setSpacing( 0 );
	masterPageGroup->layout()->setMargin( 0 );
	masterPageLayout = new QGridLayout( masterPageGroup->layout() );
	masterPageLayout->setAlignment( Qt::AlignTop );
	masterPageLayout->setSpacing( 5 );
	masterPageLayout->setMargin( 5 );
	if (currentDoc->currentPageLayout == 0)
	{
		QComboBox* pageData = new QComboBox(false, masterPageGroup, "masterPageData");
		for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
			pageData->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());
		masterPageLabel = new QLabel(pageData,  tr("&Master Page:"), masterPageGroup, "text");
		masterPageLayout->addWidget( masterPageLabel, 0, 0 );
		masterPageLayout->addWidget(pageData, 0, 1);
		masterPageCombos.append(pageData);
	}
	else
	{
		int row = 0;
		for (uint mp = 0; mp < currentDoc->pageSets[currentDoc->currentPageLayout].pageNames.count(); ++mp)
		{
			QComboBox* pageData = new QComboBox(false, masterPageGroup, "pageData");
			for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
				pageData->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());
			QLabel* pageLabel = new QLabel(pageData,  currentDoc->pageSets[currentDoc->currentPageLayout].pageNames[mp], masterPageGroup, "text");
			masterPageLayout->addWidget(pageLabel, row, 0 );
			masterPageLayout->addWidget(pageData, row, 1);
			row++;
			masterPageCombos.append(pageData);
		}
	}
	dialogLayout->addWidget(masterPageGroup);

	dsGroupBox7 = new QGroupBox( this, "GroupBox7" );
	dsGroupBox7->setTitle( tr( "Page Size" ) );
	dsGroupBox7->setColumnLayout(0, Qt::Vertical );
	dsGroupBox7->layout()->setSpacing( 0 );
	dsGroupBox7->layout()->setMargin( 0 );
	dsGroupBox7Layout = new QGridLayout( dsGroupBox7->layout() );
	dsGroupBox7Layout->setAlignment( Qt::AlignTop );
	dsGroupBox7Layout->setSpacing( 5 );
	dsGroupBox7Layout->setMargin( 5 );
	TextLabel1 = new QLabel( tr( "&Size:" ), dsGroupBox7, "TextLabel1" );
	dsGroupBox7Layout->addMultiCellWidget( TextLabel1, 0, 0, 0, 1 );
	PageSize *ps=new PageSize(currentDoc->PageSize);
	sizeQComboBox = new QComboBox( true, dsGroupBox7, "ComboBox1" );
	sizeQComboBox->setEditable(false);
	QStringList pageSizes=ps->getPageSizeList();
	sizeQComboBox->insertStringList(ps->getTrPageSizeList());
	sizeQComboBox->insertItem( tr( "Custom" ) );
	prefsPageSizeName=ps->getPageName();
	int sizeIndex = pageSizes.findIndex(ps->getPageText());
	if (sizeIndex != -1)
		sizeQComboBox->setCurrentItem(sizeIndex);
	else
		sizeQComboBox->setCurrentItem(sizeQComboBox->count()-1);
	TextLabel1->setBuddy(sizeQComboBox);
	dsGroupBox7Layout->addMultiCellWidget(sizeQComboBox, 0, 0, 2, 3);
	TextLabel2 = new QLabel( tr( "Orie&ntation:" ), dsGroupBox7, "TextLabel2" );
	dsGroupBox7Layout->addMultiCellWidget( TextLabel2, 1, 1, 0, 1 );
	orientationQComboBox = new QComboBox( true, dsGroupBox7, "ComboBox2" );
	orientationQComboBox->insertItem( tr( "Portrait" ) );
	orientationQComboBox->insertItem( tr( "Landscape" ) );
	orientationQComboBox->setEditable(false);
	orientationQComboBox->setCurrentItem(currentDoc->PageOri );
	TextLabel2->setBuddy(orientationQComboBox);
	dsGroupBox7Layout->addMultiCellWidget( orientationQComboBox, 1, 1, 2, 3 );
	widthMSpinBox = new MSpinBox( 1, 10000, dsGroupBox7, unitGetDecimalsFromIndex(currentDoc->unitIndex()) );
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7, "widthLabel" );
	widthMSpinBox->setSuffix(unitGetSuffixFromIndex(currentDoc->unitIndex()));
	widthMSpinBox->setValue(currentDoc->pageWidth * currentDoc->unitRatio());
	widthQLabel->setBuddy(widthMSpinBox);
	dsGroupBox7Layout->addWidget( widthQLabel, 2, 0 );
	dsGroupBox7Layout->addWidget( widthMSpinBox, 2, 1 );
	heightMSpinBox = new MSpinBox( 1, 10000, dsGroupBox7, unitGetDecimalsFromIndex(currentDoc->unitIndex()) );
	heightMSpinBox->setSuffix(unitGetSuffixFromIndex(currentDoc->unitIndex()));
	heightMSpinBox->setValue(currentDoc->pageHeight * currentDoc->unitRatio());
	heightQLabel = new QLabel(heightMSpinBox,  tr( "&Height:" ), dsGroupBox7, "heightLabel" );
	dsGroupBox7Layout->addWidget( heightQLabel, 2, 2 );
	dsGroupBox7Layout->addWidget( heightMSpinBox, 2, 3 );
	moveObjects = new QCheckBox( dsGroupBox7, "moveObjects" );
	moveObjects->setText( tr( "Move Objects with their Page" ) );
	moveObjects->setChecked( true );
	dsGroupBox7Layout->addMultiCellWidget( moveObjects, 3, 3, 0, 3 );
	dialogLayout->addWidget( dsGroupBox7 );
	if (sizeQComboBox->currentText() == tr("Custom"))
	{
		heightMSpinBox->setEnabled( true );
		widthMSpinBox->setEnabled( true );
	}
	else
	{
		heightMSpinBox->setEnabled( false );
		widthMSpinBox->setEnabled( false );
	}
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
	widthMSpinBox->setEnabled(false);
	heightMSpinBox->setEnabled(false);
	PageSize *ps2 = new PageSize(gr);
	prefsPageSizeName = ps2->getPageName();
	if (gr == tr("Custom"))
	{
		widthMSpinBox->setEnabled(true);
		heightMSpinBox->setEnabled(true);
	}
	else
	{
		widthMSpinBox->setValue(ps2->getPageWidth() * unitRatio);
		heightMSpinBox->setValue(ps2->getPageHeight() * unitRatio);
	}
	delete ps2;
}

void InsPage::setOrien(int ori)
{
	double br;
	setSize(sizeQComboBox->currentText());
	if (ori == 0)
	{
		if (sizeQComboBox->currentText() == tr("Custom"))
		{
			br = widthMSpinBox->value();
			widthMSpinBox->setValue(heightMSpinBox->value());
			heightMSpinBox->setValue(br);
		}
	}
	else
	{
		br = widthMSpinBox->value();
		widthMSpinBox->setValue(heightMSpinBox->value());
		heightMSpinBox->setValue(br);
	}
}

const QStringList InsPage::getMasterPages()
{
	QStringList ret;
	for (uint n = 0; n < masterPageCombos.count(); ++n)
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
	return insWhereData->currentItem();
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
