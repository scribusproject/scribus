/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2004 by Craig Bradney                                   *
*   cbradney@zip.com.au                                                   *
*   Copyright (C) 2005 by Petr Vanek                                      *
*   petr@yarpen.cz                                                        *
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

#include "helpbrowser2.h"

#include <QAction>
#include <QDir>
#include <QDomDocument>
#include <QFileInfo>
#include <QHeaderView>
#include <QInputDialog>
#include <QList>
#include <QPushButton>
#include <QTextEdit>


#include "prefsmanager.h"

HelpBrowser2::HelpBrowser2(QWidget* parent)
	: QMainWindow( parent )
{
	setupUi(this);
}

HelpBrowser2::HelpBrowser2( QWidget* parent, const QString& /*caption*/, const QString& guiLanguage, const QString& jumpToSection, const QString& jumpToFile)
	: QMainWindow( parent )
{
	setupUi(this);
	setupLocalUI();

	language = guiLanguage.isEmpty() ? QString("en") : guiLanguage.left(2);

	loadMenu();
}

HelpBrowser2::~HelpBrowser2()
{
}

void HelpBrowser2::setupLocalUI()
{
	//Add Menus
	fileMenu=menuBar()->addMenu(tr("&File"));
	editMenu=menuBar()->addMenu(tr("&Edit"));
	bookMenu=menuBar()->addMenu(tr("&Bookmarks"));
	histMenu=new QMenu(this);

	//Add Menu items
	filePrint=fileMenu->addAction(loadIcon("16/document-print.png"), tr("&Print..."), this, SLOT(print()), Qt::CTRL+Qt::Key_P);
	fileMenu->insertSeparator();
	fileExit=fileMenu->addAction(loadIcon("exit.png"), tr("E&xit"), this, SLOT(close()));
	editFind=editMenu->addAction(loadIcon("find.png"), tr("&Find..."), this, SLOT(find()), Qt::CTRL+Qt::Key_F);
	editFindNext=editMenu->addAction( tr("Find &Next"), this, SLOT(findNext()), Qt::Key_F3);
	editFindPrev=editMenu->addAction( tr("Find &Previous"), this, SLOT(findPrevious()), Qt::SHIFT+Qt::Key_F3);
	bookAdd=bookMenu->addAction( tr("&Add Bookmark"), this, SLOT(bookmarkButton_clicked()), Qt::CTRL+Qt::Key_D);
	bookDel=bookMenu->addAction( tr("&Delete"), this, SLOT(deleteBookmarkButton_clicked()));
	bookDelAll=bookMenu->addAction( tr("D&elete All"), this, SLOT(deleteAllBookmarkButton_clicked()));

	//Add Toolbar items
	goHome=toolBar->addAction(loadIcon("16/go-home.png"), "");//, textBrowser, SLOT(home()));
	goBack=toolBar->addAction(loadIcon("16/go-previous.png"), "");//, textBrowser, SLOT(forward()));
	goFwd=toolBar->addAction(loadIcon("16/go-next.png"), "");//, textBrowser, SLOT(backward()));
	goBack->setMenu(histMenu);
	
	

	listView->header()->hide();
	searchingView->header()->hide();
	bookmarksView->header()->hide();

	splitter->setResizeMode( tabWidget, QSplitter::KeepSize );
	splitter->setResizeMode( textBrowser, QSplitter::Stretch );
	// reset previous size
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("helpbrowser");
	int xsize = prefs->getUInt("xsize", 640);
	int ysize = prefs->getUInt("ysize", 480);
	resize(QSize(xsize, ysize).expandedTo(minimumSizeHint()) );

	// searching
	connect(searchingEdit, SIGNAL(returnPressed()), this, SLOT(searchingButton_clicked()));
	connect(searchingButton, SIGNAL(clicked()), this, SLOT(searchingButton_clicked()));
	// bookmarks
	connect(bookmarkButton, SIGNAL(clicked()), this, SLOT(bookmarkButton_clicked()));
	connect(deleteBookmarkButton, SIGNAL(clicked()), this, SLOT(deleteBookmarkButton_clicked()));
	connect(deleteAllBookmarkButton, SIGNAL(clicked()), this, SLOT(deleteAllBookmarkButton_clicked()));
}

void HelpBrowser2::languageChange()
{
	setCaption( tr( "Scribus Online Help" ) );
// 	listView->header()->setLabel( 0, tr( "Contents" ) );
// 	listView->clear();

}

void HelpBrowser2::print()
{
}

void HelpBrowser2::searchingButton_clicked()
{
}

void HelpBrowser2::find()
{
	findText = QInputDialog::getText( tr("Find"), tr("Search Term:"), QLineEdit::Normal, findText, 0, this);
	if (findText.isNull())
		return;
	findNext();
}

void HelpBrowser2::findNext()
{
	if (findText.isNull())
	{
		find();
		return;
	}
	// find it. finally
	textBrowser->find(findText, 0);
}

void HelpBrowser2::findPrevious()
{
	if (findText.isNull())
	{
		find();
		return;
	}
	// find it. finally
	textBrowser->find(findText);
}

void HelpBrowser2::bookmarkButton_clicked()
{
	QString title = textBrowser->documentTitle();
	QString fname(QDir::cleanDirPath(textBrowser->source()));
	title = QInputDialog::getText( tr("New Bookmark"), tr("New Bookmark's Title:"), QLineEdit::Normal, title, 0, this);
	// user cancel
	if (title.isNull())
		return;
// 	Q3ListViewItem *item = new Q3ListViewItem(bookmarksView, title, fname);
// 	bookmarksView->insertItem(item);
}

void HelpBrowser2::deleteBookmarkButton_clicked()
{
	delete(bookmarksView->currentItem());
}

void HelpBrowser2::deleteAllBookmarkButton_clicked()
{
	bookmarksView->clear();
}


void HelpBrowser2::loadHelp(const QString& filename)
{
	struct histd2 his;
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
			toLoad = QDir::convertSeparators(ScPaths::instance().docDir() + "en/index.html");
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
		his.title = textBrowser->documentTitle();
		if (his.title.isEmpty())
			his.title = toLoad;
		his.url = toLoad;
		mHistory[histMenu->insertItem(his.title)] = his;
	}
	if (mHistory.count() > 15)
	{
		int itk = histMenu->idAt(0);
		mHistory.remove(itk);
		histMenu->removeItem(itk);
	}
}

void HelpBrowser2::loadMenu()
{
	QString pfad = ScPaths::instance().docDir();
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
		sDebug("Scribus help in your selected language does not exist, trying English. Otherwise, please visit http://docs.scribus.net.");
	}

	if (fi.exists())
	{
		QDomDocument doc( "menuentries" );
		QFile file( toLoad );
		if ( !file.open( QIODevice::ReadOnly ) )
			return;
		if ( !doc.setContent( &file ) )
		{
			file.close();
			return;
		}
		file.close();

		QDomElement docElem = doc.documentElement();
		QDomNode n = docElem.firstChild();
/*
		Q3ListViewItem *qlvi=NULL, *qlvi2=NULL, *qlvi3=NULL, *qlvi4=NULL, *qlvi5=NULL, *qlvi6=NULL;
		Q3ListViewItem *tutorialsMenuItem=NULL;
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
						qlvi=new Q3ListViewItem(listView, textAttr.value(), fileAttr.value());
					else
						qlvi=new Q3ListViewItem(listView, qlvi2, textAttr.value(), fileAttr.value());
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
				for(int i=0 ; i<= nl.count() ; i++)
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
									qlvi3=new Q3ListViewItem(qlvi, textAttr.value(), fileAttr.value());
								else
									qlvi3=new Q3ListViewItem(qlvi, qlvi4, textAttr.value(), fileAttr.value());
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
							for(int i=0 ; i<= nl2.count() ; i++)
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
												qlvi5=new Q3ListViewItem(qlvi3, textAttr.value(), fileAttr.value());
											else
												qlvi5=new Q3ListViewItem(qlvi3, qlvi6, textAttr.value(), fileAttr.value());
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
		*/
		/*
		//scan for installed tutorials
		if (haveTutorials)
		{
			QString path = ScPaths::instance().docDir();
			path += language + "/tutorials/";
			QDir dir(path, "*", QDir::Name, QDir::Dirs | QDir::NoSymLinks);

			if (dir.exists() && (dir.count() != 0))
			{
				for (uint i = 0; i < dir.count(); ++i)
				{
					if (dir[i]!="." && dir[i]!="..")
					{
						QString tutorialdir = QDir::convertSeparators("tutorials/" + dir[i] + "/");
						QFileInfo file(QDir::convertSeparators(path + dir[i] + "/menu.xml"));
						if (file.exists())  // menu.xml exists for tutorial
						{
							QDomDocument docTutorial( "tutorialmenuentries" );
							QFile fileTutorialMenu( file.filePath() );
							if ( !fileTutorialMenu.open( QIODevice::ReadOnly ) )
								break;
							if ( !docTutorial.setContent( &fileTutorialMenu ) )
							{
								fileTutorialMenu.close();
								break;
							}
							fileTutorialMenu.close();

							QDomElement docElemTutorial = docTutorial.documentElement();
							QDomNode nTutorial = docElemTutorial.firstChild();
							Q3ListViewItem *tutorialQLVI=NULL;

							while( !nTutorial.isNull() )
							{
								QDomElement eTutorial = nTutorial.toElement(); // try to convert the node to an element.
								if( !eTutorial.isNull() )
								{
									if (tutorialsMenuItem!=NULL && eTutorial.hasAttribute( "text" ) && eTutorial.hasAttribute( "file" ))
									{
										QDomAttr textAttr = eTutorial.attributeNode( "text" );
										QDomAttr fileAttr = eTutorial.attributeNode( "file" );
										tutorialQLVI=new Q3ListViewItem(tutorialsMenuItem, textAttr.value(), tutorialdir + fileAttr.value());
									}
									QDomNodeList nl=nTutorial.childNodes();
									Q3ListViewItem *tutorialSubMenuItem, *tutorialSubMenuItemLast=NULL;
									for(int j=0 ; j<= nl.count() ; j++)
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
														tutorialSubMenuItem=new Q3ListViewItem(tutorialQLVI, textAttr.value(), tutorialdir + fileAttr.value());
													else
														tutorialSubMenuItem=new Q3ListViewItem(tutorialQLVI, tutorialSubMenuItemLast, textAttr.value(), tutorialdir + fileAttr.value());
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
		*/
	}
}

void HelpBrowser2::readBookmarks()
{
// 	BookmarkParser handler;
// 	handler.view = bookmarksView;
// 	QFile xmlFile(bookmarkFile());
// 	QXmlInputSource source(xmlFile);
// 	QXmlSimpleReader reader;
// 	reader.setContentHandler(&handler);
// 	reader.parse(source);
}

void HelpBrowser2::readHistory()
{
// 	HistoryParser handler;
// 	handler.helpBrowser = this;
// 	QFile xmlFile(historyFile());
// 	QXmlInputSource source(xmlFile);
// 	QXmlSimpleReader reader;
// 	reader.setContentHandler(&handler);
// 	reader.parse(source);
}

void HelpBrowser2::setText(const QString& str)
{
	textBrowser->setText(str);
}
