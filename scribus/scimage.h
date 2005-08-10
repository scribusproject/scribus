#ifndef SCIMAGE_H
#define SCIMAGE_H

#include "scconfig.h"
#include "scribusapi.h"

#include <setjmp.h>
#include <cstdlib>
#include <cmath>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <valarray>
#include <qimage.h>
#include <qstring.h>
#include <qdatastream.h>
#include <qmap.h>
#include <qvaluelist.h>
#include <qfile.h>
#include <qdir.h>
#include <qstringlist.h>
#include "fpointarray.h"
#include "sccolor.h"
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

class SCRIBUS_API ScImage : public QImage
{
public:
	ScImage();
	ScImage(QImage image);
	ScImage( int width, int height );
	~ScImage() {};
	
	enum ImageEffectCode
	{
		EF_INVERT = 0,
		EF_GRAYSCALE = 1,
		EF_COLORIZE = 2,
		EF_BRIGHTNESS = 3,
		EF_CONTRAST = 4,
		EF_SHARPEN = 5,
		EF_BLUR = 6,
		EF_SOLARIZE = 7
	};
	struct imageEffect
	{
		int effectCode;
		QString effectParameters;
	};
	void initialize();
	QString ImageToTxt();
	QString ImageToCMYK();
	QString ImageToGray();
	QString ImageToCMYK_PS(int pl, bool pre);
	QString ImageToCMYK_PDF(bool pre);
	void Convert2JPG(QString fn, int Quality, bool isCMYK, bool isGray);
	QString MaskToTxt(bool PDF = true);
	QString MaskToTxt14();
	void applyEffect(QValueList<imageEffect> effectsList, QMap<QString,ScColor> colors, bool cmyk);
	void solarize(double factor, bool cmyk);
	void blur(double radius= 0.0, double sigma = 1.0);
	void sharpen(double radius= 0.0, double sigma = 1.0);
	void contrast(int contrastValue, bool cmyk);
	void brightness(int brightnessValue, bool cmyk);
	void invert(bool cmyk);
	void colorize(ScColor color, int shade, bool cmyk);
	void toGrayscale(bool cmyk);
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
	void liberateMemory(void **memory);
	void blurScanLine(double *kernel, int width, unsigned int *src, unsigned int *dest, int columns);
	int getBlurKernel(int width, double sigma, double **kernel);
	bool convolveImage(QImage *dest, const unsigned int order, const double *kernel);
	int getOptimalKernelWidth(double radius, double sigma);
	void applyCurve(bool cmyk);
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
	std::valarray<int> curveTable;
};
#endif
