/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABTYPOGRAPHY_H
#define TABTYPOGRAPHY_H

#include <QWidget>
class QGridLayout;
class QGroupBox;
class QLabel;
class QSpinBox;

#include "scribusapi.h"
class ScrSpinBox;

class SCRIBUS_API TabTypograpy : public QWidget
{
	Q_OBJECT

public:
	TabTypograpy( QWidget* parent, struct typoPrefs *prefsData);
	~TabTypograpy() {};
	void restoreDefaults(struct typoPrefs *prefsData);

	QGroupBox* groupBox1a;
	QLabel* textLabel1a;
	QLabel* textLabel2a;
	QSpinBox* subDisplacement;
	QSpinBox* subScaling;
	QGroupBox* groupBox2a;
	QLabel* textLabel3a;
	QLabel* textLabel4a;
	QSpinBox* superDisplacement;
	QSpinBox* superScaling;
	QGroupBox* groupBox3a;
	QLabel* textLabel5a;
	QSpinBox* capsScaling;
	QGroupBox* underlineGroup;
	QLabel* textLabel1;
	ScrSpinBox* underlinePos;
	QLabel* textLabel2;
	ScrSpinBox* underlineWidth;
	QGroupBox* strikethruGroup;
	QLabel* textLabel1_2;
	ScrSpinBox* strikethruPos;
	QLabel* textLabel2_2;
	ScrSpinBox* strikethruWidth;
	QGroupBox* groupBox4a;
	QLabel* textLabel8a;
	QSpinBox* autoLine;

protected:
	QGridLayout* tabTypoLayout;
	QGridLayout* groupBox1aLayout;
	QGridLayout* groupBox2aLayout;
	QGridLayout* groupBox3aLayout;
	QGridLayout* groupBox4aLayout;
	QGridLayout* underlineGroupLayout;
    QGridLayout* strikethruGroupLayout;
};

#endif
