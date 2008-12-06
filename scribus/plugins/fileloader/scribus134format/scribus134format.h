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

#include <QDomElement>
#include <QMap>
#include <QString>
#include <QList>
#include <QProgressBar>

class ScXmlStreamWriter;

class PLUGIN_API Scribus134Format : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		Scribus134Format();
		virtual ~Scribus134Format();
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
		};
		void registerFormats();
		//Scribus Doc vars, not plugin vars
		void GetItemText(QDomElement *it, ScribusDoc *doc, PageItem* obj, LastStyles* last, bool impo=false, bool VorLFound=false);
		void GetCStyle(const QDomElement *it, ScribusDoc *doc, CharStyle & newStyle);
		void readParagraphStyle(ParagraphStyle& vg, const QDomElement& pg, SCFonts &avail, ScribusDoc *doc);
		PageItem* PasteItem(QDomElement *obj, ScribusDoc *doc, const QString& baseDir);
		void GetStyle(QDomElement *pg, ParagraphStyle *vg, StyleSet<ParagraphStyle> *docParagraphStyles, ScribusDoc* doc, bool fl);
		QString readSLA(const QString & fileName);

		void writeCheckerProfiles(ScXmlStreamWriter& docu);
		void writeLinestyles(ScXmlStreamWriter& docu);
		void writeJavascripts(ScXmlStreamWriter& docu);
		void writeBookmarks(ScXmlStreamWriter& docu);
		void writeColors(ScXmlStreamWriter& docu);
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
		void writePatterns(ScXmlStreamWriter& docu, const QString& baseDir);
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
		int itemCountF;
		QMap<uint,QString> DoVorl;
		uint VorlC;
};

extern "C" PLUGIN_API int scribus134format_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribus134format_getPlugin();
extern "C" PLUGIN_API void scribus134format_freePlugin(ScPlugin* plugin);

#endif
