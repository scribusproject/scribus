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


#ifndef CANVASMODE_IMAGEIMPORT_H
#define CANVASMODE_IMAGEIMPORT_H

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QStringList>

#include "canvasmode.h"

class PageItem;
class ScribusView;
class ScribusMainWindow;

class CanvasMode_ImageImport : public CanvasMode
{
	Q_OBJECT

public:
	explicit CanvasMode_ImageImport(ScribusView* view);
	 ~CanvasMode_ImageImport() override = default;

	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;

	void activate(bool) override;
	void deactivate(bool) override;
	void keyPressEvent(QKeyEvent *e) override;
	void mouseDoubleClickEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	void drawControls(QPainter* p) override;

	void setImageList(QStringList l);
	void setImage(PageItem* item);
	void updateList();

private:
	bool m_keyRepeat {false};
	QString m_tipText;
	QStringList m_imageList;
	ScribusMainWindow *m_ScMW {nullptr};
	double m_Mx {0.0};
	double m_My {0.0};
	void newToolTip(const QString& name);
};

#endif // CANVASMODE_IMAGEIMPORT_H
