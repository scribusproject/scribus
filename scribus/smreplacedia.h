/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMREPLACEDIA_H
#define SMREPLACEDIA_H

#include "smreplacediabase.h"
#include "styleitem.h"

class SMReplaceDia : public SMReplaceDiaBase {
	Q_OBJECT
public:
	SMReplaceDia(const QStringList &toBeDeleted, const QStringList &replaceOptions);
	~SMReplaceDia();

	QValueList<RemoveItem> items();
};

#endif
