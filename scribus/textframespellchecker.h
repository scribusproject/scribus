/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TEXTFRAMESPELLCHECKER_H
#define TEXTFRAMESPELLCHECKER_H

#include <QObject>
#include <QTimer>
#include <QHash>
#include <QThread>
#include <QVector>
#include "text/storytextsnapshot.h"

class PageItem_TextFrame;
class SpellCheckerWorker;

/**
* @brief Represents a spelling error found in text
*/
struct SpellError
{
	int position;              // Position in the text
	int length;	               // Length of the error
	QString word;              // The misspelled word
	QString language;          // Language context
};

/**
* @brief Singleton spell checker with dedicated worker thread
*
* Manages spell checking for all text frames with explicit thread control.
* The spell checking runs in a dedicated background thread that can be
* started, stopped, paused, and resumed by the main program.
*
* Thread Safety:
* - All public methods must be called from the main/UI thread
* - Spell checking runs in dedicated worker thread
* - Communication via signals/slots (thread-safe)
* - Snapshots are immutable (safe to pass between threads)
*/
class TextFrameSpellChecker : public QObject
{
	Q_OBJECT

	public:
		/**
		* @brief Get the singleton instance
		*/
		static TextFrameSpellChecker* instance();

		/**
		* @brief Destroy the singleton instance
		* Automatically stops the worker thread
		*/
		static void destroy();

		// Prevent copying
		TextFrameSpellChecker(const TextFrameSpellChecker&) = delete;
		TextFrameSpellChecker& operator=(const TextFrameSpellChecker&) = delete;

		// ========================================================================
		// Thread Control
		// ========================================================================

		/**
		* @brief Start the spell checker worker thread
		* Must be called before spell checking can occur
		*/
		void startThread();

		/**
		* @brief Stop the spell checker worker thread
		* Waits for current check to complete, then stops the thread
		*/
		void stopThread();

		/**
		* @brief Pause spell checking
		* Pending checks are delayed until resumed
		*/
		void pauseChecking();

		/**
		* @brief Resume spell checking
		* Processes any pending checks
		*/
		void resumeChecking();

		/**
		* @brief Check if the worker thread is running
		*/
		bool isThreadRunning() const;

		/**
		* @brief Check if spell checking is paused
		*/
		bool isPaused() const { return m_paused; }

		/**
		* @brief Set thread priority
		* @param priority QThread::Priority (e.g., QThread::LowPriority)
		*/
		void setThreadPriority(QThread::Priority priority);

		// ========================================================================
		// Configuration
		// ========================================================================

		/**
		* @brief Enable or disable spell checking globally
		* When disabled, no new checks are started
		*/
		void setEnabled(bool enabled);
		bool isEnabled() const { return m_enabled; }

		/**
		* @brief Set the debounce delay for typing (milliseconds)
		*/
		void setDebounceDelay(int ms);
		int debounceDelay() const { return m_debounceDelay; }

		/**
		* @brief Set maximum number of cached results to keep
		*/
		void setMaxCachedFrames(int max);

		// ========================================================================
		// Frame Notifications (called by PageItem_TextFrame)
		// ========================================================================

		/**
		* @brief Notify that a frame has become active (user is editing it)
		*/
		void frameActivated(PageItem_TextFrame* frame);

		/**
		* @brief Notify that a frame has become inactive
		*/
		void frameDeactivated(PageItem_TextFrame* frame);

		/**
		* @brief Notify that text in a frame has changed
		*/
		void frameTextChanged(PageItem_TextFrame* frame);

		/**
		* @brief Notify that language settings changed (force recheck)
		*/
		void frameLanguageChanged(PageItem_TextFrame* frame);

		/**
		* @brief Notify that a frame is being deleted
		*/
		void frameDeleted(PageItem_TextFrame* frame);

		/**
		* @brief Force immediate check of a frame
		*/
		void checkFrameNow(PageItem_TextFrame* frame);



		// ========================================================================
		// Query Methods
		// ========================================================================

		/**
		* @brief Get cached spell check results for a frame
		* Returns empty vector if no results available
		*/
		QVector<SpellError> getErrors(PageItem_TextFrame* frame) const;

		/**
		* @brief Check if a frame has cached results
		*/
		bool hasResults(PageItem_TextFrame* frame) const;

		/**
		* @brief Check if a frame is currently being checked
		*/
		bool isCheckingFrame(PageItem_TextFrame* frame) const;

		/**
		* @brief Cancel all pending checks
		*/
		void cancelAll();

		/**
		* @brief Clear all cached results
		*/
		void clearCache();

	signals:
		// ========================================================================
		// Signals to UI
		// ========================================================================

		/**
		* @brief Emitted when spell check results are ready for a frame
		*/
		void resultsReady(PageItem_TextFrame* frame, const QVector<SpellError>& errors);

		/**
		* @brief Emitted when checking starts for a frame
		*/
		void checkStarted(PageItem_TextFrame* frame);

		/**
		* @brief Emitted when checking is cancelled
		*/
		void checkCancelled(PageItem_TextFrame* frame);

		/**
		* @brief Emitted when worker thread starts
		*/
		void threadStarted();

		/**
		* @brief Emitted when worker thread stops
		*/
		void threadStopped();

		// ========================================================================
		// Signals to Worker Thread
		// ========================================================================

		/**
		* @brief Signal to worker to check a snapshot
		* @internal Connected to worker's checkSnapshot slot
		*/
		void requestCheck(PageItem_TextFrame* frame, StoryTextSnapshot snapshot);

	private:
		// Private constructor for singleton
		explicit TextFrameSpellChecker(QObject* parent = nullptr);
		~TextFrameSpellChecker() override;

		// Frame state tracking
		struct FrameState
		{
			bool needsRecheck = true;
			QVector<SpellError> cachedErrors;
			QString textHash; // Hash of text when last checked
		};

		// Singleton instance
		static TextFrameSpellChecker* m_instance;

		// Get or create state for a frame
		FrameState& getFrameState(PageItem_TextFrame* frame);

		// Calculate hash of frame's text
		QString calculateTextHash(PageItem_TextFrame* frame) const;

		// Schedule a check (with debouncing)
		void scheduleCheck(PageItem_TextFrame* frame);

		// Perform the actual check
		void performCheck(PageItem_TextFrame* frame);

		// Cleanup state for a frame
		void removeFrameState(PageItem_TextFrame* frame);

	private slots:
		void onDebounceTimeout();
		void onCheckComplete(PageItem_TextFrame* frame, const QVector<SpellError>& errors);

	private:
		// Configuration
		bool m_enabled {true};
		bool m_paused {false};
		int m_debounceDelay {500}; // milliseconds
		int m_maxCachedFrames {10};

		// Thread management
		QThread* m_workerThread {nullptr};
		SpellCheckerWorker* m_worker {nullptr};

		// Currently active frame (only one can be edited at a time)
		PageItem_TextFrame* m_activeFrame {nullptr};

		// Current snapshot (only for active frame while preparing to check)
		StoryTextSnapshot m_currentSnapshot;

		// Frame states (cached results, flags)
		QHash<PageItem_TextFrame*, FrameState> m_frameStates;

		// Debounce timer for the active frame
		QTimer m_debounceTimer;

		// Currently checking frame
		PageItem_TextFrame* m_checkingFrame {nullptr};
};

// ============================================================================
// Worker Class - Lives in Dedicated Thread
// ============================================================================

/**
* @brief Worker object that performs spell checking
*
* This object lives in the worker thread and has its own event loop.
* It receives check requests via signals and emits results back.
*/
class SpellCheckerWorker : public QObject
{
	Q_OBJECT

	public:
		explicit SpellCheckerWorker(QObject* parent = nullptr);
		~SpellCheckerWorker() override;

		/**
		* @brief Pause processing
		* Queued checks will wait until resumed
		*/
		void setPaused(bool paused);

	public slots:
		/**
		* @brief Check a snapshot for spelling errors
		* This runs in the worker thread
		*/
		void checkSnapshot(PageItem_TextFrame* frame, const StoryTextSnapshot& snapshot);

	signals:
		/**
		* @brief Emitted when checking is complete
		* Connected back to main thread
		*/
		void checkComplete(PageItem_TextFrame* frame, const QVector<SpellError>& errors);

		/**
		* @brief Emitted to report progress (optional)
		*/
		void progress(int percentage);

	private:
		bool m_paused {false};
};

#endif // TEXTFRAMESPELLCHECKER_H
