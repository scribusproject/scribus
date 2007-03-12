/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include "scribusapi.h"
#include "scribusstructs.h"

class ScribusDoc;

class SCRIBUS_API CMSPrefs : public QWidget
{
	Q_OBJECT

public:
	CMSPrefs( QWidget* parent, CMSData *Vor, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles);
	~CMSPrefs() {};

	void restoreDefaults(CMSData *prefs, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles);
	void updateDocSettings(ScribusDoc* doc);

	QCheckBox* checkBox1;
	Q3GroupBox* sysProfiles;
	QLabel* text2;
	QLabel* text1;
	QLabel* text1CMYK;
	QLabel* text3;
	QLabel* text4;
	QLabel* text5;
	QComboBox* inputPRGBIm;
	QComboBox* inputPCMYKIm;
	QComboBox* inputPRGB;
	QComboBox* inputPCMYK;
	QComboBox* monitorP;
	QComboBox* printerP;
	Q3GroupBox* render;
	QLabel* text22;
	QComboBox* imagesI;
	QComboBox* colorsI;
	QLabel* text21;
	QCheckBox* simulate;
	QCheckBox* convertAll;
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
	Q3VBoxLayout* cmsPrefsLayout;
	Q3GridLayout* sysProfilesLayout;
	Q3GridLayout* renderLayout;
	Q3HBoxLayout* convertLayout;
	Q3HBoxLayout* gamutLayout;
};

#endif // CMSPREFS_H
