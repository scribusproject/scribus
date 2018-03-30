/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PDFLIBCORE_H
#define PDFLIBCORE_H

#include <QFile>
#include <QDataStream>
#include <QPixmap>
#include <QList>
#include <QStack>
#include <string>
#include <vector>

class QImage;
class QRect;
class QString;
class QTextCodec;
class PageItem;
class BookMItem;
class BookMView;
class ScribusDoc;
class ScPage;
class PDFOptions;
class PrefsContext;
class MultiProgressDialog;
class ScLayer;
class ScText;

#include "pdfoptions.h"
#include "pdfstructs.h"
#include "scribusstructs.h"
#include "scimagestructs.h"
#include "tableborder.h"

#ifdef HAVE_PODOFO
#include <podofo/podofo.h>
#endif

#include "pdfwriter.h"

class PdfPainter;

/**
 * PDFLibCore provides Scribus's implementation of PDF export functionality.
 *
 * This is not public API and is not exported in Scribus's symbol table on supporting
 * platforms. Do not include this header, use pdflib.h instead. Including this header
 * from anywhere except pdflib.cpp may cause linker errors and other problems,
 * and is totally pointless since all the interfaces that are publicly
 * visible are presented by PDFlib anyway.
 *
 *@author Franz Schmid
 */

class PDFLibCore : public QObject
{
	Q_OBJECT

friend class PdfPainter;

public:
	explicit PDFLibCore(ScribusDoc & docu);
	~PDFLibCore();
	bool doExport(const QString& fn, const QString& nam, int Components,
				  const std::vector<int> & pageNs, const QMap<int, QImage> & thumbs);

	const QString& errorMessage(void) const;
	bool  exportAborted(void) const;

private:
	struct ShIm
	{
		PdfId ResNum;
		int Width;
		int Height;
		int Page;
		double reso;
		double sxa;
		double sya;
		double xa;
		double ya;
		double origXsc;
		double origYsc;
		QMap<int, ImageLoadRequest> RequestProps;
	};

	bool PDF_IsPDFX();
	bool PDF_IsPDFX(PDFOptions::PDFVersion ver);

	bool PDF_Begin_Doc(const QString& fn, SCFonts &AllFonts, const QMap<QString, QMap<uint, FPointArray> >& DocFonts, BookMView* vi);
	void PDF_Begin_Catalog();
	void PDF_Begin_MetadataAndEncrypt();
	QMap<QString, QMap<uint, FPointArray> >
	     PDF_Begin_FindUsedFonts(SCFonts &AllFonts, const QMap<QString, QMap<uint, FPointArray> >& DocFonts);
	void PDF_Begin_WriteUsedFonts(SCFonts &AllFonts, const QMap<QString, QMap<uint, FPointArray> >& ReallyUsed);
	void PDF_WriteStandardFonts();
	PdfFont PDF_WriteType3Font(const QByteArray& name, ScFace& face, const QMap<uint,FPointArray>& RealGlyphs);
	PdfFont PDF_WriteGlyphsAsXForms(const QByteArray& fontName, ScFace& face, const QMap<uint,FPointArray>& RealGlyphs);
	
	QByteArray PDF_GenerateSubsetTag(const QByteArray& fontName, QList<uint> usedGlyphs);
	PdfId PDF_WriteFontDescriptor(const QByteArray& fontName, ScFace& face, ScFace::FontFormat fformat, PdfId embeddedFontObject);
	PdfFont PDF_WriteTtfSubsetFont(const QByteArray& fontName, ScFace& face, const QMap<uint,FPointArray>& RealGlyphs);
	PdfFont PDF_WriteCffSubsetFont(const QByteArray& fontName, ScFace& face, const QMap<uint,FPointArray>& RealGlyphs);
	PdfFont PDF_EncodeSimpleFont(const QByteArray& fontname, ScFace& face,  const QByteArray& baseFont, const QByteArray& subtype, bool isEmbedded, PdfId fontDes, const ScFace::FaceEncoding& gl);
	PdfFont PDF_EncodeCidFont(const QByteArray& fontname, ScFace& face, const QByteArray& baseFont, PdfId fontDes, const ScFace::FaceEncoding& gl, const QMap<uint,uint> glyphmap);
	PdfFont PDF_EncodeFormFont(const QByteArray& fontname, ScFace& face,  const QByteArray& baseFont, const QByteArray& subtype, PdfId fontDes);
	PdfId PDF_EmbedFontObject(const QString& fontName, ScFace &face);
	PdfId PDF_EmbedFontObject(const QByteArray& ttf, const QByteArray& subtype);
	PdfId PDF_EmbedType1AsciiFontObject(const QByteArray& fontData);
	PdfId PDF_EmbedType1BinaryFontObject(const QByteArray& fontData);

	void PDF_Begin_Colors();
	void PDF_Begin_Layers();
	
	void PDF_Begin_Page(const ScPage* pag, const QImage& thumb);
	void PDF_End_Page();
	bool PDF_TemplatePage(const ScPage* pag, bool clip = false);
	bool PDF_ProcessPage(const ScPage* pag, uint PNr, bool clip = false);
	bool PDF_ProcessMasterElements(const ScLayer& layer, const ScPage* page, uint PNr);
	bool PDF_ProcessPageElements(const ScLayer& layer, const ScPage* page, uint PNr);
	
	bool PDF_End_Doc(const QString& PrintPr = "", const QString& Name = "", int Components = 0);
	void PDF_End_Bookmarks();
	void PDF_End_Resources();
	void PDF_End_Outlines();
	void PDF_End_PageTree();
	void PDF_End_NamedDests();
	void PDF_End_FormObjects();
	void PDF_End_JavaScripts();
	void PDF_End_Articles();
	void PDF_End_Layers();
	void PDF_End_OutputProfile(const QString& PrintPr, const QString& Name, int Components);
	void PDF_End_Metadata();
	bool PDF_End_XRefAndTrailer();
	bool closeAndCleanup();

	void PDF_Error(const QString& errorMsg);
	void PDF_Error_WriteFailure(void);
	void PDF_Error_ImageLoadFailure(const QString& fileName);
	void PDF_Error_ImageWriteFailure(const QString& fileName);
	void PDF_Error_MaskLoadFailure(const QString& fileName);
	void PDF_Error_InsufficientMemory(void);

//	QByteArray EncodeUTF16(const QString &in);
	QByteArray EncStream(const QByteArray & in, PdfId ObjNum);
	QByteArray EncString(const QByteArray & in, PdfId ObjNum);
	QByteArray EncStringUTF16(const QString & in, PdfId ObjNum);

	bool       EncodeArrayToStream(const QByteArray& in, PdfId ObjNum);

	int     WriteImageToStream(ScImage& image, PdfId ObjNum, ColorSpaceEnum format, bool precal);
	int     WriteJPEGImageToStream(ScImage& image, const QString& fn, PdfId ObjNum, int quality, ColorSpaceEnum format, bool sameFile, bool precal);
	int     WriteFlateImageToStream(ScImage& image, PdfId ObjNum, ColorSpaceEnum format, bool precal);

//	void    CalcOwnerKey(const QString & Owner, const QString & User);
//	void    CalcUserKey(const QString & User, int Permission);
//	QString FitKey(const QString & pass);

	QByteArray SetClipPath(PageItem *ite, bool poly = true);
	QByteArray SetClipPathArray(FPointArray *ite, bool poly = true);
	QByteArray SetClipPathImage(PageItem *ite);

	QByteArray SetImagePathAndClip(PageItem *item);
	QByteArray SetPathAndClip(PageItem *item);
	QByteArray SetPathAndClip(PageItem *item, bool fillRule);
	
	QByteArray setStrokeMulti(struct SingleLine *sl);
	QByteArray SetColor(const QString& farbe, double Shade);
	QByteArray SetColor(const ScColor& farbe, double Shade);
	QByteArray SetGradientColor(const QString& farbe, double Shade);
	QByteArray putColor(const QString& color, double Shade, bool fill);
	QByteArray putColorUncached(const QString& color, int Shade, bool fill);
    QByteArray Write_FormXObject(QByteArray &data, PageItem *controlItem = 0);
	QByteArray Write_TransparencyGroup(double trans, int blend, QByteArray &data, PageItem *controlItem = 0);
	QByteArray setTextSt(PageItem *ite, uint PNr, const ScPage* pag);
	void    getBleeds(const ScPage* page, double &left, double &right);
	void    getBleeds(const ScPage* page, double &left, double &right, double &bottom, double& top);

//	// Provide a couple of PutDoc implementations to ease transition away from
//	// QString abuse and to provide fast paths for constant strings.
//	void PutDoc(const QString & in) { outStream.writeRawData(in.toLatin1(), in.length()); }
//	void PutDoc(const QByteArray & in) { outStream.writeRawData(in, in.size()); }
//	void PutDoc(const char* in) { outStream.writeRawData(in, strlen(in)); }
//	void PutDoc(const std::string & in) { outStream.writeRawData(in.c_str(), in.length()); }

	void       PutPage(const QByteArray & in) { Content += in; }
//	void       StartObj(PdfId nr);
//	uint       newObject() { return ObjCounter++; }
	uint       WritePDFStream(const QByteArray& cc);
	uint       WritePDFStream(const QByteArray& cc, PdfId objId);
	uint       WritePDFString(const QString& cc);
	uint       WritePDFString(const QString& cc, PdfId objId);
	void       writeXObject(uint objNr, QByteArray dictionary, QByteArray stream);
	uint       writeObject(QByteArray type, QByteArray dictionary);
	uint       writeGState(QByteArray dictionary) { return writeObject("/ExtGState", dictionary); }
	uint       writeActions(const Annotation&, uint annotationObj);
//	QString    PDFEncode(const QString & in);
//	QByteArray ComputeMD5(const QString& in);
//	QByteArray ComputeRC4Key(PdfId ObjNum);

	QByteArray PDF_PutSoftShadow(PageItem* ite, const ScPage* pag);
	bool    PDF_ProcessItem(QByteArray& output, PageItem* ite, const ScPage* pag, uint PNr, bool embedded = false, bool pattern = false);
	void    PDF_Bookmark(PageItem *currItem, double ypos);
	bool	PDF_HatchFill(QByteArray& output, PageItem *currItem);
	bool    PDF_PatternFillStroke(QByteArray& output, PageItem *currItem, int kind = 0, bool forArrow = false);
	bool    PDF_MeshGradientFill(QByteArray& output, PageItem *currItem);
	bool	PDF_PatchMeshGradientFill(QByteArray& output, PageItem *c);
	bool    PDF_DiamondGradientFill(QByteArray& output, PageItem *currItem);
	bool    PDF_TensorGradientFill(QByteArray& output, PageItem *currItem);
	bool    PDF_GradientFillStroke(QByteArray& output, PageItem *currItem, bool stroke = false, bool forArrow = false);
	QByteArray PDF_TransparenzFill(PageItem *currItem);
	QByteArray PDF_TransparenzStroke(PageItem *currItem);
#ifdef HAVE_OSG
	bool    PDF_3DAnnotation(PageItem *ite, uint PNr);
#endif
	void	PDF_RadioButtons();
	uint    PDF_RadioButton(PageItem *ite, uint parent, QString parentName);
	bool    PDF_Annotation(PageItem *ite, uint PNr);
	void    PDF_Form(const QByteArray& im);
	void    PDF_xForm(uint objNr, double w, double h, QByteArray im);
	bool    PDF_Image(PageItem* c, const QString& fn, double sx, double sy, double x, double y, bool fromAN = false, const QString& Profil = "", bool Embedded = false, eRenderIntent Intent = Intent_Relative_Colorimetric, QByteArray* output = NULL);
	bool    PDF_EmbeddedPDF(PageItem* c, const QString& fn, double sx, double sy, double x, double y, bool fromAN, ShIm& imgInfo, bool &fatalError);
#if HAVE_PODOFO
	void copyPoDoFoObject(const PoDoFo::PdfObject* obj, uint scObjID, QMap<PoDoFo::PdfReference, uint>& importedObjects);
	void copyPoDoFoDirect(const PoDoFo::PdfVariant* obj, QList<PoDoFo::PdfReference>& referencedObjects, QMap<PoDoFo::PdfReference, uint>& importedObjects);
#endif

	quint32 encode32dVal(double val);
	quint16 encode16dVal(double val);
	void    encodeColor(QDataStream &vs, QString colName, int colShade, QStringList &spotColorSet, bool spotMode);

	QByteArray drawArrow(PageItem *ite, QTransform &arrowTrans, int arrowIndex);
	QByteArray createBorderAppearance(PageItem *ite);
	QByteArray paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end, const QPointF& startOffsetFactors, const QPointF& endOffsetFactors);
	QByteArray handleBrushPattern(PageItem* ite, QPainterPath &path, const ScPage* pag, uint PNr);

	void generateXMP(const QString& timeStamp);
//	int bytesWritten() { return Spool.pos(); }

	PdfCatalog Catalog;
	PdfPageData pageData;
	PdfPageTree PageTree;
	PdfOutlines Outlines;
	Pdf::Writer writer;
	QString baseDir;
	
	QByteArray Content;
	QString ErrorMessage;
	ScribusDoc & doc;
	const ScPage * ActPageP;
	const PDFOptions & Options;
	BookMView* Bvie;
	//int Dokument;
	QMap<QString,ShIm> SharedImages;
	QList<PdfDest> NamedDest;
	QList<PdfId> CalcFields;
	Pdf::ResourceMap Patterns;
	Pdf::ResourceMap Shadings;
	Pdf::ResourceMap Transpar;
	QMap<QString,PdfICCD> ICCProfiles;
	QHash<QString, PdfOCGInfo> OCGEntries;
	QTextCodec* ucs2Codec;
	QByteArray ResNam;
	int ResCount;
	QByteArray NDnam;
	QByteArray Datum;
	int NDnum;
	QMap<QString, PdfFont> UsedFontsP;
	QMap<QString, PdfFont> UsedFontsF;
//	QByteArray KeyGen;
//	QByteArray OwnerKey;
//	QByteArray UserKey;
//	QByteArray FileID;
//	QByteArray EncryKey;
//	PdfId Encrypt;
//	int KeyLen;
	QByteArray HTName;
	bool BookMinUse;
	ColorList colorsToUse;
	QMap<QString, PdfSpotC> spotMap;
	QMap<QString, PdfSpotC> spotMapReg;
	QByteArray spotNam;
	int spotCount;
	int inPattern;
	QMap<QString, QString> StdFonts;
	MultiProgressDialog* progressDialog;
	bool abortExport;
	bool usingGUI;
	double bleedDisplacementX;
	double bleedDisplacementY;
	QByteArray xmpPacket;
	QStack<QPointF> groupStackPos;
	QStack<QPointF> patternStackPos;

protected slots:
	void cancelRequested();
};

#endif



