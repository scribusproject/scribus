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
public:
	ScColorProfileImplBase(const ScColorMgmtEngine& engine);

	ScColorMgmtEngine& engine() override;
	const ScColorMgmtEngine& engine() const override;

protected:
	ScColorMgmtEngine m_engine;
};

class ScColorTransformImplBase : public ScColorTransformData
{
public:
	ScColorTransformImplBase(const ScColorMgmtEngine& engine);

	ScColorMgmtEngine& engine() override;
	const ScColorMgmtEngine& engine() const override;

protected:
	ScColorMgmtEngine m_engine;
};

#endif
