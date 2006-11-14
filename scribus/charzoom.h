#ifndef CHARZOOM_H
#define CHARZOOM_H

#include <qdialog.h>

#include "scribusapi.h"

class QPixmap;


/*! \brief "A magnifying glass" dialog for CharTable cell.
*/
class SCRIBUS_API CharZoom : public QDialog
{
	Q_OBJECT

	public:
		CharZoom( QWidget* parent, QPixmap pix, uint val);
		~CharZoom() {};
	private:
		void paintEvent(QPaintEvent *);
		QPixmap pixm;
		QString valu;
};

#endif
