#include "polyprops.h"
#include "polyprops.moc"
#include "polygonwidget.h"

extern QPixmap loadIcon(QString nam);

PolygonProps::PolygonProps(QWidget* parent, int polyC, int polyFd, double polyF, bool polyS, double polyR) : QDialog( parent, "poly", true, 0 )
{
	setCaption( tr( "Polygon Properties" ) );
	setIcon(loadIcon("AppIcon.png"));
	PolygonPropsLayout = new QVBoxLayout( this, 10, 5, "PolygonPropsLayout");
	polyWidget = new PolygonWidget(this, polyC, polyFd, polyF, polyS, polyR);
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

void PolygonProps::getValues(int* polyC, int* polyFd, double* polyF, bool* polyS, double* polyR)
{
	polyWidget->getValues(polyC, polyFd, polyF, polyS, polyR);
}
