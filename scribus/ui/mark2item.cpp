#include "mark2item.h"
#include "pageitem_noteframe.h"
#include "scribus.h"

Mark2ItemDlg::Mark2ItemDlg(QWidget *parent) : MarkInsertDlg(parent)
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

void Mark2ItemDlg::values(QString &label, PageItem* &ptr)
{
	QString itemName = ItemList->currentText();
	if (itemName != "")
	{
		ScribusMainWindow* scmw = (ScribusMainWindow*) parent();
		ptr = scmw->doc->getItemFromName(itemName);
		label = labelEdit->text();
	}
}

void Mark2ItemDlg::setValues(const QString label, const PageItem* ptr)
{
	int index = (ptr == NULL)? -1:ItemList->findText(ptr->itemName());
	ItemList->setCurrentIndex(index);
	labelEdit->setText(label);
}

void Mark2ItemDlg::changeEvent(QEvent *e)
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
