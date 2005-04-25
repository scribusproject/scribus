/***************************************************************************
*   Copyright (C) 2004 by Craig Bradney                                   *
*   cbradney@zip.com.au                                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "helpbrowser.h"
#include "helpbrowser.moc"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qheader.h>
#include <qlistview.h>
#include <qtextbrowser.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qfileinfo.h>
#include <qtextcodec.h>
#include <qdom.h>
#include <qdir.h>
#include <qsplitter.h>
#include <qmime.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qaccel.h>
#include <qinputdialog.h>

#ifdef _MSC_VER
#if (_MSC_VER >= 1200)
#include "win-config.h"
#endif
#else
#include "config.h"
#endif

extern QPixmap loadIcon(QString nam);


HelpBrowser::HelpBrowser( QWidget* parent, QString /*caption*/, QString guiLanguage, QString jumpToSection, QString jumpToFile)
	: QWidget( parent, "Help", WType_TopLevel | WDestructiveClose )
{
	QString fileName;
	mHistory.clear();
	struct histd his;
	language = guiLanguage=="" ? "en" : guiLanguage.left(2);
	helpBrowsermainLayout = new QVBoxLayout( this);
	buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing( 6 );
	buttonLayout->setMargin( 2 );

	homeButton = new QToolButton( this, "homeButton" );
	homeButton->setText( "" );
	homeButton->setPixmap(loadIcon("gohome.png"));
	homeButton->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed, homeButton->sizePolicy().hasHeightForWidth() ) );
	buttonLayout->addWidget( homeButton );

	histMenu = new QPopupMenu( this );
	backButton = new QToolButton( this, "backButton" );
	backButton->setPixmap(loadIcon("back.png"));
	backButton->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed, backButton->sizePolicy().hasHeightForWidth() ) );
	backButton->setPopup(histMenu);
	backButton->setPopupDelay(0);
	buttonLayout->addWidget( backButton );

	forwButton = new QToolButton( this, "forwButton" );
	forwButton->setPixmap(loadIcon("forward.png"));
	forwButton->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed, forwButton->sizePolicy().hasHeightForWidth() ) );
	buttonLayout->addWidget( forwButton );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	buttonLayout->addItem( spacer );
	helpBrowsermainLayout->addLayout( buttonLayout );

	helpBrowserLayout = new QHBoxLayout;
	splitter = new QSplitter(this, "splitter");
	splitter->setChildrenCollapsible( false );
	helpBrowserLayout->addWidget( splitter );

	tabWidget = new QTabWidget( splitter, "tabWidget" );
	//tabWidget = new QTabWidget( this, "tabWidget" );
	splitter->setResizeMode(tabWidget, QSplitter::Stretch );
	tabWidget->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding, false) );
	tabContents = new QWidget( tabWidget, "tabContents" );
	tabLayout = new QHBoxLayout( tabContents, 11, 6, "tabLayout");

	listView = new QListView( tabContents, "listView" );
	listView->addColumn( tr( "Contents" ) );
	listView->addColumn( tr( "Link" ) , 0 );
//	listView->addColumn( tr( "Link" ) );
	listView->setColumnWidthMode( 0, QListView::Maximum );
	listView->setColumnWidthMode( 1, QListView::Manual );
	listView->setSorting(-1,-1);
	listView->setRootIsDecorated( true );
	listView->setSelectionMode(QListView::Single);
	listView->setDefaultRenameAction(QListView::Reject);
	listView->clear();
	tabLayout->addWidget( listView );

	tabWidget->insertTab( tabContents, tr("&Contents") );
	//helpBrowserLayout->addWidget( tabWidget );

	// searching
	tabSearching = new QWidget(tabWidget, "tabSearching");
	searchingMainLayout = new QVBoxLayout(tabSearching, 11, 6, "searchingMainLayout");
	searchingButtonLayout = new QHBoxLayout;
	searchingEdit = new QLineEdit(tabSearching, "searchingEdit");
	searchingButton = new QPushButton(tr("&Search"), tabSearching, "searchingButton");
	searchingButtonLayout->addWidget(searchingEdit);
	searchingButtonLayout->addWidget(searchingButton);
	searchingMainLayout->addLayout(searchingButtonLayout);
	searchingView = new QListView(tabSearching, "searchingView");
	searchingView->addColumn( tr( "Contents" ) );
	searchingView->addColumn( tr( "Link" ) , 0 );
	searchingView->setColumnWidthMode( 0, QListView::Maximum );
	searchingView->setColumnWidthMode( 1, QListView::Manual );
	searchingView->setSorting(-1,-1);
	searchingView->setRootIsDecorated( true );
	searchingView->setSelectionMode(QListView::Single);
	searchingView->setDefaultRenameAction(QListView::Reject);
	searchingView->clear();
	searchingMainLayout->addWidget(searchingView);
	tabWidget->insertTab(tabSearching, tr("S&earch"));

	textBrowser = new QTextBrowser( splitter, "textBrowser" );
	splitter->setResizeMode(textBrowser, QSplitter::Stretch);
	//textBrowser = new QTextBrowser( this, "textBrowser" );
	textBrowser->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Expanding, false ) );
	textBrowser->setFrameShape( QTextBrowser::StyledPanel );
	QMimeSourceFactory *textBrowserMSF=textBrowser->mimeSourceFactory();
	textBrowserMSF->setExtensionType("html", "text/html;charset=UTF-8");
	//helpBrowserLayout->addWidget( textBrowser );
	helpBrowsermainLayout->addLayout( helpBrowserLayout );

	languageChange();
	resize( QSize(602, 491).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	loadMenu();
	listView->header()->hide();
	searchingView->header()->hide();
	jumpToHelpSection(jumpToSection, jumpToFile );

	QAccel *a = new QAccel( this, "a");
	a->connectItem(a->insertItem(QAccel::stringToKey(tr("Ctrl+F"))), this, SLOT(find()));
	a->connectItem(a->insertItem(QAccel::stringToKey(tr("F3"))), this, SLOT(findNext()));

	connect( homeButton, SIGNAL( clicked() ), textBrowser, SLOT( home() ) );
	connect( forwButton, SIGNAL( clicked() ), textBrowser, SLOT( forward() ) );
	connect( backButton, SIGNAL( clicked() ), textBrowser, SLOT( backward() ) );
	connect( histMenu, SIGNAL(activated(int)), this, SLOT(histChosen(int)));
	connect( listView, SIGNAL(clicked( QListViewItem *)), this, SLOT(itemSelected( QListViewItem *)));
	// searching
	connect( searchingEdit, SIGNAL(returnPressed()), this, SLOT(searchingButton_clicked()));
	connect(searchingView, SIGNAL(clicked( QListViewItem *)), this, SLOT(itemSearchSelected(QListViewItem *)));
	connect(searchingButton, SIGNAL(clicked()), this, SLOT(searchingButton_clicked()));
}

HelpBrowser::~HelpBrowser()
{
	// no need to delete child widgets, Qt does it all for us
}

void HelpBrowser::languageChange()
{
	setCaption( tr( "Scribus Online Help" ) );
	listView->header()->setLabel( 0, tr( "Contents" ) );
	listView->clear();

	tabWidget->changeTab( tabContents, tr( "&Contents" ) );
}


void HelpBrowser::sourceChanged(const QString& url)
{
	bool inList = false;
	struct histd his;
	QString title = "";
	title = textBrowser->documentTitle();
	if (title == "")
		title = url;
	for (QMap<int, histd>::Iterator it = mHistory.begin(); it != mHistory.end(); ++it)
	{
		if (it.data().Title == title)
			inList = true;
	}
	if (!inList)
	{
		his.Title = title;
		his.Url = url;
		mHistory[histMenu->insertItem(title)] = his;
	}
	if (mHistory.count() > 15)
	{
		int itk = histMenu->idAt(0);
		mHistory.remove(itk);
		histMenu->removeItem(itk);
	}
}

void HelpBrowser::histChosen(int i)
{
	if (mHistory.contains(i))
		textBrowser->setSource(mHistory[i].Url);
}

void HelpBrowser::jumpToHelpSection(QString jumpToSection, QString jumpToFile)
{
	QString toLoad;
	bool noDocs=false;

	if (jumpToFile=="")
	{
		QString pfad = DOCDIR;
		toLoad = pfad + language + "/"; //clean this later to handle 5 char locales
		if (jumpToSection=="")
		{
			if (listView->firstChild())
			{
				toLoad+=listView->firstChild()->text(1);
				listView->setSelected( listView->firstChild(), true );
			}
			else
				noDocs=true;
		}
		else if (jumpToSection=="scripter")
		{
			toLoad+="scripter1.html";
			QListViewItemIterator it(listView);
			while (it.current())
			{
				if (it.current()->text(1)=="scripter1.html")
					listView->setSelected( it.current(), true );
				if (it.current()->text(1)=="developers.html")
					it.current()->setOpen( true );
				++it;
			}
		}
	}
	else
		toLoad=jumpToFile;

	if (!noDocs)
		loadHelp(toLoad);
	else
		textBrowser->setText("<h2>"+ tr("Sorry, no manual available! Please see: http://docs.scribus.net for updated docs\nand www.scribus.net for downloads.")+"</h2>");
}

void HelpBrowser::loadHelp(QString filename)
{
	struct histd his;
	bool Avail = true;
	QString toLoad;
	QFileInfo fi;
	fi = QFileInfo(filename);
	if (fi.fileName().length()>0)
	{
		if (fi.exists())
			toLoad=filename;
		else
		{
			QString pfad = DOCDIR;
			toLoad =  QDir::convertSeparators(pfad + "en/index.html");
			language="en";
			fi = QFileInfo(toLoad);
			if (!fi.exists())
			{
				textBrowser->setText("<h2>"+ tr("Sorry, no manual available! Please see: http://docs.scribus.net for updated docs\nand www.scribus.net for downloads.")+"</h2>");
				Avail = false;
			}
		}
	}
	else
		Avail=false;
	if (Avail)
	{
		textBrowser->setSource(toLoad);
		his.Title = textBrowser->documentTitle();
		if (his.Title == "")
			his.Title = toLoad;
		his.Url = toLoad;
		mHistory[histMenu->insertItem(his.Title)] = his;
	}
}

void HelpBrowser::loadMenu()
{
	QString pfad = DOCDIR;
	QString toLoad;
	QString pfad2 = QDir::convertSeparators(pfad + language + "/menu.xml");
	QFileInfo fi = QFileInfo(pfad2);

	if (fi.exists())
		toLoad=pfad2;
	else
	{
		toLoad = QDir::convertSeparators(pfad + "en/menu.xml");
		language="en";
		fi = QFileInfo(toLoad);
		qDebug("Scribus help in your selected language does not exist, trying English. Otherwise, please visit http://docs.scribus.net.");
	}

	if (fi.exists())
	{
		QDomDocument doc( "menuentries" );
		QFile file( toLoad );
		if ( !file.open( IO_ReadOnly ) )
			return;
		if ( !doc.setContent( &file ) )
		{
			file.close();
			return;
		}
		file.close();

		QDomElement docElem = doc.documentElement();
		QDomNode n = docElem.firstChild();
		QListViewItem *qlvi=NULL, *qlvi2=NULL, *qlvi3=NULL, *qlvi4=NULL, *qlvi5=NULL, *qlvi6=NULL;
		QListViewItem *tutorialsMenuItem;
		bool haveTutorials=false;

		while( !n.isNull() )
		{
			QDomElement e = n.toElement(); // try to convert the node to an element.
			if( !e.isNull() )
			{
				if (e.hasAttribute( "text" ) && e.hasAttribute( "file" ))
				{
					QDomAttr textAttr = e.attributeNode( "text" );
					QDomAttr fileAttr = e.attributeNode( "file" );
					if (qlvi2==NULL)
						qlvi=new QListViewItem(listView, textAttr.value(), fileAttr.value());
					else
						qlvi=new QListViewItem(listView, qlvi2, textAttr.value(), fileAttr.value());
					if (qlvi!=NULL && e.hasAttribute( "section" ))
					{
						QDomAttr sectionAttr = e.attributeNode( "section" );
						if (sectionAttr.value()=="tutorials" && !haveTutorials)
						{
							haveTutorials=true;
							tutorialsMenuItem=qlvi;
						}
					}

					if (qlvi!=NULL)
						qlvi2=qlvi;
				}

				QDomNodeList nl=n.childNodes();
				for(uint i=0 ; i<= nl.count() ; i++)
				{
					QDomNode child=nl.item(i);
					if (child.isElement())
					{
						QDomElement ec = child.toElement();
						if (!ec.isNull())
						{
							if (ec.hasAttribute( "text" ) && ec.hasAttribute( "file" ))
							{
								QDomAttr textAttr = ec.attributeNode( "text" );
								QDomAttr fileAttr = ec.attributeNode( "file" );
								if (qlvi4==NULL)
									qlvi3=new QListViewItem(qlvi, textAttr.value(), fileAttr.value());
								else
									qlvi3=new QListViewItem(qlvi, qlvi4, textAttr.value(), fileAttr.value());
								if (qlvi3!=NULL && ec.hasAttribute( "section" ))
								{
									QDomAttr sectionAttr = e.attributeNode( "section" );
									if (sectionAttr.value()=="tutorials" && !haveTutorials)
									{
										haveTutorials=true;
										tutorialsMenuItem=qlvi3;
									}
								}
								if (qlvi3!=NULL)
									qlvi4=qlvi3;
							}
							//3rd level
							QDomNodeList nl2=child.childNodes();
							for(uint i=0 ; i<= nl2.count() ; i++)
							{
								QDomNode childchild=nl2.item(i);
								if (childchild.isElement())
								{
									QDomElement ecc = childchild.toElement();
									if (!ecc.isNull())
									{
										if (ecc.hasAttribute( "text" ) && ecc.hasAttribute( "file" ))
										{
											QDomAttr textAttr = ecc.attributeNode( "text" );
											QDomAttr fileAttr = ecc.attributeNode( "file" );
											if (qlvi6==NULL)
												qlvi5=new QListViewItem(qlvi3, textAttr.value(), fileAttr.value());
											else
												qlvi5=new QListViewItem(qlvi3, qlvi6, textAttr.value(), fileAttr.value());
											if (qlvi5!=NULL && ecc.hasAttribute( "section" ))
											{
												QDomAttr sectionAttr = e.attributeNode( "section" );
												if (sectionAttr.value()=="tutorials" && !haveTutorials)
												{
													haveTutorials=true;
													tutorialsMenuItem=qlvi5;
												}
											}
										}
										if (qlvi5!=NULL)
											qlvi6=qlvi5;
									}
								}
							}
						}
					}
				}
			}
			n = n.nextSibling();
		}
		//scan for installed tutorials
		if (haveTutorials)
		{
			QString path = DOCDIR;
			path += language + "/tutorials/";
			QDir dir(path, "*", QDir::Name, QDir::Dirs | QDir::NoSymLinks);

			if (dir.exists() && (dir.count() != 0))
			{
				for (uint i = 0; i < dir.count(); ++i)
				{
					if (dir[i]!="." && dir[i]!="..")
					{
						QString tutorialdir = "tutorials/" + dir[i] + "/";
						QFileInfo file(QDir::convertSeparators(path + dir[i] + "/menu.xml"));
						if (file.exists())  // menu.xml exists for tutorial
						{
							QDomDocument docTutorial( "tutorialmenuentries" );
							QFile fileTutorialMenu( file.filePath() );
							if ( !fileTutorialMenu.open( IO_ReadOnly ) )
								break;
							if ( !docTutorial.setContent( &fileTutorialMenu ) )
							{
								fileTutorialMenu.close();
								break;
							}
							fileTutorialMenu.close();

							QDomElement docElemTutorial = docTutorial.documentElement();
							QDomNode nTutorial = docElemTutorial.firstChild();
							QListViewItem *tutorialQLVI=NULL;

							while( !nTutorial.isNull() )
							{
								QDomElement eTutorial = nTutorial.toElement(); // try to convert the node to an element.
								if( !eTutorial.isNull() )
								{
									if (tutorialsMenuItem!=NULL && eTutorial.hasAttribute( "text" ) && eTutorial.hasAttribute( "file" ))
									{
										QDomAttr textAttr = eTutorial.attributeNode( "text" );
										QDomAttr fileAttr = eTutorial.attributeNode( "file" );
										tutorialQLVI=new QListViewItem(tutorialsMenuItem, textAttr.value(), tutorialdir + fileAttr.value());
									}
									QDomNodeList nl=nTutorial.childNodes();
									QListViewItem *tutorialSubMenuItem, *tutorialSubMenuItemLast=NULL;
									for(uint j=0 ; j<= nl.count() ; j++)
									{
										QDomNode child=nl.item(j);
										if (child.isElement())
										{
											QDomElement ec = child.toElement();
											if (!ec.isNull())
											{
												if (ec.hasAttribute( "text" ) && ec.hasAttribute( "file" ))
												{
													QDomAttr textAttr = ec.attributeNode( "text" );
													QDomAttr fileAttr = ec.attributeNode( "file" );
													if (tutorialSubMenuItemLast==NULL)
														tutorialSubMenuItem=new QListViewItem(tutorialQLVI, textAttr.value(), tutorialdir + fileAttr.value());
													else
														tutorialSubMenuItem=new QListViewItem(tutorialQLVI, tutorialSubMenuItemLast, textAttr.value(), tutorialdir + fileAttr.value());
													if (tutorialSubMenuItem!=NULL)
														tutorialSubMenuItemLast=tutorialSubMenuItem;
												}
											}
										}
									}
								}
								nTutorial = nTutorial.nextSibling();
							}
						}
					}
				}
			}

		}
	}
}

void HelpBrowser::itemSelected(QListViewItem *item)
{
	if ( !item )
		return;
	if (item->text(1)!=QString::null)
	{
		QString pfad = DOCDIR;
		loadHelp(QDir::convertSeparators(pfad + language + "/" + item->text(1)));
	}
}

void HelpBrowser::itemSearchSelected(QListViewItem *item)
{
	if (item && item->text(1) != QString::null)
	{
		loadHelp(item->text(1));
		findText = searchingEdit->text();
		findNext();
	}
}

void HelpBrowser::searchingInDirectory(QString aDir)
{
	QDir dir(aDir + "/");
	QStringList lst = dir.entryList("*.html");
	for (QStringList::Iterator it = lst.begin(); it != lst.end(); ++it)
	{
		QString fname(aDir + (*it));
		QFile f(fname);
		if (f.open(IO_ReadOnly))
		{
			QTextStream stream(&f);
			QString str = stream.read().lower();
			int cnt = str.contains(searchingEdit->text().lower());
			if (cnt > 0)
			{
				// the remove() hack is here for itemSelected() handling
				QString fullname = fname;
				QString title;
				QListViewItem *refItem = listView->findItem(fname.remove(QDir::convertSeparators(QString(DOCDIR)+language+"/")), 1);

				refItem ? title = refItem->text(0) : title = tr("unknown");
				QListViewItem *item = new QListViewItem(searchingView, QString("%1x %2").arg(cnt).arg(title), fullname);
				searchingView->insertItem(item);
			}
			f.close();
		}
	}
	// get dirs - ugly recursion
	QStringList dst = dir.entryList("*", QDir::Dirs);
	for (QStringList::Iterator it = dst.begin(); it != dst.end(); ++it)
		if ((*it)!="." && (*it)!="..")
			searchingInDirectory(QDir::convertSeparators(aDir + QString((*it)) + "/"));
}

void HelpBrowser::searchingButton_clicked()
{
	searchingView->clear();
	// root files
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	searchingInDirectory(QString(DOCDIR) + language + "/");
	QApplication::restoreOverrideCursor();
}

void HelpBrowser::find()
{
	findText = QInputDialog::getText(tr("Find"), tr("Search Term:"), QLineEdit::Normal, findText, 0, this);
	if (findText == QString::null)
		return;
	findNext();
}

void HelpBrowser::findNext()
{
	if (findText == QString::null)
	{
		find();
		return;
	}
	// find it. finally
	textBrowser->find(findText, false, false, true, 0, 0);
}
