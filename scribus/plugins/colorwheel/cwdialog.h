/* $Id$ */
#ifndef CWDIALOG_H
#define CWDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include "colorwheelwidget.h"
#include <scribus.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QComboBox;
class QListView;
class QListViewItem;
class QPushButton;
class QSpinBox;

/** \brief GUI dialog for Color Wheel Plugin.
Quite everything in this class is self explanatory (except few things ;)).
\author Petr Vanek; petr@yarpen.cz
\date April 2005
*/
class ColorWheelDialog : public QDialog
{
	Q_OBJECT

	public:
		ColorWheelDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
		~ColorWheelDialog();

		QLabel* typeLabel;
		QComboBox* typeCombo;
		QListView* colorList;
		ColorWheel* colorWheel;
		QLabel* previewLabel;
		QLabel* angleLabel;
		QSpinBox* angleSpin;
		QPushButton* addButton;
		QPushButton* replaceButton;
		QPushButton* cancelButton;
		QLabel* defectLabel;
		QComboBox* defectCombo;

	protected:
		/** It fills a colors into list view. It takes colors
		    from ColorWheel widget. */
		void fillColorList();
		QGridLayout* formLayout;
		QHBoxLayout* mainLayout;
		QVBoxLayout* wheelLayout;
		QSpacerItem* spacer1;
		QHBoxLayout* angleLayout;
		QVBoxLayout* listLayout;
		QHBoxLayout* buttonLayout;
		QHBoxLayout* defectLayout;
		/** Configuration structure */
		PrefsContext* prefs;
		/** Draws a strange colorful things as preview of the color schema. */
		void setPreview();
		/*! Main color manipulator for preview.
		\param c input color. QColor from sample list.
		\retval QColor It returns c for normalVision. It calls transformations for other filters*/
		QColor computeDefect(QColor c);

	public slots:
		virtual void typeCombo_activated(int);
		virtual void defectCombo_activated(int);
		virtual void colorWheel_clicked(int, const QPoint &);
		virtual void addButton_clicked();
		virtual void replaceButton_clicked();
		virtual void cancelButton_clicked();
		virtual void angleSpin_valueChanged(int);

	protected slots:
		virtual void languageChange();

};

#endif // CWDIALOG_H
