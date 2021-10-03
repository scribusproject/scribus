/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <memory>

#include "scpaths.h"
#include "scprintengine_ps.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "scribuscore.h"
#include "pslib.h"
#include "util_file.h"
#include "util_ghostscript.h"

ScPrintEngine_PS::ScPrintEngine_PS(ScribusDoc& doc)
	: ScPrintEngine(doc)
{

}

bool ScPrintEngine_PS::print(PrintOptions& options)
{
	QString filename(options.filename);

	std::unique_ptr<PSLib> psLib(new PSLib(&m_doc, options, PSLib::OutputPS));
	if (!psLib)
		return false;

	if (!options.toFile)
		filename = ScPaths::tempFileDir() + "/tmp.ps";

	// Write the PS to a file
	filename = QDir::toNativeSeparators(filename);

	int psCreationRetVal = psLib->createPS(filename);
	if (psCreationRetVal != 0)
	{
		QFile::remove(filename);
		if (psCreationRetVal == 2)
			return true;
		m_errorMessage = psLib->errorMessage();
		return false;
	}
	if (options.prnLanguage != PrintLanguage::PostScript3 && ScCore->haveGS())
	{
		// use gs to convert our PS to a lower version
		QString tmp;
		QStringList opts;
		opts.append( QString("-dDEVICEWIDTHPOINTS=%1").arg(tmp.setNum(m_doc.pageWidth())) );
		opts.append( QString("-dDEVICEHEIGHTPOINTS=%1").arg(tmp.setNum(m_doc.pageHeight())) );
		convertPS2PS(filename, filename + ".tmp", opts, (int) options.prnLanguage);
		moveFile(filename + ".tmp", filename);
	}
	if (options.toFile)
		return true;

	// Print and delete the PS file
	QByteArray cmd;
	if (options.useAltPrintCommand)
	{
		cmd += options.printerCommand.toLocal8Bit();
		cmd += " ";
		cmd += "\"" + filename.toLocal8Bit() + "\"";
		system(cmd.data());
	}
	else
	{
		QByteArray cc;
		cmd += "lpr -P '";
		cmd += options.printer.toLocal8Bit();
		cmd += "'";
		if (options.copies > 1)
			cmd += " -#" + cc.setNum(options.copies);
		cmd += options.printerOptions.toLocal8Bit();
		cmd += " ";
		cmd += "\"" + filename.toLocal8Bit() + "\"";
		system(cmd.data());
	}
	// Disabled that for now, as kprinter won't work otherwise
	// leaving that file around doesn't harm, as it will be overwritten the next time.
	// unlink(filename);

	return true;
}
