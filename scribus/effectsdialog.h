/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EFFECTSDIALOG_H
#define EFFECTSDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include <q3valuelist.h>
#include <qmap.h>
#include <q3listbox.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <QLabel>
#include <Q3PopupMenu>
#include <QMenu>
#include <QWidgetAction>
#include <Q3VBoxLayout>

#include "scribusapi.h"
#include "pageitem.h"
class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QComboBox;
class ColorCombo;
class ScribusDoc;
class ShadeButton;
class Q3WidgetStack;
class QWidget;
class QSlider;
class Q3PopupMenu;
class QToolButton;
class ScrSpinBox;
class CurveWidget;

class SCRIBUS_API EffectListItem : public Q3ListBoxText
{
public:
    EffectListItem(Q3ListBox* parent, QString f);
    virtual ~EffectListItem() {};
    virtual const int width(const Q3ListBox *);
    virtual const int height(const Q3ListBox *);
protected:
    virtual void paint(QPainter *p);
};

class SCRIBUS_API EffectsDialog : public QDialog
{
	Q_OBJECT

public:
	EffectsDialog( QWidget* parent, PageItem* item, ScribusDoc* docc );
	~EffectsDialog() {};
	void selectEffectHelper(bool final = false);

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
	Q3WidgetStack* optionStack;
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
	ColorCombo* colDataq3;
	ShadeButton *shadeq3;
	CurveWidget *CurveQ3;
	QToolButton *CurveQ3Button;
	QMenu *CurveQ3Pop;
	QWidgetAction* CurveQ3Act;
	QLabel* textLabel4q;
	ColorCombo* colDataq4;
	ShadeButton *shadeq4;
	CurveWidget *CurveQ4;
	QToolButton *CurveQ4Button;
	QMenu *CurveQ4Pop;
	QWidgetAction* CurveQ4Act;

	QWidget* WStackPage_11;
	CurveWidget *Kdisplay;

	Q3ListBox* usedEffects;
	QPushButton* effectUp;
	QPushButton* effectDown;
	QPushButton* toEffects;
	QPushButton* fromEffects;
	Q3ListBox* availableEffects;
	QPushButton* okButton;
	QPushButton* cancelButton;
	ScImageEffectList effectsList;
	QMap<Q3ListBoxItem*, QString> effectValMap;
	Q3ListBoxItem* currentOptions;
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
	virtual void selectEffect(Q3ListBoxItem* c);
	virtual void selectAvailEffect(Q3ListBoxItem* c);
	virtual void selectAvailEffectDbl(Q3ListBoxItem* c);

protected:
	Q3HBoxLayout* EffectsDialogLayout;
	Q3VBoxLayout* WStackPageLayout;
	Q3VBoxLayout* WStackPage3Layout;
	Q3HBoxLayout* layout20;
	Q3VBoxLayout* WStackPage4Layout;
	Q3HBoxLayout* layout21;
	Q3VBoxLayout* WStackPage5Layout;
	Q3HBoxLayout* layout22;
	Q3HBoxLayout* layout23;
	Q3VBoxLayout* WStackPage6Layout;
	Q3HBoxLayout* layout24;
	Q3VBoxLayout* WStackPage7Layout;
	Q3GridLayout* WStackPage8Layout;
	Q3GridLayout* WStackPage9Layout;
	Q3GridLayout* WStackPage10Layout;
	Q3VBoxLayout* WStackPage11Layout;
	Q3HBoxLayout* layout26;
	Q3HBoxLayout* layout17;
	Q3HBoxLayout* layout19;
	Q3GridLayout* layout10;
	Q3VBoxLayout* layout8;
	Q3HBoxLayout* layout7;
	Q3VBoxLayout* layout1;
	Q3VBoxLayout* layout2;
	Q3VBoxLayout* layout16;
	Q3VBoxLayout* layout18;
	Q3HBoxLayout* layout9;
};

#endif // EFFECTSDIALOG_H
