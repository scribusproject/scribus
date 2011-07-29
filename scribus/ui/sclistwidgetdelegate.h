/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  sclistwidgetdelegate.h  -  description
							 -------------------
	copyright            : Scribus Team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SCLISTWIDGETDELEGATE_H
#define SCLISTWIDGETDELEGATE_H

#include <QItemDelegate>
#include <QListWidget>
#include "scribusapi.h"

class SCRIBUS_API ScListWidgetDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	ScListWidgetDelegate(QListWidget *view, QWidget *parent);
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const;
	void setIconOnly(bool setter);
	bool iconOnly();

private:
	QListWidget *m_view;
	bool m_dispIconOnly;
};

#endif // SCLISTWIDGETDELEGATE_H
