/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef BARCODE_H
#define BARCODE_H

#include "pluginapi.h"
#include "scplugin.h"


/*! \brief Scribus interface to the Barcode Generator.
\author Petr Vanek <petr@yarpen.cz>
*/
class PLUGIN_API Barcode : public ScActionPlugin
{
	Q_OBJECT

	public:
		Barcode();
		virtual ~Barcode();
		/*! \brief run the plug
		\param target none usage here */
		bool run(ScribusDoc* doc, const QString& target = QString()) override;
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		void addToMainWindowMenu(ScribusMainWindow *) override {};
};

extern "C" PLUGIN_API int barcodegenerator_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* barcodegenerator_getPlugin();
extern "C" PLUGIN_API void barcodegenerator_freePlugin(ScPlugin* plugin);

#endif
