/****************************************************************************
** Form interface generated from reading ui file 'Info.ui'
**
** Created: Mon Apr 30 10:14:32 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DOCINFOS_H
#define DOCINFOS_H

#include <qtabwidget.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmultilineedit.h>

#include "documentinformation.h"

/*! The DocInfos class is now part of the ReformDoc multi widgets storage.
See reformdoc.h as reference. The return values are handled via. getDocInfo()
method. It can be used as standalone widget. */
class DocInfos : public QTabWidget
{
    Q_OBJECT

public:
	DocInfos( QWidget* parent, DocumentInformation& docInfo );
	~DocInfos() {};

	/*! Class wide DocumentInformation& docInfo reference. */
	DocumentInformation infos;

	/*! Returns the changed values.
	Used e.g. in ScribusApp::SetupDoc().
	\retval DocumentInformation structure */
	DocumentInformation getDocInfo();

	/*! (Re)loads the document informations.
	See e.g. ReformDoc::restoreDefaults() */
	void restoreDefaults();

protected:
	QLineEdit* publisherEdit;
	QLineEdit* dateEdit;
	QLineEdit* typeEdit;
	QLineEdit* formatEdit;
	QLineEdit* identifierEdit;
	QLineEdit* sourceEdit;
	QLineEdit* languageEdit;
	QLineEdit* relationEdit;
	QLineEdit* coverageEdit;
	QLineEdit* rightsEdit;
	QLineEdit* titleEdit;
	QLineEdit* authorEdit;

	QMultiLineEdit* descriptionEdit;
	QMultiLineEdit* keywordsEdit;
	QMultiLineEdit* contributorsEdit;

	QLabel* authorLabel;
	QLabel* titleLabel;
	QLabel* publisherLabel;
	QLabel* dateLabel;
	QLabel* typeLabel;
	QLabel* formatLabel;
	QLabel* identifierLabel;
	QLabel* sourceLabel;
	QLabel* languageLabel;
	QLabel* relationLabel;
	QLabel* coverageLabel;
	QLabel* rightsLabel;
	QLabel* descriptionLabel;
	QLabel* keywordsLabel;
	QLabel* contributorsLabel;

	QGridLayout* layout1;
	QGridLayout* layout2;

	QWidget* page1;
	QWidget* page2;
};

#endif // DOCINFOS_H
