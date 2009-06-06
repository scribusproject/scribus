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
class QSpinBox;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QTableWidget;

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
	QTableWidget* layerTable;
	QWidget* tab_2;
	QListWidget* pathList;
	QPushButton* resetPath;
	QCheckBox* livePreview;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QList<QCheckBox*> FlagsSicht;
	QList<QCheckBox*> FlagsMask;
	ScribusView *viewWidget;
	PageItem *currentItem;
	int currentLayer;
	bool doPreview;
	ImageInfoRecord originalInfo;
	FPointArray originalImageClip;
	QMap<QString, QString> blendModes;
	QMap<QString, QString> blendModesRev;

public slots:
	void leaveOK();
	void leaveCancel();
	void changePreview();
	void changedLayer();
	void selLayer(int layer);
	void selPath(QListWidgetItem *c);
	void noPath();

protected:
	QVBoxLayout* ExtImagePropsLayout;
	QVBoxLayout* tabLayout;
	QVBoxLayout* tabLayout_2;
	QHBoxLayout* layout1;
	QHBoxLayout* layoutBottom;

};

#endif // EXTIMAGEPROPS_H
