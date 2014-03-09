/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef APPMODEHELPER_H
#define APPMODEHELPER_H

#include <QMap>
#include <QObject>
#include <QPointer>
#include <QString>
#include "actionmanager.h"
#include "scraction.h"

class AppModeHelper : public QObject
{
		Q_OBJECT

	friend class ActionManager;
	friend class ScribusMainWindow;

	public:
		explicit AppModeHelper(QObject *parent = 0);
		void setScrActions(QMap<QString, QPointer<ScrAction> > *);
		void setFrameEditMode(bool b);
		void setSymbolEditMode(bool b, ScribusDoc* doc);
		void setInlineEditMode(bool b, ScribusDoc* doc);
		void setMasterPageEditMode(bool b, ScribusDoc *doc);
		void changeLayer(ScribusDoc *doc, bool clipScrapHaveData);

	signals:

	public slots:

	protected:
		QMap<QString, QPointer<ScrAction> > *scrActions;
};

#endif // APPMODEHELPER_H
