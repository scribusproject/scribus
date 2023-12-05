/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QApplication>
#include <QDesktopWidget>
#include <QMap>
#include <QTemporaryFile>

#include "outputpreview_pdf.h"
#include "cmsettings.h"
#include "filewatcher.h"
#include "iconmanager.h"
#include "pageitemiterator.h"
#include "pdflib.h"
#include "pdflib_core.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "util.h"
#include "util_ghostscript.h"

#include "ui_outputpreviewbase.h"
#include "ui_outputpreview_pdf.h"

OutputPreview_PDF::OutputPreview_PDF(QWidget* parent, ScribusDoc* doc) :
	OutputPreviewBase(parent, doc),
	m_pdfOptions(doc->pdfOptions()),
	m_prefsManager(PrefsManager::instance())
{
	int inkTableWidth = 0;

	setModal(true);
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));

	QString caption = tr("PDF Output Preview");
	setWindowTitle(caption);

	m_havePngAlpha = ScCore->havePNGAlpha();
	m_haveTiffSep  = ScCore->haveTIFFSep();
	getNumericGSVersion(m_gsVersion);

	m_optionsUi = new Ui::OutputPreview_PDF();
	m_optionsUi->setupUi(m_uiBase->optionWidget);

	if (m_haveTiffSep)
	{
		ColorList usedSpots;
		doc->getUsedColors(usedSpots, true);
		QStringList spots = usedSpots.keys();

		m_inkMax = (spots.count() + 4) * 255;
		m_optionsUi->coverThresholdValue->setMaximum((spots.count() + 4) * 100.0);

		m_optionsUi->inkTable->setColumnCount(2);
		m_optionsUi->inkTable->setRowCount(4 + spots.count());
		m_optionsUi->inkTable->setHorizontalHeaderItem(0, new QTableWidgetItem(IconManager::instance().loadIcon("16/show-object.png"), ""));
		m_optionsUi->inkTable->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Separation Name")));

		QHeaderView *header = m_optionsUi->inkTable->horizontalHeader();
		header->setStretchLastSection(true);
		header->setSectionsMovable(false);
		header->setSectionResizeMode(QHeaderView::Fixed);

		m_optionsUi->inkTable->setColumnWidth(0, 24);
		m_optionsUi->inkTable->verticalHeader()->hide();
		m_optionsUi->inkTable->setFocusPolicy(Qt::NoFocus);
		m_optionsUi->inkTable->setSizeAdjustPolicy(QTableWidget::AdjustToContentsOnFirstShow);

		m_optionsUi->inkTable->setItem(0, 1, new QTableWidgetItem( tr("Cyan")));
		QCheckBox *cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		cp->setChecked(true);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		m_optionsUi->inkTable->setCellWidget(0, 0, cp);
		m_inkVisibilities.insert("Cyan", cp);

		m_optionsUi->inkTable->setItem(1, 1, new QTableWidgetItem( tr("Magenta")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		cp->setChecked(true);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		m_optionsUi->inkTable->setCellWidget(1, 0, cp);
		m_inkVisibilities.insert("Magenta", cp);

		m_optionsUi->inkTable->setItem(2, 1, new QTableWidgetItem( tr("Yellow")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		cp->setChecked(true);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		m_optionsUi->inkTable->setCellWidget(2, 0, cp);
		m_inkVisibilities.insert("Yellow", cp);

		m_optionsUi->inkTable->setItem(3, 1, new QTableWidgetItem( tr("Black")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		cp->setChecked(true);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		m_optionsUi->inkTable->setCellWidget(3, 0, cp);
		m_inkVisibilities.insert("Black", cp);

		for (int sp = 0; sp < spots.count(); ++sp)
		{
			const QString& spotName = spots.at(sp);
			m_optionsUi->inkTable->setItem(sp + 4, 1, new QTableWidgetItem(spotName));
			cp = new QCheckBox(this);
			cp->setFocusPolicy(Qt::NoFocus);
			cp->setChecked(true);
			connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
			m_optionsUi->inkTable->setCellWidget(sp + 4, 0, cp);
			m_inkVisibilities.insert(spotName, cp);
		}

		inkTableWidth = m_optionsUi->inkTable->columnWidth(1);

		bool inkCoverageEnabled = (m_optionsUi->enableCMYK->isChecked()) && (m_optionsUi->displayInkCoverage->isChecked());
		m_optionsUi->coverThresholdLabel->setEnabled(inkCoverageEnabled);
		m_optionsUi->coverThresholdValue->setEnabled(inkCoverageEnabled);
		m_optionsUi->coverThresholdValue->setSuffix( tr(" %"));
		
		connect(header, SIGNAL(sectionClicked(int)), this, SLOT(toggleAllFromHeader()));
		connect(m_optionsUi->enableCMYK, SIGNAL(clicked()), this, SLOT(toggleCMYK()));
		connect(m_optionsUi->displayInkCoverage, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		connect(m_optionsUi->inkTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onInkTableCellDoubleClicked(int)));
	}
	else
	{
		m_optionsUi->enableCMYK->setChecked(false);
		m_optionsUi->enableCMYK->setVisible(false);
		m_optionsUi->inkTable->setVisible(false);
		m_optionsUi->displayInkCoverage->setChecked(false);
		m_optionsUi->displayInkCoverage->setVisible(false);
		m_optionsUi->coverThresholdLabel->setVisible(false);
		m_optionsUi->coverThresholdValue->setVisible(false);
	}

	bool cmykEnabled = m_optionsUi->enableCMYK->isChecked();
	m_optionsUi->inkTable->setEnabled(cmykEnabled);
	m_optionsUi->displayInkCoverage->setEnabled(cmykEnabled);
	bool isInkCoverageEnabled = m_optionsUi->displayInkCoverage->isChecked();
	m_optionsUi->coverThresholdLabel->setEnabled(cmykEnabled && isInkCoverageEnabled);
	m_optionsUi->coverThresholdValue->setEnabled(cmykEnabled && isInkCoverageEnabled);

	if (m_optionsUi->inkTable->isVisible())
		m_optionsUi->inkTable->setEnabled(m_optionsUi->enableCMYK->isChecked());

	// Restore display settings from preferences
	m_optionsUi->antiAliasing->setChecked(m_prefsManager.appPrefs.pdfOutputPreviewPrefs.enableAntiAliasing);
	m_optionsUi->showTransparency->setChecked(m_prefsManager.appPrefs.pdfOutputPreviewPrefs.showTransparency);
	if (m_optionsUi->inkTable->isVisible() && m_optionsUi->inkTable->isEnabled())
	{
		m_optionsUi->enableCMYK->setChecked(m_prefsManager.appPrefs.pdfOutputPreviewPrefs.cmykPreviewMode);
		m_inkVisibilities["Cyan"]->setChecked(m_prefsManager.appPrefs.pdfOutputPreviewPrefs.isCyanVisible);
		m_inkVisibilities["Magenta"]->setChecked(m_prefsManager.appPrefs.pdfOutputPreviewPrefs.isMagentaVisible);
		m_inkVisibilities["Magenta"]->setChecked(m_prefsManager.appPrefs.pdfOutputPreviewPrefs.isYellowVisible);
		m_inkVisibilities["Yellow"]->setChecked(m_prefsManager.appPrefs.pdfOutputPreviewPrefs.isBlackVisible);
	}
	m_optionsUi->displayInkCoverage->setChecked(m_prefsManager.appPrefs.pdfOutputPreviewPrefs.displayInkCoverage);
	m_optionsUi->coverThresholdValue->setValue(m_prefsManager.appPrefs.pdfOutputPreviewPrefs.inkCoverageThreshold);

	// Generate a template name for temporary files
	QTemporaryFile *tempFile = new QTemporaryFile(ScPaths::tempFileDir() + "/scpreview_XXXXXX.pdf");
	if (tempFile->open())
	{
		QString tempFileBase = tempFile->fileName();
		tempFile->setAutoRemove(false);
		tempFile->close();
		m_tempBaseName = QFileInfo(tempFileBase).completeBaseName();
	}
	if (m_tempBaseName.isEmpty())
		m_tempBaseName = "scpreview";
	delete tempFile;

	setPDFOptionsToUi(m_pdfOptions);
	
	// Display preview
	QPixmap previewPix = createPreview(m_doc->currentPageNumber(), qRound(72 * m_scaleFactor));
	m_previewLabel->setPixmap(previewPix);
	m_previewLabel->resize(previewPix.size());

	m_uiBase->pageSelector->setGUIForPage(m_doc->currentPage()->pageNr());

	int w = m_previewLabel->width() + inkTableWidth + 50;
	resize(qMin(QApplication::desktop()->width() - 30, w), 500);

	//signals and slots
	connect(m_uiBase->pageSelector, SIGNAL(pageChanged(int)), this, SLOT(jumpToPage(int)));
	connect(m_uiBase->closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(m_uiBase->exportButton, SIGNAL(clicked()), this, SIGNAL(doExport()));
	connect(m_uiBase->scaleBox, SIGNAL(activated(int)), this, SLOT(onScaleBoxValueChanged(int)));

	connect(m_optionsUi->pdfVersionCombo, SIGNAL(activated(int)), this, SLOT(onPdfVersionChanged(int)));
	connect(m_optionsUi->outputModeCombo, SIGNAL(activated(int)), this, SLOT(onPdfOutputModeChanged(int)));
	connect(m_optionsUi->mirrorH, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(m_optionsUi->mirrorV, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(m_optionsUi->clipToMargins, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(m_optionsUi->convertSpots, SIGNAL(clicked()), this, SLOT(redisplay()));

	connect(m_optionsUi->antiAliasing, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(m_optionsUi->showTransparency, SIGNAL(clicked()), this, SLOT(redisplay()));

	connect(m_optionsUi->coverThresholdValue, SIGNAL(valueChanged(double)), this, SLOT(toggleCMYK_Colour()));
}

OutputPreview_PDF::~OutputPreview_PDF()
{
	cleanupTemporaryFiles();
	delete m_optionsUi;
}

bool OutputPreview_PDF::isCMYKPreviewEnabled() const
{
	return m_optionsUi->enableCMYK->isChecked();
}

bool OutputPreview_PDF::isAntialiasingEnabled() const
{
	return m_optionsUi->antiAliasing->isChecked();
}

bool OutputPreview_PDF::isTransparencyEnabled() const
{
	return m_optionsUi->showTransparency->isChecked();
}

bool OutputPreview_PDF::isInkChannelVisible(const QString& ink)
{
	if (m_inkVisibilities.contains(ink))
		return m_inkVisibilities[ink]->isChecked();
	return false;
}

bool OutputPreview_PDF::isInkCoverageEnabled() const
{
	return m_optionsUi->displayInkCoverage->isChecked();
}

double OutputPreview_PDF::inkCoverageThreshold() const
{
	return m_optionsUi->coverThresholdValue->value();
}

void OutputPreview_PDF::cleanupTemporaryFiles()
{
	QString tempFileDir = ScPaths::tempFileDir();
	QFile::remove(tempFileDir + "/" + m_tempBaseName + ".pdf");
	QFile::remove(tempFileDir + "/" + m_tempBaseName + ".png");
	QDir d(tempFileDir + "/", m_tempBaseName + "*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint i = 0; i < d.count(); i++)
			QFile::remove(tempFileDir + "/" + d[i]);
	}
}

QPixmap OutputPreview_PDF::createPreview(int pageIndex, int res)
{
	int ret = -1;
	int gsRes = qRound(res * devicePixelRatioF());
	int w = qRound(m_doc->Pages->at(pageIndex)->width() * gsRes / 72.0);
	int h = qRound(m_doc->Pages->at(pageIndex)->height() * gsRes / 72.0);

	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));

	QPixmap pixmap;
	if (pdfOptionsHaveChanged(pageIndex))
	{
		bool success = createPreviewFile(pageIndex);
		if (!success)
		{
			imageLoadError(pixmap);
			return pixmap;
		}
	}

	if (optionsHaveChanged(pageIndex))
	{
		if (m_optionsUi->enableCMYK->isChecked() && m_haveTiffSep)
			ret = renderPreviewSep(pageIndex, gsRes);
		else
			ret = renderPreview(pageIndex, gsRes);
		if (ret > 0)
		{
			imageLoadError(pixmap);
			return pixmap;
		}
	}

	QImage image;
	if (m_optionsUi->enableCMYK->isChecked() && m_haveTiffSep)
	{
		bool loadError;
		int cyan, magenta, yellow, black;

		ScImage im;
		bool mode;
		int w2 = w;
		int h2 = h;
		image = QImage(w2, h2, QImage::Format_ARGB32);
		image.fill(qRgba(0, 0, 0, 0));
		
		QStringList separationNames { "Cyan", "Magenta", "Yellow" };
		CMSettings cms(m_doc, "", Intent_Perceptual);
		cms.allowColorManagement(false);
		for (int i = 0; i < separationNames.count(); ++i)
		{
			QString separationName = separationNames.at(i);
			if (!m_inkVisibilities[separationName]->isChecked())
				continue;
			if (m_gsVersion < 854)
				loadError = im.loadPicture(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".tif." + separationName + ".tif", 1, cms, ScImage::RGBData, 72, &mode);
			else if (m_gsVersion <= 905)
				loadError = im.loadPicture(ScPaths::tempFileDir() + "/" + m_tempBaseName + "." + separationName + ".tif", 1, cms, ScImage::RGBData, 72, &mode);
			else
				loadError = im.loadPicture(ScPaths::tempFileDir() + "/" + m_tempBaseName + "(" + separationName + ").tif", 1, cms, ScImage::RGBData, 72, &mode);
			if (!loadError)
			{
				imageLoadError(pixmap);
				return pixmap;
			}
			if (m_optionsUi->displayInkCoverage->isChecked())
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
			const QCheckBox* checkBox = m_inkVisibilities.value(sepit.key(), nullptr);
			if (!checkBox || !checkBox->isChecked())
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
				imageLoadError(pixmap);
				return pixmap;
			}
			if (m_optionsUi->displayInkCoverage->isChecked())
				blendImagesSumUp(image, im);
			else
				blendImages(image, im, m_doc->PageColors[sepit.key()]);
		}
		
		if (m_inkVisibilities["Black"]->isChecked())
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
				imageLoadError(pixmap);
				return pixmap;
			}
			if (m_optionsUi->displayInkCoverage->isChecked())
				blendImagesSumUp(image, im);
			else
				blendImages(image, im, ScColor(0, 0, 0, 255));
		}

		if (m_optionsUi->displayInkCoverage->isChecked())
		{
			uint limitVal = (m_optionsUi->coverThresholdValue->value() * 255) / 100;
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
						if (!m_optionsUi->showTransparency->isChecked())
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
					if (m_optionsUi->showTransparency->isChecked())
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
						if (!m_optionsUi->showTransparency->isChecked())
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
		if (m_optionsUi->showTransparency->isChecked() && m_havePngAlpha)
			previewFile = ScPaths::tempFileDir() + "/" + m_tempBaseName + ".png";
		else
			previewFile = ScPaths::tempFileDir() + "/" + m_tempBaseName + ".tif";
		if (!image.load(previewFile))
		{
			imageLoadError(pixmap);
			return pixmap;
		}
		image = image.convertToFormat(QImage::Format_ARGB32);
		if (m_optionsUi->showTransparency->isChecked() && m_havePngAlpha)
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

	image.setDevicePixelRatio(devicePixelRatioF());
	if (m_optionsUi->showTransparency->isChecked())
	{
		pixmap = QPixmap(image.width(), image.height());
		pixmap.setDevicePixelRatio(devicePixelRatioF());
		QPainter p;
		QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill.png"));
		p.begin(&pixmap);
		p.fillRect(0, 0, image.width(), image.height(), b);
		p.drawImage(0, 0, image);
		p.end();
	}
	else
		pixmap = QPixmap::fromImage(image);
	pixmap.setDevicePixelRatio(devicePixelRatioF());

	qApp->restoreOverrideCursor();
	updateOptionsFromUI();
	return pixmap;
}

bool OutputPreview_PDF::createPreviewFile(int pageIndex)
{
	if (!m_fontEmbeddingWasSet)
	{
		setupFontEmbedding(m_pdfOptions);
		m_fontEmbeddingWasSet = true;
	}

	std::vector<int> pageNumbers { pageIndex + 1 };

	setUiOptionsToPDFOptions(m_pdfOptions);
	m_pdfOptions.Encrypt = false; // Disable PDF encryption for preview
	m_pdfOptions.Thumbnails = false; //Disable thumbnails too

	QString profileDescription;
	if ((m_pdfOptions.Version == PDFVersion::PDF_X3) || (m_pdfOptions.Version == PDFVersion::PDF_X1a) || (m_pdfOptions.Version == PDFVersion::PDF_X4))
	{
		if (m_pdfOptions.Info.isEmpty())
		{
			auto& printerProfiles = ScCore->PrinterProfiles;
			ScColorProfile outputProfile = m_doc->colorEngine.openProfileFromFile( printerProfiles[m_pdfOptions.PrintProf] );
			profileDescription = outputProfile.productDescription();
			m_pdfOptions.Info = profileDescription;
		}
		m_pdfOptions.MirrorH = false;
		m_pdfOptions.MirrorV = false;
		m_pdfOptions.PresentMode = false;
	}

	// Disable crop marks
	m_pdfOptions.cropMarks = false;
	m_pdfOptions.bleedMarks = false;
	m_pdfOptions.registrationMarks = false;
	m_pdfOptions.colorMarks = false;
	m_pdfOptions.markLength = 20.0;
	m_pdfOptions.markOffset = 0.0;
	m_pdfOptions.bleeds.set(0, 0, 0, 0);

	// Generate PDF
//	QString errorString;
	QString pdfFileName = ScPaths::tempFileDir() + "/"  + m_tempBaseName + ".pdf";
	
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	PDFlib pdflib(*m_doc, m_pdfOptions);
	bool success = pdflib.doExport(pdfFileName, pageNumbers, QMap<int, QImage>());
	success &= !pdflib.exportAborted();
	ScCore->fileWatcher->start();

	return success;
}

int OutputPreview_PDF::renderPreview(int pageIndex, int res)
{
	int ret = -1;
	QString cmd1;

	QStringList args;
	QString tmp, tmp2, tmp3;
	int w = qRound(m_doc->Pages->at(pageIndex)->width() * res / 72.0);
	int h = qRound(m_doc->Pages->at(pageIndex)->height() * res / 72.0);
	
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( QString("-r%1").arg(tmp.setNum(res)) );
	args.append( QString("-g%1x%2").arg(tmp2.setNum(w), tmp3.setNum(h)) );
	if (m_optionsUi->enableCMYK->isChecked())
	{
		if (!m_haveTiffSep)
			return 1;
		args.append("-sDEVICE=tiffsep");
	}
	else
	{
		if (m_optionsUi->showTransparency->isChecked() && m_havePngAlpha)
			args.append("-sDEVICE=pngalpha");
		else
			args.append("-sDEVICE=tiff24nc");	
	}
	if (m_optionsUi->antiAliasing->isChecked())
	{
		args.append("-dTextAlphaBits=4");
		args.append("-dGraphicsAlphaBits=4");
	}
	if ((m_doc->HasCMS) && (m_gsVersion >= 900))
	{
		args.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(m_doc->DocPrinterProf.profilePath()));
		if (m_optionsUi->enableCMYK->isChecked())
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(m_doc->DocPrinterProf.profilePath()));
		else
			args.append("-sOutputICCProfile=" + QDir::toNativeSeparators(m_doc->DocDisplayProf.profilePath()));
	}
	else if (ScCore->haveCMS() && (m_gsVersion >= 900))
	{
		args.append("-sDefaultCMYKProfile=" + QDir::toNativeSeparators(ScCore->defaultCMYKProfile.profilePath()));
		if (m_optionsUi->enableCMYK->isChecked())
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
	if (m_optionsUi->enableCMYK->isChecked())
		args.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(tempFileDir + "/" + m_tempBaseName + ".tif")) );
	else if (m_optionsUi->showTransparency->isChecked() && m_havePngAlpha)
		args.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(tempFileDir + "/" + m_tempBaseName + ".png")) );
	else
		args.append(QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(tempFileDir + "/" + m_tempBaseName + ".tif")));
	args.append( QDir::toNativeSeparators(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".pdf") );
	args.append( "-c" );
	args.append( "showpage" );
	args.append( "-c" );
	args.append( "quit" );
	ret = System(m_prefsManager.ghostscriptExecutable(), args);
	return ret;
}

int OutputPreview_PDF::renderPreviewSep(int pageIndex, int res)
{
	int ret = -1;
	QString cmd;
	QStringList args, args1, args2, args3;

	QString tmp, tmp2, tmp3;
	int w = qRound(m_doc->Pages->at(pageIndex)->width() * res / 72.0);
	int h = qRound(m_doc->Pages->at(pageIndex)->height() * res / 72.0);

	args1.append( "-q" );
	args1.append( "-dNOPAUSE" );
	args1.append( "-dPARANOIDSAFER" );
	args1.append( QString("-r%1").arg(tmp.setNum(res)) );
	args1.append( QString("-g%1x%2").arg(tmp2.setNum(w), tmp3.setNum(h)) );
	if (m_optionsUi->antiAliasing->isChecked())
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

	args2.append( QDir::toNativeSeparators(ScPaths::tempFileDir() + "/" + m_tempBaseName + ".pdf") );
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
		sepInfo.close();
	}

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

void OutputPreview_PDF::setupFontEmbedding(PDFOptions& options)
{
	QMap<QString, int> usedFonts = m_doc->reorganiseFonts();
	QStringList docFonts = usedFonts.keys();

	// Simple case: no font embedding
	if (m_pdfOptions.FontEmbedding == PDFOptions::DontEmbed)
	{
		m_pdfOptions.OutlineList.clear();
		m_pdfOptions.EmbedList.clear();
		m_pdfOptions.SubsetList.clear();
		return;
	}

	// Simple case: all fonts are outlined
	if (m_pdfOptions.FontEmbedding == PDFOptions::OutlineFonts)
	{
		m_pdfOptions.OutlineList = docFonts;
		m_pdfOptions.EmbedList.clear();
		m_pdfOptions.SubsetList.clear();
		return;
	}

	if (m_pdfOptions.EmbedList.count() != 0)
	{
		QList<QString> tmpEm;
		for (auto itef = m_pdfOptions.EmbedList.begin(); itef != m_pdfOptions.EmbedList.end(); ++itef)
		{
			if (usedFonts.contains((*itef)))
				tmpEm.append((*itef));
		}
		m_pdfOptions.EmbedList = tmpEm;
	}
	if (m_pdfOptions.SubsetList.count() != 0)
	{
		QList<QString> tmpEm;
		for (auto itef = m_pdfOptions.SubsetList.begin(); itef != m_pdfOptions.SubsetList.end(); ++itef)
		{
			if (usedFonts.contains((*itef)))
				tmpEm.append((*itef));
		}
		m_pdfOptions.SubsetList = tmpEm;
	}

	// Build a list of all fonts used in Annotations
	QMap<QString, QString> annotationFonts;

	int pageItOptions = PageItemIterator::IterateInGroups | PageItemIterator::IterateInDocItems | PageItemIterator::IterateInMasterItems | PageItemIterator::IterateInFrameItems;
	for (PageItemIterator it(m_doc, pageItOptions); *it; ++it)
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
	PDFVersion pdfVer = m_pdfOptions.Version;

	if ((m_pdfOptions.EmbedList.count() == 0) && (m_pdfOptions.SubsetList.count() == 0) && (m_pdfOptions.firstUse))
	{
		for (int i = 0; i < docFonts.count(); ++i)
		{
			QString fontName = docFonts.at(i);
			const ScFace fontFace = allFonts[fontName];
			if (!fontFace.subset() && (!fontFace.isOTF() || pdfVer.supportsEmbeddedOpenTypeFonts()))
				m_pdfOptions.EmbedList.append(fontName);
			else if (annotationFonts.contains(fontName))
				m_pdfOptions.EmbedList.append(fontName);
			else
				m_pdfOptions.SubsetList.append(fontName);
		}
	}
	else
	{
		for (int i = 0; i < docFonts.count(); ++i)
		{
			QString fontName = docFonts.at(i);
			const ScFace fontFace = allFonts[fontName];
			if (m_pdfOptions.EmbedList.contains(fontName) && (!fontFace.isOTF() || m_pdfOptions.supportsEmbeddedOpenTypeFonts()) && !fontFace.subset())
				continue;
			if (!m_pdfOptions.SubsetList.contains(fontName))
				m_pdfOptions.SubsetList.append(fontName);
		}

		for (auto itAnn = annotationFonts.begin(); itAnn != annotationFonts.end(); ++itAnn)
		{
			QString annotFont = itAnn.key();
			if (!m_pdfOptions.EmbedList.contains(annotFont))
				m_pdfOptions.EmbedList.append(annotFont);
			int subsetIndex = m_pdfOptions.SubsetList.indexOf(annotFont);
			if (subsetIndex >= 0)
				m_pdfOptions.SubsetList.removeAll(annotFont);
		}
	}
}

bool OutputPreview_PDF::optionsHaveChanged(int pageIndex) const
{
	if (m_currentPage != pageIndex)
		return true;
	if (m_scaleMode != m_uiBase->scaleBox->currentIndex())
		return true;
	if (m_cmykPreviewMode != m_optionsUi->enableCMYK->isChecked())
		return true;
	if (m_useAntialiasing != m_optionsUi->antiAliasing->isChecked())
		return true;
	if (m_showTransparency != m_optionsUi->showTransparency->isChecked())
		return true;

	if (m_pdfVersion != m_optionsUi->pdfVersionCombo->version())
		return true;
	if (m_pdfOutputMode != m_optionsUi->outputModeCombo->currentIndex())
		return true;
	if (m_mirrorH != m_optionsUi->mirrorH->isChecked())
		return true;
	if (m_mirrorV != m_optionsUi->mirrorV->isChecked())
		return true;
	if (m_clipToMargins != m_optionsUi->clipToMargins->isChecked())
		return true;
	if (m_convertSpots != m_optionsUi->convertSpots->isChecked())
		return true;

	return false;
}

bool OutputPreview_PDF::pdfOptionsHaveChanged(int pageIndex) const
{
	if (m_currentPage != pageIndex)
		return true;

	if (m_pdfVersion != m_optionsUi->pdfVersionCombo->version())
		return true;
	if (m_pdfOutputMode != m_optionsUi->outputModeCombo->currentIndex())
		return true;
	if (m_mirrorH != m_optionsUi->mirrorH->isChecked())
		return true;
	if (m_mirrorV != m_optionsUi->mirrorV->isChecked())
		return true;
	if (m_clipToMargins != m_optionsUi->clipToMargins->isChecked())
		return true;
	if (m_convertSpots != m_optionsUi->convertSpots->isChecked())
		return true;

	return false;
}

void OutputPreview_PDF::setPDFOptionsToOptions(PDFOptions& pdfOptions)
{
	m_pdfVersion = pdfOptions.Version;

	if (pdfOptions.UseRGB)
		m_pdfOutputMode = 0;
	else if (pdfOptions.isGrayscale)
		m_pdfOutputMode = 2;
	else
		m_pdfOutputMode = 1;

	m_mirrorH = pdfOptions.MirrorH;
	m_mirrorV = pdfOptions.MirrorV;
	m_clipToMargins = pdfOptions.doClip;
	m_convertSpots  = !pdfOptions.UseSpotColors;
}

void OutputPreview_PDF::setPDFOptionsToUi(PDFOptions& pdfOptions)
{
	m_optionsUi->pdfVersionCombo->setPDFXEnabled(m_doc->HasCMS && !ScCore->PDFXProfiles.isEmpty());
	m_optionsUi->pdfVersionCombo->setVersion(pdfOptions.Version);

	if (pdfOptions.UseRGB)
		m_optionsUi->outputModeCombo->setCurrentIndex(0);
	else if (pdfOptions.isGrayscale)
		m_optionsUi->outputModeCombo->setCurrentIndex(2);
	else
		m_optionsUi->outputModeCombo->setCurrentIndex(1);

	m_optionsUi->mirrorH->setChecked(pdfOptions.MirrorH);
	m_optionsUi->mirrorV->setChecked(pdfOptions.MirrorV);
	m_optionsUi->clipToMargins->setChecked(pdfOptions.doClip);
	m_optionsUi->convertSpots->setChecked(!pdfOptions.UseSpotColors);

	bool cmykEnabled = (m_optionsUi->outputModeCombo->currentIndex() > 0);
	bool cmykUserEnabled = m_optionsUi->enableCMYK->isChecked();

	m_optionsUi->enableCMYK->setEnabled(cmykEnabled);
	m_optionsUi->enableCMYK->setChecked(cmykEnabled && cmykUserEnabled);
	m_optionsUi->inkTable->setEnabled(cmykEnabled && cmykUserEnabled);
	m_optionsUi->displayInkCoverage->setEnabled(cmykEnabled && cmykUserEnabled);
	bool isInkCoverageEnabled = m_optionsUi->displayInkCoverage->isChecked();
	m_optionsUi->coverThresholdLabel->setEnabled(cmykEnabled && cmykUserEnabled && isInkCoverageEnabled);
	m_optionsUi->coverThresholdValue->setEnabled(cmykEnabled && cmykUserEnabled && isInkCoverageEnabled);
}

void OutputPreview_PDF::setUiOptionsToPDFOptions(PDFOptions& pdfOptions)
{
	pdfOptions.Version = m_optionsUi->pdfVersionCombo->version();
	if (!pdfOptions.Version.supportsOCGs())
		pdfOptions.useLayers = false;

	if (m_optionsUi->outputModeCombo->currentIndex() == 0)
	{
		pdfOptions.isGrayscale = false;
		pdfOptions.UseRGB = true;
		pdfOptions.UseProfiles = false;
		pdfOptions.UseProfiles2 = false;
	}
	else if (m_optionsUi->outputModeCombo->currentIndex() == 2)
	{
		pdfOptions.isGrayscale = true;
		pdfOptions.UseRGB = false;
		pdfOptions.UseProfiles = false;
		pdfOptions.UseProfiles2 = false;
	}
	else
	{
		pdfOptions.isGrayscale = false;
		pdfOptions.UseRGB = false;
		/*if (ScCore->haveCMS())
		{
			pdfOptions.UseProfiles = EmbedProfs->isChecked();
			pdfOptions.UseProfiles2 = EmbedProfs2->isChecked();
			pdfOptions.Intent = IntendS->currentIndex();
			pdfOptions.Intent2 = IntendI->currentIndex();
			pdfOptions.EmbeddedI = NoEmbedded->isChecked();
			pdfOptions.SolidProf = SolidPr->currentText();
			pdfOptions.ImageProf = ImageP->currentText();
			pdfOptions.PrintProf = PrintProfC->currentText();
		}*/
	}
	
	// Force font embedding for PDF-X/1
	if (pdfOptions.Version.isPDFX() && (pdfOptions.FontEmbedding == PDFOptions::DontEmbed))
	{
		m_fontEmbeddingWasSet = false;
		pdfOptions.EmbedList.clear();
		pdfOptions.SubsetList.clear();
		pdfOptions.FontEmbedding = PDFOptions::EmbedFonts;
		setupFontEmbedding(m_pdfOptions);
		m_fontEmbeddingWasSet = true;
	}
	
	// PDF/X-1, no profile embedding
	if (pdfOptions.Version == PDFVersion::PDF_X1a)
	{
		pdfOptions.UseProfiles = false;
		pdfOptions.UseProfiles2 = false;
	}
	// PDF/X-3 or PDF/X-4, enforcing color profiles on images
	if (pdfOptions.Version == PDFVersion::PDF_X3 || pdfOptions.Version == PDFVersion::PDF_X4)
		pdfOptions.UseProfiles2 = true;

	pdfOptions.MirrorH = m_optionsUi->mirrorH->isChecked();
	pdfOptions.MirrorV = m_optionsUi->mirrorV->isChecked();
	pdfOptions.doClip  = m_optionsUi->clipToMargins->isChecked();
	pdfOptions.UseSpotColors = !m_optionsUi->convertSpots->isChecked();
}

void OutputPreview_PDF::updateOptionsFromUI()
{
	m_pdfVersion = m_optionsUi->pdfVersionCombo->version();
	m_pdfOutputMode = m_optionsUi->outputModeCombo->currentIndex();
	m_mirrorH = m_optionsUi->mirrorH->isChecked();
	m_mirrorV = m_optionsUi->mirrorV->isChecked();
	m_clipToMargins = m_optionsUi->clipToMargins->isChecked();
	m_convertSpots = m_optionsUi->convertSpots->isChecked();

	m_currentPage = m_uiBase->pageSelector->getCurrentPage() - 1;
	m_scaleMode = m_uiBase->scaleBox->currentIndex();
	m_cmykPreviewMode = m_optionsUi->enableCMYK->isChecked();
	m_useAntialiasing = m_optionsUi->antiAliasing->isChecked();
	m_showTransparency = m_optionsUi->showTransparency->isChecked();
}

void OutputPreview_PDF::onPdfVersionChanged(int index)
{
	bool isPDFX = m_optionsUi->pdfVersionCombo->versionIsPDFX();
	m_optionsUi->outputModeCombo->setEnabled(!isPDFX);

	if (isPDFX)
	{
		m_optionsUi->outputModeCombo->setCurrentIndex(1);
		m_optionsUi->outputModeCombo->setEnabled(false);
		onPdfOutputModeChanged(1);
	}

	redisplay();
}

void OutputPreview_PDF::onPdfOutputModeChanged(int /*index*/)
{
	bool cmykEnabled = (m_optionsUi->outputModeCombo->currentIndex() > 0);
	bool cmykUserEnabled = m_optionsUi->enableCMYK->isChecked();

	m_optionsUi->enableCMYK->setEnabled(cmykEnabled);
	m_optionsUi->enableCMYK->setChecked(cmykEnabled && cmykUserEnabled);
	m_optionsUi->inkTable->setEnabled(cmykEnabled && cmykUserEnabled);
	m_optionsUi->displayInkCoverage->setEnabled(cmykEnabled && cmykUserEnabled);
	bool isInkCoverageEnabled = m_optionsUi->displayInkCoverage->isChecked();
	m_optionsUi->coverThresholdLabel->setEnabled(cmykEnabled && cmykUserEnabled && isInkCoverageEnabled);
	m_optionsUi->coverThresholdValue->setEnabled(cmykEnabled && cmykUserEnabled && isInkCoverageEnabled);

	redisplay();
}

void OutputPreview_PDF::onInkTableCellDoubleClicked(int row)
{
	if (!m_haveTiffSep)
		return;

	for (auto sepIt = m_inkVisibilities.begin(); sepIt != m_inkVisibilities.end(); ++sepIt)
		sepIt.value()->setChecked(false);
	((QCheckBox*)(m_optionsUi->inkTable->cellWidget(row, 0)))->setChecked(true);

	if (m_optionsUi->enableCMYK->isChecked())
		m_previewLabel->setPixmap(createPreview(m_currentPage, qRound(72 * m_scaleFactor)));
	m_previewLabel->resize(m_previewLabel->pixmap()->size());
}

void OutputPreview_PDF::toggleAllFromHeader()
{
	if (!m_haveTiffSep)
		return;

	for (auto sepIt = m_inkVisibilities.begin(); sepIt != m_inkVisibilities.end(); ++sepIt)
		sepIt.value()->setChecked(true);

	if (m_optionsUi->enableCMYK->isChecked())
		m_previewLabel->setPixmap(createPreview(m_currentPage, qRound(72 * m_scaleFactor)));
	m_previewLabel->resize(m_previewLabel->pixmap()->size());
}

void OutputPreview_PDF::toggleCMYK()
{
	if (!m_haveTiffSep)
		return;

	bool cmykEnabled = m_optionsUi->enableCMYK->isChecked();
	m_optionsUi->inkTable->setEnabled(cmykEnabled);
	m_optionsUi->displayInkCoverage->setEnabled(cmykEnabled);
	bool isInkCoverageEnabled = m_optionsUi->displayInkCoverage->isChecked();
	m_optionsUi->coverThresholdLabel->setEnabled(cmykEnabled && isInkCoverageEnabled);
	m_optionsUi->coverThresholdValue->setEnabled(cmykEnabled && isInkCoverageEnabled);

	redisplay();
}

void OutputPreview_PDF::toggleCMYK_Colour()
{
	if (m_haveTiffSep)
	{
		bool inkCoverageEnabled = (m_optionsUi->enableCMYK->isChecked()) && (m_optionsUi->displayInkCoverage->isChecked());
		m_optionsUi->coverThresholdLabel->setEnabled(inkCoverageEnabled);
		m_optionsUi->coverThresholdValue->setEnabled(inkCoverageEnabled);
	}
	if (m_optionsUi->enableCMYK->isChecked())
		m_previewLabel->setPixmap(createPreview(m_currentPage, qRound(72 * m_scaleFactor)));
	m_previewLabel->resize(m_previewLabel->pixmap()->size());
}

void OutputPreview_PDF::imageLoadError(QPixmap &pixmap)
{
	pixmap = QPixmap(1, 1);
	qApp->restoreOverrideCursor();
	updateOptionsFromUI();
}
