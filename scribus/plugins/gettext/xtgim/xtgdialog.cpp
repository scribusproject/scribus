/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 /***************************************************************************
  *   Copyright (C) 2009 by Jain Basil Aliyas                               *
  *   jainbasil@gmail.com                                                   *
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
  *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
  ***************************************************************************/

#include "xtgdialog.h"

#include <QToolTip>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>

#include "scribusapi.h"
#include "util_icon.h"

XtgDialog::XtgDialog(bool prefix) : QDialog(0)
{
	setModal(true);
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	setWindowTitle( tr("Quark XPress Tags Importer Options"));

	QBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	QBoxLayout* playout = new QHBoxLayout;
	playout->setMargin(5);
	playout->setSpacing(5);
	prefixCheck = new QCheckBox( tr("Use document name as a prefix for Styles"), this);
	prefixCheck->setChecked(prefix);
	prefixCheck->setToolTip( "<qt>" + tr("Prepend the document name to the Style name in Scribus.") +"</qt>");
	playout->addWidget(prefixCheck);
	layout->addLayout(playout);

	QBoxLayout* dlayout = new QHBoxLayout;
	dlayout->setMargin(5);
	dlayout->setSpacing(5);
	doNotAskCheck = new QCheckBox( tr("Do not ask again"), this);
	doNotAskCheck->setChecked(false);
	doNotAskCheck->setToolTip( "<qt>" + tr("Make these settings the default and do not prompt again when importing an XPress Tags document.") +"</qt>");
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

bool XtgDialog::usePrefix()
{
	return prefixCheck->isChecked();
}

bool XtgDialog::askAgain()
{
	return !(doNotAskCheck->isChecked());
}

XtgDialog::~XtgDialog()
{

}

