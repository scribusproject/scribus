/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#include "nftdialog.h"
#include <QPushButton>

#include "scconfig.h"
#include "scribusapi.h"
#include "util_icon.h"


nftdialog::nftdialog(QWidget* parent, QString lang, QString templateDir) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(loadIcon("AppIcon.png"));
	nftGui->setupSettings(lang, templateDir);
	connect(nftGui, SIGNAL(leaveOK()), this, SLOT(accept()));
	connect(nftGui, SIGNAL(ButtonBoxEnable(bool)), this, SLOT(enableButtonBox(bool)));
}

void nftdialog::enableButtonBox(bool setter)
{
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(setter);
	buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
}
