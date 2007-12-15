/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FONTLISTMODEL_H
#define FONTLISTMODEL_H

#include <QAbstractTableModel>
#include <QItemDelegate>
#include <QPixmap>

#include "scfonts.h"


/*! \brief Model for font views.
It contains quite all informations about fonts available to display
in Qt4 views. It's suggested to use custom FontListView.

You can call view->hideColumn(ColumnType) to hide unneeded columns for
user.

When user edits the allowed items (see flags()) it's promoted directly
into the doc/prefs.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API FontListModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		FontListModel(QObject * parent = 0);

		enum ColumnTypes {
			FontName = 0,
			FontUsable,
			FontFamily,
			FontStyle,
			FontVariant,
			FontType,
			FontFormat,
			FontEmbed,
			FontSubset,
			FontAccess,
			FontInDoc,
			FontFile
		};

		/*! Required inherited method. See Qt4 docs.
		It returns the count of fonts. */
		int rowCount(const QModelIndex&) const;
		//! The same behaviour as the previous one.
		int rowCount();
		/*! Required inherited method. See Qt4 docs.
		It *must* return the count of ColumnTypes items */
		int columnCount(const QModelIndex&) const;
		/*! Required inherited method. See Qt4 docs.
		It handles displaying for user itself.
		\Note the Qt::CheckStateRole stuff is handling
		user input. And should not be removed. */
		QVariant data(const QModelIndex & index,
					  int role = Qt::DisplayRole) const;
		/*! It handles user inputs. Only Qt::CheckStateRole
		should be allowed. No additional delegates are presented. */
		bool setData(const QModelIndex & index,const QVariant & value,
					 int role = Qt::EditRole);
		/*! Setup features for all columns. Only here named columns
		are able to be edited by user. */
		Qt::ItemFlags flags(const QModelIndex &index) const;

		//! Returns Scribus fonts. TODO: is it required?
		SCFonts fonts() { return m_fonts; };
		//! Sets Scribus fonts and refresh the model. TODO: is it required?
		void setFonts(SCFonts f);

		//! Get the font name for current index. TODO: It will be remove propably
		QString nameForIndex(const QModelIndex & index);

	private:
		//! Scribus fonts. \note: It's shared!
		SCFonts m_fonts;

		//! Display icons by the Qt::DecorationRole
		QPixmap ttfFont;
		QPixmap otfFont;
		QPixmap psFont;
		QPixmap substFont;

		//! Prepare strings for table headers.
		QVariant headerData(int section,
							Qt::Orientation orientation,
							int role = Qt::DisplayRole) const;
};

#endif
