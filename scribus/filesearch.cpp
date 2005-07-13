#include <filesearch.h>
#include <qtimer.h>

#include "filesearch.moc"

enum FileSearchStatus
{
	Status_NotStarted,
	Status_Running,
	Status_Cancelled,
	Status_Failed,
	Status_Finished,
};

FileSearch::FileSearch(QObject* parent, const QString & fileName, const QString & searchBase, int depthLimit) :
	QObject(parent),
	m_searchBase(searchBase.isNull() ? QDir::homeDirPath() : searchBase),
	m_fileName(fileName),
	m_depth(0),
	m_maxdepth(depthLimit),
	m_status(Status_NotStarted),
	m_timer(0)
{
	m_dir.setPath(m_searchBase);
	Q_ASSERT(m_dir.exists());
	// Give ourselves a useful name for object browsers, etc.
	setName(QString("FileSearch for \"%1\"").arg(m_searchBase).local8Bit());
	// And build the timer we'll use to access the event loop
	m_timer = new QTimer(this, "FileSearch timer");
	Q_CHECK_PTR(m_timer);
	connect(m_timer, SIGNAL(timeout()), SLOT(next()) );
}

FileSearch::~FileSearch()
{
	// We're being destroyed while the search is running.
	if (m_status == Status_Running)
	{
		m_status = Status_Failed;
		// Report a failure. This error generally indicates a program bug,
		// non-translatable because it should never be user visible.
		m_failReason = "Searcher deleted";
		emit searchAborted(false);
	}
	// Our timer, if we have one, is automatically destroyed when
	// we are so don't worry about it.
}

bool FileSearch::finished() const
{
	return m_status == Status_Finished;
}

const QStringList & FileSearch::matchingFiles() const
{
	return m_matchingFiles;
}

int FileSearch::foundCount() const
{
	return m_matchingFiles.count();
}

const QString & FileSearch::failReason() const
{
	// Should only be called when something has in fact failed
	// DO NOT use this method to check for failure!
	Q_ASSERT(!m_failReason.isEmpty());
	return m_failReason;
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
	m_status = Status_Running;
	// Push the list of subdirs for the starting dir onto the stack
	pushStack();
	// and add the current directory's files to the list
	addCurrentDirFiles();
	// Start the timer to do our search in the event loop's idle time
	m_timer->start(0, false);
}

void FileSearch::cancel()
{
	m_status = Status_Cancelled;
	if (m_timer)
		m_timer->stop();
	// Cancelled by request
	m_failReason = tr("Search cancelled");
	emit searchAborted(true);
}

void FileSearch::next()
{
	// We start off in a directory that has just been examined.
	// A list of the names of directories in this directory is at the top of
	// m_tree, and m_iter points to the first directory name in that list. The
	// files in this directory have already been checked to see if they match,
	// and added to the list of matches if they do.
	// We need to select the directory to step into, and search its contents.

	Q_ASSERT(m_tree.count() > 0);
	Q_ASSERT(m_tree.count() == m_iter.count());
	Q_ASSERT(m_tree.count() == m_depth + 1);

	// skip '.', '..'
	while ( *(m_iter.top()) == "." || *(m_iter.top()) == ".." )
		++m_iter.top();

	if ( (m_iter.top() == m_tree.top().end()) || (m_depth == m_maxdepth) )
	{
		// We're at the end of the list of subdirectory names in this directory,
		// or we've hit the maximum depth we're allowed to search to.
		// Move up to the previous directory.
		m_iter.pop();
		m_tree.pop();
		m_dir.cdUp();
		m_depth--;
		// Sanity check - if we've run out of tree to search, depth should be -1
		// and the stack should be empty. Otherwise, depth should be >= 0 and
		// the stack should be non-empty.
		Q_ASSERT( (m_depth == -1) == ( m_iter.count() == 0 ) );
		// Check if we've run out of tree and should finish up
		if (m_depth < 0)
		{
			// We've run out of tree, so we're all done. Kill the timer and
			// tell our owner we've finished.
			m_status = Status_Finished;
			m_timer->stop();
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
		QString subdir = *(m_iter.top());
		++m_iter.top();
		Q_ASSERT(!subdir.isEmpty());
		
		m_dir.cd(subdir);
		m_depth++;
		pushStack();
		addCurrentDirFiles();
	}
}

void FileSearch::pushStack()
{
	m_tree.push(m_dir.entryList(QDir::Dirs));
	m_iter.push(m_tree.top().begin());
}

void FileSearch::addCurrentDirFiles()
{
	// Search files in this dir
	const QFileInfoList *filist = m_dir.entryInfoList(m_fileName, QDir::Files);
	QFileInfoListIterator it( *filist );
	QFileInfo *fi;
	while ( ( fi = it.current() ) != 0 )
	{
		++it;
		m_matchingFiles.push_back(fi->absFilePath());
	}
}
