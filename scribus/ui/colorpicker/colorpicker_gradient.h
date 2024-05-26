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

	void setGradientList(const GradientList& list);
	void setColorList(const ColorList& list);

	Context context();
	void setContext(Context config);

	void setSelector(QComboBox *selector);

	void setGradientData(CPGradientData gradient);
	CPGradientData gradientData();

	void setGradientVectorData(CPGradientVectorData data);
	CPGradientVectorData gradientVectorData();

	void setGradientMeshData(CPColorData data);
	CPColorData gradientMeshData();

	int type();

	GradientEdit gradientEditMode();
	void setGradientEditMode(GradientEdit mode);

	void setGradientEditPropertiesEnabled(bool enabled);
	bool gradientEditPropertiesEnabled();

	QString toolTipText() const;

	ColorPickerGradientProperties *Properties() { return properties; };
	ColorPickerGradientSwatches * Swatches() { return swatches; };

	void disableGradientEditProperties();
	void enableMeshColor(bool enable);

public slots:
	void setDoc(ScribusDoc *doc);	
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

	ScribusDoc *m_doc {nullptr};
	CPGradientData m_gradient;
	CPColorData m_gradientMesh;
	Context m_context {Context::Simple};
	ColorListBox *listColor1Swatches;
	ColorListBox *listColor2Swatches;
	ColorListBox *listColor3Swatches;
	ColorListBox *listColor4Swatches;
	ColorListBox *listColorMesh;
	int currentUnit {0};
//	int m_uiState {-1};
	ColorList colList;
	QComboBox *gradientSelector;
	GradientEdit m_gradientMeshEdit;

	bool isMask();

	void connectSlots();
	void disconnectSlots();

	void setCurrentGradientType(int type);
	void setCurrentGradientName(QString name);
	void setCurrentGradient(VGradient gradient);
	void setCurrentFourColors(QString color1, QString color2, QString color3, QString color4,
							  double color1Shade, double color2Shade, double color3Shade, double color4Shade,
							  double color1Alpha, double color2Alpha, double color3Alpha, double color4Alpha
							  );
	void setCurrentMeshPoint(QString colorName, double shade, double alpha);
	void setCurrentRepeatMethod(VGradient::VGradientRepeatMethod method);
	QBrush colorBrush(QSize size, QString colorName, double shade, double opacity);

signals:
	void gradientChanged();
	void gradientVectorChanged();
	void gradientMeshChanged();
	void gradientVectorEditMode();
	void sizeChanged();

};

#endif // COLORPICKER_GRADIENT_H
