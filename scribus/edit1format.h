#ifndef EDITSTYLE_H
#define EDITSTYLE_H

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "mspinbox.h"
#include "fontcombo.h"
#include "scribusdoc.h"

class EditStyle : public QDialog
{ 
    Q_OBJECT

public:
    EditStyle( QWidget* parent, struct StVorL *vor, QValueList<StVorL> v, bool neu, preV *Prefs, double au);
    ~EditStyle() {};

    QLabel* TextLabel1;
    QLabel* TextLabel1_2;
    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QLabel* TextLabel1_2_2;
    QLabel* TextLabel1_2_3;
    QLineEdit* Name;
    MSpinBox* LineSpVal;
    MSpinBox* LeftInd;
    MSpinBox* FirstLin;
    MSpinBox* AboveV;
    MSpinBox* BelowV;
    QButtonGroup* ButtonGroup1;
    QGroupBox* GroupBox10;
    QGroupBox* AbstandV;
    QRadioButton* Left;
    QRadioButton* Center;
    QRadioButton* Right;
    QRadioButton* Full;
    QRadioButton* Forced;
    QGroupBox* GroupFont;
    QLabel* TextF1;
    FontCombo* FontC;
    MSpinBox* SizeC;
    QLabel* TextF2;
    QPushButton* Cancel;
    QPushButton* OkButton;
    struct StVorL *werte;
    QValueList<StVorL> allV;
    QString OldName;
    bool IsNew;
		double AutoVal;

private slots:
		void Verlassen();
		void FontChange(int val);

protected:
    QHBoxLayout* Layout17;
    QGridLayout* EditStyleLayout;
    QGridLayout* GroupBox10Layout;
    QGridLayout* ButtonGroup1Layout;
    QGridLayout* AbstandVLayout;
    QGridLayout* GroupFontLayout;
};

#endif // EDITSTYLE_H
