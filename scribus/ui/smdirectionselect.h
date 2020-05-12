#ifndef SMDIRECTIONSELECT_H
#define SMDIRECTIONSELECT_H

#include "directionselect.h"

class SMDirectionSelect : public DirectionSelect
{
	Q_OBJECT
public:
	SMDirectionSelect(QWidget *parent);
	~SMDirectionSelect() {}

	void setStyle(int i);
	void setStyle(int i, bool isParentValue);

	void setParentItem(int i);
	bool useParentValue();

	QToolButton *parentButton;

private:
	bool   m_hasParent = false;
	bool   m_useParentStyle = false;
	int    m_pStyle = 0;

	void setFont(bool wantBold);

private slots:
	void styleChanged();
	void pbPressed();
};

#endif // SMDIRECTIONSELECT_H
