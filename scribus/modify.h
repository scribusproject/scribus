/****************************************************************************
** Form interface generated from reading ui file 'Modify.ui'
**
** Created: Sat Jun 16 18:08:30 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef MODOBJ_H
#define MODOBJ_H

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qtoolbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcombobox.h>
#include <qpixmap.h>
#include <qlineedit.h>
#include "mspinbox.h"
#include "pageitem.h"
#include "page.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "linkbutton.h"
#include "linecombo.h"

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

class ModObj : public QDialog
{ 
    Q_OBJECT

public:
    ModObj( QWidget* parent, PageItem *b, CListe Farben, ScribusDoc *docu, ScribusView *vie, ProfilesL *InputProfiles);
    ~ModObj() {};

    QGroupBox* GroupSpecial;
    QGroupBox* GroupPos;
    QCheckBox* Printable;
    QCheckBox* FlippedV;
    QCheckBox* FlippedH;
    QCheckBox* Textflow;
    QCheckBox* Textflow2;
    QCheckBox* Visible;
    QPushButton* OK;
    QPushButton* Cancel;
    QPushButton* Apply;
    LinkButton* Kette;
    QLabel* TextLabel1_3_2;
    QLabel* TextLabel2_2;
    QLabel* TextLabel1_3_3;
    QLabel* TextLabel2_3;
    QLabel* TextLabel2_3b;
    QLabel* TextLabel1_2;
    QLabel* TextLabel1_2_2_2;
    QLabel* TextLabel1_2_2_2_2;
    QLabel* TextLabel1;
    QLabel* TextLabel1_2_2;
    QLabel* TextLabel2;
    QLabel* TextLabel1_3;
    QLabel* TextLabel3;
    QLabel* TextLabel3_2;
    QLabel* TextLabel3_2_2;
    QLabel* TextLabel3_2_2_2;
    QLabel* TextLabel3_2_2_2_2;
    QLabel* Text1;
    QLabel* Text2;
    QLabel* TextName;
		NameWidget* NameEdit;
    MSpinBox* Ypos;
    MSpinBox* Width;
    MSpinBox* Xpos;
    MSpinBox* Angle;
    MSpinBox* Height;
    QSpinBox* Radius;
    MSpinBox* Distance;
    MSpinBox* LXpos;
    MSpinBox* LYpos;
    MSpinBox* HScale;
    MSpinBox* VScale;
    QSpinBox* Shade;
    QSpinBox* Shade2;
    QSpinBox* LineW;
    QButtonGroup* Props;
    QRadioButton* NormText;
    QCheckBox* NormText2;
    QRadioButton* Bookmark;
    QRadioButton* Annotation;
    QComboBox* Background;
    QComboBox* Foreground;
    QComboBox* InputP;
    QComboBox* MonitorI;
    LineCombo* Linestyle;
    PageItem* CurMod;
    ScribusDoc* doc;
		ScribusView* view;
    int origWidth;
    int origHeight;
    bool EmbedP;
    QButtonGroup* ScaleGroup;
    QRadioButton* FreeScale;
    QRadioButton* FrameScale;
    QCheckBox* Aspect;

public slots:
		bool DoModify();
		void DoModifyExit();
		void NoModifyExit();
		void ChangeScaling();
		void ToggleFlow();
		void ToggleKette();
		void HChange();
		void VChange();
		void UnitChange();
		void EmProf(const QString& prn);
		bool NewName();

signals:
		void DocChanged();
		
protected:
    QVBoxLayout* ModObjLayout;
    QGridLayout* Layout5;
    QHBoxLayout* GroupSpecialLayout;
    QVBoxLayout* Layout2;
    QHBoxLayout* GroupPosLayout;
    QGridLayout* Layout1;
    QHBoxLayout* PropsLayout;
    QGridLayout* Layout4;
    QHBoxLayout* Layout3;
    QVBoxLayout* ScaleGroupLayout;
    QHBoxLayout* ScaleLayout2;
    QGridLayout* ScaleLayout1;
    QHBoxLayout* ScaleLayout3;
};

#endif // MODOBJ_H
