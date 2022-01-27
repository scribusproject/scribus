#include "markanchor.h"

MarkAnchor::MarkAnchor(QWidget *parent) :
	MarkInsert(parent)
{
	setupUi(this);
	setWindowTitle(tr("Anchor Mark"));
}

void MarkAnchor:: values(QString& label)
{
	label = labelEdit->text();
}

void MarkAnchor::setValues(const QString label)
{
	labelEdit->setText(label);
}

void MarkAnchor::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type())
	{
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
	}
}
