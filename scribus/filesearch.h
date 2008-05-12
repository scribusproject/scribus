/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _FILESEARCH_H
#define _FILESEARCH_H

#include "scribusapi.h"
#include "deferredtask.h"
#include <QStringList>
#include <QStack>
#include <QDir>

class QTimer;

/*! \brief A class to do a depth-first search for a file in a directory tree
efficiently and safely (I hope). The class is fire and forget,
letting you get on with other things and take action when you're informed
that the search is complete.
A FileSearch is single use.
*/
class SCRIBUS_API FileSearch : public DeferredTask
{
	Q_OBJECT

public:
	/*! \brief Construct a new FileSearch object to search for the specified filename.
	\param parent Parent object (where it will be displayed)
	\param fileName should be a basename without path. Once you've created the
	FileSearch, connect yourself to its searchComplete(int) signal then
	start() it and return to the event loop.
	\param searchBase If searchBase is not specified,
	a search of the user's home directory is done. The caller is expected to
	ensure that the search base exists.
	\param depthLimit is -1 for no limit, otherwise number of levels deep to search.
	The base dir is level 0.
	\param caseSensitive it will search case INSENSITIVE filenames if false.
	\warning It's useless to set caseSensitive on MS Windows! This will be set to
	false on windows automatically because Windows are case insensitive already.
	*/
	FileSearch(QObject* parent, const QString & fileName, const QString & searchBase = QString::null, int depthLimit = -1, bool caseSensitive=true);

	~FileSearch();

public slots:
	//! \brief  Begin searching.
	virtual void start();

	/*! \brief Return a list of files matched. Note that it is safe to call this
	while the search is still running, or after it has failed.
	*/
	const QStringList & matchingFiles() const;

	//! \brief  Return the number of files found so far.
	int foundCount() const;

	//! \brief  Return the name we're searching for
	const QString & fileName() const;

	/*! \brief Return a const reference to the QDir we're using to
	track our progress. This lets the caller get the dirname,
	absolute dirname, etc.
	*/
	const QDir & currentDir() const;

signals:
	/*! \brief Emitted when the search has finished.
	\param paths is list of paths matched,
	\param filename is filename searched for.
	Remember you can simply discard one or both params.
	*/
	void searchComplete(const QStringList& paths, const QString& filename);

protected slots:
	virtual void next();

protected:
	/*! \brief Push a list of subdirs in the current directory onto m_tree, and
	put an iterator pointing to the first subdir on top of m_iter. */
	void pushStack();

	/*! \brief Scans the current directory (where QDir is set to) and adds its contents
	to the list of matched files. */
	void addCurrentDirFiles();

	//! \brief Case sensitive flag
	bool m_caseSensitive;

	//! \brief  Where the search starts from
	QString m_searchBase;

	//! \brief  What the filename we're looking for is
	QString m_fileName;

	//! \brief  The list of files matched in the search
	QStringList m_matchingFiles;

	/*! \brief This stack holds a list of directories on the tree, from the base
	to the level we're currently searching. We use iterators into the values
	of this stack to keep the search position.
	*/
	QStack<QStringList> m_tree;

	/*! \brief A matching stack of iterators into the lists in m_tree. We use this stack
	to iterate directory by directory as we search.
	*/
	QStack<QStringList::const_iterator> m_iter;

	/*! \brief A QDir set to the current directory, used for listing files and
	directories. */
	QDir m_dir;

	//! \brief Conveniently keep track of how deeply we've recursed.
	int m_depth;

	//! \brief Maximum depth to search to
	int m_maxdepth;
};

#endif
