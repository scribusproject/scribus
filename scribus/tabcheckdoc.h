#ifndef TABCHECKDOC_H
#define TABCHECKDOC_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "scribusapi.h"
#include "prefsstructs.h"
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QCheckBox;
class Q3GroupBox;
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
	Q3GroupBox* pictResolution;
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
