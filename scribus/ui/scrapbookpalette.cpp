/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scrapbookpalette.h"

#include <QEvent>
#include <QDataStream>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QKeyEvent>
#include <QByteArray>
#include <QPixmap>
#include <QFileDialog>
#include <QUrl>
#include <QDropEvent>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QFile>
#include <QFileInfo>
#include <QDomDocument>
#include <QToolButton>
#include <QCursor>
#include <QToolBox>
#include <QMessageBox>
#include <QToolTip>
#include <QPainter>
#include <QProgressDialog>

#include "commonstrings.h"
#include "cmsettings.h"
#include "filewatcher.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "query.h"
#include "scpreview.h"
#include "scimage.h"
#include "scribuscore.h"
#include "fileloader.h"
#include "loadsaveplugin.h"
#include "plugins/formatidlist.h"
#include "util.h"
#include "util_color.h"
#include "util_file.h"
#include "util_formats.h"
#include "util_icon.h"
//CB TODO bring in the prefsManager instance locally here too

/* The Scrapbook View Class
 * inherited from QListWidget */
BibView::BibView(QWidget* parent) : QListWidget(parent)
{
	setDragEnabled(true);
	setViewMode(QListView::IconMode);
	setFlow(QListView::LeftToRight);
//	setSortingEnabled(true);
	setWrapping(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setResizeMode(QListView::Adjust);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setIconSize(QSize(60, 60));
	setGridSize(QSize(70, 80));
	setSpacing(10);
	setTextElideMode(Qt::ElideMiddle);
	objectMap.clear();
	ScFilename = "";
	visibleName = "";
	canWrite = true;
}

 void BibView::startDrag(Qt::DropActions supportedActions)
 {
	QStringList vectorFiles = LoadSavePlugin::getExtensionsForPreview(FORMATID_ODGIMPORT);
	QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::RASTORIMAGES, 1));
	QStringList rasterFiles = formatD.split("|");
	QString dt = objectMap[currentItem()->text()].Data;
	QFileInfo fi(dt);
	QMimeData *mimeData = new QMimeData;
	QList<QUrl> urlList;
	if (fi.suffix().toLower() == "sce")
	{
		if ( fi.exists() )
		{
			QUrl ur = QUrl::fromLocalFile(dt);
			urlList.append(ur);
			mimeData->setUrls(urlList);
		}
	}
	else if ((vectorFiles.contains(fi.suffix().toLower())) || (rasterFiles.contains(fi.suffix().toLower())))
	{
		if ( fi.exists() )
		{
			QUrl ur = QUrl::fromLocalFile(dt);
			urlList.append(ur);
			mimeData->setUrls(urlList);
		}
	}
	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(objectMap[currentItem()->text()].Preview);
	drag->exec(Qt::CopyAction);
	clearSelection();
 }
 
void BibView::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void BibView::dragMoveEvent(QDragMoveEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void BibView::dropEvent(QDropEvent *e)
{
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
		if (e->source() == this)
			return;
		QString nam, tmp = "";
		QString text = e->mimeData()->text();
		if (text.startsWith("<SCRIBUSELEM"))
			emit objDropped(text);
	}
	else if ( e->mimeData()->hasFormat("text/uri-list"))
	{
		QStringList vectorFiles = LoadSavePlugin::getExtensionsForPreview(FORMATID_ODGIMPORT);
		QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::RASTORIMAGES, 1));
		QStringList rasterFiles = formatD.split("|");
		QList<QUrl> fileUrls = e->mimeData()->urls();
		e->acceptProposedAction();
		for( int i = 0; i < fileUrls.count(); ++i )
		{
			QUrl url( fileUrls[i] );
			QFileInfo fi(url.path());
			if (vectorFiles.contains(fi.suffix().toLower()))
			{
				FileLoader *fileLoader = new FileLoader(url.path());
				int testResult = fileLoader->TestFile();
				delete fileLoader;
				if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT) && (fi.exists()))
					emit fileDropped(url.path(), testResult);
			}
			else if (rasterFiles.contains(fi.suffix().toLower()))
				emit fileDropped(url.path(), 9999);
		}
	}
	else
		e->ignore();
}

void BibView::AddObj(QString name, QString daten, QPixmap Bild, bool isDir, bool isRaster, bool isVector)
{
	struct Elem DrElem;
	DrElem.isDir = isDir;
	DrElem.isRaster = isRaster;
	DrElem.isVector = isVector;
	DrElem.Data = daten;
	DrElem.Preview = Bild;
	objectMap.insert(name, DrElem);
}

void BibView::checkAndChange(QString &text, QString nam, QString dir)
{
	QDomDocument docu("scridoc");
	docu.setContent(text);
	QDomElement elem = docu.documentElement();
	QDomNode DOC = elem.firstChild();
	bool hasImage = false;
	while(!DOC.isNull())
	{
		QDomElement pg = DOC.toElement();
		if(pg.tagName() == "ITEM")
		{
			PageItem::ItemType PType = static_cast<PageItem::ItemType>(pg.attribute("PTYPE").toInt());
			if ((PType == PageItem::ImageFrame) || (PType == PageItem::TextFrame))
			{
				QString Pfile = pg.attribute("PFILE");
				QString Pfile2 = pg.attribute("PFILE2","");
				QString Pfile3 = pg.attribute("PFILE3","");
				if (!Pfile.isEmpty())
					hasImage = true;
				if (!Pfile2.isEmpty())
					hasImage = true;
				if (!Pfile3.isEmpty())
					hasImage = true;
			}
		}
		DOC = DOC.nextSibling();
	}
	QFileInfo fid(nam);
	if (hasImage)
	{
		QDir dd = QDir(dir);
		dd.mkdir(QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName())));
	}
	QString source = "";
	QString target = "";
	bool first = true;
	DOC = elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg = DOC.toElement();
		if(pg.tagName() == "ITEM")
		{
			if (first)
				pg.setAttribute("ANNAME", fid.baseName());
			first = false;
			PageItem::ItemType PType = static_cast<PageItem::ItemType>(pg.attribute("PTYPE").toInt());
			if ((PType == PageItem::ImageFrame) || (PType == PageItem::TextFrame))
			{
				QString Pfile = pg.attribute("PFILE");
				if (!Pfile.isEmpty())
				{
					if (static_cast<bool>(pg.attribute("relativePaths", "0").toInt()))
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(dir+"/"+Pfile)));
						source = pfi2.absoluteFilePath();
					}
					else
					{
						QFileInfo fi(Pfile);
						source = QDir::cleanPath(QDir::toNativeSeparators(QDir::homePath()+"/"+Pfile));
					}
					QFileInfo fi(Pfile);
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile2 = pg.attribute("PFILE2","");
				if (!Pfile2.isEmpty())
				{
					if (static_cast<bool>(pg.attribute("relativePaths", "0").toInt()))
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(dir+"/"+Pfile2)));
						source = pfi2.absoluteFilePath();
					}
					else
					{
						QFileInfo fi(Pfile2);
						source = QDir::cleanPath(QDir::toNativeSeparators(QDir::homePath()+"/"+Pfile));
					}
					QFileInfo fi(Pfile2);
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE2", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile3 = pg.attribute("PFILE3","");
				if (!Pfile3.isEmpty())
				{
					if (static_cast<bool>(pg.attribute("relativePaths", "0").toInt()))
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(dir+"/"+Pfile3)));
						source = pfi2.absoluteFilePath();
					}
					else
					{
						QFileInfo fi(Pfile3);
						source = QDir::cleanPath(QDir::toNativeSeparators(QDir::homePath()+"/"+Pfile3));
					}
					QFileInfo fi(Pfile3);
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE3", fid.baseName() + "/" + fi.fileName());
				}
				pg.setAttribute("relativePaths", 1);
			}
		}
		DOC = DOC.nextSibling();
	}
	QFile f(nam);
	if(!f.open(QIODevice::WriteOnly))
		return ;
	QDataStream s;
	QByteArray cs = docu.toByteArray();
	s.setDevice(&f);
	s.writeRawData(cs.data(), cs.length());
	f.close();
}

void BibView::ReadOldContents(QString name, QString newName)
{
	bool isUtf8 = false;
	QDomDocument docu("scridoc");
	QString ff;
	QByteArray cf;
	if (!loadRawText(name, cf))
		return;
	if( cf.left(17) == "<SCRIBUSSCRAPUTF8")
	{
		ff = QString::fromUtf8(cf.data());
		isUtf8 = true;
	}
	else
		ff = QString::fromLocal8Bit(cf.data());
	if(!docu.setContent(ff))
		return;
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUSSCRAP") && (elem.tagName() != "SCRIBUSSCRAPUTF8"))
		return;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		if (dc.tagName()=="OBJEKT")
		{
			QFile fi(QDir::cleanPath(QDir::toNativeSeparators(newName + "/" + GetAttr(&dc, "NAME") + ".sce")));
			if(!fi.open(QIODevice::WriteOnly))
				continue ;
			QDataStream s(&fi);
			QString fn = GetAttr(&dc, "DATA");
			cf = isUtf8? fn.toUtf8() : fn.toLocal8Bit();
			s.writeRawData(cf.data(), cf.length());
			fi.close();
		}
		DOC=DOC.nextSibling();
	}
}

void BibView::ReadContents(QString name)
{
	clear();
	objectMap.clear();
	QString nd;
	if (name.endsWith(QDir::toNativeSeparators("/")))
		nd = name.left(name.length()-1);
	int fileCount = 0;
	int readCount = 0;
	QDir thumbs(name);
	if (thumbs.exists())
	{
		if ((canWrite) && (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews))
			thumbs.mkdir(".ScribusThumbs");
		thumbs.cd(".ScribusThumbs");
	}
	QDir dd(name, "*", QDir::Name, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
	fileCount += dd.count();
	QDir d(name, "*.sce", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	fileCount += d.count();
	QStringList vectorFiles = LoadSavePlugin::getExtensionsForPreview(FORMATID_ODGIMPORT);
	for (int v = 0; v < vectorFiles.count(); v++)
	{
		QString ext = "*." + vectorFiles[v];
		QDir d4(name, ext, QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		fileCount += d4.count();
	}
	QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::RASTORIMAGES, 1));
	QStringList rasterFiles = formatD.split("|");
	for (int v = 0; v < rasterFiles.count(); v++)
	{
		QString ext = "*." + rasterFiles[v];
		QDir d5(name, ext, QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		fileCount += d5.count();
	}
	QProgressDialog *pgDia = NULL;
	QStringList previewFiles;
	previewFiles.clear();
	if (ScCore->initialized())
	{
		pgDia = new QProgressDialog("Reading Files...", QString(), 0, fileCount, this);
		pgDia->setWindowModality(Qt::WindowModal);
	}
	if ((dd.exists()) && (dd.count() != 0))
	{
		for (uint dc = 0; dc < dd.count(); ++dc)
		{
			if (pgDia)
			{
				pgDia->setValue(readCount);
				readCount++;
			}
			QPixmap pm = loadIcon("folder.png");
			AddObj(dd[dc], "", pm, true);
		}
	}
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			if (pgDia)
			{
				pgDia->setValue(readCount);
				readCount++;
			}
			QPixmap pm;
			QByteArray cf;
			if (!loadRawText(QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d[dc])), cf))
				continue;
			QFileInfo fi(QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d[dc])));
			QFileInfo fi2(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/.ScribusThumbs/"+fi.baseName()+".png")));
			QFileInfo fi3(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/.ScribusThumbs/"+fi.baseName()+".png")));
			else
			{
				if (fi3.exists())
					pm.load(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/"+fi.baseName()+".png")));
				else
				{
					QString f;
					if (cf.left(16) == "<SCRIBUSELEMUTF8")
						f = QString::fromUtf8(cf.data());
					else
						f = cf.data();
					ScPreview *pre = new ScPreview();
					pm = QPixmap::fromImage(pre->createPreview(f));
					if ((canWrite) && (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews))
						pm.save(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/.ScribusThumbs/"+fi.baseName()+".png")), "PNG");
					delete pre;
				}
			}
			previewFiles.append(fi.baseName()+".png");
			AddObj(fi.baseName(), QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d[dc])), pm);
		}
	}
	for (int v = 0; v < vectorFiles.count(); v++)
	{
		QString ext = "*." + vectorFiles[v];
		QDir d4(name, ext, QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((d4.exists()) && (d4.count() != 0))
		{
			for (uint dc = 0; dc < d4.count(); ++dc)
			{
				if (pgDia)
				{
					pgDia->setValue(readCount);
					readCount++;
				}
				QPixmap pm;
				QFileInfo fi(QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d4[dc])));
				QFileInfo fi2(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/.ScribusThumbs/"+fi.fileName()+".png")));
				if (fi2.exists())
					pm.load(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/.ScribusThumbs/"+fi.fileName()+".png")));
				else
				{
					FileLoader *fileLoader = new FileLoader(QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d4[dc])));
					int testResult = fileLoader->TestFile();
					delete fileLoader;
					if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT))
					{
						const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
						if( fmt )
						{
							QImage im = fmt->readThumbnail(QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d4[dc])));
							im = im.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
							if ((canWrite) && (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews))
								im.save(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/.ScribusThumbs/"+fi.fileName()+".png")), "PNG");
							pm = QPixmap::fromImage(im);
						}
					}
				}
				AddObj(fi.fileName(), QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d4[dc])), pm, false, false, true);
			}
		}
	}
	for (int v = 0; v < rasterFiles.count(); v++)
	{
		QString ext = "*." + rasterFiles[v];
		QDir d5(name, ext, QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((d5.exists()) && (d5.count() != 0))
		{
			for (uint dc = 0; dc < d5.count(); ++dc)
			{
				if (pgDia)
				{
					pgDia->setValue(readCount);
					readCount++;
				}
				if (previewFiles.contains(d5[dc]))
					continue;
				QPixmap pm;
				QFileInfo fi(QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d5[dc])));
				QFileInfo fi2(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/.ScribusThumbs/"+fi.fileName()+".png")));
				if (fi2.exists())
					pm.load(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/.ScribusThumbs/"+fi.fileName()+".png")));
				else
				{
					bool mode = false;
					ScImage im;
					CMSettings cms(0, "", Intent_Perceptual);
					cms.allowColorManagement(false);
					if (im.loadPicture(QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d5[dc])), 1, cms, ScImage::Thumbnail, 72, &mode))
					{
						QImage img = im.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
						if ((canWrite) && (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews))
							img.save(QDir::cleanPath(QDir::toNativeSeparators(fi.path()+"/.ScribusThumbs/"+fi.fileName()+".png")), "PNG");
						pm = QPixmap::fromImage(img);
					}
				}
				AddObj(fi.fileName(), QDir::cleanPath(QDir::toNativeSeparators(name + "/" + d5[dc])), pm, false, true);
			}
		}
	}
	if (pgDia)
	{
		pgDia->setValue(fileCount);
		delete pgDia;
	}
	QMap<QString,Elem>::Iterator itf;
	for (itf = objectMap.begin(); itf != objectMap.end(); ++itf)
	{
		if (itf.value().isDir)
		{
			QPixmap pm(60, 60);
			pm.fill(palette().color(QPalette::Base));
			QPainter p;
			p.begin(&pm);
			p.drawPixmap(30 - itf.value().Preview.width() / 2, 30 - itf.value().Preview.height() / 2, itf.value().Preview);
			p.end();
			QListWidgetItem *item = new QListWidgetItem(QIcon(pm), itf.key(), this);
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			itf.value().widgetItem = item;
		}
	}
	for (itf = objectMap.begin(); itf != objectMap.end(); ++itf)
	{
		if (!itf.value().isDir)
		{
			itf.value().Preview = itf.value().Preview.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			QPixmap pm(60, 60);
			pm.fill(palette().color(QPalette::Base));
			QPainter p;
			p.begin(&pm);
			p.drawPixmap(30 - itf.value().Preview.width() / 2, 30 - itf.value().Preview.height() / 2, itf.value().Preview);
			p.end();
			QListWidgetItem *item = new QListWidgetItem(QIcon(pm), itf.key(), this);
			itf.value().widgetItem = item;
		}
	}
}

/* This is the main Dialog-Class for the Scrapbook */
Biblio::Biblio( QWidget* parent) : ScrPaletteBase( parent, "Sclib", false, 0 )
{
	resize( 230, 190 );
	BiblioLayout = new QVBoxLayout( this );
	BiblioLayout->setSpacing( 0 );
	BiblioLayout->setMargin( 0 );

	buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing( 5 );
	buttonLayout->setMargin( 0 );
	newButton = new QToolButton(this);
	newButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	newButton->setIcon(loadIcon("16/document-new.png"));
	newButton->setIconSize(QSize(16, 16));
	upButton = new QToolButton(this);
	upButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	upButton->setIcon(loadIcon("16/go-up.png"));
	upButton->setIconSize(QSize(16, 16));
	importButton = new QToolButton(this);
	importButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	importButton->setIcon(loadIcon("compfile16.png"));
	importButton->setIconSize(QSize(16, 16));
	closeButton = new QToolButton(this);
	closeButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	closeButton->setIcon(loadIcon("16/close.png"));
	closeButton->setIconSize(QSize(16, 16));
	configButton = new QToolButton(this);
	configButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	configButton->setIcon(loadIcon("16/configure.png"));
	configButton->setIconSize(QSize(16, 16));
	configMenue = new QMenu();
	conf_HideDirs = configMenue->addAction( tr("Hide Directorys"));
	conf_HideDirs->setCheckable(true);
	conf_HideDirs->setChecked(false);
	conf_HideImages = configMenue->addAction( tr("Hide Images"));
	conf_HideImages->setCheckable(true);
	conf_HideImages->setChecked(false);
	conf_HideVectors = configMenue->addAction( tr("Hide Vector files"));
	conf_HideVectors->setCheckable(true);
	conf_HideVectors->setChecked(false);
	configMenue->addSeparator();
	conf_OpenMode = configMenue->addAction( tr("Open Directorys in a new tab"));
	conf_OpenMode->setCheckable(true);
	conf_OpenMode->setChecked(false);
	configButton->setMenu(configMenue);
	configButton->setPopupMode(QToolButton::InstantPopup);
	buttonLayout->addWidget( newButton );
	buttonLayout->addWidget( upButton );
	buttonLayout->addWidget( importButton );
	buttonLayout->addWidget( closeButton );
	QSpacerItem* spacer = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum );
	buttonLayout->addItem( spacer );
	buttonLayout->addWidget( configButton );
	BiblioLayout->addLayout( buttonLayout );

	Frame3 = new QToolBox( this );
	activeBView = new BibView(this);
	Frame3->addItem(activeBView, tr("Main"));
	activeBView->visibleName = tr("Main");
	tempBView = new BibView(this);
	Frame3->addItem(tempBView, tr("Copied Items"));
	tempBView->visibleName = tr("Copied Items");
	tempCount = 0;
	actItem = 0;
	BiblioLayout->addWidget( Frame3 );

	languageChange();
	prefs = PrefsManager::instance()->prefsFile->getContext("Scrapbook");
	conf_HideDirs->setChecked(prefs->getBool("hideDirs", false));
	conf_HideImages->setChecked(prefs->getBool("hideImages", false));
	conf_HideVectors->setChecked(prefs->getBool("hideVectors", false));
	conf_OpenMode->setChecked(prefs->getBool("openMode", false));

	connect(newButton, SIGNAL(clicked()), this, SLOT(NewLib()));
	connect(upButton, SIGNAL(clicked()), this, SLOT(goOneDirUp()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(Import()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(closeLib()));
	connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
	connect(configMenue, SIGNAL(triggered(QAction *)), this, SLOT(updateView()));
}

void Biblio::setOpenScrapbooks(QStringList &fileNames)
{
	disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
	disconnect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(ObjFromFile(QString, int)));
	disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
	disconnect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
	disconnect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
	for (int rd = 0; rd < fileNames.count(); ++rd)
	{
		QString fileName = fileNames[rd];
		if (!fileName.isEmpty())
		{
			QDir d(fileName);
			activeBView = new BibView(this);
			QFileInfo fd(fileName);
			activeBView->canWrite = fd.isWritable();
			activeBView->setAcceptDrops(activeBView->canWrite);
			if (activeBView->canWrite)
				Frame3->addItem(activeBView, d.dirName());
			else
				Frame3->addItem(activeBView, QIcon(loadIcon("16/lock.png")), d.dirName());
			activeBView->ReadContents(fileName);
			activeBView->ScFilename = fileName;
			activeBView->visibleName = d.dirName();
			ScCore->fileWatcher->addDir(d.absolutePath(), true);
			activeBView->scrollToTop();
		}
	}
	activeBView = (BibView*)Frame3->widget(0);
	Frame3->setCurrentIndex(0);
	upButton->setEnabled(false);
	updateView();
	connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
	connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
	connect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(ObjFromFile(QString, int)));
	connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
	connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
}

QStringList Biblio::getOpenScrapbooks()
{
	QStringList ret;
	ret.clear();
	if (Frame3->count() > 2) // omit the first 2 Tabs since they contain the main and temp scrapbook
	{
		for (int a = 2; a < Frame3->count(); a++)
		{
			BibView* bv = (BibView*)Frame3->widget(a);
			ret.append(bv->ScFilename);
		}
	}
	return ret;
}

QStringList Biblio::getOpenScrapbooksNames()
{
	QStringList ret;
	ret.clear();
	for (int a = 0; a < Frame3->count(); a++)
	{
		BibView* bv = (BibView*)Frame3->widget(a);
		ret.append(bv->visibleName);
	}
	return ret;
}

void Biblio::setScrapbookFileName(QString fileName)
{
	activeBView->ScFilename=fileName;
}

const QString Biblio::getScrapbookFileName()
{
	return activeBView->ScFilename;
}

const int Biblio::objectCount()
{
	return activeBView->objectMap.count();
}

void Biblio::readOldContents(QString fileName, QString newName)
{
	activeBView->ReadOldContents(fileName, newName);
	activeBView->scrollToTop();
}

void Biblio::readContents(QString fileName)
{
	activeBView->ReadContents(fileName);
}

void Biblio::readTempContents(QString fileName)
{
	tempBView->ReadContents(fileName);
	tempBView->ScFilename = fileName;
	tempCount = tempBView->objectMap.count();
}

void Biblio::installEventFilter(QObject *filterObj)
{
	ScrPaletteBase::installEventFilter(filterObj);
	activeBView->installEventFilter(filterObj);
	tempBView->installEventFilter(filterObj);
}

void Biblio::updateView()
{
	for (int a = 0; a < Frame3->count(); a++)
	{
		BibView* bv = (BibView*)Frame3->widget(a);
		QMap<QString, BibView::Elem>::Iterator itf;
		for (itf = bv->objectMap.begin(); itf != bv->objectMap.end(); ++itf)
		{
			if (itf.value().isDir)
				itf.value().widgetItem->setHidden(conf_HideDirs->isChecked());
			if (itf.value().isRaster)
				itf.value().widgetItem->setHidden(conf_HideImages->isChecked());
			if (itf.value().isVector)
				itf.value().widgetItem->setHidden(conf_HideVectors->isChecked());
		}
	}
	prefs->set("hideDirs", conf_HideDirs->isChecked());
	prefs->set("hideImages", conf_HideImages->isChecked());
	prefs->set("hideVectors", conf_HideVectors->isChecked());
	prefs->set("openMode", conf_OpenMode->isChecked());
}

void Biblio::NewLib()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Choose a Scrapbook Directory"), dirs->get("scrap_load", "."));
	if (!fileName.isEmpty())
	{
		for (int a = 0; a < Frame3->count(); a++)
		{
			BibView* bv = (BibView*)Frame3->widget(a);
			if (fileName == bv->ScFilename)
				return;
		}
		disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
		disconnect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(ObjFromFile(QString, int)));
		disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
		disconnect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
		disconnect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		QDir d(fileName);
		activeBView = new BibView(this);
		QFileInfo fd(fileName);
		activeBView->canWrite = fd.isWritable();
		activeBView->setAcceptDrops(activeBView->canWrite);
		if (activeBView->canWrite)
			Frame3->addItem(activeBView, d.dirName());
		else
			Frame3->addItem(activeBView, QIcon(loadIcon("16/lock.png")), d.dirName());
		activeBView->ReadContents(fileName);
		activeBView->ScFilename = fileName;
		activeBView->visibleName = d.dirName();
		Frame3->setCurrentWidget(activeBView);
		ScCore->fileWatcher->addDir(d.absolutePath(), true);
		d.cdUp();
		dirs->set("scrap_load", d.absolutePath());
		activeBView->scrollToTop();
		upButton->setEnabled(!((Frame3->currentIndex() == 0) || (Frame3->currentIndex() == 1)));
		updateView();
		connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
		connect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(ObjFromFile(QString, int)));
		connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
		connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
	}
}

void Biblio::Import()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString s = QFileDialog::getOpenFileName(this,
	                                         tr("Choose a scrapbook file to import"),
	                                         dirs->get("old_scrap_load", "."),
	                                         tr("Scrapbook (*.scs *.SCS)"));
	
	if (!s.isEmpty())
	{
		dirs->set("old_scrap_load", s.left(s.lastIndexOf(QDir::toNativeSeparators("/"))));
		
		QString scrapbookFileO = s;
		QFileInfo scrapbookFileInfoO = QFileInfo(scrapbookFileO);
		if (scrapbookFileInfoO.exists())
		{
			readOldContents(scrapbookFileO, activeBView->ScFilename);
			readContents(activeBView->ScFilename);
		}
		activeBView->scrollToTop();
		updateView();
	}
}

void Biblio::closeLib()
{
	if (Frame3->count() == 2)
		close();
	if ((Frame3->currentIndex() == 0) || (Frame3->currentIndex() == 1))
		return;
	else
	{
		disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
		disconnect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(ObjFromFile(QString, int)));
		disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
		disconnect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
		disconnect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		QFileInfo fi(activeBView->ScFilename);
		ScCore->fileWatcher->removeDir(fi.absolutePath());
		Frame3->removeItem(Frame3->indexOf(activeBView));
		delete activeBView;  // currently disabled as the whole TabWidget vanishes when executing that delete????? -> seems to be fixed in Qt-4.3.3
		activeBView = (BibView*)Frame3->widget(0);
		Frame3->setCurrentIndex(0);
		upButton->setEnabled(!((Frame3->currentIndex() == 0) || (Frame3->currentIndex() == 1)));
		connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
		connect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(ObjFromFile(QString, int)));
		connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
		connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
	}
}

void Biblio::libChanged(int index)
{
	disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
	disconnect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(ObjFromFile(QString, int)));
	disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
	disconnect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
	activeBView = (BibView*)Frame3->widget(index);
	upButton->setEnabled(!((Frame3->currentIndex() == 0) || (Frame3->currentIndex() == 1)));
	connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
	connect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(ObjFromFile(QString, int)));
	connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
	connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
}

void Biblio::closeOnDel(QString libName)
{
	BibView* bv = NULL;
	int libIndex = 0;
	for (int a = 0; a < Frame3->count(); a++)
	{
		bv = (BibView*)Frame3->widget(a);
		if (libName == bv->ScFilename)
		{
			libIndex = a;
			break;
		}
	}
	if ((libIndex == 0) || (libIndex == 1))
		return;
	if (bv == activeBView)
		closeLib();
	else
	{
		disconnect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		QFileInfo fi(bv->ScFilename);
		ScCore->fileWatcher->removeDir(fi.absolutePath());
		Frame3->removeItem(Frame3->indexOf(bv));
		delete bv;
		connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
	}
}

void Biblio::reloadLib(QString fileName)
{
	for (int a = 0; a < Frame3->count(); a++)
	{
		BibView* bv = (BibView*)Frame3->widget(a);
		if (bv->ScFilename == fileName)
		{
			bv->ReadContents(fileName);
			bv->ScFilename = fileName;
			QDir d(fileName);
			bv->visibleName = d.dirName();
			bv->scrollToTop();
		}
	}
	updateView();
}

void Biblio::handleDoubleClick(QListWidgetItem *ite)
{
	if (activeBView->objectMap[ite->text()].isDir)
	{
		QFileInfo fi(activeBView->ScFilename);
		ScCore->fileWatcher->removeDir(fi.absolutePath());
		QString fileName = activeBView->ScFilename + "/" + ite->text();
		QDir d(fileName);
		QFileInfo fd(fileName);
		if (conf_OpenMode->isChecked())
		{
			activeBView = new BibView(this);
			if (fd.isWritable())
				Frame3->addItem(activeBView, d.dirName());
			else
				Frame3->addItem(activeBView, QIcon(loadIcon("16/lock.png")), d.dirName());
		}
		activeBView->canWrite = fd.isWritable();
		activeBView->setAcceptDrops(activeBView->canWrite);
		activeBView->ReadContents(fileName);
		activeBView->ScFilename = fileName;
		activeBView->visibleName = d.dirName();
		Frame3->setItemText(Frame3->indexOf(activeBView), d.dirName());
		if (!activeBView->canWrite)
			Frame3->setItemIcon(Frame3->indexOf(activeBView), QIcon(loadIcon("16/lock.png")));
		ScCore->fileWatcher->addDir(d.absolutePath(), true);
		d.cdUp();
		PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
		dirs->set("scrap_load", d.absolutePath());
		activeBView->scrollToTop();
		updateView();
	}
	else
	{
		emit pasteToActualPage(ite->text());
		activeBView->clearSelection();
		actItem = 0;
	}
}

void Biblio::goOneDirUp()
{
	QFileInfo fi(activeBView->ScFilename);
	ScCore->fileWatcher->removeDir(fi.absolutePath());
	QDir d(activeBView->ScFilename);
	d.cdUp();
	QString fileName = d.absolutePath();
	QFileInfo fd(fileName);
	activeBView->canWrite = fd.isWritable();
	activeBView->setAcceptDrops(activeBView->canWrite);
	activeBView->ReadContents(fileName);
	activeBView->ScFilename = fileName;
	activeBView->visibleName = d.dirName();
	Frame3->setItemText(Frame3->indexOf(activeBView), d.dirName());
	if (!activeBView->canWrite)
		Frame3->setItemIcon(Frame3->indexOf(activeBView), QIcon(loadIcon("16/lock.png")));
	ScCore->fileWatcher->addDir(d.absolutePath(), true);
	d.cdUp();
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	dirs->set("scrap_load", d.absolutePath());
	activeBView->scrollToTop();
	updateView();
}

void Biblio::handlePasteToPage()
{
	emit pasteToActualPage(actItem->text());
	activeBView->clearSelection();
	actItem = 0;
}

void Biblio::HandleMouse(QPoint p)
{
	QListWidgetItem *ite = activeBView->itemAt(p);
	if (ite != 0)
	{
		actItem = ite;
		QMenu *pmenu = new QMenu();
		QAction* renAct;
		QAction* delAct;
		QAction* pasteAct = pmenu->addAction( tr("Paste to Page"));
		connect(pasteAct, SIGNAL(triggered()), this, SLOT(handlePasteToPage()));
		if (activeBView->canWrite)
		{
			renAct = pmenu->addAction( tr("Rename"));
			connect(renAct, SIGNAL(triggered()), this, SLOT(renameObj()));
			delAct = pmenu->addAction( tr("Delete"));
			connect(delAct, SIGNAL(triggered()), this, SLOT(deleteObj()));
		}
		QSignalMapper *signalMapper = new QSignalMapper(this);
		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(copyObj(int)));
		QMenu *pmenu2 = new QMenu( tr("Copy To:"));
		QMenu *pmenu3 = new QMenu( tr("Move To:"));
		QSignalMapper *signalMapper2 = new QSignalMapper(this);
		connect(signalMapper2, SIGNAL(mapped(int)), this, SLOT(moveObj(int)));
		for (int a = 0; a < Frame3->count(); a++)
		{
			BibView* bv = (BibView*)Frame3->widget(a);
			if (bv != activeBView)
			{
				if (bv->canWrite)
				{
        			QAction *action = pmenu2->addAction(Frame3->itemText(Frame3->indexOf(Frame3->widget(a))));
					connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
					signalMapper->setMapping(action, a);
					if (activeBView->canWrite)
					{
        				QAction *action2 = pmenu3->addAction(Frame3->itemText(Frame3->indexOf(Frame3->widget(a))));
						connect(action2, SIGNAL(triggered()), signalMapper2, SLOT(map()));
						signalMapper2->setMapping(action2, a);
					}
				}
			}
		}
		pmenu->addMenu(pmenu2);
		if (activeBView->canWrite)
			pmenu->addMenu(pmenu3);
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		pmenu->exec(QCursor::pos());
		delete pmenu;
		delete pmenu2;
		delete signalMapper;
		if (activeBView->canWrite)
		{
			delete pmenu3;
			delete signalMapper2;
		}
	}
	else
	{
		QMenu *pmenu = new QMenu();
		QAction* delAct;
		QAction* saveAct;
		QAction* closeAct;
		if (activeBView->objectMap.count() != 0)
		{
			saveAct = pmenu->addAction( tr("Save as..."));
			connect(saveAct, SIGNAL(triggered()), this, SLOT(SaveAs()));
		}
		closeAct = pmenu->addAction( tr("Close"));
		if ((activeBView->canWrite) && (activeBView->objectMap.count() != 0))
		{
			delAct = pmenu->addAction( tr("Delete Contents"));
			connect(delAct, SIGNAL(triggered()), this, SLOT(deleteAllObj()));
		}
		connect(closeAct, SIGNAL(triggered()), this, SLOT(closeLib()));
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		pmenu->exec(QCursor::pos());
		delete pmenu;
	}
	activeBView->clearSelection();
	actItem = 0;
}

bool Biblio::copyObj(int id)
{
	QListWidgetItem *ite = actItem;
	QString nam = ite->text();
	BibView* bv = (BibView*)Frame3->widget(id);
	if (bv->objectMap.contains(nam))
	{
		Query *dia = new Query(this, "tt", 1, 0, tr("&Name:"), tr("New Entry"));
		dia->setEditText(nam, true);
		dia->setTestList(activeBView->objectMap.keys());
		if (dia->exec())
		{
			nam = dia->getEditText();
		}
		else
		{
			delete dia;
			return false;
		}
	}
	QPixmap pm;
	QByteArray cf;
	if (!loadRawText(QDir::cleanPath(activeBView->objectMap[ite->text()].Data), cf))
		return false;
	pm = activeBView->objectMap[ite->text()].Preview;
	QString dt = activeBView->objectMap[ite->text()].Data;
	QDir thumbs(bv->ScFilename);
	if (thumbs.exists())
	{
		if ((bv->canWrite) && (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews))
			thumbs.mkdir(".ScribusThumbs");
	}
	QFileInfo fi(dt);
	QFile f(QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename + "/" + nam + "." + fi.completeSuffix().toLower())));
	if(!f.open(QIODevice::WriteOnly))
		return false;
	QDataStream s;
	s.setDevice(&f);
	s.writeRawData(cf.data(), cf.length());
	f.close();
	bv->AddObj(nam, QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename + "/" + nam + "." + fi.completeSuffix().toLower())), pm);
	pm.save(QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename + "/.ScribusThumbs/" + nam +".png")), "PNG");
	QFileInfo fiD(QDir::toNativeSeparators(activeBView->ScFilename + "/" + fi.baseName()));
	if ((fiD.exists()) && (fiD.isDir()))
	{
		QDir dd = QDir(QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename)));
		dd.mkdir(QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename + "/" + nam)));
		QDir d(QDir::toNativeSeparators(activeBView->ScFilename + "/" + fi.baseName()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((d.exists()) && (d.count() != 0))
		{
			for (uint dc = 0; dc < d.count(); ++dc)
			{
				QString source = QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + fi.baseName() + "/" + d[dc]));
				QString target = QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename + "/" + nam + "/" + d[dc]));
				copyFile(source, target);
			}
		}
		if (fiD.baseName() != nam)
			adjustReferences(QDir::toNativeSeparators(bv->ScFilename + "/" + nam + "." + fi.completeSuffix().toLower()));
	}
	pm = pm.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();
	QListWidgetItem *item = new QListWidgetItem(QIcon(pm), nam, bv);
	bv->objectMap[nam].widgetItem = item;
	if (bv == tempBView)
	{
		tempCount++;
		if (tempBView->objectMap.count() > PrefsManager::instance()->appPrefs.scrapbookPrefs.numScrapbookCopies)
		{
			QMap<QString,BibView::Elem>::Iterator it;
			it = tempBView->objectMap.begin();
			QFile f(it.value().Data);
			f.remove();
			QFileInfo fi(QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
			if (fi.exists())
			{
				QFile f2(QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
				f2.remove();
			}
			QFileInfo fiD(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
			if ((fiD.exists()) && (fiD.isDir()))
			{
				QDir dd = QDir(QDir::toNativeSeparators(tempBView->ScFilename));
				QDir d(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
				if ((d.exists()) && (d.count() != 0))
				{
					for (uint dc = 0; dc < d.count(); ++dc)
					{
						QFile::remove(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key() + "/" + d[dc]));
					}
				}
				dd.rmdir(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
			}
			QString name = it.key();
			tempBView->objectMap.erase(it);
			QList<QListWidgetItem *> itL = tempBView->findItems(name, Qt::MatchExactly);
			if (itL.count() > 0)
			{
				ite = itL.at(0);
				delete tempBView->takeItem(tempBView->row(ite));
			}
		}
		emit updateRecentMenue();
	}
	return true;
}

void Biblio::moveObj(int id)
{
	if (copyObj(id))
	{
		deleteObj();
	}
}

void Biblio::deleteObj()
{
	if (!activeBView->canWrite)
		return;
	QListWidgetItem *ite = actItem;
	QString name = ite->text();
	QFile::remove(activeBView->objectMap[name].Data);
	QFileInfo fi(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + name + ".png"));
	if (fi.exists())
		QFile::remove(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + name + ".png"));
	QFileInfo fiD(QDir::toNativeSeparators(activeBView->ScFilename + "/" + name));
	if ((fiD.exists()) && (fiD.isDir()))
	{
		QDir dd = QDir(QDir::toNativeSeparators(activeBView->ScFilename));
		QDir d(QDir::toNativeSeparators(activeBView->ScFilename + "/" + name), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((d.exists()) && (d.count() != 0))
		{
			for (uint dc = 0; dc < d.count(); ++dc)
			{
				QFile::remove(QDir::toNativeSeparators(activeBView->ScFilename + "/" + name + "/" + d[dc]));
			}
		}
		dd.rmdir(name);
	}
	activeBView->objectMap.remove(name);
	delete activeBView->takeItem(activeBView->row(ite));
	if (activeBView == tempBView)
		emit updateRecentMenue();
	actItem = 0;
}

void Biblio::deleteAllObj()
{
	if (!activeBView->canWrite)
		return;
	int t = QMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to delete all entries?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (t == QMessageBox::No)
		return;
	QMap<QString,BibView::Elem>::Iterator it;
	for (it = activeBView->objectMap.begin(); it != activeBView->objectMap.end(); ++it)
	{
		QFile f(it.value().Data);
		f.remove();
		QFileInfo fi(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
		if (fi.exists())
		{
			QFile f2(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
			f2.remove();
		}
		QFileInfo fiD(QDir::toNativeSeparators(activeBView->ScFilename + "/" + it.key()));
		if ((fiD.exists()) && (fiD.isDir()))
		{
			QDir dd = QDir(QDir::toNativeSeparators(activeBView->ScFilename));
			QDir d(QDir::toNativeSeparators(activeBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
			if ((d.exists()) && (d.count() != 0))
			{
				for (uint dc = 0; dc < d.count(); ++dc)
				{
					QFile::remove(QDir::toNativeSeparators(activeBView->ScFilename + "/" + it.key() + "/" + d[dc]));
				}
			}
			dd.rmdir(QDir::toNativeSeparators(activeBView->ScFilename + "/" + it.key()));
		}
	}
	activeBView->clear();
	activeBView->objectMap.clear();
	if (activeBView == tempBView)
		emit updateRecentMenue();
	actItem = 0;
}

void Biblio::renameObj()
{
	if (!activeBView->canWrite)
		return;
	QString nam;
	QString ObjData;
	QPixmap ObjPreview;
	QListWidgetItem *ite = actItem;
	QString OldName = ite->text();
	Query *dia = new Query(this, "tt", 1, 0, tr("&Name:"), tr("New Name"));
	dia->setEditText(ite->text(), true);
	dia->setTestList(activeBView->objectMap.keys());
	if (dia->exec())
	{
		nam = dia->getEditText();
	}
	else
	{
		delete dia;
		return;
	}
	delete dia;
	ite->setText(nam);
	ObjData = activeBView->objectMap[OldName].Data;
	ObjPreview = activeBView->objectMap[OldName].Preview;
	QDir d = QDir();
	d.rename(ObjData, QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")));
	QFileInfo fi(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + OldName + ".png"));
	if (fi.exists())
		d.rename(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + OldName + ".png"), QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + ite->text() + ".png")));
	QFileInfo fiD(QDir::toNativeSeparators(activeBView->ScFilename + "/" + OldName));
	if ((fiD.exists()) && (fiD.isDir()))
	{
		QDir d = QDir();
		d.rename(QDir::toNativeSeparators(activeBView->ScFilename + "/" + OldName), QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + ite->text())));
		adjustReferences(QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")));
	}
	activeBView->objectMap.remove(OldName);
	activeBView->AddObj(ite->text(), QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")), ObjPreview);
	if (activeBView == tempBView)
		emit updateRecentMenue();
}

void Biblio::adjustReferences(QString nam)
{
	QByteArray cf;
	if (loadRawText(nam, cf))
	{
		QString f;
		if (cf.left(16) == "<SCRIBUSELEMUTF8")
			f = QString::fromUtf8(cf.data());
		else
		f = cf.data();
		QDomDocument docu("scridoc");
		docu.setContent(f);
		QDomElement elem = docu.documentElement();
		QDomNode DOC = elem.firstChild();
		QFileInfo fid(nam);
		while(!DOC.isNull())
		{
			QDomElement pg = DOC.toElement();
			if(pg.tagName() == "ITEM")
			{
				PageItem::ItemType PType = static_cast<PageItem::ItemType>(pg.attribute("PTYPE").toInt());
				if ((PType == PageItem::ImageFrame) || (PType == PageItem::TextFrame))
				{
					QString Pfile = pg.attribute("PFILE");
					if (!Pfile.isEmpty())
					{
						QFileInfo fi(Pfile);
						pg.setAttribute("PFILE", fid.baseName() + "/" + fi.fileName());
					}
					QString Pfile2 = pg.attribute("PFILE2","");
					if (!Pfile2.isEmpty())
					{
						QFileInfo fi(Pfile2);
						pg.setAttribute("PFILE2", fid.baseName() + "/" + fi.fileName());
					}
					QString Pfile3 = pg.attribute("PFILE3","");
					if (!Pfile3.isEmpty())
					{
						QFileInfo fi(Pfile3);
						pg.setAttribute("PFILE3", fid.baseName() + "/" + fi.fileName());
					}
					pg.setAttribute("relativePaths", 1);
				}
			}
			DOC = DOC.nextSibling();
		}
		QFile fl(nam);
		if(!fl.open(QIODevice::WriteOnly))
			return ;
		QDataStream s;
		QByteArray cs = docu.toByteArray();
		s.setDevice(&fl);
		s.writeRawData(cs.data(), cs.length());
		fl.close();
	}
}

QString Biblio::getObjectName(QString &text)
{
	QDomDocument docu("scridoc");
	docu.setContent(text);
	QDomElement elem = docu.documentElement();
	QDomNode DOC = elem.firstChild();
	QString result = "";
	while(!DOC.isNull())
	{
		QDomElement pg = DOC.toElement();
		if(pg.tagName() == "ITEM")
		{
			result = pg.attribute("ANNAME");
			break;
		}
		DOC = DOC.nextSibling();
	}
	return result;
}

void Biblio::ObjFromFile(QString path, int testResult)
{
	if (!activeBView->canWrite)
		return;
	bool isImage = false;
	bool isVector = false;
	QString tmp;
	QFileInfo fi(path);
	if ( fi.exists() )
	{
		QString nam = "";
		nam = fi.baseName();
		if (Frame3->currentIndex() == 1)
		{
			if (activeBView->objectMap.contains(nam))
				nam += "("+ tmp.setNum(tempCount) + ")";
			tempCount++;
		}
		else
		{
			if (activeBView->objectMap.contains(nam))
				nam += "("+ tmp.setNum(tempCount) + ")";
		}
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		Query *dia = new Query(this, "tt", 1, 0, tr("&Name:"), tr("New Entry"));
		dia->setEditText(nam, true);
		dia->setTestList(activeBView->objectMap.keys());
		if (dia->exec())
			nam = dia->getEditText();
		else
		{
			delete dia;
			return;
		}
		QImage img;
		if (testResult == 9999)
		{
			bool mode = false;
			ScImage im;
			CMSettings cms(0, "", Intent_Perceptual);
			cms.allowColorManagement(false);
			if (im.loadPicture(path, 1, cms, ScImage::Thumbnail, 72, &mode))
				img = im.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			isImage = true;
		}
		else
		{
			const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
			if( fmt )
			{
				img = fmt->readThumbnail(path);
				img = img.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			}
			isVector = true;
		}
		QByteArray cf;
		if (!loadRawText(path, cf))
			return;
		QPixmap pm = QPixmap::fromImage(img);
		QFile f(QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + nam + "." + fi.completeSuffix())));
		if(!f.open(QIODevice::WriteOnly))
			return;
		QDataStream s;
		s.setDevice(&f);
		s.writeRawData(cf.data(), cf.length());
		f.close();
		if (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews)
		{
			QDir thumbs(activeBView->ScFilename);
			if (thumbs.exists())
			{
				if ((activeBView->canWrite) && (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews))
					thumbs.mkdir(".ScribusThumbs");
			}
			img.save(QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + nam +".png")), "PNG");
		}
		activeBView->AddObj(nam, QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + nam + "." + fi.completeSuffix())), pm, false, isImage, isVector);
		QPixmap pm2(60, 60);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(QIcon(pm2), nam, activeBView);
		activeBView->objectMap[nam].widgetItem = item;
		if (isImage)
			item->setHidden(conf_HideImages->isChecked());
		if (isVector)
			item->setHidden(conf_HideVectors->isChecked());
		if (Frame3->currentIndex() == 1)
		{
			if (tempBView->objectMap.count() > PrefsManager::instance()->appPrefs.scrapbookPrefs.numScrapbookCopies)
			{
				QMap<QString,BibView::Elem>::Iterator it;
				it = tempBView->objectMap.begin();
				QFile f(it.value().Data);
				f.remove();
				QFileInfo fi(QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
				if (fi.exists())
				{
					QFile f2(QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
					f2.remove();
				}
				QFileInfo fiD(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
				if ((fiD.exists()) && (fiD.isDir()))
				{
					QDir dd = QDir(QDir::toNativeSeparators(tempBView->ScFilename));
					QDir d(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
					if ((d.exists()) && (d.count() != 0))
					{
						for (uint dc = 0; dc < d.count(); ++dc)
						{
							QFile::remove(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key() + "/" + d[dc]));
						}
					}
					dd.rmdir(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
				}
				QString name = it.key();
				tempBView->objectMap.erase(it);
				QList<QListWidgetItem *> itL = tempBView->findItems(name, Qt::MatchExactly);
				if (itL.count() > 0)
				{
					QListWidgetItem *ite = itL.at(0);
					delete tempBView->takeItem(tempBView->row(ite));
				}
			}
			emit updateRecentMenue();
		}
	}
}

void Biblio::ObjFromMenu(QString text)
{
	QString nam = "";
	QString tmp;
	if (!activeBView->canWrite)
		return;
	nam = getObjectName(text);
	if (Frame3->currentIndex() == 1)
	{
		if (nam.isEmpty())
			nam = tr("Object") + tmp.setNum(tempCount);
		if (activeBView->objectMap.contains(nam))
			nam += "("+ tmp.setNum(tempCount) + ")";
		tempCount++;
	}
	else
	{
		if (nam.isEmpty())
			nam = tr("Object") + tmp.setNum(activeBView->objectMap.count());
		if (activeBView->objectMap.contains(nam))
			nam += "("+ tmp.setNum(tempCount) + ")";
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	Query *dia = new Query(this, "tt", 1, 0, tr("&Name:"), tr("New Entry"));
	dia->setEditText(nam, true);
	dia->setTestList(activeBView->objectMap.keys());
	if (dia->exec())
	{
		nam = dia->getEditText();
	}
	else
	{
		delete dia;
		return;
	}
	delete dia;
	QString ff = text;
	activeBView->checkAndChange(ff, QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + nam + ".sce")), QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename)));
	ScPreview *pre = new ScPreview();
	QPixmap pm = QPixmap::fromImage(pre->createPreview(ff));
	activeBView->AddObj(nam, QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + nam + ".sce")), pm);
	if (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews)
	{
		QDir thumbs(activeBView->ScFilename);
		if (thumbs.exists())
		{
			if ((activeBView->canWrite) && (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews))
				thumbs.mkdir(".ScribusThumbs");
		}
		pm.save(QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + nam +".png")), "PNG");
	}
	pm = pm.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();
	QListWidgetItem *item = new QListWidgetItem(QIcon(pm2), nam, activeBView);
	activeBView->objectMap[nam].widgetItem = item;
	delete pre;
	if (Frame3->currentIndex() == 1)
	{
		if (tempBView->objectMap.count() > PrefsManager::instance()->appPrefs.scrapbookPrefs.numScrapbookCopies)
		{
			QMap<QString,BibView::Elem>::Iterator it;
			it = tempBView->objectMap.begin();
			QFile f(it.value().Data);
			f.remove();
			QFileInfo fi(QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
			if (fi.exists())
			{
				QFile f2(QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
				f2.remove();
			}
			QFileInfo fiD(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
			if ((fiD.exists()) && (fiD.isDir()))
			{
				QDir dd = QDir(QDir::toNativeSeparators(tempBView->ScFilename));
				QDir d(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
				if ((d.exists()) && (d.count() != 0))
				{
					for (uint dc = 0; dc < d.count(); ++dc)
					{
						QFile::remove(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key() + "/" + d[dc]));
					}
				}
				dd.rmdir(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
			}
			QString name = it.key();
			tempBView->objectMap.erase(it);
			QList<QListWidgetItem *> itL = tempBView->findItems(name, Qt::MatchExactly);
			if (itL.count() > 0)
			{
				QListWidgetItem *ite = itL.at(0);
				delete tempBView->takeItem(tempBView->row(ite));
			}
		}
		emit updateRecentMenue();
	}
}

void Biblio::ObjFromCopyAction(QString text, QString name)
{
	QString nam = "";
	QString tmp;
	nam = name;
	if (nam.isEmpty())
		nam = tr("Object") + tmp.setNum(tempCount);
	if (tempBView->objectMap.contains(nam))
		nam += "("+ tmp.setNum(tempCount) + ")";
	tempCount++;
	QString ff = text;
	tempBView->checkAndChange(ff, QDir::cleanPath(QDir::toNativeSeparators(tempBView->ScFilename + "/" + nam + ".sce")), QDir::cleanPath(QDir::toNativeSeparators(tempBView->ScFilename)));
	ScPreview *pre = new ScPreview();
	QPixmap pm = QPixmap::fromImage(pre->createPreview(ff));
	tempBView->AddObj(nam, QDir::cleanPath(QDir::toNativeSeparators(tempBView->ScFilename + "/" + nam + ".sce")), pm);
	if (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews)
	{
		QDir thumbs(tempBView->ScFilename);
		if (thumbs.exists())
		{
			if ((tempBView->canWrite) && (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews))
				thumbs.mkdir(".ScribusThumbs");
		}
		pm.save(QDir::cleanPath(QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/" + nam +".png")), "PNG");
	}
	pm = pm.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();
	QListWidgetItem *item = new QListWidgetItem(QIcon(pm2), nam, tempBView);
	tempBView->objectMap[nam].widgetItem = item;
	delete pre;
	if (tempBView->objectMap.count() > PrefsManager::instance()->appPrefs.scrapbookPrefs.numScrapbookCopies)
	{
		QMap<QString,BibView::Elem>::Iterator it;
		it = tempBView->objectMap.begin();
		QFile f(it.value().Data);
		f.remove();
		QFileInfo fi(QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
		if (fi.exists())
		{
			QFile f2(QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/" + it.key() + ".png"));
			f2.remove();
		}
		QFileInfo fiD(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
		if ((fiD.exists()) && (fiD.isDir()))
		{
			QDir dd = QDir(QDir::toNativeSeparators(tempBView->ScFilename));
			QDir d(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
			if ((d.exists()) && (d.count() != 0))
			{
				for (uint dc = 0; dc < d.count(); ++dc)
				{
					QFile::remove(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key() + "/" + d[dc]));
				}
			}
			dd.rmdir(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
		}
		QString name = it.key();
		tempBView->objectMap.erase(it);
		QList<QListWidgetItem *> itL = tempBView->findItems(name, Qt::MatchExactly);
		if (itL.count() > 0)
		{
			QListWidgetItem *ite = itL.at(0);
			delete tempBView->takeItem(tempBView->row(ite));
		}
	}
}

void Biblio::ObjFromMainMenu(QString text, int scrapID)
{
	QString nam = "";
	QString tmp;
	int scID = scrapID;
	if (scID > 0)
		scID++;
	BibView* actBView = (BibView*)Frame3->widget(scID);
	if (!actBView->canWrite)
		return;
	nam = getObjectName(text);
	if (nam.isEmpty())
		nam = tr("Object") + tmp.setNum(actBView->objectMap.count());
	if (actBView->objectMap.contains(nam))
		nam += "("+ tmp.setNum(tempCount) + ")";
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	Query *dia = new Query(this, "tt", 1, 0, tr("&Name:"), tr("New Entry"));
	dia->setEditText(nam, true);
	dia->setTestList(actBView->objectMap.keys());
	if (dia->exec())
	{
		nam = dia->getEditText();
	}
	else
	{
		delete dia;
		return;
	}
	delete dia;
	QString ff = text;
	actBView->checkAndChange(ff, QDir::cleanPath(QDir::toNativeSeparators(actBView->ScFilename + "/" + nam + ".sce")), QDir::cleanPath(QDir::toNativeSeparators(actBView->ScFilename)));
	ScPreview *pre = new ScPreview();
	QPixmap pm = QPixmap::fromImage(pre->createPreview(ff));
	actBView->AddObj(nam, QDir::cleanPath(QDir::toNativeSeparators(actBView->ScFilename + "/" + nam + ".sce")), pm);
	if (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews)
	{
		QDir thumbs(actBView->ScFilename);
		if (thumbs.exists())
		{
			if ((actBView->canWrite) && (PrefsManager::instance()->appPrefs.scrapbookPrefs.writePreviews))
				thumbs.mkdir(".ScribusThumbs");
		}
		pm.save(QDir::cleanPath(QDir::toNativeSeparators(actBView->ScFilename + "/.ScribusThumbs/" + nam +".png")), "PNG");
	}
	pm = pm.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();
	QListWidgetItem *item = new QListWidgetItem(QIcon(pm2), nam, actBView);
	actBView->objectMap[nam].widgetItem = item;
	delete pre;
}

void Biblio::CleanUpTemp()
{
	QMap<QString,BibView::Elem>::Iterator it;
	for (it = tempBView->objectMap.begin(); it != tempBView->objectMap.end(); ++it)
	{
		QFile f(it.value().Data);
		f.remove();
		QFileInfo fi(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
		if (fi.exists())
		{
			QFile f2(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
			f2.remove();
		}
		QFileInfo fiD(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
		if ((fiD.exists()) && (fiD.isDir()))
		{
			QDir dd = QDir(QDir::toNativeSeparators(tempBView->ScFilename));
			QDir d(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
			if ((d.exists()) && (d.count() != 0))
			{
				for (uint dc = 0; dc < d.count(); ++dc)
				{
					QFile::remove(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key() + "/" + d[dc]));
				}
			}
			dd.rmdir(QDir::toNativeSeparators(tempBView->ScFilename + "/" + it.key()));
		}
	}
}

void Biblio::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void Biblio::languageChange()
{
	setWindowTitle( tr( "Scrapbook" ) );
 	newButton->setToolTip( tr( "Create a new scrapbook page" ) );
 	upButton->setToolTip( tr( "Go up one Directory" ) );
 	importButton->setToolTip( tr( "Import a scrapbook file from Scribus <=1.3.2" ) );
 	closeButton->setToolTip( tr( "Close the selected scrapbook" ) );
 	configButton->setToolTip( tr( "Configure the scrapbook" ) );
	conf_HideDirs->setText( tr("Hide Directorys"));
	conf_HideImages->setText( tr("Hide Images"));
	conf_HideVectors->setText( tr("Hide Vector files"));
	conf_OpenMode->setText( tr("Open Directorys in a new tab"));
}
