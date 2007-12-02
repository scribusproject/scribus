/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fontpreviewplugin.h"
//#include "fontpreviewplugin.moc"
#include "fontpreview.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include <QCursor>

int fontpreview_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* fontpreview_getPlugin()
{
	FontPreviewPlugin* plug = new FontPreviewPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void fontpreview_freePlugin(ScPlugin* plugin)
{
	FontPreviewPlugin* plug = dynamic_cast<FontPreviewPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

FontPreviewPlugin::FontPreviewPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

FontPreviewPlugin::~FontPreviewPlugin() {};

void FontPreviewPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "FontPreview";
	// Action text for menu, including accel
	m_actionInfo.text = tr("&Font Preview...");
	// Menu
	m_actionInfo.menu = "Extras";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.needsNumObjects = -1;
}

const QString FontPreviewPlugin::fullTrName() const
{
	return QObject::tr("Font Preview");
}

const ScActionPlugin::AboutData* FontPreviewPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Petr Van\xc4\x9bk <petr@scribus.info>");
	about->shortDescription = tr("Font Preview dialog");
	about->description = tr("Sorting, searching and browsing available fonts.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void FontPreviewPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

/**
Create dialog and insert font into Style menu when user accepts.
*/
bool FontPreviewPlugin::run(ScribusDoc* doc, QString target)
{
	ScribusMainWindow* scmw=(doc==0)?ScCore->primaryMainWindow():doc->scMW();
	return run(scmw, doc, target);
}

bool FontPreviewPlugin::run(QWidget* parent, ScribusDoc* doc, QString target)
{
	if (doc==NULL)
		return false;
	// I don't know how many fonts user has...
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	FontPreview *dlg = new FontPreview(target, parent, doc);
	qApp->restoreOverrideCursor();
	// run it and wait for user's reaction
	if (dlg->exec() == QDialog::Accepted)
	{
		if  (target.isEmpty())
			doc->scMW()->SetNewFont(dlg->getCurrentFont());
		else
			m_runResult = dlg->getCurrentFont();
	}
	delete dlg;
	return true;
}
