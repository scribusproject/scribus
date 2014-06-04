/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FILEDIALOGEVENTCATCHER_H
#define FILEDIALOGEVENTCATCHER_H

#include <QObject>

#include "scribusapi.h"

class SCRIBUS_API FileDialogEventCatcher : public QObject
{
	Q_OBJECT

public:
	FileDialogEventCatcher(QObject* parent);

protected:
	virtual bool eventFilter(QObject *o, QEvent *e);
	
signals:
	void escapePressed();
	void dropLocation(QString dl);
	void desktopPressed();
	void homePressed();
	void parentPressed();
	void enterSelectedPressed();
};

#endif
