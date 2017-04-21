/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QPixmap>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QColorDialog>

#include "commonstrings.h"
#include "iconmanager.h"
#include "marginwidget.h"
#include "pagepropertiesdialog.h"
#include "pagesize.h"
#include "pagestructs.h"
#include "scpage.h"
#include "scribusdoc.h"
#include "scrspinbox.h"
#include "units.h"

PagePropertiesDialog::PagePropertiesDialog( QWidget* parent, ScribusDoc* doc ) : QDialog( parent)
{
	setModal(true);
	setWindowTitle( tr( "Manage Page Properties" ) );
	setWindowIcon(IconManager::instance()->loadIcon("AppIcon.png"));
	unitRatio = doc->unitRatio();
	dialogLayout = new QVBoxLayout(this);
	dialogLayout->setMargin(10);
	dialogLayout->setSpacing(5);
	
	dsGroupBox7 = new QGroupBox(this);
	dsGroupBox7->setTitle( tr( "Page Size" ) );
	dsGroupBox7Layout = new QGridLayout(dsGroupBox7);
	dsGroupBox7Layout->setAlignment( Qt::AlignTop );
	dsGroupBox7Layout->setSpacing( 5 );
	dsGroupBox7Layout->setMargin( 10 );
	TextLabel1 = new QLabel( tr( "&Size:" ), dsGroupBox7 );
	dsGroupBox7Layout->addWidget( TextLabel1, 0, 0, 1, 2 );

	PageSize ps(doc->currentPage()->m_pageSize);
	prefsPageSizeName=ps.name();
	sizeQComboBox = new QComboBox(dsGroupBox7);
	QStringList insertList(ps.activeSizeTRList());
	if (insertList.indexOf(prefsPageSizeName)==-1 && prefsPageSizeName!=CommonStrings::customPageSize)
		insertList<<prefsPageSizeName;
	insertList.sort();
	insertList<<CommonStrings::trCustomPageSize;
	sizeQComboBox->addItems(insertList);
	int sizeIndex = insertList.indexOf(ps.nameTR());
	if (sizeIndex != -1)
		sizeQComboBox->setCurrentIndex(sizeIndex);
	else
		sizeQComboBox->setCurrentIndex(sizeQComboBox->count()-1);

	TextLabel1->setBuddy(sizeQComboBox);
	dsGroupBox7Layout->addWidget(sizeQComboBox, 0, 2, 1, 2);
	TextLabel2 = new QLabel( tr( "Orie&ntation:" ), dsGroupBox7 );
	dsGroupBox7Layout->addWidget( TextLabel2, 1, 0, 1, 2 );
	orientationQComboBox = new QComboBox( dsGroupBox7 );
	orientationQComboBox->addItem( tr( "Portrait" ) );
	orientationQComboBox->addItem( tr( "Landscape" ) );
	orientationQComboBox->setEditable(false);
	orientationQComboBox->setCurrentIndex(doc->currentPage()->orientation() );
	oldOri = doc->currentPage()->orientation();
	TextLabel2->setBuddy(orientationQComboBox);
	dsGroupBox7Layout->addWidget( orientationQComboBox, 1, 2, 1, 2 );
	widthSpinBox = new ScrSpinBox( 1, 16777215, dsGroupBox7, doc->unitIndex() );
	widthQLabel = new QLabel( tr( "&Width:" ), dsGroupBox7 );
	widthSpinBox->setValue(doc->currentPage()->width() * doc->unitRatio());
	widthQLabel->setBuddy(widthSpinBox);
	dsGroupBox7Layout->addWidget( widthQLabel, 2, 0 );
	dsGroupBox7Layout->addWidget( widthSpinBox, 2, 1 );
	heightSpinBox = new ScrSpinBox( 1, 16777215, dsGroupBox7, doc->unitIndex() );
	heightSpinBox->setValue(doc->currentPage()->height() * doc->unitRatio());
	heightQLabel = new QLabel( tr( "&Height:" ), dsGroupBox7 );
	heightQLabel->setBuddy(heightSpinBox);
	dsGroupBox7Layout->addWidget( heightQLabel, 2, 2 );
	dsGroupBox7Layout->addWidget( heightSpinBox, 2, 3 );
	moveObjects = new QCheckBox( dsGroupBox7 );
	moveObjects->setText( tr( "Move Objects with their Page" ) );
	moveObjects->setChecked( true );
	dsGroupBox7Layout->addWidget( moveObjects, 3, 0, 1, 4 );
	Links=0;
	if ((doc->pagePositioning() != singlePage) && (doc->masterPageMode()))
	{
		TextLabel3 = new QLabel( tr( "Type:" ), dsGroupBox7 );
		dsGroupBox7Layout->addWidget( TextLabel3, 4, 0, 1, 2 );
		Links = new QComboBox( dsGroupBox7 );
		QStringList::Iterator pNames;
		QList<PageSet> pageSet(doc->pageSets());
		for(pNames = pageSet[doc->pagePositioning()].pageNames.begin(); pNames != pageSet[doc->pagePositioning()].pageNames.end(); ++pNames )
		{
			//Links->insertItem((*pNames));
			Links->addItem(CommonStrings::translatePageSetLocString((*pNames)));
		}
		Links->setEditable(false);
		dsGroupBox7Layout->addWidget( Links, 4, 2, 1, 2 );
		if (doc->currentPage()->LeftPg == 0)
			Links->setCurrentIndex(Links->count()-1);
		else if (doc->currentPage()->LeftPg == 1)
			Links->setCurrentIndex(0);
		else
			Links->setCurrentIndex(doc->currentPage()->LeftPg-1);
	}
	dialogLayout->addWidget( dsGroupBox7 );
	
	marginWidget = new MarginWidget(this,  tr( "Margin Guides" ), &doc->currentPage()->initialMargins, doc->unitIndex(), false, false);
	marginWidget->setPageWidthHeight(doc->currentPage()->width(), doc->currentPage()->height());
	marginWidget->setFacingPages(!(doc->pagePositioning() == singlePage), doc->locationOfPage(doc->currentPage()->pageNr()));
	marginWidget->setMarginPreset(doc->currentPage()->marginPreset);
	dialogLayout->addWidget( marginWidget );

	groupMaster = new QGroupBox( this );
	groupMaster->setTitle( tr( "Other Settings" ) );
	masterLayout = new QHBoxLayout( groupMaster );
	masterLayout->setSpacing( 5 );
	masterLayout->setMargin( 10 );
	masterPageLabel = new QLabel( groupMaster );
	masterLayout->addWidget( masterPageLabel );
	if (!doc->masterPageMode())
	{
		masterPageLabel->setText( tr( "Master Page:" ) );
		masterPageComboBox = new QComboBox( groupMaster );
		QString Nam = doc->currentPage()->MPageNam;
		QString na = Nam == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : Nam, in;
		int cc = 0;
		for (QMap<QString,int>::Iterator it = doc->MasterNames.begin(); it != doc->MasterNames.end(); ++it)
		{
			in = it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key();
			masterPageComboBox->addItem(in);
			if (in == na)
				masterPageComboBox->setCurrentIndex(cc);
			++cc;
		}
		masterLayout->addWidget( masterPageComboBox );
	}
	dialogLayout->addWidget( groupMaster );
	if (doc->masterPageMode())
		groupMaster->hide();

	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 5 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this );
	okButton->setDefault( true );
	okCancelLayout->addWidget(okButton);
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	cancelButton->setDefault( false );
	okCancelLayout->addWidget(cancelButton);
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());
	pageWidth = widthSpinBox->value() / unitRatio;
	pageHeight = heightSpinBox->value() / unitRatio;
	bool isCustom = (sizeQComboBox->currentText() == CommonStrings::trCustomPageSize);
	heightSpinBox->setEnabled(isCustom);
	widthSpinBox->setEnabled(isCustom);
	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(orientationQComboBox, SIGNAL(activated(int)), this, SLOT(setOrientation(int)));
	connect(sizeQComboBox, SIGNAL(activated(const QString &)), this, SLOT(setPageSize()));
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	
	//tooltips
	sizeQComboBox->setToolTip( tr( "Size of the inserted pages, either a standard or custom size" ) );
	orientationQComboBox->setToolTip( tr( "Orientation of the page(s) to be inserted" ) );
	widthSpinBox->setToolTip( tr( "Width of the page(s) to be inserted" ) );
	heightSpinBox->setToolTip( tr( "Height of the page(s) to be inserted" ) );
	moveObjects->setToolTip( tr( "When inserting a new page between others, move objects with their current pages. This is the default action." ) );
	
	setPageSize();
	setMinimumSize(minimumSizeHint());
	setMaximumSize(minimumSizeHint());
	resize(minimumSizeHint());
}

void PagePropertiesDialog::setPageWidth(double)
{
	pageWidth = widthSpinBox->value() / unitRatio;
	marginWidget->setPageWidth(pageWidth);
	int newOrientation = (widthSpinBox->value() > heightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != orientationQComboBox->currentIndex())
	{
		orientationQComboBox->blockSignals(true);
		orientationQComboBox->setCurrentIndex(newOrientation);
		orientationQComboBox->blockSignals(false);
		oldOri = newOrientation;
	}
}

void PagePropertiesDialog::setPageHeight(double)
{
	pageHeight = heightSpinBox->value() / unitRatio;
	marginWidget->setPageHeight(pageHeight);
	int newOrientation = (widthSpinBox->value() > heightSpinBox->value()) ? landscapePage : portraitPage;
	if (newOrientation != orientationQComboBox->currentIndex())
	{
		orientationQComboBox->blockSignals(true);
		orientationQComboBox->setCurrentIndex(newOrientation);
		orientationQComboBox->blockSignals(false);
		oldOri = newOrientation;
	}
}

void PagePropertiesDialog::setPageSize()
{
	if (sizeQComboBox->currentText() != CommonStrings::trCustomPageSize)
		oldOri++;
	setOrientation(orientationQComboBox->currentIndex());
}

void PagePropertiesDialog::setSize(const QString & gr)
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
	marginWidget->setPageHeight(pageHeight);
	marginWidget->setPageWidth(pageWidth);
	marginWidget->setPageSize(gr);
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	delete ps2;
}

void PagePropertiesDialog::setOrientation(int ori)
{
	setSize(sizeQComboBox->currentText());
	disconnect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	disconnect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
	if ((sizeQComboBox->currentText() == CommonStrings::trCustomPageSize) && (ori != oldOri))
	{
		double w = widthSpinBox->value(), h = heightSpinBox->value();
		widthSpinBox->setValue((ori == portraitPage) ? qMin(w, h) : qMax(w, h));
		heightSpinBox->setValue((ori == portraitPage) ? qMax(w, h) : qMin(w, h));
	}
	else
	{
		if ((ori != portraitPage) && (ori != oldOri))
		{
			double w = widthSpinBox->value(), h = heightSpinBox->value();
			widthSpinBox->setValue(qMax(w, h));
			heightSpinBox->setValue(qMin(w, h));
		}
	}
	oldOri = ori;
	pageWidth = widthSpinBox->value() / unitRatio;
	pageHeight = heightSpinBox->value() / unitRatio;
	connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageWidth(double)));
	connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPageHeight(double)));
}

int PagePropertiesDialog::pageOrder()
{
	int lp=0;
	if (Links!=0)
		lp = Links->currentIndex();
	if (lp == 0)
		lp = 1;
	else if (lp == static_cast<int>(Links->count()-1))
		lp = 0;
	else
		lp++;
	return lp;
}

double PagePropertiesDialog::getPageWidth()
{
	return pageWidth;
}

double PagePropertiesDialog::getPageHeight()
{
	return pageHeight;
}

int PagePropertiesDialog::getPageOrientation()
{
	return orientationQComboBox->currentIndex();
}

QString PagePropertiesDialog::getPrefsPageSizeName()
{
	return prefsPageSizeName;
}

bool PagePropertiesDialog::getMoveObjects()
{
	return moveObjects->isChecked();
}

double PagePropertiesDialog::top()
{
	return marginWidget->top();
}

double PagePropertiesDialog::bottom()
{
	return marginWidget->bottom();
}

double PagePropertiesDialog::left()
{
	return marginWidget->left();
}

double PagePropertiesDialog::right()
{
	return marginWidget->right();
}

QString PagePropertiesDialog::masterPage()
{
	if (masterPageComboBox != NULL)
		return masterPageComboBox->currentText();
	return "";
}

int PagePropertiesDialog::getMarginPreset()
{
	return marginWidget->getMarginPreset();
}
