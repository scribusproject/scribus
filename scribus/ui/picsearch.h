/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PICSEARCH_H
#define PICSEARCH_H

#include <QDialog>
#include <QListWidgetItem>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QToolTip>
#include <QStringList>

#include "scribusapi.h"
#include "ui_picsearch.h"


/*! \brief Dialog for selecting one of all images founded.
It's called after image search Extras/Manage Pictures/ Click [Search] button for chosen
image frame item. */
class SCRIBUS_API PicSearch : public QDialog, Ui::PicSearch
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\brief Constructor for PicSearch.[dox?]. Used in Extras / Manage Pictures / Search function
	\param parent QWidget pointer to parent window
	\param fileName QString name of image
	\param avalableFiles QStringList List of Paths where an Image with the given Name is present
	*/
	PicSearch( QWidget* parent, const QString & fileName, const QStringList & avalableFiles, bool brokenLinksOnly);
	~PicSearch() {};

	//! \brief Selected image with its full path
	QString getSelectedImage() { return foundFilesBox->currentItem()->text(); }
	//! \brief Matching images should also be replaced?
	bool isApplyToMatchingImages() { return matchCheckBox->isChecked(); }

private slots:
	/*!
	\author Franz Schmid
	\brief If preview is desired (checked) then the image preview is shown and generated, otherwise hidden.
	*/
	void previewCheckBox_clicked();
	/*!
	\author Ale Rimoldi
	\brief If matching is desired (checked) then show a warning when only broken linked images are matched.
	*/
	void matchCheckBox_clicked();
	/*!
	\author Franz Schmid
	\brief When image is selected from the ListBox then the image preview may be shown and the Use button is enabled.
	\param c QListBoxItem
	*/
	void foundFilesBox_clicked(QListWidgetItem *c);
	/*!
	\author Franz Schmid
	\brief Generates image preview for the found Picture
	*/
	void createPreview();

signals:
	//! \brief Emitted when the pic for a row is replaced. Arg is row index.
	void rowPicChanged(unsigned int);

private:
	QSize minS;
	bool brokenLinksOnly;
};

#endif // PICSEARCH_H
