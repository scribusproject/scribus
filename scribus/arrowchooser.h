#ifndef ARROWCHOOSER_H
#define ARROWCHOOSER_H

#include <qcombobox.h>
#include "scribusstructs.h"

class ArrowChooser : public QComboBox
{
    Q_OBJECT

public:
	ArrowChooser(QWidget* pa, bool direction);
	~ArrowChooser() {};
	void rebuildList(QValueList<arrowDesc> *arrowStyles);

private:
	bool arrowDirection;
};

#endif
