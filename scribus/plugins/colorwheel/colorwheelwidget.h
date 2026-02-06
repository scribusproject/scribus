/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef COLORWHEELWIDGET_H
#define COLORWHEELWIDGET_H

#include <QLabel>

#include "scribusstructs.h"
#include "sccolor.h"

class ScribusDoc;

/*! \brief Mapping angle - color in the color wheel */
using ColorMap = QMap<int, ScColor>;

/**
\brief Widget ColorWheel graphically shows a color wheel for color theory.
Class ColorWheel is new widget inherited from the QLabel.
See e.g. http://en.wikipedia.org/wiki/Color_wheel for more info.
\author Petr Vanek <petr@scribus.info>
\date April 2005
*/
class ColorWheel : public QLabel
{
	Q_OBJECT

	public:
		ColorWheel(QWidget * parent, const char * name = nullptr);

		//! \brief It can handle these color theory methods
		enum MethodType
		{
			Monochromatic,
			Analogous,
			Complementary,
			Split,
			Triadic,
			Tetradic
		};

		/** \brief RGB interpretation of the leading point in the wheel. */
		const ScColor& actualColor() const { return m_actualColor; }
		void setActualColor(const ScColor& color) { m_actualColor = color; }

		//! \brief name of the "base color" to handle in extern color lists
		const QString& trBaseColor() const { return m_trBaseColor; }

		/**
		 * \brief Set difference between selected value and counted ones.
		 */
		int  angle() const { return m_angle; }
		void setAngle(int angle) { m_angle = angle; }

		/**
		 * \brief Angle diff between colorMap and painted wheel itself.
		 */
		int  baseAngle() const { return m_baseAngle; }
		void setBaseAngle(int angle) { m_baseAngle = angle; }

		/** \brief List of the colors created in this widget.
		Colors can be added into Scribus color list later. */
		ColorList& colorList() { return m_colorList; }
		const ColorList& colorList() const { return m_colorList; }

		//! \brief Which color model is in use.
		colorModel colorspace() const { return m_colorspace; }
		void setColorspace(colorModel model) { m_colorspace = model; }

		//! \brief Set current document
		void setDoc(ScribusDoc* doc) { m_doc = doc; }

		//! \brief Actual type of color computing. See MethodType.
		MethodType methodType() const { return m_methodType; }
		void setMethodType(MethodType methodType) { m_methodType = methodType; }

		/** \brief Returns localized name of the type.
		\param aType Type of the color algorithm. See MethodType.
		\retval QString Translated method name. */
		QString getTypeDescription(MethodType aType) const;

		/** \brief Get sample color from specific angle.
		\param angle Angle of the color in the wheel. An angle for transformation counting.
		E.g. base angle is 30, param is 90 - transformation is 120.
		There is easy "convert 665485 into 0-359 interval" algorithm too.
		\retval ScColor Scribus color of the angle. */
		ScColor colorByAngle(int angle);

		//! \brief Call one of makeFoo() methods depending on the m_methodType value.
		void makeColors();

		/*! \brief Setup the values by given QColor.
		It sets all options by given color (from input color dialogs).
		\param col examined color
		\retval true on color found, false when color not found - black or white etc.*/
		bool recomputeColor(const ScColor& col);

	signals:
		/** \brief Signal raised by mouse click on widget by user.
		\param button Mouse button number. See Qt docs.
		\param point Coordinates of the mouse pointer. */
		void clicked(int button, const QPoint & point);

	protected:
		/*! \brief Internal color mapping.
		It provides angle-color dictionary.
		*/
		ColorMap m_colorMap;

		/** \brief List of the colors created in this widget.
		Colors can be added into Scribus color list later. */
		ColorList m_colorList;

		/** \brief RGB interpretation of the leading point in the wheel. */
		ScColor m_actualColor;

		//! \brief name of the "base color" to handle in extern color lists
		QString m_trBaseColor;

		/** \brief Difference between selected value and counted ones.
		Let's set angle = 15 and base point e.g. 60 (everything in grades).
		Now you will have Analogous angles 60 (base) and 75 (+15) and 45 (-15).
		Exact interpretation of the angle value depends on the MethodType value.
		*/
		int m_angle { 0 };

		/** \brief Angle diff between colorMap and painted wheel itself.
		QTransform wheel and colorMap have different start points.
		It's taken from Qt. */
		int m_angleShift { 270 };

		/*! \brief Angle of the base color */
		int m_baseAngle { 0 };

		//! \brief Which color model is in use.
		colorModel m_colorspace { colorModelRGB };

		//! \brief Actual type of color computing. See MethodType.
		MethodType m_methodType { Monochromatic };

		struct PaintPoint
		{
			int angle { 0 };
			bool base { false };
		};
		QList<PaintPoint> m_pointList;

		/*! \brief Half of the widget sizes.
		To prevent all width()/2 divisions. */
		int m_widthH { 150 };
		int m_heightH { 150 };

		ScribusDoc* m_doc { nullptr };

		/** \brief An event for mouse actions handling.
		See \see clicked() for more info.
		\param e Mouse properties. */
		void mouseReleaseEvent(QMouseEvent *e) override;
		/** \brief Mouse handling.
		It calls mouseReleaseEvent
		\param e Mouse properties.*/
		void mousePressEvent(QMouseEvent *e) override;
		/** \brief Mouse handling.
		It calls mouseReleaseEvent
		\param e Mouse properties.*/
		void mouseMoveEvent(QMouseEvent *e) override;
		/*! \brief Repaint the widget.
		It prevents the bugs with another window moving over it */
		void paintEvent(QPaintEvent *) override;

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

		/** \brief Draw center circle filled with base color */
		void paintCenterSample();
		/** \brief Draw a color wheel. */
		void paintWheel();

		/** \brief Appends a color into \see colorList.
		\param angle Angle of the color in the wheel. An angle for transformation counting.
		E.g. base angle is 30, param is 90 - transformation is 120.
		There is easy "convert 665485 into 0-359 interval" algorithm too.
		\retval ScColor Scribus color of the angle. */
		ScColor sampleByAngle(int angle);

		/** \brief Appends the base color into color list. */
		void baseColor();

		/** \brief Creates a Scribus ScColor from rgb value.
		Its result depends on the m_colorspace value.
		\param col a ScColor to convert.
		\retval ScColor Scribus color structure */
		ScColor colorSpaceColor(const ScColor& col) const;

		/** \brief Display user selection - selected colors.
		Chosen colors are marked via bullets on the border of
		the wheel.
		\param angle angle of the drawn mark
		\param base Draw it highlighted if it is base color
		\param clear Draw point with white = clear it from painter
		*/
		void drawBorderPoint(int angle, bool base = false, bool clear = false);

		/** \brief Counts an angle of the point in color wheel.
		Modified method from Qt QDial widget.
		\param p coordinates of the point.
		\retval int index in the colorMap */
		int valueFromPoint(const QPoint & p) const;
};

#endif

