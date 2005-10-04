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

		// What modes this plugin supports. Often ORed.
		// The distinction between "import" and "load", and between
		// "export" and "save" is really mostly a user interface issue.
		enum FormatSupportType
		{
			Format_Load = 1,
			Format_Save = 2,
			Format_Import = 4,
			Format_Export = 8
		};

		// Info on each supported format. A list of these structures
		// is returned by supportedFormats()
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
			// One or more ORed values from FileTypePlugin::FormatSupportType
			// to indicate how the plugin works. Import? Export? Load? Save?
			int modes;
			// MIME type(s) that should be matched by this format.
			QStringList mimeTypes;
			// Priority of this format from 0 (lowest, tried last) to
			// 255 (highest, tried first). 64-128 recommended in general.
			// Priority controls the order options are displayed in when a given
			// file type is selected, and controls the order loaders are tried in
			// when multiple plugins support the same file type.
			unsigned short int priority;
		};

		// Return a list of all formats this plugin supports. All plugins must
		// implement this method.
		virtual QValueList<FormatSupport> supportedFormats() const = 0;

		// Examine the passed file and test to see whether it appears to be
		// loadable with this plugin. This test must be quick and simple.
		// It need not verify a file, just confirm that it looks like a supported
		// file type (eg "XML doc with root element SCRIBUSXML and version 1.3.1").
		// All plugins must implement this method.
		virtual bool fileSupported(QIODevice* file) const = 0;

		// Actual import is accomplished through the inherited
		// run() method, or its async version. Further refinement can be
		// accomplished later.
};

#endif
