/***************************************************************************
                          customfdialog.h  -  description
                             -------------------
    begin                : Fri Nov 30 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CUSTOMFDIALOG_H
#define CUSTOMFDIALOG_H

#include <qfiledialog.h>
#include <qurl.h>
#include <qlabel.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qframe.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
/**
  *@author Franz Schmid
  */
class ImIconProvider : public QFileIconProvider
{
    Q_OBJECT
    QStringList fmts;
    QPixmap imagepm;
    QPixmap docpm;
    QPixmap pspm;
    QPixmap pdfpm;
    QPixmap txtpm;

public:
    ImIconProvider(QWidget *pa);
    ~ImIconProvider() {};

    const QPixmap * pixmap(const QFileInfo &fi);
};

class FDialogPreview : public QLabel, public QFilePreview
{
    Q_OBJECT
public:
	FDialogPreview(QWidget *pa);
	~FDialogPreview() {};
	void GenPreview(QString name);
	virtual void previewUrl(const QUrl &url);
};

class CustomFDialog : public QFileDialog
{
    Q_OBJECT
public: 
	CustomFDialog(QWidget *pa, QString cap = "", QString filter = "", bool Pre = false, bool mod = true, 
					bool comp = false, bool cod = false);
	~CustomFDialog();
	QDir cDir;
	QCheckBox* SaveZip;
	QFrame* Layout;
	QToolButton* HomeB;
	QFrame* LayoutC;
	QComboBox *TxCodeM;
	QLabel *TxCodeT;
private slots:
	void slotHome();
	void HandleComp();
protected:
	QHBoxLayout* Layout1;
	QHBoxLayout* Layout1C;
};

#endif
