#include "arrowchooser.h"
#include "arrowchooser.moc"
#include "fpointarray.h"
#include "scribusdoc.h"
#include <qpixmap.h>
#include <qlistbox.h>
#include "scpainter.h"
#include <qimage.h>

extern FPoint GetMinClipF(FPointArray Clip);

ArrowChooser::ArrowChooser(QWidget* pa, bool direction)  : QComboBox(true, pa)
{
	setEditable(false);
	clear();
	insertItem( tr("None"));
	setMaximumWidth(listBox()->maxItemWidth()*3+22);
	arrowDirection = direction;
}

void ArrowChooser::rebuildList(ScribusDoc *currentDoc)
{
	clear();
	FPointArray Path;
	Path.resize(0);
	insertItem( tr("None"));
	for (uint a = 0; a < currentDoc->arrowStyles.count(); ++a)
	{
		QPixmap Ico(22, 22);
		ScPainter *painter = new ScPainter(&Ico, 22, 22);
		painter->setBrush(qRgb(0, 0, 0));
		painter->setPen(qRgb(0, 0, 0));
		painter->setFillMode(1);
		painter->translate(3.0, 3.0);
		Path.resize(0);
		Path = currentDoc->arrowStyles[a].points.copy();
		FPoint min = GetMinClipF(Path);
		Path.translate(-min.x(), -min.y());
		FPoint max = Path.WidthHeight();
		QWMatrix mm;
		QWMatrix mm2;
		if (arrowDirection)
		{
			mm2.scale(-1, 1);
			mm2.translate(-max.x(), 0);
		}
		mm.scale(16.0 / QMAX(max.x(), max.y()), 16.0 / QMAX(max.x(), max.y()));
		Path.map(mm2 * mm);
		painter->setupPolygon(&Path);
		painter->setLineWidth(1.0);
		painter->drawPolygon();
		painter->drawPolyLine();
		painter->end();
		delete painter;
		QImage image;
		image = Ico.convertToImage();
  		image = image.convertDepth(32);
		image.setAlphaBuffer(true);
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
		Ico.convertFromImage(image);
		insertItem(Ico, currentDoc->arrowStyles[a].name);
	}
	listBox()->setMinimumWidth(listBox()->maxItemWidth()+24);
}
