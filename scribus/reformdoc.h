#ifndef REFORMDOC_H
#define REFORMDOC_H

#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcolor.h>
#include "mspinbox.h"
#include "prefsdialogbase.h"
class ScribusDoc;

class ReformDoc : public PrefsDialogBase
{
	Q_OBJECT

public:
	ReformDoc( QWidget* parent, ScribusDoc* doc );
	~ReformDoc() {};
	QWidget* tabPage;
	QGroupBox* GroupBox7;
	QGroupBox* dsGroupBox7;
	QLabel* TextLabel5;
	QLabel* TextLabel7;
	QLabel* TextLabel1_3;
	QSpinBox* PgNr;
	MSpinBox* TopR;
	MSpinBox* BottomR;
	MSpinBox* LeftR;
	MSpinBox* RightR;
	MSpinBox* widthMSpinBox;
	MSpinBox* heightMSpinBox;
	QCheckBox* Doppelseiten;
	QCheckBox* ErsteSeite;
	QLabel* Links;
	QLabel* Rechts;
	QLabel*	widthQLabel;
	QLabel*	heightQLabel;
	QLabel*	sizeQLabel;
	QLabel*	orientationQLabel;
	QComboBox*	sizeQComboBox;
	QComboBox*	orientationQComboBox;

	QWidget* tabGuides;
	QWidget* tabView;
	QLabel* textLabel1;
	QLabel* textLabel2;
	QLabel* textLabel3;
	QLabel* textLabel4;
	QLabel* textLabel5;
	QLabel* textLabel6;
	QLabel* textLabel7;
	QLabel* textLabel8;
	QLabel* textLabel9;
	QGroupBox* checkGrid;
	QGroupBox* groupBox1;
	QGroupBox* groupBox2;
	QPushButton* majorGridColor;
	QPushButton* minorGridColor;
	QPushButton* baselineColor;
	QPushButton* guideColor;
	QPushButton* marginColor;
	QPushButton* backColor;
	MSpinBox* minorSpace;
	MSpinBox* majorSpace;
	MSpinBox* snapDistance;
	QButtonGroup* buttonGroup1;
	QButtonGroup* pageBackground;
	QRadioButton* inBackground;
	QRadioButton* inForeground;
	QCheckBox* checkUnprintable;
	QCheckBox* checkMargin;
	QCheckBox* checkFrame;
	QCheckBox* checkPictures;
	QCheckBox* checkGuides;
	QCheckBox* checkBaseline;
	QCheckBox* checkLink;

	int einheit;
	double Breite;
	double Hoehe;
	QColor colorMajorGrid;
	QColor colorMinorGrid;
	QColor colorBaselineGrid;
	QColor colorPaper;
	QColor colorGuides;
	QColor colorMargin;

protected:
	QVBoxLayout* ReformDocLayout;
	QHBoxLayout* dsGroupBox7Layout;
	QHBoxLayout* GroupBox7Layout;
	QGridLayout* dsLayout4;
	QGridLayout* Layout4;
	QHBoxLayout* Layout3;
	QVBoxLayout* tabGuidesLayout;
	QVBoxLayout* tabViewLayout;
	QGridLayout* checkGridLayout;
	QGridLayout* groupBox1Layout;
	QGridLayout* groupBox2Layout;
	QVBoxLayout* buttonGroup1Layout;
	QHBoxLayout* layout7;
	QGridLayout* layout11;
	QHBoxLayout* layout2;
	QHBoxLayout* layout3;
	QHBoxLayout* layout9;
	QVBoxLayout* pageBackgroundLayout;
	QHBoxLayout* layout10;
	QSpacerItem* spacer3;
	QGridLayout* layout12;

protected slots:
	virtual void changeMajorColor();
	virtual void changeMinorColor();
	virtual void changePaperColor();
	virtual void changeBaselineColor();
	virtual void changeGuideColor();
	virtual void changeMarginColor();
	virtual void setDS();
	virtual void setTop(int v);
	virtual void setBottom(int v);
	virtual void setLeft(int v);
	virtual void setRight(int v);
};

#endif // REFORMDOC_H
