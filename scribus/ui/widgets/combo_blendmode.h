#ifndef COMBOBLENDMODE_H
#define COMBOBLENDMODE_H

#include <QComboBox>
#include <QObject>

#include "scribusapi.h"

/* ********************************************************************************* *
 *
 * Section Container Header
 *
 * ********************************************************************************* */

class SCRIBUS_API ComboBlendMode : public QComboBox
{
public:
	ComboBlendMode(QWidget *parent = nullptr);

public slots:
	void languageChange();

protected:
	void initialize();
};

#endif // COMBOBLENDMODE_H
