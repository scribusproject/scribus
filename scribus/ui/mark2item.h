#ifndef MARK2ITEM_H
#define MARK2ITEM_H

#include "MarkInsertDlg.h"
#include "ui_mark2item.h"

class SCRIBUS_API Mark2ItemDlg : public MarkInsertDlg, private Ui::Mark2ItemDlg
{
    Q_OBJECT

public:
	explicit Mark2ItemDlg(QWidget *parent = 0);
	virtual void values(QString &label, PageItem* &ptr);
	virtual void setValues(const QString label, const PageItem* ptr);

protected:
	void changeEvent(QEvent *e);
};

#endif // MARK2ITEM_H
