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
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "cmykcolor.h"
#include "colorchart.h"

class CMYKChoose : public QDialog
{ 
    Q_OBJECT

public:
    CMYKChoose( QWidget* parent, CMYKColor orig, QString name );
    ~CMYKChoose() {};
		virtual void mouseReleaseEvent(QMouseEvent *m);
		QColor CMYK2RGB(int c, int m, int y, int k);
    QLabel* TextLabel1;
    QLabel* Farbname;
    QLabel* TextLabel3;
    QComboBox* ComboBox1;
    QCheckBox* Separations;
    QLabel* TextLabel5_2;
    QLabel* OldC;
    QLabel* TextLabel5;
    QLabel* NewC;
    QPushButton* Cancel;
    QPushButton* Cancel_2;
    QFrame* Frame4;
    QFrame* Frame5;
    ColorChart* ColorMap;
    QLabel* CyanP;
    QLabel* CyanT;
    QSlider* CyanSL;
    QSpinBox* CyanSp;
    QLabel* MagentaP;
    QLabel* MagentaT;
    QSlider* MagentaSL;
    QSpinBox* MagentaSp;
    QLabel* YellowP;
    QLabel* YellowT;
    QSlider* YellowSL;
    QSpinBox* YellowSp;
    QLabel* BlackP;
    QLabel* BlackT;
    QSlider* BlackSL;
    QSpinBox* BlackSp;
    QPixmap imageA;
    QPixmap imageN;
    CMYKColor Farbe;
    bool CMYKmode;
		bool Wsave;
    bool dynamic;
    int BlackComp;
public slots:
		void ToggleSL();
		QPixmap SliderPix(int farbe);
		QPixmap SliderBlack();
		void SelModel(const QString& mod);
    void setColor();
    void setColor2(int h, int s, bool ende);
    void setValues();

protected:
    QHBoxLayout* CMYKFarbenLayout;
    QVBoxLayout* Layout23;
    QGridLayout* Layout2;
    QHBoxLayout* Layout21;
    QVBoxLayout* Frame4Layout;
    QHBoxLayout* Frame5Layout;
    QHBoxLayout* Cyan;
    QVBoxLayout* Layout1_2;
    QHBoxLayout* Magenta;
    QVBoxLayout* Layout1_2_2;
    QHBoxLayout* Yellow;
    QVBoxLayout* Layout1_2_3;
    QHBoxLayout* Black;
    QVBoxLayout* Layout1_2_4;
};

#endif // CMYKFARBEN_H
