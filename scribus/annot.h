/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include <QFrame>
#include <qlineedit.h>
#include <q3textedit.h>
#include <qtabwidget.h>
#include <qcheckbox.h>
#include <q3listbox.h>
#include <qradiobutton.h>
#include <q3buttongroup.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <Q3VBoxLayout>


#include "scribusapi.h"
#include "sccolor.h"

class PageItem;
class Navigator;
class ColorCombo;
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
    QFrame* Frame9;
    QComboBox* ActionCombo;
    Q3GroupBox* GroupBox2;
    QFrame* Frame3;
    QFrame* Frame4;
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
    ColorCombo* BorderC;
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
    QFrame* FoFrameNone;
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
    QFrame* Frame3b;
    QLabel* SubText1;
    QLineEdit* SubURL;
    QCheckBox* SubAsHtml;
    QFrame* Frame3c;
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
    Q3VBoxLayout* AnnotLayout;
    Q3HBoxLayout* Layout1;
    Q3HBoxLayout* Layout1_2;
    Q3GridLayout* GroupBox11Layout;
    Q3GridLayout* GroupBox2Layout;
    Q3VBoxLayout* Frame3Layout;
    Q3VBoxLayout* Frame3bLayout;
    Q3VBoxLayout* Frame3cLayout;
    Q3HBoxLayout* Layout50;
    Q3VBoxLayout* GroupBox10Layout;
    Q3GridLayout* Layout60;
    Q3VBoxLayout* tabLayout;
    Q3HBoxLayout* Layout10;
    Q3GridLayout* GroupBox40Layout;
    Q3GridLayout* GroupBox30Layout;
    Q3GridLayout* GroupBox20Layout;
    Q3VBoxLayout* tabLayout_2;
    Q3HBoxLayout* Layout20;
    Q3VBoxLayout* tabLayout_3;
    Q3GridLayout* GroupBox40aLayout;
    Q3GridLayout* GroupBox30aLayout;
    Q3GridLayout* Frame4aLayout;
    Q3VBoxLayout* OptTextFeldLayout;
    Q3HBoxLayout* Layout7;
    Q3HBoxLayout* Layout71;
    Q3HBoxLayout* Layout8;
    Q3VBoxLayout* OptCheckLayout;
    Q3HBoxLayout* ChkLayout;
    Q3VBoxLayout* OptComboLayout;
    Q3VBoxLayout* Layout;
    Q3HBoxLayout* FLayout;
    Q3VBoxLayout* GroupCustLayout;
    Q3VBoxLayout* FLayout3;
    Q3HBoxLayout* FLayout2;
    Q3VBoxLayout* FLayout5;
    Q3HBoxLayout* FLayout4;
    Q3VBoxLayout* NumbGroupLayout;
    Q3HBoxLayout* LayoutFN1;
    Q3HBoxLayout* LayoutFN2;
    Q3GridLayout* NumGroup2Layout;
    Q3VBoxLayout* PercGroupLayout;
    Q3HBoxLayout* LayoutFN1a;
    Q3GridLayout* NumGroup2aLayout;
    Q3VBoxLayout* TimeGroupLayout;
    Q3VBoxLayout* DateGroupLayout;
    Q3HBoxLayout* LayoutFN1c;
    Q3VBoxLayout* tabLayout_4;
    Q3VBoxLayout* ValidateGroupLayout;
    Q3GridLayout* VLayout1;
    Q3HBoxLayout* VLayout2;
    Q3VBoxLayout* tabLayout_5;
    Q3VBoxLayout* CalcGroupLayout;
    Q3HBoxLayout* CLayout1;
    Q3HBoxLayout* CLayout2;
    Q3HBoxLayout* CLayout3;
    Q3HBoxLayout* Layout18;
    Q3VBoxLayout* OptIconsLayout;
    Q3HBoxLayout* Layout17;
    Q3GridLayout* Layout14;
    Q3GridLayout* Layout15;
    Q3GridLayout* Layout16;
	PrefsContext* dirs;
};

#endif // ANNOT_H
