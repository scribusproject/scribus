/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scrapbookpalette.h"

#include <QAction>
#include <QApplication>
#include <QByteArray>
#include <QCursor>
#include <QDataStream>
#include <QDomDocument>
#include <QDrag>
#include <QDropEvent>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>
#include <QProgressDialog>
#include <QSet>
#include <QSignalMapper>
#include <QSpacerItem>
#include <QToolBox>
#include <QToolButton>
#include <QToolTip>
#include <QUrl>
#include <QVBoxLayout>

#include "cmsettings.h"
#include "commonstrings.h"
#include "fileloader.h"
#include "filewatcher.h"
#include "iconmanager.h"
#include "loadsaveplugin.h"
#include "plugins/formatidlist.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "query.h"
#include "scimage.h"
#include "scpaths.h"
#include "scpreview.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "util.h"
#include "util_color.h"
#include "util_file.h"
#include "util_formats.h"

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
}

 void BibView::startDrag(Qt::DropActions supportedActions)
 {
	QStringList vectorFiles = LoadSavePlugin::getExtensionsForPreview(FORMATID_FIRSTUSER);
	QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::RASTORIMAGES, 1));
	QStringList rasterFiles = formatD.split("|");
	QString dt = objectMap[currentItem()->text()].Data;
	QFileInfo fi(dt);
	QMimeData *mimeData = new QMimeData;
	QList<QUrl> urlList;
	if (fi.suffix().toLower() == "sce")
	{
		if (fi.exists())
		{
			QUrl ur = QUrl::fromLocalFile(dt);
			urlList.append(ur);
			mimeData->setUrls(urlList);
		}
	}
	else if ((vectorFiles.contains(fi.suffix().toLower())) || (rasterFiles.contains(fi.suffix().toLower())))
	{
		if (fi.exists())
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
		QString text = e->mimeData()->text();
		int startElemPos = text.leftRef(512).indexOf("<SCRIBUSELEM");
		if (startElemPos >= 0)
			emit objDropped(text);
	}
	else if (e->mimeData()->hasFormat("text/uri-list"))
	{
		QStringList vectorFiles = LoadSavePlugin::getExtensionsForPreview(FORMATID_FIRSTUSER);
		QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::RASTORIMAGES, 1));
		QStringList rasterFiles = formatD.split("|");
		QList<QUrl> fileUrls = e->mimeData()->urls();
		e->acceptProposedAction();
		for (int i = 0; i < fileUrls.count(); ++i)
		{
			QUrl url(fileUrls[i] );
			QFileInfo fi(url.path());
			if (vectorFiles.contains(fi.suffix().toLower()))
			{
				FileLoader *fileLoader = new FileLoader(url.path());
				int testResult = fileLoader->testFile();
				delete fileLoader;
				if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER) && (fi.exists()))
					emit fileDropped(url.path(), testResult);
			}
			else if (rasterFiles.contains(fi.suffix().toLower()))
				emit fileDropped(url.path(), 9999);
		}
	}
	else
		e->ignore();
}

void BibView::addObject(const QString& name, const QString& daten, const QPixmap& Bild, bool isDir, bool isRaster, bool isVector)
{
	struct Elem DrElem;
	DrElem.isDir = isDir;
	DrElem.isRaster = isRaster;
	DrElem.isVector = isVector;
	DrElem.Data = daten;
	DrElem.Preview = Bild;
	objectMap.insert(name, DrElem);
}

void BibView::checkForImg(const QDomElement& elem, bool &hasImage)
{
	QDomNode node = elem.firstChild();
	while (!node.isNull())
	{
		QDomElement pg = node.toElement();
		if (pg.tagName() == "PAGEOBJECT")
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
			else if (PType == PageItem::OSGFrame)
			{
				QString Pfile = pg.attribute("modelFile");
				if (!Pfile.isEmpty())
					hasImage = true;
			}
			else if (PType == PageItem::Group)
				checkForImg(pg, hasImage);
			if (hasImage)
				break;
		}
		node = node.nextSibling();
	}
}

void BibView::checkAndChange(const QString& text, const QString& nam, const QString& dir)
{
	bool hasImage = false;
	QDomDocument docu("scridoc");
	docu.setContent(text);
	QDomElement elem = docu.documentElement();
	QDomNode node = elem.firstChild();
	while (!node.isNull())
	{
		QDomElement pg = node.toElement();
		if (pg.tagName() == "ITEM")
		{
			auto itemType = static_cast<PageItem::ItemType>(pg.attribute("PTYPE").toInt());
			if ((itemType == PageItem::ImageFrame) || (itemType == PageItem::TextFrame))
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
			else if (itemType == PageItem::OSGFrame)
			{
				QString Pfile = pg.attribute("modelFile");
				if (!Pfile.isEmpty())
					hasImage = true;
			}
			else if (itemType == PageItem::Group)
				checkForImg(pg, hasImage);
			if (hasImage)
				break;
		}
		node = node.nextSibling();
	}
	QFileInfo fid(nam);
	if (hasImage)
	{
		QDir dd(dir);
		dd.mkdir(QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName())));
	}
	QString source;
	QString fileDir = ScPaths::applicationDataDir();
	bool first = true;
	node = elem.firstChild();
	while (!node.isNull())
	{
		QDomElement pg = node.toElement();
		if (pg.tagName() == "ITEM")
		{
			if (first)
				pg.setAttribute("ANNAME", fid.baseName());
			first = false;
			auto itemType = static_cast<PageItem::ItemType>(pg.attribute("PTYPE").toInt());
			if ((itemType == PageItem::ImageFrame) || (itemType == PageItem::TextFrame))
			{
				QString Pfile = pg.attribute("PFILE");
				if (!Pfile.isEmpty())
				{
					QFileInfo fi(Pfile);
					if (fi.isAbsolute())
						source = QDir::cleanPath(QDir::toNativeSeparators(Pfile));
					else
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(fileDir + "/" + Pfile)));
						source = pfi2.absoluteFilePath();
					}
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile2 = pg.attribute("PFILE2","");
				if (!Pfile2.isEmpty())
				{
					QFileInfo fi(Pfile2);
					if (fi.isAbsolute())
						source = QDir::cleanPath(QDir::toNativeSeparators(Pfile2));
					else
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(fileDir + "/" + Pfile2)));
						source = pfi2.absoluteFilePath();
					}
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE2", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile3 = pg.attribute("PFILE3","");
				if (!Pfile3.isEmpty())
				{
					QFileInfo fi(Pfile3);
					if (fi.isAbsolute())
						source = QDir::cleanPath(QDir::toNativeSeparators(Pfile3));
					else
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(fileDir + "/" + Pfile3)));
						source = pfi2.absoluteFilePath();
					}
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE3", fid.baseName() + "/" + fi.fileName());
				}
				pg.setAttribute("relativePaths", 1);
			}
			else if (itemType == PageItem::OSGFrame)
			{
				QString Pfile = pg.attribute("modelFile");
				if (!Pfile.isEmpty())
				{
					QFileInfo fi(Pfile);
					if (fi.isAbsolute())
						source = QDir::cleanPath(QDir::toNativeSeparators(Pfile));
					else
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(fileDir + "/" + Pfile)));
						source = pfi2.absoluteFilePath();
					}
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("modelFile", fid.baseName() + "/" + fi.fileName());
				}
			}
			else if (itemType == PageItem::Group)
			{
				checkAndChangeGroups(pg, dir, fid);
			}
		}
		node = node.nextSibling();
	}
	QFile f(nam);
	if (!f.open(QIODevice::WriteOnly))
		return ;
	QDataStream s;
	QByteArray cs = docu.toByteArray();
	s.setDevice(&f);
	s.writeRawData(cs.data(), cs.length());
	f.close();
}

void BibView::checkAndChangeGroups(const QDomElement& elem, const QString& dir, const QFileInfo& fid)
{
	QString source;
	QString fileDir = ScPaths::applicationDataDir();
	QDomNode node = elem.firstChild();
	while (!node.isNull())
	{
		QDomElement pg = node.toElement();
		if (pg.tagName() == "PAGEOBJECT")
		{
			auto itemType = static_cast<PageItem::ItemType>(pg.attribute("PTYPE").toInt());
			if ((itemType == PageItem::ImageFrame) || (itemType == PageItem::TextFrame))
			{
				QString Pfile = pg.attribute("PFILE");
				if (!Pfile.isEmpty())
				{
					QFileInfo fi(Pfile);
					if (fi.isAbsolute())
						source = QDir::cleanPath(QDir::toNativeSeparators(Pfile));
					else
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(fileDir + "/" + Pfile)));
						source = pfi2.absoluteFilePath();
					}
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile2 = pg.attribute("PFILE2","");
				if (!Pfile2.isEmpty())
				{
					QFileInfo fi(Pfile2);
					if (fi.isAbsolute())
						source = QDir::cleanPath(QDir::toNativeSeparators(Pfile2));
					else
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(fileDir + "/" + Pfile2)));
						source = pfi2.absoluteFilePath();
					}
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE2", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile3 = pg.attribute("PFILE3","");
				if (!Pfile3.isEmpty())
				{
					QFileInfo fi(Pfile3);
					if (fi.isAbsolute())
						source = QDir::cleanPath(QDir::toNativeSeparators(Pfile3));
					else
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(fileDir + "/" + Pfile3)));
						source = pfi2.absoluteFilePath();
					}
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE3", fid.baseName() + "/" + fi.fileName());
				}
				pg.setAttribute("relativePaths", 1);
			}
			else if (itemType == PageItem::OSGFrame)
			{
				QString Pfile = pg.attribute("modelFile");
				if (!Pfile.isEmpty())
				{
					QFileInfo fi(Pfile);
					if (fi.isAbsolute())
						source = QDir::cleanPath(QDir::toNativeSeparators(Pfile));
					else
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::toNativeSeparators(fileDir + "/" + Pfile)));
						source = pfi2.absoluteFilePath();
					}
					QString target = QDir::cleanPath(QDir::toNativeSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("modelFile", fid.baseName() + "/" + fi.fileName());
				}
			}
			else if (itemType == PageItem::Group)
			{
				checkAndChangeGroups(pg, dir, fid);
			}
		}
		node = node.nextSibling();
	}
}

void BibView::readOldContents(const QString& name, const QString& newName)
{
	bool isUtf8 = false;
	QDomDocument docu("scridoc");
	QString ff;
	QByteArray cf;
	if (!loadRawText(name, cf))
		return;
	if (cf.left(17) == "<SCRIBUSSCRAPUTF8")
	{
		ff = QString::fromUtf8(cf.data());
		isUtf8 = true;
	}
	else
		ff = QString::fromLocal8Bit(cf.data());
	if (!docu.setContent(ff))
		return;
	QDomElement elem = docu.documentElement();
	if ((elem.tagName() != "SCRIBUSSCRAP") && (elem.tagName() != "SCRIBUSSCRAPUTF8"))
		return;
	QDomNode node = elem.firstChild();
	while (!node.isNull())
	{
		QDomElement dc = node.toElement();
		if (dc.tagName() == "OBJEKT")
		{
			QFile fi(QDir::cleanPath(QDir::toNativeSeparators(newName + "/" + dc.attribute("NAME", nullptr) + ".sce")));
			if (!fi.open(QIODevice::WriteOnly))
				continue ;
			QDataStream s(&fi);
			QString fn = dc.attribute("DATA", nullptr);
			cf = isUtf8? fn.toUtf8() : fn.toLocal8Bit();
			s.writeRawData(cf.data(), cf.length());
			fi.close();
		}
		node = node.nextSibling();
	}
}

void BibView::readContents(const QString& name)
{
	int fileCount = 0;
	int readCount = 0;
	QSet<QString> vectorFound;
	QSet<QString> rasterFound;

	clear();
	objectMap.clear();

	QString dirPath = QDir::cleanPath(QDir::toNativeSeparators(name));
	while ((dirPath.length() > 1) && dirPath.endsWith("/"))
		dirPath.chop(1);

	QDir thumbs(dirPath);
	if (thumbs.exists())
	{
		if (canWrite && PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews)
			thumbs.mkdir(".ScribusThumbs");
		thumbs.cd(".ScribusThumbs");
	}
	QDir dd(dirPath, "*", QDir::Name, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
	fileCount += dd.count();
	QDir d(dirPath, "*.sce", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	fileCount += d.count();

	QStringList vectorFiles = LoadSavePlugin::getExtensionsForPreview(FORMATID_FIRSTUSER);
	for (int v = 0; v < vectorFiles.count(); v++)
	{
		QString ext = "*." + vectorFiles[v];
		QDir d4(dirPath, ext, QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		fileCount += d4.count();
		if (d4.count() > 0)
			vectorFound.insert(vectorFiles[v]);
	}

	QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::RASTORIMAGES, 1));
	QStringList rasterFiles = formatD.split("|");
	for (int v = 0; v < rasterFiles.count(); v++)
	{
		QString ext = "*." + rasterFiles[v];
		QDir d5(dirPath, ext, QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		fileCount += d5.count();
		if (d5.count() > 0)
			rasterFound.insert(rasterFiles[v]);
	}

	QProgressDialog *pgDia = nullptr;
	QStringList previewFiles;
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
			if (dd[dc].compare(".ScribusThumbs", Qt::CaseInsensitive) == 0)
				continue;
			QPixmap pm = IconManager::instance().loadPixmap("folder.png");
			addObject(dd[dc], "", pm, true);
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
			if (!loadRawText(QDir::cleanPath(dirPath + "/" + d[dc]), cf))
				continue;
			QFileInfo fi(QDir::cleanPath(dirPath + "/" + d[dc]));
			QString filePath = QDir::cleanPath(QDir::toNativeSeparators(fi.path()));
			bool pngExists = QFile::exists(filePath + "/.ScribusThumbs/" + fi.baseName() + ".png");
			if (pngExists)
				pm.load(filePath + "/.ScribusThumbs/" + fi.baseName() + ".png");
			else
			{
				pngExists = QFile::exists(filePath + "/" + fi.baseName() + ".png");
				if (pngExists)
					pm.load(filePath + "/" + fi.baseName() + ".png");
				else
				{
					QString f;
					if (cf.left(16) == "<SCRIBUSELEMUTF8")
						f = QString::fromUtf8(cf.data());
					else
						f = cf.data();
					pm = QPixmap::fromImage(ScPreview::create(f));
					if (canWrite && PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews)
						pm.save(filePath + "/.ScribusThumbs/" + fi.baseName() + ".png", "PNG");
				}
			}
			previewFiles.append(fi.baseName() + ".png");
			addObject(fi.baseName(), QDir::cleanPath(dirPath + "/" + d[dc]), pm);
		}
	}

	for (int v = 0; v < vectorFiles.count(); v++)
	{
		if (!vectorFound.contains(vectorFiles[v]))
			continue;
		QString ext = "*." + vectorFiles[v];
		QDir d4(dirPath, ext, QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((!d4.exists()) || (d4.count() <= 0))
			continue;
		for (uint dc = 0; dc < d4.count(); ++dc)
		{
			if (pgDia)
			{
				pgDia->setValue(readCount);
				readCount++;
			}
			QPixmap pm;
			QFileInfo fi(QDir::cleanPath(dirPath + "/" + d4[dc]));
			QString filePath = QDir::cleanPath(QDir::toNativeSeparators(fi.path()));
			bool pngExists = QFile::exists(filePath + "/.ScribusThumbs/" + fi.fileName() + ".png");
			if (pngExists)
				pm.load(filePath + "/.ScribusThumbs/" + fi.fileName() + ".png");
			else
			{
				FileLoader *fileLoader = new FileLoader(QDir::cleanPath(dirPath + "/" + d4[dc]));
				int testResult = fileLoader->testFile();
				delete fileLoader;
				if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
				{
					const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
					if (fmt)
					{
						QImage im = fmt->readThumbnail(dirPath + "/" + d4[dc]);
						im = im.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
						if (canWrite && PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews)
							im.save(filePath + "/.ScribusThumbs/" + fi.fileName() + ".png", "PNG");
						pm = QPixmap::fromImage(im);
					}
				}
			}
			addObject(fi.fileName(), QDir::cleanPath(dirPath + "/" + d4[dc]), pm, false, false, true);
		}
	}

	for (int v = 0; v < rasterFiles.count(); v++)
	{
		if (!rasterFound.contains(rasterFiles[v]))
			continue;
		QString ext = "*." + rasterFiles[v];
		QDir d5(dirPath, ext, QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((!d5.exists()) || (d5.count() <= 0))
			continue;
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
			QFileInfo fi(QDir::cleanPath(dirPath + "/" + d5[dc]));
			QString filePath = QDir::cleanPath(QDir::toNativeSeparators(fi.path()));
			bool pngExists = QFile::exists(filePath + "/.ScribusThumbs/" + fi.fileName() + ".png");
			if (pngExists)
				pm.load(filePath + "/.ScribusThumbs/" + fi.fileName() + ".png");
			else
			{
				bool mode = false;
				ScImage im;
				CMSettings cms(nullptr, "", Intent_Perceptual);
				cms.allowColorManagement(false);
				if (im.loadPicture(dirPath + "/" + d5[dc], 1, cms, ScImage::Thumbnail, 72, &mode))
				{
					QImage img = im.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
					if (canWrite && PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews)
						img.save(filePath + "/.ScribusThumbs/" + fi.fileName() + ".png", "PNG");
					pm = QPixmap::fromImage(img);
				}
			}
			addObject(fi.fileName(), QDir::cleanPath(dirPath + "/" + d5[dc]), pm, false, true);
		}
	}
	if (pgDia)
	{
		pgDia->setValue(fileCount);
		delete pgDia;
	}

	for (auto itf = objectMap.begin(); itf != objectMap.end(); ++itf)
	{
		if (!itf.value().isDir)
			continue;
		QPixmap pm(60, 60);
		pm.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm);
		p.drawPixmap(30 - itf.value().Preview.width() / 2, 30 - itf.value().Preview.height() / 2, itf.value().Preview);
		p.end();
		auto *item = new QListWidgetItem(QIcon(pm), itf.key(), this);
		item->setToolTip(itf.key());
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		itf.value().widgetItem = item;
	}

	IconManager& iconManager = IconManager::instance();
	for (auto itf = objectMap.begin(); itf != objectMap.end(); ++itf)
	{
		if (itf.value().isDir)
			continue;
		auto& preview = itf.value().Preview;
		preview = preview.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QPixmap pm(60, 60);
		pm.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm);
		p.fillRect(0, 0, 60, 60, QBrush(iconManager.loadPixmap("testfill.png")));
		p.drawPixmap(30 - preview.width() / 2, 30 - preview.height() / 2, preview);
		p.end();
		auto *item = new QListWidgetItem(QIcon(pm), itf.key(), this);
		item->setToolTip(itf.key());
		itf.value().widgetItem = item;
	}
}

/* This is the main Dialog-Class for the Scrapbook */
Biblio::Biblio(QWidget* parent) : ScDockPalette(parent, "Sclib", Qt::WindowFlags())
{
	setObjectName(QString::fromLocal8Bit("Sclib"));
	setMinimumSize( QSize(220, 240) );
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	containerWidget = new QWidget(this);
	BiblioLayout = new QVBoxLayout( containerWidget );
	BiblioLayout->setSpacing(3);
	BiblioLayout->setContentsMargins(3, 3, 3, 3);

	buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing(3);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	newButton = new QToolButton(this);
	newButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	newButton->setIcon(IconManager::instance().loadPixmap("16/document-new.png"));
	newButton->setIconSize(QSize(16, 16));
	upButton = new QToolButton(this);
	upButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	upButton->setIcon(IconManager::instance().loadPixmap("16/go-up.png"));
	upButton->setIconSize(QSize(16, 16));
	importButton = new QToolButton(this);
	importButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	importButton->setIcon(IconManager::instance().loadPixmap("compfile16.png"));
	importButton->setIconSize(QSize(16, 16));
	closeButton = new QToolButton(this);
	closeButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	closeButton->setIcon(IconManager::instance().loadPixmap("16/close.png"));
	closeButton->setIconSize(QSize(16, 16));
	configButton = new QToolButton(this);
	configButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	configButton->setIcon(IconManager::instance().loadPixmap("16/configure.png"));
	configButton->setIconSize(QSize(16, 16));

	configMenue = new QMenu();
	conf_HideDirs = configMenue->addAction( tr("Hide Directories"));
	conf_HideDirs->setCheckable(true);
	conf_HideDirs->setChecked(false);
	conf_HideImages = configMenue->addAction( tr("Hide Images"));
	conf_HideImages->setCheckable(true);
	conf_HideImages->setChecked(false);
	conf_HideVectors = configMenue->addAction( tr("Hide Vector files"));
	conf_HideVectors->setCheckable(true);
	conf_HideVectors->setChecked(false);
	configMenue->addSeparator();
	conf_OpenMode = configMenue->addAction( tr("Open Directories in a new tab"));
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

	BiblioLayout->addWidget( Frame3 );
	setWidget( containerWidget );

	languageChange();
	m_prefs = PrefsManager::instance().prefsFile->getContext("Scrapbook");
	conf_HideDirs->setChecked(m_prefs->getBool("hideDirs", false));
	conf_HideImages->setChecked(m_prefs->getBool("hideImages", false));
	conf_HideVectors->setChecked(m_prefs->getBool("hideVectors", false));
	conf_OpenMode->setChecked(m_prefs->getBool("openMode", false));

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

	connect(newButton, SIGNAL(clicked()), this, SLOT(newLib()));
	connect(upButton, SIGNAL(clicked()), this, SLOT(goOneDirUp()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(Import()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(closeLib()));
	connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
	connect(configMenue, SIGNAL(triggered(QAction *)), this, SLOT(updateView()));
}

void Biblio::setOpenScrapbooks(const QStringList &fileNames)
{
	disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(objFromMenu(QString)));
	disconnect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(objFromFile(QString, int)));
	disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(handleMouse(QPoint)));
	disconnect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
	disconnect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
	for (int rd = 0; rd < fileNames.count(); ++rd)
	{
		QString fileName = fileNames[rd];
		if (fileName.isEmpty())
			continue;
		QDir d(fileName);
		activeBView = new BibView(this);
		QFileInfo fd(fileName);
		activeBView->canWrite = fd.isWritable();
		activeBView->setAcceptDrops(activeBView->canWrite);
		if (activeBView->canWrite)
			Frame3->addItem(activeBView, d.dirName());
		else
			Frame3->addItem(activeBView, IconManager::instance().loadIcon("16/lock.png"), d.dirName());
		activeBView->readContents(fileName);
		activeBView->ScFilename = fileName;
		activeBView->visibleName = d.dirName();
		ScCore->fileWatcher->addDir(d.absolutePath(), true);
		activeBView->scrollToTop();
	}
	activeBView = (BibView*)Frame3->widget(0);
	Frame3->setCurrentIndex(0);
	upButton->setEnabled(false);
	updateView();
	connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
	connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(objFromMenu(QString)));
	connect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(objFromFile(QString, int)));
	connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(handleMouse(QPoint)));
	connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
}

QStringList Biblio::getOpenScrapbooks() const
{
	QStringList ret;
	if (Frame3->count() > 2) // omit the first 2 Tabs since they contain the main and temp scrapbook
	{
		for (int i = 2; i < Frame3->count(); i++)
		{
			auto bv = (BibView*) Frame3->widget(i);
			ret.append(bv->ScFilename);
		}
	}
	return ret;
}

QStringList Biblio::getOpenScrapbooksNames() const
{
	QStringList ret;
	for (int i = 0; i < Frame3->count(); i++)
	{
		auto bv = (BibView*) Frame3->widget(i);
		ret.append(bv->visibleName);
	}
	return ret;
}

void Biblio::setScrapbookFileName(const QString& fileName)
{
	activeBView->ScFilename = fileName;
}

const QString& Biblio::getScrapbookFileName() const
{
	return activeBView->ScFilename;
}

int Biblio::objectCount() const
{
	return activeBView->objectMap.count();
}

bool Biblio::tempHasContents() const
{
	return (!tempBView->objectMap.isEmpty());
}

void Biblio::readOldContents(const QString& fileName, const QString& newName)
{
	activeBView->readOldContents(fileName, newName);
	activeBView->scrollToTop();
}

void Biblio::readContents(const QString& fileName)
{
	activeBView->readContents(fileName);
}

void Biblio::readTempContents(const QString& fileName)
{
	tempBView->readContents(fileName);
	tempBView->ScFilename = fileName;
	m_tempCount = tempBView->objectMap.count();
}

void Biblio::installEventFilter(QObject *filterObj)
{
//	ScrPaletteBase::installEventFilter(filterObj);
	activeBView->installEventFilter(filterObj);
	tempBView->installEventFilter(filterObj);
}

void Biblio::updateView()
{
	for (int i = 0; i < Frame3->count(); i++)
	{
		BibView* bv = (BibView*) Frame3->widget(i);
		for (auto itf = bv->objectMap.begin(); itf != bv->objectMap.end(); ++itf)
		{
			if (itf.value().isDir)
				itf.value().widgetItem->setHidden(conf_HideDirs->isChecked());
			if (itf.value().isRaster)
				itf.value().widgetItem->setHidden(conf_HideImages->isChecked());
			if (itf.value().isVector)
				itf.value().widgetItem->setHidden(conf_HideVectors->isChecked());
		}
	}
	m_prefs->set("hideDirs", conf_HideDirs->isChecked());
	m_prefs->set("hideImages", conf_HideImages->isChecked());
	m_prefs->set("hideVectors", conf_HideVectors->isChecked());
	m_prefs->set("openMode", conf_OpenMode->isChecked());
}

void Biblio::newLib()
{
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Choose a Scrapbook Directory"), dirs->get("scrap_load", "."));
	if (fileName.isEmpty())
		return;

	for (int i = 0; i < Frame3->count(); i++)
	{
		BibView* bv = (BibView*) Frame3->widget(i);
		if (fileName == bv->ScFilename)
			return;
	}

	disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(objFromMenu(QString)));
	disconnect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(objFromFile(QString, int)));
	disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(handleMouse(QPoint)));
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
		Frame3->addItem(activeBView, IconManager::instance().loadIcon("16/lock.png"), d.dirName());
	activeBView->readContents(fileName);
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
	connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(objFromMenu(QString)));
	connect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(objFromFile(QString, int)));
	connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(handleMouse(QPoint)));
	connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(handleDoubleClick(QListWidgetItem*)));

	emit scrapbookListChanged();
}

void Biblio::Import()
{
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	QString s = QFileDialog::getOpenFileName(this,
	                                         tr("Choose a scrapbook file to import"),
	                                         dirs->get("old_scrap_load", "."),
	                                         tr("Scrapbook (*.scs *.SCS)"));
	
	if (s.isEmpty())
		return;

	dirs->set("old_scrap_load", s.left(s.lastIndexOf(QDir::toNativeSeparators("/"))));
		
	QFileInfo scrapbookFileInfoO(s);
	if (scrapbookFileInfoO.exists())
	{
		readOldContents(s, activeBView->ScFilename);
		readContents(activeBView->ScFilename);
	}
	activeBView->scrollToTop();
	updateView();
}

void Biblio::closeLib()
{
	if (Frame3->count() == 2)
		close();
	if ((Frame3->currentIndex() == 0) || (Frame3->currentIndex() == 1))
		return;

	disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(objFromMenu(QString)));
	disconnect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(objFromFile(QString, int)));
	disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(handleMouse(QPoint)));
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
	connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(objFromMenu(QString)));
	connect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(objFromFile(QString, int)));
	connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(handleMouse(QPoint)));
	connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));

	emit scrapbookListChanged();
}

void Biblio::libChanged(int index)
{
	disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(objFromMenu(QString)));
	disconnect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(objFromFile(QString, int)));
	disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(handleMouse(QPoint)));
	disconnect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));

	activeBView = (BibView*)Frame3->widget(index);
	upButton->setEnabled(!((Frame3->currentIndex() == 0) || (Frame3->currentIndex() == 1)));

	connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(objFromMenu(QString)));
	connect(activeBView, SIGNAL(fileDropped(QString, int)), this, SLOT(objFromFile(QString, int)));
	connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(handleMouse(QPoint)));
	connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
}

void Biblio::closeOnDel(const QString& libName)
{
	BibView* bv = nullptr;

	int libIndex = 0;
	for (int i = 0; i < Frame3->count(); i++)
	{
		bv = (BibView*) Frame3->widget(i);
		if (libName == bv->ScFilename)
		{
			libIndex = i;
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

void Biblio::reloadLib(const QString& fileName)
{
	for (int i = 0; i < Frame3->count(); i++)
	{
		BibView* bv = (BibView*) Frame3->widget(i);
		if (bv->ScFilename == fileName)
		{
			bv->readContents(fileName);
			bv->ScFilename = fileName;
			QDir d(fileName);
			if (i > 1)
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
				Frame3->addItem(activeBView, IconManager::instance().loadIcon("16/lock.png"), d.dirName());
		}
		activeBView->canWrite = fd.isWritable();
		activeBView->setAcceptDrops(activeBView->canWrite);
		activeBView->readContents(fileName);
		activeBView->ScFilename = fileName;
		activeBView->visibleName = d.dirName();
		Frame3->setItemText(Frame3->indexOf(activeBView), d.dirName());
		if (!activeBView->canWrite)
			Frame3->setItemIcon(Frame3->indexOf(activeBView), IconManager::instance().loadIcon("16/lock.png"));
		ScCore->fileWatcher->addDir(d.absolutePath(), true);
		d.cdUp();
		PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
		dirs->set("scrap_load", d.absolutePath());
		activeBView->scrollToTop();
		updateView();
	}
	else
	{
		emit pasteToActualPage(ite->text());
		activeBView->clearSelection();
		actItem = nullptr;
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
	activeBView->readContents(fileName);
	activeBView->ScFilename = fileName;
	activeBView->visibleName = d.dirName();
	Frame3->setItemText(Frame3->indexOf(activeBView), d.dirName());
	if (!activeBView->canWrite)
		Frame3->setItemIcon(Frame3->indexOf(activeBView), IconManager::instance().loadIcon("16/lock.png"));
	ScCore->fileWatcher->addDir(d.absolutePath(), true);
	d.cdUp();
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	dirs->set("scrap_load", d.absolutePath());
	activeBView->scrollToTop();
	updateView();
}

void Biblio::handlePasteToPage()
{
	emit pasteToActualPage(actItem->text());
	activeBView->clearSelection();
	actItem = nullptr;
}

void Biblio::handleMouse(QPoint p)
{
	// #12359 : stop the file watcher here as it may run and trigger regeneration of
	// scrapbook content, hence invalidating actItem while context menu is executing.
	ScCore->fileWatcher->stop();
	QListWidgetItem *ite = activeBView->itemAt(p);
	if (ite != nullptr)
	{
		actItem = ite;
		QMenu *pmenu = new QMenu();
		QAction* renAct { nullptr };
		QAction* delAct { nullptr };
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
		for (int i = 0; i < Frame3->count(); i++)
		{
			const BibView* bv = (BibView*) Frame3->widget(i);
			if ((bv == activeBView) || (!bv->canWrite))
				continue;
			QAction *action = pmenu2->addAction(Frame3->itemText(Frame3->indexOf(Frame3->widget(i))));
			connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
			signalMapper->setMapping(action, i);
			if (activeBView->canWrite)
			{
				QAction *action2 = pmenu3->addAction(Frame3->itemText(Frame3->indexOf(Frame3->widget(i))));
				connect(action2, SIGNAL(triggered()), signalMapper2, SLOT(map()));
				signalMapper2->setMapping(action2, i);
			}
		}
		pmenu->addMenu(pmenu2);
		if (activeBView->canWrite)
			pmenu->addMenu(pmenu3);
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
		QAction* closeAct = pmenu->addAction( tr("Close"));
		if ((activeBView->canWrite) && (activeBView->objectMap.count() != 0))
		{
			QAction* delAct = pmenu->addAction( tr("Delete Contents"));
			connect(delAct, SIGNAL(triggered()), this, SLOT(deleteAllObj()));
		}
		connect(closeAct, SIGNAL(triggered()), this, SLOT(closeLib()));
		pmenu->exec(QCursor::pos());
		delete pmenu;
	}
	activeBView->clearSelection();
	actItem = nullptr;
	ScCore->fileWatcher->start();
}

bool Biblio::copyObj(int id)
{
	QListWidgetItem *ite = actItem;
	QString nam = ite->text();
	BibView* bv = (BibView*) Frame3->widget(id);
	if (bv->objectMap.contains(nam))
	{
		Query dia(this, "tt", true, tr("&Name:"), tr("New Entry"));
		dia.setValidator(QRegExp("[\\w()]+"));
		dia.setEditText(nam, true);
		dia.setTestList(activeBView->objectMap.keys());
		if (dia.exec())
			nam = dia.getEditText();
		else
			return false;
	}

	QByteArray cf;
	if (!loadRawText(QDir::cleanPath(activeBView->objectMap[ite->text()].Data), cf))
		return false;

	QPixmap pm = activeBView->objectMap[ite->text()].Preview;
	QString dt = activeBView->objectMap[ite->text()].Data;
	QDir thumbs(bv->ScFilename);
	if (thumbs.exists())
	{
		if ((bv->canWrite) && (PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews))
			thumbs.mkdir(".ScribusThumbs");
	}

	QFileInfo fi(dt);
	QFile f(QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename + "/" + nam + "." + fi.completeSuffix().toLower())));
	if (!f.open(QIODevice::WriteOnly))
		return false;

	QDataStream s;
	s.setDevice(&f);
	s.writeRawData(cf.data(), cf.length());
	f.close();

	bv->addObject(nam, QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename + "/" + nam + "." + fi.completeSuffix().toLower())), pm);
	pm.save(QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename + "/.ScribusThumbs/" + nam + ".png")), "PNG");
	QFileInfo fiD(QDir::toNativeSeparators(activeBView->ScFilename + "/" + fi.baseName()));
	if ((fiD.exists()) && (fiD.isDir()))
	{
		QDir dd(QDir::cleanPath(QDir::toNativeSeparators(bv->ScFilename)));
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
	item->setToolTip(nam);
	bv->objectMap[nam].widgetItem = item;

	if (bv == tempBView)
	{
		m_tempCount++;
		if (tempBView->objectMap.count() > PrefsManager::instance().appPrefs.scrapbookPrefs.numScrapbookCopies)
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
				QDir dd(QDir::toNativeSeparators(tempBView->ScFilename));
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
		QDir dd(QDir::toNativeSeparators(activeBView->ScFilename));
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
	actItem = nullptr;
}

void Biblio::deleteAllObj()
{
	if (!activeBView->canWrite)
		return;
	int t = ScMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to delete all entries?"),
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::No,	// GUI default
				QMessageBox::Yes);	// batch default
	if (t == QMessageBox::No)
		return;

	for (auto it = activeBView->objectMap.begin(); it != activeBView->objectMap.end(); ++it)
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
			QDir dd(QDir::toNativeSeparators(activeBView->ScFilename));
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
	actItem = nullptr;
}

void Biblio::renameObj()
{
	if (!activeBView->canWrite)
		return;

	QListWidgetItem *ite = actItem;
	QString oldName = ite->text();

	Query dia(this, "tt", true, tr("&Name:"), tr("New Name"));
	dia.setValidator(QRegExp("[\\w()]+"));
	dia.setEditText(ite->text(), true);
	dia.setTestList(activeBView->objectMap.keys());
	if (!dia.exec())
		return;
	QString nam = dia.getEditText();

	ite->setText(nam);
	ite->setToolTip(nam);
	QString objData = activeBView->objectMap[oldName].Data;
	QPixmap objPreview = activeBView->objectMap[oldName].Preview;

	QDir d;
	d.rename(objData, QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")));
	QFileInfo fi(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + oldName + ".png"));
	if (fi.exists())
		d.rename(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + oldName + ".png"), QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + ite->text() + ".png")));
	
	QFileInfo fiD(QDir::toNativeSeparators(activeBView->ScFilename + "/" + oldName));
	if ((fiD.exists()) && (fiD.isDir()))
	{
		QDir d;
		d.rename(QDir::toNativeSeparators(activeBView->ScFilename + "/" + oldName), QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + ite->text())));
		adjustReferences(QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")));
	}

	activeBView->objectMap.remove(oldName);
	activeBView->addObject(ite->text(), QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")), objPreview);
	if (activeBView == tempBView)
		emit updateRecentMenue();
}

void Biblio::adjustReferences(const QString& nam)
{
	QByteArray cf;
	if (!loadRawText(nam, cf))
		return;

	QString f;
	if (cf.left(16) == "<SCRIBUSELEMUTF8")
		f = QString::fromUtf8(cf.data());
	else
		f = cf.data();

	QDomDocument docu("scridoc");
	docu.setContent(f);
	QDomElement elem = docu.documentElement();
	QDomNode node = elem.firstChild();
	QFileInfo fid(nam);
	while (!node.isNull())
	{
		QDomElement pg = node.toElement();
		if (pg.tagName() == "ITEM")
		{
			auto itemType = static_cast<PageItem::ItemType>(pg.attribute("PTYPE").toInt());
			if ((itemType == PageItem::ImageFrame) || (itemType == PageItem::TextFrame))
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
			else if (itemType == PageItem::OSGFrame)
			{
				QString Pfile = pg.attribute("modelFile");
				if (!Pfile.isEmpty())
				{
					QFileInfo fi(Pfile);
					pg.setAttribute("modelFile", fid.baseName() + "/" + fi.fileName());
				}
			}
			else if (itemType == PageItem::Group)
			{
				adjustReferencesGroups(pg, fid);
			}
		}
		node = node.nextSibling();
	}

	QFile fl(nam);
	if (!fl.open(QIODevice::WriteOnly))
		return ;

	QDataStream s;
	QByteArray cs = docu.toByteArray();
	s.setDevice(&fl);
	s.writeRawData(cs.data(), cs.length());
	fl.close();
}

void Biblio::adjustReferencesGroups(const QDomElement& elem, const QFileInfo& fid)
{
	QDomNode node = elem.firstChild();
	while (!node.isNull())
	{
		QDomElement pg = node.toElement();
		if (pg.tagName() == "PAGEOBJECT")
		{
			auto itemType = static_cast<PageItem::ItemType>(pg.attribute("PTYPE").toInt());
			if ((itemType == PageItem::ImageFrame) || (itemType == PageItem::TextFrame))
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
			else if (itemType == PageItem::OSGFrame)
			{
				QString Pfile = pg.attribute("modelFile");
				if (!Pfile.isEmpty())
				{
					QFileInfo fi(Pfile);
					pg.setAttribute("modelFile", fid.baseName() + "/" + fi.fileName());
				}
			}
			else if (itemType == PageItem::Group)
			{
				adjustReferencesGroups(pg, fid);
			}
		}
		node = node.nextSibling();
	}
}

QString Biblio::getObjectName(const QString &text) const
{
	QDomDocument docu("scridoc");
	docu.setContent(text);
	QDomElement elem = docu.documentElement();
	QDomNode node = elem.firstChild();
	QString result;
	while (!node.isNull())
	{
		QDomElement pg = node.toElement();
		if (pg.tagName() == "ITEM")
		{
			result = pg.attribute("ANNAME");
			break;
		}
		node = node.nextSibling();
	}
	return result;
}

void Biblio::objFromFile(const QString& path, int testResult)
{
	if (!activeBView->canWrite)
		return;
	bool isImage = false;
	bool isVector = false;
	QString tmp;
	QFileInfo fi(path);
	if (!fi.exists())
		return;

	QString nam = fi.baseName();
	if (Frame3->currentIndex() == 1)
	{
		if (activeBView->objectMap.contains(nam))
			nam += "("+ tmp.setNum(m_tempCount) + ")";
		m_tempCount++;
	}
	else
	{
		if (activeBView->objectMap.contains(nam))
			nam += "("+ tmp.setNum(m_tempCount) + ")";
	}

	Query dia(this, "tt", true, tr("&Name:"), tr("New Entry"));
	dia.setValidator(QRegExp("[\\w()]+"));
	dia.setEditText(nam, true);
	dia.setTestList(activeBView->objectMap.keys());
	if (!dia.exec())
		return;
	nam = dia.getEditText();

	QImage img;
	if (testResult == 9999)
	{
		bool mode = false;
		ScImage im;
		CMSettings cms(nullptr, "", Intent_Perceptual);
		cms.allowColorManagement(false);
		if (im.loadPicture(path, 1, cms, ScImage::Thumbnail, 72, &mode))
			img = im.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		isImage = true;
	}
	else
	{
		const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
		if (fmt)
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
	if (!f.open(QIODevice::WriteOnly))
		return;

	QDataStream s;
	s.setDevice(&f);
	s.writeRawData(cf.data(), cf.length());
	f.close();

	if (PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews)
	{
		QDir thumbs(activeBView->ScFilename);
		if (thumbs.exists())
		{
			if ((activeBView->canWrite) && (PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews))
				thumbs.mkdir(".ScribusThumbs");
		}
		img.save(QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + nam + ".png")), "PNG");
	}
	activeBView->addObject(nam, QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + nam + "." + fi.completeSuffix())), pm, false, isImage, isVector);
	
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();

	QListWidgetItem *item = new QListWidgetItem(QIcon(pm2), nam, activeBView);
	item->setToolTip(nam);
	activeBView->objectMap[nam].widgetItem = item;
	if (isImage)
		item->setHidden(conf_HideImages->isChecked());
	if (isVector)
		item->setHidden(conf_HideVectors->isChecked());

	if (Frame3->currentIndex() == 1)
	{
		if (tempBView->objectMap.count() > PrefsManager::instance().appPrefs.scrapbookPrefs.numScrapbookCopies)
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
				QDir dd(QDir::toNativeSeparators(tempBView->ScFilename));
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
				const QListWidgetItem *ite = itL.at(0);
				delete tempBView->takeItem(tempBView->row(ite));
			}
		}
		emit updateRecentMenue();
	}
	reloadLib(activeBView->ScFilename);
}

void Biblio::objFromMenu(QString text)
{
	QString tmp;
	if (!activeBView->canWrite)
		return;

	QString nam(getObjectName(text));
	if (Frame3->currentIndex() == 1)
	{
		if (nam.isEmpty())
			nam = tr("Object") + tmp.setNum(m_tempCount);
		if (activeBView->objectMap.contains(nam))
			nam += "("+ tmp.setNum(m_tempCount) + ")";
		m_tempCount++;
	}
	else
	{
		if (nam.isEmpty())
			nam = tr("Object") + tmp.setNum(activeBView->objectMap.count());
		if (activeBView->objectMap.contains(nam))
			nam += "("+ tmp.setNum(m_tempCount) + ")";
	}

	Query dia(this, "tt", true, tr("&Name:"), tr("New Entry"));
	dia.setValidator(QRegExp("[\\w()]+"));
	dia.setEditText(nam, true);
	dia.setTestList(activeBView->objectMap.keys());
	if (!dia.exec())
		return;
	nam = dia.getEditText();

	QString ff = text;
	activeBView->checkAndChange(ff, QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + nam + ".sce")), QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename)));
	
	QPixmap pm = QPixmap::fromImage(ScPreview::create(ff));
	activeBView->addObject(nam, QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/" + nam + ".sce")), pm);
	if (PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews)
	{
		QDir thumbs(activeBView->ScFilename);
		if (thumbs.exists())
		{
			if ((activeBView->canWrite) && (PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews))
				thumbs.mkdir(".ScribusThumbs");
		}
		pm.save(QDir::cleanPath(QDir::toNativeSeparators(activeBView->ScFilename + "/.ScribusThumbs/" + nam + ".png")), "PNG");
	}

	pm = pm.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();

	QListWidgetItem *item = new QListWidgetItem(QIcon(pm2), nam, activeBView);
	item->setToolTip(nam);
	activeBView->objectMap[nam].widgetItem = item;

	if (Frame3->currentIndex() == 1)
	{
		if (tempBView->objectMap.count() > PrefsManager::instance().appPrefs.scrapbookPrefs.numScrapbookCopies)
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
				QDir dd(QDir::toNativeSeparators(tempBView->ScFilename));
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
		reloadLib(activeBView->ScFilename);
	}
}

void Biblio::objFromCopyAction(const QString& text, const QString& name)
{
	// Something is bad with temp scrapbook directory, either it does not exists
	// or is unreadeable, do not attempt to create temp scrapbook items in such case,
	// they may end up in root directory or drive
	if (tempBView->ScFilename.isEmpty())
		return;
	QString nativeTempScrapPath = QDir::toNativeSeparators(tempBView->ScFilename + "/");
	QString nativeTempThumbsPath = QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/");

	QString tmp;
	QString nam = name;
	if (nam.isEmpty())
		nam = tr("Object") + tmp.setNum(m_tempCount);
	if (tempBView->objectMap.contains(nam))
		nam += "("+ tmp.setNum(m_tempCount) + ")";
	m_tempCount++;

	tempBView->checkAndChange(text, QDir::cleanPath(nativeTempScrapPath + nam + ".sce"), QDir::cleanPath(QDir::toNativeSeparators(tempBView->ScFilename)));
	
	QPixmap pm = QPixmap::fromImage(ScPreview::create(text));
	tempBView->addObject(nam, QDir::cleanPath(nativeTempScrapPath + nam + ".sce"), pm);
	if (PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews)
	{
		QDir thumbs(tempBView->ScFilename);
		if (thumbs.exists())
		{
			if ((tempBView->canWrite) && (PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews))
				thumbs.mkdir(".ScribusThumbs");
		}
		pm.save(QDir::cleanPath(nativeTempThumbsPath + nam + ".png"), "PNG");
	}
	pm = pm.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();

	QListWidgetItem *item = new QListWidgetItem(QIcon(pm2), nam, tempBView);
	item->setToolTip(nam);
	tempBView->objectMap[nam].widgetItem = item;

	if (tempBView->objectMap.count() > PrefsManager::instance().appPrefs.scrapbookPrefs.numScrapbookCopies)
	{
		QMap<QString,BibView::Elem>::Iterator it;
		it = tempBView->objectMap.begin();
		QFile f(it.value().Data);
		f.remove();
		QFileInfo fi(nativeTempThumbsPath + it.key() + ".png");
		if (fi.exists())
		{
			QFile f2(nativeTempThumbsPath + it.key() + ".png");
			f2.remove();
		}
		QFileInfo fiD(nativeTempScrapPath + it.key());
		if ((fiD.exists()) && (fiD.isDir()))
		{
			QDir dd(QDir::toNativeSeparators(tempBView->ScFilename));
			QDir d(nativeTempScrapPath + it.key(), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
			if ((d.exists()) && (d.count() != 0))
			{
				for (uint dc = 0; dc < d.count(); ++dc)
				{
					QFile::remove(nativeTempScrapPath + it.key() + "/" + d[dc]);
				}
			}
			dd.rmdir(nativeTempScrapPath + it.key());
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
	reloadLib(tempBView->ScFilename);
}

void Biblio::objFromMainMenu(QString text, int scrapID)
{
	QString tmp;
	int scID = scrapID;
	if (scID > 0)
		scID++;
	BibView* actBView = (BibView*)Frame3->widget(scID);
	if (!actBView->canWrite)
		return;

	QString nam = getObjectName(text);
	if (nam.isEmpty())
		nam = tr("Object") + tmp.setNum(actBView->objectMap.count());
	if (actBView->objectMap.contains(nam))
		nam += "("+ tmp.setNum(m_tempCount) + ")";

	Query dia(this, "tt", true, tr("&Name:"), tr("New Entry"));
	dia.setValidator(QRegExp("[\\w()]+"));
	dia.setEditText(nam, true);
	dia.setTestList(activeBView->objectMap.keys());
	if (!dia.exec())
		return;
	nam = dia.getEditText();

	QString ff = text;
	actBView->checkAndChange(ff, QDir::cleanPath(QDir::toNativeSeparators(actBView->ScFilename + "/" + nam + ".sce")), QDir::cleanPath(QDir::toNativeSeparators(actBView->ScFilename)));

	QPixmap pm = QPixmap::fromImage(ScPreview::create(ff));
	actBView->addObject(nam, QDir::cleanPath(QDir::toNativeSeparators(actBView->ScFilename + "/" + nam + ".sce")), pm);
	if (PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews)
	{
		QDir thumbs(actBView->ScFilename);
		if (thumbs.exists())
		{
			if ((actBView->canWrite) && (PrefsManager::instance().appPrefs.scrapbookPrefs.writePreviews))
				thumbs.mkdir(".ScribusThumbs");
		}
		pm.save(QDir::cleanPath(QDir::toNativeSeparators(actBView->ScFilename + "/.ScribusThumbs/" + nam + ".png")), "PNG");
	}
	pm = pm.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();

	QListWidgetItem *item = new QListWidgetItem(QIcon(pm2), nam, actBView);
	item->setToolTip(nam);
	actBView->objectMap[nam].widgetItem = item;

	reloadLib(actBView->ScFilename);
}

void Biblio::cleanUpTemp()
{
	// Something is bad with temp scrapbook directory, either it does not exists
	// or is unreadeable, do not attempt to delete files in such case
	if (tempBView->ScFilename.isEmpty())
		return;
	QString nativeScrapPath = QDir::toNativeSeparators(tempBView->ScFilename + "/");
	QString nativeThumbsPath = QDir::toNativeSeparators(tempBView->ScFilename + "/.ScribusThumbs/");

	for (auto it = tempBView->objectMap.begin(); it != tempBView->objectMap.end(); ++it)
	{
		QFile f(it.value().Data);
		f.remove();
		QFileInfo fi1(nativeScrapPath + it.key() + ".png");
		if (fi1.exists())
		{
			QFile f2(nativeScrapPath + it.key() + ".png");
			f2.remove();
		}
		QFileInfo fi2(nativeThumbsPath + it.key() + ".png");
		if (fi2.exists())
		{
			QFile f2(nativeThumbsPath + it.key() + ".png");
			f2.remove();
		}
		QFileInfo fiD(nativeScrapPath + it.key());
		if ((fiD.exists()) && (fiD.isDir()))
		{
			QDir dd(QDir::toNativeSeparators(tempBView->ScFilename));
			QDir d(nativeScrapPath + it.key(), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
			if ((d.exists()) && (d.count() != 0))
			{
				for (uint dc = 0; dc < d.count(); ++dc)
					QFile::remove(nativeScrapPath + it.key() + "/" + d[dc]);
			}
			dd.rmdir(nativeScrapPath + it.key());
		}
	}
}

void Biblio::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		ScDockPalette::changeEvent(e);
}

void Biblio::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();

	newButton->setIcon(iconManager.loadPixmap("16/document-new.png"));
	newButton->setIconSize(QSize(16, 16));

	upButton->setIcon(iconManager.loadPixmap("16/go-up.png"));
	upButton->setIconSize(QSize(16, 16));

	importButton->setIcon(iconManager.loadPixmap("compfile16.png"));
	importButton->setIconSize(QSize(16, 16));

	closeButton->setIcon(iconManager.loadPixmap("16/close.png"));
	closeButton->setIconSize(QSize(16, 16));

	configButton->setIcon(iconManager.loadPixmap("16/configure.png"));
	configButton->setIconSize(QSize(16, 16));
}

void Biblio::languageChange()
{
	setWindowTitle( tr( "Scrapbook" ) );
 	newButton->setToolTip( tr( "Create a new scrapbook page" ) );
 	upButton->setToolTip( tr( "Go up one Directory" ) );
 	importButton->setToolTip( tr( "Import a scrapbook file from Scribus <=1.3.2" ) );
 	closeButton->setToolTip( tr( "Close the selected scrapbook" ) );
 	configButton->setToolTip( tr( "Configure the scrapbook" ) );
	conf_HideDirs->setText( tr("Hide Directories"));
	conf_HideImages->setText( tr("Hide Images"));
	conf_HideVectors->setText( tr("Hide Vector files"));
	conf_OpenMode->setText( tr("Open Directories in a new tab"));

	int index = Frame3->indexOf(activeBView);
	if (index >= 0)
	{
		Frame3->setItemText(index, tr("Main") );
		activeBView->visibleName = tr("Main");
	}

	index = Frame3->indexOf(tempBView);
	if (index >= 0)
	{
		Frame3->setItemText(index, tr("Copied Items"));
		tempBView->visibleName = tr("Copied Items");
	}
}
