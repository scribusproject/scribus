/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "outputpreviewbase.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "ui_outputpreviewbase.h"

OutputPreviewBase::OutputPreviewBase(QWidget* parent, ScribusDoc* doc) :
	m_doc(doc)
{
	m_uiBase = new Ui::OutputPreviewBase();
	m_uiBase->setupUi(this);

	m_previewLabel = new QLabel(this);
	m_previewLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	m_uiBase->scrollArea->setWidget(m_previewLabel);

	m_uiBase->scaleBox->setCurrentIndex(1);

	m_uiBase->pageSelector->setMaximum(m_doc->DocPages.count());
	m_uiBase->pageSelector->setGUIForPage(m_doc->currentPage()->pageNr());
}

OutputPreviewBase::~OutputPreviewBase()
{
	delete m_uiBase;
}

void OutputPreviewBase::jumpToPage(int num)
{
	int pageIndex = num - 1;
	if (pageIndex == m_currentPage)
		return;

	QPixmap previewPix = createPreview(pageIndex, qRound(72 * m_scaleFactor));
	m_previewLabel->setPixmap(previewPix);
	m_previewLabel->resize(previewPix.size());
}

void OutputPreviewBase::redisplay()
{
	QPixmap previewPix = createPreview(m_currentPage, qRound(72 * m_scaleFactor));
	m_previewLabel->setPixmap(previewPix);
	m_previewLabel->resize(previewPix.size());
}

void OutputPreviewBase::onScaleBoxValueChanged(int value)
{
	double scaleX = 1.0;
	double scaleY = 1.0;

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
			m_scaleFactor = m_uiBase->scrollArea->viewport()->width() / m_doc->Pages->at(m_currentPage)->width();
			break;
		case 5:
			m_scaleFactor = m_uiBase->scrollArea->viewport()->height() / m_doc->Pages->at(m_currentPage)->height();
			break;
		case 6:
			scaleX = m_uiBase->scrollArea->viewport()->width() / m_doc->Pages->at(m_currentPage)->width();
			scaleY = m_uiBase->scrollArea->viewport()->height() / m_doc->Pages->at(m_currentPage)->height();
			m_scaleFactor = qMin(scaleX, scaleY);
			break;
		default:
			m_scaleFactor = 1.0;
			break;
	}
	
	redisplay();
}

void OutputPreviewBase::resizeEvent(QResizeEvent * event)
{
	QDialog::resizeEvent(event);
	int scaleIndex = m_uiBase->scaleBox->currentIndex();
	// Repaint only for "fit to" options in the combo box
	if (scaleIndex <= 3)
		return;
	// HACK: m_scaleMode is reset to insane value to force redraw
	// as the value is checked for change.
	//m_scaleMode = -1;
	onScaleBoxValueChanged(scaleIndex);
}

void OutputPreviewBase::blendImages(QImage &target, ScImage &scSource, const ScColor& col)
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

void OutputPreviewBase::blendImagesSumUp(QImage &target, ScImage &scSource)
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