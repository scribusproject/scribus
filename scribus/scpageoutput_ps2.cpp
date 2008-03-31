/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QPainter>

#include "commonstrings.h"
#include "fonts/scfontmetrics.h"
#include "pageitem.h"
#include "scimage.h"
#include "scpageoutput_ps2.h"
#include "scpainterex_ps2.h"
#include "scribus.h"
#include "util.h"

ScPageOutput_Ps2::ScPageOutput_Ps2(QIODevice* dev, ScribusDoc* doc, int pageIndex, ScPs2OutputParams& options)
				: ScPageOutput(doc, options.reloadImages, options.resolution, options.useProfiles)
{
	m_device = dev;
	m_pageIndex = pageIndex;
	m_options = options;
	m_options.document = doc;
	initStream(dev);
}

ScPageOutput_Ps2::~ScPageOutput_Ps2()
{
}

void ScPageOutput_Ps2::initStream(QIODevice* dev)
{
	m_stream.setDevice(m_device);
}

void ScPageOutput_Ps2::begin(void)
{
	m_stream << QString("%%Page: %1 %2\n").arg(m_pageIndex).arg(m_pageIndex);
	m_stream << QString("/saveobj save def \n");
	if (m_clip.width() > 0 && m_clip.height() > 0)
	{
		m_stream << QString("%1 %2 moveto\n").arg(m_clip.left()).arg(m_clip.bottom());
		m_stream << QString("%1 %2 lineto\n").arg(m_clip.right()).arg(m_clip.bottom());
		m_stream << QString("%1 %2 lineto\n").arg(m_clip.right()).arg(m_clip.top());
		m_stream << QString("%1 %2 lineto closepath clip newpath\n").arg(m_clip.left()).arg(m_clip.top());
	}
}

void ScPageOutput_Ps2::drawPage(Page* page)
{
	// Get page position
	int clipx = static_cast<int>(page->xOffset());
	int clipy = static_cast<int>(page->yOffset());
	int clipw = qRound(page->width());
	int cliph = qRound(page->height());
	// Create painter
	ScPainterEx_Ps2 painter(m_device, m_clip, m_options);
	// Set transformation matrix
	double scalex = m_options.mirrorH ? -1.0 :  1.0;
	double scaley = m_options.mirrorV ?  1.0 : -1.0;
	double dx = -clipx * scalex;
	double dy = -clipy * scaley;
	if (  m_options.mirrorH ) dx += clipw;
	if ( !m_options.mirrorV ) dy += cliph;
	QMatrix matrix( scalex, 0.0, 0.0, scaley, dx, dy );
	painter.setWorldMatrix( matrix );
	// Draw page
	ScPageOutput::drawPage(page, &painter);
}

void ScPageOutput_Ps2::end(void)
{
	m_stream << "%%PageTrailer\n";
	m_stream << "saveobj restore\n";
	m_stream << "showpage\n";
}
