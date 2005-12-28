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
public:
	FDialogPreview(QWidget *pa);
	~FDialogPreview() {};
	void updtPix();
	void GenPreview(QString name);
	virtual void previewUrl(const QUrl &url);
};

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
	\param preview When true - display text/image previewer. When false do not display.
	\param existing Set to the QFileDialog::ExistingFile mode when true. When false QFileDialog::AnyFile
	\param compress Show and handle the "Compress" behaviour and CheckBox (true)
	\param codec Show and handle text codecs and ComboBox (true)
	\param dirOnly Show only directories when true.
	*/
	CustomFDialog(QWidget *parent, QString wDir, QString caption = "",
				  QString filter = "", bool preview = false,
				  bool existing = true, bool compress = false,
				  bool codec = false, bool dirOnly = false);
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
	QCheckBox* WFonts;
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
};

#endif
