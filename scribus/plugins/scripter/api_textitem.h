/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_TEXTITEM_H_
#define API_TEXTITEM_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"
#include "pageitem_textframe.h"
#include "api_item.h"

class ScribusTextItem : public ScribusItem
{
	Q_OBJECT
	
	Q_PROPERTY(QString font READ font WRITE setFont)
	Q_PROPERTY(double fontSize READ fontSize WRITE setFontSize)
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(int textLines READ textLines)
	Q_PROPERTY(int textLength READ textLength)
	Q_PROPERTY(QString allText READ allText)
	Q_PROPERTY(double lineSpacing READ lineSpacing WRITE setLineSpacing)
	Q_PROPERTY(QList<QVariant> distances READ distances)
	Q_PROPERTY(bool deleteText READ deleteText)
	Q_PROPERTY(bool traceText READ traceText)
	Q_PROPERTY(bool hyphenate READ hyphenate)
	Q_PROPERTY(bool dehyphenate READ dehyphenate)
	Q_PROPERTY(bool PDFBookMark READ PDFBookMark WRITE setPDFBookMark)

public:
	ScribusTextItem(PageItem_TextFrame* inner);
	~ScribusTextItem();

public slots:
	void insertText(QString text, int position);
	void setLineSpacingMode(int mode);
	void setDistances(double left, double right, double top, double bottom);
	void setTextAlignment(int alignment);
	void setTextColor(QString color);
	void setTextStroke(QString color);
	void setTextScalingV(double value);
	void setTextScalingH(double value);
	void setTextShade(int w);
	void selectText(int start, int selcount);
	void linkToTextFrame(QString name2);
	void unLinkTextFrames();
private:
	QString font();
	void setFont(QString name);

	double fontSize();
	void setFontSize(double size);

	QString text();
	void setText(QString text);

	int textLines();
	int textLength();
	QString allText();

	double lineSpacing();
	void setLineSpacing(double value);

	QList<QVariant> distances();
	
	bool deleteText();
	bool traceText();
	
	int textOverFlows(bool checkLinks);
	
	bool hyphenate();
	bool dehyphenate();
	
	bool PDFBookMark();
	void setPDFBookMark(bool toggle);
	
	PageItem_TextFrame* item;
};

#endif /*API_TEXTITEM_H_*/
