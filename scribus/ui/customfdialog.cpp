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

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QImageReader>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QTextCodec>
#include <QVBoxLayout>

#include "customfdialog.h"

#include "../plugins/formatidlist.h"
#include "cmsettings.h"
#include "commonstrings.h"
#include "fileloader.h"
#include "iconmanager.h"
#include "loadsaveplugin.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scfilewidget.h"
#include "scimage.h"
#include "scpreview.h"
#include "scribusstructs.h"
#include "scslainforeader.h"
#include "units.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"

ImIconProvider::ImIconProvider()
{
	QString tmp[] = {"eps", "epsi", "gif", "png", "jpg", "jpeg", "xpm", "tif", "tiff", "bmp", "pbm", "pgm", "ppm", "xbm", "xpm", "psd", "pat"};
	size_t arraySize = sizeof(tmp) / sizeof(*tmp);
	fmts.reserve(arraySize);
	for (size_t i = 0; i < arraySize; ++i)
		fmts.append(tmp[i]);
	IconManager& im = IconManager::instance();
	imagepm = im.loadIcon("16/image-x-generic.png");
	pspm = im.loadIcon("postscript.png");
	txtpm = im.loadIcon("txt.png");
	docpm = im.loadIcon("doc.png");
	pdfpm = im.loadIcon("pdf.png");
	oosxdpm = im.loadIcon("ooo_draw.png");
	oosxwpm = im.loadIcon("ooo_writer.png");
	vectorpm = im.loadIcon("vectorgfx.png");
}

QIcon ImIconProvider::icon(const QFileInfo &fi) const
{
	QStringList allFormatsV = LoadSavePlugin::getExtensionsForImport(FORMATID_FIRSTUSER);
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		return QFileIconProvider::icon(fi);
	if (fmts.contains(ext, Qt::CaseInsensitive))
		return imagepm;
	ext = fi.completeSuffix().toLower();
	if (ext.endsWith("ps", Qt::CaseInsensitive))
		return pspm;
	if (ext.endsWith("txt", Qt::CaseInsensitive))
		return txtpm;
	if (ext.endsWith("scd", Qt::CaseInsensitive) || ext.endsWith("scd.gz", Qt::CaseInsensitive))
		return docpm;
	if (ext.endsWith("sla", Qt::CaseInsensitive) || ext.endsWith("sla.gz", Qt::CaseInsensitive))
		return docpm;
	if (ext.endsWith("pdf", Qt::CaseInsensitive))
		return pdfpm;
	if (ext.endsWith("sxd", Qt::CaseInsensitive))
		return oosxdpm;
	if (ext.endsWith("sxw", Qt::CaseInsensitive))
		return oosxwpm;
	if (allFormatsV.contains(ext) || ext.endsWith("sce", Qt::CaseInsensitive) || ext.endsWith("shape", Qt::CaseInsensitive))
		return vectorpm;
	return QFileIconProvider::icon(fi);
}

FDialogPreview::FDialogPreview(QWidget *pa) : QLabel(pa)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setFixedSize( QSize( 200, 200 ) );
	setScaledContents( false );
	setFrameShape( QLabel::WinPanel );
	setFrameShadow( QLabel::Sunken );
	updatePix();
}

void FDialogPreview::updatePix()
{
	QPixmap pm;
	QRect inside = contentsRect();
	pm = QPixmap(inside.width(), inside.height());
	pm.fill(Qt::white);
	setPixmap(pm);
}

void FDialogPreview::genPreview(const QString& name)
{
	QPixmap pm;
	QString Buffer;
	updatePix();
	if (name.isEmpty())
		return;
	QFileInfo fi(name);
	if (fi.isDir())
		return;
	int w = pixmap()->width();
	int h = pixmap()->height();
	bool mode = false;
	QString ext = fi.suffix().toLower();
	QString formatD(FormatsManager::instance()->extensionListForFormat(FormatsManager::IMAGESIMGFRAME, 1));
 	QStringList formats = formatD.split("|");
	formats.append("pat");
	formats.removeAll("pdf");
	
	QStringList allFormatsV = LoadSavePlugin::getExtensionsForPreview(FORMATID_FIRSTUSER);
	if (ext.isEmpty())
		ext = getImageType(name);
	if (formats.contains(ext.toUtf8()))
	{
		ScImage im;
		//No doc to send data anyway, so no doc to get into scimage.
		CMSettings cms(nullptr, "", Intent_Perceptual);
		cms.allowColorManagement(false);
		if (!im.loadPicture(name, 1, cms, ScImage::Thumbnail, 72, &mode))
			return;
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
		QString tmp;
		QString tmp2;
		QImage im2 = im.scaled(w - 5, h - 44, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QPainter p;
		QBrush b(QColor(205, 205, 205), IconManager::instance().loadPixmap("testfill.png"));
		// Qt4 FIXME imho should be better
		pm = *pixmap();
		p.begin(&pm);
		p.fillRect(0, 0, w, h - 44, b);
		p.fillRect(0, h - 44, w, 44, QColor(255, 255, 255));
		p.drawImage((w - im2.width()) / 2, (h - 44 - im2.height()) / 2, im2);
		p.drawText(2, h - 29, tr("Size:") + " " + tmp.setNum(ix) + " x " + tmp2.setNum(iy));
		if (!(extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext)))
			p.drawText(2, h - 17, tr("Resolution:") + " " + tmp.setNum(xres) + " x " + tmp2.setNum(yres) + " " + tr("DPI"));
		QString cSpace;
		if ((extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext)) && (im.imgInfo.type != ImageType7))
			cSpace = tr("Unknown");
		else
			cSpace = colorSpaceText(im.imgInfo.colorspace);
		p.drawText(2, h - 5, tr("Colorspace:") + " " + cSpace);
		p.end();
		setPixmap(pm);
		repaint();
	}
	else if (allFormatsV.contains(ext.toUtf8()))
	{
		FileLoader *fileLoader = new FileLoader(name);
		int testResult = fileLoader->testFile();
		delete fileLoader;
		if ((testResult == -1) || (testResult < FORMATID_FIRSTUSER))
			return;
		const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
		if (!fmt)
			return;
		QImage im = fmt->readThumbnail(name);
		if (im.isNull())
			return;
		QString desc = tr("Size:")+" ";
		desc += value2String(im.text("XSize").toDouble(), PrefsManager::instance().appPrefs.docSetupPrefs.docUnitIndex, true, true);
		desc += " x ";
		desc += value2String(im.text("YSize").toDouble(), PrefsManager::instance().appPrefs.docSetupPrefs.docUnitIndex, true, true);
		im = im.scaled(w - 5, h - 21, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QPainter p;
		QBrush b(QColor(205, 205, 205), IconManager::instance().loadPixmap("testfill.png"));
		pm = *pixmap();
		p.begin(&pm);
		p.fillRect(0, 0, w, h - 21, b);
		p.fillRect(0, h - 21, w, 21, QColor(255, 255, 255));
		p.drawImage((w - im.width()) / 2, (h - 21 - im.height()) / 2, im);
		p.drawText(2, h - 5, desc);
		p.end();
		setPixmap(pm);
		repaint();
	}
	else if (ext.toUtf8() == "sce")
	{
		QByteArray cf;
		if (!loadRawText(name, cf))
			return;
		QString f;
		if (cf.left(16) == "<SCRIBUSELEMUTF8")
			f = QString::fromUtf8(cf.data());
		else
			f = cf.data();
		QImage im = ScPreview::create(f);
		im = im.scaled(w - 5, h - 21, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QPainter p;
		QBrush b(QColor(205, 205, 205), IconManager::instance().loadPixmap("testfill.png"));
		pm = *pixmap();
		p.begin(&pm);
		p.fillRect(0, 0, w, h - 21, b);
		p.fillRect(0, h - 21, w, 21, QColor(255, 255, 255));
		p.drawImage((w - im.width()) / 2, (h - 21 - im.height()) / 2, im);
		QString desc = tr("Size:") + QString(" %1 x %2").arg(im.width()).arg(im.height());
		p.drawText(2, h - 5, desc);
		p.end();
		setPixmap(pm);
		repaint();
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
			Title += ti2 + "\n";
			QString Author = tr("Author:")+" ";
			QString au2 = slaInfos.author();
			if (au2.isEmpty())
				au2 = tr("Unknown");
			Author += au2 + "\n";
			QString Format =  tr("File Format:")+" ";
			QString fm2 = slaInfos.format();
			if (fm2.isEmpty())
				fm2 = tr("Unknown");
			Format += fm2;
			setText( tr("Scribus Document") + "\n\n" + Title + Author + Format);
		}
		else  if ((ext == "txt") || (ext == "html") || (ext == "xml"))
		{
			if (loadText(name, &Buffer))
				setText(Buffer.left(200));
		}
	}
}

CustomFDialog::CustomFDialog(QWidget *parent, const QString& wDir, const QString& caption, const QString& filter, int flags)
			: QDialog(parent),
	          m_optionFlags(flags),
	          m_previewIsShown(true)
{
	m_fileDialogPrefs = PrefsManager::instance().prefsFile->getContext("customfdialog", false);

	setModal(true);
	setWindowTitle(caption);
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	vboxLayout = new QVBoxLayout(this);
	vboxLayout->setSpacing(6);
	vboxLayout->setContentsMargins(9, 9, 9, 9);
	hboxLayout = new QHBoxLayout;
	hboxLayout->setSpacing(6);
	hboxLayout->setContentsMargins(0, 0, 0, 0);
	fileDialog = new ScFileWidget(this);
	fileDialog->setIconProvider(new ImIconProvider());
	fileDialog->setNameFilter(filter);
	fileDialog->selectNameFilter(filter);
	fileDialog->setDirectory(wDir);
	hboxLayout->addWidget(fileDialog);
	vboxLayout1 = new QVBoxLayout;
	vboxLayout1->setSpacing(6);
	vboxLayout1->setContentsMargins(0, 28, 0, 0);
	vboxLayout1->setAlignment( Qt::AlignTop );
	filePreview = new FDialogPreview( this );
	filePreview->setMinimumSize(QSize(200, 200));
	filePreview->setMaximumSize(QSize(200, 200));
	vboxLayout1->addWidget(filePreview);
	hboxLayout->addLayout(vboxLayout1);
	vboxLayout->addLayout(hboxLayout);
	QHBoxLayout *hboxLayout1 = new QHBoxLayout;
	hboxLayout1->setSpacing(6);
	hboxLayout1->setContentsMargins(0, 0, 0, 0);
	showPreview = new QCheckBox(this);
	showPreview->setText( tr("Show Preview"));
	showPreview->setToolTip( tr("Show a preview and information for the selected file"));
	showPreview->setChecked(true);
	hboxLayout1->addWidget(showPreview);
	QSpacerItem *spacerItem = new QSpacerItem(2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);
	hboxLayout1->addItem(spacerItem);
	okButton = new QPushButton( CommonStrings::tr_OK, this);
	okButton->setDefault( true );
	hboxLayout1->addWidget( okButton );
	if (flags & fdDisableOk)
		okButton->setEnabled(false);
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this);
	cancelButton->setAutoDefault( false );
	hboxLayout1->addWidget( cancelButton );
	vboxLayout->addLayout(hboxLayout1);

	if (flags & fdDirectoriesOnly)
	{
		Layout = new QFrame(this);
		Layout1 = new QHBoxLayout(Layout);
		Layout1->setSpacing(6);
		Layout1->setContentsMargins(0, 0, 0, 0);
		saveZip = new QCheckBox( tr( "&Compress File" ), Layout);
		Layout1->addWidget(saveZip, Qt::AlignLeft);
		QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout1->addItem( spacer );
		vboxLayout->addWidget(Layout);
		LayoutC = new QFrame(this);
		Layout1C = new QHBoxLayout(LayoutC);
		Layout1C->setSpacing(6);
		Layout1C->setContentsMargins(0, 0, 0, 0);
		withFonts = new QCheckBox( tr( "&Include Fonts" ), LayoutC);
		Layout1C->addWidget(withFonts, Qt::AlignLeft);
		withProfiles = new QCheckBox( tr( "&Include Color Profiles" ), LayoutC);
		Layout1C->addWidget(withProfiles, Qt::AlignLeft);
		QSpacerItem* spacer2 = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout1C->addItem( spacer2 );
		vboxLayout->addWidget(LayoutC);
		fileDialog->setFileMode(QFileDialog::DirectoryOnly);
		fileDialog->forceDoubleClickActivation(false);
		filePreview->hide();
		showPreview->setVisible(false);
		showPreview->setChecked(false);
		m_previewIsShown = false;
	}
	else
	{
		if (flags & fdCompressFile)
		{
			Layout = new QFrame(this);
			Layout1 = new QHBoxLayout(Layout);
			Layout1->setSpacing(6);
			Layout1->setContentsMargins(0, 0, 0, 0);
			saveZip = new QCheckBox( tr( "&Compress File" ), Layout);
			Layout1->addWidget(saveZip);
			QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
			Layout1->addItem( spacer );
		}
		if (flags & fdExistingFiles)
			fileDialog->setFileMode(QFileDialog::ExistingFile);
		else if (flags & fdExistingFilesI)
			fileDialog->setFileMode(QFileDialog::ExistingFiles);
		else
		{
			fileDialog->setFileMode(QFileDialog::AnyFile);
			if (flags & fdCompressFile)
				vboxLayout->addWidget(Layout);
		}
		
		if (saveZip != nullptr)
			saveZip->setToolTip( "<qt>" + tr( "Compress the Scribus document on save" ) + "</qt>");
		if (withFonts != nullptr)
			withFonts->setToolTip( "<qt>" + tr( "Include fonts when collecting files for the document. Be sure to know and understand licensing information for any fonts you collect and possibly redistribute." ) + "</qt>");
		if (withProfiles != nullptr)
			withProfiles->setToolTip( "<qt>" + tr( "Include color profiles when collecting files for the document" ) + "</qt>");
		
		if (flags & fdShowCodecs)
		{
			LayoutC = new QFrame(this);
			Layout1C = new QHBoxLayout(LayoutC);
			Layout1C->setSpacing(6);
			Layout1C->setContentsMargins(0, 0, 0, 0);
			optionLabel = new QLabel(this);
			optionLabel->setText( tr("Encoding:"));
			Layout1C->addWidget(optionLabel);
			optionCombo = new QComboBox(LayoutC);
			optionCombo->setEditable(false);
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
				optionCombo->addItem(tmp_txc[a]);
			QString localEn = QTextCodec::codecForLocale()->name();
			if (localEn == "ISO-10646-UCS-2")
				localEn = "UTF-16";
			bool hasIt = false;
			for (int cc = 0; cc < optionCombo->count(); ++cc)
			{
				if (optionCombo->itemText(cc) == localEn)
				{
					optionCombo->setCurrentIndex(cc);
					hasIt = true;
					break;
				}
			}
			if (!hasIt)
			{
				optionCombo->addItem(localEn);
				optionCombo->setCurrentIndex(optionCombo->count()-1);
			}
			optionCombo->setMinimumSize(QSize(200, 0));
			Layout1C->addWidget(optionCombo);
			QSpacerItem* spacer2 = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
			Layout1C->addItem( spacer2 );
			vboxLayout->addWidget(LayoutC);
		}
		if (flags & fdShowImportOptions)
		{
			LayoutC = new QFrame(this);
			Layout1C = new QHBoxLayout(LayoutC);
			Layout1C->setSpacing(6);
			Layout1C->setContentsMargins(0, 0, 0, 0);
			optionLabel = new QLabel(this);
			optionLabel->setText( tr("Import Option:"));
			Layout1C->addWidget(optionLabel);
			optionCombo = new QComboBox(LayoutC);
			optionCombo->setEditable(false);
			optionCombo->addItem( tr("Keep original size"));
			optionCombo->addItem( tr("Downscale to page size"));
			optionCombo->addItem( tr("Upscale to page size"));
			optionCombo->setCurrentIndex(0);
			optionCombo->setMinimumSize(QSize(200, 0));
			Layout1C->addWidget(optionCombo);
			QSpacerItem* spacer2 = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
			Layout1C->addItem( spacer2 );
			vboxLayout->addWidget(LayoutC);
		}
		bool setter2 = flags & fdHidePreviewCheckBox;
		if (!setter2)
		{
			bool setter = flags & fdShowPreview;
			showPreview->setChecked(setter);
			m_previewIsShown = setter;
			filePreview->setVisible(setter);
		}
		else
		{
			showPreview->hide();
			m_previewIsShown = false;
			filePreview->setVisible(false);
		}
		if (flags & fdCompressFile)
			connect(saveZip, SIGNAL(clicked()), this, SLOT(handleCompress()));
	}
	fileDialog->forceDoubleClickActivation(m_previewIsShown);
	fileDialog->setOption(QFileDialog::HideNameFilterDetails, true);
	m_extZip = "gz";

	connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(showPreview, SIGNAL(clicked()), this, SLOT(togglePreview()));
	connect(fileDialog, SIGNAL(currentChanged(const QString &)), this, SLOT(fileClicked(const QString &)));
	connect(fileDialog, SIGNAL(filesSelected(const QStringList &)), this, SLOT(accept()));
	connect(fileDialog, SIGNAL(accepted()), this, SLOT(accept()));
	connect(fileDialog, SIGNAL(rejected()), this, SLOT(reject()));
}

void CustomFDialog::closeEvent(QCloseEvent *closeEvent)
{
	storeSize();
	QDialog::closeEvent(closeEvent);
}

void CustomFDialog::hideEvent(QHideEvent* hideEvent)
{
	storeSize();
	QDialog::hideEvent(hideEvent);
}

void CustomFDialog::showEvent(QShowEvent *showEvent)
{
	const QScreen* dialogScreen = this->screen();
	if (m_fileDialogPrefs && dialogScreen && !showEvent->spontaneous())
	{
		if (m_fileDialogPrefs->contains("width"))
		{
			QRect scr = dialogScreen->availableGeometry();
			int fdWidth  = qMax(0, qMin(m_fileDialogPrefs->getInt("width"), scr.width()));
			int fdHeight = qMax(0, qMin(m_fileDialogPrefs->getInt("height"), scr.height()));
			if (fdWidth > 0 && fdHeight > 0)
			{
				int newWidth = qMax(0, qMin(this->width() + fdWidth - fileDialog->width(), scr.width()));
				int newHeight = qMax(0, qMin(this->height() + fdHeight - fileDialog->height(), scr.height()));
				if (newWidth > 0 && newHeight > 0)
					resize(newWidth, newHeight);
			}
		}
	}
	QDialog::showEvent(showEvent);
}

void CustomFDialog::storeSize()
{
	if (!m_fileDialogPrefs)
		return;
	m_fileDialogPrefs->set("width",  fileDialog->width());
	m_fileDialogPrefs->set("height", fileDialog->height());
}

void CustomFDialog::fileClicked(const QString &path)
{
	if (m_optionFlags & fdDisableOk)
		okButton->setEnabled(!path.isEmpty());
	if (m_previewIsShown)
		filePreview->genPreview(path);
}

void CustomFDialog::okClicked()
{
	QString selFile = selectedFile();
	if (selFile.isEmpty())
		return;
	QFileInfo fi(selFile);
	if (fi.isDir() && (fileDialog->fileMode() != QFileDialog::DirectoryOnly))
		fileDialog->gotoSelectedDirectory();
	else
		accept();
}

void CustomFDialog::togglePreview()
{
	m_previewIsShown = !m_previewIsShown;
	fileDialog->forceDoubleClickActivation(m_previewIsShown);
	filePreview->setVisible(m_previewIsShown);
	if (m_previewIsShown)
	{
		QStringList sel = fileDialog->selectedFiles();
		if (!sel.isEmpty())
			filePreview->genPreview(QDir::fromNativeSeparators(sel[0]));
	}
	// #11856: Hack to avoid file dialog widget turning black with Qt5
	QCoreApplication::processEvents();
	filePreview->setVisible(!m_previewIsShown);
	QCoreApplication::processEvents();
	filePreview->setVisible(m_previewIsShown);
	fileDialog->repaint();
	QCoreApplication::processEvents();
	repaint();
}

void CustomFDialog::setSelection(const QString& fileName)
{
	fileDialog->selectFile( QFileInfo(fileName).fileName() );
	if (m_previewIsShown)
		filePreview->genPreview(fileName);
}

QString CustomFDialog::selectedFile() const
{
	QStringList sel = fileDialog->selectedFiles();
	if (!sel.isEmpty())
		return QDir::fromNativeSeparators(sel[0]);
	return QString();
}

QStringList CustomFDialog::selectedFiles() const
{
	return fileDialog->selectedFiles();
}

void CustomFDialog::addWidgets(QWidget *widgets)
{
	vboxLayout->addWidget(widgets);
}

void CustomFDialog::handleCompress()
{
	QString   fileName;
	QFileInfo tmp(selectedFile());
	QString   fn(tmp.fileName());
	QStringList fc = fn.split(".", Qt::KeepEmptyParts);
	if (fc.count() > 0)
		fileName = fc.at(0);
	for (int a = 1; a < fc.count(); a++)
	{
		if (fc.at(a).compare("sla", Qt::CaseInsensitive) == 0)
			continue;
		if (fc.at(a).compare("gz", Qt::CaseInsensitive) == 0)
			continue;
		if (fc.at(a).compare(m_ext, Qt::CaseInsensitive) == 0)
			continue;
		if (fc.at(a).compare(m_extZip, Qt::CaseInsensitive) == 0)
			continue;
		fileName += "." + fc[a];
	}
	if (saveZip->isChecked())
		tmp.setFile(fileName + "." + m_extZip);
	else
		tmp.setFile(fileName + "." + m_ext);
	setSelection(tmp.fileName());
}

void CustomFDialog::setExtension(const QString& e)
{
	m_ext = e;
}

const QString&  CustomFDialog::extension() const
{
	return m_ext;
}

void CustomFDialog::setZipExtension(const QString& e)
{
	m_extZip = e;
}

const QString& CustomFDialog::zipExtension() const
{
	return m_extZip;
}

bool CustomFDialog::isTextCodecShown() const
{
	if ((m_optionFlags & fdShowCodecs) == 0)
		return false;
	if (optionCombo)
		return optionCombo->isVisible();
	return false;
}

void CustomFDialog::setTextCodec(const QString& textCodec)
{
	if ((m_optionFlags & fdShowCodecs) == 0)
		return;
	if (!optionCombo)
		return;

	QString textEncoding = textCodec;
	if (textCodec.isEmpty())
		textEncoding = "System";

	int index = optionCombo->findText(textEncoding, Qt::MatchFixedString);
	if (index < 0)
		return;
	optionCombo->setCurrentIndex(index);
}

QString CustomFDialog::textCodec() const
{
	if ((m_optionFlags & fdShowCodecs) == 0)
		return QString();
	if (optionCombo)
		return optionCombo->currentText();
	return QString();
}

bool CustomFDialog::isSaveZipFileShown() const
{
	if (saveZip)
		return saveZip->isVisible();
	return false;
}

void CustomFDialog::setSaveZipFile(bool value)
{
	if (!saveZip)
		return;
	saveZip->setChecked(value);
}

bool CustomFDialog::saveZipFile() const
{
	if (saveZip)
		return saveZip->isChecked();
	return false;
}

bool CustomFDialog::isIncludeFontsShown() const
{
	if (withFonts)
		return withFonts->isVisible();
	return false;
}

void CustomFDialog::setIncludeFonts(bool value)
{
	if (!withFonts)
		return;
	withFonts->setChecked(value);
}

bool CustomFDialog::includeFonts() const
{
	if (withFonts)
		return withFonts->isChecked();
	return false;
}

bool CustomFDialog::isIncludeProfilesShown() const
{
	if (withProfiles)
		return withProfiles->isVisible();
	return false;
}

void CustomFDialog::setIncludeProfiles(bool value)
{
	if (!withProfiles)
		return;
	withProfiles->setChecked(value);
}

bool CustomFDialog::includeProfiles() const
{
	if (withProfiles)
		return withProfiles->isChecked();
	return false;
}

int CustomFDialog::currentOptionIndex() const
{
	if (optionCombo)
		return optionCombo->currentIndex();
	return -1;
}
