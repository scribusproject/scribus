/****************************************************************************
** Form implementation generated from reading ui file 'Help.ui'
**
** Created: Tue Jul 3 21:51:52 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "helpbrowser.h"
#include "helpbrowser.moc"
#include <qvariant.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtextcodec.h>
#include <qfileinfo.h>

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

extern QPixmap loadIcon(QString nam);

/*
 *  Constructs a HelpBrowser which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
HelpBrowser::HelpBrowser( QWidget* parent, QString Capt, QString Datei )
		: QWidget( parent, "Help", WType_TopLevel | WDestructiveClose )
{
	resize( 547, 450 );
	setCaption( Capt );
	mHistory.clear();
	struct histd his;
	setIcon(loadIcon("AppIcon.png"));
	HelpBrowserLayout = new QVBoxLayout( this );
	HelpBrowserLayout->setSpacing( 2 );
	HelpBrowserLayout->setMargin( 2 );

	Layout13 = new QHBoxLayout;
	Layout13->setSpacing( 6 );
	Layout13->setMargin( 0 );

	HomeB = new QToolButton( this, "HomeB" );
	HomeB->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1,
	                                   HomeB->sizePolicy().hasHeightForWidth() ) );
	HomeB->setMinimumSize( QSize( 0, 0 ) );
	HomeB->setMaximumSize( QSize( 32767, 32767 ) );
	HomeB->setText( "" );
	HomeB->setPixmap(loadIcon("gohome.png"));
	Layout13->addWidget( HomeB );

	hist = new QPopupMenu( this );
	BackB = new QToolButton( this, "BackB" );
	BackB->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1,
	                                   BackB->sizePolicy().hasHeightForWidth() ) );
	BackB->setMinimumSize( QSize( 0, 0 ) );
	BackB->setMaximumSize( QSize( 32767, 32767 ) );
	BackB->setPixmap(loadIcon("back.png"));
	BackB->setPopup(hist);
	BackB->setPopupDelay(0);
	Layout13->addWidget( BackB );

	ForwB = new QToolButton( this, "ForwB" );
	ForwB->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1,
	                                   ForwB->sizePolicy().hasHeightForWidth() ) );
	ForwB->setMinimumSize( QSize( 0, 0 ) );
	ForwB->setMaximumSize( QSize( 32767, 32767 ) );
	ForwB->setPixmap(loadIcon("forward.png"));
	Layout13->addWidget( ForwB );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout13->addItem( spacer );
	HelpBrowserLayout->addLayout( Layout13 );

	Anzeige = new QTextBrowser( this, "Anzeige" );
	Anzeige->setFrameShape( QTextBrowser::StyledPanel );
	QString pfad = PREL;
	QString pfad2;
	bool Avail = true;
	if (Datei == "")
		pfad2 = pfad + "/share/scribus/doc/"+QString(QTextCodec::locale()).left(2)+"/index.html";
	else
		pfad2 = Datei;
	QFileInfo fi;
	fi = QFileInfo(pfad2);
	if (!fi.exists())
	{
		pfad2 = pfad + "/share/scribus/doc/en/index.html";
		fi = QFileInfo(pfad2);
		if (!fi.exists())
		{
			Anzeige->setText("<h2>"+ tr("Sorry, no manual available! Please see: http://scribus.net for updated docs and downloads.")+"</h2>");
			Avail = false;
		}
	}
	if (Avail)
	{
		Anzeige->setSource(pfad2);
		his.Title = Anzeige->documentTitle();
		if (his.Title == "")
			his.Title = pfad2;
		his.Url = pfad2;
		mHistory[hist->insertItem(his.Title)] = his;
	}
	HelpBrowserLayout->addWidget( Anzeige );

	// signals and slots connections
	connect( HomeB, SIGNAL( clicked() ), Anzeige, SLOT( home() ) );
	connect( ForwB, SIGNAL( clicked() ), Anzeige, SLOT( forward() ) );
	connect( BackB, SIGNAL( clicked() ), Anzeige, SLOT( backward() ) );
	connect(hist, SIGNAL(activated(int)), this, SLOT(histChosen(int)));
	connect(Anzeige, SIGNAL(sourceChanged(const QString&)),this, SLOT(sourceChanged(const QString&)));
}

void HelpBrowser::sourceChanged(const QString& url)
{
	bool inList = false;
	struct histd his;
	QString title = "";
	title = Anzeige->documentTitle();
	if (title == "")
		title = url;
	QMap<int, histd>::Iterator it;
	for (it = mHistory.begin(); it != mHistory.end(); ++it)
	{
		if (it.data().Title == title)
			inList = true;
	}
	if (!inList)
	{
		his.Title = title;
		his.Url = url;
		mHistory[hist->insertItem(title)] = his;
	}
	if (mHistory.count() > 15)
	{
		int itk = hist->idAt(0);
		mHistory.remove(itk);
		hist->removeItem(itk);
	}
}

void HelpBrowser::histChosen(int i)
{
	if (mHistory.contains(i))
		Anzeige->setSource(mHistory[i].Url);
}

