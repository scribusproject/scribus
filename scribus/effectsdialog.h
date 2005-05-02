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

class EffectsDialog : public QDialog
{
	Q_OBJECT

public:
	EffectsDialog( QWidget* parent, PageItem* item, ScribusDoc* docc );
	~EffectsDialog() {};

	QLabel* pixmapLabel1;
	QLabel* textLabel5;
	QLabel* textLabel4;
	QLabel* textLabel3;
	QWidgetStack* optionStack;
	QWidget* WStackPage;
	QWidget* WStackPage_2;
	QComboBox* colData;
	ShadeButton *shade;
	QLabel* textLabel2;
	QListBox* usedEffects;
	QPushButton* effectUp;
	QPushButton* effectDown;
	QPushButton* toEffects;
	QPushButton* fromEffects;
	QLabel* textLabel1;
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
