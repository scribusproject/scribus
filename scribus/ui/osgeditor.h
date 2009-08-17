/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2009 by Franz Schmid                                   *
*   franz.schmid@altmuehlnet.de                                                   *
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

#ifndef OSGEDITOR_H
#define OSGEDITOR_H

#include <QDialog>
#include "ui_osgeditor.h"
#include "scribusapi.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "pageitem_osgframe.h"
#include <osg/StateSet>
#include <osg/Group>
#include <osg/Drawable>
#include <osg/Geometry>

class SCRIBUS_API OSGEditorDialog : public QDialog, Ui::OSGEditor
{
	Q_OBJECT

	public:
		OSGEditorDialog(QWidget* parent, PageItem_OSGFrame *frame);
		~OSGEditorDialog() {};

	public slots:
		void clearDisplay();
		void setCameraValues();
		void changeView(QString viewName);
		void addView();
		void removeView();
		void renameView(QString newName);
		void changeRenderMode(int mode);
		void changeACcolor();
		void changeFCcolor();
		void changeTransparency(int value);
		void changeLightMode(int mode);
		void reportCamera();
		void openFile();
		void accept();

	private:
		void analyse(osg::Node *nd, double transparency);
		void analyseGeode(osg::Geode *geode, double transparency);

	protected:
		PageItem_OSGFrame *currItem;
		QHash<QString, PageItem_OSGFrame::viewDefinition> viewMap;
		PageItem_OSGFrame::viewDefinition currentView;
		QString currentViewName;
		osg::ref_ptr<osg::Group> rootnode;
		osg::ref_ptr<osg::Group> decorator;
		osg::ref_ptr<osg::Node> loadedModel;
		osg::ref_ptr<osg::Node> usedModel;
		osg::ref_ptr<osg::Node> usedModel2;
		osg::ref_ptr<osg::StateSet> default_stateset;
		QString modelFile;
};

#endif
