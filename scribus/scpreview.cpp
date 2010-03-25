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
				if (Elements.count() > 0)
				{
					Selection *tmpSel = new Selection(0, false);
					bool isGroup = true;
					int firstElem = -1;
					if (Elements.at(0)->Groups.count() != 0)
						firstElem = Elements.at(0)->Groups.top();
					for (int bx = 0; bx < Elements.count(); ++bx)
					{
						PageItem* bxi = Elements.at(bx);
						if (bxi->Groups.count() != 0)
						{
							if (bxi->Groups.top() != firstElem)
								isGroup = false;
						}
						else
							isGroup = false;
					}
					if (!isGroup)
					{
						double minx = 99999.9;
						double miny = 99999.9;
						double maxx = -99999.9;
						double maxy = -99999.9;
						uint lowestItem = 999999;
						uint highestItem = 0;
						for (int a = 0; a < Elements.count(); ++a)
						{
							Elements.at(a)->Groups.push(m_Doc->GroupCounter);
							PageItem* currItem = Elements.at(a);
							lowestItem = qMin(lowestItem, currItem->ItemNr);
							highestItem = qMax(highestItem, currItem->ItemNr);
							double x1, x2, y1, y2;
							currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
							minx = qMin(minx, x1);
							miny = qMin(miny, y1);
							maxx = qMax(maxx, x2);
							maxy = qMax(maxy, y2);
						}
						double gx = minx;
						double gy = miny;
						double gw = maxx - minx;
						double gh = maxy - miny;
						PageItem *high = m_Doc->Items->at(highestItem);
						int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Rectangle, gx, gy, gw, gh, 0, m_Doc->itemToolPrefs().shapeFillColor, m_Doc->itemToolPrefs().shapeLineColor, true);
						PageItem *neu = m_Doc->Items->takeAt(z);
						m_Doc->Items->insert(lowestItem, neu);
						neu->Groups.push(m_Doc->GroupCounter);
						neu->setItemName( QObject::tr("Group%1").arg(neu->Groups.top()));
						neu->AutoName = false;
						neu->isGroupControl = true;
						neu->groupsLastItem = high;
						neu->setTextFlowMode(PageItem::TextFlowDisabled);
						for (int a = 0; a < m_Doc->Items->count(); ++a)
						{
							m_Doc->Items->at(a)->ItemNr = a;
						}
						Elements.prepend(neu);
						m_Doc->GroupCounter++;
					}
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
					QImage tmpImage = Elements.at(0)->DrawObj_toImage();
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
