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

class Autoforms;
extern bool CMSavail;

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

class Mpalette : public QDialog  {
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
    QWidget* page;
    QWidget* page_2;
    QWidget* page_2a;
    QWidget* page_2b;
    QWidget* page_3;
    QWidget* page_4;
    QWidget* page_5;
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
    QToolButton* FlipH;
    QToolButton* FlipV;
    QToolButton* Locked;
    QToolButton* NoPrint;
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
    QLabel* Text14b;
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
    QToolButton* Underline;
    QButtonGroup* ButtonGroup1;
    QToolButton* Subs;
    QToolButton* Supers;
    QToolButton* Kapital;
    QToolButton* Strike;
    QToolButton* Outlined;
    QButtonGroup* GroupAlign;
    QToolButton* TextL;
    QToolButton* TextR;
    QToolButton* TextC;
    QToolButton* TextB;
    QToolButton* TextF;
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
    MSpinBox* LXpos;
    LinkButton* Kette;
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
		QToolButton *PM1;
		QPopupMenu *TxStrokeSh;
    QLabel* FillIcon;
    QComboBox* TxFill;
		QToolButton *PM2;
		QPopupMenu *TxFillSh;
    QLabel* SprachT;
    QComboBox* LangCombo;
    QGroupBox* GroupBoxCM;
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
    void setXY(double x, double y);
    void setBH(double x, double y);
		void setR(double r);
		void setRR(double r);
		void setCols(int r, double g);
		void setLsp(double r);
		void setSize(int s);
		void setExtra(double e);
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
		void setActShade(int id);
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
		void NewAli();
		void setTypeStyle();
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
		void handlePathLine();
		void handlePathDist();
		void handlePathOffs();
		void ChProf(const QString& prn);
		void ChIntent();
		void NewName();
		void NewLanguage();
	
signals:
	void Schliessen();
	void DocChanged();
	void NewAbStyle(int);
	void BackHome();
	void Stellung(int);
	void EditCL();
	void EditLSt();
	void NewTF(QString);

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
    QHBoxLayout* layout60;
    QVBoxLayout* Layout44;
    QVBoxLayout* Layout44a;
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
    QHBoxLayout* ButtonGroup1Layout;
    QGridLayout* GroupAlignLayout;
    QGridLayout* GroupBox3Layout;
    QVBoxLayout* GroupBox3aLayout;
    QVBoxLayout* GroupBoxCMLayout;
    QHBoxLayout* layout22;
    QHBoxLayout* layout23;
    QHBoxLayout* layout24;
    QHBoxLayout* layoutLang;
};

#endif
