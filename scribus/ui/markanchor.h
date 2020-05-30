#ifndef MARKANCHOR_H
#define MARKANCHOR_H

#include "markinsert.h"
#include "ui_markanchor.h"

class SCRIBUS_API MarkAnchor : public MarkInsert, private Ui::MarkAnchorDlg
{
    Q_OBJECT

public:
	explicit MarkAnchor(QWidget *parent = nullptr);
	~MarkAnchor() {}

	void values(QString& label) override;
	void setValues(const QString label) override;

protected:
	void changeEvent(QEvent *e) override;
};

#endif // MARKANCHOR_H
