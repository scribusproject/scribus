/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/
#ifndef COLOR_SLIDER_H
#define COLOR_SLIDER_H

#include <QPointer>
#include <QSlider>
#include "sccolor.h"
#include "scribusapi.h"

class QPaintEvent;
class QWidget;
class ScribusDoc;

class SCRIBUS_API ColorSlider : public QSlider
{
    Q_OBJECT

public:
	enum Mode
	{
		RGBRed = 0,
		RGBGreen,
		RGBBlue,
		RGBWebRed,
		RGBWebGreen,
		RGBWebBlue,
		LabLightness,
		LabA,
		LabB,
		CMYKCyan,
		CMYKMagenta,
		CMYKYellow,
		CMYKKey,
		HLCHue,
		HLCLightness,
		HLCChroma,
		HSVHue,
		HSVSaturation,
		HSVValue,
		Shade,
		Opacity
	};

	ColorSlider(QWidget* parent = nullptr);
	ColorSlider(Mode mode, QWidget* parent = nullptr);

	const ScColor& color() const { return m_color; }
	double opacity() const { return m_opacity; } // 0.0 - 100.0
	double shade() const { return m_shade; } // 0.0 - 100.0
	Mode mode() const { return m_mode; }
	bool isDynamic() const { return m_dynamicColor; }
	const QImage& backgroundImage() const { return m_background; }

public slots:
	void setDoc(ScribusDoc *doc);
    void setMode(ColorSlider::Mode mode);
	void setValues(const QString &colorName, double opacity, double shade);
	void setValues(ScColor color, double opacity, double shade);
	void setColor(const QString &colorName);
	void setColor(ScColor color);
	void setShade(double shade);
	void setOpacity(double opacity);
	void setIsDynamic(double dynamic);

private slots:
	void updateColor();

protected:
	void paintEvent(QPaintEvent * e) override;
    void resizeEvent(QResizeEvent* e) override;
	void showEvent(QShowEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;

private:
	QPointer<ScribusDoc> m_doc;
    QGradientStops m_stops;
    ScColor m_color;
	double m_opacity {100.0}; // full opacity
	double m_shade {100.0}; // no shade
	int m_colorSteps {100};
	bool m_hasColor {false};
	bool m_dynamicColor {true};
	Mode m_mode {RGBRed};
    QImage m_background;

	int pixelPosToRangeValue(QPoint pos) const;
	int roundToStep(int value) const {
		int mod = maximum() / m_colorSteps;
		return (value / mod) * mod;
	}

    void setup();
    void connectSlots();
    void disconnectSlots();

    void renderMap();
    void updateValues();

signals:
    void colorChanged(ScColor);

};

#endif // COLOR_SLIDER_H
