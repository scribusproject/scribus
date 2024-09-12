/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EXTIMAGEPROPS_H
#define EXTIMAGEPROPS_H

#include <QDialog>
#include <QList>

class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;
class QWidget;
class QLabel;
class QComboBox;
class QCheckBox;
class ScrSpinBox;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QTableWidget;
class QTimer;

class ScribusView;
class PageItem;

#include "ui_extimageprops.h"
#include "scribusapi.h"
#include "scimage.h"


class SCRIBUS_API ExtImageProps : public QDialog, Ui::ExtImageProps
{
	Q_OBJECT

public:
	ExtImageProps( QWidget* parent, PageItem *item, ScribusView *view );
	~ExtImageProps() {}

public slots:
	void accept() override;
	void reject() override;

protected:

	ScribusView *m_view;
	PageItem *m_item;

	ImageInfoRecord originalInfo;
	FPointArray originalImageClip;
};

#endif // EXTIMAGEPROPS_H
