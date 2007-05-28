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


class PLUGIN_API SaveAsTemplatePlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		SaveAsTemplatePlugin();
		virtual ~SaveAsTemplatePlugin();
		virtual bool run(ScribusDoc* doc, QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

		// Special features (none)
};

extern "C" PLUGIN_API int saveastemplateplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* saveastemplateplugin_getPlugin();
extern "C" PLUGIN_API void saveastemplateplugin_freePlugin(ScPlugin* plugin);


class satdialog;


class MenuSAT : public QObject
{
	Q_OBJECT

public:
	MenuSAT() {};
    ~MenuSAT() {};

public slots:
	void RunSATPlug(ScribusDoc*);
};

// static MenuSAT* satm;

class sat
{
private:
	ScribusDoc* m_Doc;
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
	sat(ScribusDoc* doc, satdialog* satdia, QString fileName, QString tmplDir);
	~sat();
};

static MenuSAT* Sat;

#endif
