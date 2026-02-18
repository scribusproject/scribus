/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STORYTEXTSNAPSHOT_H
#define STORYTEXTSNAPSHOT_H

#include <QString>
#include <QVector>
#include "scribusapi.h"

class StoryText;

/**
 * @brief Represents a language run - a continuous span of text in the same language
 *
 * Multiple language runs can exist within a single paragraph or sentence.
 */
struct SCRIBUS_API LanguageRun
{
	int start {0};        ///< Start position in the plain text
	int length {0};       ///< Length of this language run
	QString language; ///< Language code (e.g., "en_US", "de_DE", "fr_FR")

	LanguageRun() = default;
	LanguageRun(int s, int l, const QString& lang)
		: start(s), length(l), language(lang) {}

	/// Get the end position (exclusive) of this run
	int end() const { return start + length; }

	/// Check if a position falls within this language run
	bool contains(int pos) const { return pos >= start && pos < end(); }
};

/**
 * @brief Represents a paragraph boundary in the text
 */
struct SCRIBUS_API ParagraphInfo
{
	int start {0};  ///< Start position in the plain text
	int length {0}; ///< Length of the paragraph (including PARSEP if present)

	ParagraphInfo() = default;
	ParagraphInfo(int s, int l) : start(s), length(l) {}

	/// Get the end position (exclusive) of this paragraph
	int end() const { return start + length; }

	/// Check if a position falls within this paragraph
	bool contains(int pos) const { return pos >= start && pos < end(); }
};

/**
 * @brief Thread-safe, immutable snapshot of StoryText for spell/grammar checking
 *
 * This class provides a read-only copy of text content, paragraph boundaries,
 * and language information that can be safely used in worker threads without
 * any synchronization with the main UI thread.
 *
 * Key features:
 * - Immutable after creation (thread-safe by design)
 * - Supports multiple languages within a single paragraph/sentence
 * - Lightweight - contains only data needed for linguistic analysis
 * - Can be safely copied and passed to worker threads
 *
 * @note Internal paragraph separators (PARSEP) are converted to newlines (\n)
 *       in the plainText for better compatibility with text analysis tools.
 */
class SCRIBUS_API StoryTextSnapshot
{
	public:
		/// Plain text content with PARSEP converted to \n
		QString plainText;

		/// Paragraph boundaries
		QVector<ParagraphInfo> paragraphs;

		/// Language runs - multiple runs can exist per paragraph
		QVector<LanguageRun> languages;

		/// Default constructor creates an empty snapshot
		StoryTextSnapshot() = default;

		/**
		 * @brief Create a snapshot from a StoryText object
		 *
		 * This static factory method extracts all necessary information from
		 * the StoryText in a single pass, creating an independent copy that
		 * can be safely used in other threads.
		 *
		 * @param story The StoryText to snapshot
		 * @return A complete, thread-safe snapshot
		 */
		static StoryTextSnapshot create(const StoryText& story);

		/**
		 * @brief Get the text of a specific paragraph
		 *
		 * @param paraIndex Index of the paragraph (0-based)
		 * @return The paragraph text, or empty string if index is invalid
		 */
		QString getParagraphText(int paraIndex) const;

		/**
		 * @brief Get the language code for a specific position
		 *
		 * @param pos Position in the plain text
		 * @return Language code at that position, or empty string if not found
		 */
		QString getLanguageAt(int pos) const;

		/**
		 * @brief Get all language runs that intersect with a paragraph
		 *
		 * This is useful for spell checkers that need to process a paragraph
		 * in chunks based on language.
		 *
		 * @param paraIndex Index of the paragraph
		 * @return Vector of language runs within this paragraph
		 */
		QVector<LanguageRun> getLanguageRunsForParagraph(int paraIndex) const;

		/**
		 * @brief Get all language runs that intersect with a text range
		 *
		 * @param start Start position (inclusive)
		 * @param end End position (exclusive)
		 * @return Vector of language runs within this range
		 */
		QVector<LanguageRun> getLanguageRunsInRange(int start, int end) const;

		/**
		 * @brief Get the paragraph index for a specific position
		 *
		 * @param pos Position in the plain text
		 * @return Paragraph index, or -1 if position is invalid
		 */
		int getParagraphIndexAt(int pos) const;

		/**
		 * @brief Check if the snapshot is empty
		 *
		 * @return true if there is no text content
		 */
		bool isEmpty() const { return plainText.isEmpty(); }

		/**
		 * @brief Get the total number of paragraphs
		 *
		 * @return Number of paragraphs in the snapshot
		 */
		int paragraphCount() const { return paragraphs.size(); }

		/**
		 * @brief Get the total number of language runs
		 *
		 * @return Number of language runs in the snapshot
		 */
		int languageRunCount() const { return languages.size(); }
};

#endif // STORYTEXTSNAPSHOT_H
