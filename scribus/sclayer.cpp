/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sclayer.h"

#include <QHash>   //necessary to avoid msvc warnings induced by SCRIBUS_API on ScLayers + early instanciation of templates
#include <QVector> //necessary to avoid msvc warnings induced by SCRIBUS_API on ScLayers + early instanciation of templates
#include <QtAlgorithms>

ScLayer::ScLayer(void)
{
	Name         = QObject::tr("New Layer");
	LNr          = 0;
	Level        = 0;
	isPrintable  = true;
	isViewable   = true;
	isEditable   = true;
	flowControl  = true;
	outlineMode  = false;
	transparency = 1.0;
	blendMode    = 0;
	markerColor  = QColor(0, 0, 0);
}

ScLayer::ScLayer(const QString& name, int level, int nr)
{
	Name         = name;
	LNr          = nr;
	Level        = level;
	isPrintable  = true;
	isViewable   = true;
	isEditable   = true;
	flowControl  = true;
	outlineMode  = false;
	transparency = 1.0;
	blendMode    = 0;
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

bool ScLayer::operator== (const ScLayer& other) const
{
	// ignore markerColor?
	if (Name == other.Name && LNr == other.LNr && Level == other.Level      &&
		isPrintable  == other.isPrintable  && isViewable  == other.isViewable  &&
		flowControl  == other.flowControl  && outlineMode == other.outlineMode && 
		transparency == other.transparency && isEditable == other.isEditable &&
		blendMode   == other.blendMode)
	{
		return true;
	}
	return false;
}

int ScLayers::getMaxNumber(void)
{
	int nr, maxNr = -1;
	for (int i = 0; i < this->count(); ++i)
	{
		nr = this->at(i).LNr;;
		if (nr > maxNr)
			maxNr = nr;
	}
	return maxNr;
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
			layer.isEditable   = ll.isEditable;
			layer.LNr          = ll.LNr;
			layer.Name         = ll.Name;
			layer.flowControl  = ll.flowControl;
			layer.transparency = ll.transparency;
			layer.blendMode    = ll.blendMode;
			break;
		}
	}
}

ScLayer* ScLayers::byLevel(const int level)
{
	ScLayers::Iterator itend = end();
	for (ScLayers::Iterator it = 0; it != itend; ++it)
	{
		if( it->Level == level)
			return &(*it);
	}
	return NULL;
}

ScLayer* ScLayers::byNumber(const int nr)
{
	ScLayers::Iterator itend = end();
	for (ScLayers::Iterator it = 0; it != itend; ++it)
	{
		if( it->LNr == nr)
			return &(*it);
	}
	return NULL;
}

ScLayer* ScLayers::bottom(void)
{
	ScLayer* bLayer = NULL;
	ScLayers::Iterator it, itEnd = end();
	for (it = begin(); it != itEnd; ++it)
	{
		if (!bLayer || (it->Level < bLayer->Level))
			bLayer = &(*it);
	}
	return bLayer;
}

ScLayer* ScLayers::top(void)
{
	ScLayer *tLayer = NULL;
	ScLayers::Iterator it, itEnd = end();
	for (it = begin(); it != itEnd; ++it)
	{
		if (!tLayer || (it->Level > tLayer->Level))
			tLayer = &(*it);
	}
	return tLayer;
}

ScLayer* ScLayers::above (int nr)
{
	ScLayer* lyr = byNumber(nr);
	if (lyr)
	{
		ScLayer *rlyr = top();
		int level     = lyr->Level;
		int maxLevel  = rlyr->Level;
		ScLayers::Iterator it, itEnd = end();
		for (it = begin(); it != itEnd; ++it)
		{
			if (it->Level > level && it->Level < maxLevel)
			{
				maxLevel = it->Level;
				rlyr     = &(*it);
			}
		}
		return rlyr;
	}
	return NULL;
}

ScLayer* ScLayers::below (int nr)
{
	ScLayer* lyr = byNumber(nr);
	if (lyr)
	{
		ScLayer *rlyr = bottom();
		int level     = lyr->Level;
		int minLevel  = lyr->Level;
		ScLayers::Iterator it, itEnd = end();
		for (it = begin(); it != itEnd; ++it)
		{
			if (it->Level < level && it->Level > minLevel)
			{
				minLevel = it->Level;
				rlyr     = &(*it);
			}
		}
		return rlyr;
	}
	return NULL;
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
			layer = &this->at(i);
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
			curlay = &this->at(i);
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
			layer = &this->at(i);
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
			curlay = &this->at(i);
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
	ScLayer* nl = newLayer(layerName);
	if (nl)
		return nl->LNr;
	return -1;
}

int ScLayers::addLayer(const ScLayer& layer)
{
	int   newID = layer.LNr;
	const ScLayer* lid = layerByNumber(newID);
	if (lid)
	{
		ScLayer newLyr(layer);
		newLyr.LNr = newID = getMaxNumber() + 1;
		append(newLyr);
	}
	else
		append(layer);
	sort();
	return newID;
}

ScLayer* ScLayers::newLayer(const QString& layerName)
{
	QString lname;
	int     lnr    = getMaxNumber() + 1;
	int     llevel = count();
	if (layerName.isEmpty())
	{
		QString tmp;
		lname = QObject::tr("New Layer")+" "+tmp.setNum(lnr);
	}
	else
		lname = layerName;
	ScLayer ll(lname, llevel, lnr);
	ScLayers::Iterator it = insert(end(), ll);
	return &(*it);
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

bool ScLayers::raiseLayer(int nr)
{
	ScLayer* clyr = byNumber(nr);
	ScLayer* alyr = above(clyr->LNr);
	if (clyr && (clyr != alyr))
	{
		clyr->LNr += 1;
		alyr->LNr -= 1;
		return true;
	}
	return false;
}

bool ScLayers::lowerLayer(int nr)
{
	ScLayer* clyr = byNumber(nr);
	ScLayer* blyr = below(clyr->LNr);
	if (clyr && (clyr != blyr))
	{
		clyr->LNr -= 1;
		blyr->LNr += 1;
		return true;
	}
	return false;
}

void ScLayers::sort(void)
{
	int level = 0;
	ScLayers::Iterator it, itend = end();
	qStableSort(begin(), end());
	for(it = begin(); it != itend; ++it, ++level)
		it->Level = level;
}

bool ScLayers::layerPrintable(const int layerNumber) const
{
	const ScLayer* layer = layerByNumber(layerNumber);
	if (layer)
		return layer->isPrintable;
	return false;
}

bool ScLayers::setLayerPrintable(const int layerNumber, const bool isPrintable)
{
	ScLayer* layer = byNumber(layerNumber);
	if (layer)
	{
		layer->isPrintable = isPrintable;
		return true;
	}
	return false;
}

bool ScLayers::layerVisible(const int layerNumber) const
{
	const ScLayer* layer = layerByNumber(layerNumber);
	if (layer)
		return layer->isViewable;
	return false;
}

bool ScLayers::setLayerVisible(const int layerNumber, const bool isViewable)
{
	ScLayer* layer = byNumber(layerNumber);
	if (layer)
	{
		layer->isViewable = isViewable;
		return true;
	}
	return false;
}

bool ScLayers::layerLocked(const int layerNumber) const
{
	const ScLayer* layer = layerByNumber(layerNumber);
	if (layer)
		return !(layer->isEditable);
	return false;
}

bool ScLayers::setLayerLocked(const int layerNumber, const bool isLocked)
{
	ScLayer* layer = byNumber(layerNumber);
	if (layer)
	{
		layer->isEditable = !isLocked;
		return true;
	}
	return false;
}

bool ScLayers::layerFlow(const int layerNumber) const
{
	const ScLayer* layer = layerByNumber(layerNumber);
	if (layer)
		return layer->flowControl;
	return false;
}

bool ScLayers::setLayerFlow(const int layerNumber, const bool flow)
{
	ScLayer* layer = byNumber(layerNumber);
	if (layer)
	{
		layer->flowControl = flow;
		return true;
	}
	return false;
}

bool ScLayers::layerOutline(const int layerNumber) const
{
	const ScLayer* layer = layerByNumber(layerNumber);
	if (layer)
		return layer->outlineMode;
	return false;
}

bool ScLayers::setLayerOutline(const int layerNumber, const bool outline)
{
	ScLayer* layer = byNumber(layerNumber);
	if (layer)
	{
		layer->outlineMode = outline;
		return true;
	}
	return false;
}

double ScLayers::layerTransparency(const int layerNumber) const
{
	const ScLayer* layer = layerByNumber(layerNumber);
	if (layer)
		return layer->transparency;
	return 1.0;
}

bool ScLayers::setLayerTransparency(const int layerNumber, double trans)
{
	ScLayer* layer = byNumber(layerNumber);
	if (layer)
	{
		layer->transparency = trans;
		return true;
	}
	return false;
}

int ScLayers::layerBlendMode(const int layerNumber) const
{
	const ScLayer* layer = layerByNumber(layerNumber);
	if (layer)
		return layer->blendMode;
	return 0;
}

bool ScLayers::setLayerBlendMode(const int layerNumber, int blend)
{
	ScLayer* layer = byNumber(layerNumber);
	if (layer)
	{
		layer->blendMode = blend;
		return true;
	}
	return false;
}

QColor ScLayers::layerMarker(const int layerNumber) const
{
	const ScLayer* layer = layerByNumber(layerNumber);
	if (layer)
		return layer->markerColor;
	return Qt::black;
}

bool ScLayers::setLayerMarker(const int layerNumber, QColor color)
{
	ScLayer* layer = byNumber(layerNumber);
	if (layer)
	{
		layer->markerColor = color;
		return true;
	}
	return false;
}

uint qHash(const ScLayer& layer)
{ 
	return qHash(&layer);
}
