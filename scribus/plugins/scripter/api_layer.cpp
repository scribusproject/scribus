/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_layer.h"
#include "utils.h"
#include "api_textitem.h"
#include "api_imageitem.h"

LayerAPI::LayerAPI(ScLayer *l) : QObject(COLLECTOR)
{
	qDebug() << "ScLayerWrapper loaded";
	setObjectName("layer");
	innerLayer = l;
}


QString LayerAPI::getName()
{
	return innerLayer->Name;
}

void LayerAPI::setName(QString name)
{
	innerLayer->Name = name;
}

int LayerAPI::getID()
{
	return innerLayer->ID;
}

int LayerAPI::getLevel()
{
	return innerLayer->Level;
}

void LayerAPI::setLevel(int level)
{
	innerLayer->Level = level;
}

bool LayerAPI::isPrintable()
{
	return innerLayer->isPrintable;
}

void LayerAPI::setPrintable(bool value)
{
	innerLayer->isPrintable = value;
}

bool LayerAPI::isViewable()
{
	return innerLayer->isViewable;
}

void LayerAPI::setViewable(bool value)
{
	innerLayer->isViewable = value;
}

bool LayerAPI::isEditable()
{
	return innerLayer->isEditable;
}

void LayerAPI::setEditable(bool value)
{
	innerLayer->isEditable = value;
}

bool LayerAPI::getFlowControl()
{
	return innerLayer->flowControl;
}

void LayerAPI::setFlowControl(bool value)
{
	innerLayer->flowControl = value;
}

bool LayerAPI::getOutlineMode()
{
	return innerLayer->outlineMode;
}

void LayerAPI::setOutlineMode(bool value)
{
	innerLayer->outlineMode = value;
}

double LayerAPI::getTransparency()
{
	return innerLayer->transparency;
}

void LayerAPI::setTransparency(double value)
{
	innerLayer->transparency = value;
}

int LayerAPI::getBlendMode()
{
	return innerLayer->blendMode;
}

void LayerAPI::setBlendMode(int value)
{
	innerLayer->blendMode = value;
}

bool LayerAPI::isActive()
{
	if (ScCore->primaryMainWindow()->doc->activeLayerName() == innerLayer->Name)
		return true;
	return false;
}

void LayerAPI::setActive(bool value)
{
	if (value)
	{
		bool found = ScCore->primaryMainWindow()->doc->setActiveLayer(innerLayer->Name);
		if (found)
			ScCore->primaryMainWindow()->changeLayer(ScCore->primaryMainWindow()->doc->activeLayer());
	}
}

/**
 * Will return a list of containing all items on current layer.
 *
 */

QList< QVariant > LayerAPI::items()
{
	QList<QVariant> l;
	for (int i = 0; i<ScCore->primaryMainWindow()->doc->Items->count(); ++i)
	{
		PageItem *item = ScCore->primaryMainWindow()->doc->Items->at(i);
		if (item->LayerID == this->getID())
		{
			if (item->asTextFrame())
			{
				TextAPI *textItem = new TextAPI(item->asTextFrame());
				l.append(qVariantFromValue((QObject *)(textItem)));
			}
			else if (item->asImageFrame())
			{
				ImageAPI *imageItem = new ImageAPI(item->asImageFrame());
				l.append(qVariantFromValue((QObject *)(imageItem)));
			}
			else
			{
				ItemAPI *otherItem = new ItemAPI(item);
				l.append(qVariantFromValue(
				             (QObject *)(otherItem)
				         ));
			}
		}
	}
	return l;
}

LayerAPI::~LayerAPI()
{
	qDebug() << "ScribusLayer deleted";
}


