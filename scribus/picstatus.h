#ifndef PICSTATUS_H
#define PICSTATUS_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <q3table.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <q3header.h>
#include <q3ptrlist.h>
#include <q3valuelist.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "scribusapi.h"
class ScribusDoc;
class ScribusView;
class FileSearch;

class SCRIBUS_API PicStatus : public QDialog
{ 
    Q_OBJECT

public:
    PicStatus(QWidget* parent, ScribusDoc *docu, ScribusView *viewi);
    ~PicStatus() {};

    Q3Table* PicTable;
    Q3Header *Header;
    QPushButton* CancelB;
    QPushButton* OkB;
    ScribusDoc *doc;
    ScribusView *view;
    int Zeilen;
    Q3PtrList<QCheckBox> FlagsPic;
    Q3ValueList<uint> ItemNrs;

public slots:
	void languageChange();

private slots:
    void GotoPic();
    void SearchPic();
    void SearchPicAborted(bool userCancelled);
    void SearchPicFinished(const QStringList & matches, const QString & fileName);
    void PrintPic();

protected:
    QVBoxLayout* PicStatusLayout;
    QHBoxLayout* Layout2;

	// Return the row index for the row containing the specified
	// filename. Behavour is undefined if the filename is not in the
	// table.
	int getRowByFileName(const QString& fileName);

	// Toggle a searc button in the table between "Search" and "Cancel Search",
	// fixing signal connections as well.
	void PicStatus::setSearchButton(int row, bool toCancel, const FileSearch* searcher);

	// Load the image specified into the PageItem shown on row `row'
	bool PicStatus::loadPictByRow(const QString & newFilePath, unsigned int row);

	// Column indexes
	static const unsigned short COL_FILENAME;
	static const unsigned short COL_PATH;
	static const unsigned short COL_PAGE;
	static const unsigned short COL_GOTO;
	static const unsigned short COL_PRINT;
	static const unsigned short COL_STATUS;
	static const unsigned short COL_SEARCH;

	// Common translated strings
	static QString trOK;
	static QString trMissing;
	static QString trSearch;
	static QString trCancelSearch;
	static QString trGoto;

signals:
	void selectPage(int);
	void selectMasterPage(QString);
};

#endif // PICSTATUS_H
