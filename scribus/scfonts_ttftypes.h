#ifndef SCFONTS_TTFTYPES_H
#define SCFONTS_TTFTYPES_H

#if defined(__hpux)
  typedef unsigned char u_int8_t;
  typedef unsigned short u_int16_t;
  typedef unsigned int u_int32_t;
#endif

#include "config.h"
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include <sys/types.h>

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) (a<<24|b<<16|c<<8|d)
#endif

struct fxp16_16_t
{
	int16_t mantissa;
	uint16_t fraction;
};

struct tt_datetime_t
{
	char data[8];
};

typedef uint16_t fxp14_2_t;


struct OffsetTable
{
	fxp16_16_t version;
	uint16_t numTables;
	uint16_t searchRange;
	uint16_t entrySelector;
	uint16_t rangeShift;
}; // 12 bytes


struct TableDirEntry
{
	uint32_t tag;
	uint32_t checkSum;
	uint32_t offset;
	uint32_t length;
}; // 16 bytes


struct HeadTable
{
	fxp16_16_t version;
	fxp16_16_t fontRevision;
	uint32_t checkSumAdjustment;
	uint32_t magicNumber;
	uint16_t flags;
	uint16_t unitsPerEm;
	tt_datetime_t created;
	tt_datetime_t modified;
	int16_t xMin;
	int16_t yMin;
	int16_t xMax;
	int16_t yMax;
	uint16_t macStyle;
	uint16_t lowestRecPPEM;
	uint16_t fontDirectionHint;
	int16_t indexToLocFormat;
	int16_t glyphDataFormat;
};

/*------------------------------------------------------------*/

struct NameRecord
{
	uint16_t platformID;
	uint16_t encodingID;
	uint16_t languageID;
	uint16_t nameID;
	uint16_t length;
	uint16_t offset;
};

/*------------------------------------------------------------*/

struct PostTable
{
	fxp16_16_t formatType;
	fxp16_16_t italicAngle;
	int16_t underlinePosition;
	int16_t underlineThickness;
	int32_t isFixedPitch;
	int32_t minMemType42;
	uint32_t maxMemType42;
	uint32_t minMemType1;
	uint32_t maxMemType1;
};

struct Post_GlyphName
{
	int type;
	int index;
	QString *name;
};


/*-----------------------------------------------------------------*/


struct CmapTable
{
	uint16_t version;
	uint16_t	numTables;
};

struct CmapHeader
{
	uint16_t platformID;
	uint16_t encodingID;
	uint32_t offset;
};

struct CmapFormat0
{
	uint16_t format;
	uint16_t length;
	uint16_t language;
	char glyphIdArray[256];
};

struct CmapFormat4
{
	uint16_t format;
	uint16_t length;
	uint16_t language;
	uint16_t segCountX2;
	uint16_t searchRange;
	uint16_t entrySelector;
	uint16_t rangeShift;
	uint16_t *endCount;
	uint16_t reservedPad;
	uint16_t *startCount;
	uint16_t *idDelta;
	uint16_t *idRangeOffset;
	uint16_t *glyphIdArray;
};

/*-----------------------------------------------------------------*/

struct HheaTable
{
	fxp16_16_t version;
	int16_t Ascender;
	int16_t Descender;
	int16_t LineGap;
	uint16_t advanceWidthMax;
	int16_t minLeftSideBearing;
	int16_t minRightSideBearing;
	int16_t xMaxExtent;
	int16_t caretSlopeRise;
	int16_t caretSlopeRun;
	int16_t reserved[5];
	int16_t metricDataFormat;
	uint16_t numberOfHMetrics;
};

struct tt_Box
{
	int16_t xMin;
	int16_t yMin;
	int16_t xMax;
	int16_t yMax;
};

struct tt_longHorMetric
{
	uint16_t advanceWidth;
	int16_t lsb;
};

/*------------------------------------------------------------*/

struct KernTable
{
	uint16_t version;
	uint16_t nTables;
};

struct KernSubTableHeader
{
	uint16_t version;
	uint16_t length;
	uint16_t coverage;
};

#define kernHorizontal 0x0001
#define kernMinimum 0x0002
#define kernCrossStream 0x0004
#define kernOverride 0x0008
#define kernFormat(coverage) ((coverage)>>8)

struct KernSubTable
{
	uint16_t nPairs;
	uint16_t searchRange;
	uint16_t entrySelector;
	uint16_t rangeShift;
};

struct KernEntry
{
	uint16_t left;
	uint16_t right;
	int16_t value;
};

#endif
