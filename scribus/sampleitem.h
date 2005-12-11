#ifndef SAMPLEITEM_H
#define SAMPLEITEM_H

#include <qobject.h>
#include "scribusapi.h"
#include "scribusstructs.h"

class QString;
class ParagraphStyle;
class QColor;
//class PageItem;
//class TabRecord;


class SCRIBUS_API SampleItem : QObject
{
	Q_OBJECT

	public:
		SampleItem(QWidget *parent);
		~SampleItem();

		void setText(QString aText);
		void setLoremIpsum(int para);
		void setBgColor(QColor c);

		void setStyle(ParagraphStyle aStyle);
		void setBackgroundColor(QColor c);
		void setLineSpaMode(int lineSpaMode);
		void setLineSpa(double lineSpa);
		void setTextAlignment(int textAlignment);
		void setIndent(double indent);
		void setFirst(double first);
		void setGapBefore(double gapBefore);
		void setGapAfter(double gapAfter);
		void setFont(QString font);
		/* val * 10.0 usually */
		void setFontSize(int fontSize);
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

		QPixmap getSample(int width, int height);

	private:
		QString text;
		QColor bgColor;
		ParagraphStyle tmpStyle;
};

#endif
