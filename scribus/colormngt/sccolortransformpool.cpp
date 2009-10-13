/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolormngtengine.h"
#include "sccolormngtstructs.h"
#include "sccolortransformpool.h"

ScColorTransformPool::ScColorTransformPool(int engineID) : m_engineID(engineID)
{

}

void ScColorTransformPool::clear(void)
{
	m_pool.clear();
}

void ScColorTransformPool::addTransform(const ScColorTransform& transform)
{
	// Check engine ID. If different, transform was created by another engine
	//  and we MUST NOT add it to the transform pool
	if (m_engineID != transform.engine().engineID())
		return;
	ScColorTransform trans = findTransform(transform.transformInfo());
	if (trans.isNull())
	{
		m_pool.insert(transform);
	}
}

void ScColorTransformPool::removeTransform(const ScColorTransform& transform)
{
	if (m_engineID != transform.engine().engineID())
		return;
	m_pool.remove(transform);
}

void ScColorTransformPool::removeTransform(const ScColorTransformInfo& info)
{
	QSet<ScColorTransform>::Iterator it = m_pool.begin();
	while (it != m_pool.end())
	{
		if (info == it->transformInfo())
		{
			it = m_pool.erase(it);
			continue;
		}
		++it;
	}
}

ScColorTransform ScColorTransformPool::findTransform(const ScColorTransformInfo& info) const
{
	ScColorTransform transform(NULL);
	QSet<ScColorTransform>::ConstIterator it = m_pool.begin();
	for ( ; it != m_pool.end(); ++it)
	{
		if (info == it->transformInfo())
		{
			transform = *it;
			break;
		}
	}
	return transform;
}