/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIPTER_H
#define SCRIPTER_H

#include "pluginapi.h"
#include "scplugin.h"
#include <scripterimpl.h>


class PLUGIN_API Scripter : public ScPersistentPlugin
{
    Q_OBJECT

public:
    Scripter();
    virtual ~Scripter();
    bool initPlugin() override;
    bool cleanupPlugin() override;
    QString fullTrName() const override;
    const AboutData* getAboutData() const override;
    void deleteAboutData(const AboutData* about) const override;
    void languageChange() override;
    void addToMainWindowMenu(ScribusMainWindow *) override;

private:
    ScripterImpl *scripterImpl;
};

extern "C" PLUGIN_API int scripter_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scripter_getPlugin();
extern "C" PLUGIN_API void scripter_freePlugin(ScPlugin* plugin);

#endif
