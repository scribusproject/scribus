/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef POLYGONWIDGET_H
#define POLYGONWIDGET_H

#include <QWidget>
#include "scribusapi.h"
#include "ui_polygonwidgetbase.h"
struct toolPrefs;
class SCRIBUS_API PolygonWidget : public QWidget, Ui::PolygonWidgetBase
{
	Q_OBJECT

	public:
		PolygonWidget(QWidget* parent);
		PolygonWidget(QWidget* parent, int polyCorners, int polyFd, double polyF, bool polyUseConvexFactor, double polyRotation, double polyCurvature, double polyInnerRot);
		~PolygonWidget() {};

		void setValues(int polyCorners, int polyFd, double polyF, bool polyUseConvexFactor, double polyRotation, double polyCurvature, double polyInnerRot);
		void getValues(int* polyCorners, int* polyFd, double* polyF, bool* polyUseConvexFactor, double* polyRotation, double* polyCurvature, double* polyInnerRot);
		void restoreDefaults(struct ItemToolPrefs *prefsData);
		void saveGuiToPrefs(struct ItemToolPrefs *prefsData);

	protected slots:
		void setFactorSlider(int a);
		void setRotationSlider(int a);
		void setInnerRotationSlider(int a);
		void setCurvatureSlider(int a);
		void updatePreview();
		double GetZeroFactor();
		double GetMaxFactor();
		double GetFactor();

	protected:
		double PFactor;
		void connectSignals(bool);
};

#endif
