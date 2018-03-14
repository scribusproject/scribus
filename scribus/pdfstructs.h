//
//  pdfstruct.h
//  Scribus
//
//  Created by Andreas Vox on 25.12.14.
//
//

#ifndef Scribus_pdfstructs_h
#define Scribus_pdfstructs_h

#include<QByteArray>
#include<QList>
#include<QMap>
#include<QRect>
#include<QString>

class PageItem;

typedef uint PdfId;

namespace Pdf {
	typedef QMap<QByteArray, PdfId> ResourceMap;
	
	struct Resource
	{
		QByteArray ResName;
		PdfId ResNum;
	};

	struct ResourceDictionary
	{
		ResourceMap XObject;
		ResourceMap Font;
		ResourceMap Shading;
		ResourceMap Pattern;
		ResourceMap ExtGState;
		ResourceMap Properties;
		QList<Resource> ColorSpace;
	};
	
}



struct PdfDest
{
	QString Name;
	int PageNr;
	QString Act;
};


struct PdfCatalog
{
	PdfId Outlines;
	PdfId PageTree;
	PdfId Dest;
};


struct PdfPageTree
{
	QList<PdfId> Kids;
	QMap<int, PdfId> KidsMap;
};


struct PdfPageData
{
	PdfId ObjNum;
	PdfId Thumb;
	Pdf::ResourceMap XObjects;
	Pdf::ResourceMap ImgObjects;
	Pdf::ResourceMap FObjects;
	QList<PdfId> AObjects;
	QList<PdfId> FormObjects;
	QList<PageItem*> radioButtonList;
};


struct PdfOutlines
{
	PdfId First;
	PdfId Last;
	PdfId Count;
};


struct PdfBead
{
	PdfId Parent;
	PdfId Next;
	PdfId Prev;
	int Page;
	QRect Rect;
};

struct PdfICCD : public Pdf::Resource
{
	int components;
	QByteArray ICCArray;
};


struct PdfOCGInfo
{
	PdfId ObjNum;
	bool visible;
	QByteArray Name;
};


enum FontMethod {
	Use_System = 0,
	Use_Embedded = 1,
	Use_Subset = 2,
	Use_Type3 = 3,
	Use_XForm = 4
};


enum FontEncoding {
	Encode_256 = 256,
	Encode_224 = 224,
	Encode_IdentityH = 1,
	Encode_Subset = 0
};


enum FontUsage {
	Used_in_Content = 16,
	Used_in_Forms = 32
};



/*
 
  QByteArray fontFile   : ttf
  QByteArray pdfName    : psname w/o special chars
  FaceEncoding encoding : document gid -> unicode / name
  QMap<uint,uint> gmap  : document gid -> font gid
  {t1, t1c, ttf} x {sfnt, ps} x {plain, ttfc, cff, fond, zip}
 
  = Encoding, data, method
  * IdentityH, ttf / cff cid,   xxxx Tj
  * IdentityH, subset ttf cid,  gmap[xxxx] Tj
  * enc224, type3 outlines,     /Foyy Tf xx Tj
  * enc256, xform outlines,     /objyyxx Do
  * enc224, pfa/pfb/t42,        /Foyy Tf xx Tj
  * stdenc, -/-                 cc Tj
 
  face -> contours, metrics, cmap, font descriptor, rawdata
 */
struct PdfFont {
	/// as used in PDF, with leading '/'
	QByteArray name;
	/// how it's embedded
	FontMethod method;
	/// maps document gids to unicode
	FontEncoding encoding;
	/// used for content or for forms
	FontUsage usage;
	/// maps documents gids to font gids
	QMap<uint,uint> glyphmap;
};


struct PdfSpotC : public Pdf::Resource
{
	// no additional data
};


//struct PdfGraphicsData
//{
//	PdfId ResNumG;
//	PdfId ResNumX;
//	QString ResNamG;
//	QString ResNamX;
//	QString data;
//};


#endif
