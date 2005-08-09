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
#include <qtabwidget.h>
#include <qframe.h>
#include <qlistbox.h>
#include "mspinbox.h"
#include "customfdialog.h"


#include "scribusstructs.h"
class PrefsManager;
class MarginWidget;
class PageLayouts;

class NewDoc : public QDialog
{
	Q_OBJECT

public:
	NewDoc( QWidget* parent, bool startUp = false );
	~NewDoc() {};
	void createNewDocPage();
	void createOpenDocPage();
	void createRecentDocPage();
	void setSize(QString gr);

	QTabWidget* tabWidget;
	QFrame* newDocFrame;
	PageLayouts* docLayout;
	QButtonGroup* ButtonGroup1_2;
	MarginWidget* GroupRand;
	QGroupBox* GroupBox3;
	QGroupBox* AutoFrame;
	QLabel* TextLabel1;
	QLabel* TextLabel2;
	QLabel* TextLabel3;
	QLabel* TextLabel4;
	QLabel* TextLabel1_2;
	QLabel* TextLabel1_3;
	QLabel* TextLabel1_3a;
	QLabel* TextLabel2_2;
	QLabel* TextLabel2_3;
	QComboBox* ComboBox1;
	QComboBox* ComboBox2;
	QComboBox* ComboBox3;
	QCheckBox* startUpDialog;
	MSpinBox* Distance;
	QSpinBox* SpinBox10;
	QSpinBox* PgNr;
	QSpinBox* PgNum;
	MSpinBox* Breite;
	MSpinBox* Hoehe;
	QFrame* openDocFrame;
	CustomFDialog *fileDialog;
	QFrame* recentDocFrame;
	QListBox* recentDocList;
	QPushButton* OKButton;
	QPushButton* CancelB;
	double unitRatio;
	int Orient;
	int precision;
	int choosenLayout;
	double Pagebr;
	double Pageho;
	double Dist;
	QString unitSuffix;
	int unitIndex;
	int tabSelected;
	bool onStartup;

public slots:
	void setHoehe(int v);
	void setBreite(int v);
	void setDist(int v);
	void setUnit(int u);
	void ExitOK();
	void setOrien(int ori);
	void setPGsize(const QString &);
	void setDS(int layout);
	/*! Opens document on doubleclick
	\param item QListViewItem sent by signall caller
	\author Petr Vanek <petr@yarpen.cz
	*/
	void recentDocList_doubleClicked(QListBoxItem * item);
	void openFile(const QString &);

protected:
	QVBoxLayout* TabbedNewDocLayout;
	QGridLayout* Layout4;
	QHBoxLayout* NewDocLayout;
	QVBoxLayout* Layout9;
	QVBoxLayout* ButtonGroup1_2Layout;
	QGridLayout* Layout6;
	QHBoxLayout* Layout5;
	QVBoxLayout* Layout10;
	QGridLayout* GroupBox3Layout;
	QHBoxLayout* GroupBox4Layout;
	QGridLayout* Layout2;
	QHBoxLayout* Layout1;
	QVBoxLayout* recentDocLayout;
	QString customText, customTextTR;
	PrefsManager* prefsManager;
};

#endif // NEWDOC_H
