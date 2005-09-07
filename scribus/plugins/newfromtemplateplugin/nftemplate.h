#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "pluginapi.h"
#include "scplugin.h"

class ScrAction;

class PLUGIN_API NewFromTemplatePlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		NewFromTemplatePlugin();
		virtual ~NewFromTemplatePlugin();
		virtual bool run(QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();

		// Special features (none)
};

extern "C" PLUGIN_API int newfromtemplateplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* newfromtemplateplugin_getPlugin();
extern "C" PLUGIN_API void newfromtemplateplugin_freePlugin(ScPlugin* plugin);

class MenuNFT : public QObject
{
	Q_OBJECT

public:
	MenuNFT(QWidget* /*parent*/) {};
    ~MenuNFT() {};

public slots:
	void RunNFTPlug();
};

static MenuNFT* Nft;

#endif
