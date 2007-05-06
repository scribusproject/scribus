/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EXPORTFORM_H
#define EXPORTFORM_H

#include <qvariant.h>
#include <qdialog.h>
#include <qradiobutton.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class Q3ButtonGroup;
class QComboBox;
class Q3GroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class PrefsContext;
class ScrSpinBox;
class ScribusDoc;

class ExportForm : public QDialog
{
	Q_OBJECT

public:
	ExportForm( QWidget* parent, ScribusDoc* doc, int size, int quality, QString type);
	~ExportForm();

	QString bitmapType;

	QLabel* TextLabel1;
	QLineEdit* OutputDirectory;
	QPushButton* OutputDirectoryButton;
	Q3GroupBox* groupBox1;
	QLabel* TextLabel2;
	QLabel* textLabel1;
	QLabel* textLabel3;
	QLabel* textLabel4;
	QLabel* textLabel5;
	QLabel* textLabel6;
	QComboBox* BitmapType;
	QSpinBox* QualityBox;
	QSpinBox* DPIBox;
	ScrSpinBox* EnlargementBox;
	Q3ButtonGroup* ButtonGroup1;
	QRadioButton* OnePageRadio;
	QRadioButton* AllPagesRadio;
	QRadioButton* IntervalPagesRadio;
	QLineEdit* RangeVal;
	QPushButton* OkButton;
	QPushButton* CancelButton;
	

public slots:
	virtual void OutputDirectoryButton_pressed();
	virtual void OkButton_pressed();
	virtual void IntervalPagesRadio_stateChanged();
	virtual void AllPagesRadio_stateChanged();
	virtual void OnePageRadio_stateChanged();
	virtual void computeSize();

protected:
	Q3VBoxLayout* ExportFormLayout;
	Q3HBoxLayout* layout1;
	Q3HBoxLayout* layout3;
	Q3GridLayout* groupBox1Layout;
	Q3VBoxLayout* ButtonGroup1Layout;
	Q3HBoxLayout* layout2;
	Q3HBoxLayout* layout4;
	Q3HBoxLayout *pageNumberSelectorLayout;
	QPushButton* pageNrButton;
	PrefsContext* prefs;
	ScribusDoc*  m_doc;
	int m_PageCount;
	
protected slots:
	virtual void createPageNumberRange();
	virtual void languageChange();
	virtual void readConfig();
	virtual void writeConfig();
};

#endif // EXPORTFORM_H
