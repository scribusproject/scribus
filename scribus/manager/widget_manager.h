/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include "scribusdoc.h"
#include <QObject>

class ColorPicker;
class FloatingWindow;

class WidgetManager: public QObject
{

	Q_OBJECT
public:
	WidgetManager(WidgetManager const&) = delete;
	void operator=(WidgetManager const&) = delete;
	static WidgetManager &instance();

	void setup(ScribusDoc *doc);

	FloatingWindow * colorPickerWindow();
	ColorPicker * colorPicker();

	void unitChange();

private:
	WidgetManager(QObject *parent = nullptr);
	~WidgetManager() = default;

	static WidgetManager* m_instance;

	// Widgets
	ScribusDoc *m_doc;
	FloatingWindow * m_colorPickerWindow;
	ColorPicker * m_colorPicker;

};

#endif // WIDGETMANAGER_H
