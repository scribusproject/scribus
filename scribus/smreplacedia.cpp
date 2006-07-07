/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smreplacedia.h"
#include "smreplacedia.moc"

#include <qtable.h>
#include <qheader.h>

SMReplaceDia::SMReplaceDia(const QStringList &toBeDeleted, const QStringList &replaceOptions) : SMReplaceDiaBase()
{
// 	table->verticalHeader()->hide();
// 	table->setLeftMargin(0);
// 	table->setColumnReadOnly(0, true);
// 	table->setNumRows(toBeDeleted.count());

	QStringList options;
	for (uint i = 0; i < replaceOptions.count(); ++i)
	{
		if (!toBeDeleted.contains(replaceOptions[i]))
			options << replaceOptions[i];
	}
	
// 	for (uint i = 0; i < toBeDeleted.count(); ++i)
// 	{
// 		table->setText(i, 0, toBeDeleted[i]);
// 		table->setItem(i, 1, new QComboTableItem(table, options));
// 	}
}

QValueList<RemoveItem> SMReplaceDia::items()
{
	QValueList<RemoveItem> tmp;
// 	for (int i = 0; i < table->numRows(); ++i)
// 	{
// 		QString s1 = table->text(i, 1);
// 		QComboTableItem *qcti = dynamic_cast<QComboTableItem*>(table->item(i,1));
// 		QString s2 = qcti->text(qcti->currentItem());
// 		tmp.append(RemoveItem(s1, s2));
// 		
// 	}
	return tmp;
}

SMReplaceDia::~SMReplaceDia()
{
// 	for (int i = 0; i < table->numRows(); ++i)
// 		delete table->item(i, 1);
}
