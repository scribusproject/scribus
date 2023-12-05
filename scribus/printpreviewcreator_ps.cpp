/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QScopedPointer>
#include <QTemporaryFile>
#include <QTransform>

#include "commonstrings.h"
#include "cmsettings.h"
#include "iconmanager.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "printpreviewcreator_ps.h"
#include "pslib.h"
#include "sccolorengine.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "util.h"
#include "util_ghostscript.h"
#include "util_printer.h"

PrintPreviewCreator_PS::PrintPreviewCreator_PS(ScribusDoc* doc) :
	SeparationPreviewCreator(doc),
	m_prefsManager(PrefsManager::instance())
{
	m_printOptions.prnLanguage = PrintLanguage::PostScript3;

	// Generate a template name for temporary files
	QScopedPointer<QTemporaryFile> tempFile(new QTemporaryFile(ScPaths::tempFileDir() + "/scpspreview_XXXXXX.png"));
	if (tempFile->open())
	{
		QString tempFileBase = tempFile->fileName();
		tempFile->setAutoRemove(false);
		tempFile->close();
		m_tempBaseName = QFileInfo(tempFileBase).completeBaseName();
	}
	if (m_tempBaseName.isEmpty())
		m_tempBaseName = "scpspreview";
}

PrintPreviewCreator_PS::~PrintPreviewCreator_PS()
{
	cleanupTemporaryFiles();
}

void PrintPreviewCreator_PS::cleanupTemporaryFiles()
{
	QString tempFileDir = ScPaths::tempFileDir();
	QFile::remove(tempFileDir + "/" + m_tempBaseName + ".ps");
	QFile::remove(tempFileDir + "/" + m_tempBaseName + ".png");
	QDir d(tempFileDir + "/", m_tempBaseName + "*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint i = 0; i < d.count(); i++)
			QFile::remove(tempFileDir + "/" + d[i]);
	}
}

QPixmap PrintPreviewCreator_PS::createPreview(int pageIndex)
{
	int ret = -1;
	int gsRes = qRound(m_previewResolution * m_devicePixelRatio);
	int w = qRound(m_doc->Pages->at(pageIndex)->width() * gsRes / 72.0);
	int h = qRound(m_doc->Pages->at(pageIndex)->height() * gsRes / 72.0);

	QPixmap pixmap;
	if (m_printOptionsChanged || (m_pageIndex != pageIndex))
	{
		bool success = createPreviewFile(pageIndex);
		if (!success)
		{
			imageLoadError(pixmap, pageIndex);
			return pixmap;
		}
	}

	if (m_printOptionsChanged || m_renderingOptionsChanged || (m_pageIndex != pageIndex))
	{
		if (m_sepPreviewEnabled && m_haveTiffSep)
			ret = renderPreviewSep(pageIndex, gsRes);
		else
			ret = renderPreview(pageIndex, gsRes);
		if (ret > 0)
		{
			imageLoadError(pixmap, pageIndex);
			return pixmap;
		}
	}

	QImage image;
	if (m_sepPreviewEnabled && m_haveTiffSep)
	{
		bool loadError;
		int cyan, magenta, yellow, black;

		ScImage im;
		bool mode;
		int w2 = w;
		int h2 = h;
		if (m_doc->Pages->at(pageIndex)->orientation() == 1)
			std::swap(w2, h2);
		image = QImage(w2, h2, QImage::Format_ARGB32);
		image.fill(qRgba(0, 0, 0, 0));
		
		QStringList separationNames { "Cyan", "Magenta", "Yellow" };
		CMSettings cms(m_doc, "", Intent_Perceptual);
		cms.allowColorManagement(false);
		for (int i = 0; i < separationNames.count(); ++i)
		{
			QString separationName = separationNames.at(i);
			if (!isSeparationVisible(separationName))
				continue;
			if (m_gsVersion < 854)
				loadError = im.loadPicture(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".tif." + separationName + ".tif", 1, cms, ScImage::RGBData, 72, &mode);
			else if (m_gsVersion <= 905)
				loadError = im.loadPicture(ScPaths::tempFileDir() + "/" + m_tempBaseName + "." + separationName + ".tif", 1, cms, ScImage::RGBData, 72, &mode);
			else
				loadError = im.loadPicture(ScPaths::tempFileDir() + "/" + m_tempBaseName + "(" + separationName + ").tif", 1, cms, ScImage::RGBData, 72, &mode);
			if (!loadError)
			{
				imageLoadError(pixmap, pageIndex);
				return pixmap;
			}
			if (m_showInkCoverage)
				blendImagesSumUp(image, im);
			else
			{
				int c = (i == 0) ? 255 : 0;
				int m = (i == 1) ? 255 : 0;
				int j = (i == 2) ? 255 : 0;
				blendImages(image, im, ScColor(c, m, j, 0));
			}
		}

		for (auto sepit = m_sepsToFileNum.begin(); sepit != m_sepsToFileNum.end(); ++sepit)
		{
			bool visibleSeparation = m_separationVisibilities.value(sepit.key(), false);
			if (!visibleSeparation)
				continue;
			QString sepFileName;
			if (m_gsVersion < 854)
				sepFileName = QString(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".tif.s%1.tif").arg(sepit.value());
			else if (m_gsVersion <= 905)
				sepFileName = QString(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".s%1.tif").arg(sepit.value());
			else
				sepFileName = QString(ScPaths::tempFileDir() + "/" + m_tempBaseName + "(%1).tif").arg(sepit.key());
			if (!im.loadPicture(sepFileName, 1, cms, ScImage::RGBData, 72, &mode))
			{
				imageLoadError(pixmap, pageIndex);
				return pixmap;
			}
			if (m_showInkCoverage)
				blendImagesSumUp(image, im);
			else
				blendImages(image, im, m_doc->PageColors[sepit.key()]);
		}
		
		if (m_separationVisibilities.value("Black", false))
		{
			CMSettings cms(m_doc, "", Intent_Perceptual);
			cms.allowColorManagement(false);
			if (m_gsVersion < 854)
				loadError = im.loadPicture(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".tif.Black.tif", 1, cms, ScImage::RGBData, 72, &mode);
			else if (m_gsVersion <= 905)
				loadError = im.loadPicture(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".Black.tif", 1, cms, ScImage::RGBData, 72, &mode);
			else
				loadError = im.loadPicture(ScPaths::tempFileDir() + "/" + m_tempBaseName + "(Black).tif", 1, cms, ScImage::RGBData, 72, &mode);
			if (!loadError)
			{
				imageLoadError(pixmap, pageIndex);
				return pixmap;
			}
			if (m_showInkCoverage)
				blendImagesSumUp(image, im);
			else
				blendImages(image, im, ScColor(0, 0, 0, 255));
		}

		if (m_showInkCoverage)
		{
			uint limitVal = (m_inkCoverageThreshold * 255) / 100;
			for (int yi = 0; yi < h2; ++yi)
			{
				QRgb *q = (QRgb*) image.scanLine(yi);
				for (int xi = 0; xi < w2; ++xi)
				{
					uint greyVal = *q;
					if (greyVal != 0)
					{
						if (limitVal == 0)
						{
							QColor tmpC;
							tmpC.setHsv((greyVal * 359) / m_inkMax, 255, 255);
							*q = tmpC.rgba();
						}
						else
						{
							int col = qMin(255 - static_cast<int>(((greyVal * 128) / m_inkMax) * 2), 255);
							if ((*q > 0) && (*q < limitVal))
								*q = qRgba(col, col, col, 255);
							else
								*q = qRgba(col, 0, 0, 255);
						}
					}
					else
					{
						if (!m_showTransparency)
							*q = qRgba(255, 255, 255, 255);
					}
					q++;
				}
			}
		}
		else if (m_doc->HasCMS || ScCore->haveCMS())
		{
			QRgb alphaFF = qRgba(0,0,0,255);
			QRgb alphaOO = qRgba(255,255,255,0);
			ScColorMgmtEngine engine = m_doc->colorEngine;
			ScColorProfile cmykProfile = m_doc->HasCMS ? m_doc->DocPrinterProf : ScCore->defaultCMYKProfile;
			ScColorProfile rgbProfile  = m_doc->HasCMS ? m_doc->DocDisplayProf : ScCore->defaultRGBProfile;
			ScColorTransform transCMYK = engine.createTransform(cmykProfile, Format_YMCK_8, rgbProfile, Format_BGRA_8, Intent_Relative_Colorimetric, 0);
			for (int yi = 0; yi < h2; ++yi)
			{
				uchar* ptr = image.scanLine( yi );
				transCMYK.apply(ptr, ptr, image.width());
				QRgb *q = (QRgb *) ptr;
				for (int xi = 0; xi < image.width(); xi++, q++)
				{
					if (m_showTransparency)
					{
						cyan = qRed(*q);
						magenta = qGreen(*q);
						yellow = qBlue(*q);
						if	((cyan == 255) && (magenta == 255) && (yellow == 255))
							*q = alphaOO;
						else
							*q |= alphaFF;
					}
					else
						*q |= alphaFF;
				}
			}
		}
		else
		{
			for (int yi = 0; yi < h2; ++yi)
			{
				QRgb *q = (QRgb*) image.scanLine(yi);
				for (int xi = 0; xi < w2; ++xi)
				{
					cyan = qRed(*q);
					magenta = qGreen(*q);
					yellow = qBlue(*q);
					black = qAlpha(*q);
					if ((cyan != 0) || (magenta != 0) || (yellow != 0 ) || (black != 0))
						*q = qRgba(255 - qMin(255, cyan + black), 255 - qMin(255, magenta + black), 255 - qMin(255, yellow + black), 255);
					else
					{
						if (!m_showTransparency)
							*q = qRgba(255, 255, 255, 255);
					}
					q++;
				}
			}
		}
	}
	else
	{
		QString previewFile;
		if (m_showTransparency && m_havePngAlpha)
			previewFile = ScPaths::tempFileDir() + "/" + m_tempBaseName + ".png";
		else
			previewFile = ScPaths::tempFileDir() + "/" + m_tempBaseName + ".tif";
		if (!image.load(previewFile))
		{
			imageLoadError(pixmap, pageIndex);
			return pixmap;
		}
		image = image.convertToFormat(QImage::Format_ARGB32);
		if (m_showTransparency && m_havePngAlpha)
		{
			int wi = image.width();
			int hi = image.height();
			for (int yi = 0; yi < hi; ++yi)
			{
				QRgb *s = (QRgb*) image.scanLine(yi);
				for (int xi = 0; xi < wi; ++xi)
				{
					if ((*s) == 0xffffffff)
						(*s) &= 0x00ffffff;
					s++;
				}
			}
		}
	}

	const ScPage* page = m_doc->Pages->at(pageIndex);
	if ((page->orientation() == 1) && (image.width() < image.height()))
		image = image.transformed( QTransform(0, 1, -1, 0, 0, 0) );

	image.setDevicePixelRatio(m_devicePixelRatio);
	if (m_showTransparency)
	{
		pixmap = QPixmap(image.width(), image.height());
		pixmap.setDevicePixelRatio(m_devicePixelRatio);
		QPainter p;
		QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill.png"));
		p.begin(&pixmap);
		p.fillRect(0, 0, image.width(), image.height(), b);
		p.drawImage(0, 0, image);
		p.end();
	}
	else
		pixmap = QPixmap::fromImage(image);
	pixmap.setDevicePixelRatio(m_devicePixelRatio);

	m_pageIndex = pageIndex;
	m_printOptionsChanged = false;
	m_renderingOptionsChanged = false;

	return pixmap;
}

bool PrintPreviewCreator_PS::createPreviewFile(int pageIndex)
{
	std::vector<int> pageNumbers { pageIndex + 1 };

	PrintOptions printOptions(m_printOptions);
	printOptions.pageNumbers = pageNumbers;
	printOptions.outputSeparations = false;
	printOptions.separationName = "All";
	printOptions.allSeparations = QStringList();
	printOptions.setDevParam = false;

	// Disable crop marks
	printOptions.cropMarks = false;
	printOptions.bleedMarks = false;
	printOptions.registrationMarks = false;
	printOptions.colorMarks = false;
	printOptions.markLength = 20.0;
	printOptions.markOffset = 0.0;
	printOptions.bleeds.set(0, 0, 0, 0);

	// Generate PostScript
	QString psFileName = ScPaths::tempFileDir() + "/"  + m_tempBaseName + ".ps";
	
	PSLib *psLib = new PSLib(m_doc, printOptions, PSLib::OutputPS, &m_doc->PageColors);
	if (!psLib)
		return false;
	bool success = (psLib->createPS(psFileName) == 0);
	delete psLib;

	// TODO : PostScript level < 3
	if (success && (printOptions.prnLanguage != PrintLanguage::PostScript3))
	{
		// use gs to convert our PS to a lower version
		QStringList opts;
		const ScPage* page = m_doc->Pages->at(pageIndex);
		double pageWidth = page->width();
		double pageHeight = page->height();
		if (page->orientation() == 1)
			std::swap(pageWidth, pageHeight);
		opts.append( QString("-dDEVICEWIDTHPOINTS=%1").arg(QString::number(pageWidth)) );
		opts.append( QString("-dDEVICEHEIGHTPOINTS=%1").arg(QString::number(pageHeight)) );

		QString outFileName = ScPaths::tempFileDir() + "/"  + m_tempBaseName + ".ps" + QString::number((int) printOptions.prnLanguage);
		success = (convertPS2PS(psFileName, outFileName, opts, (int) printOptions.prnLanguage) == 0);
		if (!success)
			return false;
		success &= QFile::remove(psFileName);
		success &= QFile::rename(outFileName, psFileName);
	}

	return success;
}

int PrintPreviewCreator_PS::renderPreview(int pageIndex, int res)
{
	int ret = -1;
	QString cmd1;

	QStringList args;
	QString tmp, tmp2, tmp3;
	int w = qRound(m_doc->Pages->at(pageIndex)->width() * res / 72.0);
	int h = qRound(m_doc->Pages->at(pageIndex)->height() * res / 72.0);
	if (m_doc->Pages->at(pageIndex)->orientation() == 1)
		std::swap(w, h);

	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( QString("-r%1").arg(tmp.setNum(res)) );
	args.append( QString("-g%1x%2").arg(tmp2.setNum(w), tmp3.setNum(h)) );
	if (m_sepPreviewEnabled)
	{
		if (!m_haveTiffSep)
			return 1;
		args.append("-sDEVICE=tiffsep");
	}
	else
	{
		if (m_showTransparency && m_havePngAlpha)
			args.append("-sDEVICE=pngalpha");
		else
			args.append("-sDEVICE=tiff24nc");	
	}
	if (m_useAntialiasing)
	{
		args.append("-dTextAlphaBits=4");
		args.append("-dGraphicsAlphaBits=4");
	}
	if ((m_doc->HasCMS) && (m_gsVersion >= 900))
	{
		args.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(m_doc->DocPrinterProf.profilePath()));
		if (m_sepPreviewEnabled)
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(m_doc->DocPrinterProf.profilePath()));
		else
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(m_doc->DocDisplayProf.profilePath()));
	}
	else if (ScCore->haveCMS() && (m_gsVersion >= 900))
	{
		args.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(ScCore->defaultCMYKProfile.profilePath()));
		if (m_sepPreviewEnabled)
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(ScCore->defaultCMYKProfile.profilePath()));
		else
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(ScCore->defaultRGBProfile.profilePath()));
	}

	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = m_prefsManager.prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd1 = QString("-sFONTPATH=%1").arg(QDir::toNativeSeparators(extraFonts->get(0,0)));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd1 += QString("%1%2").arg(sep).arg(QDir::toNativeSeparators(extraFonts->get(i,0)));
	if (!cmd1.isEmpty())
		args.append( cmd1 );

	// then add any final args and call gs
	QString tempFileDir = ScPaths::tempFileDir() ;
	if (m_sepPreviewEnabled)
		args.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(tempFileDir + "/" + m_tempBaseName + ".tif")) );
	else if (m_showTransparency && m_havePngAlpha)
		args.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(tempFileDir + "/" + m_tempBaseName + ".png")) );
	else
		args.append(QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(tempFileDir + "/" + m_tempBaseName + ".tif")));
	args.append( QDir::toNativeSeparators(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".ps") );
	args.append( "-c" );
	args.append( "showpage" );
	args.append( "-c" );
	args.append( "quit" );
	ret = System(m_prefsManager.ghostscriptExecutable(), args);
	return ret;
}

int PrintPreviewCreator_PS::renderPreviewSep(int pageIndex, int res)
{
	int ret = -1;
	QString cmd;
	QStringList args, args1, args2, args3;

	QString tmp, tmp2, tmp3;
	int w = qRound(m_doc->Pages->at(pageIndex)->width() * res / 72.0);
	int h = qRound(m_doc->Pages->at(pageIndex)->height() * res / 72.0);
	if (m_doc->Pages->at(pageIndex)->orientation() == 1)
		std::swap(w, h);

	args1.append( "-q" );
	args1.append( "-dNOPAUSE" );
	args1.append( "-dPARANOIDSAFER" );
	args1.append( QString("-r%1").arg(tmp.setNum(res)) );
	args1.append( QString("-g%1x%2").arg(tmp2.setNum(w), tmp3.setNum(h)) );
	if (m_useAntialiasing)
	{
		args1.append("-dTextAlphaBits=4");
		args1.append("-dGraphicsAlphaBits=4");
	}
	if ((m_doc->HasCMS) && (m_gsVersion >= 900))
	{
		args1.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(m_doc->DocPrinterProf.profilePath()));
		args1.append("-sOutputICCProfile=" + QDir::toNativeSeparators(m_doc->DocPrinterProf.profilePath()));
	}
	else if (ScCore->haveCMS() && (m_gsVersion >= 900))
	{
		args.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(ScCore->defaultCMYKProfile.profilePath()));
		args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(ScCore->defaultCMYKProfile.profilePath()));
	}

	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = m_prefsManager.prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH=%1").arg(QDir::toNativeSeparators(extraFonts->get(0,0)));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString("%1%2").arg(sep).arg(QDir::toNativeSeparators(extraFonts->get(i,0)));
	if (!cmd.isEmpty())
		args1.append(cmd);
	args1.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".tif")) );

	args2.append( QDir::toNativeSeparators(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".ps") );
	args2.append("-c");
	args2.append("quit");

	ColorList usedSpots;
	m_doc->getUsedColors(usedSpots, true);
	QStringList spots = usedSpots.keys();
	args3.append( "-sDEVICE=tiffsep" );
	if (m_gsVersion >= 954)
		args3.append( "-dPrintSpotCMYK=true" );

	QString allSeps ="[ /Cyan /Magenta /Yellow /Black ";
	for (int sp = 0; sp < spots.count(); ++sp)
	{
		allSeps += "(" + spots[sp] + ") ";
	}
	allSeps += "]";

	// Workaround for https://bugs.ghostscript.com/show_bug.cgi?id=707365
	// This issue causes separations rendered individually to be garbled,
	// so render files with a reasonable number of spot colors in one go
	constexpr int maxSpotColors = 4;
	cmd = "<< /SeparationColorNames ";
	if (spots.count() <= maxSpotColors)
		cmd += allSeps + " /SeparationOrder " + allSeps + " >> setpagedevice";
	else
		cmd += allSeps + " /SeparationOrder [ /Cyan /Magenta /Yellow /Black] >> setpagedevice";

	QFile fx(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/" +  m_tempBaseName + ".sep.ps"));
	if (fx.open(QIODevice::WriteOnly))
	{
		QTextStream tsx(&fx);
		tsx << cmd;
		fx.close();
	}

	QString gsExe(getShortPathName(m_prefsManager.ghostscriptExecutable()));
	ret = System(gsExe, args1 + args3 + args2, ScPaths::tempFileDir() + "/" +  m_tempBaseName + ".tif.txt" );

	QFile sepInfo(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/" +  m_tempBaseName + ".tif.txt"));
	m_sepsToFileNum.clear();
	if (sepInfo.open(QIODevice::ReadOnly))
	{
		QString sepLine;
		QTextStream tsC(&sepInfo);
		int counter = 0;
		while (!tsC.atEnd())
		{
			sepLine = tsC.readLine();
			if (sepLine.startsWith("Warning"))
				continue;
			QString tt;
			if (sepLine.contains("%%SeparationColor:"))
			{
				int firstQuote = sepLine.indexOf('\"');
				int lastQuote = sepLine.lastIndexOf('\"');
				if (firstQuote >= 0 && lastQuote >= 0 && firstQuote < lastQuote)
					tt = sepLine.mid(firstQuote + 1, lastQuote - firstQuote - 1);
			}
			else
			{
				tt = sepLine.remove("%%SeparationName:").trimmed();
				int cmykVals = tt.lastIndexOf("CMYK =");
				if (cmykVals >= 0)
					tt = tt.left(cmykVals).trimmed();
			}
			if (tt == "Cyan" || tt == "Magenta" || tt == "Yellow" || tt == "Black")
				tt.clear();
			if (!tt.isEmpty())
			{
				m_sepsToFileNum.insert(tt, counter);
				counter++;
			}
		}
	}
	sepInfo.close();

	QString currSeps;
	uint spc = 0;
	for (int sp = maxSpotColors; sp < spots.count(); ++sp)
	{
		currSeps += "(" + spots[sp] + ") ";
		spc++;
		if (spc > 6 || sp == spots.count() - 1)
		{
			args3.clear();
			args3.append("-sDEVICE=tiffsep");
			QFile fx(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/" +  m_tempBaseName + ".sep.ps"));
			if (fx.open(QIODevice::WriteOnly))
			{
				QTextStream tsx(&fx);
				tsx << QString("<< /SeparationColorNames " + allSeps + " /SeparationOrder [ " + currSeps + " ] >> setpagedevice");
				fx.close();
			}
			args3.append("-f");
			args3.append(QDir::toNativeSeparators(ScPaths::tempFileDir() + "/" +  m_tempBaseName + ".sep.ps"));
			ret = System(gsExe, args1 + args3 + args2);
			currSeps.clear();
			spc = 0;
		}
	}
	return ret;
}

void PrintPreviewCreator_PS::setPrintOptions(const PrintOptions& options)
{
	m_printOptions = options;
	m_printOptions.prnLanguage = PrintLanguage::PostScript3;
	m_printOptionsChanged = true;
}

void PrintPreviewCreator_PS::imageLoadError(QPixmap &pixmap, int page)
{
	pixmap = QPixmap(1, 1);
	m_pageIndex = page;
	m_printOptionsChanged = false;
	m_renderingOptionsChanged = false;
}
