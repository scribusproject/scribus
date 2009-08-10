/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#ifndef ADAPT_H
#define ADAPTC_H

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include <osgViewer/Viewer>
#include <osg/Vec3>

class AdapterWidget : public QGLWidget
{
    Q_OBJECT
	public:

		AdapterWidget ( QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, Qt::WFlags f = 0 );

		virtual ~AdapterWidget() {}

		osgViewer::GraphicsWindow* getGraphicsWindow() { return _gw.get(); }
		const osgViewer::GraphicsWindow* getGraphicsWindow() const { return _gw.get(); }

	signals:
		void mouseMoved();
	protected:

		void init();

		virtual void resizeGL ( int width, int height );
		virtual void keyPressEvent ( QKeyEvent* event );
		virtual void keyReleaseEvent ( QKeyEvent* event );
		virtual void mousePressEvent ( QMouseEvent* event );
		virtual void mouseReleaseEvent ( QMouseEvent* event );
		virtual void mouseMoveEvent ( QMouseEvent* event );

		int button;
		osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
};


class ViewerQT : public osgViewer::Viewer, public AdapterWidget
{
	public:

		ViewerQT ( QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, Qt::WFlags f = 0 ) :
				AdapterWidget ( parent, name, shareWidget, f )
		{
			getCamera()->setViewport ( new osg::Viewport ( 0,0,width(),height() ) );
			getCamera()->setProjectionMatrixAsPerspective ( 30.0f, static_cast<double> ( width() ) /static_cast<double> ( height() ), 1.0f, 10000.0f );
			QColor bg = palette().window().color();
			getCamera()->setClearColor(osg::Vec4(bg.redF(), bg.greenF(), bg.blueF(), 0.0));
			getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			getCamera()->setGraphicsContext ( getGraphicsWindow() );

			setThreadingModel ( osgViewer::Viewer::SingleThreaded );

			connect ( &_timer, SIGNAL ( timeout() ), this, SLOT ( updateGL() ) );
			_timer.start ( 10 );
		}

		virtual void paintGL()
		{
			frame();
		}

	protected:

		QTimer _timer;
};
#endif
