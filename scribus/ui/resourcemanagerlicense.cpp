/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : Sep 2015
	copyright            : (C) 2015 by Craig Bradney
	email                : cbradney@scribus.info
***************************************************************************/

/***************************************************************************
*                                                                         *
*   mainWindow program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#include <QTextEdit>

#include "resourcemanagerlicense.h"

ResourceManagerLicense::ResourceManagerLicense(QWidget* parent, Qt::WindowFlags f)
{
	setupUi(this);
	languageChange();
}

void ResourceManagerLicense::setText(const QString& s)
{
	textDocument.setPlainText(s);
	licenseTextEdit->setDocument(&textDocument);
}

void ResourceManagerLicense::languageChange()
{

}
