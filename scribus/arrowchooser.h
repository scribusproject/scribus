#ifndef ARROWCHOOSER_H
#define ARROWCHOOSER_H

#include <qcombobox.h>
//Added by qt3to4:
#include <Q3ValueList>
#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API ArrowChooser : public QComboBox
{
    Q_OBJECT

public:
	ArrowChooser(QWidget* pa, bool direction);
	~ArrowChooser() {};
	void rebuildList(Q3ValueList<ArrowDesc> *arrowStyles);

private:
	bool arrowDirection;
};

#endif
