/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "langmgr.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "spellcheckfunctions.h"
#include "textframespellchecker.h"
#include <hunspell/hunspell.hxx>
#include <QFile>
#include <QMap>
#include <QMutex>
#include <QRegularExpression>

// ============================================================================
// Helper: Hunspell dictionary management
// ============================================================================

class HunspellManager
{
	public:
		static HunspellManager* instance()
		{
			static HunspellManager manager;
			return &manager;
		}

		Hunspell* getHunspell(const QString& language)
		{
			QMutexLocker locker(&m_mutex);

			if (m_dictionaries.contains(language))
				return m_dictionaries[language];

			QString affPath = findDictionaryFile(language, ".aff");
			QString dicPath = findDictionaryFile(language, ".dic");
			QString altLanguage;
			if (affPath.isEmpty() || dicPath.isEmpty())
			{
				altLanguage = LanguageManager::instance()->getAlternativeAbbrevfromAbbrev(language);
				affPath = findDictionaryFile(altLanguage, ".aff");
				dicPath = findDictionaryFile(altLanguage, ".dic");
				if (affPath.isEmpty() || dicPath.isEmpty())
				{
					qWarning() << "Dictionary files not found for language:" << language;
					return nullptr;
				}
			}

			Hunspell* hunspell = new Hunspell(affPath.toUtf8().constData(), dicPath.toUtf8().constData());
			m_dictionaries[altLanguage.isEmpty() ? language : altLanguage] = hunspell;
			return hunspell;
		}

		~HunspellManager()
		{
			for (Hunspell* hunspell : m_dictionaries.values())
				delete hunspell;
		}

	private:
		QString findDictionaryFile(const QString& language, const QString& extension)
		{
			const QStringList searchPaths = ScPaths::instance().spellDirs();

			for (const QString& path : searchPaths)
			{
				QString filePath = path + language + extension;
				if (QFile::exists(filePath))
					return filePath;
			}

			// Fallback: try just the base language code ("en" from "en_US")
			const QString langOnly = language.split('_').first();
			for (const QString& path : searchPaths)
			{
				QString filePath = path + langOnly + extension;
				if (QFile::exists(filePath))
					return filePath;
			}

			return QString();
		}

		QMap<QString, Hunspell*> m_dictionaries;
		QMutex m_mutex;
};

// ============================================================================
// Main Spell Check Function
// ============================================================================

QVector<SpellError> performSpellCheck(const StoryTextSnapshot& snapshot)
{
	// qDebug()<<Q_FUNC_INFO;
	QVector<SpellError> errors;
	
	if (snapshot.isEmpty())
		return errors;
	
	// Process each paragraph
	for (int paraIndex = 0; paraIndex < snapshot.paragraphCount(); ++paraIndex)
	{
		// Get language runs for this paragraph
		QVector<LanguageRun> runs = snapshot.getLanguageRunsForParagraph(paraIndex);
		
		// Check each language run separately
		for (const LanguageRun& run : runs)
		{
			if (run.language.isEmpty())
				continue; // Skip runs with no language set
			
			// Extract text for this language run
			QString text = snapshot.plainText.mid(run.start, run.length);
			// qDebug()<<text;
			// Check this text segment
			QVector<SpellError> runErrors = checkTextInLanguage(text, run.language, run.start);
			
			errors.append(runErrors);
		}
	}
	
	return errors;
}

// ============================================================================
// Language-Specific Checking
// ============================================================================

QVector<SpellError> checkTextInLanguage(const QString& text,
										const QString& language,
										int basePosition)
{
	QVector<SpellError> errors;
	
	if (text.isEmpty())
		return errors;
	
	// static const QRegularExpression wordRegex("\\b([\\w']+)\\b");

	static const QRegularExpression wordRegex("\\b([\\w']*\\p{L}[\\w']*)\\b", QRegularExpression::UseUnicodePropertiesOption);


	Hunspell* hunspell = HunspellManager::instance()->getHunspell(language);
	if (!hunspell)
		return errors;

	QRegularExpressionMatchIterator it = wordRegex.globalMatch(text);
	while (it.hasNext())
	{
		const QRegularExpressionMatch match = it.next();
		const QString word = match.captured(1);

		if (word.length() < 2)
			continue;

		// Single conversion, single Hunspell call
		const std::string wordUtf8 = word.toStdString();
		if (!hunspell->spell(wordUtf8))
		{
			SpellError error;
			error.position = basePosition + match.capturedStart(1);
			error.length   = word.length();
			error.word     = word;
			error.language = language;
			errors.append(error);
		}
	}
	
	return errors;
}

QStringList getSpellingSuggestions(const QString& word, const QString& language)
{
	Hunspell* hunspell = HunspellManager::instance()->getHunspell(language);
	if (!hunspell)
		return QStringList();

	int maxSuggestions = PrefsManager::instance().appPrefs.spellCheckPrefs.maxSuggestions;

	const std::vector<std::string> suggestions = hunspell->suggest(word.toStdString());

	QStringList result;
	result.reserve(qMin((int)suggestions.size(), maxSuggestions));
	for (size_t i = 0; i < suggestions.size() && i < (size_t)maxSuggestions; ++i)
		result << QString::fromStdString(suggestions[i]);

	return result;
}
