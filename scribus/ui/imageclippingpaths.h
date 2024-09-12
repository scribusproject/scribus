/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMAGECLIPPINGPATHS_H
#define IMAGECLIPPINGPATHS_H

#include <QWidget>

#include "fpointarray.h"
#include "scribusapi.h"
#include "ui_imageclippingpaths.h"

class PageItem;
class QWidget;
class ScribusView;

class SCRIBUS_API ImageClippingPaths : public QWidget, Ui::ImageClippingPaths
{
	Q_OBJECT

public:
	explicit ImageClippingPaths(QWidget *parent = nullptr);

	void setCurrentItem(PageItem *item, ScribusView *view);

private slots:
	void selectPath(QListWidgetItem *c);
	void noPath();

protected:
	ScribusView *m_view { nullptr };
	PageItem *m_item { nullptr };

	void updatePreview();
};

#endif // IMAGECLIPPINGPATHS_H
