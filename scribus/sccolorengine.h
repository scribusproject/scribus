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

#ifndef SCCOLORENGINE_H
#define SCCOLORENGINE_H

#include "scribusapi.h"
#include "sccolor.h"
#include "scribusstructs.h"
class ScribusDoc;

class SCRIBUS_API ScColorEngine
{
public:

	/** \brief get RGB values of a specified color */
	static QColor getRGBColor(const ScColor& color, const ScribusDoc* doc);

	static ScColor convertToModel(const ScColor& color, const ScribusDoc* doc, colorModel model);

	/** \brief get RGB values of a specified color */
	static void getRGBValues(const ScColor& color, const ScribusDoc* doc, RGBColor& rgb);

	/** \brief get CMYK values of a specified color */
	static void getCMYKValues(const ScColor& color, const ScribusDoc* doc, CMYKColor& cmyk);

	/** \brief get RGB values of a specified shade */
	static void getShadeColorRGB(const ScColor& color, const ScribusDoc* doc, RGBColor&, double level);
	
	/** \brief get CMYK values of a specified shade */
	static void getShadeColorCMYK(const ScColor& color, const ScribusDoc* doc, CMYKColor& cmyk, double level);

	/** \brief Return a color converted to monitor color space. No soft-proofing is done. */
	static QColor getDisplayColor(const ScColor& color, const ScribusDoc* doc);

	/** \brief Return a color with the specified shade converted to monitor color space. 
	* No soft-proofing is done. */
	static QColor getDisplayColor(const ScColor& color, const ScribusDoc* doc, double level);

	/** \brief Return a color converted to monitor color space. No soft-proofing is done
	* If gamut check is valid, the return value may be an gamut warning . */
	static QColor getDisplayColorGC(const ScColor& color, const ScribusDoc* doc);

	/** \brief Return a proofed QColor with 100% shade and optional gamut check.
	* If color management is enabled, returned value use the monitor color space. */
	static QColor getColorProof(const ScColor& color, const ScribusDoc* doc, bool gamutCheck = false);

	/** \brief Return a QColor with the specified shade.
	* If color management is enabled, returned value use the rgb solid colors space. */
	static QColor getShadeColor(const ScColor& color, const ScribusDoc* doc, double level);

	/** \brief Return a proofed QColor with the specified shade.
	* If color management is enabled, returned value use the monitor color space. */
	static QColor getShadeColorProof(const ScColor& color, const ScribusDoc* doc, double level);

	/** \brief Return a proofed QColor from a rgb color.
	* If color management is enabled, returned value use the monitor color space. */
	static QColor getColorProof(RGBColor& rgb, const ScribusDoc* doc, bool spot, bool gamutCkeck);

	/** \brief Return a proofed QColor from a cmyk color.
	* If color management is enabled, returned value use the monitor color space. */
	static QColor getColorProof(CMYKColor& cmyk, const ScribusDoc* doc, bool spot, bool gamutCkeck);

	/** \brief Return a QColor from a rgb color using doc rgb color space as input.
	* If color management is enabled, returned value use the monitor rgb color space. */
	static QColor getDisplayColor(RGBColor& rgb, const ScribusDoc* doc, bool spot);

	/** \brief Return a QColor from a cmyk color using doc cmyk color space as input.
	* If color management is enabled, returned value use the monitor rgb color space. */
	static QColor getDisplayColor(CMYKColor& cmyk, const ScribusDoc* doc, bool spot);

	/** \brief Check if a color is inside document output color space gamut. */
	static bool isOutOfGamut(const ScColor& color, const ScribusDoc* doc);

	/** \brief Applys Gray-Component-Removal to an ScColor */
	static void applyGCR(ScColor& color, const ScribusDoc* doc);
};

#endif
