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

#include "scribusapi.h"

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
#include <qtabwidget.h>
#include <qwidget.h>

class PrefsContext;

class SCRIBUS_API Druck : public QDialog
{
	Q_OBJECT

public:
	Druck( QWidget* parent, QString PDatei, QString PDev, QString PCom, bool gcr, QStringList spots);
	~Druck() {};
	QString printerName();
	QString outputFileName();
	QCheckBox* OtherCom;
	QLabel* OthText;
	QLineEdit* Command;
	QRadioButton* RadioButton1;
	QRadioButton* CurrentPage;
	QLineEdit* PageNr;
	bool outputToFile();
	int numCopies();
	bool outputSeparations();
	QString separationName();
	QStringList allSeparations();
	QString PrinterOpts;
	bool color();
	bool mirrorHorizontal();
	bool mirrorVertical();
	bool doGCR();
	int PSLevel();
	bool doDev();
	bool doSpot();
	bool ICCinUse();

public slots:
	void setMinMax(int min, int max, int cur);

signals:
	void doPreview();

protected:
	QVBoxLayout* DruckLayout;
	QGridLayout* DruckerLayout;
	QHBoxLayout* Layout1x;
	QHBoxLayout* Layout1;
	QGridLayout* rangeGroupLayout;
	QHBoxLayout* Layout2;
	QHBoxLayout* LayoutCC;
	QGridLayout* tabLayout;
	QHBoxLayout* tabLayout_2;
	QVBoxLayout* pageOptsLayout;
	QVBoxLayout* colorOptsLayout;

private slots:
	void SetOptions();
	void SelPrinter(const QString& prn);
	void SelRange(bool e);
	void SelMode(int e);
	void SelFile();
	void SelComm();
	void okButtonClicked();

private:
	QGroupBox* Drucker;
	QComboBox* PrintDest;
	QLabel* DateiT;
	QLineEdit* LineEdit1;
	QToolButton* ToolButton1;
	QButtonGroup* rangeGroup;
	QRadioButton* RadioButton2;
	QLabel* TextLabel3;
	QSpinBox* Copies;
	QTabWidget* printOptions;
	QWidget* tab;
	QComboBox* PrintSep;
	QComboBox* colorType;
	QComboBox* SepArt;
	QComboBox* psLevel;
	QWidget* tab_2;
	QGroupBox* pageOpts;
	QCheckBox* MirrorHor;
	QCheckBox* MirrorVert;
	QCheckBox* devPar;
	QButtonGroup* colorOpts;
	QCheckBox* GcR;
	QCheckBox* spotColors;
	QCheckBox* UseICC;
	QPushButton* OKButton_2;
	QPushButton* OKButton;
	QPushButton* previewButton;
	QPushButton* OptButton;
	QString Geraet;
	bool ToFile;
	bool ToSeparation;
	PrefsContext* prefs;
	void setStoredValues(bool gcr);
};

#endif // DRUCK_H
