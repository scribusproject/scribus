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
class QIcon;
class QHBoxLayout;
class QVBoxLayout;
class QFrame;

class PrefsContext;
class ScFileWidget;

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

	void updatePix();
	void genPreview(const QString& name);
};

enum fdFlags
{
	fdNone = 0,
	fdShowPreview = 1, // display text/image previewer
	fdExistingFiles = 2, // When set, set to the QFileDialog::ExistingFile mode when true, QFileDialog::AnyFile otherwise
	fdCompressFile = 4, // Show and handle the "Compress" behaviour and CheckBox (true)
	fdShowCodecs = 8, // Show and handle text codecs and ComboBox (true)
	fdDirectoriesOnly = 16, // Show only directories
	fdHidePreviewCheckBox = 32, // Hide preview Checkbox
	fdExistingFilesI = 64, // For multiple files
	fdShowImportOptions = 128, // Show Vector Images Import Options
	fdDisableOk = 256 // Disable Ok button, used for file open, import etc
};

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
	CustomFDialog(QWidget *parent, const QString& wDir, const QString& caption = "",  const QString& filter = "", int flags = fdExistingFiles);
	~CustomFDialog() = default;

	void setSelection(const QString& fileName);
	QString selectedFile() const;
	QStringList selectedFiles() const;

	/*! \brief Set the default extension of the resulting file name.
	\param e string extension without any "."
	*/
	void setExtension(const QString& e);
	/*! \brief Get the file extension
	\retval QString see setExtension(QString e) */
	const QString& extension() const;

	/*! \brief Set the "compress" extension of the resulting file name.
	\param e string extension without any "."
	*/
	void setZipExtension(const QString& e);
	/*! \brief Get the zipped file extension
	\retval QString see setZipExtension(QString e) */
	const QString& zipExtension() const;

	bool isTextCodecShown() const;
	void setTextCodec(const QString& textCodec);
	QString textCodec() const;

	bool isSaveZipFileShown() const;
	void setSaveZipFile(bool value);
	bool saveZipFile() const;

	bool isIncludeFontsShown() const;
	void setIncludeFonts(bool value);
	bool includeFonts() const;

	bool isIncludeProfilesShown() const;
	void setIncludeProfiles(bool value);
	bool includeProfiles() const;

	int currentOptionIndex() const;

	void addWidgets(QWidget *widgets);

private slots:
	//! \brief Go to the document home dir.
	void fileClicked(const QString &path);
	void okClicked();
	void togglePreview();

public slots:
	//! \brief Switch the filename extensions by compress checkbox state.
	void handleCompress();

protected:
	QVBoxLayout *vboxLayout { nullptr };
	QVBoxLayout *vboxLayout1 { nullptr };
	QHBoxLayout *hboxLayout { nullptr };
	QFrame*      Layout { nullptr };
	QFrame*      LayoutC { nullptr };
	QHBoxLayout* Layout1 { nullptr };
	QHBoxLayout* Layout1C { nullptr };

	QCheckBox* saveZip { nullptr };
	QCheckBox* withFonts { nullptr };
	QCheckBox* withProfiles { nullptr };
	QComboBox *optionCombo { nullptr };
	QLabel *optionLabel { nullptr };
	ScFileWidget *fileDialog { nullptr };
	FDialogPreview *filePreview { nullptr };
	QCheckBox *showPreview { nullptr };
	QPushButton* okButton { nullptr };
	QPushButton* cancelButton { nullptr };

	PrefsContext* m_fileDialogPrefs { nullptr };

	//! \brief Property with default extension
	QString m_ext;
	//! \brief Property with default compress extension
	QString m_extZip;
	//! \brief Option flags given by user in ctore
	int  m_optionFlags { 0 };
	bool m_previewIsShown { false };

	/** @brief Restore the geometry of the window when showing it. */
	void showEvent(QShowEvent *showEvent) override;
	/** @brief Captures the close event and changes it to hide */
	void closeEvent(QCloseEvent *closeEvent) override;
	/** @brief Stores the geometry of the window when hiding. */
	void hideEvent(QHideEvent* hideEvent) override;

	void storeSize();
};

#endif
