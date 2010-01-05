/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 /***************************************************************************
  *   Copyright (C) 2009 by Jain Basil Aliyas                               *
  *   mail@jainbasil.net                                                    *
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

#ifndef FONTSELECTDIALOG_H
#define FONTSELECTDIALOG_H

#include <QString>
#include <QStringList>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QComboBox>

class QPushButton;
class QComboBox;
class QLabel;
class QWidget;

class FontSelect:public QDialog
{
Q_OBJECT

	public:
		FontSelect(QStringList list);
		~FontSelect(){};
		QString setFont();

	private slots:
		void selectFont();
		void resetFont();
	private:
		QLabel *label;
		QComboBox *fontList;
		QPushButton *okButton;
		QString font;
		QWidget *layoutWidget;
		QVBoxLayout *verticalLayout_2;
		QHBoxLayout *horizontalLayout_2;
		QSpacerItem *horizontalSpacer;
		QHBoxLayout *horizontalLayout_3;
};

#endif
