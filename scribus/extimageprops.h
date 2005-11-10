#ifndef EXTIMAGEPROPS_H
#define EXTIMAGEPROPS_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3PtrList>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;
class QWidget;
class QLabel;
class QComboBox;
class QSpinBox;
class Q3Table;
class Q3ListBox;
class Q3ListBoxItem;
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
	Q3Table* layerTable;
	QWidget* tab_2;
	Q3ListBox* pathList;
	QPushButton* resetPath;
	Q3PtrList<QCheckBox> FlagsSicht;
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
	QVBoxLayout* ExtImagePropsLayout;
	QVBoxLayout* tabLayout;
	QHBoxLayout* layout1;
	QVBoxLayout* tabLayout_2;

};

#endif // EXTIMAGEPROPS_H
