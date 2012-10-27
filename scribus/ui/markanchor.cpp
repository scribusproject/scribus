#include "markanchor.h"

MarkAnchorDlg::MarkAnchorDlg(QWidget *parent) :
	MarkInsertDlg(parent)
{
	setupUi(this);
	setWindowTitle(tr("Anchor Mark"));
}

void MarkAnchorDlg:: values(QString& label)
{
	label = labelEdit->text();
}

void MarkAnchorDlg::setValues(const QString label)
{
	labelEdit->setText(label);
}

void MarkAnchorDlg::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
	}
}
