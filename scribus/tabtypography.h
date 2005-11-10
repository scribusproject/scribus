#ifndef TABTYPOGRAPHY_H
#define TABTYPOGRAPHY_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QLabel>
#include <QGridLayout>
#include "scribusapi.h"
class QGridLayout;
class Q3GroupBox;
class QLabel;
class MSpinBox;
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
	MSpinBox* underlinePos;
	QLabel* textLabel2;
	MSpinBox* underlineWidth;
	Q3GroupBox* strikethruGroup;
	QLabel* textLabel1_2;
	MSpinBox* strikethruPos;
	QLabel* textLabel2_2;
	MSpinBox* strikethruWidth;
	Q3GroupBox* groupBox4a;
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
