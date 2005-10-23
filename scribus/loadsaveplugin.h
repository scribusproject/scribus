#ifndef SCRIBUS_LOADSAVEPLUGIN_H
#define SCRIBUS_LOADSAVEPLUGIN_H

#include "scplugin.h"

#include <qstring.h>
#include <qregexp.h>
#include <qiodevice.h>
#include <qvaluelist.h>
#include <qstringlist.h>

/*
 * @brief Superclass for all file import/export/load/save plugins
 *
 * This class provides the interface common to all file load/save/import/export
 * plugins. It provides the facilities for discovering what format(s) a plugin
 * supports, how they should be identified, etc.
 */
class SCRIBUS_API LoadSavePlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Construct a plugin instance.
		LoadSavePlugin();
		~LoadSavePlugin();

		// Info on each supported format. A plugin must register a
		// FormatSupport structure for every format it knows how to load or
		// save. If it both loads and saves a given format, one structure must
		// be registered for load and one for save.
		// Plugins must unregister formats when being unloaded to ensure that
		// no attempt is made to load a file using a plugin that's no longer
		// available.
		struct FormatSupport
		{
			// The human-readable, translated name of this file format.
			QString trName;
			// A short internal name for the format. This is used for deciding
			// whether two plugins implement the same format, in which case only
			// one is shown in some UI and they are tried in order of priority.
			QCString internalName;
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
			// Can we build it? Er... forget that, sorry ;-) ^U
			// Priority of this format from 0 (lowest, tried last) to
			// 255 (highest, tried first). 64-128 recommended in general.
			// Priority controls the order options are displayed in when a given
			// file type is selected, and controls the order loaders are tried in
			// when multiple plugins support the same file type.
			unsigned short int priority;
			// For convenience, a pointer back to the plugin to use to open
			// this format.
			LoadSavePlugin* plug;
		};

		// Static functions:

		// Return a list of all formats supported by all currently loaded and
		// active plugins.
		static const QValueList<FormatSupport> & supportedFormats();


		// Non-static members implemented by plugins:

		// Examine the passed file and test to see whether it appears to be
		// loadable with this plugin. This test must be quick and simple.
		// It need not verify a file, just confirm that it looks like a supported
		// file type (eg "XML doc with root element SCRIBUSXML and version 1.3.1").
		// All plugins must implement this method.
		virtual bool fileSupported(QIODevice* file) const = 0;

		// Load the requested format from the specified path.
		// Default implementation always reports falure.
		virtual bool loadFile(const QString & fileName, const FormatSupport & fmt);

		// Save the requested format to the requested path.
		virtual bool saveFile(const QString & fileName, const FormatSupport & fmt);
	
	protected:
		/// Register the passed format so it can be used by 
		void registerFormat(const FormatSupport & fmt);

		/// Unregister the format with internal name `name' that references by the calling plugin.
		void unregisterFormat(const QCString & name);

		/// Unregister all formats owned by the calling plugin
		void unregisterAll();

	private:
		// A list of all supported formats. This is maintained by plugins
		// using the protected `registerFormat(...)', `unregisterFormat(...)'
		// and `unregisterAll(...)' methods.
		static QValueList<FormatSupport> formats;

		// Return an iterator referencing the first format structure named `name'.
		// If specified, only return formats implmented by `plug'.
		// If `start' is specified, start searching at this iterator rather than the
		// start of the list.
		// The end iterator is returned if no match was found.
		QValueList<FormatSupport>::iterator findFormat(const QCString & name,
				LoadSavePlugin* plug = 0,
				QValueList<FormatSupport>::iterator it = formats.begin());
};

#endif
