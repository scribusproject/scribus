#ifndef ARROWCHOOSER_H
#define ARROWCHOOSER_H

#include <qcombobox.h>
class ScribusDoc;

class ArrowChooser : public QComboBox
{
    Q_OBJECT

public:
	ArrowChooser(QWidget* pa, bool direction);
	~ArrowChooser() {};
	void rebuildList(ScribusDoc *currentDoc);

private:
	bool arrowDirection;
};

#endif
