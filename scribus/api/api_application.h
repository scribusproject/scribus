/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.

Scribus is copyright The Scribus Team per the COPYING file.
*/

#include <QByteArray>
#include <QString>

#include "scribusapi.h"

namespace ScribusAPI {

	//
	// Returns the build information of the Scribus application
	//
	QString SCRIBUS_API getBuildInformation();

	//
	// Returns the version of the Scribus application as a string
	//
	QString SCRIBUS_API getVersion();

	//
	// Returns the major version of the Scribus application as an integer
	//
	int SCRIBUS_API getVersionMajor();

	//
	// Returns the minor version of the Scribus application as an integer
	//
	int SCRIBUS_API getVersionMinor();

	//
	// Returns the patch version of the Scribus application as an integer
	//
	int SCRIBUS_API getVersionPatch();

	//
	// Returns the suffix of version of the Scribus application as a string: blank or svn
	//
	QString SCRIBUS_API getVersionSuffix();

	//
	// Returns the word Scribus followed by the current version eg Scribus 1.5.7
	//
	QString SCRIBUS_API getVersionScribus();

	//
	// Returns the word Scribus followed by the current version eg Scribus 1.5.7
	//
	QByteArray SCRIBUS_API getVersionScribusAsByteArray();

	//
	// Returns the word Scribus (translated) followed by the current version eg Scribus 1.5.7
	//
	QString SCRIBUS_API getVersionScribusTranslated();

	//
	// Returns the word Scribus (translated) followed by the current version eg Scribus 1.5.7
	//
	QByteArray SCRIBUS_API getVersionScribusTranslatedAsByteArray();

	//
	// Is this a development version, ie unreleased code from our version control system
	//
	bool SCRIBUS_API isSVN();

	//
	// Returns if we have created a string with the version control system revision
	//
	bool SCRIBUS_API haveSVNRevision();

	//
	// Returns the current version control system revision
	//
	QString SCRIBUS_API getSVNRevision();
}
