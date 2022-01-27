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
	int type = 0;
	int copyCount = 0;
	int copyShiftOrGap = 0;
	double copyShiftGapH = 0.0;
	double copyShiftGapV = 0.0;
	double copyRotation = 0.0;
	int gridRows = 0;
	int gridCols = 0;
	double gridGapH = 0.0;
	double gridGapV = 0.0;
	int pageSelection = 0; // 1 = All, 2 = Even, 3 = Odd, 4 = Range
	QString pageRange;
	bool pageLinkText = false;
};

struct CreateRangeData
{
	QString pageRange;
};

#endif
