/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCCOLORMGMTIMPLELEM_H
#define SCCOLORMGMTIMPLELEM_H

#include "sccolormgmtelem.h"
#include "sccolormgmtengine.h"

class ScColorProfileImplBase : public ScColorProfileData
{
protected:
	ScColorMgmtEngine m_engine;

public:
	ScColorProfileImplBase(const ScColorMgmtEngine& engine);

	virtual ScColorMgmtEngine& engine();
	virtual const ScColorMgmtEngine& engine() const;
};

class ScColorTransformImplBase : public ScColorTransformData
{
protected:
	ScColorMgmtEngine m_engine;

public:
	ScColorTransformImplBase(const ScColorMgmtEngine& engine);

	virtual ScColorMgmtEngine& engine();
	virtual const ScColorMgmtEngine& engine() const;
};

#endif