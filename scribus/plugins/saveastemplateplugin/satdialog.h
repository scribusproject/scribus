/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#ifndef SATDIALOG_H
#define SATDIALOG_H

#include "ui_satdialog.h"
#include <prefscontext.h>

typedef std::pair<QString*,QString*> Pair;

class satdialog : public QDialog, public Ui::satdialog
{
	Q_OBJECT

public:
	satdialog(QWidget* parent, QString tmplName = "", int pageW = 0, int pageH = 0);
	~satdialog();

	std::vector<Pair*> cats;
private slots:
	void detailClicked(int);

private:
	PrefsContext* prefs;
	QString author;
	QString email;
	bool isFullDetail;

	void readPrefs();
	void writePrefs();
	void setupCategories();
	void setupPageSize(int w, int h);
};

#endif

