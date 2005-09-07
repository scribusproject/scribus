/*! This is the Scribus Short Words plugin interface specification.

This code is based on the Scribus-Vlna plug in rewritten for
international use.

\author Petr Vanek <petr@yarpen.cz>

This program is free software - see LICENSE file in the distribution
or documentation
*/

#ifndef SCRIBUS_SHORTWORDS_H
#define SCRIBUS_SHORTWORDS_H

#include "scconfig.h"
#include "pluginapi.h"
#include "scplugin.h"


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

		// Special features (none)
};

extern "C" PLUGIN_API int scribusshortwords_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribusshortwords_getPlugin();
extern "C" PLUGIN_API void scribusshortwords_freePlugin(ScPlugin* plugin);


class Parse;
class Config;

/** \brief This is Short Words plugin main class.
It contains main logic. */
class ShortWords : public QObject
{
	Q_OBJECT

public:
	/** inicialization - Raise dialog, handles parsing etc. */
	ShortWords();
	/** clear memaory, write cfg. */
	~ShortWords();

	/** actual page when user calls this plug */
	uint originalPage;
	/** All available configuration object */
	Config *cfg;
	/** all available "engine" */
	Parse *parse;
};


#endif
