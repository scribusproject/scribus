/***************************************************************************
 *   Copyright (C) 2009 by Pierre Marchand                                 *
 *   pierre@oep-h.com                                                      *
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

#ifndef OPENPALETTE_H
#define OPENPALETTE_H

#include "ui/scrpalettebase.h"

#include <QDialog>
#include <QList>
#include <QVBoxLayout>

class PageItemSetterBase;

/**
  * An OpenPalette is aimed to host dropped setters
  * and in some extent to serialize its content and
  * layout in order to reload it over sessions
  */
class OpenPalette : public QDialog
{
	Q_OBJECT

	OpenPalette(){}
public:
	OpenPalette(QWidget * parent);
	~OpenPalette(){}
	QStringList hostedList() const;
	void setHosted(const QStringList& sList);

protected:
	void closeEvent(QCloseEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void dragEnterEvent( QDragEnterEvent *event );
	void dropEvent ( QDropEvent * event );

private:
	QList<PageItemSetterBase*> hosted;
	QVBoxLayout * mainLayout;

signals:
	void changed();

};

#endif // OPENPALETTE_H
