/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "filewatcher.h"
#include "pageitemiterator.h"
#include "pdflib.h"
#include "pdflib_core.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scprintengine_pdf.h"
#include "scribusstructs.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "util_file.h"
#include "util_ghostscript.h"

ScPrintEngine_PDF::ScPrintEngine_PDF(ScribusDoc& doc) :
	ScPrintEngine(doc),
	m_prefsManager(PrefsManager::instance())
{

}

bool ScPrintEngine_PDF::print(PrintOptions& options)
{
	QString fileName(options.filename);

	if (!options.toFile)
		fileName = ScPaths::tempFileDir() + "/tmp.pdf";
	fileName = QDir::toNativeSeparators(fileName);

	// Write the PS to a file
	QString errorMessage;

	int pdfCreationRetVal = createPDFFile(fileName, options, errorMessage);
	if (pdfCreationRetVal != 0)
	{
		QFile::remove(fileName);
		if (pdfCreationRetVal == 2) // Aborted by user
			return true;
		m_errorMessage = errorMessage;
		return false;
	}
	if (options.toFile)
		return true;

	// Print and delete the PDF file
	QByteArray cmd;
	if (options.useAltPrintCommand)
	{
		cmd += options.printerCommand.toLocal8Bit();
		cmd += " ";
		cmd += "\"" + fileName.toLocal8Bit() + "\"";
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
		cmd += "\"" + fileName.toLocal8Bit() + "\"";
		system(cmd.data());
	}
	// Disabled that for now, as kprinter won't work otherwise
	// leaving that file around doesn't harm, as it will be overwritten the next time.
	// unlink(filename);

	return true;
}

int ScPrintEngine_PDF::createPDFFile(const QString& fileName, const PrintOptions& options, QString& errorMessage)
{
	PDFOptions pdfOptions;

	pdfOptions.Version = PDFVersion::PDF_14;
	
	// Embed all fonts
	pdfOptions.FontEmbedding = PDFOptions::EmbedFonts;
	setupFontEmbedding(pdfOptions);

	pdfOptions.doClip = options.doClip;
	pdfOptions.MirrorH = options.mirrorH;
	pdfOptions.MirrorV = options.mirrorV;
	pdfOptions.PresentMode = false;
	pdfOptions.Encrypt = false; // Disable PDF encryption for printing
	pdfOptions.Thumbnails = false; //Disable thumbnails too

	if (options.useColor)
	{
		pdfOptions.isGrayscale = false;
		pdfOptions.UseRGB = false;
		pdfOptions.UseProfiles = true;
		pdfOptions.UseProfiles2 = true;
	}
	else
	{
		pdfOptions.isGrayscale = true;
		pdfOptions.UseRGB = false;
		pdfOptions.UseProfiles = false;
		pdfOptions.UseProfiles2 = false;
	}
	pdfOptions.SolidProf = m_doc.colorPrefs().DCMSset.DefaultSolidColorRGBProfile;
	pdfOptions.ImageProf = m_doc.colorPrefs().DCMSset.DefaultImageRGBProfile;
	pdfOptions.PrintProf = m_doc.colorPrefs().DCMSset.DefaultPrinterProfile;
	pdfOptions.UseSpotColors = options.useSpotColors;

	pdfOptions.cropMarks = options.colorMarks;
	pdfOptions.bleedMarks = options.bleedMarks;
	pdfOptions.registrationMarks = options.registrationMarks;
	pdfOptions.colorMarks = options.colorMarks;
	pdfOptions.docInfoMarks = false;
	pdfOptions.markLength = options.markLength;
	pdfOptions.markOffset = options.markOffset;
	pdfOptions.bleeds = options.bleeds;

	// Generate PDF
//	QString errorString;
	
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	PDFlib pdflib(m_doc, pdfOptions);
	bool success = pdflib.doExport(fileName, options.pageNumbers, QMap<int, QImage>());
	if (!success)
		errorMessage = pdflib.errorMessage();
	ScCore->fileWatcher->start();

	if (!success)
		return 1;
	if (pdflib.exportAborted())
		return 2;
	return 0;
}

void ScPrintEngine_PDF::setupFontEmbedding(PDFOptions& options)
{
	QMap<QString, int> usedFonts = m_doc.reorganiseFonts();
	QStringList docFonts = usedFonts.keys();

	options.OutlineList.clear();
	options.EmbedList.clear();
	options.SubsetList.clear();

	// Build a list of all fonts used in Annotations
	QMap<QString, QString> annotationFonts;

	int pageItOptions = PageItemIterator::IterateInGroups | PageItemIterator::IterateInDocItems | PageItemIterator::IterateInMasterItems | PageItemIterator::IterateInFrameItems;
	for (PageItemIterator it(&m_doc, pageItOptions); *it; ++it)
	{
		PageItem *currItem = *it;
		if (((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText)) && (currItem->isAnnotation()))
		{
			int annotType  = currItem->annotation().Type();
			bool mustEmbed = ((annotType >= Annotation::Button) && (annotType <= Annotation::Listbox) && (annotType != Annotation::Checkbox));
			if (currItem->itemText.length() > 0 || mustEmbed)
				annotationFonts.insert(currItem->itemText.defaultStyle().charStyle().font().replacementName(), QString());
		}
	}

	const auto& allFonts = m_prefsManager.appPrefs.fontPrefs.AvailFonts;
	PDFVersion pdfVer = options.Version;

	for (int i = 0; i < docFonts.count(); ++i)
	{
		QString fontName = docFonts.at(i);
		const ScFace fontFace = allFonts[fontName];
		if (!fontFace.subset() && (!fontFace.isOTF() || pdfVer.supportsEmbeddedOpenTypeFonts()))
			options.EmbedList.append(fontName);
		else if (annotationFonts.contains(fontName))
			options.EmbedList.append(fontName);
		else
			options.SubsetList.append(fontName);
	}
}
