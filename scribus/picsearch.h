/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PICSEARCH_H
#define PICSEARCH_H

#include <qdialog.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qstringlist.h>

#include "scribusapi.h"

class SCRIBUS_API PicSearch : public QDialog
{ 
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\brief Constructor for PicSearch.[dox?]. Used in Extras / Manage Pictures / Search function
	\param parent QWidget pointer to parent window
	\param fileName QString name of image
	\param alt QStringList List of Paths where an Image with the given Name is present
	*/
	PicSearch( QWidget* parent, const QString & fileName, const QStringList & alt);
	~PicSearch() {};

	QLabel* TextLabel1;
	QLabel* Datei;
	QListBox* ListBox1;
	QCheckBox* Preview;
	QPushButton* UseB;
	QPushButton* CancelB;
	QLabel* PixmapLabel1;
	QSize minS;
	QString Bild;

private slots:
	/*!
	\author Franz Schmid
	\brief If preview is desired (checked) then the image preview is shown and generated, otherwise hidden.
	*/
	void ShowPrev();
	/*!
	\author Franz Schmid
	\brief When image is selected from the ListBox then the image preview may be shown and the Use button is enabled.
	\param c QListBoxItem
	*/
	void selBild(QListBoxItem *c);
	/*!
	\author Franz Schmid
	\brief Generates image preview for the found Picture
	*/
	void GenPreview();

signals:
	//! \brief Emitted when the pic for a row is replaced. Arg is row index.
	void rowPicChanged(unsigned int);

protected:
	QVBoxLayout* PicSearchLayout;
	QHBoxLayout* Layout2;
	QHBoxLayout* Layout3;
	QVBoxLayout* Layout1;
};

#endif // PICSEARCH_H
