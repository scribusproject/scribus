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

ScribusLayer::ScribusLayer(ScLayer *l) : QObject(COLLECTOR)
{
	qDebug() << "ScLayerWrapper loaded";
	setObjectName("layer");
	innerLayer = l;
}


QString ScribusLayer::getName()
{
	return innerLayer->Name;
}

void ScribusLayer::setName(QString name)
{
	innerLayer->Name = name;
}

int ScribusLayer::getID()
{
	return innerLayer->ID;
}

int ScribusLayer::getLevel()
{
	return innerLayer->Level;
}

void ScribusLayer::setLevel(int level)
{
	innerLayer->Level = level;
}

bool ScribusLayer::isPrintable()
{
	return innerLayer->isPrintable;
}

void ScribusLayer::setPrintable(bool value)
{
	innerLayer->isPrintable = value;
}

bool ScribusLayer::isViewable()
{
	return innerLayer->isViewable;
}

void ScribusLayer::setViewable(bool value)
{
	innerLayer->isViewable = value;
}

bool ScribusLayer::isEditable()
{
	return innerLayer->isEditable;
}

void ScribusLayer::setEditable(bool value)
{
	innerLayer->isEditable = value;
}

bool ScribusLayer::getFlowControl()
{
	return innerLayer->flowControl;
}

void ScribusLayer::setFlowControl(bool value)
{
	innerLayer->flowControl = value;
}

bool ScribusLayer::getOutlineMode()
{
	return innerLayer->outlineMode;
}

void ScribusLayer::setOutlineMode(bool value)
{
	innerLayer->outlineMode = value;
}

double ScribusLayer::getTransparency()
{
	return innerLayer->transparency;
}

void ScribusLayer::setTransparency(double value)
{
	innerLayer->transparency = value;
}

int ScribusLayer::getBlendMode()
{
	return innerLayer->blendMode;
}

void ScribusLayer::setBlendMode(int value)
{
	innerLayer->blendMode = value;
}

bool ScribusLayer::isActive()
{
	if (ScCore->primaryMainWindow()->doc->activeLayerName() == innerLayer->Name)
		return true;
	return false;
}

void ScribusLayer::setActive(bool value)
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

QList< QVariant > ScribusLayer::items()
{
	QList<QVariant> l;
	for (int i = 0; i<ScCore->primaryMainWindow()->doc->Items->count(); ++i)
	{
		PageItem *item = ScCore->primaryMainWindow()->doc->Items->at(i);
		if (item->LayerID == this->getID())
		{
			if (item->asTextFrame())
			{
				ScribusTextItem *textItem = new ScribusTextItem(item->asTextFrame());
				l.append(qVariantFromValue((QObject *)(textItem)));
			}
			else if (item->asImageFrame())
			{
				ScribusImageItem *imageItem = new ScribusImageItem(item->asImageFrame());
				l.append(qVariantFromValue((QObject *)(imageItem)));
			}
			else
			{
				ScribusItem *otherItem = new ScribusItem(item);
				l.append(qVariantFromValue(
				             (QObject *)(otherItem)
				         ));
			}
		}
	}
	return l;
}

ScribusLayer::~ScribusLayer()
{
	qDebug() << "ScribusLayer deleted";
}


