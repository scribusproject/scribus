/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMAGECLIPPINGPATHS_H
#define IMAGECLIPPINGPATHS_H

#include <QWidget>
#include "ui_imageclippingpaths.h"
#include "scribusapi.h"
#include "fpointarray.h"

class QWidget;
class ScribusView;
class PageItem;

class SCRIBUS_API ImageClippingPaths : public QWidget, Ui::ImageClippingPaths
{
	Q_OBJECT

public:
	explicit ImageClippingPaths(QWidget *parent = nullptr);
	~ImageClippingPaths() {};
	void setCurrentItem(PageItem *item, ScribusView *view );

private slots:

	void selectPath(QListWidgetItem *c);
	void noPath();

protected:

	ScribusView *m_view;
	PageItem *m_item;

	void updatePreview();

};

#endif // IMAGECLIPPINGPATHS_H
