/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMSPREFS_H
#define CMSPREFS_H

#include "ui_cmsprefsbase.h"
#include "scribusapi.h"

class ScribusDoc;
struct CMSData;
typedef QMap<QString,QString> ProfilesL;

class SCRIBUS_API CMSPrefs : public QWidget, Ui::CMSPrefsBase
{
	Q_OBJECT

public:
	CMSPrefs( QWidget* parent, CMSData *Vor, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles);
	~CMSPrefs() {};

	void restoreDefaults(CMSData *prefs, ProfilesL *InputProfiles, ProfilesL *InputProfilesCMYK, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles);
	void updateDocSettings(ScribusDoc* doc);
	CMSData *prefs;
	bool changed;

public slots:
	virtual void slotCMSon();
	virtual void setValues();

signals:
	void cmsOn(bool);
};

#endif // CMSPREFS_H
