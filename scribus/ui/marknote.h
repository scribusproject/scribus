#ifndef MARKNOTE_H
#define MARKNOTE_H

#include "markinsert.h"
#include "notesstyles.h"
#include "ui_marknote.h"

class SCRIBUS_API MarkNoteDlg : public MarkInsertDlg, private Ui::MarkNoteDlg
{
	Q_OBJECT
	
public:
	explicit MarkNoteDlg(const QList<NotesStyle*>& notesStylesList, QWidget *parent = NULL);
	virtual NotesStyle* values();
	virtual void setValues(NotesStyle* defaultStyle);
protected:
	void changeEvent(QEvent *e);
private slots:
	void on_buttonBox_accepted();
};

#endif // MARKNOTE_H
