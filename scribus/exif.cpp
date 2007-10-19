/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
//--------------------------------------------------------------------------
// Program to pull the information out of various types of EFIF digital
// camera files and show it in a reasonably consistent way
//
// This module parses the very complicated exif structures.
//
// Matthias Wandel,  Dec 1999 - August 2000
//--------------------------------------------------------------------------


#include "exif.h"
#include <QMatrix>


static unsigned char * LastExifRefd;
static int ExifSettingsLength;
static double FocalplaneXRes;
static double FocalplaneUnits;
static int MotorolaOrder = 0;
static int SectionsRead;
//static int HaveAll;

//--------------------------------------------------------------------------
// Table of Jpeg encoding process names

#define M_SOF0  0xC0            // Start Of Frame N
#define M_SOF1  0xC1            // N indicates which compression process
#define M_SOF2  0xC2            // Only SOF0-SOF2 are now in common use
#define M_SOF3  0xC3
#define M_SOF5  0xC5            // NB: codes C4 and CC are NOT SOF markers
#define M_SOF6  0xC6
#define M_SOF7  0xC7
#define M_SOF9  0xC9
#define M_SOF10 0xCA
#define M_SOF11 0xCB
#define M_SOF13 0xCD
#define M_SOF14 0xCE
#define M_SOF15 0xCF
#define M_SOI   0xD8            // Start Of Image (beginning of datastream)
#define M_EOI   0xD9            // End Of Image (end of datastream)
#define M_SOS   0xDA            // Start Of Scan (begins compressed data)
#define M_JFIF  0xE0            // Jfif marker
#define M_EXIF  0xE1            // Exif marker
#define M_COM   0xFE            // COMment


TagTable ProcessTable[] =
    {
        TagTable ( M_SOF0,   "Baseline" ),
        TagTable ( M_SOF1,   "Extended sequential" ),
        TagTable ( M_SOF2,   "Progressive" ),
        TagTable ( M_SOF3,   "Lossless" ),
        TagTable ( M_SOF5,   "Differential sequential" ),
        TagTable ( M_SOF6,   "Differential progressive" ),
        TagTable ( M_SOF7,   "Differential lossless" ),
        TagTable ( M_SOF9,   "Extended sequential, arithmetic coding" ),
        TagTable ( M_SOF10,  "Progressive, arithmetic coding" ),
        TagTable ( M_SOF11,  "Lossless, arithmetic coding" ),
        TagTable ( M_SOF13,  "Differential sequential, arithmetic coding" ),
        TagTable ( M_SOF14,  "Differential progressive, arithmetic coding" ),
        TagTable ( M_SOF15,  "Differential lossless, arithmetic coding" ),
        TagTable ( 0,        "Unknown" )
    };

//--------------------------------------------------------------------------
// Describes format descriptor
static int BytesPerFormat[] = {0,1,1,2,4,8,1,1,2,4,8,4,8};
#define NUM_FORMATS 12

#define FMT_BYTE       1
#define FMT_STRING     2
#define FMT_USHORT     3
#define FMT_ULONG      4
#define FMT_URATIONAL  5
#define FMT_SBYTE      6
#define FMT_UNDEFINED  7
#define FMT_SSHORT     8
#define FMT_SLONG      9
#define FMT_SRATIONAL 10
#define FMT_SINGLE    11
#define FMT_DOUBLE    12

//--------------------------------------------------------------------------
// Describes tag values

#define TAG_EXIF_OFFSET       0x8769
#define TAG_INTEROP_OFFSET    0xa005

#define TAG_MAKE              0x010F
#define TAG_MODEL             0x0110
#define TAG_ORIENTATION       0x0112

#define TAG_EXPOSURETIME      0x829A
#define TAG_FNUMBER           0x829D

#define TAG_SHUTTERSPEED      0x9201
#define TAG_APERTURE          0x9202
#define TAG_MAXAPERTURE       0x9205
#define TAG_FOCALLENGTH       0x920A

#define TAG_DATETIME_ORIGINAL 0x9003
#define TAG_USERCOMMENT       0x9286

#define TAG_SUBJECT_DISTANCE  0x9206
#define TAG_FLASH             0x9209

#define TAG_FOCALPLANEXRES    0xa20E
#define TAG_FOCALPLANEUNITS   0xa210
#define TAG_EXIF_IMAGEWIDTH   0xA002
#define TAG_EXIF_IMAGELENGTH  0xA003

// the following is added 05-jan-2001 vcs
#define TAG_EXPOSURE_BIAS     0x9204
#define TAG_WHITEBALANCE      0x9208
#define TAG_METERING_MODE     0x9207
#define TAG_EXPOSURE_PROGRAM  0x8822
#define TAG_ISO_EQUIVALENT    0x8827
#define TAG_COMPRESSION_LEVEL 0x9102

#define TAG_THUMBNAIL_OFFSET  0x0201
#define TAG_THUMBNAIL_LENGTH  0x0202


/*static TagTable_t TagTable[] = {
  {   0x100,   "ImageWidth"},
  {   0x101,   "ImageLength"},
  {   0x102,   "BitsPerSample"},
  {   0x103,   "Compression"},
  {   0x106,   "PhotometricInterpretation"},
  {   0x10A,   "FillOrder"},
  {   0x10D,   "DocumentName"},
  {   0x10E,   "ImageDescription"},
  {   0x10F,   "Make"},
  {   0x110,   "Model"},
  {   0x111,   "StripOffsets"},
  {   0x112,   "Orientation"},
  {   0x115,   "SamplesPerPixel"},
  {   0x116,   "RowsPerStrip"},
  {   0x117,   "StripByteCounts"},
  {   0x11A,   "XResolution"},
  {   0x11B,   "YResolution"},
  {   0x11C,   "PlanarConfiguration"},
  {   0x128,   "ResolutionUnit"},
  {   0x12D,   "TransferFunction"},
  {   0x131,   "Software"},
  {   0x132,   "DateTime"},
  {   0x13B,   "Artist"},
  {   0x13E,   "WhitePoint"},
  {   0x13F,   "PrimaryChromaticities"},
  {   0x156,   "TransferRange"},
  {   0x200,   "JPEGProc"},
  {   0x201,   "ThumbnailOffset"},
  {   0x202,   "ThumbnailLength"},
  {   0x211,   "YCbCrCoefficients"},
  {   0x212,   "YCbCrSubSampling"},
  {   0x213,   "YCbCrPositioning"},
  {   0x214,   "ReferenceBlackWhite"},
  {   0x828D,  "CFARepeatPatternDim"},
  {   0x828E,  "CFAPattern"},
  {   0x828F,  "BatteryLevel"},
  {   0x8298,  "Copyright"},
  {   0x829A,  "ExposureTime"},
  {   0x829D,  "FNumber"},
  {   0x83BB,  "IPTC/NAA"},
  {   0x8769,  "ExifOffset"},
  {   0x8773,  "InterColorProfile"},
  {   0x8822,  "ExposureProgram"},
  {   0x8824,  "SpectralSensitivity"},
  {   0x8825,  "GPSInfo"},
  {   0x8827,  "ISOSpeedRatings"},
  {   0x8828,  "OECF"},
  {   0x9000,  "ExifVersion"},
  {   0x9003,  "DateTimeOriginal"},
  {   0x9004,  "DateTimeDigitized"},
  {   0x9101,  "ComponentsConfiguration"},
  {   0x9102,  "CompressedBitsPerPixel"},
  {   0x9201,  "ShutterSpeedValue"},
  {   0x9202,  "ApertureValue"},
  {   0x9203,  "BrightnessValue"},
  {   0x9204,  "ExposureBiasValue"},
  {   0x9205,  "MaxApertureValue"},
  {   0x9206,  "SubjectDistance"},
  {   0x9207,  "MeteringMode"},
  {   0x9208,  "LightSource"},
  {   0x9209,  "Flash"},
  {   0x920A,  "FocalLength"},
  {   0x927C,  "MakerNote"},
  {   0x9286,  "UserComment"},
  {   0x9290,  "SubSecTime"},
  {   0x9291,  "SubSecTimeOriginal"},
  {   0x9292,  "SubSecTimeDigitized"},
  {   0xA000,  "FlashPixVersion"},
  {   0xA001,  "ColorSpace"},
  {   0xA002,  "ExifImageWidth"},
  {   0xA003,  "ExifImageLength"},
  {   0xA005,  "InteroperabilityOffset"},
  {   0xA20B,  "FlashEnergy"},                 // 0x920B in TIFF/EP
  {   0xA20C,  "SpatialFrequencyResponse"},  // 0x920C    -  -
  {   0xA20E,  "FocalPlaneXResolution"},     // 0x920E    -  -
  {   0xA20F,  "FocalPlaneYResolution"},      // 0x920F    -  -
  {   0xA210,  "FocalPlaneResolutionUnit"},  // 0x9210    -  -
  {   0xA214,  "SubjectLocation"},             // 0x9214    -  -
  {   0xA215,  "ExposureIndex"},            // 0x9215    -  -
  {   0xA217,  "SensingMethod"},            // 0x9217    -  -
  {   0xA300,  "FileSource"},
  {   0xA301,  "SceneType"},
  {      0, NULL}
} ;
*/

int ExifData::getch ( QFile &infile )
{
	QByteArray a = infile.read ( 1 );
	uint r = 0;
	r = static_cast<uint> ( a[0] );
	r &= 0x000000FF;
	return static_cast<int> ( r );
}
//--------------------------------------------------------------------------
// Parse the marker stream until SOS or EOI is seen;
//--------------------------------------------------------------------------
int ExifData::ReadJpegSections ( QFile & infile, ReadMode_t ReadMode )
{
	int a;

	a = getch ( infile );

	if ( a != 0xff || getch ( infile ) != M_SOI )
	{
		SectionsRead = 0;
		return false;
	}
	for ( SectionsRead = 0; SectionsRead < MAX_SECTIONS-1; )
	{
		int marker = 0;
		int got;
		unsigned int ll,lh;
		unsigned int itemlen;
		uchar * Data;

		for ( a=0;a<7;a++ )
		{
			marker = getch ( infile );
			if ( marker != 0xff ) break;

			if ( a >= 6 )
			{

				qDebug ( "too many padding bytes" );
				return false;

			}
		}

		if ( marker == 0xff )
		{
			// 0xff is legal padding, but if we get that many, something's wrong.
			return false;
//            throw FatalError("too many padding bytes!");
		}

		Sections[SectionsRead].Type = marker;

		// Read the length of the section.
		lh = ( uchar ) getch ( infile );
		ll = ( uchar ) getch ( infile );

		itemlen = ( lh << 8 ) | ll;

		if ( itemlen < 2 )
		{
			return false;
//           throw FatalError("invalid marker");
		}

		Sections[SectionsRead].Size = itemlen;

		Data = ( uchar * ) malloc ( itemlen+1 ); // Add 1 to allow sticking a 0 at the end.
		Sections[SectionsRead].Data = Data;

		// Store first two pre-read bytes.
		Data[0] = ( uchar ) lh;
		Data[1] = ( uchar ) ll;

		got = infile.read ( ( char* ) Data+2, itemlen-2 ); // Read the whole section.
		if ( ( unsigned ) got != itemlen-2 )
		{
			return false;
//            throw FatalError("reading from file");
		}
		SectionsRead++;

		switch ( marker )
		{

			case M_SOS:   // stop before hitting compressed data
				// If reading entire image is requested, read the rest of the data.
				if ( ReadMode & READ_IMAGE )
				{
					unsigned long size;

					size = qMax ( 0ll, infile.size()-infile.pos() );
					Data = ( uchar * ) malloc ( size );
					if ( Data == NULL )
					{
						return false;
//                        throw FatalError("could not allocate data for entire image");
					}

					got = infile.read ( ( char* ) Data,  size );
					if ( ( unsigned ) got != size )
					{
						return false;
//                       throw FatalError("could not read the rest of the image");
					}

					Sections[SectionsRead].Data = Data;
					Sections[SectionsRead].Size = size;
					Sections[SectionsRead].Type = PSEUDO_IMAGE_MARKER;
					SectionsRead ++;
					//HaveAll = 1;
				}
				return true;

			case M_EOI:   // in case it's a tables-only JPEG stream
				qDebug ( "No image in jpeg!" );
				return false;

			case M_COM: // Comment section
				// pieczy 2002-02-12
				// now the User comment goes to UserComment
				// so we can store a Comment section also in READ_EXIF mode
				process_COM ( Data, itemlen );
				break;

			case M_JFIF:
				// Regular jpegs always have this tag, exif images have the exif
				// marker instead, althogh ACDsee will write images with both markers.
				// this program will re-create this marker on absence of exif marker.
				// hence no need to keep the copy from the file.
				free ( Sections[--SectionsRead].Data );
				break;

			case M_EXIF:
				// Seen files from some 'U-lead' software with Vivitar scanner
				// that uses marker 31 for non exif stuff.  Thus make sure
				// it says 'Exif' in the section before treating it as exif.
				if ( ( ReadMode & READ_EXIF ) && memcmp ( Data+2, "Exif", 4 ) == 0 )
				{
					process_EXIF ( ( uchar * ) Data, itemlen ); // FIXME: This call
					// requires Data to be array of at least 8 bytes. Code
					// above only checks for itemlen < 2.
					exifDataValid = true;
				}
				else
				{
					// Discard this section.
					free ( Sections[--SectionsRead].Data );
//                    return false;
				}
				break;

			case M_SOF0:
			case M_SOF1:
			case M_SOF2:
			case M_SOF3:
			case M_SOF5:
			case M_SOF6:
			case M_SOF7:
			case M_SOF9:
			case M_SOF10:
			case M_SOF11:
			case M_SOF13:
			case M_SOF14:
			case M_SOF15:
				process_SOFn ( Data, marker ); //FIXME: This call requires Data to
				// be array of at least 8 bytes. Code above only checks for
				// itemlen < 2.
			default:
				break;
				break;
		}
	}
	return true;
}


//--------------------------------------------------------------------------
// Discard read data.
//--------------------------------------------------------------------------
void ExifData::DiscardData ( void )
{
	for ( int a=0; a < SectionsRead; a++ )
		free ( Sections[a].Data );
	SectionsRead = 0;
}

//--------------------------------------------------------------------------
// Convert a 16 bit unsigned value from file's native byte order
//--------------------------------------------------------------------------
int ExifData::Get16u ( void * Short )
{
	if ( MotorolaOrder )
	{
		return ( ( ( uchar * ) Short ) [0] << 8 ) | ( ( uchar * ) Short ) [1];
	}
	else
	{
		return ( ( ( uchar * ) Short ) [1] << 8 ) | ( ( uchar * ) Short ) [0];
	}
}

//--------------------------------------------------------------------------
// Convert a 32 bit signed value from file's native byte order
//--------------------------------------------------------------------------
int ExifData::Get32s ( void * Long )
{
	if ( MotorolaOrder )
	{
		return ( ( ( char * ) Long ) [0] << 24 ) | ( ( ( uchar * ) Long ) [1] << 16 )
		       | ( ( ( uchar * ) Long ) [2] << 8 ) | ( ( ( uchar * ) Long ) [3] << 0 );
	}
	else
	{
		return ( ( ( char * ) Long ) [3] << 24 ) | ( ( ( uchar * ) Long ) [2] << 16 )
		       | ( ( ( uchar * ) Long ) [1] << 8 ) | ( ( ( uchar * ) Long ) [0] << 0 );
	}
}

//--------------------------------------------------------------------------
// Convert a 32 bit unsigned value from file's native byte order
//--------------------------------------------------------------------------
unsigned ExifData::Get32u ( void * Long )
{
	return ( unsigned ) Get32s ( Long ) & 0xffffffff;
}

//--------------------------------------------------------------------------
// Evaluate number, be it int, rational, or float from directory.
//--------------------------------------------------------------------------
double ExifData::ConvertAnyFormat ( void * ValuePtr, int Format )
{
	double Value;
	Value = 0;

	switch ( Format )
	{
		case FMT_SBYTE:     Value = * ( signed char * ) ValuePtr;  break;
		case FMT_BYTE:      Value = * ( uchar * ) ValuePtr;        break;

		case FMT_USHORT:    Value = Get16u ( ValuePtr );          break;

		case FMT_ULONG:     Value = Get32u ( ValuePtr );          break;

		case FMT_URATIONAL:
		case FMT_SRATIONAL:
		{
			int Num,Den;
			Num = Get32s ( ValuePtr );
			Den = Get32s ( 4+ ( char * ) ValuePtr );
			if ( Den == 0 )
			{
				Value = 0;
			}
			else
			{
				Value = ( double ) Num/Den;
			}
			break;
		}

		case FMT_SSHORT:    Value = ( signed short ) Get16u ( ValuePtr );  break;
		case FMT_SLONG:     Value = Get32s ( ValuePtr );                break;

			// Not sure if this is correct (never seen float used in Exif format)
		case FMT_SINGLE:    Value = ( double ) * ( float * ) ValuePtr;      break;
		case FMT_DOUBLE:    Value = * ( double * ) ValuePtr;             break;
	}
	return Value;
}

//--------------------------------------------------------------------------
// Process one of the nested EXIF directories.
//--------------------------------------------------------------------------
void ExifData::ProcessExifDir ( unsigned char * DirStart, unsigned char * OffsetBase, unsigned ExifLength )
{
	int de;
	int a;
	int NumDirEntries;
	unsigned ThumbnailOffset = 0;
	unsigned ThumbnailSize = 0;
	recurseLevel++;
	if (recurseLevel > 3)
		return;
	NumDirEntries = Get16u ( DirStart );
#define DIR_ENTRY_ADDR(Start, Entry) (Start+2+12*(Entry))

	{
		unsigned char * DirEnd;
		DirEnd = DIR_ENTRY_ADDR ( DirStart, NumDirEntries );
		if ( DirEnd+4 > ( OffsetBase+ExifLength ) )
		{
			if ( DirEnd+2 == OffsetBase+ExifLength || DirEnd == OffsetBase+ExifLength )
			{
				// Version 1.3 of jhead would truncate a bit too much.
				// This also caught later on as well.
			}
			else
			{
				// Note: Files that had thumbnails trimmed with jhead 1.3 or earlier
				// might trigger this.
				return;
//                throw FatalError("Illegally sized directory");
			}
		}
		if ( DirEnd < LastExifRefd ) LastExifRefd = DirEnd;
	}

	for ( de=0;de<NumDirEntries;de++ )
	{
		int Tag, Format, Components;
		unsigned char * ValuePtr;
		int ByteCount;
		char * DirEntry;
		DirEntry = ( char * ) DIR_ENTRY_ADDR ( DirStart, de );

		Tag = Get16u ( DirEntry );
		Format = Get16u ( DirEntry+2 );
		Components = Get32u ( DirEntry+4 );

		if ( ( Format-1 ) >= NUM_FORMATS )
		{
			// (-1) catches illegal zero case as unsigned underflows to positive large.
			return;
//            throw FatalError("Illegal format code in EXIF dir");
		}

		ByteCount = Components * BytesPerFormat[Format];

		if ( ByteCount > 4 )
		{
			unsigned OffsetVal;
			OffsetVal = Get32u ( DirEntry+8 );
			// If its bigger than 4 bytes, the dir entry contains an offset.
			if ( OffsetVal+ByteCount > ExifLength )
			{
				// Bogus pointer offset and / or bytecount value
				//printf("Offset %d bytes %d ExifLen %d\n",OffsetVal, ByteCount, ExifLength);

				return ;
//                throw FatalError("Illegal pointer offset value in EXIF");
			}
			ValuePtr = OffsetBase+OffsetVal;
		}
		else
		{
			// 4 bytes or less and value is in the dir entry itself
			ValuePtr = ( unsigned char * ) DirEntry+8;
		}

		if ( LastExifRefd < ValuePtr+ByteCount )
		{
			// Keep track of last byte in the exif header that was actually referenced.
			// That way, we know where the discardable thumbnail data begins.
			LastExifRefd = ValuePtr+ByteCount;
		}

		// Extract useful components of tag
		switch ( Tag )
		{

			case TAG_MAKE:
				ExifData::CameraMake = QString ( ( char* ) ValuePtr );
				break;

			case TAG_MODEL:
				ExifData::CameraModel = QString ( ( char* ) ValuePtr );
				break;

			case TAG_ORIENTATION:
				Orientation = ( int ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_DATETIME_ORIGINAL:
				DateTime = QString ( ( char* ) ValuePtr );
				break;

			case TAG_USERCOMMENT:
				// Olympus has this padded with trailing spaces.  Remove these first.
				for ( a=ByteCount;; )
				{
					a--;
					if ( ( ValuePtr ) [a] == ' ' )
					{
						( ValuePtr ) [a] = '\0';
					}
					else
					{
						break;
					}
					if ( a == 0 ) break;
				}

				// Copy the comment
				if ( memcmp ( ValuePtr, "ASCII",5 ) == 0 )
				{
					for ( a=5;a<10;a++ )
					{
						int c;
						c = ( ValuePtr ) [a];
						if ( c != '\0' && c != ' ' )
						{
							//strncpy(ImageInfo.Comments, (const char*)(a+ValuePtr), 199);
							UserComment.sprintf ( "%s", ( const char* ) ( a+ValuePtr ) );
							break;
						}
					}
				}
				else
				{
					//strncpy(ImageInfo.Comments, (const char*)ValuePtr, 199);
					UserComment.sprintf ( "%s", ( const char* ) ValuePtr );
				}
				break;

			case TAG_FNUMBER:
				// Simplest way of expressing aperture, so I trust it the most.
				// (overwrite previously computd value if there is one)
				ExifData::ApertureFNumber = ( float ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_APERTURE:
			case TAG_MAXAPERTURE:
				// More relevant info always comes earlier, so only use this field if we don't
				// have appropriate aperture information yet.
				if ( ExifData::ApertureFNumber == 0 )
				{
					ExifData::ApertureFNumber
					= ( float ) exp ( ConvertAnyFormat ( ValuePtr, Format ) *log ( 2.0 ) *0.5 );
				}
				break;

			case TAG_FOCALLENGTH:
				// Nice digital cameras actually save the focal length as a function
				// of how far they are zoomed in.
				ExifData::FocalLength = ( float ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_SUBJECT_DISTANCE:
				// Inidcates the distacne the autofocus camera is focused to.
				// Tends to be less accurate as distance increases.
				ExifData::Distance = ( float ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_EXPOSURETIME:
				// Simplest way of expressing exposure time, so I trust it most.
				// (overwrite previously computd value if there is one)
				ExifData::ExposureTime = ( float ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_SHUTTERSPEED:
				// More complicated way of expressing exposure time, so only use
				// this value if we don't already have it from somewhere else.
				if ( ExifData::ExposureTime == 0 )
				{
					ExifData::ExposureTime
					= ( float ) ( 1/exp ( ConvertAnyFormat ( ValuePtr, Format ) *log ( 2.0 ) ) );
				}
				break;

			case TAG_FLASH:
				ExifData::FlashUsed = ( int ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_EXIF_IMAGELENGTH:
				ExifImageLength = ( int ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_EXIF_IMAGEWIDTH:
				ExifImageWidth = ( int ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_FOCALPLANEXRES:
				FocalplaneXRes = ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_FOCALPLANEUNITS:
				switch ( ( int ) ConvertAnyFormat ( ValuePtr, Format ) )
				{
					case 1: FocalplaneUnits = 25.4; break; // inch
					case 2:
						// According to the information I was using, 2 means meters.
						// But looking at the Cannon powershot's files, inches is the only
						// sensible value.
						FocalplaneUnits = 25.4;
						break;

					case 3: FocalplaneUnits = 10;   break;  // centimeter
					case 4: FocalplaneUnits = 1;    break;  // milimeter
					case 5: FocalplaneUnits = .001; break;  // micrometer
				}
				break;

				// Remaining cases contributed by: Volker C. Schoech (schoech@gmx.de)

			case TAG_EXPOSURE_BIAS:
				ExifData::ExposureBias = ( float ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_WHITEBALANCE:
				ExifData::Whitebalance = ( int ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_METERING_MODE:
				ExifData::MeteringMode = ( int ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_EXPOSURE_PROGRAM:
				ExifData::ExposureProgram = ( int ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_ISO_EQUIVALENT:
				ExifData::ISOequivalent = ( int ) ConvertAnyFormat ( ValuePtr, Format );
				if ( ExifData::ISOequivalent < 50 ) ExifData::ISOequivalent *= 200;
				break;

			case TAG_COMPRESSION_LEVEL:
				ExifData::CompressionLevel = ( int ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_THUMBNAIL_OFFSET:
				ThumbnailOffset = ( unsigned ) ConvertAnyFormat ( ValuePtr, Format );
				break;

			case TAG_THUMBNAIL_LENGTH:
				ThumbnailSize = ( unsigned ) ConvertAnyFormat ( ValuePtr, Format );
				break;
		}

		if ( Tag == TAG_EXIF_OFFSET || Tag == TAG_INTEROP_OFFSET )
		{
			unsigned char * SubdirStart;
			SubdirStart = OffsetBase + Get32u ( ValuePtr );
			if ( SubdirStart < OffsetBase || SubdirStart > OffsetBase+ExifLength )
			{
				return ;
//                throw FatalError("Illegal subdirectory link");
			}
			ProcessExifDir ( SubdirStart, OffsetBase, ExifLength );
			recurseLevel--;
			continue;
		}
	}

	{
		// In addition to linking to subdirectories via exif tags,
		// there's also a potential link to another directory at the end of each
		// directory.  this has got to be the result of a comitee!
		unsigned char * SubdirStart;
		unsigned Offset;

		if ( DIR_ENTRY_ADDR ( DirStart, NumDirEntries ) + 4 <= OffsetBase+ExifLength )
		{
			Offset = Get32u ( DIR_ENTRY_ADDR ( DirStart, NumDirEntries ) );
			// There is at least one jpeg from an HP camera having an Offset of almost MAXUINT.
			// Adding OffsetBase to it produces an overflow, so compare with ExifLength here.
			// See http://bugs.kde.org/show_bug.cgi?id=54542
			if ( Offset && Offset < ExifLength )
			{
				SubdirStart = OffsetBase + Offset;
				if ( SubdirStart > OffsetBase+ExifLength )
				{
					if ( SubdirStart < OffsetBase+ExifLength+20 )
					{
						// Jhead 1.3 or earlier would crop the whole directory!
						// As Jhead produces this form of format incorrectness,
						// I'll just let it pass silently
						qDebug ( "Thumbnail removed with Jhead 1.3 or earlier" );
					}
					else
					{
						return ;
//                       throw FatalError("Illegal subdirectory link 2");
					}
				}
				else
				{
					if ( SubdirStart <= OffsetBase+ExifLength )
					{
						ProcessExifDir ( SubdirStart, OffsetBase, ExifLength );
						recurseLevel--;
					}
				}
			}
		}
		else
		{
			// The exif header ends before the last next directory pointer.
		}
	}

	if ( ThumbnailSize && ThumbnailOffset )
	{
		if ( ThumbnailSize + ThumbnailOffset <= ExifLength )
		{
			// The thumbnail pointer appears to be valid.  Store it.
			Thumbnail.loadFromData ( OffsetBase + ThumbnailOffset, ThumbnailSize, "JPEG" );
		}
	}
}

//--------------------------------------------------------------------------
// Process a COM marker.  We want to leave the bytes unchanged.  The
// progam that displays this text may decide to remove blanks, convert
// newlines, or otherwise modify the text.  In particular we want to be
// safe for passing utf-8 text.
//--------------------------------------------------------------------------
void ExifData::process_COM ( const uchar * Data, int length )
{
	Comment = QString::fromUtf8 ( ( char * ) Data+2, ( length-2 ) );
}


//--------------------------------------------------------------------------
// Process a SOFn marker.  This is useful for the image dimensions
//--------------------------------------------------------------------------
void ExifData::process_SOFn ( const uchar * Data, int marker )
{
	int data_precision, num_components;

	data_precision = Data[2];
	ExifData::Height = Get16m ( Data+3 );
	ExifData::Width = Get16m ( Data+5 );
	num_components = Data[7];

	if ( num_components == 3 )
	{
		ExifData::IsColor = 1;
	}
	else
	{
		ExifData::IsColor = 0;
	}

	ExifData::Process = marker;

}

//--------------------------------------------------------------------------
// Get 16 bits motorola order (always) for jpeg header stuff.
//--------------------------------------------------------------------------
int ExifData::Get16m ( const void * Short )
{
	return ( ( ( uchar * ) Short ) [0] << 8 ) | ( ( uchar * ) Short ) [1];
}


//--------------------------------------------------------------------------
// Process a EXIF marker
// Describes all the drivel that most digital cameras include...
//--------------------------------------------------------------------------
void ExifData::process_EXIF ( unsigned char * CharBuf, unsigned int length )
{
	ExifData::FlashUsed = 0; // If it s from a digicam, and it used flash, it says so.

	FocalplaneXRes = 0;
	FocalplaneUnits = 0;
	ExifImageWidth = 0;
	ExifImageLength = 0;

	{   // Check the EXIF header component
		static const uchar ExifHeader[] = "Exif\0\0";
		if ( memcmp ( CharBuf+2, ExifHeader,6 ) )
		{
			return ;
//            throw FatalError("Incorrect Exif header");
		}
	}

	if ( memcmp ( CharBuf+8,"II",2 ) == 0 )
	{
		// printf("Exif section in Intel order\n");
		MotorolaOrder = 0;
	}
	else
	{
		if ( memcmp ( CharBuf+8,"MM",2 ) == 0 )
		{
			// printf("Exif section in Motorola order\n");
			MotorolaOrder = 1;
		}
		else
		{
			return ;
//            throw FatalError("Invalid Exif alignment marker.");
		}
	}

	// Check the next two values for correctness.
	if ( Get16u ( CharBuf+10 ) != 0x2a
	        || Get32u ( CharBuf+12 ) != 0x08 )
	{
		return ;
//        throw FatalError("Invalid Exif start (1)");
	}

	LastExifRefd = CharBuf;

	// First directory starts 16 bytes in.  Offsets start at 8 bytes in.
	ProcessExifDir ( CharBuf+16, CharBuf+8, length-6 );
	recurseLevel--;
	// This is how far the interesting (non thumbnail) part of the exif went.
	ExifSettingsLength = LastExifRefd - CharBuf;

	// Compute the CCD width, in milimeters.
	if ( FocalplaneXRes != 0 )
	{
		ExifData::CCDWidth = ( float ) ( ExifImageWidth * FocalplaneUnits / FocalplaneXRes );
	}
}

//--------------------------------------------------------------------------
// Convert exif time to Unix time structure
//--------------------------------------------------------------------------
int ExifData::Exif2tm ( struct tm * timeptr, char * ExifTime )
{
	int a;

	timeptr->tm_wday = -1;

	// Check for format: YYYY:MM:DD HH:MM:SS format.
	a = sscanf ( ExifTime, "%d:%d:%d %d:%d:%d",
	             &timeptr->tm_year, &timeptr->tm_mon, &timeptr->tm_mday,
	             &timeptr->tm_hour, &timeptr->tm_min, &timeptr->tm_sec );

	if ( a == 6 )
	{
		timeptr->tm_isdst = -1;
		timeptr->tm_mon -= 1;      // Adjust for unix zero-based months
		timeptr->tm_year -= 1900;  // Adjust for year starting at 1900
		return true; // worked.
	}

	return false; // Wasn't in Exif date format.
}

//--------------------------------------------------------------------------
// Contructor for initialising
//--------------------------------------------------------------------------
ExifData::ExifData()
{
	ExifData::Whitebalance = -1;
	ExifData::MeteringMode = -1;
	ExifData::FlashUsed = 0;
	Orientation = 0;
	Height = 0;
	Width = 0;
	IsColor = 0;
	Process = 0;
	FocalLength = 0;
	ExposureTime = 0;
	ApertureFNumber = 0;
	Distance = 0;
	CCDWidth = 0;
	ExposureBias = 0;
	ExposureProgram = 0;
	ISOequivalent = 0;
	CompressionLevel = 0;
	exifDataValid = false;
	recurseLevel = 0;
}

//--------------------------------------------------------------------------
// process a EXIF jpeg file
//--------------------------------------------------------------------------
bool ExifData::scan ( const QString & path )
{
	int ret;

	QFile f ( path );
	if ( !f.open ( QIODevice::ReadOnly ) )
		return false;

//    try {
	// Scan the JPEG headers.
	ret = ReadJpegSections ( f, READ_EXIF );
	/*    }
	    catch (FatalError& e) {
	        e.debug_print();
	        f.close();
	        return false;
	    }
	*/
	if ( ret == false )
	{
		DiscardData();
		f.close();
		return false;
	}
	f.close();
	DiscardData();

	//now make the strings clean,
	// for exmaple my Casio is a "QV-4000   "
	CameraMake = CameraMake.trimmed();
	CameraModel = CameraModel.trimmed();
	UserComment = UserComment.trimmed();
	Comment = Comment.trimmed();
	return true;
}

//--------------------------------------------------------------------------
// Does the embedded thumbnail match the jpeg image?
//--------------------------------------------------------------------------
#ifndef JPEG_TOL
#define JPEG_TOL 0.02
#endif
bool ExifData::isThumbnailSane()
{
	bool ret = true;
	if ( Thumbnail.isNull() )
		ret = false;

	// check whether thumbnail dimensions match the image
	// not foolproof, but catches some altered images (jpegtran -rotate)
	if ( ExifImageLength != 0 && ExifImageLength != Height )
		ret = false;
	if ( ExifImageWidth != 0 && ExifImageWidth != Width )
		ret = false;
	if ( Thumbnail.width() == 0 || Thumbnail.height() == 0 )
		ret = false;
	if ( Height == 0 || Width == 0 )
		ret = false;
	double d = ( double ) Height/Width*Thumbnail.width() /Thumbnail.height();
	if (!(( 1-JPEG_TOL < d ) && ( d < 1+JPEG_TOL )))
		ret = false;
	exifDataValid = ret;
	return ret;
}


//--------------------------------------------------------------------------
// return a thumbnail that respects the orientation flag
// only if it seems sane
//--------------------------------------------------------------------------
QImage ExifData::getThumbnail()
{
	if ( Thumbnail.isNull() ) return QImage(); // Qt4 NULL->QImage() is it sane?
	if ( !Orientation || Orientation == 1 ) return Thumbnail;

	// now fix orientation
	QMatrix M;
	QMatrix flip = QMatrix ( -1,0,0,1,0,0 );
	switch ( Orientation )
	{  // notice intentional fallthroughs
		case 2: M = flip; break;
		case 4: M = flip;
		case 3: M.rotate ( 180 ); break;
		case 5: M = flip;
		case 6: M.rotate ( 90 ); break;
		case 7: M = flip;
		case 8: M.rotate ( 270 ); break;
		default: break; // should never happen
	}
	return Thumbnail.transformed ( M );
}
