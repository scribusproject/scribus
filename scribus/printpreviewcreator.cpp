/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "printpreviewcreator.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "util_ghostscript.h"
#include "util_printer.h"

PrintPreviewCreator::PrintPreviewCreator(ScribusDoc* doc) :
	m_doc(doc),
	m_printOptions(doc->Print_Options)
{
	// Setup default print options
	if (m_printOptions.firstUse)
		PrinterUtil::getDefaultPrintOptions(m_printOptions, m_doc->bleedsVal());
	m_printOptions.outputSeparations = false;
	m_printOptions.separationName = "All";
	m_printOptions.allSeparations = QStringList();
	m_printOptions.setDevParam = false;
	m_printOptions.cropMarks = false;
	m_printOptions.bleedMarks = false;
	m_printOptions.registrationMarks = false;
	m_printOptions.colorMarks = false;
	m_printOptions.markLength = 20.0;
	m_printOptions.markOffset = 0.0;
	m_printOptions.bleeds.set(0, 0, 0, 0);
}

void PrintPreviewCreator::setAntialisingEnabled(bool enabled)
{
	if (m_useAntialiasing == enabled)
		return;
	m_useAntialiasing = enabled;
	m_renderingOptionsChanged = true;
}

void PrintPreviewCreator::setDevicePixelRatio(double ratio)
{
	if (m_devicePixelRatio == ratio)
		return;
	m_devicePixelRatio = ratio;
	m_renderingOptionsChanged = true;
}

void PrintPreviewCreator::setPreviewResolution(int res)
{
	if (m_previewResolution == res)
		return;
	m_previewResolution = res;
	m_renderingOptionsChanged = true;
}

void PrintPreviewCreator::setPrintOptions(const PrintOptions& options)
{
	m_printOptions = options;
	m_printOptionsChanged = true;
}

void PrintPreviewCreator::setShowTransparency(bool showTrans)
{
	if (m_showTransparency == showTrans)
		return;
	m_showTransparency = showTrans;
	m_renderingOptionsChanged = true;
}

SeparationPreviewCreator::SeparationPreviewCreator(ScribusDoc* doc) :
	PrintPreviewCreator(doc)
{
	m_havePngAlpha = ScCore->havePNGAlpha();
	m_haveTiffSep  = ScCore->haveTIFFSep();
	getNumericGSVersion(m_gsVersion);

	// Populate default separation list
	m_separationVisibilities["Cyan"] = true;
	m_separationVisibilities["Magenta"] = true;
	m_separationVisibilities["Yellow"] = true;
	m_separationVisibilities["Black"] = true;

	if (m_haveTiffSep)
	{
		ColorList usedSpots;
		doc->getUsedColors(usedSpots, true);
		QStringList spots = usedSpots.keys();

		for (int i = 0; i < spots.count(); ++i)
		{
			QString spotName = spots.at(i);
			m_separationVisibilities[spotName] = true;
		}

		m_inkMax = (4 + usedSpots.count()) * 255;
		m_spotColorCount = usedSpots.count();
	}
}

void SeparationPreviewCreator::setSeparationPreviewEnabled(bool enabled)
{
	if (m_sepPreviewEnabled == enabled)
		return;
	m_sepPreviewEnabled = enabled;
	m_renderingOptionsChanged = true;
}

void SeparationPreviewCreator::setSeparationVisible(const QString& name, bool visible)
{
	if (m_separationVisibilities.contains(name) && (m_separationVisibilities[name] == visible))
		return;
	m_separationVisibilities[name] = visible;
}

void SeparationPreviewCreator::setShowInkCoverage(bool show)
{
	if (m_showInkCoverage == show)
		return;
	m_showInkCoverage = show;
	m_renderingOptionsChanged = true;
}

void SeparationPreviewCreator::setInkCoverageThreshold(double value)
{
	if (m_inkCoverageThreshold == value)
		return;
	m_inkCoverageThreshold = value;
	m_renderingOptionsChanged = true;
}

void SeparationPreviewCreator::blendImages(QImage &target, ScImage &scSource, const ScColor& col)
{
	QImage source = scSource.qImage(); // FIXME: this will not work once qImage always returns ARGB!
	
	//FIXME: if source and target have different size something went wrong.
	// eg. loadPicture() failed and returned a 1x1 image
	CMYKColor cmykValues;
	int w = qMin(target.width(), source.width());
	int h = qMin(target.height(), source.height());
	int cyan, c, m, yc, k, cc, mm, yy, kk;
	ScColorEngine::getCMYKValues(col, m_doc, cmykValues);
	cmykValues.getValues(c, m, yc, k);
	for (int y = 0; y < h; ++y )
	{
		QRgb *p = (QRgb *) target.scanLine(y);
		QRgb *pq = (QRgb *) source.scanLine(y);
		for (int x = 0; x < w; ++x )
		{
			cyan = 255 - qRed(*pq);
			if (cyan != 0)
			{
				(c == 0) ? cc = qRed(*p) : cc = qMin(c * cyan / 255 + qRed(*p), 255);
				(m == 0) ? mm = qGreen(*p) : mm = qMin(m * cyan / 255 + qGreen(*p), 255);
				(yc == 0) ? yy = qBlue(*p) : yy = qMin(yc * cyan / 255 + qBlue(*p), 255);
				(k == 0) ? kk = qAlpha(*p) : kk = qMin(k * cyan / 255 + qAlpha(*p), 255);
				*p = qRgba(cc, mm, yy, kk);
			}
			p++;
			pq++;
		}
	}
}

void SeparationPreviewCreator::blendImagesSumUp(QImage &target, ScImage &scSource)
{
	 // FIXME: this will not work once qImage always returns ARGB!
	QImage source = scSource.qImage();

	//FIXME: if source and target have different sizesomething went wrong.
	// eg. loadPicture() failed and returned a 1x1 image
	int w = qMin(target.width(), source.width());
	int h = qMin(target.height(), source.height());
	int cyan;
	for (int y = 0; y < h; ++y )
	{
		uint *p = (QRgb *) target.scanLine(y);
		QRgb *pq = (QRgb *) source.scanLine(y);
		for (int x = 0; x < w; ++x )
		{
			cyan = 255 - qRed(*pq);
			*p += cyan;
			p++;
			pq++;
		}
	}
}