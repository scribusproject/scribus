#ifndef IMAGEINFODIALOG_H
#define IMAGEINFODIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "scribusapi.h"
#include "scimage.h"

class SCRIBUS_API ImageInfoDialog : public QDialog
{
	Q_OBJECT

public:
	ImageInfoDialog( QWidget* parent, ScImage::ExifValues *info );
	~ImageInfoDialog() {};

protected:
	QVBoxLayout* ImageInfoDialogLayout;
	QGridLayout* layout2;
	QHBoxLayout* layout1;
	QLabel* Text1;
	QLabel* Text2;
	QLabel* Text3;
	QLabel* Comment;
	QLabel* Model;
	QLabel* Manufact;
	QPushButton* okButton;
};

#endif
