/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/*
 *  colorcombo.h
 *  ScribusCVS
 *
 *  Created by Jean Ghali on 18.02.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */

#ifndef COLORCOMBO_H
#define COLORCOMBO_H

#include <QComboBox>

/*
 *#ifdef Q_OS_MACOS
#include <QMacStyle>
#endif
*/
#include "scribusapi.h"
#include "colorlistbox.h"

/*!
    \class ColorCombo colorcombo.h
    \brief The ColorCombo widget is a combo box for displaying list of colors.
*/
class SCRIBUS_API ColorCombo : public QComboBox 
{
	Q_OBJECT

public:

	enum PixmapType
	{
		smallPixmaps,
		widePixmaps,
		fancyPixmaps
	};

	ColorCombo( QWidget* parent = nullptr );
	ColorCombo( ColorCombo::PixmapType type, QWidget* parent = nullptr );
	ColorCombo( bool rw, QWidget* parent = nullptr );

	QString currentColor() const;

	void setColors(ColorList& list, bool insertNone = true);
	void setCurrentColor(QString colorName);
	void setPixmapType(ColorCombo::PixmapType type);

	void initColorList(ColorList* colorList, ScribusDoc* doc, const QString& colorValue);
};

#endif
