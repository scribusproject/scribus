/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include "pluginapi.h"
#include "scplugin.h"

/** \brief This is a simple "Color Theory" plugin for Scribus 1.3 and later.
Harmonious colors are colors that work well together, that produce a color
scheme that looks attractive; the color wheel can be used as a valuable
tool for determining harmonious colors.
More on: http://en.wikipedia.org/wiki/Color_wheel
\author Petr Vanek <petr@scribus.info>
\date April 2005
*/
class PLUGIN_API ColorWheelPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		ColorWheelPlugin();
		virtual ~ColorWheelPlugin();
		bool run(ScribusDoc* doc, const QString& target = QString()) override;
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		void addToMainWindowMenu(ScribusMainWindow *) override {};

		// Special features (none)
};

extern "C" PLUGIN_API int colorwheel_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* colorwheel_getPlugin();
extern "C" PLUGIN_API void colorwheel_freePlugin(ScPlugin* plugin);

#endif
