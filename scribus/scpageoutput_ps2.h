/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCPAGEOUTPUTPS2_H
#define SCPAGEOUTPUTPS2_H

#include <qiodevice.h>
#include <qdatastream.h>

#include "scconfig.h"
#include "scribusapi.h"
#include "scribusdoc.h"
#include "page.h"
#include "sccolor.h"
#include "scpageoutput.h"
#include "scpainterex_ps2.h"

class SCRIBUS_API ScPageOutput_Ps2 : public ScPageOutput
{
protected:

	QRect m_clip;
	QIODevice*	m_device;
	QTextStream m_stream;
	ScPs2OutputParams m_options;

	int m_pageIndex;

	virtual void initStream(QIODevice* dev);

public:

	ScPageOutput_Ps2(QIODevice* dev, ScribusDoc* doc, int pageIndex, ScPs2OutputParams& options);
	~ScPageOutput_Ps2();

	virtual void begin(void);
	virtual void DrawPage(Page* page);
	virtual void end(void);

	const QRect clip(void);
	void  setClip(const QRect& rect) { m_clip = rect; }
};

#endif
