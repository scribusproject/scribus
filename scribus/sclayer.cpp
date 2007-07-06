/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sclayer.h"

#include <QtAlgorithms>

ScLayer::ScLayer(void)
{
	LNr          = 0;
	Level        = 0;
	blendMode    = 0;
	isPrintable  = true;
	isViewable   = true;
	isEditable   = true;
	flowControl  = true;
	outlineMode  = false;
	transparency = 1.0;
	markerColor  = QColor(0, 0, 0);
	Name         = "New Layer";
}

ScLayer::ScLayer(const QString& name, int level, int nr)
{
	Name         = name;
	LNr          = nr;
	Level        = level;
	blendMode    = 0;
	isPrintable  = true;
	isViewable   = true;
	isEditable   = true;
	flowControl  = true;
	outlineMode  = false;
	transparency = 1.0;
	markerColor  = QColor(0, 0, 0);
	switch (LNr % 7)
	{
		case 0:
			markerColor = Qt::black;
			break;
		case 1:
			markerColor = Qt::red;
			break;
		case 2:
			markerColor = Qt::green;
			break;
		case 3:
			markerColor = Qt::blue;
			break;
		case 4:
			markerColor = Qt::cyan;
			break;
		case 5:
			markerColor = Qt::magenta;
			break;
		case 6:
			markerColor = Qt::yellow;;
			break;
	}
}

bool ScLayer::operator< (const ScLayer& other) const
{
	return (Level < other.Level);
}

const ScLayer*  ScLayers::bottomLayer (void) const
{
	const ScLayer* layer, *bLayer = NULL;
	for (int i = 0; i < this->count(); ++i)
	{
		layer = &this->at(i);
		if (!bLayer || (layer->Level < bLayer->Level))
			bLayer = layer;
	}
	return bLayer;
}

const ScLayer* ScLayers::topLayer(void) const
{
	const ScLayer *layer, *tLayer = NULL;
	for (int i = 0; i < this->count(); ++i)
	{
		layer = &this->at(i);
		if (!tLayer || (layer->Level > tLayer->Level))
			tLayer = layer;
	}
	return tLayer;
}

void ScLayers::levelToLayer (ScLayer& layer, int level) const
{
	uint layerCount = count();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		if (this->at(la2).Level == level)
		{
			const ScLayer& ll  = this->at(la2);
			layer.isViewable   = ll.isViewable;
			layer.isPrintable  = ll.isPrintable;
			layer.LNr         = ll.LNr;
			layer.Name         = ll.Name;
			layer.flowControl  = ll.flowControl;
			layer.transparency = ll.transparency;
			layer.blendMode    = ll.blendMode;
			break;
		}
	}
}

const ScLayer* ScLayers::layerByLevel (int level) const
{
	const ScLayer *layer = NULL;
	for (int i = 0; i < this->count(); ++i)
	{
		layer = &this->at(i);
		if( layer->Level == level)
			return layer;
	}
	return NULL;
}

const ScLayer* ScLayers::layerByNumber (int nr) const
{
	const ScLayer *layer = NULL;
	for (int i = 0; i < this->count(); ++i)
	{
		layer = &this->at(i);
		if( layer->LNr == nr)
			return layer;
	}
	return NULL;
}

const ScLayer* ScLayers::layerByName (const QString& name) const
{
	const ScLayer *layer = NULL;
	for (int i = 0; i < this->count(); ++i)
	{
		layer = &this->at(i);
		if( layer->Name == name)
			return layer;
	}
	return NULL;
}

const ScLayer* ScLayers::layerAbove (int level) const
{
	const ScLayer* top = topLayer();
	if (top)
	{
		const ScLayer *layer, *retlay = top;
		int   maxLevel = top->Level;
		for (int i = 0; i < this->count(); ++i)
		{
			layer = &this->at(i);;
			if (layer->Level > level && layer->Level < maxLevel)
			{
				retlay = layer;
				maxLevel = layer->Level;
			}
		}
		return retlay;
	}
	return NULL;
}

const ScLayer* ScLayers::layerAbove (const ScLayer& layer) const
{
	const ScLayer* top = topLayer();
	if (top)
	{
		const ScLayer *curlay, *retlay = top;
		int   level    = layer.Level;
		int   maxLevel = top->Level;
		for (int i = 0; i < this->count(); ++i)
		{
			curlay = &this->at(i);;
			if (curlay->Level > level && curlay->Level < maxLevel)
			{
				maxLevel = curlay->Level;
				retlay   = curlay;
			}
		}
		return retlay;
	}
	return NULL;
}

const ScLayer* ScLayers::layerBelow (int level) const
{
	const ScLayer* bottom = bottomLayer();
	if (bottom)
	{
		const ScLayer *layer, *retlay = bottom;
		int   minLevel = bottom->Level;
		for (int i = 0; i < this->count(); ++i)
		{
			layer = &this->at(i);;
			if (layer->Level < level && layer->Level > minLevel)
			{
				minLevel = layer->Level;
				retlay   = layer;
			}
		}
		return retlay;
	}
	return NULL;
}

const ScLayer* ScLayers::layerBelow (const ScLayer& layer) const
{
	const ScLayer* bottom = bottomLayer();
	if (bottom)
	{
		const ScLayer *curlay, *retlay = bottom;
		int   level    = layer.Level;
		int   minLevel = bottom->Level;
		for (int i = 0; i < this->count(); ++i)
		{
			curlay = &this->at(i);;
			if (curlay->Level < level && curlay->Level > minLevel)
			{
				minLevel = curlay->Level;
				retlay   = curlay;
			}
		}
		return retlay;
	}
	return NULL;
}

int ScLayers::addLayer(const QString& layerName)
{
	QString lname;
	int     lnr    = (count() > 0) ? (last().LNr + 1) : 1;
	int     llevel = count();
	if (layerName.isEmpty())
	{
		QString tmp;
		lname = QObject::tr("New Layer")+" "+tmp.setNum(lnr);
	}
	else
		lname = layerName;
	ScLayer ll(lname, llevel, lnr);
	append(ll);
	return lnr;
}

bool ScLayers::removeLayerByNumber(int nr)
{
	int index = -1;
	int layerLevel = -1;
	for (int i = 0; i < count(); ++i)
	{
		if (this->at(i).LNr == nr)
		{
			index      = i;
			layerLevel = this->at(i).Level;
			break;
		}
	}
	if (index >= 0)
	{
		removeAt(index);
		ScLayers::Iterator it, itEnd = end();
		for (it = begin(); it != itEnd; ++it)
		{
			if ((*it).Level > layerLevel)
				(*it).Level -= 1;
		}
		return true;
	}
	return false;
}

bool ScLayers::removeLayerByLevel(int level)
{
	int index = -1;
	for (int i = 0; i < count(); ++i)
	{
		if (this->at(i).Level == level)
		{
			index = i;
			break;
		}
	}
	if (index >= 0)
	{
		removeAt(index);
		ScLayers::Iterator it, itEnd = end();
		for (it = begin(); it != itEnd; ++it)
		{
			if ((*it).Level > level)
				(*it).Level -= 1;
		}
	}
	return false;
}

void ScLayers::sort(void)
{
	qStableSort(begin(), end());
}
