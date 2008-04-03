/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SXWDIA_H
#define SXWDIA_H

#include "scconfig.h"

#include <QDialog>
class QCheckBox;
class QPushButton;

class SxwDialog : public QDialog
{
	Q_OBJECT

public:
	SxwDialog(bool update, bool prefix, bool pack);
	~SxwDialog();
	bool shouldUpdate();
	bool usePrefix();
	bool askAgain();
	bool packStyles();
private:
	QCheckBox* updateCheck;
	QCheckBox* prefixCheck;
	QCheckBox* doNotAskCheck;
	QCheckBox* packCheck;
	QPushButton* okButton;
	QPushButton* cancelButton;
};

#endif // CSVDIA_H
