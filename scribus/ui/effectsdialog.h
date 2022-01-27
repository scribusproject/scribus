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
#include <QElapsedTimer>

#include "scribusapi.h"
#include "scimage.h"

class QComboBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QMenu;
class QPushButton;
class QSlider;
class QSpacerItem;
class QStackedWidget;
class QToolButton;
class QVBoxLayout;
class QWidget;
class QWidgetAction;

class ColorCombo;
class CurveWidget;
class PageItem;
class ScribusDoc;
class ScrSpinBox;
class ShadeButton;

class SCRIBUS_API EffectsDialog : public QDialog
{
	Q_OBJECT

public:
	EffectsDialog( QWidget* parent, PageItem* item, ScribusDoc* docc );
	~EffectsDialog() {};

	void selectEffectHelper(bool finalValues = false);
	void setItemSelectable(QListWidget* widget, int itemNr, bool enable);

	QLabel* pixmapLabel1 { nullptr };
	QLabel* textLabel1 { nullptr };
	QLabel* textLabel2 { nullptr };
	QLabel* textLabel3 { nullptr };
	QLabel* textLabel4 { nullptr };
	QLabel* textLabel5 { nullptr };
	QLabel* textLabel6 { nullptr };
	QLabel* textLabel7 { nullptr };
	QLabel* textLabel8 { nullptr };
	QLabel* textLabel9 { nullptr };
	QLabel* textLabel10 { nullptr };
	QLabel* textLabel11 { nullptr };
	QLabel* textLabel12 { nullptr };
	QLabel* textLabel14 { nullptr };
	QLabel* textLabel15 { nullptr };
	QStackedWidget* optionStack { nullptr };
	QWidget* WStackPage { nullptr };
	QWidget* WStackPage_2 { nullptr };
	ColorCombo* colData { nullptr };
	ShadeButton *shade { nullptr };
	QWidget* WStackPage_3 { nullptr };
	QSlider* brightnessSlider { nullptr };
	QWidget* WStackPage_4 { nullptr };
	QSlider* contrastSlider { nullptr };
	QWidget* WStackPage_5 { nullptr };
	ScrSpinBox* shRadius { nullptr };
	ScrSpinBox* shValue { nullptr };
	QWidget* WStackPage_6 { nullptr };
	ScrSpinBox* blRadius { nullptr };
	QWidget* WStackPage_7 { nullptr };
	QSlider* solarizeSlider { nullptr };
	QWidget* WStackPage_8 { nullptr };
	QLabel* textLabel1d { nullptr };
	ColorCombo* colData1 { nullptr };
	ShadeButton *shade1 { nullptr };
	CurveWidget *CurveD1 { nullptr };
	QToolButton *CurveD1Button { nullptr };
	QMenu *CurveD1Pop { nullptr };
	QWidgetAction* CurveD1Act { nullptr };
	QLabel* textLabel2d { nullptr };
	ColorCombo* colData2 { nullptr };
	ShadeButton *shade2 { nullptr };
	CurveWidget *CurveD2 { nullptr };
	QToolButton *CurveD2Button { nullptr };
	QMenu *CurveD2Pop { nullptr };
	QWidgetAction* CurveD2Act { nullptr };

	QWidget* WStackPage_9 { nullptr };
	QLabel* textLabel1t { nullptr };
	ColorCombo* colDatat1 { nullptr };
	ShadeButton *shadet1 { nullptr };
	CurveWidget *CurveT1 { nullptr };
	QToolButton *CurveT1Button { nullptr };
	QMenu *CurveT1Pop { nullptr };
	QWidgetAction* CurveT1Act { nullptr };
	QLabel* textLabel2t { nullptr };
	ColorCombo* colDatat2 { nullptr };
	ShadeButton *shadet2 { nullptr };
	CurveWidget *CurveT2 { nullptr };
	QToolButton *CurveT2Button { nullptr };
	QMenu *CurveT2Pop { nullptr };
	QWidgetAction* CurveT2Act { nullptr };
	QLabel* textLabel3t { nullptr };
	ColorCombo* colDatat3 { nullptr };
	ShadeButton *shadet3 { nullptr };
	CurveWidget *CurveT3 { nullptr };
	QToolButton *CurveT3Button { nullptr };
	QMenu *CurveT3Pop { nullptr };
	QWidgetAction* CurveT3Act { nullptr };

	QWidget* WStackPage_10 { nullptr };
	QLabel* textLabel1q { nullptr };
	ColorCombo* colDataq1 { nullptr };
	ShadeButton *shadeq1 { nullptr };
	CurveWidget *CurveQ1 { nullptr };
	QToolButton *CurveQ1Button { nullptr };
	QMenu *CurveQ1Pop { nullptr };
	QWidgetAction* CurveQ1Act { nullptr };
	QLabel* textLabel2q { nullptr };
	ColorCombo* colDataq2 { nullptr };
	ShadeButton *shadeq2 { nullptr };
	CurveWidget *CurveQ2 { nullptr };
	QToolButton *CurveQ2Button { nullptr };
	QMenu *CurveQ2Pop { nullptr };
	QWidgetAction* CurveQ2Act { nullptr };
	QLabel* textLabel3q { nullptr };
	ColorCombo* colDataqc3 { nullptr };
	ShadeButton *shadeqc3 { nullptr };
	CurveWidget *CurveQc3 { nullptr };
	QToolButton *CurveQc3Button { nullptr };
	QMenu *CurveQc3Pop { nullptr };
	QWidgetAction* CurveQc3Act { nullptr };
	QLabel* textLabel4q { nullptr };
	ColorCombo* colDataq4 { nullptr };
	ShadeButton *shadeq4 { nullptr };
	CurveWidget *CurveQ4 { nullptr };
	QToolButton *CurveQ4Button { nullptr };
	QMenu *CurveQ4Pop { nullptr };
	QWidgetAction* CurveQ4Act { nullptr };

	QWidget* WStackPage_11 { nullptr };
	CurveWidget *Kdisplay { nullptr };

	QListWidget* usedEffects { nullptr };
	QPushButton* effectUp { nullptr };
	QPushButton* effectDown { nullptr };
	QPushButton* toEffects { nullptr };
	QPushButton* fromEffects { nullptr };
	QListWidget* availableEffects { nullptr };
	QPushButton* okButton { nullptr };
	QPushButton* cancelButton { nullptr };
	QListWidgetItem* currentOptions { nullptr };

	QGroupBox *groupBox { nullptr };
	QSpacerItem* spacer { nullptr };
	QSpacerItem* spacer3 { nullptr };

	ScImageEffectList effectsList;

public slots:
	virtual void leaveOK();
	virtual void updateSolarize(int val);
	virtual void updateContrast(int val);
	virtual void updateBright(int val);
	virtual void createPreview();
	virtual void saveValues(bool finalValues);
	virtual void moveToEffects();
	virtual void moveFromEffects();
	virtual void moveEffectUp();
	virtual void moveEffectDown();
	virtual void selectEffect(QListWidgetItem* c);
	virtual void selectAvailEffect(QListWidgetItem* c);
	virtual void selectAvailEffectDbl(QListWidgetItem* c);

protected:
	ScribusDoc* m_doc {nullptr};
	PageItem* m_item {nullptr};

	ScImage m_image;
	double  m_imageScale { 1.0 };

	QElapsedTimer m_time;
	QMap<QListWidgetItem*, QString> m_effectValMap;

	QGridLayout* layoutGrid { nullptr };
	QVBoxLayout* EffectsDialogLayout { nullptr };
	QVBoxLayout* WStackPageLayout { nullptr };
	QVBoxLayout* WStackPage3Layout { nullptr };
	QHBoxLayout* layout20 { nullptr };
	QVBoxLayout* WStackPage4Layout { nullptr };
	QHBoxLayout* layout21 { nullptr };
	QVBoxLayout* WStackPage5Layout { nullptr };
	QHBoxLayout* layout22 { nullptr };
	QHBoxLayout* layout23 { nullptr };
	QVBoxLayout* WStackPage6Layout { nullptr };
	QHBoxLayout* layout24 { nullptr };
	QVBoxLayout* WStackPage7Layout { nullptr };
	QGridLayout* WStackPage8Layout { nullptr };
	QGridLayout* WStackPage9Layout { nullptr };
	QGridLayout* WStackPage10Layout { nullptr };
	QVBoxLayout* WStackPage11Layout { nullptr };
	QHBoxLayout* layout26 { nullptr };
	QHBoxLayout* layout17 { nullptr };
	QHBoxLayout* layout19 { nullptr };
	QVBoxLayout* layout8 { nullptr };
	QHBoxLayout* layout7 { nullptr };
	QHBoxLayout* layoutDialogButtonBox { nullptr };
};

#endif // EFFECTSDIALOG_H
