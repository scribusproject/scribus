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
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3PtrList>
#include <Q3HBoxLayout>
#include <QLabel>
#include <QCheckBox>

class Q3VBoxLayout;
class Q3HBoxLayout;
class QTabWidget;
class QWidget;
class QLabel;
class QComboBox;
class QSpinBox;
class Q3Table;
class Q3ListBox;
class Q3ListBoxItem;
class QPushButton;

class ScribusView;
class PageItem;
#include "scribusapi.h"
#include "scimage.h"

class SCRIBUS_API ExtImageProps : public QDialog
{
	Q_OBJECT

public:
	ExtImageProps( QWidget* parent, ImageInfoRecord *info, PageItem *item, ScribusView *view );
	~ExtImageProps() {};

	QTabWidget* propsTab;
	QWidget* tab;
	QLabel* textLabel1;
	QComboBox* blendMode;
	QLabel* textLabel2;
	QSpinBox* opacitySpinBox;
	Q3Table* layerTable;
	QWidget* tab_2;
	Q3ListBox* pathList;
	QPushButton* resetPath;
	Q3PtrList<QCheckBox> FlagsSicht;
	Q3PtrList<QCheckBox> FlagsMask;
	ScribusView *viewWidget;
	PageItem *currentItem;
	int currentLayer;
	QMap<QString, QString> blendModes;
	QMap<QString, QString> blendModesRev;

public slots:
	void changedLayer();
	void selLayer(int layer);
	void selPath(Q3ListBoxItem *c);
	void noPath();

protected:
	Q3VBoxLayout* ExtImagePropsLayout;
	Q3VBoxLayout* tabLayout;
	Q3HBoxLayout* layout1;
	Q3VBoxLayout* tabLayout_2;

};

#endif // EXTIMAGEPROPS_H
