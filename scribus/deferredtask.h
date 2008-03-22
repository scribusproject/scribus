/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _DEFERREDTASK_H
#define _DEFERREDTASK_H

#include <QObject>
#include <QString>
#include "scribusapi.h"

class QTimer;

/*! \brief DeferredTask is an abstraction of a generally long-running operation
that is done in small steps under the control of the event loop. It
is an abstract class that provides a common interface for such tasks,
but cannot be used directly.

To use it, subclass it and add the private methods and members you need
to keep track of your task. FileSearch uses this interface and might
be worth looking at. You can change the constructor arguments around
as much as you like, but should maintain the rest of the interface.

When you instantiate a DeferredTask subclass, it should do any general setup
it requires but not begin processing. The start() slot will be called to
trigger the beginning of processing. next() will then be called repeatedly
by an idle timer to do small chunks of processing work. When the class
emits finished() or aborted(bool), the timer will stop and next() will not be
called again.

A DeferredTask is single use. It will be automatically deleted if its parent
is deleted (and it hasn't yet been deleted). Otherwise, delete it when
you're done with it. You should not delete it without cancelling it first.
*/
class SCRIBUS_API DeferredTask : public QObject
{
	Q_OBJECT

public:
	/*! \brief If you override the destructor, call DeferredTask::cleanup() at the
	end of your dtor. */
	virtual ~DeferredTask();

	/*! \brief Returns true iff processing has finished. Note that this will return false
	both while processing, and before start() has been called. */
	bool isFinished() const;

	/*! \brief Return a human-readable version of the last error message. Unless
	overridden, returns the value of m_lastError. Value is undefined
	until either finished() or aborted(bool) have been emitted. */
	virtual const QString& lastError() const;

public slots:

	/*! \brief Starts the timer and begins processing. If you override this you must
	call DeferredTask::start() at the end of your start() implementation.
	If something goes wrong, emit aborted(); */
	virtual void start();

	/*! \brief Run continuously without returning control to the event loop, until
	either aborted(bool) or finished() are emitted. Note that EVENTS ARE NOT
	PROCESSED, including deleteLater() and posted events. It is not
	recommended that you use this method. */
	void runUntilFinished();

	/*! \brief Abort the search and emit aborted(true). This is treated as an abort by
	the user. If you reimplement this you must call DeferredTask::cancel()
	at the end of your implementation. Do not emit aborted(bool) yourself
	from your cancel() implementation, let DeferredTask::cancel() do that. */
	virtual void cancel();

signals:
	/*! \brief Emitted if the processing work was aborted before completion. The signal
	argument is true if processing was aborted by direct or indirect user action,
	and false if it was aborted for any other reason (such as an internal error). */
	void aborted(bool);

	/*! \brief Emitted when processing is complete, from DeferredTask::done(). You can
	also emit your own custom signal if you want to convey more information,
	so long as you do so AFTER calling done(). */
	void finished();

	/*! \brief Emitted to inform anybody interested of the approximate level of
	progress made in the processing task. The argument is a positive integer
	percentage from 0 to 100 inclusive. Subclasses are not required to emit
	this signal, but it's a good idea if the user is likely to care about
	what your class does. */
	void progress(int);

protected slots:
	/*! \brief Do the next small chunk of processing, then return. You must implement this method
	to do whatever processing work you need to. It will be called repeatedly until it emits
	either aborted(bool) or finished() to indicate that it's done. */
	virtual void next() = 0;

protected:
	/*! \brief You must provide a new, public, constructor. It should set up the class
	but not begin processing. You must call DeferredTask::init() from your
	ctor, preferably early on. */
	DeferredTask(QObject* parent);

	//! \brief  Human readable, translated version of last error encountered.
	QString m_lastError;

	//! \brief  Do some setup of the DeferredTask. Must be called from all subclass ctors.
	void init();

	//! \brief  Do some cleanup before deletion. Must be called from all subclass dtors.
	void cleanup();

	/*! \brief Indicate that processing has finished. You must call this before
	emitting finished(), since it sets up some state that must happen before
	any slots connected to finished() are called, and the ordering of slot
	calls from a signal can not be guaranteed. */
	void done();

private:
	/*! \brief Status code indicating whether we're running, not yet started, failed,
	etc. */
	int m_status;

	//! \brief  The timer used to control next() calls.
	QTimer* m_timer;
};

#endif
