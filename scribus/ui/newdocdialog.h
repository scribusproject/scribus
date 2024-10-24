/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef NEWDOCDIALOG_H
#define NEWDOCDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QListWidget>
#include <QVBoxLayout>

class QButtonGroup;
class QFileDialog;
class QFrame;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QListWidgetItem;
class QPushButton;
class QSpinBox;

#include "scribusapi.h"
#include "scribusstructs.h"
#include "ui/customfdialog.h"
#include "ui/nftwidget.h"

#include "ui_newdocdialog.h"

class NewMarginWidget;
class PrefsManager;
class ScrSpinBox;

class SCRIBUS_API NewDocDialog : public QDialog, public Ui::newDocDialog
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
	void createOpenDocPage();
	void createRecentDocPage();
	void setSize(const QString& gr);
	QString pageSizeName() { return m_pageSize; };

	QFileDialog *fileDialog {nullptr};

	bool onStartup() const { return m_onStartup;}
	int tabSelected() const { return m_tabSelected;}
	QString selectedFile() const { return m_selectedFile; }

	int unitIndex() const { return m_unitIndex;}
	QString unitSuffix() const { return m_unitSuffix;}
	double unitRatio() const { return m_unitRatio; }

	int orientation() const { return m_orientation;}
	int choosenLayout() const { return m_choosenLayout;}
	double pageWidth() const { return m_pageWidth;}
	double pageHeight() const { return m_pageHeight;}
	double distance() const { return m_distance;}
	double bleedBottom() const { return m_bleedBottom;}
	double bleedTop() const { return m_bleedTop;}
	double bleedLeft() const { return m_bleedLeft;}
	double bleedRight() const { return m_bleedRight;}

public slots:
	void setHeight(double v);
	void setWidth(double v);
	void selectItem(uint nr);	
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

private slots:
	void changeMargin(MarginStruct margin);
	void changeBleed(MarginStruct bleed);
	void changeFirstPage(int firstPage);
	void changeCategory(PageSizeInfo::Category category);
	void changePageSize(const QModelIndex &ic);
	void changeScheme(int ic);
	void changeSortMode(int ic);

protected:
	PrefsManager& prefsManager;
	QStringList recentDocList;
	QButtonGroup* pageOrientationButtons;

	double m_unitRatio { 1.0 };
	int m_orientation { 0 };
	int m_choosenLayout { 0 };
	double m_pageWidth { 1.0 };
	double m_pageHeight { 1.0 };
	double m_distance { 11.0 };
	QString m_unitSuffix;
	QString m_selectedFile;
	QString m_pageSize;
	int m_unitIndex { 0 };
	int m_tabSelected { 0 };
	bool m_onStartup { false };
	double m_bleedBottom { 0.0 };
	double m_bleedTop { 0.0 };
	double m_bleedLeft { 0.0 };
	double m_bleedRight { 0.0 };

	void windowFitInScreen();
};

#endif // NEWDOC_H
