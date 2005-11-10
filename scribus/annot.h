/****************************************************************************
** Form interface generated from reading ui file 'Annot.ui'
**
** Created: Tue Feb 19 22:31:41 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef ANNOT_H
#define ANNOT_H

#include <qdialog.h>
#include <qcombobox.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3widgetstack.h>
#include <q3frame.h>
#include <qlineedit.h>
#include <q3textedit.h>
#include <qtabwidget.h>
#include <qcheckbox.h>
#include <q3listbox.h>
#include <qradiobutton.h>
#include <q3buttongroup.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>


#include "scribusstructs.h"
#include "scribusapi.h"
#include "sccolor.h"

class Navigator;
class ScribusView;
class PrefsContext;

class SCRIBUS_API Annot : public QDialog
{ 
    Q_OBJECT

public:
    Annot(QWidget* parent, PageItem *it, int Seite, int b, int h, ColorList Farben, ScribusView* vie);
    ~Annot();
	void DecodeNum();
	void DecodeVali();
	void DecodeCalc();
	Q3WidgetStack* Fram;
	Q3WidgetStack* Fram2;
	Q3WidgetStack* FramOp;
	Q3WidgetStack* FoFram;
    QLabel* TextLabel1;
    QComboBox* ComboBox1;
    QLabel* TextVa;
    Q3Frame* Frame9;
    QComboBox* ActionCombo;
    Q3GroupBox* GroupBox2;
    Q3Frame* Frame3;
    Q3Frame* Frame4;
    Q3GroupBox* Frame4a;
    QLabel* TextLabel4a;
	QLineEdit *Name;
    QLabel* TextLabel4b;
	QLineEdit *Tip;
    QLabel* AcText1;
    QLabel* AcText2;
	QComboBox* SelAction;
    Q3TextEdit* EditJava;
    Q3GroupBox* GroupBox11;
    QLabel* TextLabel31;
    QLabel* TextLabel41;
    QLabel* TextLabel51;
	QCheckBox* LExtern;
	QLineEdit* Destfile;
	QPushButton* ChFile;
    Navigator* Pg1;
    QSpinBox* SpinBox11;
    QSpinBox* SpinBox21;
    QSpinBox* SpinBox31;
    Q3GroupBox* GroupBox10;
    QLabel* TextLabel20;
    QLabel* TextLabel30;
    QTabWidget* TabWidget2;
    QWidget* tab;
    Q3GroupBox* GroupBox40;
    QComboBox* Schrift;
    QLabel* TextLabel60;
    QLabel* CheckBox30;
    QLabel* CheckBox40;
    QLineEdit* DownT;
    QLineEdit* TextO;
    Q3GroupBox* GroupBox30;
    QCheckBox* ReadOnly;
	QCheckBox* Required;
	QCheckBox* NoExport;
    QLabel* TextLabel8_2;
    QComboBox* ComboBox7_2;
    QLabel* TextLabel90;
    QComboBox* Visib;
    Q3GroupBox* GroupBox20;
    QComboBox* BorderC;
    QLabel* TextLabel40a;
    QComboBox* BorderW;
    QLabel* TextLabel40;
    QLabel* TextLabel50;
    QComboBox* BorderS;
    QWidget* tab_2;
    QLabel* TextLabel70;
    QWidget* tab_3;
    Q3GroupBox* GroupBox40a;
    Q3GroupBox* GroupBox30a;
    Q3GroupBox* OptTextFeld;
    QCheckBox* MultiL;
    QCheckBox* Passwd;
    QCheckBox* Limit;
    QSpinBox* MaxChars;
    QCheckBox* NoScroll;
    QCheckBox* NoSpell;
    QLabel* TextLabel2_2;
    Q3GroupBox* OptCheck;
    QLabel* CText1;
    QComboBox* ChkStil;
    QCheckBox* isChkd;
    Q3GroupBox* OptCombo;
    QCheckBox* CanEdit;
    QWidget* tab4;
    QLabel* TextForm1;
    QComboBox* TxFormat;
    Q3GroupBox* GroupCust;
    QLabel* TextForm2;
    QPushButton* EditFormat;
    Q3TextEdit* FormatScript;
    QLabel* TextForm3;
    QPushButton* EditKeystr;
    Q3TextEdit* KeyScript;
    Q3Frame* FoFrameNone;
    Q3GroupBox* NumbGroup;
    QLabel* TextNu1;
    QSpinBox* Decim;
    QCheckBox* UseCurr;
    QLineEdit* CurSym;
    QCheckBox* PreCurr;
    Q3ButtonGroup* NumGroup2;
    QRadioButton* Format0;
    QRadioButton* Format1;
    QRadioButton* Format2;
    QRadioButton* Format3;
    Q3GroupBox* PercGroup;
    QLabel* TextNu1a;
    QSpinBox* Decim2;
    Q3ButtonGroup* NumGroup2a;
    QRadioButton* Format0a;
    QRadioButton* Format1a;
    QRadioButton* Format2a;
    QRadioButton* Format3a;
    Q3ButtonGroup* TimeGroup;
    QRadioButton* Format0b;
    QRadioButton* Format1b;
    QRadioButton* Format2b;
    QRadioButton* Format3b;
    Q3GroupBox* DateGroup;
    QComboBox* Format0c;
    QLabel* TextDa1;
    QWidget* tab_4;
    Q3ButtonGroup* ValidateGroup;
    QRadioButton* NoValid;
    QRadioButton* SimpleValid;
    QLineEdit* MinValid;
    QLineEdit* MaxValid;
    QRadioButton* CustomValid;
    Q3TextEdit* ValidScript;
    QPushButton* EditValScript;
    QWidget* tab_5;
    Q3ButtonGroup* CalcGroup;
    QRadioButton* NoCalc;
    QRadioButton* SimpleCalc;
    QComboBox* CalcArt;
    QLabel* TextLabel1_2;
    QLineEdit* CalcFields;
    QRadioButton* CustomCalc;
    Q3TextEdit* CalcScript;
    QPushButton* EditCalc;
    QPushButton* EditJ;
    QPushButton* SeField;
    Q3Frame* Frame3b;
    QLabel* SubText1;
    QLineEdit* SubURL;
    QCheckBox* SubAsHtml;
    Q3Frame* Frame3c;
    QLabel* SubText1a;
    QLineEdit* SubURLa;
    Q3GroupBox* OptIcons;
    QCheckBox* UseIcons;
    QPushButton* IconN;
    QLabel* NiconPrev;
    QPushButton* IconP;
    QLabel* PiconPrev;
    QPushButton* IconR;
    QLabel* RiconPrev;
    QPushButton* IconNR;
    QPushButton* IconPR;
    QPushButton* IconRR;
    QPushButton* PlaceIcon;
    QPushButton* PushButton1;
    QPushButton* PushButton2;
    PageItem* item;
    ScribusView* view;
    int Breite;
    int Hoehe;
    int OriBreite;
    int OriHoehe;
	int ScrEdited;
	int FormNum;
	QString JavaScr;
	int MaxSeite;
	QString OldName;

public slots:
	void NewName();
	void IPlace();
	void RemoveNIcon();
	void RemovePIcon();
	void RemoveRIcon();
	void IconsEin();
	void GetNIcon();
	void GetPIcon();
	void GetRIcon();
	void SelectFelder();
	void editKeySc();
	void editFormatSc();
	void editValidSc();
	void editCalcSc();
	void editJavaSc();
	void SetFormNum();
	void SetCurr();
	void HandleVali();
	void SetVali();
	void HandleCalc();
	void SetCalc();
	void SetFoScript(int it);
	void SetCo(double x, double y);
	void SetPg(int v);
	void SetCross();
	void SetVals();
	void SetZiel(int i);
	void GetFile();
	void SetActTyp(int it);
	void SetLimit();
	void SetActScript(int it);
	void setDateSample(const QString& ds);
	void SetExternL();

protected:
    QVBoxLayout* AnnotLayout;
    QHBoxLayout* Layout1;
    QHBoxLayout* Layout1_2;
    QGridLayout* GroupBox11Layout;
    QGridLayout* GroupBox2Layout;
    QVBoxLayout* Frame3Layout;
    QVBoxLayout* Frame3bLayout;
    QVBoxLayout* Frame3cLayout;
    QHBoxLayout* Layout50;
    QVBoxLayout* GroupBox10Layout;
    QGridLayout* Layout60;
    QVBoxLayout* tabLayout;
    QHBoxLayout* Layout10;
    QGridLayout* GroupBox40Layout;
    QGridLayout* GroupBox30Layout;
    QGridLayout* GroupBox20Layout;
    QVBoxLayout* tabLayout_2;
    QHBoxLayout* Layout20;
    QVBoxLayout* tabLayout_3;
    QGridLayout* GroupBox40aLayout;
    QGridLayout* GroupBox30aLayout;
    QGridLayout* Frame4aLayout;
    QVBoxLayout* OptTextFeldLayout;
    QHBoxLayout* Layout7;
    QHBoxLayout* Layout71;
    QHBoxLayout* Layout8;
    QVBoxLayout* OptCheckLayout;
    QHBoxLayout* ChkLayout;
    QVBoxLayout* OptComboLayout;
    QVBoxLayout* Layout;
    QHBoxLayout* FLayout;
    QVBoxLayout* GroupCustLayout;
    QVBoxLayout* FLayout3;
    QHBoxLayout* FLayout2;
    QVBoxLayout* FLayout5;
    QHBoxLayout* FLayout4;
    QVBoxLayout* NumbGroupLayout;
    QHBoxLayout* LayoutFN1;
    QHBoxLayout* LayoutFN2;
    QGridLayout* NumGroup2Layout;
    QVBoxLayout* PercGroupLayout;
    QHBoxLayout* LayoutFN1a;
    QGridLayout* NumGroup2aLayout;
    QVBoxLayout* TimeGroupLayout;
    QVBoxLayout* DateGroupLayout;
    QHBoxLayout* LayoutFN1c;
    QVBoxLayout* tabLayout_4;
    QVBoxLayout* ValidateGroupLayout;
    QGridLayout* VLayout1;
    QHBoxLayout* VLayout2;
    QVBoxLayout* tabLayout_5;
    QVBoxLayout* CalcGroupLayout;
    QHBoxLayout* CLayout1;
    QHBoxLayout* CLayout2;
    QHBoxLayout* CLayout3;
    QHBoxLayout* Layout18;
    QVBoxLayout* OptIconsLayout;
    QHBoxLayout* Layout17;
    QGridLayout* Layout14;
    QGridLayout* Layout15;
    QGridLayout* Layout16;
	PrefsContext* dirs;
};

#endif // ANNOT_H
