#ifndef TABCHECKDOC_H
#define TABCHECKDOC_H

#include <qvariant.h>
#include <qwidget.h>

#include "scribusstructs.h"
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QCheckBox;
class QGroupBox;
class QLabel;
class QSpinBox;

class TabCheckDoc : public QWidget
{
	Q_OBJECT

public:
	TabCheckDoc( QWidget* parent, QMap<QString, checkerPrefs> prefsData, QString prefProfile );
	~TabCheckDoc() {};

	QComboBox* curCheckProfile;
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
	QMap<QString, checkerPrefs> checkerProfile;
	QString currentProfile;

public slots:
	void putProfile();
	void setProfile(const QString& name);

protected:
	QVBoxLayout* TabCheckDocLayout;
	QHBoxLayout* pictResolutionLayout;
};

#endif // TABCHECKDOC_H
