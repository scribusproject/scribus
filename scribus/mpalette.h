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
#include "mspinbox.h"
#include "pageitem.h"
#include "page.h"
#include "scribusdoc.h"
#include "linkbutton.h"
#include "linecombo.h"
#include "cpalette.h"
#include "spalette.h"


class Mpalette : public QDialog  {
Q_OBJECT
public:
    Mpalette(QWidget* parent, QPopupMenu* FontMenu);
    ~Mpalette() {};
    void closeEvent(QCloseEvent *ce);

    QButtonGroup* buttonGroup5;
    QToolButton* SShape;
    QToolButton* SText;
    QToolButton* SImage;
    QToolButton* SLine;
    QToolButton* SGeom;
    QToolButton* SColor;
    QWidgetStack* TabStack;
    QWidget* page;
    QWidget* page_2;
    QWidget* page_3;
    QWidget* page_4;
    QWidget* page_5;
    Cpalette *Cpal;
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
    QToolButton* FlipH;
    QToolButton* FlipV;
    QButtonGroup* ShapeGroup;
    QToolButton* SRect;
    QToolButton* SOval;
    QToolButton* SCustom;
    QLabel* Text4;
    QSpinBox* RoundRect;
    QCheckBox* Textflow;
    QToolButton* EditShape;
    QGroupBox* Distance;
    QLabel* Text14;
    QLabel* Text15;
    QLabel* Text16;
    MSpinBox* DTop;
    MSpinBox* DBottom;
    MSpinBox* DLeft;
    QLabel* Text17;
    MSpinBox* DRight;
    QToolButton* Fonts;
    QSpinBox* Size;
    QToolButton* Revert;
    QToolButton* Underline;
    QButtonGroup* ButtonGroup1;
    QToolButton* Subs;
    QToolButton* Supers;
    QToolButton* Kapital;
    QToolButton* Strike;
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
    QComboBox* LineMode;
    QLabel* Text9;
    MSpinBox* LSize;
    QLabel* TextLabel1;
    QComboBox* LJoinStyle;
    QLabel* Text10;
    QComboBox* LEndStyle;
    QLabel* Text8;
    LineCombo* LStyle;
    PageItem *CurItem;
    bool HaveDoc;
    bool HaveItem;
    ScribusDoc *doc;
    float Umrech;
    bool LMode;

public slots:
		void SetDoc(ScribusDoc *d);
		void UnsetDoc();
		void NewSel(int nr);
		void SetCurItem(PageItem *i);
		void UnitChange();
    void setXY(float x, float y);
    void setBH(float x, float y);
		void setR(float r);
		void setRR(float r);
		void setRM(int r);
		void setLsp(float r);
		void setSize(int s);
		void setExtra(float e);
		void ChangeScaling();
		void setLvalue(float scx, float scy, float x, float y);
		void setSvalue(float s);
		void setLIvalue(PenStyle p, PenCapStyle pc, PenJoinStyle pj);
		void setStil(int s);
		void setAli(int e);
		
private slots:
		void SelTab(int t);
		void NewX();
		void NewY();
		void NewW();
		void NewH();
		void NewR();
		void NewRR();
		void NewLsp();
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
		void MakeRect();
		void MakeOval();
		void MakeIrre();
		void EditSh();
		void NewTDist();
		void DoRevert();
	
signals:
	void Schliessen();
	void DocChanged();
	void NewAbStyle(int);
	void BackHome();
	void Stellung(int);
	void EditCL();

protected:
    QVBoxLayout* MpalLayout;
    QGridLayout* buttonGroup5Layout;
    QVBoxLayout* pageLayout;
    QVBoxLayout* pageLayout_2;
    QVBoxLayout* pageLayout_3;
    QVBoxLayout* pageLayout_4;
    QVBoxLayout* pageLayout_5;
    QHBoxLayout* layout60;
    QHBoxLayout* layout60a;
    QVBoxLayout* Layout44;
    QGridLayout* Layout12;
    QVBoxLayout* Layout15_2;
    QHBoxLayout* Layout13;
    QHBoxLayout* layout47;
    QVBoxLayout* layout46;
    QGridLayout* layout41;
    QHBoxLayout* Layout1;
    QGridLayout* layout43;
    QVBoxLayout* Layout24;
    QHBoxLayout* Layout18;
    QHBoxLayout* Layout13_2;
    QGridLayout* Layout12_2;
    QGridLayout* GeoGroupLayout;
    QGridLayout* LayerGroupLayout;
    QHBoxLayout* RotationGroupLayout;
    QHBoxLayout* ShapeGroupLayout;
    QGridLayout* DistanceLayout;
    QHBoxLayout* ButtonGroup1Layout;
    QGridLayout* GroupAlignLayout;
    QGridLayout* GroupBox3Layout;
    QVBoxLayout* GroupBox3aLayout;
};

#endif
