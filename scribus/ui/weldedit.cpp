/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  weldedit.cpp  -  description
							 -------------------
	begin                : Tue Nov 01 2011
	copyright            : (C) 2011 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "weldedit.h"

WeldEditDialog::WeldEditDialog(QWidget* parent) : ScrPaletteBase( parent, "WeldEditDialog", false, 0 )
{
	setupUi(this);
	moveWeldPoint->setChecked(true);
	moveObject->setChecked(false);
	connect(moveWeldPoint, SIGNAL(clicked()), this, SIGNAL(modeMoveWeld()));
	connect(moveObject, SIGNAL(clicked()), this, SIGNAL(modeMoveObject()));
	connect(buttonEndEdit, SIGNAL(clicked()), this, SIGNAL(endEdit()));
	languageChange();
	resize(minimumSizeHint());
}

void WeldEditDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void WeldEditDialog::languageChange()
{
	retranslateUi(this);
	resize(minimumSizeHint());
}
