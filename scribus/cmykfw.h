/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMYKFARBEN_H
#define CMYKFARBEN_H

#include <qdialog.h>
#include <qpixmap.h>
#include <qcolor.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qlineedit.h>
#include <qwidgetstack.h>
#include <qlistbox.h>

class ScColor;
#include "colorchart.h"
#include "colorsetmanager.h"
#include "mspinbox.h"
#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API CMYKChoose : public QDialog
{
	Q_OBJECT

public:
	CMYKChoose( QWidget* parent, ScColor orig, QString name, ColorList *Colors, QStringList Cust );
	~CMYKChoose() {};
	virtual void mouseReleaseEvent(QMouseEvent *m);
	QColor CMYK2RGB(int c, int m, int y, int k);
	QLabel* TextLabel1;
	QLineEdit* Farbname;
	QLabel* TextLabel3;
	QComboBox* ComboBox1;
	QCheckBox* Separations;
	QCheckBox* Regist;
	QComboBox* Swatches;
	QLabel* TextLabel5_2;
	QLabel* OldC;
	QLabel* TextLabel5;
	QLabel* NewC;
	QPushButton* Cancel;
	QPushButton* Cancel_2;
	QFrame* Frame4;
	QWidgetStack* TabStack;
	QFrame* Frame5;
	QFrame* Frame5a;
	QListBox* ColorSwatch;
	ColorChart* ColorMap;
	QLabel* CyanP;
	QLabel* CyanT;
	QSlider* CyanSL;
	MSpinBox* CyanSp;
	QLabel* MagentaP;
	QLabel* MagentaT;
	QSlider* MagentaSL;
	MSpinBox* MagentaSp;
	QLabel* YellowP;
	QLabel* YellowT;
	QSlider* YellowSL;
	MSpinBox* YellowSp;
	QLabel* BlackP;
	QLabel* BlackT;
	QSlider* BlackSL;
	MSpinBox* BlackSp;
	QPixmap imageA;
	QPixmap imageN;
	QPixmap alertIcon;
	ScColor Farbe;
	bool CMYKmode;
	bool Wsave;
	bool dynamic;
	int BlackComp;
	ColorList *EColors;
	ColorList CurrSwatch;
	QStringList CColSet;
	QString Fnam;

public slots:
	void SetValueS(int val);
	void ToggleSL();
	QPixmap SliderPix(int farbe);
	QPixmap SliderBlack();
	void SelSwatch(int n);
	void setSpot();
	void setRegist();
	void SelModel(const QString& mod);
	void setColor();
	void setColor2(int h, int s, bool ende);
	void SelFromSwatch( int c);
	void setValues();
	void Verlassen();

protected:
	QHBoxLayout* CMYKFarbenLayout;
	QVBoxLayout* Layout23;
	QGridLayout* Layout2;
	QGridLayout* Layout2x;
	QHBoxLayout* Layout21;
	QVBoxLayout* Frame4Layout;
	QHBoxLayout* Frame5Layout;
	QHBoxLayout* Frame5aLayout;
	QVBoxLayout* Layout1_2;
	QVBoxLayout* Layout1_2_2;
	QVBoxLayout* Layout1_2_3;
	QVBoxLayout* Layout1_2_4;
	
	ColorSetManager csm;
	int customSetStartIndex;
};

#endif // CMYKFARBEN_H
