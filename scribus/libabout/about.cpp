#include "about.h"
#include "about.moc"
#include <qpixmap.h>
#include <qtextstream.h>
#include <qtooltip.h>

#ifdef _MSC_VER
 #if (_MSC_VER >=1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

extern QPixmap loadIcon(QString nam);

extern "C" void* Run(QWidget *d);

/*!
 \fn void* Run(QWidget *d)
 \author Franz Schmid
 \date  
 \brief Creates About dialog and returns pointer to it
 \param d QWidget pointer to About dialog widget
 \retval dia returned widget pointer
 */
void* Run(QWidget *d)
{
	About *dia = new About(d);
	return dia;
}

/*!
 \fn About::About( QWidget* parent )
 \author Franz Schmid 
 \date  
 \brief Constructor for About dialog box
 \param parent QWidget pointer to parent window
 \retval About dialog
 */
About::About( QWidget* parent )
		: QDialog( parent, "About", true, 0 )
{
	setCaption( tr("About Scribus %1").arg(VERSION) );
	setIcon(loadIcon("AppIcon.png"));
	AboutLayout = new QVBoxLayout( this );
	AboutLayout->setSpacing( 6 );
	AboutLayout->setMargin( 10 );
	TabWidget2 = new QTabWidget( this, "TabWidget2" );
	TabWidget2->setMinimumSize( QSize( 438, 258 ) );
	tab = new QWidget( TabWidget2, "tab" );
	tabLayout1 = new QVBoxLayout( tab );
	tabLayout1->setSpacing( 6 );
	tabLayout1->setMargin( 15 );
	PixmapLabel1 = new QLabel( tab, "PixmapLabel1" );
	PixmapLabel1->setPixmap(loadIcon("scribus_logo.jpg"));
	PixmapLabel1->setAlignment(Qt::AlignCenter);
	tabLayout1->addWidget( PixmapLabel1 );
	BuildID = new QLabel( tab, "BB" );
	BuildID->setAlignment(Qt::AlignCenter);
	QString bu = tr("%1. %2 %3 ").arg("14").arg("February").arg("2005");
#ifdef HAVE_CMS
	bu += "C";
#else
	bu += "*";
#endif
	bu += "-";
#ifdef HAVE_CUPS
	bu += "C";
#else
	bu += "*";
#endif
	bu += "-";
#ifdef HAVE_TIFF
	bu += "T";
#else
	bu += "*";
#endif

	BuildID->setText( tr("Scribus Version %1\n%2 %3").arg(VERSION).arg( tr("Build-ID:")).arg(bu));
	tabLayout1->addWidget( BuildID );
	TabWidget2->insertTab( tab, tr( "&About" ) );
	tab_2 = new QWidget( TabWidget2, "tab_2" );
	tabLayout = new QHBoxLayout( tab_2 );
	tabLayout->setSpacing( 6 );
	tabLayout->setMargin( 10 );
	TextView1 = new QTextView( tab_2, "TextView1" );
	TextView1->setText(QString::fromUtf8("<table><tr><td><b>" + tr("Development Team:").utf8() + "</b></td><td> </td></tr>" +
										"<tr><td>Franz Schmid</td><td>Franz.Schmid@altmuehlnet.de</td></tr>" +
										"<tr><td>Peter Linnell</td><td>scribusdocs@atlantictechsolutions.com</td></tr>" + 
										"<tr><td>Paul F. Johnson</td><td>paul@all-the-johnsons.co.uk</td></tr>" +
										"<tr><td>Craig Bradney</td><td>cbradney@zip.com.au</td></tr>" +
										"<tr><td>Petr Vaněk</td><td>petr@yarpen.cz</td></tr>" +
										"<tr><td>Riku Leino</td><td>riku.leino@gmail.com</td></tr>" +
										"<tr><td> </td><td> </td></tr>" +

										"<tr><td><b>" + tr("Contributions from:").utf8()  + "</b></td><td></td></tr>" +
										"<tr><td>Niyam Bhushan</td><td></td></tr>" +
										"<tr><td>Martin Costabel</td><td></td></tr>" +
										"<tr><td>Steve Callcott</td><td>stotte@ntlworld.com</td></tr>" +
										"<tr><td>Frank Everdij</td><td>f.everdij@citg.tudelft.nl</td></tr>" +
										"<tr><td>S. Hakim Hamdani</td><td>hakimoto@eroteme.org</td></tr>" +
										"<tr><td>Maciej Hański</td><td>m.hanski@gmx.at</td></tr>" +
										"<tr><td>Oleksandr Moskalenko</td><td>malex@tagancha.org</td></tr>" +
										"<tr><td>Marti Maria</td><td></td></tr>" +
										"<tr><td>Alexandre Prokoudine</td><td>techwriter@land.ru</td></tr>"
										"<tr><td>Alessandro Rimoldi</td><td>http://ideale.ch/contact</td></tr>" +
										"<tr><td>Craig Ringer</td><td></td></tr>" +
										"<tr><td>Alastair Robinson</td><td>blackfive@fakenhamweb.co.uk</td></tr>" +
										"<tr><td>Johannes Rüschel</td><td>jo.rueschel@gmx.de</td></tr>" +
										"<tr><td>Jean-Jacques Sarton</td><td>jj.sarton@t-online.de</td></tr>" +
										"<tr><td>Christian Töpp</td><td>mr-ct@gmx.de</td></tr>" +	                                     
										"<tr><td></td><td> </td></tr>" +
#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
										"<tr><td><b>" + tr("Windows port:").utf8()  + "</b></td><td> </td></tr>" +
										"<tr><td>Paul F. Johnson</td><td>paul@all-the-johnsons.co.uk</td></tr>" +
										"<tr><td> </td><td> </td></tr>" +
 #endif
#endif
										"<tr><td><b>" + tr("Official Documentation:").utf8()  + "</b></td><td></td></tr>" +
										"<tr><td>Peter Linnell</td><td>scribusdocs@atlantictechsolutions.com</td></tr>" + 
										"<tr><td>Craig Bradney</td><td>cbradney@zip.com.au</td></tr>" +
										"<tr><td>Frédéric Dubuy</td><td>effediwhy@gmail.com</td></tr>" +
										"<tr><td>Johannes Rüschel</td><td>jo.rueschel@gmx.de</td></tr>" + 
										"<tr><td> </td><td> </td></tr>" +

										"<tr><td><b>" + tr("Other Documentation:").utf8()  + "</b></td><td></td></tr>" +
										"<tr><td>Yves Ceccone</td><td>yves@yeccoe.org</td></tr>" + 
										"<tr><td>Holger Reibold</td><td>http://www.bomots.de/scribus/</td></tr>" +
										"<tr><td>Thomas Zastrow</td><td>webmaster@thomas-zastrow.de</td></tr></table>"));
	TextView1->setTextFormat( QTextView::RichText );
	tabLayout->addWidget( TextView1 );
	TabWidget2->insertTab( tab_2, tr( "A&uthors" ) );
	tab_3 = new QWidget( TabWidget2, "tab_3" );
	tabLayout_2 = new QHBoxLayout( tab_3 );
	tabLayout_2->setSpacing( 6 );
	tabLayout_2->setMargin( 10 );
	TextView2 = new QTextView( tab_3, "TextView1_2" );
	TextView2->setText(QString::fromUtf8( "<table><tr><td><b><i>" + tr("Official Translations and Translators:").utf8() + "</i></b></td><td></td></tr>" +
	                                      "<tr><td><b>" + QString("Afrikaans:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Kobus Wolvaardt</td><td>kobuswolf@diewereld.co.za</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
									      "<tr><td><b>" + tr("Catalan:").utf8()  + "</b></td><td> </td></tr>" +
									      "<tr><td>Xavier Sala Pujolar</td><td>utrescu@xaviersala.net</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Czech:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Petr Vaněk</td><td>petr@yarpen.cz</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Danish:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Morten Langlo</td><td>mlanglo@mail.dk</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + QString("Dutch:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Wilbert Berendsen</td><td>wbsoft@xs4all.nl</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("English (British):").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Craig Bradney</td><td>cbradney@zip.com.au</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +                                        
	                                      "<tr><td><b>" + tr("Esperanto:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Pier Luigi Cinquantini</td><td>plcinquantini@katamail.com</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Finnish:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Riku Leino</td><td>riku.leino@gmail.com</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +                                      
	                                      "<tr><td><b>" + tr("French:").utf8()  + "</b></td><td> </td>" +
	                                      "<tr><td>Nicolas Boos</td><td>nicolas.boos@wanadoo.fr</td></tr>" +
	                                      "<tr><td>Louis Desjardins</td><td>louis_desjardins@mardigrafe.com</td></tr>" +
	                                      "<tr><td>Frédéric Dubuy</td><td>effediwhy@gmail.com</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("German:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Johannes Rüschel</td><td>jo.rueschel@gmx.de</td></tr>" +
	                                      "<tr><td>Franz Schmid</td><td>Franz.Schmid@altmuehlnet.de</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Italian:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Pier Luigi Cinquantini</td><td>plcinquantini@katamail.com</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Korean:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Kitae Kim</td><td>neeum@yahoo.com</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +               
	                                      "<tr><td><b>" + tr("Norwegian:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Klaus Ade Johnstad</td><td>klaus@inout.no</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Polish:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Maciej Hański</td><td>m.hanski@gmx.at</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Portugese (Brazilian):").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Ludi Maciel</td><td>iludi@uol.com.br</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +	
	                                      "<tr><td><b>" + tr("Russian:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Alexandre Prokoudine</td><td>techwriter@land.ru</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Serbian:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Bojan Božovi&#263;</td><td>bole89@infosky.net</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Slovak:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Zdenko Podobn&yacute;</td><td>zdpo@mailbox.sk</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Slovenian:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Boštjan Špetič</td><td>igzebedze@kiberpipa.org</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +  
	                                      "<tr><td><b>" + tr("Spanish:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Josep Febrer</td><td>josep@linuxmail.org</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +  
	                                      "<tr><td><b>" + tr("Swedish:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Göran Bondeson</td><td>goran@bondeson.net</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Ukrainian:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Oleksandr Moskalenko</td><td>malex@tagancha.org</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +  
	                                      "<tr><td><b>" + tr("Welsh:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Kevin Donnelly</td><td>kevin@dotmon.com</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "</table>" +

										  "<table><tr><td><b><i>" + tr("Previous Translation Contributors:").utf8() + "</i></b></td><td></td></tr>" +
										  "<tr><td><b>" + tr("Basque:").utf8() + "</b></td><td></td></tr>" +
	                                      "<tr><td>Pablo Saratxaga</td><td>pablo@mandrakesoft.com</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Brazilian:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Celio Santos</td><td>celio@electronic.srv.br</td></tr>" +
	                                      "<tr><td>Cezar de Souza Marson Nido</td><td>cesar@electronic.srv.br</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Bulgarian:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Vasko Tomanov</td><td>vasko@web.bg</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("English (British):").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Paul F. Johnson</td><td>paul@all-the-johnsons.co.uk</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +                                        
	                                      "<tr><td><b>" + tr("French:").utf8()  + "</b></td><td> </td>" +
	                                      "<tr><td>Michel Briand</td><td>michelbriand@free.fr</td></tr>" +
	                                      "<tr><td>Yves Ceccone</td><td>yves@yeccoe.org</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Galician:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Xose Calvo</td><td>xosecalvo@galizaweb.net</td></tr>" +
	                                      "<tr><td>Manuel Anxo Rei</td><td>manxopar@avogaciagalega.org</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +                                      
	                                      "<tr><td><b>" + tr("Hungarian:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Giovanni Biczó</td><td>gbiczo@freestart.hu</td></tr>" +
	                                      "<tr><td>Bence Nagy</td><td>scribus@tipogral.hu</td></tr>" +
	                                      "<tr><td>Zoltán Böszörményi</td><td>zboszor@freemail.hu</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Lithuanian:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Aivaras Kirejevas</td><td>kiras@mail.lt</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +                                      
	                                      "<tr><td><b>" + tr("Norwegian:").utf8()  + "</b></td><td></td></tr>" +
	                                      "<tr><td>Axel Bojer</td><td>axelb@skolelinux.no</td></tr>" +
	                                      "<tr><td>Johannes Wilm</td><td>j@indymedia.no</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +
	                                      "<tr><td><b>" + tr("Turkish:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Erkan Kaplan</td><td>Selamsana@uni.de</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +  
	                                      "<tr><td><b>" + tr("Ukrainian:").utf8()  + "</b></td><td> </td></tr>" +
	                                      "<tr><td>Sergiy Kudryk</td><td>kudryk@yahoo.com</td></tr>" +
	                                      "<tr><td> </td><td> </td></tr>" +  
	                                      "</table>"));
	TextView2->setTextFormat( QTextView::RichText );
	tabLayout_2->addWidget( TextView2 );
	TabWidget2->insertTab( tab_3, tr( "&Translations" ) );

	// online tab (03/04/2004 petr vanek)
	tab_4 = new QWidget( TabWidget2, "tab_4" );
	TextView4 = new QTextView( tab_4, "TextView4" );
	TextView4->setText(QString::fromUtf8(
		"<table><tr><td><b>" + tr("Homepage").utf8() + "</b></td><td></td></tr>" +
		"<tr><td colspan=\"2\"><p><a href=\"http://www.scribus.net\">http://www.scribus.net</a></p></td></tr>" +
		"<tr><td><b>" + tr("Online Reference").utf8() + "</b></td><td></td></tr>" +
		"<tr><td colspan=\"2\"><p><a href=\"http://docs.scribus.net\">http://docs.scribus.net</a></p></td></tr>" +
		"<tr><td><b>" + tr("Bugs and Feature Requests").utf8() + "</b></td><td></td></tr>" +
		"<tr><td colspan=\"2\"><p><a href=\"http://bugs.scribus.net\">http://bugs.scribus.net</a></p></td></tr>" +
		"<tr><td><b>" + tr("Mailing List").utf8() + "</b></td><td></td></tr>" +
		"<tr><td colspan=\"2\"><p><a href=\"http://nashi.altmuehlnet.de/mailman/listinfo/scribus\">http://nashi.altmuehlnet.de/mailman/listinfo/scribus</a></p></td></tr>" +
		"</table>"
		));
	TextView4->setTextFormat( QTextView::RichText );
	tabLayout_4 = new QHBoxLayout( tab_4 );
	tabLayout_4->setSpacing( 6 );
	tabLayout_4->setMargin( 10 );
	tabLayout_4->addWidget( TextView4 );
	TabWidget2->insertTab( tab_4, tr( "&Online" ) );
	AboutLayout->addWidget( TabWidget2 );
	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );
	PushButton1 = new QPushButton( tr( "&Close" ), this, "PushButton1" );
	PushButton1->setDefault( true );
	Layout2->addWidget( PushButton1 );
	AboutLayout->addLayout( Layout2 );
	setMaximumSize(sizeHint());

//tooltips
	QToolTip::add( BuildID, tr( "This panel shows the version, build date and\n compiled in library support in Scribus\nThe C-C-T equates to C=littlecms C=CUPS T=TIFF support.\nMissing library support is indicated by a *" ) );
	// signals and slots connections
	connect( PushButton1, SIGNAL( clicked() ), this, SLOT( accept() ) );
}



