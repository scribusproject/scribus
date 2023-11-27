/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          svgexplugin.cpp  -  description
                             -------------------
    begin                : Sun Aug 3 08:00:00 CEST 2002
    copyright            : (C) 2002 by Franz Schmid
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

#include <QBuffer>
#include <QByteArray>
#include <QCheckBox>
#include <QDataStream>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QScopedPointer>
#include <QTextStream>

#include "svgexplugin.h"

#include "scconfig.h"
#include "canvas.h"
#include "cmsettings.h"
#include "commonstrings.h"
#include "pageitem_table.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "qtiocompressor.h"
#include "scpage.h"
#include "scpattern.h"
#include "scribuscore.h"
#include "sctextstruct.h"
#include "tableutils.h"
#include "util.h"
#include "ui/customfdialog.h"
#include "ui/guidemanager.h"
#include "ui/scmessagebox.h"
#include "sccolorengine.h"
#include "util_formats.h"
#include "util_math.h"
#include "text/textlayout.h"
#include "text/textlayoutpainter.h"
#include "text/boxes.h"

int svgexplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* svgexplugin_getPlugin()
{
	SVGExportPlugin* plug = new SVGExportPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void svgexplugin_freePlugin(ScPlugin* plugin)
{
	SVGExportPlugin* plug = qobject_cast<SVGExportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

using namespace TableUtils;

SVGExportPlugin::SVGExportPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

SVGExportPlugin::~SVGExportPlugin() {};

void SVGExportPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "ExportAsSVG";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Save as &SVG...");
	// Menu
	m_actionInfo.menu = "FileExport";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.exportPlugin = true;
	m_actionInfo.needsNumObjects = -1;
}

QString SVGExportPlugin::fullTrName() const
{
	return QObject::tr("SVG Export");
}

const ScActionPlugin::AboutData* SVGExportPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Exports SVG Files");
	about->description = tr("Exports the current page into an SVG file.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void SVGExportPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool SVGExportPlugin::run(ScribusDoc* doc, const QString& filename)
{
	Q_ASSERT(filename.isEmpty());
	QString fileName;
	if (doc != nullptr)
	{
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("svgex");
		QString wdir = prefs->get("wdir", ".");
		QScopedPointer<CustomFDialog> openDia( new CustomFDialog(doc->scMW(), wdir, QObject::tr("Save as"), QObject::tr("%1;;All Files (*)").arg(FormatsManager::instance()->extensionsForFormat(FormatsManager::SVG)), fdHidePreviewCheckBox) );
		openDia->setSelection(getFileNameByPage(doc, doc->currentPage()->pageNr(), "svg"));
		openDia->setExtension("svg");
		openDia->setZipExtension("svgz");
		QCheckBox* compress = new QCheckBox(openDia.data());
		compress->setText( tr("Compress File"));
		compress->setChecked(false);
		openDia->addWidgets(compress);
		QCheckBox* inlineImages = new QCheckBox(openDia.data());
		inlineImages->setText( tr("Save Images inline"));
		inlineImages->setToolTip( tr("Adds all Images on the Page inline to the SVG.\nCaution: this will increase the file size!"));
		inlineImages->setChecked(true);
		openDia->addWidgets(inlineImages);
		QCheckBox* exportBack = new QCheckBox(openDia.data());
		exportBack->setText( tr("Export Page background"));
		exportBack->setToolTip( tr("Adds the Page itself as background to the SVG"));
		exportBack->setChecked(false);
		openDia->addWidgets(exportBack);

		if (!openDia->exec())
			return true;
		fileName = openDia->selectedFile();
		QFileInfo fi(fileName);
		QString m_baseDir = fi.absolutePath();
		if (compress->isChecked())
			fileName = m_baseDir + "/" + fi.baseName() + ".svgz";
		else
			fileName = m_baseDir + "/" + fi.baseName() + ".svg";

		SVGOptions Options;
		Options.inlineImages = inlineImages->isChecked();
		Options.exportPageBackground = exportBack->isChecked();
		Options.compressFile = compress->isChecked();

		if (fileName.isEmpty())
			return true;
		prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
		QFile f(fileName);
		if (f.exists())
		{
			int exit = ScMessageBox::warning(doc->scMW(), CommonStrings::trWarning,
				QObject::tr("Do you really want to overwrite the file:\n%1 ?").arg(fileName),
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::NoButton,	// GUI default
				QMessageBox::Yes);	// batch default
			if (exit == QMessageBox::No)
				return true;
		}
		SVGExPlug *dia = new SVGExPlug(doc);
		dia->doExport(fileName, Options);
		delete dia;
	}
	return true;
}

SVGExPlug::SVGExPlug( ScribusDoc* doc )
{
	m_Doc = doc;
	Options.inlineImages = true;
	Options.exportPageBackground = false;
	Options.compressFile = false;
	m_glyphNames.clear();
}

bool SVGExPlug::doExport( const QString& fName, SVGOptions &Opts )
{
	Options = Opts;
	QFileInfo fiBase(fName);

	m_baseDir = fiBase.absolutePath();
	m_gradCount = 0;
	m_clipCount = 0;
	m_pattCount = 0;
	m_maskCount = 0;
	m_filterCount = 0;

	m_domDoc = QDomDocument("svgdoc");
	QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	QString st = "<svg></svg>";
	m_domDoc.setContent(st);

	ScPage *page = m_Doc->currentPage();
	double pageWidth  = page->width();
	double pageHeight = page->height();
	m_domElem = m_domDoc.documentElement();
	m_domElem.setAttribute("width", FToStr(pageWidth) + "pt");
	m_domElem.setAttribute("height", FToStr(pageHeight) + "pt");
	m_domElem.setAttribute("viewBox", QString("0 0 %1 %2").arg(pageWidth).arg(pageHeight));
	m_domElem.setAttribute("xmlns", "http://www.w3.org/2000/svg");
	m_domElem.setAttribute("xmlns:inkscape","http://www.inkscape.org/namespaces/inkscape");
	m_domElem.setAttribute("xmlns:xlink","http://www.w3.org/1999/xlink");
	m_domElem.setAttribute("version","1.1");
	if (!m_Doc->documentInfo().title().isEmpty())
	{
		QDomText title = m_domDoc.createTextNode(m_Doc->documentInfo().title());
		QDomElement titleElem = m_domDoc.createElement("title");
		titleElem.appendChild(title);
		m_domElem.appendChild(titleElem);
	}
	if (!m_Doc->documentInfo().comments().isEmpty())
	{
		QDomText desc = m_domDoc.createTextNode(m_Doc->documentInfo().comments());
		QDomElement descElem = m_domDoc.createElement("desc");
		descElem.appendChild(desc);
		m_domElem.appendChild(descElem);
	}
	m_globalDefs = m_domDoc.createElement("defs");
	writeBasePatterns();
	writeBaseSymbols();
	m_domElem.appendChild(m_globalDefs);
	if (Options.exportPageBackground)
	{
		QDomElement backG = m_domDoc.createElement("rect");
		backG.setAttribute("x", "0");
		backG.setAttribute("y", "0");
		backG.setAttribute("width", FToStr(pageWidth));
		backG.setAttribute("height", FToStr(pageHeight));
		backG.setAttribute("style", "fill:" + m_Doc->paperColor().name() + ";" + "stroke:none;");
		m_domElem.appendChild(backG);
	}
	ScLayer ll;
	ll.isPrintable = false;
	for (int la = 0; la < m_Doc->Layers.count(); la++)
	{
		m_Doc->Layers.levelToLayer(ll, la);
		if (ll.isPrintable)
		{
			page = m_Doc->MasterPages.at(m_Doc->MasterNames[m_Doc->currentPage()->masterPageName()]);
			processPageLayer(page, ll);
			page = m_Doc->currentPage();
			processPageLayer(page, ll);
		}
	}
	if(Options.compressFile)
	{
		// zipped saving
		QString wr = vo;
		wr += m_domDoc.toString();
		QByteArray utf8wr = wr.toUtf8();
		QFile file(fName);
		QtIOCompressor compressor(&file);
		compressor.setStreamFormat(QtIOCompressor::GzipFormat);
		compressor.open(QIODevice::WriteOnly);
		compressor.write(utf8wr);
		compressor.close();
	}
	else
	{
		QFile f(fName);
		if(!f.open(QIODevice::WriteOnly))
			return false;
		QDataStream s(&f);
		QString wr = vo;
		wr += m_domDoc.toString();
		QByteArray utf8wr = wr.toUtf8();
		s.writeRawData(utf8wr.data(), utf8wr.length());
		f.close();
	}
	return true;
}

void SVGExPlug::processPageLayer(ScPage *page, ScLayer& layer)
{
	QDomElement layerGroup;
	PageItem *item;
	QList<PageItem*> items;
	ScPage* SavedAct = m_Doc->currentPage();
	if (page->pageNameEmpty())
		items = m_Doc->DocItems;
	else
		items = m_Doc->MasterItems;
	if (items.count() == 0)
		return;
	if (!layer.isPrintable)
		return;
	m_Doc->setCurrentPage(page);

	layerGroup = m_domDoc.createElement("g");
	layerGroup.setAttribute("id", layer.Name);
	layerGroup.setAttribute("inkscape:label", layer.Name);
	layerGroup.setAttribute("inkscape:groupmode", "layer");
	if (layer.transparency != 1.0)
		layerGroup.setAttribute("opacity", FToStr(layer.transparency));
	for (int j = 0; j < items.count(); ++j)
	{
		item = items.at(j);
		if (item->m_layerID != layer.ID)
			continue;
		if (!item->printEnabled())
			continue;
		double x = page->xOffset();
		double y = page->yOffset();
		double w = page->width();
		double h = page->height();
		double x2 = item->BoundingX;
		double y2 = item->BoundingY;
		double w2 = item->BoundingW;
		double h2 = item->BoundingH;
		if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h, y2+h2 )))
			continue;
		if ((!page->pageNameEmpty()) && (item->OwnPage != static_cast<int>(page->pageNr())) && (item->OwnPage != -1))
			continue;
		processItemOnPage(item->xPos()-page->xOffset(), item->yPos()-page->yOffset(), item, &layerGroup);
	}
	m_domElem.appendChild(layerGroup);

	m_Doc->setCurrentPage(SavedAct);
}

void SVGExPlug::processItemOnPage(double xOffset, double yOffset, PageItem *item, QDomElement *parentElem)
{
	QDomElement ob;
	QString trans = "translate(" + FToStr(xOffset) + ", " + FToStr(yOffset) + ")";
	if (item->rotation() != 0)
		trans += " rotate(" + FToStr(item->rotation()) + ")";
	QString fill = getFillStyle(item);
	fill += processDropShadow(item);
	QString stroke = "stroke:none";
	stroke = getStrokeStyle(item);
	switch (item->itemType())
	{
		case PageItem::Arc:
		case PageItem::Polygon:
		case PageItem::PolyLine:
		case PageItem::RegularPolygon:
		case PageItem::Spiral:
			ob = processPolyItem(item, trans, fill, stroke);
			if ((item->lineColor() != CommonStrings::None) && ((item->startArrowIndex() != 0) || (item->endArrowIndex() != 0)))
				ob = processArrows(item, ob, trans);
			break;
		case PageItem::Line:
			ob = processLineItem(item, trans, stroke);
			if ((item->lineColor() != CommonStrings::None) && ((item->startArrowIndex() != 0) || (item->endArrowIndex() != 0)))
				ob = processArrows(item, ob, trans);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
			ob = processImageItem(item, trans, fill, stroke);
			break;
		case PageItem::TextFrame:
		case PageItem::PathText:
			ob = processTextItem(item, trans, fill, stroke);
			break;
		case PageItem::Symbol:
			ob = processSymbolItem(item, trans);
			break;
		case PageItem::Group:
			if (item->groupItemList.count() > 0)
			{
				ob = m_domDoc.createElement("g");
				if (!item->AutoName)
					ob.setAttribute("id", item->itemName());
				if (item->GrMask > 0)
					ob.setAttribute("mask", handleMask(item, xOffset, yOffset));
				else
				{
					if (item->fillTransparency() != 0)
						ob.setAttribute("opacity", FToStr(1.0 - item->fillTransparency()));
				}
				QString tr = trans;
				if (item->imageFlippedH())
				{
					tr += QString(" translate(%1, 0.0)").arg(item->width());
					tr += QString(" scale(-1.0, 1.0)");
				}
				if (item->imageFlippedV())
				{
					tr += QString(" translate(0.0, %1)").arg(item->height());
					tr += QString(" scale(1.0, -1.0)");
				}
				tr += QString(" scale(%1, %2)").arg(item->width() / item->groupWidth).arg(item->height() / item->groupHeight);
				ob.setAttribute("transform", tr);
				ob.setAttribute("style", "fill:none; stroke:none");
				if (item->groupClipping())
				{
					FPointArray clipPath = item->PoLine;
					QTransform transform;
					transform.scale(item->width() / item->groupWidth, item->height() / item->groupHeight);
					transform = transform.inverted();
					clipPath.map(transform);
					QDomElement obc = createClipPathElement(&clipPath);
					if (!obc.isNull())
						ob.setAttribute("clip-path", "url(#" + obc.attribute("id") + ")");
					if (item->fillRule)
						ob.setAttribute("clip-rule", "evenodd");
					else
						ob.setAttribute("clip-rule", "nonzero");
				}
				for (int em = 0; em < item->groupItemList.count(); ++em)
				{
					PageItem* embed = item->groupItemList.at(em);
					processItemOnPage(embed->gXpos, embed->gYpos, embed, &ob);
				}
			}
			break;
		case PageItem::Table:
			ob = m_domDoc.createElement("g");
			ob.setAttribute("transform", trans + QString("translate(%1, %2)").arg(item->asTable()->gridOffset().x()).arg(item->asTable()->gridOffset().y()));
			// Paint table fill.
			if (item->asTable()->fillColor() != CommonStrings::None)
			{
				int lastCol = item->asTable()->columns() - 1;
				int lastRow = item->asTable()->rows() - 1;
				double x = item->asTable()->columnPosition(0);
				double y = item->asTable()->rowPosition(0);
				double width = item->asTable()->columnPosition(lastCol) + item->asTable()->columnWidth(lastCol) - x;
				double height = item->asTable()->rowPosition(lastRow) + item->asTable()->rowHeight(lastRow) - y;
				QDomElement cl = m_domDoc.createElement("rect");
				cl.setAttribute("fill", setColor(item->asTable()->fillColor(), item->asTable()->fillShade()));
				cl.setAttribute("x", "0");
				cl.setAttribute("y", "0");
				cl.setAttribute("width", FToStr(width));
				cl.setAttribute("height", FToStr(height));
				ob.appendChild(cl);
			}
			// Pass 1: Paint cell fills.
			for (int row = 0; row < item->asTable()->rows(); ++row)
			{
				int colSpan = 0;
				for (int col = 0; col < item->asTable()->columns(); col += colSpan)
				{
					TableCell cell = item->asTable()->cellAt(row, col);
					if (row == cell.row())
					{
						QString colorName = cell.fillColor();
						if (colorName != CommonStrings::None)
						{
							int row = cell.row();
							int col = cell.column();
							int lastRow = row + cell.rowSpan() - 1;
							int lastCol = col + cell.columnSpan() - 1;
							double x = item->asTable()->columnPosition(col);
							double y = item->asTable()->rowPosition(row);
							double width = item->asTable()->columnPosition(lastCol) + item->asTable()->columnWidth(lastCol) - x;
							double height = item->asTable()->rowPosition(lastRow) + item->asTable()->rowHeight(lastRow) - y;
							QDomElement cl = m_domDoc.createElement("rect");
							cl.setAttribute("fill", setColor(colorName, cell.fillShade()));
							cl.setAttribute("x", FToStr(x));
							cl.setAttribute("y", FToStr(y));
							cl.setAttribute("width", FToStr(width));
							cl.setAttribute("height", FToStr(height));
							ob.appendChild(cl);
						}
					}
					colSpan = cell.columnSpan();
				}
			}
			// Pass 2: Paint vertical borders.
			for (int row = 0; row < item->asTable()->rows(); ++row)
			{
				int colSpan = 0;
				for (int col = 0; col < item->asTable()->columns(); col += colSpan)
				{
					TableCell cell = item->asTable()->cellAt(row, col);
					if (row == cell.row())
					{
						const int lastRow = cell.row() + cell.rowSpan() - 1;
						const int lastCol = cell.column() + cell.columnSpan() - 1;
						const double borderX = item->asTable()->columnPosition(lastCol) + item->asTable()->columnWidth(lastCol);
						QPointF start(borderX, 0.0);
						QPointF end(borderX, 0.0);
						QPointF startOffsetFactors, endOffsetFactors;
						int startRow, endRow;
						for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
						{
							TableCell rightCell = item->asTable()->cellAt(row, lastCol + 1);
							startRow = qMax(cell.row(), rightCell.row());
							endRow = qMin(lastRow, rightCell.isValid() ? rightCell.row() + rightCell.rowSpan() - 1 : lastRow);
							TableCell topLeftCell = item->asTable()->cellAt(startRow - 1, lastCol);
							TableCell topRightCell = item->asTable()->cellAt(startRow - 1, lastCol + 1);
							TableCell bottomRightCell = item->asTable()->cellAt(endRow + 1, lastCol + 1);
							TableCell bottomLeftCell = item->asTable()->cellAt(endRow + 1, lastCol);
							TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
							resolveBordersVertical(topLeftCell, topRightCell, cell, rightCell, bottomLeftCell, bottomRightCell,
												   &topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, item->asTable());
							if (border.isNull())
								continue; // Quit early if the border to paint is null.
							start.setY(item->asTable()->rowPosition(startRow));
							end.setY((item->asTable()->rowPosition(endRow) + item->asTable()->rowHeight(endRow)));
							joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
							paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
						}
						if (col == 0)
						{
							const int lastRow = cell.row() + cell.rowSpan() - 1;
							const int firstCol = cell.column();
							const double borderX = item->asTable()->columnPosition(firstCol);
							QPointF start(borderX, 0.0);
							QPointF end(borderX, 0.0);
							QPointF startOffsetFactors, endOffsetFactors;
							int startRow, endRow;
							for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
							{
								TableCell leftCell = item->asTable()->cellAt(row, firstCol - 1);
								startRow = qMax(cell.row(), leftCell.row());
								endRow = qMin(lastRow, leftCell.isValid() ? leftCell.row() + leftCell.rowSpan() - 1 : lastRow);
								TableCell topLeftCell = item->asTable()->cellAt(startRow - 1, firstCol - 1);
								TableCell topRightCell = item->asTable()->cellAt(startRow - 1, firstCol);
								TableCell bottomRightCell = item->asTable()->cellAt(lastRow + 1, firstCol);
								TableCell bottomLeftCell = item->asTable()->cellAt(lastRow + 1, firstCol - 1);
								TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
								resolveBordersVertical(topLeftCell, topRightCell, leftCell, cell, bottomLeftCell, bottomRightCell,
													   &topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, item->asTable());
								if (border.isNull())
									continue; // Quit early if the border to paint is null.
								start.setY(item->asTable()->rowPosition(startRow));
								end.setY((item->asTable()->rowPosition(endRow) + item->asTable()->rowHeight(endRow)));
								joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
								paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
							}
						}
					}
					colSpan = cell.columnSpan();
				}
			}
			// Pass 3: Paint horizontal borders.
			for (int row = 0; row < item->asTable()->rows(); ++row)
			{
				int colSpan = 0;
				for (int col = 0; col < item->asTable()->columns(); col += colSpan)
				{
					TableCell cell = item->asTable()->cellAt(row, col);
					if (row == cell.row())
					{
						const int lastRow = cell.row() + cell.rowSpan() - 1;
						const int lastCol = cell.column() + cell.columnSpan() - 1;
						const double borderY = (item->asTable()->rowPosition(lastRow) + item->asTable()->rowHeight(lastRow));
						QPointF start(0.0, borderY);
						QPointF end(0.0, borderY);
						QPointF startOffsetFactors, endOffsetFactors;
						int startCol, endCol;
						for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
						{
							TableCell bottomCell = item->asTable()->cellAt(lastRow + 1, col);
							startCol = qMax(cell.column(), bottomCell.column());
							endCol = qMin(lastCol, bottomCell.isValid() ? bottomCell.column() + bottomCell.columnSpan() - 1 : lastCol);
							TableCell topLeftCell = item->asTable()->cellAt(lastRow, startCol - 1);
							TableCell topRightCell = item->asTable()->cellAt(lastRow, endCol + 1);
							TableCell bottomRightCell = item->asTable()->cellAt(lastRow + 1, endCol + 1);
							TableCell bottomLeftCell = item->asTable()->cellAt(lastRow + 1, startCol - 1);
							TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
							resolveBordersHorizontal(topLeftCell, cell, topRightCell, bottomLeftCell, bottomCell,
													 bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, item->asTable());
							if (border.isNull())
								continue; // Quit early if the border is null.
							start.setX(item->asTable()->columnPosition(startCol));
							end.setX(item->asTable()->columnPosition(endCol) + item->asTable()->columnWidth(endCol));
							joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
							paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
						}
						if (row == 0)
						{
							const int firstRow = cell.row();
							const int lastCol = cell.column() + cell.columnSpan() - 1;
							const double borderY = item->asTable()->rowPosition(firstRow);
							QPointF start(0.0, borderY);
							QPointF end(0.0, borderY);
							QPointF startOffsetFactors, endOffsetFactors;
							int startCol, endCol;
							for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
							{
								TableCell topCell = item->asTable()->cellAt(firstRow - 1, col);
								startCol = qMax(cell.column(), topCell.column());
								endCol = qMin(lastCol, topCell.isValid() ? topCell.column() + topCell.columnSpan() - 1 : lastCol);
								TableCell topLeftCell = item->asTable()->cellAt(firstRow - 1, startCol - 1);
								TableCell topRightCell = item->asTable()->cellAt(firstRow - 1, endCol + 1);
								TableCell bottomRightCell = item->asTable()->cellAt(firstRow, endCol + 1);
								TableCell bottomLeftCell = item->asTable()->cellAt(firstRow, startCol - 1);
								TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
								resolveBordersHorizontal(topLeftCell, topCell, topRightCell, bottomLeftCell, cell,
														 bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, item->asTable());
								if (border.isNull())
									continue; // Quit early if the border is null.
								start.setX(item->asTable()->columnPosition(startCol));
								end.setX(item->asTable()->columnPosition(endCol) + item->asTable()->columnWidth(endCol));
								joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
								paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
							}
						}
					}
					colSpan = cell.columnSpan();
				}
			}
			// Pass 4: Paint cell content.
			for (int row = 0; row < item->asTable()->rows(); ++row)
			{
				for (int col = 0; col < item->asTable()->columns(); col ++)
				{
					TableCell cell = item->asTable()->cellAt(row, col);
					if (cell.row() == row && cell.column() == col)
					{
						PageItem* textFrame = cell.textFrame();
						processItemOnPage(cell.contentRect().x(), cell.contentRect().y(), textFrame, &ob);
					}
				}
			}
			break;
		default:
			break;
	}
	if (item->GrMask > 0)
		ob.setAttribute("mask", handleMask(item, xOffset, yOffset));
	if (!item->AutoName)
		ob.setAttribute("id", item->itemName());
	parentElem->appendChild(ob);
}

void SVGExPlug::paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end, const QPointF& startOffsetFactors, const QPointF& endOffsetFactors, QDomElement &ob)
{
	QPointF lineStart, lineEnd;
	for (const TableBorderLine& line : border.borderLines())
	{
		lineStart.setX(start.x() + line.width() * startOffsetFactors.x());
		lineStart.setY(start.y() + line.width() * startOffsetFactors.y());
		lineEnd.setX(end.x() + line.width() * endOffsetFactors.x());
		lineEnd.setY(end.y() + line.width() * endOffsetFactors.y());
		QDomElement cl = m_domDoc.createElement("path");
		cl.setAttribute("d", "M " + FToStr(lineStart.x()) + " " + FToStr(lineStart.y()) + " L " + FToStr(lineEnd.x()) + " " + FToStr(lineEnd.y()));
		QString stroke;
		if (line.color() != CommonStrings::None)
			cl.setAttribute("stroke", setColor(line.color(), line.shade()));
		if (line.width() != 0.0)
			stroke = "stroke-width:" + FToStr(line.width()) + ";";
		else
			stroke = "stroke-width:1px;";
		stroke += " stroke-linecap:butt;";
		stroke += " stroke-linejoin:miter;";
		stroke += " stroke-dasharray:";
		if (line.style() == Qt::SolidLine)
			stroke += "none;";
		else
		{
			QString Da = getDashString(line.style(), qMax(line.width(), 1.0));
			if (Da.isEmpty())
				stroke += "none;";
			else
				stroke += Da.replace(" ", ", ") + ";";
		}
		cl.setAttribute("style", stroke);
		ob.appendChild(cl);
	}
}

QString SVGExPlug::processDropShadow(PageItem *item)
{
	if (!item->hasSoftShadow())
		return "";
	QString ID = "Filter" + IToStr(m_filterCount);
	QDomElement filter = m_domDoc.createElement("filter");
	filter.setAttribute("id", ID);
	filter.setAttribute("inkscape:label", "Drop shadow");
	QDomElement ob = m_domDoc.createElement("feGaussianBlur");
	ob.setAttribute("id", "feGaussianBlur" + IToStr(m_filterCount));
	ob.setAttribute("in", "SourceAlpha");
	ob.setAttribute("stdDeviation", FToStr(item->softShadowBlurRadius()));
	ob.setAttribute("result", "blur");
	filter.appendChild(ob);
	QDomElement ob2 = m_domDoc.createElement("feColorMatrix");
	ob2.setAttribute("id", "feColorMatrix" + IToStr(m_filterCount));
	const ScColor& col = m_Doc->PageColors[item->softShadowColor()];
	QColor color = ScColorEngine::getShadeColorProof(col, m_Doc, item->softShadowShade());
	ob2.setAttribute("type", "matrix");
	ob2.setAttribute("values", QString("1 0 0 %1 0 0 1 0 %2 0 0 0 1 %3 0 0 0 0 %4 0").arg(color.redF()).arg(color.greenF()).arg(color.blueF()).arg(1.0 - item->softShadowOpacity()));
	ob2.setAttribute("result", "bluralpha");
	filter.appendChild(ob2);
	QDomElement ob3 = m_domDoc.createElement("feOffset");
	ob3.setAttribute("id", "feOffset" + IToStr(m_filterCount));
	ob3.setAttribute("in", "bluralpha");
	ob3.setAttribute("dx", FToStr(item->softShadowXOffset()));
	ob3.setAttribute("dy", FToStr(item->softShadowYOffset()));
	ob3.setAttribute("result", "offsetBlur");
	filter.appendChild(ob3);
	QDomElement ob4 = m_domDoc.createElement("feMerge");
	ob4.setAttribute("id", "feMerge" + IToStr(m_filterCount));
	QDomElement ob5 = m_domDoc.createElement("feMergeNode");
	ob5.setAttribute("id", "feMergeNode1" + IToStr(m_filterCount));
	ob5.setAttribute("in", "offsetBlur");
	ob4.appendChild(ob5);
	QDomElement ob6 = m_domDoc.createElement("feMergeNode");
	ob6.setAttribute("id", "feMergeNode2" + IToStr(m_filterCount));
	ob6.setAttribute("in", "SourceGraphic");
	ob4.appendChild(ob6);
	filter.appendChild(ob4);
	m_globalDefs.appendChild(filter);
	m_filterCount++;
	return "filter:url(#" + ID + ");";
}

QDomElement SVGExPlug::processHatchFill(PageItem *item, const QString& transl)
{
	QDomElement ob;
	ob = m_domDoc.createElement("g");
	if (!transl.isEmpty())
		ob.setAttribute("transform", transl);
	QDomElement obc = createClipPathElement(&item->PoLine);
	if (!obc.isNull())
		ob.setAttribute("clip-path", "url(#" + obc.attribute("id") + ")");
	if (item->fillRule)
		ob.setAttribute("clip-rule", "evenodd");
	else
		ob.setAttribute("clip-rule", "nonzero");
	if (item->hatchUseBackground)
	{
		QDomElement ob2 = m_domDoc.createElement("path");
		ob2.setAttribute("d", setClipPath(&item->PoLine, true));
		ob2.setAttribute("fill", setColor(item->hatchBackground, 100));
		ob.appendChild(ob2);
	}
	QString stroke;
	stroke += "stroke-width:1;";
	stroke += " stroke-linecap:butt;";
	stroke += " stroke-linejoin:miter;";
	double lineLen = sqrt((item->width() / 2.0) * (item->width() / 2.0) + (item->height() / 2.0) * (item->height() / 2.0));
	double dist = 0.0;
	while (dist < lineLen)
	{
		QTransform mpx;
		mpx.translate(item->width() / 2.0, item->height() / 2.0);
		if (item->hatchAngle != 0.0)
			mpx.rotate(-item->hatchAngle);
		QDomElement ob3 = m_domDoc.createElement("path");
		ob3.setAttribute("transform", matrixToStr(mpx));
		ob3.setAttribute("d", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(dist).arg(lineLen).arg(dist));
		ob3.setAttribute("stroke", setColor(item->hatchForeground, 100));
		ob3.setAttribute("style", stroke);
		ob.appendChild(ob3);
		if (dist > 0)
		{
			QDomElement ob4 = m_domDoc.createElement("path");
			ob4.setAttribute("transform", matrixToStr(mpx));
			ob4.setAttribute("d", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(-dist).arg(lineLen).arg(-dist));
			ob4.setAttribute("stroke", setColor(item->hatchForeground, 100));
			ob4.setAttribute("style", stroke);
			ob.appendChild(ob4);
		}
		dist += item->hatchDistance;
	}
	if ((item->hatchType == 1) || (item->hatchType == 2))
	{
		dist = 0.0;
		while (dist < lineLen)
		{
			QTransform mpx;
			mpx.translate(item->width() / 2.0, item->height() / 2.0);
			if (item->hatchAngle != 0.0)
				mpx.rotate(-item->hatchAngle + 90);
			QDomElement ob3 = m_domDoc.createElement("path");
			ob3.setAttribute("transform", matrixToStr(mpx));
			ob3.setAttribute("d", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(dist).arg(lineLen).arg(dist));
			ob3.setAttribute("stroke", setColor(item->hatchForeground, 100));
			ob3.setAttribute("style", stroke);
			ob.appendChild(ob3);
			if (dist > 0)
			{
				QDomElement ob4 = m_domDoc.createElement("path");
				ob4.setAttribute("transform", matrixToStr(mpx));
				ob4.setAttribute("d", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(-dist).arg(lineLen).arg(-dist));
				ob4.setAttribute("stroke", setColor(item->hatchForeground, 100));
				ob4.setAttribute("style", stroke);
				ob.appendChild(ob4);
			}
			dist += item->hatchDistance;
		}
	}
	if (item->hatchType == 2)
	{
		dist = 0.0;
		while (dist < lineLen)
		{
			double dDist = dist * sqrt(2.0);
			QTransform mpx;
			mpx.translate(item->width() / 2.0, item->height() / 2.0);
			if (item->hatchAngle != 0.0)
				mpx.rotate(-item->hatchAngle + 45);
			QDomElement ob3 = m_domDoc.createElement("path");
			ob3.setAttribute("transform", matrixToStr(mpx));
			ob3.setAttribute("d", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(dDist).arg(lineLen).arg(dDist));
			ob3.setAttribute("stroke", setColor(item->hatchForeground, 100));
			ob3.setAttribute("style", stroke);
			ob.appendChild(ob3);
			if (dist > 0)
			{
				QDomElement ob4 = m_domDoc.createElement("path");
				ob4.setAttribute("transform", matrixToStr(mpx));
				ob4.setAttribute("d", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(-dDist).arg(lineLen).arg(-dDist));
				ob4.setAttribute("stroke", setColor(item->hatchForeground, 100));
				ob4.setAttribute("style", stroke);
				ob.appendChild(ob4);
			}
			dist += item->hatchDistance;
		}
	}
	return ob;
}

QDomElement SVGExPlug::processSymbolStroke(PageItem *item, const QString& trans)
{
	QDomElement ob;
	ob = m_domDoc.createElement("g");
	ob.setAttribute("transform", trans);
	QPainterPath path = item->PoLine.toQPainterPath(false);
	ScPattern pat = m_Doc->docPatterns[item->strokePattern()];
	double pLen = path.length() - ((pat.width / 2.0) * item->patternStrokeTransfrm.scaleX);
	double adv = pat.width * item->patternStrokeTransfrm.scaleX * item->patternStrokeTransfrm.space;
	double xpos = item->patternStrokeTransfrm.offsetX * item->patternStrokeTransfrm.scaleX;
	while (xpos < pLen)
	{
		double currPerc = path.percentAtLength(xpos);
		double currAngle = path.angleAtPercent(currPerc);
		if (currAngle <= 180.0)
			currAngle *= -1.0;
		else
			currAngle = 360.0 - currAngle;
		QPointF currPoint = path.pointAtPercent(currPerc);
		QTransform trans;
		trans.translate(currPoint.x(), currPoint.y());
		trans.rotate(-currAngle);
		trans.translate(0.0, item->patternStrokeTransfrm.offsetY);
		trans.rotate(-item->patternStrokeTransfrm.rotation);
		trans.shear(item->patternStrokeTransfrm.skewX, -item->patternStrokeTransfrm.skewY);
		trans.scale(item->patternStrokeTransfrm.scaleX, item->patternStrokeTransfrm.scaleY);
		trans.translate(-pat.width / 2.0, -pat.height / 2.0);
		QDomElement obS;
		obS = m_domDoc.createElement("use");
		obS.setAttribute("transform", matrixToStr(trans));
		if (item->patternStrokeMirrorX)
		{
			trans.translate(pat.width, 0);
			trans.scale(-1, 1);
		}
		if (item->patternStrokeMirrorY)
		{
			trans.translate(0, pat.height);
			trans.scale(1, -1);
		}
		obS.setAttribute("x", "0");
		obS.setAttribute("y", "0");
		obS.setAttribute("width", FToStr(pat.width));
		obS.setAttribute("height", FToStr(pat.height));
		obS.setAttribute("xlink:href", "#S" + item->strokePattern());
		ob.appendChild(obS);
		xpos += adv;
	}
	return ob;
}

QDomElement SVGExPlug::processSymbolItem(PageItem *item, const QString& trans)
{
	QDomElement ob;
	ScPattern pat = m_Doc->docPatterns[item->pattern()];
	ob = m_domDoc.createElement("use");
	ob.setAttribute("x", "0");
	ob.setAttribute("y", "0");
	ob.setAttribute("width", FToStr(pat.width));
	ob.setAttribute("height", FToStr(pat.height));
	ob.setAttribute("xlink:href", "#S" + item->pattern());
	QString tr = trans + QString(" scale(%1, %2)").arg(item->width() / pat.width).arg(item->height() / pat.height);
	ob.setAttribute("transform", tr);
	return ob;
}

QDomElement SVGExPlug::processPolyItem(PageItem *item, const QString& trans, const QString& fill, const QString& stroke)
{
	bool closedPath;
	QDomElement ob;
	closedPath = (item->itemType() == PageItem::Polygon) || (item->itemType() == PageItem::RegularPolygon) || (item->itemType() == PageItem::Arc);
	if (item->NamedLStyle.isEmpty())
	{
		if ((!item->strokePattern().isEmpty()) && (item->patternStrokePath))
		{
			ob = m_domDoc.createElement("g");
			if (item->GrType == Gradient_Hatch)
			{
				QDomElement ob1 = processHatchFill(item, trans);
				ob.appendChild(ob1);
			}
			QDomElement ob2 = m_domDoc.createElement("path");
			ob2.setAttribute("d", setClipPath(&item->PoLine, closedPath));
			ob2.setAttribute("transform", trans);
			if (item->GrType != Gradient_Hatch)
				ob2.setAttribute("style", fill);
			else
			{
				QString drS = processDropShadow(item);
				if (!drS.isEmpty())
					ob2.setAttribute("style", "fill:none;" + drS);
			}
			ob.appendChild(ob2);
			ob.appendChild(processSymbolStroke(item, trans));
		}
		else
		{
			if (item->GrType == Gradient_Hatch)
			{
				ob = m_domDoc.createElement("g");
				ob.setAttribute("transform", trans);
				QDomElement ob1 = processHatchFill(item);
				ob.appendChild(ob1);
				QDomElement ob2 = m_domDoc.createElement("path");
				ob2.setAttribute("d", setClipPath(&item->PoLine, closedPath));
				ob2.setAttribute("style", stroke + "fill:none;" + processDropShadow(item));
				ob.appendChild(ob2);
			}
			else
			{
				ob = m_domDoc.createElement("path");
				ob.setAttribute("d", setClipPath(&item->PoLine, closedPath));
				ob.setAttribute("transform", trans);
				ob.setAttribute("style", fill + stroke);
			}
		}
	}
	else
	{
		ob = m_domDoc.createElement("g");
		ob.setAttribute("transform", trans);
		if (item->GrType == Gradient_Hatch)
		{
			QDomElement ob1 = processHatchFill(item);
			ob.appendChild(ob1);
		}
		QDomElement ob2 = m_domDoc.createElement("path");
		ob2.setAttribute("d", setClipPath(&item->PoLine, closedPath));
		if (item->GrType != Gradient_Hatch)
			ob2.setAttribute("style", fill);
		else
		{
			QString drS = processDropShadow(item);
			if (!drS.isEmpty())
				ob2.setAttribute("style", "fill:none;" + drS);
		}
		ob.appendChild(ob2);
		multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				QDomElement ob3 = m_domDoc.createElement("path");
				ob3.setAttribute("d", setClipPath(&item->PoLine, closedPath));
				ob3.setAttribute("style", getMultiStroke(&ml[it], item));
				ob.appendChild(ob3);
			}
		}
	}
	return ob;
}

QDomElement SVGExPlug::processLineItem(PageItem *item, const QString& trans, const QString& stroke)
{
	QDomElement ob;
	if (item->NamedLStyle.isEmpty())
	{
		ob = m_domDoc.createElement("path");
		ob.setAttribute("d", "M 0 0 L " + FToStr(item->width()) + " 0");
		ob.setAttribute("transform", trans);
		ob.setAttribute("style", stroke);
	}
	else
	{
		ob = m_domDoc.createElement("g");
		ob.setAttribute("transform", trans);
		multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
		for (int i = ml.size()-1; i > -1; i--)
		{
			if ((ml[i].Color != CommonStrings::None) && (ml[i].Width != 0))
			{
				QDomElement ob2 = m_domDoc.createElement("path");
				ob2.setAttribute("d", "M 0 0 L " + FToStr(item->width()) + " 0");
				ob2.setAttribute("style", getMultiStroke(&ml[i], item));
				ob.appendChild(ob2);
			}
		}
	}
	return ob;
}

QDomElement SVGExPlug::processImageItem(PageItem *item, const QString& trans, const QString& fill, const QString& stroke)
{
	QDomElement ob;
	ob = m_domDoc.createElement("g");
	ob.setAttribute("transform", trans);
	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		if (item->GrType == Gradient_Hatch)
		{
			QDomElement ob1 = processHatchFill(item);
			ob.appendChild(ob1);
			QString drS = processDropShadow(item);
			if (!drS.isEmpty())
				ob.setAttribute("style", "fill:none;" + drS);
		}
		else
		{
			QDomElement ob1 = m_domDoc.createElement("path");
			ob1.setAttribute("d", setClipPath(&item->PoLine, true));
			ob1.setAttribute("style", fill);
			ob.appendChild(ob1);
		}
	}
	if ((item->imageIsAvailable) && (!item->Pfile.isEmpty()))
	{
		QDomElement cl, ob2;
		if (!item->imageClip.empty())
			ob2 = createClipPathElement(&item->imageClip, &cl);
		else
			ob2 = createClipPathElement(&item->PoLine, &cl);
		if (!ob2.isNull())
		{
			ob2.setAttribute("clipPathUnits", "userSpaceOnUse");
			ob2.setAttribute("clip-rule", "evenodd");
			QTransform mpc;
			if (item->imageFlippedH())
			{
				mpc.translate(item->width(), 0);
				mpc.scale(-1, 1);
			}
			if (item->imageFlippedV())
			{
				mpc.translate(0, item->height());
				mpc.scale(1, -1);
			}
			cl.setAttribute("transform", matrixToStr(mpc));
		}
		QDomElement ob6 = m_domDoc.createElement("g");
		if (!ob2.isNull())
			ob6.setAttribute("clip-path", "url(#" + ob2.attribute("id") + ")");
		QDomElement ob3 = m_domDoc.createElement("image");
		ScImage img;
		CMSettings cms(m_Doc, item->ImageProfile, item->ImageIntent);
		cms.setUseEmbeddedProfile(item->UseEmbedded);
		cms.allowSoftProofing(true);
		img.loadPicture(item->Pfile, item->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, 72);
		img.applyEffect(item->effectsInUse, m_Doc->PageColors, true);
		if (Options.inlineImages)
		{
			QBuffer buffer;
			buffer.open(QIODevice::WriteOnly);
			img.qImage().save(&buffer, "PNG");
			QByteArray ba = buffer.buffer().toBase64();
			buffer.close();
			ob3.setAttribute("xlink:href", "data:image/png;base64," + QString(ba));
		}
		else
		{
			QFileInfo fi = QFileInfo(item->Pfile);
			QString imgFileName = m_baseDir + "/" + fi.baseName() + ".png";
			QFileInfo im = QFileInfo(imgFileName);
			if (im.exists())
				imgFileName = m_baseDir + "/" + fi.baseName() + "_copy.png";
			img.qImage().save(imgFileName, "PNG");
			QFileInfo fi2 = QFileInfo(imgFileName);
			ob3.setAttribute("xlink:href", fi2.baseName() + ".png");
		}
		ob3.setAttribute("x", FToStr(item->imageXOffset() * item->imageXScale()));
		ob3.setAttribute("y", FToStr(item->imageYOffset() * item->imageYScale()));
		ob3.setAttribute("width", FToStr(img.width() * item->imageXScale()));
		ob3.setAttribute("height", FToStr(img.height() * item->imageYScale()));
		QTransform mpa;
		if (item->imageFlippedH())
		{
			mpa.translate(item->width(), 0);
			mpa.scale(-1, 1);
		}
		if (item->imageFlippedV())
		{
			mpa.translate(0, item->height());
			mpa.scale(1, -1);
		}
		mpa.rotate(item->imageRotation());
		ob3.setAttribute("transform", matrixToStr(mpa));
		ob6.appendChild(ob3);
		ob.appendChild(ob6);
	}
	if (item->NamedLStyle.isEmpty())
	{
		if ((!item->strokePattern().isEmpty()) && (item->patternStrokePath))
		{
			QDomElement ob4 = m_domDoc.createElement("g");
			QDomElement ob2 = m_domDoc.createElement("path");
			ob2.setAttribute("d", setClipPath(&item->PoLine, true));
			ob2.setAttribute("transform", trans);
			ob2.setAttribute("style", fill);
			ob4.appendChild(ob2);
			ob4.appendChild(processSymbolStroke(item, trans));
			ob.appendChild(ob4);
		}
		else
		{
			QDomElement ob4 = m_domDoc.createElement("path");
			ob4.setAttribute("d", setClipPath(&item->PoLine, true));
			ob4.setAttribute("style", "fill:none; " + stroke);
			ob.appendChild(ob4);
		}
	}
	else
	{
		multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				QDomElement ob5 = m_domDoc.createElement("path");
				ob5.setAttribute("d", setClipPath(&item->PoLine, true));
				ob5.setAttribute("style", "fill:none; " + getMultiStroke(&ml[it], item));
				ob.appendChild(ob5);
			}
		}
	}
	return ob;
}

class SvgPainter: public TextLayoutPainter
{
	QDomElement m_elem;
	SVGExPlug *m_svg;
	QString m_trans;

public:
	SvgPainter(const QString& trans, SVGExPlug *svg, QDomElement &elem)
		: m_elem(elem)
		, m_svg(svg)
		, m_trans(trans)
	{}

	void drawGlyph(const GlyphCluster& gc) override
	{
		if (gc.isControlGlyphs() || gc.isEmpty())
			return;
		double current_x = 0.0;
		for (const GlyphLayout& gl : gc.glyphs())
		{
			if (gl.glyph >= ScFace::CONTROL_GLYPHS)
			{
				current_x += gl.xadvance * gl.scaleH;
				continue;
			}

			QTransform transform = matrix();
			transform.translate(x() + gl.xoffset + current_x, y() - (fontSize() * gc.scaleV()) + gl.yoffset);
			transform.scale(gc.scaleH() * fontSize() / 10.0, gc.scaleV() * fontSize() / 10.0);
			QDomElement glyph = m_svg->m_domDoc.createElement("use");
			glyph.setAttribute("xlink:href", "#" + m_svg->handleGlyph(gl.glyph, font()));
			glyph.setAttribute("transform", m_svg->matrixToStr(transform));
			QString fill = "fill:" + m_svg->setColor(fillColor().color, fillColor().shade) + ";";
			QString stroke = "stroke:none;";
			glyph.setAttribute("style", fill + stroke);
			m_elem.appendChild(glyph);

			current_x += gl.xadvance * gl.scaleH;
		}
	}

	void drawGlyphOutline(const GlyphCluster& gc, bool hasFill) override
	{
		if (gc.isControlGlyphs() || gc.isEmpty())
			return;

		double current_x = 0.0;
		for (const GlyphLayout& gl : gc.glyphs())
		{
			if (gl.glyph >= ScFace::CONTROL_GLYPHS)
			{
				current_x += gl.xadvance * gl.scaleH;
				continue;
			}

			QTransform transform = matrix();
			transform.translate(x() + gl.xoffset + current_x, y() - (fontSize() * gc.scaleV()) + gl.yoffset);
			transform.scale(gc.scaleH() * fontSize() / 10.0, gc.scaleV() * fontSize() / 10.0);
			QDomElement glyph = m_svg->m_domDoc.createElement("use");
			glyph.setAttribute("xlink:href", "#" + m_svg->handleGlyph(gl.glyph, font()));
			glyph.setAttribute("transform", m_svg->matrixToStr(transform));
			QString fill = "fill:none;";
			if (hasFill)
				fill = "fill:" + m_svg->setColor(fillColor().color, fillColor().shade) + ";";
			QString stroke ="stroke:" + m_svg->setColor(strokeColor().color, strokeColor().shade) + ";";
			stroke += " stroke-width:" + m_svg->FToStr(strokeWidth() / (gc.scaleV() * fontSize() / 10.0)) + ";";
			glyph.setAttribute("style", fill + stroke);
			m_elem.appendChild(glyph);

			current_x += gl.xadvance * gl.scaleH;
		}
	}

	void drawLine(const QPointF& start, const QPointF& end) override
	{
		QTransform transform = matrix();
		transform.translate(x(), y());
		QDomElement path = m_svg-> m_domDoc.createElement("path");
		path.setAttribute("d", QString("M %1 %2 L%3 %4").arg(start.x()).arg(start.y()).arg(end.x()).arg(end.y()));
		QString stroke = "stroke:none;";
		if (fillColor().color != CommonStrings::None)
		{
			stroke = "stroke:" + m_svg->setColor(fillColor().color, fillColor().shade) + ";";
			stroke += " stroke-width:" + m_svg->FToStr(strokeWidth()) + ";";
		}
		path.setAttribute("style", "fill:none;" + stroke);
		path.setAttribute("transform", m_svg->matrixToStr(transform));
		m_elem.appendChild(path);
	}

	void drawRect(const QRectF& rect) override
	{
		QTransform transform = matrix();
		transform.translate(x(), y());
		QString paS = QString("M %1 %2 ").arg(rect.x()).arg(rect.y());
		paS += QString("L %1 %2 ").arg(rect.x() + rect.width()).arg(rect.y());
		paS += QString("L %1 %2 ").arg(rect.x() + rect.width()).arg(rect.y() + rect.height());
		paS += QString("L %1 %2 ").arg(rect.x()).arg(rect.y() + rect.height());
		paS += "Z";
		QDomElement path = m_svg->m_domDoc.createElement("path");
		path.setAttribute("d", paS);
		path.setAttribute("transform", m_svg->matrixToStr(transform));
		path.setAttribute("style", "fill:" + m_svg->setColor(fillColor().color, fillColor().shade) + ";" + "stroke:none;");
		m_elem.appendChild(path);
	}

	void drawObject(PageItem* item) override
	{
		QTransform transform = matrix();
		transform.translate(x(), y());
		transform.rotate(item->rotation());
		transform.scale(scaleH(), scaleV());
		QDomElement Group = m_svg->m_domDoc.createElement("g");
		QDomElement layerGroup = m_svg->processInlineItem(item, m_trans, scaleH(), scaleV());
		Group.appendChild(layerGroup);
		Group.setAttribute("transform", m_svg->matrixToStr(transform));
		m_elem.appendChild(Group);
	}

	void drawObjectDecoration(PageItem* item) override {}
};

QDomElement SVGExPlug::processTextItem(PageItem *item, const QString& trans, const QString& fill, const QString& stroke)
{
	QDomElement ob;
	ob = m_domDoc.createElement("g");
	ob.setAttribute("transform", trans);
	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		if (item->GrType == Gradient_Hatch)
		{
			QDomElement ob1 = processHatchFill(item);
			ob.appendChild(ob1);
			QString drS = processDropShadow(item);
			if (!drS.isEmpty())
				ob.setAttribute("style", "fill:none;" + drS);
		}
		else
		{
			QDomElement ob1 = m_domDoc.createElement("path");
			ob1.setAttribute("d", setClipPath(&item->PoLine, true));
			ob1.setAttribute("style", fill);
			ob.appendChild(ob1);
		}
	}

	if (item->itemText.length() != 0)
	{
		SvgPainter p(trans, this, ob);
		item->textLayout.renderBackground(&p);
		item->textLayout.render(&p);
	}
	if (item->isTextFrame())
	{
		if (item->NamedLStyle.isEmpty())
		{
			if ((!item->strokePattern().isEmpty()) && (item->patternStrokePath))
			{
				QDomElement ob4 = m_domDoc.createElement("g");
				QDomElement ob2 = m_domDoc.createElement("path");
				ob2.setAttribute("d", setClipPath(&item->PoLine, true));
				ob2.setAttribute("transform", trans);
				ob2.setAttribute("style", fill);
				ob4.appendChild(ob2);
				ob4.appendChild(processSymbolStroke(item, trans));
				ob.appendChild(ob4);
			}
			else
			{
				QDomElement ob4 = m_domDoc.createElement("path");
				ob4.setAttribute("d", setClipPath(&item->PoLine, true));
				ob4.setAttribute("style", "fill:none; " + stroke);
				ob.appendChild(ob4);
			}
		}
		else
		{
			multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob5 = m_domDoc.createElement("path");
					ob5.setAttribute("d", setClipPath(&item->PoLine, true));
					ob5.setAttribute("style", "fill:none; " + getMultiStroke(&ml[it], item));
					ob.appendChild(ob5);
				}
			}
		}
	}
	else if (item->isPathText() && item->PoShow)
	{
		if (item->NamedLStyle.isEmpty())
		{
			if ((!item->strokePattern().isEmpty()) && (item->patternStrokePath))
			{
				QDomElement ob4 = m_domDoc.createElement("g");
				QDomElement ob2 = m_domDoc.createElement("path");
				ob2.setAttribute("d", setClipPath(&item->PoLine, false));
				ob2.setAttribute("transform", trans);
				ob2.setAttribute("style", fill);
				ob4.appendChild(ob2);
				ob4.appendChild(processSymbolStroke(item, trans));
				ob.appendChild(ob4);
			}
			else
			{
				QDomElement ob4 = m_domDoc.createElement("path");
				ob4.setAttribute("d", setClipPath(&item->PoLine, false));
				ob4.setAttribute("style", "fill:none; " + stroke);
				ob.appendChild(ob4);
			}
		}
		else
		{
			multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob5 = m_domDoc.createElement("path");
					ob5.setAttribute("d", setClipPath(&item->PoLine, false));
					ob5.setAttribute("style", "fill:none; " + getMultiStroke(&ml[it], item));
					ob.appendChild(ob5);
				}
			}
		}
	}
	return ob;
}

QDomElement SVGExPlug::processInlineItem(PageItem* embItem, const QString& trans, double scaleH, double scaleV)
{
	QList<PageItem*> emG;
	if (embItem->isGroup())
		emG = embItem->groupItemList;
	else 
		emG.append(embItem);

	QDomElement layerGroup = m_domDoc.createElement("g");
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		QDomElement obE;
		QString fill = getFillStyle(embedded);
		QString stroke = "stroke:none";
		stroke = getStrokeStyle(embedded);
		QString transE;
		if (embItem->isGroup())
		{
			transE = "translate(" + FToStr(embedded->gXpos) + ", " + FToStr(embedded->gYpos) + ")";
			if (embedded->rotation() != 0)
				transE += " rotate(" + FToStr(embedded->rotation()) + ")";
		}
		switch (embedded->itemType())
		{
			case PageItem::Arc:
			case PageItem::Polygon:
			case PageItem::PolyLine:
			case PageItem::RegularPolygon:
			case PageItem::Spiral:
				obE = processPolyItem(embedded, transE, fill, stroke);
				if ((embedded->lineColor() != CommonStrings::None) && ((embedded->startArrowIndex() != 0) || (embedded->endArrowIndex() != 0)))
					obE = processArrows(embedded, obE, transE);
				break;
			case PageItem::Line:
				obE = processLineItem(embedded, transE, stroke);
				if ((embedded->lineColor() != CommonStrings::None) && ((embedded->startArrowIndex() != 0) || (embedded->endArrowIndex() != 0)))
					obE = processArrows(embedded, obE, transE);
				break;
			case PageItem::ImageFrame:
			case PageItem::LatexFrame:
				obE = processImageItem(embedded, transE, fill, stroke);
				break;
			case PageItem::TextFrame:
			case PageItem::PathText:
				obE = processTextItem(embedded, transE, fill, stroke);
				break;
			case PageItem::Symbol:
				obE = processSymbolItem(embedded, transE);
				break;
			case PageItem::Group:
				if (embedded->groupItemList.count() > 0)
				{
					obE = m_domDoc.createElement("g");
					if (!embedded->AutoName)
						obE.setAttribute("id", embedded->itemName());
					if (embedded->GrMask > 0)
						obE.setAttribute("mask", handleMask(embedded, embedded->xPos() - m_Doc->currentPage()->xOffset(), embedded->yPos() - m_Doc->currentPage()->yOffset()));
					else
					{
						if (embedded->fillTransparency() != 0)
							obE.setAttribute("opacity", FToStr(1.0 - embedded->fillTransparency()));
					}
					QString tr = trans;
					if (embedded->imageFlippedH())
					{
						tr += QString(" translate(%1, 0.0)").arg(embedded->width());
						tr += QString(" scale(-1.0, 1.0)");
					}
					if (embedded->imageFlippedV())
					{
						tr += QString(" translate(0.0, %1)").arg(embedded->height());
						tr += QString(" scale(1.0, -1.0)");
					}
					tr += QString(" scale(%1, %2)").arg(embedded->width() / embedded->groupWidth).arg(embedded->height() / embedded->groupHeight);
					obE.setAttribute("transform", tr);
					obE.setAttribute("style", "fill:none; stroke:none");
					if (embedded->groupClipping())
					{
						FPointArray clipPath = embedded->PoLine;
						QTransform transform;
						transform.scale(embedded->width() / embedded->groupWidth, embedded->height() / embedded->groupHeight);
						transform = transform.inverted();
						clipPath.map(transform);
						QDomElement obc = createClipPathElement(&clipPath);
						if (!obc.isNull())
							obE.setAttribute("clip-path", "url(#" + obc.attribute("id") + ")");
						if (embedded->fillRule)
							obE.setAttribute("clip-rule", "evenodd");
						else
							obE.setAttribute("clip-rule", "nonzero");
					}
					for (int em = 0; em < embedded->groupItemList.count(); ++em)
					{
						PageItem* embed = embedded->groupItemList.at(em);
						processItemOnPage(embed->gXpos, embed->gYpos, embed, &obE);
					}
				}
				break;
			default:
				break;
		}
		layerGroup.appendChild(obE);
	}
	QTransform mm;
	if (embItem->isGroup())
		mm.scale(embItem->width() / embItem->groupWidth, embItem->height() / embItem->groupHeight);
	layerGroup.setAttribute("transform", matrixToStr(mm));
	return layerGroup;
}

QString SVGExPlug::handleGlyph(uint gid, const ScFace& font)
{
	QString glName = QString("Gl%1%2").arg(font.psName().simplified().replace(QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )).arg(gid);
	if (m_glyphNames.contains(glName))
		return glName;
	FPointArray pts = font.glyphOutline(gid);
	QDomElement ob = m_domDoc.createElement("path");
	ob.setAttribute("d", setClipPath(&pts, true));
	ob.setAttribute("id", glName);
	m_globalDefs.appendChild(ob);
	m_glyphNames.append(glName);
	return glName;
}

QDomElement SVGExPlug::processArrows(PageItem *item, const QDomElement& line, const QString& trans)
{
	QDomElement ob, gr;
	gr = m_domDoc.createElement("g");
	gr.appendChild(line);
	if (item->startArrowIndex() != 0)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(item->startArrowIndex()-1).points.copy();
		if (item->itemType() == PageItem::Line)
		{
			arrowTrans.translate(0, 0);
			arrowTrans.scale(item->startArrowScale() / 100.0, item->startArrowScale() / 100.0);
			if (item->NamedLStyle.isEmpty())
			{
				if (item->lineWidth() != 0.0)
					arrowTrans.scale(item->lineWidth(), item->lineWidth());
			}
			else
			{
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
			}
			arrowTrans.scale(-1,1);
		}
		else
		{
			FPoint Start = item->PoLine.point(0);
			for (int xx = 1; xx < item->PoLine.size(); xx += 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(item->startArrowScale() / 100.0, item->startArrowScale() / 100.0);
					if (item->NamedLStyle.isEmpty())
					{
						if (item->lineWidth() != 0.0)
							arrowTrans.scale(item->lineWidth(), item->lineWidth());
					}
					else
					{
						multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
						if (ml[ml.size()-1].Width != 0.0)
							arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
					}
					break;
				}
			}
		}
		arrow.map(arrowTrans);
		if (item->NamedLStyle.isEmpty())
		{
			ob = m_domDoc.createElement("path");
			ob.setAttribute("d", setClipPath(&arrow, true));
			ob.setAttribute("transform", trans);
			QString aFill;
			if (!item->strokePattern().isEmpty())
			{
				QString pattID = item->strokePattern()+IToStr(m_pattCount);
				m_pattCount++;
				ScPattern pa = m_Doc->docPatterns[item->strokePattern()];
				QDomElement patt = m_domDoc.createElement("pattern");
				patt.setAttribute("id", pattID);
				patt.setAttribute("height", pa.height);
				patt.setAttribute("width", pa.width);
				patt.setAttribute("patternUnits", "userSpaceOnUse");
				const ScStrokePatternTransform& strokePatTrans = item->strokePatternTransform();
				bool mirrorX, mirrorY;
				item->strokePatternFlip(mirrorX, mirrorY);
				QTransform mpa;
				mpa.translate(-item->lineWidth() / 2.0, -item->lineWidth() / 2.0);
				mpa.translate(strokePatTrans.offsetX, strokePatTrans.offsetY);
				mpa.rotate(strokePatTrans.rotation);
				mpa.shear(-strokePatTrans.skewX, strokePatTrans.skewY);
				mpa.scale(pa.scaleX, pa.scaleY);
				mpa.scale(strokePatTrans.scaleX, strokePatTrans.scaleY);
				if (mirrorX)
					mpa.scale(-1, 1);
				if (mirrorY)
					mpa.scale(1, -1);
				patt.setAttribute("patternTransform", matrixToStr(mpa));
				patt.setAttribute("xlink:href", "#" + item->strokePattern());
				m_globalDefs.appendChild(patt);
				aFill += "fill:url(#" + pattID + ");";
			}
			else if (item->GrTypeStroke > 0)
			{
				QDomElement grad;
				if (item->GrTypeStroke == Gradient_Radial)
				{
					grad = m_domDoc.createElement("radialGradient");
					grad.setAttribute("r", FToStr(sqrt(pow(item->GrStrokeEndX - item->GrStrokeStartX, 2) + pow(item->GrStrokeEndY - item->GrStrokeStartY,2))));
					grad.setAttribute("cx", FToStr(item->GrStrokeStartX));
					grad.setAttribute("cy", FToStr(item->GrStrokeStartY));
				}
				else
				{
					grad = m_domDoc.createElement("linearGradient");
					grad.setAttribute("x1", FToStr(item->GrStrokeStartX));
					grad.setAttribute("y1", FToStr(item->GrStrokeStartY));
					grad.setAttribute("x2", FToStr(item->GrStrokeEndX));
					grad.setAttribute("y2", FToStr(item->GrStrokeEndY));
				}
				bool   isFirst = true;
				double actualStop = 0.0, lastStop = 0.0;
				QList<VColorStop*> cstops = item->stroke_gradient.colorStops();
				for (int cst = 0; cst < item->stroke_gradient.stops(); ++cst)
				{
					actualStop = cstops.at(cst)->rampPoint;
					if ((actualStop != lastStop) || (isFirst))
					{
						QDomElement itcl = m_domDoc.createElement("stop");
						itcl.setAttribute("offset", FToStr(cstops.at(cst)->rampPoint*100) + "%");
						if (cstops.at(cst)->name == CommonStrings::None)
							itcl.setAttribute("stop-opacity", FToStr(0));
						else
							itcl.setAttribute("stop-opacity", FToStr(cstops.at(cst)->opacity));
						itcl.setAttribute("stop-color", setColor(cstops.at(cst)->name, cstops.at(cst)->shade));
						grad.appendChild(itcl);
						lastStop = actualStop;
						isFirst  = false;
					}
				}
				grad.setAttribute("id", "Grad" + IToStr(m_gradCount));
				grad.setAttribute("gradientUnits", "userSpaceOnUse");
				m_globalDefs.appendChild(grad);
				aFill = " fill:url(#Grad" + IToStr(m_gradCount) + ");";
				m_gradCount++;
			}
			else
				aFill = "fill:" + setColor(item->lineColor(), item->lineShade()) + ";";
			if (item->lineTransparency() != 0)
				aFill += " fill-opacity:" + FToStr(1.0 - item->lineTransparency()) + ";";
			ob.setAttribute("style", aFill + " stroke:none;");
			gr.appendChild(ob);
		}
		else
		{
			multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
			if (ml[0].Color != CommonStrings::None)
			{
				ob = m_domDoc.createElement("path");
				ob.setAttribute("d", setClipPath(&arrow, true));
				ob.setAttribute("transform", trans);
				QString aFill = "fill:" + setColor(ml[0].Color, ml[0].Shade) + ";";
				ob.setAttribute("style", aFill + " stroke:none;");
				gr.appendChild(ob);
			}
			for (int it = ml.size()-1; it > 0; it--)
			{
				if (ml[it].Color != CommonStrings::None)
				{
					QDomElement ob5 = m_domDoc.createElement("path");
					ob5.setAttribute("d", setClipPath(&arrow, true));
					ob5.setAttribute("transform", trans);
					QString stroke = "fill:none; stroke:" + setColor(ml[it].Color, ml[it].Shade) + "; stroke-linecap:butt; stroke-linejoin:miter; stroke-dasharray:none;";
					if (ml[it].Width != 0.0)
						stroke += " stroke-width:" + FToStr(ml[it].Width) + ";";
					else
						stroke += " stroke-width:1px;";
					ob5.setAttribute("style", stroke);
					gr.appendChild(ob5);
				}
			}
		}
	}
	if (item->endArrowIndex() != 0)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(item->endArrowIndex()-1).points.copy();
		if (item->itemType() == PageItem::Line)
		{
			arrowTrans.translate(item->width(), 0);
			arrowTrans.scale(item->endArrowScale() / 100.0, item->endArrowScale() / 100.0);
			if (item->NamedLStyle.isEmpty())
			{
				if (item->lineWidth() != 0.0)
					arrowTrans.scale(item->lineWidth(), item->lineWidth());
			}
			else
			{
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size() - 1].Width, ml[ml.size() - 1].Width);
			}
		}
		else
		{
			FPoint End = item->PoLine.point(item->PoLine.size() - 2);
			for (uint xx = item->PoLine.size() - 1; xx > 0; xx -= 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y() - Vector.y(), End.x() - Vector.x()) * (180.0 / M_PI);
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(item->endArrowScale() / 100.0, item->endArrowScale() / 100.0);
					if (item->NamedLStyle.isEmpty())
					{
						if (item->lineWidth() != 0.0)
							arrowTrans.scale(item->lineWidth(), item->lineWidth());
					}
					else
					{
						multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
						if (ml[ml.size() - 1].Width != 0.0)
							arrowTrans.scale(ml[ml.size() - 1].Width, ml[ml.size() - 1].Width);
					}
					break;
				}
			}
		}
		arrow.map(arrowTrans);
		if (item->NamedLStyle.isEmpty())
		{
			ob = m_domDoc.createElement("path");
			ob.setAttribute("d", setClipPath(&arrow, true));
			ob.setAttribute("transform", trans);
			QString aFill;
			if (!item->strokePattern().isEmpty())
			{
				QString pattID = item->strokePattern() + IToStr(m_pattCount);
				m_pattCount++;
				ScPattern pa = m_Doc->docPatterns[item->strokePattern()];
				QDomElement patt = m_domDoc.createElement("pattern");
				patt.setAttribute("id", pattID);
				patt.setAttribute("height", pa.height);
				patt.setAttribute("width", pa.width);
				patt.setAttribute("patternUnits", "userSpaceOnUse");
				const ScStrokePatternTransform& strokePatTrans = item->strokePatternTransform();
				bool mirrorX, mirrorY;
				item->strokePatternFlip(mirrorX, mirrorY);
				QTransform mpa;
				mpa.translate(-item->lineWidth() / 2.0, -item->lineWidth() / 2.0);
				mpa.translate(strokePatTrans.offsetX, strokePatTrans.offsetY);
				mpa.rotate(strokePatTrans.rotation);
				mpa.shear(-strokePatTrans.skewX, strokePatTrans.skewY);
				mpa.scale(pa.scaleX, pa.scaleY);
				mpa.scale(strokePatTrans.scaleX, strokePatTrans.scaleY);
				if (mirrorX)
					mpa.scale(-1, 1);
				if (mirrorY)
					mpa.scale(1, -1);
				patt.setAttribute("patternTransform", matrixToStr(mpa));
				patt.setAttribute("xlink:href", "#" + item->strokePattern());
				m_globalDefs.appendChild(patt);
				aFill += "fill:url(#" + pattID + ");";
			}
			else if (item->GrTypeStroke > 0)
			{
				QDomElement grad;
				if (item->GrTypeStroke == Gradient_Radial)
				{
					grad = m_domDoc.createElement("radialGradient");
					grad.setAttribute("r", FToStr(sqrt(pow(item->GrStrokeEndX - item->GrStrokeStartX, 2) + pow(item->GrStrokeEndY - item->GrStrokeStartY,2))));
					grad.setAttribute("cx", FToStr(item->GrStrokeStartX));
					grad.setAttribute("cy", FToStr(item->GrStrokeStartY));
				}
				else
				{
					grad = m_domDoc.createElement("linearGradient");
					grad.setAttribute("x1", FToStr(item->GrStrokeStartX));
					grad.setAttribute("y1", FToStr(item->GrStrokeStartY));
					grad.setAttribute("x2", FToStr(item->GrStrokeEndX));
					grad.setAttribute("y2", FToStr(item->GrStrokeEndY));
				}
				bool   isFirst = true;
				double actualStop = 0.0, lastStop = 0.0;
				QList<VColorStop*> cstops = item->stroke_gradient.colorStops();
				for (int cst = 0; cst < item->stroke_gradient.stops(); ++cst)
				{
					actualStop = cstops.at(cst)->rampPoint;
					if ((actualStop != lastStop) || (isFirst))
					{
						QDomElement itcl = m_domDoc.createElement("stop");
						itcl.setAttribute("offset", FToStr(cstops.at(cst)->rampPoint*100) + "%");
						if (cstops.at(cst)->name == CommonStrings::None)
							itcl.setAttribute("stop-opacity", FToStr(0));
						else
							itcl.setAttribute("stop-opacity", FToStr(cstops.at(cst)->opacity));
						itcl.setAttribute("stop-color", setColor(cstops.at(cst)->name, cstops.at(cst)->shade));
						grad.appendChild(itcl);
						lastStop = actualStop;
						isFirst  = false;
					}
				}
				grad.setAttribute("id", "Grad" + IToStr(m_gradCount));
				grad.setAttribute("gradientUnits", "userSpaceOnUse");
				m_globalDefs.appendChild(grad);
				aFill = " fill:url(#Grad" + IToStr(m_gradCount) + ");";
				m_gradCount++;
			}
			else
				aFill = "fill:" + setColor(item->lineColor(), item->lineShade()) + ";";
			if (item->lineTransparency() != 0)
				aFill += " fill-opacity:" + FToStr(1.0 - item->lineTransparency()) + ";";
			ob.setAttribute("style", aFill + " stroke:none;");
			gr.appendChild(ob);
		}
		else
		{
			multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
			if (ml[0].Color != CommonStrings::None)
			{
				ob = m_domDoc.createElement("path");
				ob.setAttribute("d", setClipPath(&arrow, true));
				ob.setAttribute("transform", trans);
				QString aFill = "fill:" + setColor(ml[0].Color, ml[0].Shade) + ";";
				ob.setAttribute("style", aFill + " stroke:none;");
				gr.appendChild(ob);
			}
			for (int it = ml.size()-1; it > 0; it--)
			{
				if (ml[it].Color != CommonStrings::None)
				{
					QDomElement ob5 = m_domDoc.createElement("path");
					ob5.setAttribute("d", setClipPath(&arrow, true));
					ob5.setAttribute("transform", trans);
					QString stroke = "fill:none; stroke:" + setColor(ml[it].Color, ml[it].Shade) + "; stroke-linecap:butt; stroke-linejoin:miter; stroke-dasharray:none;";
					if (ml[it].Width != 0.0)
						stroke += " stroke-width:" + FToStr(ml[it].Width) + ";";
					else
						stroke += " stroke-width:1px;";
					ob5.setAttribute("style", stroke);
					gr.appendChild(ob5);
				}
			}
		}
	}
	return gr;
}

QString SVGExPlug::handleMask(PageItem *item, double xOffset, double yOffset)
{
	QDomElement grad;
	QString retVal;
	if (item->GrMask == 0)
		return retVal;

	QString maskID = "Mask" + IToStr(m_maskCount);
	m_maskCount++;
	QDomElement mask = m_domDoc.createElement("mask");
	mask.setAttribute("id", maskID);
	QDomElement ob = m_domDoc.createElement("path");
	ob.setAttribute("d", "M 0 0 L " + FToStr(item->width()) + " 0 L " + FToStr(item->width()) + " " + FToStr(item->height()) + " L 0 " + FToStr(item->height()) + " Z");
	if (item->isGroup())
	{
		QString trans = "translate(" + FToStr(xOffset) + ", " + FToStr(yOffset) + ")";
		if (item->rotation() != 0)
			trans += " rotate(" + FToStr(item->rotation()) + ")";
		ob.setAttribute("transform", trans);
	}
	if ((item->GrMask == GradMask_Pattern) || (item->GrMask == GradMask_PatternLumAlpha))
	{
		QString pattID = item->patternMask() + IToStr(m_pattCount);
		m_pattCount++;
		ScPattern pa = m_Doc->docPatterns[item->patternMask()];
		QDomElement patt = m_domDoc.createElement("pattern");
		patt.setAttribute("id", pattID);
		patt.setAttribute("height", FToStr(pa.height));
		patt.setAttribute("width", FToStr(pa.width));
		patt.setAttribute("patternUnits", "userSpaceOnUse");
		const ScMaskTransform& maskTrans = item->maskTransform();
		bool mirrorX, mirrorY;
		item->maskFlip(mirrorX, mirrorY);
		QTransform mpa;
		mpa.translate(maskTrans.offsetX, maskTrans.offsetY);
		mpa.rotate(maskTrans.rotation);
		mpa.shear(-maskTrans.skewX, maskTrans.skewY);
		mpa.scale(pa.scaleX, pa.scaleY);
		mpa.scale(maskTrans.scaleX, maskTrans.scaleY);
		if (mirrorX)
			mpa.scale(-1, 1);
		if (mirrorY)
			mpa.scale(1, -1);
		patt.setAttribute("patternTransform", matrixToStr(mpa));
		patt.setAttribute("xlink:href", "#" + item->patternMask());
		m_globalDefs.appendChild(patt);
		ob.setAttribute("fill", "url(#" + pattID + ")");
	}
	else if ((item->GrMask == GradMask_Linear) || (item->GrMask == GradMask_Radial) || (item->GrMask == GradMask_LinearLumAlpha) || (item->GrMask == GradMask_RadialLumAlpha))
	{
		if ((item->GrMask == GradMask_Linear) || (item->GrMask == GradMask_LinearLumAlpha))
		{
			grad = m_domDoc.createElement("linearGradient");
			grad.setAttribute("x1", FToStr(item->GrMaskStartX));
			grad.setAttribute("y1", FToStr(item->GrMaskStartY));
			grad.setAttribute("x2", FToStr(item->GrMaskEndX));
			grad.setAttribute("y2", FToStr(item->GrMaskEndY));
		}
		else
		{
			grad = m_domDoc.createElement("radialGradient");
			grad.setAttribute("r", FToStr(sqrt(pow(item->GrMaskEndX - item->GrMaskStartX, 2) + pow(item->GrMaskEndY - item->GrMaskStartY,2))));
			grad.setAttribute("cx", FToStr(item->GrMaskStartX));
			grad.setAttribute("cy", FToStr(item->GrMaskStartY));
			grad.setAttribute("fx", FToStr(item->GrMaskFocalX));
			grad.setAttribute("fy", FToStr(item->GrMaskFocalY));
		}
		double gradientSkew;
		if (item->GrMaskSkew == 90)
			gradientSkew = 1;
		else if (item->GrMaskSkew == 180)
			gradientSkew = 0;
		else if (item->GrMaskSkew == 270)
			gradientSkew = -1;
		else if (item->GrMaskSkew == 390)
			gradientSkew = 0;
		else
			gradientSkew = tan(M_PI / 180.0 * item->GrMaskSkew);
		QTransform qmatrix;
		if (item->GrType == Gradient_Linear)
		{
			qmatrix.translate(item->GrMaskStartX, item->GrMaskStartY);
			qmatrix.shear(-gradientSkew, 0);
			qmatrix.translate(-item->GrMaskStartX, -item->GrMaskStartY);
		}
		else
		{
			double rotEnd = xy2Deg(item->GrMaskEndX - item->GrMaskStartX, item->GrMaskEndY - item->GrMaskStartY);
			qmatrix.translate(item->GrMaskStartX, item->GrMaskStartY);
			qmatrix.rotate(rotEnd);
			qmatrix.shear(gradientSkew, 0);
			qmatrix.translate(0, item->GrMaskStartY * (1.0 - item->GrMaskScale));
			qmatrix.translate(-item->GrMaskStartX, -item->GrMaskStartY);
			qmatrix.scale(1, item->GrMaskScale);
		}
		grad.setAttribute("gradientTransform", matrixToStr(qmatrix));
		grad.setAttribute("id", "Grad" + IToStr(m_gradCount));
		grad.setAttribute("gradientUnits", "userSpaceOnUse");
		QList<VColorStop*> cstops = item->mask_gradient.colorStops();
		for (int cst = 0; cst < item->mask_gradient.stops(); ++cst)
		{
			QDomElement itcl = m_domDoc.createElement("stop");
			itcl.setAttribute("offset", FToStr(cstops.at(cst)->rampPoint*100) + "%");
			if (cstops.at(cst)->name == CommonStrings::None)
				itcl.setAttribute("stop-opacity", FToStr(0));
			else
				itcl.setAttribute("stop-opacity", FToStr(cstops.at(cst)->opacity));
			itcl.setAttribute("stop-color", setColor(cstops.at(cst)->name, cstops.at(cst)->shade));
			grad.appendChild(itcl);
		}
		m_globalDefs.appendChild(grad);
		ob.setAttribute("fill", "url(#Grad" + IToStr(m_gradCount) + ")");
		m_gradCount++;
	}
	if ((item->lineColor() != CommonStrings::None) && (!item->isGroup()))
	{
		ob.setAttribute("stroke", "white");
		if (item->lineWidth() != 0.0)
			ob.setAttribute("stroke-width", FToStr(item->lineWidth()));
		else
			ob.setAttribute("stroke-width", "1px");
	}
	else
		ob.setAttribute("stroke", "none");
	mask.appendChild(ob);
	m_globalDefs.appendChild(mask);

	retVal = "url(#" + maskID + ")";
	return retVal;
}

QString SVGExPlug::getFillStyle(PageItem *item)
{
	QDomElement grad;
	QString fill;
	if (item->asPathText())
		return "fill:none;";
	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		fill = "fill:" + setColor(item->fillColor(), item->fillShade()) + ";";
		if (item->GrType != 0)
		{
			if (item->GrType == Gradient_Pattern)
			{
				QString pattID = item->pattern() + IToStr(m_pattCount);
				m_pattCount++;
				ScPattern pa = m_Doc->docPatterns[item->pattern()];
				QDomElement patt = m_domDoc.createElement("pattern");
				patt.setAttribute("id", pattID);
				patt.setAttribute("height", FToStr(pa.height));
				patt.setAttribute("width", FToStr(pa.width));
				patt.setAttribute("patternUnits", "userSpaceOnUse");
				const ScPatternTransform& patternTrans = item->patternTransform();
				bool mirrorX, mirrorY;
				item->patternFlip(mirrorX, mirrorY);
				QTransform mpa;
				mpa.translate(patternTrans.offsetX, patternTrans.offsetY);
				mpa.rotate(patternTrans.rotation);
				mpa.shear(-patternTrans.skewX, patternTrans.skewY);
				mpa.scale(pa.scaleX, pa.scaleY);
				mpa.scale(patternTrans.scaleX, patternTrans.scaleY);
				if (mirrorX)
					mpa.scale(-1, 1);
				if (mirrorY)
					mpa.scale(1, -1);
				patt.setAttribute("patternTransform", matrixToStr(mpa));
				patt.setAttribute("xlink:href", "#" + item->pattern());
				m_globalDefs.appendChild(patt);
				fill = "fill:url(#" + pattID + ");";
			}
			else
			{
				if (item->GrType == Gradient_Linear)
				{
					grad = m_domDoc.createElement("linearGradient");
					grad.setAttribute("x1", FToStr(item->GrStartX));
					grad.setAttribute("y1", FToStr(item->GrStartY));
					grad.setAttribute("x2", FToStr(item->GrEndX));
					grad.setAttribute("y2", FToStr(item->GrEndY));
				}
				else
				{
					grad = m_domDoc.createElement("radialGradient");
					grad.setAttribute("r", FToStr(sqrt(pow(item->GrEndX - item->GrStartX, 2) + pow(item->GrEndY - item->GrStartY,2))));
					grad.setAttribute("cx", FToStr(item->GrStartX));
					grad.setAttribute("cy", FToStr(item->GrStartY));
					grad.setAttribute("fx", FToStr(item->GrFocalX));
					grad.setAttribute("fy", FToStr(item->GrFocalY));
				}
				double gradientSkew;
				if (item->GrSkew == 90)
					gradientSkew = 1;
				else if (item->GrSkew == 180)
					gradientSkew = 0;
				else if (item->GrSkew == 270)
					gradientSkew = -1;
				else if (item->GrSkew == 390)
					gradientSkew = 0;
				else
					gradientSkew = tan(M_PI / 180.0 * item->GrSkew);
				QTransform qmatrix;
				if (item->GrType == Gradient_Linear)
				{
					qmatrix.translate(item->GrStartX, item->GrStartY);
					qmatrix.shear(-gradientSkew, 0);
					qmatrix.translate(-item->GrStartX, -item->GrStartY);
				}
				else
				{
					double rotEnd = xy2Deg(item->GrEndX - item->GrStartX, item->GrEndY - item->GrStartY);
					qmatrix.translate(item->GrStartX, item->GrStartY);
					qmatrix.rotate(rotEnd);
					qmatrix.shear(gradientSkew, 0);
					qmatrix.translate(0, item->GrStartY * (1.0 - item->GrScale));
					qmatrix.translate(-item->GrStartX, -item->GrStartY);
					qmatrix.scale(1, item->GrScale);
				}
				grad.setAttribute("gradientTransform", matrixToStr(qmatrix));
				grad.setAttribute("id", "Grad" + IToStr(m_gradCount));
				grad.setAttribute("gradientUnits", "userSpaceOnUse");
				bool   isFirst = true;
				double actualStop = 0.0, lastStop = 0.0;
				QList<VColorStop*> cstops = item->fill_gradient.colorStops();
				for (int cst = 0; cst < item->fill_gradient.stops(); ++cst)
				{
					actualStop = cstops.at(cst)->rampPoint;
					if ((actualStop != lastStop) || (isFirst))
					{
						QDomElement itcl = m_domDoc.createElement("stop");
						itcl.setAttribute("offset", FToStr(cstops.at(cst)->rampPoint * 100) + "%");
						if (cstops.at(cst)->name == CommonStrings::None)
							itcl.setAttribute("stop-opacity", FToStr(0));
						else
							itcl.setAttribute("stop-opacity", FToStr(cstops.at(cst)->opacity));
						itcl.setAttribute("stop-color", setColor(cstops.at(cst)->name, cstops.at(cst)->shade));
						grad.appendChild(itcl);
						lastStop = actualStop;
						isFirst  = false;
					}
				}
				m_globalDefs.appendChild(grad);
				fill = "fill:url(#Grad" + IToStr(m_gradCount) + ");";
				m_gradCount++;
			}
		}
		if (item->fillRule)
			fill += " fill-rule:evenodd;";
		else
			fill += " fill-rule:nonzero;";
		if (item->fillTransparency() != 0)
			fill += " fill-opacity:" + FToStr(1.0 - item->fillTransparency()) + ";";
	}
	else
		fill = "fill:none;";
	return fill;
}

void SVGExPlug::writeBasePatterns()
{
	QStringList patterns = m_Doc->getPatternDependencyList(m_Doc->getUsedPatterns());
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = m_Doc->docPatterns[patterns[c]];
		QDomElement patt = m_domDoc.createElement("pattern");
		patt.setAttribute("id", patterns[c]);
		patt.setAttribute("height", FToStr(pa.height));
		patt.setAttribute("width", FToStr(pa.width));
		for (int em = 0; em < pa.items.count(); ++em)
		{
			PageItem* item = pa.items.at(em);
			processItemOnPage(item->gXpos, item->gYpos, item, &patt);
		}
		m_globalDefs.appendChild(patt);
	}
}

void SVGExPlug::writeBaseSymbols()
{
	QStringList patterns = m_Doc->getUsedSymbols();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = m_Doc->docPatterns[patterns[c]];
		QDomElement patt = m_domDoc.createElement("symbol");
		patt.setAttribute("id", "S" + patterns[c]);
		patt.setAttribute("viewbox", "0 0 " +  FToStr(pa.height) + " " + FToStr(pa.width));
		for (int em = 0; em < pa.items.count(); ++em)
		{
			PageItem* item = pa.items.at(em);
			processItemOnPage(item->gXpos, item->gYpos, item, &patt);
		}
		m_globalDefs.appendChild(patt);
	}
}

QString SVGExPlug::getStrokeStyle(PageItem *item)
{
	QString stroke;
	if (item->lineTransparency() != 0)
		stroke = "stroke-opacity:" + FToStr(1.0 - item->lineTransparency()) + ";";
	if (item->lineWidth() != 0.0)
		stroke = "stroke-width:" + FToStr(item->lineWidth()) + ";";
	else
		stroke = "stroke-width:1px;";
	stroke += " stroke-linecap:";
	switch (item->PLineEnd)
	{
		case Qt::FlatCap:
			stroke += "butt;";
			break;
		case Qt::SquareCap:
			stroke += "square;";
			break;
		case Qt::RoundCap:
			stroke += "round;";
			break;
		default:
			stroke += "butt;";
			break;
	}
	stroke += " stroke-linejoin:";
	switch (item->PLineJoin)
	{
		case Qt::MiterJoin:
			stroke += "miter;";
			break;
		case Qt::BevelJoin:
			stroke += "bevel;";
			break;
		case Qt::RoundJoin:
			stroke += "round;";
			break;
		default:
			stroke += "miter;";
			break;
	}
	stroke += " stroke-dasharray:";
	if (item->DashValues.count() != 0)
	{
		for (auto it = item->DashValues.cbegin(); it != item->DashValues.cend(); ++it )
		{
			stroke += IToStr(static_cast<int>(*it)) + " ";
		}
		stroke += "; stroke-dashoffset:" + IToStr(static_cast<int>(item->DashOffset)) + ";";
	}
	else
	{
		if (item->PLineArt == Qt::SolidLine)
			stroke += "none;";
		else
		{
			QString Da = getDashString(item->PLineArt, item->lineWidth());
			if (Da.isEmpty())
				stroke += "none;";
			else
				stroke += Da.replace(" ", ", ") + ";";
		}
	}
	if ((!item->strokePattern().isEmpty()) && (!item->patternStrokePath))
	{
		QString pattID = item->strokePattern() + IToStr(m_pattCount);
		m_pattCount++;
		ScPattern pa = m_Doc->docPatterns[item->strokePattern()];
		QDomElement patt = m_domDoc.createElement("pattern");
		patt.setAttribute("id", pattID);
		patt.setAttribute("height", FToStr(pa.height));
		patt.setAttribute("width", FToStr(pa.width));
		patt.setAttribute("patternUnits", "userSpaceOnUse");
		const ScStrokePatternTransform& strokePatTrans = item->strokePatternTransform();
		bool mirrorX, mirrorY;
		item->strokePatternFlip(mirrorX, mirrorY);
		QTransform mpa;
		mpa.translate(-item->lineWidth() / 2.0, -item->lineWidth() / 2.0);
		mpa.translate(strokePatTrans.offsetX, strokePatTrans.offsetY);
		mpa.rotate(strokePatTrans.rotation);
		mpa.shear(-strokePatTrans.skewX, strokePatTrans.skewY);
		mpa.scale(pa.scaleX, pa.scaleY);
		mpa.scale(strokePatTrans.scaleX, strokePatTrans.scaleY);
		if (mirrorX)
			mpa.scale(-1, 1);
		if (mirrorY)
			mpa.scale(1, -1);
		patt.setAttribute("patternTransform", matrixToStr(mpa));
		patt.setAttribute("xlink:href", "#" + item->strokePattern());
		m_globalDefs.appendChild(patt);
		stroke += " stroke:url(#" + pattID + ");";
	}
	else if (item->GrTypeStroke > 0)
	{
		QDomElement grad;
		if (item->GrTypeStroke == Gradient_Radial)
		{
			grad = m_domDoc.createElement("radialGradient");
			grad.setAttribute("r", FToStr(sqrt(pow(item->GrStrokeEndX - item->GrStrokeStartX, 2) + pow(item->GrStrokeEndY - item->GrStrokeStartY,2))));
			grad.setAttribute("cx", FToStr(item->GrStrokeStartX));
			grad.setAttribute("cy", FToStr(item->GrStrokeStartY));
			grad.setAttribute("fx", FToStr(item->GrStrokeFocalX));
			grad.setAttribute("fy", FToStr(item->GrStrokeFocalY));
		}
		else
		{
			grad = m_domDoc.createElement("linearGradient");
			grad.setAttribute("x1", FToStr(item->GrStrokeStartX));
			grad.setAttribute("y1", FToStr(item->GrStrokeStartY));
			grad.setAttribute("x2", FToStr(item->GrStrokeEndX));
			grad.setAttribute("y2", FToStr(item->GrStrokeEndY));
		}
		bool   isFirst = true;
		double actualStop = 0.0, lastStop = 0.0;
		QList<VColorStop*> cstops = item->stroke_gradient.colorStops();
		for (int cst = 0; cst < item->stroke_gradient.stops(); ++cst)
		{
			actualStop = cstops.at(cst)->rampPoint;
			if ((actualStop != lastStop) || (isFirst))
			{
				QDomElement itcl = m_domDoc.createElement("stop");
				itcl.setAttribute("offset", FToStr(cstops.at(cst)->rampPoint*100) + "%");
				if (cstops.at(cst)->name == CommonStrings::None)
					itcl.setAttribute("stop-opacity", FToStr(0));
				else
					itcl.setAttribute("stop-opacity", FToStr(cstops.at(cst)->opacity));
				itcl.setAttribute("stop-color", setColor(cstops.at(cst)->name, cstops.at(cst)->shade));
				grad.appendChild(itcl);
				lastStop = actualStop;
				isFirst  = false;
			}
		}
		double gradientSkew;
		if (item->GrStrokeSkew == 90)
			gradientSkew = 1;
		else if (item->GrStrokeSkew == 180)
			gradientSkew = 0;
		else if (item->GrStrokeSkew == 270)
			gradientSkew = -1;
		else if (item->GrStrokeSkew == 390)
			gradientSkew = 0;
		else
			gradientSkew = tan(M_PI / 180.0 * item->GrStrokeSkew);
		QTransform qmatrix;
		if (item->GrType == Gradient_Linear)
		{
			qmatrix.translate(item->GrStrokeStartX, item->GrStrokeStartY);
			qmatrix.shear(-gradientSkew, 0);
			qmatrix.translate(-item->GrStrokeStartX, -item->GrStrokeStartY);
		}
		else
		{
			double rotEnd = xy2Deg(item->GrStrokeEndX - item->GrStrokeStartX, item->GrStrokeEndY - item->GrStrokeStartY);
			qmatrix.translate(item->GrStrokeStartX, item->GrStrokeStartY);
			qmatrix.rotate(rotEnd);
			qmatrix.shear(gradientSkew, 0);
			qmatrix.translate(0, item->GrStrokeStartY * (1.0 - item->GrStrokeScale));
			qmatrix.translate(-item->GrStrokeStartX, -item->GrStrokeStartY);
			qmatrix.scale(1, item->GrStrokeScale);
		}
		grad.setAttribute("gradientTransform", matrixToStr(qmatrix));
		grad.setAttribute("id", "Grad" + IToStr(m_gradCount));
		grad.setAttribute("gradientUnits", "userSpaceOnUse");
		m_globalDefs.appendChild(grad);
		stroke += " stroke:url(#Grad" + IToStr(m_gradCount) + ");";
		m_gradCount++;
	}
	else if (item->lineColor() != CommonStrings::None)
	{
		stroke += " stroke:" + setColor(item->lineColor(), item->lineShade()) + ";";
	}
	else
		stroke = "stroke:none;";
	return stroke;
}

QDomElement SVGExPlug::createClipPathElement(FPointArray *ite, QDomElement* pathElem)
{
	QString clipPathStr = setClipPath(ite, true);
	if (clipPathStr.isEmpty())
		return QDomElement();
	QDomElement clipPathElem = m_domDoc.createElement("clipPath");
	clipPathElem.setAttribute("id", "Clip" + IToStr(m_clipCount));
	QDomElement cl = m_domDoc.createElement("path");
	if (pathElem)
		*pathElem = cl;
	cl.setAttribute("d", clipPathStr);
	clipPathElem.appendChild(cl);
	m_globalDefs.appendChild(clipPathElem);
	m_clipCount++;
	return clipPathElem;
}

QString SVGExPlug::setClipPath(FPointArray *ite, bool closed)
{
	QString tmp;
	FPoint np, np1, np2, np3, np4, firstP;
	bool nPath = true;
	bool first = true;

	if (ite->size() <= 3)
		return tmp;

	for (int poi=0; poi<ite->size()-3; poi += 4)
	{
		if (ite->isMarker(poi))
		{
			nPath = true;
			continue;
		}
		if (nPath)
		{
			np = ite->point(poi);
			if ((!first) && (closed) && (np4 == firstP))
				tmp += "Z ";
			tmp += QString("M%1 %2 ").arg(np.x()).arg(np.y());
			nPath = false;
			first = false;
			firstP = np;
			np4 = np;
		}
		np = ite->point(poi);
		np1 = ite->point(poi+1);
		np2 = ite->point(poi+3);
		np3 = ite->point(poi+2);
		if ((np == np1) && (np2 == np3))
			tmp += QString("L%1 %2 ").arg(np3.x()).arg(np3.y());
		else
			tmp += QString("C%1 %2 %3 %4 %5 %6 ").arg(np1.x()).arg(np1.y()).arg(np2.x()).arg(np2.y()).arg(np3.x()).arg(np3.y());
		np4 = np3;
	}
	if (closed)
		tmp += "Z";
	return tmp;
}

QString SVGExPlug::FToStr(double c)
{
	QString cc;
	return cc.setNum(c);
}

QString SVGExPlug::IToStr(int c)
{
	QString cc;
	return cc.setNum(c);
}

QString SVGExPlug::matrixToStr(QTransform &mat)
{
	QString cc("matrix(%1 %2 %3 %4 %5 %6)");
	return  cc.arg(mat.m11()).arg(mat.m12()).arg(mat.m21()).arg(mat.m22()).arg(mat.dx()).arg(mat.dy());
}

QString SVGExPlug::setColor(const QString& farbe, int shad)
{
	if (farbe == CommonStrings::None)
		return "#FFFFFF";
	const ScColor& col = m_Doc->PageColors[farbe];
	return ScColorEngine::getShadeColorProof(col, m_Doc, shad).name();
}

QString SVGExPlug::getMultiStroke(struct SingleLine *sl, PageItem *item)
{
	QString tmp = "fill:none; ";
	tmp += "stroke:" + setColor(sl->Color, sl->Shade) + "; ";
	if (item->fillTransparency() != 0)
		tmp += QString(" stroke-opacity:%1; ").arg(1.0 - item->fillTransparency());
	tmp += QString("stroke-width:%1; ").arg(sl->Width);
	tmp += "stroke-linecap:";
	switch (static_cast<Qt::PenCapStyle>(sl->LineEnd))
	{
		case Qt::FlatCap:
			tmp += "butt;";
			break;
		case Qt::SquareCap:
			tmp += "square;";
			break;
		case Qt::RoundCap:
			tmp += "round;";
			break;
		default:
			tmp += "butt;";
			break;
	}
	tmp += " stroke-linejoin:";
	switch (static_cast<Qt::PenJoinStyle>(sl->LineJoin))
	{
		case Qt::MiterJoin:
			tmp += "miter;";
			break;
		case Qt::BevelJoin:
			tmp += "bevel;";
			break;
		case Qt::RoundJoin:
			tmp += "round;";
			break;
		default:
			tmp += "miter;";
			break;
	}
	tmp += " stroke-dasharray:";
	if (static_cast<Qt::PenStyle>(sl->Dash) == Qt::SolidLine)
		tmp += "none;";
	else
	{
		QString Da = getDashString(sl->Dash, sl->Width);
		if (Da.isEmpty())
			tmp += "none;";
		else
			tmp += Da.replace(" ", ", ") + ";";
	}
	return tmp;
}

SVGExPlug::~SVGExPlug()
{
}
