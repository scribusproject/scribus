/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS134FORMAT_H
#define SCRIBUS134FORMAT_H

#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "scfonts.h"
#include "scribusstructs.h"
#include "styles/styleset.h"
#include "selection.h"

#include <QMap>
#include <QString>
#include <QList>
#include <QProgressBar>

class  ColorList;
class  multiLine;
class  ScLayer;
class  ScribusDoc;
//struct ScribusDoc::BookMa;
class  ScXmlStreamAttributes;
class  ScXmlStreamReader;
class  ScXmlStreamWriter;

class PLUGIN_API Scribus150Format : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		Scribus150Format();
		virtual ~Scribus150Format();
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		//Not the same as readSLA. This one only reads max 4k of the file for speed.
		virtual bool fileSupported(QIODevice* file, const QString & fileName=QString::null) const;

		virtual bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0);
		virtual bool saveFile(const QString & fileName, const FileFormat & fmt);
		virtual bool savePalette(const QString & fileName);
		virtual QString saveElements(double xp, double yp, double wp, double hp, Selection* selection, QByteArray &prevData);
		virtual bool loadPalette(const QString & fileName);
		virtual bool loadElements(const QString & data, QString fileDir, int toLayer, double Xp_in, double Yp_in, bool loc);
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};

		// Special features - .sla page extraction support
		virtual bool loadPage(const QString & fileName, int pageNumber, bool Mpage, QString renamedPageName=QString::null);
		virtual bool readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles);
		virtual bool readCharStyles(const QString& fileName, ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles);
		virtual bool readLineStyles(const QString& fileName, QMap<QString,multiLine> *Sty);
		virtual bool readColors(const QString& fileName, ColorList & colors);
		virtual bool readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames);
		virtual void getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces);

	private:

		enum ItemSelection {
			ItemSelectionMaster = 0,
			ItemSelectionPage   = 1,
			ItemSelectionFrame  = 2,
			ItemSelectionPattern= 3,
			ItemSelectionGroup  = 4,
			ItemSelectionElements = 5,
		};

		class ItemInfo
		{
		public:
			ItemInfo(void) { groupLastItem = nextItem = ownLink = 0; item = NULL; isGroupFlag = false; };
			PageItem* item;
			int groupLastItem;
			int nextItem;
			int ownLink;
			bool isGroupFlag;
		};

		void registerFormats();
		
		QString readSLA(const QString & fileName);

		void getStyle(ParagraphStyle& style, ScXmlStreamReader& reader, StyleSet<ParagraphStyle> *docParagraphStyles, ScribusDoc* doc, bool fl);

		void readDocAttributes(ScribusDoc* doc, ScXmlStreamAttributes& attrs);
		void readCMSSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs);
		void readDocumentInfo(ScribusDoc* doc, ScXmlStreamAttributes& attrs);
		void readGuideSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs);
		void readToolSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs);
		void readTypographicSettings(ScribusDoc* doc, ScXmlStreamAttributes& attrs);

		bool readArrows(ScribusDoc* doc, ScXmlStreamAttributes& attrs);
		bool readBookMark(ScribusDoc::BookMa& bookmark, int& elem, ScXmlStreamAttributes& attrs); 
		bool readCheckProfile(ScribusDoc* doc, ScXmlStreamAttributes& attrs);
		bool readColor(ColorList& colors, ScXmlStreamAttributes& attrs);
		bool readGradient(ScribusDoc *doc, VGradient &gra, ScXmlStreamReader& reader);
		void readCharacterStyleAttrs(ScribusDoc *doc, ScXmlStreamAttributes& attrs, CharStyle & newStyle);
		void readNamedCharacterStyleAttrs(ScribusDoc *doc, ScXmlStreamAttributes& attrs, CharStyle & newStyle);
		bool readDocItemAttributes(ScribusDoc *doc, ScXmlStreamReader& reader);
		bool readHyphen(ScribusDoc *doc, ScXmlStreamReader& reader);
		bool readItemText(PageItem* item, ScXmlStreamAttributes& attrs, LastStyles* last);
		bool readLatexInfo(PageItem_LatexFrame* item, ScXmlStreamReader& reader);
		void readLayers(ScLayer& layer, ScXmlStreamAttributes& attrs);
		bool readMultiline(multiLine& ml, ScXmlStreamReader& reader);
		bool readObject(ScribusDoc* doc, ScXmlStreamReader& reader, ItemInfo& info, const QString& baseDir, bool loadPage);
		bool readPage(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readPageItemAttributes(PageItem* item, ScXmlStreamReader& reader);
		bool readPageSets(ScribusDoc* doc, ScXmlStreamReader& reader);
		void readParagraphStyle(ScribusDoc *doc, ScXmlStreamReader& reader, ParagraphStyle& newStyle);
		bool readPattern(ScribusDoc* doc, ScXmlStreamReader& reader, const QString& baseDir);
		bool readPDFOptions(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readPrinterOptions(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readSections(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readTableOfContents(ScribusDoc* doc, ScXmlStreamReader& reader);

		PageItem* pasteItem(ScribusDoc *doc, ScXmlStreamAttributes& attrs, const QString& baseDir, int pagenr = -2 /* currentPage*/);

		void writeCheckerProfiles(ScXmlStreamWriter& docu);
		void writeLinestyles(ScXmlStreamWriter& docu);
		void writeJavascripts(ScXmlStreamWriter& docu);
		void writeBookmarks(ScXmlStreamWriter& docu);
		void writeColors(ScXmlStreamWriter& docu, bool part = false);
		void writeGradients(ScXmlStreamWriter & docu, bool part = false);
		void writeHyphenatorLists(ScXmlStreamWriter& docu);
		void writePStyles(ScXmlStreamWriter& docu);
		void writeCStyles(ScXmlStreamWriter& docu);
		void putPStyle(ScXmlStreamWriter& docu, const ParagraphStyle & style, const QString &nodeName);
		void putCStylePT(ScXmlStreamWriter& docu, const CharStyle & style);
		void putCStyle(ScXmlStreamWriter& docu, const CharStyle & style);
		void putNamedCStyle(ScXmlStreamWriter& docu, const CharStyle & style);
		void writeITEXTs(ScribusDoc *doc, ScXmlStreamWriter&, PageItem* item);
		void writeLayers(ScXmlStreamWriter& docu);
		void writePrintOptions(ScXmlStreamWriter& docu);
		void writePdfOptions(ScXmlStreamWriter& docu);
		void writeDocItemAttributes(ScXmlStreamWriter& docu);
		void writeTOC(ScXmlStreamWriter& docu);
		void writePageSets(ScXmlStreamWriter& docu);
		void writeSections(ScXmlStreamWriter& docu);
		void writePatterns(ScXmlStreamWriter& docu, const QString& baseDir, bool part = false, Selection* selection = 0);
		void writeContent(ScXmlStreamWriter& docu, const QString& baseDir);

		void WritePages(ScribusDoc *doc, ScXmlStreamWriter& docu, QProgressBar *dia2, uint maxC, bool master);
		void WriteObjects(ScribusDoc *doc, ScXmlStreamWriter& docu, const QString& baseDir, QProgressBar *dia2, uint maxC, ItemSelection master, QList<PageItem*> *items = 0);
		void SetItemProps(ScXmlStreamWriter& docu, PageItem* item, const QString& baseDir, bool newFormat);

		QMap<int, int> itemRemap;
		QMap<int, int> itemNext;
		QMap<int, int> itemRemapF;
		QMap<int, int> itemNextF;
		QMap<int, int> itemRemapM;
		QMap<int, int> itemNextM;

		int itemCount;
		int itemCountM;
		bool layerFound;
		int LayerToPaste;
		double Xp;
		double GrX;
		double Yp;
		double GrY;
		QString clipPath;
};

extern "C" PLUGIN_API int scribus150format_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribus150format_getPlugin();
extern "C" PLUGIN_API void scribus150format_freePlugin(ScPlugin* plugin);

#endif
