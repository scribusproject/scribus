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

#include <QDialog>
#include <QFileIconProvider>
#include <QLabel>
class QPushButton;
class QComboBox;
class QCheckBox;
class QHBoxLayout;
class QVBoxLayout;
class QFrame;
class ScFileWidget;
class QIcon;

#include "scribusapi.h"

/**
  *@author Franz Schmid
  */
class SCRIBUS_API ImIconProvider : public QFileIconProvider
{
public:
	ImIconProvider();
	~ImIconProvider() {};
	QIcon icon(const QFileInfo &fi) const;
	QStringList fmts;
	QIcon imagepm;
	QIcon docpm;
	QIcon pspm;
	QIcon pdfpm;
	QIcon txtpm;
	QIcon oosxdpm;
	QIcon oosxwpm;
	QIcon vectorpm;
};

class SCRIBUS_API FDialogPreview : public QLabel
{
    Q_OBJECT
public:
	FDialogPreview(QWidget *pa);
	~FDialogPreview() {};
	void updtPix();
	void GenPreview(QString name);
};

typedef enum {
	fdNone = 0,
	fdShowPreview = 1, // display text/image previewer
	fdExistingFiles = 2, // When set, set to the QFileDialog::ExistingFile mode when true, QFileDialog::AnyFile otherwise
	fdCompressFile = 4, // Show and handle the "Compress" behaviour and CheckBox (true)
	fdShowCodecs = 8, // Show and handle text codecs and ComboBox (true)
	fdDirectoriesOnly = 16, // Show only directories
	fdHidePreviewCheckBox = 32 // Hide preview Checkbox
} fdFlags;

/*! \brief A Scribus own file dialog.
Used almost everywhere. You can see extension handling
in e.g. bool SVGExportPlugin::run(QString filename).
*/
class SCRIBUS_API CustomFDialog : public QDialog
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
	CustomFDialog(QWidget *parent, QString wDir, QString caption = "",  QString filter = "", int flags = fdExistingFiles);
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

	QCheckBox* SaveZip;
	QCheckBox* WithFonts;
	QCheckBox* WithProfiles;
	QFrame* Layout;
	QFrame* LayoutC;
	QComboBox *TxCodeM;
	QLabel *TxCodeT;
	ScFileWidget *fileDialog;
	FDialogPreview *pw;
	QCheckBox *showPreview;
	QPushButton* OKButton;
	QPushButton* CancelB;
	void setSelection(QString );
	QString selectedFile();
	void addWidgets(QWidget *widgets);
private slots:
	//! \brief Go to the document home dir.
	void fileClicked(const QString &path);
	void togglePreview();
public slots:
	//! \brief Switch the filename extensions by compress checkbox state.
	void handleCompress();
protected:
	QVBoxLayout *vboxLayout;
	QVBoxLayout *vboxLayout1;
	QHBoxLayout *hboxLayout;
	QHBoxLayout* Layout1;
	QHBoxLayout* Layout1C;
	//! \brief Property with default extension
	QString ext;
	//! \brief Property with default compress extension
	QString extZip;
	//! \brief Option flags given by user in ctore
	int optionFlags;
	bool previewIsShown;
};

#endif
