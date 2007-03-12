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
#include <q3whatsthis.h>
#include <q3groupbox.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>

#include "scribusapi.h"
#include "scimage.h"

class SCRIBUS_API ImageInfoDialog : public QDialog
{
	Q_OBJECT

public:
	ImageInfoDialog( QWidget* parent, ImageInfoRecord *info );
	~ImageInfoDialog() {};

protected:
	Q3VBoxLayout* ImageInfoDialogLayout;
	Q3HBoxLayout* layout1;
	Q3GroupBox *GenGroup;
	Q3GridLayout* layout3;
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
	Q3GroupBox *ExGroup;
	Q3GridLayout* layout2;
	QLabel* Text1;
	QLabel* Text2;
	QLabel* Text3;
	QLabel* Text4;
	QLabel* Text5;
	QLabel* Text6;
	QLabel* Text7;
	QLabel* Text8;
	QLabel* Comment;
	QLabel* UserComment;
	QLabel* Artist;
	QLabel* Model;
	QLabel* Manufact;
	QLabel* ExposureTime;
	QLabel* ApertureFNumber;
	QLabel* ISOequivalent;
	QPushButton* okButton;
};

#endif
