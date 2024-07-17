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

#ifndef COLORPICKER_GRADIENT_H
#define COLORPICKER_GRADIENT_H

#include "ui_colorpicker_gradient.h"
#include <QWidget>
#include <QComboBox>
#include "scribusdoc.h"
#include "colorpicker_enum.h"

class ColorPickerGradient : public QWidget, Ui::ColorPickerGradient
{
	Q_OBJECT

public:
	explicit ColorPickerGradient(QWidget *parent = nullptr);
	~ColorPickerGradient() {};

	void setDoc(ScribusDoc* doc);

	void setGradientList(const GradientList& list);
	void setColorList(const ColorList& list);

	Context context() const;
	void setContext(Context config);

	void setSelector(QComboBox *selector);

	const CPGradientData& gradientData() const;
	void setGradientData(const CPGradientData& gradient);

	const CPGradientVectorData& gradientVectorData() const;
	void setGradientVectorData(const CPGradientVectorData& data);

	const CPColorData& gradientMeshData() const;
	void setGradientMeshData(const CPColorData& data);

	int type() const;

	GradientEdit gradientEditMode() const;
	void setGradientEditMode(GradientEdit mode);

	bool gradientEditPropertiesEnabled() const;
	void setGradientEditPropertiesEnabled(bool enabled);

	QString toolTipText() const;

	ColorPickerGradientProperties *Properties() { return properties; }
	ColorPickerGradientSwatches * Swatches() { return swatches; }

	void disableGradientEditProperties();
	void enableMeshColor(bool enable);

public slots:
	void languageChange();
	void unitChange();

	void handleGradientEditProperties();

private slots:
	void updateGradientFromSwatches(QString name, VGradient gradient);
	void updateGradientFromEditor();
	void updateGradientTypeFromSelector();
	void updateFourColors();
	void updateMeshColor();
	void updateRepeatMethod();
	void updateSize();

private:
	void updateGradient();
	void updateGradientMesh();
	void updateUI();

	ColorListBox* listColor1Swatches { nullptr };
	ColorListBox* listColor2Swatches { nullptr };
	ColorListBox* listColor3Swatches { nullptr };
	ColorListBox* listColor4Swatches { nullptr };
	ColorListBox* listColorMesh { nullptr };
	QComboBox* gradientSelector { nullptr };

	ScribusDoc *m_doc { nullptr };
	CPGradientData m_gradient;
	CPColorData m_gradientMesh;
	Context m_context { Context::Simple };
	int currentUnit {0};
//	int m_uiState {-1};
	ColorList colList;
	GradientEdit m_gradientMeshEdit;

	bool isMask() const;

	void connectSlots();
	void disconnectSlots();

	void setCurrentGradientType(int type);
	void setCurrentGradientName(QString name);
	void setCurrentGradient(const VGradient& gradient);
	void setCurrentFourColors(QString color1, QString color2, QString color3, QString color4,
							  double color1Shade, double color2Shade, double color3Shade, double color4Shade,
							  double color1Alpha, double color2Alpha, double color3Alpha, double color4Alpha
							  );
	void setCurrentMeshPoint(QString colorName, double shade, double alpha);
	void setCurrentRepeatMethod(VGradient::VGradientRepeatMethod method);
	QBrush colorBrush(QSize size, QString colorName, double shade, double opacity) const;

signals:
	void gradientChanged();
	void gradientVectorChanged();
	void gradientMeshChanged();
	void gradientVectorEditMode();
	void sizeChanged();
};

#endif // COLORPICKER_GRADIENT_H
