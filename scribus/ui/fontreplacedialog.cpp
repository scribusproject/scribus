/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fontreplacedialog.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QHeaderView>
#include <QSpacerItem>
#include <QToolTip>
#include <QCloseEvent>

#include "commonstrings.h"
#include "fontcombo.h"
#include "iconmanager.h"
#include "scribusstructs.h"
#include "util.h"

FontReplaceDialog::FontReplaceDialog( QWidget* parent, QMap<QString, QString> *RList) : QDialog( parent )
{
	setModal(true);
	setWindowTitle( tr( "Font Substitution" ) );
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	ReplaceList = RList;
	FontReplaceDialogLayout = new QVBoxLayout( this );
	FontReplaceDialogLayout->setContentsMargins(9, 9, 9, 9);
	FontReplaceDialogLayout->setSpacing(6);

	textLabel1 = new QLabel(this);
	textLabel1->setAlignment(Qt::AlignVCenter);
	textLabel1->setWordWrap(true);
	textLabel1->setText( "<qt>" + tr("This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.") + "</qt>" );
	FontReplaceDialogLayout->addWidget(textLabel1);
	
	replacementTable = new QTableWidget(0, 2, this);
	replacementTable->setHorizontalHeaderItem(0, new QTableWidgetItem( tr("Original Font")));
	replacementTable->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Substitution Font")));
	replacementTable->setSortingEnabled(false);
	replacementTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	QHeaderView *header = replacementTable->horizontalHeader();
	header->setSectionsClickable(false);
	header->setSectionsMovable(false);
	header->setSectionResizeMode(0, QHeaderView::Stretch);
	header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	replacementTable->verticalHeader()->hide();
	replacementTable->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	replacementTable->setRowCount(RList->count());

	int i = 0;
	for (auto itfsu = RList->begin(); itfsu != RList->end(); ++itfsu)
	{
		replacementTable->setItem(i, 0, new QTableWidgetItem(itfsu.key()));
		FontCombo* item = new FontCombo(this);
		setCurrentComboItem(item, itfsu.value());
		replacementTable->setCellWidget(i, 1, item);
		i++;
	}
	FontReplaceDialogLayout->addWidget( replacementTable );

	layout1 = new QHBoxLayout;
	layout1->setContentsMargins(0, 0, 0, 0);
	layout1->setSpacing(6);
	stickyReplacements = new QCheckBox( this );
	stickyReplacements->setText( tr( "Make these substitutions permanent" ) );
	layout1->addWidget(stickyReplacements);
	QSpacerItem* spacer1 = new QSpacerItem(2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout1->addItem(spacer1);
	okButton = new QPushButton(CommonStrings::tr_OK, this);
	layout1->addWidget(okButton);
	cancelButton = new QPushButton(CommonStrings::tr_Cancel, this);
	layout1->addWidget(cancelButton);
	FontReplaceDialogLayout->addLayout(layout1);
	resize( QSize(450, 250) );

	cancelButton->setToolTip( "<qt>" + tr( "Cancels these font substitutions and stops loading the document") + "</qt>" );
	stickyReplacements->setToolTip( "<qt>" + tr( "Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in File > Preferences > Fonts.") + "</qt>" );
	okButton->setToolTip( "<qt>" + tr( "If you select OK, then save, these substitutions are made permanent in the document") + "</qt>" );
	connect(okButton, SIGNAL(clicked()), this, SLOT(leaveOK()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void FontReplaceDialog::closeEvent(QCloseEvent *closeEvent)
{
	leaveOK();
	closeEvent->accept();
}

void FontReplaceDialog::leaveOK()
{
	for (int i = 0; i < replacementTable->rowCount(); ++i)
	{
		FontCombo* item = (FontCombo*)replacementTable->cellWidget(i, 1);
		ReplaceList->remove(replacementTable->item(i, 0)->text());
		ReplaceList->insert(replacementTable->item(i, 0)->text(), item->currentText());
	}
	if (okButton == sender())
		accept();
}
