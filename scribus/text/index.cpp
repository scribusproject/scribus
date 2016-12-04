/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include <QDebug>

#include "index.h"



// find run with runStart <= pos < runEnd
uint RunIndex::search(int pos) const
{
	std::vector<uint>::const_iterator it = std::upper_bound(runEnds.begin(), runEnds.end(), pos);
	return it - runEnds.begin();
}


uint RunIndex::insert(int pos)
{
	uint i = search(pos);
	
	if (i >= runEnds.size())
	{
		runEnds.push_back(pos);
		return runEnds.size() - 1;
	}
	else
	{
		runEnds.insert(runEnds.begin() + i, pos);
		return i;
	}
}


void RunIndex::remove (uint idx)
{
	assert ( idx < runEnds.size() );

	runEnds.erase(runEnds.begin() + idx);
}


void RunIndex::adjust(int pos, int delta)
{
	uint idx = search(pos);
	for (uint i = idx; i < runEnds.size(); ++i)
	{
		runEnds[i] += delta;
	}
}


