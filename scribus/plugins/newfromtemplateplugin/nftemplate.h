#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <scribus.h>
#include "nftdialog.h"

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

class MenuNFT : public QObject
{
	Q_OBJECT

public:
    MenuNFT(QWidget* parent) {};
    ~MenuNFT() {};

public slots:
	void RunNFTPlug();
};

static MenuNFT* Nft;

#endif
