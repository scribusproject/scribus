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
		void writeCheckerProfiles(QDomDocument & docu);
		void writeLinestyles(QDomDocument & docu);
		void writeJavascripts(QDomDocument & docu);
		void writeBookmarks(QDomDocument & docu);
		void writeColors(QDomDocument & docu);
		void writeHyphenatorLists(QDomDocument &docu);
		void writePStyles(QDomDocument & docu);
		void writeCStyles(QDomDocument & docu);
		void putPStyle(QDomDocument & docu, QDomElement & fo, const ParagraphStyle & style);
		void putCStylePT(QDomDocument & docu, QDomElement & fo, const CharStyle & style);
		void putCStyle(QDomDocument & docu, QDomElement & fo, const CharStyle & style);
		void writeITEXTs(ScribusDoc *doc, QDomDocument *docu, QDomElement ob, PageItem* item);
		void writeLayers(QDomDocument & docu);
		void writePrintOptions(QDomDocument & docu);
		void writePdfOptions(QDomDocument & docu);
		void writeDocItemAttributes(QDomDocument & docu);
		void writeTOC(QDomDocument & docu);
		void writePageSets(QDomDocument & docu);
		void writeSections(QDomDocument & docu);
		void writePatterns(QDomDocument & docu, const QString& baseDir);
		void writeContent(QDomDocument & docu, const QString& baseDir);
		void WritePages(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, QProgressBar *dia2, uint maxC, bool master);
		void WriteObjects(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, const QString& baseDir, QProgressBar *dia2, uint maxC, ItemSelection master, QList<PageItem*> *items = 0);
		void SetItemProps(QDomElement *ob, PageItem* item, const QString& baseDir, bool newFormat);
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
