/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef GUIDESDELEGATE_H
#define GUIDESDELEGATE_H

#include <QItemDelegate>

class ScribusDoc;


/*! \brief A delegate/editor for guides model.
It's based on the ScrSpinBox widget. User cannot enter
any others (ugly and bad) values.
\author Petr Vanek <petr@scribus.info>
*/
class GuidesDelegate : public QItemDelegate
{
	Q_OBJECT

	public:
		GuidesDelegate(QObject *parent = 0);

// implemented in inheritance
// 		QWidget *createEditor(QWidget *parent,
// 							const QStyleOptionViewItem &option,
// 							const QModelIndex &index) const;

		void setEditorData(QWidget *editor, const QModelIndex &index) const;

		void setModelData(QWidget *editor,
						QAbstractItemModel *model,
						const QModelIndex &index) const;

		void updateEditorGeometry(QWidget *editor,
								const QStyleOptionViewItem &option,
								const QModelIndex &index) const;

		void unitChange(int docUnitDecimals);
		void setDoc(ScribusDoc * doc);

	// protected due const in createEditor
	protected:
		ScribusDoc *m_doc;
};


/*! \brief A delegate for horizontal guides model/view.
Limit guides for page height
 */
class GuidesHDelegate : public GuidesDelegate
{
	Q_OBJECT

	public:
		GuidesHDelegate(QObject *parent = 0);

		QWidget *createEditor(QWidget *parent,
							  const QStyleOptionViewItem &option,
							  const QModelIndex &index) const;
};


/*! \brief A delegate for vertical guides model/view.
Limit guides for page width
 */
class GuidesVDelegate : public GuidesDelegate
{
	Q_OBJECT

	public:
		GuidesVDelegate(QObject *parent = 0);

		QWidget *createEditor(QWidget *parent,
							  const QStyleOptionViewItem &option,
							  const QModelIndex &index) const;
};

#endif
