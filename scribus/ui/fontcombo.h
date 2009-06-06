/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          fontcombo.h  -  description
                             -------------------
    begin                : Die Jun 17 2003
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

#ifndef FONTCOMBO_H
#define FONTCOMBO_H

#include <QWidget>
#include <QComboBox>
#include "scribusapi.h"

class QEvent;
class QFont;
class QGridLayout;
class QPixmap;
class QLabel;
class ScribusDoc;
class PrefsManager;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API FontCombo : public QComboBox
{
public:
	FontCombo(QWidget* pa);
	~FontCombo() {};
	
	

	void RebuildList(ScribusDoc *currentDoc, bool forAnnotation = false, bool forSubstitute = false);
private:
	PrefsManager* prefsManager;
	QPixmap ttfFont;
	QPixmap otfFont;
	QPixmap psFont;
	QPixmap substFont;
};

class SCRIBUS_API FontComboH : public QWidget
{

	Q_OBJECT

public:
	FontComboH(QWidget* parent, bool labels=false);
	~FontComboH() {};
	
	virtual void changeEvent(QEvent *e);
	
	QString currentFont();
	void RebuildList(ScribusDoc *currentDoc, bool forAnnotation = false, bool forSubstitute = false);

public slots:
	void setCurrentFont(QString f);
	void languageChange();

signals:
	void fontSelected(QString);

protected slots:
	void familySelected(int id);
	void styleSelected(int id);

protected:
	QComboBox* fontFamily;
	QComboBox* fontStyle;
	QLabel *fontFaceLabel;
	QLabel *fontStyleLabel;
	PrefsManager* prefsManager;
	ScribusDoc *currDoc;
	QPixmap ttfFont;
	QPixmap otfFont;
	QPixmap psFont;
	QPixmap substFont;
	QGridLayout* fontComboLayout;
	bool showLabels;
};

#endif
