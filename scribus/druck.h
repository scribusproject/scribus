/****************************************************************************
** Form interface generated from reading ui file 'Druck.ui'
**
** Created: Mon Sep 17 21:42:05 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DRUCK_H
#define DRUCK_H

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>

class AdvOptions : public QDialog
{
    Q_OBJECT

public:
	AdvOptions(QWidget* parent, bool Hm, bool Vm, bool Ic);
	~AdvOptions() {};
	QCheckBox* MirrorH;
	QCheckBox* MirrorV;
	QCheckBox* UseICC;
	QPushButton* PushButton1;
	QPushButton* PushButton2;

protected:
	QVBoxLayout* AdvOptionsLayout;
	QHBoxLayout* Layout2;
};

class Druck : public QDialog
{ 
    Q_OBJECT

public:
    Druck( QWidget* parent, QString PDatei, QString PDev, QString PCom );
    ~Druck() {};
	QString printerName();
	QString outputFileName();
    QCheckBox* OtherCom;
    QLabel* OthText;
    QLineEdit* Command;
	bool outputToFile();
	int fromPage();
	int toPage();
	int numCopies();
	int pageOrder();
	bool outputSeparations();
	QString separationName();
	QString PrinterOpts;
	bool color();
    bool MirrorH;
    bool MirrorV;
    bool ICCinUse;

public slots:
	void setMinMax(int min, int max);
	void setFromTo(int min, int max);
	
protected:
    QVBoxLayout* DruckLayout;
    QGridLayout* DruckerLayout;
    QHBoxLayout* Layout1x;
    QHBoxLayout* Layout1;
    QHBoxLayout* UmfangLayout;
    QGridLayout* ButtonGroup5Layout;
    QGridLayout* ButtonGroup4Layout;
    QVBoxLayout* ButtonGroup3Layout;
    QVBoxLayout* ButtonGroup3_2Layout;
    QGridLayout* OptionenLayout;
    QHBoxLayout* Layout2;
    QHBoxLayout* LayoutCC;

private slots:
	void SetAdvOptions();
	void SetOptions();
	void SelPrinter(const QString& prn);
	void SelRange(bool e);
	void SelMode(bool e);
	void SelFile();
	void SelComm();
	
private:
    QGroupBox* Drucker;
    QComboBox* PrintDest;
    QLabel* DateiT;
    QLineEdit* LineEdit1;
    QToolButton* ToolButton1;
    QButtonGroup* Umfang;
    QButtonGroup* ButtonGroup5;
    QLabel* TextLabel2_2;
    QSpinBox* To;
    QSpinBox* From;
    QLabel* TextLabel2;
    QRadioButton* RadioButton2;
    QRadioButton* RadioButton1;
    QButtonGroup* ButtonGroup4;
    QRadioButton* FirstPlast;
    QRadioButton* FirstPfirst;
    QLabel* TextLabel3;
    QSpinBox* Copies;
    QButtonGroup* Optionen;
    QButtonGroup* ButtonGroup3;
    QButtonGroup* ButtonGroup3_2;
    QRadioButton* NormalP;
    QRadioButton* PrintSep;
    QRadioButton* PrintGray;
    QRadioButton* PrintGray2;
    QComboBox* SepArt;
    QPushButton* OKButton_2;
    QPushButton* OKButton;
	QPushButton* OptButton;
	QPushButton* AdvOptButton;
    QString Geraet;
    bool ToFile;
    bool ToSeparation;
};

#endif // DRUCK_H
