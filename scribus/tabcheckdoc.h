#ifndef TABCHECKDOC_H
#define TABCHECKDOC_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;
class QGroupBox;
class QLabel;
class QSpinBox;

class TabCheckDoc : public QWidget
{
	Q_OBJECT

public:
	TabCheckDoc( QWidget* parent, struct checkerPrefs *prefsData );
	~TabCheckDoc() {};

	QCheckBox* ignoreErrors;
	QCheckBox* automaticCheck;
	QCheckBox* missingGlyphs;
	QCheckBox* checkOrphans;
	QCheckBox* textOverflow;
	QCheckBox* tranparentObjects;
	QCheckBox* missingPictures;
	QGroupBox* pictResolution;
	QLabel* textLabel1;
	QSpinBox* resolutionValue;

protected:
	QVBoxLayout* TabCheckDocLayout;
	QHBoxLayout* pictResolutionLayout;
};

#endif // TABCHECKDOC_H
