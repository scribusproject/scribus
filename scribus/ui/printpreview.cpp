/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          preview.cpp  -  description
                             -------------------
    begin                : Sun Jun 1 08:00:00 CEST 2003
    copyright            : (C) 2003 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "printpreview.h"

#include <QApplication>
#include <QColor>
#include <QComboBox>
#include <QCursor>
#include <QDesktopWidget>
#include <QFile>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QToolTip>

#include <cstdlib>
#include <algorithm>

#include "checkDocument.h"
#include "cmsettings.h"
#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "prefstable.h"
#include "printpreviewcreator.h"
#include "printpreviewcreatorfactory.h"
#include "pslib.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "ui/pageselector.h"
#include "util.h"
#include "util_ghostscript.h"
#include "util_printer.h"

#if defined(_WIN32)
#include "scprintengine_gdi.h"
#endif

#include "ui_printpreviewbase.h"

extern bool printDinUse;

PrintPreview::PrintPreview(QWidget* parent, ScribusDoc *docu, const QString& printer, PrintLanguage engine) : QDialog(parent),
	m_doc(docu),
	m_prefsManager(PrefsManager::instance())
{
	int inkTableWidth = 0;

	m_ui = new Ui::PrintPreviewBase();
	m_ui->setupUi(this);

	QString caption = tr("Print Preview");
	setWindowTitle(caption);

	setModal(true);
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	Q_ASSERT(!docu->masterPageMode());

	bool printToFile = (printer == CommonStrings::File || printer == CommonStrings::trFile);
	m_ui->printLanguageCombo->setupLanguages(printer, printToFile);
	m_ui->printLanguageCombo->setCurrentLanguage(engine);

	m_previewCreator = PrintPreviewCreatorFactory::create(docu, m_ui->printLanguageCombo->currentLanguage());
	m_previewCreator->setDevicePixelRatio(devicePixelRatioF());

	m_useGhostscript = m_previewCreator->isGhostscriptBased();
	m_haveTiffSep  = ScCore->haveTIFFSep() && (m_ui->printLanguageCombo->hasPDF() || m_ui->printLanguageCombo->hasPostscript());
	getNumericGSVersion(m_gsVersion);

	m_previewLabel = new QLabel(this);
	m_previewLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	m_ui->previewArea->setWidget(m_previewLabel);

	m_ui->scaleBox->setCurrentIndex(1);

	m_ui->pageSelector->setMaximum(m_doc->DocPages.count());
	m_ui->pageSelector->setGUIForPage(m_doc->currentPage()->pageNr());

	if (m_haveTiffSep)
	{
		ColorList usedSpots;
		m_doc->getUsedColors(usedSpots, true);
		QStringList spots = usedSpots.keys();

		m_ui->coverThresholdValue->setMaximum((spots.count() + 4) * 100.0);

		m_ui->inkTable->setColumnCount(2);
		m_ui->inkTable->setRowCount(4 + spots.count());
		m_ui->inkTable->setHorizontalHeaderItem(0, new QTableWidgetItem(IconManager::instance().loadIcon("16/show-object.png"), ""));
		m_ui->inkTable->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Separation Name")));

		QHeaderView *header = m_ui->inkTable->horizontalHeader();
		header->setStretchLastSection(true);
		header->setSectionsMovable(false);
		header->setSectionResizeMode(QHeaderView::Fixed);

		m_ui->inkTable->setColumnWidth(0, 24);
		m_ui->inkTable->verticalHeader()->hide();
		m_ui->inkTable->setFocusPolicy(Qt::NoFocus);
		m_ui->inkTable->setSelectionMode(QAbstractItemView::NoSelection);
		m_ui->inkTable->setSizeAdjustPolicy(QTableWidget::AdjustToContentsOnFirstShow);

		m_ui->inkTable->setItem(0, 1, new QTableWidgetItem( tr("Cyan")));
		QCheckBox *cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		cp->setChecked(m_prefsManager.appPrefs.printPreviewPrefs.PrPr_C);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		m_ui->inkTable->setCellWidget(0, 0, cp);
		m_inkVisibilities.insert("Cyan", cp);

		m_ui->inkTable->setItem(1, 1, new QTableWidgetItem( tr("Magenta")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		cp->setChecked(m_prefsManager.appPrefs.printPreviewPrefs.PrPr_M);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		m_ui->inkTable->setCellWidget(1, 0, cp);
		m_inkVisibilities.insert("Magenta", cp);

		m_ui->inkTable->setItem(2, 1, new QTableWidgetItem( tr("Yellow")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		cp->setChecked(m_prefsManager.appPrefs.printPreviewPrefs.PrPr_Y);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		m_ui->inkTable->setCellWidget(2, 0, cp);
		m_inkVisibilities.insert("Yellow", cp);

		m_ui->inkTable->setItem(3, 1, new QTableWidgetItem( tr("Black")));
		cp = new QCheckBox(this);
		cp->setFocusPolicy(Qt::NoFocus);
		cp->setChecked(m_prefsManager.appPrefs.printPreviewPrefs.PrPr_K);
		connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		m_ui->inkTable->setCellWidget(3, 0, cp);
		m_inkVisibilities.insert("Black", cp);

		for (int sp = 0; sp < spots.count(); ++sp)
		{
			const QString& spotName = spots.at(sp);
			m_ui->inkTable->setItem(sp + 4, 1, new QTableWidgetItem(spotName));
			cp = new QCheckBox(this);
			cp->setFocusPolicy(Qt::NoFocus);
			cp->setChecked(true);
			connect(cp, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
			m_ui->inkTable->setCellWidget(sp + 4, 0, cp);
			m_inkVisibilities.insert(spotName, cp);
		}

		inkTableWidth = m_ui->inkTable->columnWidth(1);

		bool inkCoverageEnabled = (m_ui->enableCMYK->isChecked()) && (m_ui->displayInkCoverage->isChecked());
		m_ui->coverThresholdLabel->setEnabled(inkCoverageEnabled);
		m_ui->coverThresholdValue->setEnabled(inkCoverageEnabled);
		m_ui->coverThresholdValue->setSuffix( tr(" %"));
		m_ui->coverThresholdValue->setSpecialValueText( tr("None"));
		
		connect(header, SIGNAL(sectionClicked(int)), this, SLOT(toggleAllFromHeader()));
		connect(m_ui->enableCMYK, SIGNAL(clicked()), this, SLOT(toggleCMYK()));
		connect(m_ui->displayInkCoverage, SIGNAL(clicked()), this, SLOT(toggleCMYK_Colour()));
		connect(m_ui->inkTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onInkTableCellDoubleClicked(int)));
	}
	else
	{
		m_ui->enableCMYK->setChecked(false);
		m_ui->enableCMYK->setVisible(false);
		m_ui->inkTable->setVisible(false);
		m_ui->displayInkCoverage->setChecked(false);
		m_ui->displayInkCoverage->setVisible(false);
		m_ui->coverThresholdLabel->setVisible(false);
		m_ui->coverThresholdValue->setVisible(false);
	}

	// Restore display settings from preferences
	m_ui->antiAliasing->setChecked(m_useGhostscript ? m_prefsManager.appPrefs.printPreviewPrefs.PrPr_AntiAliasing : false);
	m_ui->antiAliasing->setEnabled(m_useGhostscript);

	m_ui->showTransparency->setChecked(m_useGhostscript ? m_prefsManager.appPrefs.printPreviewPrefs.PrPr_Transparency : false);
	m_ui->showTransparency->setEnabled(m_useGhostscript);

	m_ui->enableCMYK->setChecked((m_haveTiffSep && m_useGhostscript) ? m_prefsManager.appPrefs.printPreviewPrefs.PrPr_Mode : false);
	m_ui->enableCMYK->setEnabled(m_haveTiffSep && m_useGhostscript);

	if (m_ui->inkTable->isVisible() && m_ui->inkTable->isEnabled())
	{
		m_inkVisibilities["Cyan"]->setChecked(m_prefsManager.appPrefs.printPreviewPrefs.PrPr_C);
		m_inkVisibilities["Magenta"]->setChecked(m_prefsManager.appPrefs.printPreviewPrefs.PrPr_M);
		m_inkVisibilities["Magenta"]->setChecked(m_prefsManager.appPrefs.printPreviewPrefs.PrPr_Y);
		m_inkVisibilities["Yellow"]->setChecked(m_prefsManager.appPrefs.printPreviewPrefs.PrPr_K);
	}

	bool cmykEnabled = m_ui->enableCMYK->isChecked();
	m_ui->inkTable->setEnabled(cmykEnabled);
	m_ui->displayInkCoverage->setEnabled(cmykEnabled);
	
	m_ui->displayInkCoverage->setChecked((m_haveTiffSep && m_useGhostscript) ? m_prefsManager.appPrefs.printPreviewPrefs.PrPr_InkCoverage : false);
	m_ui->displayInkCoverage->setEnabled((m_haveTiffSep && m_useGhostscript));

	bool isInkCoverageEnabled = m_ui->displayInkCoverage->isChecked();
	m_ui->coverThresholdLabel->setEnabled(cmykEnabled && isInkCoverageEnabled);
	m_ui->coverThresholdValue->setEnabled(cmykEnabled && isInkCoverageEnabled);
	m_ui->coverThresholdValue->setValue(m_prefsManager.appPrefs.printPreviewPrefs.PrPr_InkThreshold);

	m_ui->printButton->setEnabled(!printDinUse);

	setValues();

	// Display preview
	QPixmap previewPix = createPreview(m_doc->currentPageNumber(), qRound(72 * m_scaleFactor));
	m_previewLabel->setPixmap(previewPix);
	m_previewLabel->resize(previewPix.size());

	m_ui->pageSelector->setGUIForPage(m_doc->currentPage()->pageNr());

	int w = m_previewLabel->width() + inkTableWidth + 50;
	resize(qMin(QApplication::desktop()->width() - 30, w), 500);

	// tooltips
	m_ui->antiAliasing->setToolTip( "<qt>" + tr( "Provides a more pleasant view of Type 1 fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing" ) + "</qt>" );
	m_ui->showTransparency->setToolTip( "<qt>" + tr( "Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later." ) + "</qt>");
	m_ui->enableCMYK->setToolTip( "<qt>" + tr( "Gives a print preview using simulations of generic CMYK inks, instead of RGB colors" ) + "</qt>");
	m_ui->enableGCR->setToolTip( "<qt>" + tr( "A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks." ) + "</qt>" );
	m_ui->scaleBox->setToolTip( "<qt>" + tr("Resize the scale of the page") + "</qt>");
	m_ui->convertSpots->setToolTip("<qt>" + tr( "Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled." ) + "</qt>");

	//signals and slots
	connect(m_ui->pageSelector, SIGNAL(pageChanged(int)), this, SLOT(jumpToPage(int)));
	connect(m_ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(m_ui->printButton, SIGNAL(clicked()), this, SIGNAL(doPrint()));
	connect(m_ui->scaleBox, SIGNAL(activated(int)), this, SLOT(scaleBox_valueChanged(int)));

	connect(m_ui->antiAliasing, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(m_ui->showTransparency, SIGNAL(clicked()), this, SLOT(redisplay()));

	connect(m_ui->printLanguageCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onPrintLanguageChange(int)));
	connect(m_ui->colorOutputMode, SIGNAL(activated(int)), this, SLOT(redisplay()));
	connect(m_ui->mirrorH, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(m_ui->mirrorV, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(m_ui->clipToMargins, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(m_ui->enableGCR, SIGNAL(clicked()), this, SLOT(redisplay()));
	connect(m_ui->convertSpots, SIGNAL(clicked()), this, SLOT(redisplay()));

	connect(m_ui->coverThresholdValue, SIGNAL(valueChanged(double)), this, SLOT(toggleCMYK_Colour()));
}

PrintPreview::~PrintPreview()
{
	// Cleanup temporary files
	QString tempFileDir = ScPaths::tempFileDir();
	QFile::remove(tempFileDir + "/tmp.ps");
	QFile::remove(tempFileDir + "/sc.png");
	QDir d(tempFileDir + "/", "sc.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint i = 0; i < d.count(); i++)
			QFile::remove(tempFileDir + "/" + d[i]);
	}

	if (m_previewCreator)
	{
		delete m_previewCreator;
		m_previewCreator = nullptr;
	}

	delete m_ui;
}

bool PrintPreview::isCMYKPreviewEnabled() const
{
	return m_ui->enableCMYK->isChecked();
}

bool PrintPreview::isAntialiasingEnabled() const
{
	return m_ui->antiAliasing->isChecked();
}

bool PrintPreview::isTransparencyEnabled() const
{
	return m_ui->showTransparency->isChecked();
}

bool PrintPreview::isInkCoverageEnabled() const
{
	return m_ui->displayInkCoverage->isChecked();
}

bool PrintPreview::useGhostscriptPreview() const
{
	return m_useGhostscript;
}

double PrintPreview::inkCoverageThreshold() const
{
	return m_ui->coverThresholdValue->value();
}

void PrintPreview::setValues()
{
	if ((printDinUse) || (!m_doc->Print_Options.firstUse))
	{
		m_ui->enableGCR->setChecked(m_useGhostscript ? m_doc->Print_Options.doGCR : false);
		m_ui->enableGCR->setEnabled(m_useGhostscript );
		m_ui->mirrorH->setChecked(m_doc->Print_Options.mirrorH);
		m_ui->mirrorV->setChecked(m_doc->Print_Options.mirrorV);
		m_ui->clipToMargins->setChecked(m_doc->Print_Options.doClip);
		m_ui->convertSpots->setChecked(!m_doc->Print_Options.useSpotColors);
		m_ui->colorOutputMode->setCurrentIndex(m_doc->Print_Options.useColor ? 0 : 1);
	}
	else
	{
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getContext("print_options");
		m_ui->enableGCR->setChecked(m_useGhostscript ? prefs->getBool("DoGCR", false) : false);
		m_ui->enableGCR->setEnabled(m_useGhostscript);
		m_ui->mirrorH->setChecked(prefs->getBool("MirrorH", false));
		m_ui->mirrorV->setChecked(prefs->getBool("MirrorV", false));
		m_ui->clipToMargins->setChecked(prefs->getBool("Clip", false));
		m_ui->convertSpots->setChecked(!prefs->getBool("doSpot", true));
		m_ui->colorOutputMode->setCurrentIndex((prefs->getInt("PrintColor", 0) == 1) ? 1 : 0);
	}
}

void PrintPreview::jumpToPage(int num)
{
	int n = num-1;
	if (n == m_currentPage)
		return;
	QPixmap previewPix = createPreview(n, qRound(72 * m_scaleFactor));
	m_previewLabel->setPixmap(previewPix);
	m_previewLabel->resize(previewPix.size());
}

void PrintPreview::redisplay()
{
	QPixmap previewPix = createPreview(m_currentPage, qRound(72 * m_scaleFactor));
	m_previewLabel->setPixmap(previewPix);
	m_previewLabel->resize(previewPix.size());
}

void PrintPreview::toggleCMYK()
{
	if (!m_haveTiffSep)
		return;

	bool c = m_ui->enableCMYK->isChecked();
	m_ui->inkTable->setEnabled(c);
	m_ui->displayInkCoverage->setEnabled(c);
	if (m_ui->displayInkCoverage->isChecked())
	{
		m_ui->coverThresholdLabel->setEnabled(c);
		m_ui->coverThresholdValue->setEnabled(c);
	}
	redisplay();
}

void PrintPreview::toggleCMYK_Colour()
{
	if (m_haveTiffSep)
	{
		if ((m_ui->enableCMYK->isChecked()) && (m_ui->displayInkCoverage->isChecked()))
		{
			m_ui->coverThresholdLabel->setEnabled(true);
			m_ui->coverThresholdValue->setEnabled(true);
		}
		else
		{
			m_ui->coverThresholdLabel->setEnabled(false);
			m_ui->coverThresholdValue->setEnabled(false);
		}
	}
	if (m_ui->enableCMYK->isChecked())
		m_previewLabel->setPixmap(createPreview(m_currentPage, qRound(72 * m_scaleFactor)));
	m_previewLabel->resize(m_previewLabel->pixmap()->size());
}

void PrintPreview::onInkTableCellDoubleClicked(int row)
{
	if (!m_haveTiffSep)
		return;

	for (auto sepIt = m_inkVisibilities.begin(); sepIt != m_inkVisibilities.end(); ++sepIt)
		sepIt.value()->setChecked(false);
	((QCheckBox*)(m_ui->inkTable->cellWidget(row, 0)))->setChecked(true);

	if (m_ui->enableCMYK->isChecked())
		m_previewLabel->setPixmap(createPreview(m_currentPage, qRound(72 * m_scaleFactor)));
	m_previewLabel->resize(m_previewLabel->pixmap()->size());
}

void PrintPreview::onPrintLanguageChange(int /*langIndex*/)
{
	if (m_previewCreator)
	{
		delete m_previewCreator;
		m_previewCreator = nullptr;
	}

	PrintLanguage newPrnLanguage = m_ui->printLanguageCombo->currentLanguage();
	m_previewCreator = PrintPreviewCreatorFactory::create(m_doc, newPrnLanguage);
	m_useGhostscript = m_previewCreator->isGhostscriptBased();

	m_previewCreator->setDevicePixelRatio(devicePixelRatioF());

	m_ui->antiAliasing->setEnabled(m_useGhostscript);
	m_ui->showTransparency->setEnabled(m_useGhostscript);

	m_ui->enableCMYK->setEnabled(m_haveTiffSep && m_useGhostscript);
	if (!m_ui->enableCMYK->isEnabled())
		m_ui->enableCMYK->setChecked(false);

	bool cmykEnabled = m_ui->enableCMYK->isChecked();
	m_ui->inkTable->setEnabled(cmykEnabled);
	m_ui->displayInkCoverage->setEnabled(cmykEnabled);
	
	m_ui->displayInkCoverage->setEnabled((m_haveTiffSep && m_useGhostscript));
	if (!m_ui->displayInkCoverage->isEnabled())
		m_ui->displayInkCoverage->setChecked(false);

	bool isInkCoverageEnabled = m_ui->displayInkCoverage->isChecked();
	m_ui->coverThresholdLabel->setEnabled(cmykEnabled && isInkCoverageEnabled);
	m_ui->coverThresholdValue->setEnabled(cmykEnabled && isInkCoverageEnabled);

	m_currentPage = -1;

	QPixmap previewPix = createPreview(m_doc->currentPageNumber(), qRound(72 * m_scaleFactor));
	m_previewLabel->setPixmap(previewPix);
	m_previewLabel->resize(previewPix.size());
}

void PrintPreview::toggleAllFromHeader()
{
	if (!m_haveTiffSep)
		return;

	for (auto sepIt = m_inkVisibilities.begin(); sepIt != m_inkVisibilities.end(); ++sepIt)
		sepIt.value()->setChecked(true);

	if (m_ui->enableCMYK->isChecked())
		m_previewLabel->setPixmap(createPreview(m_currentPage, qRound(72 * m_scaleFactor)));
	m_previewLabel->resize(m_previewLabel->pixmap()->size());
}

void PrintPreview::scaleBox_valueChanged(int value)
{
	switch (value)
	{
		case 0:
			m_scaleFactor = 0.5;
			break;
		case 1:
			m_scaleFactor = 1.0;
			break;
		case 2:
			m_scaleFactor = 1.5;
			break;
		case 3:
			m_scaleFactor = 2.0;
			break;
		case 4:
			m_scaleFactor = m_ui->previewArea->viewport()->width() / m_doc->Pages->at(m_currentPage)->width();
			break;
		case 5:
			m_scaleFactor = m_ui->previewArea->viewport()->height() / m_doc->Pages->at(m_currentPage)->height();
			break;
		case 6:
			m_scaleFactor = qMin(m_ui->previewArea->viewport()->height() / m_doc->Pages->at(m_currentPage)->height(), m_ui->previewArea->viewport()->width() / m_doc->Pages->at(m_currentPage)->width());
			break;
		default:
			m_scaleFactor = 1.0;
			break;
	}
	redisplay();
}

QPixmap PrintPreview::createPreview(int pageIndex, int res)
{
	QPixmap pixmap;

	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));

	// Recreate Postscript-File only when necessary
	if (printOptionsHaveChanged(pageIndex))
	{
		PrintOptions options;
		options.pageNumbers.push_back(pageIndex + 1);
		options.outputSeparations = false;
		options.separationName = "All";
		options.allSeparations = QStringList();
		options.useSpotColors = !m_ui->convertSpots->isChecked();
		options.useColor = (m_ui->colorOutputMode->currentIndex() == 0);
		options.mirrorH = m_ui->mirrorH->isChecked();
		options.mirrorV = m_ui->mirrorV->isChecked();
		options.doGCR = m_ui->enableGCR->isChecked();
		options.setDevParam = false;
		options.doClip = m_ui->clipToMargins->isChecked();
		options.cropMarks = false;
		options.bleedMarks = false;
		options.registrationMarks = false;
		options.colorMarks = false;
		options.includePDFMarks = false;
		options.markLength = 20.0;
		options.markOffset = 0.0;
		options.bleeds.set(0, 0, 0, 0);
		m_previewCreator->setPrintOptions(options);
	}

	m_previewCreator->setDevicePixelRatio(devicePixelRatioF());
	m_previewCreator->setPreviewResolution(res);
	m_previewCreator->setAntialisingEnabled(m_ui->antiAliasing->isChecked());
	m_previewCreator->setShowTransparency(m_ui->showTransparency->isChecked());
	
	SeparationPreviewCreator* sepPreviewCreator = dynamic_cast<SeparationPreviewCreator*>(m_previewCreator);
	if (sepPreviewCreator)
	{
		sepPreviewCreator->setSeparationPreviewEnabled(m_ui->enableCMYK->isChecked());
		sepPreviewCreator->setShowInkCoverage(m_ui->displayInkCoverage->isChecked());
		sepPreviewCreator->setInkCoverageThreshold(m_ui->coverThresholdValue->value());

		QStringList separationNames = sepPreviewCreator->separationNames();
		for (int i = 0; i < separationNames.count(); ++i)
		{
			const QString& separationName = separationNames.at(i);
			const QCheckBox* checkBox = m_inkVisibilities.value(separationName, nullptr);
			if (checkBox)
				sepPreviewCreator->setSeparationVisible(separationName, checkBox->isChecked());
		}
	}

	pixmap = m_previewCreator->createPreview(pageIndex);

	qApp->restoreOverrideCursor();
	getUserSelection(pageIndex);
	return pixmap;
}

//-------------------------------------------------------------------------------------------------

bool PrintPreview::usesGhostscript(const QString& printerName, PrintLanguage engine)
{
#ifdef _WIN32
	if (printerName == tr("File"))
		return true;
	else if (printerName.isEmpty())
		return PrinterUtil::isPostscriptPrinter( ScPrintEngine_GDI::getDefaultPrinter() );
	else if (engine >= PrintLanguage::PostScript1 && engine <= PrintLanguage::PostScript3)
		return PrinterUtil::isPostscriptPrinter( printerName );
	return false;
#else
	return true;
#endif
}

//-------------------------------------------------------------------------------------------------

bool PrintPreview::printOptionsHaveChanged(int pageIndex) const
{
	if (pageIndex != m_currentPage)
		return true;

	if (m_colorOutputMode != m_ui->colorOutputMode->currentIndex())
		return true;
	if (m_ui->mirrorH->isChecked() != m_mirrorH)
		return true;
	if (m_ui->mirrorV->isChecked() != m_mirrorV)
		return true;
	if (m_ui->clipToMargins->isChecked() != m_clipToMargins)
		return true;
	if (m_ui->enableGCR->isChecked() != m_useGCR)
		return true;
	if (m_ui->convertSpots->isChecked() != m_convertSpots)
		return true;

	return false;
}

void PrintPreview::getUserSelection(int page)
{
	m_currentPage = page;
	m_colorOutputMode = m_ui->colorOutputMode->currentIndex();
	m_colorMode = m_ui->enableCMYK->isChecked();
	m_useAntialiasing = m_ui->antiAliasing->isChecked();
	m_showTransparency = m_ui->showTransparency->isChecked();
	m_useGCR = m_ui->enableGCR->isChecked();
	m_scaleMode = m_ui->scaleBox->currentIndex();
	m_mirrorH = m_ui->mirrorH->isChecked();
	m_mirrorV = m_ui->mirrorV->isChecked();
	m_clipToMargins = m_ui->clipToMargins->isChecked();
	m_convertSpots = m_ui->convertSpots->isChecked();
}

void PrintPreview::resizeEvent(QResizeEvent * event)
{
	QDialog::resizeEvent(event);
	int scaleIndex = m_ui->scaleBox->currentIndex();
	// Repaint only for "fit to" options in the combo box
	if (scaleIndex <= 3)
		return;
	// HACK: m_scaleMode is reset to insane value to force redraw
	// as the value is checked for change.
	m_scaleMode = -1;
	scaleBox_valueChanged(scaleIndex);
}
