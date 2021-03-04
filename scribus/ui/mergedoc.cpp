/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "commonstrings.h"
#include "fileloader.h"
#include "iconmanager.h"
#include "mergedoc.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "scpaths.h"
#include "ui/customfdialog.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QCursor>
#include <QDir>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPixmap>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QToolTip>
#include <QVBoxLayout>

MergeDoc::MergeDoc(QWidget* parent, bool importMasterPages, int targetDocPageCount, int currentPage) : QDialog(parent)
{
	masterPages = importMasterPages;
	setModal(true);
	setWindowTitle((masterPages) ? tr("Import Master Page") : tr( "Import Page(s)" ));
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));

	dialogLayout = new QVBoxLayout(this);
	dialogLayout->setContentsMargins(9, 9, 9, 9);
	dialogLayout->setSpacing(6);
	fromInfoLayout = new QGridLayout;
	fromInfoLayout->setContentsMargins(0, 0, 0, 0);
	fromInfoLayout->setSpacing(6);
	fromDocData = new QLineEdit( this );
	fromDocData->setMinimumWidth(QWidget::fontMetrics().horizontalAdvance('a') * 50);
	fromDocLabel = new QLabel( tr( "&From Document:"), this );
	fromDocLabel->setBuddy( fromDocData );
	fromInfoLayout->addWidget( fromDocLabel, 0, 0 );
	fromInfoLayout->addWidget( fromDocData, 0, 1 );
	changeButton = new QPushButton( tr( "&Select..." ), this );
	changeButton->setAutoDefault( false );
	fromInfoLayout->addWidget( changeButton, 0, 2 );
	importPageLabel = new QLabel( tr( "&Import Page(s):" ), this );
	fromInfoLayout->addWidget( importPageLabel, 1, 0 );

	if (masterPages)
	{
		importPageLabel->setText( tr("&Import Master Page:") );
		masterPageNameData = new QListWidget(this);
		masterPageNameData->setEnabled(false);
		masterPageNameData->setSelectionMode(QAbstractItemView::ExtendedSelection);
		importPageLabel->setBuddy( masterPageNameData );
		fromInfoLayout->addWidget( masterPageNameData, 1, 1, 1, 2);
	}
	else
	{
		pageNumberData = new QLineEdit( this );
		pageNumberData->setEnabled(false);
		importPageLabel->setBuddy( pageNumberData );
		masterPageNameData = nullptr;
		pageNumberData->setToolTip( "<qt>" + tr( "Insert a comma separated list of tokens import where "
									"a token can be * for all the pages, 1-5 for "
									"a range of pages or a single page number") + "</qt>");
		fromInfoLayout->addWidget( pageNumberData, 1, 1 );
		fromLabel = new QLabel(this);
		fromInfoLayout->addWidget( fromLabel, 1, 2 );
		createPageData = new QCheckBox( this );
		createPageData->setText( tr("Create Page(s)"));
		fromInfoLayout->addWidget( createPageData, 2, 0 );
		importWhereData = new QComboBox( this );
		importWhereData->setEnabled(false);
		importWhereData->addItem( tr("Before Page"));
		importWhereData->addItem( tr("After Page"));
		importWhereData->addItem( tr("At End"));
		importWhereData->setCurrentIndex( 2 );
		fromInfoLayout->addWidget( importWhereData, 2, 1 );
		importWherePageData = new QSpinBox( this );
		importWherePageData->setMinimum(1);
		importWherePageData->setMaximum(targetDocPageCount);
		importWherePageData->setValue( currentPage );
		importWherePageData->setEnabled(false);
		fromInfoLayout->addWidget( importWherePageData, 2, 2 );
	}

	dialogLayout->addLayout( fromInfoLayout );
	importCancelLayout = new QHBoxLayout;
	importCancelLayout->setContentsMargins(0, 0, 0, 0);
	importCancelLayout->setSpacing(6);
	QSpacerItem* spacer = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum );
	importCancelLayout->addItem( spacer );
	importButton = new QPushButton( tr( "&Import" ), this );
	importButton->setEnabled(false);
	importCancelLayout->addWidget( importButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	cancelButton->setDefault(true);
	importCancelLayout->addWidget( cancelButton );
	dialogLayout->addLayout( importCancelLayout );
	resize(minimumSizeHint());

	connect( importButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( changeButton, SIGNAL( clicked() ), this, SLOT( changeFile() ) );
	if (!masterPages)
	{
		connect( importWhereData, SIGNAL( activated(int) ), this, SLOT( checkDestPageStatus(int) ) );
		connect( createPageData, SIGNAL( clicked() ), this, SLOT( enableCreateWidgets() ) );
	}
}

/*
 *  Destroys the object and frees any allocated resources
 */
MergeDoc::~MergeDoc()
{
    // no need to delete child widgets, Qt does it all for us
}

void MergeDoc::changeFile()
{
	count = 0;
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	QString wdir = dirs->get("merge", ".");
	CustomFDialog *dia = new CustomFDialog(this, wdir, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
	if (!fromDocData->text().isEmpty())
		dia->setSelection(fromDocData->text());
	if (dia->exec() == QDialog::Accepted)
	{
		QString fn(dia->selectedFile());
		if (!fn.isEmpty())
		{
			dirs->set("merge", fn.left(fn.lastIndexOf("/")));
			qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
			FileLoader fl(fn);
			if (fl.testFile() == -1)
			//TODO put in nice user warning
				return;
			QStringList masterPageNames;
			bool ret = false;
			int dummy;
			if (masterPages)
				ret = fl.readPageCount(&dummy, &count, masterPageNames);
			else
				ret = fl.readPageCount(&count, &dummy, masterPageNames);
			qApp->restoreOverrideCursor();
			if (ret && count > 0)
			{
				fromDocData->setText( QDir::toNativeSeparators(fn) );
				importButton->setEnabled(true);
				importButton->setDefault(true);
				if (masterPages)
				{
					masterPageNameData->clear();
					masterPageNameData->setEnabled(true);
					masterPageNameData->addItems(masterPageNames);
					masterPageNameData->setCurrentRow(0);
				}
				else
				{
					pageNumberData->setEnabled(true);
					pageNumberData->setText("*");
				}
				if (!masterPages)
					fromLabel->setText( tr(" from %1").arg(count));
			}
		}
	}
	else
	{
		fromDocData->setText("");
		count = 0;
		importButton->setEnabled(false);
		cancelButton->setDefault(true);
		if (masterPages)
		{
			masterPageNameData->clear();
			masterPageNameData->setEnabled(false);
		}
		else
		{
			pageNumberData->setEnabled(false);
		}
		if (!masterPages)
			fromLabel->setText( tr(" from %1").arg(count));
	}
	delete dia;
}

void MergeDoc::checkDestPageStatus(int positionComboSelection)
{
	importWherePageData->setDisabled( positionComboSelection == 2 );
}

void MergeDoc::enableCreateWidgets()
{
	if (createPageData->isChecked())
	{
		importWhereData->setEnabled(true);
		if (importWhereData->currentIndex() != 2)
			importWherePageData->setEnabled(true);
	}
	else
	{
		importWhereData->setEnabled(false);
		importWherePageData->setEnabled(false);
	}
}

QString MergeDoc::getFromDoc() const
{
	return QDir::fromNativeSeparators(fromDocData->text());
}

QStringList MergeDoc::getMasterPageNames() const
{
	QStringList result;
	for (const auto item : masterPageNameData->selectedItems())
		result << item->text();
	return result;
}

QList<int> MergeDoc::getMasterPageIndexes() const
{
	QList<int> result;
	for (const auto& row: masterPageNameData->selectionModel()->selectedRows())
		result << row.row();
	return result;
}

int MergeDoc::getImportWhere() const
{
	return importWhereData->currentIndex();
}

int MergeDoc::getImportWherePage() const
{
	return importWherePageData->value();
}

bool MergeDoc::getCreatePageChecked() const
{
	return createPageData->isChecked();
}

QString MergeDoc::getPageNumbers() const
{
	return pageNumberData->text();
}

int MergeDoc::getPageCounter() const
{
	return count;
}
