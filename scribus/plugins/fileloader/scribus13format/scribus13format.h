/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS13FORMAT_H
#define SCRIBUS13FORMAT_H

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

class PLUGIN_API Scribus13Format : public LoadSavePlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		Scribus13Format();
		virtual ~Scribus13Format();

		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		//Not the same as readSLA. This one only reads max 4k of the file for speed.
		bool fileSupported(QIODevice* file, const QString& fileName = QString()) const override;

		bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0) override;
		bool saveFile(const QString & fileName, const FileFormat & fmt) override { return false; };
		void addToMainWindowMenu(ScribusMainWindow *) override {};

		// Special features - .sla page extraction support
		bool loadPage(const QString& fileName, int pageNumber, bool Mpage, const QString& renamedPageName = QString()) override;
		bool readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles) override;
		bool readLineStyles(const QString& fileName, QHash<QString, multiLine> *Sty) override;
		bool readColors(const QString& fileName, ColorList & colors) override;
		bool readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames) override;
		void getReplacedFontData(bool & getNewReplacement, QMap<QString, QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces) override;

	private:
		void registerFormats();
		//Scribus Doc vars, not plugin vars
		void GetItemText(QDomElement* it, ScribusDoc* doc, PageItem* obj, LastStyles* last, bool impo = false, bool VorLFound = false);
		void readParagraphStyle(ParagraphStyle& vg, const QDomElement& pg, ScribusDoc *doc);
		PageItem* PasteItem(QDomElement *obj, ScribusDoc *doc, const QString& baseDir, PageItem::ItemKind itemKind, int pagenr = -2 /* currentPage*/);
		void GetStyle(QDomElement *pg, ParagraphStyle *vg, StyleSet<ParagraphStyle> *tempParagraphStyles, ScribusDoc* doc, bool fl);
		QString readSLA(const QString & fileName);
		
		QMap<int, int> itemRemap;
		QMap<int, int> itemNext;
		QList<PageItem*> FrameItems;
		int itemCount {0};
		bool newReplacement {false};
		QMap<QString, QString> ReplacedFonts;
		QMap<uint, QString> DoVorl;
		uint VorlC {0};
		QList<PDFPresentationData> EffVal;
};

extern "C" PLUGIN_API int scribus13format_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* scribus13format_getPlugin();
extern "C" PLUGIN_API void scribus13format_freePlugin(ScPlugin* plugin);

#endif
