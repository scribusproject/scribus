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

#include <q3filedialog.h>
#include <q3url.h>
#include <qlabel.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <q3frame.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qstring.h>
//Added by qt3to4:
#include <QHBoxLayout>

#include "scribusapi.h"

/**
  *@author Franz Schmid
  */
class SCRIBUS_API ImIconProvider : public Q3FileIconProvider
{
    Q_OBJECT
    QStringList fmts;
    QPixmap imagepm;
    QPixmap docpm;
    QPixmap pspm;
    QPixmap pdfpm;
    QPixmap txtpm;
    QPixmap oosxdpm;
    QPixmap oosxwpm;
    QPixmap vectorpm;
public:
    ImIconProvider(QWidget *pa);
    ~ImIconProvider() {};

    const QPixmap * pixmap(const QFileInfo &fi);
};

class SCRIBUS_API FDialogPreview : public QLabel, public Q3FilePreview
{
    Q_OBJECT
public:
	FDialogPreview(QWidget *pa);
	~FDialogPreview() {};
	void updtPix();
	void GenPreview(QString name);
	virtual void previewUrl(const Q3Url &url);
};

class SCRIBUS_API CustomFDialog : public Q3FileDialog
{
    Q_OBJECT
public: 
	CustomFDialog(QWidget *pa, QString wDir, QString cap = "", QString filter = "", 
	              bool Pre = false, bool mod = true, bool comp = false, bool cod = false,
	              bool dirOnly = false);
	~CustomFDialog();
	QDir cDir;
	QCheckBox* SaveZip;
	QCheckBox* WFonts;
	Q3Frame* Layout;
	QToolButton* HomeB;
	Q3Frame* LayoutC;
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
