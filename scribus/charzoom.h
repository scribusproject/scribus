#ifndef CHARZOOM_H
#define CHARZOOM_H

#include <qdialog.h>
//Added by qt3to4:
#include <QPixmap>
#include <QPaintEvent>

#include "scribusapi.h"
#include "fonts/scface.h"
class QPixmap;


/*! \brief "A magnifying glass" dialog for CharTable cell.
*/
class SCRIBUS_API CharZoom : public QDialog
{
	Q_OBJECT

	public:
		CharZoom(QWidget* parent, uint currentChar, ScFace face);
		~CharZoom() {};
	private:
		void paintEvent(QPaintEvent *);
		//! \brief Pixmap generated in constructor
		QPixmap pixm;
		//! \brief A hex value as string
		QString valu;
};

#endif
