//
// C++ Interface: sccursor
//
// Description: A QCursor descendant that allows rotation
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SCCURSOR_H
#define SCCURSOR_H

#include <QCursor>
#include <QMap>

/**
* This class provides a custom Size_ cursor which can be rotated
* to adjust with the rotation of a frame.
*
* It maintains a database of pre-rotated cursors so user is not 
* expected to cache cursors.
*
* It provides a cast operator to QCursor& so you can use it directly
* where reference to QCursor are required.
* example:
 * QApplication::setOverrideCursor(ScResizeCursor(19.76));
*
* Note that rotation is rounded (with qRound()) in order to save memory usage.
*/

class ScResizeCursor
{
	void initCursorDb(int idx);
	static QMap<unsigned int, QCursor> cdb;
	int cIdx;
	public:
		/**
		* Build a new ScResizeCursor
		* rotation is in degrees and clockwise
		* initial position of the cursor is vertical, corresponding to Qt::SizeVerCursor
		*/
		ScResizeCursor();
		ScResizeCursor(double rotation);
		~ScResizeCursor() {}
		
		operator const QCursor& ();
};

#endif // SCCURSOR_H
