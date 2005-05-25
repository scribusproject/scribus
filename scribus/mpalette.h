#ifndef MPALETTE_H
#define MPALETTE_H

#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qtoolbutton.h>
#include <qwidget.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qlistbox.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <qtoolbox.h>
#include "scrpalettebase.h"
#include "mspinbox.h"
#include "pageitem.h"
#include "page.h"
class ScribusDoc;
#include "linkbutton.h"
#include "linecombo.h"
#include "spalette.h"
#include "fontcombo.h"
class StyleSelect;
#include "alignselect.h"
#include "shadebutton.h"
#include "cpalette.h"
class Autoforms;
class ArrowChooser;
class UserActionSniffer;
extern bool CMSavail;

class LabelButton : public QLabel
{
	Q_OBJECT

public:
	LabelButton(QWidget* parent, QString text1, QString text2);
	~LabelButton() {};
	void setTexts(QString text1, QString text2);
	bool getState();

private:
	bool state;
	QString TextA;
	QString TextB;

signals:
	void clicked();

protected:
	virtual void enterEvent(QEvent*);
	virtual void leaveEvent(QEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent *);
};

class NameWidget : public QLineEdit
{
	Q_OBJECT

public:
	NameWidget(QWidget* parent);
	~NameWidget() {};

signals:
	void Leaved();

protected:
	virtual void focusOutEvent(QFocusEvent *);
};

class Mpalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	Mpalette(QWidget* parent, ApplicationPrefs *Prefs);
	~Mpalette() {};
	void updateCList();
	void updateCmsList();
	void ShowCMS();
	void fillLangCombo(QMap<QString,QString> langMap);
	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
                         // commmited

	Autoforms* SCustom;
	LabelButton* colgapLabel;
	FontCombo* Fonts;
	StyleSelect* SeStyle;
	AlignSelect* GroupAlign;
	Spalette *Spal;
	LinkButton* Kette;
	LinkButton* Kette2;
	LineCombo* LStyle;
	ArrowChooser* startArrow;
	ArrowChooser* endArrow;
	ShadeButton *PM1;
	ShadeButton *PM2;
	PageItem *CurItem;
	bool HaveDoc;
	bool HaveItem;
	ScribusDoc *doc;
	double Umrech;
	bool LMode;
	double RoVal;

	QButtonGroup* RotationGroup;
	QButtonGroup* ShapeGroup;

	QPushButton* TabsButton;

	QToolButton* Zup;
	QToolButton* ZDown;
	QToolButton* ZTop;
	QToolButton* ZBottom;

	QToolBox* TabStack;
	QWidgetStack* TabStack2;
	QWidgetStack* TabStack3;

	QWidget* page;
	QWidget* page_2;
	QWidget* page_2a;
	QWidget* page_2b;
	QWidget* page_3;
	QWidget* page_4;
	QWidget* page_5;
	QWidget* page_5a;
	QWidget* page_5b;

	Cpalette *Cpal;
	NameWidget* NameEdit;

	QGroupBox* NameGroup;
	QGroupBox* GeoGroup;
	QGroupBox* LayerGroup;
	QGroupBox* Distance;
	QGroupBox* Distance2;
	QGroupBox* GroupBoxCM;
	QGroupBox* TLines;
	QGroupBox* GroupBox3;
	QGroupBox* GroupBox3a;

	QButtonGroup* textFlowsAroundFrame;
	QCheckBox* textFlowUsesBoundingBox;
	QCheckBox* Textflow3;
	QCheckBox* Aspect;
	QCheckBox* showcurveCheckBox;
	QCheckBox* TopLine;
	QCheckBox* LeftLine;
	QCheckBox* RightLine;
	QCheckBox* BottomLine;

	QComboBox* langCombo;
	QComboBox* InputP;
	QComboBox* MonitorI;
	QComboBox* LineMode;
	QComboBox* TxStroke;
	QComboBox* LJoinStyle;
	QComboBox* LEndStyle;
	QComboBox* TxFill;

	QLabel* xposLabel;
	QLabel* widthLabel;
	QLabel* yposLabel;
	QLabel* heightLabel;
	QLabel* rotationLabel;
	QLabel* basepointLabel;
	QLabel* LevelTxt;
	QLabel* SRect;
	QLabel* rndcornersLabel;
	QLabel* startoffsetLabel;
	QLabel* distfromcurveLabel;
	QLabel* topLabel;
	QLabel* columnsLabel;
	QLabel* bottomLabel;
	QLabel* leftLabel;
	QLabel* rightLabel;
	QLabel* trackingLabel;
	QLabel* linespacingLabel;
	QLabel* fontsizeLabel;
	QLabel* yscaleLabel;
	QLabel* xscaleLabel;
	QLabel* xposImgLabel;
	QLabel* yposImgLabel;
	QLabel* linewidthLabel;
	QLabel* endingsLabel;
	QLabel* linetypeLabel;
	QLabel* StrokeIcon;
	QLabel* ShadeTxt2;
	QLabel* ShadeTxt1;
	QLabel* FillIcon;
	QLabel* styleLabel;
	QLabel* langLabel;
	QLabel* LineModeT;
	QLabel* TextCms1;
	QLabel* TextCms2;
	QLabel* edgesLabel;
	QLabel* ChBaseTxt;
	QLabel* ScaleTxt;
	QLabel* ScaleTxtV;
	QLabel* startArrowText;
	QLabel* endArrowText;

	QListBox* StyledLine;

	MSpinBox* Width;
	MSpinBox* Xpos;
	MSpinBox* Ypos;
	MSpinBox* Height;
	MSpinBox* Rot;
	MSpinBox* RoundRect;
	MSpinBox* dGap;
	MSpinBox* DTop;
	MSpinBox* DBottom;
	MSpinBox* DLeft;
	MSpinBox* DRight;
	MSpinBox* Dist;
	MSpinBox* LineW;
	MSpinBox* Size;
	MSpinBox* LineSp;
	MSpinBox* Extra;
	MSpinBox* LYpos;
	MSpinBox* LXpos;
	MSpinBox* ScaleY;
	MSpinBox* ScaleX;
	MSpinBox* LSize;

	MSpinBox* ChScale;
	MSpinBox* ChScaleV;
	MSpinBox* ChBase;
	QSpinBox* DCol;

	QRadioButton* TopLeft;
	QRadioButton* TopRight;
	QRadioButton* Center;
	QRadioButton* BottomLeft;
	QRadioButton* BottomRight;
	QRadioButton* FreeScale;
	QRadioButton* FrameScale;

	QFrame* Line1;
	QFrame* Line2;
	QFrame* Line4;
	QFrame* Line5;
	QFrame* Line5_2;
	QFrame* Frame4;

	QToolButton* FlipH;
	QToolButton* FlipV;
	QToolButton* Locked;
	QToolButton* NoPrint;
	QToolButton* NoResize;
	QToolButton* Revert;
	QToolButton* EditShape;


public slots:
	void languageChange();
	void SetDoc(ScribusDoc *d);
	void UnsetDoc();
	void NewSel(int nr);
	void SetCurItem(PageItem *i);
	void UnitChange();
	void setLevel(uint l);
	void setXY(double x, double y);
	void setBH(double x, double y);
	void setR(double r);
	void setRR(double r);
	void setCols(int r, double g);
	void setLsp(double r);
	void setSize(int s);
	void setExtra(double e);
	void setDvals(double left, double top, double bottom, double right);
	void ChangeScaling();
	void setLvalue(double scx, double scy, double x, double y);
	void setSvalue(double s);
	void setLIvalue(PenStyle p, PenCapStyle pc, PenJoinStyle pj);
	void setStil(int s);
	void setAli(int e);
	void setShadowOffs(int x, int y);
	void setOutlineW(int x);
	void newOutlineW();
	void setTScale(int e);
	void NewTScale();
	void NewTScaleV();
	void NewTBase();
	void setTScaleV(int e);
	void setTBase(int e);
	void SetLineFormats(ScribusDoc *dd);
	void SetSTline(QListBoxItem *c);
	void NewTFont(int);
	void newTxtFill();
	void newTxtStroke();
	void setActShade();
	void setActFarben(QString p, QString b, int shp, int shb);
	void ManageTabs();

private slots:

	void SelTab(int t);
	void NewX();
	void NewY();
	void NewW();
	void NewH();
	void NewR();
	void NewRR();
	void NewLsp();
	void HandleGapSwitch();
	void NewCols();
	void NewGap();
	void NewSize();
	void NewExtra();
	void DoFlipH();
	void DoFlipV();
	void ToggleKette();
	void HChange();
	void VChange();
	void NewLocalXY();
	void NewLocalSC();
	void NewLS();
	void NewLSty();
	void NewLJoin();
	void NewLEnd();
	void NewAli(int a);
	void setTypeStyle(int s);
	void newShadowOffs();
	void NewLMode();
	void DoLower();
	void DoRaise();
	void DoFront();
	void DoBack();
	void NewRotMode(int m);
	void DoFlow(int id);
	void MakeIrre(int f, int c, double *vals);
	void EditSh();
	void NewTDist();
	void NewSpGradient(double x1, double y1, double x2, double y2);
	void toggleGradientEdit();
	void DoRevert();
	void handleLock();
	void handlePrint();
	void handleResize();
	void handlePathLine();
	void handlePathDist();
	void handlePathOffs();
	void ChProf(const QString& prn);
	void ChIntent();
	void NewName();
	void NewLanguage();
	void HandleTLines();
	void setStartArrow(int id);
	void setEndArrow(int id);

protected slots:
	//virtual void reject();
	void mspinboxStartUserAction();
	void mspinboxFinishUserAction();

signals:
	void ToggleAllPalettes();
	void DocChanged();
	void NewAbStyle(int);
	void BackHome();
	void Stellung(int);
	void EditCL();
	void EditLSt();
	void NewTF(QString);
	void UpdtGui(int);

protected:
	QVBoxLayout* MpalLayout;
	QVBoxLayout* pageLayout;
	QVBoxLayout* pageLayout_2;
	QVBoxLayout* pageLayout_2a;
	QVBoxLayout* pageLayout_2b;
	QVBoxLayout* pageLayout_3;
	QVBoxLayout* pageLayout_4;
	QVBoxLayout* pageLayout_5;
	QVBoxLayout* pageLayout_5a;
	QVBoxLayout* pageLayout_5b;
	QVBoxLayout* TLineLayout;
	QHBoxLayout* layout60;
	QGridLayout* Layout44;
	QHBoxLayout* Layout13;
	QGridLayout* Layout12;
	QHBoxLayout* layout47;
	QVBoxLayout* layout46;
	QGridLayout* layout41;
	QHBoxLayout* Layout1;
	QGridLayout* layout43;
	QVBoxLayout* Layout24;
	QHBoxLayout* Layout18;
	QHBoxLayout* Layout13_2;
	QGridLayout* Layout12_2;
	QHBoxLayout* NameGroupLayout;
	QGridLayout* GeoGroupLayout;
	QGridLayout* LayerGroupLayout;
	QHBoxLayout* RotationGroupLayout;
	QHBoxLayout* ShapeGroupLayout;
	QGridLayout* DistanceLayout;
	QGridLayout* DistanceLayout2;
	QGridLayout* GroupBox3Layout;
	QGridLayout* GroupBox3aLayout;
	QVBoxLayout* GroupBoxCMLayout;
	QVBoxLayout* textFlowsAroundFrameLayout;
	QHBoxLayout* layout23;
	QHBoxLayout* layout24;
	bool _userActionOn;
	UserActionSniffer *userActionSniffer;
	void installSniffer(MSpinBox *spinBox);
	
	int idXYZItem;
	int idShapeItem;
	int idTextItem;
	int idImageItem;
	int idLineItem;
	int idColorsItem;
};

class UserActionSniffer : public QObject
{
	Q_OBJECT

public:
	UserActionSniffer();
protected:
	bool eventFilter( QObject *o, QEvent *e );
signals:
	void actionStart();
	void actionEnd();
};

#endif
