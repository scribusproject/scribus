/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABCHECKDOC_H
#define TABCHECKDOC_H

#include <QWidget>
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;
class QGroupBox;
class QLabel;
class QComboBox;
class QPushButton;
class QSpinBox;

#include "scribusapi.h"
#include "prefsstructs.h"


/*! \brief A setup widget for checking Profiles (for Preflight Verifier constraints)
*/
class SCRIBUS_API TabCheckDoc : public QWidget
{
	Q_OBJECT

public:
	TabCheckDoc( QWidget* parent, CheckerPrefsList prefsData, QString prefProfile );
	~TabCheckDoc() {};
	void restoreDefaults(CheckerPrefsList *prefsData, QString prefProfile);

	QComboBox* curCheckProfile;
	QCheckBox* ignoreErrors;
	QCheckBox* automaticCheck;
	QCheckBox* missingGlyphs;
	QCheckBox* checkOrphans;
	QCheckBox* textOverflow;
	QCheckBox* tranparentObjects;
	QCheckBox* missingPictures;
	QCheckBox* useAnnotations;
	QCheckBox* rasterPDF;
	QCheckBox* checkForGIF;
	QCheckBox* ignoreOffLayers;
	QCheckBox* checkOffConflictLayers;
	QGroupBox* pictResolution;
	QLabel* textLabel1;
	QSpinBox* resolutionValue;
	QLabel* textLabel1m;
	QSpinBox* resolutionValueM;
	QPushButton* addProfile;
	QPushButton* removeProfile;
	CheckerPrefsList checkerProfile;
	QString currentProfile;
	void updateProfile(const QString& name);

public slots:
	void putProfile();
	void setProfile(const QString& name);
	void addProf();
	void delProf();

protected:
	QVBoxLayout* TabCheckDocLayout;
	QGridLayout* pictResolutionLayout;
	QHBoxLayout* layout1;
	QString tempNewProfileName;
};

#endif // TABCHECKDOC_H
