/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QImageReader>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QPushButton>
#include <QTextCodec>
#include <QVBoxLayout>

#include "customfdialog.h"

#include "scfilewidget.h"
#include "cmsettings.h"
#include "commonstrings.h"
#include "sccombobox.h"
#include "scimage.h"
#include "scribusstructs.h"
#include "scslainforeader.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"



extern QString DocDir;

ImIconProvider::ImIconProvider() : QFileIconProvider()
{
	fmts.clear();
	QString tmp[] = {"eps", "epsi", "gif", "png", "jpg", "jpeg", "xpm", "tif", "tiff", "bmp", "pbm", "pgm", "ppm", "xbm", "xpm", "psd", "pat"};
	size_t array = sizeof(tmp) / sizeof(*tmp);
	for (uint a = 0; a < array; ++a)
		fmts.append(tmp[a]);
	imagepm = QIcon(loadIcon("16/image-x-generic.png"));
	pspm = QIcon(loadIcon("postscript.png"));
	txtpm = QIcon(loadIcon("txt.png"));
	docpm = QIcon(loadIcon("doc.png"));
	pdfpm = QIcon(loadIcon("pdf.png"));
	oosxdpm = QIcon(loadIcon("ooo_draw.png"));
	oosxwpm = QIcon(loadIcon("ooo_writer.png"));
	vectorpm = QIcon(loadIcon("vectorgfx.png"));
}

QIcon ImIconProvider::icon(const QFileInfo &fi) const
{
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		return QFileIconProvider::icon(fi);
	if (fmts.contains(ext, Qt::CaseInsensitive))
		return imagepm;
	else
	{
		ext = fi.completeSuffix().toLower();
		if (ext.endsWith("ps", Qt::CaseInsensitive))
			return pspm;
		else if (ext.endsWith("txt", Qt::CaseInsensitive))
			return txtpm;
		else if (ext.endsWith("scd", Qt::CaseInsensitive) || ext.endsWith("scd.gz", Qt::CaseInsensitive))
			return docpm;
		else if (ext.endsWith("sla", Qt::CaseInsensitive) || ext.endsWith("sla.gz", Qt::CaseInsensitive))
			return docpm;
		else if (ext.endsWith("pdf", Qt::CaseInsensitive))
			return pdfpm;
		else if (ext.endsWith("sxd", Qt::CaseInsensitive))
			return oosxdpm;
		else if (ext.endsWith("sxw", Qt::CaseInsensitive))
			return oosxwpm;
		else if (ext.endsWith("svg", Qt::CaseInsensitive) || ext.endsWith("svgz", Qt::CaseInsensitive))
			return vectorpm;
		else
			return QFileIconProvider::icon(fi);
	}
	return QIcon();
}

FDialogPreview::FDialogPreview(QWidget *pa) : QLabel(pa)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setFixedSize( QSize( 200, 200 ) );
	setScaledContents( false );
	setFrameShape( QLabel::WinPanel );
	setFrameShadow( QLabel::Sunken );
	updtPix();
}

void FDialogPreview::updtPix()
{
	QPixmap pm;
	QRect inside = contentsRect();
	pm = QPixmap(inside.width(), inside.height());
	pm.fill(Qt::white);
	setPixmap(pm);
}

void FDialogPreview::GenPreview(QString name)
{
	QPixmap pm;
	QString Buffer = "";
	updtPix();
	if (name.isEmpty())
		return;
	QFileInfo fi = QFileInfo(name);
	if (fi.isDir())
		return;
	int w = pixmap()->width();
	int h = pixmap()->height();
	bool mode = false;
	QString ext = fi.suffix().toLower();
	QList<QByteArray> formats(QImageReader::supportedImageFormats());
// 	FormatsManager::instance()->imageFormatSupported(ext);
	formats.append("jpg");
	formats.append("tif");
	formats.append("tiff");
	formats.append("psd");
	formats.append("eps");
	formats.append("epsi");
	formats.append("pdf");
	formats.append("ps");
	formats.append("pat");
// 	QString allFormats = formats.join( " " );
// 	formats.clear();
// 	allFormats = allFormats.toLower();
// 	formats = QStringList::split( " ", allFormats );
	if (ext.isEmpty())
		ext = getImageType(name);
	if (formats.contains(ext.toUtf8()))
	{
		ScImage im;
		//No doc to send data anyway, so no doc to get into scimage.
		CMSettings cms(0, "", 0);
		if (im.LoadPicture(name, 1, cms, false, false, ScImage::Thumbnail, 72, &mode))
		{
			int ix,iy;
			if ((im.imgInfo.exifDataValid) && (!im.imgInfo.exifInfo.thumbnail.isNull()))
			{
				ix = im.imgInfo.exifInfo.width;
				iy = im.imgInfo.exifInfo.height;
			}
			else
			{
				ix = im.width();
				iy = im.height();
			}
			int xres = im.imgInfo.xres;
			int yres = im.imgInfo.yres;
			QString tmp = "";
			QString tmp2 = "";
			QImage im2;
			if ((ix > w-5) || (iy > h-44))
			{
				double sx = im.width() / static_cast<double>(w-5);
				double sy = im.height() / static_cast<double>(h-44);
				im2 = sy < sx ? im.scaled(qRound(im.width() / sx), qRound(im.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
								: im.scaled(qRound(im.width() / sy), qRound(im.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			}
			else
				im2 = im.qImage(); // no need to copy
			QPainter p;
			QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
			// Qt4 FIXME imho should be better
			pm = *pixmap();
			p.begin(&pm);
			p.fillRect(0, 0, w, h-44, b);
			p.fillRect(0, h-44, w, 44, QColor(255, 255, 255));
			p.drawImage((w - im2.width()) / 2, (h - 44 - im2.height()) / 2, im2);
			p.drawText(2, h-29, tr("Size:")+" "+tmp.setNum(ix)+" x "+tmp2.setNum(iy));
			p.drawText(2, h-17, tr("Resolution:")+" "+tmp.setNum(xres)+" x "+tmp2.setNum(yres)+" "+ tr("DPI"));
			QString cSpace;
			if ((extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext)) && (im.imgInfo.type != ImageType7))
				cSpace = tr("Unknown");
			else
				cSpace=colorSpaceText(im.imgInfo.colorspace);
			p.drawText(2, h-5, tr("Colorspace:")+" "+cSpace);
			p.end();
			setPixmap(pm);
			repaint();
		}
	}
	else
	{
		ScSlaInfoReader slaInfos;
		if (slaInfos.readInfos(name))
		{
			QString Title = tr("Title:")+" ";
			QString ti2 = slaInfos.title();
			if (ti2.isEmpty())
				ti2= tr("No Title");
			Title += ti2+"\n";
			QString Author = tr("Author:")+" ";
			QString au2 = slaInfos.author();
			if (au2.isEmpty())
				au2 = tr("Unknown");
			Author += au2+"\n";
			QString Format =  tr("File Format:")+" ";
			QString fm2 = slaInfos.format();
			if (fm2.isEmpty())
				fm2 = tr("Unknown");
			Format += fm2;
			setText( tr("Scribus Document")+"\n\n"+Title+Author+Format);
		}
		else  if ((ext == "txt") || (ext == "html") || (ext == "xml"))
		{
			if (loadText(name, &Buffer))
				setText(Buffer.left(200));
		}
	}
}

CustomFDialog::CustomFDialog(QWidget *parent, QString wDir, QString caption, QString filter, int flags)
			: QDialog(parent), optionFlags(flags)
{
	setModal(true);
	setWindowTitle(caption);
	setWindowIcon(QIcon(loadIcon("AppIcon.png")));
	vboxLayout = new QVBoxLayout(this);
	vboxLayout->setSpacing(5);
	vboxLayout->setMargin(10);
    hboxLayout = new QHBoxLayout;
	hboxLayout->setSpacing(5);
	hboxLayout->setMargin(0);
	fileDialog = new ScFileWidget(this);
	hboxLayout->addWidget(fileDialog);
	fileDialog->setIconProvider(new ImIconProvider());
	fileDialog->setFilter(filter);
	fileDialog->selectFilter(filter);
	fileDialog->setDirectory(wDir);
	vboxLayout1 = new QVBoxLayout;
	vboxLayout1->setSpacing(0);
	vboxLayout1->setMargin(0);
	vboxLayout1->setContentsMargins(0, 37, 0, 0);
	vboxLayout1->setAlignment( Qt::AlignTop );
	pw = new FDialogPreview( this );
	pw->setMinimumSize(QSize(200, 200));
	pw->setMaximumSize(QSize(200, 200));
	vboxLayout1->addWidget(pw);
	hboxLayout->addLayout(vboxLayout1);
	vboxLayout->addLayout(hboxLayout);
    QHBoxLayout *hboxLayout1 = new QHBoxLayout;
	hboxLayout1->setSpacing(5);
	hboxLayout1->setContentsMargins(9, 0, 0, 0);
	showPreview = new QCheckBox(this);
	showPreview->setText( tr("Show Preview"));
	showPreview->setChecked(true);
	previewIsShown = true;
	hboxLayout1->addWidget(showPreview);
	QSpacerItem *spacerItem = new QSpacerItem(2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);
	hboxLayout1->addItem(spacerItem);
	OKButton = new QPushButton( CommonStrings::tr_OK, this);
	OKButton->setDefault( true );
	hboxLayout1->addWidget( OKButton );
	CancelB = new QPushButton( CommonStrings::tr_Cancel, this);
	CancelB->setAutoDefault( false );
	hboxLayout1->addWidget( CancelB );
	vboxLayout->addLayout(hboxLayout1);
	if (flags & fdDirectoriesOnly)
	{
		Layout = new QFrame(this);
		Layout1 = new QHBoxLayout(Layout);
		Layout1->setSpacing( 0 );
		Layout1->setContentsMargins(9, 0, 0, 0);
		SaveZip = new QCheckBox( tr( "&Compress File" ), Layout);
		Layout1->addWidget(SaveZip, Qt::AlignLeft);
		QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout1->addItem( spacer );
		vboxLayout->addWidget(Layout);
		LayoutC = new QFrame(this);
		Layout1C = new QHBoxLayout(LayoutC);
		Layout1C->setSpacing( 0 );
		Layout1C->setContentsMargins(9, 0, 0, 0);
		WithFonts = new QCheckBox( tr( "&Include Fonts" ), LayoutC);
		Layout1C->addWidget(WithFonts, Qt::AlignLeft);
		WithProfiles = new QCheckBox( tr( "&Include Color Profiles" ), LayoutC);
		Layout1C->addWidget(WithProfiles, Qt::AlignLeft);
		QSpacerItem* spacer2 = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout1C->addItem( spacer2 );
		vboxLayout->addWidget(LayoutC);
		fileDialog->setFileMode(QFileDialog::DirectoryOnly);
		pw->hide();
		showPreview->setVisible(false);
		showPreview->setChecked(false);
		previewIsShown = false;
	}
	else
	{
		if (flags & fdCompressFile)
		{
			Layout = new QFrame(this);
			Layout1 = new QHBoxLayout(Layout);
			Layout1->setSpacing( 5 );
			Layout1->setContentsMargins(9, 0, 0, 0);
			SaveZip = new QCheckBox( tr( "&Compress File" ), Layout);
			Layout1->addWidget(SaveZip);
			QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
			Layout1->addItem( spacer );
		}
		if (flags & fdExistingFiles)
			fileDialog->setFileMode(QFileDialog::ExistingFile);
		else
		{
			fileDialog->setFileMode(QFileDialog::AnyFile);
			if (flags & fdCompressFile)
				vboxLayout->addWidget(Layout);
		}
		if (flags & fdShowCodecs)
		{
			LayoutC = new QFrame(this);
			Layout1C = new QHBoxLayout(LayoutC);
			Layout1C->setSpacing( 0 );
			Layout1C->setContentsMargins(9, 0, 0, 0);
			TxCodeT = new QLabel(this);
			TxCodeT->setText( tr("Encoding:"));
			Layout1C->addWidget(TxCodeT);
			TxCodeM = new ScComboBox(LayoutC);
			TxCodeM->setEditable(false);
			QString tmp_txc[] = {"ISO 8859-1", "ISO 8859-2", "ISO 8859-3",
								"ISO 8859-4", "ISO 8859-5", "ISO 8859-6",
								"ISO 8859-7", "ISO 8859-8", "ISO 8859-9",
								"ISO 8859-10", "ISO 8859-13", "ISO 8859-14",
								"ISO 8859-15", "UTF-8", "UTF-16", "KOI8-R", "KOI8-U",
								"CP1250", "CP1251", "CP1252", "CP1253",
								"CP1254", "CP1255", "CP1256", "CP1257",
								"Apple Roman"};
			size_t array = sizeof(tmp_txc) / sizeof(*tmp_txc);
			for (uint a = 0; a < array; ++a)
				TxCodeM->addItem(tmp_txc[a]);
			QString localEn = QTextCodec::codecForLocale()->name();
			if (localEn == "ISO-10646-UCS-2")
				localEn = "UTF-16";
			bool hasIt = false;
			for (int cc = 0; cc < TxCodeM->count(); ++cc)
			{
				if (TxCodeM->itemText(cc) == localEn)
				{
					TxCodeM->setCurrentIndex(cc);
					hasIt = true;
					break;
				}
			}
			if (!hasIt)
			{
				TxCodeM->addItem(localEn);
				TxCodeM->setCurrentIndex(TxCodeM->count()-1);
			}
			TxCodeM->setMinimumSize(QSize(200, 0));
			Layout1C->addWidget(TxCodeM);
			QSpacerItem* spacer2 = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
			Layout1C->addItem( spacer2 );
			vboxLayout->addWidget(LayoutC);
		}
		bool setter2 = flags & fdHidePreviewCheckBox;
		if (!setter2)
		{
			bool setter = flags & fdShowPreview;
			showPreview->setChecked(setter);
			previewIsShown = setter;
			pw->setVisible(setter);
		}
		else
		{
			showPreview->hide();
			previewIsShown = false;
			pw->setVisible(false);
		}
		if (flags & fdCompressFile)
			connect(SaveZip, SIGNAL(clicked()), this, SLOT(handleCompress()));
	}
	extZip = "gz";
	connect(OKButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(CancelB, SIGNAL(clicked()), this, SLOT(reject()));
	connect(showPreview, SIGNAL(clicked()), this, SLOT(togglePreview()));
	connect(fileDialog, SIGNAL(currentChanged(const QString &)), this, SLOT(fileClicked(const QString &)));
	connect(fileDialog, SIGNAL(filesSelected(const QStringList &)), this, SLOT(accept()));
	connect(fileDialog, SIGNAL(accepted()), this, SLOT(accept()));
	connect(fileDialog, SIGNAL(rejected()), this, SLOT(reject()));
	resize(minimumSizeHint());
}


void CustomFDialog::fileClicked(const QString &path)
{
	if (previewIsShown)
		pw->GenPreview(path);
}

void CustomFDialog::togglePreview()
{
	previewIsShown = !previewIsShown;
	pw->setVisible(previewIsShown);
}

void CustomFDialog::setSelection(QString sel)
{
	fileDialog->selectFile( QFileInfo(sel).fileName() );
	if (previewIsShown)
		pw->GenPreview(sel);
}

QString CustomFDialog::selectedFile()
{
	QStringList sel = fileDialog->selectedFiles();
	if (!sel.isEmpty())
		return sel[0];
	return QString();
}

void CustomFDialog::addWidgets(QWidget *widgets)
{
	vboxLayout->addWidget(widgets);
}

CustomFDialog::~CustomFDialog()
{
}

void CustomFDialog::handleCompress()
{
	QFileInfo tmp;
	tmp.setFile(selectedFile());
	QString e(tmp.completeSuffix());
	QStringList ex = e.split(".", QString::SkipEmptyParts);
	QString baseExt = "";
	for (int a = 0; a < ex.count(); a++)
	{
		if ((ex[a] != "sla") && (ex[a] != "SLA") && (ex[a] != "gz") && (ex[a] != "GZ"))
			baseExt += "."+ex[a];
	}
	if (SaveZip->isChecked())
	{
		if (e != extZip)
			tmp.setFile(tmp.baseName() + baseExt + "." + extZip);
	}
	else
	{
		if (e != ext)
			tmp.setFile(tmp.baseName() + baseExt + "." + ext);
	}
	setSelection(tmp.fileName());
}

void CustomFDialog::setExtension(QString e)
{
	ext = e;
}

QString CustomFDialog::extension()
{
	return ext;
}

void CustomFDialog::setZipExtension(QString e)
{
	extZip = e;
}

QString CustomFDialog::zipExtension()
{
	return extZip;
}
