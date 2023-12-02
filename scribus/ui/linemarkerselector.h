#ifndef LINEMARKERSELECTOR_H
#define LINEMARKERSELECTOR_H

#include <QWidget>
#include "scribusapi.h"
#include "scribusstructs.h"
#include "ui_linemarkerselector.h"

class SCRIBUS_API LineMarkerSelector : public QWidget, private Ui::LineMarkerSelector
{
	Q_OBJECT

public:
	explicit LineMarkerSelector(QWidget *parent = nullptr);
	~LineMarkerSelector() {};

	void rebuildList(const QList<ArrowDesc> *arrowStyles);
	void setArrowDirection(ArrowDirection direction);
	void languageChange();

	QIcon currentIcon();
	double scale();
	void setScale(double value);
	int marker();
	void setMarker(int id);
	QString scaleSuffix() const;
	void setScaleSuffix(const QString suffix);

private:
	ArrowDirection m_arrowDirection { ArrowDirection::StartArrow };
	int arrow = 23;

	QPixmap renderPixmap(FPointArray path, int width, int height, int scale);

	void addItem(QPixmap pixmap, const QString &label, int id);

signals:
	void scaleChanged(double);
	void markerChanged(int);

};

#endif // LINEMARKERSELECTOR_H
