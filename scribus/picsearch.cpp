#include "picsearch.h"
#include "picsearch.moc"
#include <qpixmap.h>
#include "scribusstructs.h"
#include "scimage.h"
extern QPixmap loadIcon(QString nam);

/*!
 \fn PicSearch::PicSearch(QWidget* parent, QString name, QStringList alt)
 \author Franz Schmid
 \date
 \brief Constructor for PicSearch.[dox?]. Used in Extras / Manage Pictures / Search function
 \param parent QWidget pointer to parent window
 \param name QString name of image
 \param alt QStringList List of Paths where an Image with the given Name is present
 \retval None
 */
PicSearch::PicSearch(QWidget* parent, const QString & fileName, const QStringList & alt)
    : QDialog( parent, "pi2", true, 0 )
{
    setCaption( tr( "Result" ) );
  	setIcon(loadIcon("AppIcon.png"));
  	Bild = "";
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1,
								sizePolicy().hasHeightForWidth() ) );
    PicSearchLayout = new QVBoxLayout( this ); 
    PicSearchLayout->setSpacing( 6 );
    PicSearchLayout->setMargin( 11 );

    Layout2 = new QHBoxLayout; 
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Search Results for: " ) );
    Layout2->addWidget( TextLabel1 );

    Datei = new QLabel( this, "Datei" );
    Datei->setText( tr( fileName ) );
    Layout2->addWidget( Datei );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout2->addItem( spacer );
    PicSearchLayout->addLayout( Layout2 );

    Layout3 = new QHBoxLayout; 
    Layout3->setSpacing( 6 );
    Layout3->setMargin( 0 );

    ListBox1 = new QListBox( this, "ListBox1" );
    ListBox1->setMinimumSize( QSize( 250, 210 ) );
    ListBox1->setMaximumSize( QSize( 250, 32767 ) );
    ListBox1->insertStringList( alt );
    Layout3->addWidget( ListBox1 );

    Layout1 = new QVBoxLayout; 
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );

    Preview = new QCheckBox( this, "Preview" );
    Preview->setText( tr( "Preview" ) );
    Preview->setChecked(false);
    Layout1->addWidget( Preview );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout1->addItem( spacer_2 );

    UseB = new QPushButton( this, "UseB" );
    UseB->setMinimumSize( QSize( 100, 30 ) );
    UseB->setText( tr( "Select" ) );
    UseB->setDefault( true );
    UseB->setEnabled(false);
    Layout1->addWidget( UseB );

    CancelB = new QPushButton( this, "CancelB" );
    CancelB->setMinimumSize( QSize( 100, 30 ) );
    CancelB->setText( tr( "Cancel" ) );
    Layout1->addWidget( CancelB );
    Layout3->addLayout( Layout1 );

    PixmapLabel1 = new QLabel( this, "PixmapLabel1" );
    PixmapLabel1->setMinimumSize( QSize( 210, 210 ) );
    PixmapLabel1->setAlignment(AlignLeft | AlignTop);
    PixmapLabel1->setScaledContents( FALSE );
    PixmapLabel1->setBackgroundColor( white );
    PixmapLabel1->setFrameShape( QLabel::WinPanel );
    PixmapLabel1->setFrameShadow( QLabel::Sunken );
    Layout3->addWidget( PixmapLabel1 );
	PixmapLabel1->hide();
    PicSearchLayout->addLayout( Layout3 );
    minS = minimumSize();
    // signals and slots connections
    connect( CancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( UseB, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Preview, SIGNAL( clicked() ), this, SLOT( ShowPrev() ) );
  	connect(ListBox1, SIGNAL(clicked(QListBoxItem*)), this, SLOT(selBild(QListBoxItem*)));
}

/*!
 \fn void PicSearch::ShowPrev()
 \author Franz Schmid
 \date
 \brief If preview is desired (checked) then the image preview is shown and generated, otherwise hidden.
 \param None
 \retval None
 */
void PicSearch::ShowPrev()
{
	if (Preview->isChecked())
	{
		PixmapLabel1->show();
		if (!Bild.isEmpty())
			GenPreview();
	}
	else
	{
		PixmapLabel1->hide();
		setMinimumSize(minS);
		resize(minS);
	}
}

/*!
 \fn void PicSearch::selBild(QListBoxItem *c)
 \author Franz Schmid
 \date
 \brief When image is selected from the ListBox then the image preview may be shown and the Use button is enabled.
 \param c QListBoxItem
 \retval None
 */
void PicSearch::selBild(QListBoxItem *c)
{
	if (c == NULL)
		return;
	Bild = c->text();
	if (Preview->isChecked())
		GenPreview();
	UseB->setEnabled(true);
}

/*!
 \fn void PicSearch::GenPreview()
 \author Franz Schmid
 \date
 \brief Generates image preview for the found Picture
 \param None
 \retval None
 */
void PicSearch::GenPreview()
{
	QPixmap pm;
	ScImage im;
	im.LoadPicture(Bild, "", 0, false, false, 1, 72);
	if ((im.width() > 200) || (im.height() > 200))
	{
		QImage im2;
		double sx = im.width() / 200.0;
		double sy = im.height() / 200.0;
		im2 = sy < sx ?
			im2 = im.smoothScale(qRound(im.width() / sx), qRound(im.height() / sx)) :
			im2 = im.smoothScale(qRound(im.width() / sy), qRound(im.height() / sy));
		im = im2;
		im2.detach();
	}
	pm.convertFromImage(im);
	PixmapLabel1->setPixmap(pm);
}	
