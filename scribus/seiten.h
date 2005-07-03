/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SEITENPAL_H
#define SEITENPAL_H

#include <qdialog.h>
#include <qtable.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qdragobject.h>
#include <qevent.h>
#include <qsplitter.h>
#include "scribusview.h"

class SeDrag : public QStoredDrag
{
public:
    SeDrag( QString s, QWidget * parent = 0, const char * name = 0 );
    ~SeDrag() {};

    static bool canDecode( QDragMoveEvent* e );
    static bool decode( QDropEvent* e, QString& s );
};

class SeItem : public QTableItem
{
public:
	SeItem(QTable* parent, QString text, QPixmap Pix, bool ss);
    ~SeItem() {};
	virtual void paint(QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected);
	bool Side;
};

class SeList : public QListBox
{
    Q_OBJECT

public:
    SeList(QWidget* parent);
    ~SeList() {};
    QPoint Mpos;
	QListBoxItem *CurItem;
    bool Mpressed;
	bool Thumb;

private slots:
	void ToggleTh();

signals:
	void ThumbChanged();

protected:
	void mouseReleaseEvent(QMouseEvent *m);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
};

class SeView : public QTable
{
    Q_OBJECT

public:
    SeView(QWidget* parent);
    ~SeView() {};
	void ClearPix();
	int GetPage(int r, int c);
    QPoint Mpos;
    bool Mpressed;
	bool Doppel;
	bool Links;
	bool Namen;
	int MaxC;
	QPixmap pix;

public slots:
	void ToggleNam();

signals:
	void UseTemp(QString, int);
	void NewPage(int, QString);
	void MovePage(int, int);
	void Click(int, int, int);

protected:
	virtual void contentsDropEvent(QDropEvent * e);
	virtual void contentsDragEnterEvent(QDragEnterEvent *e);
	virtual void contentsDragLeaveEvent(QDragLeaveEvent *e);
	virtual void contentsDragMoveEvent(QDragMoveEvent *e);
	virtual void contentsMouseReleaseEvent(QMouseEvent *m);
	virtual void contentsMousePressEvent(QMouseEvent* e);
	virtual void contentsMouseMoveEvent(QMouseEvent* e);
};

class TrashBin : public QLabel
{
    Q_OBJECT

public:
	TrashBin( QWidget * parent );
	~TrashBin() {};
	QPixmap Normal;
	QPixmap Offen;
	void dragEnterEvent( QDragEnterEvent *e );
	void dragLeaveEvent( QDragLeaveEvent * );
	void dropEvent( QDropEvent * e );

signals:
	void DelPage(int);
	void DelMaster(QString);
};

class SeitenPal : public QDialog
{ 
    Q_OBJECT

public:
    SeitenPal(QWidget* parent);
    ~SeitenPal() {};
	void keyPressEvent(QKeyEvent *k);
    void closeEvent(QCloseEvent *ce);

    QSplitter* Splitter1;
    TrashBin* Trash;
	QLabel* TextLabel1;
	QLabel* TextLabel2;
    SeList* TemplList;
    SeView* PageView;
	QCheckBox* DS;
	QCheckBox* LP;
	ScribusView *Vie;
	QPixmap pix;

public slots:
	void SetView(ScribusView *view);
	void DelMPage(QString tmp);
	void MPage(int r, int c);
	void GotoPage(int r, int c, int b);
	void DisablePal();
	void EnablePal();
	void HandleDS();
	void HandleLP();
	void RebuildTemp();
	void RebuildPage();
	void Rebuild();
	void selTemplate();
	QPixmap CreateIcon(int nr, QPixmap ret);

signals:
	void ToggleAllPalettes();
	void EditTemp(QString);
	void GotoSeite(int);
	void Schliessen();

protected:
    QVBoxLayout* SeitenPalLayout;
    QHBoxLayout* Layout1;
    QVBoxLayout* Layout2;
    QVBoxLayout* Layout3;
    QVBoxLayout* Layout4;

protected slots:
	virtual void reject();
};

#endif // SEITENPAL_H
