#ifndef MARKVARIABLETEXT_H
#define MARKVARIABLETEXT_H

#include "scribusapi.h"
#include "markinsert.h"
#include "ui_markvariabletext.h"

class SCRIBUS_API MarkVariableTextDlg : public MarkInsertDlg, private Ui::MarkVariableTextDlg
{
	Q_OBJECT
public:
	explicit MarkVariableTextDlg(const QList<Mark*>&, QWidget *parent = 0);
	explicit MarkVariableTextDlg(const Mark*, QWidget *parent = 0);
	~MarkVariableTextDlg();
	virtual Mark* values(QString& label, QString& text);
	virtual void setValues(const QString label, const QString text);
protected:
	void changeEvent(QEvent *e);
private slots:
	void onLabelList_currentIndexChanged(int index);
private:
	QWidget* labelEditWidget;
	const Mark* mrk;
};

#endif // MARKVARIABLETEXT_H
