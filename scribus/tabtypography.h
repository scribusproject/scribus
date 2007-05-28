/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABTYPOGRAPHY_H
#define TABTYPOGRAPHY_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <QLabel>
#include "scribusapi.h"
class Q3GridLayout;
class Q3GroupBox;
class QLabel;
class ScrSpinBox;
class QSpinBox;

class SCRIBUS_API TabTypograpy : public QWidget
{
	Q_OBJECT

public:
	TabTypograpy( QWidget* parent, struct typoPrefs *prefsData);
	~TabTypograpy() {};
	void restoreDefaults(struct typoPrefs *prefsData);

	Q3GroupBox* groupBox1a;
	QLabel* textLabel1a;
	QLabel* textLabel2a;
	QSpinBox* subDisplacement;
	QSpinBox* subScaling;
	Q3GroupBox* groupBox2a;
	QLabel* textLabel3a;
	QLabel* textLabel4a;
	QSpinBox* superDisplacement;
	QSpinBox* superScaling;
	Q3GroupBox* groupBox3a;
	QLabel* textLabel5a;
	QSpinBox* capsScaling;
	Q3GroupBox* underlineGroup;
	QLabel* textLabel1;
	ScrSpinBox* underlinePos;
	QLabel* textLabel2;
	ScrSpinBox* underlineWidth;
	Q3GroupBox* strikethruGroup;
	QLabel* textLabel1_2;
	ScrSpinBox* strikethruPos;
	QLabel* textLabel2_2;
	ScrSpinBox* strikethruWidth;
	Q3GroupBox* groupBox4a;
	QLabel* textLabel8a;
	QSpinBox* autoLine;

protected:
	Q3GridLayout* tabTypoLayout;
	Q3GridLayout* groupBox1aLayout;
	Q3GridLayout* groupBox2aLayout;
	Q3GridLayout* groupBox3aLayout;
	Q3GridLayout* groupBox4aLayout;
	Q3GridLayout* underlineGroupLayout;
    Q3GridLayout* strikethruGroupLayout;
};

#endif
