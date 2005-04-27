#ifndef SCIMAGE_H
#define SCIMAGE_H

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#include <setjmp.h>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <qimage.h>
#include <qstring.h>
#include <qdatastream.h>
#include <qmap.h>
#include <qvaluelist.h>
#include <qfile.h>
#include <qdir.h>
#include <qstringlist.h>
#include "fpointarray.h"
extern "C"
{
#define XMD_H           // shut JPEGlib up
#if defined(Q_OS_UNIXWARE)
#  define HAVE_BOOLEAN  // libjpeg under Unixware seems to need this
#endif
#include <jpeglib.h>
#include <jerror.h>
#undef HAVE_STDLIB_H
#ifdef const
#  undef const          // remove crazy C hackery in jconfig.h
#endif
}
#ifdef HAVE_TIFF
	#include <tiffio.h>
#endif

class ScImage : public QImage
{
public:
	ScImage();
	ScImage(QImage image);
	ScImage( int width, int height );
	~ScImage() {};
	void initialize();
	QString ImageToTxt();
	QString ImageToCMYK();
	QString ImageToGray();
	QString ImageToCMYK_PS(int pl, bool pre);
	QString ImageToCMYK_PDF(bool pre);
	void Convert2JPG(QString fn, int Quality, bool isCMYK, bool isGray);
	QString MaskToTxt(bool PDF = true);
	QString MaskToTxt14();
	void swapRGBA();
	void createLowRes(double scale);
	void scaleImage(int width, int height);
	bool LoadPicture(QString fn, QString Prof, int rend, bool useEmbedded, bool useProf, int requestType, int gsRes, bool *realCMYK = 0);
	QString getAlpha(QString fn, bool PDF, bool pdf14);

	enum PSDColorMode
	{
		CM_BITMAP = 0,
		CM_GRAYSCALE = 1,
		CM_INDEXED = 2,
		CM_RGB = 3,
		CM_CMYK = 4,
		CM_MULTICHANNEL = 7,
		CM_DUOTONE = 8,
		CM_LABCOLOR = 9
	};
	
	struct PSDHeader
	{
		uint signature;
		ushort version;
		uchar reserved[6];
		ushort channel_count;
		uint height;
		uint width;
		ushort depth;
		ushort color_mode;
	};

struct PSDLayer
{
	QValueList<uint> channelLen;
	QValueList<int> channelType;
	int xpos;
	int ypos;
	int width;
	int height;
	ushort opacity;
	uchar clipping;
	uchar flags;
	QString layerName;
	QString blend;
	QImage thumb;
};

struct LoadRequest
{
	bool visible;
	ushort opacity;
	QString blend;
};

struct ImageInfoRecord
{
	int xres;
	int yres;
	int colorspace; /* 0 = RGB  1 = CMYK  2 = Grayscale */
	bool valid;
	bool isRequest;
	bool progressive;
	int lowResType; /* 0 = full Resolution, 1 = 72 dpi, 2 = 36 dpi */
	double lowResScale;
	QMap<QString, FPointArray> PDSpathData;
	QMap<int, LoadRequest> RequestProps;
	QString clipPath;
	QString usedPath;
	QString profileName;
	QValueList<PSDLayer> layerInfo;
} imgInfo;

private:
	bool IsValid( const PSDHeader & header );
	bool IsSupported( const PSDHeader & header );
	unsigned char INT_MULT ( unsigned char a, unsigned char b );
	void RGBTOHSV ( uchar& red, uchar& green, uchar& blue );
	void HSVTORGB ( uchar& hue, uchar& saturation, uchar& value );
	void RGBTOHLS ( uchar& red, uchar& green, uchar& blue );
	int HLSVALUE ( double n1, double n2, double hue );
	void HLSTORGB ( uchar& hue, uchar& lightness, uchar& saturation );
	bool loadLayerChannels( QDataStream & s, const PSDHeader & header, QValueList<PSDLayer> &layerInfo, uint layer, bool* firstLayer);
	bool loadLayer( QDataStream & s, const PSDHeader & header);
	QString getLayerString(QDataStream & s);
	QString getPascalString(QDataStream & s);
	void parseRessourceData( QDataStream & s, const PSDHeader & header, uint size);
	bool parseLayer( QDataStream & s, const PSDHeader & header);
	bool LoadPSD( QDataStream & s, const PSDHeader & header);
	bool marker_is_icc (jpeg_saved_marker_ptr marker);
	bool marker_is_photoshop (jpeg_saved_marker_ptr marker);
	bool read_jpeg_marker (UINT8 requestmarker, j_decompress_ptr cinfo, JOCTET **icc_data_ptr, unsigned int *icc_data_len);
	char* iccbuf;
	uint icclen;
};
#endif
