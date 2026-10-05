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
#include <QStringDecoder>
#include <QStringEncoder>

// ============================================================================
// Helper: Hunspell dictionary management
// ============================================================================

class HunspellManager
{
	public:
		struct DictEntry
		{
			Hunspell* hunspell {nullptr};
			QStringDecoder decoder;
			QStringEncoder encoder;
		};

		static HunspellManager* instance()
		{
			static HunspellManager manager;
			return &manager;
		}

		DictEntry* getDictEntry(const QString& language)
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

				// Check if we already have the alt language cached
				if (m_dictionaries.contains(altLanguage))
				{
					// Alias the original language to the existing entry
					m_dictionaries[language] = m_dictionaries[altLanguage];
					return m_dictionaries[language];
				}

				affPath = findDictionaryFile(altLanguage, ".aff");
				dicPath = findDictionaryFile(altLanguage, ".dic");
				if (affPath.isEmpty() || dicPath.isEmpty())
				{
					qWarning() << "Dictionary files not found for language:" << language;
					m_dictionaries[language] = nullptr;
					if (!altLanguage.isEmpty() && altLanguage != language)
						m_dictionaries[altLanguage] = nullptr;
					return nullptr;
				}
			}

			Hunspell* hunspell = new Hunspell(affPath.toUtf8().constData(), dicPath.toUtf8().constData());
			const std::string encoding = hunspell->get_dict_encoding();

			DictEntry* entry = new DictEntry;
			entry->hunspell = hunspell;
			entry->decoder = QStringDecoder(encoding.c_str());
			entry->encoder = QStringEncoder(encoding.c_str());
			if (!entry->decoder.isValid() || !entry->encoder.isValid())
			{
				qWarning() << "Unknown dictionary encoding" << QString::fromStdString(encoding)
						   << "for language" << language << "- falling back to UTF-8";
				entry->decoder = QStringDecoder(QStringDecoder::Utf8);
				entry->encoder = QStringEncoder(QStringEncoder::Utf8);
			}

			// Cache under both the requested language AND the alt language if different
			m_dictionaries[language] = entry;
			if (!altLanguage.isEmpty() && altLanguage != language)
				m_dictionaries[altLanguage] = entry;

			return entry;
		}

		void invalidateMissingCache()
		{
			QMutexLocker locker(&m_mutex);
			for (auto it = m_dictionaries.begin(); it != m_dictionaries.end(); )
			{
				if (it.value() == nullptr)
					it = m_dictionaries.erase(it);
				else
					++it;
			}
		}

		~HunspellManager()
		{
			// Multiple keys may point to the same DictEntry (aliasing for language fallbacks),
			// so collect unique pointers before deleting.
			QList<DictEntry*> toDelete;
			for (DictEntry* entry : m_dictionaries.values())
			{
				if (entry && !toDelete.contains(entry))
					toDelete.append(entry);
			}
			for (DictEntry* entry : toDelete)
			{
				delete entry->hunspell;
				delete entry;
			}
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

		QMap<QString, DictEntry*> m_dictionaries;
		QMutex m_mutex;
};

// ============================================================================
// Main Spell Check Function
// ============================================================================

QVector<SpellError> performSpellCheck(const StoryTextSnapshot& snapshot)
{
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
			QVector<SpellError> runErrors = checkTextInLanguage(text, run.language, run.start);

			errors.append(runErrors);
		}
	}

	return errors;
}

// ============================================================================
// Language-Specific Checking
// ============================================================================

QVector<SpellError> checkTextInLanguage(const QString& text, const QString& language, int basePosition)
{
	QVector<SpellError> errors;

	if (text.isEmpty())
		return errors;

	static const QRegularExpression wordRegex("\\b([\\w']*\\p{L}[\\w']*)\\b", QRegularExpression::UseUnicodePropertiesOption);

	HunspellManager::DictEntry* entry = HunspellManager::instance()->getDictEntry(language);
	if (!entry)
		return errors;

	// int wordCount = 0;
	// int misspelledCount = 0;

	QRegularExpressionMatchIterator it = wordRegex.globalMatch(text);
	while (it.hasNext())
	{
		const QRegularExpressionMatch match = it.next();
		const QString word = match.captured(1);

		if (word.length() < 2)
			continue;

		// wordCount++;
		// Encode word in the dictionary's native encoding before passing to Hunspell
		QByteArray encoded = entry->encoder.encode(word);
		if (!entry->hunspell->spell(encoded.toStdString()))
		{
			// misspelledCount++;
			SpellError error;
			error.position = basePosition + match.capturedStart(1);
			error.length   = word.length();
			error.word     = word;
			error.language = language;
			errors.append(error);
		}
	}

	// qDebug() << "checkTextInLanguage" << language
	// 			 << "text length:" << text.length()
	// 			 << "words checked:" << wordCount
	// 			 << "misspelled:" << misspelledCount;

	return errors;
}

QStringList getSpellingSuggestions(const QString& word, const QString& language)
{
	HunspellManager::DictEntry* entry = HunspellManager::instance()->getDictEntry(language);
	if (!entry)
		return QStringList();

	int maxSuggestions = PrefsManager::instance().appPrefs.spellCheckPrefs.maxSuggestions;

	QByteArray encoded = entry->encoder.encode(word);
	const std::vector<std::string> suggestions = entry->hunspell->suggest(encoded.toStdString());

	QStringList result;
	result.reserve(qMin((int)suggestions.size(), maxSuggestions));
	for (size_t i = 0; i < suggestions.size() && i < (size_t)maxSuggestions; ++i)
		result << entry->decoder.decode(QByteArray::fromStdString(suggestions[i]));

	return result;
}
void invalidateMissingDictionaries()
{
	HunspellManager::instance()->invalidateMissingCache();
}
