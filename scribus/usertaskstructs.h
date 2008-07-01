/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef USERTASKSTRUCTS_H
#define USERTASKSTRUCTS_H

#include <QString>

#include "gtgettext.h"
#include "pageitem.h"

struct InsertAFrameData
{
	PageItem::ItemType frameType;
	int locationType;
	QString pageList;
	int positionType;
	int sizeType;
	double x;
	double y;
	double width;
	double height;
	QString source;
	ImportSetup impsetup;
	int columnCount;
	double columnGap;
	bool linkTextFrames;
	bool linkToExistingFrame;
	PageItem* linkToExistingFramePtr;
};

struct ItemMultipleDuplicateData
{
	int type;
	int copyCount;
	int copyShiftOrGap;
	double copyShiftGapH;
	double copyShiftGapV;
	double copyRotation;
	int gridRows;
	int gridCols;
	double gridGapH;
	double gridGapV;
};

struct CreateRangeData
{
	QString pageRange;
};

#endif
