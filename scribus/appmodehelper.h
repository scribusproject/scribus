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
#include "appmodes.h"
#include "scraction.h"

class PageItem;

class AppModeHelper : public QObject
{
	Q_OBJECT

	friend class ActionManager;
	friend class ScribusMainWindow;

	public:
		explicit AppModeHelper(QObject *parent = 0);
		void setup(ActionManager* am, QMap<QString, QPointer<ScrAction> > *, QMap<QString, QPointer<ScrAction> > *, QMap<QString, QPointer<ScrAction> > *, QMap<QString, QPointer<ScrAction> > *, QMap<QString, QPointer<ScrAction> > *, QMap<QString, QPointer<ScrAction> > *);
		void resetApplicationMode(ScribusMainWindow* scmw, int newMode);
		void setApplicationMode(ScribusMainWindow* scmw, ScribusDoc* doc, int newMode);
		void enableActionsForSelection(ScribusMainWindow* scmw, ScribusDoc *doc);
		void setModeActionsPerMode(int newMode);
		void setActionGroupEnabled(QMap<QString, QPointer<ScrAction> > *, bool enabled);
		void setTextEditMode(bool b);
		void setSpecialEditMode(bool b);
		void setFrameEditMode(bool b);
		void setSymbolEditMode(bool b, ScribusDoc* doc);
		void setInlineEditMode(bool b, ScribusDoc* doc);
		void setMasterPageEditMode(bool b, ScribusDoc *doc);
		void updateTableMenuActions(ScribusDoc* doc);
		void changeLayer(ScribusDoc *doc, bool clipScrapHaveData);
		void mainWindowHasNewDoc(ScribusDoc *doc, bool clipScrapHaveData);
		void mainWindowSwitchWin(ScribusDoc *doc);
		void mainWindowCloseLastDoc();
		void setPreviewMode(bool b);
		//! \brief enable or disable the unicode actions and their menus
		void enableTextActions(bool enabled, const QString& fontName=QString::null);
		void setStartupActionsEnabled(bool enabled);


	signals:
		void AppModeChanged(int oldMode, int newMode);
		void UpdateRequest(int updateFlags);

	public slots:

	protected:
		QMap<QString, QPointer<ScrAction> > *a_scrActions;
		QMap<QString, QPointer<ScrAction> > *a_scrRecentFileActions;
		QMap<QString, QPointer<ScrAction> > *a_scrWindowsActions;
		QMap<QString, QPointer<ScrAction> > *a_scrScrapActions;
		QMap<QString, QPointer<ScrAction> > *a_scrLayersActions;
		QMap<QString, QPointer<ScrAction> > *a_scrRecentPasteActions;
		ActionManager* a_actMgr;
};

#endif // APPMODEHELPER_H
