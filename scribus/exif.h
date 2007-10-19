/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef __EXIF_H__
#define __EXIF_H__

/**
	exif.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "qstring.h"
#include "qfile.h"
#include "qimage.h"

typedef enum {
    READ_EXIF = 1,
    READ_IMAGE = 2,
    READ_ALL = 3
}ReadMode_t;

//--------------------------------------------------------------------------
// This structure is used to store jpeg file sections in memory.
typedef struct
{
	uchar *  Data;
	int      Type;
	unsigned Size;
}
Section_t;

typedef unsigned char uchar;

class TagTable
{
	public:
		TagTable ( unsigned short t, const char* d ) : Tag ( t ), Desc ( d ) {}
		unsigned short Tag;
		const char*const Desc;
};

#define MAX_SECTIONS 60
#define PSEUDO_IMAGE_MARKER 0x123; // Extra value.

class ExifData
{
		Section_t Sections[MAX_SECTIONS];
		QString CameraMake;
		QString CameraModel;
		QString DateTime;
		int   Orientation;
		int   Height, Width;
		int   ExifImageLength, ExifImageWidth;
		int   IsColor;
		int   Process;
		int   FlashUsed;
		float FocalLength;
		float ExposureTime;
		float ApertureFNumber;
		float Distance;
		int    Whitebalance;
		int    MeteringMode;
		float CCDWidth;
		float ExposureBias;
		int   ExposureProgram;
		int   ISOequivalent;
		int   CompressionLevel;
		QString UserComment;
		QString Comment;
		int recurseLevel;

		int getch ( QFile &infile );
		int ReadJpegSections ( QFile & infile, ReadMode_t ReadMode );
		void DiscardData ( void );
		int Get16u ( void * Short );
		int Get32s ( void * Long );
		unsigned Get32u ( void * Long );
		double ConvertAnyFormat ( void * ValuePtr, int Format );
		void ProcessExifDir ( unsigned char * DirStart, unsigned char * OffsetBase, unsigned ExifLength );
		void process_COM ( const uchar * Data, int length );
		void process_SOFn ( const uchar * Data, int marker );
		int Get16m ( const void * Short );
		void process_EXIF ( unsigned char * CharBuf, unsigned int length );
		int Exif2tm ( struct tm * timeptr, char * ExifTime );

	public:
		ExifData();
		bool scan ( const QString & );
		QString getCameraMake() { return CameraMake; }
		QString getCameraModel() { return CameraModel; }
		QString getDateTime() { return DateTime; }
		int getOrientation() { return Orientation; }
		int getHeight() { return Height; }
		int getWidth() { return Width; }
		int getIsColor() { return IsColor; }
		int getProcess() { return Process; }
		int getFlashUsed() { return FlashUsed; }
		float getFocalLength() { return FocalLength; }
		float getExposureTime() { return ExposureTime; }
		float getApertureFNumber() { return ApertureFNumber; }
		float getDistance() { return Distance; }
		int getWhitebalance() { return Whitebalance; }
		int getMeteringMode() { return MeteringMode; }
		float getCCDWidth() { return CCDWidth; }
		float getExposureBias() { return ExposureBias; }
		int getExposureProgram() { return ExposureProgram; }
		int getISOequivalent() { return ISOequivalent; }
		int getCompressionLevel() { return CompressionLevel; }
		QString getUserComment() { return UserComment; }
		QString getComment() { return Comment; }
		QImage getThumbnail();
		bool isThumbnailSane();
		bool isNullThumbnail() { return !isThumbnailSane(); }
		bool exifDataValid;
		QImage Thumbnail;
};

class FatalError
{
		const char* ex;
	public:
		FatalError ( const char* s ) { ex = s; }
		void debug_print() const { qDebug ( "exception: " ); }
};

extern TagTable ProcessTable[];

//--------------------------------------------------------------------------
// Define comment writing code, impelemented in setcomment.c
extern int safe_copy_and_modify ( const char * original_filename, const char * comment );

#endif

