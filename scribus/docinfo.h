/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef DOCINFOS_H
#define DOCINFOS_H

#include <QTabWidget>

#include "scribusapi.h"
#include "documentinformation.h"

class QLineEdit;
class QTextEdit;
class QLabel;
class QWidget;
class QGridLayout;


/*! \brief The DocInfos class is now part of the ReformDoc multi widgets storage.
See reformdoc.h as reference. The return values are handled via. getDocInfo()
method. It can be used as standalone widget. */
class SCRIBUS_API DocInfos : public QTabWidget
{
    Q_OBJECT

public:
	DocInfos( QWidget* parent, DocumentInformation& docInfo );
	~DocInfos() {};

	/*! \brief Class wide DocumentInformation& docInfo reference. */
	DocumentInformation infos;

	/*! \brief Returns the changed values.
	Used e.g. in ScribusMainWindow::SetupDoc().
	\retval DocumentInformation structure */
	DocumentInformation getDocInfo();

	/*! \brief (Re)loads the document informations.
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
	QLineEdit* subjectEdit;

	QTextEdit* descriptionEdit;
	QTextEdit* keywordsEdit;
	QTextEdit* contributorsEdit;

	QLabel* authorLabel;
	QLabel* subjectLabel;
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
