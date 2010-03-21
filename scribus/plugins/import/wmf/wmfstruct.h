/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/* WMF Metafile Structures
 * Author: Stefan Taferner <taferner@kde.org>
 */
#ifndef wmfstruct_h
#define wmfstruct_h

typedef qint16 WORD16;
typedef qint32 WORD32;
typedef qint32 LONG32;
typedef void* _HANDLE;

typedef struct _RECT16
{
    WORD16 left;
    WORD16 top;
    WORD16 right;
    WORD16 bottom;
} RECT16;

typedef struct _RECT32
{
    LONG32 left;
    LONG32 top;
    LONG32 right;
    LONG32 bottom;
} RECT32;

typedef struct _SIZE16
{
    WORD16 width;
    WORD16 height;
} SIZE16;

typedef struct _SIZE32
{
    LONG32 width;
    LONG32 height;
} SIZE32;


struct WmfEnhMetaHeader
{
    WORD32   iType;              // Record type EMR_HEADER
    WORD32   nSize;              // Record SIZE16 in bytes.  This may be greater
    // than the sizeof( ENHMETAHEADER ).
    RECT32   rclBounds;          // Inclusive-inclusive bounds in device units
    RECT32   rclFrame;           // Inclusive-inclusive Picture Frame of metafile
    // in .01 mm units
    WORD32   dSignature;         // Signature.  Must be ENHMETA_SIGNATURE.
    WORD32   nVersion;           // Version number
    WORD32   nBytes;             // SIZE16 of the metafile in bytes
    WORD32   nRecords;           // Number of records in the metafile
    WORD16    nHandles;           // Number of handles in the handle table
    // Handle index zero is reserved.
    WORD16    sReserved;          // Reserved.  Must be zero.
    WORD32   nDescription;       // Number of chars in the unicode description string
    // This is 0 if there is no description string
    WORD32   offDescription;     // Offset to the metafile description record.
    // This is 0 if there is no description string
    WORD32   nPalEntries;        // Number of entries in the metafile palette.
    SIZE32   szlDevice;          // SIZE16 of the reference device in pels
    SIZE32   szlMillimeters;     // SIZE16 of the reference device in millimeters
};
#define ENHMETA_SIGNATURE       0x464D4520


struct WmfMetaHeader
{
    WORD16        mtType;
    WORD16        mtHeaderSize;
    WORD16        mtVersion;
    WORD32       mtSize;
    WORD16        mtNoObjects;
    WORD32       mtMaxRecord;
    WORD16        mtNoParameters;
};


struct WmfPlaceableHeader
{
    WORD32 key;
    WORD16 hmf;
    RECT16 bbox;
    WORD16 inch;
    WORD32 reserved;
    WORD16 checksum;
};
#define APMHEADER_KEY 0x9AC6CDD7


struct WmfMetaRecord
{
    WORD32 rdSize;       // Record SIZE16 ( in words ) of the function
    WORD16  rdFunction;   // Record function number
    WORD16  rdParm[ 1 ];  // WORD16 array of parameters
};


struct WmfEnhMetaRecord
{
    WORD32 iType;        // Record type EMR_xxx
    WORD32 nSize;        // Record SIZE16 in bytes
    WORD32 dParm[ 1 ];   // WORD32 array of parameters
};


#endif /*wmfstruct_h*/
