/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS_SHORTWORDS_H
#define SCRIBUS_SHORTWORDS_H

#include "scconfig.h"
#include "pluginapi.h"
#include "scplugin.h"
//Added by qt3to4:
#include <QPixmap>

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
		virtual bool run(ScribusDoc* doc, QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};
		/*! \brief Preference widget */
		virtual bool newPrefsPanelWidget(QWidget* parent, PrefsPanel*& panel,
										 QString& caption, QPixmap& icon);
		// Special features (none)
};

extern "C" PLUGIN_API int scribusshortwords_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribusshortwords_getPlugin();
extern "C" PLUGIN_API void scribusshortwords_freePlugin(ScPlugin* plugin);

#endif
