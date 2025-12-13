#ifndef SCRIBUSPROXYSTYLE_H
#define SCRIBUSPROXYSTYLE_H

#include <QProxyStyle>

class ScribusProxyStyle : public QProxyStyle
{
	Q_OBJECT

public:

	enum class ApplicationTheme {
		System,
		Light,
		Dark
	};

	ScribusProxyStyle(QStyle *style = nullptr);
	ScribusProxyStyle(const QString &key);

	static ScribusProxyStyle* instance();

	QRect subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *widget = nullptr) const override;
	void drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QPainter *painter, const QWidget *widget) const override;

protected:
	bool eventFilter(QObject *object, QEvent *event) override;

public slots:
	void setBaseStyleName(const QString &styleName);
	void setApplicationTheme(ApplicationTheme theme);

private:
	bool blockRefresh {false};
};

#endif // SCRIBUSPROXYSTYLE_H
