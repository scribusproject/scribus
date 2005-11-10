#ifndef CMSPREFS_H
#define CMSPREFS_H

#include <qwidget.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QGridLayout>
#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API CMSPrefs : public QWidget
{
	Q_OBJECT

public:
	CMSPrefs( QWidget* parent, CMSData *Vor, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles);
	~CMSPrefs() {};
	void restoreDefaults();

	QCheckBox* checkBox1;
	Q3GroupBox* sysProfiles;
	QLabel* text2;
	QLabel* text1;
	QLabel* text1CMYK;
	QLabel* text3;
	QLabel* text4;
	QComboBox* inputP;
	QComboBox* inputPCMYK;
	QComboBox* inputP2;
	QComboBox* monitorP;
	QComboBox* printerP;
	Q3GroupBox* render;
	QLabel* text22;
	QComboBox* monitorI;
	QComboBox* printerI;
	QLabel* text21;
	QCheckBox* simulate;
	QCheckBox* gamutC;
	QCheckBox* blackP;
	CMSData *prefs;
	bool changed;

public slots:
	virtual void slotCMSon();
	virtual void setValues();
	virtual void slotSimula();

signals:
	void cmsOn(bool);

protected:
	QVBoxLayout* cmsPrefsLayout;
	QGridLayout* sysProfilesLayout;
	QGridLayout* renderLayout;
};

#endif // CMSPREFS_H
