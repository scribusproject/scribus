/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.

Scribus is copyright The Scribus Team per the COPYING file.
*/

#include "api_color.h"

#include "scribuscore.h"
#include "scribus.h"

namespace ScribusAPI {

	bool createColorCMYK(const QString& name, int c, int m, int y, int k)
	{
	}

	bool createColorCMYKFloat(const QString& name, double c, double m, double y, double k)
	{
	}

	bool createColorLAB(const QString& name, double l, double a, double b)
	{
	}

	bool createColorRGB(const QString& name, int r, int g, int b)
	{
	}

	bool createColorRGBFloat(const QString& name, double r, double g, double b)
	{
	}

	bool deleteColor(const QString& name)
	{
	}

	bool existsColor(const QString& name, QString& error)
	{
		if (name.isEmpty())
		{
			error = "Color name is an empty string";
			return false;
		}
		if (!ScCore->primaryMainWindow()->HaveDoc)
		{
			error = "No document is open";
			return false;
		}
		return ScCore->primaryMainWindow()->doc->PageColors.contains(name);
	}

//	tuple getColorCMYK(const QString& name)
//	{
//	}

//	tuple getColorCMYKFloat(const QString& name)
//	{
//	}

//	tuple getColorLAB(const QString& name)
//	{
//	}

//	tuple getColorRGB(const QString& name)
//	{
//	}

//	tuple getColorRGBFloat(const QString& name)
//	{
//	}

	QStringList getColorNames()
	{
	}

	bool isSpotColor(const QString& name)
	{
		if (ScCore->primaryMainWindow()->HaveDoc &&
			ScCore->primaryMainWindow()->doc->PageColors.contains(name))
			return ScCore->primaryMainWindow()->doc->PageColors[name].isSpotColor();
		return false;
	}

	bool replaceColor(const QString& oldColorName, const QString& newColorName)
	{
	}

//	tuple setColorCMYK(const QString& name)
//	{
//	}

//	tuple setColorCMYKFloat(const QString& name)
//	{
//	}

//	tuple setColorLAB(const QString& name)
//	{
//	}

//	tuple setColorRGB(const QString& name)
//	{
//	}

//	tuple setColorRGBFloat(const QString& name)
//	{
//	}

	bool setSpotColor(const QString& name)
	{
	}


}
