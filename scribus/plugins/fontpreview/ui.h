#ifndef FONTPREVIEW_UI_H
#define FONTPREVIEW_UI_H

#include "qdialog.h"
#include "qpixmap.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QListView;
class QListViewItem;
class QListBoxItem;
class QPushButton;
class QLabel;
class QSpinBox;
class QSpacerItem;
class QLineEdit;
class PrefsContext;
class SampleItem;


/*! \brief Main window for "Font Preview" plugin. It's only gui.
Constructs a FontPreview as a child of 'parent', with the
name 'name' and widget flags set to 'f'.
The dialog will by default be modeless, unless you set 'modal' to
true to construct a modal dialog.
*/
class FontPreview : public QDialog
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
	/** gui widgets */
	QLabel* searchLabel;
	QLineEdit* searchEdit;
	QPushButton* searchButton;
	QListView* fontList;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QLabel* fontPreview;
	QLabel* sizeLabel;
	QSpinBox* sizeSpin;
	/** \brief Font iteration */
	QMap<QString,int> reallyUsedFonts;
	/** \brief Pixmaps for each font type */
	QPixmap ttfFont;
	QPixmap otfFont;
	QPixmap psFont;
	QPixmap okIcon;
	/** \brief gui layout */
	QGridLayout* FontPreviewLayout;
	QVBoxLayout* mainLayout;
	QHBoxLayout* searchLayout;
	QHBoxLayout* sizeLayout;
	QSpacerItem* buttonSpacer;
	/** \brief Configuration structure */
	PrefsContext* prefs;
	/*! \brief Column for sorting. */
    uint sortColumn;
	/*! \brief window size */
    uint xsize;
    uint ysize;
	/*! \brief Setup fontList by search string.
	If the search string is empty list all fonts
	\param searchStr text to search */
	void updateFontList(QString searchStr);
	/*! \brief A sample text frame to render the pixmap. */
	SampleItem *sampleItem;

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
	/*! \brief Creates pixmap with font sample */
	virtual void fontList_changed();
};

#endif // FONTPREVIEW_H

