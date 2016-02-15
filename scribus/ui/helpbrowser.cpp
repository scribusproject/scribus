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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#include "helpbrowser.h"
#include "ui/helpnavigation.h"

#include <QAction>
// #include <QDebug>
#include <QDir>
#include <QDockWidget>
#include <QDomDocument>
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QList>
#include <QMessageBox>
#include <QModelIndex>
#include <QModelIndexList>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QProcess>
#include <QPushButton>
#include <QString>
#include <QStandardItem>
#include <QTextEdit>
#include <QTreeView>
#include <QXmlDefaultHandler>

#include "prefsmanager.h"
#include "scribuscore.h"
#include "util_debug.h"
#include "iconmanager.h"
#include "util.h"

/*! \brief XML parsef for documantation history.
This is small helper class which reads saved bookmarks configuration
from ~/.scribus/doc/history.xml file.
The reference to historyBrowser is a reference to the dialog.
\author Petr Vanek <petr@yarpen.cz>
*/
class HistoryParser2 : public QXmlDefaultHandler
{
	public:
		HelpBrowser *helpBrowser;

		bool startDocument()
		{
			return true;
		}

		bool startElement(const QString&, const QString&, const QString& qName, const QXmlAttributes& attrs)
		{
			if (qName == "item")
			{
				struct histd2 his;
				his.title = attrs.value(0);
				his.url = attrs.value(1);
				helpBrowser->mHistory[helpBrowser->histMenu->addAction(his.title)] = his;
			}
			return true;
		}

		bool endElement(const QString&, const QString&, const QString&)
		{
			return true;
		}
};

/*! \brief XML parsef for documantation bookmarks.
This is small helper class which reads saved bookmarks configuration
from ~/.scribus/doc/bookmarks.xml file.
The reference to QListView *view is a reference to the list view with bookmarks
\author Petr Vanek <petr@yarpen.cz>
*/
class BookmarkParser2 : public QXmlDefaultHandler
{
	public:
		QTreeWidget* view;
		QMap<QString, QString>* quickHelpIndex;
		QMap<QString, QPair<QString, QString> >* bookmarkIndex;

		bool startDocument()
		{
			return true;
		}

		bool startElement(const QString&, const QString&, const QString& qName, const QXmlAttributes& attrs)
		{
			if (qName == "item")
			{
				//TODO : This will dump items if bookmarks get loaded into a different GUI language
				if (quickHelpIndex->contains(attrs.value(1)))
				{
					bookmarkIndex->insert(attrs.value(0), qMakePair(attrs.value(1), attrs.value(2)));
					view->addTopLevelItem(new QTreeWidgetItem(view, QStringList() << attrs.value(0)));
				}
			}
			return true;
		}

		bool endElement(const QString&, const QString&, const QString&)
		{
			return true;
		}
};

bool HelpBrowser::firstRun = true;

HelpBrowser::HelpBrowser( QWidget* parent, const QString& /*caption*/, const QString& guiLanguage, const QString& jumpToSection, const QString& jumpToFile)
	: QMainWindow( parent )
{
	firstRun=true;
	setupUi(this);
	helpNav = new HelpNavigation(this);
#if defined(Q_OS_MAC) && defined(HELP_NAV_AS_DRAWER)
	// TODO
#else
	QDockWidget * dckw = new QDockWidget(tr("Navigation"),this);
	dckw->setWidget(helpNav);
	addDockWidget(Qt::LeftDockWidgetArea, dckw);
#endif
	progressBar = new QProgressBar(this);
	progressBar->setRange(0,100);
	statusBar()->addPermanentWidget(progressBar);

	setupLocalUI();
	language = guiLanguage.isEmpty() ? QString("en") : guiLanguage.left(2);
	finalBaseDir = ScPaths::instance().docDir() + "en/"; //Sane default for help location
	textBrowser->setSearchPaths(QStringList(finalBaseDir));
	menuModel=NULL;
	loadMenu();
	if (menuModel!=NULL)
	{
		readBookmarks();
		readHistory();
		jumpToHelpSection(jumpToSection, jumpToFile );
		languageChange();
	}
	else
	{
		displayNoHelp();
	}
}

HelpBrowser::~HelpBrowser()
{
	firstRun=true;
}

void HelpBrowser::closeEvent(QCloseEvent * event)
{
	delete menuModel;

	// no need to delete child widgets, Qt does it all for us
	// bookmarks
	QFile bookFile(bookmarkFile());
	if (bookFile.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&bookFile);
		stream.setCodec("UTF-8");
		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		stream << "<bookmarks>\n";
		QTreeWidgetItemIterator it(helpNav->bookmarksView);
		while (*it) 
		{
			if (bookmarkIndex.contains((*it)->text(0)))
			{
				QString pagetitle(bookmarkIndex.value((*it)->text(0)).first);
				QString filename(bookmarkIndex.value((*it)->text(0)).second);
				stream << "\t<item title=\"" << (*it)->text(0) << "\" pagetitle=\"" << pagetitle << "\" url=\"" << filename << "\" />\n";
			}
			++it;
		}
		stream << "</bookmarks>\n";
		bookFile.close();
	}
	// history
  	QFile histFile(historyFile());
	if (histFile.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&histFile);
		stream.setCodec("UTF-8");
		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		stream << "<history>\n";
		for (QMap<QAction*,histd2>::Iterator it = mHistory.begin() ; it != mHistory.end(); ++it)
			stream << "\t<item title=\"" << it.value().title << "\" url=\"" << it.value().url << "\" />\n";
		stream << "</history>\n";
		histFile.close();
	}
	// size
	prefs->set("xsize", width());
	prefs->set("ysize", height());

	emit closed();
}

void HelpBrowser::setupLocalUI()
{
	setWindowIcon(IconManager::instance()->loadIcon("AppIcon.png"));
	//Add Menus
	fileMenu=menuBar()->addMenu("");
	editMenu=menuBar()->addMenu("");
	bookMenu=menuBar()->addMenu("");
	histMenu=new QMenu(this);

	//Add Menu items
	filePrint=fileMenu->addAction(IconManager::instance()->loadIcon("16/document-print.png"), "", this, SLOT(print()), Qt::CTRL+Qt::Key_P);
	fileMenu->addSeparator();
	fileExit=fileMenu->addAction(IconManager::instance()->loadIcon("exit.png"), "", this, SLOT(close()), Qt::CTRL+Qt::Key_W);
	editFind=editMenu->addAction(IconManager::instance()->loadIcon("find.png"), "", this, SLOT(find()), Qt::CTRL+Qt::Key_F);
	editFindNext=editMenu->addAction( "", this, SLOT(findNext()), Qt::Key_F3);
	editFindPrev=editMenu->addAction( "", this, SLOT(findPrevious()), Qt::SHIFT+Qt::Key_F3);
	bookAdd=bookMenu->addAction( "", this, SLOT(bookmarkButton_clicked()), Qt::CTRL+Qt::Key_D);
	bookDel=bookMenu->addAction( "", this, SLOT(deleteBookmarkButton_clicked()));
	bookDelAll=bookMenu->addAction( "", this, SLOT(deleteAllBookmarkButton_clicked()));

	//Add Toolbar items
	goHome=toolBar->addAction(IconManager::instance()->loadIcon("16/go-home.png"), "", textBrowser, SLOT(home()));
	goBack=toolBar->addAction(IconManager::instance()->loadIcon("16/go-previous.png"), "", textBrowser, SLOT(backward()));
	goFwd=toolBar->addAction(IconManager::instance()->loadIcon("16/go-next.png"), "", textBrowser, SLOT(forward()));
	goBack->setMenu(histMenu);
	
	helpNav->listView->header()->hide();
	helpNav->searchingView->header()->hide();
	helpNav->bookmarksView->header()->hide();

//	splitter->setStretchFactor(splitter->indexOf(tabWidget), 0);
//	splitter->setStretchFactor(splitter->indexOf(textBrowser), 1);
	// reset previous size
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("helpbrowser");
	int xsize = prefs->getUInt("xsize", 640);
	int ysize = prefs->getUInt("ysize", 480);
	resize(QSize(xsize, ysize).expandedTo(minimumSizeHint()) );

	//basic ui
	connect(histMenu, SIGNAL(triggered(QAction*)), this, SLOT(histChosen(QAction*)));
	// searching
	connect(helpNav->searchingEdit, SIGNAL(returnPressed()), this, SLOT(searchingButton_clicked()));
	connect(helpNav->searchingButton, SIGNAL(clicked()), this, SLOT(searchingButton_clicked()));
	connect(helpNav->searchingView, SIGNAL(itemClicked( QTreeWidgetItem *, int)), this, SLOT(itemSearchSelected(QTreeWidgetItem *, int)));
	// bookmarks
	connect(helpNav->bookmarkButton, SIGNAL(clicked()), this, SLOT(bookmarkButton_clicked()));
	connect(helpNav->deleteBookmarkButton, SIGNAL(clicked()), this, SLOT(deleteBookmarkButton_clicked()));
	connect(helpNav->deleteAllBookmarkButton, SIGNAL(clicked()), this, SLOT(deleteAllBookmarkButton_clicked()));
	connect(helpNav->bookmarksView, SIGNAL(itemClicked( QTreeWidgetItem *, int)), this, SLOT(itemBookmarkSelected(QTreeWidgetItem *, int)));
	// links hoover
//	connect(textBrowser, SIGNAL(overLink(const QString &)), this, SLOT(showLinkContents(const QString &)));

	// status bar
//	connect(textBrowser,SIGNAL(statusBarMessage(QString)), this->statusBar(), SLOT(showMessage(QString)));
	
	languageChange();
}

//void HelpBrowser::showLinkContents(const QString &link)
//{
//	statusBar()->showMessage(link);
//}

void HelpBrowser::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void HelpBrowser::setHtml(const QString& str)
{
	textBrowser->setHtml(str);
}

void HelpBrowser::languageChange()
{
	setWindowTitle( tr( "Scribus Online Help" ) );
	
	fileMenu->setTitle(tr("&File"));
	editMenu->setTitle(tr("&Edit"));
	bookMenu->setTitle(tr("&Bookmarks"));
	
	filePrint->setText(tr("&Print..."));
	fileExit->setText(tr("&Quit"));
	editFind->setText(tr("&Find..."));
	editFindNext->setText(tr("Find &Next"));
	editFindPrev->setText(tr("Find &Previous"));
	bookAdd->setText(tr("&Add Bookmark"));
	bookDel->setText(tr("&Delete"));
	bookDelAll->setText(tr("D&elete All"));
	Ui::HelpBrowser::retranslateUi(this);
	if (!firstRun)
	{
		QString fname(QDir::cleanPath(textBrowser->source().toLocalFile()));
		QFileInfo fi(fname);
		QString filename(fi.fileName());
		if (ScCore->getGuiLanguage().isEmpty())
			language="en";
		else
			language=ScCore->getGuiLanguage();
		loadMenu();
		if (menuModel!=NULL)
			loadHelp(finalBaseDir + "/" + filename);
	}
	else
		firstRun=false;
}

void HelpBrowser::print()
{
	QPrinter printer;
	printer.setFullPage(true);
	QPrintDialog dialog(&printer, this);
	if (dialog.exec())
 		textBrowser->print(&printer);
}

void HelpBrowser::searchingButton_clicked()
{
	// root files
	helpNav->searchingView->clear();
	QApplication::changeOverrideCursor(QCursor(Qt::WaitCursor));
	searchingInDirectory(finalBaseDir);
	QApplication::changeOverrideCursor(Qt::ArrowCursor);
}

void HelpBrowser::searchingInDirectory(const QString& aDir)
{
	QDir dir(QDir::toNativeSeparators(aDir + "/"));
	QStringList in;
	in.append("*.html");
	QStringList lst = dir.entryList(in);
	for (QStringList::Iterator it = lst.begin(); it != lst.end(); ++it)
	{
		QString fname(aDir + "/" + (*it));
		QFile f(fname);
		if (f.open(QIODevice::ReadOnly))
		{
			QTextStream stream(&f);
			QString str = stream.readAll();
			int cnt = str.count(helpNav->searchingEdit->text(), Qt::CaseInsensitive);
			if (cnt > 0)
			{
				QString fullname = fname;
				QString toFind(fname.remove(finalBaseDir + "/"));
				QMapIterator<QString, QString> i(quickHelpIndex);
				while (i.hasNext())
				{
					i.next();
					if (i.value()==toFind)
						helpNav->searchingView->addTopLevelItem(new QTreeWidgetItem(helpNav->searchingView, QStringList() << i.key()));
				}
			}
			f.close();
		}
	}
	// get dirs - ugly recursion
	in.clear();
	in.append("*");
	QStringList dst = dir.entryList(in, QDir::Dirs);
	for (QStringList::Iterator it = dst.begin(); it != dst.end(); ++it)
		if ((*it)!="." && (*it)!="..")
			searchingInDirectory(QDir::toNativeSeparators(aDir + QString((*it)) + "/"));
}

void HelpBrowser::find()
{
	findText = QInputDialog::getText( this, tr("Find"), tr("Search Term:"), QLineEdit::Normal, findText, 0);
	if (findText.isNull())
		return;
	findNext();
}

void HelpBrowser::findNext()
{
	if (findText.isNull())
	{
		find();
		return;
	}
	// find it. finally
	textBrowser->find(findText, 0);
}

void HelpBrowser::findPrevious()
{
	if (findText.isNull())
	{
		find();
		return;
	}
	// find it. finally
	textBrowser->find(findText);
}

void HelpBrowser::bookmarkButton_clicked()
{
/*	QString title = textBrowser->title();
	QString fname(QDir::cleanPath(textBrowser->source().toLocalFile()));
	title = QInputDialog::getText(this, tr("New Bookmark"), tr("New Bookmark's Title:"), QLineEdit::Normal, title, 0);
	// user cancel
	if (title.isNull())
		return;
	//TODO: start storing full paths
 	QString toFind(fname.remove(QDir::toNativeSeparators(finalBaseDir)));
	toFind=toFind.mid(1, toFind.length()-1);
	QMapIterator<QString, QString> i(quickHelpIndex);
	while (i.hasNext())
	{
		i.next();
		if (i.value()==toFind)
		{
			bookmarkIndex.insert(title, qMakePair(i.key(), i.value()));
			helpNav->bookmarksView->addTopLevelItem(new QTreeWidgetItem(helpNav->bookmarksView, QStringList() << title));
		}
	}
*/
}

void HelpBrowser::deleteBookmarkButton_clicked()
{
	QTreeWidgetItem *twi=helpNav->bookmarksView->currentItem();
	if (twi!=NULL)
	{
		if (bookmarkIndex.contains(twi->text(0)))
			bookmarkIndex.remove(twi->text(0));
		delete twi;
	}
}

void HelpBrowser::deleteAllBookmarkButton_clicked()
{
	bookmarkIndex.clear();
	helpNav->bookmarksView->clear();
}

void HelpBrowser::histChosen(QAction* i)
{
	if (mHistory.contains(i))
		textBrowser->setSource( QUrl::fromLocalFile(mHistory[i].url) );

}

void HelpBrowser::jumpToHelpSection(const QString& jumpToSection, const QString& jumpToFile, bool dontChangeIfAlreadyLoaded)
{
	QString toLoad;
	bool noDocs=false;
	if (jumpToFile.isEmpty())
	{
		toLoad = finalBaseDir + "/"; //clean this later to handle 5 char locales
		if (jumpToSection.isEmpty())
		{
			QModelIndex index=menuModel->index(0,1);
			if (index.isValid())
			{
				helpNav->listView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
				toLoad += menuModel->data(index, Qt::DisplayRole).toString();
			}
			else
				noDocs=true;
		}
		else if (jumpToSection=="scripter")
		{
 			toLoad+="scripter1.html";
		}
	}
	else
	{
		toLoad=finalBaseDir + "/" + jumpToFile;
	}
	if (!noDocs)
		loadHelp(toLoad);
	else
		if (!dontChangeIfAlreadyLoaded)
			displayNoHelp();
}

void HelpBrowser::loadHelp(const QString& filename)
{
	struct histd2 his;
	bool Avail = false;
	QString toLoad;
	QFileInfo fi;
	fi = QFileInfo(filename);
	if (fi.fileName().length()>0)
	{
		if (fi.exists())
			toLoad = filename;
		else
		{
			toLoad = finalBaseDir + "/index.html";
			fi = QFileInfo(toLoad);
		}
		Avail = fi.exists();
	}
	if (Avail)
	{
		textBrowser->setSource(QUrl::fromLocalFile(toLoad));
		
//		his.title = textBrowser->title();
		if (his.title.isEmpty())
			his.title = toLoad;
		his.url = toLoad;
		mHistory[histMenu->addAction(his.title)] = his;
	}
	else
	{
		displayNoHelp();
	}
	if (mHistory.count() > 15)
	{
		QAction* first=histMenu->actions().first();
		mHistory.remove(first);
		histMenu->removeAction(first);
	}
}

void HelpBrowser::loadMenu()
{
	QString baseHelpDir = ScPaths::instance().docDir();
	QString altHelpDir  = ScPaths::instance().getApplicationDataDir();
	QString baseHelpMenuFile = QDir::toNativeSeparators(baseHelpDir + language + "/menu.xml");
	QString altHelpMenuFile = QDir::toNativeSeparators(altHelpDir + "doc/" + language + "/menu.xml");
	QFileInfo baseFi = QFileInfo(baseHelpMenuFile);
	QFileInfo altFi = QFileInfo(altHelpMenuFile);
	QString toLoad = baseHelpMenuFile;
	if (!baseFi.exists())
	{
		if (altFi.exists())
		{
			toLoad=altHelpMenuFile;
			baseFi=altFi;
		}
		else		
		{
			if (!language.isEmpty())
			{
				//Check if we can load, eg "de" when "de_CH" docs don't exist
				QString baseHelpMenuFile3 = QDir::toNativeSeparators(baseHelpDir + language.left(2) + "/menu.xml");
				QString altHelpMenuFile3 = QDir::toNativeSeparators(altHelpDir + "doc/" + language.left(2) + "/menu.xml");
				QFileInfo fi3 = QFileInfo(baseHelpMenuFile3);
				QFileInfo altfi3 = QFileInfo(altHelpMenuFile3);
				if (fi3.exists())
				{
					language=language.left(2);
					toLoad = QDir::toNativeSeparators(baseHelpMenuFile3);
				}
				else
				if (altfi3.exists())
				{
					language=language.left(2);
					toLoad = QDir::toNativeSeparators(altHelpMenuFile3);
				}
				else
				{
					//Fall back to English
					sDebug("Scribus help in your selected language does not exist, trying English. Otherwise, please visit http://docs.scribus.net.");
					language="en";
					toLoad = QDir::toNativeSeparators(baseHelpDir + language + "/menu.xml");
				}
			}
			else
			{
				language="en";
				toLoad = QDir::toNativeSeparators(baseHelpDir + language + "/menu.xml");
			}
			baseFi = QFileInfo(toLoad);
		}
	}
	//Set our final location for loading the help files
	finalBaseDir=baseFi.path();
	textBrowser->setSearchPaths(QStringList(finalBaseDir));
	if (baseFi.exists())
	{
		if (menuModel!=NULL)
			delete menuModel;
		menuModel=new ScHelpTreeModel(toLoad, "Topic", "Location", &quickHelpIndex);
	
		helpNav->listView->setModel(menuModel);
		helpNav->listView->setSelectionMode(QAbstractItemView::SingleSelection);
		QItemSelectionModel *selectionModel = new QItemSelectionModel(menuModel);
		helpNav->listView->setSelectionModel(selectionModel);
		connect(helpNav->listView->selectionModel(), SIGNAL(selectionChanged( const QItemSelection &, const QItemSelection &)), this, SLOT(itemSelected( const QItemSelection &, const QItemSelection &)));
	
		helpNav->listView->setColumnHidden(1,true);
	}
	else
		menuModel=NULL;
}

void HelpBrowser::readBookmarks()
{
	BookmarkParser2 handler;
	handler.view = helpNav->bookmarksView;
	handler.quickHelpIndex=&quickHelpIndex;
	handler.bookmarkIndex=&bookmarkIndex;
	QFile xmlFile(bookmarkFile());
	QXmlInputSource source(&xmlFile);
	QXmlSimpleReader reader;
	reader.setContentHandler(&handler);
	reader.parse(source);
}

void HelpBrowser::readHistory()
{
 	HistoryParser2 handler;
 	handler.helpBrowser = this;
 	QFile xmlFile(historyFile());
 	QXmlInputSource source(&xmlFile);
 	QXmlSimpleReader reader;
 	reader.setContentHandler(&handler);
 	reader.parse(source);
}

void HelpBrowser::itemSelected(const QItemSelection & selected, const QItemSelection & deselected)
{
	Q_UNUSED(deselected);

	QModelIndex index;
	QModelIndexList items = selected.indexes();
	int i=0;
	foreach (index, items)
	{
		if (i==1) // skip 0, as this is always the rootitem, even if we are selecting the rootitem. hmm
		{
			QString filename(menuModel->data(index, Qt::DisplayRole).toString());
			if (!filename.isEmpty())
			{
				loadHelp(finalBaseDir + "/" + filename);
			}
		}
		++i;
	}
}

void HelpBrowser::itemSearchSelected(QTreeWidgetItem *twi, int i)
{
	Q_UNUSED(i);
	if (!twi)
		return;
	if (quickHelpIndex.contains(twi->text(0)))
	{
		QString filename(quickHelpIndex.value(twi->text(0)));
		if (!filename.isEmpty())
		{
			loadHelp(finalBaseDir + "/" + filename);
			findText = helpNav->searchingEdit->text();
			findNext();
		}
	}
}

void HelpBrowser::itemBookmarkSelected(QTreeWidgetItem *twi, int i)
{
	Q_UNUSED(i);
	if (!twi)
		return;
	if (bookmarkIndex.contains(twi->text(0)))
	{
		QString filename(bookmarkIndex.value(twi->text(0)).second);
		if (!filename.isEmpty())
			loadHelp(finalBaseDir + "/" + filename);
	}
}

/*! \brief Returns the name of the cfg file for bookmarks.
A helper function.
\author Petr Vanek <petr@yarpen.cz>
*/
QString HelpBrowser::bookmarkFile()
{
	QString appDataDir(ScPaths::getApplicationDataDir());
	QString fname(appDataDir + "doc/bookmarks.xml");
	if (!QFile::exists(fname))
	{
		QDir d(QDir::toNativeSeparators(appDataDir));
		d.mkdir("doc");
	}
	return fname;
}


/*! \brief Returns the name of the cfg file for persistent history.
A helper function.
\author Petr Vanek <petr@yarpen.cz>
*/
QString HelpBrowser::historyFile()
{
	QString appDataDir(ScPaths::getApplicationDataDir());
	QString fname(appDataDir + "doc/history.xml");
	if (!QFile::exists(fname))
	{
		QDir d(QDir::toNativeSeparators(appDataDir));
		d.mkdir("doc");
	}
	return fname;
}

void HelpBrowser::displayNoHelp()
{
	QString noHelpMsg=tr("<h2><p>Sorry, no manual is installed!</p><p>Please see:</p><ul><li>http://docs.scribus.net for updated documentation</li><li>http://www.scribus.net for downloads</li></ul></h2>",
						 "HTML message for no documentation available to show");

	textBrowser->setHtml(noHelpMsg);
	
	filePrint->setEnabled(false);
	editFind->setEnabled(false);
	editFindNext->setEnabled(false);
	editFindPrev->setEnabled(false);
	bookAdd->setEnabled(false);
	bookDel->setEnabled(false);
	bookDelAll->setEnabled(false);
	goHome->setEnabled(false);
	goBack->setEnabled(false);
	goFwd->setEnabled(false);
	
	histMenu->disconnect();
	helpNav->searchingEdit->disconnect();
	helpNav->searchingButton->disconnect();
	helpNav->searchingView->disconnect();
	helpNav->bookmarkButton->disconnect();
	helpNav->deleteBookmarkButton->disconnect();
	helpNav->deleteAllBookmarkButton->disconnect();
	helpNav->bookmarksView->disconnect();
	textBrowser->disconnect();
}
