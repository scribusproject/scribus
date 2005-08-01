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
Ehm... And now to the "few things". There is some magic (read strange)
stuff since it's using the Vison Defects simulations. It is based on
the matrix operations. I'm computing it this way (plain multiplication)
to prevent new library dependecy - IMHO is no need to have some matrix
library for onlu 3 or 4 transformations of the 3x3 matrix.
I've took the "Digital Video Colourmaps for Checking the Legibility
of Displays of Dichromats" article by Francoise Vienot, Hans Brettel
and John D. Mollon as the base reference. Don't forget that google is
your friend ;)
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
		/*! Enumerate Vision Defects for readable ComboBox access */
		enum defectMode {
			normalVision = 0,
			protanopeVision = 1,
			deuteranopeVision = 2,
			colorBlindnessVision = 3
		};
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
		/*! Converts RGB to the LMS color space
		\param rgb original color
		\param l returning reference to the "L" from LMS
		\param m returning reference to the "M" from LMS
		\param s returning reference to the "S" from LMS
		*/
		void getLMSfromRGB(QColor rgb, double *l, double *m, double *s);
		/*! Converts LMS color to the RGB
		\param l the "L" from LMS
		\param m the "M" from LMS
		\param s the "S" from LMS
		\retval rgb color
		*/
		QColor getRGBfromLMS(double l, double m, double s);
		/*! Compute Protanopy filter.
		It doesn't need L.
		\param m M from LMS
		\param s S from LMS
		\retval L from LMS
		*/
		double getProtanopesLMS(double m, double s);
		/*! Compute Deuteranopy filter.
		It doesn't need M.
		\param l L from LMS
		\param s S from LMS
		\retval M from LMS
		*/
		double getDeuteranopesLMS(double l, double s);


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
