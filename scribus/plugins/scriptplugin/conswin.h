/***************************************************************************
                          conswin.h  -  description
                             -------------------
    begin                : Mon Okt 14 2002
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

#ifndef CONSWIN_H
#define CONSWIN_H

#include <qtextedit.h>

/**
  *@author Franz Schmid
  */

class ConsWin : public QTextEdit
{
    Q_OBJECT
public: 
	ConsWin(QWidget* parent);
	~ConsWin() {} ;
	void keyPressEvent(QKeyEvent *k);
	void contentsMousePressEvent(QMouseEvent *m);
	void contentsMouseReleaseEvent(QMouseEvent *) {return;}
	void contentsMouseMoveEvent(QMouseEvent *) {return;}
	void contentsMouseDoubleClickEvent(QMouseEvent *) {return;}
	QString LastComm;
	QString Prompt;
signals:
	void closeFromKeyB();
};

#endif
