/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_LAYER_H_
#define API_LAYER_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"
#include "sclayer.h"

/*
 * ScLayerWrapper is a wrapper class for ScLayer, which has
 * all properties of ScLayer defined via Q_PROPERTY
 */

class LayerAPI : public QObject
{
    Q_OBJECT
	
    Q_PROPERTY(QString name READ getName WRITE setName)
	Q_PROPERTY(int id READ getID)
    Q_PROPERTY(int level READ getLevel WRITE setLevel)
    Q_PROPERTY(bool printable READ isPrintable WRITE setPrintable)
    Q_PROPERTY(bool viewable READ isViewable WRITE setViewable)
    Q_PROPERTY(bool editable READ isEditable WRITE setEditable)
    Q_PROPERTY(bool flowControl READ getFlowControl WRITE setFlowControl)
    Q_PROPERTY(bool outlineMode READ getOutlineMode WRITE setOutlineMode)
    Q_PROPERTY(double transparency READ getTransparency WRITE setTransparency)
    Q_PROPERTY(int blendMode READ getBlendMode WRITE setBlendMode)
    Q_PROPERTY(bool active READ isActive WRITE setActive)
    Q_PROPERTY(QList<QVariant> items READ items)

private:
	ScLayer *innerLayer;
	
public:
    LayerAPI(ScLayer *l);
    virtual ~LayerAPI();
	
	QList<QVariant> items();
    

public slots:
	QString getName();
	void setName(QString name);
	
	int getID();
	
	int getLevel();
	void setLevel(int level);
	
	bool isPrintable();
	void setPrintable(bool value);
	
	bool isViewable();
	void setViewable(bool value);
	
	bool isEditable();
	void setEditable(bool value);
	
	bool getFlowControl();
	void setFlowControl(bool value);
	
	bool getOutlineMode();
	void setOutlineMode(bool value);
	
	double getTransparency();
	void setTransparency(double value);
	
	int getBlendMode();
	void setBlendMode(int value);
	
	bool isActive();
	void setActive(bool value);
	
};

#endif /*API_LAYER_H_*/
