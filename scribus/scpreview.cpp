/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scpreview.h"
#include <QByteArray>
#include <QDomDocument>
#include <QImage>
#include <QList>
#include "pageitem.h"
#include "prefsmanager.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusXml.h"
#include "selection.h"

ScPreview::ScPreview()
{
}

QImage ScPreview::createPreview(QString data)
{
	PrefsManager *prefsManager = PrefsManager::instance();
	double gx, gy, gw, gh;
	ScriXmlDoc ss;
	if(ss.ReadElemHeader(data, false, &gx, &gy, &gw, &gh))
	{
		QDomDocument docu("scridoc");
		docu.setContent(data);
		QDomElement elem = docu.documentElement();
		if (elem.hasAttribute("previewData"))
		{
			QString dat = elem.attribute("previewData", "");
			QByteArray inlineImageData;
			inlineImageData.append(dat);
			inlineImageData = QByteArray::fromBase64(inlineImageData);
			QImage tmp;
			tmp.loadFromData(inlineImageData);
			return tmp;
		}
		else
		{
			ScribusDoc *m_Doc = new ScribusDoc();
			m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
			m_Doc->setPage(gw, gh, 0, 0, 0, 0, 0, 0, false, false);
			m_Doc->addPage(0);
			m_Doc->setGUI(false, ScCore->primaryMainWindow(), 0);
			m_Doc->setLoading(true);
			m_Doc->DoDrawing = false;
			if(ss.ReadElem(data, prefsManager->appPrefs.fontPrefs.AvailFonts, m_Doc, 0, 0, false, true, prefsManager->appPrefs.fontPrefs.GFontSub))
			{
				QList<PageItem*> Elements = *m_Doc->Items;
				Selection *tmpSel = new Selection(m_Doc, false);
				if (Elements.count() > 0)
				{
					if (Elements.count() > 1)
						m_Doc->groupObjectsList(Elements);
					m_Doc->DoDrawing = true;
					m_Doc->m_Selection->delaySignalsOn();
					for (int dre=0; dre<Elements.count(); ++dre)
					{
						tmpSel->addItem(Elements.at(dre), true);
					}
					tmpSel->setGroupRect();
					double xs = tmpSel->width();
					double ys = tmpSel->height();
				//	double sc = 60.0 / qMax(xs, ys);
				//	m_Doc->scaleGroup(sc, sc, true, tmpSel);
					QImage tmpImage = Elements.at(0)->DrawObj_toImage(128);
					tmpImage.setText("XSize", QString("%1").arg(xs));
					tmpImage.setText("YSize", QString("%1").arg(ys));
					m_Doc->m_Selection->delaySignalsOff();
					delete tmpSel;
					delete m_Doc;
					return tmpImage;
				}
				else
				{
					delete m_Doc;
					return QImage();
				}
			}
			else
			{
				delete m_Doc;
				return QImage();
			}
		}
	}
	else
		return QImage();
}
