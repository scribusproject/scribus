#ifndef FONTPREVIEW_H
#define FONTPREVIEW_H

#include <qvariant.h>
#include <qdialog.h>

#include "scribus.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QListView;
class QListViewItem;
class QListBoxItem;
class QPushButton;
class QLabel;

class FontPreview : public QDialog
{
	Q_OBJECT

public:
	FontPreview(ScribusApp *carrier, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);
	~FontPreview();

	/** Reference on the parent application object */
	ScribusApp *carrier;
	/** gui widgets */
	QListView* fontList;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QLabel* fontPreview;

public slots:
	virtual void fontList_changed(QListViewItem *);

protected:
	/** gui layout */
	QGridLayout* FontPreviewLayout;
	QVBoxLayout* layout6;
	QHBoxLayout* layout5;
	QVBoxLayout* layout2;
	QVBoxLayout* layout1;
	/** Configuration structure */
	PrefsContext* prefs; 
    /*! Column for sorting. */
    uint sortColumn;
    /*! window size */
    uint xsize;
    uint ysize;

protected slots:
	/** tr() */
	virtual void languageChange();

};

#endif // FONTPREVIEW_H

