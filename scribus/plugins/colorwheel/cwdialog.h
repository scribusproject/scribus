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


/*! \brief GUI for colors from preferences/document.
List all available colors in one dialog with samples.
\author Petr Vanek; petr@yarpen.cz
*/
class ScribusColorList : public QDialog
{
	Q_OBJECT

	public:
		ScribusColorList( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
		~ScribusColorList(){};

		QListView* listView;
		QPushButton* okButton;
		QPushButton* cancelButton;

		/*! Selected color when user press "OK" */
		QColor selectedColor;

	protected:
		QGridLayout* ScribusColorListLayout;
		QVBoxLayout* listLayout;
		QHBoxLayout* btnLayout;
		QSpacerItem* btnSpacer;

	protected slots:
		virtual void languageChange();
		virtual void okButton_clicked();
};

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
		QLabel* sLabel;
		QLabel* vLabel;
		QSpinBox* angleSpin;
		QSpinBox* sSpin;
		QSpinBox* vSpin;
		QPushButton* addButton;
		QPushButton* replaceButton;
		QPushButton* cancelButton;
		QLabel* defectLabel;
		QComboBox* defectCombo;

	protected:
		/** \brief It fills a colors into list view.
		It takes colors from ColorWheel widget. */
		void fillColorList();

		QGridLayout* formLayout;
		QHBoxLayout* mainLayout;
		QVBoxLayout* wheelLayout;
		QSpacerItem* spacer1;
		QHBoxLayout* angleLayout;
		QHBoxLayout* sLayout;
		QHBoxLayout* vLayout;
		QVBoxLayout* listLayout;
		QHBoxLayout* buttonLayout;
		QHBoxLayout* defectLayout;

		/** \brief Configuration structure */
		PrefsContext* prefs;

		/** \brief Draws a strange colorful things as preview of the color schema. */
		void setPreview();

		/*! \brief Main color manipulator for preview.
		\param c input color. QColor from sample list.
		\retval QColor It returns c for normalVision. It calls transformations for other filters*/
		QColor computeDefect(QColor c);

		/*! \brief Handling of the user requested colors via menus.
		\param c a color given by create/importColor */
		void userColorInput(QColor c);

	public slots:
		virtual void typeCombo_activated(int);
		virtual void defectCombo_activated(int);
		virtual void colorWheel_clicked(int, const QPoint &);
		virtual void addButton_clicked();
		virtual void replaceButton_clicked();
		virtual void cancelButton_clicked();
		virtual void angleSpin_valueChanged(int);
		virtual void sSpin_valueChanged(int);
		virtual void vSpin_valueChanged(int);

	protected slots:
		virtual void languageChange();

		/*! \brief Color from "new color" dialog. Via menu. */
		virtual void createColor();
		/*! \brief Color from "show existing colors" dialog. Via menu. */
		virtual void importColor();

};

#endif // CWDIALOG_H
