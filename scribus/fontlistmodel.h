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

class ScribusDoc;


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
		FontListModel(QObject * parent = 0, ScribusDoc * doc = 0, bool includeDisabled = false);

		enum ColumnTypes {
			FontUsable = 0,
			FontName,
			FontType,
			FontFormat,
			FontSubset,
			FontAccess,
			FontInDoc,
			FontFile,
			SortIndex // used for sorting by lowercased name
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
		\note the Qt::CheckStateRole stuff is handling
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

		//! Returns the font list
		QList<QString> fonts() { return m_font_names; }
		//! Sets font list and refresh the model. This will detach the model from the main Scribus font list!
		void setFonts(QList<QString> f);

		/*! Get the font name for current index.
		\note Remember to use the mapToSource() if you're using QSortFilterProxyModel
		*/
		QString nameForIndex(const QModelIndex & index);

        bool isLive() const { return m_embedFlags.count() == 0; }
	private:
    
		enum EmbedMethod {
			DontEmbed = 0,
			EmbedFont = 1,
			SubsetFont = 2,
			OutlineFont = 3
		};

		enum EmbedFlags {
			EmbedPS = 1,
			SubsetPDF = 2
		};
    
		ScribusDoc * m_doc;
		//! Scribus fonts. \note: It's shared!
		SCFonts m_fonts;
		QList<ScFace>  m_font_values;
        QList<QString> m_font_names;
		QList<bool>    m_enabledFonts;
        QList<int>     m_embedFlags;
    
		//! Display icons by the Qt::DecorationRole
		QPixmap ttfFont;
		QPixmap otfFont;
		QPixmap psFont;
		QPixmap substFont;

		//! Prepare strings for table headers.
		QVariant headerData(int section,
							Qt::Orientation orientation,
							int role = Qt::DisplayRole) const;

		bool m_includeDisabled;
};

#endif
