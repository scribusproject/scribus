/****************************************************************************
** Form interface generated from reading ui file 'CMY.ui'
**
** Created: Thu Sep 6 22:13:03 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
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

class CMYKcolor;
#include "colorchart.h"
#include "mspinbox.h"
#include "scribusstructs.h"

class CMYKChoose : public QDialog
{
	Q_OBJECT

public:
	CMYKChoose( QWidget* parent, CMYKColor orig, QString name, ColorList *Colors, QStringList Cust );
	~CMYKChoose() {};
	virtual void mouseReleaseEvent(QMouseEvent *m);
	QColor CMYK2RGB(int c, int m, int y, int k);
	QLabel* TextLabel1;
	QLineEdit* Farbname;
	QLabel* TextLabel3;
	QComboBox* ComboBox1;
	QCheckBox* Separations;
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
	CMYKColor Farbe;
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
};

#endif // CMYKFARBEN_H
