/***************************************************************************
                          scribuswin.h  -  description
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

#ifndef SCRIBUSWIN_H
#define SCRIBUSWIN_H

#include <qmainwindow.h>
#include <qmessagebox.h>
#include "scribusview.h"
class ScribusDoc;
#include "muster.h"
/**
  *@author Franz Schmid
  */

class ScribusWin : public QMainWindow
{
	Q_OBJECT

public: 
	ScribusWin(QWidget* parent, ScribusDoc* ddoc);
	~ScribusWin() {};
	void setView(ScribusView* dview);
	void closeEvent(QCloseEvent *ce);
	void OpenCMSProfiles(ProfilesL InPo, ProfilesL MoPo, ProfilesL PrPo);
	void CloseCMSProfiles();
	ScribusView* view;
	ScribusDoc* doc;
	MusterPages* muster;
	bool MenuStat[7];
#ifdef HAVE_CMS
	cmsHTRANSFORM stdTrans;
	cmsHTRANSFORM stdProof;
	cmsHTRANSFORM stdTransImg;
	cmsHTRANSFORM stdProofImg;
	bool SoftProofing;
	bool Gamut;
	int IntentMonitor;
	int IntentPrinter;
#endif
	int	NrItems;
	int First;
	int Last;
	
public slots:
	void slotAutoSave();
	
signals:
	void Schliessen();
	void AutoSaved();
};

#endif
