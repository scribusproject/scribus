#ifndef TABGUIDES_H
#define TABGUIDES_H

#include <qvariant.h>
#include <qwidget.h>
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class QLabel;
class MSpinBox;
class QSpinBox;
class QPushButton;
class QCheckBox;
class QColor;

class TabGuides : public QWidget
{
	Q_OBJECT

public:
	TabGuides( QWidget* parent, struct guidesPrefs *prefsData, double unitBase, QString unit);
	~TabGuides() {};

	MSpinBox* majorSpace;
	MSpinBox* minorSpace;
	MSpinBox* snapDistance;
	QCheckBox* checkLink;
	QCheckBox* checkFrame;
	QCheckBox* checkPictures;
	QCheckBox* checkMargin;
	QCheckBox* checkGuides;
	QCheckBox* checkBaseline;
	QColor colorGuides;
	QColor colorMargin;
	QColor colorMajorGrid;
	QColor colorMinorGrid;
	QColor colorBaselineGrid;
	QGroupBox* checkGrid;
	QGroupBox* groupBox1;
	QGroupBox* groupBox2;
	QLabel* textLabel1;
	QLabel* textLabel2;
	QLabel* textLabel3;
	QLabel* textLabel4;
	QLabel* textLabel5;
	QLabel* textLabel6;
	QLabel* textLabel7;
	QLabel* textLabel8;
	QLabel* textLabel82;
	QPushButton* majorGridColor;
	QPushButton* minorGridColor;
	QPushButton* baselineColor;
	QPushButton* guideColor;
	QPushButton* marginColor;
	QSpinBox* grabDistance;

protected slots:
	virtual void changeMajorColor();
	virtual void changeMinorColor();
	virtual void changeBaselineColor();
	virtual void changeGuideColor();
	virtual void changeMarginColor();

protected:
	QVBoxLayout* tabGuidesLayout;
	QGridLayout* checkGridLayout;
	QGridLayout* groupBox1Layout;
	QGridLayout* groupBox2Layout;
	QGridLayout* layout11;
	QHBoxLayout* layout2;
	QHBoxLayout* layout3;
	QHBoxLayout* layout9;
	QGridLayout* layout12;
};
#endif
