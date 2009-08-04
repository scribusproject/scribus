/***************************************************************************
 *   Copyright (C) 2009 by Pierre Marchand                                 *
 *   pierre@oep-h.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef OPENPALETTEMANAGER_H
#define OPENPALETTEMANAGER_H

#include <QObject>
#include <QString>
#include <QList>


class OpenPalette;
class PrefsFile;
class QWidget;

/**
  * This manager maintains records of instanciated open
  * palettesand is able to serialize the whole palettes
  * collection for further reload.
  * It is a singleton (hard one if I can say (no means to grab an instance of it))
  */

class OpenPaletteManager : public QObject
{
	Q_OBJECT

	OpenPaletteManager(){}
	OpenPaletteManager(QObject * parent);
	static OpenPaletteManager * instance;
	static OpenPaletteManager* that();

public:
	/**
	  * Save the configuration of the palettes in an XML file
	  */
	static void savePaletteCollection(const QString& filepath);

	/**
	  * Load palettes configuration from an XML file
	  */
	static void loadPaletteCollection(const QString& filepath);

	/**
	  * create a new OpenPalette
	  */
	static void newPalette(QWidget * parent = 0);

	/**
	  * Return number of palettes
	  */
	static unsigned int count();

	/**
	  * Return the name of the palette at index idx
	  */
	static QString title(unsigned int idx);

	/**
	  * Return a property of a palette
	  */
	static QVariant paletteProperty(unsigned int idx, const QString& prop);


	/**
	  * Set A property of a palette
	  */
	static bool paletteProperty(unsigned int idx, const QString& prop, const QVariant& val);

	/**
	  * Return the list of setters attached to the palette at index idx
	  */
	static QStringList items(unsigned int idx);

/**
	  * Reset the list of setters attached to the palette at index idx
	  */
	static void setItems(unsigned int idx, const QStringList& ilist);

	/**
	  * subscribe to changes notifications
	  */
	static void subChanges(QObject * subscriber, const char * method);

private:
	QList<OpenPalette*> palettes;

private slots:
	void unRegisterPalette();
	void catchPalette();

signals:
	void changed();

};

#endif // OPENPALETTEMANAGER_H
