/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scripter.h"
#include "scripterimpl.h"


Scripter::Scripter() : ScPersistentPlugin()
{
    languageChange();
}



Scripter::~Scripter()
{
    //delete scripterImpl;
};



void Scripter::languageChange()
{
}



const QString Scripter::fullTrName() const
{
    return QObject::tr("Scripter Plugin");
}



const ScActionPlugin::AboutData* Scripter::getAboutData() const
{
    AboutData* about = new AboutData;
    about->authors = "Henning Schroeder <henning.schroeder@gmail.com>";
    about->shortDescription = "Scripting plug-in for Scribus";
    about->description = "Please visit http://scripterng.blogspot.com for more information.";
    about->version = "0.2";
    // XXX: do I needed some exception clauses?
    about->license = "GPL2 or later";
    Q_CHECK_PTR(about);
    return about;
}



void Scripter::deleteAboutData(const AboutData* about) const
{
    Q_ASSERT(about);
    delete about;
}



bool Scripter::initPlugin()
{
    scripterImpl = new ScripterImpl();
    Q_CHECK_PTR(scripterImpl);
    return scripterImpl->init();
}



/*
 * Strangely cleanupPlugin is not called on exit by default.
 */
bool Scripter::cleanupPlugin()
{
    bool result = scripterImpl->cleanup();
    return result;
}



void Scripter::addToMainWindowMenu(ScribusMainWindow *mainwin) {
    scripterImpl->addToMainWindowMenu(mainwin);
}



int scripter_getPluginAPIVersion()
{
    return PLUGIN_API_VERSION;
}



ScPlugin* scripter_getPlugin()
{
    Scripter* plug = new Scripter();
    Q_CHECK_PTR(plug);
    return plug;
}




void scripter_freePlugin(ScPlugin* plugin)
{
    Scripter* plug = dynamic_cast<Scripter*>(plugin);
    Q_ASSERT(plug);
    plug->cleanupPlugin();
    delete plug;
}

