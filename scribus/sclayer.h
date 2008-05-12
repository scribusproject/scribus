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
#include <QSet>    //necessary to avoid msvc warnings induced by SCRIBUS_API on ScLayers + early instanciation of templates

#include "scribusapi.h"

class SCRIBUS_API ScLayer
{
public:
	ScLayer(void);
	ScLayer(const QString& name, int level, int nr);
	QString Name;
	int     LNr;
	int     Level;
	bool    isPrintable;
	bool    isViewable;
	bool    isEditable;
	bool    flowControl;
	bool    outlineMode;
	double  transparency;
	int     blendMode;
	QColor  markerColor;
	bool operator< (const ScLayer& other) const;
	bool operator== (const ScLayer& other) const;
};

class SCRIBUS_API ScLayers : public QList<ScLayer>
{
protected:
	/**
	 * @brief  Get layer max identifier
	 * @return Layer max identifier or -1 is list is empty
	 */
	int getMaxNumber(void);
public:

	/**
	 * @brief  Get bottom layer
	 * @return bottom layer or NULL if list is empty
	 */
	const ScLayer* bottomLayer (void) const;

	/**
	 * @brief  Get top layer
	 * @return top layer or NULL if list is empty
	 */
	const ScLayer* topLayer (void) const;

	/**
	 * @brief  Get layer at a specific level, replace old Level2Layer in util.cpp
	\param layer a reference to the ScLayer
	 * @param  level the layer level
	 * @return the desired layer or NULL if not found
	 */
	void levelToLayer (ScLayer& layer, int level) const;

	/**
	 * @brief  Get bottom layer
	 * @return bottom layer or NULL if list is empty
	 */
	ScLayer* bottom(void);

	/**
	 * @brief  Get top layer
	 * @return top layer or NULL if list is empty
	 */
	ScLayer* top(void);

	/**
	 * @brief  Get layer at a specific level
	 * @param  level the layer level
	 * @return layer with the specified level or NULL if not found
	 */
	ScLayer* byLevel(const int level);

	/**
	 * @brief  Get layer with a specific number
	 * @param  nr the layer number
	 * @return layer with the specified number or NULL if not found
	 */
	ScLayer* byNumber(const int nr);

	/**
	 * @brief  Get layer above the layer with the specified ID
	 * @param  nr number of the layer level
	 * @return layer above the specific layer or top layer if no layer above specified level was found
	 */
	ScLayer* above (int nr);

	/**
	 * @brief  Get layer below the layer with the specified ID
	 * @param  nr the layer level
	 * @return layer below the specific layer or bottom layer if no layer below specified level was found
	 */
	ScLayer* below (int nr);

	/**
	 * @brief  Get layer at a specific level
	 * @param  level the layer level
	 * @return layer with the specified level or NULL if not found
	 */
	const ScLayer* layerByLevel (int level) const;

	/**
	 * @brief  Get layer with a specific number
	 * @param  nr the layer number
	 * @return layer with the specified number or NULL if not found
	 */
	const ScLayer* layerByNumber (int nr) const;

	/**
	 * @brief  Get layer with a specific name
	 * @param  name the layer name
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
	 * @return the new layer id on success, -1 on failure
	 */
	int addLayer(const QString& layerName);

	/**
	 * @brief  Add a layer to the layer list
	 * @param  layer the layer object
	 * @return the new layer id on success, -1 on failure
	 */
	int addLayer(const ScLayer& layer);

	/**
	 * @brief  Add a layer to the layer list
	 * @param  layerName the layer name (may be empty)
	 * @return the new layer on success, NULL on failure
	 */
	ScLayer* newLayer(const QString& layerName);

	/**
	 * @brief  Remove a layer from the layer list
	 * @param  the layer number to remove
	 */
	bool removeLayerByNumber(int nr);

	/**
	 * @brief  Remove a layer from the layer list
	 * @param  the layer level to remove
	 */
	bool removeLayerByLevel(int level);

	/**
	 * @brief  Raise the layer with the specified ID
	 * @return success or failure
	 */
	bool raiseLayer(int nr);

	/**
	 * @brief  Lower the layer with the specified ID
	 * @return success or failure
	 */
	bool lowerLayer(int nr);

	/**
	 * @brief  sort layers by level order
	 */
	void sort(void);

	/**
	 * @brief Is the layer printable
	 * @param layerNumber ID of the layer
	 * @return Printable or not
	 */
	bool layerPrintable(const int layerNumber) const;

	/**
	 * @brief Set the layer printable via the layer number
	 * @param layerNumber ID of the layer
	 * @param isPrintable bool true = layer is prantable
	 * @return Success or failure
	 */
	bool setLayerPrintable(const int layerNumber, const bool isPrintable);

	/**
	 * @brief Is the layer visible
	 * @param layerNumber ID of the layer
	 * @return Visible or not
	 */
	bool layerVisible(const int layerNumber) const;

	/**
	 * @brief Set the layer visible via the layer number
	 * @param layerNumber ID of the layer
	 * @param isViewable true = layer is visible
	 * @return Success or failure
	 */
	bool setLayerVisible(const int layerNumber, const bool isViewable);

	/**
	 * @brief Is the layer locked
	 * @param layerNumber ID of the layer
	 * @return Locked or not
	 */
	bool layerLocked(const int layerNumber) const;

	/**
	 * @brief Set the layer locked via the layer number
	 * @param layerNumber ID of the layer
	 * @param isViewable true = layer is locked
	 * @return bool Success or failure
	 */
	bool setLayerLocked(const int layerNumber, const bool isViewable);

	/**
	 * @brief does text flow around objects on this layer
	 * @param layerNumber ID of the layer
	 * @return flow or not
	 */
	bool layerFlow(const int layerNumber) const;

	/**
	 * @brief Set the layer flow via the layer number
	 * @param layerNumber ID of the layer
	 * @param flow true = Text flows around objects on this layer
	 * @return Success or failure
	 */
	bool setLayerFlow(const int layerNumber, const bool flow);

	/**
	 * @brief is this layer in outline mode
	 * @param layerNumber ID of the layer
	 * @return outline or not
	 */
	bool layerOutline(const int layerNumber) const;

	/**
	 * @brief Set the layer outline mode via the layer number
	 * @param layerNumber ID of the layer
	 * @param outline true = layer is displayed in outlines only
	 * @return Success or failure
	 */
	bool setLayerOutline(const int layerNumber, const bool outline);

	/**
	 * @brief returns the layer transparency
	 * @param layerNumber ID of the layer
	 * @return transparency value 0.0 - 1.0
	 */
	double layerTransparency(const int layerNumber) const;

	/**
	 * @brief Set the layer transparency via the layer number
	 * @param layerNumber ID of the layer
	 * @param trans transparency value 0.0 - 1.0
	 * @return Success or failure
	 */
	bool setLayerTransparency(const int layerNumber, double trans);

	/**
	 * @brief returns the layer BlendMode
	 * @param layerNumber ID of the layer
	 * @return layerBlendMode
	 */
	int layerBlendMode(const int layerNumber) const;

	/**
	 * @brief Set the layer layerBlendMode via the layer number
	 * @param layerNumber ID of the layer
	 * @param trans layerBlendMode
	 * @return Success or failure
	 */
	bool setLayerBlendMode(const int layerNumber, int blend);

	/**
	 * @brief returns the layer marker color
	 * @param layerNumber Number of the layer
	 * @return marker color
	 */
	QColor layerMarker(const int layerNumber) const;

	/**
	 * @brief Set the layer marker color
	 * @param layerNumber ID of the layer
	 * @param color color of the marker
	 * @return Success or failure
	 */
	 bool setLayerMarker(const int layerNumber, QColor color);
};

uint qHash(const ScLayer& layer);

#endif
