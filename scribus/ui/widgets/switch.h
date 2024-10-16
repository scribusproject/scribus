#ifndef SWITCH_H
#define SWITCH_H

#include <QAbstractButton>
#include <QPropertyAnimation>

class Switch : public QAbstractButton
{
	Q_OBJECT
	Q_PROPERTY(int position READ position WRITE setPosition)

public:
	enum Size
	{
		Normal = 0,
		Small = 1
	};

	Switch(QWidget *parent = nullptr);

	int position() const {return m_pos;}
	void setPosition(int x)
	{
		m_pos = x;
		update();
	}

	Size sizeMode() const;
	void setSizeMode(Size mode);

	void setText(QString const &text);

	QSize sizeHint() const override;

protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
	int m_radius { 10 };
	int m_margin { 2 };
	int m_space { 8 };
	int m_pos {m_radius + m_margin};
	bool m_hasShortcut {false};
	int m_shortcutId {0};
	QPropertyAnimation *m_anim { nullptr };
	Size m_size { Size::Normal };

	QSize textSize() const;
	QSize switchSize() const;

	void updateShortcut();

private slots:
	void animate(bool toggled);
};

#endif // SWITCH_H
