#ifndef REFORMDOC_H
#define REFORMDOC_H

#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
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

	QWidget* tabView;
	QCheckBox* checkMargin;
	QCheckBox* checkFrame;
	QCheckBox* checkPictures;
	QCheckBox* checkGrid;
	QCheckBox* checkGuides;
	QCheckBox* checkBaseline;

	int einheit;
	double Breite;
	double Hoehe;

protected:
	QVBoxLayout* ReformDocLayout;
	QHBoxLayout* dsGroupBox7Layout;
	QHBoxLayout* GroupBox7Layout;
	QGridLayout* dsLayout4;
	QGridLayout* Layout4;
	QHBoxLayout* Layout3;
	QVBoxLayout* tabViewLayout;

protected slots:
	virtual void setDS();
	virtual void setTop(int v);
	virtual void setBottom(int v);
	virtual void setLeft(int v);
	virtual void setRight(int v);
};

#endif // REFORMDOC_H
