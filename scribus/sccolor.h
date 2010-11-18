/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          sccolor.h  -  description
                             -------------------
    begin                : Sun Sep 9 2001
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

#ifndef SCCOLOR_H
#define SCCOLOR_H

#include <QColor>
#include <QMap>
#include <QPointer>
#include <QString>

#include "scribusapi.h"

class ScribusDoc;


/**
  *@author Franz Schmid
  * \brief This Class adds support for CMYK-Colors to QT.
  * its API is based on the API of QColor
  */

/** Scribus color models */
enum colorModel
{
	colorModelRGB,
	colorModelCMYK
};

class SCRIBUS_API ScColor 
{
public:

	friend class ScColorEngine;

	/** \brief Constructs a ScColor with 4 Components set to 0 */
	ScColor(void);
	/** \brief Constructs a ScColor with 4 Components in the range from 0 - 255 */
	ScColor(int c, int m, int y, int k);
	/** \brief Constructs a RGBColor with 3 Components in the range from 0 - 255 */
	ScColor(int r, int g, int b);
	
	bool operator==(const ScColor& other) const;

	/** \brief Set color from an existing document */
	void setColor(ScribusDoc* doc, const QString& name);

	/** \brief Same as the Constructor but for an existing Color */
	void setColor(int c, int m, int y, int k);

	/** \brief Computes a ScColor from an RGB-Color
	* Applies Gray-Component-Removal to the resulting ScColor
	* or if color management is enabled, an approriate transform */
	void setColorRGB(int r, int g, int b);

	/** \brief get the color model */
	colorModel getColorModel () const;

	/** \brief Computes a ScColor for a QColor */
	void fromQColor(QColor color);

	/** \brief Returns the RGB color  
	* Must not be called if color is the None Color. */
	void getRawRGBColor(int *r, int *g, int *b) const;
	QColor getRawRGBColor() const;

	/** \brief Returns the 4 Values that form an ScColor. 
	* Returns meaningful results only if color is a cmyk color.
	* Must not be called if color is the None Color. */
	void getCMYK(int *c, int *m, int *y, int *k) const;

	/** \brief Returns the 3 Values that form an RGBColor
	* Returns meaningful results only if color is a rgb color.
	* Must not be called if color is the None Color. */
	void getRGB(int *r, int *g, int *b) const;

	/** \brief Returns the ScColor as an Hex-String in the Form #CCYYMMKK for a CMYK color or ##RRGGBB for a RGB color. 
	* For the None color return the "None" string. */
	QString name();
	/** \brief Returns the ScColor as an Hex-String in the Form #RRGGBB
	* If doc member is not specified, return meaningful result only for RGB colors. */
	QString nameRGB(const ScribusDoc* doc = NULL);
	/** \brief Returns the ScColor as an Hex-String in the Form #CCYYMMKK
	* If doc member is not specified, return meaningful result only for CMYK colors. */
	QString nameCMYK(const ScribusDoc* doc = NULL);

	/** \brief Sets the Values of a color from an Hex-String in the Form #CCMMYYKK or #RRGGBB */
	void setNamedColor(QString nam);

	/** \brief If the color is a spot color */
	bool isSpotColor() const;
	/** \brief Set a color to be a spot color or not. No effect if color is the None color. */
	void setSpotColor(bool s);
	/** \brief If the color is a registration color */
	bool isRegistrationColor() const;
	/** \brief Set a color to be a registration color or not. No effect if color is the None color. */
	void setRegistrationColor(bool s);

private:

	/** \brief Cyan or Red Component of Color (depends of color model)*/
	int CR;
	/** \brief Magenta or Green Component of Color (depends of color model)*/
	int MG;
	/** \brief Yellow or Blue Component of Color (depends of color model)*/
	int YB;
	/** \brief Black-Component of Color */
	int K;

	/** \brief Flag, true if the Color is a Spotcolor */
	bool Spot;
	/** \brief Flag, true if the Color is a Registration color */
	bool Regist;
	/** \brief Color model of the current color */
	colorModel Model;
};

class SCRIBUS_API ColorList : public QMap<QString,ScColor>
{
protected:
	QPointer<ScribusDoc> m_doc;
	bool m_retainDoc;

	/** \brief Ensure availability of black color. */
	void ensureBlack(void);

	/** \brief Ensure availability of white color. */
	void ensureWhite(void);

	/** \brief Ensure availability of registration color. */
	void ensureRegistration(void);

public:
	ColorList(ScribusDoc* doc = NULL, bool retainDoc = false);

	ColorList& operator= (const ColorList& list);

	/** \brief Get the document the list is related , return in cpp due to scribusdoc class delcaration */
	ScribusDoc* document(void) const;

	/** \brief Assign the doc to which the list belong to.*/
	void setDocument(ScribusDoc* doc);

	/** \brief Add colors from the specified list. Colors are added using shadow copy.*/
	void addColors(const ColorList& colorList, bool overwrite = TRUE);

	/** \brief Copy colors from the specified list.*/
	void copyColors(const ColorList& colorList, bool overwrite = TRUE);

	/** \brief Ensure availability of black and white colors. */
	void ensureDefaultColors(void);
};

#endif
