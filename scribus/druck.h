/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef DRUCK_H
#define DRUCK_H

#include "scribusapi.h"

#include <qdialog.h>
#include <q3buttongroup.h>
#include <qcombobox.h>
#include <q3groupbox.h>
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
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <Q3VBoxLayout>
#include "mspinbox.h"

class PrefsContext;
class ScribusDoc;
class CupsOptions;

class SCRIBUS_API Druck : public QDialog
{
	Q_OBJECT

public:
	Druck( QWidget* parent, ScribusDoc* doc, QString PDatei, QString PDev, QString PCom, QByteArray& PSettings, bool gcr, QStringList spots);
	~Druck();
	QString printerName();
	QString outputFileName();
	QCheckBox* OtherCom;
	QLabel* OthText;
	QLineEdit* Command;
	QRadioButton* RadioButton1;
	QRadioButton* CurrentPage;
	QLineEdit* pageNr;
	QCheckBox* cropMarks;
	QCheckBox* bleedMarks;
	QCheckBox* registrationMarks;
	QCheckBox* colorMarks;
	MSpinBox* markOffset;
	MSpinBox* BleedBottom;
	MSpinBox* BleedLeft;
	MSpinBox* BleedRight;
	MSpinBox* BleedTop;
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
	bool doClip();
	int PSLevel();
	bool doDev();
	bool doSpot();
	bool doOverprint();
	bool ICCinUse();
	QByteArray DevMode;

public slots:
	void setMinMax(int min, int max, int cur);

signals:
	void doPreview();

protected slots:
	void doDocBleeds();
	void createPageNumberRange();
	void SetOptions();
	void SelPrinter(const QString& prn);
	void SelRange(bool e);
	void SelMode(int e);
	void SelFile();
	void SelComm();
	void okButtonClicked();
	void previewButtonClicked();

protected:
	Q3VBoxLayout* DruckLayout;
	Q3GridLayout* DruckerLayout;
	Q3HBoxLayout* Layout1x;
	Q3HBoxLayout* Layout1;
	Q3GridLayout* rangeGroupLayout;
	Q3HBoxLayout* Layout2;
	Q3HBoxLayout* LayoutCC;
	Q3GridLayout* tabLayout;
	Q3HBoxLayout* tabLayout_2;
	Q3VBoxLayout* pageOptsLayout;
	Q3VBoxLayout* colorOptsLayout;
	Q3HBoxLayout *pageNumberSelectorLayout;
	Q3GroupBox* Drucker;
	QComboBox* PrintDest;
	QLabel* DateiT;
	QLineEdit* LineEdit1;
	QToolButton* ToolButton1;
	Q3ButtonGroup* rangeGroup;
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
	Q3GroupBox* pageOpts;
	QCheckBox* MirrorHor;
	QCheckBox* MirrorVert;
	QCheckBox* devPar;
	Q3ButtonGroup* colorOpts;
	QCheckBox* ClipMarg;
	QCheckBox* GcR;
	QCheckBox* spotColors;
	QCheckBox* overprintMode;
	QCheckBox* UseICC;
	QWidget* tab_3;
	Q3GridLayout* tabLayout_3;
	Q3GroupBox* MarkGroup;
	Q3GridLayout* MarkGroupLayout;
	QLabel* MarkTxt1;
	QWidget* tab_4;
	Q3GridLayout* tabLayout_4;
	Q3GroupBox* BleedGroup;
	Q3GridLayout* BleedGroupLayout;
	QLabel* BleedTxt1;
	QLabel* BleedTxt2;
	QLabel* BleedTxt3;
	QLabel* BleedTxt4;
	QCheckBox *docBleeds;
	QPushButton* OKButton_2;
	QPushButton* OKButton;
	QPushButton* previewButton;
	QPushButton* OptButton;
	QPushButton* pageNrButton;
	ScribusDoc* m_doc;
	QString Geraet;
	bool ToFile;
	bool ToSeparation;
	PrefsContext* prefs;
	void setStoredValues(bool gcr);
	/* CB Moved to printerutil.cpp
	QStringList getPrinterNames(void);
	bool getDefaultSettings( QString printerName );
	bool initDeviceSettings( QString printerName );
	*/
	CupsOptions *cdia;
	QString unit;
	int precision;
	double unitRatio;
	void getOptions();
	void storeValues();
};

#endif // DRUCK_H
