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
class QFormLayout;
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

#include "ui/customfdialog.h"
#include "ui/nftwidget.h"

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

	//! \brief Indexes of the dialog's tabs.
	enum {
		NewDocumentTab = 0,
		NewFromTemplateTab,
		OpenExistingTab,
		OpenRecentTab
	} ActionSelected;

	NewDoc( QWidget* parent, const QStringList& recentDocs, bool startUp = false, QString lang = "", QString templateDir = "" );
	~NewDoc() {};
	void createNewDocPage();
	void createNewFromTempPage();
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
	QFrame* newFromTempFrame;
	nftwidget *nftGui;
	QFrame* recentDocFrame;
	QListWidget* recentDocListBox;
	QPushButton* OKButton;
	QPushButton* CancelB;

	bool onStartup(void) const { return m_onStartup;}
	int  tabSelected(void) const { return m_tabSelected;}
	QString selectedFile(void) const { return m_selectedFile; }

	int    unitIndex(void)   const { return m_unitIndex;}
	QString unitSuffix(void) const { return m_unitSuffix;}
	double unitRatio(void)   const { return m_unitRatio; }

	int    orientation(void) const { return m_orientation;} 
	int    precision(void) const { return m_precision;}
	int    choosenLayout(void) const { return m_choosenLayout;}
	double pageWidth(void) const { return m_pageWidth;}
	double pageHeight(void) const { return m_pageHeight;}
	double distance(void) const { return m_distance;}
	double bleedBottom(void) const { return m_bleedBottom;}
	double bleedTop(void)    const { return m_bleedTop;}
	double bleedLeft(void)   const { return m_bleedLeft;}
	double bleedRight(void)  const { return m_bleedRight;}

public slots:
	void setHeight(double v);
	void setWidth(double v);
	void selectItem(uint nr);
	void itemSelected(QListWidgetItem* ic);
	void handleAutoFrame();
	void setDistance(double v);
	void setUnit(int u);
	void ExitOK();
	void setOrientation(int ori);
	void setPageSize(const QString &);
	void setDocLayout(int layout);
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
	QFormLayout* optionsGroupBoxLayout;
	QVBoxLayout* openDocLayout;
	QVBoxLayout* recentDocLayout;
	QVBoxLayout *verticalLayout;
	PrefsManager* prefsManager;
	QStringList recentDocList;

	double m_unitRatio;
	int m_orientation;
	int m_precision;
	int m_choosenLayout;
	double m_pageWidth;
	double m_pageHeight;
	double m_distance;
	QString m_unitSuffix;
	QString m_selectedFile;
	int m_unitIndex;
	int m_tabSelected;
	bool m_onStartup;
	double m_bleedBottom;
	double m_bleedTop;
	double m_bleedLeft;
	double m_bleedRight;
};

#endif // NEWDOC_H
