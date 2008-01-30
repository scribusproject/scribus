/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PICSEARCHOPTIONS_H
#define PICSEARCHOPTIONS_H

#include "scribusapi.h"
#include "ui_picsearchoptions.h"

class FileSearch;

class SCRIBUS_API PicSearchOptions : public QDialog, Ui::PicSearchOptions
{ 
	Q_OBJECT

public:
	PicSearchOptions(QWidget* parent, const QString & fileName,const QString & searchBase);
	~PicSearchOptions() {};
	QString dirToSearch;
	QString m_fileName;
	QStringList m_matches;
	bool m_userCancelled;

private slots:
	void changeSearchDir();
	void SearchPic();
	void SearchPicAborted(bool userCancelled);
	void SearchPicFinished(const QStringList & matches, const QString & fileName);


protected:
	/*! \brief Toggle a search button in the table between "Search" and "Cancel Search",
	 fixing signal connections as well.
	\param toCancel true in the "cancelation" state/process
	\param searcher a reference to the searcher object */
	void setSearchButton(bool toCancel, const FileSearch* searcher);
};

#endif
