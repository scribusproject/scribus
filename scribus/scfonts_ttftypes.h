#ifndef SCFONTS_TTFTYPES_H
#define SCFONTS_TTFTYPES_H

#include <sys/types.h>

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) (a<<24|b<<16|c<<8|d)
#endif

struct fxp16_16_t
{
	int16_t mantissa;
	u_int16_t fraction;
};

struct tt_datetime_t
{
	char data[8];
};

typedef u_int16_t fxp14_2_t;


struct OffsetTable
{
	fxp16_16_t version;
	u_int16_t numTables;
	u_int16_t searchRange;
	u_int16_t entrySelector;
	u_int16_t rangeShift;
}; // 12 bytes


struct TableDirEntry
{
	u_int32_t tag;
	u_int32_t checkSum;
	u_int32_t offset;
	u_int32_t length;
}; // 16 bytes


struct HeadTable
{
	fxp16_16_t version;
	fxp16_16_t fontRevision;
	u_int32_t checkSumAdjustment;
	u_int32_t magicNumber;
	u_int16_t flags;
	u_int16_t unitsPerEm;
	tt_datetime_t created;
	tt_datetime_t modified;
	int16_t xMin;
	int16_t yMin;
	int16_t xMax;
	int16_t yMax;
	u_int16_t macStyle;
	u_int16_t lowestRecPPEM;
	u_int16_t fontDirectionHint;
	int16_t indexToLocFormat;
	int16_t glyphDataFormat;
};

/*------------------------------------------------------------*/

struct NameRecord
{
	u_int16_t platformID;
	u_int16_t encodingID;
	u_int16_t languageID;
	u_int16_t nameID;
	u_int16_t length;
	u_int16_t offset;
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
	u_int32_t maxMemType42;
	u_int32_t minMemType1;
	u_int32_t maxMemType1;
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
	u_int16_t version;
	u_int16_t	numTables;
};

struct CmapHeader
{
	u_int16_t platformID;
	u_int16_t encodingID;
	u_int32_t offset;
};

struct CmapFormat0
{
	u_int16_t format;
	u_int16_t length;
	u_int16_t language;
	char glyphIdArray[256];
};

struct CmapFormat4
{
	u_int16_t format;
	u_int16_t length;
	u_int16_t language;
	u_int16_t segCountX2;
	u_int16_t searchRange;
	u_int16_t entrySelector;
	u_int16_t rangeShift;
	u_int16_t *endCount;
	u_int16_t reservedPad;
	u_int16_t *startCount;
	u_int16_t *idDelta;
	u_int16_t *idRangeOffset;
	u_int16_t *glyphIdArray;
};

/*-----------------------------------------------------------------*/

struct HheaTable
{
	fxp16_16_t version;
	int16_t Ascender;
	int16_t Descender;
	int16_t LineGap;
	u_int16_t advanceWidthMax;
	int16_t minLeftSideBearing;
	int16_t minRightSideBearing;
	int16_t xMaxExtent;
	int16_t caretSlopeRise;
	int16_t caretSlopeRun;
	int16_t reserved[5];
	int16_t metricDataFormat;
	u_int16_t numberOfHMetrics;
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
	u_int16_t advanceWidth;
	int16_t lsb;
};

/*------------------------------------------------------------*/

struct KernTable
{
	u_int16_t version;
	u_int16_t nTables;
};

struct KernSubTableHeader
{
	u_int16_t version;
	u_int16_t length;
	u_int16_t coverage;
};

#define kernHorizontal 0x0001
#define kernMinimum 0x0002
#define kernCrossStream 0x0004
#define kernOverride 0x0008
#define kernFormat(coverage) ((coverage)>>8)

struct KernSubTable
{
	u_int16_t nPairs;
	u_int16_t searchRange;
	u_int16_t entrySelector;
	u_int16_t rangeShift;
};

struct KernEntry
{
	u_int16_t left;
	u_int16_t right;
	int16_t value;
};

#endif
