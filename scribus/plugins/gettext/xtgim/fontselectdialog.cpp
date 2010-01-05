/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 /***************************************************************************
  *   Copyright (C) 2009 by Jain Basil Aliyas                               *
  *   mail@jainbasil.net                                                    *
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

#include "fontselectdialog.h"
#include "util_icon.h"
#include <QDebug>

FontSelect::FontSelect(QStringList list):QDialog(0)
{
	setModal(true);
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	setWindowTitle( tr("Select Alternate Font"));

        resize(370, 83);
        layoutWidget = new QWidget(this);
        layoutWidget->setGeometry(QRect(10, 10, 350, 63));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        label = new QLabel(layoutWidget);
	label->setText( tr("Font") );
        label->setMinimumSize(QSize(61, 0));

        horizontalLayout_2->addWidget(label);

        fontList = new QComboBox(layoutWidget);
        fontList->setMinimumSize(QSize(281, 0));
	fontList->addItems(list);

        horizontalLayout_2->addWidget(fontList);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalSpacer = new QSpacerItem(128, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        okButton = new QPushButton("OK",layoutWidget);
        okButton->setEnabled(true);
        okButton->setMinimumSize(QSize(105, 0));

        horizontalLayout_3->addWidget(okButton);

        verticalLayout_2->addLayout(horizontalLayout_3);
        label->setBuddy(fontList);
	font = fontList->currentText();

	connect(fontList, SIGNAL(activated(int)), this, SLOT(selectFont()));
	connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));
}

void FontSelect::selectFont()
{
	font = fontList->currentText();
}

QString FontSelect::setFont()
{
	return font;
}

void FontSelect::resetFont()
{
	font = "";
}