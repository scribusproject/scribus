/* $Id$ */
#ifndef COLORWHEELWIDGET_H
#define COLORWHEELWIDGET_H

#include <qlabel.h>
#include <qvaluevector.h>
#include <sccolor.h>
#include <scribusstructs.h>

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
		~ColorWheel();

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

		/** Coordinates of the leading point. */
		QPoint actualPoint;
		QPoint oldPoint;
		/** RGB interpretation of the leading point. */
		QRgb actualRgb;

		/** \brief List of the colors created in this widget.
		Colors can be added into Scribus color list later. */
		ColorList colorList;

		/** \brief Draw a color wheel.
		\param QValueVector<QPoint> points to be drawed.
		*/
		void paintWheel(QValueVector<QPoint>);

		/** \brief Returns localized name of the type */
		QString getTypeDescription(MethodType aType);

		/** Draws a sample square filled with specified color. */
		QPixmap sample(QColor);

		/** Finds the color of the point in this widget
		\param QPoint Coordinates of the point.
		\return QRgb value with color of the point. */
		QRgb getPointColor(QPoint);

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

	signals:
		/** \brief Signal raised by mouse click on widget by user.
		\param int Mouse button number. See Qt docs.
		\param QPoint Coordinates of the mouse pointer. */
		void clicked(int, const QPoint &);

	protected:
		bool mousePressed;

		/** \brief An event for mouse actions handling.
		See \see clicked() for more info.
		\param QMouseEvent Mouse properties. */
		void mouseReleaseEvent(QMouseEvent *);

		void mousePressEvent(QMouseEvent *);
		void mouseMoveEvent(QMouseEvent *);
		void paintCenterSample();

		/** \brief Appends a color into \see colorList.
		\param double an angle for transformation counting.
		              E.g. base angle is 30, param is 90 - transformation is 120.
		\param QString name of the color. */
		void sampleByAngle(double, QString);

		/** \brief Counts an angle of the point in color wheel
		\param QPoint coordinates of the point. */
		double pointAngle(QPoint);

		/** \brief Appends the base color into color list. */
		void baseColor();

		/** \brief Creates a Scribus ScColor from rgb value.
		\param QRgb a rgb value of the color. */
		ScColor cmykColor(QRgb rgb);

		/** \brief Quick and dirty border checking.
		It prevents crashes when user moves mouse out of bounds
		\param p current point
		\retval QPoint point surely in widget
		*/
		QPoint checkBounds(QPoint p);
};

#endif
