#include "deferredtask.h"
#include "qtimer.h"

#include "deferredtask.moc"

enum FileSearchStatus
{
	Status_NotStarted,
	Status_Running,
	Status_Cancelled,
	Status_Failed,
	Status_Finished,
};

DeferredTask::DeferredTask(QObject* parent) : QObject(parent)
{
	init();
}

DeferredTask::~DeferredTask()
{
	cleanup();
}

void DeferredTask::init()
{
	m_status = Status_NotStarted,
	m_lastError = QString::null;
	// Build the timer we'll use to access the event loop
	m_timer = new QTimer(this, "DeferredTask timer");
	Q_CHECK_PTR(m_timer);
	// and hook up to it.
	connect(m_timer, SIGNAL(timeout()), SLOT(next()) );
}

void DeferredTask::cleanup()
{
	// We're being destroyed while the search is running.
	if (m_status == Status_Running)
	{
		m_status = Status_Failed;
		// Report a failure. This error generally indicates a program bug,
		// non-translatable because it should never be user visible.
		m_lastError = "DeferredTask unexpectedly deleted";
		emit aborted(false);
	}
	// Our timer, if we have one, is automatically destroyed when
	// we are so don't worry about it.
}

bool DeferredTask::isFinished() const
{
	return m_status == Status_Finished;
}

const QString& DeferredTask::lastError() const
{
	Q_ASSERT(m_status == Status_Cancelled || m_status == Status_Failed || m_status == Status_Finished);
	Q_ASSERT(!m_lastError.isNull());
	return m_lastError;
}

void DeferredTask::start()
{
	m_status = Status_Running;
	// Start the timer to do our search in the event loop's idle time
	m_timer->start(0, false);
}

void DeferredTask::cancel()
{
	Q_ASSERT(m_status == Status_Running);
	m_status = Status_Cancelled;
	m_timer->stop();
	// Cancelled by request
	m_lastError = tr("Cancelled by user");
	emit aborted(true);
}

void DeferredTask::done()
{
	Q_ASSERT(m_status == Status_Running);
	m_status = Status_Finished;
	m_timer->stop();
	emit finished();
}

void DeferredTask::runUntilFinished()
{
	Q_ASSERT(m_status == Status_Running);
	while (m_status == Status_Running)
		next();
}
