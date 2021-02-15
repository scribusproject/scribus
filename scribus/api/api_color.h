/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.

Scribus is copyright The Scribus Team per the COPYING file.
*/

#include <QString>

#include "scribusapi.h"

namespace ScribusAPI {

	//
	// Create a new color with CMYK values. Use integers for each channel (0-255)
	//
	bool SCRIBUS_API createColorCMYK(const QString& name, int c, int m, int y, int k);

	//
	// Create a new color with CMYK values. Use decimals for each channel (0.0-1.0)
	//
	bool SCRIBUS_API createColorCMYKFloat(const QString& name, double c, double m, double y, double k);

	//
	// Create a new color with Lab values. Use decimals for each channel (0.0-1.0)
	//
	bool SCRIBUS_API createColorLAB(const QString& name, double l, double a, double b);

	//
	// Create a new color with RGB values. Use integers for each channel (0-255)
	//
	bool SCRIBUS_API createColorRGB(const QString& name, int r, int g, int b);

	//
	// Create a new color with CMYK values. Use decimals for each channel (0.0-1.0)
	//
	bool SCRIBUS_API createColorRGBFloat(const QString& name, double r, double g, double b);

	//
	// Delete color from list by its name
	//
	bool SCRIBUS_API deleteColor(const QString& name);

	//
	// Check if a color exists by its name
	//
	bool SCRIBUS_API existsColor(const QString& name, QString& error);

	//
	// Get a color by its name in CMYK format
	//
//	tuple getColorCMYK(const QString& name);

	//
	// Get a color by its name in CMYK format. Decimals will be returned for each channel (0.0-1.0)
	//
//	tuple getColorCMYKFloat(const QString& name);

	//
	// Get a color by its name in Lab format
	//
//	tuple getColorLAB(const QString& name);

	//
	// Get a color by its name in RGB format
	//
//	tuple getColorRGB(const QString& name);

	//
	// Get a color by its name in RGB format. Decimals will be returned for each channel (0.0-1.0)
	//
//	tuple getColorRGBFloat(const QString& name);

	//
	// Get the list of color names in this document
	//
	QStringList SCRIBUS_API getColorNames();

	//
	// Check if a named color is a spot color
	//
	bool SCRIBUS_API isSpotColor(const QString& name);

	//
	// Replace one named color by another
	//
	bool SCRIBUS_API replaceColor(const QString& oldColorName, const QString& newColorName);

	//
	// Set the color values to specified CMYK values
	//
//	tuple setColorCMYK(const QString& name);

	//
	// Set the color values to specified CMYK decimal values (0.0-1.0)
	//
//	tuple setColorCMYKFloat(const QString& name);

	//
	// Set the color values to specified Lab values
	//
//	tuple setColorLAB(const QString& name);

	//
	// Set the color values to specified RGB values
	//
//	tuple setColorRGB(const QString& name);

	//
	// Set the color values to specified RGB decimal values (0.0-1.0)
	//
//	tuple setColorRGBFloat(const QString& name);

	//
	// Set a named color to be a spot color
	//
	bool SCRIBUS_API setSpotColor(const QString& name);

}

