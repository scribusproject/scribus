/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          spalette.h  -  description
                             -------------------
    begin                : Wed Apr 25 2001
    copyright            : (C) 2001 by Franz Schmid
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

#ifndef SPALETTE_H
#define SPALETTE_H

#include <QComboBox>
#include <QLayout>
#include "scribusapi.h"
class ScribusDoc;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API ParaStyleComboBox : public QComboBox
{
	Q_OBJECT

public: 
	ParaStyleComboBox(QWidget* parent);
	~ParaStyleComboBox() {};
	
	ScribusDoc *currentDoc;
	
public slots:
	void setDoc(ScribusDoc *newCurrentDoc);
	void setFormat(QString name);
	void updateFormatList();
	void selFormat(int e);
	
signals:
	void newStyle(const QString&);
	void editStyle();
};

class SCRIBUS_API CharStyleComboBox : public QComboBox
{
	Q_OBJECT

public: 
	CharStyleComboBox(QWidget* parent);
	~CharStyleComboBox() {};
	
	ScribusDoc *currentDoc;
	
public slots:
	void setDoc(ScribusDoc *newCurrentDoc);
	void setFormat(QString name);
	void updateFormatList();
	void selFormat(int e);
	
signals:
	void newStyle(const QString&);
	void editCharStyle();
};

#endif
