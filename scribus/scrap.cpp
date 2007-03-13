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
#include "scrap.h"
//#include "scrap.moc"
#include <qfileinfo.h>
#include <qfile.h>
#include <q3url.h>
#include <q3textstream.h>
#include <qdom.h>
#include <q3filedialog.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qtoolbutton.h>
#include <qcursor.h>
#include <qtoolbox.h>
#include "query.h"
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <QKeyEvent>
#include <Q3ValueList>
#include <Q3CString>
#include <QPixmap>
#include <Q3Frame>
#include <QDropEvent>
#include <Q3PopupMenu>
#include <Q3VBoxLayout>
#include "scpreview.h"
#include "prefsfile.h"
#include "util.h"
#include "commonstrings.h"
#include "prefsmanager.h"
#include "stencilreader.h"
extern QPixmap loadIcon(QString nam);
//CB TODO bring in the prefsManager instance locally here too

/* The Scrapbook View Class
 * inherited from QIconView */
BibView::BibView(QWidget* parent) : Q3IconView(parent)
{
	setAutoArrange(true);
	setSorting(true);
	setResizeMode(Q3IconView::Adjust);
	objectMap.clear();
	ScFilename = "";
}

void BibView::keyPressEvent(QKeyEvent *k)
{
	//Why doesnt this widget send Escape to the QDialog? Lets make Escape work for now anyway.
	if (k->key()==Key_Escape && k->state() == 0)
	{
		Q3Frame *f=dynamic_cast<Q3Frame *>(parent());
		if (f)
		{
			QDialog *d=dynamic_cast<QDialog *>(f->parent());
			if (d)
				d->close();
		}
	}
	else
		Q3IconView::keyPressEvent(k);
}

Q3DragObject *BibView::dragObject()
{
	QString dt = objectMap[currentItem()->text()].Data;
	QFileInfo fi(dt);
	if (fi.extension(true).lower() == "sml")
	{
		Q3CString cf;
		loadRawText(dt, cf);
		QString f = QString::fromUtf8(cf.data());
		StencilReader *pre = new StencilReader();
		dt = pre->createObjects(f);
		delete pre;
	}
	else if (fi.extension(true).lower() == "shape")
	{
		Q3CString cf;
		loadRawText(dt, cf);
		QString f = QString::fromUtf8(cf.data());
		StencilReader *pre = new StencilReader();
		dt = pre->createShape(f);
		delete pre;
	}
	else if (fi.extension(true).lower() == "sce")
	{
		if ( fi.exists() )
		{
			Q3Url ur(dt);
			dt = ur.toString();
		}
	}
	Q3DragObject *dr = new Q3TextDrag(dt, this);
	dr->setPixmap(objectMap[currentItem()->text()].Preview);
	clearSelection();
	return dr;
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
		dd.mkdir(QDir::cleanDirPath(QDir::convertSeparators(dir + "/" + fid.baseName())));
	}
	QString source = "";
	QString target = "";
	DOC = elem.firstChild();
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
					if (static_cast<bool>(pg.attribute("relativePaths", "0").toInt()))
					{
						QFileInfo pfi2(QDir::cleanDirPath(QDir::convertSeparators(dir+"/"+Pfile)));
						source = pfi2.absFilePath();
					}
					else
					{
						QFileInfo fi(Pfile);
						source = QDir::cleanDirPath(QDir::convertSeparators(QDir::homeDirPath()+"/"+Pfile));
					}
					QFileInfo fi(Pfile);
					QString target = QDir::cleanDirPath(QDir::convertSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile2 = pg.attribute("PFILE2","");
				if (!Pfile2.isEmpty())
				{
					if (static_cast<bool>(pg.attribute("relativePaths", "0").toInt()))
					{
						QFileInfo pfi2(QDir::cleanDirPath(QDir::convertSeparators(dir+"/"+Pfile2)));
						source = pfi2.absFilePath();
					}
					else
					{
						QFileInfo fi(Pfile2);
						source = QDir::cleanDirPath(QDir::convertSeparators(QDir::homeDirPath()+"/"+Pfile));
					}
					QFileInfo fi(Pfile2);
					QString target = QDir::cleanDirPath(QDir::convertSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
					copyFile(source, target);
					pg.setAttribute("PFILE2", fid.baseName() + "/" + fi.fileName());
				}
				QString Pfile3 = pg.attribute("PFILE3","");
				if (!Pfile3.isEmpty())
				{
					if (static_cast<bool>(pg.attribute("relativePaths", "0").toInt()))
					{
						QFileInfo pfi2(QDir::cleanDirPath(QDir::convertSeparators(dir+"/"+Pfile3)));
						source = pfi2.absFilePath();
					}
					else
					{
						QFileInfo fi(Pfile3);
						source = QDir::cleanDirPath(QDir::convertSeparators(QDir::homeDirPath()+"/"+Pfile3));
					}
					QFileInfo fi(Pfile3);
					QString target = QDir::cleanDirPath(QDir::convertSeparators(dir + "/" + fid.baseName() + "/" + fi.fileName()));
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
	Q3TextStream s;
	Q3CString cs = docu.toCString();
	s.setEncoding(Q3TextStream::UnicodeUTF8);
	s.setDevice(&f);
	s.writeRawBytes(cs.data(), cs.length());
	f.close();
}

void BibView::SaveContents(QString name, QString oldName)
{
	QDir d(oldName, "*.sce", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			Q3CString cf;
			if (!loadRawText(QDir::cleanDirPath(QDir::convertSeparators(oldName + "/" + d[dc])), cf))
				continue;
			QString ff = QString::fromUtf8(cf.data());
			QPixmap pm;
			QFileInfo fi(QDir::cleanDirPath(QDir::convertSeparators(oldName + "/" + d[dc])));
			QFileInfo fi2(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
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
			QFileInfo fi3(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d[dc])));
			pm.save(QDir::cleanDirPath(QDir::convertSeparators(fi3.dirPath()+"/"+fi3.baseName()+".png")), "PNG");
			checkAndChange(ff, QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d[dc])), QDir::cleanDirPath(QDir::convertSeparators(name)));
		}
	}
	QDir d2(oldName, "*.sml", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d2.exists()) && (d2.count() != 0))
	{
		for (uint dc = 0; dc < d2.count(); ++dc)
		{
			Q3CString cf;
			if (!loadRawText(QDir::cleanDirPath(QDir::convertSeparators(oldName + "/" + d2[dc])), cf))
				continue;
			QFile fil(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d2[dc])));
			if(!fil.open(QIODevice::WriteOnly))
				continue ;
			Q3TextStream s(&fil);
			s.writeRawBytes(cf.data(), cf.length());
			fil.close();
			QPixmap pm;
			QFileInfo fi(QDir::cleanDirPath(QDir::convertSeparators(oldName + "/" + d2[dc])));
			QFileInfo fi2(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
			else
			{
				QString f = QString::fromUtf8(cf.data());
				StencilReader *pre = new StencilReader();
				pm = pre->createPreview(f);
				delete pre;
			}
			QFileInfo fi3(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d2[dc])));
			pm.save(QDir::cleanDirPath(QDir::convertSeparators(fi3.dirPath()+"/"+fi3.baseName()+".png")), "PNG");
		}
	}
	QDir d3(name, "*.shape", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d3.exists()) && (d3.count() != 0))
	{
		for (uint dc = 0; dc < d3.count(); ++dc)
		{
			Q3CString cf;
			if (!loadRawText(QDir::cleanDirPath(QDir::convertSeparators(oldName + "/" + d3[dc])), cf))
				continue;
			QFile fil(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d3[dc])));
			if(!fil.open(QIODevice::WriteOnly))
				continue ;
			Q3TextStream s(&fil);
			s.writeRawBytes(cf.data(), cf.length());
			fil.close();
			QPixmap pm;
			QFileInfo fi(QDir::cleanDirPath(QDir::convertSeparators(oldName + "/" + d3[dc])));
			QFileInfo fi2(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
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
			QFileInfo fi3(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d3[dc])));
			pm.save(QDir::cleanDirPath(QDir::convertSeparators(fi3.dirPath()+"/"+fi3.baseName()+".png")), "PNG");
		}
	}
}

void BibView::ReadOldContents(QString name, QString newName)
{
	bool isUtf8 = false;
	QDomDocument docu("scridoc");
	QString ff;
	Q3CString cf;
	if (!loadRawText(name, cf))
		return;
	// these were corrupting the scrapbook entries, removed and works ok now, Riku
// 	if (f.startsWith("<SCRIBUSSCRAPUTF8"))
// 		ff = QString::fromUtf8(f);
// 	else
//		ff = f;
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
			QFile fi(QDir::cleanDirPath(QDir::convertSeparators(newName + "/" + GetAttr(&dc, "NAME") + ".sce")));
			if(!fi.open(QIODevice::WriteOnly))
				continue ;
			Q3TextStream s(&fi);
			QString fn = GetAttr(&dc, "DATA");
			cf = isUtf8? fn.utf8() : fn.local8Bit();
			s.writeRawBytes(cf.data(), cf.length());
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
//	QFileInfo fd(nd);
//	canWrite = fd.permission( QFileInfo::WriteUser );
// Above code is commented out because QFileInfo::permissons does not work properly
	QDir d(name, "*.sce", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			QPixmap pm;
			Q3CString cf;
			if (!loadRawText(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d[dc])), cf))
				continue;
			QFileInfo fi(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d[dc])));
			QFileInfo fi2(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
			else
			{
				QString f;
				if (cf.left(16) == "<SCRIBUSELEMUTF8")
					f = QString::fromUtf8(cf.data());
				else
					f = cf.data();
				ScPreview *pre = new ScPreview();
				pm = pre->createPreview(f);
//				if (canWrite)
// Above code is commented out because QFileInfo::permissons does not work properly
					pm.save(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")), "PNG");
				delete pre;
			}
			AddObj(fi.baseName(), QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d[dc])), pm);
		}
	}
	QDir d2(name, "*.sml", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d2.exists()) && (d2.count() != 0))
	{
		for (uint dc = 0; dc < d2.count(); ++dc)
		{
			QPixmap pm;
			Q3CString cf;
			if (!loadRawText(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d2[dc])), cf))
				continue;
			QFileInfo fi(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d2[dc])));
			QFileInfo fi2(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
			if (fi2.exists())
				pm.load(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
			else
			{
				QString f = QString::fromUtf8(cf.data());
				StencilReader *pre = new StencilReader();
				pm = pre->createPreview(f);
//				if (canWrite)
// Above code is commented out because QFileInfo::permissons does not work properly
					pm.save(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")), "PNG");
				delete pre;
			}
			AddObj(fi.baseName(), QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d2[dc])), pm);
		}
	}
	QDir d3(name, "*.shape", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
	if ((d3.exists()) && (d3.count() != 0))
	{
		for (uint dc = 0; dc < d3.count(); ++dc)
		{
			QPixmap pm;
			Q3CString cf;
			if (!loadRawText(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d3[dc])), cf))
				continue;
			QFileInfo fi(QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d3[dc])));
			QFileInfo fi2(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
			QFileInfo fi2p(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".xpm")));
			if ((fi2.exists()) || (fi2p.exists()))
			{
				if (fi2.exists())
					pm.load(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".png")));
				else
					pm.load(QDir::cleanDirPath(QDir::convertSeparators(fi.dirPath()+"/"+fi.baseName()+".xpm")));
			}
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
			AddObj(fi.baseName(), QDir::cleanDirPath(QDir::convertSeparators(name + "/" + d3[dc])), pm);
		}
	}
	QMap<QString,Elem>::Iterator itf;
	for (itf = objectMap.begin(); itf != objectMap.end(); ++itf)
	{
		(void) new Q3IconViewItem(this, itf.key(), itf.data().Preview);
	}
}

/* This is the main Dialog-Class for the Scrapbook */
Biblio::Biblio( QWidget* parent) : ScrPaletteBase( parent, "Sclib", false, 0 )
{
	resize( 230, 190 );
	BiblioLayout = new Q3VBoxLayout( this );
	BiblioLayout->setSpacing( 0 );
	BiblioLayout->setMargin( 0 );

	buttonLayout = new Q3HBoxLayout;
	buttonLayout->setSpacing( 5 );
	buttonLayout->setMargin( 0 );
	newButton = new QToolButton(this, "newButton" );
	newButton->setPixmap(loadIcon("16/document-new.png"));
	loadButton = new QToolButton(this, "loadButton" );
	loadButton->setPixmap(loadIcon("16/document-open.png"));
	saveAsButton = new QToolButton(this, "saveAsButton" );
	saveAsButton->setPixmap(loadIcon("16/document-save-as.png"));
	importButton = new QToolButton(this, "importButton" );
	importButton->setPixmap(loadIcon("compfile16.png"));
	closeButton = new QToolButton(this, "closeButton" );
	closeButton->setPixmap(loadIcon("16/close.png"));
	buttonLayout->addWidget( newButton );
	buttonLayout->addWidget( loadButton );
	buttonLayout->addWidget( saveAsButton );
	buttonLayout->addWidget( importButton );
	buttonLayout->addWidget( closeButton );
	QSpacerItem* spacer = new QSpacerItem( 16, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
	buttonLayout->addItem( spacer );
	BiblioLayout->addLayout( buttonLayout );


//	Frame3 = new QTabWidget( this, "Frame3" );
	Frame3 = new QToolBox( this, "Frame3" );
	activeBView = new BibView(this);
//	Frame3->addTab(activeBView, tr("Main"));
	Frame3->addItem(activeBView, tr("Main"));
	tempBView = new BibView(this);
//	Frame3->addTab(tempBView, tr("Copied Items"));
	Frame3->addItem(tempBView, tr("Copied Items"));
	tempCount = 0;
	BiblioLayout->addWidget( Frame3 );

	languageChange();

	connect(newButton, SIGNAL(clicked()), this, SLOT(NewLib()));
	connect(loadButton, SIGNAL(clicked()), this, SLOT(Load()));
	connect(saveAsButton, SIGNAL(clicked()), this, SLOT(SaveAs()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(Import()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(closeLib()));
	connect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
	connect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
	connect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
//	connect(Frame3, SIGNAL(currentChanged(QWidget*)), this, SLOT(libChanged(QWidget* )));
	connect(Frame3, SIGNAL(currentChanged(int)), this, SLOT(libChanged(int )));
}

void Biblio::setOpenScrapbooks(QStringList &fileNames)
{
	disconnect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
	disconnect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
	disconnect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
	for (uint rd = 0; rd < fileNames.count(); ++rd)
	{
		QString fileName = fileNames[rd];
		if (!fileName.isEmpty())
		{
			QDir d(fileName);
			activeBView = new BibView(this);
/*			if (!activeBView->canWrite)
				Frame3->addTab(activeBView, loadIcon("spot.png"), d.dirName());
			else */
// Above code is commented out because QFileInfo::permissons does not work properly
//			Frame3->addTab(activeBView, d.dirName());
			Frame3->addItem(activeBView, d.dirName());
			activeBView->ReadContents(fileName);
			activeBView->ScFilename = fileName;
		}
	}
//	activeBView = (BibView*)Frame3->page(0);
//	Frame3->setCurrentPage(0);
	activeBView = (BibView*)Frame3->item(0);
	Frame3->setCurrentIndex(0);
	connect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
	connect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
	connect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
}

QStringList Biblio::getOpenScrapbooks()
{
	QStringList ret;
	ret.clear();
	if (Frame3->count() > 2) // omit the first 2 Tabs since they contain the main and temp scrapbook
	{
		for (int a = 2; a < Frame3->count(); a++)
		{
//			BibView* bv = (BibView*)Frame3->page(a);
			BibView* bv = (BibView*)Frame3->item(a);
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

void Biblio::installEventFilter(const QObject *filterObj)
{
	ScrPaletteBase::installEventFilter(filterObj);
	activeBView->installEventFilter(filterObj);
	tempBView->installEventFilter(filterObj);
}

void Biblio::NewLib()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString fileName = Q3FileDialog::getExistingDirectory("", this, "d", tr("Choose a Scrapbook Directory"), true);
	if (!fileName.isEmpty())
	{
		for (int a = 0; a < Frame3->count(); a++)
		{
//			BibView* bv = (BibView*)Frame3->page(a);
			BibView* bv = (BibView*)Frame3->item(a);
			if (fileName == bv->ScFilename)
				return;
		}
		disconnect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
		disconnect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
		disconnect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
		QDir d(fileName);
		activeBView = new BibView(this);
//		Frame3->addTab(activeBView, d.dirName());
		Frame3->addItem(activeBView, d.dirName());
		activeBView->ScFilename = fileName;
//		Frame3->showPage(activeBView);
		Frame3->setCurrentItem(activeBView);
		d.cdUp();
		dirs->set("scrap_load", d.absPath());
		connect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
		connect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
		connect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
	}
}

void Biblio::Load()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString fileName = Q3FileDialog::getExistingDirectory(dirs->get("scrap_load", "."), this, "d", tr("Choose a Scrapbook Directory"), true);
	if (!fileName.isEmpty())
	{
		for (int a = 0; a < Frame3->count(); a++)
		{
//			BibView* bv = (BibView*)Frame3->page(a);
			BibView* bv = (BibView*)Frame3->item(a);
			if (fileName == bv->ScFilename)
				return;
		}
		disconnect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
		disconnect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
		disconnect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
		QDir d(fileName);
		activeBView = new BibView(this);
/*		if (!activeBView->canWrite)
			Frame3->addTab(activeBView, loadIcon("spot.png"), d.dirName());
		else */
// Above code is commented out because QFileInfo::permissons does not work properly
//		Frame3->addTab(activeBView, d.dirName());
		Frame3->addItem(activeBView, d.dirName());
		activeBView->ReadContents(fileName);
		activeBView->ScFilename = fileName;
//		Frame3->showPage(activeBView);
		Frame3->setCurrentItem(activeBView);
		d.cdUp();
		dirs->set("scrap_load", d.absPath());
		connect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
		connect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
		connect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
	}
}

void Biblio::Import()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString s = Q3FileDialog::getOpenFileName(dirs->get("old_scrap_load", "."),
	                                         tr("Scrapbook (*.scs)"),
	                                         this,
	                                         "open file dialog",
	                                         tr("Choose a scrapbook file to import"));
	
	if (!s.isEmpty())
	{
		dirs->set("old_scrap_load", s.left(s.findRev(QDir::convertSeparators("/"))));
		
		QString scrapbookFileO = s;
		QFileInfo scrapbookFileInfoO = QFileInfo(scrapbookFileO);
		if (scrapbookFileInfoO.exists())
		{
			readOldContents(scrapbookFileO, activeBView->ScFilename);
			readContents(activeBView->ScFilename);
		}
	}
}

void Biblio::SaveAs()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString fn = Q3FileDialog::getExistingDirectory(dirs->get("scrap_saveas", "."), this, "d", tr("Choose a Directory"), true);
	if (!fn.isEmpty())
	{
		for (int a = 0; a < Frame3->count(); a++)
		{
//			BibView* bv = (BibView*)Frame3->page(a);
			BibView* bv = (BibView*)Frame3->item(a);
			if (fn == bv->ScFilename)
				return;
		}
		QDir d(fn);
		dirs->set("scrap_saveas", fn);
		activeBView->SaveContents(fn, activeBView->ScFilename);
//		activeBView->ScFilename = fn;
//		Frame3->changeTab(activeBView, d.dirName());
		d.cdUp();
		dirs->set("scrap_saveas", d.absPath());
	}
}

void Biblio::closeLib()
{
	if (Frame3->count() == 2)
		close();
//	if ((Frame3->currentPageIndex() == 0) || (Frame3->currentPageIndex() == 1))
	if ((Frame3->currentIndex() == 0) || (Frame3->currentIndex() == 1))
		return;
	else
	{
		disconnect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
		disconnect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
		disconnect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
//		Frame3->removePage(activeBView);
		Frame3->removeItem(activeBView);
//		delete activeBView;   currently disabled as the whole TabWidget vanishes when executing that delete?????
//		activeBView = (BibView*)Frame3->page(0);
//		Frame3->setCurrentPage(0);
		activeBView = (BibView*)Frame3->item(0);
		Frame3->setCurrentIndex(0);
		connect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
		connect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
		connect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
	}
}

// void Biblio::libChanged(QWidget *lib)
void Biblio::libChanged(int index)
{
	disconnect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
	disconnect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
	disconnect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
//	activeBView = (BibView*)lib;
	activeBView = (BibView*)Frame3->item(index);
	connect(activeBView, SIGNAL(dropped(QDropEvent *, const Q3ValueList<Q3IconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
	connect(activeBView, SIGNAL(mouseButtonClicked(int, Q3IconViewItem*, const QPoint &)), this, SLOT(HandleMouse(int, Q3IconViewItem*)));
	connect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
}

void Biblio::HandleMouse(int button, Q3IconViewItem *ite)
{
	int mret, del, ren;
	if ((ite != 0) && (button == Qt::RightButton))
	{
		Q3PopupMenu *pmenu = new Q3PopupMenu();
		Q3PopupMenu *pmenu2 = new Q3PopupMenu();
		Q3PopupMenu *pmenu3 = new Q3PopupMenu();
		for (int a = 0; a < Frame3->count(); a++)
		{
//			BibView* bv = (BibView*)Frame3->page(a);
			BibView* bv = (BibView*)Frame3->item(a);
			if (bv != activeBView)
			{
//				pmenu2->insertItem(Frame3->tabLabel(Frame3->page(a)), a);
//				pmenu3->insertItem(Frame3->tabLabel(Frame3->page(a)), a);
				pmenu2->insertItem(Frame3->itemLabel(Frame3->indexOf(Frame3->item(a))), a);
				pmenu3->insertItem(Frame3->itemLabel(Frame3->indexOf(Frame3->item(a))), a);
			}
		}
		connect(pmenu2, SIGNAL(activated(int)), this, SLOT(copyObj(int)));
		connect(pmenu3, SIGNAL(activated(int)), this, SLOT(moveObj(int)));
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		ren = pmenu->insertItem( tr("Rename"));
		del = pmenu->insertItem( tr("Delete"));
		pmenu->insertItem( tr("Copy To:"), pmenu2);
		pmenu->insertItem( tr("Move To:"), pmenu3);
		mret = pmenu->exec(QCursor::pos());
		if (mret == del)
			DeleteObj(ite->text(), ite);
		if (mret == ren)
		{
			ite->setRenameEnabled(true);
			OldName = ite->text();
			ite->rename();
			ite->setRenameEnabled(false);
		}
		delete pmenu;
	}
	activeBView->clearSelection();
}

bool Biblio::copyObj(int id)
{
	Q3IconViewItem *ite = activeBView->currentItem();
	QString nam = ite->text();
//	BibView* bv = (BibView*)Frame3->page(id);
	BibView* bv = (BibView*)Frame3->item(id);
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
	Q3CString cf;
	if (!loadRawText(QDir::cleanDirPath(activeBView->objectMap[ite->text()].Data), cf))
		return false;
	pm = activeBView->objectMap[ite->text()].Preview;
	QString dt = activeBView->objectMap[ite->text()].Data;
	QFileInfo fi(dt);
	QFile f(QDir::cleanDirPath(QDir::convertSeparators(bv->ScFilename + "/" + nam + "." + fi.extension(true).lower())));
	if(!f.open(QIODevice::WriteOnly))
		return false;
	Q3TextStream s;
	s.setEncoding(Q3TextStream::UnicodeUTF8);
	s.setDevice(&f);
	s.writeRawBytes(cf.data(), cf.length());
	f.close();
	bv->AddObj(nam, QDir::cleanDirPath(QDir::convertSeparators(bv->ScFilename + "/" + nam + "." + fi.extension(true).lower())), pm);
	pm.save(QDir::cleanDirPath(QDir::convertSeparators(bv->ScFilename + "/" + nam +".png")), "PNG");
	QFileInfo fiD(QDir::convertSeparators(activeBView->ScFilename + "/" + fi.baseName()));
	if ((fiD.exists()) && (fiD.isDir()))
	{
		QDir dd = QDir(QDir::cleanDirPath(QDir::convertSeparators(bv->ScFilename)));
		dd.mkdir(QDir::cleanDirPath(QDir::convertSeparators(bv->ScFilename + "/" + nam)));
		QDir d(QDir::convertSeparators(activeBView->ScFilename + "/" + fi.baseName()), "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((d.exists()) && (d.count() != 0))
		{
			for (uint dc = 0; dc < d.count(); ++dc)
			{
				QString source = QDir::cleanDirPath(QDir::convertSeparators(activeBView->ScFilename + "/" + fi.baseName() + "/" + d[dc]));
				QString target = QDir::cleanDirPath(QDir::convertSeparators(bv->ScFilename + "/" + nam + "/" + d[dc]));
				copyFile(source, target);
			}
		}
		if (fiD.baseName() != nam)
			adjustReferences(QDir::convertSeparators(bv->ScFilename + "/" + nam + "." + fi.extension(true).lower()));
	}
	(void) new Q3IconViewItem(bv, nam, pm);
	if (bv == tempBView)
	{
		tempCount++;
		if (tempBView->objectMap.count() > static_cast<uint>(PrefsManager::instance()->appPrefs.numScrapbookCopies))
		{
			QMap<QString,BibView::Elem>::Iterator it;
			it = tempBView->objectMap.begin();
			QFile f(it.data().Data);
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
			tempBView->objectMap.remove(it);
			Q3IconViewItem* ite = tempBView->firstItem();
			if (ite != 0)
				delete ite;
			tempBView->sort(activeBView->sortDirection());
			tempBView->arrangeItemsInGrid(true);
		}
		emit updateRecentMenue();
	}
	return true;
}

void Biblio::moveObj(int id)
{
	if (copyObj(id))
	{
		Q3IconViewItem *ite = activeBView->currentItem();
		DeleteObj(ite->text(), ite);
	}
}

void Biblio::DeleteObj(QString name, Q3IconViewItem *ite)
{
/*	if (!activeBView->canWrite)
		return; */
// Above code is commented out because QFileInfo::permissons does not work properly
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
	delete ite;
	activeBView->sort(activeBView->sortDirection());
	activeBView->arrangeItemsInGrid(true);
	if (activeBView == tempBView)
		emit updateRecentMenue();
}

void Biblio::ItemRenamed(Q3IconViewItem *ite)
{
/*	if (!activeBView->canWrite)
		return; */
// Above code is commented out because QFileInfo::permissons does not work properly
	QString ObjData;
	QPixmap ObjPreview;
	disconnect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
	if (OldName != ite->text())
	{
		if (activeBView->objectMap.contains(ite->text()))
		{
			QMessageBox::warning(this, CommonStrings::trWarning, tr("Name \"%1\" is not unique.\nPlease choose another.").arg(ite->text()), CommonStrings::tr_OK);
			ite->setText(OldName);
		}
		else
		{
			ObjData = activeBView->objectMap[OldName].Data;
			ObjPreview = activeBView->objectMap[OldName].Preview;
			QDir d = QDir();
			d.rename(ObjData, QDir::cleanDirPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")));
			QFileInfo fi(QDir::convertSeparators(activeBView->ScFilename + "/" + OldName + ".png"));
			if (fi.exists())
				d.rename(QDir::convertSeparators(activeBView->ScFilename + "/" + OldName + ".png"), QDir::cleanDirPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text() + ".png")));
			QFileInfo fiD(QDir::convertSeparators(activeBView->ScFilename + "/" + OldName));
			if ((fiD.exists()) && (fiD.isDir()))
			{
				QDir d = QDir();
				d.rename(QDir::convertSeparators(activeBView->ScFilename + "/" + OldName), QDir::cleanDirPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text())));
				adjustReferences(QDir::cleanDirPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")));
			}
			activeBView->objectMap.remove(OldName);
			activeBView->AddObj(ite->text(), QDir::cleanDirPath(QDir::convertSeparators(activeBView->ScFilename + "/" + ite->text() + ".sce")), ObjPreview);
			activeBView->sort(activeBView->sortDirection());
			activeBView->arrangeItemsInGrid(true);
			if (activeBView == tempBView)
				emit updateRecentMenue();
		}
	}
	connect(activeBView, SIGNAL(itemRenamed(Q3IconViewItem*)), this, SLOT(ItemRenamed(Q3IconViewItem*)));
}

void Biblio::adjustReferences(QString nam)
{
	Q3CString cf;
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
		Q3TextStream s;
		Q3CString cs = docu.toCString();
		s.setEncoding(Q3TextStream::UnicodeUTF8);
		s.setDevice(&fl);
		s.writeRawBytes(cs.data(), cs.length());
		fl.close();
	}
}

void Biblio::DropOn(QDropEvent *e)
{
	bool img;
	QString text, nam, tmp = "";
	if (Q3TextDrag::decode(e, text))
	{
		Q3Url ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).lower();
		img = ((ext=="eps")||(ext=="epsi")||(ext=="ps")||(ext=="png")||(ext=="gif")||(ext=="jpg")||(ext=="xpm"));
		if ((fi.exists()) && (!img))
		{
			Q3CString rawText;
			if (loadRawText(ur.path(), rawText))
			{
				if (rawText.left(16) == "<SCRIBUSELEMUTF8")
					tmp = QString::fromUtf8(rawText.data());
				else if (rawText.left(13) == "<SCRIBUSELEM>")
					tmp = rawText;
			}
		}
		else
		{
			if (text.startsWith("<SCRIBUSELEM"))
			{
				tmp = text;
			}
		}
		text = tmp;
		ObjFromMenu(text);
	}
}

void Biblio::ObjFromMenu(QString text)
{
	QString nam, tmp;
/*	if (!activeBView->canWrite)
		return; */
// Above code is commented out because QFileInfo::permissons does not work properly
//	if (Frame3->currentPageIndex() == 1)
	if (Frame3->currentIndex() == 1)
	{
		nam = tr("Object") + tmp.setNum(tempCount);
		tempCount++;
	}
	else
		nam = tr("Object") + tmp.setNum(activeBView->objectMap.count());
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
	QFile f(QDir::cleanDirPath(QDir::convertSeparators(activeBView->ScFilename + "/" + nam + ".sce")));
	if(!f.open(QIODevice::WriteOnly))
		return ;
	Q3TextStream s;
	Q3CString cs = ff.utf8();
	s.setEncoding(Q3TextStream::UnicodeUTF8);
	s.setDevice(&f);
	s.writeRawBytes(cs.data(), cs.length());
	f.close();
	ScPreview *pre = new ScPreview();
	QPixmap pm = pre->createPreview(ff);
	activeBView->AddObj(nam, QDir::cleanDirPath(QDir::convertSeparators(activeBView->ScFilename + "/" + nam + ".sce")), pm);
	pm.save(QDir::cleanDirPath(QDir::convertSeparators(activeBView->ScFilename + "/" + nam +".png")), "PNG");
	(void) new Q3IconViewItem(activeBView, nam, pm);
	delete pre;
//	if (Frame3->currentPageIndex() == 1)
	if (Frame3->currentIndex() == 1)
	{
		if (tempBView->objectMap.count() > static_cast<uint>(PrefsManager::instance()->appPrefs.numScrapbookCopies))
		{
			QMap<QString,BibView::Elem>::Iterator it;
			it = tempBView->objectMap.begin();
			QFile f(it.data().Data);
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
			tempBView->objectMap.remove(it);
			Q3IconViewItem* ite = tempBView->firstItem();
			if (ite != 0)
				delete ite;
			tempBView->sort(activeBView->sortDirection());
			tempBView->arrangeItemsInGrid(true);
		}
		emit updateRecentMenue();
	}
}

void Biblio::ObjFromCopyAction(QString text)
{
	QString nam, tmp;
	nam = tr("Object") + tmp.setNum(tempCount);
	tempCount++;
	QString ff = text;
	QFile f(QDir::cleanDirPath(QDir::convertSeparators(tempBView->ScFilename + "/" + nam + ".sce")));
	if(!f.open(QIODevice::WriteOnly))
		return ;
	Q3TextStream s;
	Q3CString cs = ff.utf8();
	s.setEncoding(Q3TextStream::UnicodeUTF8);
	s.setDevice(&f);
	s.writeRawBytes(cs.data(), cs.length());
	f.close();
	ScPreview *pre = new ScPreview();
	QPixmap pm = pre->createPreview(ff);
	tempBView->AddObj(nam, QDir::cleanDirPath(QDir::convertSeparators(tempBView->ScFilename + "/" + nam + ".sce")), pm);
	pm.save(QDir::cleanDirPath(QDir::convertSeparators(tempBView->ScFilename + "/" + nam +".png")), "PNG");
	(void) new Q3IconViewItem(tempBView, nam, pm);
	delete pre;
	if (tempBView->objectMap.count() > static_cast<uint>(PrefsManager::instance()->appPrefs.numScrapbookCopies))
	{
		QMap<QString,BibView::Elem>::Iterator it;
		it = tempBView->objectMap.begin();
		QFile f(it.data().Data);
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
		tempBView->objectMap.remove(it);
		Q3IconViewItem* ite = tempBView->firstItem();
		if (ite != 0)
			delete ite;
		tempBView->sort(tempBView->sortDirection());
		tempBView->arrangeItemsInGrid(true);
	}
}

void Biblio::CleanUpTemp()
{
	QMap<QString,BibView::Elem>::Iterator it;
	for (it = tempBView->objectMap.begin(); it != tempBView->objectMap.end(); ++it)
	{
		QFile f(it.data().Data);
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

void Biblio::languageChange()
{
	setCaption( tr( "Scrapbook" ) );
 	QToolTip::add( newButton, tr( "Create a new scrapbook page" ) );
 	QToolTip::add( loadButton, tr( "Load an existing scrapbook" ) );
 	QToolTip::add( saveAsButton, tr( "Save the selected scrapbook" ) );
 	QToolTip::add( importButton, tr( "Import an scrapbook file from Scribus <=1.3.2" ) );
 	QToolTip::add( closeButton, tr( "Close the selected scrapbook" ) );
}
