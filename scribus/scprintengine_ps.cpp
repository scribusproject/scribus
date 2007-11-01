/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scprintengine_ps.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "scribuscore.h"
#include "prefsmanager.h"
#include "pslib.h"
#include "util.h"
#include "util_ghostscript.h"

bool ScPrintEngine_PS::print(ScribusDoc& doc, PrintOptions& options)
{
	bool retw = false;
	ColorList usedColors;
	QMap<QString, QMap<uint, FPointArray> > usedFonts;
	QString filename(options.filename);
	doc.getUsedFonts(usedFonts);
	doc.getUsedColors(usedColors);
	PrefsManager *prefsManager = PrefsManager::instance();
	PSLib *dd = new PSLib(options, true, prefsManager->appPrefs.AvailFonts, usedFonts, usedColors, false, options.useSpotColors);
	if (dd != NULL)
	{
		if (!options.toFile)
			filename = prefsManager->preferencesLocation()+"/tmp.ps";
		bool PSfile = dd->PS_set_file(filename);
		filename = QDir::convertSeparators(filename);
		if (PSfile)
		{
			// Write the PS to a file
			int psCreationRetVal=dd->CreatePS(&doc, options);
			if (psCreationRetVal!=0)
			{
				QFile::remove(filename);
				if (psCreationRetVal==2)
					return true;
				else
					return false;
			}
			if (options.prnEngine != PostScript3 && ScCore->haveGS())
			{
				// use gs to convert our PS to a lower version
				QString tmp;
				QStringList opts;
				opts.append( QString("-dDEVICEWIDTHPOINTS=%1").arg(tmp.setNum(doc.pageWidth)) );
				opts.append( QString("-dDEVICEHEIGHTPOINTS=%1").arg(tmp.setNum(doc.pageHeight)) );
				convertPS2PS(filename, filename + ".tmp", opts, options.prnEngine);
				moveFile( filename + ".tmp", filename );
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
//				unlink(filename);
			}
			retw = true;
		}
		else
			retw = false;
		delete dd;
	}
	return retw;
}