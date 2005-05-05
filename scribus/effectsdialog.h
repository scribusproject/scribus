#ifndef EFFECTSDIALOG_H
#define EFFECTSDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include <qvaluelist.h>
#include <qmap.h>
#include "pageitem.h"
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QListBox;
class QListBoxItem;
class QPushButton;
class QComboBox;
class ScribusDoc;
class ShadeButton;
class QWidgetStack;
class QWidget;
class QSlider;
class MSpinBox;

class EffectsDialog : public QDialog
{
	Q_OBJECT

public:
	EffectsDialog( QWidget* parent, PageItem* item, ScribusDoc* docc );
	~EffectsDialog() {};

	QLabel* pixmapLabel1;
	QLabel* textLabel1;
	QLabel* textLabel2;
	QLabel* textLabel3;
	QLabel* textLabel4;
	QLabel* textLabel5;
	QLabel* textLabel6;
	QLabel* textLabel7;
	QLabel* textLabel8;
	QLabel* textLabel9;
	QLabel* textLabel10;
	QLabel* textLabel11;
	QLabel* textLabel12;
	QLabel* textLabel13;
	QLabel* textLabel14;
	QLabel* textLabel15;
	QWidgetStack* optionStack;
	QWidget* WStackPage;
	QWidget* WStackPage_2;
	QComboBox* colData;
	ShadeButton *shade;
	QWidget* WStackPage_3;
	QSlider* brightnessSlider;
	QWidget* WStackPage_4;
	QSlider* contrastSlider;
	QWidget* WStackPage_5;
	MSpinBox* shRadius;
	MSpinBox* shValue;
	QWidget* WStackPage_6;
	MSpinBox* blRadius;
	MSpinBox* blValue;
	QWidget* WStackPage_7;
	QSlider* solarizeSlider;
	QListBox* usedEffects;
	QPushButton* effectUp;
	QPushButton* effectDown;
	QPushButton* toEffects;
	QPushButton* fromEffects;
	QListBox* availableEffects;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QValueList<ScImage::imageEffect> effectsList;
	QMap<QListBoxItem*, QString> effectValMap;
	QListBoxItem* currentOptions;
	ScribusDoc* doc;
	PageItem* currItem;
	ScImage image;

public slots:
	virtual void leaveOK();
	virtual void updateSolarize(int val);
	virtual void updateContrast(int val);
	virtual void updateBright(int val);
	virtual void createPreview();
	virtual void saveValues();
	virtual void moveToEffects();
	virtual void moveFromEffects();
	virtual void moveEffectUp();
	virtual void moveEffectDown();
	virtual void selectEffect(QListBoxItem* c);
	virtual void selectAvailEffect(QListBoxItem* c);

protected:
	QHBoxLayout* EffectsDialogLayout;
	QVBoxLayout* WStackPageLayout;
	QVBoxLayout* WStackPage3Layout;
	QHBoxLayout* layout20;
	QVBoxLayout* WStackPage4Layout;
	QHBoxLayout* layout21;
	QVBoxLayout* WStackPage5Layout;
	QHBoxLayout* layout22;
	QHBoxLayout* layout23;
	QVBoxLayout* WStackPage6Layout;
	QHBoxLayout* layout24;
	QHBoxLayout* layout25;
	QVBoxLayout* WStackPage7Layout;
	QHBoxLayout* layout26;
	QHBoxLayout* layout17;
	QHBoxLayout* layout19;
	QGridLayout* layout10;
	QVBoxLayout* layout8;
	QHBoxLayout* layout7;
	QVBoxLayout* layout1;
	QVBoxLayout* layout2;
	QVBoxLayout* layout16;
	QVBoxLayout* layout18;
	QHBoxLayout* layout9;
};

#endif // EFFECTSDIALOG_H
