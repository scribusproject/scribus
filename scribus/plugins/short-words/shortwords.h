#ifndef SCRIBUS_SHORTWORDS_H
#define SCRIBUS_SHORTWORDS_H

#include "scconfig.h"
#include "pluginapi.h"
#include "scplugin.h"

/*! \brief This is the Scribus Short Words plugin interface specification.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

\author Petr Vanek <petr@yarpen.cz>
*/
class PLUGIN_API ShortWordsPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ShortWordsPlugin();
		virtual ~ShortWordsPlugin();
		virtual bool run(QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		/*! \brief Preference widget */
		virtual bool newPrefsPanelWidget(QWidget* parent, PrefsPanel*& panel,
										 QString& caption, QPixmap& icon);
		// Special features (none)
};

extern "C" PLUGIN_API int scribusshortwords_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribusshortwords_getPlugin();
extern "C" PLUGIN_API void scribusshortwords_freePlugin(ScPlugin* plugin);


class SWParse;
class SWConfig;

/*! \brief  This is Short Words plugin main class.
It contains main logic. */
class ShortWords : public QObject
{
	Q_OBJECT

public:
	/*! \brief  inicialization - Raise dialog, handles parsing etc. */
	ShortWords();
	/*! \brief  clear memaory, write cfg. */
	~ShortWords();

	/*! \brief  actual page when user calls this plug */
	uint originalPage;
	/*! \brief  All available configuration object */
	SWConfig *cfg;
	/*! \brief  all available "engine" */
	SWParse *parse;
};


#endif
