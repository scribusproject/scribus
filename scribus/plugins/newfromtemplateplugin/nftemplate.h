/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "pluginapi.h"
#include "scplugin.h"

class ScrAction;
class ScribusDoc;

class PLUGIN_API NewFromTemplatePlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		NewFromTemplatePlugin();
		virtual ~NewFromTemplatePlugin();
		//! \brief main method
		virtual bool run(ScribusDoc* doc, QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

		// Special features (none)
};

extern "C" PLUGIN_API int newfromtemplateplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* newfromtemplateplugin_getPlugin();
extern "C" PLUGIN_API void newfromtemplateplugin_freePlugin(ScPlugin* plugin);

class MenuNFT : public QObject
{
	Q_OBJECT

public:
	MenuNFT() {};
    ~MenuNFT() {};

public slots:
	void RunNFTPlug(ScribusDoc*);
};

static MenuNFT* Nft;

#endif
