/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/****************************************************************************
** Form implementation generated from reading ui file 'Bib.ui'
**
** Created: Sun Oct 14 19:47:56 2001
**
****************************************************************************/
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

#include "scpreview.h"
#include "prefsfile.h"
#include "util.h"
#include "commonstrings.h"
#include "prefsmanager.h"
#include "stencilreader.h"
#include "query.h"
#include "util_icon.h"
//CB TODO bring in the prefsManager instance locally here too

/* The Scrapbook View Class
 * inherited from QListWidget */
BibView::BibView(QWidget* parent) : QListWidget(parent)
{
	setDragEnabled(true);
	setViewMode(QListView::IconMode);
	setFlow(QListView::LeftToRight);
	setSortingEnabled(true);
	setWrapping(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setResizeMode(QListView::Adjust);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setIconSize(QSize(60, 60));
	objectMap.clear();
	ScFilename = "";
	canWrite = true;
}

 void BibView::startDrag(Qt::DropActions supportedActions)
 {
	QString dt = objectMap[currentItem()->text()].Data;
	QFileInfo fi(dt);
	if (fi.suffix().toLower() == "sml")
	{
		QByteArray cf;
		loadRawText(dt, cf);
		QString f = QString::fromUtf8(cf.data());
		StencilReader *pre = new StencilReader();
		dt = pre->createObjects(f);
		delete pre;
	}
	else if (fi.suffix().toLower() == "shape")
	{
		QByteArray cf;
		loadRawText(dt, cf);
		QString f = QString::fromUtf8(cf.data());
		StencilReader *pre = new StencilReader();
		dt = pre->createShape(f);
		delete pre;
	}
	else if (fi.suffix().toLower() == "sce")
	{
		if ( fi.exists() )
		{
			QUrl ur(dt);
			dt = ur.toString();
		}
	}
	QMimeData *mimeData = new QMimeData;
	mimeData->setText(dt);
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
	else
		e->ignore();
}

void BibView::AddObj(QString name, QString daten, QPixmap Bild)
{
	struct Elem DrElem;
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
		dd.mkdir(QDir::cleanPath(QDir::convertSeparators(dir + "/" + fid.baseName())));
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
						QFileInfo pfi2(QDir::cleanPath(QDir::convertSeparators(dir+"/"+Pfile)));
						source = pfi2.absoluteFilePath();
					}
					else
					{
						QFileInfo fi(Pfile);
						source = QDir::cleanPath(QDir::convertSeparators(QDir::homePath()+"/"+Pfile));
					}
					QFileInfo fi(Pfile);
					QString target = QDir::cleanPath(QDir::convertSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile2 = pg.attribute("PFILE2","");
				if (!Pfile2.isEmpty())
				{
					if (static_cast<bool>(pg.attribute("relativePaths", "0").toInt()))
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::convertSeparators(dir+"/"+Pfile2)));
						source = pfi2.absoluteFilePath();
					}
					else
					{
						QFileInfo fi(Pfile2);
						source = QDir::cleanPath(QDir::convertSeparators(QDir::homePath()+"/"+Pfile));
					}
					QFileInfo fi(Pfile2);
					QString target = QDir::cleanPath(QDir::convertSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE2", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile3 = pg.attribute("PFILE3","");
				if (!Pfile3.isEmpty())
				{
					if (static_cast<bool>(pg.attribute("relativePaths", "0").toInt()))
					{
						QFileInfo pfi2(QDir::cleanPath(QDir::convertSeparators(dir+"/"+Pfile3)));
						source = pfi2.absoluteFilePath();
					}
					else
					{
						QFileInfo fi(Pfile3);
						source = QDir::cleanPath(QDir::convertSeparators(QDir::homePath()+"/"+Pfile3));
					}
					QFileInfo fi(Pfile3);
					QString target = QDir::cleanPath(QDir::convertSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
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

void BibView::SaveContents(QString name, QString oldName)
{
	QDir d(oldName, "*.sce", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			QByteArray cf;
			if (!loadRawText(QDir::cleanPath(QDir::convertSeparators(oldName + "/" + d[dc])), cf))
				continue;
			QString ff = QString::fromUtf8(cf.data());
			QPixmap pm;
			QFileInfo fi(QDir::cleanPath(QDir::convertSeparators(oldName + "/" + d[dc])));
			QFileInfo fi2(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			else
			{
				QString f;
				if (cf.left(16) == "<SCRIBUSELEMUTF8")
					f = QString::fromUtf8(cf.data());
				else
					f = cf.data();
				ScPreview *pre = new ScPreview();
				pm = pre->createPreview(f);
				delete pre;
			}
			QFileInfo fi3(QDir::cleanPath(QDir::convertSeparators(name + "/" + d[dc])));
			pm.save(QDir::cleanPath(QDir::convertSeparators(fi3.path()+"/"+fi3.baseName()+".png")), "PNG");
			checkAndChange(ff, QDir::cleanPath(QDir::convertSeparators(name + "/" + d[dc])), QDir::cleanPath(QDir::convertSeparators(name)));
		}
	}
	QDir d2(oldName, "*.sml", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d2.exists()) && (d2.count() != 0))
	{
		for (uint dc = 0; dc < d2.count(); ++dc)
		{
			QByteArray cf;
			if (!loadRawText(QDir::cleanPath(QDir::convertSeparators(oldName + "/" + d2[dc])), cf))
				continue;
			QFile fil(QDir::cleanPath(QDir::convertSeparators(name + "/" + d2[dc])));
			if(!fil.open(QIODevice::WriteOnly))
				continue ;
			QDataStream s(&fil);
			s.writeRawData(cf.data(), cf.length());
			fil.close();
			QPixmap pm;
			QFileInfo fi(QDir::cleanPath(QDir::convertSeparators(oldName + "/" + d2[dc])));
			QFileInfo fi2(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			else
			{
				QString f = QString::fromUtf8(cf.data());
				StencilReader *pre = new StencilReader();
				pm = pre->createPreview(f);
				delete pre;
			}
			QFileInfo fi3(QDir::cleanPath(QDir::convertSeparators(name + "/" + d2[dc])));
			pm.save(QDir::cleanPath(QDir::convertSeparators(fi3.path()+"/"+fi3.baseName()+".png")), "PNG");
		}
	}
	QDir d3(name, "*.shape", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d3.exists()) && (d3.count() != 0))
	{
		for (uint dc = 0; dc < d3.count(); ++dc)
		{
			QByteArray cf;
			if (!loadRawText(QDir::cleanPath(QDir::convertSeparators(oldName + "/" + d3[dc])), cf))
				continue;
			QFile fil(QDir::cleanPath(QDir::convertSeparators(name + "/" + d3[dc])));
			if(!fil.open(QIODevice::WriteOnly))
				continue ;
			QDataStream s(&fil);
			s.writeRawData(cf.data(), cf.length());
			fil.close();
			QPixmap pm;
			QFileInfo fi(QDir::cleanPath(QDir::convertSeparators(oldName + "/" + d3[dc])));
			QFileInfo fi2(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			else
			{
				QString f = QString::fromUtf8(cf.data());
				StencilReader *pre = new StencilReader();
				QString f2 = pre->createShape(f);
				ScPreview *pre2 = new ScPreview();
				pm = pre2->createPreview(f2);
				delete pre;
				delete pre2;
			}
			QFileInfo fi3(QDir::cleanPath(QDir::convertSeparators(name + "/" + d3[dc])));
			pm.save(QDir::cleanPath(QDir::convertSeparators(fi3.path()+"/"+fi3.baseName()+".png")), "PNG");
		}
	}
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
			QFile fi(QDir::cleanPath(QDir::convertSeparators(newName + "/" + GetAttr(&dc, "NAME") + ".sce")));
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
	if (name.endsWith(QDir::convertSeparators("/")))
		nd = name.left(name.length()-1);
	QDir d(name, "*.sce", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			QPixmap pm;
			QByteArray cf;
			if (!loadRawText(QDir::cleanPath(QDir::convertSeparators(name + "/" + d[dc])), cf))
				continue;
			QFileInfo fi(QDir::cleanPath(QDir::convertSeparators(name + "/" + d[dc])));
			QFileInfo fi2(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			else
			{
				QString f;
				if (cf.left(16) == "<SCRIBUSELEMUTF8")
					f = QString::fromUtf8(cf.data());
				else
					f = cf.data();
				ScPreview *pre = new ScPreview();
				pm = pre->createPreview(f);
				if (canWrite)
					pm.save(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")), "PNG");
				delete pre;
			}
			AddObj(fi.baseName(), QDir::cleanPath(QDir::convertSeparators(name + "/" + d[dc])), pm);
		}
	}
	QDir d2(name, "*.sml", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d2.exists()) && (d2.count() != 0))
	{
		for (uint dc = 0; dc < d2.count(); ++dc)
		{
			QPixmap pm;
			QByteArray cf;
			if (!loadRawText(QDir::cleanPath(QDir::convertSeparators(name + "/" + d2[dc])), cf))
				continue;
			QFileInfo fi(QDir::cleanPath(QDir::convertSeparators(name + "/" + d2[dc])));
			QFileInfo fi2(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			else
			{
				QString f = QString::fromUtf8(cf.data());
				StencilReader *pre = new StencilReader();
				pm = pre->createPreview(f);
				if (canWrite)
					pm.save(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")), "PNG");
				delete pre;
			}
			AddObj(fi.baseName(), QDir::cleanPath(QDir::convertSeparators(name + "/" + d2[dc])), pm);
		}
	}
	QDir d3(name, "*.shape", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d3.exists()) && (d3.count() != 0))
	{
		for (uint dc = 0; dc < d3.count(); ++dc)
		{
			QPixmap pm;
			QByteArray cf;
			if (!loadRawText(QDir::cleanPath(QDir::convertSeparators(name + "/" + d3[dc])), cf))
				continue;
			QFileInfo fi(QDir::cleanPath(QDir::convertSeparators(name + "/" + d3[dc])));
			QFileInfo fi2(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
			QFileInfo fi2p(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".xpm")));
			if ((fi2.exists()) || (fi2p.exists()))
			{
				if (fi2.exists())
					pm.load(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")));
				else
					pm.load(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".xpm")));
			}
			else
			{
				QString f = QString::fromUtf8(cf.data());
				StencilReader *pre = new StencilReader();
				QString f2 = pre->createShape(f);
				ScPreview *pre2 = new ScPreview();
				pm = pre2->createPreview(f2);
				if (canWrite)
					pm.save(QDir::cleanPath(QDir::convertSeparators(fi.path()+"/"+fi.baseName()+".png")), "PNG");
				delete pre;
				delete pre2;
			}
			AddObj(fi.baseName(), QDir::cleanPath(QDir::convertSeparators(name + "/" + d3[dc])), pm);
		}
	}
	QMap<QString,Elem>::Iterator itf;
	for (itf = objectMap.begin(); itf != objectMap.end(); ++itf)
	{
		if (itf.value().Preview.width() > 60)
			itf.value().Preview = itf.value().Preview.scaledToWidth(60, Qt::SmoothTransformation);
		if (itf.value().Preview.height() > 60)
			itf.value().Preview = itf.value().Preview.scaledToHeight(60, Qt::SmoothTransformation);
		QPixmap pm(60, 60);
		pm.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm);
		p.drawPixmap(30 - itf.value().Preview.width() / 2, 30 - itf.value().Preview.height() / 2, itf.value().Preview);
		p.end();
		new QListWidgetItem(QIcon(pm), itf.key(), this);
	}
	sortItems();
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
	loadButton = new QToolButton(this);
	loadButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	loadButton->setIcon(loadIcon("16/document-open.png"));
	loadButton->setIconSize(QSize(16, 16));
	saveAsButton = new QToolButton(this);
	saveAsButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	saveAsButton->setIcon(loadIcon("16/document-save-as.png"));
	saveAsButton->setIconSize(QSize(16, 16));
	importButton = new QToolButton(this);
	importButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	importButton->setIcon(loadIcon("compfile16.png"));
	importButton->setIconSize(QSize(16, 16));
	closeButton = new QToolButton(this);
	closeButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	closeButton->setIcon(loadIcon("16/close.png"));
	closeButton->setIconSize(QSize(16, 16));
	buttonLayout->addWidget( newButton );
	buttonLayout->addWidget( loadButton );
	buttonLayout->addWidget( saveAsButton );
	buttonLayout->addWidget( importButton );
	buttonLayout->addWidget( closeButton );
	QSpacerItem* spacer = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum );
	buttonLayout->addItem( spacer );
	BiblioLayout->addLayout( buttonLayout );

	Frame3 = new QToolBox( this );
	activeBView = new BibView(this);
	Frame3->addItem(activeBView, tr("Main"));
	tempBView = new BibView(this);
	Frame3->addItem(tempBView, tr("Copied Items"));
	tempCount = 0;
	actItem = 0;
	BiblioLayout->addWidget( Frame3 );

	languageChange();

	connect(newButton, SIGNAL(clicked()), this, SLOT(NewLib()));
	connect(loadButton, SIGNAL(clicked()), this, SLOT(Load()));
	connect(saveAsButton, SIGNAL(clicked()), this, SLOT(SaveAs()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(Import()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(closeLib()));
	connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
}

void Biblio::setOpenScrapbooks(QStringList &fileNames)
{
	disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
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
			activeBView->scrollToTop();
		}
	}
	activeBView = (BibView*)Frame3->widget(0);
	Frame3->setCurrentIndex(0);
	connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
	connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
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
		disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
		disconnect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
		disconnect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		QDir d(fileName);
		activeBView = new BibView(this);
		Frame3->addItem(activeBView, d.dirName());
		activeBView->ScFilename = fileName;
		Frame3->setCurrentWidget(activeBView);
		d.cdUp();
		dirs->set("scrap_load", d.absolutePath());
		connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
		connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
		connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
	}
}

void Biblio::Load()
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
		Frame3->setCurrentWidget(activeBView);
		d.cdUp();
		dirs->set("scrap_load", d.absolutePath());
		activeBView->scrollToTop();
		connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
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
		dirs->set("old_scrap_load", s.left(s.lastIndexOf(QDir::convertSeparators("/"))));
		
		QString scrapbookFileO = s;
		QFileInfo scrapbookFileInfoO = QFileInfo(scrapbookFileO);
		if (scrapbookFileInfoO.exists())
		{
			readOldContents(scrapbookFileO, activeBView->ScFilename);
			readContents(activeBView->ScFilename);
		}
		activeBView->scrollToTop();
	}
}

void Biblio::SaveAs()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString fn = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), dirs->get("scrap_saveas", "."));
	if (!fn.isEmpty())
	{
		for (int a = 0; a < Frame3->count(); a++)
		{
			BibView* bv = (BibView*)Frame3->widget(a);
			if (fn == bv->ScFilename)
				return;
		}
		QDir d(fn);
		dirs->set("scrap_saveas", fn);
		activeBView->SaveContents(fn, activeBView->ScFilename);
		d.cdUp();
		dirs->set("scrap_saveas", d.absolutePath());
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
		disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
		disconnect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
		disconnect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		Frame3->removeItem(Frame3->indexOf(activeBView));
		delete activeBView;  // currently disabled as the whole TabWidget vanishes when executing that delete????? -> seems to be fixed in Qt-4.3.3
		activeBView = (BibView*)Frame3->widget(0);
		Frame3->setCurrentIndex(0);
		connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
		connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
		connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
		connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
	}
}

void Biblio::libChanged(int index)
{
	disconnect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
	disconnect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
	disconnect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
	activeBView = (BibView*)Frame3->widget(index);
	connect(activeBView, SIGNAL(objDropped(QString)), this, SLOT(ObjFromMenu(QString)));
	connect(activeBView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleMouse(QPoint)));
	connect(activeBView, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(handleDoubleClick(QListWidgetItem *)));
}

void Biblio::handleDoubleClick(QListWidgetItem *ite)
{
	emit pasteToActualPage(ite->text());
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
		if (dia->exec())
		{
			nam = dia->getEditText();
			while (activeBView->objectMap.contains(nam))
			{
				if (!dia->exec())
				{
					delete dia;
					return false;
				}
				nam = dia->getEditText();
			}
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
	QFileInfo fi(dt);
	QFile f(QDir::cleanPath(QDir::convertSeparators(bv->ScFilename + "/" + nam + "." + fi.completeSuffix().toLower())));
	if(!f.open(QIODevice::WriteOnly))
		return false;
	QDataStream s;
	s.setDevice(&f);
	s.writeRawData(cf.data(), cf.length());
	f.close();
	bv->AddObj(nam, QDir::cleanPath(QDir::convertSeparators(bv->ScFilename + "/" + nam + "." + fi.completeSuffix().toLower())), pm);
	pm.save(QDir::cleanPath(QDir::convertSeparators(bv->ScFilename + "/" + nam +".png")), "PNG");
	QFileInfo fiD(QDir::convertSeparators(activeBView->ScFilename + "/" + fi.baseName()));
	if ((fiD.exists()) && (fiD.isDir()))
	{
		QDir dd = QDir(QDir::cleanPath(QDir::convertSeparators(bv->ScFilename)));
		dd.mkdir(QDir::cleanPath(QDir::convertSeparators(bv->ScFilename + "/" + nam)));
		QDir d(QDir::convertSeparators(activeBView->ScFilename + "/" + fi.baseName()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((d.exists()) && (d.count() != 0))
		{
			for (uint dc = 0; dc < d.count(); ++dc)
			{
				QString source = QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename + "/" + fi.baseName() + "/" + d[dc]));
				QString target = QDir::cleanPath(QDir::convertSeparators(bv->ScFilename + "/" + nam + "/" + d[dc]));
				copyFile(source, target);
			}
		}
		if (fiD.baseName() != nam)
			adjustReferences(QDir::convertSeparators(bv->ScFilename + "/" + nam + "." + fi.completeSuffix().toLower()));
	}
	if (pm.width() > 60)
		pm = pm.scaledToWidth(60, Qt::SmoothTransformation);
	if (pm.height() > 60)
		pm = pm.scaledToHeight(60, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();
	new QListWidgetItem(QIcon(pm), nam, bv);
	if (bv == tempBView)
	{
		tempCount++;
		if (tempBView->objectMap.count() > PrefsManager::instance()->appPrefs.numScrapbookCopies)
		{
			QMap<QString,BibView::Elem>::Iterator it;
			it = tempBView->objectMap.begin();
			QFile f(it.value().Data);
			f.remove();
			QFileInfo fi(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
			if (fi.exists())
			{
				QFile f2(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
				f2.remove();
			}
			QFileInfo fiD(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()));
			if ((fiD.exists()) && (fiD.isDir()))
			{
				QDir dd = QDir(QDir::convertSeparators(tempBView->ScFilename));
				QDir d(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
				if ((d.exists()) && (d.count() != 0))
				{
					for (uint dc = 0; dc < d.count(); ++dc)
					{
						QFile::remove(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + "/" + d[dc]));
					}
				}
				dd.rmdir(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()));
			}
			QString name = it.key();
			tempBView->objectMap.erase(it);
			QList<QListWidgetItem *> itL = tempBView->findItems(name, Qt::MatchExactly);
			if (itL.count() > 0)
			{
				ite = itL.at(0);
				delete tempBView->takeItem(tempBView->row(ite));
			}
			tempBView->sortItems();
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
	QFileInfo fi(QDir::convertSeparators(activeBView->ScFilename + "/" + name + ".png"));
	if (fi.exists())
		QFile::remove(QDir::convertSeparators(activeBView->ScFilename + "/" + name + ".png"));
	QFileInfo fiD(QDir::convertSeparators(activeBView->ScFilename + "/" + name));
	if ((fiD.exists()) && (fiD.isDir()))
	{
		QDir dd = QDir(QDir::convertSeparators(activeBView->ScFilename));
		QDir d(QDir::convertSeparators(activeBView->ScFilename + "/" + name), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((d.exists()) && (d.count() != 0))
		{
			for (uint dc = 0; dc < d.count(); ++dc)
			{
				QFile::remove(QDir::convertSeparators(activeBView->ScFilename + "/" + name + "/" + d[dc]));
			}
		}
		dd.rmdir(name);
	}
	activeBView->objectMap.remove(name);
	delete activeBView->takeItem(activeBView->row(ite));
	activeBView->sortItems();
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
		QFileInfo fi(QDir::convertSeparators(activeBView->ScFilename + "/" + it.key() + ".png"));
		if (fi.exists())
		{
			QFile f2(QDir::convertSeparators(activeBView->ScFilename + "/" + it.key() + ".png"));
			f2.remove();
		}
		QFileInfo fiD(QDir::convertSeparators(activeBView->ScFilename + "/" + it.key()));
		if ((fiD.exists()) && (fiD.isDir()))
		{
			QDir dd = QDir(QDir::convertSeparators(activeBView->ScFilename));
			QDir d(QDir::convertSeparators(activeBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
			if ((d.exists()) && (d.count() != 0))
			{
				for (uint dc = 0; dc < d.count(); ++dc)
				{
					QFile::remove(QDir::convertSeparators(activeBView->ScFilename + "/" + it.key() + "/" + d[dc]));
				}
			}
			dd.rmdir(QDir::convertSeparators(activeBView->ScFilename + "/" + it.key()));
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
	if (dia->exec())
	{
		nam = dia->getEditText();
		while (activeBView->objectMap.contains(nam))
		{
			QMessageBox::warning(this, CommonStrings::trWarning, tr("Name \"%1\" is not unique.\nPlease choose another.").arg(nam), CommonStrings::tr_OK);
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			nam = dia->getEditText();
		}
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
	d.rename(ObjData, QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")));
	QFileInfo fi(QDir::convertSeparators(activeBView->ScFilename + "/" + OldName + ".png"));
	if (fi.exists())
		d.rename(QDir::convertSeparators(activeBView->ScFilename + "/" + OldName + ".png"), QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text() + ".png")));
	QFileInfo fiD(QDir::convertSeparators(activeBView->ScFilename + "/" + OldName));
	if ((fiD.exists()) && (fiD.isDir()))
	{
		QDir d = QDir();
		d.rename(QDir::convertSeparators(activeBView->ScFilename + "/" + OldName), QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text())));
		adjustReferences(QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")));
	}
	activeBView->objectMap.remove(OldName);
	activeBView->AddObj(ite->text(), QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")), ObjPreview);
	activeBView->sortItems();
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
	if (dia->exec())
	{
		nam = dia->getEditText();
		while (activeBView->objectMap.contains(nam))
		{
			if (!dia->exec())
			{
				delete dia;
				return;
			}
			nam = dia->getEditText();
		}
	}
	else
	{
		delete dia;
		return;
	}
	delete dia;
	QString ff = text;
	activeBView->checkAndChange(ff, QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename + "/" + nam + ".sce")), QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename)));
	ScPreview *pre = new ScPreview();
	QPixmap pm = pre->createPreview(ff);
	activeBView->AddObj(nam, QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename + "/" + nam + ".sce")), pm);
	pm.save(QDir::cleanPath(QDir::convertSeparators(activeBView->ScFilename + "/" + nam +".png")), "PNG");
	if (pm.width() > 60)
		pm = pm.scaledToWidth(60, Qt::SmoothTransformation);
	if (pm.height() > 60)
		pm = pm.scaledToHeight(60, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();
	new QListWidgetItem(QIcon(pm2), nam, activeBView);
	delete pre;
	if (Frame3->currentIndex() == 1)
	{
		if (tempBView->objectMap.count() > PrefsManager::instance()->appPrefs.numScrapbookCopies)
		{
			QMap<QString,BibView::Elem>::Iterator it;
			it = tempBView->objectMap.begin();
			QFile f(it.value().Data);
			f.remove();
			QFileInfo fi(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
			if (fi.exists())
			{
				QFile f2(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
				f2.remove();
			}
			QFileInfo fiD(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()));
			if ((fiD.exists()) && (fiD.isDir()))
			{
				QDir dd = QDir(QDir::convertSeparators(tempBView->ScFilename));
				QDir d(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
				if ((d.exists()) && (d.count() != 0))
				{
					for (uint dc = 0; dc < d.count(); ++dc)
					{
						QFile::remove(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + "/" + d[dc]));
					}
				}
				dd.rmdir(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()));
			}
			QString name = it.key();
			tempBView->objectMap.erase(it);
			QList<QListWidgetItem *> itL = tempBView->findItems(name, Qt::MatchExactly);
			if (itL.count() > 0)
			{
				QListWidgetItem *ite = itL.at(0);
				delete tempBView->takeItem(tempBView->row(ite));
			}
			tempBView->sortItems();
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
	tempBView->checkAndChange(ff, QDir::cleanPath(QDir::convertSeparators(tempBView->ScFilename + "/" + nam + ".sce")), QDir::cleanPath(QDir::convertSeparators(tempBView->ScFilename)));
	ScPreview *pre = new ScPreview();
	QPixmap pm = pre->createPreview(ff);
	tempBView->AddObj(nam, QDir::cleanPath(QDir::convertSeparators(tempBView->ScFilename + "/" + nam + ".sce")), pm);
	pm.save(QDir::cleanPath(QDir::convertSeparators(tempBView->ScFilename + "/" + nam +".png")), "PNG");
	if (pm.width() > 60)
		pm = pm.scaledToWidth(60, Qt::SmoothTransformation);
	if (pm.height() > 60)
		pm = pm.scaledToHeight(60, Qt::SmoothTransformation);
	QPixmap pm2(60, 60);
	pm2.fill(palette().color(QPalette::Base));
	QPainter p;
	p.begin(&pm2);
	p.drawPixmap(30 - pm.width() / 2, 30 - pm.height() / 2, pm);
	p.end();
	new QListWidgetItem(QIcon(pm2), nam, tempBView);
	delete pre;
	if (tempBView->objectMap.count() > PrefsManager::instance()->appPrefs.numScrapbookCopies)
	{
		QMap<QString,BibView::Elem>::Iterator it;
		it = tempBView->objectMap.begin();
		QFile f(it.value().Data);
		f.remove();
		QFileInfo fi(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
		if (fi.exists())
		{
			QFile f2(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
			f2.remove();
		}
		QFileInfo fiD(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()));
		if ((fiD.exists()) && (fiD.isDir()))
		{
			QDir dd = QDir(QDir::convertSeparators(tempBView->ScFilename));
			QDir d(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
			if ((d.exists()) && (d.count() != 0))
			{
				for (uint dc = 0; dc < d.count(); ++dc)
				{
					QFile::remove(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + "/" + d[dc]));
				}
			}
			dd.rmdir(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()));
		}
		QString name = it.key();
		tempBView->objectMap.erase(it);
		QList<QListWidgetItem *> itL = tempBView->findItems(name, Qt::MatchExactly);
		if (itL.count() > 0)
		{
			QListWidgetItem *ite = itL.at(0);
			delete tempBView->takeItem(tempBView->row(ite));
		}
		tempBView->sortItems();
	}
}

void Biblio::CleanUpTemp()
{
	QMap<QString,BibView::Elem>::Iterator it;
	for (it = tempBView->objectMap.begin(); it != tempBView->objectMap.end(); ++it)
	{
		QFile f(it.value().Data);
		f.remove();
		QFileInfo fi(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
		if (fi.exists())
		{
			QFile f2(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + ".png"));
			f2.remove();
		}
		QFileInfo fiD(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()));
		if ((fiD.exists()) && (fiD.isDir()))
		{
			QDir dd = QDir(QDir::convertSeparators(tempBView->ScFilename));
			QDir d(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
			if ((d.exists()) && (d.count() != 0))
			{
				for (uint dc = 0; dc < d.count(); ++dc)
				{
					QFile::remove(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key() + "/" + d[dc]));
				}
			}
			dd.rmdir(QDir::convertSeparators(tempBView->ScFilename + "/" + it.key()));
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
 	loadButton->setToolTip( tr( "Load an existing scrapbook" ) );
 	saveAsButton->setToolTip( tr( "Save the selected scrapbook" ) );
 	importButton->setToolTip( tr( "Import an scrapbook file from Scribus <=1.3.2" ) );
 	closeButton->setToolTip( tr( "Close the selected scrapbook" ) );
}
