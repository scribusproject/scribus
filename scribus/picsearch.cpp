/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "picsearch.h"
#include "picsearch.moc"
#include <qpixmap.h>
#include "scribusstructs.h"
#include "scimage.h"
#include "commonstrings.h"
#include "scpaths.h"

extern QPixmap loadIcon(QString nam);


PicSearch::PicSearch(QWidget* parent, const QString & fileName, const QStringList & avalableFiles)
	: PicSearchBase( parent, "PicSearch", true, 0 ),
	currentImage(QString())
{
	setIcon(loadIcon("AppIcon.png"));
	cancelButton->setText(CommonStrings::tr_Cancel);
	previewLabel->hide();
	fileNameLabel->setText(fileName);

	for (int i = 0; i < avalableFiles.count(); ++i)
		foundFilesBox->insertItem( QDir::convertSeparators(avalableFiles[i]) );

	// signals and slots connections
	connect(cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect(useButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect(previewCheckBox, SIGNAL( clicked() ), this, SLOT( previewCheckBox_clicked() ) );
	connect(foundFilesBox, SIGNAL(clicked(QListBoxItem*)), this, SLOT(foundFilesBox_clicked(QListBoxItem*)));
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

void PicSearch::foundFilesBox_clicked(QListBoxItem *c)
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
	QPixmap pm;
	ScImage im;
	CMSettings cms(0, "", 0);
	im.LoadPicture(currentImage, cms, false, false, ScImage::RGBData, 72);
	QImage im2;
	if ((im.width() > 200) || (im.height() > 200))
	{
		double sx = im.width() / 200.0;
		double sy = im.height() / 200.0;
		im2 = sy < sx ?
			im2 = im.smoothScale(qRound(im.width() / sx), qRound(im.height() / sx)) :
			im2 = im.smoothScale(qRound(im.width() / sy), qRound(im.height() / sy));
	}
	else
		im2 = im.qImage();
	pm.convertFromImage(im2);
	previewLabel->setPixmap(pm);
}
