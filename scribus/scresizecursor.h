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

class ScResizeCursor
{
	void initCursorDb();
	static QMap<unsigned int, QCursor> cdb;
	int cIdx;
	public:
		ScResizeCursor(double rotation = 0.0);
		~ScResizeCursor() {}
		
		operator const QCursor& ();
};

#endif // SCCURSOR_H
