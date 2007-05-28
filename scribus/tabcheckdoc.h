/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABCHECKDOC_H
#define TABCHECKDOC_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

#include "scribusapi.h"
#include "prefsstructs.h"
class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QComboBox;
class QCheckBox;
class Q3GroupBox;
class QLabel;
class QSpinBox;
class QPushButton;


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
	Q3GroupBox* pictResolution;
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
	Q3VBoxLayout* TabCheckDocLayout;
	Q3GridLayout* pictResolutionLayout;
	Q3HBoxLayout* layout1;
	QString tempNewProfileName;
};

#endif // TABCHECKDOC_H
