/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
	QLabel* Text0g;
	QLabel* timeInfo;
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
	QLabel* Text4;
	QLabel* Text5;
	QLabel* Comment;
	QLabel* UserComment;
	QLabel* Artist;
	QLabel* Model;
	QLabel* Manufact;
	QPushButton* okButton;
};

#endif
