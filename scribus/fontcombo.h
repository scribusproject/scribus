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
#include <q3listbox.h>
#include <qlayout.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QVBoxLayout>

#include "scribusapi.h"
#include "scribusstructs.h"
class Q3ListBoxItem;
class QFont;

class ScribusDoc;
class PrefsManager;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API FontListItem : public QWidget
{
public:
    FontListItem(QComboBox* parent, QString f, QFont fo);
    virtual ~FontListItem() {};

    virtual const int width(const Q3ListBox *);
    virtual const int height(const Q3ListBox *);

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
