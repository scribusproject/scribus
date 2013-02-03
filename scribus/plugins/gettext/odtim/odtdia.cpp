/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 /***************************************************************************
  *   Copyright (C) 2004 by Riku Leino                                      *
  *   tsoots@gmail.com                                                      *
  *                                                                         *
  *   This program is free software; you can redistribute it and/or modify  *
  *   it under the terms of the GNU General Public License as published by  *
  *   the Free Software Foundation; either version 2 of the License, or     *
  *   (at your option) any later version.                                   *
  *                                                                         *
  *   This program is distributed in the hope that it will be useful,       *
  *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
  *   GNU General Public License for more details.                          *
  *                                                                         *
  *   You should have received a copy of the GNU General Public License     *
  *   along with this program; if not, write to the                         *
  *   Free Software Foundation, Inc.,                                       *
  *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "odtdia.h"

#include <QToolTip>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include "scribusapi.h"
#include "util_icon.h"

OdtDialog::OdtDialog(bool update, bool prefix, bool pack) : QDialog(0)
{
	setModal(true);
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	setWindowTitle( tr("OpenDocument Importer Options"));

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
	doNotAskCheck->setToolTip( "<qt>" + tr("Make these settings the default and do not prompt again when importing an OASIS OpenDocument.") +"</qt>");
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

bool OdtDialog::shouldUpdate()
{
	return updateCheck->isChecked();
}

bool OdtDialog::usePrefix()
{
	return prefixCheck->isChecked();
}

bool OdtDialog::askAgain()
{
	return !(doNotAskCheck->isChecked());
}

bool OdtDialog::packStyles()
{
	return packCheck->isChecked();
}

OdtDialog::~OdtDialog()
{

}
