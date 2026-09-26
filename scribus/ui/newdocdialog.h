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
class QWidget;

#include "scdialog.h"
#include "scribusapi.h"
#include "scribusstructs.h"
#include "ui/customfdialog.h"
#include "ui/nftwidget.h"

#include "ui_newdocdialog.h"

class NewMarginWidget;
class PrefsManager;
class ScrSpinBox;

class SCRIBUS_API NewDocDialog : public ScDialog, public Ui::newDocDialog
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
	void setSize(QSizeF size);

	QFileDialog *fileDialog {nullptr};

	bool onStartup() const { return m_onStartup;}
	int tabSelected() const { return m_tabSelected;}
	QString selectedFile() const { return m_selectedFile; }

	int unitIndex() const { return m_unitIndex;}
	QString unitSuffix() const { return m_unitSuffix;}
	double unitRatio() const { return m_unitRatio; }

	int orientation() const { return m_orientation;}
	int choosenLayout() const { return m_choosenLayout;}
	int bindingDirection() const { return m_bindingDirection; }
	int layoutFirstPage() const { return m_layoutFirstPage; }
	double pageWidth() const { return m_pageWidth;}
	double pageHeight() const { return m_pageHeight;}
	double distance() const { return m_distance;}
	MarginStruct margins() const { return marginGroup->margins(); }
	MarginStruct bleeds() const { return bleedGroup->margins(); }

public slots:
	void setHeight(double v);
	void setWidth(double v);
	void handleAutoFrame();
	void setDistance(double v);
	void setUnit(int u);
	void ExitOK();
	void setOrientation(int ori);
	void setLayout(int layoutId);
	void setBindingDirection(bool checked);
	void setPageSize(QSizeF size);
	void setDocLayout(int layout);
	void setDocFirstPage(int firstPage);
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
	void showWelcomePage();

private slots:
	void changeMargin(MarginStruct margin);
	void changeBleed(MarginStruct bleed);
	void changeCategory(const QString& category);
	void changePageSize(const QModelIndex &ic);
	void changeSortMode(int ic);
	void savePagePreset();
	void updateCategorySelector();

protected:
	PrefsManager& prefsManager;
	QStringList recentDocList;
	QButtonGroup* pageOrientationButtons;
	QButtonGroup* pageLayoutButtons;

	double m_unitRatio { 1.0 };
	int m_orientation { 0 };
	int m_choosenLayout { 0 };
	int m_bindingDirection { 0 }; // 0 = LTR, 1 = RTL
	int m_layoutFirstPage { 0 };
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
	bool m_labelVisibity {true};
	QWidget* m_welcomePage { nullptr };
	QListWidget* m_welcomeRecentList { nullptr };
	QPushButton* m_backButton { nullptr };
	QCheckBox* m_openPreviewCheck { nullptr };
	QWidget* m_openPreviewContainer { nullptr };
	FDialogPreview* m_openPreview { nullptr };

	bool eventFilter(QObject *object, QEvent *event);
	void createWelcomePage();
	int logicalTabIndex(int widgetTabIndex) const;
	void updateCategory(const QString& category, bool forceUpdate = false);
};

#endif // NEWDOC_H
