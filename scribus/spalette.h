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

#include <qcombobox.h>
#include <qlayout.h>
#include "scribusapi.h"
class ScribusDoc;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API Spalette : public QComboBox  
{
	Q_OBJECT

public: 
	Spalette(QWidget* parent);
	~Spalette() {};
	
	ScribusDoc *currentDoc;
	
public slots:
	void setFormats(ScribusDoc *newCurrentDoc);
	void setFormat(int e);
	void updateFormatList();
	void selFormat(int e);
	
signals:
	void newStyle(int);
	void editStyle();
};

#endif
