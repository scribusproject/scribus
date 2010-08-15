/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/

#ifndef NFTDIALOG_H
#define NFTDIALOG_H

#include <utility>
#include <vector>

#include "ui_nftdialog.h"
#include "nftsettings.h"

class nftdialog: public QDialog, public Ui::nftdialog
{
	Q_OBJECT
public:
	nftdialog(QWidget* parent, QString lang);
	~nftdialog() {};
public slots:
	void enableButtonBox(bool);
};

#endif
