#ifndef PRVIEW_H
#define PRVIEW_H

#include <qdialog.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qscrollview.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include "scribus.h"
#include "pageselector.h"

/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" QString Name();
/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugins is a import Plugin, which appears in the Import Menue
  * 3 = the Plugins is a export Plugin, which appears in the Export Menue */
extern "C" int Type();

/** Initializes the Plugin if it's a Plugin of Type 4 or 5 */
extern "C" void InitPlug(QWidget *d, ScribusApp *plug);

/** Possible CleanUpOperations when closing the Plugin */
extern "C" void CleanUpPlug();

class MenuPreview : public QObject
{
    Q_OBJECT

public:
    MenuPreview(QWidget* parent) {};
    ~MenuPreview() {};

public slots:
	void RunPreview();
};

static MenuPreview* Tes;

class PPreview : public QDialog
{
	Q_OBJECT

public:
	PPreview( QWidget* parent, ScribusApp *plug );
	~PPreview() {};
	int RenderPreview(int Seite, int Res);
	QPixmap CreatePreview(int Seite, int Res);
	PageSelector *PGSel;
	QCheckBox* AliasText;
	QCheckBox* AliasGr;
	QCheckBox* AliasTr;
	QCheckBox* EnableCMYK;
	QCheckBox* EnableCMYK_C;
	QCheckBox* EnableCMYK_M;
	QCheckBox* EnableCMYK_Y;
	QCheckBox* EnableCMYK_K;
	QCheckBox* EnableGCR;
	QScrollView* Anzeige;
	QLabel* Anz;
	ScribusApp *app;
	int APage;
	int MPage;
	bool CMode;
	bool TxtAl;
	bool GrAl;
	bool Trans;
	bool GMode;

public slots:
	void ToSeite(int num);
	void ToggleTextAA();
	void ToggleGr();
	void ToggleTr();
	void ToggleCMYK();
	void ToggleGCR();
	void ToggleCMYK_Colour();

protected:
	QVBoxLayout* PLayout;
	QHBoxLayout* Layout1;
	QVBoxLayout* Layout2;
	QVBoxLayout* Layout3;
	QVBoxLayout* Layout4;
	QHBoxLayout* Layout5;
};
#endif // QUERY_H
