/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EFFECTSDIALOG_H
#define EFFECTSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QTime>

#include "scribusapi.h"
#include "scimage.h"

class PageItem;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;
class QComboBox;
class ColorCombo;
class ScribusDoc;
class ShadeButton;
class QStackedWidget;
class QWidget;
class QSlider;
class QMenu;
class QWidgetAction;
class QToolButton;
class QListWidgetItem;
class QListWidget;
class ScrSpinBox;
class CurveWidget;

class SCRIBUS_API EffectsDialog : public QDialog
{
	Q_OBJECT

public:
	EffectsDialog( QWidget* parent, PageItem* item, ScribusDoc* docc );
	~EffectsDialog() {};
	void selectEffectHelper(bool final = false);
	void setItemSelectable(QListWidget* widget, int itemNr, bool enable);

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
	QLabel* textLabel14;
	QLabel* textLabel15;
	QStackedWidget* optionStack;
	QWidget* WStackPage;
	QWidget* WStackPage_2;
	ColorCombo* colData;
	ShadeButton *shade;
	QWidget* WStackPage_3;
	QSlider* brightnessSlider;
	QWidget* WStackPage_4;
	QSlider* contrastSlider;
	QWidget* WStackPage_5;
	ScrSpinBox* shRadius;
	ScrSpinBox* shValue;
	QWidget* WStackPage_6;
	ScrSpinBox* blRadius;
	QWidget* WStackPage_7;
	QSlider* solarizeSlider;
	QWidget* WStackPage_8;
	QLabel* textLabel1d;
	ColorCombo* colData1;
	ShadeButton *shade1;
	CurveWidget *CurveD1;
	QToolButton *CurveD1Button;
	QMenu *CurveD1Pop;
	QWidgetAction* CurveD1Act;
	QLabel* textLabel2d;
	ColorCombo* colData2;
	ShadeButton *shade2;
	CurveWidget *CurveD2;
	QToolButton *CurveD2Button;
	QMenu *CurveD2Pop;
	QWidgetAction* CurveD2Act;

	QWidget* WStackPage_9;
	QLabel* textLabel1t;
	ColorCombo* colDatat1;
	ShadeButton *shadet1;
	CurveWidget *CurveT1;
	QToolButton *CurveT1Button;
	QMenu *CurveT1Pop;
	QWidgetAction* CurveT1Act;
	QLabel* textLabel2t;
	ColorCombo* colDatat2;
	ShadeButton *shadet2;
	CurveWidget *CurveT2;
	QToolButton *CurveT2Button;
	QMenu *CurveT2Pop;
	QWidgetAction* CurveT2Act;
	QLabel* textLabel3t;
	ColorCombo* colDatat3;
	ShadeButton *shadet3;
	CurveWidget *CurveT3;
	QToolButton *CurveT3Button;
	QMenu *CurveT3Pop;
	QWidgetAction* CurveT3Act;

	QWidget* WStackPage_10;
	QLabel* textLabel1q;
	ColorCombo* colDataq1;
	ShadeButton *shadeq1;
	CurveWidget *CurveQ1;
	QToolButton *CurveQ1Button;
	QMenu *CurveQ1Pop;
	QWidgetAction* CurveQ1Act;
	QLabel* textLabel2q;
	ColorCombo* colDataq2;
	ShadeButton *shadeq2;
	CurveWidget *CurveQ2;
	QToolButton *CurveQ2Button;
	QMenu *CurveQ2Pop;
	QWidgetAction* CurveQ2Act;
	QLabel* textLabel3q;
	ColorCombo* colDataqc3;
	ShadeButton *shadeqc3;
	CurveWidget *CurveQc3;
	QToolButton *CurveQc3Button;
	QMenu *CurveQc3Pop;
	QWidgetAction* CurveQc3Act;
	QLabel* textLabel4q;
	ColorCombo* colDataq4;
	ShadeButton *shadeq4;
	CurveWidget *CurveQ4;
	QToolButton *CurveQ4Button;
	QMenu *CurveQ4Pop;
	QWidgetAction* CurveQ4Act;

	QWidget* WStackPage_11;
	CurveWidget *Kdisplay;

	QListWidget* usedEffects;
	QPushButton* effectUp;
	QPushButton* effectDown;
	QPushButton* toEffects;
	QPushButton* fromEffects;
	QListWidget* availableEffects;
	QPushButton* okButton;
	QPushButton* cancelButton;
	ScImageEffectList effectsList;
	QMap<QListWidgetItem*, QString> effectValMap;
	QListWidgetItem* currentOptions;
	ScribusDoc* doc;
	PageItem* currItem;
	ScImage image;
	QTime tim;
	double imageScale;

public slots:
	virtual void leaveOK();
	virtual void updateSolarize(int val);
	virtual void updateContrast(int val);
	virtual void updateBright(int val);
	virtual void createPreview();
	virtual void saveValues(bool final);
	virtual void moveToEffects();
	virtual void moveFromEffects();
	virtual void moveEffectUp();
	virtual void moveEffectDown();
	virtual void selectEffect(QListWidgetItem* c);
	virtual void selectAvailEffect(QListWidgetItem* c);
	virtual void selectAvailEffectDbl(QListWidgetItem* c);

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
	QVBoxLayout* WStackPage7Layout;
	QGridLayout* WStackPage8Layout;
	QGridLayout* WStackPage9Layout;
	QGridLayout* WStackPage10Layout;
	QVBoxLayout* WStackPage11Layout;
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
