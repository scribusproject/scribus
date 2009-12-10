/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS_LOADSAVEPLUGIN_H
#define SCRIBUS_LOADSAVEPLUGIN_H

#include "scplugin.h"

#include <QString>
#include <QRegExp>
#include <QIODevice>
#include <QProgressBar>
#include <QStringList>
#include <QList>

class FileFormat;
//TODO REmove includes one day
//class ScribusDoc;
//class ScribusView;
#include "scfonts.h"
#include "scribusdoc.h"
#include "scribusview.h"

/*!
 * @brief Superclass for all file import/export/load/save plugins
 *
 * This class provides the interface common to all file load/save/import/export
 * plugins. It provides the facilities for discovering what format(s) a plugin
 * supports, how they should be identified, etc.
 */
class SCRIBUS_API LoadSavePlugin : public ScPlugin
{
	Q_OBJECT

	public:
		// Construct a plugin instance.
		LoadSavePlugin();
		~LoadSavePlugin();

		enum loadFlags
		{
			lfCreateDoc = 1,
			lfUseCurrentPage = 2,
			lfInsertPage = 4,
			lfInteractive = 8,
			lfScripted = 16
		};

		// Static functions:

		// Return a list of format descriptions suitable for use with
		// QFileDialog.  You can convert it to QString form with
		// fileDialogSaveFilter().join(";;")
		static const QStringList fileDialogLoadFilter();

		// Same deal but for save
		static const QStringList fileDialogSaveFilter();

		// Get the highest priority format of a given id, or 0 if
		// not found / not available.
		static const FileFormat * getFormatById(const int id);

		// Non-static members implemented by plugins:
		//
		// Load the requested format from the specified path.
		// Default implementation always reports failure.
		virtual bool loadFile(const QString & fileName, const FileFormat & fmt, int flags, int index = 0);

		// Save the requested format to the requested path.
		virtual bool saveFile(const QString & fileName, const FileFormat & fmt);

		// Return last file saved, this may be the last fileName argument passed to saveFile(),
		// a temporary file name or an empty string if last call to saveFile() could not create
		// a valid file
		virtual const QString& lastSavedFile(void);

		// Examine the passed file and test to see whether it appears to be
		// loadable with this plugin. This test must be quick and simple.
		// It need not verify a file, just confirm that it looks like a supported
		// file type (eg "XML doc with root element SCRIBUSXML and version 1.3.1").
		// All plugins must implement this method.
		virtual bool fileSupported(QIODevice* file, const QString & fileName=QString::null) const = 0;
		
		// Return a list of all formats supported by all currently loaded and
		// active plugins. This list is sorted in a very specific order:
		// First, by descending order of `id', then descending order of priority.
		static const QList<FileFormat> & supportedFormats();
		
		virtual void setupTargets(ScribusDoc *targetDoc, ScribusView* targetView, ScribusMainWindow* targetMW, QProgressBar* targetMWPRogressBar, SCFonts* targetAvailableFonts);
		virtual void getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces);
		virtual bool loadPage(const QString & fileName, int pageNumber, bool Mpage, QString renamedPageName=QString::null);
		virtual bool readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles);
		virtual bool readCharStyles(const QString& fileName, ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles);
		virtual bool readLineStyles(const QString& fileName, QMap<QString,multiLine> *Sty);
		virtual bool readColors(const QString& fileName, ColorList & colors);
		virtual bool readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames);
		
	protected:

		/// Check a loadFlags combination
		virtual bool checkFlags(int flags);

		/// Register the passed format so it can be used by the app
		void registerFormat(const FileFormat & fmt);

		/// Unregister the format with format ID `id' that references by the calling plugin.
		void unregisterFormat(unsigned int id);

		/// Unregister all formats owned by the calling plugin
		void unregisterAll();

		// Set standard message for file read errors
		virtual void setFileReadError();

		// Set standard message for dom style errors with line and column
		virtual void setDomParsingError(const QString& msg, int line, int column);
		
		ScribusDoc*        m_Doc;
		ScribusView*       m_View; //For 1.2.x loader at the moment
		ScribusMainWindow* m_ScMW; //For plugins when required
		QProgressBar*      m_mwProgressBar;
		SCFonts*           m_AvailableFonts;
		QString            m_lastSavedFile;

	private:
		// A list of all supported formats. This is maintained by plugins
		// using the protected `registerFormat(...)', `unregisterFormat(...)'
		// and `unregisterAll(...)' methods. This is sorted in a very specific
		// order - ascending ID, then descending priority.
		static QList<FileFormat> formats;

		// Return an iterator referencing the first format structure named `name'.
		// If specified, only return formats implmented by `plug'.
		// If `start' is specified, start searching at this iterator rather than the
		// start of the list.
		// The end iterator is returned if no match was found.
		// Note that due to the sort order maintained in `formats', the first
		// iterator returned by this method will always be to the highest
		// priority format of the required ID, and each subsequent call will
		// return the next lowest priority format.
		static QList<FileFormat>::iterator findFormat(unsigned int id,
				LoadSavePlugin* plug = 0,
				QList<FileFormat>::iterator it = formats.begin());
				
		static QList<FileFormat>::iterator findFormat(const QString& extension,
				LoadSavePlugin* plug = 0,
				QList<FileFormat>::iterator it = formats.begin());

		// Print out a format list for debugging purposes
		static void printFormatList();

		// Base implementation for fileDialogLoadFilter and fileDialogSaveFilter
		static const QStringList getDialogFilter(bool forLoad);
};



// Info on each supported format. A plugin must register a
// FileFormat structure for every format it knows how to load or
// save. If it both loads and saves a given format, one structure must
// be registered for load and one for save.
// Plugins must unregister formats when being unloaded to ensure that
// no attempt is made to load a file using a plugin that's no longer
// available.
//
// This class also provides methods to ask the implementation to load / save a
// file in this format.
class SCRIBUS_API FileFormat
{
	public:
		// Default ctor to make QValueList happy
		FileFormat() : load(false), save(false), plug(0) {}
		// Standard ctor that sets up a valid FileFormat
		FileFormat(LoadSavePlugin * plug) : load(false), save(false), plug(plug) {}
		// Load a file with this format
		bool loadFile(const QString & fileName, int flags, int index = 0) const;
		// Save a file with this format
		bool saveFile(const QString & fileName) const;
		// Get last saved file
		QString lastSavedFile(void) const;
		
		void setupTargets(ScribusDoc *targetDoc, ScribusView* targetView, ScribusMainWindow* targetMW, QProgressBar* targetMWPRogressBar, SCFonts* targetAvailableFonts) const;
		void getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces) const;
		bool loadPage(const QString & fileName, int pageNumber, bool Mpage, QString renamedPageName=QString::null) const;
		bool readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles) const;
		bool readCharStyles(const QString& fileName, ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles) const;
		bool readLineStyles(const QString& fileName, QMap<QString,multiLine> *Sty) const;
		bool readColors(const QString& fileName, ColorList & colors) const;
		bool readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames) const;
		//
		// Data members
		//
		// An integer ID code used to idenfify formats. Should be unique
		// across all FileFormat structures except where they implement
		// support for the same file format, eg sla 1.2.x, sla 1.3.x and
		// "new format" SLA should all have equal IDs (with different
		// priorities to control what order they're tried in when a user
		// tries to open a file).
		// Note that dialog box options are sorted in descending `id' order.
		uint formatId;
		// The human-readable, translated name of this file format.
		QString trName;
		// A filter in the format used by QFileDialog that should be used to
		// select for this format.
		QString filter;
		// Regexp to match filenames for this format
		QRegExp nameMatch;
		// MIME type(s) that should be matched by this format.
		QStringList mimeTypes;
		// Can we load it?
		bool load;
		// Can we save it?
		bool save;
		// Priority of this format from 0 (lowest, tried last) to
		// 255 (highest, tried first). 64-128 recommended in general.
		// Priority controls the order options are displayed in when a file
		// of a given type is selected in a dialog, and controls the order
		// loaders are tried in when multiple plugins support the same file
		// type.
		unsigned short int priority;
		// For convenience, a pointer back to the plugin to use to open
		// this format.
		LoadSavePlugin * plug;
//		LoadSavePlugin * const plug;
};



#endif
