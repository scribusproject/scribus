#include "seiten.h"
#include "seiten.moc"
#include <qcursor.h>
#include "scribus.h"
#include "dynamictip.h"
extern QPixmap loadIcon(QString nam);


/* Code fuer DragObjekt */
SeDrag::SeDrag(QString secret, QWidget * parent, const char * name): QStoredDrag("page/magic", parent, name)
{
	QByteArray data(secret.length());
	for (uint a = 0; a < secret.length(); ++a)
		data[a]= QChar(secret[a]);
	setEncodedData( data );
}

bool SeDrag::canDecode( QDragMoveEvent* e )
{
	return e->provides("page/magic");
}

bool SeDrag::decode( QDropEvent* e, QString& str )
{
	QByteArray payload = e->data("page/magic");
	if (payload.size())
	{
		e->accept();
		str = "";
		for (uint a = 0; a < payload.size(); ++a)
			str += payload[a];
		return true;
	}
	return false;
}

/* IconItems Code */
SeItem::SeItem(QTable* parent, QString text, QPixmap Pix)
		: QTableItem(parent, QTableItem::Never, "", Pix)
{
	pageName = text;
	setWordWrap(true);
}

/* ListBox Subclass */
SeList::SeList(QWidget* parent) : QListBox(parent)
{
	Mpressed = false;
	setAcceptDrops(true);
}

void SeList::mouseReleaseEvent(QMouseEvent *m)
{
	Mpressed = false;
	if (m->button() == RightButton)
	{
		QPopupMenu *pmen = new QPopupMenu();
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		int px = pmen->insertItem( tr("Show Page Previews"), this, SLOT(ToggleTh()));
		if (Thumb)
			pmen->setItemChecked(px, true);
		pmen->exec(QCursor::pos());
		delete pmen;
	}
}

void SeList::ToggleTh()
{
	Thumb = !Thumb;
	emit ThumbChanged();
}

void SeList::mousePressEvent(QMouseEvent* e)
{
	e->accept();
	CurItem = 0;
	QListBoxItem *i = itemAt(e->pos());
	if (i)
	{
		CurItem = i;
		Mpos = e->pos();
		Mpressed = true;
	}
}

void SeList::mouseMoveEvent(QMouseEvent* e)
{
	if ((Mpressed) && ((Mpos - e->pos()).manhattanLength() > 4))
	{
		Mpressed = false;
		QListBoxItem *i = itemAt(Mpos);
		if (i)
		{
			QDragObject *dr = new SeDrag("1"+i->text(), this, "te");
			dr->setPixmap(loadIcon("doc.png"));
			dr->drag();
		}
	}
}

/* QTable Subclass */
SeView::SeView(QWidget* parent) : QTable(parent)
{
	setDragEnabled(true);
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setShowGrid(false);
	Mpressed = false;
	Doppel = false;
	Namen = true;
	setFocusPolicy(NoFocus);
}

void SeView::contentsMousePressEvent(QMouseEvent* e)
{
	e->accept();
	Mpos = e->pos();
	Mpressed = true;
}

void SeView::contentsMouseReleaseEvent(QMouseEvent* e)
{
	e->accept();
	Mpressed = false;
/*	if (e->button() == RightButton)
	{
		QPopupMenu *pmen = new QPopupMenu();
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		int px = pmen->insertItem( tr("Show Master Page Names"), this, SLOT(ToggleNam()));
		if (Namen)
			pmen->setItemChecked(px, true);
		pmen->exec(QCursor::pos());
		delete pmen;
	} */
	emit Click(rowAt(e->pos().y()), columnAt(e->pos().x()), e->button());
}

void SeView::ToggleNam()
{
/*	Namen = !Namen;
	int val = 35;
	if (Namen)
		val = 100;
	setColumnWidth(1, val);
	setColumnWidth(3, val);
	hide();
	show(); */
}

void SeView::contentsMouseMoveEvent(QMouseEvent* e)
{
	QString str, tmp;
	bool dummy;
	int p;
	int a = rowAt(e->pos().y());
	int b = columnAt(e->pos().x());
	if ((Mpressed) && ((Mpos - e->pos()).manhattanLength() > 4))
	{
		Mpressed = false;
		if ((a != -1) && (b != -1))
		{
			QTableItem* ite = item(a, b);
			if (ite == 0)
				return;
			SeItem* it = (SeItem*)ite;
			str = it->pageName;
			p = GetPage(a, b, &dummy);
			QDragObject *dr = new SeDrag("2 "+tmp.setNum(p)+" "+str, this, "te");
			dr->setPixmap(loadIcon("doc.png"));
			dr->drag();
		}
	}
	QTable::contentsMouseMoveEvent(e);
}

void SeView::contentsDropEvent(QDropEvent * e)
{
	QString str, tmp;
	bool lastPage = false;
	if (SeDrag::decode(e, str))
	{
		ClearPix();
		if (str.startsWith("1"))
		{
			int a = rowAt(e->pos().y());
			int b = columnAt(e->pos().x());
			int p;
			tmp = str.remove(0,1);
			if ((a == -1) || (b == -1))
				return;
			if (a == numRows()-1)
			{
				emit NewPage(MaxC+1, tmp);
				return;
			}
			p = GetPage(a, b, &lastPage);
			if (numCols() == 1)
			{
				if ((a % 2) == 0)
					emit NewPage(p, tmp);
				else
					emit UseTemp(tmp, p);
				return;
			}
			else
			{
				if ((b % 2) == 0)
				{
					if (lastPage)
						emit NewPage(p+1, tmp);
					else
						emit NewPage(p, tmp);
				}
				else
					emit UseTemp(tmp, p);
				return;
			}
		}
		if (str.startsWith("2"))
		{
			int st = str.find(" ");
			int en = str.find(" ", st+1);
			tmp = str.mid(en+1);
			int dr = str.mid(st, en-st).toInt();
			int a = rowAt(e->pos().y());
			int b = columnAt(e->pos().x());
			int p;
			if ((a == -1) || (b == -1))
				return;
			QTableItem* ite = item(a, b);
			p = GetPage(a, b, &lastPage);
			if (a == numRows()-1)
			{
				emit MovePage(dr, p+1);
				return;
			}
			if (numCols() == 1)
			{
				if ((a % 2) == 0)
					emit MovePage(dr, p);
				else
				{
					emit UseTemp(tmp, p);
					if (ite == 0)
						return;
					SeItem* it = (SeItem*)ite;
					it->pageName = tmp;
				}
				return;
			}
			else
			{
				if ((b % 2) == 0)
				{
					if (lastPage)
						emit MovePage(dr, p+1);
					else
						emit MovePage(dr, p);
				}
				else
				{
					emit UseTemp(tmp, p);
					if (ite == 0)
						return;
					SeItem* it = (SeItem*)ite;
					it->pageName = tmp;
				}
				return;
			}
		}
	}
}

void SeView::contentsDragEnterEvent(QDragEnterEvent *e)
{
	if (SeDrag::canDecode(e))
		e->accept();
}

void SeView::contentsDragLeaveEvent(QDragLeaveEvent *)
{
	ClearPix();
}

void SeView::contentsDragMoveEvent(QDragMoveEvent *e)
{
	QString str, tmp;
	if (SeDrag::decode(e, str))
	{
		int a = rowAt(e->pos().y());
		int b = columnAt(e->pos().x());
		ClearPix();
		if ((a == -1) || (b == -1))
			return;
		if (numCols() == 1)
		{
			if ((a % 2) == 0)
			{
				QPixmap pm;
				pm = QPixmap(columnWidth(b), rowHeight(a));
				pm.fill(darkBlue);
				setPixmap(a, 0, pm);
			}
		}
		else
		{
			if (((b % 2) == 0) || (a == numRows()-1))
			{
				QPixmap pm;
				pm = QPixmap(columnWidth(b), rowHeight(a));
				pm.fill(darkBlue);
				setPixmap(a, b, pm);
			}
		}
	}
}

void SeView::ClearPix()
{
	int counter = 0;
	int rowcounter = 0;
	for (int a = 0; a < numRows(); ++a)
	{
		counter = 0;
		if (numCols() == 1)
		{
			if ((a % 2) == 0)
			{
				clearCell(rowcounter, 0);
				rowcounter += 2;
			}
		}
		else
		{
			for (int b = 0; b < numCols(); ++b)
			{
				if ((b % 2) == 0)
				{
					clearCell(rowcounter, counter);
					counter += 2;
				}
			}
			rowcounter++;
		}
	}
	for (int c = 0; c < numCols(); ++c)
	{
		clearCell(numRows()-1, c);
	}
}

int SeView::GetPage(int r, int c, bool *last)
{
	int counter = firstP;
	int rowcounter = 0;
	int ret = MaxC;
	*last = false;
	if (r == numRows()-1)
	{
		*last = true;
		return ret;
	}
	if ((r == 0) && (c < firstP*colmult+coladd))
		return 0;
	for (int a = 0; a < MaxC+1; ++a)
	{
		if ((rowcounter*rowmult+rowadd == r) && (counter*colmult+coladd == c))
		{
			ret = a;
			return ret;
		}
		else
		{
			if (numCols() == 1)
			{
				if (rowcounter*rowmult == r)
				{
					ret = a;
					return ret;
				}
			}
			else
			{
				if ((counter*colmult == c) && (rowcounter*rowmult+rowadd == r))
				{
					ret = a;
					return ret;
				}
			}
		}
		counter++;
		if (counter > cols-1)
		{
			counter = 0;
			rowcounter++;
		}
	}
	*last = true;
	return ret;
}

/* Der Muelleimer */
TrashBin::TrashBin(QWidget * parent) : QLabel(parent, "Trash")
{
	Normal = loadIcon("trashcan.png");
	Offen = loadIcon("trashcan2.png");
	setPixmap(Normal);
	setScaledContents(false);
	setAcceptDrops(true);
}

void TrashBin::dragEnterEvent(QDragEnterEvent *e)
{
	if (SeDrag::canDecode(e))
	{
		e->accept();
		setPixmap(Offen);
	}
}

void TrashBin::dragLeaveEvent(QDragLeaveEvent *)
{
	setPixmap(Normal);
}

void TrashBin::dropEvent(QDropEvent * e)
{
	setPixmap(Normal);
	QString str, tmp;
	if (SeDrag::decode(e, str))
	{
		if (str.startsWith("2"))
		{
			int st = str.find(" ");
			int en = str.find(" ", st+1);
			emit DelPage(str.mid(st, en-st).toInt());
		}
		if (str.startsWith("1"))
		{
			tmp = str.remove(0,1);
			emit DelMaster(tmp);
		}
	}
}


SeitenPal::SeitenPal(QWidget* parent) : ScrPaletteBase( parent, "SP", false, 0)
{
	setIcon(loadIcon("AppIcon.png"));
	SeitenPalLayout = new QVBoxLayout( this );
	SeitenPalLayout->setSpacing( 5 );
	SeitenPalLayout->setMargin( 5 );
	Splitter1 = new QSplitter( this, "Splitter1" );
	Splitter1->setOrientation( QSplitter::Vertical );
	QWidget* privateLayoutWidget = new QWidget( Splitter1, "Layout2" );
	Layout2 = new QVBoxLayout( privateLayoutWidget, 0, 5, "Layout2");
	TextLabel1 = new QLabel( privateLayoutWidget, "TextLabel1" );
	Layout2->addWidget( TextLabel1 );
	masterPageList = new SeList(privateLayoutWidget);
	masterPageList->setMinimumSize(QSize(130,70));
	masterPageList->Thumb = false;
	Layout2->addWidget( masterPageList );
	QWidget* privateLayoutWidget_2 = new QWidget( Splitter1, "Layout3" );
	Layout3 = new QVBoxLayout( privateLayoutWidget_2, 0, 5, "Layout3");
	TextLabel2 = new QLabel( privateLayoutWidget_2, "TextLabel2" );
	Layout3->addWidget( TextLabel2 );
	PageView = new SeView(privateLayoutWidget_2);
	PageView->setLeftMargin(0);
	PageView->verticalHeader()->hide();
	PageView->setTopMargin(0);
	PageView->horizontalHeader()->hide();
	PageView->setSorting(false);
	PageView->setSelectionMode(QTable::NoSelection);
	PageView->setColumnMovingEnabled(false);
	PageView->setRowMovingEnabled(false);
	PageView->setNumRows(1);
	PageView->setNumCols(1);
	PageView->setMinimumSize(QSize(130,120));
	Layout3->addWidget( PageView );
	SeitenPalLayout->addWidget( Splitter1 );

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 6 );
	Layout1->setMargin( 0 );

	Layout4 = new QVBoxLayout;
	Layout4->setSpacing( 6 );
	Layout4->setMargin( 0 );
	facingPagesChk = new QCheckBox(this, "facingPagesChk");
	Layout4->addWidget( facingPagesChk );
	firstPageLeftChk = new QCheckBox(this, "firstPageLeftChk");
	Layout4->addWidget( firstPageLeftChk );
	Layout1->addLayout( Layout4 );

	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );

	Trash = new TrashBin( this );
	Trash->setMinimumSize(QSize(22,22));
	Layout1->addWidget( Trash );
	SeitenPalLayout->addLayout( Layout1 );
	pix = loadIcon("document2.png");
	Vie = 0;
	Rebuild();
	languageChange();
	dynTip = new DynamicTip(PageView);
	connect(masterPageList, SIGNAL(doubleClicked(QListBoxItem*)), this, SLOT(selMasterPage()));
	connect(masterPageList, SIGNAL(ThumbChanged()), this, SLOT(RebuildTemp()));
	connect(PageView, SIGNAL(Click(int, int, int)), this, SLOT(GotoPage(int, int, int)));
	connect(PageView, SIGNAL(MovePage(int, int)), this, SLOT(MPage(int, int)));
//	connect(facingPagesChk, SIGNAL(clicked()), this, SLOT(handleFacingPagesChk()));
//	connect(firstPageLeftChk, SIGNAL(clicked()), this, SLOT(handleFirstPageLeftChk()));
	connect(Trash, SIGNAL(DelMaster(QString)), this, SLOT(DelMPage(QString)));
	QToolTip::add(Trash, "<qt>" + tr("Drag pages or master pages onto the trashbin to delete them") + "</qt>");
//	QToolTip::add(PageView, "<qt>" + tr("Previews all the pages of your document") + "</qt>");
	QToolTip::add(masterPageList, "<qt>" + tr("Here are all your master pages. To create a new page, drag a master page to the page view below") + "</qt>");
	facingPagesChk->setEnabled(false);
	firstPageLeftChk->setEnabled(false);
}

void SeitenPal::DelMPage(QString tmp)
{
	if (tmp == tr("Normal"))
		return;
	int Nr = Vie->Doc->MasterNames[tmp];
	Page* Seite = Vie->Doc->MasterPages.at(Nr);
	Vie->Doc->MasterPages.remove(Nr);
	delete Seite;
	Vie->Doc->MasterNames.clear();
	for (uint aa=0; aa < Vie->Doc->MasterPages.count(); ++aa)
	{
		Seite = Vie->Doc->MasterPages.at(aa);
		Seite->setPageNr(aa);
		if (Vie->Doc->currentPageLayout == doublePage)
		{
			Seite->Margins.Left = Seite->LeftPg ? Vie->Doc->pageMargins.Right : Vie->Doc->pageMargins.Left;
			Seite->Margins.Right= Seite->LeftPg? Vie->Doc->pageMargins.Left : Vie->Doc->pageMargins.Right;
		}
		else
		{
			Seite->Margins.Right = Vie->Doc->pageMargins.Right;
			Seite->Margins.Left = Vie->Doc->pageMargins.Left;
		}
		Seite->Margins.Top = Vie->Doc->pageMargins.Top;
		Seite->Margins.Bottom = Vie->Doc->pageMargins.Bottom;
		Vie->Doc->MasterNames[Seite->PageNam] = aa;
	}
	for (uint b=0; b<Vie->Doc->DocPages.count(); ++b)
	{
		if (Vie->Doc->DocPages.at(b)->MPageNam == tmp)
			Vie->Doc->DocPages.at(b)->MPageNam = "Normal";
	}
	Vie->DrawNew();
	RebuildTemp();
	RebuildPage();
	Vie->Doc->setModified(true);
}

void SeitenPal::MPage(int r, int c)
{
	if (r == c)
		return;
	if (c > PageView->MaxC)
		Vie->Doc->movePage(r, r + 1, c, 2);
	else
		Vie->Doc->movePage(r, r + 1, c, 0);
	Vie->reformPages();
	RebuildPage();
	Vie->DrawNew();
	Vie->Doc->setModified(true);
}

void SeitenPal::GotoPage(int r, int c, int b)
{
	int p;
	bool dummy;
	if ((b == LeftButton) && (r != -1) && (c != -1))
	{
		p = PageView->GetPage(r, c, &dummy);
		emit GotoSeite(p);
	}
}

void SeitenPal::DisablePal()
{
	PageView->setEnabled(false);
	masterPageList->setEnabled(false);
	facingPagesChk->setEnabled(false);
	firstPageLeftChk->setEnabled(false);
}

void SeitenPal::EnablePal()
{
	PageView->setEnabled(true);
	masterPageList->setEnabled(true);
/*	facingPagesChk->setEnabled(true);
	firstPageLeftChk->setEnabled(PageView->Doppel ? true : false); */
}

void SeitenPal::handleFacingPagesChk()
{
	double tpr = Vie->Doc->pageMargins.Top;
	double lr = Vie->Doc->pageMargins.Left;
	double rr = Vie->Doc->pageMargins.Right;
	double br = Vie->Doc->pageMargins.Bottom;
	bool fp = facingPagesChk->isChecked();
	Vie->Doc->resetPage(tpr, lr, rr, br, fp);
	Vie->reformPages();
	Vie->DrawNew();
	Vie->GotoPage(Vie->Doc->currentPage->pageNr());
	RebuildPage();
	Vie->Doc->setModified(true);
//	firstPageLeftChk->setEnabled(fp ? true : false);
}

void SeitenPal::handleFirstPageLeftChk()
{
	double tpr = Vie->Doc->pageMargins.Top;
	double lr = Vie->Doc->pageMargins.Left;
	double rr = Vie->Doc->pageMargins.Right;
	double br = Vie->Doc->pageMargins.Bottom;
	int fp2 = Vie->Doc->currentPageLayout;
	if (fp2 == doublePage)
		Vie->Doc->pageSets[Vie->Doc->currentPageLayout].FirstPage = firstPageLeftChk->isChecked();
	Vie->Doc->resetPage(tpr, lr, rr, br, fp2);
	Vie->reformPages();
	Vie->DrawNew();
	Vie->GotoPage(Vie->Doc->currentPage->pageNr());
	RebuildPage();
	Vie->Doc->setModified(true);
}

void SeitenPal::RebuildTemp()
{
	if (ScApp->ScriptRunning)
		return;
	masterPageList->clear();
	if (Vie == 0)
		return;
	QPixmap pm;
	QMap<QString,int>::Iterator it;
	for (it = Vie->Doc->MasterNames.begin(); it != Vie->Doc->MasterNames.end(); ++it)
	{
		if (masterPageList->Thumb)
		{
			pm.convertFromImage(Vie->MPageToPixmap(it.key(),60));
			masterPageList->insertItem(pm, it.key() == "Normal" ? tr("Normal") : it.key());
		}
		else
			masterPageList->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());
	}
}

void SeitenPal::RebuildPage()
{
	if (ScApp->ScriptRunning)
		return;
	QString str;
	disconnect(facingPagesChk, SIGNAL(clicked()), this, SLOT(handleFacingPagesChk()));
	disconnect(firstPageLeftChk, SIGNAL(clicked()), this, SLOT(handleFirstPageLeftChk()));
	PageView->setNumRows(1);
	PageView->setNumCols(1);
	if (Vie == 0)
	{
		connect(facingPagesChk, SIGNAL(clicked()), this, SLOT(handleFacingPagesChk()));
		connect(firstPageLeftChk, SIGNAL(clicked()), this, SLOT(handleFirstPageLeftChk()));
		return;
	}
	PageView->Doppel = Vie->Doc->currentPageLayout == doublePage;
	PageView->Links = Vie->Doc->pageSets[Vie->Doc->currentPageLayout].FirstPage;
//	facingPagesChk->setChecked(PageView->Doppel);
//	firstPageLeftChk->setChecked(PageView->Links);
//	if (PageView->Doppel)
//		firstPageLeftChk->setEnabled(true);
	PageView->MaxC = Vie->Doc->Pages.count()-1;
	int counter, rowcounter, colmult, rowmult, coladd,rowadd;
	counter = Vie->Doc->pageSets[Vie->Doc->currentPageLayout].FirstPage;
	int cols = Vie->Doc->pageSets[Vie->Doc->currentPageLayout].Columns;
	int rows = (Vie->Doc->pageCount+counter) / Vie->Doc->pageSets[Vie->Doc->currentPageLayout].Columns;
	if (((Vie->Doc->pageCount+counter) % Vie->Doc->pageSets[Vie->Doc->currentPageLayout].Columns) != 0)
		rows++;
	rowcounter = 0;
	if (cols == 1)
	{
		PageView->setNumCols(cols);
		PageView->setNumRows(rows*2+1);
		colmult = 1;
		coladd = 0;
		rowmult = 2;
		rowadd = 1;
	}
	else
	{
		PageView->setNumCols(cols*2);
		PageView->setNumRows(rows+1);
		colmult = 2;
		coladd = 1;
		rowmult = 1;
		rowadd = 0;
	}
	PageView->coladd = coladd;
	PageView->colmult = colmult;
	PageView->rowadd = rowadd;
	PageView->rowmult = rowmult;
	PageView->firstP = counter;
	PageView->cols = Vie->Doc->pageSets[Vie->Doc->currentPageLayout].Columns;
	for (uint a = 0; a < Vie->Doc->Pages.count(); ++a)
	{
		str = Vie->Doc->Pages.at(a)->MPageNam;
		QTableItem *it = new SeItem( PageView, str, CreateIcon(a, pix));
		PageView->setItem(rowcounter*rowmult+rowadd, counter*colmult+coladd, it);
		PageView->setColumnWidth(counter*colmult+coladd, pix.width());
		if (cols == 1)
		{
			PageView->setRowHeight(rowcounter*rowmult, 10);
			PageView->setRowHeight(rowcounter*rowmult+rowadd, pix.height());
		}
		else
			PageView->setRowHeight(rowcounter*rowmult+rowadd, pix.height()+5);
		counter++;
		if (counter > Vie->Doc->pageSets[Vie->Doc->currentPageLayout].Columns-1)
		{
			counter = 0;
			rowcounter++;
		}
	}
	PageView->setRowHeight(PageView->numRows()-1, 10);
	counter = 0;
	if (cols != 1)
	{
		for (int c = 0; c < PageView->numCols(); ++c)
		{
			if ((counter % 2) == 0)
				PageView->setColumnWidth(counter, 10);
			else
				PageView->setColumnWidth(counter, pix.width());
			counter++;
		}
	}
	PageView->repaint();
	connect(facingPagesChk, SIGNAL(clicked()), this, SLOT(handleFacingPagesChk()));
	connect(firstPageLeftChk, SIGNAL(clicked()), this, SLOT(handleFirstPageLeftChk()));
}

void SeitenPal::Rebuild()
{
	RebuildTemp();
	RebuildPage();
	if (Vie == 0)
		DisablePal();
	else
		EnablePal();
}

void SeitenPal::SetView(ScribusView *view)
{
	Vie = view;
}

void SeitenPal::selMasterPage()
{
	if (masterPageList->CurItem != 0)
		emit EditTemp(masterPageList->CurItem->text());
}

QPixmap SeitenPal::CreateIcon(int nr, QPixmap ret)
{
	QString tmp;
	QPainter p;
	if (p.begin(&ret))
	{
		p.setBrush(white);
		p.setBackgroundColor(white);
		p.setBackgroundMode(QPainter::OpaqueMode);
		p.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
		p.setFont(QFont("Helvetica", 12, QFont::Bold));
		tmp = tmp.setNum(nr+1);
		QRect b = p.fontMetrics().boundingRect(tmp);
		QRect c = QRect((ret.width() / 2 - b.width() / 2)-2, (ret.height() / 2 - b.height() / 2)-2, b.width()+4, b.height()+4);
		p.drawRect(c);
		QRect d = QRect((ret.width() / 2 - b.width() / 2), (ret.height() / 2 - b.height() / 2), b.width(), b.height());
		p.setFont(QFont("Helvetica", 11, QFont::Bold));
		p.drawText(d, Qt::AlignCenter, tmp);
		p.end();
	}
	return ret;
}

void SeitenPal::languageChange()
{
	setCaption( tr( "Arrange Pages" ) );
	TextLabel1->setText( tr( "Available Master Pages:" ) );
	TextLabel2->setText( tr( "Document Pages:" ) );
	facingPagesChk->setText( tr( "Facing Pages" ) );
	firstPageLeftChk->setText( tr( "Left Page First" ) );
}
