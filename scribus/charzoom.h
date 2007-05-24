#ifndef CHARZOOM_H
#define CHARZOOM_H

#include <QDialog>

#include "scribusapi.h"

class ScFace;
class QPixmap;
class QPaintEvent;


/*! \brief "A magnifying glass" dialog for CharTableView cell.
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
