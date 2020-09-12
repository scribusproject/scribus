/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FONTLISTVIEW_H
#define FONTLISTVIEW_H

#include <QTableView>

#include "scfonts.h"


/*! \brief A table view for FontListModel.
Customized QTableView to handle FontListModel in the
unified way (look and feel).
\author Petr Vanek <petr@scribus.info>
 */
class SCRIBUS_API FontListView : public QTableView
{
	Q_OBJECT

	public:
		FontListView(QWidget* parent = nullptr);

		void setModel(QAbstractItemModel * model);
		void setFonts(const SCFonts& f);

		QString fontName(int i);
		bool    isFontUsable(int i);
		bool    isFontSubsetted(int i);
};

#endif
