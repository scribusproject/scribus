/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ANNOTA_H
#define ANNOTA_H

#include <QDialog>
class QHBoxLayout;
class QGridLayout;
class QVBoxLayout;
class QFrame;
class QLabel;
class QComboBox;
class QStackedWidget;
class QGroupBox;
class QLineEdit;
class QPushButton;
class ScrSpinBox;
class QCheckBox;

#include "scribusapi.h"

class PageItem;
class Navigator;
class ScribusDoc;
class ScribusView;

class SCRIBUS_API Annota : public QDialog
{
	Q_OBJECT

public:
	Annota(QWidget* parent, PageItem *it, ScribusDoc* doc, ScribusView* vie);
	~Annota() {};

public slots:
	void SetCoords(double x, double y);
	void SetPage(double v);
	void SetCross();
	void SetValues();
	void SetTarget(int i);
	void GetFile();

protected:
	PageItem* m_item { nullptr };
	ScribusDoc* m_doc { nullptr };
	ScribusView* m_view { nullptr };

	int m_pageCount;
	int m_width;
	int m_height;
	int m_oriWidth;
	int m_oriHeight;

protected:
	QVBoxLayout* AnnotLayout { nullptr };
	QHBoxLayout* Layout1 { nullptr };
	QGridLayout* GroupBox1Layout { nullptr };
	QGridLayout* GroupBox2Layout { nullptr };
	QHBoxLayout* Layout1_2 { nullptr };
	
	QStackedWidget* Fram { nullptr };
	QLabel* TextLabel1 { nullptr };
	QComboBox* ComboBox1 { nullptr };
	QGroupBox* GroupBox1 { nullptr };
	QLabel* TextLabel3 { nullptr };
	QLabel* TextLabel4 { nullptr };
	QLabel* TextLabel5 { nullptr };
	QLineEdit* Destfile { nullptr };
	QPushButton* ChFile { nullptr };
	QCheckBox* useAbsolute { nullptr };
	Navigator* navigator { nullptr };
	ScrSpinBox* SpinBox1 { nullptr };
	ScrSpinBox* SpinBox2 { nullptr };
	ScrSpinBox* SpinBox3 { nullptr };
//	QFrame* Frame9;
	QPushButton* PushButton1 { nullptr };
	QPushButton* PushButton2 { nullptr };
	QGroupBox* GroupBox2 { nullptr };
	QLabel* TextLabelG1 { nullptr };
	QComboBox* ComboBox2 { nullptr };
	QCheckBox* textIsOpen { nullptr };
};

#endif // ANNOT_H
