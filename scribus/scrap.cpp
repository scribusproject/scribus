/****************************************************************************
** Form implementation generated from reading ui file 'Bib.ui'
**
** Created: Sun Oct 14 19:47:56 2001
**
****************************************************************************/
#include "scrap.h"
#include "scrap.moc"
#include <qfileinfo.h>
#include <qfile.h>
#include <qurl.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qcursor.h>
#include "query.h"
#include "scpreview.h"
#include "prefsfile.h"
#include "util.h"
#include "commonstrings.h"
#include "prefsmanager.h"
//CB TODO bring in the prefsManager instance locally here too

/* The Scrapbook View Class
 * inherited from QIconView */
BibView::BibView(QWidget* parent) : QIconView(parent)
{
	objectMap.clear();
}

void BibView::keyPressEvent(QKeyEvent *k)
{
	//Why doesnt this widget send Escape to the QDialog? Lets make Escape work for now anyway.
	if (k->key()==Key_Escape && k->state() == 0)
	{
		QFrame *f=dynamic_cast<QFrame *>(parent());
		if (f)
		{
			QDialog *d=dynamic_cast<QDialog *>(f->parent());
			if (d)
				d->close();
		}
	}
	else
		QIconView::keyPressEvent(k);
}

QDragObject *BibView::dragObject()
{
	QString dt = objectMap[currentItem()->text()].Data.utf8();
	QDragObject *dr = new QTextDrag(dt, this);
	dr->setPixmap(loadIcon("DragPix.xpm"));
	return dr;
}

void BibView::AddObj(QString name, QString daten, QPixmap Bild)
{
	struct Elem DrElem;
	DrElem.Data = daten;
	DrElem.Preview = Bild;
	objectMap.insert(name, DrElem);
}

void BibView::SaveContents(QString name)
{
	QDomDocument docu("scribus");
	QString st="<SCRIBUSSCRAPUTF8></SCRIBUSSCRAPUTF8>";
	docu.setContent(st);
	QDomElement ele=docu.documentElement();
	for (QMap<QString,Elem>::Iterator itf = objectMap.begin(); itf != objectMap.end(); ++itf)
	{
		QDomElement dc=docu.createElement("OBJEKT");
		dc.setAttribute("NAME",itf.key());
		dc.setAttribute("DATA",itf.data().Data);
		ele.appendChild(dc);
	}
	QFile f(name);
	if(!f.open(IO_WriteOnly))
		return ;
	QTextStream s(&f);
	QString wr = docu.toString().utf8();
	s.writeRawBytes(wr, wr.length());
	f.close();
}

void BibView::ReadContents(QString name)
{
	QDomDocument docu("scridoc");
	QString f = "";
	if (!loadText(name, &f))
		return;
	QString ff = "";
	if (f.startsWith("<SCRIBUSSCRAPUTF8"))
		ff = QString::fromUtf8(f);
	else
		ff = f;
	if(!docu.setContent(ff))
		return;
	QDomElement elem=docu.documentElement();
	if ((elem.tagName() != "SCRIBUSSCRAP") && (elem.tagName() != "SCRIBUSSCRAPUTF8"))
		return;
	clear();
	objectMap.clear();
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		if (dc.tagName()=="OBJEKT")
		{
			ScPreview *pre = new ScPreview();
			AddObj(GetAttr(&dc, "NAME"), GetAttr(&dc, "DATA"), pre->createPreview(GetAttr(&dc, "DATA")));
			delete pre;
		}
		DOC=DOC.nextSibling();
	}
	QMap<QString,Elem>::Iterator itf;
	for (itf = objectMap.begin(); itf != objectMap.end(); ++itf)
	{
		(void) new QIconViewItem(this, itf.key(), itf.data().Preview);
	}
}

void BibView::RebuildView()
{
	clear();
	QMap<QString,Elem>::Iterator itf;
	for (itf = objectMap.begin(); itf != objectMap.end(); ++itf)
	{
		ScPreview *pre = new ScPreview();
		itf.data().Preview = pre->createPreview(itf.data().Data);
		(void) new QIconViewItem(this, itf.key(), itf.data().Preview);
		delete pre;
	}
}

/* This is the main Dialog-Class for the Scrapbook */
Biblio::Biblio( QWidget* parent) : ScrPaletteBase( parent, "Sclib", false, 0 )
{
	resize( 230, 190 );
	setIcon(loadIcon("AppIcon.png"));
	ScFilename = "";
	Changed = false;
	BiblioLayout = new QVBoxLayout( this );
	BiblioLayout->setSpacing( 0 );
	BiblioLayout->setMargin( 0 );
	fmenu = new QPopupMenu();
	fNew = fmenu->insertItem(loadIcon("DateiNeu16.png"), "", this, SLOT(NewLib()), CTRL+Key_N);
	fLoad = fmenu->insertItem(loadIcon("DateiOpen16.png"), "", this, SLOT(Load()), CTRL+Key_O);
	fSave = fmenu->insertItem(loadIcon("DateiSave16.png"), "", this, SLOT(Save()), CTRL+Key_S);
	fSaveAs = fmenu->insertItem( "", this, SLOT(SaveAs()));
	fClose = fmenu->insertItem(loadIcon("DateiClos16.png"), "", this, SLOT(close()));
	vmenu = new QPopupMenu();
	vSmall = vmenu->insertItem( "" );
	vMedium = vmenu->insertItem( "" );
	vLarge = vmenu->insertItem( "" );
	switch (PrefsManager::instance()->appPrefs.PSize)
	{
	case 40:
		vmenu->setItemChecked(vSmall, true);
		break;
	case 60:
		vmenu->setItemChecked(vMedium, true);
		break;
	case 80:
		vmenu->setItemChecked(vLarge, true);
		break;
	}
	menuBar = new QMenuBar(this);
	mFile=menuBar->insertItem( "", fmenu);
	mView=menuBar->insertItem( "", vmenu);
	BiblioLayout->setMenuBar( menuBar );

	Frame3 = new QFrame( this, "Frame3" );
	Frame3->setFrameShape( QFrame::Box );
	Frame3->setFrameShadow( QFrame::Sunken );
	Frame3Layout = new QVBoxLayout( Frame3 );
	Frame3Layout->setSpacing( 6 );
	Frame3Layout->setMargin( 11 );

	BibWin = new BibView(Frame3);
	BibWin->setAutoArrange(true);
	BibWin->setSorting(true);
	BibWin->setResizeMode(QIconView::Adjust);
	Frame3Layout->addWidget( BibWin );
	BiblioLayout->addWidget( Frame3 );
	languageChange();
	connect(BibWin, SIGNAL(dropped(QDropEvent *, const QValueList<QIconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
	connect(BibWin, SIGNAL(rightButtonClicked(QIconViewItem*, const QPoint &)), this, SLOT(HandleMouse(QIconViewItem*)));
	connect(BibWin, SIGNAL(itemRenamed(QIconViewItem*)), this, SLOT(ItemRenamed(QIconViewItem*)));
	connect(vmenu, SIGNAL(activated(int)), this, SLOT(SetPreview(int)));
}

void Biblio::setScrapbookFileName(QString fileName)
{
	ScFilename=fileName;
}

const QString Biblio::getScrapbookFileName()
{
	return ScFilename;
}

const bool Biblio::changed()
{
	return Changed;
}

const int Biblio::objectCount()
{
	return BibWin->objectMap.count();
}

void Biblio::rebuildView()
{
	BibWin->RebuildView();	
}

void Biblio::readContents(QString fileName)
{
	BibWin->ReadContents(fileName);
}

void Biblio::installEventFilter(const QObject *filterObj)
{
	ScrPaletteBase::installEventFilter(filterObj);
	BibWin->installEventFilter(filterObj);
}


void Biblio::Save()
{
	if ((!ScFilename.isEmpty()) && (BibWin->objectMap.count() != 0))
	{
		BibWin->SaveContents(ScFilename);
		Changed = false;
	}
}

void Biblio::SaveAs()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString fn = QFileDialog::getSaveFileName(dirs->get("scrap_saveas", "."),
	                                          tr("Scrapbooks (*.scs);;All Files (*)"), this);
	if (!fn.isEmpty())
	{
		QString saveFileName;
		if (fn.endsWith(".scs"))
			saveFileName = fn;
		else
			saveFileName = fn+".scs";
		dirs->set("scrap_saveas", saveFileName.left(saveFileName.findRev("/")));
		BibWin->SaveContents(saveFileName);
		ScFilename = saveFileName;
		setCaption(saveFileName);
		fmenu->setItemEnabled(fSave, 1);
		Changed = false;
	}
}

void Biblio::Load()
{
	Save();
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString fileName = QFileDialog::getOpenFileName(dirs->get("scrap_load", "."),
	                                                tr("Scrapbooks (*.scs);;All Files (*)"),this);
	if (!fileName.isEmpty())
	{
		dirs->set("scrap_load", fileName.left(fileName.findRev("/")));
		BibWin->ReadContents(fileName);
		ScFilename = fileName;
		setCaption(fileName);
		Changed = false;
	}
}

void Biblio::SetPreview(int id)
{
	int a = vmenu->indexOf(id);
	switch (a)
	{
	case 0:
		PrefsManager::instance()->appPrefs.PSize = 40;
		break;
	case 1:
		PrefsManager::instance()->appPrefs.PSize = 60;
		break;
	case 2:
		PrefsManager::instance()->appPrefs.PSize = 80;
		break;
	}
	AdjustMenu();
	BibWin->RebuildView();
}

void Biblio::AdjustMenu()
{
	vmenu->setItemChecked(vSmall, false);
	vmenu->setItemChecked(vMedium, false);
	vmenu->setItemChecked(vLarge, false);
	switch (PrefsManager::instance()->appPrefs.PSize)
	{
	case 40:
		vmenu->setItemChecked(vSmall, true);
		break;
	case 60:
		vmenu->setItemChecked(vMedium, true);
		break;
	case 80:
		vmenu->setItemChecked(vLarge, true);
		break;
	}
}

void Biblio::HandleMouse(QIconViewItem *ite)
{
	int mret, del, ren;
	if (ite != 0)
	{
		QPopupMenu *pmenu = new QPopupMenu();
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		ren = pmenu->insertItem( tr("Rename"));
		del = pmenu->insertItem( tr("Delete"));
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
}

void Biblio::NewLib()
{
	Save();
	BibWin->objectMap.clear();
	BibWin->clear();
	ScFilename = "";
	setCaption( tr("Scrapbook"));
	fmenu->setItemEnabled(fSave, 0);
}

void Biblio::DeleteObj(QString name, QIconViewItem *ite)
{
	BibWin->objectMap.remove(name);
	delete ite;
	BibWin->sort(BibWin->sortDirection());
	BibWin->arrangeItemsInGrid(true);
	Changed = true;
	if (PrefsManager::instance()->appPrefs.SaveAtQ)
		Save();
}

void Biblio::ItemRenamed(QIconViewItem *ite)
{
	QString ObjData;
	QPixmap ObjPreview;
	disconnect(BibWin, SIGNAL(itemRenamed(QIconViewItem*)), this, SLOT(ItemRenamed(QIconViewItem*)));
	if (OldName != ite->text())
	{
		if (BibWin->objectMap.contains(ite->text()))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Name \"%1\" isn't unique.\nPlease choose another.").arg(ite->text()), CommonStrings::tr_OK);
			ite->setText(OldName);
		}
		else
		{
			ObjData = BibWin->objectMap[OldName].Data;
			ObjPreview = BibWin->objectMap[OldName].Preview;
			BibWin->objectMap.remove(OldName);
			BibWin->AddObj(ite->text(), ObjData, ObjPreview);
			BibWin->sort(BibWin->sortDirection());
			BibWin->arrangeItemsInGrid(true);
			Changed = true;
			if (PrefsManager::instance()->appPrefs.SaveAtQ)
				Save();
		}
	}
	connect(BibWin, SIGNAL(itemRenamed(QIconViewItem*)), this, SLOT(ItemRenamed(QIconViewItem*)));
}

void Biblio::DropOn(QDropEvent *e)
{
	QString text, tmp, nam;
	bool img;
	tmp = "";
	if (QTextDrag::decode(e, text))
	{
		QUrl ur(text);
		QFileInfo fi = QFileInfo(ur.path());
		QString ext = fi.extension(false).lower();
		img = ((ext=="eps")||(ext=="ps")||(ext=="png")||(ext=="gif")||(ext=="jpg")||(ext=="xpm"));
		if ((fi.exists()) && (!img))
		{
			if (loadText(ur.path(), &text))
			{
				if (text.startsWith("<SCRIBUSELEM>"))
				{
					tmp = text;
				}
			}
		}
		else
		{
			if (text.startsWith("<SCRIBUSELEM>"))
			{
				tmp = text;
			}
		}
		ObjFromMenu(text);
		if (PrefsManager::instance()->appPrefs.SaveAtQ)
			Save();
	}
}

void Biblio::ObjFromMenu(QString text)
{
	QString nam, tmp;
	nam = tr("Object") + tmp.setNum(BibWin->objectMap.count());
	Query *dia = new Query(this, "tt", 1, 0, tr("&Name:"), tr("New Entry"));
	dia->setEditText(nam, true);
	if (dia->exec())
	{
		nam = dia->getEditText();
		while (BibWin->objectMap.contains(nam))
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
	tmp = text;
	QString ff = "";
	if (tmp.startsWith("<SCRIBUSELEMUTF8"))
		ff = QString::fromUtf8(tmp);
	else
		ff = tmp;
	ScPreview *pre = new ScPreview();
	QPixmap pm = pre->createPreview(ff);
	BibWin->AddObj(nam, ff, pm);
	(void) new QIconViewItem(BibWin, nam, pm);
	Changed = true;
	delete pre;
}

void Biblio::languageChange()
{
	setCaption( tr( "Scrapbook" ) );
	menuBar->changeItem( mFile, tr("&File"));
	menuBar->changeItem( mView, tr("&Preview"));

	fmenu->changeItem(fNew, tr("&New"));
	fmenu->changeItem(fLoad, tr("&Load..."));
	fmenu->changeItem(fSave, tr("&Save"));
	fmenu->changeItem(fSaveAs, tr("Save &As..."));
	fmenu->changeItem(fClose, tr("&Close"));
	vmenu->changeItem(vSmall, tr("&Small" ));
	vmenu->changeItem(vMedium, tr("&Medium" ));
	vmenu->changeItem(vLarge, tr("&Large" ));
}
