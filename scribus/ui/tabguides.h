/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABGUIDES_H
#define TABGUIDES_H

#include "scribusapi.h"

#include <QWidget>

class QHBoxLayout;
class QGridLayout;
class QVBoxLayout;
class QGroupBox;
class QRadioButton;
class QLabel;
class QSpinBox;
class QPushButton;
class ScrSpinBox;
class QColor;

class SCRIBUS_API TabGuides : public QWidget
{
	Q_OBJECT

public:
	TabGuides( QWidget* parent, struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex);
	~TabGuides() {};
	void restoreDefaults(struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex);
	void unitChange(QString unit, int docUnitIndex, double invUnitConversion);

	ScrSpinBox* majorSpace;
	ScrSpinBox* minorSpace;
	QSpinBox* snapDistance;
	QLabel* checkBaseline;
	QColor colorGuides;
	QColor colorMargin;
	QColor colorMajorGrid;
	QColor colorMinorGrid;
	QColor colorBaselineGrid;
	QGroupBox* checkGrid;
	QGroupBox* groupBox1;
	QGroupBox* groupBox2;
	QGroupBox* commonBox;
	QGroupBox* snapBox;
	QGroupBox* guideBox;
	QGroupBox* marginBox;
	QGroupBox* baselineBox;
	QGroupBox* baseGridBox;
	QGroupBox* buttonGroup1;
	QRadioButton* inBackground;
	QRadioButton* inForeground;
	QLabel* textLabel4;
	QLabel* textLabel5;
	QLabel* textLabel6;
	QLabel* textLabel7;
	QLabel* textLabel8;
	QLabel* textLabel82;
	QLabel* checkMargin;
	QLabel* checkGuides;
	QPushButton* majorGridColor;
	QPushButton* minorGridColor;
	QPushButton* baselineColor;
	QPushButton* guideColor;
	QPushButton* marginColor;
	QSpinBox* grabDistance;
	QLabel* textLabel6a;
	QLabel* textLabel7a;
	ScrSpinBox* baseGrid;
	ScrSpinBox* baseOffset;
	QLabel* textLabel8a;

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
	QHBoxLayout* commonBoxLayout;
	QGridLayout* snapBoxLayout;
	QHBoxLayout* guideBoxLayout;
	QHBoxLayout* marginBoxLayout;
	QGridLayout* baselineBoxLayout;
	QGridLayout* baseGridBoxLayout;
	QVBoxLayout* buttonGroup1Layout;
	QHBoxLayout* layout9;
	QHBoxLayout* layout9a;
};
#endif
