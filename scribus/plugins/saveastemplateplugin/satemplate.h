#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <qobject.h>
#include <qdatetime.h>
#include <qdir.h>
#include <scribus.h>
#include "satdialog.h"

/** Calls the Plugin with the main Application window as parent
  * and the main Application Class as parameter */
extern "C" void Run(QWidget *d, ScribusApp *plug);


/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" QString Name();


/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugin is a Import Plugin, which appears in the Import Menue
  * 3 = the Plugin is a Export Plugin, which appears in the Export Menue
  * 4 = the Plugin is a resident Plugin   */
extern "C" int Type();

/** Initializes the Plugin if it's a Plugin of Type 4 or 5 */
extern "C" void InitPlug(QWidget *d, ScribusApp *plug);

/** Possible CleanUpOperations when closing the Plugin */
extern "C" void CleanUpPlug();

class MenuSAT : public QObject
{
    Q_OBJECT

public:
    MenuSAT(QWidget* parent) {};
    ~MenuSAT() {};

public slots:
	void RunSATPlug();
};

static MenuSAT* satm;

class sat 
{
private:
	ScribusApp* sapp;
	satdialog* dia; 
	QString file;
	QString dir;
	QString tmplXmlFile;
	void appendTmplXml();
	QString getTemplateTag();
public:
	void createTmplXml();
	void createImages();
	sat(ScribusApp* scribusApp, satdialog* satdia, QString fileName, QString tmplDir);
	~sat();
};

#endif
