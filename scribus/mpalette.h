#ifndef MPALETTE_H
#define MPALETTE_H

#include <qdialog.h>
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
#include "mspinbox.h"
#include "pageitem.h"
#include "page.h"
#include "scribusdoc.h"
#include "linkbutton.h"
#include "linecombo.h"
#include "cpalette.h"
#include "spalette.h"
#include "fontcombo.h"
#include "styleselect.h"
#include "alignselect.h"
#include "shadebutton.h"

class Autoforms;
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

class Mpalette : public QDialog
{
	Q_OBJECT

public:
	Mpalette(QWidget* parent, preV *Prefs);
	~Mpalette() {};
	void closeEvent(QCloseEvent *ce);
	void ToggleFlow();
	void updateCList();
	void updateCmsList();
	void ShowCMS();
	void fillLangCombo(QMap<QString,QString> Sprachen);
	QButtonGroup* buttonGroup5;
	QToolButton* SShape;
	QToolButton* SText;
	QToolButton* SImage;
	QToolButton* SLine;
	QToolButton* SGeom;
	QToolButton* SColor;
	QWidgetStack* TabStack;
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
	QGroupBox* NameGroup;
	NameWidget* NameEdit;
	QGroupBox* GeoGroup;
	QLabel* Text1;
	QLabel* Text2;
	QLabel* Text1a;
	QLabel* Text2a;
	MSpinBox* Width;
	MSpinBox* Xpos;
	MSpinBox* Ypos;
	MSpinBox* Height;
	QGroupBox* LayerGroup;
	QToolButton* Zup;
	QToolButton* ZDown;
	QToolButton* ZTop;
	QToolButton* ZBottom;
	QButtonGroup* RotationGroup;
	QRadioButton* TopLeft;
	QFrame* Line1;
	QRadioButton* TopRight;
	QFrame* Line2;
	QRadioButton* Center;
	QFrame* Line4;
	QRadioButton* BottomLeft;
	QFrame* Line5;
	QRadioButton* BottomRight;
	QLabel* Text3;
	MSpinBox* Rot;
	QLabel* Text3r;
	QLabel* LevelTxt;
	QToolButton* FlipH;
	QToolButton* FlipV;
	QToolButton* Locked;
	QToolButton* NoPrint;
	QToolButton* NoResize;
	QButtonGroup* ShapeGroup;
	QLabel* SRect;
	Autoforms* SCustom;
	QLabel* Text4;
	MSpinBox* RoundRect;
	QCheckBox* Textflow;
	QCheckBox* Textflow2;
	QToolButton* EditShape;
	QGroupBox* Distance;
	QGroupBox* Distance2;
	QLabel* TextLabel3;
	QLabel* TextLabel2;
	MSpinBox* Dist;
	MSpinBox* LineW;
	QLabel* Text14;
	QLabel* Text14a;
	LabelButton* Text14b;
	QLabel* Text15;
	QLabel* Text16;
	QSpinBox* DCol;
	MSpinBox* DGap;
	MSpinBox* DTop;
	MSpinBox* DBottom;
	MSpinBox* DLeft;
	QPushButton* TabsButton;
	QLabel* Text17;
	MSpinBox* DRight;
	QCheckBox* NormText2;
	FontCombo* Fonts;
	MSpinBox* Size;
	QToolButton* Revert;
	StyleSelect* SeStyle;
	AlignSelect* GroupAlign;
	QGroupBox* GroupBox3;
	MSpinBox* LineSp;
	MSpinBox* Extra;
	QLabel* Text5;
	QLabel* Text6;
	QGroupBox* GroupBox3a;
	Spalette *Spal;
	QLabel* Text20;
	QRadioButton* FreeScale;
	QLabel* Text13;
	MSpinBox* LYpos;
	QLabel* Text12;
	QLabel* Text11;
	QLabel* Text11a;
	MSpinBox* LXpos;
	LinkButton* Kette;
	LinkButton* Kette2;
	MSpinBox* ScaleY;
	MSpinBox* ScaleX;
	QFrame* Line5_2;
	QRadioButton* FrameScale;
	QFrame* Frame4;
	QCheckBox* Aspect;
	QLabel* TextCms1;
	QLabel* TextCms2;
	QComboBox* InputP;
	QComboBox* MonitorI;
	QLabel* LineModeT;
	QComboBox* LineMode;
	QLabel* Text9;
	MSpinBox* LSize;
	QLabel* TextLabel1;
	QComboBox* LJoinStyle;
	QLabel* Text10;
	QComboBox* LEndStyle;
	QLabel* Text8;
	LineCombo* LStyle;
	QListBox* StyledLine;
	QLabel* ScaleTxt;
	QSpinBox* ChScale;
	QLabel* StrokeIcon;
	QComboBox* TxStroke;
	QLabel* ShadeTxt2;
	QLabel* ShadeTxt1;
	ShadeButton *PM1;
	QLabel* FillIcon;
	QComboBox* TxFill;
	ShadeButton *PM2;
	QLabel* StilT;
	QLabel* SprachT;
	QComboBox* LangCombo;
	QGroupBox* GroupBoxCM;
	QGroupBox* TLines;
	QCheckBox* TopLine;
	QCheckBox* LeftLine;
	QCheckBox* RightLine;
	QCheckBox* BottomLine;
	PageItem *CurItem;
	bool HaveDoc;
	bool HaveItem;
	ScribusDoc *doc;
	double Umrech;
	bool LMode;
	double RoVal;

public slots:
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
	void setTScale(int e);
	void NewTScale();
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
	void NewLMode();
	void DoLower();
	void DoRaise();
	void DoFront();
	void DoBack();
	void NewRotMode(int m);
	void DoFlow();
	void DoFlow2();
	void MakeIrre(int f, int c, double *vals);
	void EditSh();
	void NewTDist();
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

protected slots:
	virtual void reject();

signals:
	void Schliessen();
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
	QGridLayout* buttonGroup5Layout;
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
	QHBoxLayout* layout22;
	QHBoxLayout* layout23;
	QHBoxLayout* layout24;
};

#endif
