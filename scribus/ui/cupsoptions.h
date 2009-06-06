/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QDialog>
#include <QMap>
#include <QList>
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QComboBox;
class QTableWidget;

#include "scribusapi.h"

/**
  *@author Franz Schmid
  */

class PrefsContext;

class SCRIBUS_API CupsOptions : public QDialog
{
    Q_OBJECT

public: 
	CupsOptions(QWidget* parent, QString Geraet);
	~CupsOptions();
	QPushButton* PushButton1;
	QPushButton* PushButton2;
	QTableWidget* Table;
	struct OpData { 
					int Cnum;
					QString KeyW;
				  };
	QList<QComboBox*> FlagsOpt;
	QMap<QString,OpData> KeyToText;
	QMap<QString,QString> KeyToDefault;

protected:
	QVBoxLayout* CupsOptionsLayout;
	QHBoxLayout* Layout2;
	PrefsContext* prefs;
};

#endif
