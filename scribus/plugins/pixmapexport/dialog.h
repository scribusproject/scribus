#ifndef EXPORTFORM_H
#define EXPORTFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QSpinBox;
class QButtonGroup;
class QRadioButton;

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
	QLabel* TextLabel2;
	QComboBox* BitmapType;
	QLabel* textLabel1;
	QSpinBox* QualityBox;
	QLabel* textLabel2;
	QLabel* textLabel3;
	QSpinBox* SizeBox;
	QButtonGroup* ButtonGroup1;
	QRadioButton* OnePageRadio;
	QRadioButton* AllPagesRadio;
	QRadioButton* IntervalPagesRadio;
	QSpinBox* FromBox;
	QLabel* TextLabel4;
	QSpinBox* ToBox;
	QPushButton* OkButton;
	QPushButton* CancelButton;

public slots:
	virtual void OutputDirectoryButton_pressed();
	virtual void OkButton_pressed();
	virtual void CancelButton_pressed();
	virtual void IntervalPagesRadio_stateChanged( int );
	virtual void AllPagesRadio_stateChanged( int );
	virtual void OnePageRadio_stateChanged( int );
	virtual void ChFrom();
	virtual void ChTo();

protected:
	QGridLayout* ExportFormLayout;
	QHBoxLayout* layout9;
	QHBoxLayout* layout11;
	QSpacerItem* Spacer2;
	QHBoxLayout* layout10;
	QHBoxLayout* layout14;
	QSpacerItem* spacer4;
	QSpacerItem* spacer5;
	QHBoxLayout* layout12;
	QHBoxLayout* layout13;
	QHBoxLayout* layout13_2;
	QHBoxLayout* layout16;
	QHBoxLayout* layout8;
	QSpacerItem* Spacer1;

protected slots:
	virtual void languageChange();
	virtual void readConfig();
	virtual void writeConfig();
};

#endif // EXPORTFORM_H
