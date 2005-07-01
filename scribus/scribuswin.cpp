/***************************************************************************
                          scribuswin.cpp  -  description
                             -------------------
    begin                : Mit Nov 6 2002
    copyright            : (C) 2002 by Franz Schmid
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
#include <qfileinfo.h>
#include <qdir.h>
#include "scribuswin.h"
#include "scribuswin.moc"
#include "scribusXml.h"
#include "scribus.h"
#include "story.h"
#include "util.h"

extern ScribusApp* ScApp;

ScribusWin::ScribusWin(QWidget* parent, ScribusDoc* ddoc) : QMainWindow(parent, "", WDestructiveClose)
{
	setIcon(loadIcon("AppIcon2.png"));
	doc = ddoc;
	muster = NULL;
}

void ScribusWin::setView(ScribusView* dview)
{
	view = dview;
	doc->viewCount++;
	doc->viewID++;
	winIndex = doc->viewID;
}

void ScribusWin::slotAutoSave()
{
	if ((doc->hasName) && (doc->isModified()))
	{
		moveFile(doc->DocName, doc->DocName+".bak");
		QString fn = doc->DocName;
		QFileInfo fi(fn);
		QDir::setCurrent(fi.dirPath(true));
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (ss->WriteDoc(fn, doc, 0))
		{
			doc->setUnModified();
			setCaption(doc->DocName);
			qApp->processEvents();
			emit AutoSaved();
		}
		delete ss;
	}
}

void ScribusWin::closeEvent(QCloseEvent *ce)
{
	if (doc->isModified() && (doc->viewCount == 1))
	{
		QString CloseTxt;
		if (ScApp->singleClose)
			CloseTxt = tr("&Leave Anyway");
		else
			CloseTxt = tr("C&lose Anyway");
		int exit=QMessageBox::information(this,
		                                  tr("Warning"),
		                                  tr("Document:")+" "+doc->DocName+"\n"+ tr("has been changed since the last save."),
		                                  tr("&Save Now"),
		                                  tr("&Cancel"),
		                                  CloseTxt,
		                                  0, 1);
		switch (exit)
		{
		case 0:
			if (ScApp->DoSaveClose())
			{
				if (doc==ScApp->storyEditor->currentDocument())
					ScApp->storyEditor->close();
				ce->accept();
			}
			else
				return;
			break;
		case 1:
			break;
		case 2:
			emit Schliessen();
			ce->accept();
			break;
		}
	}
	else
	{
		emit Schliessen();
		ce->accept();
	}
}
