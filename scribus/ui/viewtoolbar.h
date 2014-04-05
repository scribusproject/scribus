/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VIEWTOOLBAR_H
#define VIEWTOOLBAR_H

#include <QComboBox>

#include "scribusapi.h"
#include "ui/sctoolbar.h"

class ScribusDoc;
class ScribusMainWindow;

class SCRIBUS_API ViewToolBar : public ScToolBar
{
	Q_OBJECT

public:
	ViewToolBar(ScribusMainWindow* parent);
	~ViewToolBar() {};

	QComboBox *previewQualitySwitcher;
	QComboBox *visualMenu;
	void setDoc(ScribusDoc *doc);
	void setViewPreviewMode(bool);

public slots:
	void languageChange();

protected:
	bool inPreview;
};

#endif
