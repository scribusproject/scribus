/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "imagedialog.h"

#include <QApplication>
#include <QDesktopWidget>

Imagedialog::Imagedialog ( const QString imageFile, ScribusDoc* doc , QWidget *parent ) : QDialog ( parent )
{
	setupUi ( this );
	setAttribute ( Qt::WA_DeleteOnClose );
	setWindowTitle ( imageFile );
	m_hRatio = double(QApplication::desktop()->logicalDpiX()) / 72.0;
	m_vRatio = double(QApplication::desktop()->logicalDpiY()) / 72.0;

//no realCMYK
	bool mode=false;
//no document needs to be assigned to this
	CMSettings cms ( doc, "", 0 );

	//load image
	if ( image.LoadPicture ( imageFile, 1, cms, true, true, ScImage::RGBProof, 72 , &mode ) )
	{
		
		pView->setImage(QPixmap::fromImage ( image.qImage() ));
		pView->fitImage();
		pView->setKeepFitted(true);

		connect ( fitToWindowRadiobutton, SIGNAL ( toggled ( bool ) ), this, SLOT ( fitToWindowRadiobuttonToggled ( bool ) ) );
		connect ( zoomRadiobutton, SIGNAL ( toggled ( bool ) ), this, SLOT ( zoomRadiobuttonToggled ( bool ) ) );
		connect ( zoomSpinbox, SIGNAL ( valueChanged ( int ) ), this, SLOT ( zoomSpinboxValueChanged ( int ) ) );
		connect ( showOriginalSizeButton, SIGNAL ( clicked() ), this, SLOT ( showOriginalSizeButtonClicked() ) );
	}
	else
	{
		//show error
// 		imageLabel->setText ( QString ( "Error loading image: %1" ).arg ( imageFile ) );
	}
}


void Imagedialog::resizeEvent ( QResizeEvent * event )
{
	if ( fitToWindowRadiobutton->isChecked() )
	{
		zoomSpinbox->setValue ( qRound ( pView->getZoom() * 100  * m_hRatio) );
	}
}


void Imagedialog::fitToWindowRadiobuttonToggled ( bool toggled )
{
	if ( toggled )
	{
		pView->fitImage();
		zoomSpinbox->setValue ( qRound ( pView->getZoom() * 100  * m_hRatio) );
		zoomSpinbox->setEnabled ( false );
	}
	pView->setKeepFitted(toggled);
}


void Imagedialog::zoomRadiobuttonToggled ( bool toggled )
{
	if ( toggled )
	{
		zoomSpinbox->setEnabled ( true );
	}
}


void Imagedialog::zoomSpinboxValueChanged ( int value )
{
	if ( zoomRadiobutton->isChecked() )
	{

		double scaleFactor = ( double( value ) / 100.0 / m_hRatio );
		pView->setZoom(scaleFactor);
	}
}


void Imagedialog::showOriginalSizeButtonClicked()
{
	zoomRadiobutton->setChecked ( true );
	zoomSpinbox->setValue ( 100 );
}
