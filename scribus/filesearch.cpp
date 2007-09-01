/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "filesearch.h"
//#include "filesearch.moc"
#include <QTimer>
#include <QRegExp>


enum FileSearchStatus
{
	Status_NotStarted,
	Status_Running,
	Status_Cancelled,
	Status_Failed,
	Status_Finished,
};


FileSearch::FileSearch(QObject* parent, const QString & fileName,const QString & searchBase, int depthLimit, bool caseSensitive) :
	DeferredTask(parent),
	m_searchBase(searchBase.isNull() ? QDir::homePath() : searchBase),
	m_fileName(fileName),
	m_depth(0),
	m_maxdepth(depthLimit)
{
	m_caseSensitive = caseSensitive;
#ifdef _WIN32
	// it has no meaning to set case sensitiveness on win
	m_caseSensitive = false;
#endif
	DeferredTask::init();
	m_dir.setPath(m_searchBase);
	Q_ASSERT(m_dir.exists());
	// Give ourselves a useful name for object browsers, etc.
	setObjectName(QString("FileSearch for \"%1\"").arg(m_searchBase).toLocal8Bit());
}

FileSearch::~FileSearch()
{
	DeferredTask::cleanup();
}

const QStringList & FileSearch::matchingFiles() const
{
	return m_matchingFiles;
}

int FileSearch::foundCount() const
{
	return m_matchingFiles.count();
}

const QString & FileSearch::fileName() const
{
	return m_fileName;
}

const QDir & FileSearch::currentDir() const
{
	return m_dir;
}

void FileSearch::start()
{
	// Push the list of subdirs for the starting dir onto the stack
	pushStack();
	// and add the current directory's files to the list
	addCurrentDirFiles();
	DeferredTask::start();
}

void FileSearch::next()
{
	// We start off in a directory that has just been examined.
	// A list of the names of directories in this directory is at the top of
	// m_tree, and m_iter points to the first directory name in that list. The
	// files in this directory have already been checked to see if they match,
	// and added to the list of matches if they do.
	// We need to select the directory to step into, and search its contents.
	// skip '.', '..'
// Qt-4
// this crashes under Qt-4, don't know why FS
//	while ( *(m_iter.top()) == "." || *(m_iter.top()) == ".." )
//		++m_iter.top();

	if ( (m_iter.top() == m_tree.top().end()) || (m_depth == m_maxdepth) )
	{
		// We're at the end of the list of subdirectory names in this directory,
		// or we've hit the maximum depth we're allowed to search to.
		// Move up to the previous directory.
		m_iter.pop();
		m_tree.pop();
		m_dir.cdUp();
		m_depth--;
		// Check if we've run out of tree and should finish up
		if (m_depth < 0)
		{
			// We've run out of tree, so we're all done. Kill the timer and
			// tell our owner we've finished.
			Q_ASSERT(m_iter.count() == 0);
			Q_ASSERT(m_tree.count() == 0);
			DeferredTask::done();
			emit searchComplete(m_matchingFiles, m_fileName);
		}
		else
		{
			// As an optimisation, call next() to do a short recursion back to the
			// next subdir we actually need to search, so we don't have to wait for
			// the timer to get around to firing for this trivial step. We're not
			// putting anything on the stack so this should be pretty safe.
			next();
		}
	}
	else
	{
		// There are still subdirectories to search. Select the next one and step
		// into it, incrementing the iterator for the current dir in the process.
		m_dir.cd(*(m_iter.top()));
		++m_iter.top();
		m_depth++;
		pushStack();
		addCurrentDirFiles();
	}
}

void FileSearch::pushStack()
{
	m_tree.push(m_dir.entryList(QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot));
	m_iter.push(m_tree.top().begin());
}

void FileSearch::addCurrentDirFiles()
{
	QStringList flist;
	m_caseSensitive ? flist.append(m_fileName) : flist.append("*");
	QFileInfoList filist = m_dir.entryInfoList(flist, QDir::Files);
	QListIterator<QFileInfo> it( filist );
	QFileInfo fi;
	// Search files in this dir
	if (m_caseSensitive)
	{
		while (it.hasNext())
		{
			fi = it.next();
			m_matchingFiles.push_back(fi.absoluteFilePath());
		}
// Qt4
/*		while ( ( fi = it.current() ) != 0 )
		{
			++it;
			m_matchingFiles.push_back(fi->absFilePath());
		} */
	}
	else
	{
		// unix only, resp. no meaning in windows
		QRegExp r(m_fileName, Qt::CaseInsensitive, QRegExp::Wildcard);
		while (it.hasNext())
		{
			fi = it.next();
			if (r.exactMatch(fi.fileName()))
				m_matchingFiles.push_back(fi.absoluteFilePath());
		}
// Qt4
// 		while ( ( fi = it.current() ) != 0 )
// 		{
// 			++it;
// 			if (r.exactMatch(fi->fileName()))
// 				m_matchingFiles.push_back(fi->absFilePath());
// 		}
	}
}
