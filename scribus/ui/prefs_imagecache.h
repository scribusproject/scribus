/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_IMAGECACHE_H
#define PREFS_IMAGECACHE_H

#include "ui_prefs_imagecachebase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_ImageCache : public Prefs_Pane, Ui::Prefs_ImageCache
{
	Q_OBJECT

	public:
		Prefs_ImageCache(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_ImageCache();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();
};

#endif // PREFS_PATHS_H
