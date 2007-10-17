/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHARTABLEMODEL_H
#define CHARTABLEMODEL_H

#include <QAbstractTableModel>

#include "scribusapi.h"

class ScribusDoc;
class ScFace;


//! \brief A special type for character classes
typedef QList<uint> CharClassDef;


/*! \brief A model (MVC) to handle unicode characters map.
It's a backend for CharTableView - its GUI representation.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API CharTableModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		CharTableModel(QObject *parent = 0, int cols = 4, ScribusDoc * doc = 0, const QString & font = 0);

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		//! \brief Get a graphics representation/pixmap of the glyph
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

		void setFontInUse(QString font);

		//! \brief Font in use. It's used in model's view.
		ScFace fontFace();

		void setCharacters(CharClassDef ch);
		CharClassDef characters() { return m_characters; };

		//! \brief called to erase glyph at index from table.
		bool removeCharacter(int index);

		void setDoc(ScribusDoc *doc);

	public slots:
		/*! \brief appends an unicode char into m_characters list.
		\param s a QString with numerical representation of the character.
		\param base an optional parameter containing base of the numerical converion. See QString::toInt() documentation.
		The base parameter is used mainly in normal code - not in slot calls.
		*/
		void appendUnicode(QString s, uint base = 16);

	private:
		ScribusDoc *m_doc;
		//! \brief Number of the columns for model
		int m_cols;

		QString m_fontInUse;
		CharClassDef m_characters;

		/*! \brief All drag'n'drop actions are handled in this model only
		See Qt4 docs "Using Drag and Drop with Item Views" for more info.
		*/
		Qt::ItemFlags flags(const QModelIndex &index) const;
		Qt::DropActions supportedDropActions() const;
		QStringList mimeTypes() const;
		QMimeData * mimeData(const QModelIndexList &indexes) const;
		bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
};

#endif
