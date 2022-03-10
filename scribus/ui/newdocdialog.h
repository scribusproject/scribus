/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef NEWDOCDIALOG_H
#define NEWDOCDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QListWidget>

class QCheckBox;
class QComboBox;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QFrame;
class QGroupBox;
class QLabel;
class QListWidgetItem;
class QPushButton;
class QSpinBox;

#include "scribusapi.h"
#include "scribusstructs.h"

#include "ui/customfdialog.h"
#include "ui/nftwidget.h"

class PrefsManager;
class MarginWidget;
class ScrSpinBox;
class QFileDialog;

class SCRIBUS_API PageLayoutsWidget : public QListWidget
{
	Q_OBJECT

public:
	PageLayoutsWidget(QWidget* parent);
	~PageLayoutsWidget() = default;
	void arrangeIcons();
	QSize minimumSizeHint() const override;
	int maxX {0};
	int maxY {0};
};


class SCRIBUS_API NewDocDialog : public QDialog
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

	NewDocDialog( QWidget* parent, const QStringList& recentDocs, bool startUp = false, const QString& lang = "");
	~NewDocDialog() = default;

	void createNewDocPage();
	void createNewFromTempPage();
	void createOpenDocPage();
	void createRecentDocPage();
	void setSize(const QString& gr);

	QTabWidget* tabWidget {nullptr};
	QFrame* newDocFrame {nullptr};
	PageLayoutsWidget* layoutsView {nullptr};
	QLabel* layoutLabel1 {nullptr};
	QComboBox* firstPage {nullptr};
	QGroupBox* pageSizeGroupBox {nullptr};
	MarginWidget* marginGroup {nullptr};
	QGroupBox* optionsGroupBox {nullptr};
	QLabel* TextLabel1 {nullptr};
	QLabel* TextLabel2 {nullptr};
	QLabel* TextLabel3 {nullptr};
	QLabel* TextLabel4 {nullptr};
	QLabel* TextLabel1_2 {nullptr};
	QLabel* TextLabel1_3 {nullptr};
	QLabel* pageCountLabel {nullptr};
	QLabel* TextLabel2_2 {nullptr};
	QLabel* unitOfMeasureLabel {nullptr};
	QComboBox* pageSizeComboBox {nullptr};
	QComboBox* pageOrientationComboBox {nullptr};
	QComboBox* unitOfMeasureComboBox {nullptr};
	QCheckBox* startUpDialog {nullptr};
	QCheckBox* autoTextFrame {nullptr};
	QCheckBox* startDocSetup {nullptr};
	ScrSpinBox* Distance {nullptr};
	QSpinBox* numberOfCols {nullptr};
	QSpinBox* pageCountSpinBox {nullptr};
	ScrSpinBox* widthSpinBox {nullptr};
	ScrSpinBox* heightSpinBox {nullptr};
	QFrame* openDocFrame {nullptr};
	QFileDialog *fileDialog {nullptr};
	QFrame* newFromTempFrame {nullptr};
	nftwidget *nftGui {nullptr};
	QFrame* recentDocFrame {nullptr};
	QListWidget* recentDocListBox {nullptr};
	QDialogButtonBox* buttonBox {nullptr};
	QPushButton* okButton {nullptr};
	QPushButton* cancelButton {nullptr};

	bool onStartup() const { return m_onStartup;}
	int  tabSelected() const { return m_tabSelected;}
	QString selectedFile() const { return m_selectedFile; }

	int    unitIndex()   const { return m_unitIndex;}
	QString unitSuffix() const { return m_unitSuffix;}
	double unitRatio()   const { return m_unitRatio; }

	int    orientation() const { return m_orientation;}
	int    choosenLayout() const { return m_choosenLayout;}
	double pageWidth() const { return m_pageWidth;}
	double pageHeight() const { return m_pageHeight;}
	double distance() const { return m_distance;}
	double bleedBottom() const { return m_bleedBottom;}
	double bleedTop()    const { return m_bleedTop;}
	double bleedLeft()   const { return m_bleedLeft;}
	double bleedRight()  const { return m_bleedRight;}

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
	void adjustTitles(int tab);
	void locationDropped(const QString& fileUrl);
	void gotoParentDirectory();
	void gotoSelectedDirectory();
	void gotoDesktopDirectory();
	void gotoHomeDirectory();
	void openFileDialogFileClicked(const QString &path);

protected:
	QVBoxLayout* TabbedNewDocLayout {nullptr};
	QHBoxLayout* Layout1 {nullptr};
	QGridLayout* NewDocLayout {nullptr};
	QGridLayout* pageSizeGroupBoxLayout {nullptr};
	QGridLayout* optionsGroupBoxLayout {nullptr};
	QVBoxLayout* openDocLayout {nullptr};
	QVBoxLayout* recentDocLayout {nullptr};
	QVBoxLayout *verticalLayout {nullptr};
	PrefsManager& prefsManager;
	QStringList recentDocList;

	double m_unitRatio { 1.0 };
	int m_orientation { 0 };
	int m_choosenLayout { 0 };
	double m_pageWidth { 1.0 };
	double m_pageHeight { 1.0 };
	double m_distance { 11.0 };
	QString m_unitSuffix;
	QString m_selectedFile;
	int m_unitIndex { 0 };
	int m_tabSelected { 0 };
	bool m_onStartup { false };
	double m_bleedBottom { 0.0 };
	double m_bleedTop { 0.0 };
	double m_bleedLeft { 0.0 };
	double m_bleedRight { 0.0 };
};

#endif // NEWDOC_H
