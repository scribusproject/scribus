#ifndef MARKVARIABLETEXT_H
#define MARKVARIABLETEXT_H

#include "scribusapi.h"
#include "markinsert.h"
#include "ui_markvariabletext.h"

class SCRIBUS_API MarkVariableText : public MarkInsert, private Ui::MarkVariableTextDlg
{
	Q_OBJECT

public:
	explicit MarkVariableText(const QList<Mark*>&, QWidget* parent = nullptr);
	explicit MarkVariableText(const Mark*, QWidget* parent = nullptr);
	~MarkVariableText();

	Mark* values(QString& label, QString& text) override;
	void setValues(const QString label, const QString text) override;

protected:
	void changeEvent(QEvent *e) override;

private slots:
	void onLabelList_currentIndexChanged(int index);

private:

	enum WidgetType
	{
		ComboBox = 0,
		LineEdit = 1
	};

	QWidget* labelEditWidget = nullptr;
	const WidgetType m_widgetType;

	const Mark* m_mark = nullptr;
};

#endif // MARKVARIABLETEXT_H
