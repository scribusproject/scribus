#ifndef POLYGONPROPS_H
#define POLYGONPROPS_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qpixmap.h>
class PolygonWidget;

class PolygonProps : public QDialog
{
	Q_OBJECT

public:
	PolygonProps(QWidget* parent, int PolyC, int PolyFd, double PolyF, bool PolyS, double PolyR);
	~PolygonProps() {};
	void getValues(int* PolyC, int* PolyFd, double* PolyF, bool* PolyS, double* PolyR);
	PolygonWidget* polyWidget;
	QPushButton* okButton;
	QPushButton* cancelButton;

protected:
	QVBoxLayout* PolygonPropsLayout;
	QHBoxLayout* Layout1;
};

#endif // POLYGONPROPS_H
