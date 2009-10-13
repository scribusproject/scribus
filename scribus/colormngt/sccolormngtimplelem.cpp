/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolormngtimplelem.h"

ScColorProfileImplBase::ScColorProfileImplBase(const ScColorMngtEngine& engine)
                      : m_engine(engine)
{

}

ScColorMngtEngine& ScColorProfileImplBase::engine()
{
	return m_engine;
}

const ScColorMngtEngine& ScColorProfileImplBase::engine() const
{
	return m_engine;
}

ScColorTransformImplBase::ScColorTransformImplBase(const ScColorMngtEngine& engine)
                        : m_engine(engine)
{

}

ScColorMngtEngine& ScColorTransformImplBase::engine()
{
	return m_engine;
}

const ScColorMngtEngine& ScColorTransformImplBase::engine() const
{
	return m_engine;
}