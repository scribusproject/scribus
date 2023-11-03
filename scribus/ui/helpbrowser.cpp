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
#include <QVector>
#include <QXmlStreamReader>

#include "prefsmanager.h"
#include "scribuscore.h"
#include "util_debug.h"
#include "iconmanager.h"
#include "util.h"

/*! \brief XML parsed for documentation history.
This is small helper class which reads saved bookmarks configuration
from ~/.scribus/doc/history.xml file.
The reference to historyBrowser is a reference to the dialog.
\author Petr Vanek <petr@yarpen.cz>
*/
class HistoryParser2
{
	public:
		HelpBrowser *helpBrowser { nullptr };

		bool parse(const QString& fileName)
		{
			QFile qFile(fileName);
			if (!qFile.open(QFile::ReadOnly))
				return false;

			QXmlStreamReader sReader(&qFile);
			QXmlStreamReader::TokenType tagType;
			QStringRef tagName;

			while (!sReader.atEnd() && !sReader.hasError())
			{
				tagType = sReader.readNext();
				if (tagType != QXmlStreamReader::StartElement)
					continue;

				tagName = sReader.name();
				if (tagName != "item")
					continue;

				struct histd2 his;
				QXmlStreamAttributes attrs = sReader.attributes();
				his.title = attrs.value(QLatin1String("title")).toString();
				his.url = attrs.value(QLatin1String("url")).toString();
				helpBrowser->addHistoryItem(his);
			}
			qFile.close();

			return (!sReader.hasError());
		}
};

/*! \brief XML parsed for documentation bookmarks.
This is small helper class which reads saved bookmarks configuration
from ~/.scribus/doc/bookmarks.xml file.
The reference to QListView *view is a reference to the list view with bookmarks
\author Petr Vanek <petr@yarpen.cz>
*/
class BookmarkParser2
{
	public:
		QTreeWidget* view { nullptr };
		QMap<QString, QString>* quickHelpIndex { nullptr };
		QMap<QString, QPair<QString, QString> >* bookmarkIndex { nullptr };

		bool parse(const QString& fileName)
		{
			QFile qFile(fileName);
			if (!qFile.open(QFile::ReadOnly))
				return false;

			QXmlStreamReader sReader(&qFile);
			QXmlStreamReader::TokenType tagType;
			QStringRef tagName;

			while (!sReader.atEnd() && !sReader.hasError())
			{
				tagType = sReader.readNext();
				if (tagType != QXmlStreamReader::StartElement)
					continue;

				tagName = sReader.name();
				if (tagName != "item")
					continue;

				QXmlStreamAttributes attrs = sReader.attributes();
				QString title = attrs.value(QLatin1String("title")).toString();
				QString pageTitle = attrs.value(QLatin1String("pagetitle")).toString();
				QString pageUrl = attrs.value(QLatin1String("url")).toString();

				//TODO : This will dump items if bookmarks get loaded into a different GUI language
				if (quickHelpIndex->contains(pageTitle))
				{
					bookmarkIndex->insert(title, qMakePair(pageTitle, pageUrl));
					view->addTopLevelItem(new QTreeWidgetItem(view, QStringList() << title));
				}
			}
			qFile.close();

			return (!sReader.hasError());
		}
};

bool HelpBrowser::firstRun = true;

HelpBrowser::HelpBrowser( QWidget* parent, const QString& /*caption*/, const QString& guiLanguage, const QString& jumpToSection, const QString& jumpToFile)
	: QMainWindow( parent )
{
	firstRun = true;
	setupUi(this);
	helpNav = new HelpNavigation(this);
#if defined(Q_OS_MACOS) && defined(HELP_NAV_AS_DRAWER)
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
	m_language = guiLanguage.isEmpty() ? QString("en") : guiLanguage.left(2);
	m_defaultBaseDir = ScPaths::instance().docDir() + "en/"; //Sane default for help location
	m_finalBaseDir = ScPaths::instance().docDir() + "en/"; //Sane default for help location
	textBrowser->setSearchPaths(QStringList(m_finalBaseDir));

	loadMenu();
	if (m_menuModel != nullptr)
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
	firstRun = true;
}

void HelpBrowser::closeEvent(QCloseEvent * event)
{
	delete m_menuModel;

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
			if (m_bookmarkIndex.contains((*it)->text(0)))
			{
				QString pageTitle(m_bookmarkIndex.value((*it)->text(0)).first);
				QString fileName(m_bookmarkIndex.value((*it)->text(0)).second);
				stream << "\t<item title=\"" << (*it)->text(0) << "\" pagetitle=\"" << pageTitle << "\" url=\"" << fileName << "\" />\n";
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
		for (auto it = m_history.cbegin() ; it != m_history.cend(); ++it)
			stream << "\t<item title=\"" << it.value().title << "\" url=\"" << it.value().url << "\" />\n";
		stream << "</history>\n";
		histFile.close();
	}
	// size
	m_prefs->set("xsize", width());
	m_prefs->set("ysize", height());

	emit closed();
}

void HelpBrowser::setupLocalUI()
{
	IconManager& iconManager = IconManager::instance();

	setWindowIcon(iconManager.loadIcon("AppIcon.png"));
	//Add Menus
	fileMenu = menuBar()->addMenu("");
	editMenu = menuBar()->addMenu("");
	bookMenu = menuBar()->addMenu("");
	histMenu = new QMenu(this);

	//Add Menu items
	filePrint = fileMenu->addAction(iconManager.loadIcon("16/document-print.png"), "", this, SLOT(print()), Qt::CTRL+Qt::Key_P);
	fileMenu->addSeparator();
	fileExit = fileMenu->addAction(iconManager.loadIcon("exit.png"), "", this, SLOT(close()), Qt::CTRL+Qt::Key_W);
	editFind = editMenu->addAction(iconManager.loadIcon("find.png"), "", this, SLOT(find()), Qt::CTRL+Qt::Key_F);
	editFindNext = editMenu->addAction( "", this, SLOT(findNext()), Qt::Key_F3);
	editFindPrev = editMenu->addAction( "", this, SLOT(findPrevious()), Qt::SHIFT+Qt::Key_F3);
	bookAdd = bookMenu->addAction( "", this, SLOT(bookmarkButton_clicked()), Qt::CTRL+Qt::Key_D);
	bookDel = bookMenu->addAction( "", this, SLOT(deleteBookmarkButton_clicked()));
	bookDelAll = bookMenu->addAction( "", this, SLOT(deleteAllBookmarkButton_clicked()));

	//Add Toolbar items
	goHome = toolBar->addAction(iconManager.loadIcon("16/go-home.png"), "", textBrowser, SLOT(home()));
	goBack = toolBar->addAction(iconManager.loadIcon("16/go-previous.png"), "", textBrowser, SLOT(backward()));
	goFwd = toolBar->addAction(iconManager.loadIcon("16/go-next.png"), "", textBrowser, SLOT(forward()));
	goBack->setMenu(histMenu);
	
	helpNav->listView->header()->hide();
	helpNav->searchingView->header()->hide();
	helpNav->bookmarksView->header()->hide();

//	splitter->setStretchFactor(splitter->indexOf(tabWidget), 0);
//	splitter->setStretchFactor(splitter->indexOf(textBrowser), 1);
	// reset previous size
	m_prefs = PrefsManager::instance().prefsFile->getPluginContext("helpbrowser");
	int xsize = m_prefs->getUInt("xsize", 640);
	int ysize = m_prefs->getUInt("ysize", 480);
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

void HelpBrowser::addHistoryItem(const histd2& histItem)
{
	m_history[histMenu->addAction(histItem.title)] = histItem;
}

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

	if (firstRun)
	{
		firstRun = false;
		return;
	}

	QString fname(QDir::cleanPath(textBrowser->source().toLocalFile()));
	QFileInfo fi(fname);
	QString fileName(fi.fileName());
	if (ScCore->getGuiLanguage().isEmpty())
		m_language = "en";
	else
		m_language = ScCore->getGuiLanguage();
	loadMenu();
	if (m_menuModel != nullptr)
		loadHelp(fileName);
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
	searchingInDirectory(m_finalBaseDir);
	QApplication::changeOverrideCursor(Qt::ArrowCursor);
}

void HelpBrowser::searchingInDirectory(const QString& aDir)
{
	QDir dir(QDir::toNativeSeparators(aDir + "/"));
	QStringList in;
	in.append("*.html");
	QStringList dirEntries = dir.entryList(in);
	for (auto it = dirEntries.cbegin(); it != dirEntries.cend(); ++it)
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
				QString toFind(fname.remove(m_finalBaseDir + "/"));
				QMapIterator<QString, QString> i(m_quickHelpIndex);
				while (i.hasNext())
				{
					i.next();
					if (i.value() == toFind)
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
	for (auto it = dst.cbegin(); it != dst.cend(); ++it)
	{
		if ((*it) == "." || (*it) == "..")
			continue;
		searchingInDirectory(QDir::toNativeSeparators(aDir + QString((*it)) + "/"));
	}
}

void HelpBrowser::find()
{
	m_findText = QInputDialog::getText( this, tr("Find"), tr("Search Term:"), QLineEdit::Normal, m_findText, nullptr);
	if (m_findText.isNull())
		return;
	findNext();
}

void HelpBrowser::findNext()
{
	if (m_findText.isNull())
	{
		find();
		return;
	}
	// find it. finally
	textBrowser->find(m_findText, 0);
}

void HelpBrowser::findPrevious()
{
	if (m_findText.isNull())
	{
		find();
		return;
	}
	// find it. finally
	textBrowser->find(m_findText);
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
 	QString toFind(fname.remove(QDir::toNativeSeparators(m_finalBaseDir)));
	toFind = toFind.mid(1, toFind.length() - 1);
	QMapIterator<QString, QString> i(m_quickHelpIndex);
	while (i.hasNext())
	{
		i.next();
		if (i.value() == toFind)
		{
			m_bookmarkIndex.insert(title, qMakePair(i.key(), i.value()));
			helpNav->bookmarksView->addTopLevelItem(new QTreeWidgetItem(helpNav->bookmarksView, QStringList() << title));
		}
	}
*/
}

void HelpBrowser::deleteBookmarkButton_clicked()
{
	QTreeWidgetItem *twi = helpNav->bookmarksView->currentItem();
	if (twi != nullptr)
	{
		if (m_bookmarkIndex.contains(twi->text(0)))
			m_bookmarkIndex.remove(twi->text(0));
		delete twi;
	}
}

void HelpBrowser::deleteAllBookmarkButton_clicked()
{
	m_bookmarkIndex.clear();
	helpNav->bookmarksView->clear();
}

void HelpBrowser::histChosen(QAction* i)
{
	if (m_history.contains(i))
		textBrowser->setSource( QUrl::fromLocalFile(m_history[i].url) );
}

void HelpBrowser::jumpToHelpSection(const QString& jumpToSection, const QString& jumpToFile, bool dontChangeIfAlreadyLoaded)
{
	QString toLoad;
	if (jumpToFile.isEmpty())
	{
		if (jumpToSection.isEmpty())
		{
			QModelIndex index = m_menuModel->index(0, 1);
			if (index.isValid())
			{
				helpNav->listView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
				toLoad = m_menuModel->data(index, Qt::DisplayRole).toString();
			}
		}
		else if (jumpToSection == "scripter")
		{
 			toLoad += "scripter1.html";
		}
	}
	else
	{
		toLoad = jumpToFile;
	}
	if (!toLoad.isEmpty())
		loadHelp(toLoad);
	else if (!dontChangeIfAlreadyLoaded)
		displayNoHelp();
}

void HelpBrowser::loadHelp(const QString& fileName)
{
	struct histd2 his;
	bool isAvail = false;
	QString toLoad;

	QFileInfo fi(fileName);
	if (fi.fileName().length() > 0)
	{
		if (fi.isAbsolute() && fi.exists())
			toLoad = fileName;
		if (toLoad.isEmpty())
		{
			QStringList searchPaths = textBrowser->searchPaths();
			for (int i = 0; i < searchPaths.count(); ++i)
			{
				const QString& searchPath = searchPaths.at(i);
				fi = QFileInfo(searchPath + "/" + fi.fileName());
				if (fi.exists())
				{
					toLoad = fi.fileName();
					break;
				}
			}
		}
		if (toLoad.isEmpty())
		{
			QStringList searchPaths = textBrowser->searchPaths();
			for (int i = 0; i < searchPaths.count(); ++i)
			{
				const QString& searchPath = searchPaths.at(i);
				fi = QFileInfo(searchPath + "/index.html");
				if (fi.exists())
				{
					toLoad = "index.html";
					break;
				}
			}
		}
		isAvail = fi.exists();
	}
	if (isAvail)
	{
		textBrowser->setSource(QUrl::fromLocalFile(toLoad));
		
//		his.title = textBrowser->title();
		if (his.title.isEmpty())
			his.title = toLoad;
		his.url = toLoad;
		m_history[histMenu->addAction(his.title)] = his;
	}
	else
	{
		displayNoHelp();
	}
	if (m_history.count() > 15)
	{
		QAction* first = histMenu->actions().first();
		m_history.remove(first);
		histMenu->removeAction(first);
	}
}

void HelpBrowser::loadMenu()
{
	QString baseHelpDir = ScPaths::instance().userHelpFilesDir(false);
	QString installHelpDir  = ScPaths::instance().docDir();

	QVector<QPair<QString, QString> > helpMenuFiles;
	helpMenuFiles.reserve(6);
	helpMenuFiles.append(qMakePair(baseHelpDir, m_language));
	helpMenuFiles.append(qMakePair(installHelpDir, m_language));
	if (!m_language.isEmpty())
	{
		QStringList langDecomp = m_language.split(QChar('_'), Qt::SkipEmptyParts);
		QString altLanguage = langDecomp.first();
		helpMenuFiles.append(qMakePair(baseHelpDir, altLanguage));
		helpMenuFiles.append(qMakePair(installHelpDir, altLanguage));
	}
	QString enLanguage("en");
	helpMenuFiles.append(qMakePair(baseHelpDir, enLanguage));
	helpMenuFiles.append(qMakePair(installHelpDir, enLanguage));

	QString toLoad;
	for (int i = 0; i < helpMenuFiles.count(); ++i)
	{
		auto helpPair = helpMenuFiles.at(i);
		QString helpFile = QDir::toNativeSeparators(helpPair.first + helpPair.second + "/menu.xml");
		QFileInfo helpInfo(helpFile);
		if (helpInfo.exists())
		{
			toLoad = helpFile;
			m_language = helpPair.second;
			break;
		}
	}

	if (toLoad.isEmpty())
	{
		m_language = "en";
		toLoad = QDir::toNativeSeparators(installHelpDir + m_language + "/menu.xml");
	}

	//Set our final location for loading the help files
	QFileInfo baseFi = QFileInfo(toLoad);
	m_finalBaseDir = baseFi.path();
	QStringList searchPaths;
	searchPaths << m_finalBaseDir;
	searchPaths << m_defaultBaseDir;
	textBrowser->setSearchPaths(searchPaths);

	delete m_menuModel;
	m_menuModel = nullptr;

	if (baseFi.exists())
	{
		m_menuModel = new ScHelpTreeModel(toLoad, "Topic", "Location", &m_quickHelpIndex);
	
		helpNav->listView->setModel(m_menuModel);
		helpNav->listView->setSelectionMode(QAbstractItemView::SingleSelection);
		QItemSelectionModel *selectionModel = new QItemSelectionModel(m_menuModel);
		helpNav->listView->setSelectionModel(selectionModel);
		connect(helpNav->listView->selectionModel(), SIGNAL(selectionChanged( const QItemSelection &, const QItemSelection &)), this, SLOT(itemSelected( const QItemSelection &, const QItemSelection &)));
	
		helpNav->listView->setColumnHidden(1,true);
	}
}

void HelpBrowser::readBookmarks()
{
	BookmarkParser2 bookmarkParser;
	bookmarkParser.view = helpNav->bookmarksView;
	bookmarkParser.quickHelpIndex = &m_quickHelpIndex;
	bookmarkParser.bookmarkIndex = &m_bookmarkIndex;
	bookmarkParser.parse(bookmarkFile());
}

void HelpBrowser::readHistory()
{
 	HistoryParser2 historyParser;
 	historyParser.helpBrowser = this;
	historyParser.parse(historyFile());
}

void HelpBrowser::itemSelected(const QItemSelection & selected, const QItemSelection & deselected)
{
	Q_UNUSED(deselected);

	int i = 0;
	const QModelIndexList items = selected.indexes();
	for (const QModelIndex& index : items)
	{
		if (i == 1) // skip 0, as this is always the rootitem, even if we are selecting the rootitem. hmm
		{
			QString fileName(m_menuModel->data(index, Qt::DisplayRole).toString());
			if (!fileName.isEmpty())
			{
				loadHelp(fileName);
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
	if (m_quickHelpIndex.contains(twi->text(0)))
	{
		QString fileName(m_quickHelpIndex.value(twi->text(0)));
		if (!fileName.isEmpty())
		{
			loadHelp(fileName);
			m_findText = helpNav->searchingEdit->text();
			findNext();
		}
	}
}

void HelpBrowser::itemBookmarkSelected(QTreeWidgetItem *twi, int i)
{
	Q_UNUSED(i);
	if (!twi)
		return;
	if (m_bookmarkIndex.contains(twi->text(0)))
	{
		QString fileName(m_bookmarkIndex.value(twi->text(0)).second);
		if (!fileName.isEmpty())
			loadHelp(fileName);
	}
}

/*! \brief Returns the name of the cfg file for bookmarks.
A helper function.
\author Petr Vanek <petr@yarpen.cz>
*/
QString HelpBrowser::bookmarkFile()
{
	QString appDataDir(ScPaths::applicationDataDir());
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
	QString appDataDir(ScPaths::applicationDataDir());
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
	QString noHelpMsg = tr("<h2><p>Sorry, no manual is installed!</p><p>Please see:</p><ul><li>http://docs.scribus.net for updated documentation</li><li>http://www.scribus.net for downloads</li></ul></h2>",
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
