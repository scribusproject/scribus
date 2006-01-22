/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PICSEARCH_H
#define PICSEARCH_H

#include <qdialog.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qstringlist.h>

#include "scribusapi.h"

class SCRIBUS_API PicSearch : public QDialog
{ 
    Q_OBJECT

public:
    PicSearch( QWidget* parent, const QString & fileName, const QStringList & alt);
    ~PicSearch() {};

    QLabel* TextLabel1;
    QLabel* Datei;
    QListBox* ListBox1;
    QCheckBox* Preview;
    QPushButton* UseB;
    QPushButton* CancelB;
    QLabel* PixmapLabel1;
    QSize minS;
    QString Bild;

private slots:
	void ShowPrev();
	void selBild(QListBoxItem *c);
	void GenPreview();

signals:
	// Emitted when the pic for a row is replaced. Arg is row index.
	void rowPicChanged(unsigned int);

protected:
    QVBoxLayout* PicSearchLayout;
    QHBoxLayout* Layout2;
    QHBoxLayout* Layout3;
    QVBoxLayout* Layout1;
};

#endif // PICSEARCH_H
