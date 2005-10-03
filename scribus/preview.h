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
#include <qpushbutton.h>
#include "scribusapi.h"

class PageSelector;
class ScribusDoc;
class ScribusView;
class ScImage;
class ScColor;
class QSpinBox;
class PrefsManager;

class QImage;
class QTable;

class SCRIBUS_API PPreview : public QDialog
{
	Q_OBJECT

public:
	PPreview( QWidget* parent, ScribusView *vin, ScribusDoc *docu, int pngAlpha, int tiffSep );
	~PPreview() {};
	int RenderPreview(int Seite, int Res);
	int RenderPreviewSep(int Seite, int Res);
	void blendImages(QImage &target, ScImage &source, ScColor col);
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
	QPushButton *closeButton;
	QPushButton *printButton;
	/*! scaling GUI */
	QLabel* scaleLabel;
	QComboBox* scaleBox;
	ScribusView *view;
	ScribusDoc *doc;
	int HavePngAlpha;
	int HaveTiffSep;
	int APage;
	int MPage;
	int SMode;
	bool CMode;
	bool TxtAl;
	bool GrAl;
	bool Trans;
	bool GMode;
	QMap<QString, int> sepsToFileNum;
	QMap<QString, QCheckBox*> flagsVisible;
	QTable* Table;

public slots:
	void ToSeite(int num);
	void ToggleTextAA();
	void ToggleGr();
	void ToggleTr();
	void ToggleCMYK();
	void ToggleGCR();
	void ToggleCMYK_Colour();
	void scaleBox_valueChanged(int);

signals:
	void doPrint();

protected:
	/*! Percentage value of the scaling widget */
	double scaleFactor;
	QVBoxLayout* PLayout;
	QHBoxLayout* Layout1;
	QVBoxLayout* Layout2;
	QVBoxLayout* Layout3;
	QVBoxLayout* Layout4;
	QHBoxLayout* Layout5;
	QHBoxLayout* Layout6;
	PrefsManager *prefsManager;
};
#endif // QUERY_H
