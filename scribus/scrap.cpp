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
extern QPixmap loadIcon(QString nam);
extern bool loadText(QString nam, QString *Buffer);
extern QString GetAttr(QDomElement *el, QString at, QString def="0");
extern PrefsFile* prefsFile;

/* The Scrapbook View Class
 * inherited from QIconView */
BibView::BibView(QWidget* parent, ApplicationPrefs *prefs) : QIconView(parent)
{
	Objekte.clear();
	Prefs = prefs;
}

void BibView::keyPressEvent(QKeyEvent *k)
{
	/*
	int KeyMod;
	int kk = k->key();
	switch (k->state())
	{
	case ShiftButton:
		KeyMod = 0x00200000;
		break;
	case AltButton:
		KeyMod = 0x00800000;
		break;
	case ControlButton:
		KeyMod = 0x00400000;
		break;
	default:
		KeyMod = 0;
		break;
	}
	if (kk == Key_F10)
		emit ToggleAllPalettes();
	// CB TODO
	//if ((kk + KeyMod) == Prefs->KeyActions[46].KeyID)
	//	emit CloseMpal();
	//if ((kk + KeyMod) == Prefs->KeyActions[47].KeyID)
	//	emit CloseTpal();
	//if ((kk + KeyMod) == Prefs->KeyActions[48].KeyID)
	//	emit Schliessen();
	
	QIconView::keyPressEvent(k);
	*/	
}

QDragObject *BibView::dragObject()
{
	QString dt = Objekte[currentItem()->text()].Data.utf8();
	QDragObject *dr = new QTextDrag(dt, this);
	dr->setPixmap(loadIcon("DragPix.xpm"));
	return dr;
}

void BibView::AddObj(QString name, QString daten, QPixmap Bild)
{
	struct Elem DrElem;
	DrElem.Data = daten;
	DrElem.Preview = Bild;
	Objekte.insert(name, DrElem);
}

void BibView::SaveContents(QString name)
{
	QDomDocument docu("scribus");
	QString st="<SCRIBUSSCRAPUTF8></SCRIBUSSCRAPUTF8>";
	docu.setContent(st);
	QDomElement ele=docu.documentElement();
	for (QMap<QString,Elem>::Iterator itf = Objekte.begin(); itf != Objekte.end(); ++itf)
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
	Objekte.clear();
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc=DOC.toElement();
		if (dc.tagName()=="OBJEKT")
		{
			ScPreview *pre = new ScPreview(Prefs);
			AddObj(GetAttr(&dc, "NAME"), GetAttr(&dc, "DATA"), pre->createPreview(GetAttr(&dc, "DATA")));
			delete pre;
		}
		DOC=DOC.nextSibling();
	}
	QMap<QString,Elem>::Iterator itf;
	for (itf = Objekte.begin(); itf != Objekte.end(); ++itf)
	{
		(void) new QIconViewItem(this, itf.key(), itf.data().Preview);
	}
}

void BibView::RebuildView()
{
	clear();
	QMap<QString,Elem>::Iterator itf;
	for (itf = Objekte.begin(); itf != Objekte.end(); ++itf)
	{
		ScPreview *pre = new ScPreview(Prefs);
		itf.data().Preview = pre->createPreview(itf.data().Data);
		(void) new QIconViewItem(this, itf.key(), itf.data().Preview);
		delete pre;
	}
}

/* This is the main Dialog-Class for the Scrapbook */
Biblio::Biblio( QWidget* parent, ApplicationPrefs *prefs)
		: QDialog( parent, "Sclib", false, 0 )
{
	resize( 230, 190 );
	setCaption( tr( "Scrapbook" ) );
	setIcon(loadIcon("AppIcon.png"));
	ScFilename = "";
	Prefs = prefs;
	Changed = false;
	BiblioLayout = new QVBoxLayout( this );
	BiblioLayout->setSpacing( 0 );
	BiblioLayout->setMargin( 0 );
	fmenu = new QPopupMenu();
	fmenu->insertItem(loadIcon("DateiNeu16.png"), tr("&New"), this, SLOT(NewLib()), CTRL+Key_N);
	fmenu->insertItem(loadIcon("DateiOpen16.png"), tr("&Load..."), this, SLOT(Load()), CTRL+Key_O);
	fSave = fmenu->insertItem(loadIcon("DateiSave16.png"), tr("&Save"), this, SLOT(Save()), CTRL+Key_S);
	fmenu->insertItem( tr("Save &As..."), this, SLOT(SaveAs()));
	fmenu->insertItem(loadIcon("DateiClos16.png"), tr("&Close"), this, SLOT(CloseWin()));
	vmenu = new QPopupMenu();
	vS = vmenu->insertItem( tr( "&Small" ) );
	vM = vmenu->insertItem( tr( "&Medium" ) );
	vB = vmenu->insertItem( tr( "&Large" ) );
	switch (prefs->PSize)
	{
	case 40:
		vmenu->setItemChecked(vS, true);
		break;
	case 60:
		vmenu->setItemChecked(vM, true);
		break;
	case 80:
		vmenu->setItemChecked(vB, true);
		break;
	}
	menuBar = new QMenuBar(this);
	menuBar->insertItem( tr("&File"), fmenu);
	menuBar->insertItem( tr("&Preview"), vmenu);
	BiblioLayout->setMenuBar( menuBar );

	Frame3 = new QFrame( this, "Frame3" );
	Frame3->setFrameShape( QFrame::Box );
	Frame3->setFrameShadow( QFrame::Sunken );
	Frame3Layout = new QVBoxLayout( Frame3 );
	Frame3Layout->setSpacing( 6 );
	Frame3Layout->setMargin( 11 );

	BibWin = new BibView(Frame3, prefs);
	BibWin->setAutoArrange(true);
	BibWin->setSorting(true);
	BibWin->setResizeMode(QIconView::Adjust);
	Frame3Layout->addWidget( BibWin );
	BiblioLayout->addWidget( Frame3 );
	connect(BibWin, SIGNAL(dropped(QDropEvent *, const QValueList<QIconDragItem> &)), this, SLOT(DropOn(QDropEvent *)));
	connect(BibWin, SIGNAL(rightButtonClicked(QIconViewItem*, const QPoint &)), this, SLOT(HandleMouse(QIconViewItem*)));
	connect(BibWin, SIGNAL(itemRenamed(QIconViewItem*)), this, SLOT(ItemRenamed(QIconViewItem*)));
	connect(vmenu, SIGNAL(activated(int)), this, SLOT(SetPreview(int)));
}

void Biblio::closeEvent(QCloseEvent *ce)
{
	emit Schliessen();
	ce->accept();
}

void Biblio::CloseWin()
{
	emit Schliessen();
}

void Biblio::reject()
{
	emit Schliessen();
	QDialog::reject();
}

void Biblio::Save()
{
	if ((!ScFilename.isEmpty()) && (BibWin->Objekte.count() != 0))
	{
		BibWin->SaveContents(ScFilename);
		Changed = false;
	}
}

void Biblio::SaveAs()
{
	PrefsContext* dirs = prefsFile->getContext("dirs");
	QString fn = QFileDialog::getSaveFileName(dirs->get("scrap_saveas", "."),
	                                          tr("Scrapbooks (*.scs);;All Files (*)"), this);
	if (!fn.isEmpty())
	{
		dirs->set("scrap_saveas", fn.left(fn.findRev("/")));
		BibWin->SaveContents(fn);
		ScFilename = fn;
		setCaption(fn);
		fmenu->setItemEnabled(fSave, 1);
		Changed = false;
	}
}

void Biblio::Load()
{
	Save();
	PrefsContext* dirs = prefsFile->getContext("dirs");
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
		Prefs->PSize = 40;
		break;
	case 1:
		Prefs->PSize = 60;
		break;
	case 2:
		Prefs->PSize = 80;
		break;
	}
	AdjustMenu();
	BibWin->RebuildView();
}

void Biblio::AdjustMenu()
{
	vmenu->setItemChecked(vS, false);
	vmenu->setItemChecked(vM, false);
	vmenu->setItemChecked(vB, false);
	switch (Prefs->PSize)
	{
	case 40:
		vmenu->setItemChecked(vS, true);
		break;
	case 60:
		vmenu->setItemChecked(vM, true);
		break;
	case 80:
		vmenu->setItemChecked(vB, true);
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
	BibWin->Objekte.clear();
	BibWin->clear();
	ScFilename = "";
	setCaption( tr("Scrapbook"));
	fmenu->setItemEnabled(fSave, 0);
}

void Biblio::DeleteObj(QString name, QIconViewItem *ite)
{
	BibWin->Objekte.remove(name);
	delete ite;
	BibWin->sort(BibWin->sortDirection());
	BibWin->arrangeItemsInGrid(true);
	Changed = true;
	if (Prefs->SaveAtQ)
		Save();
}

void Biblio::ItemRenamed(QIconViewItem *ite)
{
	QString ObjData;
	QPixmap ObjPreview;
	disconnect(BibWin, SIGNAL(itemRenamed(QIconViewItem*)), this, SLOT(ItemRenamed(QIconViewItem*)));
	if (OldName != ite->text())
	{
		if (BibWin->Objekte.contains(ite->text()))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Name \"%1\" isn't unique.\nPlease choose another.").arg(ite->text()), tr("OK"));
			ite->setText(OldName);
		}
		else
		{
			ObjData = BibWin->Objekte[OldName].Data;
			ObjPreview = BibWin->Objekte[OldName].Preview;
			BibWin->Objekte.remove(OldName);
			BibWin->AddObj(ite->text(), ObjData, ObjPreview);
			BibWin->sort(BibWin->sortDirection());
			BibWin->arrangeItemsInGrid(true);
			Changed = true;
			if (Prefs->SaveAtQ)
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
		if (Prefs->SaveAtQ)
			Save();
	}
}

void Biblio::ObjFromMenu(QString text)
{
	QString nam, tmp;
	nam = tr("Object") + tmp.setNum(BibWin->Objekte.count());
	Query *dia = new Query(this, "tt", 1, 0, tr("&Name:"), tr("New Entry"));
	dia->setEditText(nam, true);
	if (dia->exec())
	{
		nam = dia->getEditText();
		while (BibWin->Objekte.contains(nam))
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
	ScPreview *pre = new ScPreview(Prefs);
	QPixmap pm = pre->createPreview(ff);
	BibWin->AddObj(nam, ff, pm);
	(void) new QIconViewItem(BibWin, nam, pm);
	Changed = true;
	delete pre;
}
