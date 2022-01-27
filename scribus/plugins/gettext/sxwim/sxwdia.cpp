/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "sxwdia.h"

#include <QToolTip>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>

#include "scribusapi.h"
#include "iconmanager.h"

SxwDialog::SxwDialog(bool update, bool prefix, bool pack) : QDialog(nullptr)
{
	setModal(true);
	setWindowIcon(QIcon(IconManager::instance().loadIcon("AppIcon.png")));
	setWindowTitle( tr("OpenOffice.org Writer Importer Options"));

	QBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(9, 9, 9, 9);
	layout->setSpacing(6);

	QBoxLayout* hlayout = new QHBoxLayout;
	hlayout->setContentsMargins(0, 0, 0, 0);
	hlayout->setSpacing(6);
	updateCheck = new QCheckBox( tr("Overwrite Paragraph Styles"), this);
	updateCheck->setChecked(update);
	updateCheck->setToolTip( "<qt>" + tr("Enabling this will overwrite existing styles in the current Scribus document") + "</qt>");
	hlayout->addWidget(updateCheck);
	layout->addLayout(hlayout);
	
	QBoxLayout* palayout = new QHBoxLayout;
	palayout->setContentsMargins(0, 0, 0, 0);
	palayout->setSpacing(6);
	packCheck = new QCheckBox( tr("Merge Paragraph Styles"), this);
	packCheck->setChecked(pack);
	packCheck->setToolTip( "<qt>" + tr("Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.") +"</qt>");
	palayout->addWidget(packCheck);
	layout->addLayout(palayout);

	QBoxLayout* playout = new QHBoxLayout;
	playout->setContentsMargins(0, 0, 0, 0);
	playout->setSpacing(6);
	prefixCheck = new QCheckBox( tr("Use document name as a prefix for paragraph styles"), this);
	prefixCheck->setChecked(prefix);
	prefixCheck->setToolTip( "<qt>" + tr("Prepend the document name to the paragraph style name in Scribus.") +"</qt>");
	playout->addWidget(prefixCheck);
	layout->addLayout(playout);

	QBoxLayout* dlayout = new QHBoxLayout;
	dlayout->setContentsMargins(0, 0, 0, 0);
	dlayout->setSpacing(6);
	doNotAskCheck = new QCheckBox( tr("Do not ask again"), this);
	doNotAskCheck->setChecked(false);
	doNotAskCheck->setToolTip( "<qt>" + tr("Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document") +"</qt>");
	//dlayout->addStretch(10);
	dlayout->addWidget(doNotAskCheck);
	layout->addLayout(dlayout);

	QBoxLayout* blayout = new QHBoxLayout;
	blayout->setContentsMargins(0, 0, 0, 0);
	blayout->setSpacing(6);
	blayout->addStretch(10);
	okButton = new QPushButton( tr("OK"), this);
	blayout->addWidget(okButton);
	cancelButton = new QPushButton( tr("Cancel"), this);
	blayout->addWidget(cancelButton);
	layout->addLayout(blayout);

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

bool SxwDialog::shouldUpdate()
{
	return updateCheck->isChecked();
}

bool SxwDialog::usePrefix()
{
	return prefixCheck->isChecked();
}

bool SxwDialog::askAgain()
{
	return !(doNotAskCheck->isChecked());
}

bool SxwDialog::packStyles()
{
	return packCheck->isChecked();
}

SxwDialog::~SxwDialog()
{

}

