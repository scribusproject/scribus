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

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <QString>
#include <QFile>
#include <QImage>

enum ReadMode_t
{
	READ_EXIF = 1,
	READ_IMAGE = 2,
	READ_ALL = 3
};

//--------------------------------------------------------------------------
// This structure is used to store jpeg file sections in memory.
/*
typedef struct
{
	uchar *  Data;
	int      Type;
	unsigned Size;
}
Section_t;
*/
using uchar = unsigned char;

class TagTable
{
	public:
		TagTable(unsigned short t, const char* d) : Tag (t), Desc (d) {}
		unsigned short Tag;
		const char*const Desc;
};

#define MAX_SECTIONS 60
#define PSEUDO_IMAGE_MARKER 0x123; // Extra value.

class ExifData
{
	public:
		ExifData() = default;

		bool scan ( const QString & );
		QString getCameraMake() const { return CameraMake; }
		QString getCameraModel() const { return CameraModel; }
		QString getDateTime() const { return DateTime; }
		int getOrientation() const { return Orientation; }
		int getHeight() const { return Height; }
		int getWidth() const { return Width; }
		int getIsColor() const { return IsColor; }
		int getProcess() const { return Process; }
		int getFlashUsed() const { return FlashUsed; }
		float getFocalLength() const { return FocalLength; }
		float getExposureTime() const { return ExposureTime; }
		float getApertureFNumber() const { return ApertureFNumber; }
		float getDistance() const { return Distance; }
		int getWhitebalance() const { return Whitebalance; }
		int getMeteringMode() const { return MeteringMode; }
		float getCCDWidth() const { return CCDWidth; }
		float getExposureBias() const { return ExposureBias; }
		int getExposureProgram() const { return ExposureProgram; }
		int getISOequivalent() const { return ISOequivalent; }
		int getCompressionLevel() const { return CompressionLevel; }
		QString getUserComment() const { return UserComment; }
		QString getComment() const { return Comment; }
		QImage getThumbnail() const;
		bool isThumbnailSane();
		bool isNullThumbnail() { return !isThumbnailSane(); }

		bool exifDataValid { false };
		QImage Thumbnail;

	private:
		//	Section_t Sections[MAX_SECTIONS];
		QString CameraMake;
		QString CameraModel;
		QString DateTime;
		int Orientation { 0 };
		int orientationCount { 0 };
		int Height { 0 };
		int Width { 0 };
		int ExifImageLength { 0 };
		int ExifImageWidth { 0 };
		int IsColor { 0 };
		int Process { 0 };
		int FlashUsed { 0 };
		float FocalLength { 0.0f };
		float ExposureTime { 0.0f };
		float ApertureFNumber { 0.0f };
		float Distance { 0.0f };
		int Whitebalance { -1 };
		int MeteringMode { -1 };
		float CCDWidth { 0.0f };
		float ExposureBias { 0.0f };
		int ExposureProgram { 0 };
		int ISOequivalent { 0 };
		int CompressionLevel { 0 };
		QString UserComment;
		QString Comment;
		int recurseLevel { 0 };
		unsigned char* LastExifRefd { nullptr };
		int ExifSettingsLength { 0 };
		double FocalplaneXRes { 0.0 };
		double FocalplaneUnits { 0.0 };
		int MotorolaOrder { 0 };

		int getch(QFile& infile);
		int ReadJpegSections(QFile& infile, ReadMode_t ReadMode);
		void DiscardData();
		int Get16u(void* Short);
		int Get32s(void* Long);
		unsigned Get32u(void* Long);
		double ConvertAnyFormat(void* ValuePtr, int Format);
		void ProcessExifDir(unsigned char* DirStart, unsigned char* OffsetBase, unsigned ExifLength);
		void process_COM(const uchar* Data, int length);
		void process_SOFn(const uchar* Data, int marker);
		int Get16m(const void* Short);
		void process_EXIF(unsigned char* CharBuf, unsigned int length);
		int Exif2tm(struct tm* timeptr, char* ExifTime);
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
// Define comment writing code, implemented in setcomment.c
extern int safe_copy_and_modify ( const char * original_filename, const char * comment );

#endif

