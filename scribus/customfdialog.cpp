/***************************************************************************
                          customfdialog.cpp  -  description
                             -------------------
    begin                : Fri Nov 30 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "customfdialog.h"
#include "customfdialog.moc"
#include <qpixmap.h>
#include <qpainter.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qdom.h>
#include <qtextcodec.h>
#include "scribusstructs.h"
#include "scimage.h"
#include <util.h>

extern QString DocDir;

ImIconProvider::ImIconProvider(QWidget *pa) : QFileIconProvider(pa)
{
	fmts.clear();
	QString tmp[] = {"eps", "gif", "png", "jpg", "jpeg", "xpm", "tif", "tiff", "bmp", "pbm", "pgm", "ppm", "xbm", "xpm", "psd"};
	size_t array = sizeof(tmp) / sizeof(*tmp);
	for (uint a = 0; a < array; ++a)
		fmts += tmp[a];
	imagepm = loadIcon("image.png");
	pspm = loadIcon("postscript.png");
	txtpm = loadIcon("txt.png");
	docpm = loadIcon("doc.png");
	pdfpm = loadIcon("pdf.png");
	oosxdpm = loadIcon("ooo_draw.png");
	oosxwpm = loadIcon("ooo_writer.png");
	vectorpm = loadIcon("vectorgfx.png");
}

const QPixmap * ImIconProvider::pixmap(const QFileInfo &fi)
{
	QString ext = fi.extension(false).lower();
	if (fmts.contains(ext))
		return &imagepm;
	else
	{
		ext = fi.extension(true).lower();
		if (ext.endsWith("ps"))
			return &pspm;
		if (ext.endsWith("txt"))
			return &txtpm;
		if (ext.endsWith("scd") || ext.endsWith("scd.gz"))
			return &docpm;
		if (ext.endsWith("sla") || ext.endsWith("sla.gz"))
			return &docpm;
		if (ext.endsWith("pdf"))
			return &pdfpm;
		if (ext.endsWith("sxd"))
			return &oosxdpm;
		if (ext.endsWith("sxw"))
			return &oosxwpm;
		if (ext.endsWith("svg") || ext.endsWith("svgz"))
			return &vectorpm;
		return QFileIconProvider::pixmap(fi);
	}
}

FDialogPreview::FDialogPreview(QWidget *pa) : QLabel(pa)
{
	setAlignment(AlignLeft | AlignTop);
	setMinimumSize( QSize( 100, 100 ) );
	setMaximumSize( QSize( 300, 300 ) );
	setScaledContents( false );
	setEraseColor( white );
	setFrameShape( QLabel::WinPanel );
	setFrameShadow( QLabel::Sunken );
	updtPix();
}

void FDialogPreview::updtPix()
{
	QPixmap pm;
	QRect inside = contentsRect();
	pm = QPixmap(inside.width(), inside.height());
	pm.fill(white);
	setPixmap(pm);
}

void FDialogPreview::GenPreview(QString name)
{
	QPixmap pm;
	QString Buffer = "";
	QFileInfo fi = QFileInfo(name);
	if (fi.isDir())
		return;
	updtPix();
	int w = pixmap()->width();
	int h = pixmap()->height();
	bool mode = false;
	QString ext = fi.extension(false).lower();
	QStringList formats;
	formats = QStringList::fromStrList(QImageIO::inputFormats());
	formats.append("jpg");
#ifdef HAVE_TIFF
	formats.append("tif");
	formats.append("tiff");
#endif
	formats.append("psd");
	formats.append("eps");
	formats.append("pdf");
	formats.append("ps");
	QString allFormats = formats.join( " " );
	formats.clear();
	allFormats = allFormats.lower();
	formats = QStringList::split( " ", allFormats );
	if (formats.contains(ext))
	{
		ScImage im;
		if (im.LoadPicture(name, "", 0, false, false, 1, 72, &mode))
		{
			int ix = im.width();
			int iy = im.height();
			int xres = im.imgInfo.xres;
			int yres = im.imgInfo.yres;
			QString tmp = "";
			QString tmp2 = "";
			QImage im2;
			if ((im.width() > w-5) || (im.height() > h-44))
			{
				double sx = im.width() / static_cast<double>(w-5);
				double sy = im.height() / static_cast<double>(h-44);
				im2 = sy < sx ?  im.smoothScale(qRound(im.width() / sx), qRound(im.height() / sx)) :
									im.smoothScale(qRound(im.width() / sy), qRound(im.height() / sy));
			}
			else
				im2 = im.copy();
			QPainter p;
			pixmap()->fill(white);
			p.begin(pixmap());
			p.drawImage(0, 0, im2);
			p.drawText(2, h-29, tr("Size:")+" "+tmp.setNum(ix)+" x "+tmp2.setNum(iy));
			p.drawText(2, h-17, tr("Resolution:")+" "+tmp.setNum(xres)+" x "+tmp2.setNum(yres)+" "+ tr("DPI"));
			QString cSpace;
			if ((ext == "pdf") || (ext == "eps") || (ext == "ps"))
				cSpace = tr("Unknown");
			else
			{
				switch (im.imgInfo.colorspace)
				{
					case 0:
						cSpace = tr("RGB");
						break;
					case 1:
						cSpace = tr("CMYK");
						break;
					case 2:
						cSpace = tr("Grayscale");
						break;
				}
			}
			p.drawText(2, h-5, tr("Colorspace:")+" "+cSpace);
			p.end();
			repaint();
		}
	}
	else
	{
		if (loadText(name, &Buffer))
		{
			if (Buffer.startsWith("<SCRIBUS"))
			{
				QDomDocument docu("scridoc");
				if(!docu.setContent(Buffer))
					return;
				QDomElement elem=docu.documentElement();
				if ((elem.tagName() != "SCRIBUS") && (elem.tagName() != "SCRIBUSUTF8") && (elem.tagName() != "SCRIBUSUTF8NEW"))
					return;
				QDomNode DOC=elem.firstChild();
				QDomElement dc=DOC.toElement();
				QString Tit = tr("Title:")+" ";
				QString ti2 = dc.attribute("TITLE");
				if (ti2.isEmpty())
					ti2= tr("No Title");
				Tit += ti2+"\n";
				QString Aut = tr("Author:")+" ";
				QString au2 = dc.attribute("AUTHOR");
				if (au2.isEmpty())
					au2 = tr("Unknown");
				Aut += au2;
				setText( tr("Scribus Document")+"\n\n"+Tit+Aut);
			}
			else  if ((ext == "txt") || (ext == "html") || (ext == "xml"))
				setText(Buffer.left(200));
		}
	}
}	

void FDialogPreview::previewUrl( const QUrl &url )
{
	if (url.isLocalFile())
		GenPreview(url.path());
}

CustomFDialog::CustomFDialog(QWidget *pa, QString wDir, QString cap, QString filter, 
                             bool Pre, bool mod, bool comp, bool cod, bool dirOnly)
                             : QFileDialog(QString::null, filter, pa, 0, true)
{
 	setIcon(loadIcon("AppIcon.png"));
 	setCaption(cap);
	cDir = QDir(wDir);
	setDir(cDir);
	setIconProvider(new ImIconProvider(this));
	FDialogPreview *pw;
	if (dirOnly)
	{
		Layout = new QFrame(this);
		Layout1 = new QHBoxLayout(Layout);
		Layout1->setSpacing( 0 );
		Layout1->setMargin( 0 );
		SaveZip = new QCheckBox( tr( "&Compress File" ), Layout, "test");
		Layout1->addWidget(SaveZip, Qt::AlignLeft);
		QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout1->addItem( spacer );
		addWidgets(0, Layout, 0);
		LayoutC = new QFrame(this);
		Layout1C = new QHBoxLayout(LayoutC);
		Layout1C->setSpacing( 0 );
		Layout1C->setMargin( 0 );
		WFonts = new QCheckBox( tr( "&Include Fonts" ), LayoutC, "test");
		Layout1C->addWidget(WFonts, Qt::AlignLeft);
		QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout1C->addItem( spacer2 );
		addWidgets(0, LayoutC, 0);
		setMode(QFileDialog::DirectoryOnly);
	}
	else
	{
		setContentsPreviewEnabled( true );
		pw = new FDialogPreview( this );
		setContentsPreview( pw, pw );
		if (comp)
		{
			Layout = new QFrame(this);
			Layout1 = new QHBoxLayout(Layout);
			Layout1->setSpacing( 6 );
			Layout1->setMargin( 0 );
			SaveZip = new QCheckBox( tr( "&Compress File" ), Layout, "test");
			Layout1->addWidget(SaveZip);
			QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
			Layout1->addItem( spacer );
		}
		if (mod)
			setMode(QFileDialog::ExistingFile);
		else
		{
			setMode(QFileDialog::AnyFile);
			if (comp)
				addWidgets(0, Layout, 0);
		}
		if (cod)
		{
			LayoutC = new QFrame(this);
			Layout1C = new QHBoxLayout(LayoutC);
			Layout1C->setSpacing( 0 );
			Layout1C->setMargin( 4 );
			TxCodeT = new QLabel(this);
			TxCodeT->setText( tr("Encoding:"));
			TxCodeM = new QComboBox(true, LayoutC, "Cod");
			TxCodeM->setEditable(false);
			QString tmp_txc[] = {"ISO 8859-1", "ISO 8859-2", "ISO 8859-3", "ISO 8859-4", "ISO 8859-5", "ISO 8859-6",
								"ISO 8859-7", "ISO 8859-8", "ISO 8859-9", "ISO 8859-10", "ISO 8859-13", "ISO 8859-14",
								"ISO 8859-15", "utf8", "KOI8-R", "KOI8-U", "CP1250", "CP1251", "CP1252", "CP1253",
								"CP1254", "CP1255", "CP1256", "CP1257", "Apple Roman"};
			size_t array = sizeof(tmp_txc) / sizeof(*tmp_txc);
			for (uint a = 0; a < array; ++a)
				TxCodeM->insertItem(tmp_txc[a]);
			QString localEn = QTextCodec::codecForLocale()->name();
			bool hasIt = false;
			for (int cc = 0; cc < TxCodeM->count(); ++cc)
			{
				if (TxCodeM->text(cc) == localEn)
				{
					TxCodeM->setCurrentItem(cc);
					hasIt = true;
					break;
				}
			}
			if (!hasIt)
			{
				TxCodeM->insertItem(localEn);
				TxCodeM->setCurrentItem(TxCodeM->count()-1);
			}
			TxCodeM->setMinimumSize(QSize(200, 0));
			Layout1C->addWidget(TxCodeM);
			QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
			Layout1C->addItem( spacer2 );
			addWidgets(TxCodeT, LayoutC, 0);
		}
		setPreviewMode(Pre ? QFileDialog::Contents : QFileDialog::NoPreview );
		setViewMode( QFileDialog::List );
		if (comp)
			connect(SaveZip, SIGNAL(clicked()), this, SLOT(HandleComp()));
	}
	HomeB = new QToolButton(this);
	HomeB->setIconSet(loadIcon("gohome.png"));
	HomeB->setTextLabel( tr("Moves to your Document Directory.\nThis can be set in the Preferences."));
	connect(HomeB, SIGNAL(clicked()), this, SLOT(slotHome()));
	addToolButton(HomeB);
}

CustomFDialog::~CustomFDialog()
{
	setIconProvider(0);
	cDir.setCurrent(dir()->path());
}

void CustomFDialog::slotHome()
{
	setDir(QDir(DocDir));
}

void CustomFDialog::HandleComp()
{
	QString tmp;
	tmp = selectedFile();
	if (SaveZip->isChecked())
	{
		if (tmp.right(3) != ".gz")
		{
//			tmp = tmp + tmp.right(3) == "svg" ? "z" : ".gz";
			if (tmp.right(3) == "svg")
				tmp = tmp+"z";
			else
				tmp = tmp+".gz";
		}
		setSelection(tmp);
	}
	else
	{
		int en = tmp.findRev(".gz");
		if (en > 0)
			tmp.remove(en,3);
		else
		{
			en = tmp.findRev("z");
			if (en > 0)
				tmp.remove(en,1);
		}
	}
	setSelection(tmp);
}
