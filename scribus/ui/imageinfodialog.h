/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMAGEINFODIALOG_H
#define IMAGEINFODIALOG_H

#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QGroupBox;
class QLabel;
class QPushButton;

#include "scribusapi.h"
#include "scimage.h"

class SCRIBUS_API ImageInfoDialog : public QDialog
{
	Q_OBJECT

public:
	ImageInfoDialog( QWidget* parent, ImageInfoRecord *info );
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
