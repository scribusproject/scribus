/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PREFSPANEL_H
#define PREFSPANEL_H

#include <QWidget>
#include "scribusapi.h"

/**
 * \brief A base class for all preferences panels.
 *
 * This class provides a simple interface for prefs panel widgets.
 * They must provide an apply() slot that applies any changes to
 * the app, and they must NOT apply any changes until and unless
 * this slot is called.
 */
class SCRIBUS_API PrefsPanel : public QWidget
{
	Q_OBJECT

	public:
		PrefsPanel(QWidget* parent);
		virtual ~PrefsPanel();

	public slots:
		/**
		 * \brief applies changes made to prefs to the app.
		 *
		 * All subclasses MUST override this.
		 *
		 * This slot may be called multiple times, once, or not at all;
		 * no assumptions should be made.
		 */
		virtual void apply() = 0;
};

#endif
