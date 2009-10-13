/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCCOLORMNGTIMPLELEM_H
#define SCCOLORMNGTIMPLELEM_H

#include "sccolormngtelem.h"
#include "sccolormngtengine.h"

class ScColorProfileImplBase : public ScColorProfileData
{
protected:
	ScColorMngtEngine m_engine;

public:
	ScColorProfileImplBase(const ScColorMngtEngine& engine);

	virtual ScColorMngtEngine& engine();
	virtual const ScColorMngtEngine& engine() const;
};

class ScColorTransformImplBase : public ScColorTransformData
{
protected:
	ScColorMngtEngine m_engine;

public:
	ScColorTransformImplBase(const ScColorMngtEngine& engine);

	virtual ScColorMngtEngine& engine();
	virtual const ScColorMngtEngine& engine() const;
};

#endif