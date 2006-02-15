/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PICSTATUS_H
#define PICSTATUS_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qptrlist.h>
#include <qvaluelist.h>

#include "scribusapi.h"
class ScribusDoc;
class ScribusView;
class FileSearch;


class SCRIBUS_API PicStatus : public QDialog
{ 
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\brief Constructs a Dialog, which list all Images in the current Document. In this Dialog it is possible
		to search for missing Images.
	\param parent Pointer to parent window
	\param docu Pointer to the current Document
	\param viewi Pointer to the current View
	 */
	PicStatus(QWidget* parent, ScribusDoc *docu, ScribusView *viewi);
	~PicStatus() {};

public slots:
	void languageChange();

private slots:
	/*!
	\author Franz Schmid
	\brief Moves to the Page containing the selected Image.
	 */
	void GotoPic();
	/*!
	\author Franz Schmid
	\brief Searches for the given Picture. Displays a Dialog when more than one Picture is found.
	*/
	void SearchPic();
	void SearchPicAborted(bool userCancelled);
	void SearchPicFinished(const QStringList & matches, const QString & fileName);
	/*!
	\author Franz Schmid
	\brief Enables or disables printing of the selected Image.
	*/
	void PrintPic();

protected:
	QVBoxLayout* PicStatusLayout;
	QHBoxLayout* Layout2;

	/*! \brief Return the row index for the row containing the specified
		filename. Behavour is undefined if the filename is not in the
		table.
	\param fileName a search string
	\retval int table row
	*/
	int getRowByFileName(const QString& fileName);

	/*! \brief Toggle a search button in the table between "Search" and "Cancel Search",
	 fixing signal connections as well.
	\param row table row
	\param toCancel true in the "cancelation" state/process
	\param searcher a reference to the searcher object */
	void setSearchButton(int row, bool toCancel, const FileSearch* searcher);

	/*! \brief Load the image specified into the PageItem shown on row `row'
	\param newFilePath a file path
	\param row row number */
	bool loadPictByRow(const QString & newFilePath, unsigned int row);

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

private:
	QTable* PicTable;
	QHeader *Header;
	QPushButton* CancelB;
	QPushButton* OkB;
	ScribusDoc *doc;
	ScribusView *view;
	int Zeilen;
	QPtrList<QCheckBox> FlagsPic;
	QValueList<uint> ItemNrs;
};

#endif // PICSTATUS_H
