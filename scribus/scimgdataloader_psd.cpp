/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scconfig.h"
#include "scimgdataloader_psd.h"
#include "util_color.h"
#include "sccolorengine.h"

#include <QFile>
#include <QFileInfo>
#include <QList>

#include CMS_INC

static QDataStream & operator>> ( QDataStream & s, PSDHeader & header )
{
	s >> header.signature;
	s >> header.version;
	for( int i = 0; i < 6; i++ )
	{
		s >> header.reserved[i];
	}
	s >> header.channel_count;
	s >> header.height;
	s >> header.width;
	s >> header.depth;
	s >> header.color_mode;
	return s;
}

ScImgDataLoader_PSD::ScImgDataLoader_PSD(void)
{
	initSupportedFormatList();
}

void ScImgDataLoader_PSD::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "psd" );
}

void ScImgDataLoader_PSD::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
	cmsHPROFILE prof = 0;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return;
	QString ext = fi.suffix().toLower();
	if (ext == "psd")
	{
		QFile f(fn);
		if (f.open(QIODevice::ReadOnly))
		{
			m_imageInfoRecord.xres = 72;
			m_imageInfoRecord.yres = 72;
			QDataStream s( &f );
			s.setByteOrder( QDataStream::BigEndian );
			PSDHeader header;
			s >> header;
			// Check image file format.
			if( s.atEnd() || !IsValid( header ) )
				return;
			// Check if it's a supported format.
			if( !IsSupported( header ) )
				return;
			if( !LoadPSDResources(s, header, s.device()->pos()) )
				return;
			if (m_embeddedProfile.size() > 0)
			{
				prof = cmsOpenProfileFromMem(m_embeddedProfile.data(), m_embeddedProfile.size());
				if (prof)
				{
					if (static_cast<int>(cmsGetColorSpace(prof)) == icSigRgbData)
						m_profileComponents = 3;
					if (static_cast<int>(cmsGetColorSpace(prof)) == icSigCmykData)
						m_profileComponents = 4;
				}
				cmsCloseProfile(prof);
			}
			f.close();
		}
		else
			return;
	}
}

bool ScImgDataLoader_PSD::preloadAlphaChannel(const QString& fn, int /*page*/, int res, bool& hasAlpha)
{
	bool valid = m_imageInfoRecord.isRequest;
	QMap<int, ImageLoadRequest> req = m_imageInfoRecord.RequestProps;
	initialize();
	hasAlpha = false;
	m_imageInfoRecord.RequestProps = req;
	m_imageInfoRecord.isRequest = valid;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return false;

	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream s( &f );
		s.setByteOrder( QDataStream::BigEndian );
		PSDHeader header;
		s >> header;
		// Check image file format.
		if( s.atEnd() || !IsValid( header ) )
			return false;
		// Check if it's a supported format.
		if( !IsSupported( header ) )
			return false;
		if( !LoadPSD(s, header) )
			r_image.resize(0);
		f.close();
		m_imageInfoRecord.valid = true;
		if (header.color_mode == CM_CMYK)
		{
			if ( maxChannels == 4)
				m_imageInfoRecord.valid = hasAlpha = false;
		}
		else
		{
			if ( maxChannels == 3)
				m_imageInfoRecord.valid = hasAlpha = false;
		}
		return true;
	}
	return false;
}

bool ScImgDataLoader_PSD::loadPicture(const QString& fn, int /*page*/, int res, bool thumbnail)
{
	bool isCMYK = false;
	float xres = 72.0, yres = 72.0;
	if (!QFile::exists(fn))
		return false;
	bool valid = m_imageInfoRecord.isRequest;
	QMap<int, ImageLoadRequest> req = m_imageInfoRecord.RequestProps;
	initialize();
	m_imageInfoRecord.RequestProps = req;
	m_imageInfoRecord.isRequest = valid;
	m_imageInfoRecord.type = ImageTypePSD;
	m_imageInfoRecord.exifDataValid = false;
	m_imageInfoRecord.layerInfo.clear();
	m_imageInfoRecord.PDSpathData.clear();
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		m_imageInfoRecord.xres = 72;
		m_imageInfoRecord.yres = 72;
		QDataStream s( &f );
		s.setByteOrder( QDataStream::BigEndian );
		PSDHeader header;
		s >> header;
		// Check image file format.
		if( s.atEnd() || !IsValid( header ) )
			return false;
		// Check if it's a supported format.
		if( !IsSupported( header ) )
			return false;
		if (thumbnail)
			header.reserved[0] = 't';
		if( !LoadPSD(s, header) )
			return false;
		if (m_embeddedProfile.size() > 0)
		{
			cmsHPROFILE prof = cmsOpenProfileFromMem(m_embeddedProfile.data(), m_embeddedProfile.size());
			const char *Descriptor;
			Descriptor = cmsTakeProductDesc(prof);
			m_imageInfoRecord.profileName = QString(Descriptor);
			m_imageInfoRecord.isEmbedded = true;
		}
		if (header.color_mode == CM_CMYK)
			isCMYK = true;
		else
			isCMYK = false;
		if (header.color_mode == CM_CMYK)
			m_imageInfoRecord.colorspace = ColorSpaceCMYK;
		else if (header.color_mode == CM_RGB)
			m_imageInfoRecord.colorspace = ColorSpaceRGB;
		else if (header.color_mode == CM_GRAYSCALE)
			m_imageInfoRecord.colorspace = ColorSpaceGray;
		else if (header.color_mode == CM_DUOTONE)
			m_imageInfoRecord.colorspace = ColorSpaceDuotone;
		xres = m_imageInfoRecord.xres;
		yres = m_imageInfoRecord.yres;
		f.close();
		if (thumbnail)
		{
			if (!m_imageInfoRecord.exifInfo.thumbnail.isNull())
			{
				if (isCMYK)
					r_image.create(m_imageInfoRecord.exifInfo.thumbnail.width(), m_imageInfoRecord.exifInfo.thumbnail.height(), 5);
				else
					r_image.create(m_imageInfoRecord.exifInfo.thumbnail.width(), m_imageInfoRecord.exifInfo.thumbnail.height(), 4);
				QRgb *s;
				unsigned char cc, cm, cy, ck;
				uchar *d;
				for( int yit=0; yit < m_imageInfoRecord.exifInfo.thumbnail.height(); ++yit )
				{
					s = (QRgb*)(m_imageInfoRecord.exifInfo.thumbnail.scanLine( yit ));
					d = r_image.scanLine( yit );
					for(int xit=0; xit < m_imageInfoRecord.exifInfo.thumbnail.width(); ++xit )
					{
						if (isCMYK)
						{
							cc = 255 - qRed(*s);
							cm = 255 - qGreen(*s);
							cy = 255 - qBlue(*s);
							ck = qMin(qMin(cc, cm), cy);
							d[0] = cc-ck;
							d[1] = cm-ck;
							d[2] = cy-ck;
							d[3] = ck;
							d[4] = 255;
						}
						else
						{
							d[0] = qRed(*s);
							d[1] = qGreen(*s);
							d[2] = qBlue(*s);
							d[3] = 255;
						}
						s++;
						d += r_image.channels();
					}
				}
				m_imageInfoRecord.exifInfo.width = header.width;
				m_imageInfoRecord.exifInfo.height = header.height;
				return true;
			}
		}
		m_imageInfoRecord.exifInfo.width = header.width;
		m_imageInfoRecord.exifInfo.height = header.height;
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = r_image.height();
		return true;
	}
	return false;
}

bool ScImgDataLoader_PSD::LoadPSD( QDataStream & s, const PSDHeader & header)
{
	// Create dst image.
	m_imageInfoRecord.valid = false;
	if (header.color_mode == CM_CMYK)
	{
		if (!r_image.create(header.width, header.height, 5))
			return false;
	}
	else
	{
		if (!r_image.create(header.width, header.height, 4))
			return false;
	}
	r_image.fill(0);
	maxChannels = header.channel_count;
	uint tmp;
	uint cresStart;
	uint cdataStart;
	uint ressourceDataLen;
	uint startRessource;

	cresStart = s.device()->pos();
	// Skip mode data. FIX: this is incorrect, it's the Colormap Data for indexed Images
	s >> tmp;
	cdataStart = s.device()->pos();

	LoadPSDResources(s, header, cresStart); 
	
	s.device()->seek( cdataStart + tmp );
	s >> ressourceDataLen;
	startRessource = s.device()->pos();

	if  ((!m_imageInfoRecord.exifInfo.thumbnail.isNull()) && (header.reserved[0] == 't'))
		return true;
	bool ret = LoadPSDImgData(s, header, startRessource + ressourceDataLen);
	return ret;
}

bool ScImgDataLoader_PSD::LoadPSDResources( QDataStream & s, const PSDHeader & header, uint dataOffset )
{
	// Create dst image.
	m_imageInfoRecord.valid = false;

	uint tmp;
	uint cdataStart;
	uint ressourceDataLen;
	uint startRessource;

	s.device()->seek( dataOffset );

	srand(314159265);
	for (int i = 0; i < 4096; i++)
		random_table[i] = rand();
	int tmpd;
	int swap;
	for (int i = 0; i < 4096; i++)
	{
		swap = i + rand() % (4096 - i);
		tmpd = random_table[i];
		random_table[i] = random_table[swap];
		random_table[swap] = tmpd;
	}
	// Skip mode data. FIX: this is incorrect, it's the Colormap Data for indexed Images
	s >> tmp;
	cdataStart = s.device()->pos();
	if (tmp != 0)
	{
		if ((header.color_mode == CM_DUOTONE))
		{
			short signature;
			short count;
			unsigned short c, m, y, k;
			uchar hc, sc, bc;
			ScColor col;
			s >> signature;
			s >> count;
			uint duodataStart = s.device()->pos();
			bool specialColour = false;
			for (int cda = 0; cda < count; cda++)
			{
				s >> signature;
				s >> c >> m >> y >> k;
				switch (signature)
				{
					case 0:	// RGB colour
						col.setColorRGB(c >> 8, m >> 8, y >> 8);
						colorTableSc.append(col);
						break;
					case 1:	// HSB colour
						hc = c >> 8;
						sc = m >> 8;
						bc = y >> 8;
						HSVTORGB(hc, sc, bc);
						col.setColorRGB(hc, sc, bc);
						colorTableSc.append(col);
						break;
					case 2:	// CMYK colour
						col.setColor(c >> 8, m >> 8, y >> 8, k >> 8);
						colorTableSc.append(col);
						break;
					case 3:	// Pantone
					case 4:	// Focoltone
					case 5:	// Truematch
					case 6:	// Toyo 88 colorfinder 1050
					case 7:	// LAB colour space
					case 10: // HKS colors
						if (colorTableSc.count() == 0)
							colorTableSc.append(ScColor(0, 0, 0, 255));
						else
							colorTableSc.append(ScColor(0, 0, 0, 0));
						specialColour = true;
						break;
					case 8: // Grayscale
						c = qRound((c / 10000.0) * 255);
						colorTableSc.append(ScColor(0, 0, 0, c));
						break;
				}
			}
			if (specialColour) // we will only load the grayscale image data and do the colorizing with the Image Effects;
			{
				s.device()->seek( duodataStart + 40 );
				uint duoNameStart = s.device()->pos();
				uint duoCurveStart = duoNameStart+256;
				for (int cda = 0; cda < count; cda++)
				{
					QString colName;
					s.device()->seek( duoNameStart + (64 * static_cast<uint>(cda)) );
					colName = getPascalString(s);
					s.device()->seek( duoCurveStart + (28 * static_cast<uint>(cda)) );
					FPointArray tmcu;
					tmcu.resize(0);
					for (int cu = 0; cu < 13; cu++)
					{
						short val;
						s >> val;
						if (val != -1)
							tmcu.addPoint(cu * 1.0 / 12.0, val / 1000.0);
					}
					PSDDuotone_Color colSpec;
					colSpec.Name = colName;
					colSpec.Color = colorTableSc[cda];
					colSpec.Curve = tmcu;
					m_imageInfoRecord.duotoneColors.append(colSpec);
				}
				// Initialize loading curve tables with default values
				curveTable1.resize(256);
				curveTable2.resize(256);
				curveTable3.resize(256);
				curveTable4.resize(256);
				for (int x = 0 ; x < 256 ; x++)
				{
					curveTable1[x] = x;
					curveTable2[x] = x;
					curveTable3[x] = x;
					curveTable4[x] = x;
				}
			}
			else
			{
				s.device()->seek( duodataStart + 40 );
				uint duoNameStart = s.device()->pos();
				uint duoCurveStart = duoNameStart+256;
				for (int cda = 0; cda < count; cda++)
				{
					s.device()->seek( duoCurveStart + (28 * static_cast<uint>(cda)) );
					FPointArray tmcu;
					tmcu.resize(0);
					for (int cu = 0; cu < 13; cu++)
					{
						short val;
						s >> val;
						if (val != -1)
							tmcu.addPoint(cu * 1.0 / 12.0, val / 1000.0);
					}
					if (cda == 0)
					{
						curveTable1.resize(256);
						for (int x = 0 ; x < 256 ; x++)
						{
							curveTable1[x] = qMin(255, qMax(0, qRound(getCurveYValue(tmcu, x / 255.0) * 255)));
						}
					}
					else if (cda == 1)
					{
						curveTable2.resize(256);
						for (int x = 0 ; x < 256 ; x++)
						{
							curveTable2[x] = qMin(255, qMax(0, qRound(getCurveYValue(tmcu, x / 255.0) * 255)));
						}
					}
					else if (cda == 2)
					{
						curveTable3.resize(256);
						for (int x = 0 ; x < 256 ; x++)
						{
							curveTable3[x] = qMin(255, qMax(0, qRound(getCurveYValue(tmcu, x / 255.0) * 255)));
						}
					}
					else if (cda == 3)
					{
						curveTable4.resize(256);
						for (int x = 0 ; x < 256 ; x++)
						{
							curveTable4[x] = qMin(255, qMax(0, qRound(getCurveYValue(tmcu, x / 255.0) * 255)));
						}
					}
				}
			}
		}
		else
		{
			QList<uchar> colorTableR;
			QList<uchar> colorTableG;
			QList<uchar> colorTableB;
			colorTableR.clear();
			colorTableG.clear();
			colorTableB.clear();
			colorTable.clear();
			uchar r;
			for (uint cc = 0; cc < 256; cc++)
			{
				s >> r;
				colorTableR.append(r);
			}
			for (uint cc = 0; cc < 256; cc++)
			{
				s >> r;
				colorTableG.append(r);
			}
			for (uint cc = 0; cc < 256; cc++)
			{
				s >> r;
				colorTableB.append(r);
			}
			for (uint cc = 0; cc < 256; cc++)
			{
				colorTable.append(qRgb(colorTableR[cc], colorTableG[cc], colorTableB[cc]));
			}
		}
	}
	s.device()->seek( cdataStart + tmp );
	s >> ressourceDataLen;
	startRessource = s.device()->pos();
	if (ressourceDataLen != 0)
		parseRessourceData(s, header, ressourceDataLen);
	return true;
}

bool ScImgDataLoader_PSD::LoadPSDImgData( QDataStream & s, const PSDHeader & header, uint dataOffset )
{
	uint layerDataLen;
	uint startLayers;
	s.device()->seek( dataOffset );
	// Skip the reserved data. FIX: Also incorrect, this is the actual Layer Data for Images with Layers
	s >> layerDataLen;
	startLayers = s.device()->pos();
	if (layerDataLen != 0)
	{
		bool re = parseLayer(s, header);
		if (re)
		{
			m_imageInfoRecord.valid = true;
			return re;
		}
		else
		{
			// Try to decode simple psd file, no layers
			s.device()->seek(startLayers + layerDataLen);
			if(s.atEnd())
				return false;
			return loadLayer( s, header);
		}
	}
	else
	{
		// Decoding simple psd file, no layers
		s.device()->seek( s.device()->pos() + layerDataLen );
		loadLayer( s, header);
	}
	return true;
}

bool ScImgDataLoader_PSD::parseLayer( QDataStream & s, const PSDHeader & header )
{
	uint addRes, layerinfo, channelLen, signature, extradata, layermasksize, layerRange, dummy;
	int top, left, bottom, right;
	short numLayers, numChannels;
	short channelType;
	uchar blendKey[4];
	uchar opacity, clipping, flags, filler;
	QString layerName, blend;
	struct PSDLayer lay;
	s >> layerinfo;
	s >> numLayers;
	if (numLayers < 0)
		numLayers = -numLayers;
	if (numLayers != 0)
	{
		for (int layer = 0; layer < numLayers; layer++)
		{
			s >> top;
			lay.ypos = top;
			s >> left;
			lay.xpos = left;
			s >> bottom;
			lay.height = bottom - top;
			s >> right;
			lay.width = right - left;
			s >> numChannels;
			// Qt4 check these conversions
			maxChannels = qMax(maxChannels, (int)numChannels);
			if (numChannels > 6)	// we don't support images with more than 6 channels yet
			{
				m_imageInfoRecord.layerInfo.clear();
				return false;
			}
			lay.channelType.clear();
			lay.channelLen.clear();
			for (int channels = 0; channels < numChannels; channels++)
			{
				s >> channelType;
				s >> channelLen;
				lay.channelType.append(channelType);
				lay.channelLen.append(channelLen);
			}
			s >> signature;
			blend = "";
			for( int i = 0; i < 4; i++ )
			{
				s >> blendKey[i];
				blend += QChar(blendKey[i]);
			}
			lay.blend = blend;
			s >> opacity;
			lay.opacity = opacity;
			s >> clipping;
			lay.clipping = clipping;
			s >> flags;
			if (flags & 8)
			{
				if (flags & 16)	// Unknown combination of layer flags, probably an adjustment or effects layer
				{
					m_imageInfoRecord.layerInfo.clear();
					return false;
				}
			}
			lay.flags = flags;
			s >> filler;
			s >> extradata;
			s >> layermasksize;
			lay.maskYpos = 0;
			lay.maskXpos = 0;
			lay.maskHeight = 0;
			lay.maskWidth = 0;
			if (layermasksize != 0)
			{
				s >> lay.maskYpos;
				s >> lay.maskXpos;
				s >> dummy;
				lay.maskHeight = dummy - lay.maskYpos;
				s >> dummy;
				lay.maskWidth = dummy - lay.maskXpos;
				s >> dummy;
			}
			s >> layerRange;
			s.device()->seek( s.device()->pos() + layerRange );
			lay.layerName = getLayerString(s);
			m_imageInfoRecord.layerInfo.append(lay);
			s >> signature;
			if( signature == 0x3842494D )
			{
				while (signature == 0x3842494D )
				{
					s >> signature;
					s >> addRes;
					s.device()->seek( s.device()->pos() + addRes );
					s >> signature;
				}
				s.device()->seek( s.device()->pos() - 4 );
			}
			else
			{
				s.device()->seek( s.device()->pos() - 2 );
				s >> signature;
				if( signature == 0x3842494D )
				{
					while (signature == 0x3842494D )
					{
						s >> signature;
						s >> addRes;
						s.device()->seek( s.device()->pos() + addRes );
						s >> signature;
					}
					s.device()->seek( s.device()->pos() - 4 );
				}
				else
					s.device()->seek( s.device()->pos() - 6 );
			}
		}
		bool firstLayer = true;
		for (int layer = 0; layer < numLayers; layer++)
		{
			loadLayerChannels( s, header, m_imageInfoRecord.layerInfo, layer, &firstLayer );
		}
	}
	else
	{
		s >> numLayers;
		loadLayer( s, header);
	}
	return true;
}

bool ScImgDataLoader_PSD::loadChannel( QDataStream & s, const PSDHeader & header, QList<PSDLayer> &layerInfo, uint layer, int channel, int component, RawImage &tmpImg)
{
	uint base = s.device()->pos();
	uchar cbyte;
	ushort compression;
	s >> compression;
	if( compression > 1 )
		return false;
	if (compression == 0)
	{
		int count = layerInfo[layer].channelLen[channel]-2;
		uchar *ptr;
		for (int i = 0; i < tmpImg.height(); i++)
		{
			ptr =  tmpImg.scanLine(i);
			for (int j = 0; j < tmpImg.width(); j++)
			{
				s >> cbyte;
				count--;
				if ((header.color_mode == CM_CMYK) && (component < 4))
					cbyte = 255 - cbyte;
				if ((header.color_mode == CM_GRAYSCALE) && (component != 3))
				{
					ptr[0] = cbyte;
					ptr[1] = cbyte;
					ptr[2] = cbyte;
				}
				else if ((header.color_mode == CM_DUOTONE) && (component != 3))
				{
					ptr -= component;
					putDuotone(ptr, cbyte);
					ptr += component;
				}
				else if ((header.color_mode == CM_INDEXED) && (component != 3))
				{
					int ccol = colorTable[cbyte];
					ptr[0] = qRed(ccol);
					ptr[1] = qGreen(ccol);
					ptr[2] = qBlue(ccol);
				}
				else
					ptr[component] = cbyte;
				if (count == 0)
					break;
				ptr += tmpImg.channels();
			}
			if (count == 0)
				break;
		}
	}
	else
	{
		s.device()->seek( s.device()->pos() + tmpImg.height() * 2 );
		uint pixel_count = tmpImg.width();
		uchar *ptr;
		uchar *ptr2;
		uint count, len;
		uchar c;
		for (int hh = 0; hh < tmpImg.height(); hh++)
		{
			count = 0;
			ptr = tmpImg.scanLine(hh);
			ptr2 = ptr+tmpImg.width() * tmpImg.channels();
			ptr += component;
			while( count < pixel_count )
			{
				if(s.atEnd())
					return false;
				s >> c;
				len = c;
				if( len < 128 )
				{
					// Copy next len+1 bytes literally.
					len++;
					count += len;
					while( len != 0 )
					{
						s >> cbyte;
						if (ptr < ptr2)
						{
							if ((header.color_mode == CM_CMYK) && (component < 4))
								cbyte = 255 - cbyte;
							if ((header.color_mode == CM_GRAYSCALE) && (component != 3))
							{
								ptr -= component;
								ptr[0] = cbyte;
								ptr[1] = cbyte;
								ptr[2] = cbyte;
								ptr += component;
							}
							else if ((header.color_mode == CM_DUOTONE) && (component != 3))
							{
								ptr -= component;
								putDuotone(ptr, cbyte);
								ptr += component;
							}
							else if ((header.color_mode == CM_INDEXED) && (component != 3))
							{
								ptr -= component;
								int ccol = colorTable[cbyte];
								ptr[0] = qRed(ccol);
								ptr[1] = qGreen(ccol);
								ptr[2] = qBlue(ccol);
								ptr += component;
							}
							else
							{
								*ptr = cbyte;
							}
						}
						ptr += tmpImg.channels();
						len--;
					}
				}
				else if( len > 128 )
				{
					// Next -len+1 bytes in the dest are replicated from next source byte.
					// (Interpret len as a negative 8-bit int.)
					len ^= 0xFF;
					len += 2;
					count += len;
					uchar val;
					s >> val;
					if ((header.color_mode == CM_CMYK) && (component < 4))
						val = 255 - val;
					while( len != 0 )
					{
						if (ptr < ptr2)
						{
							if ((header.color_mode == CM_GRAYSCALE) && (component != 3))
							{
								ptr -= component;
								ptr[0] = val;
								ptr[1] = val;
								ptr[2] = val;
								ptr += component;
							}
							else if ((header.color_mode == CM_DUOTONE) && (component != 3))
							{
								ptr -= component;
								putDuotone(ptr, val);
								ptr += component;
							}
							else if ((header.color_mode == CM_INDEXED) && (component != 3))
							{
								ptr -= component;
								int ccol = colorTable[val];
								ptr[0] = qRed(ccol);
								ptr[1] = qGreen(ccol);
								ptr[2] = qBlue(ccol);
								ptr += component;
							}
							else
								*ptr = val;
						}
						ptr += tmpImg.channels();
						len--;
					}
				}
				else if( len == 128 )
				{
					// No-op.
				}
			}
		}
	}
	s.device()->seek( base+layerInfo[layer].channelLen[channel] );
	return true;
}

bool ScImgDataLoader_PSD::loadLayerChannels( QDataStream & s, const PSDHeader & header, QList<PSDLayer> &layerInfo, uint layer, bool* firstLayer)
{
	// Find out if the data is compressed.
	// Known values:
	//   0: no compression
	//   1: RLE compressed
	uint base = s.device()->pos();
	uint base2 = base;
	uint channel_num = layerInfo[layer].channelLen.count();
	bool hasMask = false;
	bool hasAlpha = false;
	RawImage r2_image;
	RawImage mask;
	bool createOk = false;
	if (header.color_mode == CM_CMYK)
	{
		createOk = r2_image.create(layerInfo[layer].width, layerInfo[layer].height, qMax(channel_num, (uint)5));
		r2_image.fill(0);
	}
	else
	{
		createOk = r2_image.create(layerInfo[layer].width, layerInfo[layer].height, qMax(channel_num, (uint)4));
		r2_image.fill(0);
	}
	if( !createOk )
	{
		for(uint channel = 0; channel < channel_num; channel++)
		{
			base2 += layerInfo[layer].channelLen[channel];
		}
		s.device()->seek( base2 );
		return false;
	}
	channel_num = qMin(channel_num, (uint)39);
	uint components[40];
	for(uint channel = 0; channel < channel_num; channel++)
	{
		switch(layerInfo[layer].channelType[channel])
		{
		case 0:
			components[channel] = 0;
			break;
		case 1:
			components[channel] = 1;
			break;
		case 2:
			components[channel] = 2;
			break;
		case 3:
			components[channel] = 3;
			break;
		case -1:
			if (header.color_mode == CM_CMYK)
			{
				if (channel_num == 6)
					components[channel] = channel_num-2;
				else
					components[channel] = channel_num-1;
			}
			else
			{
				if (channel_num == 5)
					components[channel] = channel_num-2;
				else
				{
					if (header.color_mode == CM_GRAYSCALE)
						components[channel] = 3;
					else
						components[channel] = channel_num-1;
				}
			}
			hasAlpha = true;
			break;
		case -2:
			components[channel] = channel_num-1;
			break;
		}
	}
	if (!hasAlpha)
		r2_image.fill(255);
	for(uint channel = 0; channel < channel_num; channel++)
	{
		if (layerInfo[layer].channelType[channel] == -2)
		{
			if (!mask.create( layerInfo[layer].maskWidth, layerInfo[layer].maskHeight, 1 ))
				break;
			mask.fill(0);
			if (!loadChannel(s, header, layerInfo, layer, channel, 0, mask))
				break;
			hasMask = true;
		}
		if (!loadChannel(s, header, layerInfo, layer, channel, components[channel], r2_image))
			break;
	}
	for(uint channel = 0; channel < channel_num; channel++)
	{
		base2 += layerInfo[layer].channelLen[channel];
	}
	if (header.color_mode == CM_LABCOLOR)
	{
		cmsHPROFILE hsRGB = cmsCreate_sRGBProfile();
		cmsHPROFILE hLab  = cmsCreateLabProfile(NULL);
		DWORD inputProfFormat = (COLORSPACE_SH(PT_Lab)|CHANNELS_SH(3)|BYTES_SH(1)|EXTRA_SH(1));
		cmsHTRANSFORM xform = cmsCreateTransform(hLab, inputProfFormat, hsRGB, TYPE_RGBA_8, INTENT_PERCEPTUAL, 0);
		for (int i = 0; i < r2_image.height(); i++)
		{
			LPBYTE ptr = r2_image.scanLine(i);
			cmsDoTransform(xform, ptr, ptr, r2_image.width());
		}
		cmsDeleteTransform (xform);
		cmsCloseProfile(hsRGB);
		cmsCloseProfile(hLab);
	}
	s.device()->seek( base2 );
	QImage tmpImg2;
	if (header.color_mode == CM_CMYK)
		tmpImg2 = r2_image.convertToQImage(true);
	else
		tmpImg2 = r2_image.convertToQImage(false);
	QImage imt;
	double sx = tmpImg2.width() / 40.0;
	double sy = tmpImg2.height() / 40.0;
	imt = sy < sx ?  tmpImg2.scaled(qRound(tmpImg2.width() / sx), qRound(tmpImg2.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation) :
	      tmpImg2.scaled(qRound(tmpImg2.width() / sy), qRound(tmpImg2.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	layerInfo[layer].thumb = imt.copy();
	if (hasMask)
	{
		QImage imt2;
		QImage tmpImg;
		tmpImg = mask.convertToQImage(true);
		double sx = tmpImg.width() / 40.0;
		double sy = tmpImg.height() / 40.0;
		imt2 = sy < sx ?  tmpImg.scaled(qRound(tmpImg.width() / sx), qRound(tmpImg.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation) :
	      tmpImg.scaled(qRound(tmpImg.width() / sy), qRound(tmpImg.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		imt2.invertPixels();
		layerInfo[layer].thumb_mask = imt2.copy();
	}
	else
		layerInfo[layer].thumb_mask = QImage();
	if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
		m_imageInfoRecord.RequestProps[layer].useMask = m_imageInfoRecord.RequestProps[layer].useMask;
	else
		m_imageInfoRecord.RequestProps[layer].useMask = true;
	bool visible = !(layerInfo[layer].flags & 2);
	if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
		visible = m_imageInfoRecord.RequestProps[layer].visible;
	if (visible)
	{
		unsigned int startSrcY, startSrcX, startDstY, startDstX;
		if (layerInfo[layer].ypos < 0)
		{
			startSrcY = abs(layerInfo[layer].ypos);
			startDstY = 0;
		}
		else
		{
			startSrcY = 0;
			startDstY = layerInfo[layer].ypos;
		}
		if (layerInfo[layer].xpos < 0)
		{
			startSrcX = abs(layerInfo[layer].xpos);
			startDstX = 0;
		}
		else
		{
			startSrcX = 0;
			startDstX = layerInfo[layer].xpos;
		}
		unsigned int startSrcYm, startSrcXm, startDstYm, startDstXm;
		if (layerInfo[layer].maskYpos < 0)
		{
			startSrcYm = abs(layerInfo[layer].maskYpos);
			startDstYm = 0;
		}
		else
		{
			startSrcYm = 0;
			startDstYm = layerInfo[layer].maskYpos;
		}
		if (layerInfo[layer].maskXpos < 0)
		{
			startSrcXm = abs(layerInfo[layer].maskXpos);
			startDstXm = 0;
		}
		else
		{
			startSrcXm = 0;
			startDstXm = layerInfo[layer].maskXpos;
		}
		QString layBlend2 = layerInfo[layer].blend;
		if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
			layBlend2 = m_imageInfoRecord.RequestProps[layer].blend;
		if (layBlend2 == "diss")
		{
			hasAlpha = true;
			int layOpa = layerInfo[layer].opacity;
			if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
				layOpa = m_imageInfoRecord.RequestProps[layer].opacity;
			for (int l = 0; l < r2_image.height(); l++)
			{
				srand(random_table[ l  % 4096]);
				for (int k = 0; k < r2_image.width(); k++)
				{
					int rand_val = rand() & 0xff;
					if (rand_val > layOpa)
						r2_image.setAlpha(k, l, 0);
				}
			}
		}
		if (*firstLayer)
		{
			unsigned char *s;
			unsigned char *d;
			for( int yi=static_cast<int>(startSrcY); yi < qMin(r2_image.height(),  r_image.height()); ++yi )
			{
				s = r2_image.scanLine( yi );
				d = r_image.scanLine( qMin(static_cast<int>(startDstY),  r_image.height()-1) );
				d += qMin(static_cast<int>(startDstX), r_image.width()-1) * r_image.channels();
				s += qMin(static_cast<int>(startSrcX), r2_image.width()-1) * r2_image.channels();
				for(int xi=static_cast<int>(startSrcX); xi < qMin(r2_image.width(),  r_image.width()); ++xi )
				{
					d[0] = s[0];
					d[1] = s[1];
					d[2] = s[2];
					if (header.color_mode != CM_CMYK)
					{
						if (hasAlpha)
							d[3] = s[3];
						else
							d[3] = 255;
					}
					else
					{
						d[3] = s[3];
						if (hasAlpha)
							d[4] = s[4];
						else
							d[4] = 255;
					}
					s += r2_image.channels();
					d += r_image.channels();
				}
				startDstY++;
			}
		}
		else
		{
			unsigned char *s;
			unsigned char *d;
			unsigned char *sm = 0;
			unsigned char r, g, b, src_r, src_g, src_b, src_a, src_alpha, dst_alpha;
			unsigned char a = 0;
			uchar new_r, new_g, new_b;
			unsigned int maxDestX;
			for (int i = static_cast<int>(startSrcY); i < layerInfo[layer].height; i++)
			{
				d = r_image.scanLine(qMin(static_cast<int>(startDstY),  r_image.height()-1));
				s = r2_image.scanLine(qMin(i, r2_image.height()-1));
				d += qMin(static_cast<int>(startDstX),  r_image.width()-1) * r_image.channels();
				s += qMin(static_cast<int>(startSrcX), r2_image.width()-1) * r2_image.channels();
				sm = 0;
				if (hasMask)
				{
					sm = mask.scanLine(qMin(i, mask.height()-1));
					sm += qMin(static_cast<int>(startSrcXm), mask.width()-1) * mask.channels();
				}
				startDstY++;
				maxDestX = r_image.width() - startDstX + startSrcX - 1;
				for (unsigned int j = startSrcX; j < qMin(maxDestX, static_cast<unsigned int>(layerInfo[layer].width)); j++)
				{
					src_r = s[0];
					src_g = s[1];
					src_b = s[2];
					src_a = s[3];
					if (hasAlpha)
					{
						if (hasMask)
						{
							if (m_imageInfoRecord.RequestProps[layer].useMask)
								src_alpha = sm[0];
							else
								src_alpha = s[channel_num - 2];
						}
						else
						{
							if (header.color_mode == CM_GRAYSCALE)
								src_alpha = s[3];
							else
								src_alpha = s[channel_num - 1];
						}
					}
					else
						src_alpha = 255;
					if ((hasMask) && (m_imageInfoRecord.RequestProps[layer].useMask))
						src_alpha = sm[0];
					int layOpa = layerInfo[layer].opacity;
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
						layOpa = m_imageInfoRecord.RequestProps[layer].opacity;
					QString layBlend = layerInfo[layer].blend;
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
						layBlend = m_imageInfoRecord.RequestProps[layer].blend;
					if (layBlend != "diss")
						src_alpha = INT_MULT(src_alpha, layOpa);
					if (header.color_mode == CM_CMYK)
						dst_alpha = d[4];
					else
						dst_alpha = d[3];
					if ((dst_alpha > 0) && (src_alpha > 0))
					{
						if (layBlend == "mul ")
						{
							src_r = INT_MULT(src_r, d[0]);
							src_g = INT_MULT(src_g, d[1]);
							src_b = INT_MULT(src_b, d[2]);
							if (header.color_mode == CM_CMYK)
								src_a = INT_MULT(src_a, d[3]);
						}
						else if (layBlend == "scrn")
						{
							src_r = 255 - ((255-src_r) * (255-d[0]) / 128);
							src_g = 255 - ((255-src_g) * (255-d[1]) / 128);
							src_b = 255 - ((255-src_b) * (255-d[2]) / 128);
							if (header.color_mode == CM_CMYK)
								src_a = 255 - ((255-src_a) * (255-d[3]) / 128);
						}
						else if (layBlend == "over")
						{
							src_g = d[1] < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
							src_b = d[2] < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
							src_a = d[3] < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
							if (header.color_mode == CM_CMYK)
								src_r = d[0] < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
						}
						else if (layBlend == "diff")
						{
							src_r = d[0] > src_r ? d[0] - src_r : src_r - d[0];
							src_g = d[1] > src_g ? d[1] - src_g : src_g - d[1];
							src_b = d[2] > src_b ? d[2] - src_b : src_b - d[2];
							if (header.color_mode == CM_CMYK)
								src_a = d[3] > src_a ? d[3] - src_a : src_a - d[3];
						}
						else if (layBlend == "dark")
						{
							src_r = d[0]  < src_r ? d[0]  : src_r;
							src_g = d[1] < src_g ? d[1] : src_g;
							src_b = d[2] < src_b ? d[2] : src_b;
							if (header.color_mode == CM_CMYK)
								src_a = d[3] < src_a ? d[3] : src_a;
						}
						else if (layBlend == "hLit")
						{
							src_r = src_r < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
							src_g = src_g < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
							src_b = src_b < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
							if (header.color_mode == CM_CMYK)
								src_a = src_a < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
						}
						else if (layBlend == "sLit")
						{
							src_r = src_r * d[0] / 256 + src_r * (255 - ((255-src_r)*(255-d[0]) / 256) - src_r * d[0] / 256) / 256;
							src_g = src_g * d[1] / 256 + src_g * (255 - ((255-src_g)*(255-d[1]) / 256) - src_g * d[1] / 256) / 256;
							src_b = src_b * d[2] / 256 + src_b * (255 - ((255-src_b)*(255-d[2]) / 256) - src_b * d[2] / 256) / 256;
							if (header.color_mode == CM_CMYK)
								src_a = src_a * d[3] / 256 + src_a * (255 - ((255-src_a)*(255-d[3]) / 256) - src_a * d[3] / 256) / 256;
						}
						else if (layBlend == "lite")
						{
							src_r = d[0] < src_r ? src_r : d[0];
							src_g = d[1] < src_g ? src_g : d[1];
							src_b = d[2] < src_b ? src_b : d[2];
							if (header.color_mode == CM_CMYK)
								src_a = d[3] < src_a ? src_a : d[3];
						}
						else if (layBlend == "smud")
						{
							src_r = d[0] + src_r - src_r * d[0] / 128;
							src_g = d[1] + src_g - src_g * d[1] / 128;
							src_b = d[2] + src_b - src_b * d[2] / 128;
							if (header.color_mode == CM_CMYK)
								src_a = d[3] + src_a - src_a * d[3] / 128;
						}
						else if (layBlend == "div ")
						{
							src_r = src_r == 255 ? 255 : ((d[0] * 256) / (255-src_r)) > 255 ? 255 : (d[0] * 256) / (255-src_r);
							src_g = src_g == 255 ? 255 : ((d[1] * 256) / (255-src_g)) > 255 ? 255 : (d[1] * 256) / (255-src_g);
							src_b = src_b == 255 ? 255 : ((d[2] * 256) / (255-src_b)) > 255 ? 255 : (d[2] * 256) / (255-src_b);
							if (header.color_mode == CM_CMYK)
								src_a = src_a == 255 ? 255 : ((d[3] * 256) / (255-src_a)) > 255 ? 255 : (d[3] * 256) / (255-src_a);
						}
						else if (layBlend == "idiv")
						{
							src_r = src_r == 0 ? 0 : (255 - (((255-d[0]) * 256) / src_r)) < 0 ? 0 : 255 - (((255-d[0]) * 256) / src_r);
							src_g = src_g == 0 ? 0 : (255 - (((255-d[1]) * 256) / src_g)) < 0 ? 0 : 255 - (((255-d[1]) * 256) / src_g);
							src_b = src_b == 0 ? 0 : (255 - (((255-d[2]) * 256) / src_b)) < 0 ? 0 : 255 - (((255-d[2]) * 256) / src_b);
							if (header.color_mode == CM_CMYK)
								src_a = src_a == 0 ? 0 : (255 - (((255-d[3]) * 256) / src_a)) < 0 ? 0 : 255 - (((255-d[3]) * 256) / src_a);
						}
						else if (layBlend == "hue ")
						{
							if (header.color_mode != CM_CMYK)
							{
								new_r = d[0];
								new_g = d[1];
								new_b = d[2];
								RGBTOHSV(src_r, src_g, src_b);
								RGBTOHSV(new_r, new_g, new_b);
								new_r = src_r;
								HSVTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
						}
						else if (layBlend == "sat ")
						{
							if (header.color_mode != CM_CMYK)
							{
								new_r = d[0];
								new_g = d[1];
								new_b = d[2];
								RGBTOHSV(src_r, src_g, src_b);
								RGBTOHSV(new_r, new_g, new_b);
								new_g = src_g;
								HSVTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
						}
						else if (layBlend == "lum ")
						{
							if (header.color_mode != CM_CMYK)
							{
								new_r = d[0];
								new_g = d[1];
								new_b = d[2];
								RGBTOHSV(src_r, src_g, src_b);
								RGBTOHSV(new_r, new_g, new_b);
								new_b = src_b;
								HSVTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
						}
						else if (layBlend == "colr")
						{
							if (header.color_mode != CM_CMYK)
							{
								new_r = d[0];
								new_g = d[1];
								new_b = d[2];
								RGBTOHLS(src_r, src_g, src_b);
								RGBTOHLS(new_r, new_g, new_b);
								new_r = src_r;
								new_b = src_b;
								HLSTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
						}
					}
					if (dst_alpha == 0)
					{
						r = src_r;
						g = src_g;
						b = src_b;
						a = src_a;
					}
					else
					{
						if (src_alpha > 0)
						{
							r = (d[0] * (255 - src_alpha) + src_r * src_alpha) / 255;
							g = (d[1] * (255 - src_alpha) + src_g * src_alpha) / 255;
							b = (d[2] * (255 - src_alpha) + src_b * src_alpha) / 255;
							if (header.color_mode == CM_CMYK)
								a = (d[3] * (255 - src_alpha) + src_a * src_alpha) / 255;
							if (layBlend != "diss")
								src_alpha = dst_alpha + INT_MULT(255 - dst_alpha, src_alpha);
						}
					}
					if (src_alpha > 0)
					{
						d[0] = r;
						d[1] = g;
						d[2] = b;
						if (header.color_mode == CM_CMYK)
						{
							d[3] = a;
							d[4] = src_alpha;
						}
						else
							d[3] = src_alpha;
					}
					s += r2_image.channels();
					d += r_image.channels();
					if (hasMask)
						sm += mask.channels();
				}
			}
		}
	}
	*firstLayer = false;
	return true;
}

bool ScImgDataLoader_PSD::loadLayer( QDataStream & s, const PSDHeader & header )
{
	// Find out if the data is compressed.
	// Known values:
	//   0: no compression
	//   1: RLE compressed
	ushort compression;
	uchar cbyte;
	s >> compression;
	if( compression > 1 )
	{
		// Unknown compression type.
		return false;
	}
	uint channel_num = header.channel_count;
	r_image.fill(255);
	const uint pixel_count = header.height * header.width;
	static const uint components[5] = {0, 1, 2, 3, 4};
	if( compression )
	{
		// Skip row lengths.
		ushort w;
		for(uint i = 0; i < header.height * header.channel_count; i++)
		{
			s >> w;
		}
		// Read RLE data.
		uchar * ptr;
		uint count = 0;
		uchar c;
		uint len;
		for(uint channel = 0; channel < channel_num; channel++)
		{
			ptr = r_image.bits() + components[channel];
			count = 0;
			while( count < pixel_count )
			{
				if(s.atEnd())
					return false;
				s >> c;
				len = c;
				if( len < 128 )
				{
					// Copy next len+1 bytes literally.
					len++;
					count += len;
					if ( count > pixel_count )
						return false;
					while( len != 0 )
					{
						s >> cbyte;
						if ((header.color_mode == CM_CMYK) && (components[channel] < 4))
							cbyte = 255 - cbyte;
						if ((header.color_mode == CM_GRAYSCALE) && (components[channel] != 3))
						{
							ptr -= components[channel];
							ptr[0] = cbyte;
							ptr[1] = cbyte;
							ptr[2] = cbyte;
							ptr += components[channel];
						}
						else if ((header.color_mode == CM_DUOTONE) && (components[channel] != 3))
						{
							ptr -= components[channel];
							putDuotone(ptr, cbyte);
							ptr += components[channel];
						}
						else if ((header.color_mode == CM_INDEXED) && (components[channel] != 3))
						{
							ptr -= components[channel];
							int ccol = colorTable[cbyte];
							ptr[0] = qRed(ccol);
							ptr[1] = qGreen(ccol);
							ptr[2] = qBlue(ccol);
							ptr += components[channel];
						}
						else
							*ptr = cbyte;
						ptr += r_image.channels();
						len--;
					}
				}
				else if( len > 128 )
				{
					// Next -len+1 bytes in the dest are replicated from next source byte.
					// (Interpret len as a negative 8-bit int.)
					len ^= 0xFF;
					len += 2;
					count += len;
					if(s.atEnd() || count > pixel_count)
						return false;
					uchar val;
					s >> val;
					if ((header.color_mode == CM_CMYK) && (components[channel] < 4))
						val = 255 - val;
					while( len != 0 )
					{
						if ((header.color_mode == CM_GRAYSCALE) && (components[channel] != 3))
						{
							ptr -= components[channel];
							ptr[0] = val;
							ptr[1] = val;
							ptr[2] = val;
							ptr += components[channel];
						}
						else if ((header.color_mode == CM_DUOTONE) && (components[channel] != 3))
						{
							ptr -= components[channel];
							putDuotone(ptr, val);
							ptr += components[channel];
						}
						else if ((header.color_mode == CM_INDEXED) && (components[channel] != 3))
						{
							ptr -= components[channel];
							int ccol = colorTable[val];
							ptr[0] = qRed(ccol);
							ptr[1] = qGreen(ccol);
							ptr[2] = qBlue(ccol);
							ptr += components[channel];
						}
						else
							*ptr = val;
						ptr += r_image.channels();
						len--;
					}
				}
				else if( len == 128 )
				{
					// No-op.
				}
			}
		}
	}
	else
	{
		// We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
		// where each channel consists of an 8-bit value for each pixel in the image.
		// Read the data by channel.
		uchar * ptr;
		uint count = 0;
		for(uint channel = 0; channel < channel_num; channel++)
		{
			ptr =  r_image.bits() + components[channel];
			// Read the data.
			count = pixel_count;
			while( count != 0 )
			{
				s >> cbyte;
				if ((header.color_mode == CM_CMYK) && (components[channel] < 4))
					cbyte = 255 - cbyte;
				if ((header.color_mode == CM_GRAYSCALE) && (components[channel] != 3))
				{
					ptr -= components[channel];
					ptr[0] = cbyte;
					ptr[1] = cbyte;
					ptr[2] = cbyte;
					ptr += components[channel];
				}
				else if ((header.color_mode == CM_DUOTONE) && (components[channel] != 3))
				{
					ptr -= components[channel];
					putDuotone(ptr, cbyte);
					ptr += components[channel];
				}
				else if ((header.color_mode == CM_INDEXED) && (components[channel] != 3))
				{
					ptr -= components[channel];
					int ccol = colorTable[cbyte];
					ptr[0] = qRed(ccol);
					ptr[1] = qGreen(ccol);
					ptr[2] = qBlue(ccol);
					ptr += components[channel];
				}
				else
					*ptr = cbyte;
				ptr += r_image.channels();
				count--;
			}
		}
	}
	if (header.color_mode == CM_LABCOLOR)
	{
		cmsHPROFILE hsRGB = cmsCreate_sRGBProfile();
		cmsHPROFILE hLab  = cmsCreateLabProfile(NULL);
		DWORD inputProfFormat = (COLORSPACE_SH(PT_Lab)|CHANNELS_SH(3)|BYTES_SH(1)|EXTRA_SH(1));
		cmsHTRANSFORM xform = cmsCreateTransform(hLab, inputProfFormat, hsRGB, TYPE_RGBA_8, INTENT_PERCEPTUAL, 0);
		for (int i = 0; i < r_image.height(); i++)
		{
			LPBYTE ptr = r_image.scanLine(i);
			cmsDoTransform(xform, ptr, ptr, r_image.width());
		}
		cmsDeleteTransform (xform);
		cmsCloseProfile(hsRGB);
		cmsCloseProfile(hLab);
	}
	return true;
}

QString ScImgDataLoader_PSD::getLayerString(QDataStream & s)
{
	uchar len, tmp;
	uint adj;
	QString ret = "";
	s >> len;
	if (len == 0)
	{
		s >> tmp;
		s >> tmp;
		s >> tmp;
		return ret;
	}
	for( int i = 0; i < len; i++ )
	{
		s >> tmp;
		ret += QChar(tmp);
	}
	adj = 0;
	if (((ret.length()+1) % 4) != 0)
		adj = 4 - ((ret.length()+1) % 4);
	s.device()->seek( s.device()->pos() + adj );
	return ret;
}

bool ScImgDataLoader_PSD::IsValid( const PSDHeader & header )
{
	if( header.signature != 0x38425053 )
		return false;
	return true;
}

// Check that the header is supported.
bool ScImgDataLoader_PSD::IsSupported( const PSDHeader & header )
{
	if ( header.version != 1 )
		return false;
	if ( header.channel_count > 16 )
		return false;
	if ( header.depth != 8 )
		return false;
	if ((header.color_mode == CM_RGB) || (header.color_mode == CM_CMYK) || (header.color_mode == CM_LABCOLOR)
	 || (header.color_mode == CM_GRAYSCALE) || (header.color_mode == CM_INDEXED) || (header.color_mode == CM_DUOTONE))
		return true;
	return false;
}

void ScImgDataLoader_PSD::putDuotone(uchar *ptr, uchar cbyte)
{
	CMYKColor cmyk;
	int c, c1, c2, c3, m, m1, m2, m3, y, y1, y2, y3, k, k1, k2, k3;
	uchar cb = 255 - cbyte;
	ScColor col;
	if (colorTableSc.count() == 1)
	{
		colorTableSc[0].getRawRGBColor(&c, &m, &y);
		ptr[0] = qMin((c * curveTable1[(int)cbyte]) >> 8, 255);
		ptr[1] = qMin((m * curveTable1[(int)cbyte]) >> 8, 255);
		ptr[2] = qMin((y * curveTable1[(int)cbyte]) >> 8, 255);
	}
	else if (colorTableSc.count() == 2)
	{
		ScColorEngine::getCMYKValues(colorTableSc[0], NULL, cmyk);
		cmyk.getValues(c, m, y, k);
		c = qMin((c * curveTable1[(int)cb]) >> 8, 255);
		m = qMin((m * curveTable1[(int)cb]) >> 8, 255);
		y = qMin((y * curveTable1[(int)cb]) >> 8, 255);
		k = qMin((k * curveTable1[(int)cb]) >> 8, 255);
		ScColorEngine::getCMYKValues(colorTableSc[1], NULL, cmyk);
		cmyk.getValues(c1, m1, y1, k1);
		c1 = qMin((c1 * curveTable2[(int)cb]) >> 8, 255);
		m1 = qMin((m1 * curveTable2[(int)cb]) >> 8, 255);
		y1 = qMin((y1 * curveTable2[(int)cb]) >> 8, 255);
		k1 = qMin((k1 * curveTable2[(int)cb]) >> 8, 255);
		col = ScColor(qMin(c+c1, 255), qMin(m+m1, 255), qMin(y+y1, 255), qMin(k+k1, 255));
		col.getRawRGBColor(&c, &m, &y);
		ptr[0] = c;
		ptr[1] = m;
		ptr[2] = y;
	}
	else if (colorTableSc.count() == 3)
	{
		ScColorEngine::getCMYKValues(colorTableSc[0], NULL, cmyk);
		cmyk.getValues(c, m, y, k);
		c = qMin((c * curveTable1[(int)cb]) >> 8, 255);
		m = qMin((m * curveTable1[(int)cb]) >> 8, 255);
		y = qMin((y * curveTable1[(int)cb]) >> 8, 255);
		k = qMin((k * curveTable1[(int)cb]) >> 8, 255);
		ScColorEngine::getCMYKValues(colorTableSc[1], NULL, cmyk);
		cmyk.getValues(c1, m1, y1, k1);
		c1 = qMin((c1 * curveTable2[(int)cb]) >> 8, 255);
		m1 = qMin((m1 * curveTable2[(int)cb]) >> 8, 255);
		y1 = qMin((y1 * curveTable2[(int)cb]) >> 8, 255);
		k1 = qMin((k1 * curveTable2[(int)cb]) >> 8, 255);
		ScColorEngine::getCMYKValues(colorTableSc[2], NULL, cmyk);
		cmyk.getValues(c2, m2, y2, k2);
		c2 = qMin((c2 * curveTable3[(int)cb]) >> 8, 255);
		m2 = qMin((m2 * curveTable3[(int)cb]) >> 8, 255);
		y2 = qMin((y2 * curveTable3[(int)cb]) >> 8, 255);
		k2 = qMin((k2 * curveTable3[(int)cb]) >> 8, 255);
		col = ScColor(qMin(c+c1+c2, 255), qMin(m+m1+m2, 255), qMin(y+y1+y2, 255), qMin(k+k1+k2, 255));
		col.getRawRGBColor(&c, &m, &y);
		ptr[0] = c;
		ptr[1] = m;
		ptr[2] = y;
	}
	else if (colorTableSc.count() == 4)
	{
		ScColorEngine::getCMYKValues(colorTableSc[0], NULL, cmyk);
		cmyk.getValues(c, m, y, k);
		c = qMin((c * curveTable1[(int)cb]) >> 8, 255);
		m = qMin((m * curveTable1[(int)cb]) >> 8, 255);
		y = qMin((y * curveTable1[(int)cb]) >> 8, 255);
		k = qMin((k * curveTable1[(int)cb]) >> 8, 255);
		ScColorEngine::getCMYKValues(colorTableSc[1], NULL, cmyk);
		cmyk.getValues(c1, m1, y1, k1);
		c1 = qMin((c1 * curveTable2[(int)cb]) >> 8, 255);
		m1 = qMin((m1 * curveTable2[(int)cb]) >> 8, 255);
		y1 = qMin((y1 * curveTable2[(int)cb]) >> 8, 255);
		k1 = qMin((k1 * curveTable2[(int)cb]) >> 8, 255);
		ScColorEngine::getCMYKValues(colorTableSc[2], NULL, cmyk);
		cmyk.getValues(c2, m2, y2, k2);
		c2 = qMin((c2 * curveTable3[(int)cb]) >> 8, 255);
		m2 = qMin((m2 * curveTable3[(int)cb]) >> 8, 255);
		y2 = qMin((y2 * curveTable3[(int)cb]) >> 8, 255);
		k2 = qMin((k2 * curveTable3[(int)cb]) >> 8, 255);
		ScColorEngine::getCMYKValues(colorTableSc[3], NULL, cmyk);
		cmyk.getValues(c3, m3, y3, k3);
		c3 = qMin((c3 * curveTable4[(int)cb]) >> 8, 255);
		m3 = qMin((m3 * curveTable4[(int)cb]) >> 8, 255);
		y3 = qMin((y3 * curveTable4[(int)cb]) >> 8, 255);
		k3 = qMin((k3 * curveTable4[(int)cb]) >> 8, 255);
		col = ScColor(qMin(c+c1+c2+c3, 255), qMin(m+m1+m2+m3, 255), qMin(y+y1+y2+y3, 255), qMin(k+k1+k2+k3, 255));
		col.getRawRGBColor(&c, &m, &y);
		ptr[0] = c;
		ptr[1] = m;
		ptr[2] = y;
	}
}
