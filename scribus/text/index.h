/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef INDEX_H
#define INDEX_H

#undef NDEBUG

#include "scribusapi.h"
#include <cassert>
#include <vector>
#include <QList>

typedef unsigned int uint;

/**
 * Holds track of an ordered set of integers, e.g. positions of inline frames within a text.
 * When the underlying text is edited, the adjust() method must be used to update the index.
 */

class SCRIBUS_API RunIndex {
private:
	std::vector<uint> runEnds;
	/**
	 *  returns the first idx where runStart(idx) >= pos and pos < runEnd(idx) 
	 *  returns count() if pos > runStart(count()-1)
	 */
	uint search(int pos) const;
	
public:
	
	struct Run {
		uint start;
		uint end;
		Run(uint s, uint e) : start(s), end(e) {}
	};
	
	
	uint runCount() const
	{ 
		return runEnds.size();
	}
	
	
	int runStart(uint idx) const
	{
		assert( idx <= unsigned(runEnds.size()) );
		
		return idx == 0? 0 : runEnds[idx-1];
	}
	
	
	int runEnd(uint idx) const
	{
		return runEnds[idx];
	}
	
	
	/** return the length of the run ending with position idx, usually runEnd(idx) - runEnd(idx-1) */
	int runLength(uint idx) const
	{
		return runEnd(idx) - runStart(idx);
	}
	
	
	int length() const
	{
		return runEnds.size() == 0? 0 : runEnds[runEnds.size()-1];
	}

	
	/** Find the run index for a given position */
	uint operator() (int pos) const
	{
		return search(pos);
	}
	

	Run operator[] (int pos)
	{
		uint idx = search(pos);
		return Run(runStart(idx), runEnd(idx));
	}
	
	
	/** insert a new position and return its index. Splits the run currently covering pos */
	uint insert(int pos);
	
	
	/** remove a run, merging it with the preceding one */
	void remove (uint idx);
	
	
	/** update all positions >= pos with delta */ 
	void adjust(int pos, int delta);
	
	void clear()
	{
		runEnds.clear();
	}
};






template<typename T>
class IndexedList
{
	QList<T> elems;
	RunIndex offsets;
	
public:
	
	T& operator() (int pos)
	{
		return elems[offsets(pos)];
	}
	
	
	bool containsPosition(int pos) const
	{
		return pos >= 0 && offsets(pos) < offsets.runCount();
	}
	
	
	const T& operator() (int pos) const
	{
		return elems[offsets(pos)];
	}
	
	
	void adjust(int pos, int delta)
	{
		offsets.adjust(pos, delta);
	}
	
	
	T& operator[] (uint idx)
	{
		return elems[idx];
	}
	
	const T& operator[] (uint idx) const
	{
		return elems[idx];
	}
	
	
	T& at(uint idx)
	{
		return elems.at(idx);
	}
	
	const T& at(uint idx) const
	{
		return elems.at(idx);
	}

	int count() const { return elems.size(); }

	bool isExactPosition(int pos) const
	{
		return pos == 0 || pos == exactPositionAfter(pos-1);
	}

	int exactPositionBefore(int pos) const
	{
		return offsets.runStart(offsets(pos));
	}

	int exactPositionAfter(int pos) const
	{
		return offsets.runEnd(offsets(pos));
	}

	void removeAt(uint idx)
	{
		elems.removeAt(idx);
		offsets.remove(idx);
	}

	void remove(int pos)
	{
		uint idx = offsets(pos);
		removeAt(idx);
	}

	void insert(int pos, const T& elem)
	{
		uint idx = offsets.insert(pos);
		elems.insert(idx, elem);
	}

	void replace(int pos, uint len, const T& elem)
	{
		if (!isExactPosition(pos + len))
		{
			insert(pos + len, (*this)(pos + len));
		}
		insert(pos, elem);
		uint idx1 = offsets(pos) + 1;
		uint idx2 = offsets(pos + len);
		for (uint i = idx1; i < idx2; ++i)
		{
			removeAt(idx1);
		}
	}

	void clear()
	{
		offsets.clear();
		elems.clear();
	}
};

#endif
