/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "scribusapi.h"

class QLabel;
class QWidget;
class QTabWidget;
class QPushButton;
class QShowEvent;


class SCRIBUS_API About : public QDialog
{ 
    	Q_OBJECT

public:

	typedef enum
	{
		Default = 0,
		CheckUpdates = 1
	} AboutMode;

	About( QWidget* parent, AboutMode mode = About::Default );
	~About() {}
	
protected:
	QLabel* pixmapLabel1 { nullptr };
	QLabel* buildID { nullptr };
	QPushButton* okButton { nullptr };
	QPushButton* checkForUpdateButton { nullptr };
	QTabWidget* tabWidget2 { nullptr };
	QWidget* tab { nullptr };
	QWidget* tab_2 { nullptr };
	QWidget* tab_3 { nullptr };
	QWidget* tab_4 { nullptr };
	QWidget* tab_5 { nullptr };
	QWidget* tab_Licence { nullptr };
	QWidget* tab_build { nullptr };
	QTextBrowser* authorView { nullptr };
	QTextBrowser* transView { nullptr };
	QTextBrowser* onlineView { nullptr };
	QTextBrowser* updateView { nullptr };
	QTextBrowser* textViewLicence { nullptr };
	QTextBrowser* textViewBuild { nullptr };
	QVBoxLayout* aboutLayout { nullptr };
	QVBoxLayout* updateLayout { nullptr };
	QVBoxLayout* licenceLayout { nullptr };
	QVBoxLayout* buildLayout { nullptr };
	QHBoxLayout* tabLayout { nullptr };
	QVBoxLayout* tabLayout1 { nullptr };
	QHBoxLayout* tabLayout_2 { nullptr };
	QHBoxLayout* tabLayout_4 { nullptr };
	QHBoxLayout* layout2 { nullptr };

	bool m_firstShow { true };
	AboutMode m_mode { About::Default };

	void showEvent(QShowEvent* event) override;
	QString trAuthorTitle(const QString& title);
	QString trTranslationTitle(const QString& title);
	QString trLinksTitle(const QString& title);
	QString parseAuthorFile(const QString& fileName);
	QString parseTranslationFile(const QString& fileName);
	QString parseLinksFile(const QString& fileName);
	QString generateBuildInfo();

protected slots:
	void runUpdateCheck();

public slots:
	virtual void setVisible (bool visible); 
};

#endif // ABOUT_H
