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
class PolygonWidget;

class SCRIBUS_API PolygonProps : public QDialog
{
	Q_OBJECT

public:
	PolygonProps(QWidget* parent, int polyC, int polyFd, double polyF, bool polyS, double polyR, double polyCurvature);
	~PolygonProps() {};
	void getValues(int* polyC, int* polyFd, double* polyF, bool* polyS, double* polyR, double* polyCurvature);
	PolygonWidget* polyWidget;
	QPushButton* okButton;
	QPushButton* cancelButton;

protected:
	QVBoxLayout* PolygonPropsLayout;
	QHBoxLayout* Layout1;
};

#endif // POLYGONPROPS_H
