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
class ScribusView;
class ScribusDoc;
#include "scribusapi.h"
#include "muster.h"
/**
  *@author Franz Schmid
  */

class SCRIBUS_API ScribusWin : public QMainWindow
{
	Q_OBJECT

public: 
	ScribusWin(QWidget* parent, ScribusDoc* doc);
	~ScribusWin() {};
	void closeEvent(QCloseEvent *ce);
	void setView(ScribusView* newView);
	ScribusView* view() const { return m_View;}
	ScribusDoc* document() const { return m_Doc;}
	void setMasterPagesPalette(MasterPagesPalette* newMPP) { m_masterPagesPalette=newMPP; }
	MasterPagesPalette* masterPagesPalette() const { return m_masterPagesPalette; }
	void setMenuStatus(int index, bool value) { if (index>=0 && index <=6) MenuStat[index]=value;}
	bool menuStatus(int index) const { if (index>=0 && index <=6) return MenuStat[index]; else return false; }
	void setMasterPagesPaletteShown(bool isShown) const;
	
public slots:
	void slotAutoSave();
	
signals:
	void AutoSaved();

protected:
	QHBoxLayout* statusFrameLayout;
	ScribusView* m_View;
	ScribusDoc* m_Doc;
	MasterPagesPalette* m_masterPagesPalette;
	QFrame *statusFrame;
	bool MenuStat[7];
	int winIndex;
};

#endif
