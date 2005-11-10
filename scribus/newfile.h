#ifndef NEWDOC_H
#define NEWDOC_H

#include <qdialog.h>
#include <q3buttongroup.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qtabwidget.h>
#include <q3frame.h>
#include <q3listbox.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "scribusapi.h"
#include "scribusstructs.h"

#include "customfdialog.h"

class PrefsManager;
class MarginWidget;
class PageLayouts;
class MSpinBox;
class CustomFDialog;


class SCRIBUS_API NewDoc : public QDialog
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
	Q3Frame* newDocFrame;
	PageLayouts* docLayout;
	Q3ButtonGroup* ButtonGroup1_2;
	MarginWidget* GroupRand;
	Q3GroupBox* GroupBox3;
	Q3GroupBox* AutoFrame;
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
	Q3Frame* openDocFrame;
	CustomFDialog *fileDialog;
	Q3Frame* recentDocFrame;
	Q3ListBox* recentDocList;
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
	void recentDocList_doubleClicked(Q3ListBoxItem * item);
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
