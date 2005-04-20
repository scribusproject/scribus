#ifndef FONTPREVIEW_H
#define FONTPREVIEW_H

#include <qvariant.h>
#include <qdialog.h>
#include "scribus.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QListBox;
class QListBoxItem;
class QPushButton;
class QLabel;

class FontPreview : public QDialog
{
	Q_OBJECT

public:
	FontPreview( ScribusApp *carrier, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	~FontPreview();

	/** Reference on the parent application object */
	ScribusApp *carrier;
	/** gui widgets */
	QListBox* fontList;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QLabel* fontPreview;
	QSpinBox* fontSizeSpinner;

public slots:
	virtual void fontList_changed();

protected:
	/** gui layout */
	QGridLayout* FontPreviewLayout;
	QVBoxLayout* layout6;
	QHBoxLayout* layout5;
	QVBoxLayout* layout2;
	QVBoxLayout* layout1;

protected slots:
	/** tr() */
	virtual void languageChange();

};

#endif // FONTPREVIEW_H
