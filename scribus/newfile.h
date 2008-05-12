/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef NEWDOC_H
#define NEWDOC_H

#include <QDialog>
#include <QListWidget>
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QFrame;
class QListWidgetItem;
class QGroupBox;
class QSpinBox;
class QLabel;
class QCheckBox;
class QPushButton;
#include "scribusapi.h"
#include "scribusstructs.h"

#include "customfdialog.h"

class PrefsManager;
class MarginWidget;
class PageLayouts;
class ScrSpinBox;
class ScComboBox;
#if QT_VERSION  >= 0x040300
	class QFileDialog;
#else
	class CustomFDialog;
#endif

class SCRIBUS_API PageLayoutsWidget : public QListWidget
{
	Q_OBJECT

public:
	PageLayoutsWidget(QWidget* parent);
	~PageLayoutsWidget() {};
	void arrangeIcons();
	const QSize minimumSizeHint();
	int maxX;
	int maxY;
};


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
	QFrame* newDocFrame;
	PageLayoutsWidget* layoutsView;
	QLabel* layoutLabel1;
	ScComboBox* firstPage;
	QGroupBox* pageSizeGroupBox;
	MarginWidget* marginGroup;
	QGroupBox* optionsGroupBox;
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
	QFrame* openDocFrame;
#if QT_VERSION  >= 0x040300
	QFileDialog *fileDialog;
#else
	CustomFDialog *fileDialog;
#endif
	QFrame* recentDocFrame;
	QListWidget* recentDocListBox;
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
	QString selectedFile;
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
	void itemSelected(QListWidgetItem* ic);
	void handleAutoFrame();
	void setDist(double v);
	void setUnit(int u);
	void ExitOK();
	void setOrien(int ori);
	void setPGsize(const QString &);
	void setDS(int layout);
	/*! Opens document on doubleclick
	\author Petr Vanek <petr@yarpen.cz>
	*/
	void recentDocListBox_doubleClicked();
	void openFile();

protected:
	QVBoxLayout* TabbedNewDocLayout;
	QHBoxLayout* Layout1;
	QGridLayout* NewDocLayout;
	QGridLayout* pageSizeGroupBoxLayout;
	QGridLayout* optionsGroupBoxLayout;
	QVBoxLayout* openDocLayout;
	QVBoxLayout* recentDocLayout;
	PrefsManager* prefsManager;
	QStringList recentDocList;
};

#endif // NEWDOC_H
