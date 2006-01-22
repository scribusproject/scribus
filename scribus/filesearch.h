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
#include <qstringlist.h>
#include <qvaluestack.h>
#include <qdir.h>

class QTimer;

// A class to do a depth-first search for a file in a directory tree
// efficiently and safely (I hope). The class is fire and forget,
// letting you get on with other things and take action when you're informed
// that the search is complete.
// A FileSearch is single use.
class SCRIBUS_API FileSearch : public DeferredTask
{
	Q_OBJECT

public:
	// Construct a new FileSearch object to search for the specified filename.
	// fileName should be a basename without path. Once you've created the
	// FileSearch, connect yourself to its searchComplete(int) signal then
	// start() it and return to the event loop. If searchBase is not specified,
	// a search of the user's home directory is done. The caller is expected to
	// ensure that the search base exists.
	// depthLimit is -1 for no limit, otherwise number of levels deep to search.
	// The base dir is level 0.
	FileSearch(QObject* parent, const QString & fileName, const QString & searchBase = QString::null, int depthLimit = -1);

	~FileSearch();

public slots:
	// Begin searching.
	virtual void start();

	// Return a list of files matched. Note that it is safe to call this
	// while the search is still running, or after it has failed.
	const QStringList & matchingFiles() const;

	// Return the number of files found so far.
	int foundCount() const;

	// Return the name we're searching for
	const QString & fileName() const;

	// Return a const reference to the QDir we're using to
	// track our progress. This lets the caller get the dirname,
	// absolute dirname, etc.
	const QDir & currentDir() const;

signals:
	// Emitted when the search has finished.
	// param 1 is list of paths matched,
	// param 2 is filename searched for.
	// Remember you can simply discard one or both params.
	void searchComplete(const QStringList&, const QString&);

protected slots:
	virtual void next();

protected:
	// Push a list of subdirs in the current directory onto m_tree, and
	// put an iterator pointing to the first subdir on top of m_iter.
	void pushStack();

	// Scans the current directory (where QDir is set to) and adds its contents
	// to the list of matched files.
	void addCurrentDirFiles();

	// Where the search starts from
	QString m_searchBase;

	// What the filename we're looking for is
	QString m_fileName;

	// The list of files matched in the search
	QStringList m_matchingFiles;

	// This stack holds a list of directories on the tree, from the base
	// to the level we're currently searching. We use iterators into the values
	// of this stack to keep the search position.
	QValueStack<QStringList> m_tree;

	// A matching stack of iterators into the lists in m_tree. We use this stack
	// to iterate directory by directory as we search.
	QValueStack<QStringList::const_iterator> m_iter;

	// A QDir set to the current directory, used for listing files and
	// directories.
	QDir m_dir;

	// Conveniently keep track of how deeply we've recursed.
	int m_depth;

	// Maximum depth to search to
	int m_maxdepth;
};

#endif
