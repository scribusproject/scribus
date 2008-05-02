/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef BASEPOINTWIDGET_H
#define BASEPOINTWIDGET_H

#include <QGroupBox>
class QGridLayout;
class QButtonGroup;
class QRadioButton;
class QFrame;

#include "pluginapi.h"

class PLUGIN_API BasePointWidget : public QGroupBox
{
	Q_OBJECT
public:
	BasePointWidget( QWidget* parent );
	~BasePointWidget() {};
	QButtonGroup* RotationGroup;
private:
	QRadioButton* TopLeft;
	QRadioButton* TopRight;
	QRadioButton* Center;
	QRadioButton* BottomLeft;
	QRadioButton* BottomRight;
	QFrame* Line1;
	QFrame* Line2;
	QFrame* Line4;
	QFrame* Line5;
	QGridLayout* Layout12a;
	QGridLayout* Layout12;
};

#endif
