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

#include "scribusapi.h"
#include "prefsstructs.h"
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QCheckBox;
class QGroupBox;
class QLabel;
class QSpinBox;
class QPushButton;

class SCRIBUS_API TabCheckDoc : public QWidget
{
	Q_OBJECT

public:
	TabCheckDoc( QWidget* parent, CheckerPrefsList prefsData, QString prefProfile );
	~TabCheckDoc() {};
	void restoreDefaults();

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
	QGroupBox* pictResolution;
	QLabel* textLabel1;
	QSpinBox* resolutionValue;
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
	QHBoxLayout* pictResolutionLayout;
	QHBoxLayout* layout1;
};

#endif // TABCHECKDOC_H
