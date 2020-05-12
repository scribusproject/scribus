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

#include <QHash>
#include <QList>
#include <QMap>
#include <QProgressBar>
#include <QString>

class QIODevice;

class  ColorList;
class  multiLine;
class  ScLayer;
class  ScribusDoc;
//struct ScribusDoc::BookMa;
class  ScXmlStreamAttributes;
class  ScXmlStreamReader;
class  ScXmlStreamWriter;

class PLUGIN_API Scribus134Format : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		Scribus134Format();
		virtual ~Scribus134Format();
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		//Not the same as readSLA. This one only reads max 4k of the file for speed.
		bool fileSupported(QIODevice* file, const QString & fileName=QString()) const override;

		bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0) override;
		bool saveFile(const QString & fileName, const FileFormat & fmt) override { return false; };
		void addToMainWindowMenu(ScribusMainWindow *) override {};

		// Special features - .sla page extraction support
		bool loadPage(const QString & fileName, int pageNumber, bool Mpage, const QString& renamedPageName=QString()) override;
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
		};

		class ItemInfo
		{
		public:
			ItemInfo(void) { groupLastItem = nextItem = ownLink = ownNr = 0; item = nullptr; isGroupFlag = false; };
			PageItem* item;
			int groupLastItem;
			int nextItem;
			int ownLink;
			int ownNr;
			bool isGroupFlag;
		};

		void registerFormats();
		
		QIODevice* slaReader(const QString & fileName);

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
		void readCharacterStyleAttrs(ScribusDoc *doc, ScXmlStreamAttributes& attrs, CharStyle & newStyle);
		void readNamedCharacterStyleAttrs(ScribusDoc *doc, ScXmlStreamAttributes& attrs, CharStyle & newStyle);
		bool readDocItemAttributes(ScribusDoc *doc, ScXmlStreamReader& reader);
		bool readHyphen(ScribusDoc *doc, ScXmlStreamReader& reader);
		bool readItemText(PageItem* item, ScXmlStreamAttributes& attrs, LastStyles* last);
		bool readLatexInfo(PageItem_LatexFrame* item, ScXmlStreamReader& reader);
		void readLayers(ScLayer& layer, ScXmlStreamAttributes& attrs);
		bool readMultiline(multiLine& ml, ScXmlStreamReader& reader);
		bool readObject(ScribusDoc* doc, ScXmlStreamReader& reader, ItemInfo& info, const QString& baseDir, bool loadPage, const QString& renamedPageName = QString());
		bool readPage(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readPageItemAttributes(PageItem* item, ScXmlStreamReader& reader);
		bool readPageSets(ScribusDoc* doc, ScXmlStreamReader& reader);
		void readParagraphStyle(ScribusDoc *doc, ScXmlStreamReader& reader, ParagraphStyle& newStyle);
		bool readPattern(ScribusDoc* doc, ScXmlStreamReader& reader, const QString& baseDir);
		bool readPDFOptions(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readPrinterOptions(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readSections(ScribusDoc* doc, ScXmlStreamReader& reader);
		bool readTableOfContents(ScribusDoc* doc, ScXmlStreamReader& reader);

		PageItem* pasteItem(ScribusDoc *doc, ScXmlStreamAttributes& attrs, const QString& baseDir, PageItem::ItemKind itemKind, int pagenr = -2 /* currentPage*/);

		QMap<QString, QString> parStyleMap;
		QMap<uint, QString> legacyStyleMap;
		uint legacyStyleCount;

		QMap<int, int> groupRemap;
		QMap<int, int> itemRemap;
		QMap<int, int> itemNext;
		QMap<int, int> itemRemapM;
		QMap<int, int> itemNextM;
		QList<PageItem*> FrameItems;
		QList<PDFPresentationData> EffVal;

		int itemCount;
		int itemCountM;
		QFile aFile;
};

extern "C" PLUGIN_API int scribus134format_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribus134format_getPlugin();
extern "C" PLUGIN_API void scribus134format_freePlugin(ScPlugin* plugin);

#endif
