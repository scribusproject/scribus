/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "imagedialog.h"

Imagedialog::Imagedialog ( const QString imageFile, ScribusDoc* doc , QWidget *parent ) : QDialog ( parent )
{
	setupUi ( this );

	setWindowTitle ( imageFile );

//no realCMYK
	bool mode=false;
//no document needs to be assigned to this
	CMSettings cms ( doc, "", 0 );

	//load image
	if ( image.LoadPicture ( imageFile, 1, cms, true, true, ScImage::RGBProof, 72, &mode ) )
	{
		imageLabel->setPixmap ( QPixmap::fromImage ( image.scaled ( this->size().width()-3, this->size().height()-3, Qt::KeepAspectRatio, Qt::SmoothTransformation ) ) );

		scrollArea = new QScrollArea ( this );
		scrollArea->setFrameShape ( QFrame::NoFrame );
		scrollArea->setBackgroundRole ( QPalette::Dark );
		scrollArea->setSizePolicy ( QSizePolicy::Ignored, QSizePolicy::Ignored );
		scrollArea->setWidget ( imageLabel );

		scrollArea->setWidgetResizable ( true );

		layout()->addWidget ( scrollArea );

		connect ( fitToWindowRadiobutton, SIGNAL ( toggled ( bool ) ), this, SLOT ( fitToWindowRadiobuttonToggled ( bool ) ) );
		connect ( zoomRadiobutton, SIGNAL ( toggled ( bool ) ), this, SLOT ( zoomRadiobuttonToggled ( bool ) ) );
		connect ( zoomSpinbox, SIGNAL ( valueChanged ( int ) ), this, SLOT ( zoomSpinboxValueChanged ( int ) ) );
		connect ( showOriginalSizeButton, SIGNAL ( clicked() ), this, SLOT ( showOriginalSizeButtonClicked() ) );
	}
	else
	{
		//show error
		imageLabel->setText ( QString ( "error loading image: %1" ).arg ( imageFile ) );
	}
}


void Imagedialog::resizeEvent ( QResizeEvent * event )
{
	if ( fitToWindowRadiobutton->isChecked() )
	{
		imageLabel->setPixmap ( QPixmap::fromImage ( image.scaled ( scrollArea->size().width(), scrollArea->size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation ) ) );
		zoomSpinbox->setValue ( qRound ( ( double ) ( ( double ) imageLabel->pixmap()->size().width() / ( double ) image.width() ) *100 ) );
	}
}


void Imagedialog::fitToWindowRadiobuttonToggled ( bool toggled )
{
	if ( toggled )
	{
		scrollArea->setWidgetResizable ( true );
		imageLabel->setPixmap ( QPixmap::fromImage ( image.scaled ( scrollArea->size().width(), scrollArea->size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation ) ) );
		zoomSpinbox->setValue ( qRound ( ( double ) ( ( double ) imageLabel->pixmap()->size().width() / ( double ) image.width() ) *100 ) );

		zoomSpinbox->setEnabled ( false );
	}
}


void Imagedialog::zoomRadiobuttonToggled ( bool toggled )
{
	if ( toggled )
	{
		scrollArea->setWidgetResizable ( false );

		zoomSpinbox->setEnabled ( true );
	}
}


void Imagedialog::zoomSpinboxValueChanged ( int value )
{
	if ( zoomRadiobutton->isChecked() )
	{
		if ( value == 100 )
		{
			imageLabel->setPixmap ( QPixmap::fromImage ( image.qImage() ) );
			imageLabel->adjustSize();
		}
		else
		{
			double scaleFactor = ( ( double ) value/100 );
			imageLabel->setPixmap ( QPixmap::fromImage ( image.scaled ( qRound ( image.width() *scaleFactor ), qRound ( image.height() *scaleFactor ), Qt::KeepAspectRatio, Qt::SmoothTransformation ) ) );
			imageLabel->adjustSize();
		}
	}
}


void Imagedialog::showOriginalSizeButtonClicked()
{
	zoomRadiobutton->setChecked ( true );
	zoomSpinbox->setValue ( 100 );
}
