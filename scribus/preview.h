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
#include "scribusapi.h"
#include "pageselector.h"
#include "prefsmanager.h"

class ScribusDoc;
class ScribusView;

class SCRIBUS_API PPreview : public QDialog
{
	Q_OBJECT

public:
	PPreview( QWidget* parent, ScribusView *vin, ScribusDoc *docu, int pngAlpha );
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
	ScribusView *view;
	ScribusDoc *doc;
	int HavePngAlpha;
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
	PrefsManager *prefsManager;
};
#endif // QUERY_H
