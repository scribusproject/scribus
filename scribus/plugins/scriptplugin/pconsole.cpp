#include "pconsole.h"
#include "pconsole.moc"
#include "qpixmap.h"
extern QPixmap loadIcon(QString nam);

PConsole::PConsole( QWidget* parent )
		: QWidget( parent, "PConsole", WType_TopLevel )
{
	resize( 431, 306 );
	setCaption( tr( "Script Console" ) );
	setIcon(loadIcon("AppIcon.png"));
	PConsoleLayout = new QVBoxLayout( this, 11, 6, "PConsoleLayout");
	OutWin = new ConsWin(this);
	OutWin->setMinimumSize( QSize( 300, 0 ) );
	PConsoleLayout->addWidget( OutWin );
	connect( OutWin, SIGNAL( closeFromKeyB() ), this, SLOT( close() ) );
}

void PConsole::closeEvent(QCloseEvent *ce)
{
	emit paletteShown(false);
	ce->accept();
}

