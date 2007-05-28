/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          dynamictip.cpp  -  description
                             -------------------
    begin                : Wed Aug 31 2005
    copyright            : (C) 2005 by Franz Schmid
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

#include "dynamictip.h"
#include "seiten.h"
#include "tree.h"
#include "pageitem.h"

// Qt4 FIXME: QToolTip is totally rewritten in the Qt4. We have to use new API.
// Qt4 FIXME: all references and calls of DynamicTip are commented out

// DynamicTip::DynamicTip( Q3ListBox* parent, ColorList* pale ) : QToolTip( parent )
// {
// 	colorList = pale;
// 	listB = parent;
// 	kind = ColorListBox;
// }
// 
// DynamicTip::DynamicTip( Q3Table* parent ) : QToolTip( parent->viewport() )
// {
// 	table = parent;
// 	kind = Table;
// }
// 
// DynamicTip::DynamicTip( Q3Header *parent ) : QToolTip( parent )
// {
// 	header = parent;
// 	kind = TableHeader;
// }
// 
// DynamicTip::DynamicTip( Q3ListView* parent ) : QToolTip( parent->viewport() )
// {
// 	listV = parent;
// 	kind = TreeView;
// }
// 
// void DynamicTip::maybeTip( const QPoint &pos )
// {
// 	if (kind == ColorListBox)
// 	{
// 		Q3ListBoxItem* it = listB->itemAt(pos);
// 		if (it != 0)
// 		{
// 			if (!colorList->contains(it->text()))
// 				return;
// 			QString tipText = "";
// 			ScColor col = (*colorList)[it->text()];
// 			if (col.getColorModel() == colorModelCMYK)
// 			{
// 				int c, m, y, k;
// 				col.getCMYK(&c, &m, &y, &k);
// 				tipText = QString("C:%1% M:%2% Y:%3% K:%4%").arg(qRound(c / 2.55)).arg(qRound(m / 2.55)).arg(qRound(y / 2.55)).arg(qRound(k / 2.55));
// 			}
// 			else
// 			{
// 				int r, g, b;
// 				col.getRawRGBColor(&r, &g, &b);
// 				tipText = QString("R:%1 G:%2 B:%3").arg(r).arg(g).arg(b);
// 			}
// 			tip(listB->itemRect(it), tipText);
// 		}
// 	}
// 	else if (kind == Table)
// 	{
// 		QPoint cp = table->viewportToContents( pos );
// 		int row = table->rowAt( cp.y() );
// 		int col = table->columnAt( cp.x() );
// 		Q3TableItem* ite = table->item(row, col);
// 		if (ite == 0)
// 			return;
// 		QRect cr = table->cellGeometry( row, col );
// 		cr.moveTopLeft( table->contentsToViewport( cr.topLeft() ) );
// 		SeItem* it = (SeItem*)ite;
// 		QString tipString = it->getPageName();
// 		tip( cr, tipString );
// 	}
// 	else if (kind == TableHeader)
// 	{
// 		int col = header->sectionAt(pos.x());
// 		tip( header->sectionRect(col), headerTips[col] );
// 	}
// 	else if (kind == TreeView)
// 	{
// 		Q3ListViewItem* it = listV->itemAt(pos);
// 		if (it != 0)
// 		{
// 			TreeItem *item = (TreeItem*)it;
// 			if (item != NULL)
// 			{
// 				QString tipText = "";
// 				if ((item->type == 1) || (item->type == 3) || (item->type == 4))
// 				{
// 					PageItem *pgItem = item->PageItemObject;
// 					switch (pgItem->itemType())
// 					{
// 						case PageItem::ImageFrame:
// 							tipText = QObject::tr("Image");
// 							break;
// 						case PageItem::TextFrame:
// 							switch (pgItem->annotation().Type())
// 							{
// 								case 2:
// 									tipText = QObject::tr("PDF Push Button");
// 									break;
// 								case 3:
// 									tipText = QObject::tr("PDF Text Field");
// 									break;
// 								case 4:
// 									tipText = QObject::tr("PDF Check Box");
// 									break;
// 								case 5:
// 									tipText = QObject::tr("PDF Combo Box");
// 									break;
// 								case 6:
// 									tipText = QObject::tr("PDF List Box");
// 									break;
// 								case 10:
// 									tipText = QObject::tr("PDF Text Annotation");
// 									break;
// 								case 11:
// 									tipText = QObject::tr("PDF Link Annotation");
// 									break;
// 								default:
// 									tipText = QObject::tr("Text");
// 									break;
// 							}
// 							break;
// 						case PageItem::Line:
// 							tipText = QObject::tr("Line");
// 							break;
// 						case PageItem::Polygon:
// 							tipText = QObject::tr("Polygon");
// 							break;
// 						case PageItem::PolyLine:
// 							tipText = QObject::tr("Polyline");
// 							break;
// 						case PageItem::PathText:
// 							tipText = QObject::tr("PathText");
// 							break;
// 						default:
// 							break;
// 					}
// 					tip(listV->itemRect(it), tipText);
// 				}
// 			}
// 		}
// 	}
// }
// 
// void DynamicTip::addHeaderTip( QString tip )
// {
// 	headerTips.append(tip);
// }
// 
// void DynamicTip::clearHeaderTips()
// {
// 	headerTips.clear();
// }
