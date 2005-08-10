#ifndef ARROWCHOOSER_H
#define ARROWCHOOSER_H

#include <qcombobox.h>
#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API ArrowChooser : public QComboBox
{
    Q_OBJECT

public:
	ArrowChooser(QWidget* pa, bool direction);
	~ArrowChooser() {};
	void rebuildList(QValueList<ArrowDesc> *arrowStyles);

private:
	bool arrowDirection;
};

#endif
