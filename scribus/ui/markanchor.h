#ifndef MARKANCHOR_H
#define MARKANCHOR_H

#include "markinsert.h"
#include "ui_markanchor.h"

class SCRIBUS_API MarkAnchorDlg : public MarkInsertDlg, private Ui::MarkAnchorDlg
{
    Q_OBJECT

public:
	explicit MarkAnchorDlg(QWidget *parent = 0);
	~MarkAnchorDlg() {}
	virtual void values(QString& label);
	virtual void setValues(const QString label);
protected:
	void changeEvent(QEvent *e);
};

#endif // MARKANCHOR_H
