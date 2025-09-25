#include <utility>

/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sclayer.h"

#include <QHash>   //necessary to avoid msvc warnings induced by SCRIBUS_API on ScLayers + early instantiation of templates
#include <QObject>
#include <QVector> //necessary to avoid msvc warnings induced by SCRIBUS_API on ScLayers + early instantiation of templates
#include <QtAlgorithms>

ScLayer::ScLayer()
{
	Name = QObject::tr("New Layer");
}

ScLayer::ScLayer(const QString& name, int level, int id)
{
	Name         = name;
	ID           = id;
	Level        = level;
	switch (ID % 7)
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
			markerColor = Qt::yellow;
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
	return Name == other.Name &&
		ID == other.ID &&
		Level == other.Level &&
		isPrintable == other.isPrintable &&
		isViewable  == other.isViewable &&
		flowControl == other.flowControl &&
		outlineMode == other.outlineMode &&
		transparency == other.transparency &&
		isEditable == other.isEditable &&
		blendMode == other.blendMode;
}

int ScLayers::getMaxID() const
{
	int maxNr = -1;
	for (int i = 0; i < this->count(); ++i)
	{
		int nr = this->at(i).ID;
		if (nr > maxNr)
			maxNr = nr;
	}
	return maxNr;
}

const ScLayer*  ScLayers::bottomLayer () const
{
	const ScLayer* layer;
	const ScLayer *bLayer = nullptr;
	for (int i = 0; i < this->count(); ++i)
	{
		layer = &this->at(i);
		if (!bLayer || (layer->Level < bLayer->Level))
			bLayer = layer;
	}
	return bLayer;
}

const ScLayer* ScLayers::topLayer() const
{
	const ScLayer *layer;
	const ScLayer *tLayer = nullptr;
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
	const auto* pLayer = layerByLevel(level);
	if (!pLayer)
		return;

	layer.isViewable   = pLayer->isViewable;
	layer.isPrintable  = pLayer->isPrintable;
	layer.isEditable   = pLayer->isEditable;
	layer.isSelectable = pLayer->isSelectable;
	layer.ID           = pLayer->ID;
	layer.Name         = pLayer->Name;
	layer.flowControl  = pLayer->flowControl;
	layer.transparency = pLayer->transparency;
	layer.blendMode    = pLayer->blendMode;
}

ScLayer* ScLayers::byLevel(int level)
{
	for (auto& layer : *this)
	{
		if (layer.Level == level)
			return &layer;
	}
	return nullptr;
}

ScLayer* ScLayers::byID(int nr)
{
	for (auto& layer : *this)
	{
		if (layer.ID == nr)
			return &layer;
	}
	return nullptr;
}

ScLayer* ScLayers::bottom()
{
	ScLayer* bLayer = nullptr;
	for (auto& layer : *this)
	{
		if (!bLayer || (layer.Level < bLayer->Level))
			bLayer = &layer;
	}
	return bLayer;
}

ScLayer* ScLayers::top()
{
	ScLayer *tLayer = nullptr;
	for (auto& layer : *this)
	{
		if (!tLayer || (layer.Level > tLayer->Level))
			tLayer = &layer;
	}
	return tLayer;
}

ScLayer* ScLayers::above (int nr)
{
	const ScLayer* lyr = byID(nr);
	if (!lyr)
		return nullptr;

	ScLayer *rlyr = top();
	int level = lyr->Level;
	int maxLevel = rlyr->Level;
	for (auto& layer : *this)
	{
		if (layer.Level > level && layer.Level < maxLevel)
		{
			maxLevel = layer.Level;
			rlyr     = &layer;
		}
	}
	return rlyr;
}

ScLayer* ScLayers::below (int nr)
{
	const ScLayer* lyr = byID(nr);
	if (!lyr)
		return nullptr;

	ScLayer *rlyr = bottom();
	int level = lyr->Level;
	int minLevel = lyr->Level;
	for (auto& layer : *this)
	{
		if (layer.Level < level && layer.Level > minLevel)
		{
			minLevel = layer.Level;
			rlyr     = &layer;
		}
	}
	return rlyr;
}

const ScLayer* ScLayers::layerByLevel (int level) const
{
	for (const auto& layer : *this)
	{
		if (layer.Level == level)
			return &layer;
	}
	return nullptr;
}

const ScLayer* ScLayers::layerByID (int nr) const
{
	for (const auto& layer : *this)
	{
		if (layer.ID == nr)
			return &layer;
	}
	return nullptr;
}

const ScLayer* ScLayers::layerByName (const QString& name) const
{
	for (const auto& layer : *this)
	{
		if (layer.Name == name)
			return &layer;
	}
	return nullptr;
}

const ScLayer* ScLayers::layerAbove (int level) const
{
	const ScLayer* top = topLayer();
	if (!top)
		return nullptr;

	const ScLayer* retlay = top;
	int   maxLevel = top->Level;
	for (const auto& layer : *this)
	{
		if (layer.Level > level && layer.Level < maxLevel)
		{
			retlay = &layer;
			maxLevel = layer.Level;
		}
	}
	return retlay;
}

const ScLayer* ScLayers::layerAbove (const ScLayer& layer) const
{
	const auto* retLayer = layerAbove(layer.Level);
	return retLayer;
}

const ScLayer* ScLayers::layerBelow (int level) const
{
	const ScLayer* bottom = bottomLayer();
	if (!bottom)
		return nullptr;

	const ScLayer* retlay = bottom;
	int   minLevel = bottom->Level;
	for (const auto& layer : *this)
	{
		if (layer.Level < level && layer.Level > minLevel)
		{
			minLevel = layer.Level;
			retlay   = &layer;
		}
	}
	return retlay;
}

const ScLayer* ScLayers::layerBelow (const ScLayer& layer) const
{
	const auto* retLayer = layerBelow(layer.Level);
	return retLayer;
}

int ScLayers::addLayer(const QString& layerName)
{
	const ScLayer* nl = newLayer(layerName);
	if (nl)
		return nl->ID;
	return -1;
}

int ScLayers::addLayer(const ScLayer& layer)
{
	int newID = layer.ID;
	const ScLayer* lid = layerByID(newID);
	if (lid)
	{
		ScLayer newLyr(layer);
		newLyr.ID = newID = getMaxID() + 1;
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
	int lId = getMaxID() + 1;
	int llevel = count();
	if (layerName.isEmpty())
	{
		QString tmp;
		lname = QObject::tr("New Layer") + " " + tmp.setNum(lId);
	}
	else
		lname = layerName;
	ScLayer ll(lname, llevel, lId);
	ScLayers::Iterator it = insert(end(), ll);
	return &(*it);
}

bool ScLayers::removeLayerByID(int id)
{
	int index = -1;
	int layerLevel = -1;
	for (int i = 0; i < count(); ++i)
	{
		if (this->at(i).ID == id)
		{
			index      = i;
			layerLevel = this->at(i).Level;
			break;
		}
	}
	if (index >= 0)
	{
		removeAt(index);
		for (auto& layer : *this)
		{
			if (layer.Level > layerLevel)
				layer.Level -= 1;
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
		for (auto& layer : *this)
		{
			if (layer.Level > level)
				layer.Level -= 1;
		}
	}
	return false;
}

bool ScLayers::raiseLayer(int nr)
{
	ScLayer* clyr = byID(nr);
	if (!clyr)
		return false;

	ScLayer* alyr = above(clyr->ID);
	if ((!alyr) || (clyr == alyr))
		return false;
	
	clyr->Level += 1;
	alyr->Level -= 1;
	return true;
}

bool ScLayers::lowerLayer(int nr)
{
	ScLayer* clyr = byID(nr);
	if (!clyr)
		return false;

	ScLayer* blyr = below(clyr->ID);
	if (!blyr || (clyr == blyr))
		return false;

	clyr->Level -= 1;
	blyr->Level += 1;
	return true;
}

void ScLayers::sort()
{
	int level = 0;
	std::stable_sort(begin(), end());
	for (auto it = begin(); it != end(); ++it, ++level)
		it->Level = level;
}

bool ScLayers::layerPrintable(int layerID) const
{
	const ScLayer* layer = layerByID(layerID);
	if (!layer)
		return false;
	return layer->isPrintable;
}

bool ScLayers::setLayerPrintable(int layerID, bool isPrintable)
{
	ScLayer* layer = byID(layerID);
	if (!layer)
		return false;
	layer->isPrintable = isPrintable;
	return true;
}

bool ScLayers::layerVisible(int layerID) const
{
	const ScLayer* layer = layerByID(layerID);
	if (!layer)
		return false;
	return layer->isViewable;
}

bool ScLayers::setLayerVisible(int layerID, bool isViewable)
{
	ScLayer* layer = byID(layerID);
	if (!layer)
		return false;
	layer->isViewable = isViewable;
	return true;
}

bool ScLayers::layerLocked(int layerID) const
{
	const ScLayer* layer = layerByID(layerID);
	if (!layer)
		return false;
	return !(layer->isEditable);
}

bool ScLayers::setLayerLocked(int layerID, bool isLocked)
{
	ScLayer* layer = byID(layerID);
	if (!layer)
		return false;
	layer->isEditable = !isLocked;
	return true;
}

bool ScLayers::layerFlow(int layerID) const
{
	const ScLayer* layer = layerByID(layerID);
	if (!layer)
		return false;
	return layer->flowControl;
}

bool ScLayers::setLayerFlow(int layerID, bool flow)
{
	ScLayer* layer = byID(layerID);
	if (!layer)
		return false;
	layer->flowControl = flow;
	return true;
}

bool ScLayers::layerOutline(int layerID) const
{
	const ScLayer* layer = layerByID(layerID);
	if (!layer)
		return false;
	return layer->outlineMode;
}

bool ScLayers::setLayerOutline(int layerID, bool outline)
{
	ScLayer* layer = byID(layerID);
	if (!layer)
		return false;
	layer->outlineMode = outline;
	return true;
}

double ScLayers::layerTransparency(int layerID) const
{
	const ScLayer* layer = layerByID(layerID);
	if (!layer)
		return 1.0;
	return layer->transparency;
}

bool ScLayers::setLayerTransparency(int layerID, double trans)
{
	ScLayer* layer = byID(layerID);
	if (!layer)
		return false;
	layer->transparency = trans;
	return true;
}

int ScLayers::layerBlendMode(int layerID) const
{
	const ScLayer* layer = layerByID(layerID);
	if (!layer)
		return 0;
	return layer->blendMode;
}

bool ScLayers::setLayerBlendMode(int layerID, int blend)
{
	ScLayer* layer = byID(layerID);
	if (!layer)
		return false;
	layer->blendMode = blend;
	return true;
}

QColor ScLayers::layerMarker(int layerID) const
{
	const ScLayer* layer = layerByID(layerID);
	if (!layer)
		return Qt::black;
	return layer->markerColor;
}

bool ScLayers::setLayerMarker(int layerID, QColor color)
{
	ScLayer* layer = byID(layerID);
	if (!layer)
		return false;
	layer->markerColor = color;
	return true;
}

bool ScLayers::layerSelectable(int layerID) const
{
	const ScLayer* layer = layerByID(layerID);
	if (!layer)
		return false;
	return layer->isSelectable;
}

bool ScLayers::setLayerSelectable(int layerID, bool isSelectable)
{
	ScLayer* layer = byID(layerID);
	if (!layer)
		return false;
	layer->isSelectable = isSelectable;
	return true;
}

size_t qHash(const ScLayer& layer)
{ 
	return qHash(&layer);
}
