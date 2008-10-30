//
// C++ Implementation: sccursor
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "scresizecursor.h"

#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QBrush>

#define SCCURSORMAXANGLE 360

QMap<unsigned int, QCursor> ScResizeCursor::cdb;

void ScResizeCursor::initCursorDb(int idx)
{
	if(cdb.contains(idx))
		return;
	
	QRect rect(0,0,32,32);
	int ww(rect.width());
	int hh(rect.height());
	int uu(ww / 32 );
	QPen pen(QColor(232,232,232), 1);
	QBrush brush(QColor(32,32,32));
	
	// the double-arrow
	// we build it around (0,0) to ease rotations
	// TODO design a _nice_ cursor
	
	QPainterPath path;
	double pext(6);
	double pint(4);
	double sext(12);
	double sint(1);
	
	path.moveTo(0, -sext * uu);
	path.lineTo( pint*uu,-pext*uu);
	path.lineTo( sint*uu,-pext*uu);
	path.lineTo( sint*uu, pext*uu);
	path.lineTo( pint*uu, pext*uu);
	path.lineTo( 0   , sext*uu);
	path.lineTo(-pint*uu, pext*uu);
	path.lineTo(-sint*uu, pext*uu);
	path.lineTo(-sint*uu,-pext*uu);
	path.lineTo(-pint*uu,-pext*uu);
	path.lineTo( 0, -sext*uu);
	
// 	for(unsigned int rot(0); rot < SCCURSORMAXANGLE; ++rot)
// 	{
	int rot(idx);
		QPixmap pxm(rect.size());
		pxm.fill(QColor (255, 255, 255, 0 ));
		
		QPainter painter(&pxm);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.translate( ww/2, ww/2 );
		painter.rotate(rot);
		painter.setPen(pen);
		painter.setBrush(brush);
		painter.drawPath(path);
		
		cdb[rot] = QCursor ( pxm, ww/2, hh/2 );
// 	}

}

ScResizeCursor::ScResizeCursor(double rotation)
{
	int irot(qRound(rotation));
	if(irot < 0)
		cIdx = SCCURSORMAXANGLE + (irot % SCCURSORMAXANGLE);
	else
		cIdx = irot % SCCURSORMAXANGLE ;
	
	initCursorDb(cIdx);
}

ScResizeCursor::operator const QCursor &()
{
	return cdb[cIdx];
}

