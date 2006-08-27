/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          customfdialog.h  -  description
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

#ifndef CUSTOMFDIALOG_H
#define CUSTOMFDIALOG_H

#include <qfiledialog.h>
#include <qurl.h>
#include <qlabel.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qframe.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qstring.h>

#include "scribusapi.h"

/**
  *@author Franz Schmid
  */
class SCRIBUS_API ImIconProvider : public QFileIconProvider
{
    Q_OBJECT
    QStringList fmts;
    QPixmap imagepm;
    QPixmap docpm;
    QPixmap pspm;
    QPixmap pdfpm;
    QPixmap txtpm;
    QPixmap oosxdpm;
    QPixmap oosxwpm;
    QPixmap vectorpm;
public:
    ImIconProvider(QWidget *pa);
    ~ImIconProvider() {};

    const QPixmap * pixmap(const QFileInfo &fi);
};

class SCRIBUS_API FDialogPreview : public QLabel, public QFilePreview
{
    Q_OBJECT
protected:
	QString filePath;
public:
	FDialogPreview(QWidget *pa);
	~FDialogPreview() {};
	void updtPix();
	void GenPreview(QString name);
	virtual void previewUrl(const QUrl &url);
};

typedef enum {
	fdNone = 0,
	fdShowPreview = 1, // display text/image previewer
	fdExistingFiles = 2, // When set, set to the QFileDialog::ExistingFile mode when true, QFileDialog::AnyFile otherwise
	fdCompressFile = 4, // Show and handle the "Compress" behaviour and CheckBox (true)
	fdShowCodecs = 8, // Show and handle text codecs and ComboBox (true)
	fdDirectoriesOnly = 16 // Show only directories
} fdFlags;

/*! \brief A Scribus own file dialog.
Used almost everywhere. You can see extension handling
in e.g. bool SVGExportPlugin::run(QString filename).
*/
class SCRIBUS_API CustomFDialog : public QFileDialog
{
    Q_OBJECT
public:
	/*! \brief Create Scribus file dialog.
	\param parent a reference to the QWidget parent.
	\param wDir starting working directory
	\param caption text caption of the dialog window
	\param filter a mask/filter. E.g.: *.txt
	\param flags combination of fdFlags, default to fdExistingFiles
	*/
	CustomFDialog(QWidget *parent, QString wDir, QString caption = "",  QString filter = "",
				int flags = fdExistingFiles);
	~CustomFDialog();

	/*! \brief Set the default extension of the resulting file name.
	\param e string extension without any "."
	*/
	void setExtension(QString e);
	/*! \brief Get the file extension
	\retval QString see setExtension(QString e) */
	QString extension();
	/*! \brief Set the "compress" extension of the resulting file name.
	\param e string extension without any "."
	*/
	void setZipExtension(QString e);
	/*! \brief Get the zipped file extension
	\retval QString see setZipExtension(QString e) */
	QString zipExtension();

	QDir cDir;
	QCheckBox* SaveZip;
	QCheckBox* WithFonts;
	QCheckBox* WithProfiles;
	QFrame* Layout;
	QToolButton* HomeB;
	QFrame* LayoutC;
	QComboBox *TxCodeM;
	QLabel *TxCodeT;
private slots:
	//! \brief Go to the document home dir.
	void slotHome();
	//! \brief Switch the filename extensions by compress checkbox state.
	void handleCompress();
protected:
	QHBoxLayout* Layout1;
	QHBoxLayout* Layout1C;
	//! \brief Property with default extension
	QString ext;
	//! \brief Property with default compress extension
	QString extZip;
	//! \brief Option flags given by user in ctore
	int optionFlags;
};

#endif
