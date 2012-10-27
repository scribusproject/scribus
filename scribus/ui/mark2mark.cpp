#include "marks.h"
#include "mark2mark.h"
#include <QStandardItemModel>

Mark2Mark::Mark2Mark(const QList<Mark*>& marks, Mark* omitMark, QWidget *parent) : MarkInsert(marks, parent)
{
	setupUi(this);
	LabelList->addItem("", QVariant::fromValue((void*) NULL));
	
	//for each marks type
	QString typeStr;
	MarkType typeMrk;

	int index = 0;
	typeMrk = MARKAnchorType;	typeStr = tr("Anchors");
	//adding name of marks type, and make it unselectable
	LabelList->addItem("+++ " + typeStr);
	qobject_cast<QStandardItemModel *>(LabelList->model())->item( ++index )->setEnabled( false );
	for (int i = 0; i < marks.size(); ++i)
	{
		if (marks[i]->isType(typeMrk))
		{
			LabelList->addItem(marks[i]->label, QVariant::fromValue((void*) marks[i]));
			index++;
		}
	}
	typeMrk = MARK2MarkType;	typeStr = tr("Mark to Mark");
	LabelList->addItem("+++ " + typeStr);
	qobject_cast<QStandardItemModel *>(LabelList->model())->item( ++index )->setEnabled( false );
	for (int i = 0; i < marks.size(); ++i)
	{
		if (marks[i]->isType(typeMrk) && marks[i] != omitMark)
		{
			LabelList->addItem(marks[i]->label, QVariant::fromValue((void*) marks[i]));
			index++;
		}
	}
	typeMrk = MARK2ItemType;	typeStr = tr("Mark to Item");
	LabelList->addItem("+++ " + typeStr);
	qobject_cast<QStandardItemModel *>(LabelList->model())->item( ++index )->setEnabled( false );
	for (int i = 0; i < marks.size(); ++i)
	{
		if (marks[i]->isType(typeMrk))
		{
			LabelList->addItem(marks[i]->label, QVariant::fromValue((void*) marks[i]));
			index++;
		}
	}
	typeMrk = MARKNoteMasterType;	typeStr = tr("Note mark");
	LabelList->addItem("+++ " + typeStr);
	qobject_cast<QStandardItemModel *>(LabelList->model())->item( ++index )->setEnabled( false );
	for (int i = 0; i < marks.size(); ++i)
	{
		if (marks[i]->isType(typeMrk))
		{
			LabelList->addItem(marks[i]->label, QVariant::fromValue((void*) marks[i]));
			index++;
		}
	}
//	typeMrk = MARKIndexType;	typeStr = tr("Index entry");
//	LabelList->addItem("+++ " + typeStr);
//	qobject_cast<QStandardItemModel *>(LabelList->model())->item( ++index )->setEnabled( false );
//	for (int i = 0; i < marks.size(); ++i)
//	{
//		if (marks[i]->isType(typeMrk))
//		{
//			LabelList->addItem(marks[i]->label, QVariant::fromValue((void*) marks[i]));
//			index++;
//		}
//	}
	setWindowTitle(tr("Mark to Mark"));
}

void Mark2Mark::values(QString& label, Mark* &mrk)
{
	label = this->labelEdit->text();
	int labelID = LabelList->currentIndex();
	if (labelID == 0)
		mrk= NULL;
	else
		mrk = (Mark*) LabelList->itemData(labelID).value<void*>();
}

void Mark2Mark::setValues(const QString label, const Mark* mrk)
{
	int index = (mrk == NULL)? -1:LabelList->findText(mrk->label);
	LabelList->setCurrentIndex(index);
	labelEdit->setText(label);
}

void Mark2Mark::changeEvent(QEvent *e)
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
