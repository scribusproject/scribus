/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QScrollArea>
//#include <QMessageBox>

#include "scimage.h"
#include "scribusstructs.h"
#include "cmsettings.h"

#include "ui_imagedialog.h"


//central class, manages gui
class Imagedialog : public QDialog, Ui::imagedialog
{
		Q_OBJECT

	public:
		Imagedialog ( const QString imageFile, ScribusDoc* doc, QWidget *parent = 0 );
		void resizeEvent ( QResizeEvent * event );

		ScImage image;

	signals:


	private slots:
		void fitToWindowRadiobuttonToggled ( bool toggled );
		void zoomRadiobuttonToggled ( bool toggled );
		void zoomSpinboxValueChanged ( int value );
		void showOriginalSizeButtonClicked();


	private:
		QScrollArea *scrollArea;
};

#endif

