/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMSTYLEIMPORT_H
#define SMSTYLEIMPORT_H

#include <QDialog>

#include "ui_smstyleimport.h"
#include "scribusapi.h"
#include "styles/styleset.h"
#include "scribusstructs.h"


/*! \brief Import styles from outer sla document.
User can choose what to import and what to do with the style
names here.
*/
class SCRIBUS_API SMStyleImport : public QDialog, public Ui::SMStyleImport
{
	Q_OBJECT

	public:
		SMStyleImport(QWidget* parent,
					StyleSet<ParagraphStyle> *pstyleList,
					StyleSet<CharStyle> *cstyleList,
					QMap<QString, multiLine> *lstyleList);
		~SMStyleImport() {};

		//! \brief True if the rename feature is ON.
		bool clashRename();
		QStringList paragraphStyles();
		QStringList characterStyles();
		QStringList lineStyles();

	protected:
		//! \brief Root items in the styleWidget
		QTreeWidgetItem * pstyleItem;
		QTreeWidgetItem * cstyleItem;
		QTreeWidgetItem * lstyleItem;

		/*! \brief Following constants are used as flags for searching.
		Soemthing like: give me all character style related items from
		the list. See commonStyles() for more info.
		*/
		static const int cType = QTreeWidgetItem::UserType;
		static const int pType = QTreeWidgetItem::UserType + 1;
		static const int lType = QTreeWidgetItem::UserType + 2;

		QStringList commonStyles(QTreeWidgetItem * rootItem, int type);

	protected slots:
		void checkAll(bool allChecked);

};

#endif // SMSTYLEIMPORT_H
