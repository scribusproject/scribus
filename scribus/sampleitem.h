/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SAMPLEITEM_H
#define SAMPLEITEM_H

#include <qobject.h>
#include "scribusapi.h"
#include "scribusstructs.h"

class QString;
class QColor;
class ScribusDoc;


/*! \brief Provides sample "text frame" as pixmap.
You can create a pixmap with standard Scribus text frame
here. It can be used as a kind of preview.
It needs at least one existing ScribusDoc - so there
is one created if ScWM->doc is null (then it's destroyed
of course).
\author Petr Vanek <petr@yarpen.cz>
*/
class SCRIBUS_API SampleItem : QObject
{
	Q_OBJECT

	public:
		SampleItem();
		~SampleItem();

		/*! \brief Set sample text.
		\param aText QString to set. */
		void setText(QString aText);
		/*! \brief Use lorem ipsum generator as text source.
		\param para count of the LI paragraphs */
		void setLoremIpsum(int para);
		/*! \brief Set backgroud color of the pixmap.
		\param c a QColor */
		void setBgColor(QColor c);

		/*! \brief Set whole Scribus paragraph style in one.
		\param aStyle Paragraph style to set.*/
		void setStyle(ParagraphStyle aStyle);
		/*! \brief Folowing methods set only one style characteristic */
		void setLineSpaMode(int lineSpaMode);
		void setLineSpa(double lineSpa);
		void setTextAlignment(int textAlignment);
		void setIndent(double indent);
		void setFirst(double first);
		void setGapBefore(double gapBefore);
		void setGapAfter(double gapAfter);
		void setFont(QString font);
		/*! \brief Set size of the font.
		\param fontSize fontSize * 10.0 usually
		\param autoLineSpa if true - use automatic line spacing computing */
		void setFontSize(int fontSize, bool autoLineSpa=false);
		//void setTabValues(QValueList<PageItem::TabRecord> tabValues);
		void setDrop(bool drop);
		void setDropLin(int dropLin);
		void setDropDist(double dropDist);
		void setFontEffect(int fontEffect);
		void setFColor(QString fColor);
		void setFShade(int fShade);
		void setSColor(QString sColor);
		void setSShade(int sShade);
		void setBaseAdj(bool baseAdj);
		void setTxtShadowX(int txtShadowX);
		void setTxtShadowY(int txtShadowY);
		void setTxtOutline(int txtOutline);
		void setTxtUnderPos(int txtUnderPos);
		void setTxtUnderWidth(int txtUnderWidth);
		void setTxtStrikePos(int txtStrikePos);
		void setTxtStrikeWidth(int txtStrikeWidth);
		void setScaleH(int scaleH);
		void setScaleV(int scaleV);
		void setBaseOff(int baseOff);
		void setKernVal(int kernVal);

		/*! \brief Get the sample.
		\param width W of the sample
		\param width H of the sample
		\retval QPixmap rendered image
		*/
		QPixmap getSample(int width, int height);

	private:
		//! \brief Text to be rendered
		QString text;
		//! \brief Background color
		QColor bgColor;
		//! \brief Internal temporary paragraph style
		ParagraphStyle tmpStyle;
		/*! \brief Reference to a document.
		Existing or created one */
		ScribusDoc *doc;
		//! \brief Is the doc created used only? true = used
		bool used;
};

#endif
