#ifndef MARKANCHOR_H
#define MARKANCHOR_H

#include "markinsert.h"
#include "ui_markanchor.h"

class SCRIBUS_API MarkAnchor : public MarkInsert, private Ui::MarkAnchorDlg
{
    Q_OBJECT

public:
	explicit MarkAnchor(QWidget *parent = 0);
	~MarkAnchor() {}
	virtual void values(QString& label);
	virtual void setValues(const QString label);

protected:
	void changeEvent(QEvent *e);
};

#endif // MARKANCHOR_H
