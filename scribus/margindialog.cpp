/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <QPixmap>
#include <Q3VBoxLayout>

#include "margindialog.h"
//#include "margindialog.moc"
#include "marginWidget.h"
#include "pagestructs.h"
#include "page.h"
#include "scribusdoc.h"
#include "units.h"
#include "scrspinbox.h"
#include "pagesize.h"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

MarginDialog::MarginDialog( QWidget* parent, ScribusDoc* doc ) : QDialog( parent, "MarginDialog", true, 0 )
{
	setCaption( tr( "Manage Page Properties" ) );
	setIcon(loadIcon("AppIcon.png"));
	unitRatio = doc->unitRatio();
	dialogLayout = new Q3VBoxLayout( this, 10, 5);
	
	dsGroupBox7 = new Q3GroupBox( this, "GroupBox7" );
	dsGroupBox7->setTitle( tr( "Page Size" ) );
	dsGroupBox7->setColumnLayout(0, Qt::Vertical );
	dsGroupBox7->layout()->setSpacing( 0 );
	dsGroupBox7->layout()->setMargin( 0 );
	dsGroupBox7Layout = new Q3GridLayout( dsGroupBox7->layout() );
	dsGroupBox7Layout->setAlignment( Qt::AlignTop );
	dsGroupBox7Layout->setSpacing( 5 );
	dsGroupBox7Layout->setMargin( 10 );
	TextLabel1 = new QLabel( tr( "&Size:" ), dsGroupBox7, "TextLabel1" );
	dsGroupBox7Layout->addMultiCellWidget( TextLabel1, 0, 0, 0, 1 );
	PageSize *ps=new PageSize(doc->currentPage()->m_pageSize);
	sizeQComboBox = new QComboBox( true, dsGroupBox7, "ComboBox1" );
	sizeQComboBox->setEditable(false);
	QStringList pageSizes=ps->sizeList();
	sizeQComboBox->insertStringList(ps->sizeTRList());
	sizeQComboBox->insertItem( CommonStrings::trCustomPageSize );
	prefsPageSizeName=ps->name();
	int sizeIndex = pageSizes.findIndex(ps->nameTR());
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
	orientationQComboBox->setCurrentItem(doc->currentPage()->PageOri );
	oldOri = doc->currentPage()->PageOri;
	TextLabel2->setBuddy(orientationQComboBox);
	dsGroupBox7Layout->addMultiCellWidget( orientationQComboBox, 1, 1, 2, 3 );
	widthSpinBox = new ScrSpinBox( 1, 100000, dsGroupBox7, doc->unitIndex() );
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7, "widthLabel" );
	widthSpinBox->setValue(doc->currentPage()->width() * doc->unitRatio());
	widthQLabel->setBuddy(widthSpinBox);
	dsGroupBox7Layout->addWidget( widthQLabel, 2, 0 );
	dsGroupBox7Layout->addWidget( widthSpinBox, 2, 1 );
	heightSpinBox = new ScrSpinBox( 1, 100000, dsGroupBox7, doc->unitIndex() );
	heightSpinBox->setValue(doc->currentPage()->height() * doc->unitRatio());
	heightQLabel = new QLabel(heightSpinBox,  tr( "&Height:" ), dsGroupBox7, "heightLabel" );
	dsGroupBox7Layout->addWidget( heightQLabel, 2, 2 );
	dsGroupBox7Layout->addWidget( heightSpinBox, 2, 3 );
	moveObjects = new QCheckBox( dsGroupBox7, "moveObjects" );
	moveObjects->setText( tr( "Move Objects with their Page" ) );
	moveObjects->setChecked( true );
	dsGroupBox7Layout->addMultiCellWidget( moveObjects, 3, 3, 0, 3 );
	Links=0;
	if ((doc->currentPageLayout != singlePage) && (doc->masterPageMode()))
	{
		TextLabel3 = new QLabel( tr( "Type:" ), dsGroupBox7, "TextLabel3" );
		dsGroupBox7Layout->addMultiCellWidget( TextLabel3, 4, 4, 0, 1 );
		Links = new QComboBox( true, dsGroupBox7, "links" );
		QStringList::Iterator pNames;
		for(pNames = doc->pageSets[doc->currentPageLayout].pageNames.begin(); pNames != doc->pageSets[doc->currentPageLayout].pageNames.end(); ++pNames )
		{
			//Links->insertItem((*pNames));
			Links->insertItem(CommonStrings::translatePageSetLocString((*pNames)));
		}
		Links->setEditable(false);
		dsGroupBox7Layout->addMultiCellWidget( Links, 4, 4, 2, 3 );
		if (doc->currentPage()->LeftPg == 0)
			Links->setCurrentItem(Links->count()-1);
		else if (doc->currentPage()->LeftPg == 1)
			Links->setCurrentItem(0);
		else
			Links->setCurrentItem(doc->currentPage()->LeftPg-1);
	}
	dialogLayout->addWidget( dsGroupBox7 );
	
	GroupRand = new MarginWidget(this,  tr( "Margin Guides" ), &doc->currentPage()->initialMargins, doc->unitIndex(), false, false);
	GroupRand->setPageWidthHeight(doc->currentPage()->width(), doc->currentPage()->height());
	GroupRand->setFacingPages(!(doc->currentPageLayout == singlePage), doc->locationOfPage(doc->currentPage()->pageNr()));
	dialogLayout->addWidget( GroupRand );

	groupMaster = new Q3GroupBox( this, "groupMaster" );
	groupMaster->setTitle( tr( "Other Settings" ) );
	groupMaster->setColumnLayout(0, Qt::Vertical );
	groupMaster->layout()->setSpacing( 0 );
	groupMaster->layout()->setMargin( 0 );
	masterLayout = new Q3HBoxLayout( groupMaster->layout() );
	masterLayout->setAlignment( Qt::AlignTop );
	masterLayout->setSpacing( 5 );
	masterLayout->setMargin( 10 );
	masterPageLabel = new QLabel( groupMaster, "masterPageLabel" );
	masterPageLabel->setText( tr( "Master Page:" ) );
	masterLayout->addWidget( masterPageLabel );
	masterPageComboBox = new QComboBox( false, groupMaster, "masterPageComboBox" );
	QString Nam = doc->currentPage()->MPageNam;
	QString na = Nam == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : Nam, in;
	int cc = 0;
	for (QMap<QString,int>::Iterator it = doc->MasterNames.begin(); it != doc->MasterNames.end(); ++it)
	{
		in = it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key();
		masterPageComboBox->insertItem(in);
		if (in == na)
			masterPageComboBox->setCurrentItem(cc);
		++cc;
	}
	masterLayout->addWidget( masterPageComboBox );
	dialogLayout->addWidget( groupMaster );
	if (doc->masterPageMode())
		groupMaster->hide();

	okCancelLayout = new Q3HBoxLayout;
	okCancelLayout->setSpacing( 6 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton" );
	okButton->setDefault( true );
	okCancelLayout->addWidget(okButton);
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "cancelButton" );
	cancelButton->setDefault( false );
	okCancelLayout->addWidget(cancelButton);
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());
	pageWidth = widthSpinBox->value() / unitRatio;
	pageHeight = heightSpinBox->value() / unitRatio;
	if (sizeQComboBox->currentText() == CommonStrings::trCustomPageSize)
	{
		heightSpinBox->setEnabled( true );
		widthSpinBox->setEnabled( true );
	}
	else
	{
		heightSpinBox->setEnabled( false );
		widthSpinBox->setEnabled( false );
	}
	delete ps;
	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(orientationQComboBox, SIGNAL(activated(int)), this, SLOT(setOrien(int)));
	connect(sizeQComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	
	setPageSize();
	setMinimumSize(minimumSizeHint());
	setMaximumSize(minimumSizeHint());
	resize(minimumSizeHint());
}

void MarginDialog::setPageWidth(double)
{
	pageWidth = widthSpinBox->value() / unitRatio;
	GroupRand->setPageWidth(pageWidth);
}

void MarginDialog::setPageHeight(double)
{
	pageHeight = heightSpinBox->value() / unitRatio;
	GroupRand->setPageHeight(pageHeight);
}

void MarginDialog::setPageSize()
{
	if (sizeQComboBox->currentText() != CommonStrings::trCustomPageSize)
		oldOri++;
	setOrien(orientationQComboBox->currentItem());
}

void MarginDialog::setSize(const QString & gr)
{
	pageWidth = widthSpinBox->value() / unitRatio;
	pageHeight = heightSpinBox->value() / unitRatio;
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
		pageWidth = ps2->width();
		pageHeight = ps2->height();
	}
	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	widthSpinBox->setValue(pageWidth * unitRatio);
	heightSpinBox->setValue(pageHeight * unitRatio);
	GroupRand->setPageHeight(pageHeight);
	GroupRand->setPageWidth(pageWidth);
	GroupRand->setPageSize(gr);
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	delete ps2;
}

void MarginDialog::setOrien(int ori)
{
	double br;
	setSize(sizeQComboBox->currentText());
	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	if ((sizeQComboBox->currentText() == CommonStrings::trCustomPageSize) && (ori != oldOri))
	{
		br = widthSpinBox->value();
		widthSpinBox->setValue(heightSpinBox->value());
		heightSpinBox->setValue(br);
	}
	else
	{
		if ((ori != 0) && (ori != oldOri))
		{
			br = widthSpinBox->value();
			widthSpinBox->setValue(heightSpinBox->value());
			heightSpinBox->setValue(br);
		}
	}
	oldOri = ori;
	pageWidth = widthSpinBox->value() / unitRatio;
	pageHeight = heightSpinBox->value() / unitRatio;
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
}

int MarginDialog::pageOrder()
{
	int lp=0;
	if (Links!=0)
		lp = Links->currentItem();
	if (lp == 0)
		lp = 1;
	else if (lp == static_cast<int>(Links->count()-1))
		lp = 0;
	else
		lp++;
	return lp;
}

double MarginDialog::getPageWidth()
{
	return pageWidth;
}

double MarginDialog::getPageHeight()
{
	return pageHeight;
}

int MarginDialog::getPageOrientation()
{
	return orientationQComboBox->currentItem();
}

QString MarginDialog::getpPrefsPageSizeName()
{
	return prefsPageSizeName;
}

bool MarginDialog::getMoveObjects()
{
	return moveObjects->isChecked();
}

double MarginDialog::top()
{
	return GroupRand->top();
}

double MarginDialog::bottom()
{
	return GroupRand->bottom();
}

double MarginDialog::left()
{
	return GroupRand->left();
}

double MarginDialog::right()
{
	return GroupRand->right();
}

QString MarginDialog::masterPage()
{
	return masterPageComboBox->currentText();
}
