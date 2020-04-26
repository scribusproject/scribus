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

	QString getFromDoc() const;
	QStringList getMasterPageNames() const;
	QList<int> getMasterPageIndexes() const;
	int getImportWhere() const;
	int getImportWherePage() const;
	bool getCreatePageChecked() const;
	QString getPageNumbers() const;
	int getPageCounter() const;

private:
	QLabel* fromDocLabel {nullptr};
	QLabel* importPageLabel {nullptr};
	QLabel* fromLabel {nullptr};
	QLineEdit* pageNumberData {nullptr};
	QLineEdit* fromDocData {nullptr};
	QPushButton* importButton {nullptr};
	QPushButton* cancelButton {nullptr};
	QPushButton* changeButton {nullptr};
	QCheckBox* createPageData {nullptr};
	QListWidget* masterPageNameData {nullptr};
	QComboBox* importWhereData {nullptr};
	QSpinBox* importWherePageData {nullptr};
	QVBoxLayout* dialogLayout {nullptr};
	QGridLayout* fromInfoLayout {nullptr};
	QHBoxLayout* importCancelLayout {nullptr};
	int count {0};
	bool masterPages {false};

private slots:
	void changeFile();
	void checkDestPageStatus( int positionComboSelection );
	void enableCreateWidgets();

};

#endif // MERGEDOC_H
