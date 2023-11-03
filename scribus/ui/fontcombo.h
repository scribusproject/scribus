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
#include <QFontDatabase>
#include <QValidator>

#include "scribusapi.h"

class QEvent;
class QFont;
class QGridLayout;
class QPixmap;
class QLabel;
class PrefsManager;
class ScFace;
class ScribusDoc;


/**
  *@author Franz Schmid
  */

class SCRIBUS_API FontCombo : public QComboBox
{
	Q_OBJECT
public:
	FontCombo(QWidget* pa);
	~FontCombo() {}

	void RebuildList(ScribusDoc *currentDoc, bool forAnnotation = false, bool forSubstitute = false);

private:
	PrefsManager& prefsManager;
	QPixmap ttfFont;
	QPixmap otfFont;
	QPixmap psFont;
	QPixmap substFont;

private slots:
	void iconSetChange();
};

class SCRIBUS_API FontComboH : public QWidget
{
	Q_OBJECT

public:
	FontComboH(QWidget* parent, bool labels = false);
	~FontComboH() {}
	
	QString currentFont() const;
	void rebuildList(ScribusDoc *currentDoc, bool forAnnotation = false, bool forSubstitute = false);

public slots:
	void setCurrentFont(const QString& f);
	void languageChange();

signals:
	void fontSelected(QString);

protected slots:
	void familySelected(int id);
	void styleSelected(int id);

protected:
	QComboBox* fontFamily { nullptr };
	QComboBox* fontStyle { nullptr };
	QLabel *fontFaceLabel { nullptr };
	QLabel *fontStyleLabel { nullptr };
	QGridLayout* fontComboLayout { nullptr };
	PrefsManager& prefsManager;
	ScribusDoc *currDoc { nullptr };
	bool showLabels { false };
	bool isForAnnotation { true };  // this is merely to ensure that the list is rebuilt
	bool isForSubstitute { false };
	QPixmap ttfFont;
	QPixmap otfFont;
	QPixmap psFont;
	QPixmap substFont;

	void changeEvent(QEvent *e) override;
};

// based on Qfontcombobox class from Qt project
class FontFamilyDelegate : public QAbstractItemDelegate
{
	Q_OBJECT
	public:
		explicit FontFamilyDelegate(QObject *parent);

		// painting
		void paint(QPainter *painter,
				   const QStyleOptionViewItem &option,
				   const QModelIndex &index) const Q_DECL_OVERRIDE;

		bool helpEvent(QHelpEvent * event, QAbstractItemView * view,
					   const QStyleOptionViewItem & option, const QModelIndex & index) Q_DECL_OVERRIDE;

		QSize sizeHint(const QStyleOptionViewItem &option,
					   const QModelIndex &index) const Q_DECL_OVERRIDE;

		QFontDatabase::WritingSystem writingSystem { QFontDatabase::Any };

	protected:
		QFont m_font;
};

class FontComboValidator : public QValidator
{
	Q_OBJECT

	public:
		FontComboValidator(QObject* parent = nullptr);
		virtual State validate(QString & input, int & pos) const;
};
#endif
