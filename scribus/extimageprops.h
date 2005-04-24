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
#include "scribus.h"
#include "scimage.h"

class ExtImageProps : public QDialog
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

protected:
	QVBoxLayout* ExtImagePropsLayout;
	QVBoxLayout* tabLayout;
	QHBoxLayout* layout1;
	QVBoxLayout* tabLayout_2;

};

#endif // EXTIMAGEPROPS_H
