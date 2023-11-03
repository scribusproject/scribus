/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS150FORMAT_H
#define SCRIBUS150FORMAT_H

#include "pluginapi.h"
#include "loadsaveplugin.h"
#include "notesstyles.h"
#include "scfonts.h"
#include "scribusstructs.h"
#include "styles/styleset.h"
#include "selection.h"

#include <QList>
#include <QMap>
#include <QProgressBar>
#include <QString>

class QIODevice;

class  ColorList;
class  multiLine;
class  PageItem_NoteFrame;
class  ScLayer;
class  ScribusDoc;
//struct ScribusDoc::BookMa;
class  ScXmlStreamAttributes;
class  ScXmlStreamReader;
class  ScXmlStreamWriter;
class  StoryText;
class  TextNote;

class PLUGIN_API Scribus150Format : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		Scribus150Format();
		virtual ~Scribus150Format();

		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;

		//Not the same as readSLA. This one only reads max 4k of the file for speed.
		bool fileSupported(QIODevice* file, const QString& fileName = QString()) const override;
		bool paletteSupported(QIODevice* file, const QString& fileName = QString()) const override;
		bool storySupported(const QByteArray& storyData) const override;

		bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0) override;
		bool saveFile(const QString & fileName, const FileFormat & fmt) override;
		
		bool loadPalette(const QString & fileName) override;
		bool savePalette(const QString & fileName) override;

		bool loadElements(const QString & data, const QString& fileDir, int toLayer, double Xp_in, double Yp_in, bool loc) override;
		QString saveElements(double xp, double yp, double wp, double hp, Selection* selection, QByteArray &prevData) override;

		bool loadStory(const QByteArray& data, StoryText& story, PageItem* item) override;
		bool saveStory(StoryText& story, PageItem* item, QByteArray& data) override;

		void addToMainWindowMenu(ScribusMainWindow *) override {};

		// Special features - .sla page extraction support
		bool loadPage(const QString& fileName, int pageNumber, bool Mpage, const QString& renamedPageName = QString()) override;
		bool readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles) override;
		bool readCharStyles(const QString& fileName, ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles) override;
		bool readLineStyles(const QString& fileName, QHash<QString, multiLine> *Sty) override;
		bool readColors(const QString& fileName, ColorList & colors) override;
		bool readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames) override;
		void getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces) override;

	private:

		enum ItemSelection {
			ItemSelectionMaster = 0,
			ItemSelectionPage   = 1,
			ItemSelectionFrame  = 2,
			ItemSelectionPattern= 3,
			ItemSelectionGroup  = 4,
			ItemSelectionElements = 5
		};

		class ItemInfo
		{
		public:
			ItemInfo(void) {}
			PageItem* item {nullptr};
			int groupLastItem {0};
			int nextItem {0};
			int ownLink {0};
			int ownWeld {0};
			int ownNr {0};
			int itemID {0};
			bool isGroupFlag {false};
			bool isWeldFlag {false};
		};

		class ReadObjectParams
		{
		public:
			ReadObjectParams() {}

			PageItem::ItemKind itemKind { PageItem::StandardItem };
			bool    loadingPage { false };
			QString baseDir;
			QString renamedMasterPage;
		};

		void registerFormats();
		
		QIODevice* slaReader(const QString & fileName);
		QIODevice* paletteReader(const QString & fileName);

		void getStyle(ParagraphStyle& style, ScXmlStreamReader& reader, StyleSet<ParagraphStyle> *docParagraphStyles, ScribusDoc* doc, bool equiv);
		void getStyle(CharStyle& style, ScXmlStreamReader& reader, StyleSet<CharStyle> *docCharStyles, ScribusDoc* doc, bool equiv);

		void readDocAttributes(ScribusDoc* doc, const ScXmlStreamAttributes& attrs) const;
		void readCMSSettings(ScribusDoc* doc, const ScXmlStreamAttributes& attrs) const;
		void readDocumentInfo(ScribusDoc* doc, const ScXmlStreamAttributes& attrs) const;
		void readGuideSettings(ScribusDoc* doc, const ScXmlStreamAttributes& attrs) const;
		void readToolSettings(ScribusDoc* doc, const ScXmlStreamAttributes& attrs) const;
		void readTypographicSettings(ScribusDoc* doc, const ScXmlStreamAttributes& attrs) const;

		bool readArrows(ScribusDoc* doc, const ScXmlStreamAttributes& attrs) const;
		bool readBookMark(ScribusDoc::BookMa& bookmark, int& elem, const ScXmlStreamAttributes& attrs) const;
		bool readCheckProfile(ScribusDoc* doc, const ScXmlStreamAttributes& attrs) const;
		void readColor(ColorList& colors, const ScXmlStreamAttributes& attrs) const;
		bool readGradient(ScribusDoc *doc, VGradient &gra, ScXmlStreamReader& reader) const;
		void readCharacterStyleAttrs(ScribusDoc *doc, const ScXmlStreamAttributes& attrs, CharStyle & newStyle) const;
		void readNamedCharacterStyleAttrs(ScribusDoc *doc, const ScXmlStreamAttributes& attrs, CharStyle & newStyle) const;
		bool readDocItemAttributes(ScribusDoc *doc, ScXmlStreamReader& reader) const;
		bool readHyphen(ScribusDoc *doc, ScXmlStreamReader& reader) const;
		bool readStoryText(ScribusDoc *doc, ScXmlStreamReader& reader, StoryText& story, PageItem* item) const;
		bool readItemText(StoryText& story, const ScXmlStreamAttributes& attrs, LastStyles* last) const;
		bool readItemTableData(PageItem_Table* item, ScXmlStreamReader& reader, ScribusDoc *doc) const;
		bool readItemTableCell(PageItem_Table* item, ScXmlStreamReader& reader, ScribusDoc *doc) const;
		bool readLatexInfo(PageItem_LatexFrame* item, ScXmlStreamReader& reader) const;
		void readLayers(ScLayer& layer, const ScXmlStreamAttributes& attrs) const;
		bool readMultiline(multiLine& ml, ScXmlStreamReader& reader) const;
		bool readObject(ScribusDoc* doc, ScXmlStreamReader& reader, const ReadObjectParams& loadParams, ItemInfo& info);
		bool readPage(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readPageItemAttributes(PageItem* item, ScXmlStreamReader& reader) const;
		bool readPageSets(ScribusDoc* doc, ScXmlStreamReader& reader) const;
		void readParagraphStyle(ScribusDoc *doc, ScXmlStreamReader& reader, ParagraphStyle& newStyle) const;
		void readTableStyle(ScribusDoc *doc, ScXmlStreamReader& reader, TableStyle& newStyle) const;
		void readTableBorderLines(ScribusDoc* doc, ScXmlStreamReader& reader, TableBorder& border) const;
		void readCellStyle(ScribusDoc *doc, ScXmlStreamReader& reader, CellStyle& newStyle) const;
		bool readPattern(ScribusDoc* doc, ScXmlStreamReader& reader, const QString& baseDir);
		bool readPDFOptions(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readPrinterOptions(ScribusDoc* doc, ScXmlStreamReader& reader) const;
		bool readSections(ScribusDoc* doc, ScXmlStreamReader& reader) const;
		bool readTableOfContents(ScribusDoc* doc, ScXmlStreamReader& reader) const;
		bool readNotes(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readNotesStyles(ScribusDoc* doc, ScXmlStreamReader& reader) const;
		bool readNotesFrames(ScXmlStreamReader &reader);
		bool readMarks(ScribusDoc* doc, ScXmlStreamReader& reader);

		//lists of items and marks with names only, which need update to pointers
		QMap<Mark*, int> markeredItemsMap;
		QMap<Mark*, QMap<QString, MarkType> > markeredMarksMap;
		QMap<QString, int> nsetRangeItemNamesMap;
		QMap<QString, TextNote*> notesMasterMarks;
		QMap<TextNote*, QString> notesNSets;
		struct NoteFrameData
		{
			QString NSname;
			int myID;
			int itemID;
			int index;
			NumerationRange NSrange;
		};
		QList<NoteFrameData> notesFramesData;
		QList<PDFPresentationData> pdfPresEffects;
		
		void updateNames2Ptr(); //after document load items pointers should be updated in markeredItemList

		PageItem* pasteItem(ScribusDoc *doc, const ScXmlStreamAttributes& attrs, const QString& baseDir, PageItem::ItemKind itemKind, int pageNr = -2 /* currentPage*/);

		void writeCheckerProfiles(ScXmlStreamWriter& docu);
		void writeLineStyles(ScXmlStreamWriter& docu);
		void writeLineStyles(ScXmlStreamWriter& docu, const QStringList& styleNames);
		void writeArrowStyles(ScXmlStreamWriter& docu);
		void writeJavascripts(ScXmlStreamWriter& docu);
		void writeBookmarks(ScXmlStreamWriter& docu);
		void writeColors(ScXmlStreamWriter& docu, bool part = false);
		void writeColors(ScXmlStreamWriter& docu, const QStringList& colorNames);
		void writeGradients(ScXmlStreamWriter & docu, bool part = false);
		void writeGradients(ScXmlStreamWriter & docu, const QStringList& gradientNames);
		void writeHyphenatorLists(ScXmlStreamWriter& docu);
		void writeParagraphStyles(ScXmlStreamWriter& docu);
		void writeCharStyles(ScXmlStreamWriter& docu);
		void writeTableStyles(ScXmlStreamWriter& docu);
		void writeCellStyles(ScXmlStreamWriter& docu);
		void putPStyle(ScXmlStreamWriter& docu, const ParagraphStyle & style, const QString &nodeName);
		void putCStyle(ScXmlStreamWriter& docu, const CharStyle & style);
		void putNamedCStyle(ScXmlStreamWriter& docu, const CharStyle & style);
		void putTableStyle(ScXmlStreamWriter& docu, const TableStyle & style);
		void putCellStyle(ScXmlStreamWriter& docu, const CellStyle & style);
		void writeStoryText(ScribusDoc *doc, ScXmlStreamWriter&, StoryText& story, PageItem* item = nullptr);
		void writeITEXTs(ScribusDoc *doc, ScXmlStreamWriter&, StoryText& story, PageItem* item = nullptr);
		void writeLayers(ScXmlStreamWriter& docu);
		void writePrintOptions(ScXmlStreamWriter& docu);
		void writePdfOptions(ScXmlStreamWriter& docu);
		void writeDocItemAttributes(ScXmlStreamWriter& docu);
		void writeTOC(ScXmlStreamWriter& docu);
		void writeMarks(ScXmlStreamWriter & docu);
		void writeNotesStyles(ScXmlStreamWriter & docu);
		void writeNotesStyles(ScXmlStreamWriter & docu, const QStringList& styleSelection);
		void writeNotesFrames(ScXmlStreamWriter & docu);
		void writeNotesFrames(ScXmlStreamWriter & docu, const QList<PageItem_NoteFrame*>& nfList);
		void writeNotes(ScXmlStreamWriter & docu);
		void writeNotes(ScXmlStreamWriter & docu, const QList<TextNote*>& noteList);
		void writePageSets(ScXmlStreamWriter& docu);
		void writeSections(ScXmlStreamWriter& docu);
		void writePatterns(ScXmlStreamWriter& docu, const QString& baseDir, bool part = false, Selection* selection = 0);
		void writeContent(ScXmlStreamWriter& docu, const QString& baseDir);

		void WritePages(ScribusDoc *doc, ScXmlStreamWriter& docu, QProgressBar *dia2, uint maxC, bool master);
		void WriteObjects(ScribusDoc *doc, ScXmlStreamWriter& docu, const QString& baseDir, QProgressBar *dia2, uint maxC, ItemSelection master, QList<PageItem*> *items = 0);
		void SetItemProps(ScXmlStreamWriter& docu, PageItem* item, const QString& baseDir);
		
		QMap<QString, QString> charStyleMap;
		QMap<QString, QString> parStyleMap;
		
		QMap<int, int> itemRemap;
		QMap<int, int> itemNext;
		QMap<int, int> itemRemapF;
		QMap<int, int> itemNextF;
		QMap<int, int> itemRemapM;
		QMap<int, int> itemNextM;
		QMap<int, PageItem*> LinkID;
		QList<PageItem*> FrameItems;
		QMap<PageItem*, QString> itemsWeld;  //item* and master name

		QFile aFile;
		QString clipPath;
		bool isNewFormat {false};
		bool layerFound {false};
		double GrX {0.0};
		double GrY {0.0};
		double Xp {0.0};
		double Yp {0.0};
		int LayerToPaste {0};
		int itemCount {0};
		int itemCountM {0};
};

extern "C" PLUGIN_API int scribus150format_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribus150format_getPlugin();
extern "C" PLUGIN_API void scribus150format_freePlugin(ScPlugin* plugin);

#endif
