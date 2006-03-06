/***************************************************************************
 *   Copyright (C) 2005 by Riku Leino                                      *
 *   riku@scribus.info                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SMLINESTYLE_H
#define SMLINESTYLE_H

#include "scribusstructs.h"
#include "styleitem.h"
#include "smlinestylewbase.h"

#include <qobject.h>
#include <qcolor.h>

class ScribusDoc;
class LineCombo;
class MSpinBox;

class LineStyleWidget : public LineStyleWBase // .ui implementation
{
	Q_OBJECT
public:
	LineStyleWidget();
	~LineStyleWidget();
	void showStyle(const multiLine &lineStyle, ColorList &colorList);
private:
	LineCombo *dashCombo;
	MSpinBox  *lineWidth;
	multiLine  currentStyle;
	ColorList  colors;

	void updateLineList();
	QColor getColor(const QString &name, int shade);

private slots:
	void slotEditNewLine(int);
};

class SMLineStyle : public StyleItem
{
	Q_OBJECT
public:
	SMLineStyle();
	~SMLineStyle();
	QTabWidget* widget();
	QString typeName();
	void currentDoc(ScribusDoc *doc);
	QStringList styles();
	void selected(const QStringList &styleNames);
	void apply();
	void deleteSelected();
	void nameChanged(const QString &newName);

signals:
	void deleteDone();

private:
	ScribusDoc              *doc_;
	LineStyleWidget         *widget_;
	QTabWidget              *twidget_;
	QMap<QString, multiLine> tmpLines;

	void reset();
	void setSelection(const QString& styleName);
	void setMultiSelection(const QStringList& styles);
};

#endif
