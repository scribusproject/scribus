#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <qobject.h>
#include <qdatetime.h>
#include <qdir.h>
#include <scribus.h>
#include <pluginmanager.h>

#include "pluginapi.h"
#include "satdialog.h"

/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" PLUGIN_API QString name();


/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugin is a Import Plugin, which appears in the Import Menue
  * 3 = the Plugin is a Export Plugin, which appears in the Export Menue
  * 4 = the Plugin is a resident Plugin   */
extern "C" PLUGIN_API PluginManager::PluginType type();

///** Initializes the Plugin if it's a Plugin of Type 4 or 5 */
//extern "C" void InitPlug(QWidget *d, ScribusApp *plug);

/** Possible CleanUpOperations when closing the Plugin */
extern "C" PLUGIN_API void cleanUpPlug();
extern "C" PLUGIN_API int ID();

extern "C" PLUGIN_API QString actionName();
extern "C" PLUGIN_API QString actionKeySequence();
extern "C" PLUGIN_API QString actionMenu();
extern "C" PLUGIN_API QString actionMenuAfterName();
extern "C" PLUGIN_API bool actionEnabledOnStartup();

extern "C" PLUGIN_API void run(QWidget *d, ScribusApp *plug);


class MenuSAT : public QObject
{
	Q_OBJECT

public:
	MenuSAT(QWidget* /*parent*/) {};
    ~MenuSAT() {};

public slots:
	void RunSATPlug();
};

// static MenuSAT* satm;

class sat
{
private:
	ScribusApp* sapp;
	satdialog* dia;
	QString file;
	QString dir;
	QString tmplXmlFile;
	QString lang;
	void appendTmplXml();
	QString getTemplateTag();
	QString findTemplateXml(QString dir);
	void replaceIllegalChars(QString& s);
public:
	void createTmplXml();
	void createImages();
	sat(ScribusApp* scribusApp, satdialog* satdia, QString fileName, QString tmplDir);
	~sat();
};

static MenuSAT* Sat;

#endif
