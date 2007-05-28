/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include <qstringlist.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qtabwidget.h>
#include <q3frame.h>
#include <q3listbox.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "scribusapi.h"
#include "scribusstructs.h"

#include "customfdialog.h"

class PrefsManager;
class MarginWidget;
class PageLayouts;
class ScrSpinBox;
class CustomFDialog;
class ScComboBox;
class Q3IconView;
class Q3IconViewItem;


class SCRIBUS_API NewDoc : public QDialog
{
	Q_OBJECT

public:
	NewDoc( QWidget* parent, const QStringList& recentDocs, bool startUp = false );
	~NewDoc() {};
	void createNewDocPage();
	void createOpenDocPage();
	void createRecentDocPage();
	void setSize(QString gr);

	QTabWidget* tabWidget;
	Q3Frame* newDocFrame;
	Q3IconView* layoutsView;
	QLabel* layoutLabel1;
	ScComboBox* firstPage;
	Q3GroupBox* pageSizeGroupBox;
	MarginWidget* marginGroup;
	Q3GroupBox* optionsGroupBox;
	QLabel* TextLabel1;
	QLabel* TextLabel2;
	QLabel* TextLabel3;
	QLabel* TextLabel4;
	QLabel* TextLabel1_2;
	QLabel* TextLabel1_3;
	QLabel* pageCountLabel;
	QLabel* TextLabel2_2;
	QLabel* unitOfMeasureLabel;
	QComboBox* pageSizeComboBox;
	QComboBox* pageOrientationComboBox;
	QComboBox* unitOfMeasureComboBox;
	QCheckBox* startUpDialog;
	QCheckBox* autoTextFrame;
	QCheckBox* startDocSetup;
	ScrSpinBox* Distance;
	QSpinBox* numberOfCols;
	QSpinBox* pageCountSpinBox;
	ScrSpinBox* widthSpinBox;
	ScrSpinBox* heightSpinBox;
	Q3Frame* openDocFrame;
	CustomFDialog *fileDialog;
	Q3Frame* recentDocFrame;
	Q3ListBox* recentDocListBox;
	QPushButton* OKButton;
	QPushButton* CancelB;
	double unitRatio;
	int Orient;
	int precision;
	int choosenLayout;
	double pageWidth;
	double pageHeight;
	double Dist;
	QString unitSuffix;
	int unitIndex;
	int tabSelected;
	bool onStartup;
	double bleedBottom;
	double bleedTop;
	double bleedLeft;
	double bleedRight;

public slots:
	void setHeight(double v);
	void setWidth(double v);
	void selectItem(uint nr);
	void itemSelected(Q3IconViewItem* ic);
	void handleAutoFrame();
	void setDist(double v);
	void setUnit(int u);
	void ExitOK();
	void setOrien(int ori);
	void setPGsize(const QString &);
	void setDS(int layout);
	/*! Opens document on doubleclick
	\param index no of the item
	\author Petr Vanek <petr@yarpen.cz
	*/
	void recentDocListBox_doubleClicked(int index);
	void openFile(const QString &);

protected:
	Q3VBoxLayout* TabbedNewDocLayout;
	Q3GridLayout* Layout4;
	Q3GridLayout* NewDocLayout;
	Q3HBoxLayout* Layout9;
	Q3GridLayout* pageSizeGroupBoxLayout;
	Q3HBoxLayout* Layout5;
	Q3HBoxLayout* Layout10;
	Q3GridLayout* optionsGroupBoxLayout;
	Q3HBoxLayout* Layout1;
	Q3VBoxLayout* recentDocLayout;
	PrefsManager* prefsManager;
	QStringList recentDocList;
};

#endif // NEWDOC_H
