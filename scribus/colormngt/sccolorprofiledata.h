/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORPROFILEDATA_H
#define SCCOLORPROFILEDATA_H

#include <QByteArray>
#include <QString>

#include "icc34.h" //part of lcms1
#include "sccolormngtelem.h"

class ScColorProfileData : public ScColorMngtElem
{
protected:
	QString m_profilePath;
	QString m_profileData;

public:

	QString path() const { return m_profilePath; }

	virtual bool isNull() const = 0;
	virtual icColorSpaceSignature   colorSpace()  const = 0;
	virtual icProfileClassSignature deviceClass() const = 0;
	virtual QString productDescription() const = 0;
};

#endif
