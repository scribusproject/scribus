#ifndef MARKNOTE_H
#define MARKNOTE_H

#include "markinsert.h"
#include "notesstyles.h"
#include "ui_marknote.h"

class SCRIBUS_API MarkNote : public MarkInsert, private Ui::MarkNoteDlg
{
	Q_OBJECT
	
public:
	explicit MarkNote(const QList<NotesStyle*>& notesStylesList, QWidget *parent = NULL);
	virtual NotesStyle* values();
	virtual void setValues(NotesStyle* defaultStyle);

protected:
	void changeEvent(QEvent *e);

private slots:
	void on_buttonBox_accepted();
};

#endif // MARKNOTE_H
