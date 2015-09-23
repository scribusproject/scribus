/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  resourcemanagerlicense.h  -  description
							 -------------------
	begin                : Sun 20150919
	copyright            : (C) 2015 by Craig Bradney
	email                : cbradney@scribus.info
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef RESOURCEMANAGERLICENSE_H
#define RESOURCEMANAGERLICENSE_H

#include <QDialog>
#include <QTextDocument>

#include "scribusapi.h"
#include "ui_resourcemanagerlicensebase.h"



class SCRIBUS_API ResourceManagerLicense : public QDialog, Ui::ResourceManagerLicenseBase
{
	Q_OBJECT

	public:
		ResourceManagerLicense(QWidget * parent = 0, Qt::WindowFlags f = 0);
		~ResourceManagerLicense() {};

		void setText(const QString& s);

	public slots:
		void languageChange();

	protected:
		QTextDocument textDocument;

};

#endif // RESOURCEMANAGERLICENSE_H
