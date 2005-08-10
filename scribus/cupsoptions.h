/***************************************************************************
                          cupsoptions.h  -  description
                             -------------------
    begin                : Fre Jan 3 2003
    copyright            : (C) 2003 by Franz Schmid
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

#ifndef CUPSOPTIONS_H
#define CUPSOPTIONS_H

#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtable.h>
#include <qcombobox.h>
#include <qptrlist.h>
#include <qmap.h>

#include "scribusapi.h"
#include "prefscontext.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API CupsOptions : public QDialog
{
    Q_OBJECT

public: 
	CupsOptions(QWidget* parent, QString Geraet);
	~CupsOptions();
	QPushButton* PushButton1;
	QPushButton* PushButton2;
	QHeader *Header;
	QTable* Table;
	struct OpData { 
					int Cnum;
					QString KeyW;
				  };
	QPtrList<QComboBox> FlagsOpt;
	QMap<QString,OpData> KeyToText;
	QMap<QString,QString> KeyToDefault;

protected:
	QVBoxLayout* CupsOptionsLayout;
	QHBoxLayout* Layout2;
	PrefsContext* prefs;
};

#endif
