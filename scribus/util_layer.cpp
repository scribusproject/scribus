/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          util.cpp  -  description
                             -------------------
    begin                : Fri Sep 14 2001
    copyright            : (C) 2001 by Franz Schmid
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

#include "scconfig.h"
#include "sclayer.h"
#include "scribusdoc.h"
#include "util_layer.h"

#include <QMap>

using namespace std;

void handleOldLayerBehavior(ScribusDoc* doc)
{
	if (doc->Layers.count() <= 1)
		return;
	if (doc->MasterItems.count() <= 0)
		return;
	
	// Sort layers to be sure the levels are properly set
	doc->Layers.sort();

	// Create new background layers for master page elements
	ScLayers newLayers;
	QMap<int, int> layerMap;
	int layersCount = doc->Layers.count();
	// Create layers for std pages elements, keep old layer id
	for (int i = 0; i < doc->Layers.count(); ++i)
	{
		const ScLayer* layer = doc->Layers.layerByLevel(i);
		if (layer)
		{
			// Change level of std elements layer
			ScLayer stdElemLayer(*layer);
			stdElemLayer.Level = i + layersCount;
			newLayers.append(stdElemLayer);
		}
	}
	// Create layers for master pages elements with new layer id
	for (int i = 0; i < doc->Layers.count(); ++i)
	{
		const ScLayer* layer = doc->Layers.layerByLevel(i);
		if (layer)
		{
			// Create new layer for master elements
			ScLayer masterElemLayer(*layer);
			int oldId = masterElemLayer.ID;
			int newId = newLayers.getMaxID() + 1;
			masterElemLayer.ID  = newId;
			masterElemLayer.Name = layer->Name + QObject::tr(" (mp items)");
			newLayers.append(masterElemLayer);
			layerMap.insert(oldId, newId);
		}
	}
	newLayers.sort();
	doc->Layers = newLayers;
	
	// Reassign layer of master page elements
	PageItem* item;
	QMap<int, int>::iterator idIt;
	int mpCount = doc->MasterItems.count();
	for (int i = 0; i < mpCount; ++i)
	{
		item = doc->MasterItems.at(i);
		idIt = layerMap.find(item->LayerID);
		if (idIt != layerMap.end())
			item->LayerID = idIt.value();
	}
}
