#ifndef POLYGONPROPS_H
#define POLYGONPROPS_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qpixmap.h>

#include "scribusapi.h"
class PolygonWidget;

class SCRIBUS_API PolygonProps : public QDialog
{
	Q_OBJECT

public:
	PolygonProps(QWidget* parent, int polyC, int polyFd, double polyF, bool polyS, double polyR);
	~PolygonProps() {};
	void getValues(int* polyC, int* polyFd, double* polyF, bool* polyS, double* polyR);
	PolygonWidget* polyWidget;
	QPushButton* okButton;
	QPushButton* cancelButton;

protected:
	QVBoxLayout* PolygonPropsLayout;
	QHBoxLayout* Layout1;
};

#endif // POLYGONPROPS_H
