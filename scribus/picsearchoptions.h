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
	/**
	 * \brief Fetch the last used searchBase.
	 * @return Path string.
	 */
	const QString &getLastDirSearched () { return m_strLastDirSearched; }
 
	/**
	 * \brief Fetch the filename used for the last search.
	 * @return Filename string.
	 */
	const QString &getFileName () { return m_strFileName; }

	/**
	 * \brief Fetch all found matches.
	 * @return List of file paths.
	 */
	const QStringList &getMatches () { return m_listMatches; }

	/**
	 * \brief Did the user cancel the search?
	 * @return <code>bool</code> TRUE if cancelled.
	 */
	bool getCancelled () { return m_bCancelled; }

private slots:
	void slotChangeSearchDir();
	void slotSearchPic();
	void slotSearchPicAborted(bool userCancelled);
	void slotSearchPicFinished(const QStringList & matches, const QString & fileName);

private:
	QString 		m_strFileName;
	QString 		m_strLastDirSearched;
	QStringList 	m_listMatches;
	bool 		m_bCancelled;

protected:
	/**
	 * \brief Toggle a search button in the table between "Search" and "Cancel Search",
	 * fixing signal connections as well.
	 * \param toCancel true in the "cancelation" state/process
	 * \param searcher a reference to the searcher object 
	 */
	void setSearchButton(bool toCancel, const FileSearch* searcher);
	/**
	 * \brief Enable/disable gui elements while searching.
	 * @param enable Set to FALSE if searching.
	 */
	void enableGuiWhileSearching (bool enable);
};

#endif
