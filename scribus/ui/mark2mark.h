#ifndef MARK2MARK_H
#define MARK2MARK_H

#include "markinsert.h"
#include "ui_mark2mark.h"

class SCRIBUS_API Mark2Mark : public MarkInsert, private Ui::Mark2MarkDlg
{
    Q_OBJECT

public:
	explicit Mark2Mark(const QList<Mark*>& marks, Mark* omitMark = nullptr, QWidget *parent = nullptr);
	virtual void values(QString& label, Mark* &mrk);
	virtual void setValues(const QString label, const Mark* mrk);

protected:
    void changeEvent(QEvent *e) override;
};

#endif // MARK2MARK_H
