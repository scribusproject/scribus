/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Spell Checking Functions
 * 
 * This header declares the actual spell checking implementation functions
 * that are called by the SpellCheckerWorker thread.
 */

#ifndef SPELLCHECKFUNCTIONS_H
#define SPELLCHECKFUNCTIONS_H

#include <QVector>
#include "text/storytextsnapshot.h"

// Forward declarations
struct SpellError;

/**
 * @brief Perform spell checking on a text snapshot
 * 
 * This function processes a snapshot and returns all spelling errors found.
 * It handles multiple languages within the text by checking each language
 * run with the appropriate dictionary.
 * 
 * This function is designed to be called from a worker thread and does NOT
 * access any UI elements or mutable shared state. It only uses the immutable
 * snapshot.
 * 
 * @param snapshot Immutable text snapshot to check
 * @return Vector of spelling errors found
 * 
 * @note This function can be safely called from any thread
 */
QVector<SpellError> performSpellCheck(const StoryTextSnapshot& snapshot);

/**
 * @brief Check a single text string in a specific language
 * 
 * Helper function to check a text segment in a single language.
 * 
 * @param text Text to check
 * @param language Language code (e.g., "en_US", "de_DE")
 * @param basePosition Offset to add to error positions (for multi-run checking)
 * @return Vector of spelling errors found in this text
 */
QVector<SpellError> checkTextInLanguage(const QString& text, const QString& language, int basePosition = 0);

/**
 * @brief Check if a word is misspelled in a given language
 * 
 * @param word Word to check
 * @param language Language code
 * @return true if word is misspelled, false if correct
 */
bool isWordMisspelled(const QString& word, const QString& language);

/**
 * @brief Get spelling suggestions for a misspelled word
 * 
 * @param word Misspelled word
 * @param language Language code
 * @return List of suggested corrections
 */
QStringList getSpellingSuggestions(const QString& word, const QString& language);

#endif // SPELLCHECKFUNCTIONS_H
