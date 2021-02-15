/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.

Scribus is copyright The Scribus Team per the COPYING file.
*/

#include <QByteArray>
#include <QString>

namespace ScribusAPI {

	//
	// Returns the build information of the Scribus application
	//
	QString getBuildInformation();

	//
	// Returns the version of the Scribus application as a string
	//
	QString getVersion();

	//
	// Returns the major version of the Scribus application as an integer
	//
	int getVersionMajor();

	//
	// Returns the minor version of the Scribus application as an integer
	//
	int getVersionMinor();

	//
	// Returns the patch version of the Scribus application as an integer
	//
	int getVersionPatch();

	//
	// Returns the suffix of version of the Scribus application as a string: blank or svn
	//
	QString getVersionSuffix();

	//
	// Returns the word Scribus followed by the current version eg Scribus 1.5.7
	//
	QString getVersionScribus();

	//
	// Returns the word Scribus followed by the current version eg Scribus 1.5.7
	//
	QByteArray getVersionScribusAsByteArray();

	//
	// Returns the word Scribus (translated) followed by the current version eg Scribus 1.5.7
	//
	QString getVersionScribusTranslated();

	//
	// Returns the word Scribus (translated) followed by the current version eg Scribus 1.5.7
	//
	QByteArray getVersionScribusTranslatedAsByteArray();

	//
	// Is this a development version, ie unreleased code from our version control system
	//
	bool isSVN();

	//
	// Returns if we have created a string with the version control system revision
	//
	bool haveSVNRevision();

	//
	// Returns the current version control system revision
	//
	QString getSVNRevision();
}
