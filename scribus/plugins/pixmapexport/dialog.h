#ifndef EXPORTFORM_H
#define EXPORTFORM_H

#include <qvariant.h>
#include <qdialog.h>
#include <qradiobutton.h>
#include <prefscontext.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QButtonGroup;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;

class ExportForm : public QDialog
{
	Q_OBJECT

public:
	ExportForm( QWidget* parent, int size, int quality, QString type);
	~ExportForm();

	QString bitmapType;

    QLabel* TextLabel1;
    QLineEdit* OutputDirectory;
    QPushButton* OutputDirectoryButton;
    QGroupBox* groupBox1;
    QLabel* TextLabel2;
    QLabel* textLabel1;
    QLabel* textLabel3;
	QLabel* textLabel4;
    QComboBox* BitmapType;
    QSpinBox* QualityBox;
    QSpinBox* DPIBox;
	QSpinBox* EnlargementBox;
    QButtonGroup* ButtonGroup1;
    QRadioButton* OnePageRadio;
    QRadioButton* AllPagesRadio;
    QRadioButton* IntervalPagesRadio;
    QLineEdit* RangeVal;
    QPushButton* OkButton;
    QPushButton* CancelButton;

public slots:
	virtual void OutputDirectoryButton_pressed();
	virtual void OkButton_pressed();
	virtual void IntervalPagesRadio_stateChanged( int );
	virtual void AllPagesRadio_stateChanged( int );
	virtual void OnePageRadio_stateChanged( int );

protected:
    QVBoxLayout* ExportFormLayout;
    QHBoxLayout* layout1;
    QHBoxLayout* layout3;
    QGridLayout* groupBox1Layout;
    QVBoxLayout* ButtonGroup1Layout;
    QHBoxLayout* layout2;
    QHBoxLayout* layout4;
    PrefsContext* prefs;

protected slots:
    virtual void languageChange();
	virtual void readConfig();
	virtual void writeConfig();
};

#endif // EXPORTFORM_H
