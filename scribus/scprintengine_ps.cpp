/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scpaths.h"
#include "scprintengine_ps.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "scribuscore.h"
#include "prefsmanager.h"
#include "pslib.h"
#include "util_file.h"
#include "util_ghostscript.h"

bool ScPrintEngine_PS::print(ScribusDoc& doc, PrintOptions& options)
{
	bool retw = false;
	QString filename(options.filename);
	PrefsManager& prefsManager = PrefsManager::instance();
	PSLib *dd = new PSLib(&doc, options, PSLib::OutputPS);
	if (dd == nullptr)
		return false;

	if (!options.toFile)
		filename = ScPaths::tempFileDir() + "/tmp.ps";

	// Write the PS to a file
	filename = QDir::toNativeSeparators(filename);

	int psCreationRetVal = dd->createPS(filename);
	if (psCreationRetVal != 0)
	{
		QFile::remove(filename);
		if (psCreationRetVal == 2)
			return true;
		m_errorMessage = dd->errorMessage();
		return false;
	}
	if (options.prnEngine != PostScript3 && ScCore->haveGS())
	{
		// use gs to convert our PS to a lower version
		QString tmp;
		QStringList opts;
		opts.append( QString("-dDEVICEWIDTHPOINTS=%1").arg(tmp.setNum(doc.pageWidth())) );
		opts.append( QString("-dDEVICEHEIGHTPOINTS=%1").arg(tmp.setNum(doc.pageHeight())) );
		convertPS2PS(filename, filename + ".tmp", opts, options.prnEngine);
		moveFile(filename + ".tmp", filename);
	}
	if (!options.toFile)
	{
		// print and delete the PS file
		QByteArray cmd;
		if (options.useAltPrintCommand)
		{
			cmd += options.printerCommand;
			cmd += " ";
			cmd += filename;
			system(cmd.data());
		}
		else
		{
			QByteArray cc;
			cmd += "lpr -P '";
			cmd += options.printer;
			cmd += "'";
			if (options.copies > 1)
				cmd += " -#" + cc.setNum(options.copies);
			cmd += options.printerOptions;
			cmd += " "+filename;
			system(cmd.data());
		}
		// Disabled that for now, as kprinter won't work otherwise
		// leaving that file around doesn't harm, as it will be overwritten the next time.
		// unlink(filename);
	}

	retw = true;
	return retw;
}
