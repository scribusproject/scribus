/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABGUIDES_H
#define TABGUIDES_H

#include "scribusapi.h"

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>
class Q3GridLayout;
class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GroupBox;
class Q3ButtonGroup;
class QRadioButton;
class QLabel;
class MSpinBox;
class QSpinBox;
class QPushButton;
class QCheckBox;
class QColor;

class SCRIBUS_API TabGuides : public QWidget
{
	Q_OBJECT

public:
	TabGuides( QWidget* parent, struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex);
	~TabGuides() {};
	void restoreDefaults(struct guidesPrefs *prefsData, struct typoPrefs *prefsData2, int unitIndex);
	void unitChange(QString unit, int docUnitIndex, int decimals, double invUnitConversion);

	MSpinBox* majorSpace;
	MSpinBox* minorSpace;
	QSpinBox* snapDistance;
	QLabel* checkBaseline;
	QColor colorGuides;
	QColor colorMargin;
	QColor colorMajorGrid;
	QColor colorMinorGrid;
	QColor colorBaselineGrid;
	Q3GroupBox* checkGrid;
	Q3GroupBox* groupBox1;
	Q3GroupBox* groupBox2;
	Q3GroupBox* commonBox;
	Q3GroupBox* snapBox;
	Q3GroupBox* guideBox;
	Q3GroupBox* marginBox;
	Q3GroupBox* baselineBox;
	Q3GroupBox* baseGridBox;
	Q3ButtonGroup* buttonGroup1;
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
	MSpinBox* baseGrid;
	MSpinBox* baseOffset;
	QLabel* textLabel8a;

protected slots:
	virtual void changeMajorColor();
	virtual void changeMinorColor();
	virtual void changeBaselineColor();
	virtual void changeGuideColor();
	virtual void changeMarginColor();

protected:
	Q3VBoxLayout* tabGuidesLayout;
	Q3GridLayout* checkGridLayout;
	Q3GridLayout* groupBox1Layout;
	Q3GridLayout* groupBox2Layout;
	Q3HBoxLayout* commonBoxLayout;
	Q3GridLayout* snapBoxLayout;
	Q3HBoxLayout* guideBoxLayout;
	Q3HBoxLayout* marginBoxLayout;
	Q3GridLayout* baselineBoxLayout;
	Q3GridLayout* baseGridBoxLayout;
	Q3VBoxLayout* buttonGroup1Layout;
	Q3HBoxLayout* layout9;
	Q3HBoxLayout* layout9a;
	
// 	double unitRatio;
// 	QString unit;
// 	int precision;
};
#endif
