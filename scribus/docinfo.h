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

#include <qtabdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmultilineedit.h>

#include "documentinformation.h"

class DocInfos : public QTabDialog
{
    Q_OBJECT

public:
	DocInfos( QWidget* parent, DocumentInformation& docInfo );
	~DocInfos() {};
	
	DocumentInformation getDocInfo();

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
