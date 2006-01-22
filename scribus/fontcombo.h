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

#include <qcombobox.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qwidget.h>

#include "scribusapi.h"
#include "scribusstructs.h"
class QListBoxItem;
class QFont;

class ScribusDoc;
class PrefsManager;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API FontListItem : public QListBoxItem
{
public:
    FontListItem(QComboBox* parent, QString f, QFont fo);
    virtual ~FontListItem() {};

    virtual const int width(const QListBox *);
    virtual const int height(const QListBox *);

protected:
    virtual void paint(QPainter *p);

private:
    QFont iFont;
    QString fontName;
};

class SCRIBUS_API FontCombo : public QComboBox  
{
public:
	FontCombo(QWidget* pa);
	~FontCombo() {};

	void RebuildList(ScribusDoc *currentDoc);
private:
	PrefsManager* prefsManager;
};

class SCRIBUS_API FontComboH : public QWidget
{

	Q_OBJECT

public:
	FontComboH(QWidget* parent);
	~FontComboH() {};
	QString currentFont();
	void RebuildList(ScribusDoc *currentDoc);

public slots:
	void setCurrentFont(QString f);

signals:
	void fontSelected(QString);

private slots:
	void familySelected(int id);
	void styleSelected(int id);

private:
	QComboBox* fontFamily;
	QComboBox* fontStyle;
	PrefsManager* prefsManager;
	ScribusDoc *currDoc;

protected:
	QVBoxLayout* fontComboLayout;
};

#endif
