/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef CWDIALOG_H
#define CWDIALOG_H

#include "ui_cwdialog.h"
#include "colorwheelwidget.h"
#include "scribus.h"

class QSpacerItem;
class QComboBox;
class QPushButton;
class QSpinBox;
class QSlider;
class ColorListBox;
class ScribusDoc;
class ScColor;


/** \brief GUI dialog for Color Wheel Plugin.
Quite everything in this class is self explanatory (except few things ;)).
\author Petr Vanek <petr@scribus.info>
\date April 2005
 */
class CWDialog : public QDialog, Ui::CWDialog
{
	Q_OBJECT
	public:
		CWDialog( QWidget* parent = 0, ScribusDoc* doc = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
		~CWDialog();
	private:
		/** \brief Configuration structure */
		PrefsContext* prefs;
		//! \brief a parent doc reference
		ScribusDoc* m_Doc;

		/** \brief Draws a strange colorful things as preview of the color schema.
		User can see what will see a person with selected kind of color blindness.
		*/
		void setPreview();

		/*! \brief Main color manipulator for preview.
		It calls transformations for the other vision defects filters.
		\param c input color. QColor from sample list.
		\retval QColor It returns c for chosen defect filter.
		*/
		QColor computeDefect(QColor c);

		/** \brief It fills colors into list view.
		It takes colors from ColorWheel widget. */
		void fillColorList();

		/*! \brief Set the spins with its color component value.
		It fills recomputed components regarding chosen color model
		and the changed channel. E.g. if user change R in RGB palette
		all CMYK channels are recomputed calling setupCMYKComponent(). */
		void setupColorComponents();

		/*! \brief A GUI setter for all components when is one of CMYK changed.
		*/
		void setupRGBComponent(ScColor col);

		/*! \brief A GUI setter for all components when is one of RGB changed.
		*/
		void setupCMYKComponent(ScColor col);

		/*! \brief A GUI setter for all components when is one of HSV changed.
		It uses a dummy QColor->ScColor conversion
		*/
		void setupHSVComponent(ScColor col);

		//! \brief Set named colors for labels
		void updateNamedLabels();

		/*! \brief Connect or disconnect rgbcmyk spinboxes signals.
		\param conn if true perform connect. Disconnect in the case of false
		*/
		void connectSlots(bool conn=true);

		/*! \brief Call main color calculation.
		It calls a ColorWheel methods to get requested harmonious colors depending
		on the dialog's settings.
		\param index an index of the typeCombo (Color Scheme Method)
		\param updateSpins if true call setupCMYKComponent() and setupRGBComponent() methods.
		if false don't reset any of these component spins - it's used for colorWheel
		mouse clicked/released events.
		*/
		void processColors(int index, bool updateSpins=true);

		//! \brief Get a #hhssvv string for given ScColor
		QString getHexHsv(ScColor c);

	private slots:
		void colorspaceTab_currentChanged(int);
		void angleSpin_valueChanged(int);
		void colorWheel_clicked(int, const QPoint &);
		void typeCombo_activated(int);
		void documentColorList_currentChanged(QListWidgetItem *);
		void defectCombo_activated(int);
		void addButton_clicked();
		void replaceButton_clicked();
		void cancelButton_clicked();
		void colorList_currentChanged(QListWidgetItem *);

		void cSpin_valueChanged( int );
		void mSpin_valueChanged( int );
		void ySpin_valueChanged( int );
		void kSpin_valueChanged( int );
		void rSpin_valueChanged( int );
		void gSpin_valueChanged( int );
		void bSpin_valueChanged( int );
		void hSpin_valueChanged( int );
		void sSpin_valueChanged( int );
		void vSpin_valueChanged( int );
};

#endif // CWDIALOG_H
