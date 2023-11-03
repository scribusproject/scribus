/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SAMPLEITEM_H
#define SAMPLEITEM_H

#include <QObject>
#include <QPixmap>
#include "scribusapi.h"
#include "scribusstructs.h"

class QString;
class QColor;
class ScribusDoc;


/*! \brief Provides sample "text frame" as pixmap.
You can create a pixmap with standard Scribus text frame
here. It can be used as a kind of preview.
\author Petr Vanek <petr@yarpen.cz>
*/
class SCRIBUS_API SampleItem : QObject
{
	Q_OBJECT

	public:
		SampleItem();
		~SampleItem();

		/*! \brief Set device pixel ratio */
		void setDevicePixelRatio(double ratio) { m_devicePixelRatio = ratio; }

		/*! \brief Set sample text.
		\param aText QString to set. */
		void setText(const QString& aText);
		/*! \brief Use lorem ipsum generator as text source.
		\param para count of the LI paragraphs */
		void setLoremIpsum(int para);
		/*! \brief Set background color of the pixmap.
		\param c a QColor */
		void setBgColor(const QColor& c);
		void setBgShade(int c);
		/*! \brief Set color management for background color of the pixmap.
		\param enable if color management should be enabled */
		void setBgColorMgmt(bool enable);
		/*! \brief Set Text color of the pixmap.
		\param c a QColor */
		void setTxColor(const QColor& c);
		void setTxShade(int c);

		/*! \brief Set whole Scribus paragraph style in one.
		\param aStyle Paragraph style to set.*/
		void setStyle(const ParagraphStyle& aStyle);
		/*! \brief Following methods set only one style characteristic */
		void setLineSpacingMode(int lineSpaMode);
		void setLineSpacing(double lineSpa);
		void setTextAlignment(int textAlignment);
		void setLeftMargin(double indent);
		void setRightMargin(double indent);
		void setFirstIndent(double first);
		void setGapBefore(double gapBefore);
		void setGapAfter(double gapAfter);
		void setFont(const QString& font);
		/*! \brief Set size of the font.
		\param fontSize fontSize * 10.0 usually
		\param autoLineSpa if true - use automatic line spacing computing */
		void setFontSize(int fontSize, bool autoLineSpa = false);
		//void setTabValues(QValueList<PageItem::TabRecord> tabValues);
		void setDrop(bool drop);
		void setBullet(bool bul);
		void setNum(bool num);
		void setDropCapLines(int dropLin);
		void setParEffectDist(double dropDist);
		void setFontEffect(int fontEffect);
		void setTxtFillColor(const QString& fColor);
		void setTxtFillShade(double fShade);
		void setTxtStrokeColor(const QString& sColor);
		void setTxtStrokeShade(double sShade);
		void setTxtShadowX(double txtShadowX);
		void setTxtShadowY(double txtShadowY);
		void setTxtOutlineWidth(double txtOutline);
		void setTxtUnderlineOffset(double txtUnderPos);
		void setTxtUnderlineWidth(double txtUnderWidth);
		void setTxtStrikethruOffset(double txtStrikePos);
		void setTxtStrikethruWidth(double txtStrikeWidth);
		void setScaleH(double scaleH);
		void setScaleV(double scaleV);
		void setBaselineOffset(double baseOff);
		void setKernVal(double kernVal);
		void setWordTracking(double wordTracking);

		/*! \brief Get the sample.
		\param width W of the sample
		\param height H of the sample
		\retval QPixmap rendered image
		*/
		QPixmap getSample(int width, int height);

	private:
		//! \brief Text to be rendered
		QString m_text;
		//! \brief Internal temporary paragraph style
		ParagraphStyle m_tmpStyle;
		/*! \brief Reference to a document.
		Existing or created one */
		ScribusDoc* m_Doc { nullptr };
		//! \brief Is the doc created used only? true = used
		int m_bgShade { 100 };
		//! \brief Device pixel ratio
		double m_devicePixelRatio { 1.0 };
};

#endif
