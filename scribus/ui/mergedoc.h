/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**
 * "page > import" and, "import master pages" from the "master pages" window.
 */
#ifndef MERGEDOC_H
#define MERGEDOC_H

#include <QDialog>
#include "scribusapi.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QSpinBox;
class QString;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;


class SCRIBUS_API MergeDoc : public QDialog
{
	Q_OBJECT

public:
	MergeDoc( QWidget* parent, bool importMasterPages, int targetDocPageCount = -1, int currentPage = 1 );
	~MergeDoc();

	const QString getFromDoc();
	const QStringList getMasterPageNames() const;
	QList<int> getMasterPageIndexes() const;
	const int getImportWhere();
	const int getImportWherePage();
	const bool getCreatePageChecked();
	const QString getPageNumbers();
	const int getPageCounter();

private:
	QLabel* fromDocLabel;
	QLabel* importPageLabel;
	QLabel* fromLabel;
	QLineEdit* pageNumberData;
	QLineEdit* fromDocData;
	QPushButton* importButton;
	QPushButton* cancelButton;
	QPushButton* changeButton;
	QCheckBox* createPageData;
	QListWidget* masterPageNameData;
	QComboBox* importWhereData;
	QSpinBox* importWherePageData;
	QVBoxLayout* dialogLayout;
	QGridLayout* fromInfoLayout;
	QHBoxLayout* importCancelLayout;
	int count;
	bool masterPages;

private slots:
	void changeFile();
	void checkDestPageStatus( int positionComboSelection );
	void enableCreateWidgets();

};

#endif // MERGEDOC_H
