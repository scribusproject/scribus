/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EXTIMAGEPROPS_H
#define EXTIMAGEPROPS_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;
class QWidget;
class QLabel;
class QComboBox;
class QSpinBox;
class QTable;
class QListBox;
class QListBoxItem;
class QCheckBox;
class QPushButton;

class ScribusView;
class PageItem;
#include "scribusapi.h"
#include "scimage.h"

class SCRIBUS_API ExtImageProps : public QDialog
{
	Q_OBJECT

public:
	ExtImageProps( QWidget* parent, ScImage::ImageInfoRecord *info, PageItem *item, ScribusView *view );
	~ExtImageProps() {};

	QTabWidget* propsTab;
	QWidget* tab;
	QLabel* textLabel1;
	QComboBox* blendMode;
	QLabel* textLabel2;
	QSpinBox* opacitySpinBox;
	QTable* layerTable;
	QWidget* tab_2;
	QListBox* pathList;
	QPushButton* resetPath;
	QPtrList<QCheckBox> FlagsSicht;
	ScribusView *viewWidget;
	PageItem *currentItem;
	int currentLayer;
	QMap<QString, QString> blendModes;
	QMap<QString, QString> blendModesRev;

public slots:
	void changedLayer();
	void selLayer(int layer);
	void selPath(QListBoxItem *c);
	void noPath();

protected:
	QVBoxLayout* ExtImagePropsLayout;
	QVBoxLayout* tabLayout;
	QHBoxLayout* layout1;
	QVBoxLayout* tabLayout_2;

};

#endif // EXTIMAGEPROPS_H
