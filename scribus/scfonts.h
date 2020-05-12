/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCFONTS_H
#define SCFONTS_H

#include <QByteArray>
#include <QDateTime>
#include <QFont>
#include <QList>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QString>
#include <QStringList>

#include "fonts/scface.h"
#include "fpointarray.h"
#include "scconfig.h"
#include "scribusapi.h"

/* Forward declaration so we don't have to include all of Freetype. */
typedef struct FT_LibraryRec_  *FT_Library;

class ScribusDoc;

/*! \brief Main class SCFonts.
Subclass of QDict<ScFace>.
This class replaces the previous SCFonts typedef, and is nearly as convenient.
The chief difference from the application point of view is that while data can
still be retrieved with SCFonts[fontname], this cannot be used to add members.
Since the only piece of code that will generally add members is scfonts.h, this
is not a major problem.
*/
class SCRIBUS_API SCFonts : public QMap<QString,ScFace>
{
	public:
		SCFonts();
		~SCFonts();

		void updateFontMap();
		void getFonts(const QString& pf, bool showFontInfo=false);
		ScFace loadScalableFont(const QString &filename);
		void addScalableFonts(const QString& path, const QString& DocName = "");

		/// Returns a font with that name; creates a replacement font if not found
		const ScFace& findFont(const QString& fontName, ScribusDoc* doc = nullptr);
		const ScFace& findFont(const QString& fontFamily, const QString& fontStyle, ScribusDoc* doc = nullptr);
		/// Returns a map of pairs (scName, replacementName). Using this map for replaceFonts() will make substitutions permanent
		QMap<QString,QString> getSubstitutions(const QList<QString>& skip = QList<QString>()) const;
		/// Changes replacement fonts to point to new real fonts. For all keys 'nam' in 'substitutes', findFont(name).isReplacement() must be true
		void setSubstitutions(const QMap<QString,QString>& substitutes, ScribusDoc* doc = nullptr);
		void removeFont(const QString& name);
		/// Write checked fonts file
		void writeFontCache();

		/// maps family name to face variants
		QMap<QString, QStringList> fontMap;
		QMap<QString, QString> rejectedFonts;

	private:
		void readFontCache(const QString& pf);
		void writeFontCache(const QString& pf);
		void addPath(QString p);
		bool addScalableFont(const QString& filename, FT_Library &library, const QString& DocName);
		void addRejectedFont(const QString& fontPath, const QString& message);
		void addUserPath(const QString& pf);
#ifdef HAVE_FONTCONFIG
		void addFontconfigFonts();
#elif defined(Q_OS_WIN32)
		void addRegistryFonts();
		void addType1RegistryFonts();
#endif
		QStringList m_fontPaths;

		struct testCache
		{
			bool isOK;
			bool isChecked;
			QDateTime lastMod;
		};
		QMap<QString, testCache> m_checkedFonts;

	protected:
		bool m_showFontInfo;
};

struct SCFontsIterator
{
	SCFontsIterator(SCFonts& fonts): m_it(fonts.begin()), m_end_it(fonts.end())
	{}
	ScFace& current()          { return *m_it; }
	QString currentKey() const { return m_it.key(); }
	bool hasNext()       const { return m_it != m_end_it; }
	ScFace& next()             { ++m_it; return current(); }

private:
	QMap<QString,ScFace>::Iterator m_it, m_end_it;
};

#endif
