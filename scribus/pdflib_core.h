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
#include <QMultiMap>
#include <QStack>
#include <string>
#include <vector>

class QImage;
class QRect;
class QString;
class QTextCodec;
class PageItem;
class BookMItem;
class BookmarkView;
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
	explicit PDFLibCore(ScribusDoc & docu, const PDFOptions& options);
	~PDFLibCore();

	bool doExport(const QString& fn, const std::vector<int> & pageNs, const QMap<int, QImage> & thumbs);

	const QString& errorMessage() const;
	bool  exportAborted() const;

private:
	struct ShIm
	{
		PdfId ResNum = 0;
		int   Width = 0;
		int   Height = 0;
		int   Page = 0;
		double reso = 1.0;
		double sxa = 0;
		double sya = 0;
		double xa = 0;
		double ya = 0;
		double origXsc = 1.0;
		double origYsc = 1.0;
		bool isBitmapFromGS = false;
		bool isEmbeddedPDF = false;
		bool useEmbeddedProfile = true;
		QString inputProfile;
		eRenderIntent renderingIntent { Intent_Perceptual };
		ScImageEffectList imageEffects;
		QMap<int, ImageLoadRequest> RequestProps;
	};

	class SharedImgRsrc : public QMultiMap<QString, ShIm>
	{
	public:

		QMultiMap<QString, ShIm>::const_iterator findSuitable(const QString& fileName, const ShIm& shIm) const
		{
			auto it = this->find(fileName);
			for ( ; (it != this->end()) && (it.key() == fileName); ++it)
			{
				if (it->Page != shIm.Page)
					continue;
				if (it->origXsc != shIm.origXsc)
					continue;
				if (it->origYsc != shIm.origYsc)
					continue;
				if (it->useEmbeddedProfile != shIm.useEmbeddedProfile)
					continue;
				if (it->inputProfile != shIm.inputProfile)
					continue;
				if (it->renderingIntent != shIm.renderingIntent)
					continue;
				if (it->imageEffects != shIm.imageEffects)
					continue;
				if (it->RequestProps != shIm.RequestProps)
					continue;
				return it;
			}
			return this->end();
		}

		bool containsSuitable(const QString& fileName, const ShIm& shIm) const
		{
			return (findSuitable(fileName, shIm) != this->end());
		}
	};

	bool PDF_HasXMP() const;
	bool PDF_IsPDFX() const;
	bool PDF_IsPDFX(const PDFVersion& ver) const;

	bool PDF_Begin_Doc(const QString& fn, SCFonts &AllFonts, const QMap<QString, QMap<uint, QString> >& DocFonts, BookmarkView* vi);
	void PDF_Begin_Catalog();
	void PDF_Begin_MetadataAndEncrypt();
	QMap<QString, QMap<uint, QString> > PDF_Begin_FindUsedFonts(const QMap<QString, QMap<uint, QString> >& DocFonts);
	void PDF_Begin_WriteUsedFonts(SCFonts &AllFonts, const QMap<QString, QMap<uint, QString> >& usedFonts);
	void PDF_WriteStandardFonts();
	PdfFont PDF_WriteType3Font(const QByteArray& name, ScFace& face, const QMap<uint, QString>& usedGlyphs);
	PdfFont PDF_WriteGlyphsAsXForms(const QByteArray& fontName, const ScFace& face, const QMap<uint, QString>& usedGlyphs);
	
	QByteArray PDF_GenerateSubsetTag(const QByteArray& fontName, const QList<uint>& usedGlyphs) const;
	PdfId PDF_WriteFontDescriptor(const QByteArray& fontName, const ScFace& face, ScFace::FontFormat fformat, PdfId embeddedFontObject);
	PdfFont PDF_WriteTtfSubsetFont(const QByteArray& fontName, ScFace& face, const QMap<uint, QString>& usedGlyphs);
	PdfFont PDF_WriteCffSubsetFont(const QByteArray& fontName, ScFace& face, const QMap<uint, QString>& usedGlyphs);
	PdfFont PDF_WriteOpenTypeSubsetFont(const QByteArray& fontName, ScFace& face, const QMap<uint, QString>& usedGlyphs);
	PdfFont PDF_EncodeSimpleFont(const QByteArray& fontname, ScFace& face,  const QByteArray& baseFont, const QByteArray& subtype, bool isEmbedded, PdfId fontDes, const QMap<uint, QString>& usedGlyphs);
	PdfFont PDF_EncodeCidFont(const QByteArray& fontname, ScFace& face, const QByteArray& baseFont, PdfId fontDes, const QMap<uint, QString>& usedGlyphs, const QMap<uint, uint>& glyphmap);
	PdfFont PDF_EncodeFormFont(const QByteArray& fontname, const ScFace& face,  const QByteArray& baseFont, const QByteArray& subtype, PdfId fontDes);
	PdfId PDF_EmbedFontObject(const QString& fontName, ScFace &face);
	PdfId PDF_EmbedFontObject(const QByteArray& font, const QByteArray& subtype);
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
	
	bool PDF_End_Doc(const QString& outputProfilePath = QString());
	void PDF_End_Bookmarks();
	void PDF_End_Resources();
	void PDF_End_Outlines();
	void PDF_End_PageTree();
	void PDF_End_NamedDests();
	void PDF_End_FormObjects();
	void PDF_End_JavaScripts();
	void PDF_End_Articles();
	void PDF_End_Layers();
	bool PDF_End_OutputProfile(const QString& profilePath);
	void PDF_End_Metadata();
	bool PDF_End_XRefAndTrailer();
	bool closeAndCleanup();

	void PDF_Error(const QString& errorMsg);
	void PDF_Error_WriteFailure();
	void PDF_Error_ImageLoadFailure(const QString& fileName);
	void PDF_Error_ImageWriteFailure(const QString& fileName);
	void PDF_Error_MaskLoadFailure(const QString& fileName);
	void PDF_Error_InsufficientMemory();

	QByteArray EncStream(const QByteArray & in, PdfId ObjNum);
	QByteArray EncString(const QByteArray & in, PdfId ObjNum);
	QByteArray EncStringUTF16(const QString & in, PdfId ObjNum);

	bool       EncodeArrayToStream(const QByteArray& in, PdfId ObjNum);

	int     WriteImageToStream(const ScImage& image, PdfId ObjNum, ColorSpaceEnum format, bool precal);
	int     WriteJPEGImageToStream(ScImage& image, const QString& fn, PdfId ObjNum, int quality, ColorSpaceEnum format, bool sameFile, bool precal);
	int     WriteFlateImageToStream(const ScImage& image, PdfId ObjNum, ColorSpaceEnum format, bool precal);

//	void    CalcOwnerKey(const QString & Owner, const QString & User);
//	void    CalcUserKey(const QString & User, int Permission);
//	QString FitKey(const QString & pass);

	QByteArray SetClipPath(const PageItem *ite, bool poly = true) const;
	QByteArray SetClipPathArray(const FPointArray *ite, bool poly = true) const;
	QByteArray SetClipPathImage(const PageItem *ite) const;

	QByteArray SetImagePathAndClip(const PageItem *item) const;
	QByteArray SetPathAndClip(const PageItem *item) const;
	QByteArray SetPathAndClip(const PageItem *item, bool fillRule) const;
	
	QByteArray setStrokeMulti(const SingleLine *sl);
	QByteArray SetColor(const QString& farbe, double Shade) const;
	QByteArray SetColor(const ScColor& farbe, double Shade) const;
	QByteArray SetGradientColor(const QString& farbe, double Shade);
	QByteArray putColor(const QString& color, double Shade, bool fill);
	QByteArray putColorUncached(const QString& color, int Shade, bool fill);
    QByteArray Write_FormXObject(QByteArray &data, const PageItem *controlItem = 0);
	QByteArray Write_TransparencyGroup(double trans, int blend, QByteArray &data, const PageItem *controlItem = 0);
	QByteArray setTextSt(PageItem *ite, uint PNr, const ScPage* pag);

	void    getBleeds(const ScPage* page, double &left, double &right) const;
	void    getBleeds(const ScPage* page, double &left, double &right, double &bottom, double& top) const;

//	// Provide a couple of PutDoc implementations to ease transition away from
//	// QString abuse and to provide fast paths for constant strings.
//	void PutDoc(const QString & in) { outStream.writeRawData(in.toLatin1(), in.length()); }
//	void PutDoc(const QByteArray & in) { outStream.writeRawData(in, in.size()); }
//	void PutDoc(const char* in) { outStream.writeRawData(in, strlen(in)); }
//	void PutDoc(const std::string & in) { outStream.writeRawData(in.c_str(), in.length()); }

	void       PutPage(const QByteArray & in) { Content += in; }
//	uint       newObject() { return ObjCounter++; }
	uint       WritePDFStream(const QByteArray& cc);
	uint       WritePDFStream(const QByteArray& cc, PdfId objId);
	uint       WritePDFString(const QString& cc);
	uint       WritePDFString(const QString& cc, PdfId objId);
	void       writeXObject(uint objNr, const QByteArray& dictionary, const QByteArray& stream);
	uint       writeObject(const QByteArray& type, const QByteArray& dictionary);
	uint       writeGState(QByteArray dictionary) { return writeObject("/ExtGState", dictionary); }
	uint       writeActions(const Annotation&, PdfId annotationObj);

	QByteArray PDF_PutSoftShadow(PageItem* ite);
	bool    PDF_ProcessItem(QByteArray& output, PageItem* ite, const ScPage* pag, uint PNr, bool embedded = false, bool pattern = false);
	void    PDF_Bookmark(const PageItem *currItem, double ypos);
	bool	PDF_HatchFill(QByteArray& output, const PageItem *currItem);
	bool    PDF_PatternFillStroke(QByteArray& output, const PageItem *currItem, int kind = 0, bool forArrow = false);
	bool    PDF_MeshGradientFill(QByteArray& output, const PageItem *currItem);
	bool	PDF_PatchMeshGradientFill(QByteArray& output, const PageItem *c);
	bool    PDF_DiamondGradientFill(QByteArray& output, const PageItem *currItem);
	bool    PDF_TensorGradientFill(QByteArray& output, const PageItem *currItem);
	bool    PDF_GradientFillStroke(QByteArray& output, const PageItem *currItem, bool stroke = false, bool forArrow = false);
	QByteArray PDF_TransparenzFill(const PageItem *currItem);
	QByteArray PDF_TransparenzStroke(const PageItem *currItem);
#ifdef HAVE_OSG
	bool    PDF_3DAnnotation(PageItem *ite, uint PNr);
#endif
	void    PDF_RadioButtonGroups();
	PdfId   PDF_RadioButton(PageItem *ite, PdfId parent, const QString& parentName);
	bool    PDF_Annotation(PageItem *ite);
	void    PDF_Form(const QByteArray& im);
	void    PDF_xForm(PdfId objNr, double w, double h, const QByteArray& im);
	bool    PDF_Image(PageItem* c, const QString& fn, double sx, double sy, double x, double y, bool fromAN = false, const QString& Profil = "", bool Embedded = false, eRenderIntent Intent = Intent_Relative_Colorimetric, QByteArray* output = nullptr);
	bool    PDF_EmbeddedPDF(PageItem* c, const QString& fn, double sx, double sy, double x, double y, ShIm& imgInfo, bool &fatalError);
#if HAVE_PODOFO
	void copyPoDoFoObject(const PoDoFo::PdfObject* obj, PdfId scObjID, QMap<PoDoFo::PdfReference, uint>& importedObjects);
	void copyPoDoFoDirect(const PoDoFo::PdfObject* obj, QList<PoDoFo::PdfReference>& referencedObjects, QMap<PoDoFo::PdfReference, uint>& importedObjects);
#endif

	quint32 encode32dVal(double val) const;
	quint16 encode16dVal(double val) const;
	void    encodeColor(QDataStream &vs, const QString& colName, int colShade, const QStringList &spotColorSet, bool spotMode);

	QByteArray drawArrow(PageItem *ite, QTransform &arrowTrans, int arrowIndex);
	QByteArray createBorderAppearance(PageItem *ite);
	QByteArray paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end, const QPointF& startOffsetFactors, const QPointF& endOffsetFactors);
	QByteArray handleBrushPattern(const PageItem* ite, const QPainterPath &path, const ScPage* pag, uint PNr);

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
	const ScPage * ActPageP { nullptr };
	const PDFOptions & Options;
	BookmarkView* Bvie { nullptr };
	//int Dokument;
	SharedImgRsrc SharedImages;
	QList<PdfDest> NamedDest;
	QList<PdfId> CalcFields;
	Pdf::ResourceMap Patterns;
	Pdf::ResourceMap Shadings;
	Pdf::ResourceMap Transpar;
	QMap<QString,PdfICCD> ICCProfiles;
	QHash<QString, PdfOCGInfo> OCGEntries;
	QTextCodec* ucs2Codec { nullptr };
	QByteArray ResNam { "RE" };
	int ResCount { 0 };
	QByteArray NDnam { "LI" };
	QByteArray Datum;
	int NDnum { 0 };
	QMap<QString, PdfFont> UsedFontsP;
	QMap<QString, PdfFont> UsedFontsF;
	QByteArray HTName;
	bool BookMinUse { false };
	ColorList colorsToUse;
	QMap<QString, PdfSpotC> spotMap;
	QMap<QString, PdfSpotC> spotMapReg;
	QByteArray spotNam { "Spot" };
	int spotCount { 0 };
	int inPattern { 0 };
	QMap<QString, QString> StdFonts;
	MultiProgressDialog* progressDialog { nullptr };
	bool abortExport { false };
	bool usingGUI;
	double bleedDisplacementX { 0.0 };
	double bleedDisplacementY { 0.0 };
	QByteArray xmpPacket;
	QStack<QPointF> groupStackPos;
	QStack<QPointF> patternStackPos;

protected slots:
	void cancelRequested();
};

#endif



