#include "mark2item.h"
#include "pageitem_noteframe.h"
#include "scribus.h"

Mark2Item::Mark2Item(QWidget *parent) : MarkInsert(parent)
{
	setupUi(this);
	ScribusMainWindow* scmw = (ScribusMainWindow*) parent;
	QStringList itemsList;

	for (int i = 0; i < scmw->doc->Items->count(); i++)
	{
		PageItem* item = scmw->doc->Items->at(i);
		if (item->isAutoNoteFrame())
			continue; //omit notes frames automaticaly created and removed
		itemsList.append(item->itemName());
	}
	ItemList->addItems(itemsList);
	setWindowTitle(tr("Mark to Item"));
}

void Mark2Item::values(QString &label, PageItem* &ptr)
{
	QString itemName = ItemList->currentText();
	if (!itemName.isEmpty())
	{
		ScribusMainWindow* scmw = (ScribusMainWindow*) parent();
		ptr = scmw->doc->getItemFromName(itemName);
		label = labelEdit->text();
	}
}

void Mark2Item::setValues(const QString label, const PageItem* ptr)
{
	int index = (ptr == nullptr) ? -1 : ItemList->findText(ptr->itemName());
	ItemList->setCurrentIndex(index);
	labelEdit->setText(label);
}

void Mark2Item::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
