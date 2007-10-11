/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTDIALOG_H
#define PRINTDIALOG_H

#include "scribusapi.h"

#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QLabel;
class QGroupBox;
class QComboBox;
class QPushButton;
class QRadioButton;
class QLineEdit;
class QCheckBox;
class QTabWidget;
class QSpinBox;
class QWidget;
class QToolButton;

class PrefsContext;
class ScribusDoc;
class CupsOptions;
class ScrSpinBox;

class SCRIBUS_API PrintDialog : public QDialog
{
	Q_OBJECT

public:
	PrintDialog( QWidget* parent, ScribusDoc* doc, QString PDatei, QString PDev, QString PCom, QByteArray& PSettings, bool gcr, QStringList spots);
	~PrintDialog();
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
	ScrSpinBox* markOffset;
	ScrSpinBox* BleedBottom;
	ScrSpinBox* BleedLeft;
	ScrSpinBox* BleedRight;
	ScrSpinBox* BleedTop;
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
	QVBoxLayout* PrintDialogLayout;
	QGridLayout* PrintDialogLayout2;
	QHBoxLayout* Layout1x;
	QHBoxLayout* Layout1;
	QGridLayout* rangeGroupLayout;
	QHBoxLayout* Layout2;
	QHBoxLayout* LayoutCC;
	QGridLayout* tabLayout;
	QHBoxLayout* tabLayout_2;
	QVBoxLayout* pageOptsLayout;
	QVBoxLayout* colorOptsLayout;
	QHBoxLayout *pageNumberSelectorLayout;
	QGroupBox* PrintDialogGroup;
	QComboBox* PrintDest;
	QLabel* DateiT;
	QLineEdit* LineEdit1;
	QToolButton* ToolButton1;
	QGroupBox* rangeGroup;
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
	QGroupBox* colorOpts;
	QCheckBox* ClipMarg;
	QCheckBox* GcR;
	QCheckBox* spotColors;
	QCheckBox* overprintMode;
	QCheckBox* UseICC;
	QWidget* tab_3;
	QGridLayout* tabLayout_3;
	QGroupBox* MarkGroup;
	QGridLayout* MarkGroupLayout;
	QLabel* MarkTxt1;
	QWidget* tab_4;
	QGridLayout* tabLayout_4;
	QGroupBox* BleedGroup;
	QGridLayout* BleedGroupLayout;
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
	int unit;
	double unitRatio;
	void getOptions();
	void storeValues();
};

#endif // PRINTDIALOG_H
