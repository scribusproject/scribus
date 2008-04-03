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
#include <QToolTip>
#include "util_icon.h"

SxwDialog::SxwDialog(bool update, bool prefix, bool pack) : QDialog(0)
{
	setModal(true);
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	setWindowTitle( tr("OpenOffice.org Writer Importer Options"));

	QBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	QBoxLayout* hlayout = new QHBoxLayout;
	hlayout->setMargin(5);
	hlayout->setSpacing(5);
	updateCheck = new QCheckBox( tr("Overwrite Paragraph Styles"), this);
	updateCheck->setChecked(update);
	updateCheck->setToolTip( "<qt>" + tr("Enabling this will overwrite existing styles in the current Scribus document") + "</qt>");
	hlayout->addWidget(updateCheck);
	layout->addLayout(hlayout);
	
	QBoxLayout* palayout = new QHBoxLayout;
	palayout->setMargin(5);
	palayout->setSpacing(5);
	packCheck = new QCheckBox( tr("Merge Paragraph Styles"), this);
	packCheck->setChecked(pack);
	packCheck->setToolTip( "<qt>" + tr("Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.") +"</qt>");
	palayout->addWidget(packCheck);
	layout->addLayout(palayout);

	QBoxLayout* playout = new QHBoxLayout;
	playout->setMargin(5);
	playout->setSpacing(5);
	prefixCheck = new QCheckBox( tr("Use document name as a prefix for paragraph styles"), this);
	prefixCheck->setChecked(prefix);
	prefixCheck->setToolTip( "<qt>" + tr("Prepend the document name to the paragraph style name in Scribus.") +"</qt>");
	playout->addWidget(prefixCheck);
	layout->addLayout(playout);

	QBoxLayout* dlayout = new QHBoxLayout;
	dlayout->setMargin(5);
	dlayout->setSpacing(5);
	doNotAskCheck = new QCheckBox( tr("Do not ask again"), this);
	doNotAskCheck->setChecked(false);
	doNotAskCheck->setToolTip( "<qt>" + tr("Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.") +"</qt>");
	//dlayout->addStretch(10);
	dlayout->addWidget(doNotAskCheck);
	layout->addLayout(dlayout);

	QBoxLayout* blayout = new QHBoxLayout;
	blayout->setMargin(5);
	blayout->setSpacing(5);
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

