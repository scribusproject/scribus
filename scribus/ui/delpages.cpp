/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QLabel>
#include <QPushButton>

#include "delpages.h"
#include "iconmanager.h"
#include "commonstrings.h"
#include "ui/scrspinbox.h"

DelPages::DelPages( QWidget* parent, int currentPage, int maxPage ) : QDialog( parent )
{
	setWindowTitle( tr( "Delete Pages" ) );
	setModal(true);
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this );
	dialogLayout->setSpacing(6);
	dialogLayout->setContentsMargins(9, 9, 9, 9);
	fromToLayout = new QHBoxLayout;
	fromToLayout->setSpacing(6);
	fromToLayout->setContentsMargins(0, 0, 0, 0);
	fromLabel = new QLabel( tr( "Delete From:" ), this );
	fromToLayout->addWidget( fromLabel );
	fromPageData = new ScrSpinBox(this);
	fromPageData->setDecimals(0);
	fromPageData->setSuffix("");
	fromPageData->setMinimum(1);
	fromPageData->setMaximum(maxPage);
	fromPageData->setSuffix("");
	fromPageData->setValue( currentPage );
	fromToLayout->addWidget( fromPageData );
	toLabel = new QLabel( this );
	toLabel->setText( tr( "to:" ) );
	fromToLayout->addWidget( toLabel );
	toPageData = new ScrSpinBox(this);
	toPageData->setDecimals(0);
	toPageData->setMinimum(1);
	toPageData->setMaximum(maxPage);
	toPageData->setSuffix("");
	toPageData->setValue( currentPage );
	toPageData->setSuffix("");
	fromToLayout->addWidget( toPageData );
	dialogLayout->addLayout( fromToLayout );

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	dialogLayout->addWidget( buttonBox );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect(buttonBox, &QDialogButtonBox::accepted, this, &DelPages::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &DelPages::reject);
	connect( fromPageData, SIGNAL( valueChanged(double) ), this, SLOT( fromChanged() ) );
	connect( toPageData, SIGNAL( valueChanged(double) ), this, SLOT( toChanged() ) );
}

void DelPages::fromChanged()
{
	int pageNumber=static_cast<int>(fromPageData->value());
	if (pageNumber > toPageData->value())
		toPageData->setValue(pageNumber);
	if ((pageNumber == 1) && (toPageData->value() == toPageData->maximum()))
		toPageData->setValue(toPageData->maximum()-1);
}

void DelPages::toChanged()
{
	int pageNumber=toPageData->value();
	if (pageNumber < fromPageData->value())
		fromPageData->setValue(pageNumber);
	if ((fromPageData->value() == 1) && (pageNumber == toPageData->maximum()))
		fromPageData->setValue(2);
}

int DelPages::getFromPage() const
{
	return static_cast<int>(fromPageData->value());
}

int DelPages::getToPage() const
{
	return toPageData->value();
}

