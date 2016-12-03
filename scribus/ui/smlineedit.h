/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMLINEEDIT_H
#define SMLINEEDIT_H

#include <QLineEdit>

class SMLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	SMLineEdit(QWidget *parent);
	~SMLineEdit() {}

	void setValue(QString val);
	void setValue(QString val, bool isParentVal);

	void clear();

	void setParentValue(QString val);

	bool useParentValue();
private:
	void setFont(bool wantBold);
	bool m_hasParent;
	bool m_useParentValue;
	QString m_pValue;

private slots:
	void slotValueChanged();
};

#endif // SMLINEEDIT_H
