/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QTemporaryFile>
#include <QTransform>

#include "iconmanager.h"
#include "printpreviewcreator_gdi.h"
#include "scpaths.h"
#include "scprintengine_gdi.h"
#include "scribusdoc.h"

PrintPreviewCreator_GDI::PrintPreviewCreator_GDI(ScribusDoc* doc) :
	PrintPreviewCreator(doc)
{
	m_printOptions.prnLanguage = PrintLanguage::WindowsGDI;

	// Generate a template name for temporary files
	QTemporaryFile *tempFile = new QTemporaryFile(ScPaths::tempFileDir() + "/scgdipreview_XXXXXX.png");
	if (tempFile->open())
	{
		QString tempFileBase = tempFile->fileName();
		tempFile->setAutoRemove(false);
		tempFile->close();
		m_tempBaseName = QFileInfo(tempFileBase).completeBaseName();
	}
	if (m_tempBaseName.isEmpty())
		m_tempBaseName = "scgdipreview";
	delete tempFile;
}

PrintPreviewCreator_GDI::~PrintPreviewCreator_GDI()
{
	cleanupTemporaryFiles();
}

void PrintPreviewCreator_GDI::cleanupTemporaryFiles()
{
	QString tempFileDir = ScPaths::tempFileDir();
	QFile::remove(tempFileDir + "/" + m_tempBaseName + ".png");
}

QPixmap PrintPreviewCreator_GDI::createPreview(int pageIndex)
{
	QPixmap pixmap;

	if (m_printOptionsChanged || m_renderingOptionsChanged || (pageIndex != m_pageIndex))
	{
		bool done = createPreviewFile(pageIndex, m_previewResolution);
		if (!done)
			return QPixmap(1, 1);
		m_printOptionsChanged = false;
		m_renderingOptionsChanged = false;
		m_pageIndex = pageIndex;
	}

	QString previewFile = ScPaths::tempFileDir() + m_tempBaseName + ".png";
	
	QImage image;
	if (!image.load(previewFile))
		return QPixmap(1, 1);
	image = image.convertToFormat(QImage::Format_ARGB32);
	if (m_showTransparency)
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

	return pixmap;
}

bool PrintPreviewCreator_GDI::createPreviewFile(int pageIndex, int res)
{
	QImage image;
	ScPrintEngine_GDI winPrint(*m_doc);
	PrintOptions options;
	ScPage* page = m_doc->Pages->at(pageIndex);
	options.copies = 1;
	options.doGCR = false;
	options.mirrorH = m_printOptions.mirrorH;
	options.mirrorV = m_printOptions.mirrorV;
	options.outputSeparations = false;
	options.pageNumbers.push_back(pageIndex);
	options.prnLanguage = PrintLanguage::WindowsGDI;
	options.separationName = "All";
	options.toFile = false;
	options.useColor = m_printOptions.useColor;
	options.useSpotColors = false;
	bool done = winPrint.gdiPrintPreview(page, &image, options, res / 72.0 * m_devicePixelRatio);
	if (done)
		image.save( ScPaths::tempFileDir() + m_tempBaseName + ".png", "PNG" );
	return done;
}

void PrintPreviewCreator_GDI::setPrintOptions(const PrintOptions& options)
{
	m_printOptions = options;
	m_printOptions.prnLanguage = PrintLanguage::WindowsGDI;
	m_printOptionsChanged = true;
}
