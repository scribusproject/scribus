/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef POLYGONPROPS_H
#define POLYGONPROPS_H

#include <QDialog>
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;

#include "scribusapi.h"
#include "ui/scrpalettebase.h"
class PolygonWidget;

class SCRIBUS_API PolyVectorDialog :  public ScrPaletteBase
{
	Q_OBJECT

public:
	PolyVectorDialog(QWidget* parent, int polyC, double polyF, bool polyS, double polyR, double polyCurvature, double polyInnerRot, double polyOuterCurvature);
	~PolyVectorDialog() {};
	void setValues(int polyC, double polyF, bool polyS, double polyR, double polyCurvature, double polyInnerRot, double polyOuterCurvature);
	PolygonWidget* polyWidget;
	QPushButton* okButton;

signals:
	void NewVectors(int, double, bool, double, double, double, double);
	void endEdit();

protected:
	QVBoxLayout* PolygonPropsLayout;
	QHBoxLayout* Layout1;

};

class SCRIBUS_API PolygonProps : public QDialog
{
	Q_OBJECT

public:
	PolygonProps(QWidget* parent, int polyC, double polyF, bool polyS, double polyR, double polyCurvature, double polyInnerRot, double polyOuterCurvature);
	~PolygonProps() {};
	void getValues(int* polyC, double* polyF, bool* polyS, double* polyR, double* polyCurvature, double* polyInnerRot, double* polyOuterCurvature);
	void setValues(int polyC, double polyF, bool polyS, double polyR, double polyCurvature, double polyInnerRot, double polyOuterCurvature);
	PolygonWidget* polyWidget;
	QPushButton* okButton;
	QPushButton* cancelButton;

protected:
	QVBoxLayout* PolygonPropsLayout;
	QHBoxLayout* Layout1;

signals:
	void NewVectors(int, double, bool, double, double, double, double);
};

#endif // POLYGONPROPS_H
