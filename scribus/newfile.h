#ifndef NEWDOC_H
#define NEWDOC_H

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "mspinbox.h"

#include "scribusstructs.h"
class MarginWidget;

class NewDoc : public QDialog
{
	Q_OBJECT

public:
	NewDoc( QWidget* parent, ApplicationPrefs *Vor );
	~NewDoc() {};

	QButtonGroup* ButtonGroup1_2;
	MarginWidget* GroupRand;
	QGroupBox* GroupBox3;
	QGroupBox* GroupBox4;
	QLabel* TextLabel1;
	QLabel* TextLabel2;
	QLabel* TextLabel3;
	QLabel* TextLabel4;
	QLabel* TextLabel1_2;
	QLabel* TextLabel1_3;
	QLabel* TextLabel2_2;
	QLabel* TextLabel2_3;
	QComboBox* ComboBox1;
	QComboBox* ComboBox2;
	QComboBox* ComboBox3;
	QCheckBox* Doppelseiten;
	QCheckBox* ErsteSeite;
	QCheckBox* AutoFrame;
	MSpinBox* Distance;
	QSpinBox* SpinBox10;
	QSpinBox* PgNr;
	MSpinBox* Breite;
	MSpinBox* Hoehe;
	QPushButton* OKButton;
	QPushButton* CancelB;
	double unitRatio;
	int Orient;
	double Pagebr;
	double Pageho;
	double Dist;
	QString unitSuffix;
	int unitIndex;
	void setSize(QString gr);
	void code_repeat(int);

public slots:
	void setHoehe(int v);
	void setBreite(int v);
	void setDist(int v);
	void setUnit(int u);
	void ExitOK();
	void setOrien(int ori);
	void setPGsize(const QString &);
	void setDS();
	void setAT();

protected:
	QGridLayout* Layout4;
	QHBoxLayout* NewDocLayout;
	QVBoxLayout* Layout9;
	QVBoxLayout* ButtonGroup1_2Layout;
	QGridLayout* Layout6;
	QHBoxLayout* Layout5;
	QHBoxLayout* Layout8;
	QVBoxLayout* Layout10;
	QGridLayout* GroupBox3Layout;
	QHBoxLayout* GroupBox4Layout;
	QGridLayout* Layout2;
	QHBoxLayout* Layout1;
	QString customText, customTextTR;
};

#endif // NEWDOC_H
