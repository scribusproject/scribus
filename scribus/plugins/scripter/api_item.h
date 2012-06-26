/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_ITEM_H_
#define API_ITEM_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"

class ItemAPI : public QObject
{
	Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
	Q_PROPERTY(QString fillColor READ fillColor WRITE setFillColor)
	Q_PROPERTY(QString lineColor READ lineColor WRITE setLineColor)
	Q_PROPERTY(double fillShade READ fillShade WRITE setFillShade)
	Q_PROPERTY(double lineShade READ lineShade WRITE setLineShade)
	Q_PROPERTY(double fillTransparency READ fillTransparency WRITE setFillTransparency)
	Q_PROPERTY(double lineTransparency READ lineTransparency WRITE setLineTransparency)
	Q_PROPERTY(bool locked READ locked WRITE setLocked)
	Q_PROPERTY(bool sizeLocked READ sizeLocked WRITE setSizeLocked)
	Q_PROPERTY(bool flipVertical READ imageFlippedV WRITE setImageFlippedV)
	Q_PROPERTY(bool flipHorizontal READ imageFlippedH WRITE setImageFlippedH)
	Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth)
	Q_PROPERTY(QString customLineStyle READ customLineStyle WRITE setCustomLineStyle)
	Q_PROPERTY(int startArrowIndex READ startArrowIndex WRITE setStartArrowIndex)
	Q_PROPERTY(int endArrowIndex READ endArrowIndex WRITE setEndArrowIndex)

	Q_PROPERTY(bool printEnabled READ printEnabled WRITE setPrintEnabled)
	Q_PROPERTY(double xPos READ xPos WRITE setXPos)
	Q_PROPERTY(double yPos READ yPos WRITE setYPos)
	Q_PROPERTY(double width READ width WRITE setWidth)
	Q_PROPERTY(double height READ height WRITE setHeight)
	Q_PROPERTY(double rotation READ rotation WRITE setRotation)
	Q_PROPERTY(bool reversed READ reversed WRITE setReversed)
	Q_PROPERTY(double cornerRadius READ cornerRadius WRITE setCornerRadius)
	Q_PROPERTY(double columnGap READ columnGap WRITE setColumnGap)
	Q_PROPERTY(int columns READ columns WRITE setColumns)

public:
    ItemAPI(PageItem *it);
    virtual ~ItemAPI();

public slots:
	void move(double dx, double dy);
	void moveAbs(double x, double y);
	void rotate(double rot);
	void rotateAbs(double rot);
	void resize(double width, double height);

private:
	PageItem *item;
	
	/**
	 * Properties
	 */
    QString name();
    void setName(QString name);
	
	QString fillColor();
	void setFillColor(QString color);
	
	QString lineColor();
	void setLineColor(QString color);
	
	double fillShade();
	void setFillShade(double value);
	
	double lineShade();
	void setLineShade(double value);
	
	double fillTransparency();
	void setFillTransparency(double value);
	
	double lineTransparency();
	void setLineTransparency(double value);
	
	bool locked();
	void setLocked(bool value);
	
	bool sizeLocked();
	void setSizeLocked(bool value);
	
	bool imageFlippedV();
	void setImageFlippedV(bool value);
	
	bool imageFlippedH();
	void setImageFlippedH(bool value);
	
	double lineWidth();
	void setLineWidth(double value);
	
	QString customLineStyle();
	void setCustomLineStyle(QString name);
	
	int startArrowIndex();
	void setStartArrowIndex(int value);
	
	int endArrowIndex();
	void setEndArrowIndex(int value);
	
	bool printEnabled();
	void setPrintEnabled(bool value);
	
	double xPos();
	void setXPos(double value);
	
	double yPos();
	void setYPos(double value);
	
	double width();
	void setWidth(double value);

	double height();
	void setHeight(double value);
	
	double rotation();
	void setRotation(double rotation);
	
	bool reversed();
	void setReversed(bool value);
	
	double cornerRadius();
	void setCornerRadius(double value);
	
	double columnGap();
	void setColumnGap(double value);
	
	int columns();
	void setColumns(int value);
};

#endif /*API_ITEM_H_*/
