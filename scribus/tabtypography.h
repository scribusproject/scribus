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
	TabTypograpy( QWidget* parent, struct typoStruct *prefsData, double unitBase, QString unit);
	~TabTypograpy() {};

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
	QGroupBox* groupBox4a;
	QLabel* textLabel6a;
	QSpinBox* autoLine;
	QLabel* textLabel7a;
	MSpinBox* baseGrid;
	MSpinBox* baseOffset;
	QLabel* textLabel8a;

protected:
	QGridLayout* tabTypoLayout;
	QGridLayout* groupBox1aLayout;
	QGridLayout* groupBox2aLayout;
	QGridLayout* groupBox3aLayout;
	QGridLayout* groupBox4aLayout;
};

#endif
