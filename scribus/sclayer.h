/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCLAYER_H
#define SCLAYER_H

#include <QColor>
#include <QString>
#include <QList>
#include <QObject>

#include "scribusapi.h"

class SCRIBUS_API ScLayer
{
public:
	ScLayer(void);
	ScLayer(const QString& name, int level, int nr);
	int     LNr;
	int     Level;
	int     blendMode;
	bool    isPrintable;
	bool    isViewable;
	bool    isEditable;
	bool    flowControl;
	bool    outlineMode;
	double  transparency;
	QColor  markerColor;
	QString Name;
	bool operator< (const ScLayer& other) const;
};

class ScLayers : public QList<ScLayer>
{
public:
	/**
	 * @brief  Get layer at a specific level
	 * @param  level the layer level
	 * @return bottom layer or NULL if list is empty
	 */
	const ScLayer* bottomLayer (void) const;

	/**
	 * @brief  Get layer at a specific level
	 * @param  level the layer level
	 * @return top layer or NULL if list is empty
	 */
	const ScLayer* topLayer (void) const;

	/**
	 * @brief  Get layer at a specific level, replace old Level2Layer in util.cpp
	 * @param  level the layer level
	 * @return the desired layer or NULL if not found
	 */
	void levelToLayer (ScLayer& layer, int level) const;

	/**
	 * @brief  Get layer at a specific level
	 * @param  level the layer level
	 * @return layer with the specified level or NULL if not found
	 */
	const ScLayer* layerByLevel (int level) const;

	/**
	 * @brief  Get layer with a specific number
	 * @param  level the layer number
	 * @return layer with the specified number or NULL if not found
	 */
	const ScLayer* layerByNumber (int nr) const;

	/**
	 * @brief  Get layer with a specific number
	 * @param  level the layer number
	 * @return layer with the specified name or NULL if not found
	 */
	const ScLayer* layerByName (const QString& name) const;

	/**
	 * @brief  Get layer above a specific level
	 * @param  level the layer level
	 * @return layer above the specific layer or top layer if no layer above specified level was found
	 */
	const ScLayer* layerAbove (int level) const;
	/**
	 * @brief  Get layer above a specific layer
	 * @param  layer the layer
	 * @return layer above the specific layer or top layer if no layer above specified level was found
	 */
	const ScLayer* layerAbove (const ScLayer& layer) const;
	/**
	 * @brief  Get layer below a specific level
	 * @param  level the layer level
	 * @return layer below the specific layer or bottom layer if no layer below specified level was found
	 */
	const ScLayer* layerBelow (int level) const;
	/**
	 * @brief  Get layer below a specific layer
	 * @param  layer the layer
	 * @return layer below the specific layer or bottom layer if no layer below specified level was found
	 */
	const ScLayer* layerBelow (const ScLayer& layer) const;

	/**
	 * @brief  Add a layer to the layer list
	 * @param  layerName the layer name (may be empty)
	 */
	int addLayer(const QString& layerName);

	/**
	 * @brief  remove a layer from the layer list
	 * @param  the layer number to remove
	 */
	bool removeLayerByNumber(int nr);

	/**
	 * @brief  remove a layer from the layer list
	 * @param  the layer level to remove
	 */
	bool removeLayerByLevel(int level);

	/**
	 * @brief  sort layers by level order
	 */
	void sort(void);
};

#endif
