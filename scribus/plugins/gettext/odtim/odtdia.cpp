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
  *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "odtdia.h"

#ifdef HAVE_XML

#include "odtdia.moc"
#include <qtooltip.h>
#include "scribusapi.h"

extern QPixmap SCRIBUS_API loadIcon(QString nam);

OdtDialog::OdtDialog(bool update, bool prefix, bool pack) : QDialog(0, "sxwdia", true, 0)
{
	setCaption(tr("OpenDocument Importer Options"));
	setIcon(loadIcon("AppIcon.png"));

	QBoxLayout* layout = new QVBoxLayout(this);

	QBoxLayout* hlayout = new QHBoxLayout(0, 5, 5, "hlayout");
	updateCheck = new QCheckBox(tr("Overwrite Paragraph Styles"), this,
"updateCheck");
	updateCheck->setChecked(update);
	QToolTip::add(updateCheck, "<qt>" + tr("Enabling this will overwrite existing styles in the current Scribus document") + "</qt>");
	hlayout->addWidget(updateCheck);
	layout->addLayout(hlayout);

	QBoxLayout* palayout = new QHBoxLayout(0,5,5, "palayout");
	packCheck = new QCheckBox(tr("Merge Paragraph Styles"), this, "packCheck");
	packCheck->setChecked(pack);
	QToolTip::add(packCheck, "<qt>" + tr("Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.") +"</qt>");
	palayout->addWidget(packCheck);
	layout->addLayout(palayout);

	QBoxLayout* playout = new QHBoxLayout(0, 5, 5, "playout");
	prefixCheck = new QCheckBox(tr("Use document name as a prefix for paragraph styles"), this, "prefixCheck");
	prefixCheck->setChecked(prefix);
	QToolTip::add(prefixCheck, "<qt>" + tr("Prepend the document name to the paragraph style name in Scribus.") +"</qt>");
	playout->addWidget(prefixCheck);
	layout->addLayout(playout);

	QBoxLayout* dlayout = new QHBoxLayout(0, 5, 5, "dlayout");
	doNotAskCheck = new QCheckBox(tr("Do not ask again"), this,
"doNotAskCheck");
	doNotAskCheck->setChecked(false);
	QToolTip::add(doNotAskCheck, "<qt>" + tr("Make these settings the default and do not prompt again when importing an OASIS OpenDocument.") +"</qt>");
	//dlayout->addStretch(10);
	dlayout->addWidget(doNotAskCheck);
	layout->addLayout(dlayout);

	QBoxLayout* blayout = new QHBoxLayout(0, 5, 5, "blayout");
	blayout->addStretch(10);
	okButton = new QPushButton(tr("OK"), this, "okButton");
	blayout->addWidget(okButton);
	cancelButton = new QPushButton(tr("Cancel"), this, "cancelButton");
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

#endif // HAVE_XML
