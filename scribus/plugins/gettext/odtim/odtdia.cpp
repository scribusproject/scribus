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

extern QPixmap loadIcon(QString nam);

OdtDialog::OdtDialog(bool update, bool prefix, bool pack) : QDialog(0, "sxwdia", true, 0)
{
	setCaption(tr("OpenOffice.org Writer Importer Options"));
	setIcon(loadIcon("AppIcon.png"));

	QBoxLayout* layout = new QVBoxLayout(this);

	QBoxLayout* hlayout = new QHBoxLayout(0, 5, 5, "hlayout");
	updateCheck = new QCheckBox(tr("Update paragraph styles"), this, "updateCheck");
	updateCheck->setChecked(update);
	QToolTip::add(updateCheck, tr("If a paragraph style already exists with the same name as the current\n"
	                              "OpenOffice.org document's paragraph, should the style in Scribus be\n"
	                              "edited to match the one being imported, or left untouched"));
	hlayout->addWidget(updateCheck);
	layout->addLayout(hlayout);

	QBoxLayout* palayout = new QHBoxLayout(0,5,5, "palayout");
	packCheck = new QCheckBox(tr("Pack paragraph styles"), this, "packCheck");
	packCheck->setChecked(pack);
	QToolTip::add(packCheck, tr("Group paragraph styles by attributes.\n"
	                            "Less paragraph styles but controlling them may be hard.\n"
	                            "Should be used if it is known that text must not be edited\n"
	                            "after importing."));
	palayout->addWidget(packCheck);
	layout->addLayout(palayout);

	QBoxLayout* playout = new QHBoxLayout(0, 5, 5, "playout");
	prefixCheck = new QCheckBox(tr("Use document name as a prefix for paragraph styles"), this, "prefixCheck");
	prefixCheck->setChecked(prefix);
	QToolTip::add(prefixCheck, tr("Should importer add the name of the document\n"
	                              "on front of the paragraph style name in Scribus"));
	playout->addWidget(prefixCheck);
	layout->addLayout(playout);

	QBoxLayout* dlayout = new QHBoxLayout(0, 5, 5, "dlayout");
	doNotAskCheck = new QCheckBox(tr("Do not ask again"), this, "doNotAskCheck");
	doNotAskCheck->setChecked(false);
	QToolTip::add(doNotAskCheck, tr("Should the importer always use currently\n"
	                                "set value when importing OpenOffice.org document and\n"
	                                "never ask your confirmation again"));
	dlayout->addStretch(10);
	dlayout->addWidget(doNotAskCheck);
	layout->addLayout(dlayout);

	QBoxLayout* blayout = new QHBoxLayout(0, 5, 5, "blayout");
	blayout->addStretch(10);
	okButton = new QPushButton(tr("OK"), this, "okButton");
	blayout->addWidget(okButton);
	layout->addLayout(blayout);

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
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
