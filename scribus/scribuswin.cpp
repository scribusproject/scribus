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

extern QPixmap loadIcon(QString nam);
extern ScribusApp* ScApp;
extern int moveFile(QString source, QString target);

ScribusWin::ScribusWin(QWidget* parent, ScribusDoc* ddoc)
		: QMainWindow(parent, "", WDestructiveClose)
{
	setIcon(loadIcon("AppIcon.png"));
	doc = ddoc;
	muster = NULL;
	NrItems = 0;
	First = 1;
	Last = 0;
}

void ScribusWin::setView(ScribusView* dview)
{
	view = dview;
}

void ScribusWin::slotAutoSave()
{
	if ((doc->hasName) && (doc->isModified()) && (!doc->TemplateMode))
	{
		moveFile(doc->DocName, doc->DocName+".bak");
		QString fn = doc->DocName;
		QFileInfo fi(fn);
		QDir::setCurrent(fi.dirPath(true));
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (ss->WriteDoc(fn, doc, view, 0))
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
	if (doc->isModified())
	{
		QString CloseTxt;
		if (ScApp->singleClose)
			CloseTxt = tr("Leave Anyway");
		else
			CloseTxt = tr("Close Anyway");
		int exit=QMessageBox::information(this,
		                                  tr("Warning"),
		                                  tr("Document:")+" "+doc->DocName+"\n"+ tr("has been changed since the last save."),
		                                  tr("Save Now"),
		                                  tr("Cancel"),
		                                  CloseTxt,
		                                  0, 1);
		switch (exit)
		{
		case 0:
			emit SaveAndClose();
			ce->accept();
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

void ScribusWin::CloseCMSProfiles()
{
#ifdef HAVE_CMS
	cmsCloseProfile(doc->DocInputProf);
	cmsCloseProfile(doc->DocOutputProf);
	cmsCloseProfile(doc->DocPrinterProf);
	cmsDeleteTransform(stdTrans);
	cmsDeleteTransform(stdProof);
	cmsDeleteTransform(stdTransImg);
	cmsDeleteTransform(stdProofImg);
#endif
}

void ScribusWin::OpenCMSProfiles(ProfilesL InPo, ProfilesL MoPo, ProfilesL PrPo)
{
#ifdef HAVE_CMS
	doc->DocInputProf = cmsOpenProfileFromFile(InPo[doc->CMSSettings.DefaultInputProfile2], "r");
	doc->DocOutputProf = cmsOpenProfileFromFile(MoPo[doc->CMSSettings.DefaultMonitorProfile], "r");
	doc->DocPrinterProf = cmsOpenProfileFromFile(PrPo[doc->CMSSettings.DefaultPrinterProfile], "r");
	if ((doc->DocInputProf == NULL) || (doc->DocOutputProf == NULL) || (doc->DocPrinterProf == NULL))
	{
		doc->CMSSettings.CMSinUse = false;
		return;
	}
	int dcmsFlags = 0;
	int dcmsFlags2 = cmsFLAGS_NOTPRECALC;
	if (Gamut)
		dcmsFlags |= cmsFLAGS_GAMUTCHECK;
	else
		dcmsFlags |= cmsFLAGS_SOFTPROOFING;
#ifdef cmsFLAGS_BLACKPOINTCOMPENSATION
	if (doc->CMSSettings.BlackPoint)
	{
		dcmsFlags2 |= cmsFLAGS_BLACKPOINTCOMPENSATION;
		dcmsFlags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
	}
#endif
	stdProof = cmsCreateProofingTransform(doc->DocInputProf, TYPE_RGB_16,
	                                      doc->DocOutputProf, TYPE_RGB_16,
	                                      doc->DocPrinterProf,
	                                      IntentPrinter,
	                                      IntentMonitor, dcmsFlags);
	stdTrans = cmsCreateTransform(doc->DocInputProf, TYPE_RGB_16,
	                              doc->DocOutputProf, TYPE_RGB_16,
	                              IntentMonitor,
	                              dcmsFlags2);
	stdProofImg = cmsCreateProofingTransform(doc->DocInputProf, TYPE_RGBA_8,
	              doc->DocOutputProf, TYPE_RGBA_8,
	              doc->DocPrinterProf,
	              IntentPrinter,
	              IntentMonitor, dcmsFlags);
	stdTransImg = cmsCreateTransform(doc->DocInputProf, TYPE_RGBA_8,
	                                 doc->DocOutputProf, TYPE_RGBA_8,
	                                 IntentMonitor,
	                                 dcmsFlags2);
#endif
}
