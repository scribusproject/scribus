/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2009 by Franz Schmid                                    *
*   franz.schmid@altmuehlnet.de                                           *
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

#include "gradientaddedit.h"
#include <QMessageBox>

gradientEditDialog::gradientEditDialog(QWidget* parent, QString name, VGradient gradient, ColorList doco, ScribusDoc *doc, QMap<QString, VGradient> *gradients, bool newFlag) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	editor->setColors(doco);
	editor->setGradient(gradient);
	gradientName->setText(name);
	m_doc = doc;
	m_name = name;
	m_gradients = gradients;
	isNew = newFlag;
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(quitDialog()));
}

QString gradientEditDialog::name()
{
	return gradientName->text();
}

VGradient gradientEditDialog::gradient()
{
	return editor->gradient();
}

void gradientEditDialog::quitDialog()
{
	if (gradientName->text().isEmpty())
	{
		QMessageBox::information(this, CommonStrings::trWarning, tr("You cannot create a gradient without a name\nPlease give it a name"), 0);
		gradientName->setFocus();
		gradientName->selectAll();
		return;
	}
	if ((m_name != gradientName->text()) || (isNew))
	{
		if (m_gradients->contains(gradientName->text()))
		{
			QMessageBox::information(this, CommonStrings::trWarning, tr("The name of the gradient already exists,\nplease choose another one."), CommonStrings::tr_OK, 0, 0, 0, QMessageBox::Ok);
			gradientName->selectAll();
			gradientName->setFocus();
			return;
		}
		else
			accept();
	}
	else
		accept();
}
