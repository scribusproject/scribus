/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "picsearch.h"
#include <QPixmap>
#include <QPainter>

#include "cmsettings.h"
#include "commonstrings.h"
#include "scimage.h"
#include "scpaths.h"
#include "scribusstructs.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"


PicSearch::PicSearch(QWidget* parent, const QString & fileName, const QStringList & avalableFiles) : QDialog(parent), currentImage(QString())
{
	setupUi(this);
	setModal(true);
	cancelButton->setText(CommonStrings::tr_Cancel);
	previewLabel->hide();
	fileNameLabel->setText(fileName);

	for (int i = 0; i < avalableFiles.count(); ++i)
		foundFilesBox->addItem( QDir::convertSeparators(avalableFiles[i]) );

	// signals and slots connections
	connect(cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(useButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect(previewCheckBox, SIGNAL( clicked() ), this, SLOT( previewCheckBox_clicked() ) );
	connect(foundFilesBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(foundFilesBox_clicked(QListWidgetItem*)));
}

void PicSearch::previewCheckBox_clicked()
{
	if (previewCheckBox->isChecked())
	{
		previewLabel->show();
		if (!currentImage.isEmpty())
			createPreview();
	}
	else
		previewLabel->hide();
}

void PicSearch::foundFilesBox_clicked(QListWidgetItem *c)
{
	if (c == NULL)
		return;
	currentImage = ScPaths::separatorsToSlashes(c->text());
	if (previewCheckBox->isChecked())
		createPreview();
	useButton->setEnabled(true);
}

void PicSearch::createPreview()
{
	QPixmap pm(200, 200);
	QFileInfo fi = QFileInfo(currentImage);
	int w = 200;
	int h = 200;
	bool mode = false;
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		ext = getImageType(currentImage);
	ScImage im;
	//No doc to send data anyway, so no doc to get into scimage.
	CMSettings cms(0, "", 0);
	if (im.LoadPicture(currentImage, cms, false, false, ScImage::Thumbnail, 72, &mode))
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
		repaint();
	}
	previewLabel->setPixmap(pm);
}
