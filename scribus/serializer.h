/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          serializer.h  -  description
                             -------------------
    begin                : Sat May 5 2001
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

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QString>
#include <QFile>
#include <QByteArray>
#include "scribusapi.h"
#include "selection.h"
#include "desaxe/saxio.h"
#include "desaxe/digester.h"

/**
  * This class reads/writes Scribus datastructures from/to SAX streams 
  * (was done by ScribusXML before). It also serves as a testbed for the new
  * Scribus 1.4 file format
  *
  * Currently it also holds to old methods which are used to read/write text 
  * from/to a file.
  *
  * @author Andreas Vox
  */

class ScribusDoc;
class ColorList;

class SCRIBUS_API Serializer : public desaxe::Digester {
public: 

	/** Construct a Serializer object which can be used to (de)serialize Scribus
		elements. Since it has Digester as a superclass, a Serializer object can
	    also be used where a SaxHandler is required as argument, eg. the saxx() method.
	 */
	Serializer(ScribusDoc& doc);
	/**
	  Writes all objects in selection to handler as a fragment. Needed styles and colors
	  are included.
	*/
	static void serializeObjects(const Selection& objects, SaxHandler& handler);
	/**
	  Reads objects from the given QString which must represent a fragment, imports them
	  into the document and returns a (nonGUI) selection to it.
	*/
	Selection deserializeObjects(const QByteArray & xml);
	/**
	  Reads objects from the given QFile which must represent a fragment, imports them
	  into the document and returns a (nonGUI) selection to it.
	 */
	Selection deserializeObjects(const QFile & xml);
	
	/** Legacy method to read plain text for a textframe */
	static bool readWithEncoding(const QString& filename, const QString& encoding, 
								 QString & txt);
	/** Legacy method to write plain text from a textframe */
	static bool writeWithEncoding(const QString& filename, const QString& encoding, 
								  const QString& txt);

private:
	ScribusDoc& m_Doc;
	ColorList backUpColors;
	Selection importCollection();
	void updateGradientColors(const ColorList& colors);
};

#endif
