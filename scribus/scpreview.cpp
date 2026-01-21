/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <memory>

#include <QByteArray>
#include <QDomDocument>
#include <QImage>
#include <QList>

#include "scpreview.h"

#include "pageitem.h"
#include "prefsmanager.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusXml.h"
#include "selection.h"

QImage ScPreview::create(const QString& data)
{
	ScPreview preview;
	QImage im = preview.createPreview(data);
	return im;
}

QImage ScPreview::createPreview(const QString& data)
{
//	PrefsManager& prefsManager = PrefsManager::instance();
	double gx, gy, gw, gh;
	ScriXmlDoc ss;
	if (!ss.readElemHeader(data, false, &gx, &gy, &gw, &gh))
		return QImage();

	QDomDocument docu("scridoc");
	docu.setContent(data);
	QDomElement elem = docu.documentElement();
	if (elem.hasAttribute("previewData"))
	{
		QString dat = elem.attribute("previewData", "");
		QByteArray inlineImageData;
		inlineImageData.append(dat.toUtf8());
		inlineImageData = QByteArray::fromBase64(inlineImageData);
		QImage tmp;
		tmp.loadFromData(inlineImageData);
		return tmp;
	}

	auto m_Doc = std::make_shared<ScribusDoc>();
	m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
	m_Doc->setPage(gw, gh, 0, 0, 0, 0, 0, 0, false, false);
	m_Doc->addPage(0);
	m_Doc->setGUI(false, ScCore->primaryMainWindow(), nullptr);
	m_Doc->setLoading(true);
	m_Doc->DoDrawing = false;
	if (!ss.readElem(data, m_Doc.get(), 0, 0, false, true))
		return QImage();

	QList<PageItem*> elements = *m_Doc->Items;
	if (elements.isEmpty())
		return QImage();

	auto tmpSel = std::make_shared<Selection>(m_Doc.get(), false);
	if (elements.count() > 1)
		m_Doc->groupObjectsList(elements);
	m_Doc->DoDrawing = true;
	m_Doc->m_Selection->delaySignalsOn();
	tmpSel->addItems(elements);
	tmpSel->setGroupRect();
	double xs = tmpSel->width();
	double ys = tmpSel->height();
	QImage tmpImage = elements.at(0)->DrawObj_toImage(128);
	tmpImage.setText("XSize", QString("%1").arg(xs));
	tmpImage.setText("YSize", QString("%1").arg(ys));
	m_Doc->m_Selection->delaySignalsOff();

	return tmpImage;
}
