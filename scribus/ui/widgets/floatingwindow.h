#ifndef STICKYPOPUP_H
#define STICKYPOPUP_H

#include <QToolButton>
#include <QWidget>
#include <QVBoxLayout>

class FloatingWindow : public QWidget
{
	Q_OBJECT

public:
	explicit FloatingWindow(QWidget * child, QWidget *reference, QWidget *parent = nullptr);

	QWidget *child();
	QWidget *reference();

public slots:
	void iconSetChange();

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void hideEvent(QHideEvent *event) override;

private:
	QWidget * m_child { nullptr };
	QWidget * m_reference { nullptr };
	QWidget * m_handle { nullptr };
	QVBoxLayout * m_layout { nullptr };
	QToolButton* buttonClose { nullptr };

	QPoint m_mousePos;

	/*!
	 * \brief Screen size of all screens
	 * \return
	 */
	QSize screenSize() const;

	/*!
	 * \brief Calculate screen position relative to reference() widget.
	 */
	void calculatePosition();

signals:
	void closed();

public slots:
	void show(QWidget *reference = nullptr);
	void updateSize();
};

#endif // STICKYPOPUP_H
