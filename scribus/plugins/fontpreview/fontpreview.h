/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FONTPREVIEW_UI_H
#define FONTPREVIEW_UI_H

#include "ui_fontpreview.h"

class PrefsContext;
class SampleItem;
class ScribusDoc;
class FontListModel;
class QSortFilterProxyModel;


/*! \brief Main window for "Font Preview" plugin. It's only gui.
Constructs a FontPreview as a child of 'parent', with the
name 'name' and widget flags set to 'f'.
The dialog will by default be modeless, unless you set 'modal' to
true to construct a modal dialog.
*/
class FontPreview : public QDialog, Ui::FontPreview
{
	Q_OBJECT

public:
	/*! \brief Create Font Preview window,
	\param fontName Optional name of the font to init/select on start.
	*/
	FontPreview(QString fontName = QString::null, QWidget* parent = 0, ScribusDoc* doc=0);
	~FontPreview();

	/*! \brief Get the current selected font name.
	\retval QString the font name. */
	QString getCurrentFont();

protected:
	/** \brief Pixmaps for each font type */
	QPixmap ttfFont;
	QPixmap otfFont;
	QPixmap psFont;
	QPixmap okIcon;
	//! \brief Default "blue quartz" text
	QString defaultStr;
	/** \brief Configuration structure */
	PrefsContext* prefs;
	/*! \brief window size */
	uint xsize;
	uint ysize;

	//! \brief Sample text frame to render the preview
	SampleItem *sampleItem;

	//! \brief Store fonts in the Qt4 MVC model part.
	FontListModel * fontModel;

	/*! \brief A model for searching in the font set.
	see searchButton_clicked() for more. */
	QSortFilterProxyModel * proxyModel;

	/*! \brief Create a pixmap sample with font preview.
	With GUI oprions. */
 	void paintSample();

	/*! \brief Check if is there any item in list to prevent crashes.
	It allows not to paint the sample if there could be memory leak.
	\retval bool true if you can paint sample safely. */
	bool allowSample();

	//! \brief Paint sample on the dialog opening.
	void showEvent(QShowEvent * event);
	void resizeEvent(QResizeEvent * event);

	ScribusDoc* m_Doc;

protected slots:
	/*! \brief Sets the strings of the subwidgets using the current
	language. */
	virtual void languageChange();

	/** \brief Called after each text change.
	It calls paintSample().
	\param s non-used string*/
	virtual void searchEdit_textChanged(const QString &s);

	//! \brief The same as searchEdit_textChanged().
	virtual void searchButton_clicked();

	//! \brief Slot to handle row changes in the table view.
	void fontList_currentChanged(const QModelIndex &, const QModelIndex &);

	//! \brief Setup the preview phrase instead of "blue quartz..."
	void displayButton_clicked();

	//! \brief Reset the sample phrase to the standard "blue quartz"
	void resetDisplayButton_clicked();

	//! \brief Font size change.
	void sizeSpin_valueChanged( int );

	void cancelButton_clicked();

	//! \brief Show/hide "advanced" columns in the view
	void setExtendedView(bool state);
};

#endif // FONTPREVIEW_H

