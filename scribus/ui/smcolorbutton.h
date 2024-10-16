#ifndef SMCOLORBUTTON_H
#define SMCOLORBUTTON_H

#include "ui/widgets/color_button.h"
#include "ui/widgets/form_widget.h"

class SMColorButton : public FormWidget
{
	Q_OBJECT

public:
	SMColorButton(QWidget *parent = nullptr);

	void setColor(const QString name, const double shade = 100.0);
	void setColor(const QString name, const double shade, const QString pName, const double pShade, bool isParentValue);

	bool useParentValue();

	ColorButton *colorButton {nullptr};
	QToolButton *parentButton {nullptr};

private:
	bool m_hasParent {false};
	bool m_useParentValue {false};
	double m_pShade {100.0};
	QString m_pColor;

private slots:
	void styleChanged();
	void iconSetChange();
	void pbPressed();
};

#endif // SMCOLORBUTTON_H
