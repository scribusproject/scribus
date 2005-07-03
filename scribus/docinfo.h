/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef DOCINFOS_H
#define DOCINFOS_H

#include <qtabdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmultilineedit.h>
#include "scribusdoc.h"

class DocInfos : public QTabDialog
{
    Q_OBJECT

public:
	DocInfos( QWidget* parent, ScribusDoc* doc );
	~DocInfos() {};

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
	QMultiLineEdit* contributorsEdit;

	QLineEdit* titleEdit;
	QLineEdit* authorEdit;
	QMultiLineEdit* descriptionEdit;
	QMultiLineEdit* keywordsEdit;

protected:
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
