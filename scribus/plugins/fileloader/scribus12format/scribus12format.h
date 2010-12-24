/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS12FORMAT_H
#define SCRIBUS12FORMAT_H

#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "scfonts.h"
#include "scribusstructs.h"
#include "styles/styleset.h"

#include <QDomElement>
#include <QMap>
#include <QString>
#include <QList>

class LastStyles;

class PLUGIN_API Scribus12Format : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		Scribus12Format();
		virtual ~Scribus12Format();
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		//Not the same as readSLA. This one only reads max 4k of the file for speed.
		virtual bool fileSupported(QIODevice* file, const QString & fileName=QString::null) const;

		virtual bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0);
		virtual bool saveFile(const QString & fileName, const FileFormat & fmt);
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

		// Special features - .sla page extraction support
		virtual bool loadPage(const QString & fileName, int pageNumber, bool Mpage, QString renamedPageName=QString::null);
		virtual bool readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles);
		virtual bool readLineStyles(const QString& fileName, QMap<QString,multiLine> *Sty);
		virtual bool readColors(const QString& fileName, ColorList & colors);
		virtual bool readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames);
		virtual void getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces);

	private:
		void registerFormats();
		//Scribus Doc vars, not plugin vars
		void GetItemText(QDomElement *it, ScribusDoc *doc, bool VorLFound, bool impo, PageItem* obj, LastStyles* last);
		void GetStyle(QDomElement *pg, ParagraphStyle *vg, StyleSet<ParagraphStyle> *tempParagraphStyles, ScribusDoc* doc, bool fl);
		QString readSLA(const QString & fileName);
		/**
		 * @brief Paste an item to the document.
		 * The bulk of a paste item process runs here for want of a better place, but its a better place
		 * than the view where it used to be. 
		 * TODO Once the pageitem restructure is done, this is probably unnecessary but it removes the 
		 * unnecessary part from the view for now which is overloaded with non ScrollView code.
		 */
		//TODO: void PasteItem(struct CopyPasteBuffer *Buffer, bool loading, bool drag = false);
		QMap<int,int> groupRemap;
		QMap<long long,int> itemRemap;
		QMap<int,long long> itemNext;
		QMap<uint,QString> DoVorl;
		uint VorlC;
		struct CopyPasteBuffer
		{
			PageItem::ItemType PType;
			double Xpos;
			double Ypos;
			double Width;
			double Height;
			double RadRect;
			int FrameType;
			bool ClipEdited;
			double Pwidth;
			QString Pcolor;
			QString Pcolor2;
			double Shade;
			double Shade2;
			int FillRule;
			QString GrColor;
			QString GrColor2;
			int GrShade;
			int GrShade2;
			VGradient fill_gradient;
			int GrType;
			QString pattern;
			double patternScaleX;
			double patternScaleY;
			double patternOffsetX;
			double patternOffsetY;
			double patternRotation;
			double patternSkewX;
			double patternSkewY;
			bool patternMirrorX;
			bool patternMirrorY;
			double GrStartX;
			double GrStartY;
			double GrEndX;
			double GrEndY;
			double GrFocalX;
			double GrFocalY;
			double GrScale;
			double GrSkew;
			VGradient stroke_gradient;
			int GrTypeStroke;
			double GrStrokeStartX;
			double GrStrokeStartY;
			double GrStrokeEndX;
			double GrStrokeEndY;
			double GrStrokeFocalX;
			double GrStrokeFocalY;
			double GrStrokeScale;
			double GrStrokeSkew;
			int GrMask;
			double GrMaskStartX;
			double GrMaskStartY;
			double GrMaskEndX;
			double GrMaskEndY;
			double GrMaskFocalX;
			double GrMaskFocalY;
			double GrMaskScale;
			double GrMaskSkew;
			double patternMaskScaleX;
			double patternMaskScaleY;
			double patternMaskOffsetX;
			double patternMaskOffsetY;
			double patternMaskRotation;
			double patternMaskSkewX;
			double patternMaskSkewY;
			bool patternMaskMirrorX;
			bool patternMaskMirrorY;
			double GrCol1transp;
			double GrCol2transp;
			double GrCol3transp;
			double GrCol4transp;
			int GrCol1Shade;
			int GrCol2Shade;
			int GrCol3Shade;
			int GrCol4Shade;
			QString patternMaskVal;
			VGradient mask_gradient;
			FPoint GrControl1;
			FPoint GrControl2;
			FPoint GrControl3;
			FPoint GrControl4;
			FPoint GrControl5;
			QString GrColorP1;
			QString GrColorP2;
			QString GrColorP3;
			QString GrColorP4;
			QList<QList<meshPoint> > meshGradientArray;
			int mGArrayRows;
			int mGArrayCols;
			QString TxtStroke;
			QString TxtFill;
			int ShTxtStroke;
			int ShTxtFill;
			int TxtScale;
			int TxtScaleV;
			int TxTStyle;
			int TxTBase;
			int TxtShadowX;
			int TxtShadowY;
			int TxtOutline;
			int TxtUnderPos;
			int TxtUnderWidth;
			int TxtStrikePos;
			int TxtStrikeWidth;
			double Rot;
			int PLineArt;
			int PLineJoin;
			int PLineEnd;
			double LineSp;
			int LineSpMode;
			int ExtraV;
			double LocalScX;
			double LocalScY;
			double LocalX;
			double LocalY;
			double LocalRot;
			bool PicArt;
			bool flippedH;
			bool flippedV;
			bool isPrintable;
			bool isBookmark;
			bool m_isAnnotation;
			Annotation m_annotation;
			QString AnName;
			double Extra;
			double TExtra;
			double BExtra;
			double RExtra;
			FirstLineOffsetPolicy firstLineOffsetP;
			QString Pfile;
			QString Pfile2;
			QString Pfile3;
			QString IProfile;
			QString EmProfile;
			eRenderIntent IRender;
			bool UseEmbedded;
			bool OverrideCompressionMethod;
			int CompressionMethodIndex;
			bool OverrideCompressionQuality;
			int CompressionQualityIndex;
			QString itemText;
			QPolygon Clip;
			FPointArray PoLine;
			FPointArray ContourLine;
			bool PoShow;
			double BaseOffs;
			int textPathType;
			bool textPathFlipped;
			int TextflowMode;
			int textAlignment;
			QString IFont;
			int ISize;
			QStack<int> Groups;
			double groupHeight;
			double groupWidth;
			int  LayerID;
			bool ScaleType;
			bool AspectRatio;
			bool Locked;
			bool LockRes;
			double Transparency;
			double TranspStroke;
			int TransBlend;
			int TransBlendS;
			bool Reverse;
			QString NamedLStyle;
			QString Language;
			QString guiLanguage;
			int Cols;
			double ColGap;
			QList<ParagraphStyle::TabRecord> TabValues;
			QVector<double> DashValues;
			double DashOffset;
			bool isTableItem;
			bool TopLine;
			bool LeftLine;
			bool RightLine;
			bool BottomLine;
			int LeftLinkID;
			int RightLinkID;
			int TopLinkID;
			int BottomLinkID;
			int startArrowIndex;
			int endArrowIndex;
			int startArrowScale;
			int endArrowScale;
			ObjAttrVector pageItemAttributes;
		};
		void PasteItem(struct CopyPasteBuffer *Buffer, bool drag = false, bool noResize = true);
		void GetItemProps(QDomElement *obj, struct CopyPasteBuffer *OB, const QString& baseDir, bool newVersion);
		
};

extern "C" PLUGIN_API int scribus12format_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribus12format_getPlugin();
extern "C" PLUGIN_API void scribus12format_freePlugin(ScPlugin* plugin);

#endif
