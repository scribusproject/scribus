/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "arrowchooser.h"
//#include "arrowchooser.moc"
#include "fpointarray.h"
#include <qpixmap.h>
#include <q3listbox.h>
//Added by qt3to4:
#include <QList>
#include <QListView>
#include "scpainter.h"
#include <qimage.h>
#include "util.h"

ArrowChooser::ArrowChooser(QWidget* pa, bool direction)  : Q3ComboBox(true, pa)
{
	setEditable(false);
	clear();
	insertItem( tr("None"));
	listBox()->setMinimumWidth(listBox()->maxItemWidth()*2 + 22);
	arrowDirection = direction;
}

void ArrowChooser::rebuildList(QList<ArrowDesc> *arrowStyles)
{
	clear();
	FPointArray Path;
	Path.resize(0);
	insertItem( tr("None"));
	for (int a = 0; a < arrowStyles->count(); ++a)
	{
		QImage image(22, 22, QImage::Format_ARGB32);
		ScPainter *painter = new ScPainter(&image, 22, 22);
		painter->clear();
		painter->setBrush(qRgb(0, 0, 0));
		painter->setPen(qRgb(0, 0, 0));
		painter->setFillMode(1);
		painter->translate(3.0, 3.0);
		Path.resize(0);
		Path = arrowStyles->at(a).points.copy();
		FPoint min = getMinClipF(&Path);
		Path.translate(-min.x(), -min.y());
		FPoint max = Path.WidthHeight();
		QMatrix mm;
		QMatrix mm2;
		if (arrowDirection)
		{
			mm2.scale(-1, 1);
			mm2.translate(-max.x(), 0);
		}
		mm.scale(16.0 / qMax(max.x(), max.y()), 16.0 / qMax(max.x(), max.y()));
		Path.map(mm2 * mm);
		painter->setupPolygon(&Path);
		painter->setLineWidth(1.0);
		painter->drawPolygon();
		painter->drawPolyLine();
		painter->end();
		delete painter;
		int wi = image.width();
		int hi = image.height();
    	for( int yi=0; yi < hi; ++yi )
		{
			QRgb *s = (QRgb*)(image.scanLine( yi ));
			for(int xi=0; xi < wi; ++xi )
			{
				if((*s) == 0xffffffff)
					(*s) &= 0x00ffffff;
				s++;
			}
    	}
    	QPixmap Ico;
		Ico.convertFromImage(image);
		insertItem(Ico, arrowStyles->at(a).name);
	}
	listBox()->setMinimumWidth(listBox()->maxItemWidth()+24);
}
