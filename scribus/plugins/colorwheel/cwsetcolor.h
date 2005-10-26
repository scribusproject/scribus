#ifndef CWSETCOLOR_H
#define CWSETCOLOR_H

#include <qvariant.h>
#include <qdialog.h>

class QLabel;
class QSpinBox;
class QPushButton;
class QGroupBox;
class QGridLayout;
class QVBoxLayout;
class QColor;
class QPixmap;


/*! \brief A dialog to set color by exact numeric values.
It provides GUI where the user can set color via its components.
There are 3 ways to do it - CMYK, RGB and HSV models. Every model
has its own small color sample too. Wou can access user chosen
color at newColor public variable.
\author Petr Vanek <petr@yarpen.cz>
*/
class CwSetColor: public QDialog
{
	Q_OBJECT

	public:
		CwSetColor(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);
		~CwSetColor(){};

		/*! \brief This is the color which user created.
		It's available after exec()==accept() */
		QColor newColor;

	protected:
		/*! \brief Color sample, pixmaps. */
		QLabel* rgbSample;
		QLabel* cmykSample;
		QLabel* hsvSample;

		QLabel* hLabel;
		QLabel* sLabel;
		QLabel* vLabel;
		QLabel* rLabel;
		QLabel* gLabel;
		QLabel* bLabel;
		QLabel* cLabel;
		QLabel* mLabel;
		QLabel* yLabel;
		QLabel* kLabel;
		QSpinBox* hSpin;
		QSpinBox* sSpin;
		QSpinBox* vSpin;
		QSpinBox* rSpin;
		QSpinBox* gSpin;
		QSpinBox* bSpin;
		QSpinBox* cSpin;
		QSpinBox* mSpin;
		QSpinBox* ySpin;
		QSpinBox* kSpin;
		QPushButton* hsvButton;
		QPushButton* rgbButton;
		QPushButton* cmykButton;
		QGroupBox* rgbBox;
		QGridLayout* rgbBoxLayout;
		QGroupBox* hsvBox;
		QGridLayout* hsvBoxLayout;
		QGroupBox* cmykBox;
		QGridLayout* cmykBoxLayout;
		QVBoxLayout* mainLayout;
		QGridLayout* CwSetColorLayout;
		QPushButton* cancelButton;

		/*! \brief Draws a pixmap sample.
		\param c a color to fill with. */
		QPixmap sample(QColor c);

	protected slots:
		virtual void languageChange();
		virtual void hsvButton_clicked();
		virtual void rgbButton_clicked();
		virtual void cmykButton_clicked();

		/*! \brief Draw a sample for specified model */
		virtual void sampleRGB();
		virtual void sampleHSV();
		virtual void sampleCMYK();
};

#endif
