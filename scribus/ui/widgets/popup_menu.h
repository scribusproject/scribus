#ifndef POPUP_MENU_H
#define POPUP_MENU_H

#include <QMenu>
#include <QWidgetAction>

#include "scribusapi.h"

/* ********************************************************************************* *
 *
 * Section Container Header
 *
 * ********************************************************************************* */

class SCRIBUS_API PopupMenu : public QMenu
{
public:
	PopupMenu(QWidget*contentWidget);

protected:
	bool eventFilter(QObject *obj, QEvent *event);

	// Painting on MacOS has issues with menu item selection
#ifndef Q_OS_MACOS
	void paintEvent(QPaintEvent *event);
#endif
};

#endif // POPUP_MENU_H
