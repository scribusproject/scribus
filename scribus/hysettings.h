/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef HYSETTINGS_H
#define HYSETTINGS_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QGridLayout>

#include "scribusapi.h"
#include "prefsstructs.h"
class ScribusDoc;

class SCRIBUS_API HySettings : public QWidget
{ 
	Q_OBJECT

public:
	HySettings( QWidget* parent, QMap<QString,QString>* langs);
	~HySettings() {};
	void restoreDefaults(struct ApplicationPrefs *prefsData);
	void restoreDefaults(ScribusDoc *prefsData);

	QCheckBox* verbose;
	QCheckBox* input;
	QComboBox* language;
	QLabel* text1;
	QLabel* text2;
	QLabel* text3;
	QSpinBox* wordLen;
	QSpinBox* maxCount;

protected:
	QMap<QString,QString> langsMap;
	QGridLayout* layout3;
};

#endif // HYSETTINGS_H
