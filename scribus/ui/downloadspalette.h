/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  transparencypalette.h  -  description
							 -------------------
	begin                : Sun 20150201
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

#ifndef DOWNLOADSPALETTE_H
#define DOWNLOADSPALETTE_H

#include "scribusapi.h"
#include "scribusstructs.h"
#include "ui/scrpalettebase.h"
#include "ui_downloadspalettebase.h"



class SCRIBUS_API DownloadsPalette : public ScrPaletteBase, public Ui::DownloadsPalette_Base
{
	Q_OBJECT

	public:
		DownloadsPalette(QWidget* parent);
		~DownloadsPalette() {};

	//events:
		//virtual void changeEvent(QEvent *e);

	public slots:
		void languageChange();
		void addDownload(const QString &name, const QString &source, const QString &destination);

	protected:

};

#endif // DOWNLOADSPALETTE_H
