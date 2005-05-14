#ifndef TABTYPOGRAPHY_H
#define TABTYPOGRAPHY_H

#include <qvariant.h>
#include <qwidget.h>
class QGridLayout;
class QGroupBox;
class QLabel;
class MSpinBox;
class QSpinBox;

class TabTypograpy : public QWidget
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
	QSpinBox* underlinePos;
	QLabel* textLabel2;
	QSpinBox* underlineWidth;
    QGroupBox* strikethruGroup;
    QLabel* textLabel1_2;
    QSpinBox* strikethruPos;
    QLabel* textLabel2_2;
    QSpinBox* strikethruWidth;
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
