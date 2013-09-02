#ifndef SELECTIONRUBBERBAND_H
#define SELECTIONRUBBERBAND_H

#include <QRubberBand>
#include <QPaintEvent>

#include "scribusapi.h"

class SCRIBUS_API SelectionRubberBand : public QRubberBand
{
	public:
		SelectionRubberBand(QRubberBand::Shape s, QWidget * p = 0);
		~SelectionRubberBand();

		virtual void paintEvent(QPaintEvent *p);

};

#endif // SELECTIONRUBBERBAND_H
