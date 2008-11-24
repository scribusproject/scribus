/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scraction.h"
#include "aspellplugin.h"
#include "aspellpluginimpl.h"

// See scplugin.h and pluginmanager.{cpp,h} for detail on what these methods
// do. That documentatation is not duplicated here.
// Please don't implement the functionality of your plugin here; do that
// in aspellpluginimpl.h and aspellpluginimpl.cpp .

AspellPlugin::AspellPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

AspellPlugin::~AspellPlugin() {};

void AspellPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "AspellPlugin";
	// Action text for menu, including &accel
	m_actionInfo.text = tr("Spell Checker");
	// Menu
	m_actionInfo.menu = "Item";
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.notSuitableFor.append(PageItem::LatexFrame);
	m_actionInfo.notSuitableFor.append(PageItem::Polygon);
	m_actionInfo.notSuitableFor.append(PageItem::PolyLine);
	m_actionInfo.notSuitableFor.append(PageItem::ImageFrame);
	m_actionInfo.forAppMode.append(modeNormal);
	m_actionInfo.needsNumObjects = 1;
	// If needed, what item to add the menu item after
	//m_actionInfo.menuAfterName = "ColorWheel"
	// If needed, the keyboard shortcut for the plugin
	// GM: gedit, OpenOffice use F7 for spell-checking. There
	//seems to be no standard for KDE apps, other than the
	//suggestion of CTRL+ALT+S in the KClipSpellApplet.
	m_actionInfo.keySequence = "F7";
	// Should the menu item be enabled when the app starts
	// (even without a document open) ?
	m_actionInfo.enabledOnStartup = false;
}

const QString AspellPlugin::fullTrName() const
{
	return QObject::tr("Spell check (aspell)");
}

const ScActionPlugin::AboutData* AspellPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = "Gora Mohanty <gora@srijan.in>";
	about->shortDescription = tr( "Spell-checking support" );
	about->description =
	  tr( "Adds support for spell-checking via aspell. Languages "
	      "can be chosen from among the installed aspell "
	      "dictionaries, and spell-checking can be done on the "
	      "fly, or on selected text." );
	about->version = tr( "0.1" );
	about->copyright = QString::fromUtf8( "Copyright \xa9 Gora Mohanty <gora@srijan.in>" );
	about->license = "LGPL";
	return about;
}

void AspellPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool AspellPlugin::run(ScribusDoc* doc, QString target)
{
	AspellPluginImpl *myPluginImpl = new AspellPluginImpl( doc );
	Q_CHECK_PTR(myPluginImpl);
	// The spellcheck is disabled when there are no available
	// dictionaries.
	if (myPluginImpl->errorMessage().isEmpty())
		myPluginImpl->exec();
	else
	{
		doc->scMW()->scrActions[m_actionInfo.name]->setEnabled(false);
		doc->scMW()->scrActions[m_actionInfo.name]->setVisible(false);
		QMessageBox::warning(doc->scMW(),
							 tr("Aspell Plugin Error"),
							 myPluginImpl->errorMessage());
	}
	delete myPluginImpl;
	return true;
}

// Low level plugin API
int x_aspellplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* x_aspellplugin_getPlugin()
{
	AspellPlugin* plug = new AspellPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void x_aspellplugin_freePlugin(ScPlugin* plugin)
{
	AspellPlugin* plug = dynamic_cast<AspellPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}
