/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          stylecombos.h  -  description
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

#ifndef STYLECOMBOS_H
#define STYLECOMBOS_H

#include <QComboBox>
#include <QLayout>
#include "scribusapi.h"

class QEvent;
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

	QString currentStyle() const;

	const QString& defaultStyle() const { return m_defaultStyle; }
	void setDefaultStyle(const QString& defStyle);
	
public slots:
	void setDoc(ScribusDoc *newCurrentDoc);
	void setStyle(const QString& name);
	void updateStyleList();

protected:
	ScribusDoc *m_doc { nullptr };
	QString m_defaultStyle;

	QString firstItemString();

	void changeEvent(QEvent *e) override;
	void languageChange();

protected slots:
	void selectedStyle(int e);
	
signals:
	void newStyle(const QString&);
};

class SCRIBUS_API CharStyleComboBox : public QComboBox
{
	Q_OBJECT

public: 
	CharStyleComboBox(QWidget* parent);
	~CharStyleComboBox() {};

	QString currentStyle() const;

	const QString& defaultStyle() const { return m_defaultStyle; }
	void setDefaultStyle(const QString& defStyle);
	
public slots:
	void setDoc(ScribusDoc *newCurrentDoc);
	void setStyle(const QString& name);
	void updateStyleList();

protected:
	ScribusDoc *m_doc { nullptr };
	QString m_defaultStyle;

	QString firstItemString();

	void changeEvent(QEvent *e) override;
	void languageChange();

protected slots:
	void selectedStyle(int e);
	
signals:
	void newStyle(const QString&);
};

class SCRIBUS_API CellStyleComboBox : public QComboBox
{
	Q_OBJECT

public:
	CellStyleComboBox(QWidget* parent);
	~CellStyleComboBox() {};

	QString currentStyle() const;

	const QString& defaultStyle() const { return m_defaultStyle; }
	void setDefaultStyle(const QString& defStyle);

public slots:
	void setDoc(ScribusDoc *newCurrentDoc);
	void setStyle(const QString& name);
	void updateStyleList();

protected:
	ScribusDoc *m_doc { nullptr };
	QString m_defaultStyle;

	QString firstItemString();

	void changeEvent(QEvent *e) override;
	void languageChange();

protected slots:
	void selectedStyle(int e);

signals:
	void newStyle(const QString&);
};

class SCRIBUS_API TableStyleComboBox : public QComboBox
{
	Q_OBJECT

public:
	TableStyleComboBox(QWidget* parent);
	~TableStyleComboBox() {};

	QString currentStyle() const;

	const QString& defaultStyle() const { return m_defaultStyle; }
	void setDefaultStyle(const QString& defStyle);

public slots:
	void setDoc(ScribusDoc *newCurrentDoc);
	void setStyle(const QString& name);
	void updateStyleList();

protected:
	ScribusDoc *m_doc { nullptr };
	QString m_defaultStyle;

	QString firstItemString();

	void changeEvent(QEvent *e) override;
	void languageChange();

protected slots:
	void selectedStyle(int e);

signals:
	void newStyle(const QString&);
};

#endif
