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

class ColorEffectVals : public QDialog
{
	Q_OBJECT

public:
	ColorEffectVals(QWidget* parent, QString colorName, ScribusDoc* doc );
	~ColorEffectVals() {};

	QLabel* label1;
	QLabel* label2;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QComboBox* colData;
	ShadeButton *shade;

protected:
	QVBoxLayout* dialogLayout;
	QHBoxLayout* layout1;
	QHBoxLayout* okCancelLayout;
};

class EffectsDialog : public QDialog
{
	Q_OBJECT

public:
	EffectsDialog( QWidget* parent, QValueList<PageItem::imageEffect> eList, ScribusDoc* docc );
	~EffectsDialog() {};

	QLabel* textLabel2;
	QListBox* usedEffects;
	QPushButton* effectUp;
	QPushButton* effectDown;
	QPushButton* effectOptions;
	QPushButton* toEffects;
	QPushButton* fromEffects;
	QLabel* textLabel1;
	QListBox* availableEffects;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QValueList<PageItem::imageEffect> effectsList;
	QMap<QListBoxItem*, QString> effectValMap;
	ScribusDoc* doc;

public slots:
	virtual void saveValues();
	virtual void moveToEffects();
	virtual void moveFromEffects();
	virtual void moveEffectUp();
	virtual void moveEffectDown();
	virtual void editEffect();
	virtual void selectEffect(QListBoxItem* c);
	virtual void selectAvailEffect(QListBoxItem* c);

protected:
	QVBoxLayout* EffectsDialogLayout;
	QGridLayout* layout10;
	QVBoxLayout* layout8;
	QHBoxLayout* layout7;
	QVBoxLayout* layout1;
	QVBoxLayout* layout2;
	QHBoxLayout* layout9;
};

#endif // EFFECTSDIALOG_H
