/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FONTPREVIEW_UI_H
#define FONTPREVIEW_UI_H

#include <qdialog.h>
#include <qpixmap.h>
#include "fontpreviewbase.h"

class PrefsContext;
class SampleItem;
class QTimer;


/*! \brief Main window for "Font Preview" plugin. It's only gui.
Constructs a FontPreview as a child of 'parent', with the
name 'name' and widget flags set to 'f'.
The dialog will by default be modeless, unless you set 'modal' to
true to construct a modal dialog.
*/
class FontPreview : public FontPreviewBase
{
	Q_OBJECT

public:
	/*! \brief Create Font Preview window,
	\param fontName Optional name of the font to init/select on start.
	*/
	FontPreview(QString fontName = QString::null);
	~FontPreview();

	/*! \brief Get the current selected font name.
	\retval QString the font name. */
	QString getCurrentFont();

protected:
	/** \brief Font iteration */
	QMap<QString,int> reallyUsedFonts;
	/** \brief Pixmaps for each font type */
	QPixmap ttfFont;
	QPixmap otfFont;
	QPixmap psFont;
	QPixmap okIcon;
	//! \brief Default "blue quartz" text
	QString defaultStr;
	/** \brief Configuration structure */
	PrefsContext* prefs;
	/*! \brief Column for sorting. */
	uint sortColumn;
	/*! \brief window size */
	uint xsize;
	uint ysize;
	//! \brief Sample text frame to render the preview
	SampleItem *sampleItem;

	/*! \brief Setup fontList by search string.
	If the search string is empty list all fonts
	\param searchStr text to search */
	void updateFontList(QString searchStr);
	void keyReleaseEvent(QKeyEvent *k);
	void paintSample(QListViewItem *item);

protected slots:
	/** \brief Translations. */
	virtual void languageChange();
	/** \brief Called after each text change.
	It checks the string length for 0 length- calling font list re-filling with all
	fonts = without search filter. Of course that search filling can be used afterall
	all changes but it will slow down the dialog.
	\param s string*/
	virtual void searchEdit_textChanged(const QString &s);
	/** \brief Fill the font list by search mask. */
	virtual void searchButton_clicked();
	/*! \brief Creates pixmap with font sample
	\param item current list item */
	//virtual void fontList_currentChanged( QListViewItem * item);
	virtual void fontList_mouseButtonClicked(int button, QListViewItem *item, const QPoint &point, int column);
	//! \brief Setup the preview phrase instead of "blue quartz..."
	void displayButton_clicked();
	//! \brief Reset the sample phrase to the standard "blue quartz"
	void resetDisplayButton_clicked();
	void cancelButton_clicked();
	void okButton_clicked();
	void timeout();
};

#endif // FONTPREVIEW_H

