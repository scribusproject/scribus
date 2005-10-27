#ifndef IMAGEINFODIALOG_H
#define IMAGEINFODIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qgroupbox.h>

#include "scribusapi.h"
#include "scimage.h"

class SCRIBUS_API ImageInfoDialog : public QDialog
{
	Q_OBJECT

public:
	ImageInfoDialog( QWidget* parent, ScImage::ImageInfoRecord *info );
	~ImageInfoDialog() {};

protected:
	QVBoxLayout* ImageInfoDialogLayout;
	QHBoxLayout* layout1;
	QGroupBox *GenGroup;
	QGridLayout* layout3;
	QLabel* Text1g;
	QLabel* emProfile;
	QLabel* Text2g;
	QLabel* emProfileN;
	QLabel* Text3g;
	QLabel* emPath;
	QLabel* Text4g;
	QLabel* emLayer;
	QGroupBox *ExGroup;
	QGridLayout* layout2;
	QLabel* Text1;
	QLabel* Text2;
	QLabel* Text3;
	QLabel* Comment;
	QLabel* Model;
	QLabel* Manufact;
	QPushButton* okButton;
};

#endif
