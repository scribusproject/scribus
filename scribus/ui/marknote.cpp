#include "marknote.h"
#include "scribus.h"

static int lastNotesStyleIndex = 0;  //remember last selected notes style

MarkNote::MarkNote(const QList<NotesStyle*>& notesStylesList, QWidget *parent) :
	MarkInsert(notesStylesList, parent)
{
	setupUi(this);

	for (int i = 0; i < notesStylesList.count(); i++)
		ItemList->addItem(notesStylesList.at(i)->name(), QVariant::fromValue((void*) notesStylesList.at(i)));
	setWindowTitle(tr("Insert Foot/Endnote"));
	ItemList->setCurrentIndex(lastNotesStyleIndex);
}

NotesStyle* MarkNote::values()
{
	int index = ItemList->currentIndex();
	return (NotesStyle*) ItemList->itemData(index).value<void*>();
}

void MarkNote::setValues(NotesStyle* defaultStyle)
{
	ItemList->setCurrentIndex(ItemList->findText(defaultStyle->name()));
}

void MarkNote::changeEvent(QEvent *e)
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

void MarkNote::on_buttonBox_accepted()
{
	lastNotesStyleIndex = ItemList->currentIndex();
}
