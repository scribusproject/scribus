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

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include "stylemanagerbase.h"

class StyleItem;
class ScribusDoc;
class QHideEvent;
class QGridLayout;

class StyleManager : public SMBase {
	Q_OBJECT
public:
	StyleManager(QWidget *parent = 0, const char *name = 0);
	~StyleManager();

	void addStyle(StyleItem *item);

public slots:
	void currentDoc(ScribusDoc *doc);
	void languageChange();

signals:
	void closed();

protected:
	void hideEvent(QHideEvent *e);

private:
	QPtrList<StyleItem> items_;
	StyleItem          *item_;
	QGridLayout        *layout_;
	QWidget            *widget_;

private slots:
	void slotApply();
	void slotDelete();
	void slotImport();
	void slotClone();
	void slotNew();
	void slotPageChanged(QWidget*);
	void slotTypeChanged(QListViewItem*);
	void slotStyleChanged();
	void slotNameChanged(const QString& name);
};

#endif
