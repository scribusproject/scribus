#ifndef SELECTIONRUBBERBAND_H
#define SELECTIONRUBBERBAND_H

#include <QRubberBand>
#include <QPaintEvent>

#include "scribusapi.h"

class SCRIBUS_API SelectionRubberBand : public QRubberBand
{
	Q_OBJECT
	public:
		SelectionRubberBand(QRubberBand::Shape s, QWidget * p = 0);
		~SelectionRubberBand();

	protected:
		void paintEvent(QPaintEvent *p) override;
};

#endif // SELECTIONRUBBERBAND_H
