/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2008 by Franz Schmid                                     *
*   franz.schmid@altmuehlnet.de                                            *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
*                                                                          *
*   This program is distributed in the hope that it will be useful,        *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*   GNU General Public License for more details.                           *
*                                                                          *
*   You should have received a copy of the GNU General Public License      *
*   along with this program; if not, write to the                          *
*   Free Software Foundation, Inc.,                                        *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.              *
****************************************************************************/

#ifndef TRANSFORMDIALOG_H
#define TRANSFORMDIALOG_H

#include <QDialog>
#include "ui_transformdialogbase.h"
#include "pluginapi.h"
#include "scribusdoc.h"
#include "scribus.h"

class PLUGIN_API TransformItem : public QListWidgetItem
{
public:
	TransformItem(QString text, QListWidget* parent, int type, double val1, double val2);
	~TransformItem() {};
	double firstValue;
	double secondValue;
};

class PLUGIN_API TransformDialog : public QDialog, Ui::TransformDialogBase
{
	Q_OBJECT

public:
	TransformDialog(QWidget* parent, ScribusDoc *doc);
	~TransformDialog() {};
	QMatrix getTransformMatrix();
	int getCount();
	int getBasepoint();

private slots:
	void newScaling();
	void newTranslation();
	void newRotation();
	void newSkewing();
	void setCurrentTransform(QListWidgetItem* item);
	void changeHScale(double val);
	void changeVScale(double val);
	void toggleLink();
	void changeHTranslation(double val);
	void changeVTranslation(double val);
	void changeRotation(double val);
	void changeHSkew(double val);
	void changeVSkew(double val);
	void toggleLinkSkew();
	void moveTransformUp();
	void moveTransformDown();
	void removeTransform();

private:
	QMenu *newTransformMenu;
	ScribusDoc *m_doc;
	double m_unitRatio;
	QString m_suffix;
};

#endif
