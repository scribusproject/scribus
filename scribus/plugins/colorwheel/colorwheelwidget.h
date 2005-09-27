/* $Id$ */
#ifndef COLORWHEELWIDGET_H
#define COLORWHEELWIDGET_H

#include <qlabel.h>
#include <qvaluevector.h>
#include <scribusstructs.h>


typedef QMap<int,QColor> ColorMap;

/**
\brief Widget ColorWheel graphicaly shows a color wheel for color theory.
Class ColorWheel is new widget inherited from the QLabel.
See e.g. http://en.wikipedia.org/wiki/Color_wheel for more info.
\author Petr Vanek; petr@yarpen.cz
\date April 2005
*/
class ColorWheel : public QLabel
{
	Q_OBJECT

	public:
		ColorWheel(QWidget * parent, const char * name = 0);
		~ColorWheel(){};

		/** It can handle these color theory methods */
		enum MethodType {
			Monochromatic,
			Analogous,
			Complementary,
			Split,
			Triadic,
			Tetradic
		};

		/** \brief Difference between selected value and counted ones.
		Let's set angle = 15 and base point e.g. 60 (everything in grades).
		Now you will have Analogous angles 60 (base) and 75 (+15) and 45 (-15).
		Exact interpretation of the angle value depends on the MethodType value.
		*/
		int angle;

		/*! \brief Angle of the base color */
		int baseAngle;

		/** \brief RGB interpretation of the leading point. */
		QColor actualColor;

		/** \brief List of the colors created in this widget.
		Colors can be added into Scribus color list later. */
		ColorList colorList;

		void setS(int value);
		void setV(int value);
		int s();
		int v();

		/** \brief Draw a color wheel. */
		void paintWheel();

		/** \brief Returns localized name of the type.
		\param aType Type of the color algorithm. See MethodType.
		\retval QString Translated method name. */
		QString getTypeDescription(MethodType aType);

		/** Draws a sample square filled with specified color.
		\param c A QColor to fill sample pixmap.
		\retval QPixmap An image with sample. */
		QPixmap sample(QColor c);

		/** \brief Counts the monochromatic colors.
		The monochromatic color scheme uses variations in lightness
		and saturation of a single color. It's clean and elegant color
		schema. I like it's minimalism ;) */
		void makeMonochromatic();

		/** \brief Counts the analogous colors.
		The analogous color scheme uses colors that are changed by an angle
		in the color wheel. It looks richer than mono scheme. */
		void makeAnalogous();

		/** \brief Counts the complementary colors.
		It takes two opposite colors (180 deg). It looks great when you choose a warm
		color against a cool one. E.g. dark green vs. wine red. Hmmm wine... */
		void makeComplementary();

		/** \brief Counts the split-complementary colors.
		It's enthanced complementary method. It takes base and opposite colors
		with applied angle differences. */
		void makeSplit();

		/** \brief Counts the triadic colors.
		It takes three colors balanced on the wheel (by 120 grades). */
		void makeTriadic();

		/** \brief Counts the tetradic colors.
		It's two times complementary. */
		void makeTetradic();

		/*! \brief Setup the values by given QColor.
		It sets all options by given color (from input color dialogs).
		\param col examined color
		\retval true on color found, false when color not found - black or white etc.*/
		bool recomputeColor(QColor col);

	signals:
		/** \brief Signal raised by mouse click on widget by user.
		\param button Mouse button number. See Qt docs.
		\param point Coordinates of the mouse pointer. */
		void clicked(int button, const QPoint & point);

	protected:
		/*! \brief Internal color mapping.
		It provides angle-color dictionary.
		*/
		ColorMap colorMap;

		/** \brief Angle diff between colorMap and painted wheel itself.
		QWMatrix wheel and colorMap have different start points.
		It's taken from Qt. */
		int angleShift;

		/*! \brief "V" component in HSV model. */
		int vcomp;
		/*! \brief "S" component in HSV model. */
		int scomp;

		/*! \brief Half of the widget sizes.
		To prevent all width()/2 divisions. */
		int widthH;
		int heightH;

		/** \brief An event for mouse actions handling.
		See \see clicked() for more info.
		\param e Mouse properties. */
		void mouseReleaseEvent(QMouseEvent *e);
		/** \brief Mouse handling.
		It calls mouseReleaseEvent
		\param e Mouse properties.*/
		void mousePressEvent(QMouseEvent *e);
		/** \brief Mouse handling.
		It calls mouseReleaseEvent
		\param e Mouse properties.*/
		void mouseMoveEvent(QMouseEvent *e);

		/** \brief Draw center circle filled with base color */
		void paintCenterSample();

		/** \brief Appends a color into \see colorList.
		\param angle Angle of the color in the wheel. An angle for transformation counting.
		E.g. base angle is 30, param is 90 - transformation is 120.
		There is easy "convert 665485 into 0-359 interval" algorithm too.
		\param name Human readable name of the color. */
		void sampleByAngle(int angle, QString name);

		/** \brief Appends the base color into color list. */
		void baseColor();

		/** \brief Creates a Scribus ScColor from rgb value.
		\param col a QColor to convert.
		\retval ScColor Scribus color structure */
		ScColor cmykColor(QColor col);

		/** \brief Display user selection - selected colors.
		Chosen colors are marked via bullets on the border of
		the wheel.
		\param angle angle of the drawed mark
		\param base Draw it highlighted if it is base color
		*/
		void drawBorderPoint(int angle, bool base=false);

		/*! \brief Clear border marks before redrawing.
		It redraws only small piece of the bitmap */
		void clearBorder();

		/** \brief Counts an angle of the point in color wheel.
		Modified method from Qt QDial widget.
		\param p coordinates of the point.
		\retval int index in the colorMap */
		int valueFromPoint(const QPoint & p) const;
};

#endif

