#include "polyprops.h"
#include "polyprops.moc"
#include "polygonwidget.h"

extern QPixmap loadIcon(QString nam);

PolygonProps::PolygonProps(QWidget* parent, int PolyC, int PolyFd, double PolyF, bool PolyS, double PolyR) : QDialog( parent, "poly", true, 0 )
{
	setCaption( tr( "Polygon Properties" ) );
	setIcon(loadIcon("AppIcon.png"));
	PolygonPropsLayout = new QVBoxLayout( this, 10, 5, "PolygonPropsLayout");
	polyWidget = new PolygonWidget(this, PolyC, PolyFd, PolyF, PolyS, PolyR);
	PolygonPropsLayout->addWidget( polyWidget );
	Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1_2");
	QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer_2 );
	okButton = new QPushButton( this, "PushButton1" );
	okButton->setText( tr( "&OK" ) );
	okButton->setDefault( true );
	Layout1->addWidget( okButton );
	cancelButton = new QPushButton( this, "PushButton2" );
	cancelButton->setText( tr( "&Cancel" ) );
	Layout1->addWidget( cancelButton );
	PolygonPropsLayout->addLayout( Layout1 );
	// signals and slots connections
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void PolygonProps::getValues(int* PolyC, int* PolyFd, double* PolyF, bool* PolyS, double* PolyR)
{
	polyWidget->getValues(PolyC, PolyFd, PolyF, PolyS, PolyR);
}
